#pragma once
#include "base_character.hpp"

class Rogue final : public BaseCharacter
{
private:
    int poisonDamage;
    static constexpr int staminaAttackCost = 3;
    static constexpr int staminaSkillCost = 10;

public:
    Rogue(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int poisonDamage);
    ~Rogue() override;
    [[nodiscard]] int Attack(BaseCharacter& target) override;
    [[nodiscard]] std::unique_ptr<BaseCharacter> Clone() const override;
    [[nodiscard]] std::string Describe() const override;
    void UseSkill(BaseCharacter& target) override;
    void SkipTurn() override;
    [[nodiscard]] int GetPoisonDamage() const;
};