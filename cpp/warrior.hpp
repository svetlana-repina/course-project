#pragma once
#include "base_character.hpp"

class Warrior final : public BaseCharacter
{
private:
    int rage;
    static constexpr int ragePerAttack = 5;
    static constexpr int maxRage = 100;
    static constexpr int staminaAttackCost = 3;
    static constexpr int staminaSkillCost = 10;
    
public:
    Warrior(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int rage);
    ~Warrior() override;
    [[nodiscard]] int Attack(BaseCharacter& target) override;
    [[nodiscard]] std::unique_ptr<BaseCharacter> Clone() const override;
    [[nodiscard]] std::string Describe() const override;
    void UseSkill(BaseCharacter& target) override;
    void SkipTurn() override;
    [[nodiscard]] int GetRage() const;
};