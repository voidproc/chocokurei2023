#include "stdafx.h"
#include "Balloon.h"

Balloon::Balloon()
{
}

void Balloon::update()
{
}

void Balloon::draw() const
{
	const Vec2 basePos = Scene::Rect().bl().movedBy(0, -32);

	TextureAsset(U"balloon").draw(basePos);

	StringView balloonText = text_.substrView(0, (int)(32 * swTextAnimate_.sF()));
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12 + 1), ColorF(0, 0.1));
	FontAsset(U"main")(balloonText).draw(basePos.movedBy(42, 12), Color(U"#421d12"));
}

void Balloon::setText(StringView text)
{
	text_ = text;
	swTextAnimate_.restart();
}
