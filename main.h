//
// Created by Darko on 7.08.2025.
//

#ifndef SIMPLECLIGAME_MAIN_H
#define SIMPLECLIGAME_MAIN_H
#include <string>

#include "Enemy.h"

class Game {
    void HealPlayerByRandomNumber(Player &player);
    public:
    void StartGame(Player &ptr);

    private:
    std::string action[5]={
        "walk",
        "inventory",
        "rest",
        "Save Game",
        "Quit Game"
    };
    int Walked = 0;
public:

    bool LoadGame(Player &player);
    void QuitGame();
    void GameOver(Player &player);
    void SaveGame(const Player &player);
    void StartFight(Player &player);
    void PlayerIsWalking(Player& player);


    float GenerateFloatNumber(float min, float max);
    int GenerateIntNumber(int min, int max);


};


#endif //SIMPLECLIGAME_MAIN_H