#pragma once
#include <SFML/Graphics.hpp>

class Counter {//帧数计数器
	static int num;
	int no_;//计数器编号
	int now{ 0 };//当前为多少帧
	bool isOver{ 0 };//是否达到上限
	int limit{10};//上限
public:
	Counter();
	Counter(int lim);
	int getNow();
	int getLimit();
	int getNo_();
	bool getOver();
	void add();//计数加1
	void reset();//重置
	void setLimit(int lim);//重置上限
};
