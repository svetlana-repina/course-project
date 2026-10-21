using System;

namespace RpgBattleSystem
{
    public class Character
    {
        private static readonly Random _random = new Random();

        public string Name { get; }
        public int MaxHealth { get; }
        public int Strength { get; }
        public int MaxStamina { get; }
        public int MaxMana { get; }
        public int Agility { get; }
        public int Defense { get; }
        public int Intelligence { get; }

        private int _health;
        public int Health
        {
            get => _health;
            private set => _health = Math.Max(0, value);
        }

        private int _stamina;
        public int Stamina
        {
            get => _stamina;
            private set => _stamina = Math.Max(0, value);
        }

        private int _mana;
        public int Mana
        {
            get => _mana;
            private set => _mana = Math.Max(0, value);
        }

        private double _crit;
        public double Crit
        {
            get => _crit;
            private set => _crit = value >= 1.0 ? value : 1.0;
        }

        private double _critChance;
        public double CritChance
        {
            get => _critChance;
            private set => _critChance = (value <= 100 && value >= 0.0) ? value : 0.0;
        }

        private readonly Weapon _weapon;
        public Weapon Weapon => _weapon;

        public Character()
            : this("Безымянный", 100, 25, 50, 50, 10, 10, 10, 1.2, 10.0, "Кулак", 1, DamageType.Physical, 100, 0.0, 0.0)
        {
            Console.WriteLine($"Создан персонаж по умолчанию: {Name}");
        }

        public Character(string name, int maxHealth, int strength, int maxStamina, int maxMana, 
                        int agility, int defense, int intelligence, double crit, double critChance, 
                        string weaponName, int weaponDamage, DamageType weaponType, 
                        int weaponDurability, double weaponBonusCrit, double weaponBonusCritChance)
        {
            Name = name;
            Strength = strength;
            Agility = agility;
            Defense = defense;
            Intelligence = intelligence;

            if (maxHealth <= 0)
            {
                Console.WriteLine("Ошибка! Здоровье должно быть больше 0. Установлено значение 100.");
                maxHealth = 100;
            }
            MaxHealth = maxHealth;
            Health = maxHealth;

            if (maxStamina <= 0)
            {
                Console.WriteLine("Ошибка! Выносливость должна быть больше 0. Установлено значение 25.");
                maxStamina = 25;
            }
            MaxStamina = maxStamina;
            Stamina = maxStamina;

            if (maxMana < 0)
            {
                Console.WriteLine("Ошибка! Мана должна быть >=0. Установлено значение 0.");
                maxMana = 0;
            }
            MaxMana = maxMana;
            Mana = maxMana;

            if (crit < 1.0)
            {
                Console.WriteLine("Ошибка! Множитель критического урона должен быть >=1. Установлено значение 1.");
            }
            Crit = crit;

            if (critChance < 0.0 || critChance > 100.0)
            {
                Console.WriteLine("Ошибка! Шанс критического удара должен быть >=0 и <=100. Установлено значение 0.");
            }
            CritChance = critChance;

            _weapon = new Weapon(weaponName, weaponDamage, weaponType, weaponDurability, weaponBonusCrit, weaponBonusCritChance);
            Console.WriteLine($"Создан персонаж: {Name}");
        }

        public double GetTotalCrit()
        {
            return Crit + _weapon.BonusCrit;
        }

        public double GetTotalCritChance()
        {
            double totalCritChance = CritChance + _weapon.BonusCritChance;
            return totalCritChance > 100.0 ? 100.0 : totalCritChance;
        }

        public int Attack(Character target)
        {
            if (!IsAlive())
            {
                Console.WriteLine("Персонаж мёртв и не может атаковать");
                return 0;
            }
            if (!target.IsAlive())
            {
                Console.WriteLine("Нельзя атаковать мёртвого персонажа");
                return 0;
            }
            int baseDamage = _weapon.CalculateBaseDamage(Strength);
            double rand_num = _random.NextDouble() * 100.0;
            bool isCrit = rand_num < GetTotalCritChance();
            int finalDamage;
            if (isCrit)
            {
                finalDamage = (int)Math.Round(baseDamage * GetTotalCrit());
            }
            else
            {
                finalDamage = baseDamage;
            }
            int dealtDamage = target.TakeDamage(finalDamage);
            _weapon.Use();
            Console.Write($"{Name} атакует {target.Name}");
            if (isCrit)
            {
                Console.WriteLine($" и наносит критический удар (x{GetTotalCrit()}) {dealtDamage}");
            }
            else
            {
                Console.WriteLine($" и наносит {dealtDamage} урона");
            }
            return dealtDamage;
        }

        public int TakeDamage(int amount)
        {
            if (amount < 0)
            {
                Console.WriteLine("Ошибка! Урон не может быть отрицательным.");
                return 0;
            }
            int reducedHealth = amount - Defense;
            if (reducedHealth < 0)
            {
                reducedHealth = 0;
            }
            if (reducedHealth > Health)
            {
                reducedHealth = Health;
            }
            Health -= reducedHealth;
            return reducedHealth;
        }

        public void Heal(int amount)
        {
            if (!IsAlive())
            {
                Console.WriteLine("Мёртвого персонажа нельзя вылечить");
                return;
            }
            if (amount <= 0)
            {
                Console.WriteLine("Ошибка! Лечение должно быть больше 0.");
                return;
            }
            Health += amount;
            if (Health > MaxHealth)
            {
                Health = MaxHealth;
            }
            Console.WriteLine($"{Name} восстановил часть здоровья. Текущее здоровье: {Health}/{MaxHealth}");
        }

        public bool IsAlive()
        {
            return Health > 0;
        }

        public override string ToString()
        {
            return $"============ {Name} ============\n" +
                   $"Здоровье:                {Health}/{MaxHealth}\n" +
                   $"Сила:                    {Strength}\n" +
                   $"Выносливость:            {Stamina}/{MaxStamina}\n" +
                   $"Мана:                    {Mana}/{MaxMana}\n" +
                   $"Ловкость:                {Agility}\n" +
                   $"Защита:                  {Defense}\n" +
                   $"Интеллект:               {Intelligence}\n" +
                   $"Критический урон:        x{GetTotalCrit()}\n" +
                   $"Шанс критического удара: {GetTotalCritChance()}%\n" +
                   _weapon.ToString() + "\n" +
                   "====================================";

        }
    }
}