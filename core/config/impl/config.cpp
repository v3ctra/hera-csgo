#include "../../csgo.hpp"

std::map<int, const char*> weapon_names = {
	{ item_definition_indexes::WEAPON_AK47, ("AK-47") }, { item_definition_indexes::WEAPON_AUG, ("AUG") },
	{ item_definition_indexes::WEAPON_AWP, ("AWP") }, { item_definition_indexes::WEAPON_CZ75A, ("CZ75 Auto") },
	{ item_definition_indexes::WEAPON_DEAGLE, ("Desert Eagle") }, { item_definition_indexes::WEAPON_ELITE, ("Dual Berettas") },
	{ item_definition_indexes::WEAPON_FAMAS, ("FAMAS") }, { item_definition_indexes::WEAPON_FIVESEVEN, ("Five-SeveN") },
	{ item_definition_indexes::WEAPON_G3SG1, ("G3SG1") }, { item_definition_indexes::WEAPON_GALILAR, ("Galil AR") },
	{ item_definition_indexes::WEAPON_GLOCK, ("Glock-18") }, { item_definition_indexes::WEAPON_M249, ("M249") },
	{ item_definition_indexes::WEAPON_M4A1_SILENCER, ("M4A1-S") }, { item_definition_indexes::WEAPON_M4A1, ("M4A4") },
	{ item_definition_indexes::WEAPON_MAC10, ("MAC-10") }, { item_definition_indexes::WEAPON_MAG7, ("MAG-7") },
	{ item_definition_indexes::WEAPON_MP7, ("MP7") }, { item_definition_indexes::WEAPON_MP5SD, ("MP5") },
	{ item_definition_indexes::WEAPON_MP9, ("MP9") }, { item_definition_indexes::WEAPON_NEGEV, ("Negev") },
	{ item_definition_indexes::WEAPON_NOVA, ("Nova") }, { item_definition_indexes::WEAPON_HKP2000, ("P2000") },
	{ item_definition_indexes::WEAPON_P250, ("P250") }, { item_definition_indexes::WEAPON_P90, ("P90") },
	{ item_definition_indexes::WEAPON_BIZON, ("PP-Bizon") }, { item_definition_indexes::WEAPON_REVOLVER, ("R8 Revolver") },
	{ item_definition_indexes::WEAPON_SAWEDOFF, ("Sawed-Off") }, { item_definition_indexes::WEAPON_SCAR20, ("SCAR-20") },
	{ item_definition_indexes::WEAPON_SSG08, ("SSG 08") }, { item_definition_indexes::WEAPON_SG556, ("SG 553") },
	{ item_definition_indexes::WEAPON_TEC9, ("Tec-9") }, { item_definition_indexes::WEAPON_UMP45, ("UMP-45") },
	{ item_definition_indexes::WEAPON_USP_SILENCER, ("USP-S") }, { item_definition_indexes::WEAPON_XM1014, ("XM1014") },
};
#define PREFIX ""

#define CHEAT_GENERAL xorstr("GENERAL")

#define WEAPON_SETTINGS xorstr("WEAPON_SETTINGS")

namespace config {
	void c_config::save() {
		std::ofstream out(xorstr("config.json"));

		if (!out.is_open())
			return;

		Json::Value save;

		save[PREFIX][(CHEAT_GENERAL)][xorstr("panic.key")] = VK_NUMPAD0;
		save[PREFIX][(CHEAT_GENERAL)][xorstr("config.reload.key")] = VK_INSERT;

		save[PREFIX][(CHEAT_GENERAL)][xorstr("triggerbot.key")] = VK_MENU;
		save[PREFIX][(CHEAT_GENERAL)][xorstr("aimbot.key")] = 0x1;

		for (auto& [key, val] : weapon_names) {
			save[WEAPON_SETTINGS][(val)][xorstr("enabled")] = true;
			save[WEAPON_SETTINGS][(val)][xorstr("fov")] = 2;
			save[WEAPON_SETTINGS][(val)][xorstr("smooth")] = 10;
			save[WEAPON_SETTINGS][(val)][xorstr("rcs")] = true;
			save[WEAPON_SETTINGS][(val)][xorstr("bone")] = 8;


			//save[config_header][("curve_streght")][val] = false;
			//save[config_header][("curve_offset")][val] = false;
			//save[config_header][("smooth")][val] = false;
			//save[config_header][("rcs_x")][val] = false;
			//save[config_header][("rcs_y")][val] = false;
		}

		out << save;
		out.close();
	}

	void c_config::load() {
		std::ifstream in(xorstr("config.json"));

		if (!in.good())
			save();

		if (!in.is_open())
			return;

		Json::Value load;

		in >> load;

		reload_key = load[PREFIX][(CHEAT_GENERAL)][xorstr("config.reload.key")].asInt();
		panic_key = load[PREFIX][(CHEAT_GENERAL)][xorstr("panic.key")].asInt();

		trigger_key = load[PREFIX][(CHEAT_GENERAL)][xorstr("triggerbot.key")].asInt();
		aimbot_key = load[PREFIX][(CHEAT_GENERAL)][xorstr("aimbot.key")].asInt();

		for (auto& [key, val] : weapon_names) {
			aimbot[key].enabled = load[WEAPON_SETTINGS][val][xorstr("enabled")].asBool();
			aimbot[key].fov = load[WEAPON_SETTINGS][(val)][xorstr("fov")].asInt();
			aimbot[key].smooth = load[WEAPON_SETTINGS][(val)][xorstr("smooth")].asFloat();
			aimbot[key].rcs = load[WEAPON_SETTINGS][val][xorstr("rcs")].asBool();
			aimbot[key].bone = load[WEAPON_SETTINGS][val][xorstr("bone")].asInt();
		}
		
		in.close();
	}
}