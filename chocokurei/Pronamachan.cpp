#include "stdafx.h"
#include "Pronamachan.h"
#include "SpriteSheet.h"

Pronamachan::Pronamachan()
{
	startNormalAnimation_();
}

void Pronamachan::update()
{
	if (timerAnimate_.reachedZero())
	{
		startNormalAnimation_();
	}
}

void Pronamachan::draw() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	// ミスした時のアニメーション
	if (swMiss_.isRunning())
	{
		double xd = 0;
		if (swMiss_.sF() < 0.3)
		{
			xd = 1.5 * Periodic::Sine1_1(0.15s);
		}

		TextureAsset(U"pronama-chan-fail").draw(basePos.movedBy(xd, 0));
		return;
	}

	int frame = (int)((timerAnimate_.duration().count() - timerAnimate_.sF()) / 0.5 * 8);
	if (frame >= 8)
	{
		frame = 0;
	}

	SpriteSheet::frame(U"pronama-chan", frame, 8).draw(basePos);
}

void Pronamachan::miss()
{
	swMiss_.restart();
}

void Pronamachan::reset()
{
	swMiss_.pause();
}

void Pronamachan::startNormalAnimation_()
{
	timerAnimate_.set(Duration(Random(2.0, 5.0)));
	timerAnimate_.start();
}
