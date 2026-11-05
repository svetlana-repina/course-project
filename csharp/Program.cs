using System;
using System.Collections.Generic;
using CharacterList = System.Collections.Generic.List<RpgBattleSystem.BaseCharacter>;

namespace RpgBattleSystem
{
    class Program
    {
        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            var sword  = new Weapon("Меч", 15, DamageType.Physical, 100, 0.2, 2.5);
            var staff1 = new Weapon("Посох", 12, DamageType.Magical, 100, 0.25, 3.0);
            var staff2 = new Weapon("Посох", 12, DamageType.Magical, 100, 0.25, 3.0);
            var dagger = new Weapon("Кинжал", 7, DamageType.Physical, 100, 0.15, 2.0);

            Console.WriteLine("\n========= ПЕРЕДАЧА ТЕКУЩЕГО ОБЪЕКТА ДРУГОМУ ОБЪЕКТУ: =========");
            using (var log = new BattleLog("Турнир"))
            {
                Console.WriteLine("\n========= МАССИВ ИЗ ОБЪЕКТОВ РАЗНЫХ НАСЛЕДНИКОВ: =========");
                Console.WriteLine("Создаём массив указателей из объектов разных наследников");

                CharacterList characters = new CharacterList
                {
                    new Warrior("Воин", 150, sword, 10),
                    new Mage("Маг", 120, staff1, 100),
                    new Priest("Жрец", 100, staff2, 75),
                    new Rogue("Разбойник", 110, dagger, 5)
                };

                Console.WriteLine("\nАТАКИ:");
                characters[0].Attack(characters[1]);
                characters[1].Attack(characters[0]);
                characters[2].Attack(characters[3]);
                characters[3].Attack(characters[2]);

                Console.WriteLine("\nНАВЫКИ:");
                characters[0].UseSkill(characters[1]);
                characters[1].UseSkill(characters[0]);
                characters[2].UseSkill(characters[2]);
                characters[3].UseSkill(characters[2]);

                Console.WriteLine("\nПРОПУСК ХОДА:");
                foreach (var hero in characters)
                {
                    hero.SkipTurn();
                }

                Console.WriteLine("\nОПИСАНИЕ:");
                foreach (var hero in characters)
                {
                    Console.WriteLine(hero.Describe() + "\n");
                }

                BattleLog.PrintBattleLog();
            }


            Console.WriteLine("\n========= УНИЧТОЖЕНИЕ НАСЛЕДНИКА ЧЕРЕЗ УКАЗАТЕЛЬ НА БАЗОВЫЙ КЛАСС: =========");
            Console.WriteLine("Создаём персонажа через переменную базового класса во вложенном блоке");
            {
                BaseCharacter character = new Priest("Жрец1", 100, staff2, 75);
                Console.WriteLine("Выходим из вложенного блока");
            }


            Console.WriteLine("\n========= ПОВЕРХНОСТНОЕ И ГЛУБОКОЕ КОПИРОВАНИЕ: =========");
            Console.WriteLine("ПОВЕРХНОСТНАЯ КОПИЯ:");
            var sharedSword = new Weapon("Общий меч", 15, DamageType.Physical, 100, 0.2, 2.5);
            Console.WriteLine($"Создано общее оружие. Прочность: {sharedSword.Durability}");
            var fighter1 = new Warrior("Поверхностный1", 120, sharedSword, 20);
            Console.WriteLine("Создаём поверхностную копию");
            Warrior fighter2 = fighter1.ShallowCopy();

            Console.WriteLine($"Прочность оружия у 1 до использования: {fighter1.Weapon.Durability}");
            Console.WriteLine($"Прочность оружия у 2 до использования: {fighter2.Weapon.Durability}");

            Console.WriteLine("Используем оружие через одного из владельцев");
            fighter1.Weapon.Use();

            Console.WriteLine($"Прочность оружия у 1 после использования: {fighter1.Weapon.Durability}");
            Console.WriteLine($"Прочность оружия у 2 после использования: {fighter2.Weapon.Durability}");

            Console.WriteLine("\nГЛУБОКАЯ КОПИЯ:");
            var original = new Warrior("Оригинал", 120, sharedSword, 20);
            var copy = (Warrior)original.Clone();
            Console.WriteLine($"Прочность у оружия оригинала до использования: {original.Weapon.Durability}");
            Console.WriteLine($"Прочность у оружия копии до использования: {copy.Weapon.Durability}");

            Console.WriteLine("Используем оружие через одного из владельцев");
            original.Weapon.Use();

            Console.WriteLine($"Прочность у оригинала после использования: {original.Weapon.Durability}");
            Console.WriteLine($"Прочность у копии после использования: {copy.Weapon.Durability}");


            Console.WriteLine("\n========= РАБОТА СО СТРОКАМИ: =========");
            Console.WriteLine("КОНКАТЕНАЦИЯ:");
            string firstName = "Спардудл";
            string lastName = "Дребезжалка";
            string fullName = firstName + " " + lastName;
            Console.WriteLine($"Полное имя: {fullName}");

            Console.WriteLine("\nПОИСК:");
            int pos = fullName.IndexOf("Дребезжалка");
            if (pos >= 0)
            {
                string alias = fullName.Substring(pos);
                Console.WriteLine($"Найдено прозвище: {alias}");
            }
            else
            {
                Console.WriteLine("Ничего не найдено");
            }

            Console.WriteLine("\nСРАВНЕНИЕ:");
            int isSame = string.Compare(lastName, "Дребезжалка");
            Console.WriteLine($"Результат сравнения: {(isSame == 0 ? "равно" : "не равно")}");


            Console.WriteLine("\n========= СРАВНЕНИЕ ПО СОДЕРЖИМОМУ: =========");
            var a = new Warrior("Тестер", 100, new Weapon("Меч", 15, DamageType.Physical, 100, 0.2, 2.5), 0);
            var b = new Warrior("Тестер", 100, new Weapon("Меч", 15, DamageType.Physical, 100, 0.2, 2.5), 0);
            Console.WriteLine($"ReferenceEquals: {ReferenceEquals(a, b)}");
            Console.WriteLine($"Equals: {a.Equals(b)}");
            Console.WriteLine($"HashCode совпадает: {a.GetHashCode() == b.GetHashCode()}");


            Console.WriteLine("\n========= СТАТИЧЕСКОЕ ПОЛЕ: =========");
            Console.WriteLine($"Количество созданных персонажей: {BaseCharacter.GetCount()}");
        }
    }
}