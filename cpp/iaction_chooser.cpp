#include "iaction_chooser.hpp"
#include "base_character.hpp"
#include <iostream>
#include <limits>
#include <random>

namespace
{
    std::mt19937& generator()
    {
        static std::mt19937 gen(std::random_device{}());
        return gen;
    }
}

ActionType ConsoleActionChooser::ChooseAction(const BaseCharacter& hero) const
{
    while(true)
    {
        std::cout << hero.GetName()
                  << ", выберите действие: \n"
                  << "1. Атака\n"
                  << "2. Навык\n"
                  << "3. Пропуск хода\n";

        int choice = 0;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Введите число\n";
            continue;
        }

        switch(choice)
        {
            case 1: return ActionType::Attack;
            case 2: return ActionType::Skill;
            case 3: return ActionType::Skip;
            default: std::cout << "Такого действия нет, введите другое число\n";
        }
    }
}

ActionType RandomActionChooser::ChooseAction(const BaseCharacter&) const
{
    std::uniform_int_distribution<int> dist(0, 2);
    return static_cast<ActionType>(dist(generator()));
}