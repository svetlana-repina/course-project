using System;
using System.Collections.Generic;

namespace RpgBattleSystem
{
    public class BattleLog : IDisposable
    {
        public string Title { get; }
        private List<string> _entries;
        private bool _disposed;

        public  BattleLog(string title)
        {
            Title = title;
            _entries = new List<string>();
            _disposed = false;
            Console.WriteLine($"Журнал {Title} открыт");
        }

        public void Log(string message)
        {
            if (_disposed)
            {
                Console.WriteLine("Журнал закрыт, невозможно добавить запись");
                return;
            }

            _entries.Add(message);
        }

        public void LogTurn(int turnNumber, string attacker, string target, int damage, bool isCrit)
        {
            if (_disposed)
            {
                Console.WriteLine("Журнал закрыт, невозможно добавить запись");
                return;
            }
            string critMark = isCrit ? "(крит)" : "";
            _entries.Add($"Ход {turnNumber}: {attacker} атакует {target}, урон: {damage}{critMark}");
        }

        public void LogResult(Character winner)
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

        public int GetEntryCount()
        {
            return _entries.Count;
        }

        public void PrintBattleLog()
        {
            Console.WriteLine($"\n========== ЖУРНАЛ БОЯ {Title} ==========");
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
            Console.WriteLine($"Журнал {Title} закрыт.\nЗаписей: {_entries.Count}");
            _disposed = true;
        }
    }
}