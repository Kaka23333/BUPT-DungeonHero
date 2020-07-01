#include"Prop.h"
using std::string;


extern std::vector<Counter>counters;
extern RenderWindow window_;
extern std::vector<Block> blocks; //�ϰ���
extern std::list<Box> boxes; //δ����ı���,ͨ��ʱ��Ҫ���
extern std::list<Box>::iterator boxes_index;  //�����б������
extern std::list<Prop>prop; //δʰȡ�ĵ���
extern std::list<Prop>::iterator prop_index; //�����б������
extern PropMenu propmenu[6]; //������ ������� ��Ҫpropmenu[0]��
extern std::queue<Bomb>bomb_que{}; //ը������
extern std::queue<Bomb>burst_que{}; //��ը��Ч����
extern int pause_no_;//ʱ��ֹͣ��Ӧ�ļ��������


extern sf::Texture box_tex; //��������
extern sf::Texture box_open_tex; //�򿪵ı�������
extern sf::Texture fheal_tex;
extern sf::Texture pheal_tex;
extern sf::Texture bomb_tex;
extern sf::Texture lvup_tex;
extern sf::Texture pause_tex;
extern sf::Texture proplist;
extern sf::Texture chose;//��������ѡ��



//���Ա����
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
void Box::drop() {//�������
	state = !state;
	std::mt19937 rng;
	rng.seed(std::random_device()());//��ʼ���������
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





//����
void textureInit() {//��ʼ������
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

void boxInit() {//�������1-3������
	std::mt19937 rng;
	rng.seed(std::random_device()());//��ʼ���������
	std::uniform_int_distribution<std::mt19937::result_type> num_range(1, 3);
	int num = num_range(rng);//������ɱ�����
	int flag = num;

	while (flag > 0) {
		std::mt19937 rng;
		rng.seed(std::random_device()());//��ʼ���������
		std::uniform_int_distribution<std::mt19937::result_type> x_(100, 1100);
		std::uniform_int_distribution<std::mt19937::result_type> y_(100, 650);
		int temp_x = x_(rng);
		int temp_y = y_(rng);//��ʼ�����λ��
		int temp_flag = 1;

		for (int i = 0; i < blocks.size(); i++) {//��ⱦ�������Ƿ���ϰ����غ�
			if (temp_x == blocks[i].x && temp_y == blocks[i].y) {
				temp_flag = 0;
				break;
			}
		}

		for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {//������ɵı����Ƿ��غ�
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

void boxShow() { //������
	for (boxes_index = boxes.begin(); boxes_index != boxes.end(); boxes_index++) {
		if (boxes_index->getState() == 1)
			window_.draw(boxes_index->getCimage());
		else if (boxes_index->getState() == 0)
			window_.draw(boxes_index->getOimage());
	}
}

void propShow() { //��ʾ�������
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++)
		window_.draw(prop_index->getImage());
}

void propMenuShow() { //չʾ��Ʒ��
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

bool checkCollision(sf::Sprite s1, sf::Sprite s2) {//������
	return 1;
}
sf::Sprite s4;//�����ã�s4�������ǵ�sprite
void propPick() { //�������Ƿ�ʰȡ����ʰȡ��propmenu��Ӧ������Ŀ��1
	for (prop_index = prop.begin(); prop_index != prop.end(); prop_index++) {
		if (checkCollision(prop_index->getImage(), s4)){//������Ǻ͵�����ײ
			switch (prop_index->getMode()) {//������Ŀ����
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
			prop.erase(prop_index);//�ӵ�������б��Ƴ�
		}
	}
}

class Master {//���ڲ��Զ�������Ч�ĵ���
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
void prop_to_master() {//ʹ�ö�������Ч�ĵ���
	int health = master.getHp();
	int wplv = master.getWeaponLevel();
	if (propmenu[1].isChosen) {
		master.setHp(5);//��Ѫƿ������
		propmenu[1].reset();
	}
	else if (propmenu[2].isChosen) {//СѪƿ��2
		if (health <= 5 - 2)master.setHp(health + 2);
		else master.setHp(5);
		propmenu[2].reset();
	}
	else if (propmenu[4].isChosen) {//������һ��
		if (wplv <= 5 - 2)master.setWeaponLevel(wplv + 1);
		else master.setWeaponLevel(3);
		propmenu[4].reset();
	}
}

std::vector<Enemy>monster_data{};//�����ã������������
std::vector<sf::Sprite>monster_sp{};//�����ã�������ͼ����
void prop_to_monster(int x_, int y_) {//ʹ�öԹ�����Ч�ĵ���
	if (propmenu[3].isChosen) {//ը�����Է�Χ�ڵ�С�����˺�
		counters.push_back(Counter{ 150 });//ը������ʱ��Ϊ150֡��ʱ��1.5s
		bomb_que.push(Bomb{ x_, y_, counters.back().getNo_() });//push��Ӧ���������
		propmenu[3].reset();
	}
	else if (propmenu[5].isChosen) {//ʱ��ֹͣ������С��ֹͣ�ƶ�һ��ʱ��
		counters.push_back(Counter{ 400 });//ʱ��ֹͣ400֡��ʱ��4s
		pause_no_ = counters.back().getNo_();//��Ӧ���������
		for (int i = 0; i < monster_data.size(); i++) {
			monster_data[i].setSpeed(0);
		}
		propmenu[5].reset();
	}

	int no = bomb_que.back().getNo();
	if (counters[no].getOver()) {//ը����ʱ��
		for (int i = 0; i < monster_data.size(); i++) {
			int dx = monster_sp[i].getPosition().x - bomb_que.back().getx();
			int dy = monster_sp[i].getPosition().y - bomb_que.back().gety();
			if (dx * dx + dy * dy <= 50)monster_data[i].beHitted(100);//ը���˺�
		}
		counters[no].reset();
		counters[no].setLimit(60);//��ը��Чʱ��
		burst_que.push(bomb_que.back());
		bomb_que.pop();
	}
	if (counters[pause_no_].getOver()) {//ʱ��ֹͣ����
		for (int i = 0; i < monster_data.size(); i++)
			monster_data[i].setSpeed(2);
	}
}

void effectShow() {//������Ч
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

	while (!bomb_que.empty()) {//��ը����ͼ
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

	while (!burst_que.empty()) {//����ը��ͼ
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
