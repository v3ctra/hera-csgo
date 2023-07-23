#pragma once

namespace hacks {
	class c_aimbot {
	public:
		void run() const;

		// Get closest player
		valve::cs_player_t* get_closest_player(math::vec3_t viewangles, math::vec3_t eye_pos, float fov) const;
	private:
		// Math thing...
		math::vec3_t get_target_angle(const math::vec3_t eye_pos, const math::vec3_t position) const;
	};
}
// Global instance of c_aimbot
inline const auto g_aimbot = std::make_unique< hacks::c_aimbot >();