#include "stdafx.h"
#include "ChocolateBox.h"
#include "Constants.h"

ChocolateBox::ChocolateBox(int row, int column)
	: row_(row), column_(column)
{
	fill();
}

void ChocolateBox::update()
{
	for (auto& choco : choco_)
	{
		choco.update();
	}

	checkChocoMouseState_();
}

void ChocolateBox::draw() const
{
	// 箱
	TextureAsset(U"box-{}x{}"_fmt(row_, column_)).drawAt(Scene::Center());

	// チョコ
	for (const auto& choco : choco_)
	{
		choco.draw();
	}
}

int ChocolateBox::row() const
{
	return row_;
}

int ChocolateBox::column() const
{
	return column_;
}

void ChocolateBox::resize(int row, int column)
{
	row_ = row;
	column_ = column;

	if (not choco_.isEmpty())
	{
		choco_.resize(row * column);
	}
}

void ChocolateBox::fill()
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

bool ChocolateBox::fullfill(const Chocolate& target, Condition condition) const
{
	switch (condition)
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

		if (condition == Condition::IsMostAmount)
		{
			return typeCount[target.type()] == *std::max_element(typeCount.begin(), typeCount.end());
		}
		else if (condition == Condition::IsLeastAmount)
		{
			return typeCount[target.type()] == *std::min_element(typeCount.begin(), typeCount.end());
		}
		return false;
	}

	return false;
}

bool ChocolateBox::anyFullfill(Condition condition) const
{
	for (const auto& choco : choco_)
	{
		if (fullfill(choco, condition))
		{
			return true;
		}
	}

	return false;
}

void ChocolateBox::enableMouseAction(bool enable)
{
	if (enable)
	{
		swWaitMouseAction_.restart();
		chocoClicked_ = none;
	}
	else
	{
		swWaitMouseAction_.pause();
	}
}

void ChocolateBox::checkChocoMouseState_()
{
	chocoMouseOver_ = none;

	if (!isMouseActionEnabled())
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

bool ChocolateBox::isMouseActionEnabled() const
{
	return swWaitMouseAction_.isRunning() && swWaitMouseAction_.sF() > 1.1;
}

Optional<Chocolate> ChocolateBox::chocolateMouseOver()
{
	return chocoMouseOver_;
}

const Optional<Chocolate> ChocolateBox::chocolateMouseOver() const
{
	return chocoMouseOver_;
}

Optional<Chocolate> ChocolateBox::chocolateClicked()
{
	return chocoClicked_;
}

const Optional<Chocolate> ChocolateBox::chocolateClicked() const
{
	return chocoClicked_;
}
