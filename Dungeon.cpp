#include "enemy.h"
#include "hero.h"
#include "shoot.h"
#include <iostream>
#include <string>
#include <vector>
#define HORI 320  //英雄宽度像素
#define HPIC 0.25 //英雄放缩比例
using namespace sf;
typedef sf::Keyboard sk; //重要sf::Keyboard sk 简写

//重要 : 方向统一 上0下1左2右三
//英雄偏移向量1

sf::RenderWindow window;
Mutex bulletMutex;
Mutex fireMutex;
class Room {
private:
  int type;           // 房间类型，共三种
  bool isOpen{false}; // 房间门是否打开
  Mutex roomMutex;
  Sprite scene;           // 场景贴图
  Texture block_, scene_; // 边界块

public:
  Sprite block, chest;
  std::vector<Vector2f> barrier; // 存放障碍物
  Room(int type_) : type{type_} {
    block_.loadFromFile("images/block.png");
    scene_.loadFromFile("images/back.png");
    block.setTexture(block_);
    scene.setTexture(scene_);
    if (0 == type) {
      for (int i = 0; i < 7; i++) {
        barrier.push_back(sf::Vector2f{325, 250});
        barrier.push_back(sf::Vector2f{410, 250});
        barrier.push_back(sf::Vector2f{325, 330});
        barrier.push_back(sf::Vector2f{950, 250});
        barrier.push_back(sf::Vector2f{1030, 250});
        barrier.push_back(sf::Vector2f{1030, 330});
        barrier.push_back(sf::Vector2f{325, 560});
        barrier.push_back(sf::Vector2f{325, 640});
        barrier.push_back(sf::Vector2f{410, 635});
        barrier.push_back(sf::Vector2f{1030, 560});
        barrier.push_back(sf::Vector2f{1030, 640});
        barrier.push_back(sf::Vector2f{950, 640});
      }
      // 普通房间
      // 障碍物位置存储到barrier
    } else if (1 == type) {
      // boss房间
      // 障碍物位置存储到barrier
    } else if (2 == type) {
      // 道具房间
      // 障碍物位置存储到barrier
    }
  }

  void print() {
    roomMutex.lock();
    // 房间边框
    window.draw(scene);
    // 障碍物
    for (int i = 0; i < barrier.size() && 3 != type; i++) {
      // block.setPosition(barrier[i].x, barrier[i].y);
      // window.draw(block);
    }
    if (3 == type) {
      // 宝箱房直接画出宝箱(多个)
    } else if (isOpen) {
      // 房间开门后在窗口上画出宝箱
    }

    roomMutex.unlock();
  }
};

// 菜单界面
int menu() {
  // 创建纹理对象
  Texture mainMenu_, start_, setting_, help_, exit_, basicHelp_, menuSetting_;
  // 初始化纹理
  mainMenu_.loadFromFile("images/mainMenu.png");
  start_.loadFromFile("images/start.png");
  setting_.loadFromFile("images/setting.png");
  help_.loadFromFile("images/help.png");
  exit_.loadFromFile("images/exit.png");
  basicHelp_.loadFromFile("images/basicHelp.png");
  menuSetting_.loadFromFile("images/menuSetting.png");
  // 创建精灵对象
  Sprite mainMenu(mainMenu_), start(start_), setting(setting_), help(help_),
      exit(exit_), basicHelp(basicHelp_), menuSetting(menuSetting_);
  // 打印菜单界面
  window.draw(mainMenu);

  Event mouse;

  while (window.isOpen()) {
    if (window.pollEvent(mouse)) // 检查是否有事件发生
    {
      if (mouse.type == Event::MouseMoved) // 鼠标碰到按钮
        // 开始
        if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
            mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432)
          window.draw(start);
        // 设置
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600)
          window.draw(setting);
        // 帮助
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768)
          window.draw(help);
        // 退出
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912)
          window.draw(exit);
        // 没有碰到按钮
        else
          window.draw(mainMenu);
      if (mouse.type == Event::MouseButtonPressed) // 鼠标点击按钮
        // 开始
        if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
            mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
            mouse.mouseButton.button == Mouse::Button::Left)
          return 1;
        // 设置
        else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                 mouse.mouseButton.y > 504 && mouse.mouseButton.y < 600 &&
                 mouse.mouseButton.button == Mouse::Button::Left) {
          window.draw(menuSetting);
          Event change;
          while (window.isOpen()) {
            if (window.pollEvent(change)) {
              if (mouse.type == Event::MouseMoved) // 鼠标碰到按钮
                // 音乐位置
                if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
                    mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432) {
                  // 等待添加
                }
                // 音效位置
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                         mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600) {
                  // 等待添加
                }
                // 音量--
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                         mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768) {
                  // 等待添加
                }
                // 音量++
                else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                         mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
                  // 等待添加
                }
              if (mouse.type == Event::MouseButtonPressed) // 鼠标点击按钮
                                                           // 音乐开启/关闭
                if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
                    mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
                    mouse.mouseButton.button == Mouse::Button::Left) {
                  // 等待添加
                }
                // 音效开启/关闭
                else if (mouse.mouseButton.x > 600 &&
                         mouse.mouseButton.x < 816 &&
                         mouse.mouseButton.y > 504 &&
                         mouse.mouseButton.y < 600 &&
                         mouse.mouseButton.button == Mouse::Button::Left) {
                  // 等待添加
                }
                // 音量--
                else if (mouse.mouseButton.x > 600 &&
                         mouse.mouseButton.x < 816 &&
                         mouse.mouseButton.y > 504 &&
                         mouse.mouseButton.y < 600 &&
                         mouse.mouseButton.button == Mouse::Button::Left) {
                  // 等待添加
                }
                // 音量++
                else if (mouse.mouseButton.x > 600 &&
                         mouse.mouseButton.x < 816 &&
                         mouse.mouseButton.y > 504 &&
                         mouse.mouseButton.y < 600 &&
                         mouse.mouseButton.button == Mouse::Button::Left) {
                  // 等待添加
                }
            }
            window.display();
          }
        }
        // 帮助
        else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                 mouse.mouseButton.y > 672 && mouse.mouseButton.y < 768 &&
                 mouse.mouseButton.button == Mouse::Button::Left) {
          window.draw(basicHelp);
          window.display();
          Sleep(2000);
        }
        // 退出
        else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 8167 &&
                 mouse.mouseButton.y > 840 && mouse.mouseButton.y < 912 &&
                 mouse.mouseButton.button == Mouse::Button::Left)
          std::exit(0);
      // 显示所画的图片
      window.display();
    }
  }
}

void avoidBug(std::string address) {
  if ("" == address)
    sleep(milliseconds(1));
}

// 碰撞检测函数
bool checkCollision(Sprite s1, Sprite s2) {
  return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}

int main() {
  window.create(sf::VideoMode(1440, 960), "Dumbledore's adventures");

  Room room{0};

  // 音乐初始化
  Music menuMusic, gameMusic, bossMusic;
  menuMusic.openFromFile("music/menu.wav");
  gameMusic.openFromFile("music/game.wav");
  bossMusic.openFromFile("music/Boss.wav");
  menuMusic.setLoop(true);
  gameMusic.setLoop(true);
  bossMusic.setLoop(true);

  // 播放菜单音乐
  menuMusic.play();

  menu();

  // 关闭菜单音乐，播放游戏音乐
  menuMusic.stop();
  gameMusic.play();

  // std::vector<Thread> enemyThread;
  std::vector<Enemy> enemy{1, 1, 2};
  std::vector<Sprite> enemySprite{};

  Hero h1{200, 200};
  Weapon b1;

  // 射击线程
  Thread shootThread2{&Weapon::shoot_mouse, &b1};
  shootThread2.launch();

  Thread enemyThread0{&Enemy::enemyMove, &enemy[0]};
  Thread enemyThread1{&Enemy::enemyMove, &enemy[1]};
  Thread enemyThread2{&Enemy::enemyMove, &enemy[2]};

  enemyThread0.launch();
  enemyThread1.launch();
  enemyThread2.launch();

  for (int i = 0; i < 3; i++)
    enemySprite.push_back(Sprite{});

  Texture t;
  Sprite s, s4, ebullet;

  sf::Clock clock;
  float dtTime = 0;   // deltatime 时间间隔
  float delay = 0.02; //
  //自定义帧率
  h1.launch();

  Event e;
  b1.setFire(false); //拉保险
  while (window.isOpen()) {
    dtTime += clock.restart().asSeconds();
    //循环核心 时间刷新
    if (dtTime > 0.01)
      b1.setFire(true); //冷却时间过,允许开炮
    room.print();

    while (window.pollEvent(e)) //这样会快
    {                           //事件轮询
      char skills = 'B';
      if (e.type == sf::Event::Closed)
        window.close();
      if (sk::isKeyPressed) {
        switch (e.key.code) { //上0下1左2右三
        case sk::Left:
          h1.dir = 2;
          break; //方向控制
        case sk::Down:
          h1.dir = 1;
          break;
        case sk::Right:
          h1.dir = 3;
          break;
        case sk::Up:
          h1.dir = 0;
          break;
        case sk::Q:
          skills = 'Q';
          break;
        case sk::W:
          skills = 'W';
          break;
        case sk::E:
          skills = 'E';
          break;
        case sk::R:
          skills = 'R';
          break;
        case sk::G:
          skills = 'G';
          break;
        }
        std::cout << skills;
      }

      // 主角部分

      // 射击并打印主角子弹
      if (b1.getFire()) {
        if (e.type == sf::Event::MouseButtonPressed) // 鼠标按住
        {                                            //上0下1左2右三
          sf::Vector2f mouse{sf::Mouse::getPosition(window)}; //装填弹药
          b1.setDirection(mouse, h1, (h1.dir % 2) * HPIC * HORI);
          bulletMutex.unlock();
          b1.setFire(false); //火炮冷却
        }
      }
    }

    //人物碰撞边框+打印
    h1.renderHero();
    t.loadFromFile(h1.getTexture());
    s4.setTexture(t);
    s4.setScale(HPIC, HPIC);
    s4.setPosition(h1.getCoord());
    if (dtTime > delay) { //重要的时间间隔刷新函数
      dtTime = 0;
      int temp[2]{h1.x, h1.y};
      h1.update(); //逻辑移动

      // for (int i = 0; i < room.barrier.size(); i++) {
      //检测与障碍物碰撞，没写完，少边框位置
      // room.block.setPosition(room.barrier[i]);
      if (h1.x < 50 || h1.x > 1350 || h1.y < 50 || h1.y > 700) {
        h1.x = temp[0];
        h1.y = temp[1];
      }
      //}
      s4.setPosition(h1.getCoord());
    }
    window.draw(s4);

    //判断英雄和怪物碰撞
    for (int i = 0; i < enemy.size(); i++) {
      if (enemy[i].getExist()) {
        if (checkCollision(enemySprite[i], s4)) {
          h1.beHitted(enemy[i].getDamge());
        }
      }
    }
    //打印子弹+边界+怪物击中检测
    t.loadFromFile(b1.getTexture());
    s.setTexture(t);
    for (int i = 0; b1.bullets.size() > i; i++) {

      //子弹存在
      if (b1.bullets[i].getExist()) {
        s.setPosition(b1.bullets[i].getCoord());

        //检测与边框碰撞
        if (b1.bullets[i].getCoord().x < 50 ||
            b1.bullets[i].getCoord().x > 1350 ||
            b1.bullets[i].getCoord().y < 50 ||
            b1.bullets[i].getCoord().y > 800) {
          b1.bullets[i].setExist(false);
          break;
        }
        //检测打击怪物
        for (int j = 0; j < enemy.size(); j++) {
          if (enemy[j].getExist()) {
            if (checkCollision(enemySprite[j], s)) {
              enemy[j].beHitted(b1.getDamage()*20);
              b1.bullets[i].setExist(false);
              break;
            }
          }
        }
        //子弹消失音效
        if (b1.bullets[i].getExist()) {
          window.draw(s);
        }
      }
    }

    // 打印敌人子弹+边界+人物击中
    t.loadFromFile("images/bullet4.png");
    ebullet.setTexture(t);
    for (int i = 0; i < enemy.size(); i++) {
      if (enemy[i].getShoot()&&enemy[i].getExist()) {

        for (int j = 0; j < enemy[i].ebullets.size(); j++) {
          if (enemy[i].ebullets[j].getExist()) {
            ebullet.setPosition(enemy[i].ebullets[j].getCoord());

            //检测与障碍物碰撞，没写完，少边框位置
            if (enemy[i].ebullets[j].getCoord().x < 50 ||
                enemy[i].ebullets[j].getCoord().x > 1350 ||
                enemy[i].ebullets[j].getCoord().y < 50 ||
                enemy[i].ebullets[j].getCoord().y > 800) {
              enemy[i].ebullets[j].setExist(false);
            }
            //检测与人物碰撞
            if (checkCollision(ebullet, s4)) {
              h1.beHitted(enemy[i].ebullets[j].damage);
              enemy[i].ebullets[j].setExist(false);
            }

            if (enemy[i].ebullets[j].getExist()) {
              window.draw(ebullet);
            }
          }
        }
      }
    }

    // 敌人
    for (int i = 0; i < 3; i++) {
      if (enemy[i].getExist()) {
        t.loadFromFile(enemy[i].getTexture());
        enemySprite[i].setTexture(t);
        enemySprite[i].setScale(2 * HPIC, 2 * HPIC);
        enemySprite[i].setPosition(enemy[i].getCoord());
        window.draw(enemySprite[i]);
      }
    }

    // 打印到屏幕上
    window.display();
    window.clear();
    Sleep(5);
  }

  return 0;
}