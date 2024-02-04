#pragma once
#include <string>
#include "..\core.h"
#include "menu.h"

namespace Menu {
	namespace Player {
		extern float playerHealth;
		extern float playerMaxHealth;
		extern KeyBindOption godMode;
		extern KeyBindOption freezePlayer;
		extern Option playerVariables;
		extern Option disableOutOfBoundsTimer;
		extern KeyBindOption nightrunnerMode;

		extern std::string saveSCRPath;
		extern std::string loadSCRFilePath;

		class Tab : MenuTab {
		public:
			Tab() : MenuTab("Player", 0) {}
			void Update() override;
			void Render() override;

			static Tab instance;
		};
	}
}