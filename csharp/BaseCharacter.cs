using System;

namespace RpgBattleSystem
{
    public abstract class BaseCharacter
    {
        private static int _count = 0;
        private static readonly Random _random = new Random();

        public string Name { get; }
        public int Health { get; private set; }
        public int MaxHealth { get; }
        public int Strength { get; }
        public int Stamina { get; private set; }
        public int MaxStamina { get; }
        public int Agility { get; }
        public int Defense { get; }
        public int Intelligence { get; }
        public double Crit { get; }
        public double CritChance { get; }
        public Weapon Weapon { get; private set; }

        protected BaseCharacter()
            : this("Безымянный", 100, 3, 25, 5, 0, 3, 1.0, 0.0, new Weapon())
        {
             Console.WriteLine($"Создан персонаж по умолчанию: {Name}");
        }

        protected BaseCharacter(string name, int maxHealth, int strength, int maxStamina, int agility, 
                                int defense, int intelligence, double crit, double critChance, Weapon weapon)
        {
            Name = name;
            if (maxHealth <= 0)
            {
                Console.WriteLine("Ошибка! Здоровье должно быть > 0. Установлено значение 100.");
                maxHealth = 100;
            }
            if (strength < 0)
            {
                Console.WriteLine("Ошибка! Сила должна быть >= 0. Установлено значение 3.");
                strength = 3;
            }
            if (maxStamina < 0)
            {
                Console.WriteLine("Ошибка! Выносливость должна быть >= 0. Установлено значение 25.");
                maxStamina = 25;
            }
            if (agility < 0)
            {
                Console.WriteLine("Ошибка! Ловкость должна быть >= 0. Установлено значение 5.");
                agility = 5;
            }

            if (defense < 0)
            {
                Console.WriteLine("Ошибка! Защита должна быть >= 0. Установлено значение 0.");
                defense = 0;
            }
            if (intelligence < 0)
            {
                Console.WriteLine("Ошибка! Интеллект должен быть >= 0. Установлено значение 3.");
                intelligence = 3;
            }
            if (crit < 1.0)
            {
                Console.WriteLine("Ошибка! Множитель критического урона должен быть >=1. Установлено значение 1.");
                crit = 1.0;
            }
            if (critChance < 0.0 || critChance > 100.0)
            {
                Console.WriteLine("Ошибка! Шанс критического удара должен быть >=0 и <=100. Установлено значение 0.");
                critChance = 0.0;
            }
            Health = maxHealth;
            MaxHealth = maxHealth;
            Strength = strength;
            Stamina = maxStamina;
            MaxStamina = maxStamina;
            Agility = agility;
            Defense = defense;
            Intelligence = intelligence;
            Crit = crit;
            CritChance = critChance;
            Weapon = weapon ?? new Weapon();

            _count++;
            Console.WriteLine($"Создан персонаж: {Name}");
        }

        protected BaseCharacter(BaseCharacter other)
        {
            Name = other.Name;
            Health = other.Health;
            MaxHealth = other.MaxHealth;
            Strength = other.Strength;
            Stamina = other.Stamina;
            MaxStamina = other.MaxStamina;
            Agility = other.Agility;
            Defense = other.Defense;
            Intelligence = other.Intelligence;
            Crit = other.Crit;
            CritChance = other.CritChance;
            Weapon = other.Weapon != null ? new Weapon(other.Weapon) : null;
            _count++;

            Console.WriteLine($"Создана копия персонажа: {Name}");
        }

        public abstract int Attack(BaseCharacter target);
        public abstract void UseSkill(BaseCharacter target);
        public abstract BaseCharacter Clone();
        public abstract void SkipTurn();

        public virtual string Describe()
        {
            return $"{Name}\nЗдоровье:               {Health}/{MaxHealth}\nВыносливость:            {Stamina}/{MaxStamina}\n" +
                   $"Ловкость:                {Agility}\nЗащита:                  {Defense}\nИнтеллект:               {Intelligence}\n" +
                   $"Критический урон:       x{GetTotalCrit()}\nШанс критического удара: {GetTotalCritChance():F2}%\n";
        }

        public double GetTotalCrit() => Crit + (Weapon?.BonusCrit ?? 0.0);

        public double GetTotalCritChance()
        {
            double total = CritChance + (Weapon?.BonusCritChance ?? 0.0);
            return total > 100.0 ? 100.0 : total;
        }

        public int TakeDamage(int amount)
        {
            if (amount < 0)
            {
                Console.WriteLine("Ошибка! Урон не может быть отрицательным.");
                return 0;
            }

            double dodgeChance = Agility * 1.5;
            if (dodgeChance > 50.0)
            {
                dodgeChance = 50.0;
            }

            double rand_num = _random.NextDouble() * 100;
            if (rand_num < dodgeChance)
            {
                Console.WriteLine($"{Name} уклоняется от атаки!");
                return 0;
            }

            int reducedHealth = Math.Max(amount - Defense, 0);
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

        public bool IsAlive() => Health > 0;

        public static int GetCount() => _count;

        public override bool Equals(object obj)
        {
            if (obj is not BaseCharacter other) 
            {
                return false;
            }
            return Name == other.Name
            && Health == other.Health
            && MaxHealth == other.MaxHealth
            && Strength == other.Strength
            && Stamina == other.Stamina
            && MaxStamina == other.MaxStamina
            && Agility == other.Agility
            && Defense == other.Defense
            && Intelligence == other.Intelligence
            && Crit == other.Crit
            && CritChance == other.CritChance
            && Weapon?.Name == other.Weapon?.Name
            && Weapon?.Durability == other.Weapon?.Durability;
        }

        public override int GetHashCode()
        {
            var hash = new HashCode();
            hash.Add(Name);
            hash.Add(Health);
            hash.Add(MaxHealth);
            hash.Add(Strength);
            hash.Add(Stamina);
            hash.Add(MaxStamina);
            hash.Add(Agility);
            hash.Add(Defense);
            hash.Add(Intelligence);
            hash.Add(Crit);
            hash.Add(CritChance);
            hash.Add(Weapon?.Name);
            hash.Add(Weapon?.Durability);
            return hash.ToHashCode();
        }

        protected bool SpendStamina(int amount)
        {
            if (amount < 0 || amount > Stamina) 
            {
                return false;
            }
            Stamina -= amount;
            return true;
        }

        protected void RestoreStamina(int amount)
        {
            if (amount > 0) 
            {
                Stamina = Math.Min(MaxStamina, Stamina + amount);
            }
        }

        protected bool IsCrit() => _random.NextDouble() * 100.0 < GetTotalCritChance();

        protected int ApplyCrit(int baseDamage) => IsCrit() ? (int)Math.Round(baseDamage * GetTotalCrit()) : baseDamage;
    }
}