using System;

namespace RpgBattleSystem
{
    public sealed class Rogue : BaseCharacter
    {
        private const int StaminaAttackCost = 3;
        private const int StaminaSkillCost = 10;

        public int PoisonDamage { get; }

        public Rogue(string name, int maxHealth, Weapon weapon, int poisonDamage)
            : base(name, maxHealth, 5, 65, 20, 5, 3, 1.5, 20.0, weapon)
        {
            if (poisonDamage < 0)
            {
                Console.WriteLine("Ошибка! Урон от яда не может быть <0. Установлено значение 0.");
                poisonDamage = 0;
            }
            PoisonDamage = poisonDamage;
            Console.WriteLine($"Создан разбойник: {Name}");
        }

        private Rogue(Rogue other) : base(other)
        {
            PoisonDamage = other.PoisonDamage;
            Console.WriteLine($"Создана копия разбойника: {Name}");
        }

        public Rogue ShallowCopy() => (Rogue)MemberwiseClone();

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

            int totalDealtDamage = 0;

            for (int i = 0; i < 2; i++)
            {
                if (!SpendStamina(StaminaAttackCost))
                {
                    Console.WriteLine($"{Name} слишком устал и не может атаковать");
                    SkipTurn();
                    break;
                }

                int baseDamage = Weapon.CalculateBaseDamage(Strength);
                int finalDamage = ApplyCrit(baseDamage);
                bool isCrit = (finalDamage > baseDamage);
                int dealtDamage = target.TakeDamage(finalDamage);
                BattleLog.LogAttack(this, target);
                totalDealtDamage += dealtDamage;
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

            }

            return totalDealtDamage;
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

            for (int i = 0; i < 2; i++)
            {
                int baseDamage = Weapon.CalculateBaseDamage(Strength);
                int skillDamage = ApplyCrit(baseDamage) + PoisonDamage;
                bool isCrit = (skillDamage > baseDamage + PoisonDamage);
                int dealtDamage = target.TakeDamage(skillDamage);
                BattleLog.LogAttack(this, target);
                Weapon.Use();

                Console.Write($"{Name} использует навык и наносит удар ядовитым оружием {target.Name} и наносит {dealtDamage}");
                if (isCrit)
                {
                    Console.Write("(КРИТ)");
                }
                Console.WriteLine();
            }
        }

        public override void SkipTurn()
        {
            RestoreStamina(StaminaAttackCost);
            Console.WriteLine($"{Name} пропускает ход и восстанавливает часть энергии");
        }

        public override BaseCharacter Clone() => new Rogue(this);

        public override string Describe() =>
                $"{Name}\n" +
                $"Здоровье:                {Health}/{MaxHealth}\n" +
                $"Выносливость:            {Stamina}/{MaxStamina}\n" +
                $"Ловкость:                {Agility}\n" +
                $"Защита:                  {Defense}\n" +
                $"Интеллект:               {Intelligence}\n" +
                $"Критический урон:       x{GetTotalCrit()}\n" +
                $"Шанс критического удара: {GetTotalCritChance():F2}%\n" +
                $"Урон от яда:             {PoisonDamage}\n";
    }
}