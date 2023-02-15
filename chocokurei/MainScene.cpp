#include "stdafx.h"
#include "MainScene.h"
#include "Constants.h"

MainScene::MainScene(const InitData& init)
	: IScene{ init }
{
	setNewStage_();
}

void MainScene::update()
{
	pronamachan_.update();

	chocolateBox_.update();

	// 回答残り時間

	if (swAnswer_.isRunning() && swAnswer_.sF() > (4.0 + 1.1))
	{
		balloon_.setText(TimeUpText.choice());
		levelAdjust_ = -1;

		// 回答タイマーストップ
		swAnswer_.pause();

		// マウス操作ストップ
		chocolateBox_.enableMouseAction(false);
		swWaitMouseAction_.pause();

		// すこし待機した後、次のステージへ
		swNextStage_.restart();
	}

	// マウス操作

	if (swWaitMouseAction_.isRunning() && swWaitMouseAction_.sF() > 1.1)
	{
		if (not chocolateBox_.isMouseActionEnabled())
		{
			chocolateBox_.enableMouseAction(true);
		}
	}

	if (chocolateBox_.isMouseActionEnabled())
	{
		// チョコがクリックされた

		if (auto chocoClicked = chocolateBox_.chocolateClicked(); chocoClicked)
		{
			(*chocoClicked)->take();

			if (chocolateBox_.fullfill(**chocoClicked, condition_))
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
			chocolateBox_.enableMouseAction(false);
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

	chocolateBox_.draw();

	// 回答残り時間

	if (swAnswer_.sF() > 1.1)
	{
		const double barWidth = Clamp(108 * (1 - (swAnswer_.sF() - 1.1) / 4.0), 0.01, 108.0);
		RectF{ Scene::Center().movedBy(-108 / 2, 46), SizeF(108, 4) }.draw(Palette::Black);
		RectF{ Scene::Center().movedBy(-108 / 2, 46), SizeF(barWidth, 4) }.draw(Palette::White);
	}

	// マウス位置

	if (chocolateBox_.isMouseActionEnabled())
	{
		if (auto chocoMouseOver = chocolateBox_.chocolateMouseOver(); chocoMouseOver)
		{
			const ColorF areaColor = Sample({ Palette::Yellow, Palette::Cyan, Palette::Pink });
			(*chocoMouseOver)->collision().drawFrame(0.0, 1.0, ColorF(areaColor, Periodic::Square0_1(100ms)));
		}
	}

	// チョコを取った時の判定文字

	if (auto chocoClicked = chocolateBox_.chocolateClicked();
		swNextStage_.isRunning() && swNextStage_.sF() < 1.5 && chocoClicked)
	{
		ColorF color = Palette::White;
		if (swNextStage_.sF() > 1.0) {
			color = ColorF{ 1.0, Periodic::Square0_1(0.08ms) };
		}
		const double t = Clamp(swNextStage_.sF() / 0.30, 0.0, 1.0);
		Vec2 pos = (*chocoClicked)->pos() - Vec2(0, 5.0 * EaseOutQuint(t));
		TextureAsset(levelAdjust_ == 1 ? U"good" : U"miss").drawAt(pos.asPoint(), color);
	}
}

void MainScene::setNewStage_()
{
	level_ = Max(level_ + levelAdjust_, 1);
	levelAdjust_ = 0;

	if (level_ > 16)
	{
		chocolateBox_.resize(4, 6);
	}
	else if (level_ > 12)
	{
		chocolateBox_.resize(4, 5);
	}
	else if (level_ > 8)
	{
		chocolateBox_.resize(4, 4);
	}
	else if (level_ > 4)
	{
		chocolateBox_.resize(3, 4);
	}
	else
	{
		chocolateBox_.resize(3, 3);
	}

	// チョコ生成
	chocolateBox_.fill();

	// 条件を選ぶ
	// 選んでも大丈夫な条件が出るまで待つ

	bool isValidCondition = false;
	while (!isValidCondition)
	{
		condition_ = randomCondition_();

		if (chocolateBox_.anyFullfill(condition_))
		{
			isValidCondition = true;
			break;
		}
	}

	balloon_.setText(ConditionText[static_cast<int>(condition_)] + ConditionTextEnd.choice());

	// 少し待機した後、マウス操作を有効にする
	swWaitMouseAction_.restart();

	// 回答タイマー
	swAnswer_.restart();

	// プロ生ちゃんの顔をリセット
	pronamachan_.reset();
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
