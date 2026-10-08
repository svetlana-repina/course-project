#include "battle.hpp"
#include <iostream>

Battle::Battle()
    : fighter1(nullptr), 
    fighter2(nullptr), 
    turnCount(0)
{
    std::cout << "Создан пустой бой\n";
}
Battle::Battle(Character* f1, Character* f2)
    : fighter1(f1), 
    fighter2(f2), 
    turnCount(0)
{
    if (f1 == nullptr || f2 == nullptr)
    {
        std::cout << "Ошибка! Оба участника должны быть заданы.\n";
        return;
    }
    std::cout << "Начался бой между " << f1->getName() << " и " << f2->getName() << "\n";
}
Battle::~Battle()
{
    std::cout << "Бой завершён и уничтожен\n";
}
Character* Battle::getWinner() const
{
    if (fighter1 == nullptr || fighter2 == nullptr) 
    {
        return nullptr;
    }
    if (!isFinished()) 
    {
        return nullptr;
    }
    bool f1IsAlive = fighter1->isAlive();
    bool f2IsAlive = fighter2->isAlive();
    if (!f1IsAlive && !f2IsAlive) 
    {
        return nullptr;
    }
    if (f1IsAlive) 
    {
        return fighter1;
    }
    else 
    {
        return fighter2;
    }
}
bool Battle::isFinished() const
{
    if (fighter1 == nullptr || fighter2 == nullptr) 
    {
        return true;
    }
    return !fighter1->isAlive() || !fighter2->isAlive();
}
int Battle::getTurnCount() const
{
    return turnCount;
}
void Battle::performTurn()
{
    if (isFinished()) 
    {
        std::cout << "Бой уже завершён\n";
        return;
    }
    turnCount++;
    std::cout << "-------- Ход " << turnCount << " --------\n";
    if (turnCount%2 != 0) 
    {
        fighter1->attack(*fighter2);
    }
    else 
    {
        fighter2->attack(*fighter1);
    }
}
void Battle::printWinner() const
{
    if (!isFinished())
    {
        std::cout << "Бой ещё не закончен\n";
        return;
    }
    Character* winner = getWinner();
    if (winner!=nullptr)
    {
        std::cout << winner->getName() << " победил за " << turnCount << " ходов\n";
    }
    else 
    {
        std::cout << "Ничья. Оба бойца умерли\n";
    }
}
void Battle::printState() const
{
    std::cout << "======== Текущее состояние боя (ход " << turnCount << ") ========\n";
    if (fighter1 != nullptr) 
    {
        fighter1->printState();
    }
    if (fighter2 != nullptr) 
    {
        fighter2->printState();
    }
    std::cout << "===============================================\n";
}