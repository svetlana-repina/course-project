using System;
using static System.Math;

namespace RpgBattleSystem
{
    public sealed class Mage : BaseCharacter, IManaUser
    {
        private const int ManaAttackCost = 5;
        private const int ManaSkillCost = 20;

        public int Mana { get; private set; }
        public int MaxMana { get; }

        public Mage(string name, int maxHealth, Weapon weapon, int maxMana)
            : base(name, maxHealth, 3, 10, 10, 5, 25, 2.0, 25.0, weapon)
        {
            if (maxMana <= 0)
            {
                throw new InvalidCharacterParameter("maxMana", maxMana);
            }
            Mana = maxMana;
            MaxMana = maxMana;
            Console.WriteLine($"Создан маг: {Name}");
        }

        private Mage(Mage other) : base(other)
        {
            Mana = other.Mana;
            MaxMana = other.MaxMana;
            Console.WriteLine($"Создана копия мага: {Name}");
        }

        public Mage ShallowCopy() => (Mage)MemberwiseClone();

        public override int Attack(BaseCharacter target)
        {
            if (!IsAlive())
            {
                throw new CharacterIsDead(Name);
            }
            if (!target.IsAlive())
            {
                throw new CharacterIsDead(target.Name);
            }
            if (Mana < ManaAttackCost)
            {
                throw new NotEnoughMana(Name, ManaAttackCost, Mana);
            }

            Mana -= ManaAttackCost;
            int intelligenceBonus = (int)Math.Round(Intelligence / 1.5);
            int baseDamage = Weapon.CalculateBaseDamage(Strength) + intelligenceBonus;
            int finalDamage = ApplyCrit(baseDamage);
            bool isCrit = (finalDamage > baseDamage);
            int dealtDamage = target.TakeDamage(finalDamage);
            BattleLog.LogAttack(this, target);
            Weapon.Use();

            Console.Write($"{Name} атакует {target.Name}");
            if (isCrit)
            {
                Console.WriteLine($" и наносит критический удар(x{GetTotalCrit()}) {dealtDamage}");
            }
            else
            {
                Console.WriteLine($" и наносит {dealtDamage} урона.");
            }

            return dealtDamage;
        }

        public override void UseSkill(BaseCharacter target)
        {
            if (!IsAlive())
            {
                throw new CharacterIsDead(Name);
            }
            if (!target.IsAlive())
            {
                throw new CharacterIsDead(target.Name);
            }
            if (Mana < ManaSkillCost)
            {
                throw new NotEnoughMana(Name, ManaSkillCost, Mana);
            }

            Mana -= ManaSkillCost;
            int skillDamage = Intelligence * 2;
            int finalDamage = ApplyCrit(skillDamage);
            int dealtDamage = target.TakeDamage(finalDamage);
            BattleLog.LogAttack(this, target);

            Console.WriteLine($"{Name} использует навык, бросает огненный шар в {target.Name} и наносит {dealtDamage} урона");
        }

        public override void SkipTurn()
        {
            Mana = Min(Mana + ManaAttackCost, MaxMana);
            Console.WriteLine($"{Name} пропускает ход и восстанавливает часть маны");
        }

        public override BaseCharacter Clone() => new Mage(this);

        public override string Describe() => base.Describe() + $"Мана:                    {Mana}/{MaxMana}\n";

        public bool SpendMana(int amount)
        {
            if (amount < 0 || Mana < amount) return false;
            Mana -= amount;
            return true;
        }

        public void RestoreMana(int amount)
        {
            if (amount > 0)
            {
                Mana = Min(Mana + amount, MaxMana);
            }
        }
    }
}