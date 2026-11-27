using System;

namespace RpgBattleSystem
{
    public class Battle
    {
        private BaseCharacter _fighter1;
        private BaseCharacter _fighter2;
        private int _turnCount;
        private IActionChooser _chooser1;
        private IActionChooser _chooser2;

        public Battle()
        {
            _fighter1 = null;
            _fighter2 = null;
            _turnCount = 0;
            _chooser1 = new ConsoleActionChooser();
            _chooser2 = new ConsoleActionChooser();
            Console.WriteLine("Создан пустой бой");
        }

        public Battle(BaseCharacter f1, BaseCharacter f2)
            : this(f1, f2, new ConsoleActionChooser(), new ConsoleActionChooser())
        {}

        public Battle(BaseCharacter f1, BaseCharacter f2, IActionChooser c1, IActionChooser c2)
        {
            if (f1 == null || f2 == null)
            {
                Console.WriteLine("Ошибка! Оба участника должны быть заданы.");
                _fighter1 = null;
                _fighter2 = null;
                _turnCount = 0;
                _chooser1 = new ConsoleActionChooser();
                _chooser2 = new ConsoleActionChooser();
                return;
            }
            _fighter1 = f1;
            _fighter2 = f2;
            _turnCount = 0;
            _chooser1 = c1;
            _chooser2 = c2;
            Console.WriteLine($"Начался бой между {f1.Name} и {f2.Name}");
        }

        public void SetChoosers(IActionChooser c1, IActionChooser c2)
        {
            if (c1 != null)
            {
                _chooser1 = c1;
            }
            if (c2 != null)
            {
                _chooser2 = c2;
            }
        }

        public BaseCharacter GetWinner()
        {
            if (_fighter1 == null || _fighter2 == null)
            {
                return null;
            }
            if (!IsFinished())
            {
                return null;
            }
            bool f1IsAlive = _fighter1.IsAlive();
            bool f2IsAlive = _fighter2.IsAlive();
            if (!f1IsAlive && !f2IsAlive)
            {
                return null;
            }
            return f1IsAlive ? _fighter1 : _fighter2;
        }

        public bool IsFinished()
        {
            if (_fighter1 == null || _fighter2 == null)
            {
                return true;
            }
            return !_fighter1.IsAlive() || !_fighter2.IsAlive();
        }

        public int GetTurnCount()
        {
            return _turnCount;
        }

        public void PerformTurn()
        {
            if (IsFinished())
            {
                Console.WriteLine("Бой уже завершён");
                return;
            }
            _turnCount++;
            Console.WriteLine($"-------- Ход {_turnCount} --------");

            BaseCharacter actor = (_turnCount % 2 == 1) ? _fighter1 : _fighter2;
            BaseCharacter target = (_turnCount % 2 == 1) ? _fighter2 : _fighter1;
            IActionChooser chooser = (_turnCount % 2 == 1) ? _chooser1 : _chooser2;
            ActionType action = chooser.ChooseAction(actor);

            try
            {
                switch(action)
                {
                    case ActionType.Attack: actor.Attack(target); break;
                    case ActionType.Skill: actor.UseSkill(target); break;
                    case ActionType.Skip: actor.SkipTurn(); break;
                }
            }
            catch (NotEnoughMana error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}, ход пропущен");
                actor.SkipTurn();
            }
            catch (NotEnoughStamina error)
            {
                Console.WriteLine($"Перехвачено: {error.Message}, ход пропущен");
                actor.SkipTurn();
            }
            catch (Exception error)
            {
                Console.WriteLine($"Ошибка: {error.Message}");
            }
        }

        public void PrintWinner()
        {
            if (!IsFinished())
            {
                Console.WriteLine("Бой ещё не закончен");
                return;
            }
            BaseCharacter winner = GetWinner();
            if (winner != null)
            {
                Console.WriteLine($"{winner.Name} победил за {_turnCount} ходов");
            }
            else
            {
                Console.WriteLine("Ничья. Оба бойца умерли");
            }
        }

        public override string ToString()
        {
            string str = $"======== Текущее состояние боя (ход {_turnCount}) ========\n";
            if (_fighter1 != null)
            {
                str += _fighter1.Describe();
            }
            if (_fighter2 != null)
            {
                str += _fighter2.Describe();
            }
            str += "===============================================";
            return str;
        }
    }
}