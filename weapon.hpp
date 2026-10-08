#pragma once
#include <string>

class Weapon{
    private:
        std::string name;
        int damage;
        std::string damageType;
        int durability;
        int maxDurability;
        double bonusCrit;
        double bonusCritChance;

    public:
        Weapon();
        Weapon(const std::string& name, int damage, const std::string& damageType, 
            int maxDurability, double bonusCrit, double bonusCritChance);
        ~Weapon();

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] int getDamage() const;
        [[nodiscard]] std::string getDamageType() const;
        [[nodiscard]] int getDurability() const;
        [[nodiscard]] double getBonusCrit() const;
        [[nodiscard]] double getBonusCritChance() const;
        [[nodiscard]] int calculateBaseDamage(int strength) const;
        bool use();
        [[nodiscard]] bool isBroken() const;
        void printInfo() const;
};