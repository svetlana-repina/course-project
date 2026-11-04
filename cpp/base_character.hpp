#pragma once
#include <string>
#include <memory>
#include <random>
#include "weapon.hpp"

class BaseCharacter
{
private:
    std::string name;
    int health;
    int maxHealth;
    int strength;
    int stamina;
    int maxStamina;
    int agility;
    int defense;
    int intelligence;
    double crit;
    double critChance;
    std::shared_ptr<Weapon> weapon;
    static int count;
    static std::mt19937 gen;

public:
    BaseCharacter();
    BaseCharacter(const std::string& name, int maxHealth, int strength, int maxStamina, int agility, 
        int defense, int intelligence, double crit, double critChance, std::shared_ptr<Weapon> weapon);
    BaseCharacter(const BaseCharacter& other);
    BaseCharacter(BaseCharacter&& other) noexcept;
    BaseCharacter& operator=(const BaseCharacter& other);
    BaseCharacter& operator=(BaseCharacter&& other) noexcept;
    virtual ~BaseCharacter();
    [[nodiscard]] virtual int Attack(BaseCharacter& target) = 0;
    [[nodiscard]] virtual std::unique_ptr<BaseCharacter> Clone() const = 0;
    [[nodiscard]] virtual std::string Describe() const;
    virtual void UseSkill(BaseCharacter& target) = 0;
    virtual void SkipTurn() = 0;
    [[nodiscard]] static int GetCount();
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] int GetHealth() const;
    [[nodiscard]] int GetStrength() const;
    [[nodiscard]] int GetStamina() const;
    [[nodiscard]] int GetAgility() const;
    [[nodiscard]] int GetDefense() const;
    [[nodiscard]] int GetIntelligence() const;
    [[nodiscard]] double GetTotalCrit() const;
    [[nodiscard]] double GetTotalCritChance() const;
    [[nodiscard]] std::shared_ptr<Weapon> GetWeapon() const;
    int TakeDamage(int amount);
    void Heal(int amount);
    [[nodiscard]] bool IsAlive() const;

protected:
    bool SpendStamina(int amount);
    void RestoreStamina(int amount);
    [[nodiscard]] bool IsCrit() const;
    [[nodiscard]] int ApplyCrit(int baseDamage) const;

};