#pragma once

namespace hacks {
	class c_sound {
	public:
		void run() const;
	};
}
// Global instance of c_aimbot
inline const auto g_sound_esp = std::make_unique< hacks::c_sound >();