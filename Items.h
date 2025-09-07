//
// Created by Darko on 29.08.2025.
//

#ifndef SIMPLECLIGAME_ITEMS_H
#define SIMPLECLIGAME_ITEMS_H
#include <vector>
#include <random>
#include <cmath>
#include <string>

#include "json.hpp"

class Player;
class Enemy;

enum class Weapon_Name {
    EXCALIBUR,
    MJOLNIR,
    GUNGNIR,
    DURANDAL,
    GRAM,
    BALMUNG,
    CLARENT,
    ARONDIGHT,
    GALATINE,
    TYRFING
};

enum class Weapon_Type {
    SWORD = 10,
    AXE = 12,
    BAT = 7,
    MAGIC_WAND = 9,
    BOW = 13
};

enum class Damage_Type {
    NORMAL = 0,
    FIRE = 5,
    ICE = 2,
    ELECTRIC = 3,
    POISON = 4,
    HOLY = 1,
    BLOODY = 6
};

enum class Weapon_Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};


struct Weapon {
    //Weapon xD
    Weapon_Name weapon_name;
    Weapon_Type weapon_type;
    Weapon_Rarity weapon_rarity;
    Damage_Type damage_type;

    // Dealing damage to enemy
   int dealDamage_Player() const {
        return calculateTotalDamage_Player();
    }

   int dealDamage_Enemy() const {
       return calculateTotalDamage_Enemy();
   }

    // CalculatingTotalDamage
    int calculateTotalDamage_Player() const {
        int baseDamage = static_cast<int>(weapon_type);
        int bonusDamage = static_cast<int>(damage_type);
        float rarityMultiplier = getRarityMultiplier();

       float bonusDamageMultiplier = 1.0f + (0.1f * player_level);

        return static_cast<int>((baseDamage + bonusDamage) * rarityMultiplier * bonusDamageMultiplier);
    }

    int calculateTotalDamage_Enemy() const {
       int baseDamage = static_cast<int>(weapon_type);
       int bonusDamage = static_cast<int>(damage_type);
       float rarityMultiplier = getRarityMultiplier();

       float bonusDamageMultiplier = 1.0f + (0.1f * (enemy_level - (player_level * 0.5f)));

       return static_cast<int>((baseDamage + bonusDamage) * rarityMultiplier * bonusDamageMultiplier);
   }



    // Getting Rarity Mult
    float getRarityMultiplier() const {
        switch (weapon_rarity) {
            case Weapon_Rarity::COMMON: return 1.0f;
            case Weapon_Rarity::UNCOMMON: return 1.2f;
            case Weapon_Rarity::RARE: return 1.4f;
            case Weapon_Rarity::EPIC: return 1.6f;
            case Weapon_Rarity::LEGENDARY: return 2.5f;
                default: return 1.0f;
        }
    }

    // Getting player's level and Enemy's level
    int player_level = 1;
    int enemy_level = 1;

    void setPlayerLevel(int level) {
        player_level = level;
    }
    void setEnemyLevel(int level) {
        enemy_level = level;
    }
};

inline void to_json(nlohmann::json& j, const Weapon& w) {
    j = nlohmann::json{
            {"name", w.weapon_name},
            {"type", w.weapon_type},
            {"rarity", w.weapon_rarity},
            {"damage_type", w.damage_type}
    };
}

inline void from_json(const nlohmann::json& j, Weapon& w) {
    j.at("name").get_to(w.weapon_name);
    j.at("type").get_to(w.weapon_type);
    j.at("rarity").get_to(w.weapon_rarity);
    j.at("damage_type").get_to(w.damage_type);
}

class ItemGenerator {
    std::random_device rd;
    std::mt19937 gen;
public:
    ItemGenerator() : gen(rd()) {}

    //Generating weapon
    Weapon generateRandomWeapon() {
        // Tables
        std::vector<Weapon_Name> weapon_names = {
            Weapon_Name::EXCALIBUR, Weapon_Name::MJOLNIR, Weapon_Name::GUNGNIR,
            Weapon_Name::DURANDAL, Weapon_Name::GRAM, Weapon_Name::BALMUNG,
            Weapon_Name::CLARENT, Weapon_Name::ARONDIGHT, Weapon_Name::GALATINE,
            Weapon_Name::TYRFING
        };
        
        std::vector<Weapon_Type> weapon_types = {
            Weapon_Type::SWORD, Weapon_Type::AXE, Weapon_Type::BAT,
            Weapon_Type::MAGIC_WAND, Weapon_Type::BOW
        };
        
        std::vector<Damage_Type> damage_types = {
            Damage_Type::NORMAL, Damage_Type::FIRE, Damage_Type::ICE,
            Damage_Type::ELECTRIC, Damage_Type::POISON, Damage_Type::HOLY,
            Damage_Type::BLOODY
        };

        // generating random elements of the weapon
        std::uniform_int_distribution<> name_dist(0, weapon_names.size() - 1);
        std::uniform_int_distribution<> type_dist(0, weapon_types.size() - 1);
        std::uniform_int_distribution<> damage_dist(0, damage_types.size() - 1);
        std::discrete_distribution<> rarity_dist({50, 30, 15, 4, 1});

        return {
            weapon_names[name_dist(gen)],
            weapon_types[type_dist(gen)],
            static_cast<Weapon_Rarity>(rarity_dist(gen)),
            damage_types[damage_dist(gen)]
        };
    }

   static std::string weaponNameToString(Weapon_Name name);
   static std::string weaponTypeToString(Weapon_Type type);
   static std::string damageTypeToString(Damage_Type type);
   static std::string weaponRarityToString(Weapon_Rarity rarity);

    void displayWeapon(const Weapon& weapon);


};


#endif //SIMPLECLIGAME_ITEMS_H