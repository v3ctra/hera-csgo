#include "../../../core/csgo.hpp"

namespace sdk {
	NTSTATUS c_winapi::query_system_info(SYSTEM_INFORMATION_CLASS info_class, PVOID system_info, ULONG info_length, PULONG ret_length) {
		static auto NtQuerySystemInformation = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtQuerySystemInformation")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<NTSTATUS>(
			NtQuerySystemInformation, g_ctx->offsets().m_gadget, 
			info_class, system_info, info_length, ret_length);

		//return NtQuerySystemInformation(info_class, system_info, info_length, ret_length);
	}

	NTSTATUS c_winapi::query_process_info(HANDLE proc, PROCESSINFOCLAS info_class, PVOID proc_info, ULONG info_length, PULONG ret_length) {
		static auto NtQueryInformationProcess = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtQueryInformationProcess")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<NTSTATUS>(
			NtQueryInformationProcess, g_ctx->offsets().m_gadget,
			proc, info_class, proc_info, info_length, ret_length);

		//return NtQueryInformationProcess(proc, info_class, proc_info, info_length, ret_length);
	}

	DWORD c_winapi::get_proc_info(HANDLE process) {
		PROCESS_BASIC_INFORMATION pbi;

		if (NT_SUCCESS(query_process_info(process, ProcessBasicInformation, &pbi, sizeof(pbi), 0)))
			return pbi.UniqueProcessId;

		return 0;
	}
	//RtlCreateUserThread

	BOOL c_winapi::virtual_protect(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
		static auto NtProtectVirtualMemory = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtProtectVirtualMemory")).as<uintptr_t>();

		/* Change the protection */
		NTSTATUS status = x86RetSpoof::invokeStdcall<NTSTATUS>(
			NtProtectVirtualMemory,
			g_ctx->offsets().m_gadget, hProcess,
			&lpAddress,
			&dwSize,
			flNewProtect,
			(PULONG)lpflOldProtect);

		if (!NT_SUCCESS(status)) {
			/* We failed */
			return FALSE;
		}

		/* Return success */
		return TRUE;
	}


	HANDLE c_winapi::open_process(ACCESS_MASK access, DWORD pid) {
		static auto NtOpenProcess = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtOpenProcess")).as<uintptr_t>();

		HANDLE hProc = INVALID_HANDLE_VALUE;
		CLIENT_IDD cid = { (HANDLE)pid, NULL };

		if (pid) {
			OBJECT_ATTRIBUTES attribs;
			InitializeObjectAttributes(&attribs, NULL, NULL, NULL, NULL);

			NTSTATUS status = x86RetSpoof::invokeStdcall<NTSTATUS>(NtOpenProcess, g_ctx->offsets().m_gadget,
				&hProc, access, &attribs, &cid);
			//NTSTATUS status = NtOpenProcess(&hProc, access, &attribs, &cid);
			if (NT_SUCCESS(status))
				return hProc;
		}

		return hProc;
	}

	NTSTATUS c_winapi::create_user_thread(HANDLE ProcessHandle,
		PSECURITY_DESCRIPTOR SecurityDescriptor,
		BOOLEAN CreateSuspended,
		ULONG StackZeroBits,
		SIZE_T StackReserve,
		SIZE_T StackCommit,
		PTHREAD_START_ROUTINE StartAddress,
		PVOID Parameter,
		PHANDLE ThreadHandle,
		PCLIENT_IDD ClientId) {
		
		static auto RtlCreateUserThread = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("RtlCreateUserThread")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<NTSTATUS>(RtlCreateUserThread, g_ctx->offsets().m_gadget, ProcessHandle, SecurityDescriptor, CreateSuspended, StackZeroBits,
			StackReserve, StackCommit, StartAddress, Parameter, ThreadHandle, ClientId);
	}

	BOOL c_winapi::get_thread_context(HANDLE hThread, LPCONTEXT lpContext) {
		static auto NtGetContextThread = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtGetContextThread")).as<uintptr_t>();

		NTSTATUS Status = x86RetSpoof::invokeStdcall<NTSTATUS>(NtGetContextThread, g_ctx->offsets().m_gadget, hThread, lpContext);
		if (!NT_SUCCESS(Status)) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL c_winapi::play_sound(LPCTSTR pszSound, HMODULE hmod, DWORD fdwSound) {
		static auto play_sound = g_pe->get_export(g_ctx->modules().m_Winmm,
			hash("PlaySound")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<BOOL>(play_sound, g_ctx->offsets().m_gadget, pszSound, hmod, fdwSound);
	}

	BOOL c_winapi::set_thread_context(HANDLE hThread, const CONTEXT* lpContext) {
		static auto NtSetContextThread = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtSetContextThread")).as<uintptr_t>();

		NTSTATUS Status = x86RetSpoof::invokeStdcall<NTSTATUS>(NtSetContextThread, g_ctx->offsets().m_gadget, hThread, lpContext);
		if (!NT_SUCCESS(Status)) {
			return FALSE;
		}

		return TRUE;
	}

	DWORD c_winapi::resume_thread(HANDLE hThread) {
		static auto NtResumeThread = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtResumeThread")).as<uintptr_t>();

		ULONG PreviousResumeCount;

		NTSTATUS Status = x86RetSpoof::invokeStdcall<NTSTATUS>(NtResumeThread, g_ctx->offsets().m_gadget, hThread, &PreviousResumeCount);
		if (!NT_SUCCESS(Status)) {
			return -1;
		}

		return PreviousResumeCount;
	}

	NTSTATUS c_winapi::close_handle(HANDLE handle) {

		static auto NtClose = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtClose")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<NTSTATUS>(
			NtClose, g_ctx->offsets().m_gadget,
			handle);

		//return NtClose(handle);
	}

	NTSTATUS c_winapi::duplicate_object(HANDLE srcProcHandle, HANDLE srcHandle, HANDLE targetProcHandle, PHANDLE targetHandle, ACCESS_MASK access, BOOL attributes, ULONG options) {
		static auto NtDuplicateObject = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtDuplicateObject")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<NTSTATUS>(
			NtDuplicateObject, g_ctx->offsets().m_gadget,
			srcProcHandle, srcHandle, targetProcHandle, targetHandle, access, attributes, options);

		//return NtDuplicateObject(srcProcHandle, srcHandle, targetProcHandle, targetHandle, access, attributes, options);
	}

	void c_winapi::mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo) {
		static auto NtUserSendInput = g_pe->get_export(g_ctx->modules().m_win32u,
			hash("NtUserSendInput")).as<uintptr_t>();

		INPUT Input;

		Input.type = INPUT_MOUSE;
		Input.mi.dx = dx;
		Input.mi.dy = dy;
		Input.mi.mouseData = dwData;
		Input.mi.dwFlags = dwFlags;
		Input.mi.time = 0;
		Input.mi.dwExtraInfo = dwExtraInfo;
		
		x86RetSpoof::invokeStdcall<UINT>(NtUserSendInput, g_ctx->offsets().m_gadget, 1, &Input, sizeof(INPUT));
	}

	SHORT c_winapi::get_key_state(int vKey) {
		static auto GetAsyncKeyState = g_pe->get_export(g_ctx->modules().m_user32,
			hash("GetAsyncKeyState")).as<uintptr_t>();

		return x86RetSpoof::invokeStdcall<SHORT>(
			GetAsyncKeyState, g_ctx->offsets().m_gadget,
			vKey);
	}

	bool c_winapi::read_memory(HANDLE proc, LPCVOID base, LPVOID buffer, SIZE_T size, SIZE_T* bytes_read) {
		static auto NtReadVirtualMemory = g_pe->get_export(g_ctx->modules().m_ntdll,
			hash("NtReadVirtualMemory")).as<uintptr_t>();

		unsigned int numberOfBytesRead[3];

		NTSTATUS status = x86RetSpoof::invokeStdcall<NTSTATUS>(NtReadVirtualMemory, g_ctx->offsets().m_gadget, proc, (PVOID)base, buffer, size, &proc);
		if (bytes_read)
			*bytes_read = size;

		if (NT_SUCCESS(status))
			return true;

		return false;
	}

	//reactOS
	BOOL c_winapi::find_module(HANDLE proc, HMODULE hModule, PLDR_DATA_TABLE_ENTRY Module) {
		DWORD Count;
		NTSTATUS Status;
		PPEB_LDR_DATA LoaderData;
		PLIST_ENTRY ListHead, ListEntry;
		PROCESS_BASIC_INFORMATION ProcInfo;

		/* Query the process information to get its PEB address */
		Status = query_process_info(proc, ProcessBasicInformation, &ProcInfo, sizeof(ProcInfo), NULL);
		if (!NT_SUCCESS(Status)) {
			return false;
		}

		/* If no module was provided, get base as module */
		if (hModule == nullptr) {
			if (!read_memory(proc, &ProcInfo.PebBaseAddress->ImageBaseAddress, &hModule, sizeof(hModule), NULL)) {
				return false;
			}
		}

		/* Read loader data address from PEB */
		if (!read_memory(proc, &ProcInfo.PebBaseAddress->LdrData, &LoaderData, sizeof(LoaderData), NULL)) {
			return false;
		}

		if (LoaderData == NULL) {
			return FALSE;
		}

		/* Store list head address */
		ListHead = &(LoaderData->InMemoryOrderModuleList);

		/* Read first element in the modules list */
		if (!read_memory(proc,
			&(LoaderData->InMemoryOrderModuleList.Flink),
			&ListEntry,
			sizeof(ListEntry),
			NULL))
		{
			return false;
		}

		Count = 0;

		/* Loop on the modules */
		while (ListEntry != ListHead) {
			/* Load module data */
			if (!read_memory(proc,
				CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks),
				Module,
				sizeof(*Module),
				NULL))
			{
				return false;
			}

			/* Does that match the module we're looking for? */
			if (Module->DllBase == hModule) {
				return true;
			}

			++Count;
			if (Count > 0x2710) {
				break;
			}

			/* Get to next listed module */
			ListEntry = Module->InMemoryOrderLinks.Flink;
		}

		return false;
	}
	bool c_winapi::get_module_information(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb) {
		LDR_DATA_TABLE_ENTRY module{};

		/* Check output size */
		if (cb < sizeof(MODULEINFO)) {
			return false;
		}

		/* Get the matching module */
		if (!find_module(hProcess, hModule, &module)) {
			return false;
		}

		/* Get a local copy first, to check for valid pointer once */
		MODULEINFO local_info{};
		local_info.lpBaseOfDll = hModule;
		local_info.SizeOfImage = module.SizeOfImage;
		local_info.EntryPoint = module.EntryPoint;

		if (memcpy_s(lpmodinfo, sizeof(MODULEINFO), &local_info, sizeof(MODULEINFO)) != 0) {
			return false;
		}

		return true;
	}

	DWORD c_winapi::get_module_name(HANDLE proc, HMODULE hModule, LPWSTR base_name, DWORD size) {
		DWORD Len;
		LDR_DATA_TABLE_ENTRY Module;

		if (!find_module(proc, hModule, &Module)) {
			return 0;
		}

		Len = Module.BaseDllName.Length + sizeof(UNICODE_NULL);
		if (size * sizeof(WCHAR) < Len) {
			Len = size * sizeof(WCHAR);
		}

		if (!read_memory(proc, (&Module.BaseDllName)->Buffer, base_name, Len, NULL)) {
			return 0;
		}

		if (Len == Module.BaseDllName.Length + sizeof(UNICODE_NULL)) {
			Len -= sizeof(UNICODE_NULL);
		}

		if (Len >= size * sizeof(WCHAR))
		{
			if (size) {
				base_name[size - 1] = UNICODE_NULL;
			}
		}
		else {
			//assert(Len + sizeof(UNICODE_NULL) <= nSize * sizeof(WCHAR));
			base_name[Len / sizeof(WCHAR)] = UNICODE_NULL;
		}

		return Len / sizeof(WCHAR);
	}
	BOOL c_winapi::enumerate_modules(HANDLE proc, HMODULE* hModule, DWORD cb, LPDWORD needed) {
		PPEB_LDR_DATA LoaderData;
		PLIST_ENTRY ListHead, ListEntry;
		PROCESS_BASIC_INFORMATION ProcInfo;
		LDR_DATA_TABLE_ENTRY CurrentModule;

		NTSTATUS status = query_process_info(proc, ProcessBasicInformation, &ProcInfo, sizeof(ProcInfo), nullptr);
		if (!NT_SUCCESS(status)) {
			return false;
		}

		if (ProcInfo.PebBaseAddress == nullptr) {
			return false;
		}

		/* Read loader data address from PEB */
		if (!read_memory(proc, &ProcInfo.PebBaseAddress->LdrData, &LoaderData, sizeof(LoaderData), NULL)) {
			return false;
		}

		/* Store list head address */
		ListHead = &LoaderData->InLoadOrderModuleList;

		/* Read first element in the modules list */
		if (!read_memory(proc, &LoaderData->InLoadOrderModuleList.Flink, &ListEntry, sizeof(ListEntry), NULL)) {
			return false;
		}

		DWORD NbOfModules = cb / sizeof(HMODULE), count = 0;

		while (ListEntry != ListHead) {
			/* Load module data */
			if (!read_memory(proc,
				CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks),
				&CurrentModule,
				sizeof(CurrentModule),
				NULL)) {
				return FALSE;
			}

			/* Check if we can output module, do it if so */
			if (count < NbOfModules) {
				__try
				{
					hModule[count] = (HMODULE)CurrentModule.DllBase;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					return false;
				}
			}

			++count;
			if (count > 0x2710) {
				return false;
			}

			/* Get to next listed module */
			ListEntry = CurrentModule.InLoadOrderLinks.Flink;
		}
		__try
		{
			*needed = count * sizeof(HMODULE);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}

		return true;
	}}