using System;
using System.Runtime.CompilerServices;

namespace RpgBattleSystem
{
    public class Program
    {
        static void Main()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Composition();
            Aggregation();
            RuleCheck();
            LinksAndCopies();
            Struct();
            ResourceRelease();

            var watcher = GarbageCollector();
            Console.WriteLine("Собираем мусор");
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
            Console.WriteLine("Проверяем жив ли объект:");
            if (watcher.TryGetTarget(out _))
            {
                Console.WriteLine("Объект жив");
            }
            else
            {
                Console.WriteLine("Объект уничтожен");
            }
        }

        static void Composition()
        {
            Console.WriteLine("========= РАБОТА КОМПОЗИЦИИ: =========");
            {
                Console.WriteLine("Создаём персонажа во вложенном блоке");
                Character hero = new Character("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType.Physical, 100, 0.25, 2.5);
                Console.WriteLine(hero);
                Console.WriteLine("Выходим из вложенного блока");
            }
        }

        static void Aggregation()
        {
            Console.WriteLine("========= РАБОТА АГРЕГАЦИИ: =========");
            Console.WriteLine("Создаём двух персонажей");
            Character warrior = new Character("Воин", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType.Physical, 30, 0.25, 2.5);
            Character mage = new Character("Маг", 80, 5, 15, 50, 10, 5, 35, 1.2, 25.0, "Посох", 10, DamageType.Magical, 35, 1.0, 0.5);
            {
                Console.WriteLine("Создаём бой во вложенном блоке");
                Battle battle = new Battle(warrior, mage);
                Console.WriteLine(battle);
                Console.WriteLine("Делаем несколько ходов");
                while (!battle.IsFinished())
                {
                    battle.PerformTurn();
                }
                battle.PrintWinner();
                Console.WriteLine("Выходим из созданного блока");
            }
            Console.WriteLine("Проверяем состояние персонажей");
            Console.WriteLine(warrior);
            Console.WriteLine(mage);
        }

        static void RuleCheck()
        {
            Console.WriteLine("========= ПРОВЕРКА ПРАВИЛ: =========");
            Console.WriteLine("------ Здоровье персонажа не может стать отрицательным ------");
            Console.WriteLine("Создаём персонажа с неотрицательным здоровьем");
            Character hero1 = new Character("Воин1", 100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType.Physical, 100, 0.25, 2.5);

            Console.WriteLine("Попытка создать персонажа с отрицательным здоровьем");
            Character brokenHero = new Character("Воин2", -100, 25, 50, 0, 10, 10, 5, 1.5, 20.0, "Меч", 12, DamageType.Physical, 100, 0.25, 2.5);

            Console.WriteLine("Попытка нанести персонажу больше урона, чем у него здоровья");
            Console.WriteLine($"Наносим персонажу 999 урона (здоровье персонажа: {hero1.Health})");
            int dealtDamage = hero1.TakeDamage(999);
            Console.WriteLine($"Полученный урон: {dealtDamage}");
            Console.WriteLine($"Здоровье персонажа после получения урона: {hero1.Health}");

            Console.WriteLine("------ Нельзя атаковать, если здоровье персонажа равно 0 ------");
            Console.WriteLine("Попытка живого персонажа атаковать противника");
            Character aliveHero = new Character("Живой", 100, 10, 50, 0, 10, 5, 5, 1.0, 0.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
            Console.WriteLine("Проверяем, жив ли персонаж");
            if (aliveHero.IsAlive())
            {
                Console.WriteLine("персонаж жив");
            }
            else
            {
                Console.WriteLine("персонаж мёртв");
            }
            Console.WriteLine("Пробуем атаковать от лица персонажа");
            aliveHero.Attack(hero1);

            Console.WriteLine("Попытка мёртвого персонажа атаковать противника");
            Console.WriteLine("Проверяем, жив ли Воин1");
            if (hero1.IsAlive())
            {
                Console.WriteLine("Воин1 жив");
            }
            else
            {
                Console.WriteLine("Воин1 мёртв");
            }
            Console.WriteLine("Пробуем атаковать от лица Воина1");
            hero1.Attack(aliveHero);

            Console.WriteLine("------ Прочность снаряжения не может стать отрицательной ------");
            Console.WriteLine("Попытка создать оружие с отрицательной прочностью");
            Weapon sword = new Weapon("Меч", 20, DamageType.Physical, -12, 0.3, 15.0);

            Console.WriteLine("Создаём оружие с прочностью 2");
            Character tester = new Character("Тестер", 100, 10, 50, 0, 10, 5, 5, 1.0, 0.0, "Хрупкий меч", 5, DamageType.Physical, 2, 0.0, 0.0);
            Character target = new Character("Цель", 500, 5, 50, 0, 10, 0, 5, 1.0, 0.0,"Кулак", 1, DamageType.Physical, 100, 0.0, 0.0);
            Console.WriteLine($"Прочность оружия до атак: {tester.Weapon.Durability}");
            for (int i = 0; i < 4; i++)
            {
                tester.Attack(target);
            }
            Console.WriteLine($"Прочность после 4 атак: {tester.Weapon.Durability}");

            Console.WriteLine("------ Шанс критического удара не может быть меньше 0% и больше 100% ------");
            Console.WriteLine("Попытка создать персонажа с шансом крита от 0% до 100%");
            Character character = new Character("Персонаж", 100, 10, 50, 0, 10, 5, 5, 1.5, 20.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
            Console.WriteLine("Попытка создать персонажа с шансом крита >100%");
            Character broken = new Character("Сломанный", 100, 10, 50, 0, 10, 5, 5, 1.5, 150.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
            Console.WriteLine("Попытка создать персонажа с шансом крита <0%");
            Character broken2 = new Character("Сломанный2", 100, 10, 50, 0, 10, 5, 5, 1.5, -20.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
        }

        static void LinksAndCopies()
        {
            Console.WriteLine("========= ССЫЛКИ И КОПИИ: =========");
            Console.WriteLine("Создаём персонажа");
            Character original = new Character("Оригинал", 100, 10, 50, 0, 10, 5, 5, 1.5, 20.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
            Console.WriteLine(original);
            Console.WriteLine("Создаём копию персонажа и присваиваем ей оригинал");
            Character copy = original;
            Console.WriteLine(copy);
            Console.WriteLine("Изменяем состояние копии");
            copy.TakeDamage(15);
            Console.WriteLine($"Здоровье копии после получения урона: {copy.Health}");
            Console.WriteLine($"Здоровье оригинала:                   {original.Health}");
        }

        public struct Coordinates
        {
            public int X;
            public int Y;
        }
        
        static void Struct()
        {
            Console.WriteLine("========= СТРУКТУРЫ: =========");
            Console.WriteLine("Создаём объект a");
            var a = new Coordinates{X = 10, Y = 12};
            Console.WriteLine($"a.X = {a.X}, a.Y = {a.Y}");
            Console.WriteLine("Создаём объект b и присваиваем ему объект a");
            var b = a;
            Console.WriteLine($"b.X = {b.X}, b.Y = {b.Y}");
            Console.WriteLine("Меняем одну из координат b");
            b.X = 20;
            Console.WriteLine($"a.X = {a.X}, a.Y = {a.Y}");
            Console.WriteLine($"b.X = {b.X}, b.Y = {b.Y}");
        }

        static void ResourceRelease()
        {
            Console.WriteLine("========= ОСВОБОЖДЕНИЕ РЕСУРСА: =========");
            using (BattleLog log = new BattleLog("Тестовый бой"))
            {
                log.Log("Бой начался");
                log.LogTurn(1, "маг", "жрец", 20, false);
                log.LogTurn(2, "жрец", "маг", 12, false);
                log.LogTurn(3, "маг", "жрец", 45, true);
                log.PrintBattleLog();
                Console.WriteLine("Выход из блока");
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        static WeakReference<Character> GarbageCollector()
        {
            Console.WriteLine("========= СБОРЩИК МУСОРА: =========");
            Console.WriteLine("Создаём временный объект");
    
            var temp = new Character("Мусор", 100, 10, 50, 0, 10, 5, 5, 1.5, 20.0, "Меч", 10, DamageType.Physical, 10, 0.0, 0.0);
            var watcher = new WeakReference<Character>(temp);
    
            Console.WriteLine("Проверяем жив ли объект:");
            if (watcher.TryGetTarget(out _)) 
            {
                Console.WriteLine("Объект жив");
            }
            return watcher;
        }
    }
}