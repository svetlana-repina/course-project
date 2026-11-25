#pragma once
#include "base_character.hpp"
#include "imana_user.hpp"

class Priest final : public BaseCharacter, public IManaUser
{
private:
    int mana;
    int maxMana;
    static constexpr int manaAttackCost = 5;
    static constexpr int manaSkillCost = 15;

public:
    Priest(const std::string& name, int maxHealth, std::shared_ptr<Weapon> weapon, int maxMana);
    ~Priest() noexcept override;

    [[nodiscard]] int Attack(BaseCharacter& target) override;
    [[nodiscard]] std::unique_ptr<BaseCharacter> Clone() const override;
    [[nodiscard]] std::string Describe() const override;
    void UseSkill(BaseCharacter& target) override;
    void SkipTurn() override;

    bool SpendMana(int amount) override;
    void RestoreMana(int amount) override;
    [[nodiscard]] int GetMana() const override;
    [[nodiscard]] int GetMaxMana() const override;
};