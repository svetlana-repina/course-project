#include "battle.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <utility>

Battle::Battle()
    : turnCount(0),
      chooser1(std::make_unique<ConsoleActionChooser>()),
      chooser2(std::make_unique<ConsoleActionChooser>())
{
    std::cout << "Создан пустой бой\n";
}
Battle::Battle(std::shared_ptr<BaseCharacter> f1, std::shared_ptr<BaseCharacter> f2)
    : fighter1(f1), 
    fighter2(f2), 
    turnCount(0),
    chooser1(std::make_unique<ConsoleActionChooser>()),
    chooser2(std::make_unique<ConsoleActionChooser>())
{
    if (f1 == nullptr || f2 == nullptr)
    {
        std::cout << "Ошибка! Оба участника должны быть заданы.\n";
        return;
    }
    std::cout << "Начался бой между " << f1->GetName() << " и " << f2->GetName() << "\n";
}

Battle::Battle(std::shared_ptr<BaseCharacter> f1, std::shared_ptr<BaseCharacter> f2, std::unique_ptr<IActionChooser> c1, std::unique_ptr<IActionChooser> c2)
    : fighter1(f1), 
    fighter2(f2), 
    turnCount(0),
    chooser1(std::move(c1)),
    chooser2(std::move(c2))
{
    if (f1 == nullptr || f2 == nullptr)
    {
        std::cout << "Ошибка! Оба участника должны быть заданы.\n";
        return;
    }
    std::cout << "Начался бой между " << f1->GetName() << " и " << f2->GetName() << "\n";
}

Battle::~Battle() noexcept
{
    std::cout << "Бой завершён и уничтожен\n";
}

void Battle::setChoosers(std::unique_ptr<IActionChooser> c1, std::unique_ptr<IActionChooser> c2)
{
    if (c1) chooser1 = std::move(c1);
    if (c2) chooser2 = std::move(c2);
}

std::shared_ptr<BaseCharacter> Battle::getWinner() const
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
    bool f1IsAlive = f1->IsAlive();
    bool f2IsAlive = f2->IsAlive();
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
    return !f1->IsAlive() || !f2->IsAlive();
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
    auto actor = (turnCount % 2 == 1) ? f1 : f2;
    auto target = (turnCount % 2 == 1) ? f2 : f1;
    IActionChooser* chooser = (turnCount % 2 == 1) ? chooser1.get() : chooser2.get();
    const ActionType action = chooser->ChooseAction(*actor);

    try
    {
        switch(action)
        {
            case ActionType::Attack: actor->Attack(*target); break;
            case ActionType::Skill: actor->UseSkill(*target); break;
            case ActionType::Skip: actor->SkipTurn(); break;
        }
    }

    catch (const NotEnoughMana& error)
    {
        std::cout << "Перехвачено: " << error.what() << ", ход пропущен\n";
        actor->SkipTurn();
    }

    catch (const NotEnoughStamina& error)
    {
        std::cout << "Перехвачено: " << error.what() << ", ход пропущен\n";
        actor->SkipTurn();
    }

    catch (const std::exception& error)
    {
        std::cout << "Ошибка: " << error.what() << "\n";
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
        std::cout << winner->GetName() << " победил за " << turnCount << " ходов\n";
    }
    else 
    {
        std::cout << "Ничья. Оба бойца умерли\n";
    }
}

void Battle::Describe() const
{
    auto f1 = fighter1.lock();
    auto f2 = fighter2.lock();
    std::cout << "======== Текущее состояние боя (ход " << turnCount << ") ========\n";
    if (f1 != nullptr) 
    {
        std::cout << f1->Describe() << "\n";
    }
    if (f2 != nullptr) 
    {
        std::cout << f2->Describe() << "\n";
    }
    std::cout << "===============================================\n";
}