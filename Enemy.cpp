//
// Created by Darko on 7.08.2025.
//

#include "Enemy.h"

#include <iostream>
#include <memory>
#include <random>
#include <utility>

Enemy::Enemy() { 
    currentWeapon = nullptr;
}

Enemy::Enemy(std::string name, int HP, int Level, const Weapon& weapon, int HP_MAX) {
    this->setName(std::move(name));
    this->setHP(HP);
    this->setLevel(Level);
    this->setWeapon(weapon);
    this->setHP_MAX(HP_MAX);
}

//Generate Enemy with random stats base on player stats
Enemy Enemy::GenerateEnemy(const Player &player, const Weapon &weapon) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> HPToAdd(1, 3);
    std::uniform_int_distribution<int> LvlToAdd(0, 3);
    std::uniform_int_distribution<int> Type(0, 3);


    auto typeEnemy = static_cast<Enemy::Type>(Type(gen));
    std::string typeName;
    switch (typeEnemy) {
        case Enemy::Type::Goblin: typeName = "Goblin"; break;
        case Enemy::Type::Orc:    typeName = "Orc";    break;
        case Enemy::Type::Troll:  typeName = "Troll";  break;
        case Enemy::Type::Dragon: typeName = "Dragon"; break;
    }

    int enemyLvl = std::max(1, player.getLevel() + LvlToAdd(gen));
    int enemyHP = static_cast<int>(100 + (enemyLvl * 10 / HPToAdd(gen))) + player.getHP() * 0.5;

    std::cout << "You encountered a : " << typeName
              << " (HP: " << enemyHP << ", Level: " << enemyLvl << ")!\n";

    return Enemy(typeName, enemyHP, enemyLvl, weapon, enemyHP);
}

int Enemy::dealDamage() {
    auto ChanceToHit = GenerateFloatNumber(0.0f, 1.0f);
    if (ChanceToHit >= 0.3f) {
        if (currentWeapon) {
            return currentWeapon->dealDamage_Enemy();
        }
    }else {
        return 0;
    }
    return 1;
}

void Enemy::takeDamage(int damage) {
    Player::takeDamage(damage);
}

void Enemy::Heal(int amount) {
    Player::Heal(amount);
}

void Enemy::HasBeenDefeated(Player& player) {
    std::cout << "================\n";
    std::cout << "\n";
    std::cout << "You killed an Enemy!\n";
    std::cout << "\n";
    std::cout << "================\n";

    auto minEXP = GenerateFloatNumber(10,30) * player.getLevel();
    auto maxEXP = GenerateFloatNumber(30,90) * player.getLevel();

    auto EXPToAdd = GenerateFloatNumber(minEXP, maxEXP);
    std::cout << "You gained " << EXPToAdd << " EXP!\n";


    player.setHealingCooldown(0);
    player.LevelUp(EXPToAdd);
}

void Enemy::AIWhileFightDecision(Player &player) {
    auto const AIHP = getHP();
    auto const AI_MAX_HP = getHP_MAX();
    auto choice = 1;
    auto ChanceToHeal = GenerateFloatNumber(0.0f, 1.0f);
    auto ChanceToAttack = GenerateFloatNumber(0.0f, 1.0f);
    if (getHealingCooldown() > 0) {
        canHeal = false;
    } else {
        canHeal = true;
    }
    if (AIHP < AI_MAX_HP && ChanceToHeal >= 0.6f && canHeal) {
        choice = GenerateIntNumber(2, 3);
    }
    else if (AIHP >= AI_MAX_HP && ChanceToAttack >= 0.7f) {
        choice = GenerateIntNumber(1, 2);
    }
    else if (ChanceToAttack >= 0.5f) {
        choice = 1;
    }
    std::cout << "\n";
    std::cout << "====="<< getName() << "'s" << " turn!" <<"=====\n";
    std::cout << "\n";
    switch (choice) {
        case 1: {
            std::cout << "Enemy is attacking!\n";
            std::cout << "\n";
            auto Damage = dealDamage();
            if (Damage == 0 || Damage == 1) {
                std::cout << "================\n";
                std::cout << "\n";
                std::cout << "Enemy missed!\n";
                std::cout << "\n";
                std::cout << "================\n";
                break;
            }
            player.takeDamage(Damage);
            std::cout << "Enemy deal: " << Damage << " damage to you!\n";
            std::cout << "\n";
            player.ShowPlayersStats();
            break;
        }
        case 2: {
            std::cout << getName() << " is... doing nothing... bruh\n";
            std::cout << "\n";
            break;
        }
        case 3: {
            std::cout << getName() << " is healing!\n";
            std::cout << "\n";
            HealEnemyByRandomNumber();
            setHealingCooldown(10);
            break;
        }
        default:
            std::cout << "ERROR IN AI WHILE FIGHT DECISION FUNCTION!\n";
            break;
    }
    decreaseHealingCooldown();

}

float Enemy::GenerateFloatNumber(float min, float max) {
    return Player::GenerateFloatNumber(min, max);
}

int Enemy::GenerateIntNumber(int min, int max) {
    return Player::GenerateIntNumber(min, max);
}

void Enemy::HealEnemyByRandomNumber() {
    auto CurrentHP = getHP();
    auto oldHP = CurrentHP;
    auto amount = GenerateIntNumber(5, (getHP_MAX() / 4));
    Heal(amount);
    int actualHealed = getHP() - oldHP;
    std::cout << "Enemy healed for " << actualHealed << " HP!\n";
    std::cout << "Current Enemy's HP: " << getHP() << " / " << getHP_MAX() << "\n";
    std::cout << "\n";
}




