#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "emissionsClass.h"
#include "logan.h"
using namespace std;

int main() {
    string filename = "data/allSectors.txt";
    string introHeading = "Welcome to Team Beat Climate Change’s Homepage!";
    int consoleWidth = 60;
    int padding = (consoleWidth - introHeading.length()) / 2;
    cout << string(padding, ' ') << introHeading << "\n\n";
    unordered_map<string, string> stateNameToCode = {
        {"Alabama", "AL"}, {"Alaska", "AK"}, {"Arizona", "AZ"}, {"Arkansas", "AR"},
        {"California", "CA"}, {"Colorado", "CO"}, {"Connecticut", "CT"}, {"Delaware", "DE"},
        {"Florida", "FL"}, {"Georgia", "GA"}, {"Hawaii", "HI"}, {"Idaho", "ID"},
        {"Illinois", "IL"}, {"Indiana", "IN"}, {"Iowa", "IA"}, {"Kansas", "KS"},
        {"Kentucky", "KY"}, {"Louisiana", "LA"}, {"Maine", "ME"}, {"Maryland", "MD"},
        {"Massachusetts", "MA"}, {"Michigan", "MI"}, {"Minnesota", "MN"}, {"Mississippi", "MS"},
        {"Missouri", "MO"}, {"Montana", "MT"}, {"Nebraska", "NE"}, {"Nevada", "NV"},
        {"New Hampshire", "NH"}, {"New Jersey", "NJ"}, {"New Mexico", "NM"}, {"New York", "NY"},
        {"North Carolina", "NC"}, {"North Dakota", "ND"}, {"Ohio", "OH"}, {"Oklahoma", "OK"},
        {"Oregon", "OR"}, {"Pennsylvania", "PA"}, {"Rhode Island", "RI"}, {"South Carolina", "SC"},
        {"South Dakota", "SD"}, {"Tennessee", "TN"}, {"Texas", "TX"}, {"Utah", "UT"},
        {"Vermont", "VT"}, {"Virginia", "VA"}, {"Washington", "WA"}, {"West Virginia", "WV"},
        {"Wisconsin", "WI"}, {"Wyoming", "WY"}
    };
    cout << "We recommend reading our brief introduction below so that you are familiar "
            "with the terminology before using our emissions program.\n\n";
    cout<<"We are doing this implementing an N-ary tree and a HashMap";
    cout << "What is this program supposed to show me?\n";
    cout << " This program is aimed at showing the rate of CO2 emissions from the varying\n "
            "U.S. government sectors, which includes Electric, Residential, Industrial,\n "
            "and Transportation. By noticing the general upward trend of emissions over all\n "
            "sectors, this program will inform our users and allow them to explore just how\n "
            "much our government is contributing to the increase in global CO2 emissions.\n\n";


    cout << "+------------------------+-------------------------------+\n";
    cout << "| Options                | Description                   |\n";
    cout << "+------------------------+-------------------------------+\n";
    cout << "| All Sectors            | Shows total emissions         |\n";
    cout << "| Electric Sector        | Electric power emissions only |\n";
    cout << "| Industrial Sector      | Industrial emissions          |\n";
    cout << "| Residential Sector     | Residential emissions         |\n";
    cout << "| Transportation Sector  | Transportation emissions      |\n";
    cout << "| Years                  | 1960 - 2023                  |\n";
    cout << "| States                 | All U.S. States + DC          |\n";
    cout << "+------------------------+-------------------------------+\n";
    string stateInput, sectorInput, yearInput, structureInput;
    cout << "Type in which State you would like to see: ";
    getline(cin, stateInput);
    cout << "Type in which Sector you would like to see: Ex: Electric, All ";
    getline(cin, sectorInput);
    cout << "Type in which Year you would like to investigate: ";
    getline(cin, yearInput);
    cout<<"Type in whether you want to see this implementing N-ary or HashMap?:";
    getline(cin, structureInput);


    if (sectorInput == "Electric") {
        filename = "data/Electric Sector.txt";
    } else if (sectorInput == "Industrial") {
        filename = "data/IndustrialSector.txt";
    } else if (sectorInput == "Transportation") {
        filename = "data/transportationSector.txt";
    } else if (sectorInput == "Residential") {
        filename = "data/residentialSector.txt";
    }else if (sectorInput == "All") {
        filename = "data/allSectors.txt";
    }
     if (structureInput == "N-ary") {
        naryTree tree;
        tree.buildTreeFromFile(filename);

        double emission = tree.getEmission(stateInput, yearInput);

        if (emission >= 0) {
            string unit = (sectorInput == "Electric") ?
                "Metric tons CO2 per billion Btu" : "Million metric tons CO2";
            cout << "In " << stateInput << " (" << sectorInput << "), " << yearInput
                 << " CO2 Emissions = " << emission << " " << unit << "\n";
        } else {
            cout << "No data found for " << stateInput << " in " << yearInput << endl;
        }
    }

     else if (structureInput == "HashMap") {
         hashMap<string, vector<pair<int, double>>> map;
         buildMapFromFile(map, filename);
         string stateCode;
         if (stateNameToCode.find(stateInput) != stateNameToCode.end()) {
             stateCode = "USA-" + stateNameToCode[stateInput];
         } else {
             cout << "Invalid state name: " << stateInput << endl;
             return -1;
         }
         string key = stateCode + "|" + sectorInput;
         vector<pair<int, double>>* data = map.searchTable(key);
         if (data == nullptr) {
             cout << "No data found for " << stateInput << endl;
         } else {
             bool found = false;
             int queryYear = stoi(yearInput);
             for (auto& [year, emission] : *data) {
                 if (year == queryYear) {
                     string unit = (sectorInput == "Electric") ?
                         "Metric tons CO2 per billion Btu" : "Million metric tons CO2";
                     cout << "In " << stateInput << " (" << sectorInput << "), " << yearInput
                          << " CO2 Emissions = " << emission << " " << unit << "\n";
                     found = true;
                     break;
                 }
             }
             if (!found) {
                 cout << "No data for " << stateInput << " in " << yearInput << endl;
             }
         }
     }

    vector<string> states = {
        "Alabama", "Alaska", "Arizona", "Arkansas", "California",
    "Colorado", "Connecticut", "Delaware", "District of Columbia", "Florida", "Georgia", "Hawaii", "Idaho",
    "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts",
    "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska", "New Hampshire", "New Jersey",
    "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania",
    "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia",
    "Washington", "West Virginia", "Wisconsin", "Wyoming"
    };
    vector<string> years = {
        "1960", "1961", "1962", "1963", "1964", "1965", "1966", "1967", "1968", "1969",
        "1970", "1971", "1972", "1973", "1974", "1975", "1976", "1977", "1978", "1979",
        "1980", "1981", "1982", "1983", "1984", "1985", "1986", "1987", "1988", "1989",
        "1990", "1991", "1992", "1993", "1994", "1995", "1996", "1997", "1998", "1999",
        "2000", "2001", "2002", "2003", "2004", "2005", "2006", "2007", "2008", "2009",
        "2010", "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019",
        "2020", "2021", "2022", "2023"
    };


    return 0;
}
