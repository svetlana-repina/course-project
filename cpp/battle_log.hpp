#pragma once
#include "base_character.hpp"
#include <vector>
#include <string>

class BattleLog
{
private:
    static std::string title;
    static std::vector<std::string> entries;

public:
    static void SetTitle(const std::string& title);
    static void Log(const std::string& message);
    static void LogAttack(BaseCharacter& attacker, BaseCharacter& target);
    static void LogResult(BaseCharacter* winner);
    static int GetEntryCount();
    static void PrintBattleLog();
};