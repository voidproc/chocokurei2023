#pragma once

class Pronamachan
{
public:
	Pronamachan();

	void update();

	void draw() const;

	void miss();
	void reset();

private:
	Timer timerAnimate_;
	Stopwatch swMiss_;

	void startNormalAnimation_();
};
