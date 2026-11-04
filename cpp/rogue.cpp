#include "rogue.hpp"
#include "battle_log.hpp"
#include <iostream>
#include <algorithm>

Rogue::Rogue(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int poisonDamage)
    : BaseCharacter(name, maxHealth, 5, 65, 20, 5, 3, 1.5, 20.0, std::move(weapon)),
    poisonDamage(poisonDamage)
{
    if (poisonDamage < 0)
    {
        std::cout << "Ошибка! Урон от яда не может быть <0. Установлено значение 0.\n";
        this->poisonDamage = 0;
    }
    std::cout << "Создан разбойник: " << GetName() << "\n";
}

Rogue::~Rogue()
{
    std::cout << "Разбойник уничтожен: " << GetName() << "\n";
}

int Rogue::Attack(BaseCharacter& target)
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

    int totalDealtDamage = 0;

    for (int i = 0; i < 2; i++)
    {
        if (!SpendStamina(staminaAttackCost))
        {
            std::cout << GetName() << " устал и не может атаковать\n";
            SkipTurn();
            break;
        }

        int baseDamage = GetWeapon()->calculateBaseDamage(GetStrength());
        int finalDamage = ApplyCrit(baseDamage);
        bool isCrit = (finalDamage > baseDamage);
        int dealtDamage = target.TakeDamage(finalDamage);
        BattleLog::LogAttack(*this, target);
        totalDealtDamage += dealtDamage;
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
    }

    return totalDealtDamage;
}

std::unique_ptr<BaseCharacter> Rogue::Clone() const
{
    return std::make_unique<Rogue>(*this);
}

std::string Rogue::Describe() const
{
    return GetName() + "\nЗдоровье:                " + std::to_string(GetHealth())
                     + "\nВыносливость:            " + std::to_string(GetStamina())
                     + "\nЛовкость:                " + std::to_string(GetAgility())
                     + "\nЗащита:                  " + std::to_string(GetDefense())
                     + "\nИнтеллект:               " + std::to_string(GetIntelligence())
                     + "\nКритический урон:       x" + std::to_string(GetTotalCrit())
                     + "\nШанс критического удара: " + std::to_string(GetTotalCritChance()) + "\n";
}
    
void Rogue::UseSkill(BaseCharacter& target)
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
        std::cout << GetName() << " устал и не может использовать навык\n";
        SkipTurn();
        return;
    }

    for (int i = 0; i < 2; i++)
    {
        int baseDamage = GetWeapon()->calculateBaseDamage(GetStrength());
        int skillDamage = ApplyCrit(baseDamage) + poisonDamage;
        bool isCrit = (skillDamage > baseDamage + poisonDamage);
        int dealtDamage = target.TakeDamage(skillDamage);
        BattleLog::LogAttack(*this, target);
        GetWeapon()->use();
        std::cout << GetName() << " использует навык и наносит удар ядовитым оружием " << target.GetName() << " и наносит " << dealtDamage;
        if (isCrit)
        {
            std::cout << " (КРИТ)";
        }
        std::cout << "\n";
    }
}

void Rogue::SkipTurn()
{
    RestoreStamina(staminaAttackCost);
    std::cout << GetName() << " пропускает ход и восстанавливает часть энергии\n";
}

int Rogue::GetPoisonDamage() const
{
    return poisonDamage;
}