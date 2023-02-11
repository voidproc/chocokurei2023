#include "stdafx.h"
#include "MainScene.h"
#include "const.h"

MainScene::MainScene(const InitData& init)
	: IScene{ init }, choco_{}
{
	setNewStage_();

	timerPronamachanAnimate_.set(Duration(Random(2.0, 5.0)));
	timerPronamachanAnimate_.start();
}

void MainScene::update()
{
	for (auto& choco : choco_)
	{
		choco.update();
	}

	checkChocoAreaMouseOver_();

	if (isMouseActionEnabled_())
	{
		// チョコがクリックされた

		if (chocoAreaMouseOver_ && chocoAreaMouseOver_->leftClicked())
		{
			(*chocoMouseOver_)->take();

			if (isFullFilledCondition_(**chocoMouseOver_, condition_))
			{
				setBalloonText_(SuccessText.choice());
			}
			else
			{
				setBalloonText_(FailedText.choice());
			}

			// マウス操作ストップ
			swWaitMouseAction_.pause();

			// すこし待機した後、次のステージへ
			swNextStage_.restart();
		}
	}

	// 次のステージへ？

	if (swNextStage_.isRunning() && swNextStage_.sF() > 3.0)
	{
		swNextStage_.pause();

		setNewStage_();
	}

	// アニメーション
	if (timerPronamachanAnimate_.reachedZero())
	{
		timerPronamachanAnimate_.set(Duration(Random(2.0, 5.0)));
		timerPronamachanAnimate_.start();
	}

}

void MainScene::draw() const
{
	drawBg_();

	{
		const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);
		TextureAsset(U"bottom-shade").draw(basePos);
	}

	drawPronamachan_();

	drawBalloon_();

	TextureAsset(U"box-{}x{}"_fmt(row_, column_)).drawAt(Scene::Center());

	for (const auto& choco : choco_)
	{
		choco.draw();
	}

	// マウス位置

	if (isMouseActionEnabled_())
	{
		if (chocoAreaMouseOver_)
		{
			const ColorF areaColor = Sample({ Palette::Yellow, Palette::Cyan, Palette::Pink });
			chocoAreaMouseOver_->drawFrame(0.0, 1.0, ColorF(areaColor, Periodic::Square0_1(100ms)));
		}
	}
}

void MainScene::setNewStage_()
{
	level_ += 1;

	if (level_ > 16)
	{
		row_ = 4;
		column_ = 6;
	}
	else if (level_ > 12)
	{
		row_ = 4;
		column_ = 5;
	}
	else if (level_ > 8)
	{
		row_ = 4;
		column_ = 4;
	}
	else if (level_ > 4)
	{
		row_ = 3;
		column_ = 4;
	}
	else
	{
		row_ = 3;
		column_ = 3;
	}

	// チョコ生成

	choco_.clear();
	chocoArea_.clear();

	for (int i : step(row_ * column_))
	{
		const int chocoType = Random(0, ChocolateTypesCount - 1);
		choco_.emplace_back<Chocolate>(chocoType);

		choco_.back().setPos(Vec2(0, Random(-30, -100)));

		const double distance = 16.0;
		const double x = (i % column_) * distance - (distance * (column_ - 1)) / 2;
		const double y = (i / column_) * distance - (distance * (row_ - 1)) / 2;
		choco_.back().moveTo(Vec2(x, y) + Scene::Center(), 0.8 + Random(-0.2, 0.2));

		// 各チョコの当たり判定も生成しておく
		const Size areaSize{ 14, 14 };
		chocoArea_.emplace_back(Vec2(x, y) - areaSize/2 + Scene::Center(), areaSize);
	}

	// 選んでも大丈夫な条件が出るまで待つ

	bool validCondition = false;
	while (!validCondition)
	{
		condition_ = static_cast<Condition>(Random(0, ConditionCount - 1));

		for (const auto& choco : choco_)
		{
			if (isFullFilledCondition_(choco, condition_))
			{
				validCondition = true;
			}
		}
	}

	setBalloonText_(ConditionText[static_cast<int>(condition_)] + ConditionTextEnd.choice());

	// 少し待機した後、マウス操作を有効にする
	swWaitMouseAction_.restart();
}

void MainScene::setBalloonText_(StringView text)
{
	balloonText_ = text;
	swBalloonTextAnimate_.restart();
}

void MainScene::drawPronamachan_() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	int frame = (int)((timerPronamachanAnimate_.duration().count() - timerPronamachanAnimate_.sF()) / 0.5 * 8);
	if (frame >= 8)
	{
		frame = 0;
	}

	TextureAsset(U"pronama-chan")(Rect(32).movedBy(frame * 32, 0)).draw(basePos);
}

void MainScene::drawBalloon_() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	TextureAsset(U"balloon").draw(basePos);

	StringView balloonText = balloonText_.substrView(0, (int)(32 * swBalloonTextAnimate_.sF()));
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12), Palette::Black);
}

void MainScene::drawBg_() const
{
	Scene::SetBackground(Color(U"#dc678c"));

	const double distance = 114.0;
	const double offset = distance * Periodic::Sawtooth0_1(8.0s);
	const ColorF color{ Palette::White, 0.2 + 0.5 * Periodic::Sine0_1(2.4s)};
	TextureAsset(U"bg").draw(Vec2(0, offset), color);
	TextureAsset(U"bg").draw(Vec2(0, offset + distance), color);
	TextureAsset(U"bg").draw(Vec2(0, offset - distance), color);

}

bool MainScene::isFullFilledCondition_(const Chocolate& target, Condition cond)
{
	switch (cond)
	{
	case Condition::Any:
		return true;

	case Condition::IsHeartShape:
		return target.feature().isHeartShape;

	case Condition::IsRoundShape:
		return target.feature().isRoundShape;

	case Condition::IsSquareShape:
		return target.feature().isSquareShape;

	case Condition::IsBrownColor:
		return target.feature().isBrownColor;

	case Condition::IsWhiteColor:
		return target.feature().isWhiteColor;

	case Condition::IsMostAmount:
	case Condition::IsLeastAmount:
		Array<int> typeCount;
		typeCount.resize(ChocolateTypesCount, 0);
		for (const auto& choco : choco_)
		{
			typeCount[choco.type()]++;
		}
		
		if (cond == Condition::IsMostAmount)
		{
			return typeCount[target.type()] == *std::max_element(typeCount.begin(), typeCount.end());
		}
		else if (cond == Condition::IsLeastAmount)
		{
			return typeCount[target.type()] == *std::min_element(typeCount.begin(), typeCount.end());
		}
		return false;
	}

	return false;
}

bool MainScene::isMouseActionEnabled_() const
{
	return swWaitMouseAction_.isRunning() && swWaitMouseAction_.sF() > 1.1;
}

void MainScene::checkChocoAreaMouseOver_()
{
	if (!isMouseActionEnabled_())
	{
		chocoAreaMouseOver_ = none;
		chocoMouseOver_ = none;
		return;
	}

	for (auto [i, area] : Indexed(chocoArea_))
	{
		if (area.mouseOver())
		{
			chocoAreaMouseOver_ = area;
			chocoMouseOver_ = &choco_[i];
			return;
		}
	}

	chocoAreaMouseOver_ = none;
	chocoMouseOver_ = none;
}
