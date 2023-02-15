#pragma once

#include "Chocolate.h"
#include "Condition.h"

class ChocolateBox
{
public:
	ChocolateBox(int row  = 3, int column = 3);

	void update();

	void draw() const;

	int row() const;

	int column() const;

	void resize(int row, int column);

	void fill();

	bool fullfill(const Chocolate& target, Condition condition) const;

	bool anyFullfill(Condition condition) const;

	void enableMouseAction(bool enable);

	bool isMouseActionEnabled() const;

	Optional<Chocolate> chocolateMouseOver();
	const Optional<Chocolate> chocolateMouseOver() const;

	Optional<Chocolate> chocolateClicked();
	const Optional<Chocolate> chocolateClicked() const;

private:
	int row_;
	int column_;
	Array<Chocolate> choco_;
	Optional<Chocolate> chocoMouseOver_;
	Optional<Chocolate> chocoClicked_;
	Stopwatch swWaitMouseAction_;

	void checkChocoMouseState_();

};
