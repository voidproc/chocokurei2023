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


	// 回答残り時間

	if (swAnswer_.isRunning() && swAnswer_.sF() > (4.0 + 1.1))
	{

		swAnswer_.pause();
		setBalloonText_(TimeUpText.choice());
		levelAdjust_ = -1;
		swWaitMouseAction_.pause();
		swNextStage_.restart();
	}


	// マウス操作

	if (isMouseActionEnabled_())
	{
		// チョコがクリックされた

		if (chocoAreaMouseOver_ && chocoAreaMouseOver_->leftClicked())
		{
			(*chocoMouseOver_)->take();

			chocoClicked_ = *chocoMouseOver_;

			if (isFullFilledCondition_(**chocoMouseOver_, condition_))
			{
				setBalloonText_(SuccessText.choice());
				levelAdjust_ = 1;
			}
			else
			{
				setBalloonText_(FailedText.choice());
				levelAdjust_ = -1;
				miss_++;
			}

			// 回答タイマーストップ
			swAnswer_.pause();

			// マウス操作ストップ
			swWaitMouseAction_.pause();

			// すこし待機した後、次のステージへ
			swNextStage_.restart();
		}
	}

	// 次のステージかエンディングへ

	if (swNextStage_.isRunning() && swNextStage_.sF() > 3.0)
	{
		swNextStage_.pause();

		if (miss_ == 5)
		{
			getData().endingType = EndingType::Bad;

			changeScene(U"EndingScene", 0ms);

		}
		else if (level_ > 20)
		{
			getData().endingType = EndingType::Good;

			changeScene(U"EndingScene", 0ms);
		}
		else {
			setNewStage_();
		}
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

	drawPronamachan_();

	drawBalloon_();

	// ステージ名
	const auto stagenameRect = RectF{ 0, 12, Scene::Width(), 16 };
	stagenameRect.movedBy(0, 1).draw(ColorF{ 0, 0.1 });
	stagenameRect.draw(ColorF{ U"#d13288" });
	FontAsset(U"main")(U"～ ASSORTED CHOCOLATES No.{} ～"_fmt(level_)).drawAt(stagenameRect.center(), Palette::White);

	// 箱
	TextureAsset(U"box-{}x{}"_fmt(row_, column_)).drawAt(Scene::Center());

	for (const auto& choco : choco_)
	{
		choco.draw();
	}

	// 回答残り時間

	if (swAnswer_.sF() > 1.1)
	{
		const double barWidth = Clamp(108 * (1 - (swAnswer_.sF() - 1.1) / 4.0), 0.01, 108.0);
		RectF{ Scene::Center().movedBy(-108 / 2, 46), SizeF(108, 4) }.draw(Palette::Black);
		RectF{ Scene::Center().movedBy(-108 / 2, 46), SizeF(barWidth, 4) }.draw(Palette::White);
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

	// チョコを取った時の判定文字

	if (swNextStage_.isRunning() && swNextStage_.sF() < 1.5 && chocoClicked_)
	{
		ColorF color = Palette::White;
		if (swNextStage_.sF() > 1.0) {
			color = ColorF{ 1.0, Periodic::Square0_1(0.08ms) };
		}
		const double t = Clamp(swNextStage_.sF() / 0.30, 0.0, 1.0);
		Vec2 pos = (*chocoClicked_)->pos() - Vec2(0, 5.0 * EaseOutQuint(t));
		TextureAsset(levelAdjust_ == 1 ? U"good" : U"miss").drawAt(pos.asPoint(), color);
	}
}

void MainScene::setNewStage_()
{
	level_ = Max(level_ + levelAdjust_, 1);
	levelAdjust_ = 0;

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

	// 回答タイマー
	swAnswer_.restart();

	chocoClicked_ = none;
}

void MainScene::setBalloonText_(StringView text)
{
	balloonText_ = text;
	swBalloonTextAnimate_.restart();
}

void MainScene::drawPronamachan_() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	if (levelAdjust_ == -1 && swNextStage_.isRunning())
	{
		double xd = 0;
		if (swNextStage_.sF() < 0.3)
		{
			xd = 1.5 * Periodic::Sine1_1(0.15s);
		}

		TextureAsset(U"pronama-chan-fail").draw(basePos.movedBy(xd, 0));
		return;
	}

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
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12 + 1), ColorF(0, 0.1));
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12), Color(U"#421d12"));
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
