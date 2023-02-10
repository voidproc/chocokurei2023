#pragma once

class Chocolate
{
public:
	Chocolate(int type);

	void update();

	void draw() const;

	void moveTo(const Vec2& targetPos, double timeSec = 1.0);

private:
	int type_; // 0..11

	Vec2 pos_;

	Vec2 targetPos_;
	Vec2 srcPos_;
	double moveTimeSec_;
	Stopwatch swMoveToTarget_;
};
