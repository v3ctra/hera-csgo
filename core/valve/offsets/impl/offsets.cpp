#include "../../../csgo.hpp"

namespace offsets {
	void c_offsets::update() {
		dwLocalPlayer = g_memory->find_pattern(g_memory->client_base(), xorstr("8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF"), 0x3, 0x4);
		if (!dwLocalPlayer)
			std::abort();

		dwEntityList = g_memory->find_pattern(g_memory->client_base(), xorstr("BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8"), 0x1, 0x0);
		if (!dwEntityList)
			std::abort();

		dwClientState = g_memory->find_pattern(g_memory->engine_base(), xorstr("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0"), 0x1, 0x0);
		if (!dwClientState)
			std::abort();

		dwSensitivity = g_memory->find_pattern(g_memory->client_base(), xorstr("81 F9 ? ? ? ? 75 7E F3 0F 10 05"), 0x2, 0x0);
		if (!dwSensitivity)
			std::abort();

		LOG("offsets found!");
	}
}