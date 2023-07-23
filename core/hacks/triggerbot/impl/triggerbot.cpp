#include "../../../csgo.hpp"

namespace hacks {
	void c_triggerbot::run() const {
		const auto local = g_engine->get_local_player().as<valve::cs_player_t*>();
		if (!local)
			return;

		int my_team = local->get_team();

		int my_crosshair = local->get_crosshair_id();

		if (g_winapi->get_key_state(g_config->trigger_key) && my_crosshair > 0 && my_crosshair <= g_engine->get_max_players()) {
			const auto entity = g_engine->get_entity_from_index(my_crosshair - 1).as<valve::cs_player_t*>();
			if (!entity || entity->has_immunity() || !entity->is_alive())
				return;

			int entity_team = entity->get_team();
			if (entity_team != my_team && entity_team > 1) {
				g_winapi->mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				g_winapi->mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
	}
}