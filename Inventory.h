//: Inventory.h

 /*
 Title: Inventory.h
 Description: Wonderland game Inventory class...
 Date: September 20, 2026
 Author: Samuel Oluyemi
 Version: 1.0
 Copyright: 2026 Samuel Oluyemi
*/
/*
 DOCUMENTATION
 
 Program Purpose:
    Implements the Inventory class of the Wonderland Adventure Game

 Compile: cd into Wonderland Game directory; then run g++ Wonderland.cpp -o Wonderland in the command line
 Execution: ./Wonderland from the Wonderland Game directory where the source code was compiled

 
 Classes:
	Inventory class

 Variables:
 	member properties of Inventory: vector of type Item (from Item data structure in Items.h) named stored_items
 
 Command Line Argument: None
*/

/*
 TEST PLAN
 
 Normal case:
    >All public methods should be calleable
    >Player should be able to add, drop or use available items in their inventory

 Edge case (inventory)
    >If player attempts to take multiple items or interact with an item already collected
    > Expected:If multiple items are available, then player can take them. If already taken item, an output should display to player that the item is no longer available to for collection.
 
 Discussion:
 
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Utilities.h"
#include "Items.h"

#include <iostream>
#include <string>
#include <vector>

/* Inventory Class */
class Inventory {

	private:
		
		std::vector<Item> stored_items;

		// compare names without changing their original capitalization
		static bool sameText(const std::string &a, const std::string &b) {
			const std::string lower_a = toLowerCase(static_cast<const std::string&>(a));
			const std::string lower_b = toLowerCase(static_cast<const std::string&>(b));
			return lower_a == lower_b;
		}

	protected:

	public:

		// get items method
		std::vector<Item> getItems() {
			return stored_items;
		}
        // get the reference to the stored items
		std::vector<Item> &getItemsRef() {
			return stored_items;
		}

		// add item method
		void addItem(const Item &item) {

            // if the item doesn't have a count, return
			if (item.count <= 0) return;

			// increment an already owned item in inventory
			for (Item &stored : stored_items) {
				//if (toLowerCase(stored.name) == toLowerCase(item.name)) {
				if (sameText(stored.name, item.name)) {
					stored.count += item.count;
					return;
				}
			}

			// store a new item in player's inventory
			Item copy = item;
            // remember every item has a location where it is found as per items.txt
			copy.item_location = "Inventory";
			stored_items.push_back(copy);       // push the new item into inventory
		}

		/* method to find an item in the player inventory by name. return the pointer to the item */
		Item *getItemByName(const std::string &item_name) {

			for (Item &item : stored_items) {
				//if (toLowerCase(item.name) == toLowerCase(item_name)) return &item;
				if (sameText(item.name, item_name)) return &item;
			}

			return nullptr;
		}

		/* method to check if player has an item in the inventory
		 	if getting that item by name returns nullptr, it means player doens't have
		 */
		bool hasItem(const std::string item) {

			return (getItemByName(item) != nullptr);
		}

		/* method to remove a specified quantity from the player's inventory */
		bool removeItem(const std::string &item_name, int quantity, Item &removed) {

			if (quantity <= 0) return false;

			Item *item = getItemByName(item_name);

			if (item == nullptr) return false;

			if (item->count < quantity) return false;

			// copy the item details and set the transferred quantity
			removed = *item;
			removed.count = quantity;

			// decrment the quantity in the inventory
			item->count -= quantity;

				// remove the entry when the player has no entry left
				if (item->count == 0) {
					for (auto it = stored_items.begin(); it != stored_items.end(); it++) {

						if (&(*it) == item) {
							stored_items.erase(it);
							break;
						}
					}
				}

				return true;
		}

		/* method to display inventory */
		void displayInventory() const {

			if (stored_items.empty()) {
				std::cout << "Your inventory is empty.\n";
				return;
			}

			std::cout << "\n /===== YOUR INVENTORY =====/\n";

			// show each item the player currently owns
			for (const Item &item : stored_items) {
				std::cout << item.name
					<< " | Count: " << item.count << "\n";
			}
		}
    
        /* reset inventory: called after player has won and choose to play again */
        void resetInventory() {
        
            stored_items.clear();           // simply clear the inventory
            return;
        }
};

#endif // !INVENTORY_H

