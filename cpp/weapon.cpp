#include "weapon.hpp"
#include "exceptions.hpp"
#include <iostream>

Weapon::Weapon()
    : name("Кулак"), 
    damage(1), 
    damageType(DamageType::Physical), 
    durability(100), 
    maxDurability(100), 
    bonusCrit(0.0), 
    bonusCritChance(0.0)
{
    std::cout << "Создано оружие по умолчанию: " << name << "\n";
}

Weapon::Weapon(const std::string& name, int damage, DamageType damageType, int maxDurability, double bonusCrit, double bonusCritChance)
    : name(name), 
    damage(damage), 
    damageType(damageType), 
    durability(maxDurability), 
    maxDurability(maxDurability), 
    bonusCrit(bonusCrit), 
    bonusCritChance(bonusCritChance)
{
    if (damage <= 0)
    {
        throw InvalidCharacterParameter("damage", damage);
    }
    if (maxDurability <= 0 || maxDurability > 100)
    {
        throw InvalidCharacterParameter("maxDurability", maxDurability);
    }
    if (bonusCrit < 0.0)
    {
        throw InvalidCharacterParameter("bonusCrit", bonusCrit);
    }
    if (bonusCritChance < 0.0)
    {
        throw InvalidCharacterParameter("bonusCritChance", bonusCritChance);
    }
    std::cout << "Создано оружие: " << name << "\n";
}

Weapon::~Weapon() noexcept
{
    std::cout << "Оружие уничтожено: " << name << "\n";
}

std::string Weapon::getName() const 
{
    return name;
}

int Weapon::getDamage() const 
{
    return damage;
}

DamageType Weapon::getDamageType() const 
{
    return damageType;
}

int Weapon::getDurability() const 
{
    return durability;
}

double Weapon::getBonusCrit() const 
{
    return bonusCrit;
}

double Weapon::getBonusCritChance() const 
{
    return bonusCritChance;
}

std::string Weapon::damageTypeToString() const
{
    switch(damageType)
    {
        case DamageType::Physical: return "физический";
        case DamageType::Magical: return "магический";
    }
    return "неизвестный";
}

int Weapon::calculateBaseDamage(int strength) const 
{
    if (isBroken()) 
    {
        return 1;
    }
    return damage + strength;
}

bool Weapon::use()
{
    if (durability > 0) 
    {
        durability--;
        return true;
    }
    else 
    {
        std::cout << "Ваше оружие сломано!\n";
        return false;
    }
}

bool Weapon::isBroken() const
{
    return durability <= 0;
}

void Weapon::printInfo() const
{
    std::cout << "Оружие:             " << name << "\n" 
              << "Урон:               " << damage << "\n" 
              << "Тип урона:          " << damageTypeToString() << "\n"
              << "Прочность:          " << durability << " из " << maxDurability << "\n" 
              << "Бонус к крит урону: " << bonusCrit << "\n"
              << "Бонус к крит шансу: " << bonusCritChance << "\n";
}