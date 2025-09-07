//
// Created by Darko on 7.08.2025.
//

#ifndef SIMPLECLIGAME_PLAYER_H
#define SIMPLECLIGAME_PLAYER_H
#include <string>
#include <utility>
#include <random>
#include "Inventory.h"


class Enemy;
struct Weapon;

class Player {
public:
    // Constructors and Descructors
    virtual ~Player() = default;
    Player();
    Player(const Player& other);

    // Setters
    void setName(std::string value) {
        this->name = std::move(value);
    }
    void setHP(const int value) {
        this->HP = value;
    }
    void setEXP(const float value) {
        this->EXP = value;
    }
    void setEXP_MAX(const float value) {
        this->EXP_MAX = value;
    }
    void setLevel(const int value) {
        this->Level = value;
    }
    void setIsDead(bool value) {
        this->isDead = value;
    }
    void setHP_MAX(const int value) {
        this->HP_MAX = value;
    }
    void setHealingCooldown(int value) {
        HealingCooldown = value;
    }
    void setEscaped(bool value) {
        Escaped = value;
    }
    void setWeapon(const Weapon& weapon);

    // Getters
    std::string getName() const {
        return name;
    }
    int getHP() const {
        return HP;
    }
    float getEXP() const {
        return EXP;
    }
    float getEXP_MAX() const {
        return EXP_MAX;
    }
    int getLevel() const {
        return Level;
    }
    bool getIsDead() const {
        return isDead;
    }
    int getHP_MAX() const {
        return HP_MAX;
    }
    int getHealingCooldown() const {
        return HealingCooldown;
    }
    bool getEscaped() const {
        return Escaped;
    }
    bool getHasWeapon() const {
        return hasWeapon;
    }
    const Weapon& getCurrentWeapon() const;

    void decreaseHealingCooldown() {
        if (HealingCooldown > 0) {
            HealingCooldown--;
        }
    }

protected:
    std::string name;
    int HP;
    std::unique_ptr<Weapon> currentWeapon;
    int HealingCooldown = 0;
private:
    float EXP;
    float EXP_MAX;
    int Level;
    int Level_MAX;
    int HP_MAX;
    bool hasWeapon = false;
    bool isDead = false;
    bool Escaped = false;

    void EquipItem(int index);


    Inventory inv;

    std::vector<std::string> FightDecision{
        "Attack",
        "Heal",
        "Run!!"
    };


public:
    void PlayerWalk();
    void PlayerInventory();
    void PlayerRest();
    int PlayerFightDecision();
    void ShowPlayersStats();

    void EquipItemOnLoad(int index);

    Weapon GenerateWeaponWhileSearch();

    const Inventory& getInventory() const {
        return inv;
    }
    Inventory& getInventory() {
        return inv;
    }



    void LevelUp(float ExpToAdd);
    virtual void Heal(int amount);
    virtual int dealDamage();
    virtual void takeDamage(int damage);

    virtual float GenerateFloatNumber(float min, float max);
    virtual int GenerateIntNumber(int min, int max);

};


#endif //SIMPLECLIGAME_PLAYER_H