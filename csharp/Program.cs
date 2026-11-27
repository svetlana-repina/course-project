using System;
using System.Collections.Generic;
using CharacterList = System.Collections.Generic.List<RpgBattleSystem.BaseCharacter>;

namespace RpgBattleSystem
{
    class Program
    {
        private static void TryRecoverMana(BaseCharacter character, int amount)
        {
            if (character is IManaUser manaUser)
            {
                manaUser.RestoreMana(amount);
                Console.WriteLine($"через интерфейс IManaUser восстановлено {amount}, мана: {manaUser.Mana}/{manaUser.MaxMana}");
            }
            else
            {
                Console.WriteLine("интерфейс IManaUser не реализован");
            }
        }

        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;


            var sword = new Weapon("Меч", 15, DamageType.Physical, 100, 0.2, 2.5);
            var staff1 = new Weapon("Посох", 12, DamageType.Magical, 100, 0.25, 3.0);
            var staff2 = new Weapon("Посох", 12, DamageType.Magical, 100, 0.25, 3.0);
            var dagger = new Weapon("Кинжал", 7, DamageType.Physical, 100, 0.15, 2.0);

            var warrior = new Warrior("Воин", 150, sword, 10);
            var mage = new Mage("Маг", 100, staff1, 35);
            var priest = new Priest("Жрец", 90, staff2, 25);

            Console.WriteLine("\n========= РАННЕЕ И ПОЗДНЕЕ СВЯЗЫВАНИЕ: =========");
            BaseCharacter character = warrior;
            Console.WriteLine("Раннее связывание (невиртуальный метод GetKind()):");
            Console.WriteLine(character.GetKind());
            Console.WriteLine("Позднее связывание (виртуальный метод Describe()):");
            Console.WriteLine(character.Describe());

            Console.WriteLine("\n========= ИСПОЛЬЗОВАНИЕ ИНТЕРФЕЙСА: =========");
            TryRecoverMana(mage, 5);
            TryRecoverMana(priest, 5);
            TryRecoverMana(warrior, 5);

            Console.WriteLine("\n========= ПРОВЕРКА ТИПА: =========");
            BaseCharacter warriorBase = warrior;
            BaseCharacter mageBase = mage;

            bool isNotManaUser = (warriorBase as IManaUser) == null;
            Console.WriteLine($"warrior as IManaUser: {(isNotManaUser ? "null" : "не null")}");

            isNotManaUser = (mageBase as IManaUser) == null;
            Console.WriteLine($"mage as IManaUser: {(isNotManaUser ? "null" : "не null")}");

            Console.WriteLine("\n========= ИСКЛЮЧЕНИЯ: =========");
            Console.WriteLine("--- Не хватает маны: ---");
            Console.WriteLine("используем навык мага несколько раз");
            try
            {
                for (int i = 0; i < 2; i++)
                {
                    mage.UseSkill(warrior);
                }
            }
            catch (NotEnoughMana error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}, ход пропущен");
                mage.SkipTurn();
            }

            Console.WriteLine("--- Не хватает выносливости: ---");
            var dummy = new Mage("Болванчик", 1000, staff1, 100);
            try
            {
                for (int i = 0; i < 6; i++)
                {
                    warrior.UseSkill(dummy);
                }
            }
            catch(NotEnoughStamina error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}, ход пропущен");
                warrior.SkipTurn();
            }

            Console.WriteLine("--- Недопустимые параметры констуктора: ---");
            try
            {
                Console.WriteLine("Попытка создать воина с maxHealth = -10:");
                var bad = new Warrior("Плохой", -10, sword, 5);
            }
            catch(InvalidCharacterParameter error)
            {
                Console.WriteLine($"Не удалось создать персонажа: {error.Message}");
            }

            try
            {
                Console.WriteLine("Попытка создать мага с maxMana = 0:");
                var badMage = new Mage("ПлохойМаг", 100, staff1, 0);
            }
            catch(InvalidCharacterParameter error)
            {
                Console.WriteLine($"Не удалось создать персонажа: {error.Message}");
            }

            Console.WriteLine("--- Действие мёртвого персонажа: ---");
            var newMage = new Mage("Новый Маг", 100, staff1, 35);
            try
            {
                while(warrior.IsAlive())
                {
                    warrior.TakeDamage(1000);
                }
                Console.WriteLine("Воин мёртв, пытаемся заставить его атаковать:");
                warrior.Attack(newMage);
            }
            catch(CharacterIsDead error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}");
            }

            Console.WriteLine("--- Попытка атаковать мёртвого персонажа: ---");
            try
            {
                Console.WriteLine("Пытаемся заставить мага атаковать мёртвого воина:");
                newMage.Attack(warrior);
            }
            catch(CharacterIsDead error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}");
            }

            Console.WriteLine("\n========= КОНФИГУРИРОВАНИЕ: =========");
            Console.WriteLine("--- Random vs random: ---");
            var warrior2 = new Warrior("Воин2", 100, sword, 10);
            var mage2 = new Mage("Маг2", 80, staff1, 65);

            var battle = new Battle(warrior2, mage2, new RandomActionChooser(), new RandomActionChooser());
            for (int i = 0; i < 15 && !battle.IsFinished(); i++)
            {
                battle.PerformTurn();
            }
            battle.PrintWinner();

            Console.WriteLine("--- Console vs console: ---");
            var warrior3 = new Warrior("Воин3", 150, sword, 10);
            var mage3 = new Mage("Маг3", 100, staff1, 65);
            var battle2 = new Battle(warrior3, mage3, new RandomActionChooser(), new RandomActionChooser());

            Console.WriteLine("Первый ход случайный");
            battle2.PerformTurn();

            Console.WriteLine("Меняем рандомный выбор действия на консольный ввод");
            battle2.SetChoosers(new ConsoleActionChooser(), new ConsoleActionChooser());

            Console.WriteLine("Второй ход с выбором действия через консоль");
            battle2.PerformTurn();

            Console.WriteLine("\n========= ICOMPARABLE: =========");
            Console.WriteLine("Создаём список персонажей");
            var team = new List<BaseCharacter>
            {
                new Warrior("Дагбранд", 100, sword, 5),
                new Mage("Грамрвальд", 80, staff1, 30),
                new Priest("Сэларимирена", 90, staff2, 20),
                new Rogue("Глеб", 85, dagger, 3),
            };
            Console.WriteLine("Сортируем список персонажей по именам");
            team.Sort();
            foreach (var hero in team) 
            {   
                Console.WriteLine($" {hero.Name}");
            }
        }   
    }
}