#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <deque>
#include "Bullet.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Hero.h"

RenderWindow window_;
Mutex myMutex;
std::deque<Bullet> bullets{};  //�ӵ�
std::vector<eBullet> ebullets{};

// ��ײ��⺯��
bool checkCollision(Sprite s1, Sprite s2) {
	return s1.getLocalBounds().intersects(s2.getLocalBounds());
}

//Ӣ��
const int size = 16;
const int windowWidth = 60;
const int windowHeight = 40;

Hero Master(1, 1);

void renderBorder(sf::RenderWindow& window) {  //��ײ�߿�

}

void update() {//�ƶ�
	//��ʦ�ƶ�
	if (2 == Master.HreoDirection) Master.x -= Master.HeroSpeed;
	if (1 == Master.HreoDirection) Master.y += Master.HeroSpeed;
	if (3 == Master.HreoDirection) Master.x += Master.HeroSpeed;
	if (0 == Master.HreoDirection) Master.y -= Master.HeroSpeed;
	//if (Master.x >= windowWidth - 1)Master.isLive = false;
	//if (Master.x < 1) Master.isLive = false;
	//if (Master.y >= windowHeight - 1) Master.isLive = false;
	//if (Master.y < 1) Master.isLive = false;

	//�ӵ����
	for (int i = 0; Master.wand.bullets.size() > i; i++) {
		Master.wand.bullets[i].coord_x += Master.wand.bullets[i].direction.x;
		Master.wand.bullets[i].coord_y += Master.wand.bullets[i].direction.y;
	}
	//switch (Master.wand.bullets[i].direction) {//�ӵ���������仯
	//case 0:  Master.wand.bullets[i].coord_y -= Master.wand.shot_speed; break;
	//	case 1:  Master.wand.bullets[i].coord_y += Master.wand.shot_speed; break;
	//	case 2:  Master.wand.bullets[i].coord_x -= Master.wand.shot_speed; break;
	//	case 3:  Master.wand.bullets[i].coord_x += Master.wand.shot_speed; break;
	//
	//	}
	//}
}

void renderHero(sf::RenderWindow& window) {//��ͼ����
	if (2 == Master.HreoDirection || 0 == Master.HreoDirection)  // 0��1��2��3��{
	{
		if (++Master.RunType % 2)
			Master.image.loadFromFile("images/mleft1.png");
		else
			Master.image.loadFromFile("images/mleft2.png");
	}
	else {
		if (++Master.RunType % 2)
			Master.image.loadFromFile("images/mright1.png");
		else
			Master.image.loadFromFile("images/mright2.png");  // sf::IntRect(10, 10, 32, 32));
	}
	sf::Sprite s(Master.image);//TempSprite
	s.setScale(0.25, 0.25);
	s.setPosition(Master.x, Master.y);

	myMutex.lock();
	window_.setActive(true);

	window.draw(s);

	window_.setActive(false);
	myMutex.unlock();

	//�ӵ�������
	Master.wand.check();//���������
	for (int i = 0; Master.wand.bullets.size() > i; i++) {
		//�ӵ����˺��ı��ͼ
		sf::Sprite s2(Master.wand.image);//ps:���ܸ���

		//s2.setTexture(t1);// bullet;
		s2.setPosition(static_cast<float>(Master.wand.bullets[i].coord_x),
			static_cast<float> (Master.wand.bullets[i].coord_y));

		myMutex.lock();
		window_.setActive(true);

		window.draw(s2);

		window_.setActive(false);
		myMutex.unlock();
	}
}

void render(sf::RenderWindow& window) {//
	renderBorder(window);  // ͼ��߽�
	renderHero(window);    // ͼ���ƶ�
}

// �˵�����
int menu() {
	// �����������
	Texture mainMenu_, start_, setting_, help_, exit_;
	// ��ʼ������
	mainMenu_.loadFromFile("images/mainMenu.png");
	start_.loadFromFile("images/start.png");
	setting_.loadFromFile("images/setting.png");
	help_.loadFromFile("images/help.png");
	exit_.loadFromFile("images/exit.png");
	// �����������
	Sprite mainMenu(mainMenu_), start(start_), setting(setting_),
		help(help_), exit(exit_);
	// ��ӡ�˵�����
	window_.draw(mainMenu);

	Event mouse;

	while (window_.isOpen()) {
		if (window_.pollEvent(mouse))  // ����Ƿ����¼�����
		{
			if (mouse.type == Event::MouseMoved)  // ���������ť
				// ��ʼ
				if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
					mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432)
					window_.draw(start);
				// ����
				else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
					mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600)
					window_.draw(setting);
				// ����
				else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
					mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768)
					window_.draw(help);
				// �˳�
				else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
					mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
					window_.draw(exit);
				}
				// û��������ť
				else
					window_.draw(mainMenu);
			if (mouse.type == Event::MouseButtonPressed)  // �������ť
				// ��ʼ
				if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
					mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
					mouse.mouseButton.button == Mouse::Button::Left)
					return 1;
				// ����
				else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
					mouse.mouseButton.y > 504 && mouse.mouseButton.y < 600 &&
					mouse.mouseButton.button == Mouse::Button::Left)
					return 2;
				// ����
				else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
					mouse.mouseButton.y > 672 && mouse.mouseButton.y < 768 &&
					mouse.mouseButton.button == Mouse::Button::Left)
					return 3;
				// �˳�
				else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 8167 &&
					mouse.mouseButton.y > 840 && mouse.mouseButton.y < 912 &&
					mouse.mouseButton.button == Mouse::Button::Left)
					return 0;
			// ��ʾ������ͼƬ
			window_.display();
		}
	}
}

// ���亯��
void room() {
	// ��ʼ���߽��
	Texture block_;
	block_.loadFromFile("images/block.png");
	// ��ʼ������
	Sprite block(block_);
	block.setScale(0.08f, 0.08f);

	while (window_.isOpen())
	{
		sf::Event event;
		while (window_.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window_.close();
		}

		window_.clear();
		for (int i = 0; i < 20; i++) {
			if (0 == i || 19 == i)
				for (int j = 3; j < 30; j++) {
					block.setPosition(j * 48, i * 48);
					window_.draw(block);
				}
			else {
				block.setPosition(144, i * 48);
				window_.draw(block);
				block.setPosition(1392, i * 48);
				window_.draw(block);
			}
		}

		window_.display();
	}
}

int main(int argc, char const* argv[])
{
	window_.create(VideoMode(1440, 960), "the game");

	Enemy e1{ 1 }, e2{ 2 }, e3{ 1 };

	window_.setActive(false);

	Thread enemyThread1{ &Enemy::enemyMove, &e1 };
	Thread enemyThread2{ &Enemy::enemyMove, &e2 };
	Thread enemyThread3{ &Enemy::enemyMove, &e3 };

	enemyThread1.launch();
	enemyThread2.launch();
	enemyThread3.launch();

	sf::Clock clock;
	float dtTime = 0;
	float delay = 0.01;//tmd
	//�Զ���֡��
	Master.launch();
	sf::Event event;
	while (window_.isOpen()) {//A!S!d!w!


		dtTime += clock.restart().asSeconds();

		if (dtTime > Master.wand.bullet_delay) Master.fire = true;
		//�������ӵ�
		typedef sf::Keyboard sk;  //��Ҫsf::Keyboard sk ��д
		while (window_.pollEvent(event)) {  //�¼���ѯ
			//std::cout << Master.HreoDirection;
			if (event.type == sf::Event::Closed)
				window_.close();
			if (sf::Event::KeyPressed) {
				switch (event.key.code) {//��0��1��2���� 02 13
				case sk::Left: Master.HreoDirection = 2; Master.dlx = 0; break;//�������
				case sk::Down: Master.HreoDirection = 1; Master.dlx = DLX; break;
				case sk::Right: Master.HreoDirection = 3; Master.dlx = DLX; break;
				case sk::Up: Master.HreoDirection = 0; Master.dlx = 0; break;
				}
			}
			if (Master.fire) {
				if (event.type == sf::Event::MouseButtonPressed) {
					sf::Vector2f temp_xy{ sf::Mouse::getPosition(window_) };
					float temp =
						pow(Master.wand.shot_speed * Master.wand.shot_speed /
						(pow(temp_xy.x - Master.x, 2) + pow(temp_xy.y - Master.y, 2)),
							0.5);
					Master.wand.bullets.push_back(Bullet{ Master.x + Master.dlx ,Master.y,
											 sf::Vector2f{(temp_xy.x - Master.x) * temp,
														  (temp_xy.y - Master.y) * temp},
											 Master.x + Master.dlx,Master.y });
					Master.fire = false;
				}

			}
		}
		if (dtTime > delay) {  //��Ҫ��ʱ����ˢ�º���
			dtTime = 0;
			update();  //�߼��ƶ�
		}

		myMutex.lock();
		window_.setActive(true);

		sleep(milliseconds(1));
		window_.clear(sf::Color::Black);
		render(window_);  //ͼ���ƶ�
		window_.display();

		window_.setActive(false);
		myMutex.unlock();
	}
	return 0;
}


// ������̲߳��Դ���
/*
	Enemy e1{ 1 }, e2{ 2 }, e3{ 1 };

	window_.setActive(false);

	Thread enemyThread1{ &Enemy::enemyMove, &e1 };
	Thread enemyThread2{ &Enemy::enemyMove, &e2 };
	Thread enemyThread3{ &Enemy::enemyMove, &e3 };

	enemyThread1.launch();
	enemyThread2.launch();
	enemyThread3.launch();
*/