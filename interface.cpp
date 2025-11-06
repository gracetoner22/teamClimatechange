//#include <imgui.h>
//#include <imgui-SFML.h>
//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
//
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        while (const std::optional event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//}
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

using namespace std;
int main() {
    const int windowWidth = 960,
        windowHeight = 720;

    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Team Climate Change");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Climate Change Impact by State");

        // Reserves a region in the ImGui box for a child element
        ImGui::BeginChild("TableScrollRegion", ImVec2(580, 400), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (ImGui::BeginTable("MyTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupColumn("State");
            ImGui::TableSetupColumn("Year");
            ImGui::TableSetupColumn("Emissions");
            ImGui::TableHeadersRow();

            // data generated from the nary/hash map
            for (int row = 0; row < 50; ++row)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Row %d, Col 1", row);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("Row %d, Col 2", row);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Row %d, Col 3", row);
            }

            ImGui::EndTable();
        }

        ImGui::EndChild();

        string searchBoxText = "";
        if (ImGui::InputText("Search:", &searchBoxText)) {
            // Search by state and/or year
            // Either pop user to the row that the data resides or present information in a textbox
        }

        string sourceBoxText = "";
        if (ImGui::InputText("Source:", &sourceBoxText)) {
            // Displays data from the source file entered by user
            // Probably replace with a file finder button that allows the user to select where the file is from the explorer
        }

        if (ImGui::Button("Help")) {
            // Pulls up the readMe file
        }

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}