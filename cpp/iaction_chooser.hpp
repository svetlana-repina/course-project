#pragma once

class BaseCharacter;

enum class ActionType
{
    Attack,
    Skill,
    Skip
};

class IActionChooser
{
public:
    virtual ~IActionChooser() = default;
    [[nodiscard]] virtual ActionType ChooseAction(const BaseCharacter& hero) const = 0;
};

class ConsoleActionChooser final : public IActionChooser
{
public:
    [[nodiscard]] ActionType ChooseAction(const BaseCharacter& hero) const override;
};

class RandomActionChooser final : public IActionChooser
{
public:
    [[nodiscard]] ActionType ChooseAction(const BaseCharacter&) const override;
};