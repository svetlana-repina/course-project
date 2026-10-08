#pragma once
#include <string>
#include <random>
#include "weapon.hpp"

class Character
{
    private:
        std::string name;
        int health;
        int maxHealth;
        int strength;
        int stamina;
        int maxStamina;
        int mana;
        int maxMana;
        int agility;
        int defense;
        int intelligence;
        double crit;
        double critChance;
        Weapon weapon;
        static std::mt19937 gen;

    public:
        Character();
        Character(const std::string& name, int maxHealth, int strength, int maxStamina, int maxMana, 
            int agility, int defense, int intelligence, double crit, double critChance, const std::string& weaponName, 
            int weaponDamage, const std::string& weaponType, int weaponDurability, double weaponBonusCrit, double weaponBonusCritChance);
        ~Character();

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] int getHealth() const;
        [[nodiscard]] int getStrength() const;
        [[nodiscard]] int getStamina() const;
        [[nodiscard]] int getMana() const;
        [[nodiscard]] int getAgility() const;
        [[nodiscard]] int getDefense() const;
        [[nodiscard]] int getIntelligence() const;
        [[nodiscard]] double getCrit() const;
        [[nodiscard]] double getCritChance() const;
        [[nodiscard]] const Weapon& getWeapon() const;
        int attack(Character& target);
        int takeDamage(int amount);
        void heal(int amount);
        [[nodiscard]] double getTotalCritDamage() const;
        [[nodiscard]] double getTotalCritChance() const;
        [[nodiscard]] bool isAlive() const;
        void printState() const;
};