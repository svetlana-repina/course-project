#include <iostream>
#include <random>
#include <cmath>
#include "character.hpp"

std::mt19937 Character::gen(std::random_device{}());

Character::Character()
    : name("Безымянный"), 
    health(100), 
    maxHealth(100), 
    strength(25), 
    stamina(50), 
    maxStamina(50), 
    mana(50), 
    maxMana(50), 
    agility(10), 
    defense(10), 
    intelligence(10), 
    crit(1.2), 
    critChance(10.0), 
    weapon()
{
    std::cout << "Создан персонаж по умолчанию\n";
}
Character::Character(const std::string& name, int maxHealth, int strength, int maxStamina, int maxMana, 
            int agility, int defense, int intelligence, double crit, double critChance, const std::string& weaponName, 
            int weaponDamage, const std::string& weaponType, int weaponDurability, double weaponBonusCrit, double weaponBonusCritChance)
    : name(name), 
    health(maxHealth), 
    maxHealth(maxHealth), 
    strength(strength), 
    stamina(maxStamina), 
    maxStamina(maxStamina),
    mana(maxMana), 
    maxMana(maxMana), 
    agility(agility), 
    defense(defense), 
    intelligence(intelligence), 
    crit(crit), 
    critChance(critChance),
    weapon(weaponName, weaponDamage, weaponType, weaponDurability, weaponBonusCrit, weaponBonusCritChance)
{
    if (maxHealth <= 0)
    {
        std::cout << "Ошибка! Здоровье должно быть больше 0. Установлено значение 100.\n";
        this->health = 100;
        this->maxHealth = 100;
    }
    if (maxStamina <= 0)
    {
        std::cout << "Ошибка! Выносливость должна быть больше 0. Установлено значение 25.\n";
        this->stamina = 25;
        this->maxStamina = 25;
    }
    if (maxMana < 0)
    {
        std::cout << "Ошибка! Мана должна быть >=0. Установлено значение 0.\n";
        this->mana = 0;
        this->maxMana = 0;
    }
    if (crit < 1.0)
    {
        std::cout << "Ошибка! Множитель критического урона должен быть >=1. Установлено значение 1.\n";
        this->crit = 1;
    }
    if (critChance < 0.0 || critChance > 100.0)
    {
        std::cout << "Ошибка! Шанс критического удара должен быть >=0 и <=100. Установлено значение 0.\n";
        this->critChance = 0;
    }
    std::cout << "Создан персонаж: " << name << "\n";
}
Character::~Character()
{
    std::cout << "Персонаж уничтожен: " << name << "\n";
}
std::string Character::getName() const 
{
    return name;
}
int Character::getHealth() const 
{
    return health;
}
int Character::getStrength() const 
{
    return strength;
}
int Character::getStamina() const 
{
    return stamina;
}
int Character::getMana() const 
{
    return mana;
}
int Character::getAgility() const 
{
    return agility;
}
int Character::getDefense() const 
{
    return defense;
}
int Character::getIntelligence() const 
{
    return intelligence;
}
double Character::getCrit() const 
{
    return crit;
}
double Character::getCritChance() const 
{
    return critChance;
}
const Weapon& Character::getWeapon() const 
{
    return weapon;
}

double Character::getTotalCritDamage() const
{
    return crit + weapon.getBonusCrit();
}
double Character::getTotalCritChance() const
{
    double totalCritChance = critChance + weapon.getBonusCritChance();
    if (totalCritChance > 100.0) 
    {
        totalCritChance = 100.0;
    }
    return totalCritChance;
}

int Character::attack(Character& target)
{
    if (!isAlive())
    {
        std::cout << "Персонаж мёртв и не может атаковать\n";
        return 0;
    }
    int baseDamage = weapon.calculateBaseDamage(strength);
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    double rand_num = dist(gen);
    bool isCrit = (rand_num < getTotalCritChance());
    int finalDamage;
    if (isCrit) finalDamage = static_cast<int>(std::round(baseDamage * getTotalCritDamage()));
    else finalDamage = baseDamage;
    int dealtDamage = target.takeDamage(finalDamage);
    weapon.use();
    std::cout << name << " атакует " << target.getName();
    if (isCrit) 
    {
        std::cout << " и наносит критический удар(x" << getTotalCritDamage() << ") " << dealtDamage << "\n"; 
    } 
    else std::cout << " и наносит " << dealtDamage << " урона.\n";
    return dealtDamage;
}
int Character::takeDamage(int amount)
{
    if (amount < 0) 
    {
        std::cout << "Ошибка! Урон не может быть отрицательным.\n";
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
void Character::heal(int amount)
{
    if (!isAlive())
    {
        std::cout << "Мёртвого персонажа нельзя вылечить\n";
        return;
    }
    if (amount <= 0)
    {
        std::cout << "Ошибка! Лечение должно быть больше 0.\n";
        return;
    }
    health+=amount;
    if (health > maxHealth) health = maxHealth;
    std::cout << name << " восстановил часть здоровья. Текущее здоровье: " << health << "/" << maxHealth << "\n";
}
bool Character::isAlive() const
{
    if (health > 0) return true;
    else return false;
}
void Character::printState() const
{
    std::cout << "============ " << name << " ============\n";
    std::cout << "Здоровье:                " << health << "/" << maxHealth << "\n";
    std::cout << "Сила:                    " << strength << "\n";
    std::cout << "Выносливость:            " << stamina << "/" << maxStamina << "\n";
    std::cout << "Мана:                    " << mana << "/" << maxMana << "\n";
    std::cout << "Ловкость:                " << agility << "\n";
    std::cout << "Защита:                  " << defense << "\n";
    std::cout << "Интеллект:               " << intelligence << "\n";
    std::cout << "Критический урон:        x" << getTotalCritDamage() << "\n";
    std::cout << "Шанс критического удара: " << getTotalCritChance() << "%\n";
    weapon.printInfo();
    std::cout << "==============================\n";
}