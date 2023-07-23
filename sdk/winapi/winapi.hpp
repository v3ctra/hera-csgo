#pragma once
#include "../pe/detail/structs.hpp"

namespace sdk {
	class c_winapi {
	public:
		NTSTATUS duplicate_object(HANDLE srcProcHandle, HANDLE srcHandle, HANDLE targetProcHandle, PHANDLE targetHandle, ACCESS_MASK access, BOOL attributes, ULONG options);
		NTSTATUS query_process_info(HANDLE proc, PROCESSINFOCLAS info_class, PVOID proc_info, ULONG info_length, PULONG ret_length);

		NTSTATUS create_user_thread(HANDLE ProcessHandle,
			PSECURITY_DESCRIPTOR SecurityDescriptor,
			BOOLEAN CreateSuspended,
			ULONG StackZeroBits,
			SIZE_T StackReserve,
			SIZE_T StackCommit,
			PTHREAD_START_ROUTINE StartAddress,
			PVOID Parameter,
			PHANDLE ThreadHandle,
			PCLIENT_IDD ClientId);

		NTSTATUS query_system_info(SYSTEM_INFORMATION_CLASS info_class, PVOID system_info, ULONG info_length, PULONG ret_length);
		HANDLE open_process(ACCESS_MASK access, DWORD pid);
		NTSTATUS close_handle(HANDLE handle);
		DWORD get_proc_info(HANDLE process);

		void mouse_event(DWORD dwFlags, DWORD dx, DWORD dy, DWORD dwData, ULONG_PTR dwExtraInfo);
		SHORT get_key_state(int vKey);

		BOOL virtual_protect(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
		BOOL set_thread_context(HANDLE hThread, const CONTEXT* lpContext);
		BOOL get_thread_context(HANDLE hThread, LPCONTEXT lpContext);
		BOOL play_sound(LPCTSTR pszSound, HMODULE hmod, DWORD fdwSound);

		DWORD resume_thread(HANDLE hThread);

		bool read_memory(HANDLE proc, LPCVOID base, LPVOID buffer, SIZE_T size, SIZE_T* bytes_read);
		DWORD get_module_name(HANDLE proc, HMODULE hModule, LPWSTR base_name, DWORD size);
		BOOL find_module(HANDLE proc, HMODULE hModule, PLDR_DATA_TABLE_ENTRY Module);

		BOOL enumerate_modules(HANDLE proc, HMODULE* hModule, DWORD cb, LPDWORD needed);

		bool get_module_information(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);
	};
}
inline const auto g_winapi = std::make_unique< sdk::c_winapi >();