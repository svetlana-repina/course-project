#pragma once
#include "base_character.hpp"
#include "iaction_chooser.hpp"
#include <memory>

class Battle
{
    private:
        std::weak_ptr<BaseCharacter> fighter1;
        std::weak_ptr<BaseCharacter> fighter2;
        int turnCount;
        std::unique_ptr<IActionChooser> chooser1;
        std::unique_ptr<IActionChooser> chooser2;

    public:
        Battle();
        Battle(std::shared_ptr<BaseCharacter> f1, std::shared_ptr<BaseCharacter> f2);
        Battle(std::shared_ptr<BaseCharacter> f1, std::shared_ptr<BaseCharacter> f2, std::unique_ptr<IActionChooser> c1, std::unique_ptr<IActionChooser> c2);
        ~Battle() noexcept;

        void setChoosers(std::unique_ptr<IActionChooser> c1, std::unique_ptr<IActionChooser> c2);
        [[nodiscard]] std::shared_ptr<BaseCharacter> getWinner() const;
        [[nodiscard]] bool isFinished() const;
        [[nodiscard]] int getTurnCount() const;
        void performTurn();
        void printWinner() const;
        void Describe() const;
};