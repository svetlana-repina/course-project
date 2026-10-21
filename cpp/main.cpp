#include <iostream>
#include <windows.h>
#include "character.hpp"
#include "battle.hpp"

void setConsoleUTF8() 
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

int main() {
    setConsoleUTF8();

    std::cout << "========= РАБОТА КОМПОЗИЦИИ: =========\n";
    {
        std::cout << "Создаём персонажа во вложенном блоке\n";
        Character hero("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 100, 0.25, 2.5);
        hero.printState();
        std::cout << "Выходим из вложенного блока\n";
    }


    std::cout << "========= РАБОТА АГРЕГАЦИИ: =========\n";
    std::cout << "Создаём двух персонажей\n";
    Character* warrior = new Character("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 30, 0.25, 2.5);
    Character* mage = new Character("Маг", 80, 5, 15, 50, 10, 5, 35, 1.2, 25.0, "Посох", 10, DamageType::Magical, 35, 1.0, 0.5);
    {
        std::cout << "Создаём бой во вложенном блоке\n";
        Battle battle(warrior, mage);
        battle.printState();
        std::cout << "Делаем несколько ходов\n";
        while(!battle.isFinished()) {
            battle.performTurn();
        }
        battle.printWinner();
        std::cout << "Выходим из созданного блока\n";
    }
    std::cout << "Проверяем состояние персонажей\n";
    warrior->printState();
    mage->printState();


    std::cout << "========= ПРОВЕРКА ПРАВИЛ: =========\n";
    std::cout << "------ Здоровье персонажа не может стать отрицательным ------\n";
    std::cout << "Создаём персонажа с неотрицательным здоровьем\n";
    Character Hero("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 100, 0.25, 2.5);

    std::cout << "Попытка создать персонажа с отрицательным здоровьем\n";
    Character brokenHero("Воин", -100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 100, 0.25, 2.5);

    std::cout << "Попытка нанести персонажу больше урона, чем у него здоровья\n";
    std::cout << "Наносим персонажу 999 урона (здоровье персонажа: " << Hero.getHealth() << ")\n";
    int dealtDamage = Hero.takeDamage(999);
    std::cout << "Полученный урон: " << dealtDamage << "\n";
    std::cout << "Здоровье персонажа после получения урона: " << Hero.getHealth() << "\n";

    std::cout << "------ Нельзя атаковать, если здоровье персонажа равно 0 ------\n";
    std::cout << "Попытка живого персонажа атаковать противника\n";
    Character aliveHero("Живой", 100, 10, 50, 0, 10, 5, 5, 1.0, 0.0, "Меч", 10, DamageType::Physical, 10, 0.0, 0.0);
    std::cout << "Проверяем, жив ли персонаж\n";
    if (aliveHero.isAlive())
    {
        std::cout << "персонаж жив\n";
    }
    else 
    {
        std::cout << "персонаж мёртв\n";
    }
    std::cout << "Пробуем атаковать от лица персонажа\n";
    aliveHero.attack(*warrior);

    std::cout << "Попытка мёртвого персонажа атаковать противника\n";
    std::cout << "Проверяем, жив ли маг\n";
    if (mage->isAlive())
    {
        std::cout << "Маг жив\n";
    }
    else 
    {
        std::cout << "Маг мёртв\n";
    }
    std::cout << "Пробуем атаковать от лица мага\n";
    mage->attack(*warrior);

    std::cout << "------ Прочность снаряжения не может стать отрицательной ------\n";
    std::cout << "Попытка создать оружие с отрицательной прочностью\n";
    Weapon sword("Меч", 20, DamageType::Physical, -12, 0.3, 15.0);

    std::cout << "Создаём оружие с прочностью 2\n";
    Character tester("Тестер", 100, 10, 50, 0, 10, 5, 5, 1.0, 0.0, "Хрупкий меч", 5, DamageType::Physical, 2, 0.0, 0.0);
    Character target("Цель", 500, 5, 50, 0, 10, 0, 5, 1.0, 0.0,"Кулак", 1, DamageType::Physical, 100, 0.0, 0.0);
    std::cout << "Прочность оружия до атак: " << tester.getWeapon().getDurability() << "\n";
    for (int i = 0; i < 4; i++)
    {
        tester.attack(target);
    }
    std::cout << "Прочность после 4 атак: " << tester.getWeapon().getDurability() << "\n";

    std::cout << "------ Шанс критического удара не может быть меньше 0% и больше 100% ------\n";
    std::cout << "Попытка создать персонажа с шансом крита от 0% до 100%\n";
    Character character("Персонаж", 100, 10, 50, 0, 10, 5, 5, 1.5, 20.0, "Меч", 10, DamageType::Physical, 10, 0.0, 0.0);
    std::cout << "Попытка создать персонажа с шансом крита >100%\n";
    Character broken("Сломанный", 100, 10, 50, 0, 10, 5, 5, 1.5, 150.0, "Меч", 10, DamageType::Physical, 10, 0.0, 0.0);
    std::cout << "Попытка создать персонажа с шансом крита <0%\n";
    Character broken2("Сломанный2", 100, 10, 50, 0, 10, 5, 5, 1.5, -20.0, "Меч", 10, DamageType::Physical, 10, 0.0, 0.0);

    std::cout << "------ Бой завершается, как только здоровье одного из персонажей становится равным 0 ------\n";
    std::cout << "Работа этого правила была продемонстрирована в блоке с демонстрацией работы агрегации\n";


    std::cout << "========= РАБОТА С ОПЕРАТОРАМИ ПО ССЫЛКЕ И ПО УКАЗАТЕЛЮ: =========\n";
    std::cout << "Создадим указатель на объект warrior\n";
    Character* pointer = warrior;
    pointer->printState();

    std::cout << "Создадим ссылку на тот же объект\n";
    Character& link = *pointer;
    link.printState();

    std::cout << "Изменяем объект через указатель\n";
    pointer->takeDamage(15);
    std::cout << "Указатель:\n";
    pointer->printState();
    std::cout << "Ссылка:\n";
    link.printState();

    std::cout << "Изменяем объект через ссылку\n";
    link.heal(10);
    std::cout << "Указатель:\n";
    pointer->printState();
    std::cout << "Ссылка:\n";
    link.printState();

    std::cout << "Переприсваеваем указатель на другой объект(mage)\n";
    pointer = mage;
    pointer->printState();
    
    std::cout << "Ссылка остаётся привязаной к warrior\n";
    link.printState();


    std::cout << "========= ДИНАМИЧЕСКИЙ МАССИВ ОБЪЕКТОВ: =========\n";
    std::cout << "Создаём динамический массив из 2 объектов\n";
    Character* team = new Character[2]
    {
        Character("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 100, 0.25, 2.5),
        Character("Маг", 80, 5, 15, 50, 10, 5, 35, 1.2, 25.0, "Посох", 10, DamageType::Magical, 35, 1.0, 0.5)
    };
    std::cout << "Содержимое массива:\n";
    for(int i=0; i<2; i++)
    {
        team[i].printState();
    }
    std::cout << "Удаляем массив\n";
    delete[] team;
    std::cout << "Массив удалён\n";


    std::cout << "========= МАССИВ ДИНАМИЧЕСКИХ ОБЪЕКТОВ: =========\n";
    std::cout << "Создаём массив указателей на 2 персонажей\n";
    Character* team2[2];
    team2[0] = new Character("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType::Physical, 100, 0.25, 2.5);
    team2[1] = new Character("Маг", 80, 5, 15, 50, 10, 5, 35, 1.2, 25.0, "Посох", 10, DamageType::Magical, 35, 1.0, 0.5);
    for(int i=0; i<2; i++)
    {
        team2[i]->printState();
    }
    std::cout << "Удаляем каждый объект массива\n";
    for(int i = 0; i < 2; i++)
    {
        delete team2[i];
    }
    std::cout << "Все объекты удалены\n";

    std::cout << "Удаляем ненужные объекты\n";
    delete warrior;
    delete mage;
}