#include "mage.hpp"
#include "battle_log.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

Mage::Mage(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int maxMana)
    : BaseCharacter(name, maxHealth, 3, 10, 10, 5, 25, 2.0, 25.0, std::move(weapon)), 
    mana(maxMana), 
    maxMana(maxMana)
{
    if (maxMana <= 0)
    {
        throw InvalidCharacterParameter("maxMana", maxMana);
    }
    std::cout << "Создан маг: " << GetName() << "\n";
}

Mage::~Mage() noexcept
{
    std::cout << "Маг уничтожен: " << GetName() << "\n";
}

int Mage::Attack(BaseCharacter& target)
{
    if (!IsAlive())
    {
        throw CharacterIsDead(GetName());
    }
    
    if (!target.IsAlive())
    {
        throw CharacterIsDead(target.GetName());
    }

    if (mana < manaAttackCost)
    {
        throw NotEnoughMana(GetName(), manaAttackCost, mana);
    }

    mana -= manaAttackCost;
    int intelligenceBonus = static_cast<int>(std::round(GetIntelligence() / 1.5));
    int baseDamage = GetWeapon()->calculateBaseDamage(GetStrength()) + intelligenceBonus;
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

    return dealtDamage;
}

std::unique_ptr<BaseCharacter> Mage::Clone() const
{
    return std::make_unique<Mage>(*this);
}

std::string Mage::Describe() const
{
    return BaseCharacter::Describe() + "Мана:                    " + std::to_string(mana) + "/" + std::to_string(maxMana) + "\n";
}

void Mage::UseSkill(BaseCharacter& target)
{
    if (!IsAlive())
    {
        throw CharacterIsDead(GetName());
    }

    if (!target.IsAlive())
    {
        throw CharacterIsDead(target.GetName());
    }

    if (mana < manaSkillCost)
    {
        throw NotEnoughMana(GetName(), manaSkillCost, mana);
    }

    mana -= manaSkillCost;
    int skillDamage = GetIntelligence() * 2;
    int finalDamage = ApplyCrit(skillDamage);
    int dealtDamage = target.TakeDamage(finalDamage);
    BattleLog::LogAttack(*this, target);
    std::cout << GetName() << " использует навык, бросает огненный шар в " << target.GetName() << " и наносит " << dealtDamage << " урона\n";
}

void Mage::SkipTurn()
{
    mana = std::min(mana + manaAttackCost, maxMana);
    std::cout << GetName() << " пропускает ход и восстанавливает часть маны\n";
}

bool Mage::SpendMana(int amount)
{
    if (amount < 0 || mana < amount) return false;
    mana -= amount;
    return true;
}

void Mage::RestoreMana(int amount)
{
    if (amount > 0)
    {
        mana = std::min(mana + amount, maxMana);
    }
}

int Mage::GetMana() const
{
    return mana;
}

int Mage::GetMaxMana() const
{
    return maxMana;
}