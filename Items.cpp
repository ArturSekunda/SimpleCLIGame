//
// Created by Darko on 29.08.2025.
//

#include "Items.h"

#include <iostream>

std::string ItemGenerator::weaponNameToString(Weapon_Name name) {
        switch(name) {
            case Weapon_Name::EXCALIBUR: return "Excalibur";
            case Weapon_Name::MJOLNIR: return "Mjolnir";
            case Weapon_Name::GUNGNIR: return "Gungnir";
            case Weapon_Name::DURANDAL: return "Durandal";
            case Weapon_Name::GRAM: return "Gram";
            case Weapon_Name::BALMUNG: return "Balmung";
            case Weapon_Name::CLARENT: return "Clarent";
            case Weapon_Name::ARONDIGHT: return "Arondight";
            case Weapon_Name::GALATINE: return "Galatine";
            case Weapon_Name::TYRFING: return "Tyrfing";
            default: return "Unknown Weapon";
        }
    }
    std::string ItemGenerator::weaponTypeToString(Weapon_Type type) {
        switch (type) {
            case Weapon_Type::SWORD: return "Sword";
            case Weapon_Type::AXE: return "Axe";
            case Weapon_Type::BAT: return "Bat";
            case Weapon_Type::MAGIC_WAND: return "Magic Wand";
            case Weapon_Type::BOW: return "Bow";
            default: return "Unknown Type";
        }
    }
    std::string ItemGenerator::damageTypeToString(Damage_Type type) {
        switch (type) {
            case Damage_Type::NORMAL: return "Normal";
            case Damage_Type::FIRE: return "Fire";
            case Damage_Type::ICE: return "Ice";
            case Damage_Type::ELECTRIC: return "Electric";
            case Damage_Type::POISON: return "Poison";
            case Damage_Type::HOLY: return "Holy";
            case Damage_Type::BLOODY: return "Bloody";
            default: return "Unknown Type";
        }
    }
    std::string ItemGenerator::weaponRarityToString(Weapon_Rarity rarity) {
        switch (rarity) {
            case Weapon_Rarity::COMMON: return "Common";
            case Weapon_Rarity::UNCOMMON: return "Uncommon";
            case Weapon_Rarity::RARE: return "Rare";
            case Weapon_Rarity::EPIC: return "Epic";
            case Weapon_Rarity::LEGENDARY: return "Legendary";
            default: return "Unknown Rarity";
        }
    }

void ItemGenerator::displayWeapon(const Weapon &weapon) {
        std::cout << "========== You found a weapon! ==========\n";
        std::cout << "Name: " << weaponNameToString(weapon.weapon_name) << "\n";
        std::cout << "Type: " << weaponTypeToString(weapon.weapon_type) << "\n";
        std::cout << "Rarity: " << weaponRarityToString(weapon.weapon_rarity) << "\n";
        std::cout << "Damage type: " << damageTypeToString(weapon.damage_type) << "\n";
        std::cout << "Total damage: " << weapon.calculateTotalDamage_Player() << "\n";
        std::cout << "====================================\n\n";

}

