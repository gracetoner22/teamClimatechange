#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "emissionsClass.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <regex>
#include <unordered_set>
#include <filesystem>

using namespace std;

bool exists(const string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;    // uses st as a blank canvas to attempt locating the file
}

template <typename T>
typename enable_if<is_same<T, fstream>::value || is_same<T, ifstream>::value || is_same<T, ofstream>::value>::type
openFile(T& file, const string& filename) {
    if (!exists(filename)) {
        throw runtime_error("File does not exist.");
    }

    file.open(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file");
    }
}

// Begin cite: Taken from https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
// Trim from the start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// Trim from the end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// Trim from both ends (in place)
inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}
// End cite

vector<string> split(const string& s, char delim) {
    vector<string> tokens;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        trim(item);
        tokens.push_back(item);
    }
    return tokens;
}

void dialogWindow(const string& title, const string& text, bool& show, const ImVec2 size = ImVec2(480, 360), const ImVec2 pos = ImVec2(100, 100)) {
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::Begin(title.c_str(), &show); // passing &showHelpWindow adds a close button
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);

    ImGui::BeginChild((title + " message").c_str(), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    ImGui::TextWrapped("%s", text.c_str());

    ImGui::EndChild();
    ImGui::End();
}

string setHelpText() {
    ifstream helperFile;
    const string& helpFileDir = "src/README.md";
    openFile(helperFile, helpFileDir);

    string line;
    vector<string> lines;

    ostringstream ss;
    ss << helperFile.rdbuf();
    return ss.str();
}

vector<string> generateDataSources(filesystem::path dataFolderPath) {
    vector<string> files;
    for (const auto& entry : filesystem::directory_iterator(dataFolderPath)) {
        if (entry.is_regular_file()) {
            filesystem::path p = entry.path();
            string fileName = p.filename().string();
            files.push_back(fileName);
        }
    }
    return files;
}

void populateTable(naryNode* root, unordered_set<naryNode*>& visited) {
    visited.insert(root);

    for (size_t i = 0; i < root->childrenMap.table.size(); ++i) {
        auto entry = root->childrenMap.table[i];   // Each bucket is a state which holds year values paired with their respective emissions for the state
        while (entry) {
            naryNode* childNode = entry->value;  // Content of bucket

            // Indicates children are state nodes
            
            if (!childNode->childrenMap.table.empty()) {
                if (!visited.count(childNode))
                {
                    std::cout << "Adding state: " << childNode->state << '\n';
                    visited.insert(childNode);
                    populateTable(childNode, visited);
                }
            }
            else // Indicates children are year nodes
            {
                std::cout << "State: " << root->state << ", Year: " << childNode->state
                    << ", Emission: " << childNode->emission << '\n';
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", root->state.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", childNode->emission);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", childNode->state.c_str());   // State does contain year value
            }      

            entry = entry->next;
        }
    }
}

void generateTable(naryNode* root) {
    unordered_set<naryNode*> visited;

    if (ImGui::BeginTable("MyTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("State");
        ImGui::TableSetupColumn("Carbon Emissions (million metric tons)");
        ImGui::TableSetupColumn("Year");
        ImGui::TableHeadersRow();

        /*populateTable(root, visited);*/

        ImGui::EndTable();
    }
}

int main() {
    const int windowWidth = 960,
        windowHeight = 720;

    const pair<int, int>& yearRange = {1960, 2023};

    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Team Climate Change");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    bool formatError = false;
    bool showHelpWindow = false;
    bool stateError = false;
    bool yearError = false;
    bool entryFound = false;
    bool displayEntryNotFoundWindow = false;

    string helpText = "";
    string dataFilePath = "data/allSectors.txt";
    string searchBoxText = "";
    string sourceBoxText = "";
    string emissionsDisplayText = "";

    const string& dataFolderPath = "data";
    vector<string> dataFiles = generateDataSources(filesystem::path(dataFolderPath));
    static string selectedSource = "";

    naryTree tree;
    tree.buildTreeFromFile(dataFilePath);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Carbon Emissions by State");

        // Reserves a region in the ImGui box for a child element
        ImGui::BeginChild("TableScrollRegion", ImVec2(580, 400), false, ImGuiWindowFlags_HorizontalScrollbar);

        generateTable(tree.root);

        ImGui::EndChild();

        if (ImGui::InputText("Search (State, Year):", &searchBoxText, ImGuiInputTextFlags_EnterReturnsTrue)) {
            // Search by state and/or year
            // Either pop user to the row that the data resides or present information in a textbox

            smatch match;
            regex search(R"(^[a-zA-Z0-9 ]+,\s?[a-zA-Z0-9]+$)"); // matches when following the entry schema: STATE, YEAR (space optional), without regard to alphanumeric or numeric matching for STATE or YEAR 
            if (!regex_match(searchBoxText, match, search)) {
                formatError = true;
            }
            else {
                vector<string> elements = split(searchBoxText, ',');
                regex stateVerify(R"(\d+)"); // matches any numerical character, since states don't have numbers
                regex yearVerify(R"([a-zA-Z]+)"); // matches any alphanumeric character since years are only numbers
                const string& state = elements[0];
                const string& year = elements[1];

                if (regex_search(state, match, stateVerify)) {
                    stateError = true;
                }

                if (regex_search(year, match, yearVerify)) {
                    yearError = true;
                }

                if (!stateError && !yearError) {
                    double emissions = tree.getEmission(state, year);
                    entryFound = (emissions != -1.0);
                    cout << entryFound;

                    if (entryFound) {
                        ostringstream out;
                        out << "Carbon emissions for " << state << " during the year " << year << " were " << emissions << " million metric tons.";
                        emissionsDisplayText = out.str();
                    }
                    else {
                        ostringstream out;
                        out << "Entry was not found, please enter a valid US state (District of Columbia included) or year between " << yearRange.first << " and "
                            << yearRange.second << ".";
                        emissionsDisplayText = out.str();
                        displayEntryNotFoundWindow = true;
                    }
                }
            }
        }

        // Display format error window
        if (formatError) {
            const string& errorText = "Entry contains invalid characters or is improperly formatted, please try again.";
            dialogWindow("Error Window", errorText, formatError, ImVec2(240, 180));
        }
        else
        // Display error window for both state and year error
        if (stateError && yearError) {
            const string& errorText = "State cannot contain numbers and year cannot contain letters. Please try again.";
            dialogWindow("Error Window", errorText, stateError, ImVec2(240, 180));
            yearError = stateError;
        }
        else
        // Display state error window
        if (stateError && !yearError) {
            const string& errorText = "State entry cannot contain numbers.";
            dialogWindow("Error Window", errorText, stateError, ImVec2(240, 180));
        }
        else
        // Display year error window
        if (!stateError && yearError) {
            const string& errorText = "Year entry cannot contain letters. Please try again.";
            dialogWindow("Error Window", errorText, yearError, ImVec2(240, 180));
        }

        if (entryFound) {
            dialogWindow("Emissions", emissionsDisplayText, entryFound, ImVec2(240, 180));
        }

        if (displayEntryNotFoundWindow) {
            dialogWindow("Emissions", emissionsDisplayText, displayEntryNotFoundWindow, ImVec2(240, 180));
        }

        if (ImGui::BeginCombo("Source: ", selectedSource.c_str())) {
            for (int n = 0; n < dataFiles.size(); n++) {
                bool isSelected = (selectedSource == dataFiles[n]);
                if (ImGui::Selectable(dataFiles[n].c_str(), isSelected)) {
                    selectedSource = dataFiles[n];

                    dataFilePath = dataFolderPath + "/" + selectedSource;
                    tree.deleteSubtree(tree.root);
                    tree.root = new naryNode("USA");
                    tree.buildTreeFromFile(dataFilePath);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Help")) {
            // Indicates help window should be shown.
            showHelpWindow = true;
            helpText = setHelpText();
        }

        // Displays text from the README.md file
        if (showHelpWindow) {
            dialogWindow("Error Window", helpText, showHelpWindow);
        }

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}