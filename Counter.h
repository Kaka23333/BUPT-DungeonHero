#pragma once
#include <SFML/Graphics.hpp>

class Counter {//֡��������
	static int num;
	int no_;//���������
	int now{ 0 };//��ǰΪ����֡
	bool isOver{ 0 };//�Ƿ�ﵽ����
	int limit{10};//����
public:
	Counter();
	Counter(int lim);
	int getNow();
	int getLimit();
	int getNo_();
	bool getOver();
	void add();//������1
	void reset();//����
	void setLimit(int lim);//��������
};
