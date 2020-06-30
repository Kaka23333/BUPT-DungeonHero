#include <deque>
#include "Weapon.h"
#include "Bullet.h"

extern std::deque<Bullet> bullets;
extern sf::RenderWindow window_;
extern sf::Mutex myMutex;

Weapon::Weapon(int x_, int y_, int damage_) :x{ x_ }, y{ y_ }, damage{ damage_ }{
	switch (damage) {
	case 1: image.loadFromFile("images/bullet1.png");
		bullet.setTexture(image); break;
	case 2: image.loadFromFile("images/bullet2.png");
		bullet.setTexture(image); break;
	case 3: image.loadFromFile("images/bullet3.png");
		bullet.setTexture(image); break;
	}
}

void Weapon::SetXY(int x_, int y_) {
	x = x_;
	y = y_;
}

//检测子弹的状况
void Weapon::check() {
	for (int i = bullets.size() - 1; i > -1 && (pow(bullets[i].x - bullets[i].coord_x, 2) +
		pow(bullets[i].y - bullets[i].coord_y, 2) >
		range* range); i--) {
		bullets.pop_front();  //超出射程
		  // else if (bullets[i].coord_x) 撞到地形
		  // else if (bullets[i].coord_x) 撞到怪
	}
}