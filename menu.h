#pragma once

struct Menu
{
	int Tab = 0;
	bool IsMenuOpen = true;
	void Render();
	void Theme();
};
extern Menu menu;