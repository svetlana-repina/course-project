using System;
using static System.Math;

namespace RpgBattleSystem
{
    public sealed class Priest : BaseCharacter
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
                Console.WriteLine("Ошибка! Мана должна быть >0. Установлено значение 10.");
                maxMana = 10;
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
                Console.WriteLine($"{Name} мёртв и не может атаковать");
                return 0;
            }
            if (!target.IsAlive())
            {
                Console.WriteLine($"{target.Name} уже мёртв");
                return 0;
            }
            if (Mana < ManaAttackCost)
            {
                Console.WriteLine($"{Name} не хватает маны для атаки");
                SkipTurn();
                return 0;
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
                Console.WriteLine($"{Name} мёртв и не может атаковать");
                return;
            }
            if (Mana < ManaSkillCost)
            {
                Console.WriteLine($"{Name} не хватает маны для использования навыка");
                SkipTurn();
                return;
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
    }
}