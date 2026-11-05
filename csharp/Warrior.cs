using System;
using static System.Math;

namespace RpgBattleSystem
{
    public sealed class Warrior : BaseCharacter
    {
        private const int RagePerAttack = 5;
        private const int MaxRage = 100;
        private const int StaminaAttackCost = 3;
        private const int StaminaSkillCost = 10;

        public int Rage { get; private set; }

        public Warrior(string name, int maxHealth, Weapon weapon, int rage)
            : base(name, maxHealth, 10, 50, 5, 10, 3, 1.2, 15.0, weapon)
        {
            if (rage < 0)
            {
                Console.WriteLine("Ошибка! Ярость должна быть >=0. Установлено значение 0.");
                rage = 0;
            }
            if (rage > MaxRage)
            {
                Console.WriteLine($"Ошибка! Ярость не может быть больше {MaxRage}. Установлено значение {MaxRage}.");
                rage = MaxRage;
            }
            Rage = rage;
            Console.WriteLine($"Создан воин: {Name}");
        }

        private Warrior(Warrior other) : base(other)
        {
            Rage = other.Rage;
            Console.WriteLine($"Создана копия воина: {Name}");
        }

        public Warrior ShallowCopy() => (Warrior)MemberwiseClone();

        public override int Attack(BaseCharacter target)
        {
            if (!IsAlive())
            {
                Console.WriteLine($"{Name} мёртв и не может атаковать");
                return 0;
            }
            if (!target.IsAlive())
            {
                Console.WriteLine($"{target.Name} уже мёртв");
                return 0;
            }
            if (!SpendStamina(StaminaAttackCost))
            {
                Console.WriteLine($"{Name} слишком устал и не может атаковать");
                SkipTurn();
                return 0;
            }

            int rageBonus = (int)(Rage * 0.1);
            int baseDamage = Weapon.CalculateBaseDamage(Strength) + rageBonus;
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

            Rage = Min(Rage + RagePerAttack, MaxRage);
            return dealtDamage;
        }

        public override void UseSkill(BaseCharacter target)
        {
            if (!IsAlive())
            {
                Console.WriteLine($"{Name} мёртв и не может атаковать");
                return;
            }
            if (!target.IsAlive())
            {
                Console.WriteLine($"{target.Name} уже мёртв");
                return;
            }
            if (!SpendStamina(StaminaSkillCost))
            {
                Console.WriteLine($"{Name} слишком устал и не может использовать навык");
                SkipTurn();
                return;
            }

            int doubleDamage = Weapon.CalculateBaseDamage(Strength) * 2;
            int finalDamage = ApplyCrit(doubleDamage);
            bool isCrit = finalDamage > doubleDamage;
            int dealtDamage = target.TakeDamage(finalDamage);
            BattleLog.LogAttack(this, target);
            Weapon.Use();

            Console.Write($"{Name} использует навык на {target.Name} и наносит двойной урон ({dealtDamage})");
            if (isCrit) 
            {
                Console.Write("(КРИТ)");
            }
            Console.WriteLine();
            Rage = Min(Rage + 2 * RagePerAttack, MaxRage);
        }

        public override void SkipTurn()
        {
            RestoreStamina(StaminaAttackCost);
            Rage = Max(Rage - RagePerAttack, 0);
            Console.WriteLine($"{Name} пропускает ход, восстанавливает часть энергии и становится немного спокойнее");
        }

        public override BaseCharacter Clone() => new Warrior(this);

        public override string Describe() => base.Describe() + $"Ярость:                  {Rage}/{MaxRage}\n";
    }
}