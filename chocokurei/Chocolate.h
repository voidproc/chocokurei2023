#pragma once

#include "ChocolateFeature.h"

class Chocolate
{
public:
	Chocolate(int type = 0);

	void update();

	void draw() const;

	Vec2 pos() const;

	void setPos(const Vec2& pos);

	void moveTo(const Vec2& targetPos, double timeSec = 1.0);

	int type() const;

	const ChocolateFeature& feature() const;

	void take();

	bool mouseOver() const;

	bool leftClicked() const;

	RectF collision() const;

private:
	int type_;
	Vec2 pos_;
	Vec2 targetPos_;
	Vec2 srcPos_;
	double moveTimeSec_;
	Stopwatch swMoveToTarget_;
	Timer timerAnimate_;
	Stopwatch swTake_;
};
