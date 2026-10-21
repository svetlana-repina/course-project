using System;

namespace RpgBattleSystem
{
    public class Battle
    {
        private Character _fighter1;
        private Character _fighter2;
        private int _turnCount;

        public Battle()
        {
            _fighter1 = null;
            _fighter2 = null;
            _turnCount = 0;
            Console.WriteLine("Создан пустой бой");
        }

        public Battle(Character f1, Character f2)
        {
            if (f1 == null || f2 == null)
            {
                Console.WriteLine("Ошибка! Оба участника должны быть заданы.");
                _fighter1 = null;
                _fighter2 = null;
                _turnCount = 0;
                return;
            }
            _fighter1 = f1;
            _fighter2 = f2;
            _turnCount = 0;
            Console.WriteLine($"Начался бой между {f1.Name} и {f2.Name}");
        }

        public Character GetWinner()
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
            if (_turnCount % 2 != 0)
            {
                _fighter1.Attack(_fighter2);
            }
            else
            {
                _fighter2.Attack(_fighter1);
            }
        }

        public void PrintWinner()
        {
            if (!IsFinished())
            {
                Console.WriteLine("Бой ещё не закончен");
                return;
            }
            Character winner = GetWinner();
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
                str += _fighter1.ToString() + "\n";
            }
            if (_fighter2 != null)
            {
                str += _fighter2.ToString() + "\n";
            }
            str += "===============================================";
            return str;
        }
    }
}