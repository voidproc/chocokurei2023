#include "stdafx.h"
#include "Chocolate.h"

struct ChocolateFeature
{
	String name;
	bool isHeartShape;
	bool isSquareShape;
	bool isRoundShape;
	bool isBrown;
	bool isWhite;
};

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
	: type_(type), pos_{ 0, -100 }
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
	auto& feature = gChocolateFeatures[type_];

	TextureAsset(feature.name).drawAt(pos_);
}

void Chocolate::moveTo(const Vec2& targetPos, double timeSec)
{
	targetPos_ = targetPos;
	pos_.x = targetPos.x;
	srcPos_ = pos_;
	moveTimeSec_ = timeSec;

	swMoveToTarget_.restart();
}
