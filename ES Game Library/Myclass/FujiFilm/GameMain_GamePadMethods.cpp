#include "../../GameMain.h"
#include <math.h>

GameMain::AXIS GameMain::GetLeftStickAxis() {

	GamePadState padState = GamePad(0)->GetState();

	if (padState.X == 0 && padState.Y >= 0) return AXIS::NONE;

	enum WHICH {

		NONE,
		HOR,
		VER

	}which = NONE;

	if (labs(padState.X) > labs(padState.Y))which = HOR;
	else which = VER;

	if (which == NONE) return AXIS::NONE;

	switch (which) {

	case HOR:

		//��
		if (padState.X < Axis_Center) {

			return AXIS::LEFT;

		}
		//�E
		else {

			return AXIS::RIGHT;

		}

		break;

	case VER:

		//��
		if (padState.Y < Axis_Center) {

			return AXIS::UP;

		}
		//�E
		else {

			return AXIS::DOWN;

		}

		break;

	}

	return AXIS::NONE;

}