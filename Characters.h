//: Characters.h

 /*
 Title: Characters.h
 Description: Character class and MainCharacter class for Wonderland game
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
 	Implements the Characters class and MainCharacter class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled

 Classes:
	Characters class
    MainCharacter class

 Variables:
 	member properties of Characters- name, location;
    member properties of MainCharacter - name, alice_size, inventory, *current_loc, player_lives, points, player_state

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to interact with available characters in specified locations.

 Edge case (character files)
    >If the characters file characters.txt does not load
    > Expected: An appropriate message sould be displayed to the player.

 
 Discussion:
 
*/

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "Locations.h"					// required for locations
#include "Actions.h"
#include "Utilities.h"					// required for main character's current location
#include "Inventory.h"					// for inventory

#include <iostream>
#include <string>
#include <vector>
#include <sstream>					// required for stringstream
#include <algorithm>					// required for std::min() in resizeToNormal()


/* Characters Class */
class Characters {

	public:

		/* character structure */
		struct Character {
			
			std::string name;
			std::string location;

            Character(const std::string &n, const std::string &l)
                : name(n), location(l) {}
		};
	
	private:

		// vector of characters of type Character
		std::vector<Character> characters;

	protected:

	public:
	
		Characters() {
		
			loadCharacters("game_files/characters.txt");
		}

		bool loadCharacters(const std::string &filename) {

			// open character.txt file for reeading
			std::ifstream file(filename);

			if (!file.is_open()) {
				std::cerr << "Could not open character file " << filename << "\n";
				
				return false;
			}

			// clear entries before loading file again
			characters.clear();

			std::string line;				// hold one line from the text file

			// process file one line at a time
			while (std::getline(file, line)) {

				// ignore blank lines
				if (trim(line).empty()) continue;
				// use stringstream to split lines into individual fields 
				std::stringstream ss(line);

                std::string name, location;

				// read the four fields separated by the | character
                if (!std::getline(ss, name, '|') ||
                        !std::getline(ss, location, '|')) {

					// skip a line if one or more required fields are missing
					std::cerr << "Skipping invalid character: " << line << "\n";
					continue;
				}

				// remove unnecessary white spaces between field
				name = trim(name);
				location = trim(location);

				// create a character object and store it in the vector
                characters.emplace_back(name, location);
			}

			// print how many characters were loaded
			std::cout << "Loaded: " << characters.size() << "character(s).\n";

			file.close();

			return true;
					
		}

		/* method to get character by name */
		Character *getCharacterByName(const std::string &name) {

			for (Character &character : characters) {

				if (toLowerCase(character.name) == toLowerCase(trim(name))) return &character;
			}

			return nullptr;
		}

		// check if a character cna be found at a location
		bool isCharacterAtLocation(const std::string &char_name, const std::string &loc_name) {

			Character *character = getCharacterByName(char_name);			// find the requested character

			if (character == nullptr) return false;

			return toLowerCase(character->location) == toLowerCase(loc_name);
		}

		// method to display all loaded characters and their fields
		void displayCharacters() {

			for (const Character &character : characters) {
                std::cout << character.name
                    << " | " << character.location
                    << "\n";
			}
		}
};

/* a subclass of Characters that defines the Main Character and her attributes */
struct MainCharacter {

	enum PlayerState { ALIVE, PAUSED, PERISHED, QUIT, WON };

	std::string name;
	int alice_size;			// determines Alice's current size... item_effect can cause size to increase or decrease
	Inventory inventory;			// stores the items main character currently carries
	Location *current_loc = nullptr;
	int player_lives;				// will be set to 3
	int points;					// this is player's move points: as the player moves from a location to the other, they gather one point for each move
	PlayerState player_state;
	
	MainCharacter() : name("Alice"), alice_size(0), player_lives(3), player_state(ALIVE) {
	}

	/* set methods */
	void setMCCurrentLocation(Location *loc) {
		current_loc = loc;
	}
    

	/* get methods */
	std::string getMCName() {
		return name;
	}

	// returns player's move points
	int getPoints() {
		return points;
	}

	int getMCSize() {
		return alice_size;
	}

	Location *getMCCurrentLocation() const {
		return current_loc;
	}
	// non-const
	Location *&getMCCurrentLocation() {
		return current_loc;
	}


	PlayerState getPlayerState() {
		return player_state;
	}

	// resize Alice's size using coins
	bool resizeToNormal() {
		const int resize_cost = 2;
		int coins_available = 0;

		// get avaliable coin
		for (const Item &item : inventory.getItemsRef()) {
			if (toLowerCase(item.name) == "gems" && item.item_location == "Inventory") {
				coins_available += item.count;
			}
		}

		if (coins_available < resize_cost) {
			std::cout << "\nSorry, you don't have enough gems to resize yourself: 2 gems needed.\n";
			return false;
		}

		// make sure player isn't resizing at normal size
		if (alice_size == 0) {
			std::cout << "\nYou're already at normal size.\n";
			return false;
		}

		// spend exactly two coins
		int remainder = resize_cost;

		for (Item &item : inventory.getItemsRef()) {
			if (toLowerCase(item.name) == "gems" && item.item_location == "Inventory") {
				int used = std::min(item.count, remainder);
				item.count -= used;
				remainder -= used;

				if (remainder == 0) break;
			}
		}

		alice_size = 0;

		std::cout << "\nYay! You're now at normal size after spending 2 gems to resize.\n";
		return true;

	}

};

#endif // !CHARACTERS_H

