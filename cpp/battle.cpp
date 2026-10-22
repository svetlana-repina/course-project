#include "battle.hpp"
#include <iostream>

Battle::Battle()
    : turnCount(0)
{
    std::cout << "Создан пустой бой\n";
}
Battle::Battle(std::shared_ptr<Character> f1, std::shared_ptr<Character> f2)
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
std::shared_ptr<Character> Battle::getWinner() const
{
    auto f1 = fighter1.lock();
    auto f2 = fighter2.lock();
    if (f1 == nullptr || f2 == nullptr) 
    {
        return nullptr;
    }
    if (!isFinished()) 
    {
        return nullptr;
    }
    bool f1IsAlive = f1->isAlive();
    bool f2IsAlive = f2->isAlive();
    if (!f1IsAlive && !f2IsAlive) 
    {
        return nullptr;
    }
    if (f1IsAlive) 
    {
        return f1;
    }
    else 
    {
        return f2;
    }
}
bool Battle::isFinished() const
{
    auto f1 = fighter1.lock();
    auto f2 = fighter2.lock();
    if (f1 == nullptr || f2 == nullptr) 
    {
        return true;
    }
    return !f1->isAlive() || !f2->isAlive();
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
    auto f1 = fighter1.lock();
    auto f2 = fighter2.lock();
    turnCount++;
    std::cout << "-------- Ход " << turnCount << " --------\n";
    if (turnCount % 2 != 0) 
    {
        f1->attack(*f2);
    }
    else 
    {
        f2->attack(*f1);
    }
}
void Battle::printWinner() const
{
    if (!isFinished())
    {
        std::cout << "Бой ещё не закончен\n";
        return;
    }
    auto winner = getWinner();
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
    auto f1 = fighter1.lock();
    auto f2 = fighter2.lock();
    std::cout << "======== Текущее состояние боя (ход " << turnCount << ") ========\n";
    if (f1 != nullptr) 
    {
        f1->printState();
    }
    if (f2 != nullptr) 
    {
        f2->printState();
    }
    std::cout << "===============================================\n";
}