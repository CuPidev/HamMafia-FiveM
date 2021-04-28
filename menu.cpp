#include "menu.h"
#include "font_awesome.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "gui.h"
#include "TextEditor.h"
#include "ImGuiFileDialog.h"
#include <fstream>
#include "vars.h"

#define SECTION
static TextEditor LuaEditor;
static const char* current_resource = "NONE";
extern ImFont* Consolas;
Vars cVars;

void Separator(const char* Id)
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(68, 68, 68, 255));
	ImGui::BeginChild(Id, ImVec2(ImGui::GetContentRegionAvailWidth(), 1), true);
	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void Menu::Render()
{
	ImGui::Begin("HamMafia.co", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::Columns(2, "##OurColumns", true);
	ImGui::SetColumnOffset(1, 255); 

	if (ImGui::Button(ICON_FA_CODE" Executor", ImVec2(240, 45)))
		Menu::Tab = 1;
	ImGui::Spacing();
	if (ImGui::Button(ICON_FA_DOWNLOAD" Dumper", ImVec2(240, 45)))
		Menu::Tab = 2;
	ImGui::Spacing();
	if (ImGui::Button(ICON_FA_BUG" Blocker", ImVec2(240, 45)))
		Menu::Tab = 3;
	ImGui::Spacing();
	if (ImGui::Button(ICON_FA_ANCHOR" Variables", ImVec2(240, 45)))
		Menu::Tab = 4;
	ImGui::Spacing();
	if (ImGui::Button(ICON_FA_BOOK" Menus", ImVec2(240, 45)))
		Menu::Tab = 5;
	ImGui::Spacing();
	if (ImGui::Button("? Settings", ImVec2(240, 45)))
		Menu::Tab = 6;
	
	{
		ImGui::SetCursorPosX(255 / 2.41);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
		ImGui::Text("xo1337");
		ImGui::NextColumn();
	}

	SECTION;
	{
		if ((Menu::Tab == 0))
		{
			ImGui::SetCursorPosX(400);
			ImGui::Text("Please select a section you want to access");
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 1))
		{
			LuaEditor.SetShowWhitespaces(false);
			LuaEditor.SetReadOnly(false);
			LuaEditor.SetPalette(TextEditor::GetDarkPalette());
			LuaEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

			if (ImGui::Button(ICON_FA_FILE" Execute from file")) {
				igfd::ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose a file to execute", ".lua\0\0", ".");
			}
			ImGui::Spacing();

			ImGui::PushFont(Consolas);
			LuaEditor.Render("##HamMafiaLuaEditor", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() - 137));
			ImGui::PopFont();
			ImGui::Spacing();
			if (ImGui::BeginCombo("##HamMafiaResourcesMenu", current_resource)) {
				ImGui::EndCombo();
			}
			ImGui::SameLine(0, 11);
			if (ImGui::Button(" " ICON_FA_CHECK " Execute")) {}

			if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
			{
				if (igfd::ImGuiFileDialog::Instance()->IsOk == true) { 
				}			
				igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			}
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 2))
		{
			ImGuiTextFilter Filter;
			//252 x 39
			Filter.Draw("##HamMafiaResourceFilter", 250);
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
			ImGui::BeginChild("##HamMafiaResourcesList", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() - 137)); 
			{
				for (unsigned int i = 0; i < cVars.fivem.resources.size(); i++)
				{
					if (Filter.PassFilter(cVars.fivem.resources[i])) {
						if (ImGui::TreeNode(cVars.fivem.resources[i])) {
							ImGui::Text("client.lua\n__resource.lua");
							ImGui::TreePop();
						}
					}
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::Spacing();
			if (ImGui::Button(ICON_FA_FOLDER_OPEN" Save to folder", ImVec2(220, 32))) {
				ImGui::OpenPopup("Dumps");
			}
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 3))
		{
			ImGui::Checkbox("##BlockerEnable", &cVars.fivem.blocker_enabled);
			ImGui::SameLine();
			ImGui::Text("Enabled");
			ImGui::Spacing();
			{
				ImGui::BeginChild("##HamMafiaBlockerSection", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() - 228));
				ImGui::EndChild();
			}
			ImGui::Spacing();
			Separator("##HamMafiaBlockerSeparatorOne");
			ImGui::Spacing();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::InputTextWithHint("##HamMafiaEventInputName", "Event Name", cVars.fivem.blocker_buffer, 72);
			
			Separator("##HamMafiaBlockerSeparatorTwo");
			ImGui::Spacing();
			ImGui::Combo("##HamMafiaBlockerCombo", &cVars.fivem.current_type, cVars.fivem.block_type.data(), cVars.fivem.block_type.size());
			ImGui::PopItemWidth();
			if (ImGui::Button("Add", ImVec2(ImGui::GetContentRegionAvailWidth() / 2, 31))) {}
			ImGui::SameLine();
			if (ImGui::Button("Remove", ImVec2(ImGui::GetContentRegionAvailWidth() , 31))) {}
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 4))
		{
			ImGuiTextFilter Filter;
			Filter.Draw("##HamMafiaVariablesFilter", 250);
			
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 45);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::Combo("##HamMafiaVariablesCombo", &cVars.fivem.current_variable, cVars.fivem.variables.data(), cVars.fivem.variables.size());
			ImGui::PopItemWidth();
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 5))
		{
			//menus
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(36, 37, 36, 255));
			ImGui::BeginChild("##HamMafiaLuaMenus", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() - 97));
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::ListBox("##HamMafiaMenuBox", &cVars.current_menu, cVars.menus.data(), cVars.menus.size());
			ImGui::PopItemWidth();
			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::Spacing();
			if (ImGui::Button("Execute Menu", ImVec2())) {}
			ImGui::SameLine(0, 10);
			if(ImGui::Button("Redeem", ImVec2())) {}
		}
	}
	SECTION;
	{
		if ((Menu::Tab == 6))
		{
			ImGui::Checkbox("NetEvent Bypass", &cVars.fivem.netEventBypass);
			ImGui::Checkbox("Killswitch", &cVars.killswitch);
		}
	}


	ImGui::End();
}

void Menu::Theme()
{
	ImGui::GetMouseCursor();
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGui::GetIO().WantCaptureMouse = Menu::IsMenuOpen;
	ImGui::GetIO().MouseDrawCursor = Menu::IsMenuOpen;

	auto& style = ImGui::GetStyle();

	style.WindowMinSize = ImVec2(910, 420);
	style.FrameBorderSize = 0;
	style.WindowRounding = 0;
	style.TabRounding = 0;
	style.ScrollbarRounding = 0;
	style.FramePadding = ImVec2(6.5, 6.5);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ItemInnerSpacing = ImVec2(8, 8);
	style.DisplayWindowPadding.x = 0;
	style.ChildBorderSize = 0.99;
	style.WindowBorderSize = 0;

	style.Colors[ImGuiCol_TitleBg] = ImColor(249, 79, 49, 255);	
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(249, 79, 49, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 150);		    

	style.Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255);			

	style.Colors[ImGuiCol_Button] = ImColor(29, 28, 29, 255);		
	style.Colors[ImGuiCol_ButtonActive] = ImColor(54, 53, 55);	     
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(54, 53, 55);		 

	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);		

	style.Colors[ImGuiCol_FrameBg] = ImColor(36, 37, 36, 255);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(36, 37, 36, 255);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(36, 37, 36, 255);
	
	style.Colors[ImGuiCol_Header] = ImColor(33, 33, 33, 255);	
	style.Colors[ImGuiCol_HeaderActive] = ImColor(34, 33, 35);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(35, 35, 35, 100);

	style.Colors[ImGuiCol_ResizeGrip] = ImColor(51, 49, 50, 255);		  
	style.Colors[ImGuiCol_ResizeGripActive] = ImColor(54, 53, 55);		
	style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(51, 49, 50, 255);		

	style.Colors[ImGuiCol_SliderGrab] = ImColor(249, 79, 49, 255);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(249, 79, 49, 255);

	style.Colors[ImGuiCol_Border] = ImColor(54, 54, 54);

	style.Colors[ImGuiCol_ChildBg] = ImColor(32, 32, 32, 255);
	style.Colors[ImGuiCol_Separator] = ImColor(68, 68, 68, 255);
	style.Colors[ImGuiCol_SeparatorActive] = ImColor(68, 68, 68, 255);
	style.Colors[ImGuiCol_SeparatorHovered] = ImColor(68, 68, 68, 255);
}