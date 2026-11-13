//: Actions.h

 /*
 Title: Actions.h
 Description: Actions class for Wonderland game
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the Actions class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Classes:
	Actions - 

 Variables:
 	member properties - vector of type string named actions
 
 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to progress and interact in the game using available actions.

 Bad Data case 1 (invalid action/ command)
    >If player attempts to use an action/command that's not supported by the game or in their current state/ location
    >Expected:The game should reject, handle it and display an appropriate message to player without affecting the game state.
 
 Edge case (action)
    >If player attempts an action that's in action_words.txt but not implemented in control yet
    >Expected:An appropriate output is displayed indicating the action hasn't been implemented yet
 
 Discussion:
 
*/


#ifndef ACTIONS_H
#define ACTIONS_H

#include "Utilities.h"


#include <iostream>
#include <string>

#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>

/* Actions Class */
struct Actions {

	private:
		std::vector<std::string> action_words;

	public:

		Actions() {

			// Load action_word.txt
			loadActionWords("game_files/action_words.txt");
		}

		// load action words method
		bool loadActionWords(const std::string &filename) {

			std::ifstream file(filename);

			if (!file.is_open()) {
				std::cerr << "Could not load action words.\n";
				return false;
			}

			action_words.clear();

			std::string word;

			while (std::getline(file, word)) {

				// remove leading and trailing whitespace
				std::size_t first = word.find_first_not_of(" \t\r\n") ;

				// skip the line if it's just empty/white spaces
                if (first == std::string::npos) {
					continue;
				}

				std::size_t last = word.find_last_not_of(" \t\r\n");

				word = word.substr(first, last - first + 1);

				// store action words in lowercase.
				word = toLowerCase(word);

				action_words.push_back(word);
			}

			file.close();

			return true;
		}

		/* Check whether an action word exists in vector */
		bool isValidAction(const std::string &action) const {

			std::string lower_cased = toLowerCase(action);

            // return true if ther's a match, else, return false
			return std::find(action_words.begin(), action_words.end(), lower_cased) != action_words.end();
		}

		/* Extract the action word from a complete command */
		std::string getAction(const std::string &command) const {

			std::string lower_cased = toLowerCase(command);

			// remove leading whitespace
			std::size_t first = lower_cased.find_first_not_of(" \t\r\n");

			if (first == std::string::npos) {

				return "";
			}

			lower_cased.erase(0, first);

			// extract the first word
			std::size_t space = lower_cased.find_first_of(" \t\r\n");

			if (space == std::string::npos) {

				return lower_cased;
			}

			return lower_cased.substr(0, space);
		}

		/* Extract the argument after the action word */
		std::string getArgument(const std::string &command) const {

			std::size_t first = command.find_first_not_of(" \t\r\n");

			if (first == std::string::npos) {
				return "";
			}

			std::size_t end = command.find_first_of(" \t\r\n", first);

			if (end == std::string::npos) {
				return "";
			}

			std::size_t argument_start = command.find_first_not_of(" \\t\r\n", end);

			if (argument_start == std::string::npos) {
				return "";
			}

			return command.substr(argument_start);
		}

		/* Display all valid action words */
		void displayActions() const {

			std::cout << "Valid actions: \n";

			for (const std::string &action : action_words) {
				std::cout << "- " << action << "\n";
			}	
		}

		void displayHelp() const {

			std::cout << "\nRemember: In this game, Alice has a goal - get to Wonderland.\n"
					<< "This game world is a journey to Wonderland. All locations are path in/into Wonderland\n\n"
					<< "You could commands such as: \n"
					<< "go east |	east	|	e	|	forward		|	front	\n"
					<< "move south |    south    |    s    |    downward    |    down\n"
                    << "go north |    north    |    n    |    upward    |    up\n"
                    << "run west |    west    |    w    |    backward    |    back\n"
					<< "eat / drink - you consume an item	\n"
					<< "carry/ take - you take an item\n"
					<< "drop	- you drop an item\n"
					<< "escape	- you use you precious stone by entering 'escape'\n"
					<< "follow	- follow a character\n"
					<< "look	- this command shows you details of where you're at\n"
					<< "cook	- you cook something\n"
					<< "talk	- you speak to a character\n"
					<< "riddle/card/deal	- you ask for a riddle card in specific locations\n"
					<< "Joke	- you ask for a joke in a specific location\n"
                    << "size    - you check your current size\n"
					<< "resize	- you go back to normal size (costs you 2 gems)\n"
					<< "quit/ exit	- you quit the game (notice that in this game, exit does not mean exit a location, it will quit the game)\n"
					<< "cry		- you will cry\n"
					<< "pause 	- you pause the game\n"
					<< "help/h	- you get a list of commands\n\n"
					<< "Note: \n"
					<< "Enter responses using words, not numerical digits. If the game asks for a number, spell it out in words. For example, enter 'three' instead of '3'.\n"
					<< "Use commands such as take or take gem, carry, and drop to interact with items... watch out for some items that require you to provide their full name to interact with them.\n"
					<< "Use inventory, invent, or i to check your inventory.\n"
					<< "Gem: you can use two gems to resize\n"
					<< "Precious stone: you can use one precious stone to escape into the play room\n\n";

			return;
		}
};


#endif // !ACTIONS_H



