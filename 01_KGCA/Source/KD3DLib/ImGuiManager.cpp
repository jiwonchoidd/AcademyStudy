#include "ImGuiManager.h"

void ImGuiManager::Frame()
{
    if (m_bImguiEnable)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
}
void ImGuiManager::Render()
{
    if (m_bImguiEnable)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImGuiManager::OnOffImgui()
{
    m_bImguiEnable = !m_bImguiEnable;
}

bool ImGuiManager::isImguiEnable()
{
    return m_bImguiEnable;
}
ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
