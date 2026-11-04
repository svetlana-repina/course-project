#include "warrior.hpp"
#include "battle_log.hpp"
#include <iostream>
#include <algorithm>

Warrior::Warrior(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int rage)
    : BaseCharacter(name, maxHealth, 10, 50, 5, 10, 3, 1.2, 15.0, std::move(weapon)), 
    rage(rage)
{
    if (rage < 0)
    {
        std::cout << "Ошибка! Ярость должна быть >=0. Установлено значение 0.\n";
        this->rage = 0;
    }
    if (rage > maxRage)
    {
        std::cout << "Ошибка! Ярость не может быть больше " << maxRage << ". Установлено значение " << maxRage << ".\n";
        this->rage = maxRage;
    }
    std::cout << "Создан воин: " << name << "\n";
}

Warrior::~Warrior()
{
    std::cout << "Воин уничтожен: " << GetName() << "\n";
}

int Warrior::Attack(BaseCharacter& target)
{
    if (!IsAlive())
    {
        std::cout << GetName() << " мёртв и не может атаковать\n";
        return 0;
    }

    if (!target.IsAlive())
    {
        std::cout << target.GetName() << " уже мёртв\n";
        return 0;
    }
    
    if (!SpendStamina(staminaAttackCost))
    {
        std::cout << GetName() << " слишком устал и не может атаковать\n";
        SkipTurn();
        return 0;
    }

    int rageBonus = static_cast<int>(rage * 0.1);
    int baseDamage = GetWeapon()->calculateBaseDamage(GetStrength()) + rageBonus;
    int finalDamage = ApplyCrit(baseDamage);
    bool isCrit = (finalDamage > baseDamage);
    int dealtDamage = target.TakeDamage(finalDamage);
    BattleLog::LogAttack(*this, target);
    GetWeapon()->use();

    std::cout << GetName() << " атакует " << target.GetName();
    if (isCrit)
    {
        std::cout << " и наносит критический удар(x" << GetTotalCrit() << ") " << dealtDamage << "\n";
    } 
    else
    {
        std::cout << " и наносит " << dealtDamage << " урона.\n";
    }

    rage = std::min(rage + ragePerAttack, maxRage);
    return dealtDamage;
}

std::unique_ptr<BaseCharacter> Warrior::Clone() const
{
    return std::make_unique<Warrior>(*this);
}

std::string Warrior::Describe() const
{
    return BaseCharacter::Describe() + "Ярость:                  " + std::to_string(rage) + "/" + std::to_string(maxRage) + "\n";
}

void Warrior::UseSkill(BaseCharacter& target)
{
    if (!IsAlive())
    {
        std::cout << GetName() << " мёртв и не может атаковать\n";
        return;
    }

    if (!target.IsAlive())
    {
        std::cout << target.GetName() << " уже мёртв\n";
        return;
    }

    if (!SpendStamina(staminaSkillCost))
    {
        std::cout << GetName() << " слишком устал и не может использовать навык\n";
        SkipTurn();
        return;
    }

    int doubleDamage = GetWeapon()->calculateBaseDamage(GetStrength()) * 2;
    int finalDamage = ApplyCrit(doubleDamage);
    bool isCrit = (finalDamage > doubleDamage);
    int dealtDamage = target.TakeDamage(finalDamage);
    BattleLog::LogAttack(*this, target);
    GetWeapon()->use();
    std::cout << GetName() << " использует навык на " << target.GetName() << " и наносит двойной урон (" << dealtDamage << ")";
    if (isCrit)
    {
        std::cout << "(КРИТ)";
    }
    std::cout << "\n";
    rage = std::min(rage + 2 * ragePerAttack, maxRage);
}

void Warrior::SkipTurn()
{
    RestoreStamina(staminaAttackCost);
    rage = std::max(rage - ragePerAttack, 0);
    std::cout << GetName() << " пропускает ход, восстанавливает часть энергии и становится немного спокойнее\n";
}

int Warrior::GetRage() const
{
    return rage;
}