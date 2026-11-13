//: Control.h

 /*
 Title: Control.h
 Description: Control class for Wonderland game
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the Control class of the Wonderland Adventure Game. Uses all other classes to control the game and send them to Wonderland class which is the entry into the program.

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Classes:
	Control class-

 Variables:
 	member properties of Control - actions, directions, locations, items, characters, main_character, riddles_and_jokes
    member property flags - golden_door_unlocked, garden_box_open, has_cooked, has_spoken_rabbit, has_spoken_party, has_drunk_tea, card_coin_flipped, tears_cried, book_found, book_opened, barn_unlocked, has_taken_gloves, has_worn_gloves, game_riddle_answered, king_riddle_answered, queen_riddle_answered, waking_up

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >Player should be able to use implemented and available control with actions to produce expected results
    >Player should be able to move between locations using the available directions.
    >Player should be able to use available actions, interact with characters, and collect available items.
    >Player should be able to answer riddles where required.
    >Player should be able to request riddles(with answers displayed automatically) and jokes in the Play room(as a safe room).
    >Player should be able to progress through the game until the ending conditon is reached.
    >All public methods should be calleable
    

 Bad Data case (invalid action/ command)
    >If player attempts to use an action/command that's not supported by the game or in their current state/ location
    >Expected:The game should reject, handle it and display an appropriate message to player without affecting the game state.
 
 Discussion:
    
    The class reads user input and checks the input against valid actions. If an action is allowed, then the methods required to complete the action are called on the objects involved.
    Invalid inputs are handled appropriately without crashing or corrupting the game.
 
*/

#ifndef CONTROL_H
#define CONTROL_H

#include "Items.h"
#include "Actions.h"
#include "Utilities.h"					// for toLowerCase()
#include "Locations.h"					// for move() method from Locations class
#include "Directions.h"					// required for direcitons
#include "Inventory.h"
#include "Characters.h"
#include "RiddlesAndJokes.h"				// required for riddles and jokes

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
/* for pause control */
#include <algorithm>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <limits>

/* Control Class */
class Control {

	private:

		/* These refernces let Control operate on actual game objects */
		Actions &actions;				// actions to use with control
		Directions &directions;
		Locations &locations;
		Items &items;
		//Characters &characters;
		MainCharacter &main_character;
		RiddlesAndJokes &riddles_and_jokes;

		// flags to track player progress and requirements to advance
		bool golden_door_unlocked = false;
		bool garden_box_open = false;
		bool has_cooked = false;
        	bool has_spoken_mushroom = false;
        	bool has_spoken_rabbit = false;
        	bool has_spoken_party = false;
        	bool has_drunk_tea = false;
		bool card_coin_flipped = false;
		bool tears_cried = false;
        	bool book_found = false;
        	bool book_opened = false;
		bool barn_unlocked = false;
        	bool has_taken_gloves = false;
        	bool has_worn_gloves = false;
		bool game_riddle_answered = false;
		bool king_riddle_answered = false;
		bool queen_riddle_answered = false;
		bool waking_up = false;


	public:

//		Control(Actions &a, Directions &d, Locations &l, Items &i, Characters &c, MainCharacter &mc, RiddlesAndJokes &rj) 
//			: actions(a), directions(d), locations(l), items(i), characters(c), main_character(mc), riddles_and_jokes(rj) {
//				
//				// load all riddles and jokes from the start of the game
//				riddles_and_jokes.loadAll();
//			}
    Control(Actions &a, Directions &d, Locations &l, Items &i, MainCharacter &mc, RiddlesAndJokes &rj)
        : actions(a), directions(d), locations(l), items(i), main_character(mc), riddles_and_jokes(rj) {
            
            // load all riddles and jokes from the start of the game
            riddles_and_jokes.loadAll();
        }


		/* Method to make sure Alice can move:
		 * in the case where she needs to possess an item, or use an item, or do something
		 */
		bool canAdvance(const std::string &direction) {
			Location *current_loc = main_character.getMCCurrentLocation();

			if (current_loc == nullptr) return false;

			std::string dir = toLowerCase(direction);
			std::string destination;

			if (dir == "north") destination = current_loc->n_destination;
			else if (dir == "south") destination = current_loc->s_destination;
			else if (dir == "west") destination = current_loc->w_destination;
			else if (dir == "east") destination = current_loc->e_destination;

			std::string loc = toLowerCase(current_loc->name);
			std::string dest = toLowerCase(destination);

			/* Handle when player is already in location */
            		// can't enter the garden without unlocking its door
			if (loc == "the hall of doors" && dest == "the garden" && !golden_door_unlocked) {
				std::cout << "\nThe door is locked. You must use the golden key and be at least your normal size!\n";
				return false;
			}
            		if (loc == "the hall of doors" && dest == "the garden" && main_character.alice_size > 0) {
                		std::cout << "\nYou must be at your normal size or smaller to enter the garden!\n";
                		return false;
            		}

			// can't leave the garden until all requirements are met
			if (loc == "the garden" && !garden_box_open) {
				std::cout << "\nYou must open the box, take the gold coin, and flip it before you can advance from the Garden.\n";
				return false;
			}
            		if (loc == "the garden" && !card_coin_flipped) {
                		std::cout << "\nYou must flip the gold coin before you can advance from the Garden.\n";
                		return false;
           		 }
            
            		// can't go to pool of tears until tears_cried flag is true
            		if (dest == "the pool of tears" && !tears_cried) {
                		std::cout << "\nYou must have cried before tears can flow creating a pool of tears.\n";
                		return false;
            		}
            
            		// can't leave farmhouse kitchen until player cooks
            		if (loc == "the farmhouse kitchen" && !has_cooked) {
                		std::cout << "\nYou must cook before leaving the farmhouse kitchen... it's then your choice to eat or not.\n";
                		return false;
            		}
            
            		// can't leave the mushroom field, the rabbit's house, 
			// and the mad hatter's party without talking. can't leave tea party without drinking tea
            		if (loc == "the mad hatter's tea party" && !has_drunk_tea) {
                		std::cout << "\nWhat's a tea party without drinking some tea 🫖!\n";
                		return false;
            		}
            		if (loc == "the mad hatter's tea party" && !has_spoken_party) {
                		std::cout << "\nTry talking to them... you never know...\n";
                		return false;
            		}
            
            		if (loc == "the mushroom field" && !has_spoken_mushroom) {
                		std::cout << "\nTry talking to the Caterpiller... you never know...\n";
               			 return false;
            		}
            		if (loc == "the rabbit's house" && !has_spoken_rabbit) {
                		std::cout << "\nTry talking to the Rabbit... you never know...\n";
                		return false;
            		}
            		if (loc == "the rabbit's house" && !has_taken_gloves) {
                		std::cout << "\nYou should take the rabbit's gloves first\n";
                		resetGlovesItem();              // reset gloves items after there's an attempt to leave
                		return false;
            		}
            		if (loc == "the rabbit's house" && !has_worn_gloves) {
                		std::cout << "\nYou should wear the rabbit's gloves...\n"
				       << "They will wear out once you attempt to leave the house or after you leave the rabbit's house\n";
                		return false;
            		}

			// can't leave the animal game until you answer the game riddle correctly
            		if (loc == "the animal game" && !game_riddle_answered) {
                		std::cout << "\nThe Cheshire cat says 'Huh?'\n"
                    		<< "It seems you must play or run the game to advance.\n";
                		return false;
            		}

			// can't leave the king's presence until you answer the king's riddle correctly
            		if (loc == "the king's presence" && !king_riddle_answered) {
                		std::cout << "\nYou must answer the King of Heart's question to advance.\n";
                		return false;
            		}
			
			// can't leave the queen's presence until you answer the queen's riddle correctly, else you lose life
            		if (loc == "the queen's presence" && !king_riddle_answered) {
                		std::cout << "\nYou must answer the Queen of Heart's question to advance.\n";
                		return false;
            		}

			// dreamland restriction
			if (loc == "dreamland") {
				if (dir == "north" && !waking_up) {
					std::cout << "\nDreamland is fun, waking to reality is almost here.\n";
					return false;
				}
			}
			return true;
		}

		/* Method to process player commands 
		 	Each function called at eavery input is implemented below readInput()
		 * */
		void readInput(const std::string &command) {

                    std::string action = actions.getAction(command);
                    std::string argument = actions.getArgument(command);

                    // Movement: "go north", "move n", "go upward", etc.
                    if (action == "go" || action == "move" || action == "run") {
                        move(argument);
                        return;
                    }
                    
                    // Accept simple direction command
                    if (directions.isValidDirection(command)) {
                        move2(command);
                        return;
                    }
            
                    if (action == "follow") {
                        follow();
                        return;
                    }

                    // Look at Alice's current location and the items there
                    if (action == "look") {
                        look();
                        return;
                    }

                    // Take an item from the current location
                    if (action == "take" || action == "carry"|| action == "grab" || action == "collect") {
                        take(argument);
                        return;
                    }
            
                    // wear an item from the current location
                    if (action == "wear" || action == "put on") {
                        wear(argument);
                        return;
                    }

                    // Open an item from the current location
                    if (action == "open" || action == "unlock") {
                        open(argument);
                        return;
                    }
            
                    // to cook in Farmhouse
                    if (action == "cook") {
                        cook(argument);
                        return;
                    }
                    
                    // For consumption of items
                    if (action == "eat" || action == "drink" || action == "consume") {
                        consume(argument);
                        return;
                    }

                    // for crying
                    if (action == "cry") {
                        cry();
                        return;
                    }
            
                    // Drop an item from Alice's inventory at the current location
                    if (action == "drop") {
                        drop(argument);
                    }
            
                    // if player is in barn and type transsport to leave
                    if (action == "transport") {
                        leaveBarn();
                        return;
                    }
            
                    // if player wants to talk
                    if (action == "speak" || action == "talk" || action == "say" || action == "respond") {
                        speak();
                        return;
                    }

                    // if player want to resize to normal size
                    if (action == "resize") {
                        resize();
                        return;
                    }

                    // player flips coin in the garden
                    if (action == "toss coin" || action == "flip coin" || action == "flip" || action == "toss") {
                        flipCoin();
                        return;
                    }
                    // player wants to get riddle in play room
                    if (action == "riddle" || action == "card" || action == "deal") {
                        riddle();
                        return;
                    }
            
                    // player wants jokes in Play room
                    if (action == "joke" || action == "jokes" || action == "fun") {
                        getJoke();
                        return;
                    }
                    
                    // player wants to participate in animal game
                    if (action == "play" || action == "card" || action == "deal") {
                        Location *current_loc = main_character.getMCCurrentLocation();
                        
                        if (current_loc != nullptr && toLowerCase(current_loc->name) == "the animal game") {
                            gameRiddle();
                        }
                        else {
                            std::cout << "\nThere is nothing here to play or deal in.\n";
                        }
                        return;
                    }
                    
                    // wake up from dreamland
                    if (action == "wake" || action == "wake up") {
                        wake();
                        return;
                    }

                    // Display inventory
                    if (action == "inventory" || action == "invent" || action == "i") {
                        inventory();
                        return;
                    }

                    // Display available actions: get help
                    if (action == "help" || action == "h") {
                        help();
                        return;
                    }
            
                    // use precious stone to go to play room
                    if (action == "escape") {
                        usePreciousStone();
                        return;
                    }

                    // check player size
                    if (action == "size") {
                                getSize();
                                return;
                        }
                    // scores
                    if (action == "score") {
                                score();
                                return;
                        }
            
                    if (action == "lives" || action == "live") {
                        checkLivesLeft();
                        return;
                    }
            
                    // player wants to pause
                    if (action == "pause") {
                        
                        // change the player's state and allow Wonderland class handle calling the control.pause() method
                        main_character.player_state = MainCharacter::PAUSED;
                        return;
                    }
            
                    // player wants to quit or exit
                    if (action == "quit" || action == "q" || action == "exit") {
                        quit();
                        return;
                    }

                    // Check command efficiency
                    if (!actions.isValidAction(action)) {
                        std::cout << "\nInvalid command. Type 'help' for assistance.\n";
                    }
                    else {
                        std::cout << "\nThat action hasn't been implemented yet.\n";
                    }
		}

        
        /* FUNCTIONS THAT ARE CALLED IN readInput() */

        /* Pause method */
        void pause() {
            std::string command;

            std::cout << "\nGame paused.\n";
            std::cout << "Type 'play' to resume the game.\n";

            while (std::cin >> command) {
                toLowerCase(command);

                if (command == "play") {
                    std::cout << "\nResuming game...\n";
                    return;
                }
                
                std::cout << "\nGame is paused. Type 'play' to resume.\n";

            }
        }


        /* Move control : go, move, run  */
        void move(const std::string &argument) {
            std::string direction = directions.getDirection(argument);

            if (direction.empty()) {
                std::cout << "\nInvalid direction. Try north, south, east, or west.\n";
                return;
            }

            // get Alice's actual location pointer by reference so it gets updated
            Location *& current_loc = main_character.getMCCurrentLocation();

            if (current_loc == nullptr) {
                std::cout << "\nAlice has no current location.\n";
                return;
            }

            if (!canAdvance(direction)) return;				// check that player can move
            locations.move(current_loc, direction);
            
            // reset some flags after player has moved and increase their points
            resetFlags();
            main_character.points++;
            return;
        }
        // move with just direction
        void move2(const std::string &command) {
            
            // get Alice's current location and use move() method to move to the direction provided
            Location *& current_loc = main_character.getMCCurrentLocation();

            if (current_loc == nullptr) {
                std::cout << "\nYou aren't at a valid location.\n";
            return;
            }
            std::string direction = directions.getDirection(command);

            if (direction.empty()) {
                std::cout << "\nInvalid direction. Try north, south, east, or west.\n";
                return;
            }

            if (!canAdvance(direction)) return;				// check player can move
            locations.move(current_loc, direction);
            
            // reset some flags after player moves and increase their points
            resetFlags();
            main_character.points++;
            return;
        }
        
        /* follow control: used only at The Riverbank when player sights Th White rabbit and might be compelled to follow
         It leads into the Rabbit hole, where player descends into rabbit hole
         */
        void follow() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr) {
                std::cout << "\nYou aren't at a valid location.\n";
                return;
            }
            
            if (toLowerCase(current_loc->name) != "the riverbank") {
                std::cout << "\nThere's nothing to follow here.\n";
                return;
            }
            
            sendToLocation("The Rabbit Hole");
            return;
        }
        
        /* LeaveBarn control: to leave the Barn, teh provided input must match the expected answer */
        void leaveBarn() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the barn") {
                std::cout << "\nYou aren't at a location where this is allowed.\n";
                return;
            }
            
            if (!book_opened) {
                std::cout << "\nYou must open the book first!\n";
                return;
            }
            
            // vector of allowed answers at the Barn
            const std::vector<std::string> allowedAnswers = {
                "rabbit", "the rabbit", "a rabbit", "white rabbit", "the white rabbit"
            };
            
            // string to check to user answer is input
            std::cout << "\nWhat did you see at the Riverbank? \n";
            std::string input;
            std::getline(std::cin, input);
            // search the vector to see if the input matches any provided answer
            auto iterator = std::find(allowedAnswers.begin(), allowedAnswers.end(), input);
            
            if (iterator != allowedAnswers.end()) {
                std::cout << "\nCorrect! You are being moved out of the Barn!\n";
                sendToLocation("The Rabbit Hole");
                return;
            }
            else {
                std::cout << "\nThat is incorrect. You must provide the correct answer to leave the Barn!\n";
                return;
            }
        }


        /* Open control : open */
        void open(const std::string &argument) {
            Location *current_loc = main_character.getMCCurrentLocation();

            if (current_loc == nullptr) {
                std::cout << "\nYou aren't at a valid location.\n";
                return;
            }

            std::string target = toLowerCase(trim(argument));

            // if player is in the hall of doors
            if (toLowerCase(current_loc->name) == "the hall of doors") {
                if (target.find("door") == std::string::npos && target.find("key") == std::string::npos) {
                    std::cout << "\nWhat would you like to open?\n";
                    std::getline(std::cin, target);
                    target = trim(target);
                    return;
                }
                if (target.empty()) return;

                if (main_character.inventory.getItemByName("Golden key") == nullptr) {
                    std::cout << "\nYou need the golden key to unlock this door.\n";
                    return;
                }

                // player has golden key, unlock door
                golden_door_unlocked = true;
                std::cout << "\nThe golden key has unlocked the door to the Garden.\n";
                return;
            }

            // if player is in the garden
            if (toLowerCase(current_loc->name) == "the garden") {
                if (target.find("box") == std::string::npos) {
                    std::cout << "\nWhat would you like to open?\n";
                    std::getline(std::cin, target);
                    target = trim(target);
                    return;
                }
                if (target.empty()) return;
                
                // they can open the box
                garden_box_open = true;
                std::cout << "\nYou open the box. Inside, you see a card-shaped coin! I suspect you need to do something with it\n";
                return;
            }
            // if player is in the barn
            if (toLowerCase(current_loc->name) == "the barn") {
                if (target.find("book") == std::string::npos) {
                    std::cout << "\nWhat would you like to open?\n";
                    std::getline(std::cin, target);
                    target = trim(target);
                    return;
                }
                if (target.empty()) return;
                
                if (toLowerCase(current_loc->name) == "the barn" && !book_found) {
                    std::cout << "\nYou must look to find the book first\n";
                    return;
                }
                
                if (book_found) {
                    std::cout << "\nYou open the book. It reads 'The secret to leaving the barn is to type 'transport', so you're transported away'\n";
                    
                    // they can open the box
                    book_opened = true;
                    return;
                }

                return;
            }

            // if player isn't in any of these places
            std::cout << "\nThere is nothing here that you can open that way.\n";
            
        }
        
        /* Look control: */
        void look() {
            Location* current_loc = main_character.getMCCurrentLocation();

            if (current_loc == nullptr) {
                std::cout << "\nYou aren't at a valid location.\n";
                return;
            }

            if (toLowerCase(current_loc->name) == "the barn") {
                items.displayItemsAtLocation(current_loc->name);
                book_found = true;
                std::cout << "\nYou found a book! \n";
                return;
            }
            
            // display location name and description
            std::cout << "\n" << current_loc->name << "\n";
            std::cout << current_loc->desc << "\n";
            
            // show only the items available at current location
            items.displayItemsAtLocation(current_loc->name);

            return;

        }
        

        /* Consumption control : eat, drink */
        void consume(const std::string &argument) {
            std::string item_name = trim(argument);
            
            if (item_name.empty()) {
                std::cout << "\nWhat would you like to eat or drink?\n";
                std::getline(std::cin, item_name);
                item_name = trim(item_name);
            }
            
            if (item_name.empty()) return;
            
            // Check whether the item is valid and that it consumable before removing it
            Item *item = main_character.inventory.getItemByName(item_name);
            if (item == nullptr) {
                std::cout << "\nYou don't have that item.\n";
                return;
            }
            
            if (!item->isConsumable()) {
                std::cout << "\nYou cannot eat or drink " << item->name << ".\n";
                return;
            }
            
            // check if player is trying to consume pasta
            if (toLowerCase(item->name) == "egg") {
                if (!has_cooked){
                    std::cout << "\nYou need to cook egg before consuming it.\n";
                    return;
                }
            }
            
            Item consumed("", "", 0, 0, 0);
            
            // remove the item from Alice's inventory
            if (main_character.inventory.removeItem(item_name, 1, consumed)) {
                std::cout << "\nYou consumed " << consumed.name << ".\n";
                if (toLowerCase(consumed.name) == "cup of tea" ){
                    std::cout << "\nYou have drunk a cup of tea, now you feel the need to talk on the table...\n";
                    has_drunk_tea = true;           // has drunk tea flag
                    resetTeaItem();          // reset tea item
                }
                
                // if pasta was consumed, take pasta, pot, plate and spoon from player
                if (toLowerCase(consumed.name) == "egg") {
                    Item removed("", "", 0, 0, 0);
                    
                    // remove them from player's inventory
                    main_character.inventory.removeItem("saucepan", 1, removed);
                    main_character.inventory.removeItem("cutlery", 1, removed);
                    main_character.inventory.removeItem("pepper", 1, removed);
                    
                    // reset them to the location
                    resetCookItems();
                    std::cout << "\nYou have used up the cooking items and returned them.\n";
                }
                
                // item effect on Alice
                if (consumed.item_effect == 1) {
                    main_character.alice_size += 1;
                    std::cout << "\nYou grew larger! Your size is now " << main_character.alice_size << ".\n";
                }
                else if (consumed.item_effect == -1) {
                    main_character.alice_size -= 1;
                    std::cout << "\nYou've shrunk! You size is now " << main_character.alice_size << ".\n";
                }
            }
            return;
            
        }
        /* wear control : gloves */
        void wear(const std::string &argument) {
            std::string item_name = trim(argument);
            
            if (item_name.empty()) {
                std::cout << "\nWhat would you like to wear?\n";
                std::getline(std::cin, item_name);
                item_name = trim(item_name);
            }
            
            if (item_name.empty()) return;
            
            // Check whether the item is valid and that it consumable before removing it
            Item *item = main_character.inventory.getItemByName(item_name);
            if (item == nullptr) {
                std::cout << "\nYou don't have that item.\n";
                return;
            }
            
            // check if player is trying to consume pasta
            if (toLowerCase(item->name) == "gloves") {
                if (!has_taken_gloves){
                    std::cout << "\nYou need to take gloves before wearing them.\n";
                    return;
                }
            }
            
            Item worn("", "", 0, 0, 0);
            
            // remove the item from Alice's inventory
            if (main_character.inventory.removeItem(item_name, 1, worn)) {
                std::cout << "\nYou've worn " << worn.name << ".\n";
                if (toLowerCase(worn.name) == "gloves" ){
                    has_worn_gloves = true;           // has worn gloves flag
                    resetGlovesItem();                  // reset gloves item
                }
            }
            return;
        }

        /* Take control : take, carry, grab */
        void take(const std::string &argument) {
                Location *current_loc = main_character.getMCCurrentLocation();

                if (current_loc == nullptr) {
                    std::cout << "\nYou aren't at a valid location.\n";
                    return;
                }

                int quantity = 1;
                std::string item_name = trim(argument);

                if (item_name.empty()) {
                    std::cout << "\nWhat would you like to take?\n";
                    std::getline(std::cin, item_name);
                    item_name = trim(item_name);
            
                }

                if (item_name.empty()) return;

                // check for the golden key guard
                if (toLowerCase(item_name) == "golden key") {
                    if (toLowerCase(current_loc->name) != "the hall of doors") {
                        std::cout << "\nThe golden key can only be taken at the Hall of doors.\n";
                        return;
                    }

                    if (main_character.alice_size != 0) {
                        std::cout << "\nYou must be normal size to take the golden key.\n";
                        return;
                    }
                }
                // check for the gold coin guard
                if (toLowerCase(item_name) == "card coin" || toLowerCase(item_name) == "coin" || toLowerCase(item_name) == "card-shaped coin") {
                    if (toLowerCase(current_loc->name) != "the garden") {
                        std::cout << "\nThe card-shaped coin can only be taken at the garden.\n";
                        return;
                    }

                    // check that box is open
                    if (!garden_box_open) {
                        std::cout << "\nYou must open the box first!\n";
                        return;
                    }
                }
    
                // check for the gloves guard
                if (toLowerCase(item_name) == "gloves" || toLowerCase(item_name) == "glove") {
                    if (toLowerCase(current_loc->name) != "the rabbit's house") {
                        std::cout << "\nThe gloves can only be taken at the rabbit's house.\n";
                        return;
                    }
                }
                

                Item taken("", "", 0, 0, 0);

                //remove the quantity form the game world and put it in Alice's inventory
                if (items.takeItem(item_name, current_loc->name, quantity, taken)) {
                    main_character.inventory.addItem(taken);
                    if (toLowerCase(taken.name) == "gloves") has_taken_gloves = true;
                    std::cout << "\nYou took " << quantity << " " << taken.name << ".\n";

                }
                return;
                        
        }
                
        /* Drop control: drop */
        void drop(const std::string &argument) {
            Location *current_loc = main_character.getMCCurrentLocation();

            if (current_loc == nullptr) {
                std::cout << "\nYou aren't at a valid location.\n";
                return;
            }

            int quantity = 1;
            std::string item_name = trim(argument);

            if (item_name.empty()) {
                std::cout << "\nWhat would you like to drop? \n";
                std::getline(std::cin, item_name);
                item_name = trim(item_name);

            }
            if (item_name.empty()) return;

            Item dropped("", "", 0, 0, 0);

            // remove the requested quantity before returning it to the game world
            if (main_character.inventory.removeItem(item_name, quantity, dropped)) {

                items.dropItem(dropped, current_loc->name, quantity);
                std::cout << "\nYou dropped " << quantity << " " << dropped.name << ".\n";
            }
            else {
                std::cout << "\nYou don't have enough of that item in your inventory.\n";
            }
            return;
        }
    
        /* flipcoin control: */
        void flipCoin() {
                Location *current_loc = main_character.getMCCurrentLocation();

                // check we're in the graden
                if (current_loc == nullptr || toLowerCase(current_loc->name) != "the garden") {
                    std::cout << "\nYou can only flip the gold coin in the Garden.\n";
                    return;
                }

                // check that box is open
                if (!garden_box_open) {
                    std::cout << "\nYou must open the box first!\n";
                    return;
                }

                // cehck that player has the gold key
                if (main_character.inventory.getItemByName("Card Coin") == nullptr) {
                    std::cout << "\nYou need to take the card coin first.\n";
                    return;
                }

                // if player has gold coin, use Utilities's getRandom to flip
                std::vector<int> card_coin{0, 1};
                int result = getRandom(card_coin);
                card_coin_flipped = true;

                if (result == 0) {
                    std::cout << "\nHeads! You may proceed to the Farmouse kitchen.\n";
                    move2("south");
                    return;
                }
                else {
                    std::cout << "\nTails! You may proceed to the Mad Hatter's tea party.\n";
                    move2("east");
                }
            return;
        }

        /* cook control in the play room */
        void cook(const std::string &argument) {

            // must have cooked to eat
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the farmhouse kitchen"){
                    std::cout << "\nYou can't cook here.\n";
                    return;
            }
            
            if (!main_character.inventory.hasItem("saucepan") ||
                !main_character.inventory.hasItem("cutlery") ||
                !main_character.inventory.hasItem("pepper") ||
                !main_character.inventory.hasItem("egg")) {
                
                std::cout << "\nYou must have saucepan, cutlery, pepper, and egg to cook\n";
                return;
            }
            
            std::string item_name = trim(argument);
            
            if (item_name.empty()) {
                std::cout << "\nWhat would you like to cook?\n";
                std::getline(std::cin, item_name);
                item_name = trim(item_name);
            }
            
            if (item_name.empty()) return;
            
            // Check whether the item is valid and that it consumable and that it's pasta (pasta is the only cookable item in the game) before removing it
            Item *item = main_character.inventory.getItemByName(item_name);
            if (item == nullptr) {
                std::cout << "\nYou don't have that item.\n";
                return;
            }

            if (toLowerCase(item->name) != "egg") {
                std::cout << "\nYou cannot cook " << item->name << ".\n";
                return;
            }
            
            // set the has cooked flag
            std::cout << "\nYou have just cooked egg sauce... Smells really nice but a bit spicy... you might want to eat.\n";
            has_cooked = true;
            return;
        }

        /* cry control */
        void cry() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the hall of doors") {
                std::cout << "\nYou can't cry here.\n";
                return;
            }
            
            std::cout << "\nYou are crying...\n";
            tears_cried = true;
            sendToLocation("The Pool of tears");
            return;
        }

        /* Speak control: speak, talk, say, */
        void speak() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr) {
                    std::cout << "\nYou aren't at a valid location\n";
                    return;
            }
            
            if (toLowerCase(current_loc->name) != "the mushroom field" &&
                 toLowerCase(current_loc->name) != "the rabbit's house" &&
                 toLowerCase(current_loc->name) != "the mad hatter's tea party") {
                    
                    std::cout << "\nThere's no one to talk to here\n";
                    return;
            }
            
            // check speaking at the mushroom field
            if (toLowerCase(current_loc->name) == "the mushroom field") {
                std::cout << "\nThe Caterpillar keeps asking who you are but you can't answer clearly. \n"
                    << "You turn away and must leave... \n"
                << "Just before you leave, the caterpillar gives you a piece of advise 'Keep your temper' and offers you two gems.\n";
                has_spoken_mushroom = true;
                return;
            }
            
            // check speaking at the rabbit's house
            if (toLowerCase(current_loc->name) == "the rabbit's house") {
                std::cout << "\nYou talk to the rabbit but it just ran off... but you sight the rabbit's little gloves up for grabs. \n"
                    << "Over to the other side is a garden but impossible to reach from here... \n"
                << "There's surely something interesting going on down the road and straight ahead too...\n";
                has_spoken_rabbit = true;
                return;
            }
            
            // check speaking at the mad hatter's tea party
            if (toLowerCase(current_loc->name) == "the mad hatter's tea party") {
                std::cout << "\nAfter your response, none of them seem to be interested in listening. \n"
                    << "They simply want to talk. You feel the need to leave, there's a road leading off to the edge of a lawn. \n"
                << "North, a curious group of animals are assembled.\n";
                has_spoken_party = true;
                return;
            }
            return;
        }
        
        /* Resize control: */
        void resize() {
            main_character.resizeToNormal();
            return;
        }
        
        /* getJoke control in the play room */
        void getJoke() {
                Location *current_loc = main_character.getMCCurrentLocation();

                if (current_loc == nullptr || toLowerCase(current_loc->name) != "the play room") {
                    std::cout << "\nYou can only hear dad jokes in the Play room.\n";
                    return;
                }

                std::cout << "\nThe Duchess shows you a joke card...😀\n";
                riddles_and_jokes.displayRandomJoke();
            
        }

        /* playroomRiddle method: get a game riddle */
        void playroomRiddle() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the play room") {
                std::cout << "\nYou can only ask for these riddles in the play room.\n";
                return;
            }
            
            std::cout << "\nThe Duchess shows you a riddle card...🤔\n";
            Riddle r = riddles_and_jokes.getRandomPlayRiddle();
            std::cout << r.question << "\n";
            std::cout << r.answer << "\n";
        }
        
        /* helper functions to get player's answer to riddles */
        std::string askAnswer(const std::string &question) {
            
            std::cout << question << "\nYour answer: ";
            
            std::string answer;
            std::getline(std::cin, answer);
            
            return toLowerCase(trim(answer));
        }
        
        /* check correctness of riddle answer */
        bool isCorrect(const std::string &answer, const std::string &expected) {
            return toLowerCase(trim(answer)) == toLowerCase(trim(expected));
        }
        
        /* send player to next location based on response result */
        void sendToLocation(const std::string &name) {
            Location *destination = locations.findLocation(name);
            
            if (destination == nullptr) {
                std::cout << "\nLocation not found: " << name << "\n";
                return;
            }
            
            // move player
            main_character.setMCCurrentLocation(destination);
            locations.enterLocation(name);
            return;
        }
        
        /* gameRiddle method: to get a game riddle */
        void gameRiddle() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the animal game") {
                std::cout << "\nThere's no animal game challenge here.\n";
                return;
            }
            
            if (game_riddle_answered) {
                std::cout << "\nYou have played this Animal game once before.\n";
                //return;
            }
            
            Riddle r = riddles_and_jokes.getRandomGameRiddle();
            std::string answer = askAnswer(r.question);        // determine if player go it right or not and how to proceed
            
            if (isCorrect(answer, r.answer)) {
                
                std::cout << "\nCorrect!.\n";
                game_riddle_answered = true;
                
                sendToLocation("Dreamland");
                
            }
            else {
                std::cout << "\nThat is incorrect. The King of Hearts summons you!\n";
                sendToLocation("The King's presence");
            }
            return;
        }
        
        /* kingRiddle method: to get a king riddle */
        void kingRiddle() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the king's presence") {
                std::cout << "\nThe King of Hearts isn't here to ask you a riddle.\n";
                return;
            }
            
            Riddle r = riddles_and_jokes.getRandomKingRiddle();
            std::string answer = askAnswer(r.question);        // determine if player go it right or not and how to proceed
            
            if (isCorrect(answer, r.answer)) {
                std::cout << "\nCorrect! The King sends you back to the Animal game.\n";
                
                king_riddle_answered = true;
                
                // precious stone added to player if correct answer
                Item stone("Precious stone", "Inventory", 1, 1, 0);
                main_character.inventory.addItem(stone);
                std::cout << "\nThe King of Hearts has gifted you a precious stone.\n";
                
                // correct answer - go back to the animal game
                sendToLocation("The Animal game");
                
            }
            else {
                std::cout << "\nThat is incorrect. The Queen of Hearts summons you!\n";
                // wrong answer - summoned by the queen
                sendToLocation("The Queen's presence");
            }

        }

        /* queenRiddle method: to get a queen riddle */
        void queenRiddle() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr || toLowerCase(current_loc->name) != "the queen's presence") {
                std::cout << "\nThe Queen of Hearts isn't here to ask you a riddle.\n";
                return;
            }
            
            while (main_character.player_lives > 0) {
                Riddle r = riddles_and_jokes.getRandomQueenRiddle();
                std::string answer = askAnswer(r.question);        // determine if player go it right or not and how to proceed
                
                if (isCorrect(answer, r.answer)) {
                    std::cout << "\nCorrect! The Queen has vindicated you! Getting through her is rewarding... You're headed straight to Dreamland\n";
                    
                    queen_riddle_answered = true;
                    // correct answer - go straight to dreamland
                    sendToLocation("Dreamland");
                    return;
                }
                
                // otherwise, lose life
                std::cout << "\nWrong answer!... \n";
                loseLife();
                
                return;
            }
        }
        
        /* riddle control: controls which riddle method is called depending on the location */
        void riddle() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr) {
                std::cout << "\nYou are not at a valid location.\n";
                return;
            }
            
            std::string loc = toLowerCase(current_loc->name);
            
            if (loc == "the play room") playroomRiddle();
            else if (loc == "the animal game") gameRiddle();
            else if (loc == "the king's presence") kingRiddle();
            else if (loc == "the queen's presence") queenRiddle();
            else {
                std::cout << "\nThere are no riddles to answer here.\n";
            }
            return;
        }

        /* usePreciousStone control: if player has precious stone, 
	 they can use it to go to Play room from animal game, king's presence,
	 the queen's presence as long as there's still live.

         One of the advantages of this is that player can get out of the whims of the queen with precious stone and go to the playroom,
	 from room, they go back to animal game.
         However, if they still don't get riddles right and return to queen, 
	 their life doesn't reset to 3, they continue with the queen with the same number of lives.
         So it's like they got an extra round, instead of life */
        void usePreciousStone() {
            Location *current_loc = main_character.getMCCurrentLocation();
            
            if (current_loc == nullptr ||
                (toLowerCase(current_loc->name) != "the animal game" &&
                 toLowerCase(current_loc->name) != "the king's presence" &&
                 toLowerCase(current_loc->name) != "the queen's presence")
                ) {
                    std::cout << "\nYou can't use the precious stone to go to play room from this location\n";
                    return;
                
            }
            
            std::string item_name = trim("Precious stone");
            
            // Check whether the item is valid and that it consumable before removing it
            Item *item = main_character.inventory.getItemByName(item_name);
            if (item == nullptr) {
                std::cout << "\nYou don't have a precious stone.\n";
                return;
            }
            
            Item used("", "", 0, 0, 0);
            
            // remove the previous stone from Alice's inventory
            if (main_character.inventory.removeItem(item_name, 1, used)) {
                std::cout << "\nYou invoked your " << used.name << "...You're headed for the play room.\n";
                
                sendToLocation("The Play room");
                return;
            }
            return;
        
        }
        
        /* reset an cook item in a location */
        void resetCookItems() {
            items.restoreItem("saucepan");
            items.restoreItem("cutlery");
            items.restoreItem("pepper");
            items.restoreItem("egg");
            
            return;
        }
        /* reset an tea item in a location */
        void resetTeaItem() {
            items.restoreItem("cup of tea");
            return;
        }
        /* reset an tea item in a location */
        void resetGlovesItem() {
            items.restoreItem("gloves");
            return;
        }

        /* Method to reset flags of locations that if player rreturns to, they'd need to repeat the challange there */
        void resetFlags() {
            has_cooked = false;
            has_spoken_mushroom = false;
            has_spoken_rabbit = false;
            has_spoken_party = false;
            has_drunk_tea = false;
            card_coin_flipped = false;
            book_found = false;
            book_opened = false;
            barn_unlocked = false;
            has_taken_gloves = false;
            has_worn_gloves = false;
            game_riddle_answered = false;
            king_riddle_answered = false;
            queen_riddle_answered = false;
            
            return;
        }
    
                    
        /* check how many lives left */
        void checkLivesLeft() {
            std::cout << "\nYou have " << main_character.player_lives << " left.\n";
            return;
        }

        /* Inventory control: */
        void inventory() {
            main_character.inventory.displayInventory();
            return;
        }
        
        /* Help control: */
        void help() {
            actions.displayHelp();
            return;
        }

	    /*Wake control: */
	    void wake() {
		    Location *current_loc = main_character.getMCCurrentLocation();

		    if (current_loc != nullptr && toLowerCase(current_loc->name) == "dreamland") {
			    waking_up = true;
			    std::cout << "\nHey Alice, you've woken up in Wonderland!\n";
			    move("north");
		    }
		    else {
			    std::cout << "\nThere's nothing to wake up from here.\n";
		    }

		    return;
	    }
    
        /* Size control: provides player with their current size */
        void getSize() {
            std::cout << "\nYou current size: " << main_character.alice_size << "\n";
            return;
        }
        /* Score control: provides player with their current score */
        void score() {
            std::cout << "\nYou current score: " << main_character.getPoints() << "\n";
            return;
        }
    
        /* getGem control: display player's gem count */
        void displayFinalInventory() {
            std::cout << "\nYour final inventory: " << "\n";
            main_character.inventory.displayInventory();
            return;
        }

        /* Quit control : quit */
        void quit() {
            main_character.player_state = MainCharacter::QUIT;
            return;
        }

        /* Player loses life: for instance at the queen's presence */
        void loseLife() {

            if (main_character.player_lives > 0) {
                --main_character.player_lives;			// drecrement by one
            }

            std::cout << "\nLives remaining: " << main_character.player_lives << "\n";

            if (main_character.player_lives == 0) {
                main_character.player_state = MainCharacter::PERISHED;
                std::cout << "\nGame over! The Queen has defeated you.\n";
                return;
            }
        }
    
        /* method to reset the game: after player is done playing, we want to reset everything */
        void resetGameWorld() {
            
            main_character.setMCCurrentLocation(locations.getStartLocation());
            main_character.inventory.resetInventory();
            items.resetItems();
        }
};

#endif // !CONTROL_H

