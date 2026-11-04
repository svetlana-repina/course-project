#include "battle_log.hpp"
#include <iostream>

std::string BattleLog::title = "Бой";
std::vector<std::string> BattleLog::entries;

void BattleLog::SetTitle(const std::string& title)
{
    BattleLog::title = title;
    entries.clear();
}

void BattleLog::Log(const std::string& message)
{
    entries.push_back(message + "\n");
}

void BattleLog::LogAttack(BaseCharacter& attacker, BaseCharacter& target)
{
    entries.push_back(attacker.GetName() + " атакует " + target.GetName() + "\n");
}

void BattleLog::LogResult(BaseCharacter* winner)
{
    if (winner)
    {
        entries.push_back("Победитель: " + winner->GetName() + "\n");
    }
    else
    {
        entries.push_back("Ничья\n");
    }
}

int BattleLog::GetEntryCount()
{
    return entries.size();
}

void BattleLog::PrintBattleLog()
{
    std::cout << "\n========== ЖУРНАЛ БОЯ " << title << " ==========\n";
    for (const auto& entry : entries)
    {
        std::cout << entry;
    }
    std::cout << "========================================\n";
}