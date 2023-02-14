#pragma once

#include "Chocolate.h"
#include "Condition.h"
#include "Pronamachan.h"
#include "Balloon.h"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);

	void update() override;

	void draw() const override;

private:
	Stopwatch swScene_{ StartImmediately::Yes };
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
	Stopwatch swAnswer_;
	int miss_ = 0;
	Pronamachan pronamachan_;
	Balloon balloon_;

	void setNewStage_();
	Condition randomCondition_() const;
	bool isFullFilledCondition_(const Chocolate& target, Condition cond);
	bool isMouseActionEnabled_() const;
	void checkChocoAreaMouseOver_();

	void drawBg_() const;
};
