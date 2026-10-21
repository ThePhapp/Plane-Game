# 🎮 War's Plane: Sky Combat - New Features Guide

## ✨ Tính Năng Mới Đã Thêm

### 📦 **Phase 1: Core Improvements**

#### 1. ✅ **Power-ups System** (Hệ Thống Power-up)
Các power-up xuất hiện ngẫu nhiên khi tiêu diệt enemies:

- **🛡️ Shield** - Khiên bảo vệ (8 giây)
  - Miễn nhiễm damage từ enemies và boss bullets
  
- **⚡ Rapid Fire** - Bắn nhanh (10 giây)
  - Giảm fire rate xuống 100ms
  
- **🔫 Double Shot** - Bắn đôi (12 giây)
  - Bắn 2 viên đạn cùng lúc
  
- **❤️ Health** - Hồi máu
  - Khôi phục 1 máu (tối đa 3)
  
- **💣 Bomb** - Bom
  - Phá hủy tất cả enemies trên màn hình

**Cách sử dụng:** Thu thập bằng cách chạm vào power-up

---

#### 2. ✅ **Weapon System** (Hệ Thống Vũ Khí)
- Weapon có thể upgrade qua các level
- Mỗi level tăng damage và fire rate
- Hỗ trợ 4 loại vũ khí:
  - `WEAPON_NORMAL` - Súng thường
  - `WEAPON_LASER` - Đạn dài, thân hẹp
  - `WEAPON_MISSILE` - Đạn lớn
  - `WEAPON_SPREAD` - Bắn 3 hướng
- Giữ **SPACE** để bắn liên tục theo fire-rate.
- Nhấn **1–4** để đổi loại vũ khí.
- Nhấn **U** để dùng 500 điểm nâng cấp vũ khí, tối đa level 5.

---

#### 3. ✅ **Save/Load System** (Hệ Thống Lưu Game)
Game tự động lưu tiến độ vào `save/savegame.dat`:
- High score
- Coins (điểm hiện tại)
- Level đã chơi
- Weapon level
- Achievements đã mở khóa

**File lưu:**
- `save/savegame.dat` - Tiến độ game
- `save/leaderboard.dat` - Bảng xếp hạng

---

### 🎯 **Phase 2: Content**

#### 4. ✅ **More Enemy Types** (Nhiều Loại Enemies)

##### 🔹 **ENEMY_BASIC** (Thường)
- Máu: 10 HP
- Tốc độ: 4
- Điểm: 100
- Di chuyển thẳng sang trái

##### ⚡ **ENEMY_FAST** (Nhanh)
- Máu: 5 HP
- Tốc độ: 8
- Điểm: 150
- Nhỏ hơn, di chuyển nhanh

##### 🛡️ **ENEMY_TANK** (Tank)
- Máu: 30 HP
- Tốc độ: 2
- Điểm: 300
- Lớn hơn, có thanh máu hiển thị

##### 💥 **ENEMY_KAMIKAZE** (Tự sát)
- Máu: 8 HP
- Tốc độ: 6
- Điểm: 200
- Lao thẳng vào player

**Enemy spawn dựa trên level:**
- Level 1: 80% Basic, 20% Fast
- Level 2: 50% Basic, 30% Fast, 20% Tank
- Level 3: 30% Basic, 30% Fast, 25% Tank, 15% Kamikaze

---

#### 5. ✅ **Multiple Bosses** (Nhiều Loại Boss)

##### 🔵 **BOSS_NORMAL** (Level 1 - Easy)
- Máu: 80 HP
- Tốc độ: 3
- Pattern: Bắn 1 viên đạn
- Phase 2 (50% HP): Bắn nhanh hơn

##### ⚡ **BOSS_FAST** (Level 2 - Medium)
- Máu: 120 HP
- Tốc độ: 5
- Pattern: Bắn 1 viên
- Phase 2: Bắn 3 viên spread

##### 👑 **BOSS_ULTIMATE** (Level 3 - Hard)
- Máu: 180 HP
- Tốc độ: 8
- Pattern: Bắn 3 viên spread
- Phase 2: Bắn 5 viên spread

**Boss Features:**
- Thanh máu đổi màu theo HP (Xanh → Vàng → Đỏ)
- 2 phases: Phase 2 kích hoạt khi HP ≤ 50%
- Mỗi boss có bullet pattern riêng
- Boss xuất hiện khi đạt 1000 điểm

---

#### 6. ✅ **Achievement System** (Hệ Thống Thành Tựu)

##### 🏆 **Danh Sách Achievements:**

1. **First Blood** - Tiêu diệt enemy đầu tiên
2. **Boss Slayer** - Đánh bại boss
3. **Untouchable** - Hoàn thành level không bị hit
4. **Marksman** - 100% accuracy trong 1 level
5. **Collector** - Thu thập 10 power-ups
6. **Survivor** - Sống sót 5 phút
7. **Combo Master** - Đạt combo x10
8. **Destroyer** - Tiêu diệt 100 enemies
9. **Speed Runner** - Hoàn thành level dưới 2 phút
10. **Master** - Mở khóa tất cả achievements

**Hiển thị:**
- Notification popup khi unlock achievement
- Hiển thị trong 3 giây ở góc trên bên trái

---

#### 7. ✅ **Leaderboard System** (Bảng Xếp Hạng)

- Lưu top 10 điểm cao nhất
- Thông tin: Tên, Điểm, Level, Ngày chơi
- Tự động sắp xếp theo điểm cao nhất
- Lưu vào `save/leaderboard.dat`

---

## 🎮 **Cách Chơi Với Tính Năng Mới**

### **Khởi động game:**
1. Chọn "Play" từ main menu
2. Chọn độ khó (Easy/Medium/Hard)
3. Mỗi độ khó có boss type và enemy spawn khác nhau

### **Trong game:**
- Thu thập power-ups để tăng sức mạnh
- Tiêu diệt enemies để earn points và unlock achievements
- Shield bảo vệ bạn khỏi damage
- Double Shot và Rapid Fire giúp tiêu diệt enemies nhanh hơn

### **Chiến thuật:**
- Ưu tiên tiêu diệt Tank enemies (300 điểm)
- Tránh Kamikaze enemies (lao vào player)
- Thu thập Shield power-up trước khi đối đầu boss
- Sử dụng Bomb khi bị bao vây

---

## 📊 **Game Stats Tracking**

Game hiện theo dõi:
- `enemiesKilled` - Số enemies đã tiêu diệt
- `powerUpsCollected` - Số power-ups đã thu thập
- `shotsFired` - Số viên đạn đã bắn
- `shotsHit` - Số viên đạn trúng mục tiêu
- `levelStartTime` - Thời gian bắt đầu level (cho achievement Survivor & Speed Runner)

---

## 🛠️ **Technical Details**

### **New Files Created:**
```
src/
├── PowerUpObject/
│   ├── PowerUp.h
│   └── PowerUp.cpp
├── WeaponObject/
│   ├── Weapon.h
│   └── Weapon.cpp
├── SaveSystem/
│   ├── SaveGame.h
│   └── SaveGame.cpp
├── AchievementSystem/
│   ├── Achievement.h
│   └── Achievement.cpp
└── LeaderboardSystem/
    ├── Leaderboard.h
    └── Leaderboard.cpp
```

### **Modified Files:**
- `Game.h` - Added new systems and functions
- `Game.cpp` - Integrated all new features
- `BossObject.h/cpp` - Added boss types and patterns
- `ObstacleObject.h/cpp` - Completely rewritten with enemy types
- `Bullet.cpp` - Updated with power-up effects
- `Control.cpp` - Added save/load on game over
- `Makefile` - Updated with new source files

---

## 🖼️ **Required Images** (Cần thêm vào thư mục `image/`)

### Power-ups:
- `powerup_shield.png`
- `powerup_rapid.png`
- `powerup_double.png`
- `powerup_health.png`
- `powerup_bomb.png`

### Enemies:
- `obstacle.png` (Basic - đã có)
- `enemy_fast.png`
- `enemy_tank.png`
- `enemy_kamikaze.png`

**Lưu ý:** Nếu không có ảnh, game sẽ vẫn chạy nhưng power-ups và enemies mới sẽ không hiển thị texture.

---

## 🚀 **Build & Run**

```bash
# Build game
mingw32-make

# Clean build
mingw32-make clean

# Run game
.\PlaneGame.exe
```

---

## 🎯 **Roadmap Tiếp Theo**

### **Có thể thêm:**
1. **Shop System** - Dùng coins để mua upgrades
2. **Combo System** - Multiplier khi tiêu diệt liên tiếp
3. **Particle Effects** - Vệt khói, tia lửa, explosion particles
4. **Sound Effects** - Thêm âm thanh cho power-ups, achievements
5. **Config Menu** - Volume, key binding, graphics settings
6. **Story Mode** - Multiple levels with different bosses
7. **Local Co-op** - 2 players cùng chơi

---

## 📝 **Notes**

- Game tự động tạo thư mục `save/` khi lần đầu chạy
- Save files sử dụng binary format
- Leaderboard giới hạn 10 entries
- Achievements persist qua các lần chơi
- Power-up effects stack (có thể có nhiều power-up cùng lúc)

---

**Developed with ❤️ using C++ and SDL2**
