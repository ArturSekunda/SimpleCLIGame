//
// Created by Darko on 1.09.2025.
//

#ifndef SIMPLECLIGAME_INVENTORY_H
#define SIMPLECLIGAME_INVENTORY_H
#include <iostream>
#include <memory>
#include <vector>

#include "Items.h"

struct Weapon;
class ItemGenerator;

struct S_Inventory {

    std::unique_ptr<Weapon> weapon;
    bool isUsed = false;


    void ShowWeaponAndHisStats() const {
        if (!weapon) {
            std::cout << "--- EMPTY SLOT ---\n\n";
            return;
        }

        auto weaponName = weapon->weapon_name;
        auto weaponType = weapon->weapon_type;
        auto weaponRarity = weapon->weapon_rarity;
        auto weaponDamageType = weapon->damage_type;
        auto weaponDamage = weapon->calculateTotalDamage_Player();

        if (isUsed) {
            std::cout << "!YOU ARE USING ME!" << "\n";
        }
        std::cout << "Weapon: " << ItemGenerator::weaponNameToString(weaponName) << " " << ItemGenerator::weaponTypeToString(weaponType) << " (" << ItemGenerator::damageTypeToString(weaponDamageType) << ")\n";
        std::cout << "Rarity: " << ItemGenerator::weaponRarityToString(weaponRarity) << "\n";
        std::cout << "Damage: " << weaponDamage << "\n";
        std::cout << "\n";
    }


};

class Inventory {

    friend void to_json(nlohmann::json& j, const Inventory& inv);
    friend void from_json(const nlohmann::json& j, Inventory& inv);

    std::vector<S_Inventory> inventory;

public:

    void ShowInventory() const;

    void DeleteItem(int index);

    void AddItem(const Weapon &weapon);

    void MarkAsUsed(int index);

    size_t GetSizeofInv() const {
        return inventory.size();
    }

    bool IfEmptyTable() const {

        bool empty = inventory.empty();
        return empty;
    }

    const Weapon* GetWeaponAtIndex(int index) const {

        if (index < 0 || index >= inventory.size()) {
            return nullptr;
        }

        return inventory.at(index).weapon.get();
    }

    bool IsItemUsed(int index) const {
        return inventory.at(index).isUsed;
    }

    int GetEquippedItemIndex() const {
        for (int i = 0; i < inventory.size(); i++) {
            if (inventory.at(i).isUsed) {
                return i;
            }
        }
        return -1;
    }


};

inline void to_json(nlohmann::json& j, const Inventory& inv) {
    j = nlohmann::json::array();
    for (auto const& slot : inv.inventory) {
        if (slot.weapon) {
            j.push_back(*slot.weapon);
        }
    }
}

inline void from_json(const nlohmann::json& j, Inventory& inv) {
    inv.inventory.clear();
    for (auto const& slot : j) {

        Weapon temp_weapon = slot.get<Weapon>();

        inv.AddItem(temp_weapon);
    }
}


#endif //SIMPLECLIGAME_INVENTORY_H