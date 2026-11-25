#pragma once

class IManaUser
{
public:
    virtual ~IManaUser() = default;
    virtual bool SpendMana(int amount) = 0;
    virtual void RestoreMana(int amount) = 0;
    [[nodiscard]] virtual int GetMana() const = 0;
    [[nodiscard]] virtual int GetMaxMana() const = 0;
};