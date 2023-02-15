#include "stdafx.h"
#include "MainScene.h"
#include "Constants.h"

MainScene::MainScene(const InitData& init)
	: IScene{ init }, choco_{}
{
	setNewStage_();
}

void MainScene::update()
{
	pronamachan_.update();

	for (auto& choco : choco_)
	{
		choco.update();
	}

	checkChocoMouseState_();


	// 回答残り時間

	if (swAnswer_.isRunning() && swAnswer_.sF() > (4.0 + 1.1))
	{
		swAnswer_.pause();
		balloon_.setText(TimeUpText.choice());
		levelAdjust_ = -1;
		swWaitMouseAction_.pause();
		swNextStage_.restart();
	}

	// マウス操作

	if (isMouseActionEnabled_())
	{
		// チョコがクリックされた

		if (chocoClicked_)
		{
			chocoClicked_->take();

			if (isFullFilledCondition_(*chocoClicked_, condition_))
			{
				balloon_.setText(SuccessText.choice());
				levelAdjust_ = 1;
			}
			else
			{
				balloon_.setText(FailedText.choice());
				levelAdjust_ = -1;
				miss_++;
				pronamachan_.miss();
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
}

void MainScene::draw() const
{
	drawBg_();

	pronamachan_.draw();

	balloon_.draw();

	// ステージ名
	const auto stagenameRect = RectF{ 0, 12, Scene::Width(), 16 };
	stagenameRect.movedBy(0, 1).draw(ColorF{ 0, 0.1 });
	stagenameRect.draw(ColorF{ U"#d13288" });
	FontAsset(U"main")(U"～ ASSORTED CHOCOLATES No.{} ～"_fmt(level_)).drawAt(stagenameRect.center(), Palette::White);

	// 箱
	TextureAsset(U"box-{}x{}"_fmt(row_, column_)).drawAt(Scene::Center());

	// チョコ
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
		if (chocoMouseOver_)
		{
			const ColorF areaColor = Sample({ Palette::Yellow, Palette::Cyan, Palette::Pink });
			chocoMouseOver_->collision().drawFrame(0.0, 1.0, ColorF(areaColor, Periodic::Square0_1(100ms)));
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
		Vec2 pos = chocoClicked_->pos() - Vec2(0, 5.0 * EaseOutQuint(t));
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
	setNewChocolates_();

	// 条件を選ぶ
	// 選んでも大丈夫な条件が出るまで待つ

	bool isValidCondition = false;
	while (!isValidCondition)
	{
		condition_ = randomCondition_();

		for (const auto& choco : choco_)
		{
			if (isFullFilledCondition_(choco, condition_))
			{
				isValidCondition = true;
				break;
			}
		}
	}

	balloon_.setText(ConditionText[static_cast<int>(condition_)] + ConditionTextEnd.choice());

	// 少し待機した後、マウス操作を有効にする
	swWaitMouseAction_.restart();

	// 回答タイマー
	swAnswer_.restart();

	chocoClicked_ = none;

	// プロ生ちゃんの顔をリセット
	pronamachan_.reset();
}

void MainScene::setNewChocolates_()
{
	choco_.clear();

	for (int i : step(row_* column_))
	{
		const int chocoType = Random(0, ChocolateTypesCount - 1);
		auto& newChoco = choco_.emplace_back<Chocolate>(chocoType);

		const double distance = 16.0;
		const double x = (i % column_) * distance - (distance * (column_ - 1)) / 2;
		const double y = (i / column_) * distance - (distance * (row_ - 1)) / 2;
		newChoco.moveTo(Vec2(x, y) + Scene::Center(), 0.8 + Random(-0.2, 0.2));
	}

}

Condition MainScene::randomCondition_() const
{
	const Array<Condition> conditions = {
		Condition::Any,
		Condition::IsHeartShape,
		Condition::IsRoundShape,
		Condition::IsSquareShape,
		Condition::IsBrownColor,
		Condition::IsWhiteColor,
		Condition::IsMostAmount,
		Condition::IsLeastAmount,
	};

	DiscreteDistribution distribution(
	{
		2,
		10,
		10,
		10,
		9,
		9,
		25,
		25,
	});

	return DiscreteSample(conditions, distribution);
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

void MainScene::checkChocoMouseState_()
{
	chocoMouseOver_ = none;

	if (!isMouseActionEnabled_())
	{
		return;
	}

	for (auto& choco : choco_)
	{
		if (choco.leftClicked())
		{
			chocoMouseOver_ = choco;
			chocoClicked_ = choco;
			return;
		}

		if (choco.mouseOver())
		{
			chocoMouseOver_ = choco;
			return;
		}
	}
}

void MainScene::drawBg_() const
{
	Scene::SetBackground(Color(U"#dc678c"));

	const double distance = 114.0;
	const double offset = distance * Periodic::Sawtooth0_1(8.0s);
	const ColorF color{ Palette::White, 0.2 + 0.5 * Periodic::Sine0_1(2.4s) };
	TextureAsset(U"bg").draw(Vec2(0, offset), color);
	TextureAsset(U"bg").draw(Vec2(0, offset + distance), color);
	TextureAsset(U"bg").draw(Vec2(0, offset - distance), color);

}
