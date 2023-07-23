#include "../../../csgo.hpp"

#define qabs(x) ((x) < 0 ? -(x) : (x))

namespace hacks {
	math::vec3_t c_aimbot::get_target_angle(const math::vec3_t eye_pos, const math::vec3_t position) const {
		math::vec3_t angle = (position - eye_pos).to_angle().normalize();

		angle.clamp();
		return angle;
	}

	valve::cs_player_t* c_aimbot::get_closest_player(math::vec3_t viewangles, math::vec3_t eyepos, float fov) const {
		valve::cs_player_t* best_entity{};
		for (auto i = 0; i < g_engine->get_max_players(); i++) {
			const auto entity = g_engine->get_entity_from_index(i).as<valve::cs_player_t*>();
			if (!entity)
				continue;

			auto bone_pos = entity->get_bone_pos(8);
			auto current_angles = math::calculate_angle(eyepos, bone_pos, viewangles);

			float delta = std::hypot(current_angles.x, current_angles.y);
			if (delta < fov) {
				fov = delta;
				best_entity = entity;
			}
		}
		return best_entity;
	}

	void c_aimbot::run() const {
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
		if (!g_winapi->get_key_state(g_config->aimbot_key))
			return;

		const auto local = g_engine->get_local_player().as<valve::cs_player_t*>();
		if (!local)
			return;

		auto settings = g_config->aimbot[local->get_weapon_index()];
		if (!settings.enabled)
			return;

		int shots_fired = local->get_shots_fired();
		if (shots_fired > 10)
			return;

		auto view_angles = g_engine->get_view_angles();
		auto eye_pos = local->get_eye_pos();

		valve::cs_player_t* entity = get_closest_player(view_angles + local->get_punch()/* *2 */, eye_pos, settings.fov);
		if (!entity || entity->is_dormant() || entity->has_immunity() || !entity->is_alive())
			return;

		math::vec3_t bone_pos = entity->get_bone_pos(settings.bone);
		if (bone_pos.is_zero())
			return;

		math::vec3_t aim_angles = get_target_angle(eye_pos, bone_pos);
		if (aim_angles.is_zero())
			return;

		if (settings.rcs) {
			math::vec3_t recoil_compensation = local->get_punch() * 2.0f;
			aim_angles -= recoil_compensation;
			aim_angles.clamp();
		}

		math::vec3_t delta = (view_angles - aim_angles);
		delta.clamp();

		if (qabs(delta.x) > 25.00f || qabs(delta.y) > 25.00f) {
			return;
		}

		float x = delta.y;
		float y = delta.x;

		x = (x / g_engine->get_sensitivity()) / 0.022f;
		y = (y / g_engine->get_sensitivity()) / -0.022f;

		float smooth = settings.smooth;
		float sx = 0.0f, sy = 0.0f;

		if (smooth >= 1.0f) {
			float smooth_factor = 1.0f + (std::max(std::abs(x), std::abs(y)) / smooth); // Calculate the smooth factor

			if (sx < x)
				sx = std::min(sx + smooth_factor, x);
			else if (sx > x)
				sx = std::max(sx - smooth_factor, x);

			if (sy < y)
				sy = std::min(sy + smooth_factor, y);
			else if (sy > y)
				sy = std::max(sy - smooth_factor, y);
		}
		else {
			sx = x;
			sy = y;
		}

		if (qabs((int)sx) > 127)
			return;

		if (qabs((int)sy) > 127)
			return;


		g_winapi->mouse_event(MOUSEEVENTF_MOVE, sx, sy, 0, 0);
	}
}