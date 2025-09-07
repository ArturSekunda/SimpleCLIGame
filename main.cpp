//
// Created by Darko on 7.08.2025.
//

#include "main.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

using json = nlohmann::json;

int main() {

    auto player_ptr = std::make_unique<Player>();
    auto game = std::make_unique<Game>();


    std::string character_name;
    std::cout << "Simple CLI Game!\n";
    if (!game->LoadGame(*player_ptr.get())) {
        std::cout << "Creating new character...\n";
        std::cout << "Enter name for the character: ";
        std::cin >> character_name;
        player_ptr->setName(character_name);
        char choice;
        do {
            std::cout << "Are you sure you want to name your character: " << player_ptr->getName() << "? (y/n): ";
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') {
                std::cout << "Enter name for the character: ";
                std::cin >> character_name;
                player_ptr->setName(character_name);
                break;
            }
        } while (choice != 'y' && choice != 'Y');

        game->StartGame(*player_ptr.get());
    }else {
        game->StartGame(*player_ptr.get());
    }


    return 0;
}

void Game::StartGame(Player& ptr) {
    std::cout << "Welcome to the game, " << ptr.getName() << "!\n";
    std::cout << "What action do you want to take? (1, 2, 3, 4, 5)\n";
    int choice = 0;
    do {
        std::printf("1. Walk\n2. Inventory\n3. Rest\n4. Save Game\n5. Quit Game\n");
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input!\n";
            continue;
        }
        if (choice < 1 || choice > 5) {
            std::cout << "Invalid choice!\n";
            continue;
        }
        std::cout << "You chose: " << action[choice - 1] << "\n";
        switch (choice-1) {
            case 0:
                PlayerIsWalking(ptr);
                break;
            case 1:
                ptr.PlayerInventory();
                break;
            case 2:
                ptr.PlayerRest();
                break;
            case 3:
                SaveGame(ptr);
                break;
            case 4:
                SaveGame(ptr);
                QuitGame();
                break;
            default:
                std::cout << "Invalid choice!\n";
                break;
        }
    } while (true);
}

// Saving, Loading, Quiting game
bool Game::LoadGame(Player &player) {
    std::ifstream saveFile("savegame.json");
    if (!saveFile.is_open()) {
        std::cout << "File not found!\n";
        return false;
    }
    json saveData;
    saveFile >> saveData;
    saveFile.close();
    player.setName(saveData["name"]);
    player.setHP(saveData["HP"]);
    player.setHP_MAX(saveData["HP_MAX"]);
    player.setEXP(saveData["EXP"]);
    player.setLevel(saveData["Level"]);
    player.setEXP_MAX(saveData["EXP_MAX"]);
    if (saveData.contains("Inventory")) {
        player.getInventory() = saveData.at("Inventory").get<Inventory>();
    }
    if (saveData.contains("Current_Weapon")) {
        int equippedIndex = saveData.at("Current_Weapon").get<int>();
        if (equippedIndex != -1) {
            player.EquipItemOnLoad(equippedIndex);
        }
    }
    std::cout << "Game loaded!\n";
    std::cout << "Welcome back" << " " << player.getName() << "!\n";
    return true;
}

void Game::SaveGame(const Player& player) {
    json saveData;
    std::cout << "================\n";
    std::cout << "Saving game...\n";
    std::cout << "================\n";
    saveData["name"] = player.getName();
    saveData["HP"] = player.getHP();
    saveData["HP_MAX"] = player.getHP_MAX();
    saveData["EXP"] = player.getEXP();
    saveData["Level"] = player.getLevel();
    saveData["EXP_MAX"] = player.getEXP_MAX();
    saveData["Inventory"] = player.getInventory();
    saveData["Current_Weapon"] = player.getInventory().GetEquippedItemIndex();
    std::ofstream saveFile("savegame.json");
    saveFile << saveData.dump(4);
    saveFile.close();
    std::cout << "Game saved!\n";
}

void Game::QuitGame() {
    std::cout << "Quitting game...\n";
    exit(0);
}

// Player Controll
void Game::StartFight(Player &player) {
    ItemGenerator ItemForEnemy;
    Weapon newWeapon = ItemForEnemy.generateRandomWeapon();
    Enemy enemy = Enemy::GenerateEnemy(player, newWeapon);
    newWeapon.enemy_level = enemy.getLevel();
    newWeapon.player_level = player.getLevel();
    std::cout << "================\n";
    std::cout << "\n";
    std::cout << "Fighting...\n";
    std::cout << "\n";
    std::cout << "================\n";
    do {
        auto decition = player.PlayerFightDecision();

        switch (decition) {
            case 1:{
                const int damage = player.dealDamage();

                if (damage == 0 || damage == 1) {
                    std::cout << "================\n";
                    std::cout << "\n";
                    std::cout << "You missed!\n";
                    std::cout << "\n";
                    std::cout << "================\n";
                    break;
                }
                enemy.takeDamage(damage);
                const int EnemyHealth = enemy.getHP();
                std::cout << "You deal " << damage << " damage to the enemy!\n";
                std::cout << "Enemy HP: " << EnemyHealth << "\n";
                std::cout << "\n";
                break;
            }
            case 2:{
                if (player.getHealingCooldown() > 0) {
                    std::cout << "You are on cooldown! (" << player.getHealingCooldown() << " turs more)\n";
                    std::cout << "\n";
                    break;
                }
                auto PlayerHealth = player.getHP();
                auto PlayerMaxHealth = player.getHP_MAX();

                if (PlayerHealth >= PlayerMaxHealth) {
                    std::cout << "You are already at full HP!\n";
                    std::cout << "\n";
                    break;
                }
                HealPlayerByRandomNumber(player);
                player.setHealingCooldown(5);
                break;
            }
            case 3: {
                auto ChanceToRun = GenerateFloatNumber(0.0f, 1.0f);
                if (ChanceToRun >= 0.6f) {
                    std::cout << "You ran away!\n";
                    std::cout << "\n";
                    player.setEscaped(true);
                    break;
                }

                std::cout << "You didn't escaped!\n";
                std::cout << "\n";

                break;
            }
            default: {
                std::cout << "ERROR IN STARTFIGHT FUNCTION (SWITCH)!\n";
                break;
            }
        }
        player.decreaseHealingCooldown();
        if (!player.getEscaped()) {
            enemy.AIWhileFightDecision(player);
        }


    }while (player.getHP() > 0 && enemy.getHP() > 0 && !player.getEscaped());

    if (player.getHP() <= 0) {
        player.setIsDead(true);
        GameOver(player);
    }

    if (enemy.getHP() <= 0) {
        auto enemyWeapon = enemy.getCurrentWeapon();
        enemy.HasBeenDefeated(player);
        player.getInventory().AddItem(enemyWeapon);
        std::cout << "================\n";
        std::cout << "You got a new Weapon from killing " << enemy.getName() << "!\n";
        std::cout << "================\n";
    }

    if (player.getEscaped()) {
        player.setEscaped(false);
    }

}

void Game::PlayerIsWalking(Player& player) {
    player.PlayerWalk();
    Walked++;
    float chanceToFight = GenerateFloatNumber(0.0f, 1.0f) + ((Walked + GenerateIntNumber(0,5)) * 0.05f);
    if (chanceToFight >= 1.f || Walked >= 10) {
        StartFight(player);
       Walked = 0;
 }
}

void Game::GameOver(Player &player) {
    if (player.getIsDead()) {
        std::cout << "\n";
        std::cout << "Game over!\n";
        QuitGame();
    }
}

void Game::HealPlayerByRandomNumber(Player &player) {
    auto PlayerMAXHP = player.getHP_MAX();
    auto PlayerCurrentHP = player.getHP();
    auto oldHP = PlayerCurrentHP;
    auto amount = GenerateIntNumber(PlayerCurrentHP, PlayerMAXHP);
    player.Heal(amount);
    int actualHealed = player.getHP() - oldHP;
    std::cout << "================\n";
    std::cout << "\n";
    std::cout << "You healed for " << actualHealed << " HP!\n";
    std::cout << "Current HP: " << player.getHP() << " / " << PlayerMAXHP << "\n";
    std::cout << "\n";
    std::cout << "================\n";
}

// Utility tools
float Game::GenerateFloatNumber(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int Game::GenerateIntNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}
