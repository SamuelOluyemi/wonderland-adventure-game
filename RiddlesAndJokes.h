//: RiddlesAndJokes.h

 /*
 Title: RiddlesAndJokes.h
 Description: RiddleAndJokes class for Wonderland game
 Date: October 16, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the RiddleAndJokes class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Classes:
    Riddle data structure
    Joke data structure
	RiddlesAndJokes class

 Variables:
 	member properties for Riddle: question, answer
    member properties for Joke: joke
    member properties for RiddlesAndJokes: 4 vectors of type Riddle - playroom_riddles, game_riddles, king_riddles, queen_riddles
                                            1 vector of type Joke - dad_jokes

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
     >Player should be able to answer riddles where required.
     >Player should be able to request riddles(with answers displayed automatically) and jokes in the Play room(as a safe room).
 
 Edge case 1 (riddles and jokes)
    >If player provides an incorrect answer to a riddle
    >Expected: The game should handle the incorrect answer according to its rules and allow game play to continue (by moving player to a different location) where apprioprate or take one life from player in the appropriate case.
 
 Edge case 2 (riddles and jokes files)
    >If the riddles and jokes text files do not load
    > Expected: An appropriate message sould be displayed to the player.

 Discussion:
 
 
*/

#ifndef RIDDLES_AND_JOKES_H
#define RIDDLES_AND_JOKES_H

#include "Utilities.h"


#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

// Riddles data structure
struct Riddle {
    std::string question;
    std::string answer;
};

// Jokes data structure
struct Joke {
    std::string joke;
    
    Joke(const std::string &j) : joke(j){}
};


/* RiddlesAndJokes Class */
class RiddlesAndJokes {
	
	private:

		// each riddle vector and jokes vector: animal game, king, and queen
		std::vector<Riddle> playroom_riddles;
		std::vector<Riddle> game_riddles;
		std::vector<Riddle> king_riddles;
		std::vector<Riddle> queen_riddles;
		std::vector<Joke> dad_jokes;


	protected:

	public:

		// load riddles method
		bool loadRiddles(const std::string &filename, std::vector<Riddle> &riddles) {

			std::ifstream file(filename);

			if (!file.is_open()) {
				std::cerr << "Could not open " << filename << "\n";
				return false;
			}

			// clear vector before refill it 
			riddles.clear();

			std::string line;

			while (std::getline(file, line)) {

				std::size_t separator = line.find('|');

				if (separator == std::string::npos) continue;

				std::string question = trim(line.substr(0, separator));
				std::string answer = trim(line.substr(separator + 1));

				if (!question.empty() && !answer.empty()) {
					// if they're not empty, push them unto the vector
					riddles.push_back({question, answer});
				}

			}

			file.close();

			return true;
		}

		// load dad jokes
		bool loadJokes(const std::string &filename) {

			std::ifstream file(filename);

			if (!file.is_open()) {
				std::cerr << "Could not open " << filename << "\n";
				return false;
			}

			dad_jokes.clear();

			std::string line;

			while (std::getline(file, line)) {

				line = trim(line);

				if (!line.empty()) {
					dad_jokes.emplace_back(line);
				}
			}

			file.close();

			return true;
		
		}

		// method to load all
		bool loadAll() {

			bool success = true;

			success = loadRiddles("game_files/playroom_riddles.txt", playroom_riddles) && success;
			
			success = loadRiddles("game_files/game_riddles.txt", game_riddles) && success;

			success = loadRiddles("game_files/king_riddles.txt", king_riddles) && success;

			success = loadRiddles("game_files/queen_riddles.txt", queen_riddles) && success;

			success = loadJokes("game_files/dad_jokes.txt") && success;

			return success;
		}

        /* Methods to get random riddles at play room, the animal game, the king's presence, and the queen's presence
            Also, to get a random joke at play room
         */
		Riddle getRandomPlayRiddle() {
			if (playroom_riddles.empty()) {
				std::cout << "No play room riddles have been loaded.\n";
				return {"", ""};
			}

			return getRandom(playroom_riddles);
		}

		Riddle getRandomGameRiddle() {

			if (game_riddles.empty()) {
				std::cout << "No game riddles have been loaded.\n";
				return {"", ""};
			}

			return getRandom(game_riddles);
		}

		Riddle getRandomKingRiddle() {

			if (king_riddles.empty()) {
				std::cout << "No game riddles have been loaded.\n";
				return {"", ""};
			}

			return getRandom(king_riddles);
		}

		Riddle getRandomQueenRiddle() {

			if (queen_riddles.empty()) {
				std::cout << "No game riddles have been loaded.\n";
				return {"", ""};
			}

			return getRandom(queen_riddles);
		}

		void displayRandomJoke() {
			if (dad_jokes.empty()) {
				std::cout << "No dad jokes have been loaded.\n";
				return;
			}
			std::cout << getRandom(dad_jokes).joke << "\n";
            return;
		}
};

#endif // !RIDDLES_AND_JOKES_H

