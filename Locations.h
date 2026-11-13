//: Locations.h

 /*
 Title: Locations.h
 Description: Location class for Wonderland game
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
 	Implements the Locations class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
  
 Classes:
	Location data structure: name, desc, visit_count, n_destination, s_destination, w_destination, e_destination;
 
    Locations class - vector of type Location named locations
 
 Variables:
     member properties for Location: name, desc, visit_count, n_destination, s_destination, w_destination, e_destination;

     member properties for Locations - vector of type Location named locations

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to move between locations using the available directions.
    >Start point should be The Field
    >The final destination(location) should be Wonderland.

 Edge case (location file)
    >If the lcoation file loc_desc.txt does not load
    > Expected: An appropriate message sould be displayed to the player.

 
 Discussion:
 
*/

#ifndef LOCATIONS_H
#define LOCATIONS_H

#include "Utilities.h"
//#include "Characters.h"

#include <iostream>
#include <string>

#include <fstream>
#include <vector>
#include <cctype>
#include <random>

/* Location class */
/* Strucuture to represent a location */
struct Location {
        
        std::string name;                // location name
        std::string desc;                // location description
        int visit_count;                // keeps count of how many times player has visited location
        
        /* Entrances / Destination / Exits
            North -> Upward
            South -> Downward
            East -> Forward
            West -> Backward
        */
        std::string n_destination, s_destination, w_destination, e_destination;

        Location(const std::string &loc_name,
                const std::string &loc_desc,
                const std::string &n,
                const std::string &s,
                const std::string &w,
                const std::string &e) :
            name(loc_name),
            desc(loc_desc),
            visit_count(0),
            n_destination(n),
            s_destination(s),
            w_destination(w),
            e_destination(e) {}
};

/* Locations class */
class Locations {

	protected:
	private:

		std::vector<Location> locations;

	public:
		
		Locations() {
			loadLocations("game_files/loc_desc.txt");
			
		}
		
		/* Method to get the locations vector in Location class.
		   Since locations vectore is private, we provide a get method for it
		 */
		std::vector<Location> &getLocations() {
			return locations;
		}
		// test printing locations
		void printLocs() {
			
			for (std::size_t i = 0; i < locations.size(); i++) {
				std::cout << locations[i].name
					<< "| " << locations[i].desc
					<< "| " << locations[i].visit_count
					<< "| " << locations[i].n_destination
					<< "| " << locations[i].s_destination
					<< "| " << locations[i].w_destination
					<< "| " << locations[i].e_destination << "\n";
			}
		}

		/* Method to get locations vector size */
		std::size_t getLocationsSize() {
			return locations.size();
		}

		/* method to get the start location name in string */
		std::string getStartLocationName() const {
			
			return locations[0].name;
		}

		/* Method to get first location - Field */
		Location *getStartLocation() {

			if (locations.empty()) return nullptr;

			return &locations.front();
		}

		/* Method to get the end location name in string */
		std::string getEndLocationName() const {

			return locations[locations.size()-1].name;
		}

		/* Method to get end location - Wonderland */
		Location *getEndLocation() {

			if (locations.empty()) return nullptr;

			return &locations.back();
		}

		/* Method to get the name of a location in string 
		 given the index, we retrieve the name of the locaiton in string
		*/
		std::string getLocationNameByIndex(std::size_t index) {

			std::string result;
			for (std::size_t i = 0; i < locations.size(); i++) {
				result = locations[index].name;
			}

			return result;
		}

		/* Method to get location by index */
		Location *getLocationByIndex(std::size_t index) {

			if (index >= locations.size()) return nullptr;

			return &locations[index];
		}

		/* Method to get a location
		 given the name, we iterate the array to find a location with the corresponding name in string
		*/
		Location *getLocationByName(const std::string &name) {

			for (Location &loc : locations) {
				if (loc.name == name) return &loc;
			}

			return nullptr;
		}

		/* load locations method */
		bool loadLocations(const std::string &filename) {

			std::ifstream file(filename);
			if (!file.is_open()) {
				std::cerr << "Could not open location file: " << filename << "\n";

				return false;
			}
			
			// clear vector
			locations.clear();

			std::string line;

			while (std::getline(file, line)) {
				if (line.empty()) continue;

				/* Location file format
				   Name|Description|North Destination|South Destination|West dDestination|East Destination
				 */
				std::string fields[6];
				std::size_t start = 0;
				bool valid_line = true;			// check that the line is valid

				for (int i = 0; i < 5; i++) {

					std::size_t separator = line.find('|', start);

					if (separator == std::string::npos) {
						valid_line = false;
						break;
					}

					fields[i] = line.substr(start, separator - start);

					start = separator+1;
				}

				if (!valid_line) {
					std::cerr << "Invalid location record: " << line << "\n";
					continue;
				}

				fields[5] = line.substr(start);

				// Remove carriage returns if present
				for (std::string &field : fields) {
					if (!field.empty() && field.back() == '\r') {
						field.pop_back();
					}
				}

				locations.emplace_back(
						fields[0],
						fields[1],
						fields[2],
						fields[3],
						fields[4],
						fields[5]
						);
			}

			file.close();

			return true;
		}

		/* Method to reset the visit count for every location */
		void resetVisitCounts() {

			for (Location &loc : locations) {
				loc.visit_count = 0;
			}
		}


		/* Find a location name */
		Location *findLocation(const std::string &loc_name) {
			// make copies of string to compare in lower case
			//std::string search_loc_name = toLowerCase(loc_name);
			
			for (Location &loc : locations) {
				
				//std::string current_loc_name = toLowerCase(loc.name);
				//if (search_loc_name == current_loc_name) return &loc;
				if (toLowerCase(trim(loc.name)) == toLowerCase(trim(loc_name))) return &loc;
			}

			return nullptr;
		}

		/* Display a location when the player enters it */
		bool enterLocation(const std::string &loc_name) {

			Location *loc = findLocation(loc_name);

			if (loc == nullptr) {
				std::cout << "Location not found: " << loc_name << ".\n";
				return false;
			}

			if (loc->visit_count == 0) {
				std::cout << "You have arrived at " << loc->name << ".\n";
				std::cout << loc->desc << "\n";
			}
			else {
				std::cout << "You have returned to " << loc->name << ".\n";
			}

			// increment visit count
			loc->visit_count++;

			return true;
		}

		/* Method to move player in a specific direction */
		//bool move(std::string &current_loc, const std::string &direction) {
		bool move(Location *&current_loc, const std::string &direction) {

			//Location *loc = findLocation(current_loc);

			if (current_loc == nullptr) {
				std::cout << "Your current location could not be found.\n";
				return false;
			}

			Location *loc = current_loc;
			std::string destination;
			std::string direction_ = toLowerCase(direction);

			// recall that n_destination and the likes are all of type string
			if (direction_ == "north") destination = loc->n_destination;
			else if (direction_ == "south") destination = loc->s_destination;
			else if (direction_ == "west") destination = loc->w_destination;
			else if (direction_ == "east") destination = loc->e_destination;
			else {
				std::cout << "Unknown direction: " << direction << "\n";
				return false;
			}

			// check if a connection exists
			if (destination.empty() || destination == "NONE") {
				std::cout << "You can't go " << direction_ << " from " << loc->name << ".\n";
				return false;
			}
			// if random, get the random location's name and set it to the destination
			else if (destination == "random location") {
				std::string random_loc = getRandomLocation().name;
				destination = random_loc;
				std::cout << "Going back from Dreamland sends you to a random location. Goodluck! You'd need it 😉\n";
			}

			// find destination location by name
			Location *next_loc = findLocation(destination);

			// verify that the destination is a valid location
			if (next_loc == nullptr) {
				std::cout << "The destination '" << destination << "' does not exist in the location file.\n";
				return false;
			}

			// Update the main character's to the actual destination
			current_loc = next_loc;

			// display the description of the new current location
			return enterLocation(current_loc->name);
		}

		/* Display the available directions from a location */
		void displayConnections(const std::string &loc_name) {

			Location *loc = findLocation(loc_name);

			if (loc == nullptr) {
				std::cout << "Location not found.\n";
				return;
			}

			std::cout << "\nAvailable directions from " << loc->name << ".\n";

			if (loc->n_destination != "NONE" && !loc->n_destination.empty()) {
				std::cout << "North ->" << loc->n_destination << "\n";
			}

			if (loc->s_destination != "NONE" && !loc->s_destination.empty()) {
				std::cout << "South ->" << loc->s_destination << "\n";
			}

			if (loc->w_destination != "NONE" && !loc->w_destination.empty()) {
				std::cout << "West ->" << loc->w_destination << "\n";
			}

			if (loc->e_destination != "NONE" && !loc->e_destination.empty()) {
				std::cout << "East ->" << loc->e_destination << "\n";
			}

		}

		/* method to get locations */
		const std::vector<Location> &getLocations() const {
			return locations;
		}

		/* method to generate a random location 
		 return a reference to avoid returning a copy of the location instead of the original
		*/
		Location &getRandomLocation() {

			Location *random_loc;

			// while random location is Wonderland, 
			// get another random location before displaying to user
			do {
				random_loc = &getRandom(locations);
			} while (random_loc == &locations.back());

			return *random_loc;;

		}

};

#endif // !LOCATIONS_H

