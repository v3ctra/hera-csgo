#include "../../sdk.hpp"

namespace sdk {

    void c_memory::initialize(std::string_view process_name) {
        // Get the process ID based on the hashed process name
        m_pid = get_process_id(hash(process_name.data()));
        if (!m_pid) {
            LOG("Failed to get PID");
            std::abort();  // Abort if the process ID is not found
        }

        // Get the process handle using the process ID
        m_handle = get_process_handle(m_pid);
        if (m_handle == INVALID_HANDLE_VALUE) {
            LOG("Failed to get process handle...");
            std::abort();  // Abort if the process handle is invalid
        }

        // Loop until both client and engine module base addresses are found
        while (m_client_base == 0 || m_engine_base == 0) {
            // Get the base address of the client module
            m_client_base = get_module_base(m_handle, L"client.dll");

            // Get the base address of the engine module
            m_engine_base = get_module_base(m_handle, L"engine.dll");

            LOG("Getting module addresses...");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // Let's announce that it's ready.
        m_initialized = true;
    }

    uintptr_t c_memory::find_pattern(uintptr_t module_base, const std::string& pattern, int offset, int extra) const {
        if (!m_initialized) {
            if (m_handle != INVALID_HANDLE_VALUE)
                g_winapi->close_handle(m_handle);
            std::abort();
        }

        MODULEINFO module_info{};
        if (!g_winapi->get_module_information(m_handle, reinterpret_cast<HMODULE>(module_base), &module_info, sizeof(MODULEINFO)))
            return 0;
        
        uintptr_t module_base_addr = reinterpret_cast<uintptr_t>(module_info.lpBaseOfDll);
        uintptr_t module_size = module_info.SizeOfImage;

        std::vector<std::byte> bytes(module_size);

        DWORD bytes_read{};
        g_winapi->read_memory(m_handle, module_info.lpBaseOfDll, bytes.data(), module_size, &bytes_read); //reading the module and storing as bytes_read
        if (bytes_read != module_info.SizeOfImage) {
            return 0;
        }

        uintptr_t scan{};
        const char* pat = pattern.c_str();

        uintptr_t first_match{};
        for (uintptr_t pCur = reinterpret_cast<uintptr_t>(bytes.data()); pCur < reinterpret_cast<uintptr_t>(bytes.data()) + module_size; ++pCur) {
            if (!*pat) {
                scan = first_match;
                break;
            }

            if (*(uint8_t*)pat == '\?' || *(uint8_t*)pCur == get_byte(pat)) {
                if (!first_match)
                    first_match = pCur;

                if (!pat[2]) {
                    scan = first_match;
                    break;
                }

                if (*(WORD*)pat == 16191 /*??*/ || *(uint8_t*)pat != '\?')
                    pat += 3;
                else
                    pat += 2;
            }
            else {
                pat = pattern.c_str();
                first_match = 0;
            }
        }
        if (!scan) {
            return 0x0;
        }

        uint32_t read{};
        g_winapi->read_memory(m_handle, reinterpret_cast<void*>(scan - reinterpret_cast<uintptr_t>(bytes.data()) + module_base_addr + offset), &read, sizeof(read), nullptr);

        return (read + extra) - module_base_addr;
    }

    int c_memory::get_process_id(uint32_t proc_name) const {
        std::vector<std::byte> buffer(PAGE_SIZE);  // Buffer to store system process information

        NTSTATUS status{};
        do {
            buffer.resize(buffer.size() * 2);  // Resize the buffer to accommodate more data
            status = g_winapi->query_system_info(SystemProcessInformation, buffer.data(), buffer.size(), nullptr);
            LOG("PROCESS ID STATUS_INFO_LENGTH_MISMATCH...");
        } while (status == STATUS_INFO_LENGTH_MISMATCH);

        if (!NT_SUCCESS(status)) {
            LOG("querying process information failed...");
            return 0;  // Return 0 if querying process information failed
        }

        // Iterate through the system process information
        for (auto p = reinterpret_cast<PSYSTEM_PROCESS_INFO>(buffer.data()); p;
            p = p->NextEntryOffset ? reinterpret_cast<PSYSTEM_PROCESS_INFO>((std::byte*)p + p->NextEntryOffset) : nullptr) {

            // Convert the process name to lowercase for case-insensitive comparison
            auto wide_name = std::wstring(p->ImageName.Buffer, p->ImageName.Length >> 1);
            auto name = std::string(wide_name.begin(), wide_name.end());
            std::ranges::transform(name.begin(), name.end(), name.begin(), ::tolower);

           // LOG(name.c_str());

            // Compare the hashed process name with the current process name
            if (proc_name == hash(name.c_str())) {
                return reinterpret_cast<int>(p->ProcessId);  // Return the process ID if a match is found
            }
        }

        return 0;  // Return 0 if the process ID is not found
    }

    HANDLE c_memory::get_process_handle(int pid) const {
        NTSTATUS status{};
        std::vector<std::byte> buffer{};

        do {
            buffer.resize(buffer.size() + 0x1000);  // Increase the buffer size
            // Query the system for handle information
            status = g_winapi->query_system_info(SystemHandleInformation, buffer.data(), buffer.size(), nullptr);
        } while (status == STATUS_INFO_LENGTH_MISMATCH);

        if (!NT_SUCCESS(status)) {
            return INVALID_HANDLE_VALUE;  // Return invalid handle if querying handle information failed
        }

        auto phi = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(buffer.data());

        // Iterate through the handle information
        for (size_t i = 0; i < phi->HandleCount; i++) {
            auto handle = phi->Handles[i];
            HANDLE dup_handle = nullptr;

            // Skip non-process handles
            if (handle.ObjectTypeNumber != 0x5 && handle.ObjectTypeNumber != 0x7)
                continue;

            // Do not open handle to csgo...
            if (handle.ProcessId == pid)
                continue;

            // Open the process to duplicate the handle
            auto proc_handle = g_winapi->open_process(PROCESS_DUP_HANDLE, handle.ProcessId);
            if (!proc_handle || proc_handle == INVALID_HANDLE_VALUE)
                continue;

            // Duplicate the handle in the current process
            NTSTATUS status = g_winapi->duplicate_object(proc_handle, (HANDLE)handle.Handle, NtCurrentProcess(), &dup_handle, PROCESS_ALL_ACCESS, 0, 0);
            if (!NT_SUCCESS(status)) {
                g_winapi->close_handle(proc_handle);
                continue; // Skip to the next handle if duplication failed
            }

            // Check if the duplicated handle belongs to the desired process
            if (g_winapi->get_proc_info(dup_handle) != pid) {
                g_winapi->close_handle(dup_handle);
                g_winapi->close_handle(proc_handle);
                continue; // Skip to the next handle if the process ID doesn't match
            }

            g_winapi->close_handle(proc_handle);
            return dup_handle;  // Return the duplicated handle if a match is found
        }

        return INVALID_HANDLE_VALUE;  // Return invalid handle if the process handle is not found
    }

    HANDLE c_memory::create_spoofed_thread(PTHREAD_START_ROUTINE location, pe::module_t module) {
        // *anticheat and 21 others liked that* :>
        auto spoof_address = module.get_nt_header()->OptionalHeader.AddressOfEntryPoint;

        int default_thread_size = 1000;

        HANDLE handle = INVALID_HANDLE_VALUE;

        g_winapi->virtual_protect(NtCurrentProcess(), reinterpret_cast<void*>(spoof_address), default_thread_size, PAGE_EXECUTE_READWRITE, nullptr);

        g_winapi->create_user_thread(NtCurrentProcess(),
            nullptr, TRUE, NULL, NULL, NULL, reinterpret_cast<PTHREAD_START_ROUTINE>(spoof_address), reinterpret_cast<void*>(module.get_base()), &handle, nullptr);

        CONTEXT ctx;
        ctx.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;

        g_winapi->get_thread_context(handle, &ctx);

        ctx.Eax = (ULONG32)location;
        ctx.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;

        g_winapi->set_thread_context(handle, &ctx);
        
        g_winapi->resume_thread(handle);
        return handle;
    }

    int c_memory::get_module_base(HANDLE process, std::wstring_view name) const {
        HMODULE modules[1024];
        DWORD needed_size;

        if (g_winapi->enumerate_modules(process, modules, sizeof(modules), &needed_size)) {
            const auto module_count = needed_size / sizeof(HMODULE);
            for (std::size_t i = 0; i < module_count; ++i) {
                wchar_t module_name[50];
                if (g_winapi->get_module_name(process, modules[i], module_name, sizeof(module_name)))
                {
                    if (hash(module_name) == hash(name.data())) {
                        return reinterpret_cast<int>(modules[i]);
                    }
                }
            }
        }

        return 0;
    }
}