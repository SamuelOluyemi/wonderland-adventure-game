//: Wonderland.h

 /*
 Title: Wonderland.h
 Description: Wonderland game main class
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
 	Implements the entry point to the Wonderland game program world.

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Notes: The source code includes Documentation and Test cases in main() while the implementations are found in the header files.
 	The class implementations are found in their respective header files (named accordingly) in the Wonderland directory
 
 Classes:
 	Wonderland - the main class
	
	The following classes were implemented in separate header files but part of classes that constitute the game:
	Control, Locations, Items, Characters, MainCharacter(in Characters.h), Actions, Inventory, Directions, RiddlesAndJokes.
    Then, a Utilities header file for useful methods across the program. It also contains some important functions called in Wonderland class such as the displayWelcomeGreeting(), displayInstructions() which are essential to the game as a whole.

 Variables:
 	member properties of Wonderland - is_running, score, game_state, *start_loc, *end_Loc, *player_current_loc
    member classes - control, locations, items, characters, actions, directions, main_character, riddles_and_jokes

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
     >Once game program is launched, user should be met with a welcome message. Then, asked if they want to display instructions for the game.
     >If user, enters yes, they're shown the instructions, otherwise, they are asked if they want to start the game right away.
     >At the start of the game, the start point should be at the Field.
     >When game starts, player should be able to follow instructions and clues display at each location.
     >Player should be able to move between locations using the available directions.
     >Player should be able to use available actions, interact with characters, and collect available items.
     >Player should be able to answer riddles where required.
     >Player should be able to request riddles(with answers displayed automatically) and jokes in the Play room(as a safe room).
     >Player should be able to progress through the game until the ending conditon is reached.
     >The final destination(location) should be Wonderland. At Wonderland, player is asked if they want to play the game again or not.
     >If player opts to play again, they are taken to the start point, their inventory and other game-location components are reset, except their score(which indicates their how many locations they've visited).
    > game state should update and synchronize with player state when necessary to produce expected result.

 Bad Data case (invalid input)
    >If player attempts to use an unavailable input
    >Expected:The game should handle it appropriately
 
 Edge case (game completion)
    >If player satisfies the game's completion condition (by reaching the final destination).
    >Expected: The game should recognize completion, output the appropriate message, and prompt the player to choose to play again or not. In the case where they choose to play again, player should be taken to the start point and restart the game. Otherwise, the game should end correctly.
 
 
 Discussion:
     The Wonderland game program implements a text-based game in whioch the player interacts with locations, charcters, items through available actions/ commands.
    Note that if a Utility function is called, which is the case in most of the program with the use of functions like toLowerCase(), getRandom(), displayInstructions() at the start of the game, displayWelcomeGreeting(), this might cause the program to fail to compile. The Utilities header file is a potential error surface for this game because its function uses cut across the entire game.
    Testing involves verifying that movement between locations works as expected; that player's interactions with characters and items yield expected results; that inventory and items update as player interacts and progresses in the game; that player's score updates as they visit different locations; that player is rewarded for answering riddles correctly according to the game rules;
    Invalid inputs are handled appropriately without crashing or corrupting the game.
    More tests involves examining the riddles, item interactions, and game's completion conditon, helping to verify that the individual classes work and are connected correctly through Wonderland class, the control class and the MainCharacter class.
    More testing involves presenting the game to a real-world user to play the game without influence or knowedge of the game's map. They played freely, tested edge cases, identified subtle bugs, provided feedback, and these were all considered and correction/ adoptions were made where necessary.
 
*/


#ifndef WONDERLAND_H
#define WONDERLAND_H

/* Classes for the game */
#include "Utilities.h"
#include "Locations.h"
#include "Items.h"
#include "Characters.h"
#include "Actions.h"
#include "Control.h"
#include "RiddlesAndJokes.h"
#include "Directions.h"

#include <iostream>
#include <string>
#include <vector>
#include <limits>

/* Wonderland class */
class Wonderland {
    
	private:

		/* game classes */
		/* Initialize all the necessary classes that make up the game world */
		Locations locations;
		Items items;
		Characters characters;
		Actions actions;
		Directions directions;
		MainCharacter main_character;
		RiddlesAndJokes riddles_and_jokes;
		Control control;					// refernces the objects above

        /* enum structure to set player state */
        enum GameState { START, RUNNING, PAUSED, GAMEOVER, QUIT, WON };

        bool is_running;        // track if game is running or not
        int score;            // keep game score
        GameState game_state;        // keep game's state

        // Start, current and end locations
        Location *start_loc = nullptr;
        Location *end_loc = nullptr;
        Location *player_current_loc = nullptr;

	protected:

		/* Method to get game state
		 returns the state of the game and appropriate action
		 */
		void getGameState() {
			
			// if game state is START, display the welcome greeeting
			if (game_state == START) {
				
				// greet player to start
				displayWelcomeGreeting();               // found in utilities file
				return;
			}

			// if game state is RUNNING, is_running is set to true
			if (game_state == RUNNING) {
				
				is_running = true;
			}

			// if game state is PAUSED, invoke pause() from control class, and set state is_running to false
			if (game_state == PAUSED) {
				
				control.pause();				// called from Control class after player_state is changed to PAUSED

				// change game state just after the game resumes
				game_state = RUNNING;
				if (player_current_loc != nullptr) {
					locations.enterLocation(player_current_loc->name);
                    main_character.player_state = MainCharacter::ALIVE;
				}
                // clear the newline left behind by std::cin >> command
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			}

			// if game state is GAMEOVER, is_running is set to false, final score is displayed
			if (game_state == GAMEOVER) {
				
                std::cout << "\nFinal score: " << getScore() << "\n";
                control.displayFinalInventory();
                is_running = false;
				return;
			}

			// if game_state is QUIT, is_running is set to false, final score is displayed
			if (game_state == QUIT) {

				std::cout << "You've opted to exit the game.\n"
                    << "Final score: " << getScore() << "\n";
                control.displayFinalInventory();
                std::cout << "\nWe would love to see you again. Bye "<< main_character.name << "👋\n";
				is_running = false;
				return;
			}
			// if game state is WON (this state is only attained when player gets to Wonderland),
			// is_running is set to false, and user is asked if they want to keep playing
			// if yes, game state is set to RUNNING, else, final score is displayed and game is exited
			if (game_state == WON) {


				//is_running = false;				// set is_running to false

				// Ask if user wants to keep playing
				std::string answer;
				std::cout << "\nCongratulations! You have won!\n"
                    << "Final score: " << getScore() << "\n";
                control.displayFinalInventory();
                std::cout << "\nDo you want to play again? Enter Y/N: \n";
				std::cin >> answer;
				if (answer == "y" || answer == "yes") {
					//game_state = RUNNING;
					
					// reset locations visit_count
					locations.resetVisitCounts();
                    // reinitialize Alice at the starting location
                    player_current_loc = start_loc;
                    control.resetGameWorld();           // resets items, inventory, and player's current location to field

                    if (player_current_loc != nullptr) {
                        locations.enterLocation(player_current_loc->name);
                        items.displayItemsAtLocation(player_current_loc->name);
                    }

					game_state = RUNNING;
				}
				else {
					std::cout << "You've opted to exit the gmae.\n"
						<< "We would love to see you again 👋 \n\n";
					is_running = false;
					return;
				}
				
				// clear the newline left behind by std::cin >> command
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			}
		}
	public:
		/* Control class is used to reference the other classes.
            control is called in Wonderland constructor, through which the other class linked to the control that will be used to control the game in general. Control itself uses referencing to get those classes in its own constructor, so it links back to the Wonderland game world
         */
        Wonderland()
			: control(actions, directions, locations, items, main_character, riddles_and_jokes),
				is_running(false), score(0), game_state(START) {
			
			start_loc = locations.getStartLocation();
			end_loc = locations.getEndLocation();

			// initialize main character's location to start location pointer
			main_character.setMCCurrentLocation(start_loc);
			player_current_loc = main_character.getMCCurrentLocation();

			// get game state to start
			getGameState();
				
		}
		
		static int main() {
			
			Wonderland wonderland;
			wonderland.play();

			return 0;
		}

		/* Method to get current game score */
		int getScore() {
			score = main_character.getPoints();
			return score;
		}

		/* Method to get start command */
		void getStartCommand() {

			displayInstructions();				// found in Utilities file
            
			std::string command;				// user command

			// ask if user wants to start playing
			std::cout << "\nEnter 'start' or 's' to begin the game: \n";
			std::cin >> command;

			// clear the newline left behind by std::cin >> command
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			toLowerCase(command);

			if (command == "start" || command == "s") clearScreen();
			else {
				std::cerr << "Invalid input.\n";
				std::cout << "You can use 'Ctrl + C' to exit the program at any time\n";
				getStartCommand();
			}
			return;
		}
    
		/* Method to get User command */
		std::string getUserCommand() {
			
			std::string cmd;

			// if location is Dreamland (location before arriving in Wonderland)
            // we change how we ask player for command as an hint that they're almost there
			if (player_current_loc == locations.getLocationByIndex(locations.getLocationsSize()-2)) {
				std::cout << "Now what? \n";
			}
			else {
				std::cout << "What now? ";
			}
			std::getline(std::cin, cmd);

			toLowerCase(cmd);			// to lowere case

			return cmd;
		}

		/* Method to play game */
		void play() {

			getStartCommand();

			//bool is_running = true;
			game_state = RUNNING;
			getGameState();

			// Initialize Alice at the starting location
			player_current_loc = start_loc;
			main_character.setMCCurrentLocation(start_loc);

			if (player_current_loc != nullptr) {
				locations.enterLocation(player_current_loc->name);
				items.displayItemsAtLocation(player_current_loc->name);
			}

			while(is_running) {

				/* Command Logic Section */
				std::string command = getUserCommand();

				if (command.empty()) continue;
				
				/* Read user input and call appropriate function */
				control.readInput(command);

				/* Pause Section */
				if (main_character.player_state == MainCharacter::PAUSED) {

					// change game state just before the game pauses
					game_state = PAUSED;
					getGameState();				// calls the control.pause() method
				}

				/* Quit section: set game state to QUIT and call getGameState() if player's state is QUIT */
				if (main_character.player_state == MainCharacter::QUIT) {

					game_state = QUIT;
					getGameState();
					return;
				}

				/* Game over section:
                 if game state is GAMEOVER, calls getGameState() to act appropriately
                 */
				if (main_character.player_state == MainCharacter::PERISHED) {
				
					game_state = GAMEOVER;
					getGameState();
					return;
				}

				// keep wonderland's location pointer synched with Alice's
				player_current_loc = main_character.getMCCurrentLocation();

				/* Wonderland Arrival Section */
				if (player_current_loc != nullptr && player_current_loc == end_loc) {
					game_state = WON;
					getGameState();
				}

			}
		}
};


#endif // !WONDERLAND_H

