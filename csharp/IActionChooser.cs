using System;

namespace RpgBattleSystem
{
    public enum ActionType
    {
        Attack,
        Skill,
        Skip,
    }

    public interface IActionChooser
    {
        ActionType ChooseAction(BaseCharacter hero);
    }

    public sealed class ConsoleActionChooser : IActionChooser
    {
        public ActionType ChooseAction(BaseCharacter hero)
        {
            while(true)
            {
                Console.WriteLine($"{hero.Name}, выберите действие:\n 1. Атака\n 2. Навык\n 3. Пропуск хода");

                string input = Console.ReadLine();
                if (int.TryParse(input, out int choice))
                {
                    switch(choice)
                    {
                        case 1: return ActionType.Attack;
                        case 2: return ActionType.Skill;
                        case 3: return ActionType.Skip;
                        default: Console.WriteLine("Такого действия нет, введите другое число"); break;
                    }
                }
                else
                {
                    Console.WriteLine("Введите число");
                }
            }
        }
    }

    public sealed class RandomActionChooser : IActionChooser
    {
        private static readonly Random _random = new Random();
        public ActionType ChooseAction(BaseCharacter hero)
        {
            return (ActionType)_random.Next(3);
        }
    }
}