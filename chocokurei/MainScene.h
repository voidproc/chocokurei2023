#pragma once

#include "Chocolate.h"
#include "Condition.h"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
	Stopwatch swBalloonTextAnimate_;
	String balloonText_;
	Array<Chocolate> choco_;
	int level_ = 1;
	int levelAdjust_ = 0;
	int column_ = 3;
	int row_ = 3;
	Condition condition_;
	Stopwatch swWaitMouseAction_;
	Array<RectF> chocoArea_;
	Optional<RectF> chocoAreaMouseOver_;
	Optional<Chocolate*> chocoMouseOver_;
	Optional<Chocolate*> chocoClicked_;
	Stopwatch swNextStage_;
	Timer timerPronamachanAnimate_;
	Stopwatch swAnswer_;
	int miss_ = 0;

	void setNewStage_();
	void setBalloonText_(StringView text);
	void drawPronamachan_() const;
	void drawBalloon_() const;
	void drawBg_() const;
	bool isFullFilledCondition_(const Chocolate& target, Condition cond);
	bool isMouseActionEnabled_() const;
	void checkChocoAreaMouseOver_();
};
