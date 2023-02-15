#include "stdafx.h"
#include "Chocolate.h"

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
	: type_(type), pos_{ 0, Random(-30, -100) }, targetPos_{}, srcPos_{}, moveTimeSec_(0)
{
	timerAnimate_.set(Duration(Random(1.0, 4.0)));
	timerAnimate_.start();
}

void Chocolate::update()
{
	if (swMoveToTarget_.isRunning())
	{
		const double t = Clamp(swMoveToTarget_.sF() / moveTimeSec_, 0.0, 1.0);
		const double ease = (EaseOutBounce(t) + EaseOutQuint(t)) / 2.0;

		pos_.y = srcPos_.y + (targetPos_.y - srcPos_.y) * ease;
	}

	if (timerAnimate_.reachedZero())
	{
		timerAnimate_.set(Duration(Random(4.0, 6.0)));
		timerAnimate_.start();
	}
}

void Chocolate::draw() const
{
	const auto texture = TextureAsset(feature().name);
	int frame = 0;

	if (texture.width() > 16)
	{
		//アニメあり

		frame = (int)((timerAnimate_.duration().count() - timerAnimate_.sF()) / 0.5 * 8);
		if (frame >= 8)
		{
			frame = 0;
		}
	}

	// takeされたら少し点滅して消える

	double alpha = 1.0;

	if (swTake_.isRunning())
	{
		if (swTake_.sF() < 0.5)
		{
			alpha = 0.5 + 0.5 * Periodic::Square0_1(0.08s);
		}
		else
		{
			alpha = 0;
		}
	}

	texture(Rect(16).movedBy(frame * 16, 0)).drawAt(pos_, ColorF(Palette::White, alpha));
}

Vec2 Chocolate::pos() const
{
	return pos_;
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
	return gChocolateFeatures[type_];
}

void Chocolate::take()
{
	swTake_.restart();
}

bool Chocolate::mouseOver() const
{
	return collision().mouseOver();
}

bool Chocolate::leftClicked() const
{
	return collision().leftClicked();
}

RectF Chocolate::collision() const
{
	return RectF{ Arg::center = pos_, 14 };
}
