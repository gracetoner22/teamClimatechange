#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "emissionsSource.cpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

std::string filename = "/Users/gracetoner22/Downloads/Co2 emissions/Electric Sector.txt";
      std::ifstream inputFile(filename);

      if (!inputFile.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return 1;
      }
      std::string line;
    naryTree tree;

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

    tree.addState(states);
    for (const auto& s:states) {
        tree.addYear(s,years);
    }

tree.printAll();

//regex pattern to get the state names//
    //years are in the same order for each file//
      return 0;
};