//
// Created by Darko on 7.08.2025.
//

#ifndef SIMPLECLIGAME_ENEMY_H
#define SIMPLECLIGAME_ENEMY_H
#include "Player.h"
#include "Items.h"


class Enemy : public Player {
public:
    Enemy();
    Enemy(std::string name, int HP, int Level, const Weapon& weapon, int HP_MAX);

    //In this static function we're generating an enemy and we're returning it with return statment
    static Enemy GenerateEnemy(const Player& player, const Weapon& weapon);

    enum class Type {
        Goblin,
        Orc,
        Troll,
        Dragon
    };
    int dealDamage() override;
    void takeDamage(int damage) override;
    void Heal(int amount) override;

    void HasBeenDefeated(Player& player);

    void AIWhileFightDecision(Player& player);

private:
    std::vector<std::string> AIFightDecision{
        "Attack",
        "Do Nothing",
        "Heal"
    };
    bool canHeal = true;

    float GenerateFloatNumber(float min, float max) override;
    int GenerateIntNumber(int min, int max) override;

    void HealEnemyByRandomNumber();
};


#endif //SIMPLECLIGAME_ENEMY_H