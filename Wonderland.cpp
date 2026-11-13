//: Wonderland.cpp

 /*
 Title: Wonderland.cpp
 Description: Wonderland Game Project...
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
 	Design an adventure game.

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Notes: The source code includes Documentation and Test cases in main() while the implementations are found in the header files.
 	The class implementations are found in their respective header files (named accordingly) in the Wonderland Game directory
 
 Classes: None - Wonderland.h is the main class for this project. Notes on all other classes are found in Wonderland.h and their respective header files

 Variables:
 	None here: Check the various other header files. The main class is Wonderland class form which we've called Wonderland::main()
 
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

 Bad Data case 1 (invalid direction)
    >If player attempts to go to an unavailable location from current location
    >Expected:The game should reject that movement, handle it and display the appropriate message to player without moving the player incorrectly, thereby corrupting the game and player state.
 
 Bad Data case 2 (invalid action/ command)
    >If player attempts to use an action/command that's not supported by the game or in their current state/ location
    >Expected:The game should reject, handle it and display an appropriate message to player without affecting the game state.
 
 Bad Data case 1 (invalid item interaction)
    >If player attempts to use or collect an item that's not available or does not exist
    >Expected:The game should handle it and display an appropriate message to player without affecting or corrupting the game state or inventory.
 
 Edge case 1 (inventory)
    >If player attempts to take multiple items or interact with an item already collected
    > Expected:If multiple items are available, then player can take them. If already taken item, an output should display to player that the item is no longer available to for collection.
 
 Edge case 2 (action)
    >If player attempts an action that's in action_words.txt but not implemented in control yet
    >Expected:An appropriate output is displayed indicating the action hasn't been implemented yet
 
 Edge case 2 (riddles and jokes)
    >If player provides an incorrect answer to a riddle
    >Expected: The game should handle the incorrect answer according to its rules and allow game play to continue (by moving player to a different location) where apprioprate or take one life from player in the appropriate case.
 
 Edge case 3 (game completion)
    >If player satisfies the game's completion condition (by reaching the final destination).
    >Expected: The game should recognize completion, output the appropriate message, and prompt the player to choose to play again or not. In the case where they choose to play again, player should be taken to the start point and restart the game. Otherwise, the game should end correctly.
 
 Edge case 4 (game files)
    >If a game file such as game_riddles.txt does not load
    > Expected: An appropriate message sould be displayed to the player.
 
 Discussion:
 	The Wonderland game program implements a text-based game in whioch the player interacts with locations, charcters, items through available actions/ commands.
    Note that if a Utility function is called, which is the case in most of the program with the use of functions like toLowerCase(), getRandom(), displayInstructions() at the start of the game, displayWelcomeGreeting(), this might cause the program to fail to compile. The Utilities header file is an error surface for this game because its function uses cut across the entire game.
    Testing involves verifying that movement between locations works as expected; that player's interactions with characters and items yield expected results; that inventory and items update as player interacts and progresses in the game; that player's score updates as they visit different locations; that player is rewarded for answering riddles correctly according to the game rules;
    Invalid inputs are handled appropriately without crashing or corrupting the game.
    More tests involves examining the riddles, item interactions, and game's completion conditon, helping to verify that the individual classes work and are connected correctly through Wonderland class, the control class and the MainCharacter class.
    More testing involves presenting the game to a real-world user to play the game without influence or knowedge of the game's map. They played freely, tested edge cases, identified subtle bugs, provided feedback, and these were all considered and correction/ adoptions were made where necessary.
 
*/

#include "Wonderland.h"


#include <iostream>
#include <string>


/************** Main *************/
int main() {
	
	return Wonderland::main();
	
} ///:~

/* EXPECTED OUTPUT */

/*
A text-based adventure game
*/


