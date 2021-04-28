#pragma once
#include <vector>

struct Vars
{
	struct {
		std::vector<const char*> resources = { "_cfx_internal","mysql-async","fivem", "fivem-skatermap",
			"hardcap","esx_money","esx_banking","esx_robbery","esx_gopostal","esx_garbargejob","esx_truckerjob" };
		bool blocker_enabled = true;
		std::vector<const char*> block_type = { "EVENT" };
		char blocker_buffer[72] = "";
		int current_type = 0;

		int current_variable = 0;
		std::vector<const char*> variables = { "NULL" };

		bool netEventBypass = false;
	}fivem;
	bool killswitch = false;
	std::vector<const char*> menus = { "[PREM] Balla", "[PREM] Herobrine", "[PREM] Watermalone" };
	int current_menu = 0;
};
extern Vars cVars;