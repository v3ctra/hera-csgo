#include "../../csgo.hpp"

namespace csgo {
    void c_ctx::initialize() {
        LOG("Cheat initialization routine has been called...\n");

       // g_config->save();
        g_config->load();

        LOG("Getting Modules...");
        GET_MODULE(m_modules.m_ntdll,  hash(xorstr("ntdll.dll")));
        GET_MODULE(m_modules.m_user32, hash(xorstr("user32.dll")));
        GET_MODULE(m_modules.m_win32u, hash(xorstr("win32u.dll")));
        GET_MODULE(m_modules.m_Winmm, hash(xorstr("winmm.dll")));

        LOG("address of entrypoint: " << m_modules.m_user32.get_nt_header()->OptionalHeader.AddressOfEntryPoint);

        LOG("Getting ntdll gadget address...");
        m_offsets.m_gadget = BYTESEQ(m_modules.m_ntdll, xorstr("FF 23"));
        
        g_memory->initialize(xorstr("csgo.exe"));
        
        g_offsets->update();

        auto aimbot_callback = [](void* lpParameter) -> DWORD {
            if (!g_memory->is_initialized())
                return 0;

            while (true) {
                g_aimbot->run();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            return 0;
        };
        auto aim_thread = g_memory->create_spoofed_thread(aimbot_callback, m_modules.m_user32);
        if (aim_thread)
            g_winapi->close_handle(aim_thread);

        auto triggerbot_callback = [](void* lpParameter) -> DWORD {
            if (!g_memory->is_initialized())
                return 0;

            while (true) {
                g_triggerbot->run();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            return 0;
        };
        auto trigger_thread = g_memory->create_spoofed_thread(triggerbot_callback, m_modules.m_ntdll);
        if (trigger_thread)
            g_winapi->close_handle(trigger_thread);

        auto soundesp_callback = [](void* lpParameter) -> DWORD {
            if (!g_memory->is_initialized())
                return 0;

            while (true) {
                g_sound_esp->run();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            return 0;
        };
        auto sound_thread = g_memory->create_spoofed_thread(soundesp_callback, m_modules.m_win32u);
        if (sound_thread)
            g_winapi->close_handle(sound_thread);

        while (true) {
            if (g_winapi->get_key_state(g_config->panic_key))
                break;
            
            if (g_winapi->get_key_state(g_config->reload_key) & 0x8000)
                g_config->load();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        g_memory->uninitialize();
    }
}