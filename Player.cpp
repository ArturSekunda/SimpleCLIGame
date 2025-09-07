//
// Created by Darko on 7.08.2025.
//
#include "Player.h"
#include "Items.h"
#include <limits>
#include <iostream>



Player::Player() {
    HP = 100;
    HP_MAX = 100;
    EXP = 0;
    EXP_MAX = 100;
    Level = 1;
    Level_MAX = 100;

    static Weapon defaultWeapon = {
        Weapon_Name::EXCALIBUR,
        Weapon_Type::SWORD,
        Weapon_Rarity::COMMON,
        Damage_Type::BLOODY,
    };
    currentWeapon = std::make_unique<Weapon>(defaultWeapon);
    hasWeapon = true;

}

Player::Player(const Player& other)
    : name(other.name), HP(other.HP), EXP(other.EXP),
      EXP_MAX(other.EXP_MAX), Level(other.Level),
      Level_MAX(other.Level_MAX), HP_MAX(other.HP_MAX), hasWeapon(other.hasWeapon),
      isDead(other.isDead), FightDecision(other.FightDecision) {
    if (other.currentWeapon) {

        currentWeapon = std::make_unique<Weapon>(*other.currentWeapon);
    }

    for (int i = 0; i < other.inv.GetSizeofInv(); i++) {
        // getting weapon ptr
       const Weapon* weaponptr = other.inv.GetWeaponAtIndex(i);

        //checkand then apply weapon to the new player class
        if (weaponptr != nullptr) {
            inv.AddItem(*weaponptr);
        }
    }

}


void Player::setWeapon(const Weapon& weapon) {
    currentWeapon = std::make_unique<Weapon>(weapon);
    hasWeapon = true;
}

const Weapon& Player::getCurrentWeapon() const {
    if (currentWeapon) {
        return *currentWeapon;
    }

    throw std::runtime_error("No weapon available!");
}


Weapon Player::GenerateWeaponWhileSearch() {
    ItemGenerator weapongen;
    Weapon newWeapon = weapongen.generateRandomWeapon();

    weapongen.displayWeapon(newWeapon);
    return newWeapon;
}

void Player::LevelUp(float ExpToAdd) {
    EXP += ExpToAdd;
    // Checking EXP
    if (EXP < EXP_MAX) {
        std::cout << "You need " << EXP_MAX - EXP << " EXP to level up!\n";
        std::cout << "Current EXP: " << getEXP() << "/" << getEXP_MAX() << "\n";
    }else {
        do {
            // Then we're adding levels to player stats in loop if player has more
            // EXP even if we do math for EXP_MAX
            Level++;
            float levelMultiplier = std::min(2.0f, 1.f + (getLevel() * 0.3f));
            int oldMaxHP = HP_MAX;
            int baseIncrease = 25;
            int levelBonus = (Level / 5) * 5;
            int totalIncrease = baseIncrease + levelBonus;

            HP_MAX += totalIncrease;
            HP += totalIncrease;

            EXP_MAX *= levelMultiplier;

            std::cout << "================\n";
            std::cout << "You leveled up!\n";
            std::cout << "You are now level " << Level << "!\n";
            std::cout << "Max HP increased by " << totalIncrease << " (from " << oldMaxHP << " to " << HP_MAX << ")!\n";
            std::cout << "================\n";
        }while (EXP >= EXP_MAX);
    }
}

void Player::Heal(int amount) {
    HP = std::min(HP_MAX, HP + amount);
}

void Player::PlayerWalk() {
    std::cout << "Walking...\n";
    auto Chance = GenerateFloatNumber(-1.f,2.f);
    if (Chance >= 0.5f && Chance <= 1.f) {
      inv.AddItem(GenerateWeaponWhileSearch());
    }
}

void Player::EquipItem(int index) {

    const Weapon* weaponPtr = inv.GetWeaponAtIndex(index);

    if (weaponPtr != nullptr) {
        setWeapon(*weaponPtr);

        inv.MarkAsUsed(index);

        inv.ShowInventory();
        std::cout << "Item equipped!\n";
    } else {
        std::cout << "Invalid index!\n";
    }

}

void Player::PlayerInventory() {
    auto table = inv.IfEmptyTable();
    inv.ShowInventory();
    std::cout << "\n";
    if (table) {
        return;
    }
    do {
        std::cout << "1. Equipt item\n2. Delete item\n3. Return\n";
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!\n";
            continue;
        }
        if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice!\n";
            continue;
        }
        switch (choice) {
            case 1: {
                std::cout << "Enter ID of the item to equip\n";
                int index = 0;
                if (!(std::cin >> index)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input!\n";
                    continue;
                }
                if (index < 0 || index > inv.GetSizeofInv()) {
                    std::cout << "Invalid ID!\n";
                    continue;
                }
                if (inv.IsItemUsed(index)) {
                    std::cout << "Item is already equipped!\n";
                    break;
                }

                EquipItem(index);
                break;
            }
            case 2: {
                std::cout << "Enter item ID to delete\n";
                int index = 0;
                std::cout << "If you want to return type: -1 \n";
                if (!(std::cin >> index)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input!\n";
                    continue;
                }
                if (index == -1) {
                    break;
                }
                if (index < -1 || index > inv.GetSizeofInv()) {
                    std::cout << "Invalid ID!\n";
                    continue;
                }
                if (inv.IsItemUsed(index)) {
                    std::cout << "Item is already equipped!\n";
                    break;
                }
                inv.DeleteItem(index);
                std::cout << "Item deleted!\n";

                break;
            }
            case 3:
                return;
            default: {
                std::cout << "ERROR IN PLAYER INVENTORY FUNCTION!\n";
                break;
            }

        }
    }while (true);
}

void Player::PlayerRest() {
    std::cout << "Resting...\n";

    if (HP == HP_MAX) {
        std::cout << "You are already full of energy!... and also healed.\n";
        return;
    }

    Heal(HP_MAX);

    std::cout << "You feel much better now!\n";
}

int Player::PlayerFightDecision() {

    std::cout << "====="<<"Your turn"<<"=====\n";
    int choice = 0;
    do {
        std::printf("1. Attack\n2. Heal\n3. Run\n");
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!\n";
            continue;
        }
        if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice!\n";
            continue;
        }

        std::cout << "You chose: " << FightDecision[choice - 1] << "\n";
        std::cout << "\n";
        break;
    }while (true);
    return choice;
}

void Player::ShowPlayersStats() {
    std::cout << "YOU\n";
    std::cout << "HP: " << HP << "/" << HP_MAX << "\n";
    std::cout << "EXP: " << EXP << "/" << EXP_MAX << "\n";
    std::cout << "Level: " << Level << "/" << Level_MAX << "\n";
    std::cout << "\n";
}

void Player::EquipItemOnLoad(int index) {

    const Weapon* weaponPtr = inv.GetWeaponAtIndex(index);

    if (weaponPtr != nullptr) {
        setWeapon(*weaponPtr);

        inv.MarkAsUsed(index);

    }

}

int Player::dealDamage() {
    auto ChanceToHit = GenerateFloatNumber(0.0f, 1.0f);
    if (ChanceToHit >= 0.15f) {
        if (hasWeapon && currentWeapon) {
            auto baseDamage = currentWeapon->dealDamage_Player();
            return baseDamage;
        }
    }else {
        return 0;
    }
    return 1;
}

void Player::takeDamage(int damage) {
    HP = std::max(0, HP - damage);
}

float Player::GenerateFloatNumber(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int Player::GenerateIntNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}
