//: Utilities.h

 /*
 Title: Utilities.h
 Description: Utilities class for Wonderland game
 Date: October 10, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
 	Implements the Utilities class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled

 
 Classes:
	No class, simply functions

 Variables:
 	
 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
 
    >All functions here must be calleable without producing any errors
 
 Bad Data case
    >If a utility function is called and produces error, this might cause the
    >Expected:The game should reject that movement, handle it and display the appropriate message to player without moving the player incorrectly, thereby corrupting the game and player state.
 
 Discussion:
 	If a Utility function is called, which is the case in most of the program with the use of functions like toLowerCase(), getRandom(), displayInstructions() at the start of the game, displayWelcomeGreeting(), this might cause the program to fail to compile. The Utilities header file is a potential error surface for this game because its function uses cut across the entire game.
 
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <cctype>				// required for lowercase
#include <algorithm>				// required for lower case
#include <random>				// required for generating random elements
#include <stdexcept>				// required for exceptions
#include <cstdlib>				// required for clearing terminal screen
#include <vector>
#include <fstream>
#include <limits>

/**** UTILITY FUNCTIONS ****/

/* Utility Function to transform a string or line of strings to lowercase
 Takes a line of strings as argument, and returns it in lower case
 */
inline std::string toLowerCase(std::string &str) {

    std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });
    return str;
}

/* Utility Function to transform a string or line of strings to lowercase
 Takes a line of strings as argument, and returns it in lower case copy without modifying the orginial
 */
inline std::string toLowerCase(const std::string &str) {

    std::string result = str;
    toLowerCase(result);
    return result;
}

/* Utility Function to get a random element from a vector
 Takes a vector of type T and returns a random element of type T from the vector
 */
template <typename T>
T &getRandom(std::vector<T> &arr) {

    if (arr.empty()) {
        throw std::invalid_argument("Unable to select from an empty array");
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<std::size_t> dist(0, arr.size()-1);

    return arr[dist(gen)];

}

/* Utility Function to clear terminal screen */
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Removes whitespace from the beginning and end of a string.
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";

    // Find the first character that is not whitespace.
    const std::size_t first = text.find_first_not_of(whitespace);

    // Return an empty string if the input contains only whitespace.
    if (first == std::string::npos)
        return "";

    // Find the last character that is not whitespace.
    const std::size_t last = text.find_last_not_of(whitespace);

    // Return the substring without leading or trailing whitespace.
    return text.substr(first, last - first + 1);
}

/* Greeting */
void displayWelcomeGreeting() {

        std::string guide = "Deborah";
        std::cout << "Hello Alice \n";
        std::cout << R"( 
                |========================================================|
                |                                                        |
                |         __        __   _                               |
                |         \ \      / /__| | ___ ___  _ __ ___   ___      |
                |          \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \       |
                |           \ V  V /  __/ | (_| (_) | | | | | |  __/       |
                |            \_/\_/ \___|_|\___\___/|_| |_| |_|\___|       |
                |                                                        |
                |                                      |
                |                  T O             |
                |                             |
                |        W O N D E R L A N D   G A M E            |
                |                                                        |
                |========================================================| 
        )" << "\n\n";
            
        std::cout << "I am " << guide << ", and I will be your guide through this amazing Adventure!\n"
            << "Your goal is to eventually arrive at Wonderland, "
            << "You will encounter differnt obstacles during your adventure in form of locations,\n"
            << "items, other interesting characters, and some challenges.\n"
            << "You get three lives in this game, each time you perish, the life count decreases. "
            << "Once you're at zero, we love you but it's game over 😊\n\n";
        
        return;

}

void loadInstructions(const std::string &filename) {
    
//    std::cout << "\nLoading Instructions... \n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not load file " << filename << "\n";
        return;
    }
    
    std::string line;
    
    while(std::getline(file, line)) {
        
        std::cout << line << "\n";
    }
    
    file.close();

    return;
}

/* Instructions */
void displayInstructions() {
    std::string input;
    std::cout << "Do you want to see instructions and guide for the game? Y/N \n";
    std::cin >> input;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    toLowerCase(input);
    
    if (input == "y" || input == "yes") {
        clearScreen();                  // from Utilities.h
        loadInstructions("game_files/instructions.txt");        // from Utilities.h
        return;
    }
    else if (input == "n" || input == "no") return;
    else {
        std::cerr << "\nInvalid input \n";
        std::cout << "You can use 'Ctrl + C' to exit the program at any time.\n";
        displayInstructions();
    }
    return;
}

#endif // !UTILITIES_H

