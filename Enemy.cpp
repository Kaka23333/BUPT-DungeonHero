#include "Enemy.h"
#include <random>
#include <vector>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

extern RenderWindow window_;
extern Mutex myMutex;
extern std::vector<eBullet> ebullets;

eBullet::eBullet(int x_, int y_, int direction_) {
	x = x_;
	y = y_;
	direction = direction_;
}

Enemy::Enemy(int id_) {
	switch (id_) {
	case 1:
		picturepath[0][0] = "images/С��1������1.png";
		picturepath[0][1] = "images/С��1������2.png";
		picturepath[1][0] = "images/С��1������1.png";
		picturepath[1][1] = "images/С��1������2.png";
		shoot = false;
		break;
	case 2:
		picturepath[0][0] = "images/С��2������1.png";
		picturepath[0][1] = "images/С��2������2.png";
		picturepath[1][0] = "images/С��2������1.png";
		picturepath[1][1] = "images/С��2������2.png";
		shoot = true;
		break;
	}//���������ʼ������

	std::mt19937 rng;//1300,900
	rng.seed(std::random_device()());//��ʼ���������
	std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100);//
	std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);//

	crood_x = x_(rng);
	crood_y = y_(rng);//��ʼ�����λ��

};

void Enemy::beHitted(int damage_) {
	hp -= damage_;
	if (hp <= 0) {
		exist = false;
	}
}

void Enemy::enemyMove() {
	Texture t1;
	Sprite enemy;

	t1.loadFromFile(picturepath[0][0]);
	enemy.setTexture(t1);
	//������

	Texture ebullet;
	Sprite bullet;
	ebullet.loadFromFile("images/bullet1.png");
	bullet.setTexture(ebullet);

	int futuredirect{ 0 };
	int allowdistance{ 0 };
	int distance{ 0 };
	int wantattack{ 0 };

	int lor = 0;//0 left 1right
	int picture = 0;//1or2
	int add1tion{ 0 };

	std::mt19937 rng;//1300,900
	rng.seed(std::random_device()());//��ʼ���������
	std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
	std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
	std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);

	while (window_.isOpen()) {
		futuredirect = randomdirect(rng);
		currentdirect = futuredirect;//��ȡ����

		switch (currentdirect) {
		case 0:
			allowdistance = crood_y;
			break;
		case 1:
			allowdistance = 650 - crood_y;
			break;
		case 2:
			allowdistance = crood_x;
			lor = 0;
			break;
		case 3:
			allowdistance = 1100 - crood_x;
			lor = 1;
			break;
		}  //�������ҵ�������
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
						ebullets.push_back(eBullet(this->crood_x + 40, this->crood_y + 88, randomdirect(rng)));
					}
					else {
						ebullets.push_back(eBullet(this->crood_x + 170, this->crood_y + 88, randomdirect(rng)));
					}

				}

			}
			switch (currentdirect) {
			case 0:
				crood_y -= speed;
				break;
			case 1:
				crood_y += speed;
				break;
			case 2:
				crood_x -= speed;
				break;
			case 3:
				crood_x += speed;
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
					bullet.setPosition(ebullets[i].x, ebullets[i].y);

					myMutex.lock();
					window_.setActive(true);

					window_.draw(bullet);

					window_.setActive(false);
					myMutex.unlock();

					if (ebullets[i].pass > ebullets[i].range) {
						ebullets[i].exist = false;
					}
				}
			}

			t1.loadFromFile(picturepath[lor][picture]);
			enemy.setTexture(t1);
			enemy.setScale(0.5, 0.5);
			enemy.setPosition(crood_x, crood_y);

			// ������
			myMutex.lock();
			window_.setActive(true);

			window_.draw(enemy);

			//ˢ��λ��
			window_.display();
			//Sleep(1);
		    //window_.clear();

			// ������
			window_.setActive(false);
			myMutex.unlock();

			add1tion++;
			if (add1tion == 5) {
				picture = (picture + 1) % 2;
				add1tion = 0;
			}

		}
		//��Ӧ�����ж��յ��˺��ĺ���
	}
}


//������ײ�ж�