#pragma once

class Chocolate
{
public:
	Chocolate(int type);

	void update();

	void draw() const;

	void setTargetPos(const Vec2& targetPos);

private:
	int type_; // 0..11
	Vec2 pos_;
	Vec2 targetPos_;
	Stopwatch swMoveToTarget_;
};
