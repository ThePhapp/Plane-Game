#include "Weapon.h"

Weapon::Weapon()
    : type(WEAPON_NORMAL), damage(10), fireRate(300), level(1)
{
}

Weapon::~Weapon()
{
}

void Weapon::upgrade()
{
    if (level < 5)
    {
        level++;
        damage += 5;
        
        // Giảm fire rate khi nâng cấp (bắn nhanh hơn)
        if (fireRate > 100)
            fireRate -= 30;
    }
}

void Weapon::reset()
{
    type = WEAPON_NORMAL;
    damage = 10;
    fireRate = 300;
    level = 1;
}
