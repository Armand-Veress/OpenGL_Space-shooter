// UI.h
#pragma once
#include "imgui.h"

namespace GameUI {
    // Stil Sci-Fi: Culori neon și transparență
    void SetupSciFiStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 10.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.05f, 0.7f); // Fundal transparent închis
        style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.6f, 0.8f, 0.4f);       // Buton albastru transparent
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.8f, 1.0f, 0.7f);
        style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);        // Text Cyan (Neon)
    }

    void RenderHUD(int score, float time) {
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("HUD", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowFontScale(1.8f);
        ImGui::Text("SYSTEM STATUS: ACTIVE");
        ImGui::Text("COMBAT SCORE: %d", score);
        ImGui::Text("ELAPSED TIME: %.1f s", time);
        ImGui::End();
    }

    void RenderMainMenu(bool& gameStarted, int highscore) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("MAIN_MENU", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
        
        ImGui::SetWindowFontScale(2.5f);
        ImGui::Text("DEATH STAR DEFENDER");
        ImGui::SetWindowFontScale(1.2f);
        ImGui::Text("HIGHSCORE: %d", highscore);
        ImGui::Separator();

        if (ImGui::Button("INITIATE MISSION", ImVec2(250, 60))) {
            gameStarted = true;
        }
        if (ImGui::Button("ABORT", ImVec2(250, 60))) {
            // Logică exit
        }
        ImGui::End();
    }
}