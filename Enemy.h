#pragma once
#include "hero.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <string>
#include <windows.h>
using namespace sf;
extern int nEnemy;
extern std::vector<Vector2f> barrier; // 存放障碍物
bool checkCollision(Sprite s1, Sprite s2) {
  return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

class eBullet {
public:
  eBullet(int x_, int y_, int direction_);
  sf::Vector2f getCoord() { return sf::Vector2f{x, y}; }
  bool getExist() { return exist; }
  float x{0}, y{0}; //射出点坐标
  int direction{0}; // 0上，1下，2左，3右
  int shot_speed{5};
  int range{1000};
  int pass{0};
  int damage{10};
  bool exist = true;
  void setExist(bool x) { exist = x; }
};

class Enemy {
public:
  Enemy(int id_);             //接受种类来初始化
  void beHitted(int damage_); //受到攻击时
  void launch();
  void enemyStop() { stop = true; }
  void enemyMove(); //随机攻击和移动
  void setSpeed(int speed_) { speed = speed_; }
  std::string getTexture() { return enemy; };
  sf::Vector2f getCoord() { return sf::Vector2f{coord_x, coord_y}; }
  bool getShoot();
  bool getExist();
  int getDamge();

  std::deque<eBullet> ebullets{};
  bool stop{false};
  bool exist{true}; //存在
  bool shoot{false};
  int hp{100};  //生命
  int id{1};    //种类
  int speed{3}; //速度
  int damage{10};
  int currentdirect{0}; //方向：0上，1下，2左，3右
  float coord_x{0};     // x,y坐标
  float coord_y{0};
  std::string enemy = "images/小怪2级朝左1.png";
  Texture t1, t2;
  Sprite block, temp;
};

void Enemy::launch() {
  stop = false;
  exist = true;
  hp = 100;
  speed = 3;
  std::mt19937 rng;                 // 1300,900
  rng.seed(std::random_device()()); //初始化随机种子
  std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100); //
  std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);  //
  coord_x = x_(rng);
  coord_y = y_(rng); //初始化随机位置
}

eBullet::eBullet(int x_, int y_, int direction_) {
  x = x_;
  y = y_;
  direction = direction_;
}
Enemy::Enemy(int id_) : id{id_} {
  t1.loadFromFile("images/block.png");
  block.setTexture(t1);

  switch (id_) {
  case 1:
    shoot = false;
    break;
  case 2:
    shoot = true;
    break;
  } //根据种类初始化数据

  std::mt19937 rng;                 // 1300,900
  rng.seed(std::random_device()()); //初始化随机种子
  std::uniform_int_distribution<std::mt19937::result_type> x_(0, 1100); //
  std::uniform_int_distribution<std::mt19937::result_type> y_(0, 650);  //

  coord_x = x_(rng);
  coord_y = y_(rng); //初始化随机位置
};

void Enemy::beHitted(int damage_) {
  hp -= damage_;
  if (hp <= 0) {
    exist = false;
    --nEnemy;
    if (nEnemy == 0) {
      SoundBuffer bf;
      bf.loadFromFile("Sound effect/door heavy open.wav");
      Sound dooropen(bf);
      dooropen.play();
    }
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

  int futuredirect{0};
  int allowdistance{0};
  int distance{0};
  int wantattack{0};

  int lor = 0;     // 0 left 1right
  int picture = 0; // 1or2
  int add1tion{0};

  std::mt19937 rng;                 // 1300,900
  rng.seed(std::random_device()()); //初始化随机种子
  std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
  std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
  std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);

  while (exist) {
    futuredirect = randomdirect(rng);
    currentdirect = futuredirect; //获取方向

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
    } //上下左右的最大距离
    if (allowdistance == 0) {
      continue;
    }
    if (stop == true) {
      switch (currentdirect) {
      case 0:
        currentdirect == 1;
        break;
      case 1:
        currentdirect == 0;
        break;
      case 2:
        currentdirect == 3;
        break;
      case 3:
        currentdirect == 2;
        break;
      }
      stop == false;
    }
    distance = distance_(rng) % allowdistance;
    for (int i = 0; i < distance / speed; i++) {
      if (stop == true) {
        break;
      }
      wantattack = ifattack(rng);
      int it = static_cast<int>(ebullets.size());
      if (wantattack == 10 && this->shoot == true) {
        if (it == 0 || ebullets[it - 1].pass >= 18 || !ebullets[it - 1].exist) {
          if (lor == 0) {
            ebullets.push_back(eBullet(this->coord_x + 40, this->coord_y + 88,
                                       randomdirect(rng)));
          } else {
            ebullets.push_back(eBullet(this->coord_x + 170, this->coord_y + 88,
                                       randomdirect(rng)));
          }
        }
      }
      float tempx = coord_x, tempy = coord_y;

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
      myMutex.unlock();

      // for (int j = 0; j < barrier.size(); j++) {
      // block.setPosition(barrier[j]);
      // t2.loadFromFile(enemy);
      // temp.setTexture(t2);
      // temp.setPosition(coord_x, coord_y);
      //若碰撞则恢复原位置
      // if (checkCollision(block, temp)) {
      // myMutex.lock();

      // coord_x = tempx;
      // coord_y = tempy;
      // myMutex.unlock();

      // break;
      //}
      //}

      for (int i = 0; i < ebullets.size(); i++) {
        if (ebullets[i].exist == true) {
          myMutex.lock();

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
          break;
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
          break;
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

class Boss : public Enemy {
public:
  void bossMove(Hero h_);
};

void Boss::bossMove(Hero h_) {

  sf::Mutex myMutex;
  Texture ebullet;
  Sprite bullet;
  ebullet.loadFromFile("images/bullet1.png");
  bullet.setTexture(ebullet);

  int distancex{0};
  int distancey{0};
  int directx{0};
  int directy{0};

  int allowdistance{0};
  int distance{0};
  int wantattack{0};

  int lor = 0;     // 0 left 1right
  int picture = 0; // 1or2
  int add1tion{0};

  std::mt19937 rng;                 // 1300,900
  rng.seed(std::random_device()()); //初始化随机种子
  std::uniform_int_distribution<std::mt19937::result_type> randomdirect(0, 3);
  std::uniform_int_distribution<std::mt19937::result_type> ifattack(0, 10);
  std::uniform_int_distribution<std::mt19937::result_type> distance_(0, 1960);

  while (1) {
    distancex = h_.x - coord_x;
    distancey = h_.y - coord_y;
    directx = distancex / sqrt(distancex * distancex + distancey * distancey);
    directy = distancey / sqrt(distancex * distancex + distancey * distancey);

    distance = sqrt(distancex * distancex + distancey * distancey) / 4;
    for (int i = 0; i < distance / speed; i++) {

      wantattack = ifattack(rng);
      int it = static_cast<int>(ebullets.size());
      if (wantattack == 10 && this->shoot == true) {
        if (it == 0 || ebullets[it - 1].pass >= 18) {
          ebullets.push_back(
              eBullet(this->coord_x + 40, this->coord_y + 88, 0));
          ebullets.push_back(
              eBullet(this->coord_x + 40, this->coord_y + 88, 1));
          ebullets.push_back(
              eBullet(this->coord_x + 40, this->coord_y + 88, 2));
          ebullets.push_back(
              eBullet(this->coord_x + 40, this->coord_y + 88, 3));
        }
      }
      myMutex.lock();
      coord_x += speed * directx;
      coord_y += speed * directy;

      for (int j = 0; j < ebullets.size(); j++) {
        if (ebullets[j].exist == true) {
          switch (ebullets[j].direction) {
          case 0:
            ebullets[j].y -= ebullets[j].shot_speed;
            ebullets[j].pass += ebullets[j].shot_speed;
            break;
          case 1:
            ebullets[j].y += ebullets[j].shot_speed;
            ebullets[j].pass += ebullets[j].shot_speed;
            break;
          case 2:
            ebullets[j].pass += ebullets[j].shot_speed;
            ebullets[j].x -= ebullets[j].shot_speed;
            break;
          case 3:
            ebullets[j].pass += ebullets[j].shot_speed;
            ebullets[j].x += ebullets[j].shot_speed;
            break;
          }
          myMutex.unlock();

          if (ebullets[j].pass > ebullets[j].range) {
            ebullets[j].exist = false;
          }
        }
      }
      myMutex.lock();
      if (directx >= 0) {
        if (picture == 0)
          enemy = "images/小怪1级朝左1.png";
        else
          enemy = "images/小怪1级朝左2.png";
      } else {
        if (picture == 1)
          enemy = "images/小怪1级朝右1.png";
        else
          enemy = "images/小怪1级朝右2.png";
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
