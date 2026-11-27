namespace RpgBattleSystem
{
    public interface IManaUser
    {
        int Mana { get; }
        int MaxMana { get; }
        bool SpendMana(int amount);
        void RestoreMana(int amount);
    }
}