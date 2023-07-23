#pragma once
/* todo: sort it */

#define OFFSET(var, offset, type) \
    auto var() -> type { \
        return g_memory->read<type>(reinterpret_cast<std::uintptr_t>(this) + offset); \
    }

namespace valve {
    struct cs_player_t {
       // OFFSET(get_active_weapon, 0x2F08, DWORD);
        OFFSET(get_flash_duration, 0x10470, int);
        OFFSET(get_bone_matrix, 0x26A8, DWORD);
        OFFSET(get_crosshair_id, 0x11838, int);
        OFFSET(get_shots_fired, 0x103E0, int);
        OFFSET(has_immunity, 0x9990, bool);
        OFFSET(is_spotted, 0x93D, bool);
        OFFSET(is_dormant, 0xED, bool);
        OFFSET(get_health, 0x100, int);
        OFFSET(get_team, 0xF4, int);

        OFFSET(get_view_offset, 0x108, math::vec3_t);
        OFFSET(get_punch, 0x303C, math::vec3_t);
        OFFSET(get_origin, 0x138, math::vec3_t);

        short get_weapon_index() const {
            const auto active_weapon = g_memory->read<DWORD>(reinterpret_cast<std::uintptr_t>(this) + 0x2F08);
            const auto weapon_entity = g_memory->read<DWORD>((g_memory->client_base() + offsets::dwEntityList) + ((active_weapon & 0xFFF) - 1) * 0x10);
            return g_memory->read<short>(weapon_entity + 0x2FBA);
        }

        math::vec3_t get_bone_pos(int bone) {
            math::matrix3x4_t bone_matrix = g_memory->read<math::matrix3x4_t>(get_bone_matrix() + bone * 0x30);
            return {
                bone_matrix.matrix[0][3],
                bone_matrix.matrix[1][3],
                bone_matrix.matrix[2][3]
            };
        }

        math::vec3_t get_eye_pos() {
            return get_origin() + get_view_offset();
        }

        bool is_alive() {
            return get_health() > 0;
        }
    };
}