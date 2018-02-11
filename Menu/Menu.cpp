#include "Menu.h"

#include "../Core/Font.h"
#include "../Core/options.h"
#include "../Cheat/Skinchanger.h"


const char* keyNames[] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};

void Menu::Init()
{
	m_isVisible = false;

	CWindow Main(100, 100, 345, 255);

#pragma region Visuals
	auto Visuals = new CChild(0, 0, 120, L"Visuals");
	Visuals->AddControl(new CCheckBox(L"Radar", &g_Options.VIS_ESP_Radar));
	Visuals->AddControl(new CCheckBox(L"Chams", &g_Options.VIS_Chams));
	Visuals->AddControl(new CCombo(&g_Options.MISC_ChamsType, { L"Normal", L"Flat", L"Health Based" }));
	Visuals->AddControl(new CCheckBox(L"ESP", &g_Options.VIS_ESP));
	Visuals->AddControl(new CCheckBox(L"Box", &g_Options.VIS_ESP_Box));
	Visuals->AddControl(new CCheckBox(L"Skeleton", &g_Options.VIS_ESP_Bone));
	Visuals->AddControl(new CCheckBox(L"Name", &g_Options.VIS_ESP_Name));
	Visuals->AddControl(new CCheckBox(L"Health", &g_Options.VIS_ESP_Health));
	Visuals->AddControl(new CCheckBox(L"Weapon", &g_Options.VIS_ESP_Weapon));
	Visuals->AddControl(new CCheckBox(L"Snap Lines", &g_Options.VIS_ESP_Snaplines));
	Visuals->AddControl(new CCheckBox(L"NightMode", &g_Options.VIS_ESP_NightMode));
	Visuals->AddControl(new CCheckBox(L"ChromeWorld", &g_Options.VIS_ESP_ChromeWorld));
	Visuals->AddControl(new CCheckBox(L"SniperCrosshair", &g_Options.VIS_ESP_SniperCrosshair));
	Visuals->AddControl(new CCheckBox(L"Grenade Prediction", &g_Options.MISC_GrenadeTrajectory));
	Visuals->AddControl(new CCheckBox(L"Disable PProccesing", &g_Options.VIS_ESP_DisablePost));


	Main.AddControl(Visuals);
#pragma endregion
#pragma region Misc
	auto Misc = new CChild(130, 0, 190, L"Misc");

	Misc->AddControl(new CCheckBox(L"Backtrack", &g_Options.MISC_Backtrack));
	Misc->AddControl(new CCheckBox(L"Force Inventory Open", &g_Options.MISC_ForceInventoryOpen));
	Misc->AddControl(new CCheckBox(L"Auto-Accept", &g_Options.MISC_AutoAccept));
	Misc->AddControl(new CCheckBox(L"Bunnyhop", &g_Options.MISC_Bunnyhop));
	Misc->AddControl(new CCheckBox(L"Reveal Ranks", &g_Options.MISC_RankReveal));
	Misc->AddControl(new CSlider<int>(L"Viewmodel FOV", 0, 40, &g_Options.MISC_ViewmodelFOV));
	Misc->AddControl(new CTextBox(80, 16, L"Clan Tag", &g_Options.MISC_ClanTag));
	Misc->AddControl(new CButton(80, 16, L"Load Skins", []() { Skinchanger::Get().LoadSkins(); }));

	Main.AddControl(Misc);
#pragma endregion
#pragma region LegitBot
	auto legitbot = new CChild(130, 140, 190, L"LegitBot");
	legitbot->AddControl(new CCheckBox(L"Aimbot", &g_Options.aim_LegitBotPistols));
	legitbot->AddControl(new CSlider<int>(L"Pistol FOV", 0, 180, &g_Options.aim_PistolsFOV));
	legitbot->AddControl(new CSlider<int>(L"Pistol Smooth", 0, 100, &g_Options.aim_PistolsSmooth));
	legitbot->AddControl(new CSlider<int>(L"Rifle FOV", 0, 180, &g_Options.aim_RiflesFOV));
	legitbot->AddControl(new CSlider<int>(L"Rifle Smooth", 0, 100, &g_Options.aim_RiflesSmooth));
	Main.AddControl(legitbot);
#pragma endregion
/*#pragma region Colors
	auto colors = new CChild(260, 140, 190, L"Colors");
	colors->AddControl(new CSlider<int>(L"vis chams r", 0, 255, &g_Options.VIS_Red));
	colors->AddControl(new CSlider<int>(L"vis chams g", 0, 255, &g_Options.VIS_Green));
	colors->AddControl(new CSlider<int>(L"vis chams b", 0, 255, &g_Options.VIS_Blue));
	Main.AddControl(colors);
#pragma endregion*/
	MenuForm.AddWindow(Main);

}

void Menu::Kill()
{}

void Menu::PaintTraverse()
{
	if (m_isVisible)
		MenuForm.Paint();
}

void Menu::Click()
{
	if (m_isVisible)
		MenuForm.Click();
}

void Menu::HandleInput(WPARAM vk)
{
	if (m_isVisible)
		MenuForm.HandleInput(vk);
}

void Menu::Toggle()
{
	m_isVisible = !m_isVisible;
}
