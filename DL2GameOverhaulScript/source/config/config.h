#pragma once
namespace Config {
	extern int menuToggleKey;

	extern void SaveConfig();
	extern void InitConfig();
	extern void ConfigLoop();
	extern void ConfigSaveLoop();
}