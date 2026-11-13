//: Directions.h

 /*
 Title: Directions.h
 Description: Directions class for Wonderland game
 Date: October 4, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the Directions class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Classes:
	Directions - 

 Variables:
 	member properties of Direction: name, vector of type string named aliases
 	member properties of Directions: vector of type Direction named directions

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to progress and interact in the game using available directions.

 Bad Data case 1 (invalid direction)
    >If player attempts to go to an unavailable location from current location
    >Expected:The game should reject that movement, handle it and display the appropriate message to player without moving the player incorrectly, thereby corrupting the game and player state.
 
 Discussion:
 
*/

#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include "Utilities.h"					// for utilities class and functions

#include <iostream>
#include <string>
#include <vector>

// direction data structure
struct Direction {
    std::string name;
    std::vector<std::string> aliases;
    
    Direction(const std::string &dir_name,
              const std::vector<std::string> &dir_aliases)
    : name(dir_name), aliases(dir_aliases) {}
    
};


/* Directions Class */
class Directions {

	private:

		std::vector<Direction> directions;

	protected:

	public:

		Directions() {
			
			// aliases for north
			directions.emplace_back("north", std::vector<std::string>{"north", "n", "up", "upward"});

			// aliases for south
			directions.emplace_back("south", std::vector<std::string>{"south", "s", "down", "downward"});

			// aliases for west
			directions.emplace_back("west", std::vector<std::string>{"west", "w", "back", "backward"});

			// aliases for east
			directions.emplace_back("east", std::vector<std::string>{"east", "e", "front", "forward"});
		}

		// method get a direction
		std::string getDirection(const std::string &command) {
			// convert input to lower case
			std::string input = toLowerCase(trim(command));

            // find the direction in the aliases and return the name
			for (const Direction &dir : directions) {
				for (const std::string &alias : dir.aliases) {
					if (input == toLowerCase(alias)) return dir.name;
				}
			}

			// else return empty string
			return "";
		}

		// method to check invalid direction
		bool isValidDirection(const std::string &command) {
			return !getDirection(command).empty();
		}

};

#endif // !DIRECTIONS_H



