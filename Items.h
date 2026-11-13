//: Items.h

 /*
 Title: Items.h
 Description: Items class for Wonderland game
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the Items class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled
 
 Classes:
    Item data strcuture
	Items class

 Variables:
 	member properties for Item - name, item_location, consumable, count, item_effect
 	member properties for Items - vector of type Item named items

 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to interact with available items in specified locations.

 Bad Data case 1 (invalid item interaction)
    >If player attempts to use or collect an item that's not available or does not exist
    >Expected:The game should handle it and display an appropriate message to player without affecting or corrupting the game state or inventory.
 
 Edge case (items files)
    >If the items file items.txt does not load
    > Expected: An appropriate message sould be displayed to the player.
 
 Discussion:
 
*/

#ifndef ITEMS_H
#define ITEMS_H

#include "Actions.h"
#include "Utilities.h"					// required for Item structure

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <exception>

// Item data structure represents an item
struct Item {

    std::string name;                // item name
    std::string item_location;           // location where item is found
    int consumable;                     // 1 = consumable, 0 = non-consumable
    int count;                    // count of the item in the game
                                 // user can't possess or consume more than item count
    int item_effect;                // its effect on player
    
    Item(const std::string &item_name, const std::string &item_loc, int cs, int cnt, int effect) : name(item_name), item_location(item_loc), consumable(cs), count(cnt), item_effect(effect) {
    }

    void printItem() const {
        std::cout << name
        << " |  Item location: " << item_location
        << " | Count: " << count << "\n";
        //<< " |  Effect: " << item_effect << "\n";
    }
    
    Item *getItemByName(const std::string &str) {

        if (toLowerCase(str) == toLowerCase(name)) return this;
        
        return nullptr;
    }
    
    bool isConsumable() const {
        return consumable == 1;
    }
};

/* Items Class */
class Items {
	
	private:

		std::vector<Item> items;

		static bool sameText(const std::string &a, const std::string &b) {
			const std::string lower_a = toLowerCase(static_cast<const std::string&>(a));
			const std::string lower_b = toLowerCase(static_cast<const std::string&>(b));
			return lower_a == lower_b;
		}

	public:
						
		Items() {
		
			// load items txt file
			loadItems("game_files/items.txt");
		}

		/* method to get items vector */
		std::vector<Item> getItems() {
			return items;
		}
		// access actual items collection when needd
		std::vector<Item> &getItemsRef() {
			return items;
		}

		/* method to get item by name and return a pointer to the actual stored item */
		Item *getItemByName(const std::string &name) {

			for (Item &item : items) {
				//if (toLowerCase(item.name) == toLowerCase(name)) return &item;
				if (sameText(item.name, name)) return &item;
			}

			return nullptr;
		}


		/* method to get items vector size */
		std::size_t getItemsSize() {
			return items.size();
		}

		/* method to load items into a vector from file */
		bool loadItems(const std::string &filename) {

			std::ifstream file(filename);

			if (!file.is_open()) {
				std::cerr << "Could not open " << filename << "\n";
				return false;
			}

			// clear vector
			items.clear();

			std::string line;

			while (std::getline(file, line)) {

				line = trim(line);				// eliminate surrounding whitespaces

				if (line.empty()) continue;

				/* Items file format
				   Name|Location|Consumable|Count|Item Effect
				   	location is where the item is found
				   	item_effect 1, increases main characters size by one, 0 doesn't change size
				   -1 reduces size by 1
				   	count represents how many of the items there are. Alice can't have more than the count of an item
				 */
				std::string fields[5];
				std::size_t start = 0;
				bool valid_line = true;

				for (int i = 0; i < 4; i++) {

					std::size_t separator = line.find('|', start);

					// refused rows that don't contain all three separators for the 5 fields
					if (separator == std::string::npos) {
						valid_line = false;
						break;
					}

					fields[i] = trim(line.substr(start, separator - start));

					start = separator+1;
				}

				if (!valid_line) {
					std::cerr << "Invalid item record: " << line << "\n";
					continue;
				}

				// the final field contains the effect
				fields[4] = trim(line.substr(start));

				// remove carriage returns if present
				for (std::string &field : fields) {
					if (!field.empty() && field.back() == '\r') {
						field.pop_back();
					}
				}

				// try to emplace them in fields
				try {
            				int consumable = std::stoi(fields[2]);
					int count = std::stoi(fields[3]);
            				int effect = std::stoi(fields[4]);

            				items.emplace_back(
                				fields[0],
						fields[1],
						consumable,
                				count,
                				effect
            				);
        			}
        			catch (const std::exception &) {
            				std::cerr << "Invalid item values: "
                      				<< line << "\n";
            				continue;
        			}

			}

			file.close();

			return true;
		}

		/* method to display all available items of a collection
		*/
		void displayItems() const {
			for (const Item &item : items) item.printItem();	
		}

		/* get methods */
		Item *getItemAtLocation(const std::string &item_name, const std::string &loc_name) {

			// lower case copies without changing original
			const std::string search_name = toLowerCase(static_cast<const std::string&>(item_name));

			for (Item &item : items) {

				//if (toLowerCase(item.name) == toLowerCase(item_name) &&
						//toLowerCase(item.item_location) == toLowerCase(loc_name))
					//return &item;
				const std::string stored_name = toLowerCase(static_cast<const std::string&>(item.name));

				//if (sameText(item.name, item_name) && sameText(item.item_location, loc_name)) return &item;
				if (sameText(item.item_location, loc_name) && item.count > 0 
						&& (stored_name == search_name || stored_name.find(search_name) 
							!= std::string::npos)) {
					return &item;
				}
			}

			return nullptr;
		}

		/* method to take an item from a location and put it in player's inventory */
		bool takeItem(const std::string &item_name, const std::string &loc_name, int quantity, Item &taken) {

			// watch out for zero or negative quantity
			if (quantity <= 0) return false;

			Item *item = getItemAtLocation(item_name, loc_name);

			// check that item exists in the location
			if (item == nullptr) {
				std::cout << "That item isn't here.\n";
			       return false;	
			}

			//can't take more than item count
			if (item->count < quantity) {
				std::cout << "There aren't enough of those items here.\n";
				return false;
			}

			// record item detail and quantity being taken
			taken = *item;
			taken.count = quantity;
			taken.item_location = "Inventory";

			// decrment quantity remaining in the game world/ location
			item->count -= quantity;

			return true;
		}

		/* method to drop item to a location when player drops it*/
		void dropItem(Item &dropped, const std::string &loc_name, int quantity) {

			// ignore invalid drop
			if (quantity <= 0) return;

			//if the item already exists in location, merge the quantities
			Item *existing = getItemAtLocation(dropped.name, loc_name);

			if (existing != nullptr) {
				existing->count += quantity;
				return;
			}

			// otherwise, create a new entry at the drop location
			items.emplace_back(dropped.name, loc_name, dropped.consumable, quantity, dropped.item_effect);

		}

		/* restore an item to its original location after consumption */
		void restoreItem(const std::string &item_name) {

			for (Item &item : items) {
				if (sameText(item.name, item_name)) {
					item.count = 1;
					return;
				}
			}
		}
            
            /* reset items to its original location after consumption */
            void resetItems() {

                for (Item &item : items) {
                    if (sameText(item.name, "gem")) {
                        item.count = 2;
                    }
                    else if (sameText(item.name, "cup of tea")) {
                        item.count = 2;
                    }
                    else if (sameText(item.name, "egg")) {
                        item.count = 2;
                    }
                             
                    item.count = 1;
                    
                }
                return;
            }


		/* display items currently available at a location */
		void displayItemsAtLocation(const std::string &loc_name) const {
			bool found = false;

			for (const Item &item : items) {
				if (toLowerCase(item.item_location) == toLowerCase(loc_name) && item.count > 0) {
					item.printItem();
					found = true;
				}
			}

			if (!found) {
				std::cout << "There are no items here.\n"; 
			}
		}
};


#endif // !ITEMS_H

