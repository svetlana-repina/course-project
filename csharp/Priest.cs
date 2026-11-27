using System;
using static System.Math;

namespace RpgBattleSystem
{
    public sealed class Priest : BaseCharacter, IManaUser
    {
        private const int ManaAttackCost = 5;
        private const int ManaSkillCost = 15;

        public int Mana { get; private set; }
        public int MaxMana { get; }

        public Priest(string name, int maxHealth, Weapon weapon, int maxMana)
            : base(name, maxHealth, 3, 10, 10, 5, 15, 1.75, 15.0, weapon)
        {
            if (maxMana <= 0)
            {
                throw new InvalidCharacterParameter("maxMana", maxMana);
            }
            Mana = maxMana;
            MaxMana = maxMana;

            Console.WriteLine($"Создан жрец: {Name}");
        }

        private Priest(Priest other) : base(other)
        {
            Mana = other.Mana;
            MaxMana = other.MaxMana;
            Console.WriteLine($"Создана копия жреца: {Name}");
        }

        public Priest ShallowCopy() => (Priest)MemberwiseClone();

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
            int intelligenceBonus = Intelligence / 4;
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
            if (Mana < ManaSkillCost)
            {
                throw new NotEnoughMana(Name, ManaSkillCost, Mana);
            }

            Mana -= ManaSkillCost;
            int healAmount = Intelligence;
            Console.WriteLine($"{Name} использует навык");
            Heal(healAmount);
        }

        public override void SkipTurn()
        {
            Mana = Min(Mana + ManaAttackCost, MaxMana);
            Console.WriteLine($"{Name} пропускает ход и восстанавливает часть маны");
        }

        public override BaseCharacter Clone() => new Priest(this);

        public override string Describe() =>
                $"{Name}\n" +
                $"Здоровье:                {Health}/{MaxHealth}\n" +
                $"Выносливость:            {Stamina}/{MaxStamina}\n" +
                $"Ловкость:                {Agility}\n" +
                $"Защита:                  {Defense}\n" +
                $"Интеллект:               {Intelligence}\n" +
                $"Критический урон:       x{GetTotalCrit()}\n" +
                $"Шанс критического удара: {GetTotalCritChance():F2}%\n" +
                $"Мана:                    {Mana}/{MaxMana}\n";

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