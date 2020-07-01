#include"Counter.h"


Counter::Counter() {
	num++;
	no_ = num;
}
Counter::Counter(int lim) {
	num++;
	no_ = num;
	limit = lim;
}
int Counter::getNow() {
	return now;
}
int Counter::getLimit() {
	return limit;
}
bool Counter::getOver() {
	return isOver;
}
int Counter::getNo_() {
	return no_;
}
void Counter::add() {
	if (now < limit)now++;
	else reset();
}
void Counter::reset() {
	now = 0;
}
void Counter::setLimit(int lim) {//ÖØÖÃÉÏÏŞ
	limit = lim;
}