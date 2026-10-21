using System;

namespace RpgBattleSystem
{
    public class Weapon
    {
        public string Name { get; }
        public DamageType DamageType { get; }
        public int MaxDurability { get; }
        public double BonusCrit { get; }
        public double BonusCritChance { get; }

        private int _damage;
        public int Damage
        {
            get => _damage;
            private set => _damage = value > 0 ? value : 1;
        }

        private int _durability;
        public int Durability
        {
            get => _durability;
            private set => _durability = (value <= 100 && value >= 0) ? value : 50;
        }

        public Weapon()
            : this("Кулак", 1, DamageType.Physical, 100, 0.0, 0.0)
        {
            Console.WriteLine($"Создано оружие по умолчанию: {Name}");
        }

        public Weapon(string name, int damage, DamageType damageType, int maxDurability, double bonusCrit, double bonusCritChance)
        {
            Name = name;
            DamageType = damageType;
            if (damage <= 0)
            {
                Console.WriteLine("Ошибка! Урон оружия не может быть <=0. Установлено значение 1.");
            }
            Damage = damage;
            if (maxDurability <= 0 || maxDurability > 100)
            {
                Console.WriteLine("Ошибка! Максимальная прочность оружия должна быть >0 и <=100. Установлено значение 100.");
                maxDurability = 100;
            }
            MaxDurability = maxDurability;
            Durability = maxDurability;
            if (bonusCrit < 0.0)
            {
                Console.WriteLine("Ошибка! Бонус к крит урону не может быть <0. Установлено значение 0.");
                bonusCrit = 0.0;
            }
            BonusCrit = bonusCrit;
            if (bonusCritChance < 0.0)
            {
                Console.WriteLine("Ошибка! Бонус к крит шансу не может быть <0. Установлено значение 0.");
                bonusCritChance = 0.0;
            }
            BonusCritChance = bonusCritChance;
            Console.WriteLine($"Создано оружие: {Name}");
        }
        
        public int CalculateBaseDamage(int strength)
        {
            if (IsBroken())
            {
                return 1;
            }
            return Damage + strength;
        }

        public bool Use()
        {
            if (Durability > 0)
            {
                Durability--;
                return true;
            }
            else
            {
                Console.WriteLine("Ваше оружие сломано!");
                return false;
            }
        }

        public bool IsBroken()
        {
            return Durability <= 0;
        }

        public override string ToString()
        {
            return $"Оружие:             {Name}\n" +
                   $"Урон:               {Damage}\n" +
                   $"Тип урона:          {DamageTypeToString()}\n" +
                   $"Прочность:          {Durability} из {MaxDurability}\n" +
                   $"Бонус к крит урону: {BonusCrit}\n" +
                   $"Бонус к крит шансу: {BonusCritChance}";
        }

        private string DamageTypeToString()
        {
            return DamageType switch
            {
                DamageType.Physical => "физический",
                DamageType.Magical => "магический",
                _ => "неизвестный"
            };
        }
    }
}