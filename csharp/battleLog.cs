using System;
using System.Collections.Generic;

namespace RpgBattleSystem
{
    public class BattleLog : IDisposable
    {
        public static string _title;
        private static List<string> _entries = new List<string>();
        private static bool _disposed;

        public  BattleLog(string title)
        {
            _title = title;
            _entries = new List<string>();
            _disposed = false;
            Console.WriteLine($"Журнал {_title} открыт");
        }

        public static void Log(string message)
        {
            if (_disposed)
            {
                Console.WriteLine("Журнал закрыт, невозможно добавить запись");
                return;
            }

            _entries.Add(message);
        }

        public static void LogAttack(BaseCharacter attacker, BaseCharacter target)
        {
            _entries.Add($"{attacker.Name} атакует {target.Name}");
        }

        public static void LogResult(BaseCharacter winner)
        {
            if (winner != null)
            {
                _entries.Add($"Победитель: {winner.Name}");
            }
            else
            {
                _entries.Add("Ничья");
            }
        }

        public static int GetEntryCount()
        {
            return _entries.Count;
        }

        public static void PrintBattleLog()
        {
            Console.WriteLine($"\n========== ЖУРНАЛ БОЯ {_title} ==========");
            foreach (string entry in _entries)
            {
                Console.WriteLine($" {entry}");
            }
            Console.WriteLine("========================================");
        }

        public void Dispose()
        {
            if (_disposed)
            {
                return;
            }
            Console.WriteLine($"Журнал {_title} закрыт.\nЗаписей: {_entries.Count}");
            _disposed = true;
        }
    }
}