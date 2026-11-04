#pragma once
#include "base_character.hpp"

class Mage final : public BaseCharacter
{
private:
    int mana;
    int maxMana;
    static constexpr int manaAttackCost = 5;
    static constexpr int manaSkillCost = 20;

public:
    Mage(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int maxMana);
    ~Mage() override;
    [[nodiscard]] int Attack(BaseCharacter& target) override;
    [[nodiscard]] std::unique_ptr<BaseCharacter> Clone() const override;
    [[nodiscard]] std::string Describe() const override;
    void UseSkill(BaseCharacter& target) override;
    void SkipTurn() override;
    [[nodiscard]] int GetMana() const;
};