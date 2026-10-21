#pragma once
#include <string>

enum class DamageType
{
    Physical,
    Magical
};

class Weapon
{
    private:
        std::string name;
        int damage;
        DamageType damageType;
        int durability;
        int maxDurability;
        double bonusCrit;
        double bonusCritChance;

    public:
        Weapon();
        Weapon(const std::string& name, int damage, DamageType damageType, 
            int maxDurability, double bonusCrit, double bonusCritChance);
        ~Weapon();

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] int getDamage() const;
        [[nodiscard]] DamageType getDamageType() const;
        [[nodiscard]] int getDurability() const;
        [[nodiscard]] double getBonusCrit() const;
        [[nodiscard]] double getBonusCritChance() const;
        [[nodiscard]] std::string damageTypeToString() const;
        [[nodiscard]] int calculateBaseDamage(int strength) const;
        bool use();
        [[nodiscard]] bool isBroken() const;
        void printInfo() const;
};