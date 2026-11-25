#include "warrior.hpp"
#include "battle_log.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <algorithm>

Warrior::Warrior(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int rage)
    : BaseCharacter(name, maxHealth, 10, 50, 5, 10, 3, 1.2, 15.0, std::move(weapon)), 
    rage(rage)
{
    if (rage < 0)
    {
        throw InvalidCharacterParameter("rage", rage);
    }
    if (rage > maxRage)
    {
        throw InvalidCharacterParameter("rage", rage);
    }
    std::cout << "Создан воин: " << name << "\n";
}

Warrior::~Warrior() noexcept
{
    std::cout << "Воин уничтожен: " << GetName() << "\n";
}

int Warrior::Attack(BaseCharacter& target)
{
    if (!IsAlive())
    {
        throw CharacterIsDead(GetName());
    }

    if (!target.IsAlive())
    {
        throw CharacterIsDead(target.GetName());
    }
    
    if (!SpendStamina(staminaAttackCost))
    {
        throw NotEnoughStamina(GetName(), staminaAttackCost, GetStamina());
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
        throw CharacterIsDead(GetName());
    }

    if (!target.IsAlive())
    {
        throw CharacterIsDead(target.GetName());
    }

    if (!SpendStamina(staminaSkillCost))
    {
        throw NotEnoughStamina(GetName(), staminaSkillCost, GetStamina());
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

std::string Warrior::GetKind() const
{
    return "воин";
}