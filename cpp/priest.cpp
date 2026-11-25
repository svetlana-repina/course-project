#include "priest.hpp"
#include "battle_log.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <algorithm>

Priest::Priest(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int maxMana)
    :BaseCharacter(name, maxHealth, 3, 10, 10, 5, 15, 1.75, 15.0, std::move(weapon)), 
    mana(maxMana),
    maxMana(maxMana)
{
    if (maxMana <= 0)
    {
        throw InvalidCharacterParameter("maxMana", maxMana);
    }
    std::cout << "Создан жрец: " << GetName() << "\n";
}

Priest::~Priest() noexcept
{
    std::cout << "Жрец уничтожен: " << GetName() << "\n";
}

int Priest::Attack(BaseCharacter& target)
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
    int intelligenceBonus = GetIntelligence() / 4;
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

std::unique_ptr<BaseCharacter> Priest::Clone() const
{
    return std::make_unique<Priest>(*this);
}

std::string Priest::Describe() const
{
    return GetName() + "\nЗдоровье:                " + std::to_string(GetHealth())
                     + "\nВыносливость:            " + std::to_string(GetStamina())
                     + "\nЛовкость:                " + std::to_string(GetAgility())
                     + "\nЗащита:                  " + std::to_string(GetDefense())
                     + "\nИнтеллект:               " + std::to_string(GetIntelligence())
                     + "\nКритический урон:       x" + std::to_string(GetTotalCrit())
                     + "\nШанс критического удара: " + std::to_string(GetTotalCritChance())
                     + "\nМана:                    " + std::to_string(mana) + "/" + std::to_string(maxMana) + "\n";
}

void Priest::UseSkill(BaseCharacter& target)
{
    if (!IsAlive())
    {
        throw CharacterIsDead(GetName());
    }

    if (mana < manaSkillCost)
    {
        throw NotEnoughMana(GetName(), manaSkillCost, mana);
    }

    mana -= manaSkillCost;
    int healAmount = GetIntelligence();
    std::cout << GetName() << " использует навык\n";
    Heal(healAmount);
}

void Priest::SkipTurn()
{
    mana = std::min(mana + manaAttackCost, maxMana);
    std::cout << GetName() << " пропускает ход и восстанавливает часть маны\n";
}

bool Priest::SpendMana(int amount)
{
    if (amount < 0 || mana < amount) return false;
    mana -= amount;
    return true;
}

void Priest::RestoreMana(int amount)
{
    if (amount > 0)
    {
        mana = std::min(mana + amount, maxMana);
    }
}

int Priest::GetMana() const
{
    return mana;
}

int Priest::GetMaxMana() const
{
    return maxMana;
}