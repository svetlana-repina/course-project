#pragma once
#include "character.hpp"
#include <memory>

class Battle
{
    private:
        std::weak_ptr<Character> fighter1;
        std::weak_ptr<Character> fighter2;
        int turnCount;

    public:
        Battle();
        Battle(std::shared_ptr<Character> f1, std::shared_ptr<Character> f2);
        ~Battle();

        [[nodiscard]] std::shared_ptr<Character> getWinner() const;
        [[nodiscard]] bool isFinished() const;
        [[nodiscard]] int getTurnCount() const;
        void performTurn();
        void printWinner() const;
        void printState() const;
};