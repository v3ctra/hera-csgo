#pragma once

namespace offsets {
	inline uintptr_t dwLocalPlayer{};
	inline uintptr_t dwEntityList{};

	inline uintptr_t dwSensitivity{};

	inline uintptr_t dwClientState{};
	inline uintptr_t dwClientState_ViewAngles = 0x4D90;
	inline uintptr_t dwClientState_MaxPlayer = 0x388;

	class c_offsets {
	public:
		void update();
	};
}
inline const auto g_offsets = std::make_unique< offsets::c_offsets >();