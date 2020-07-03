#include "enemy.h"
#include "hero.h"
#include "prop.h"
#include "shoot.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define HORI 320  //英雄宽度像素
#define HPIC 0.25 //英雄放缩比例
//重要 : 方向统一 上0下1左2右三
//英雄偏移向量1

using namespace sf;
typedef sf::Keyboard sk; //重要sf::Keyboard sk 简写
sf::RenderWindow window;

std::vector<sf::Sprite> enemySprite{}; //敌人图像的对象数组

std::list<Box> boxes;                 //未击碎的宝箱,通关时需要清空
std::list<Box>::iterator boxes_index; //宝箱列表迭代器
std::list<Prop> prop;                 //未拾取的道具
std::list<Prop>::iterator prop_index; //道具列表迭代器
PropMenu propmenu[6];        //道具栏 五个道具 不要propmenu[0]了
std::vector<Counter> pauses; //时间停止倒计时
std::vector<Bomb> bombs;     //炸弹倒计时队列

std::vector<Effect> effects; //特效数组
std::vector<Effect>::iterator effects_index;

sf::Texture box_tex;      //宝箱纹理
sf::Texture box_open_tex; //打开的宝箱纹理
sf::Texture fheal_tex;
sf::Texture pheal_tex;
sf::Texture bomb_tex;
sf::Texture lvup_tex;
sf::Texture pause_tex;
sf::Texture proplist; //道具栏纹理
sf::Texture bomb1, bomb2, bomb3;
sf::Texture HPUP, TMP, LVUP;
sf::Texture open1, open2, open3, open4, open5, open6, open7, open8, open9;
sf::Texture burst1, burst2, burst3, burst4, burst5, burst6, burst7, burst8;
sf::Texture pheal1, pheal2, pheal3, pheal4, pheal5;
int nEnemy{0};
std::vector<Enemy> enemy{1, 2, 2, 2, 2, 2};
std::vector<Vector2f> barrier; // 存放障碍物
Hero h1{200, 200};
Weapon b1;
Sprite s, s4, ebullet, boss_draw;
Boss boss1{500, 200, h1};
Music menuMusic, gameMusic, bossMusic;
SoundBuffer buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7,
    buffer8, buffer9, buffer10;
Sound press, mdeath, bullet_t, bullet_f, mbullet, heal1, heal2, bomb_s, box_s,
    win;
bool bossNext{false};
bool isMusicPlay{true};
bool isEffectPlay{true};
int musicVolume{20};
int effectVolume{100};
float heroCoord[4]{85 * 0.25, 206 * 0.25, 252 * 0.25, 356 * 0.25};

bool checkCollision(Sprite s1) {
  sf::FloatRect box{h1.x + heroCoord[0], h1.y + heroCoord[1],
                    heroCoord[2] - heroCoord[0],
                    heroCoord[3] - heroCoord[1]}; //坐标及边长创建方行
  return s1.getGlobalBounds().intersects(box);
}

class Room {
private:
  int type;           // 房间类型，共三种
  bool isOpen{false}; // 房间门是否打开
  Mutex roomMutex;
  Sprite scene, nextDoor, death, bossDoor, bossahead, boss,
      win_draw; // 场景贴图
  Texture block_, scene_, nextDoor_, death_, bossDoor_, bossahead_, boss_,
      win_draw_; // 边界块

public:
  Sprite block, chest;
  Room(int type_) : type{type_} {
    propsystem_init();
    block_.loadFromFile("images/block.png");
    scene_.loadFromFile("images/back.png");
    nextDoor_.loadFromFile("images/nextDoor.png");
    death_.loadFromFile("images/dead.png");
    bossDoor_.loadFromFile("images/bossdoor.png");
    bossahead_.loadFromFile("images/bossahead.png");
    boss_.loadFromFile("images/boss.png");
    win_draw_.loadFromFile("images/win.png");

    death.setTexture(death_);
    block.setTexture(block_);
    scene.setTexture(scene_);
    nextDoor.setTexture(nextDoor_);
    bossDoor.setTexture(bossDoor_);
    bossahead.setTexture(bossahead_);
    boss.setTexture(boss_);
    win_draw.setTexture(win_draw_);

    buffer1.loadFromFile("Sound effect/button press.wav");
    buffer2.loadFromFile("Sound effect/小怪死亡1.wav");
    buffer3.loadFromFile("Sound effect/子弹发射.wav");
    buffer4.loadFromFile("Sound effect/子弹消失.wav");
    buffer5.loadFromFile("Sound effect/怪物子弹.wav");
    buffer6.loadFromFile("Sound effect/道具效果1.wav");
    buffer7.loadFromFile("Sound effect/道具效果2.wav");
    buffer8.loadFromFile("Sound effect/bomb.wav");
    buffer9.loadFromFile("Sound effect/box.wav");
    buffer10.loadFromFile("Sound effect/win.wav");

    press.setBuffer(buffer1);
    mdeath.setBuffer(buffer2);
    bullet_t.setBuffer(buffer3);
    bullet_f.setBuffer(buffer4);
    mbullet.setBuffer(buffer5);
    heal1.setBuffer(buffer6);
    heal2.setBuffer(buffer7);
    bomb_s.setBuffer(buffer8);
    box_s.setBuffer(buffer9);
    win.setBuffer(buffer10);

    if (0 == type) {
      barrier.push_back(sf::Vector2f{324, 247});
      barrier.push_back(sf::Vector2f{408, 247});
      barrier.push_back(sf::Vector2f{324, 327});
      barrier.push_back(sf::Vector2f{949, 247});
      barrier.push_back(sf::Vector2f{1033, 247});
      barrier.push_back(sf::Vector2f{1030, 327});
      barrier.push_back(sf::Vector2f{324, 560});
      barrier.push_back(sf::Vector2f{324, 634});
      barrier.push_back(sf::Vector2f{408, 635});
      barrier.push_back(sf::Vector2f{1033, 560});
      barrier.push_back(sf::Vector2f{1033, 634});
      barrier.push_back(sf::Vector2f{949, 634}); // 普通房间
      boxInit(1, 4);
      // 障碍物位置存储到barrier
    } else if (1 == type) {
      barrier.clear();
    } else if (2 == type) {
      // 道具房间
      barrier.clear();
      boxInit(5, 8);
    }
  }

  //打印房间
  void print_back() {
    roomMutex.lock();
    if (!bossNext && 1 != type) {
      if (nEnemy > 0)
        window.draw(scene);
      else if (nEnemy <= 0)
        window.draw(nextDoor);
    } else if (1 == type) {
      window.draw(boss);
    } else {
      if (nEnemy > 0)
        window.draw(bossahead);
      else if (nEnemy <= 0)
        window.draw(bossDoor);
    }

    // 障碍物
    for (int i = 0; i < barrier.size() && 1 != type; i++) {
      block.setPosition(barrier[i].x, barrier[i].y);
      window.draw(block);
    }
    if (2 == type) {
      // 宝箱房直接画出宝箱(多个)
    } else if (!nEnemy) {
      // 房间开门后在窗口上画出宝箱
    }
    roomMutex.unlock();
  }

  //打印主角+小怪
  void print_game() {
    sf::Font font;
    font.loadFromFile("images/ZYKai_C.ttf");
    sf::Text textHP("HP:", font, 20), textPR("DEbug make me crazy", font, 20);

    nEnemy = 6;

    for (int i = 0; i < nEnemy; i++)
      enemySprite.push_back(Sprite{});

    Texture t;

    sf::Clock clock_move, clock_hurt;
    float dtTime = 0;   // deltatime 时间间隔
    float delay = 0.02; //
    float dmTime = 0;   //受伤时间间隔
    float dmdelay = 0.75;

    //自定义帧率
    h1.launch();

    Event e;
    b1.setFire(false); //拉保险

    //
    //
    //游戏开始
    while (window.isOpen()) {
      dtTime += clock_move.restart().asSeconds();
      dmTime += clock_hurt.restart().asSeconds();

      //循环核心 时间刷新
      if (dtTime > 0.01)
        b1.setFire(true); //冷却时间过,允许开炮

      print_back();
      propsystem_run_fdraw();
      while (window.pollEvent(e)) //这样会快
      {                           //事件轮询
        char skills = 'B';
        if (e.type == sf::Event::Closed)
          window.close();
        if (e.type == sf::Event::KeyPressed) { //提高效率
          switch (e.key.code) {                //上0下1左2右三
          case sk::A:
            h1.dir = 2;
            break; //方向控制
          case sk::S:
            h1.dir = 1;
            break;
          case sk::D:
            h1.dir = 3;
            break;
          case sk::W:
            h1.dir = 0;
            break;
          case sk::Q:
            propmenu[1].isChosen = 1;
            skills = 'Q';
            break;
          case sk::R:
            propmenu[2].isChosen = 1;
            skills = 'R';
            break;
          case sk::E:
            propmenu[3].isChosen = 1;
            skills = 'E';
            break;
          case sk::T:
            propmenu[4].isChosen = 1;
            skills = 'T';
            break;
          case sk::G:
            propmenu[5].isChosen = 1;
            skills = 'G';
            break;
          case sk::F:
            for (boxes_index = boxes.begin(); boxes_index != boxes.end();
                 boxes_index++)
              boxes_index->detect(h1.getCoord().x, h1.getCoord().y);
            skills = 'F';
            break;
          }
        }
        if (e.type == sf::Event::Closed) {
          window.close();
          std::exit(0);
        }

        // 主角部分

        // 射击并打印主角子弹
        if (b1.getFire()) {
          if (e.type == sf::Event::MouseButtonPressed) // 鼠标按住
          {                                            //上0下1左2右三
            sf::Vector2f mouse{sf::Mouse::getPosition(window)}; //装填弹药
            if (propmenu[3].isChosen && propmenu[3].num)
              prop_bomb(mouse); //点击炸弹位置 ---道具系统接口，务必增添
            else {
              b1.setDirection(mouse, h1, (h1.dir % 2) * HPIC * HORI);
              bullet_t.play();
              b1.setFire(false); //火炮冷却
            }
          }
        }
      }
      // 血量显示

      textHP.setString("HP: " + std::to_string(h1.HP));
      textHP.setPosition(1, 1);
      textPR.setString("  Q Full Heal  " + std::to_string(propmenu[1].num) +
                       " R  Part Heal " + std::to_string(propmenu[2].num) +
                       " E  Bomb " + std::to_string(propmenu[3].num) +
                       "  T  Lv UP:" + std::to_string(propmenu[4].num) +
                       "  G  Time Pause:" + std::to_string(propmenu[5].num) +
                       "  F  pick box");
      textPR.setPosition(1, 40);
      // text.setFillColor(sf::Color::Red);
      textHP.setStyle(sf::Text::Bold);
      textPR.setStyle(sf::Text::Bold);

      //人物碰撞边框+打印
      t.loadFromFile(h1.getTexture());
      s4.setTexture(t);
      s4.setScale(HPIC, HPIC);
      s4.setPosition(h1.getCoord());
      if (dtTime > delay) {
        dtTime = 0;
        int temp[2]{h1.x, h1.y};
        h1.update(); //逻辑移动
        h1.renderHero();

        for (int i = 0; i < barrier.size(); i++) {
          //检测与障碍物碰撞
          block.setPosition(barrier[i]);
          s4.setPosition(h1.getCoord());
          if (checkCollision(block)) {
            h1.x = temp[0];
            h1.y = temp[1];
            break;
          }
        }
        if (h1.x < 50 || h1.x > 1250 || h1.y < 50 || h1.y > 750) {
          h1.x = temp[0];
          h1.y = temp[1];
        }
        s4.setPosition(h1.getCoord());
      }
      window.draw(s4);

      //判断英雄和怪物碰撞
      for (int i = 0; i < enemy.size(); i++) {
        if (enemy[i].getExist()) {
          if (checkCollision(enemySprite[i], s4)) {
            if (dmTime > dmdelay) {
              h1.beHitted(enemy[i].getDamge());
              dmTime = 0;
            }
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
          for (int j = 0; j < barrier.size(); j++) {
            block.setPosition(barrier[j]);

            if (checkCollision(s, block) || b1.bullets[i].getCoord().x < 50 ||
                b1.bullets[i].getCoord().x > 1350 ||
                b1.bullets[i].getCoord().y < 50 ||
                b1.bullets[i].getCoord().y > 800) {
              b1.bullets[i].setExist(false);
              if (isEffectPlay)
                bullet_f.play();
              break;
            }
          }
          //检测打击怪物
          for (int j = 0; j < enemy.size(); j++) {
            if (enemy[j].getExist()) {
              if (checkCollision(enemySprite[j], s)) {
                enemy[j].beHitted(b1.getDamage() * 20);
                b1.bullets[i].setExist(false);
                if (isEffectPlay)
                  bullet_f.play();
                if (isEffectPlay)
                  mdeath.play();
                break;
              }
            }
          }
          //检测打击boss
          if (checkCollision(s, boss_draw)) {
            b1.bullets[i].setExist(false);
            boss1.beHitted(b1.getDamage() * 20);
            if (isEffectPlay)
              bullet_f.play();
          }

          if (b1.bullets[i].getExist()) {
            window.draw(s);
          }
        }
      }

      // 打印敌人子弹+边界+人物击中
      t.loadFromFile("images/bullet4.png");
      ebullet.setTexture(t);
      for (int i = 0; i < enemy.size(); i++) {
        if (enemy[i].getShoot() && enemy[i].getExist()) {

          for (int j = 0; j < enemy[i].ebullets.size(); j++) {
            if (enemy[i].ebullets[j].getExist()) {
              ebullet.setPosition(enemy[i].ebullets[j].getCoord());

              //检测与障碍物碰撞，没写完，少边框位置
              roomMutex.lock();
              for (int k = 0; k < barrier.size(); k++) {
                block.setPosition(barrier[k]);
                if (checkCollision(block, ebullet) ||
                    enemy[i].ebullets[j].getCoord().x < 50 ||
                    enemy[i].ebullets[j].getCoord().x > 1350 ||
                    enemy[i].ebullets[j].getCoord().y < 50 ||
                    enemy[i].ebullets[j].getCoord().y > 800) {
                  enemy[i].ebullets[j].setExist(false);
                  if (isEffectPlay)
                    bullet_f.play();
                }
              }
              roomMutex.unlock();
              //检测与人物碰撞
              if (checkCollision(ebullet, s4)) {
                //受伤间隔
                if (dmTime > dmdelay) {
                  h1.beHitted(enemy[i].ebullets[j].damage);
                  dmTime = 0;
                }
                if (isEffectPlay)
                  bullet_f.play();
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
      for (int i = 0; i < enemy.size(); i++) {
        if (enemy[i].getExist()) {
          t.loadFromFile(enemy[i].getTexture());
          enemySprite[i].setTexture(t);
          enemySprite[i].setScale(2 * HPIC, 2 * HPIC);
          enemySprite[i].setPosition(enemy[i].getCoord());
          window.draw(enemySprite[i]);
        }
      }

      if (1 == type) {
        // boss
        t.loadFromFile(boss1.enemy);
        boss_draw.setTexture(t);
        boss_draw.setPosition(boss1.getCoord());
        window.draw(boss_draw);

        // boss子弹
        t.loadFromFile("images/bullet4.png");
        ebullet.setTexture(t);
        for (int j = 0; j < boss1.ebullets.size(); j++) {
          if (boss1.ebullets[j].getExist()) {
            ebullet.setPosition(boss1.ebullets[j].getCoord());
            //检测与人物碰撞
            if (checkCollision(ebullet, s4)) {
              //受伤间隔
              if (dmTime > dmdelay) {
                h1.beHitted(boss1.ebullets[j].damage);
                dmTime = 0;
              }
              if (isEffectPlay)
                bullet_f.play();
              boss1.ebullets[j].setExist(false);
            }
            if (boss1.ebullets[j].getExist()) {
              window.draw(ebullet);
            }
          }
        }
      }

      for (int j = 0; j < boss1.ebullets.size(); j++) {
        if (boss1.ebullets[j].getExist()) {
          ebullet.setPosition(boss1.ebullets[j].getCoord());
          window.draw(ebullet);
        }
      }

      // 新加
      window.draw(textHP);
      window.draw(textPR);
      if (!h1.isLive) {
        gameMusic.stop();
        SoundBuffer bf;
        bf.loadFromFile("Sound effect/death1.wav");
        Sound dead(bf);
        if (isEffectPlay)
          dead.play();
        window.draw(death);
        window.display();
        window.clear();
        Sleep(5000);
        break;
      }
      propsystem_run_ldraw();
      propsystem_run_data();

      // 防止抖动
      if (0 >= nEnemy)
        sleep(milliseconds(50));
      else if (1 == nEnemy)
        sleep(milliseconds(30));
      else if (2 == nEnemy)
        sleep(milliseconds(15));
      else
        sleep(milliseconds(5));

      if (!boss1.exist) {
        if (isEffectPlay)
          win.play();
        window.draw(win_draw);
        window.display();
        sleep(seconds(4));
        break;
      }

      // 打印到屏幕上
      window.display();
      window.clear();

      if (nEnemy <= 0 && h1.x > 650 && h1.x < 760 && h1.y < 80 && type != 1) {
        gameMusic.stop();
        break;
      }
    }
  }
};

// 菜单界面
int menu() {
  // 创建纹理对象
  Texture mainMenu_, start_, setting_, help_, exit_, basicHelp_, menuSetting_,
      musicOn_, musicOff_, effectOn_, effectOff_, twenty_, forty_, sixty_,
      eighty_, max_;
  // 初始化纹理
  mainMenu_.loadFromFile("images/mainMenu.png");
  start_.loadFromFile("images/start.png");
  setting_.loadFromFile("images/setting.png");
  help_.loadFromFile("images/help.png");
  exit_.loadFromFile("images/exit.png");
  basicHelp_.loadFromFile("images/basicHelp.png");
  menuSetting_.loadFromFile("images/menuSetting.png");
  musicOn_.loadFromFile("images/musicOn.png");
  musicOff_.loadFromFile("images/musicOff.png");
  effectOn_.loadFromFile("images/effectOn.png");
  effectOff_.loadFromFile("images/effectOff.png");
  twenty_.loadFromFile("images/20.png");
  forty_.loadFromFile("images/40.png");
  sixty_.loadFromFile("images/60.png");
  eighty_.loadFromFile("images/80.png");
  max_.loadFromFile("images/100.png");

  // 创建精灵对象
  Sprite mainMenu(mainMenu_), start(start_), setting(setting_), help(help_),
      exit(exit_), basicHelp(basicHelp_), menuSetting(menuSetting_),
      musicOn(musicOn_), musicOff(musicOff_), effectOn(effectOn_),
      effectOff(effectOff_), twenty(twenty_), forty(forty_), sixty(sixty_),
      eighty(eighty_), max(max_);
  // 打印菜单界面
  window.draw(mainMenu);

  Event mouse;
  menuMusic.play();

  while (window.isOpen()) {
    if (window.pollEvent(mouse)) // 检查是否有事件发生
    {
      if (mouse.type == sf::Event::Closed) {
        window.close();
        std::exit(0);
      }
      if (mouse.type == Event::MouseMoved) // 鼠标碰到按钮
                                           // 开始
        if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 792 &&
            mouse.mouseMove.y > 336 && mouse.mouseMove.y < 432) {
          window.draw(start);
          if (isEffectPlay)
            press.play();
        }
        // 设置
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 504 && mouse.mouseMove.y < 600) {
          window.draw(setting);
          if (isEffectPlay)
            press.play();
        }
        // 帮助
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 672 && mouse.mouseMove.y < 768) {
          window.draw(help);
          if (isEffectPlay)
            press.play();
        }
        // 退出
        else if (mouse.mouseMove.x > 600 && mouse.mouseMove.x < 816 &&
                 mouse.mouseMove.y > 840 && mouse.mouseMove.y < 912) {
          window.draw(exit);
          if (isEffectPlay)
            press.play();
        }
        // 没有碰到按钮
        else
          window.draw(mainMenu);
      if (mouse.type == Event::MouseButtonPressed) // 鼠标点击按钮
                                                   // 开始
        if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 792 &&
            mouse.mouseButton.y > 336 && mouse.mouseButton.y < 432 &&
            mouse.mouseButton.button == Mouse::Button::Left) {
          if (isEffectPlay)
            press.play();
          // 关闭菜单音乐
          menuMusic.stop();
          return 1;
        }
        // 设置
        else if (mouse.mouseButton.x > 600 && mouse.mouseButton.x < 816 &&
                 mouse.mouseButton.y > 504 && mouse.mouseButton.y < 600 &&
                 mouse.mouseButton.button == Mouse::Button::Left) {
          Event change;
          while (window.isOpen()) {
            window.draw(menuSetting);
            if (isMusicPlay)
              window.draw(musicOn);
            else
              window.draw(musicOff);
            if (isEffectPlay)
              window.draw(effectOn);
            else
              window.draw(effectOff);
            switch (effectVolume) {
            case 20:
              window.draw(twenty);
              break;
            case 40:
              window.draw(forty);
              break;
            case 60:
              window.draw(sixty);
              break;
            case 80:
              window.draw(eighty);
              break;
            case 100:
              window.draw(max);
              break;
            }
            if (window.pollEvent(change)) {
              if (change.type == Event::MouseButtonPressed) // 鼠标碰到按钮
                                                            // 音乐位置
                if (change.mouseButton.x > 816 && change.mouseButton.x < 912 &&
                    change.mouseButton.y > 312 && change.mouseButton.y < 360 &&
                    change.mouseButton.button == Mouse::Button::Left) {
                  isMusicPlay = !isMusicPlay;
                  if (isMusicPlay)
                    menuMusic.play();
                  else
                    menuMusic.stop();
                }
                // 音效位置
                else if (change.mouseButton.x > 816 &&
                         change.mouseButton.x < 912 &&
                         change.mouseButton.y > 456 &&
                         change.mouseButton.y < 504 &&
                         change.mouseButton.button == Mouse::Button::Left) {
                  isEffectPlay = !isEffectPlay;
                }
                // 音量--
                else if (change.mouseButton.x > 720 &&
                         change.mouseButton.x < 768 &&
                         change.mouseButton.y > 600 &&
                         change.mouseButton.y < 648 &&
                         change.mouseButton.button == Mouse::Button::Left) {
                  if (musicVolume)
                    musicVolume -= 4;
                  menuMusic.setVolume(musicVolume);
                  gameMusic.setVolume(musicVolume);
                  bossMusic.setVolume(musicVolume);
                  if (effectVolume)
                    effectVolume -= 20;
                  press.setVolume(effectVolume);
                  mdeath.setVolume(effectVolume);
                  bullet_t.setVolume(effectVolume);
                  bullet_f.setVolume(effectVolume);
                  mbullet.setVolume(effectVolume);
                  heal1.setVolume(effectVolume);
                  heal2.setVolume(effectVolume);
                  bomb_s.setVolume(effectVolume);
                  box_s.setVolume(effectVolume);
                  win.setVolume(effectVolume);
                }
                // 音量++
                else if (change.mouseButton.x > 936 &&
                         change.mouseButton.x < 1070 &&
                         change.mouseButton.y > 600 &&
                         change.mouseButton.y < 648 &&
                         change.mouseButton.button == Mouse::Button::Left) {
                  if (20 != musicVolume)
                    musicVolume += 4;
                  menuMusic.setVolume(musicVolume);
                  gameMusic.setVolume(musicVolume);
                  bossMusic.setVolume(musicVolume);
                  if (100 != effectVolume)
                    effectVolume += 20;
                  press.setVolume(effectVolume);
                  mdeath.setVolume(effectVolume);
                  bullet_t.setVolume(effectVolume);
                  bullet_f.setVolume(effectVolume);
                  mbullet.setVolume(effectVolume);
                  heal1.setVolume(effectVolume);
                  heal2.setVolume(effectVolume);
                  bomb_s.setVolume(effectVolume);
                  box_s.setVolume(effectVolume);
                  win.setVolume(effectVolume);
                } else if (change.mouseButton.x > 216 &&
                           change.mouseButton.x < 360 &&
                           change.mouseButton.y > 792 &&
                           change.mouseButton.y < 864 &&
                           change.mouseButton.button == Mouse::Button::Left) {
                  break;
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

int main() {
  window.create(sf::VideoMode(1440, 960), "Dumbledore's adventures");

  // 音乐初始化
  menuMusic.openFromFile("music/menu.wav");
  gameMusic.openFromFile("music/game.wav");
  bossMusic.openFromFile("music/Boss.wav");
  menuMusic.setLoop(true);
  gameMusic.setLoop(true);
  bossMusic.setLoop(true);
  menuMusic.setVolume(musicVolume);
  gameMusic.setVolume(musicVolume);
  bossMusic.setVolume(musicVolume);
  buffer1.loadFromFile("Sound effect/button press.wav");
  press.setBuffer(buffer1);

  // 射击线程
  Thread shootThread2{&Weapon::shoot_mouse, &b1};
  Thread enemyThread0{&Enemy::enemyMove, &enemy[0]};
  Thread enemyThread1{&Enemy::enemyMove, &enemy[1]};
  Thread enemyThread2{&Enemy::enemyMove, &enemy[2]};
  Thread enemyThread3{&Enemy::enemyMove, &enemy[3]};
  Thread enemyThread4{&Enemy::enemyMove, &enemy[4]};
  Thread enemyThread5{&Enemy::enemyMove, &enemy[5]};
  Thread bossThread1{&Boss::bossMove, &boss1};

  while (menu()) {
    shootThread2.launch();
    h1.restart();
    boss1.restart();
    for (int i = 0; i < 5; i++) {
      if (isMusicPlay && i != 4) {
        gameMusic.play();
      } else if (isMusicPlay && i == 4) {
        bossMusic.play();
      }

      if (3 == i)
        bossNext = true;
      else
        bossNext = false;

      h1.x = 720;
      h1.y = 700;

      Room room1{4 == i ? 1 : 0};

      //线程开启
      if (4 == i) //
        bossThread1.launch();
      enemyThread0.launch();
      enemyThread1.launch();
      enemyThread2.launch();
      enemyThread3.launch();
      enemyThread4.launch();
      enemyThread5.launch();
      //操作
      room1.print_game();
      boxes.clear();
      prop.clear();
      if (isMusicPlay && i != 4) {
        gameMusic.stop();
      } else if (isMusicPlay && i == 4) {
        bossMusic.stop();
      }

      //线程终止
      if (4 == i) //
        bossThread1.terminate();
      enemyThread0.terminate();
      enemyThread1.terminate();
      enemyThread2.terminate();
      enemyThread3.terminate();
      enemyThread4.terminate();
      enemyThread5.terminate();
      for (int i = 0; i < 6; i++) {
        enemy[i].launch();
        enemy[i].ebullets.clear();
      }
      if (!h1.isLive) {
        break;
      }
      if (!boss1.exist) {
        break;
      }
    }
    shootThread2.terminate();
  }
  return 0;
}