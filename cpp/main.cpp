#include <iostream>
#include <memory>
#include <windows.h>
#include "battle.hpp"
#include "warrior.hpp"
#include "mage.hpp"
#include "priest.hpp"
#include "rogue.hpp"
#include "exceptions.hpp"
#include "imana_user.hpp"
#include "iaction_chooser.hpp"

void setConsoleUTF8() 
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

namespace
{
    void TryRecoverMana(BaseCharacter& character, int amount)
    {
        if (auto* manaUser = dynamic_cast<IManaUser*>(&character))
        {
            manaUser->RestoreMana(amount);
            std::cout << "через интерфейс IManaUser восстановлено " << amount << ", мана: " << manaUser->GetMana() << "/" << manaUser->GetMaxMana() << "\n";
        }
        else
        {
            std::cout << "интерфейс IManaUser не реализован\n";
        }
    }
}

int main() 
{
    setConsoleUTF8();

    auto sword = std::make_shared<Weapon>("Меч", 15, DamageType::Physical, 100, 0.2, 2.5);
    auto staff1 = std::make_shared<Weapon>("Посох", 12, DamageType::Magical, 100, 0.25, 3.0);
    auto staff2 = std::make_shared<Weapon>("Посох", 12, DamageType::Magical, 100, 0.25, 3.0);

    Warrior warrior("Воин", 150, sword, 10);
    Mage mage("Маг", 100, staff1, 35);
    Priest priest("Жрец", 90, staff2, 25);

    std::cout << "\n========= РАННЕЕ И ПОЗДНЕЕ СВЯЗЫВАНИЕ: =========\n";
    BaseCharacter& character = warrior;
    std::cout << "Раннее связывание (невиртуальный метод GetKind()):\n";
    std::cout << character.GetKind() << "\n";
    std::cout << "Позднее связывание (виртуальный метод Describe()):\n";
    std::cout << character.Describe() << "\n";

    std::cout << "\n========= ИСПОЛЬЗОВАНИЕ ИНТЕРФЕЙСА: =========\n";
    TryRecoverMana(mage, 5);
    TryRecoverMana(priest, 5);
    TryRecoverMana(warrior, 5);

    std::cout << "\n========= ИСКЛЮЧЕНИЯ: =========\n";
    std::cout << "--- Не хватает маны: ---\n";
    std::cout << "используем навык мага несколько раз\n";
    try
    {
        for (int i = 0; i < 2; i++)
        {
            mage.UseSkill(warrior);
        }
    }
    
    catch (const NotEnoughMana& error)
    {
        std::cout << "Перехвачено: " << error.what() << ", ход пропущен\n";
        mage.SkipTurn();
    }

    catch (const std::exception& error)
    {
        std::cout << "Ошибка: " << error.what() << "\n";
    }

    std::cout << "\n--- Не хватает выносливости: ---\n";
    Mage dummy("Болванчик", 1000, staff1, 100);
    try
    {
        for (int i = 0; i < 6; ++i)
        {
            std::cout << "Попытка использовать навык №" << (i + 1) << ":\n";
            warrior.UseSkill(dummy);
        }
    }

    catch (const NotEnoughStamina& error)
    {
        std::cout << "Перехвачено: " << error.what() << "\n";
        warrior.SkipTurn();
    }

    catch (const std::exception& error)
    {
        std::cout << "Ошибка: " << error.what() << "\n";
    }

    std::cout << "\n--- Недопустимые параметры констуктора: ---\n";
    try
    {
        std::cout << "Попытка создать воина с maxHealth = -10:\n";
        Warrior bad("Плохой", -10, sword, 5);
    }

    catch (const InvalidCharacterParameter& error)
    {
        std::cout << "Не удалось создать персонажа: " << error.what() << "\n";
    }

    try
    {
        std::cout << "Попытка создать мага с maxMana = 0:\n";
        Mage badMage("ПлохойМаг", 100, staff1, 0);
    }

    catch (const InvalidCharacterParameter& error)
    {
        std::cout << "Не удалось создать персонажа: " << error.what() << "\n";
    }

    std::cout << "\n--- Действие мёртвого персонажа: ---\n";
    Mage newMage("Новый Маг", 100, staff1, 35);
    try
    {
        while(warrior.IsAlive())
        {
            warrior.TakeDamage(1000);
        }
        std::cout << "Воин мёртв, пытаемся заставить его атаковать:\n";
        warrior.Attack(newMage);
    }
    catch (const CharacterIsDead& error)
    {
        std::cout << "Перехвачено: " << error.what() << "\n";
    }

    std::cout << "\n--- Попытка атаковать мёртвого персонажа: ---\n";
    try
    {
        std::cout << "Пытаемся заставить мага атаковать мёртвого воина:\n";
        newMage.Attack(warrior);
    }
    catch (const CharacterIsDead& error)
    {
        std::cout << "Перехвачено: " << error.what() << "\n";
    }


    std::cout << "\n========= КОНФИГУРИРОВАНИЕ: =========\n";
    std::cout << "--- Random vs random: ---\n";
    auto warrior2 = std::make_shared<Warrior>("Воин2", 100, sword, 10);
    auto mage2 = std::make_shared<Mage>("Маг2", 80, staff1, 65);

    Battle battle(warrior2, mage2, std::make_unique<RandomActionChooser>(), std::make_unique<RandomActionChooser>());
    for (int i = 0; i < 15 && !battle.isFinished(); i++)
    {
        battle.performTurn();
    }
    battle.printWinner();

    std::cout << "\n--- Console vs console: ---\n";
    auto warrior3 = std::make_shared<Warrior>("Воин3", 150, sword, 10);
    auto mage3 = std::make_shared<Mage>("Маг3", 100, staff1, 65);

    Battle battle2(warrior3, mage3, std::make_unique<RandomActionChooser>(), std::make_unique<RandomActionChooser>());

    std::cout << "Первый ход случайный\n";
    battle2.performTurn();

    std::cout << "Меняем рандомный выбор действия на консольный ввод\n";
    battle2.setChoosers(std::make_unique<ConsoleActionChooser>(), std::make_unique<ConsoleActionChooser>());

    std::cout << "Второй ход с выбором действия через консоль\n";
    battle2.performTurn();
}