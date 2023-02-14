#pragma once

class Balloon
{
public:
	Balloon();

	void update();

	void draw() const;

	void setText(StringView text);

private:
	String text_;
	Stopwatch swTextAnimate_;
};
