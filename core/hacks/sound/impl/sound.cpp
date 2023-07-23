#include "../../../csgo.hpp"

#include "../resource/sound.hpp"

namespace hacks {
	void c_sound::run() const {
		const auto local = g_engine->get_local_player().as<valve::cs_player_t*>();
		if (!local || !local->is_alive())
			return;

		auto view_angles = g_engine->get_view_angles();
		auto eye_pos = local->get_eye_pos();

		int my_team = local->get_team();

		valve::cs_player_t* entity = g_aimbot->get_closest_player(view_angles, eye_pos, 2);
		if (!entity || entity->is_dormant() || !entity->is_alive())
			return;

		int entity_team = entity->get_team();
		if (entity_team != my_team && entity_team > 1) {
			g_winapi->play_sound(rawData, NULL, SND_MEMORY);
		}
	}
	/*
	valve::cs_player_t* c_sound::get_closest_player(math::vec3_t viewangles, math::vec3_t eyepos, float fov) const {
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
	}*/
}