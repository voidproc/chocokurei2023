#include "stdafx.h"
#include "Chocolate.h"

//struct ChocolateFeature
//{
//	String name;
//	bool isHeartShape;
//	bool isSquareShape;
//	bool isRoundShape;
//	bool isBrownColor;
//	bool isWhiteColor;
//};
//
static const Array<ChocolateFeature> gChocolateFeatures = {
	// name,      heart, square,round, brown, white
	{ U"choco0",  true,  false, false, true,  false }, //brown-heart
	{ U"choco1",  true,  false, false, false, true  }, //white-heart
	{ U"choco2",  true,  false, false, false, false }, //red-heart
	{ U"choco3",  true,  false, false, false, false }, //purple-heart
	{ U"choco4",  false, true,  false, true,  false }, //brown-square
	{ U"choco5",  false, true,  false, false, true  }, //white-square
	{ U"choco6",  false, true,  false, true,  true  }, //white&brown-square(1)
	{ U"choco7",  false, true,  false, true,  true  }, //white&brown-square(2)
	{ U"choco8",  false, false, true,  false, true  }, //white-round
	{ U"choco9",  false, false, true,  true,  false }, //brown-round
	{ U"choco10", false, false, true,  true,  false }, //brown-round
	{ U"choco11", false, false, true,  false, true  }, //white-round-face
};

Chocolate::Chocolate(int type)
	: type_(type), feature_{ gChocolateFeatures[type] }, pos_{
	0, -100
}, targetPos_{}, srcPos_{}, moveTimeSec_(0), swMoveToTarget_{}
{
}

void Chocolate::update()
{
	if (swMoveToTarget_.isRunning())
	{
		const double t = Clamp(swMoveToTarget_.sF() / moveTimeSec_, 0.0, 1.0);
		const double ease = (EaseOutBounce(t) + EaseOutQuint(t)) / 2.0;

		pos_.y = srcPos_.y + (targetPos_.y - srcPos_.y) * ease;
	}
}

void Chocolate::draw() const
{
	const auto texture = TextureAsset(feature_.name);
	int frame = 0;

	texture(Rect(16)).drawAt(pos_);
}

void Chocolate::setPos(const Vec2& pos)
{
	pos_ = pos;
}

void Chocolate::moveTo(const Vec2& targetPos, double timeSec)
{
	targetPos_ = targetPos;
	pos_.x = targetPos.x;
	srcPos_ = pos_;
	moveTimeSec_ = timeSec;

	swMoveToTarget_.restart();
}

int Chocolate::type() const
{
	return type_;
}

const ChocolateFeature& Chocolate::feature() const
{
	return feature_;
}

