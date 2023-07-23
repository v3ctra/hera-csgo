#include "core/csgo.hpp"

#ifdef _DEBUG
int main() {
	g_ctx->initialize();
	return 0;
}

#else
extern "C" BOOL WINAPI _CRT_INIT(HMODULE hModule, DWORD dwReason, LPVOID lpReserved);

int __stdcall entry_point(HMODULE bin, unsigned long reason, void* reserved) {
    if (!_CRT_INIT(bin, reason, reserved))
        return 0;

    if (reason != DLL_PROCESS_ATTACH)
        return 0;

    g_ctx->initialize();

    return 1;
}
#endif