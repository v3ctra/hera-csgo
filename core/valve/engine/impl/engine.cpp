#include "../../../csgo.hpp"

namespace valve {
	address_t c_engine::get_local_player() const {
		return g_memory->read<DWORD>(g_memory->client_base() + offsets::dwLocalPlayer);
	}
	address_t c_engine::get_entity_from_index(int i) const {
		return g_memory->read<DWORD>(g_memory->client_base() + offsets::dwEntityList + (i * 0x10));
	}

	DWORD c_engine::get_client_state() const {
		return g_memory->read<DWORD>(g_memory->engine_base() + offsets::dwClientState);
	}

	float c_engine::get_sensitivity() const {
		DWORD sensitivity = g_memory->read<DWORD>(g_memory->client_base() + offsets::dwSensitivity + 0x2c) ^ (DWORD)(g_memory->client_base() + offsets::dwSensitivity);
		return *(float*)&sensitivity;
	}

	math::vec3_t c_engine::get_view_angles() const {
		return g_memory->read<math::vec3_t>(get_client_state() + offsets::dwClientState_ViewAngles);
	}

	int c_engine::get_max_players() const {
		return g_memory->read<int>(get_client_state() + offsets::dwClientState_MaxPlayer);
	}
}