#ifndef WEAPON_H
#define WEAPON_H

enum WeaponType
{
    WEAPON_NORMAL,
    WEAPON_LASER,     // Laser xuyên suốt
    WEAPON_MISSILE,   // Tên lửa tự tìm
    WEAPON_SPREAD     // Bắn tán
};

class Weapon
{
public:
    Weapon();
    ~Weapon();
    
    void upgrade();
    void reset();
    
    WeaponType getType() const { return type; }
    int getDamage() const { return damage; }
    int getFireRate() const { return fireRate; }
    int getLevel() const { return level; }
    
    void setType(WeaponType newType) { type = newType; }
    
private:
    WeaponType type;
    int damage;
    int fireRate;  // Delay giữa các lần bắn (ms)
    int level;     // Cấp độ vũ khí (1-5)
};

#endif // WEAPON_H
