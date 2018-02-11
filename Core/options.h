#pragma once

struct SkinInfo_t {
	int weapon_skin_id;
	int weapon_seed;
	int weapon_stat_trak;
	float weapon_wear;

	SkinInfo_t(int skin = 0, int seed = 0, int stattrak = -1, float wear = 0.000000001f) {
		weapon_skin_id = skin;
		weapon_seed = seed;
		weapon_stat_trak = stattrak;
		weapon_wear = wear;
	}
};

#include <string>

#define OPTION(type, var, val) type var = val

class Config
{
public:
#pragma region Visuals
	float CHAMS[3] = { 1.5f, 1.5f, 0.15f };
	SkinInfo_t skins[5035];
	bool menu_open;
	OPTION(bool, VIS_Chams, true);
	OPTION(bool, VIS_ESP, false);
	OPTION(bool, VIS_ESP_Box, false);
	OPTION(bool, VIS_ESP_Bone, false);
	OPTION(bool, VIS_ESP_Name, false);
	OPTION(bool, VIS_ESP_Health, false);
	OPTION(bool, VIS_ESP_Weapon, false);
	OPTION(bool, VIS_ESP_Snaplines, false);
	OPTION(bool, VIS_ESP_Radar, false);
	OPTION(bool, VIS_ESP_NightMode, false);
	OPTION(bool, VIS_ESP_ChromeWorld, false);
	OPTION(bool, VIS_ESP_SniperCrosshair, false);

	OPTION(bool, MISC_GrenadeTrajectory, true);
	OPTION(bool, VIS_ESP_DisablePost, true);
#pragma endregion
#pragma region Misc

	int MISC_ChamsType;
	OPTION(bool, MISC_Bunnyhop, true);
	OPTION(bool, MISC_AutoAccept, true);
	OPTION(bool, MISC_RankReveal, true);
	OPTION(bool, MISC_Backtrack, true);
	OPTION(bool, MISC_ForceInventoryOpen, true);
	int hitbox = 1;
	OPTION(int, MISC_ViewmodelFOV, 0);

	OPTION(std::wstring, MISC_ClanTag, L"");
#pragma endregion
#pragma region LegitBot
	OPTION(bool, aim_LegitBotPistols, false);
	OPTION(bool, aim_LegitBotRifles, false);
	OPTION(int, aim_PistolsSmooth, 1);
	OPTION(int, aim_PistolsFOV, 1);
	OPTION(int, aim_PistolsRCSMin, 1);
	OPTION(int, aim_PistolsRCSMax, 1);
	OPTION(int, aim_RiflesSmooth, 1);
	OPTION(int, aim_RiflesFOV, 1);
	OPTION(int, aim_RiflesRCSMin, 1);
	OPTION(int, aim_RiflesRCSMax, 1);

	bool enablemain;

	float MainSmooth = 1;
	float Mainfov;
	float main_random_Smooth = 1;
	float main_recoil_min;
	float main_recoil_max;
	float main_randomized_angle;
	int MainKey = 1;
	OPTION(bool, LB_AimBot, false);
	bool enablepistol = false;
	float Pistolfov;
	float PistolSmooth = 1;
	float pistol_random_Smooth = 1;
	float pistol_recoil_min;
	float pistol_recoil_max;
	float pistol_randomized_angle;
	int PistolKey = 1;
#pragma endregion
};


extern Config g_Options;
