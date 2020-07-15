#pragma once
#include "../ESGLib.h"

class Ggauge final{

public:

	enum ERROR_CODE {

		SUCCESS,
		UN_GRAPHIC_LOAD, //グラフィックよめ
		UN_INITIALIZE    //初期化しろ

	};

	Ggauge(Vector3 pos = Vector3_Zero);
	~Ggauge();

	static void LoadGraphic();
	static void ReleaseGraphic();

	ERROR_CODE Initialize();
	ERROR_CODE DrawGauge(float gaugeRate);

	void SetPosition(Vector3 pos) { this->pos_ = pos; }

private:

	static SPRITE gaugeInSp_;
	static SPRITE gaugeOutSp_;
	static const std::wstring GAUGE_IN_PATH_;
	static const std::wstring GAUGE_OUT_PATH_;

	Vector3 pos_;

	RENDERTARGET inGaugeBackBuffer_;

};