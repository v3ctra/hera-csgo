#pragma once

namespace config {
	struct legitbot_settings {
		bool enabled = true;
		int fov = 2;
		float smooth = 10;
		bool rcs = true;
		int bone = 8;
	};

	class c_config {
	public:
		std::map<int, legitbot_settings> aimbot = { };

		int reload_key = VK_INSERT;
		int panic_key = VK_NUMPAD0;

		int trigger_key = VK_MENU;
		int aimbot_key = 0x1;

		void save();
		void load();
	};
}
inline const auto g_config = std::make_unique< config::c_config >();