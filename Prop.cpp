#include"Prop.h"
using std::string;


extern std::vector<Counter>counters;
extern RenderWindow window_;
extern std::vector<Block> blocks; //障碍物
extern std::list<Box> boxes; //未击碎的宝箱,通关时需要清空
extern std::list<Box>::iterator boxes_index;  //宝箱列表迭代器
extern std::list<Prop>prop; //未拾取的道具
extern std::list<Prop>::iterator prop_index; //道具列表迭代器
extern PropMenu propmenu[6]; //道具栏 五个道具 不要propmenu[0]了
extern std::queue<Bomb>bomb_que{}; //炸弹队列
extern std::queue<Bomb>burst_que{}; //爆炸特效队列
extern int pause_no_;//时间停止对应的计数器编号


extern sf::Texture box_tex; //宝箱纹理
extern sf::Texture box_open_tex; //打开的宝箱纹理
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist;
extern sf::Texture chose;//道具栏已选中



//类成员函数
Box::Box(float x_, float y_){//ctor
	x = x_;
	y = y_;
	cimage = Sprite{ box_tex };
	cimage.setScale(0.8f, 0.8f);
	cimage.setPosition(x_, y_);
	oimage = Sprite{ box_open_tex };
	oimage.setScale(0.8f, 0.8f);
	oimage.setPosition(x_, y_);
}
float Box::getx() {//getter
	return x;
}
float Box::gety() {//getter
	return y;
}
bool Box::getState() {//getter
	return state;
}
void Box::drop() {//掉落道具
	state = !state;
	std::mt19937 rng;
	rng.seed(std::random_device()());//初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> poss(1, 100);
	int temp = poss(rng);
	if (temp > 0 && temp <= 20) prop.push_back(Prop{ x, y ,1 });
	else if (temp > 20 && temp <= 40)prop.push_back(Prop{ x, y, 2 });
	else if (temp > 40 && temp <= 60)prop.push_back(Prop{ x, y, 3 });
	else if (temp > 60 && temp <= 80)prop.push_back(Prop{ x, y ,4 });
	else if (temp > 80 && temp <= 100)prop.push_back(Prop{ x, y ,5 });
}
sf::Sprite Box::getCimage() {//getter
	return cimage;
}
sf::Sprite Box::getOimage() {//getter
	return oimage;
}
void Box::open() {
	state = 0;
}
Prop::Prop(float x_, float y_, int mode_) {//ctor
	x = x_;
	y = y_;
	mode = mode_;
	if(mode_==1)image = Sprite{ fheal_tex };
	else if (mode_ == 2)image = Sprite{ pheal_tex };
	else if (mode_ == 3)image = Sprite{ bomb_tex };
	else if (mode_ == 4)image = Sprite{ lvup_tex };
	else if (mode_ == 5)image = Sprite{ pause_tex };
	image.setScale(0.2f, 0.2f);
}
sf::Sprite Prop::getImage() {//getter
	return image;
}
int Prop::getMode() {//getter
	return mode;
}
Bomb::Bomb(int x_, int y_, int num) {
	x = x_;
	y = y_;
	no_ = num;
	mode = 3;
}
int Bomb::getx() {
	return x;
}
int Bomb::gety() {
	return y;
}
int Bomb::getNo() {
	return no_;
}
void Bomb::setPos(int x_, int y_) {
	image.setPosition(x_, y_);
}
void PropMenu::reset() {
	isChosen = 0;
}





//函数
void textureInit() {//初始化纹理
	box_tex.loadFromFile("images/box.png");
	box_open_tex.loadFromFile("images/box_open.png");
	fheal_tex.loadFromFile("images/fheal.png");
	pheal_tex.loadFromFile("images/pheal.png");
	bomb_tex.loadFromFile("images/bomb.png");
	lvup_tex.loadFromFile("images/lvup.png");
	pause_tex.loadFromFile("images/pause.png");
	proplist.loadFromFile("images/proplist.png");
	chose.loadFromFile("images/chose.png");
}

void boxInit() {//随机生成1-3个宝箱
	std::mt19937 rng;
	rng.seed(std::random_device()());//初始化随机种子
	std::uniform_int_distribution<std::mt19937::result_type> num_range(1, 3);
	int num = num_range(rng);//随机生成宝箱数
	int flag = num;

	while (flag > 0) {
		std::mt19937 rng;
		rng.seed(std::random_device()());//初始化随机种子
		std::uniform_int_distribution<std::mt19937::result_type> x_(100, 1100);
		std::uniform_int_distribution<std::mt19937::result_type> y_(100, 650);
		int temp_x = x_(rng);
		int temp_y = y_(rng);//初始化随机位置
		int temp_flag = 1;

		for (int i = 0; i < blocks.size(); i++) {//检测宝箱坐标是否和障碍物重合
			if (temp_x == blocks[i].x && temp_y == blocks[i].y) {
				temp_flag = 0;
				break;
			}
		}

		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {//检测生成的宝箱是否重合
			if (boxes_index->getx() == temp_x && boxes_index->gety() == temp_y) {
				temp_flag = 0;
				break;
			}
		}

		if (temp_flag) {
			boxes.push_back(Box(temp_x, temp_y));
			flag--;
		}
	}
}

void boxShow() { //画宝箱
	for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
		if (boxes_index->getState() == 1)
			window_.draw(boxes_index->getCimage());
		else if (boxes_index->getState() == 0)
			window_.draw(boxes_index->getOimage());
	}
}

void propShow() { //显示掉落道具
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
		window_.draw(prop_index->getImage());
}

void propMenuShow() { //展示物品栏
	sf::Sprite proplist_{ proplist }, chose_{ chose };
	chose_.setScale(0.2f, 0.f);
	proplist_.setScale(0.2f, 0.2f);
	proplist_.setPosition(500, 500);
	for (int i = 1; i <= 5; i++) {
		if (!propmenu[i].isChosen) {
			chose_.setPosition(500 + i, 500 + i);
			window_.draw(chose_);
		}
	}
}

bool checkCollision(sf::Sprite s1, sf::Sprite s2) {//测试用
	return 1;
}
sf::Sprite s4;//测试用，s4代替主角的sprite
void propPick() { //检测道具是否被拾取，若拾取则propmenu对应道具数目加1
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++) {
		if (checkCollision(prop_index->getImage(), s4)){//检测主角和道具碰撞
			switch (prop_index->getMode()) {//道具数目增加
			case 1:
				propmenu[1].num++;
				break;
			case 2:
				propmenu[2].num++;
				break;
			case 3:
				propmenu[3].num++;
				break;
			case 4:
				propmenu[4].num++;
				break;
			case 5:
				propmenu[5].num++;
				break;
			}
			prop.erase(prop_index);//从掉落道具列表移除
		}
	}
}

class Master {//用于测试对主角生效的道具
	int health;
	int weaponlv;
public:
	int getHp() {
		return health;
	}
	void setHp(int a) {
		health = a;
	}
	int getWeaponLevel() {
		return weaponlv;
	}
	void setWeaponLevel(int a) {
		weaponlv = a;
	}
}master;
void prop_to_master() {//使用对主角生效的道具
	int health = master.getHp();
	int wplv = master.getWeaponLevel();
	if (propmenu[1].isChosen) {
		master.setHp(5);//大血瓶，回满
		propmenu[1].reset();
	}
	else if (propmenu[2].isChosen) {//小血瓶回2
		if (health <= 5 - 2)master.setHp(health + 2);
		else master.setHp(5);
		propmenu[2].reset();
	}
	else if (propmenu[4].isChosen) {//武器升一级
		if (wplv <= 5 - 2)master.setWeaponLevel(wplv + 1);
		else master.setWeaponLevel(3);
		propmenu[4].reset();
	}
}

std::vector<Enemy>monster_data{};//测试用，怪物对象数组
std::vector<sf::Sprite>monster_sp{};//测试用，怪物贴图数组
void prop_to_monster(int x_, int y_) {//使用对怪物生效的道具
	if (propmenu[3].isChosen) {//炸弹，对范围内的小怪有伤害
		counters.push_back(Counter{ 150 });//炸弹引信时间为150帧，时间1.5s
		bomb_que.push(Bomb{ x_, y_, counters.back().getNo_() });//push对应计数器编号
		propmenu[3].reset();
	}
	else if (propmenu[5].isChosen) {//时间停止，所有小怪停止移动一段时间
		counters.push_back(Counter{ 400 });//时间停止400帧，时间4s
		pause_no_ = counters.back().getNo_();//对应计数器编号
		for (int i = 0; i < monster_data.size(); i++) {
			monster_data[i].setSpeed(0);
		}
		propmenu[5].reset();
	}

	int no = bomb_que.back().getNo();
	if (counters[no].getOver()) {//炸弹到时间
		for (int i = 0; i < monster_data.size(); i++) {
			int dx = monster_sp[i].getPosition().x - bomb_que.back().getx();
			int dy = monster_sp[i].getPosition().y - bomb_que.back().gety();
			if (dx * dx + dy * dy <= 50)monster_data[i].beHitted(100);//炸弹伤害
		}
		counters[no].reset();
		counters[no].setLimit(60);//爆炸特效时间
		burst_que.push(bomb_que.back());
		bomb_que.pop();
	}
	if (counters[pause_no_].getOver()) {//时间停止结束
		for (int i = 0; i < monster_data.size(); i++)
			monster_data[i].setSpeed(2);
	}
}

void effectShow() {//道具特效
	std::queue<Bomb>temp{};
	sf::Texture bomb1, bomb2;
	sf::Texture burst1, burst2, burst3;
	bomb1.loadFromFile("images/bomb1");
	bomb2.loadFromFile("images/bomb2");
	burst1.loadFromFile("images/burst1");
	burst2.loadFromFile("images/burst2");
	burst3.loadFromFile("images/burst3");
	sf::Sprite bb1{ bomb1 }, bb2{ bomb2 };
	sf::Sprite bt1{ burst1 }, bt2{ burst2 }, bt3{ burst3 };

	while (!bomb_que.empty()) {//画炸弹动图
		temp.push(bomb_que.back());
		bomb_que.pop();
		int now = counters[temp.back().getNo()].getNow();
		if ((now >= 0 && now < 30) || (now >= 60 && now < 90) || (now >= 120 && now < 150))
			window_.draw(bb1);
		else window_.draw(bb2);
	}
	while (!temp.empty()) {
		bomb_que.push(temp.back());
		temp.pop();
	}

	while (!burst_que.empty()) {//画爆炸动图
		temp.push(burst_que.back());
		burst_que.pop();
		int now = counters[temp.back().getNo()].getNow();
		if (now >= 0 && now < 20)window_.draw(bt1);
		else if (now >= 20 && now < 40)window_.draw(bt2);
		else if (now >= 40 && now < 60)window_.draw(bt3);
	}
	while (!temp.empty()) {
		burst_que.push(temp.back());
		temp.pop();
	}
}
