#pragma once

namespace valve {
	class c_engine {
	public:
		address_t get_local_player() const;
		DWORD get_client_state() const;

		address_t get_entity_from_index(int i) const;

		float get_sensitivity() const;

		math::vec3_t get_view_angles() const;
		int get_max_players() const;
	};
}
inline const auto g_engine = std::make_unique< valve::c_engine >();