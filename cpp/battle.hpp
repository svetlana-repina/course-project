#pragma once
#include "base_character.hpp"
#include <memory>

class Battle
{
    private:
        std::weak_ptr<BaseCharacter> fighter1;
        std::weak_ptr<BaseCharacter> fighter2;
        int turnCount;

    public:
        Battle();
        Battle(std::shared_ptr<BaseCharacter> f1, std::shared_ptr<BaseCharacter> f2);
        ~Battle();

        [[nodiscard]] std::shared_ptr<BaseCharacter> getWinner() const;
        [[nodiscard]] bool isFinished() const;
        [[nodiscard]] int getTurnCount() const;
        void performTurn();
        void printWinner() const;
        void Describe() const;
};