#pragma once
#include "character.hpp"

class Battle
{
    private:
        Character* fighter1;
        Character* fighter2;
        int turnCount;

    public:
        Battle();
        Battle(Character* f1, Character* f2);
        ~Battle();

        [[nodiscard]] Character* getWinner() const;
        [[nodiscard]] bool isFinished() const;
        [[nodiscard]] int getTurnCount() const;
        void performTurn();
        void printWinner() const;
        void printState() const;
};