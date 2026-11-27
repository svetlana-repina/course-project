using System;

namespace RpgBattleSystem
{
    public class NotEnoughMana : Exception
    {
        public NotEnoughMana(string name, int need, int have)
            : base($"{name} не хватает маны (нужно: {need}, есть: {have})")
        {}
    }

    public class NotEnoughStamina : Exception
    {
        public NotEnoughStamina(string name, int need, int have)
            : base($"{name} не хватает выносливости (нужно: {need}, есть: {have})")
        {}
    }

    public class InvalidCharacterParameter : ArgumentException
    {
        public InvalidCharacterParameter(string field, int value)
            : base($"Недопустимое значение в параметре персонажа({field}): {value}")
        {}

        public InvalidCharacterParameter(string field, double value)
            : base($"Недопустимое значение в параметре персонажа({field}): {value}")
        {}
    }

    public class CharacterIsDead : InvalidOperationException
    {
        public CharacterIsDead(string name)
            : base($"{name} уже мёртв")
        {}
    }
}