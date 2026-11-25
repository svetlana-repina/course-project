#include "base_character.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <utility>
#include <cmath>
#include <algorithm>

int BaseCharacter::count = 0;
std::mt19937 BaseCharacter::gen(std::random_device{}());

BaseCharacter::BaseCharacter()
    : name("Безымянный"), 
    health(100), 
    maxHealth(100), 
    strength(3),
    stamina(25), 
    maxStamina(25), 
    agility(5),
    defense(0), 
    intelligence(3),
    crit(1.0),
    critChance(0.0),
    weapon(std::make_shared<Weapon>())
{
    count++;
    std::cout << "Создан персонаж по умолчанию\n";
}

BaseCharacter::BaseCharacter(const std::string& name, int maxHealth, int strength, int maxStamina, int agility, 
        int defense, int intelligence, double crit, double critChance, std::shared_ptr<Weapon> weapon)
    : name(name), 
    health(maxHealth), 
    maxHealth(maxHealth),
    strength(strength),
    stamina(maxStamina),
    maxStamina(maxStamina),
    agility(agility),
    defense(defense),
    intelligence(intelligence),
    crit(crit),
    critChance(critChance),
    weapon(weapon ? std::move(weapon) : std::make_shared<Weapon>())
{
    if (maxHealth <= 0)
    {
        throw InvalidCharacterParameter("maxHealth", maxHealth);
    }
    if (strength < 0)
    {
        throw InvalidCharacterParameter("strength", strength);
    }
    if (maxStamina <= 0)
    {
        throw InvalidCharacterParameter("maxStamina", maxStamina);
    }
    if (agility < 0)
    {
        throw InvalidCharacterParameter("agility", agility);
    }
    if (defense < 0)
    {
        throw InvalidCharacterParameter("defense", defense);
    }
    if (intelligence < 0)
    {
        throw InvalidCharacterParameter("intelligence", intelligence);
    }
    if (crit < 1.0)
    {
        throw InvalidCharacterParameter("crit", crit);
    }
    if (critChance < 0.0 || critChance > 100.0)
    {
        throw InvalidCharacterParameter("critChance", critChance);
    }
    count++;
    std::cout << "Создан персонаж: " << this->name << "\n";
}

BaseCharacter::BaseCharacter(const BaseCharacter& other)
    : name(other.name), health(other.health), maxHealth(other.maxHealth), strength(other.strength), stamina(other.stamina), maxStamina(other.maxStamina), 
    agility(other.agility), defense(other.defense), intelligence(other.intelligence), crit(other.crit), critChance(other.critChance)
{
    if (other.weapon) 
    {
        weapon = std::make_shared<Weapon>(*other.weapon);
    } 
    else 
    {
        weapon = nullptr;
    }

    count++;
    std::cout << "Создана глубокая копия: " << name << "\n";
}

BaseCharacter::BaseCharacter(BaseCharacter&& other) noexcept
    : name(std::move(other.name)), health(other.health), maxHealth(other.maxHealth), strength(other.strength), stamina(other.stamina), maxStamina(other.maxStamina), 
    agility(other.agility), defense(other.defense), intelligence(other.intelligence), crit(other.crit), critChance(other.critChance), weapon(std::move(other.weapon))
{
    std::cout << "Перемещение: " << name << "\n";
}

BaseCharacter& BaseCharacter::operator=(const BaseCharacter& other)
{
    if (this == &other)
    {
        return *this;
    }

    name = other.name;
    health = other.health;
    maxHealth = other.maxHealth;
    strength = other.strength;
    stamina = other.stamina;
    maxStamina = other.maxStamina;
    agility = other.agility;
    defense = other.defense;
    intelligence = other.intelligence;
    crit = other.crit;
    critChance = other.critChance;

    if (other.weapon) 
    {
        weapon = std::make_shared<Weapon>(*other.weapon);
    } 
    else 
    {
        weapon = nullptr;
    }
    std::cout << "Глубокое присваивание: " << name << "\n";
    return *this;
}

BaseCharacter& BaseCharacter::operator=(BaseCharacter&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    name = std::move(other.name);
    health = other.health;
    maxHealth = other.maxHealth;
    strength = other.strength;
    stamina = other.stamina;
    maxStamina = other.maxStamina;
    agility = other.agility;
    defense = other.defense;
    intelligence = other.intelligence;
    crit = other.crit;
    critChance = other.critChance;
    weapon = std::move(other.weapon);

    std::cout << "Присваивание перемещением: " << name << "\n";
    return *this;
}

BaseCharacter::~BaseCharacter() noexcept
{
    count--;
    std::cout << "Персонаж уничтожен: " << name << "\n";
}

std::string BaseCharacter::Describe() const
{
    return name + "\nЗдоровье:                " + std::to_string(health) + "/" + std::to_string(maxHealth) 
                + "\nВыносливость:            " + std::to_string(stamina) + "/" + std::to_string(maxStamina)
                + "\nЛовкость:                " + std::to_string(agility)
                + "\nЗащита:                  " + std::to_string(defense)
                + "\nИнтеллект:               " + std::to_string(intelligence)
                + "\nКритический урон:       x" + std::to_string(GetTotalCrit())
                + "\nШанс критического удара: " + std::to_string(GetTotalCritChance()) + "\n";
}

int BaseCharacter::GetCount()
{
    return count;
}

std::string BaseCharacter::GetName() const
{
    return name;
}

int BaseCharacter::GetHealth() const
{
    return health;
}

int BaseCharacter::GetStrength() const
{
    return strength;
}

int BaseCharacter::GetStamina() const
{
    return stamina;
}

int BaseCharacter::GetAgility() const
{
    return agility;
}

int BaseCharacter::GetDefense() const
{
    return defense;
}

int BaseCharacter::GetIntelligence() const
{
    return intelligence;
}

double BaseCharacter::GetTotalCrit() const
{
    if (!weapon) 
    {
        return crit;
    }
    return crit + weapon->getBonusCrit();
}

double BaseCharacter::GetTotalCritChance() const
{
    if (!weapon) 
    {
        return critChance;
    }
    double total = critChance + weapon->getBonusCritChance();
    return total > 100.0 ? 100.0 : total;
}

std::shared_ptr<Weapon> BaseCharacter::GetWeapon() const
{
    return weapon;
}

std::string BaseCharacter::GetKind() const
{
    return "персонаж";
}

int BaseCharacter::TakeDamage(int amount)
{
    if (amount < 0) 
    {
        std::cout << "Ошибка! Урон не может быть отрицательным.\n";
        return 0;
    }

    double dodgeChance = agility * 1.5;
    if (dodgeChance > 50.0)
    {
        dodgeChance = 50.0;
    }

    std::uniform_real_distribution<double> dist(0.0, 100.0);
    double rand_num = dist(gen);
    if (rand_num < dodgeChance)
    {
        std::cout << name << " уклоняется от атаки!\n";
        return 0;
    }

    int reducedHealth = amount - defense;
    if (reducedHealth < 0) 
    {
        reducedHealth = 0;
    }
    if (reducedHealth > health) 
    {
        reducedHealth = health;
    }

    health -= reducedHealth;
    return reducedHealth;
}

void BaseCharacter::Heal(int amount)
{
    if (!IsAlive())
    {
        std::cout << "Мёртвого персонажа нельзя вылечить\n";
        return;
    }

    if (amount <= 0)
    {
        std::cout << "Ошибка! Лечение должно быть больше 0.\n";
        return;
    }

    health += amount;
    if (health > maxHealth) health = maxHealth;
    std::cout << name << " восстановил часть здоровья (" << amount << ").\n";
}

bool BaseCharacter::IsAlive() const
{
    return health > 0;
}

bool BaseCharacter::SpendStamina(int amount)
{
    if (amount > stamina || amount < 0)
    {
        return false;
    }
    else
    {
        stamina -= amount;
        return true;
    }
}

void BaseCharacter::RestoreStamina(int amount)
{
    if (amount > 0)
    {
        stamina = std::min(stamina + amount, maxStamina);
    }
}

bool BaseCharacter::IsCrit() const
{
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    double rand_num = dist(gen);
    return rand_num < GetTotalCritChance();
}
  
int BaseCharacter::ApplyCrit(int baseDamage) const
{
    if (IsCrit())
    {
        return static_cast<int>(std::round(baseDamage * GetTotalCrit()));
    }
    else
    {
        return baseDamage;
    }
}