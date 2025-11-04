#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "emissionsClass.h"
using namespace std;

int main() {
    string filename = "/Users/gracetoner22/Downloads/Co2 emissions/allSectors.txt";

    cout << "            Options:\n"
         << "  All Sectors      Electric Sector\n "
         << " Industrial Sector   Residential Sector\n"
         << "       Transportation Sector\n"
         << "          Years: 1960-2023\n"
         << "All U.S. States including the District of Columbia\n";

    string stateInput, sectorInput, yearInput;
    cout << "Type in which State you would like to see: ";
    getline(cin, stateInput);
    cout << "Type in which Sector you would like to see: ";
    getline(cin, sectorInput);
    cout << "Type in which Year you would like to investigate: ";
    getline(cin, yearInput);

    if (sectorInput == "Electric") {
        filename = "/Users/gracetoner22/Downloads/Co2 emissions/Electric Sector.txt";
    } else if (sectorInput == "Industrial") {
        filename = "/Users/gracetoner22/Downloads/Co2 emissions/IndustrialSector.txt";
    } else if (sectorInput == "Transportation") {
        filename = "/Users/gracetoner22/Downloads/Co2 emissions/transportationSector.txt";
    } else if (sectorInput == "Residential") {
        filename = "/Users/gracetoner22/Downloads/Co2 emissions/residentialSector.txt";
    }

    naryTree tree;
    tree.buildTreeFromFile(filename);

    double emission = tree.getEmission(stateInput, yearInput);

    if (emission >= 0) {
        std::string unit;
        if (sectorInput == "Electric") {
            unit = "Metric tons CO2 per billion Btu";
        } else {
            unit = "Million metric tons CO2";
        }

        cout << "In " << stateInput << " (" << sectorInput << "), " << yearInput
             << " CO2 Emissions = " << emission << " " << unit << "\n";
    } else {
        cout << "No data found for " << stateInput << " in " << yearInput << endl;
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
