#include "weapon.hpp"
#include <iostream>

Weapon::Weapon()
    : name("Кулак"), 
    damage(1), 
    damageType("физический"), 
    durability(100), 
    maxDurability(100), 
    bonusCrit(0), 
    bonusCritChance(0)
{
    std::cout << "Создано оружие по умолчанию: " << name << "\n";
}

Weapon::Weapon(const std::string& name, int damage, const std::string& damageType, int maxDurability, double bonusCrit, double bonusCritChance)
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
        std::cout << "Ошибка! Урон оружия не может быть <=0. Установлено значение 1.\n";
        this->damage = 1;
    }
    if (maxDurability <= 0 || maxDurability > 100) 
    {
        std::cout << "Ошибка! Максимальная прочность оружия должна быть >0 и <=100. Установлено значение 100.\n";
        this->durability = 100;
        this->maxDurability = 100;
    }
    if (bonusCrit < 0.0) 
    {
        std::cout << "Ошибка! Бонус к крит урону не может быть <0. Установлено значение 0.\n";
        this->bonusCrit = 0;
    }
    if (bonusCritChance < 0.0) 
    {
        std::cout << "Ошибка! Бонус к крит шансу не может быть <0. Установлено значение 0.\n";
        this->bonusCritChance = 0;
    }
    std::cout << "Создано оружие: " << name << "\n";
}

Weapon::~Weapon()
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

std::string Weapon::getDamageType() const 
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
    std::cout << "Оружие:    " << name << "\n" 
              << "Урон:      " << damage << "\n" 
              << "Прочность: " << durability << " из " << maxDurability << "\n";
}