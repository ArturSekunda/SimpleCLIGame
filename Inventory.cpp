//
// Created by Darko on 1.09.2025.
//

#include "Inventory.h"

void Inventory::ShowInventory() const {
    if (inventory.empty()) {
        std::cout << "You have no items in your inventory!\n";
        return;
    }
    int index = 0;

    // New method how to use for loop (for me xD)
    for (const auto& slot :  inventory) {


        std::cout << "ID: " << index << "\n";
        slot.ShowWeaponAndHisStats();

        index++;
    }


}

void Inventory::DeleteItem(int index) {
    // Checking if index is valid and is size of inventory isn't smaller than index.
        if (index < 0 || index >= inventory.size()) {
            std::cout << "Invalid ID!\n";
            return;
        }

    inventory.erase(inventory.begin() + index);

    ShowInventory();
    std::cout << "Item with ID "<< index <<" has been deleted!\n";


}

// Look at the name of this function... Aaaaannnndddd now you know what it's for xD
void Inventory::AddItem(const Weapon &weapon) {


    // Create a empty "room" at the end of the vector
    inventory.emplace_back();

    // Get reference to this new slot
    S_Inventory& newSlot = inventory.back();

    // Add data to the slot
    newSlot.weapon = std::make_unique<Weapon>(weapon);

}

void Inventory::MarkAsUsed(int index) {
    int newIndex = 0;
    for (auto& slot :  inventory) {
        slot.isUsed = false;

        newIndex++;
    }
    inventory.at(index).isUsed = true;
}
