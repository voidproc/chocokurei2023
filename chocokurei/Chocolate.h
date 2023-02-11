#pragma once

struct ChocolateFeature
{
	String name;
	bool isHeartShape;
	bool isSquareShape;
	bool isRoundShape;
	bool isBrownColor;
	bool isWhiteColor;
};

class Chocolate
{
public:
	Chocolate(int type);

	void update();

	void draw() const;

	void setPos(const Vec2& pos);

	void moveTo(const Vec2& targetPos, double timeSec = 1.0);

	int type() const;

	const ChocolateFeature& feature() const;

	void take();

private:
	int type_;
	const ChocolateFeature& feature_;
	Vec2 pos_;
	Vec2 targetPos_;
	Vec2 srcPos_;
	double moveTimeSec_;
	Stopwatch swMoveToTarget_;
	Timer timerAnimate_;
	Stopwatch swTake_;
};
