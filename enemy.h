#pragma once
#include <windows.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include <deque>
using namespace sf;

class eBullet {
public:
	eBullet(int x_, int y_, int direction_);
	sf::Vector2f getCoord() { return sf::Vector2f{ x, y }; }
	bool getExist() { return exist; }
	float x{ 0 }, y{ 0 };  //射出点坐标
	int direction{ 0 };  // 0上，1下，2左，3右
	int shot_speed{ 5 };
	int range{ 1000 };
	int pass{ 0 };
	int damage{ 10 };
	bool exist = true;
        void setExist(bool x) { exist = x; }
};

class Enemy {
public:
	Enemy(int id_);              //接受种类来初始化
	void beHitted(int damage_);  //受到攻击时
	void enemyMove();            //随机攻击和移动
	std::string getTexture() { return enemy; };
	sf::Vector2f getCoord() { return sf::Vector2f{ coord_x, coord_y }; }
	std::deque<eBullet> ebullets{};
	bool getShoot();
	bool getExist();
	int getDamge();
private:
	int hp{ 100 };       //生命
	int id{ 1 };         //种类
	int speed{ 3 };      //速度
	int damage{ 10 };
	bool exist{ true };  //存在
	float coord_x{ 0 };  // x,y坐标
	float coord_y{ 0 };
	std::string picturepath[2][2];  //贴图
	int currentdirect{ 0 };           //方向：0上，1下，2左，3右
	bool shoot{ false };
	std::string enemy;
};

eBullet::eBullet(int x_, int y_, int direction_) {
	x = x_;
	y = y_;
	direction = direction_;
}
Enemy::Enemy(int id_) : id{ id_ } {
	switch (id_) {
	case 1:
		shoot = false;
		break;
	case 2:
		shoot = true;
		break;
	}  //根据种类初始化数据

	std::mt19937 rng;                  // 1300,900
	rng.seed(std::random_device()());  //初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100);  //
	std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);   //

	coord_x = x_(rng);
	coord_y = y_(rng);  //初始化随机位置
};

void Enemy::beHitted(int damage_) {
	hp -= damage_;
	if (hp <= 0) {
		exist = false;
	}
}

int Enemy::getDamge() { return damage; }
bool Enemy::getShoot() { return shoot; }
bool Enemy::getExist() { return exist; }

void Enemy::enemyMove() {
	sf::Mutex myMutex;
	Texture ebullet;
	Sprite bullet;
	ebullet.loadFromFile("images/bullet1.png");
	bullet.setTexture(ebullet);

	int futuredirect{ 0 };
	int allowdistance{ 0 };
	int distance{ 0 };
	int wantattack{ 0 };

	int lor = 0;      // 0 left 1right
	int picture = 0;  // 1or2
	int add1tion{ 0 };

	std::mt19937 rng;                  // 1300,900
	rng.seed(std::random_device()());  //初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
	std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
	std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);

	while (1) {
		futuredirect = randomdirect(rng);
		currentdirect = futuredirect;  //获取方向

		switch (currentdirect) {
		case 0:
			allowdistance = coord_y;
			break;
		case 1:
			allowdistance = 650 - coord_y;
			break;
		case 2:
			allowdistance = coord_x;
			lor = 0;
			break;
		case 3:
			allowdistance = 1100 - coord_x;
			lor = 1;
			break;
		}  //上下左右的最大距离
		if (allowdistance == 0) {
			continue;
		}
		distance = distance_(rng) % allowdistance;
		for (int i = 0; i < distance / speed; i++) {
			wantattack = ifattack(rng);
			int it = static_cast<int>(ebullets.size());
			if (wantattack == 10 && this->shoot == true) {
				if (it == 0 || ebullets[it - 1].pass >= 18) {
					if (lor == 0) {
						ebullets.push_back(eBullet(this->coord_x + 40, this->coord_y + 88,
							randomdirect(rng)));
					}
					else {
						ebullets.push_back(eBullet(this->coord_x + 170, this->coord_y + 88,
							randomdirect(rng)));
					}
				}
			}
			myMutex.lock();
			switch (currentdirect) {
			case 0:
				coord_y -= speed;
				break;
			case 1:
				coord_y += speed;
				break;
			case 2:
				coord_x -= speed;
				break;
			case 3:
				coord_x += speed;
				break;
			}

			for (int i = 0; i < ebullets.size(); i++) {
				if (ebullets[i].exist == true) {
					switch (ebullets[i].direction) {
					case 0:
						ebullets[i].y -= ebullets[i].shot_speed;
						ebullets[i].pass += ebullets[i].shot_speed;
						break;
					case 1:
						ebullets[i].y += ebullets[i].shot_speed;
						ebullets[i].pass += ebullets[i].shot_speed;
						break;
					case 2:
						ebullets[i].pass += ebullets[i].shot_speed;
						ebullets[i].x -= ebullets[i].shot_speed;
						break;
					case 3:
						ebullets[i].pass += ebullets[i].shot_speed;
						ebullets[i].x += ebullets[i].shot_speed;
						break;
					}
					myMutex.unlock();

					if (ebullets[i].pass > ebullets[i].range) {
						ebullets[i].exist = false;
					}
				}
			}
			myMutex.lock();
			switch (id) {
			case 1:
				switch (lor) {
				case 0:
					if (picture == 0)
						enemy = "images/小怪1级朝左1.png";
					else
						enemy = "images/小怪1级朝左2.png";
				case 1:
					if (picture == 0)
						enemy = "images/小怪1级朝右1.png";
					else
						enemy = "images/小怪1级朝右2.png";
				}
				break;
			case 2:
				switch (lor) {
				case 0:
					if (picture == 0)
						enemy = "images/小怪2级朝左1.png";
					else
						enemy = "images/小怪2级朝左2.png";
				case 1:
					if (picture == 0)
						enemy = "images/小怪2级朝右1.png";
					else
						enemy = "images/小怪2级朝右2.png";
				}
				break;
			}
			myMutex.unlock();


			add1tion++;
			if (add1tion == 5) {
				picture = (picture + 1) % 2;
				add1tion = 0;
			}
			Sleep(50);
		}
		//还应该有判断收到伤害的函数
	}
}

//考虑碰撞判断
