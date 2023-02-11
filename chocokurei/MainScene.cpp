#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene(const InitData& init)
	: IScene{ init }, choco_{}
{
	setNewStage_();
}

void MainScene::update()
{
	if (KeySpace.down())
	{
		setNewStage_();
	}

	for (auto& choco : choco_)
	{
		choco.update();
	}
}

void MainScene::draw() const
{
	Scene::SetBackground(Color(U"#dc678c"));

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

	for (int i : step(row_ * column_))
	{
		const int chocoType = Random(0, ChocolateTypesCount - 1);
		choco_.emplace_back<Chocolate>(chocoType);

		choco_.back().setPos(Vec2(0, Random(-30, -100)));

		const double distance = 16.0;
		const double x = (i % column_) * distance - (distance * (column_ - 1)) / 2;
		const double y = (i / column_) * distance - (distance * (row_ - 1)) / 2;
		choco_.back().moveTo(Vec2(x, y) + Scene::Center(), 0.8 + Random(-0.2, 0.2));
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

	const Array<String> condMsg = {
		U"すきなチョコ",
		U"ハート型のチョコ",
		U"まるいチョコ",
		U"四角いチョコ",
		U"茶色のチョコ",
		U"ホワイトチョコ",
		U"一番多いの",
		U"一番少ないの",
	};

	const Array<String> endMsg = {
		U"を取っていいよ",
		U"を食べていいよ",
		U"をあげる",
		U"をどうぞ",
		U"を取ってね",
		U"を食べてね",
	};

	setBalloonText_(condMsg[static_cast<int>(condition_)] + endMsg.choice());
}

void MainScene::setBalloonText_(StringView text)
{
	balloonText_ = text;
	swBalloonTextAnimate_.restart();
}

void MainScene::drawPronamachan_() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	TextureAsset(U"pronama-chan").draw(basePos);
}

void MainScene::drawBalloon_() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	TextureAsset(U"balloon").draw(basePos);

	StringView balloonText = balloonText_.substrView(0, (int)(32 * swBalloonTextAnimate_.sF()));
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12), Palette::Black);
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
