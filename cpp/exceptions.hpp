#pragma once
#include <stdexcept>
#include <string>

class NotEnoughMana : public std::runtime_error
{
public:
    NotEnoughMana(const std::string& name, int need, int have)
        : std::runtime_error(name + " не хватает маны (нужно: " + std::to_string(need) + ", есть: " + std::to_string(have) + ")")
        {}
};

class NotEnoughStamina : public std::runtime_error
{
public:
    NotEnoughStamina(const std::string& name, int need, int have)
        : std::runtime_error(name + " не хватает выносливости (нужно: " + std::to_string(need) + ", есть: " + std::to_string(have) + ")")
    {}
};

class InvalidCharacterParameter : public std::invalid_argument
{
public:
    InvalidCharacterParameter(const std::string& field, int value)
        : std::invalid_argument("Недопустимое значение в параметре персонажа(" + field + "): " + std::to_string(value))
    {}

    InvalidCharacterParameter(const std::string& field, double value)
        : std::invalid_argument("Недопустимое значение в параметре персонажа(" + field + "): " + std::to_string(value))
    {}
};

class CharacterIsDead : public std::logic_error
{
public:
    explicit CharacterIsDead(const std::string& name)
        : std::logic_error(name + " уже мёртв")
    {}
};