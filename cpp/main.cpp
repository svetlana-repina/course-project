#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <windows.h>
#include "warrior.hpp"
#include "mage.hpp"
#include "priest.hpp"
#include "rogue.hpp"
#include "battle_log.hpp"

void setConsoleUTF8() 
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

int main() {
    setConsoleUTF8();
    BattleLog::SetTitle("Турнир");

    std::cout << "========= МАССИВ ИЗ ОБЪЕКТОВ РАЗНЫХ НАСЛЕДНИКОВ: =========\n";
    std::cout << "Создаём массив указателей из объектов разных наследников\n";
    auto sword = std::make_shared<Weapon>("Меч", 15, DamageType::Physical, 100, 0.2, 2.5);
    auto staff1 = std::make_shared<Weapon>("Посох", 12, DamageType::Magical, 100, 0.25, 3.0);
    auto staff2 = std::make_shared<Weapon>("Посох", 12, DamageType::Magical, 100, 0.25, 3.0);
    auto dagger = std::make_shared<Weapon>("Кинжал", 7, DamageType::Physical, 100, 0.15, 2.0);

    std::vector<std::unique_ptr<BaseCharacter>> characters;
    characters.push_back(std::make_unique<Warrior>("Воин", 150, sword, 10));
    characters.push_back(std::make_unique<Mage>("Маг", 120, staff1, 100));
    characters.push_back(std::make_unique<Priest>("Жрец", 100, staff2, 75));
    characters.push_back(std::make_unique<Rogue>("Разбойник", 110, dagger, 5));

    std::cout << "\nАТАКИ:\n";
    characters[0]->Attack(*characters[1]);
    characters[1]->Attack(*characters[0]);
    characters[2]->Attack(*characters[3]);
    characters[3]->Attack(*characters[2]);

    std::cout << "\nНАВЫКИ:\n";
    characters[0]->UseSkill(*characters[1]);
    characters[1]->UseSkill(*characters[0]);
    characters[2]->UseSkill(*characters[2]);
    characters[3]->UseSkill(*characters[2]);

    std::cout << "\nПРОПУСК ХОДА:\n";
    for (auto& hero : characters)
    {
        hero->SkipTurn();
    }

    std::cout << "\nОПИСАНИЕ:\n";
    for (auto& hero : characters)
    {
        std::cout << hero->Describe() << "\n";
    }


    std::cout << "\n========= УНИЧТОЖЕНИЕ НАСЛЕДНИКА ЧЕРЕЗ УКАЗАТЕЛЬ НА БАЗОВЫЙ КЛАСС: =========\n";
    {
        std::cout << "Создаём персонажа через указатель на базовый класс во вложенном блоке\n";
        std::unique_ptr<BaseCharacter> character = std::make_unique<Priest>("Жрец1", 100, staff2, 75);
        std::cout << "Выходим из вложенного блока\n";
    }


    std::cout << "\n========= ПОВЕРХНОСТНОЕ И ГЛУБОКОЕ КОПИРОВАНИЕ: =========\n";
    std::cout << "ПОВЕРХНОСТНАЯ КОПИЯ:\n";
    auto sharedSword = std::make_shared<Weapon>("Общий меч", 15, DamageType::Physical, 100, 0.2, 2.5);
    std::cout << "Создано общее оружие. Прочность: " << sharedSword->getDurability() << "\n";
    std::cout << "Количество владельцев до создания персонажей: " << sharedSword.use_count() << "\n";

    auto fighter1 = std::make_unique<Warrior>("Поверхностный1", 120, sharedSword, 20);
    auto fighter2 = std::make_unique<Warrior>("Поверхностный2", 120, sharedSword, 20);
    std::cout << "Количество владельцев общего оружия после создания двух персонажей: " << sharedSword.use_count() << "\n";

    std::cout << "Прочность оружия у 1 до использования: " << fighter1->GetWeapon()->getDurability() << "\n";
    std::cout << "Прочность оружия у 2 до использования: " << fighter2->GetWeapon()->getDurability() << "\n";

    std::cout << "Используем оружие через одного из владельцев\n";
    fighter1->GetWeapon()->use();

    std::cout << "Прочность оружия у 1 после использования: " << fighter1->GetWeapon()->getDurability() << "\n";
    std::cout << "Прочность оружия у 2 после использования: " << fighter2->GetWeapon()->getDurability() << "\n";

    std::cout << "\nГЛУБОКАЯ КОПИЯ:\n";
    auto original = std::make_unique<Warrior>("Оригинал", 120, sharedSword, 20);
    auto copy = original->Clone();

    std::cout << "Количество владельцев оружия после создания персонажей: " << sharedSword.use_count() << "\n";

    std::cout << "Прочность оружия у оригинала до использования: " << original->GetWeapon()->getDurability() << "\n";
    std::cout << "Прочность оружия у копии до использования: " << copy->GetWeapon()->getDurability() << "\n";

    std::cout << "Используем оружие через одного из владельцев\n";
    original->GetWeapon()->use();

    std::cout << "Прочность оружия у оригинала после использования: " << original->GetWeapon()->getDurability() << "\n";
    std::cout << "Прочность оружия у копии после использования: " << copy->GetWeapon()->getDurability() << "\n";


    std::cout << "\n========= РАБОТА СО СТРОКАМИ: =========\n";
    std::cout << "КОНКАТЕНАЦИЯ:\n";
    std::string firstName = "Спардудл";
    std::string lastName = "Дребезжалка";
    std::cout << "Имя: " << firstName << "\n";
    std::cout << "Прозвище: " << lastName << "\n";
    std::string fullName = firstName + " " + lastName;
    std::cout << "Полное имя: " << fullName << "\n";

    std::cout << "\nПОИСК:\n";
    size_t pos = fullName.find("Дребезжалка");
    if (pos != std::string::npos)
    {
        std::string alias = fullName.substr(pos);
        std::cout << "Найдено прозвище: " << alias << "\n";
    }
    else
    {
        std::cout << "Ничего не найдено\n";
    }

    std::cout << "\nСРАВНЕНИЕ:\n";
    int isSame = lastName.compare("Дребезжалка");
    std::cout << "Результат сравнения: " << (isSame == 0 ? "равно" : "не равно") << "\n";


    std::cout << "\n========= ПЕРЕДАЧА ТЕКУЩЕГО ОБЪЕКТА ДРУГОМУ ОБЪЕКТУ: =========\n";
    BattleLog::PrintBattleLog();


    std::cout << "\n========= СТАТИЧЕСКОЕ ПОЛЕ: =========\n";
    std::cout << "Количество созданных персонажей: " << BaseCharacter::GetCount() << "\n";

}