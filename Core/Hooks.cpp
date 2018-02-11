#include "Hooks.h"
#include "Utilities.h"
#include "InputSys.h"
#include "Font.h"
#include "math.h"
#include "../Menu/Menu.h"
#include <codecvt>
#include <string>


void Marquee(std::string& clantag)
{
	std::string temp = clantag;
	clantag.erase(0, 1);
	clantag += temp[0];
}

RecvVarProxyFn originialsequencehook;

struct KnifeInfo_t {
	int definition_index;
	std::string display_name;
	std::string entity_name;
	std::string model_name;
	std::string killicon_name;

	KnifeInfo_t() {
		definition_index = 0;
		display_name = entity_name = model_name = killicon_name = "";
	}
	KnifeInfo_t(int definition, std::string display, std::string entity, std::string model, std::string killicon) {
		definition_index = definition;
		display_name = display;
		entity_name = entity;
		model_name = model;
		killicon_name = killicon;
	}
};

//default, bayonet, flip, gut, karambit, m9, huntsman, bowie, butterfly, falchion, daggers

enum KNIVES : int {
	DEFAULT_T = -1,
	DEFAULT_CT = 0,
	BAYONET = 1,
	FLIP,
	GUT,
	KARAMBIT,
	M9BAYONET,
	HUNTSMAN,
	BOWIE,
	BUTTERFLY,
	FALCHION,
	SHADOWDAGGERS
};

std::unordered_map<KNIVES, KnifeInfo_t> knivesInfo = {
	{ DEFAULT_CT, KnifeInfo_t(WEAPON_KNIFE, "Knife (Counter-Terrorists)", "WEAPON_KNIFE", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct") },
	{ DEFAULT_T, KnifeInfo_t(WEAPON_KNIFE_T,"Knife (Terrorists)", "WEAPON_KNIFE_T", "models/weapons/v_knife_default_t.mdl", "knife_t") },
	{ BAYONET, KnifeInfo_t(WEAPON_KNIFE_BAYONET, "Bayonet", "WEAPON_KNIFE_BAYONET", "models/weapons/v_knife_bayonet.mdl", "bayonet") },
	{ FLIP,KnifeInfo_t(WEAPON_KNIFE_FLIP, "Flip Knife", "WEAPON_KNIFE_FLIP", "models/weapons/v_knife_flip.mdl", "knife_flip") },
	{ GUT, KnifeInfo_t(WEAPON_KNIFE_GUT, "Gut Knife", "WEAPON_KNIFE_GUT", "models/weapons/v_knife_gut.mdl", "knife_gut") },
	{ KARAMBIT, KnifeInfo_t(WEAPON_KNIFE_KARAMBIT, "Karambit", "WEAPON_KNIFE_KARAMBIT", "models/weapons/v_knife_karam.mdl", "knife_karambit") },
	{ M9BAYONET, KnifeInfo_t(WEAPON_KNIFE_M9_BAYONET,  "M9 Bayonet", "WEAPON_KNIFE_M9_BAYONET", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet") },
	{ HUNTSMAN, KnifeInfo_t(WEAPON_KNIFE_TACTICAL, "Huntsman Knife", "WEAPON_KNIFE_TACTICAL", "models/weapons/v_knife_tactical.mdl", "knife_tactical") },
	{ BOWIE, KnifeInfo_t(WEAPON_KNIFE_SURVIVAL_BOWIE,  "Bowie Knife", "WEAPON_KNIFE_SURVIVAL_BOWIE", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie") },
	{ BUTTERFLY, KnifeInfo_t(WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife", "WEAPON_KNIFE_BUTTERFLY", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly") },
	{ FALCHION, KnifeInfo_t(WEAPON_KNIFE_FALCHION, "Falchion Knife", "WEAPON_KNIFE_FALCHION", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion") },
	{ SHADOWDAGGERS, KnifeInfo_t(WEAPON_KNIFE_PUSH, "Shadow Daggers", "WEAPON_KNIFE_PUSH", "models/weapons/v_knife_push.mdl", "knife_push") }
};





void ChangeKnife(C_BasePlayer* local, std::unordered_map<int, SkinInfo_t> skinsInfo, KNIVES type) {
	if (!local)
		return;

	auto weapons = local->m_hMyWeapons();
	for (auto i = 0; weapons[i]; i++) {
		C_BaseAttributableItem* weapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
		if (!weapon)
			continue;

		int definition_index = weapon->m_Item().m_iItemDefinitionIndex();
		if ((definition_index == WEAPON_KNIFE || definition_index == WEAPON_KNIFE_T) ||
			(definition_index >= WEAPON_KNIFE_BAYONET && definition_index <= WEAPON_KNIFE_PUSH)) {

			weapon->m_nModelIndex() = g_ModelInfo->GetModelIndex(knivesInfo[type].model_name.c_str());
			weapon->m_Item().m_iItemDefinitionIndex() = knivesInfo[type].definition_index;

			int definition_index = weapon->m_Item().m_iItemDefinitionIndex();
			if (definition_index == 0)
				continue;

			if (skinsInfo[definition_index].weapon_skin_id <= 0)
				continue;

			weapon->m_nFallbackPaintKit() = skinsInfo[definition_index].weapon_skin_id;
			weapon->m_nFallbackSeed() = skinsInfo[definition_index].weapon_seed;
			weapon->m_nFallbackStatTrak() = skinsInfo[definition_index].weapon_stat_trak;
			weapon->m_flFallbackWear() = skinsInfo[definition_index].weapon_wear;

			weapon->m_iItemIDHigh() = -1;
			weapon->m_iAccountID() = local->GetPlayerInfo().xuid_low;

		}
	}
}



auto RandomSequence = [](int low, int high) -> int {
	return rand() % (high - low + 1) + low;
};
const static std::unordered_map<std::string, int(*)(int)> animation_fix_map{
	{ "models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_DRAW:
		return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
	default:
		return sequence + 1;
	}
} },
{ "models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_FALCHION_IDLE1;
	case SEQUENCE_DEFAULT_HEAVY_MISS1:
		return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	default:
		return sequence - 1;
	}
} },
{ "models/weapons/v_knife_push.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_DAGGERS_IDLE1;
	case SEQUENCE_DEFAULT_LIGHT_MISS1:
	case SEQUENCE_DEFAULT_LIGHT_MISS2:
		return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
	case SEQUENCE_DEFAULT_HEAVY_MISS1:
		return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
	case SEQUENCE_DEFAULT_HEAVY_HIT1:
	case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
	case SEQUENCE_DEFAULT_LOOKAT01:
		return sequence + 3;
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	default:
		return sequence + 2;
	}
} },
{ "models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_BOWIE_IDLE1;
	default:
		return sequence - 1;
	}
} }
};





/*uint8_t* m_present;

typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
Present_T oPresent;
HRESULT _stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
static bool mouse_enabled = false;
pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

ImGuiStyle &style = ImGui::GetStyle();

return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}*/









namespace Hooks
{
	vfunc_hook HLClientHook;
	vfunc_hook EngineHook;
	vfunc_hook Direct3DHook;
	vfunc_hook VGUIPanelHook;
	vfunc_hook VGUISurfaceHook;
	vfunc_hook MDLRenderHook;
	vfunc_hook ClientModeHook;
	vfunc_hook RenderViewHook;
	vfunc_hook GameEventManagerHook;
	vfunc_hook MatSystemHook;
	//RecvPropHook* SequenceHook;

	void Initialize()
	{
		std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

		// Set up virtual function hooks
		HLClientHook.setup(g_CHLClient);
		EngineHook.setup(g_EngineClient);
		Direct3DHook.setup(g_D3DDevice9);
		VGUIPanelHook.setup(g_VGuiPanel);
		VGUISurfaceHook.setup(g_VGuiSurface);
		MDLRenderHook.setup(g_ModelRender);
		ClientModeHook.setup(g_ClientMode);
		GameEventManagerHook.setup(g_GameEvents);
		MatSystemHook.setup(g_MatSystem);


		// Hook functions
		HLClientHook.hook_index(Index::FrameStageNotify, hkFrameStageNotify);
		HLClientHook.hook_index(Index::CreateMove, hkCreateMove_Proxy);

		EngineHook.hook_index(27, IsConnected);

		Direct3DHook.hook_index(Index::EndScene, hkEndScene);
		Direct3DHook.hook_index(Index::Reset, hkReset);

		VGUIPanelHook.hook_index(Index::PaintTraverse, hkPaintTraverse);

		VGUISurfaceHook.hook_index(Index::PlaySound, hkPlaySound);
		VGUISurfaceHook.hook_index(Index::LockCursor, hkLockCursor);

		MDLRenderHook.hook_index(Index::DrawModelExecute, hkDrawModelExecute);

		ClientModeHook.hook_index(Index::GetViewmodelFOV, hkGetViewmodelFOV);
		ClientModeHook.hook_index(18, OverrideView);


		GameEventManagerHook.hook_index(Index::FireEventClientSide, hkFireEventClientSide);

		auto SequenceProp = C_BaseViewModel::GetSequenceProp();


		Font::Get().Create();
		Menu::Get().Init();
		g_ClientState->ForceFullUpdate();
		Skinchanger::Get().Dump();

	}



	void Kill()
	{
		// Kill all hooks
		static ConVar* cl_mouseenable = g_CVar->FindVar("cl_mouseenable"); cl_mouseenable->SetValue(1);
		Direct3DHook.unhook_all();
		HLClientHook.unhook_all();
		EngineHook.unhook_all();
		VGUIPanelHook.unhook_all();
		VGUISurfaceHook.unhook_all();
		MDLRenderHook.unhook_all();
		ClientModeHook.unhook_all();
		GameEventManagerHook.unhook_all();


	}

	void __stdcall OverrideView(CViewSetup* setup)
	{
		if (g_EngineClient->IsInGame()) {

			if (!g_LocalPlayer->m_bIsScoped())
				setup->fov = 100.f;
			grenade_prediction::Get().View(setup);
		}
		reinterpret_cast<OverrideView_t>(ClientModeHook.get_original<OverrideView_t>(18))(g_ClientMode, setup);
	}

	Menu *menu;
#include "../Menu/Menu.h"

	bool __stdcall IsConnected() {

		auto ofunc = EngineHook.get_original<IsConnected_t>(27);

		static void* unk = Utilities::PatternScan(GetModuleHandleA("client.dll"), "75 04 B0 01 5F") - 2;

		if (_ReturnAddress() == unk && g_Options.MISC_ForceInventoryOpen) {
			return false;
		}

		return ofunc(g_EngineClient);
	}

	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* device)
	{
		/*
		Spoofed ConVars
		*/
		{
			static auto mat_postprocess_enable = g_CVar->FindVar("mat_postprocess_enable");
			mat_postprocess_enable->m_fnChangeCallbacks.m_Size = 0;
			mat_postprocess_enable->SetValue(0);

			static auto fog_override = g_CVar->FindVar("fog_override");
			fog_override->m_fnChangeCallbacks.m_Size = 0;
			fog_override->SetValue(1);

			static auto fog_enable = g_CVar->FindVar("fog_enable");
			fog_enable->m_fnChangeCallbacks.m_Size = 0;
			fog_enable->SetValue(0);

			static auto fog_enableskybox = g_CVar->FindVar("fog_enableskybox");
			fog_enableskybox->m_fnChangeCallbacks.m_Size = 0;
			fog_enableskybox->SetValue(0);
		}

		if (g_Options.MISC_RankReveal && InputSys::Get().IsKeyDown(VK_TAB))
			Utilities::RevealRanks();

		auto ofunc = Direct3DHook.get_original<EndScene>(Index::EndScene);
		return ofunc(device);
	}

	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		auto ofunc = Direct3DHook.get_original<Reset>(Index::Reset);


		auto hr = ofunc(device, pPresentationParameters);

		if (hr >= 0)
		{
			Font::Get().Create();
		}

		return hr;
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkPlaySound(const char* name)
	{
		static auto ofunc = VGUISurfaceHook.get_original<PlaySound>(Index::PlaySound);
		ofunc(g_VGuiSurface, name);

		if (g_Options.MISC_AutoAccept)
		{
			// Auto Accept
			if (strstr(name, "!UI/competitive_accept_beep.wav"))
			{
				static auto fnAccept = (void(*)())Utilities::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");

				fnAccept();

				//This will flash the CSGO window on the taskbar
				//so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}
	}

	//--------------------------------------------------------------------------------

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#include <TlHelp32.h>
#include <Psapi.h>

	DWORD Pattern(std::string moduleName, std::string pattern) {

		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++) {
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {

				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;
			}

			else {
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}



	void Spoof(C_BaseEntity* pLocal, int Coin, int Music, int Level, int CommendsLeader, int CommendsTeacher, int CommendsFriendly)
	{
		if (!pLocal)
			return;
		static DWORD DT_CSPlayerResource = NULL;

		if (DT_CSPlayerResource == NULL)
			DT_CSPlayerResource = Pattern("client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2;

		if (!DT_CSPlayerResource)
			return;



		DWORD ptrResource = **(DWORD**)DT_CSPlayerResource;
		DWORD m_nActiveCoinRank = (DWORD)ptrResource + 0x4838 + (pLocal->EntIndex() * 4);
		DWORD m_nMusicID = (DWORD)ptrResource + 0x493C + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicLevel = (DWORD)ptrResource + 0x4AD0 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsLeader = (DWORD)ptrResource + 0x4BD4 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsTeacher = (DWORD)ptrResource + 0x4CD8 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsFriendly = (DWORD)ptrResource + 0x4DDC + (pLocal->EntIndex() * 4);

		if (Coin > 0) *(PINT)((DWORD)m_nActiveCoinRank) = Coin;
		if (Music > 0) *(PINT)((DWORD)m_nMusicID) = Music;
		if (Level > 0) *(PINT)((DWORD)m_nPersonaDataPublicLevel) = Level;
		if (CommendsLeader > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsLeader) = CommendsLeader;
		if (CommendsTeacher > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsTeacher) = CommendsTeacher;
		if (CommendsFriendly > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsFriendly) = CommendsFriendly;
	}

	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		switch (stage)
		{
		case FRAME_UNDEFINED:
			break;
		case FRAME_START:
			break;
		case FRAME_NET_UPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			if (!Menu::Get().IsVisible())
				Utilities::SetClantag(g_Options.MISC_ClanTag);
			Skinchanger::Get().Run();
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:
			break;
		case FRAME_RENDER_END:
			break;
		}

		static auto ofunc = HLClientHook.get_original<FrameStageNotify>(Index::FrameStageNotify);
		ofunc(g_CHLClient, stage);
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkLockCursor()
	{
		static auto ofunc = VGUISurfaceHook.get_original<LockCursor>(Index::LockCursor);

		if (Menu::Get().IsVisible()) g_VGuiSurface->UnlockCursor();
		else ofunc();
	}

	//--------------------------------------------------------------------------------
	float __stdcall hkGetViewmodelFOV()
	{
		static auto ofunc = ClientModeHook.get_original<GetViewmodelFOV>(Index::GetViewmodelFOV);
		if (g_EngineClient->IsTakingScreenshot()) return ofunc();
		else return ofunc() + g_Options.MISC_ViewmodelFOV;
	}
}
