#pragma once

class Chocolate
{
	struct ChocolateFeature
	{
		String name;
		bool isHeartShape;
		bool isSquareShape;
		bool isRoundShape;
		bool isBrown;
		bool isWhite;
	};

public:
	Chocolate(int type);

	void update();

	void draw() const;

private:
	int type_; // 0..11

	Array<ChocolateFeature> features_;
};
