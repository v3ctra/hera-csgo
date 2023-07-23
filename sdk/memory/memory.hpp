#pragma once
// i won't write a driver for this. lol.

#define in_range( x, a, b ) ( x >= a && x <= b )
#define get_bits( x ) ( in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 ) )
#define get_byte( x ) ( get_bits( x[ 0 ] ) << 4 | get_bits( x[ 1 ] ) )

namespace sdk {
    constexpr auto PAGE_SIZE = 65536;

    class c_memory {
    public:
        // Initialize the memory object by specifying the process name
        void initialize(std::string_view process_name);
        void uninitialize() {
            if (!m_initialized)
                return;

            if (m_handle != INVALID_HANDLE_VALUE)
                g_winapi->close_handle(m_handle);
        }

        // Read a value of type T from the specified address in the process's memory
        template <typename T>
        T read(std::size_t address) {
            T buffer{};
            if (!g_winapi->read_memory(m_handle, reinterpret_cast<void*>(address), &buffer, sizeof(T), NULL)) {
                return T{};
            }
            return buffer;
        }

        // Create Thread with spoofed startaddress
        
        // who tf added this?
        // https://www.unknowncheats.me/forum/3077466-post442417.html
        HANDLE create_spoofed_thread(PTHREAD_START_ROUTINE location, pe::module_t module);

        // Find pattern in a process
        uintptr_t find_pattern(uintptr_t module_base, const std::string& pattern, int offset, int extra) const;

        // Get the base address of the client module
        ALWAYS_INLINE const auto& client_base() const { return m_client_base; }

        // Get the base address of the engine module
        ALWAYS_INLINE const auto& engine_base() const { return m_engine_base; }

        // Check if memory-management system has been initialized
        ALWAYS_INLINE const auto& is_initialized() const { return m_initialized; }

    private:
        // Get the process ID based on the provided process name
        int get_process_id(uint32_t proc_name) const;

        // Get the handle of the process based on the process ID
        HANDLE get_process_handle(int pid) const;

        // Get the base address of the specified module in the process
        int get_module_base(HANDLE process, std::wstring_view name) const;

    private:
        HANDLE m_handle{};        // Handle to the process
        int m_client_base{};      // Base address of the client module
        int m_engine_base{};      // Base address of the engine module
        int m_pid{};              // Process ID

        bool m_initialized{};
    };
}

// Global instance of c_memory
inline const auto g_memory = std::make_unique<sdk::c_memory>();