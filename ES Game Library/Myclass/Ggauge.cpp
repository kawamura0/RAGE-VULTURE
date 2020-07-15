#include "Ggauge.h"

SPRITE Ggauge::gaugeInSp_ = nullptr;
SPRITE Ggauge::gaugeOutSp_ = nullptr;
const std::wstring Ggauge::GAUGE_IN_PATH_  = _T("UI/armor_in_1205_trim.png");
const std::wstring Ggauge::GAUGE_OUT_PATH_ = _T("UI/armor_1205_trim.png");

void Ggauge::LoadGraphic() {

	Ggauge::gaugeInSp_  = GraphicsDevice.CreateSpriteFromFile(Ggauge::GAUGE_IN_PATH_.c_str());
	Ggauge::gaugeOutSp_ = GraphicsDevice.CreateSpriteFromFile(Ggauge::GAUGE_OUT_PATH_.c_str());

}

void Ggauge::ReleaseGraphic() {

	GraphicsDevice.ReleaseSprite(Ggauge::gaugeInSp_ );
	GraphicsDevice.ReleaseSprite(Ggauge::gaugeOutSp_);

}

Ggauge::Ggauge(Vector3 pos) {

	this->inGaugeBackBuffer_ = nullptr;
	this->pos_ = pos;

}

Ggauge::~Ggauge() {

	//if (this->inGaugeBackBuffer_ != nullptr) {

	//	GraphicsDevice.ReleaseRenderTarget(this->inGaugeBackBuffer_);

	//}

}

Ggauge::ERROR_CODE Ggauge::Initialize() {

	if (Ggauge::gaugeInSp_ == nullptr || Ggauge::gaugeOutSp_ == nullptr) {

		return UN_GRAPHIC_LOAD;

	}

	this->inGaugeBackBuffer_ = GraphicsDevice.CreateRenderTarget(242,173,PixelFormat_RGBA8888,DepthFormat_Unknown);

	GraphicsDevice.SetRenderTarget(this->inGaugeBackBuffer_);
	GraphicsDevice.Clear(Color(0.0f,0.0f,0.0f,0.0f));
	GraphicsDevice.SetDefaultRenderTarget();

	return SUCCESS;

}

Ggauge::ERROR_CODE Ggauge::DrawGauge(float gaugeRate) {

	if (Ggauge::gaugeInSp_ == nullptr || Ggauge::gaugeOutSp_ == nullptr) {

		return UN_GRAPHIC_LOAD;

	}

	if (this->inGaugeBackBuffer_ == nullptr) {

		return UN_INITIALIZE;

	}

	if (gaugeRate > 1.0f) gaugeRate = 1.0f;
	if (gaugeRate < 0.0f) gaugeRate = 0.0f;

	//中身作成
	GraphicsDevice.SetRenderTarget(this->inGaugeBackBuffer_);
	GraphicsDevice.Clear(Color(0.0f, 0.0f, 0.0f, 0.0f));

	SpriteBatch.Begin();
	SpriteBatch.Draw(*Ggauge::gaugeInSp_,Vector3_Zero);
	SpriteBatch.End();

	GraphicsDevice.ClearZBuffer();

	//切り落とし開始
	HDC hdc = this->inGaugeBackBuffer_->GetDC();

	//黒塗りの設定
	Paint paint;
	paint.SetLineColor(Color(0, 0, 0, 0)); //枠線の色
	paint.SetPaintColor(Color(0, 0, 0, 0)); //内部の色

	Canvas canvas(hdc);

	//右上長方形っぽいのきりおとし
	float rectRate = (gaugeRate - 0.75f) / 0.25f;
	if (gaugeRate <= 0.75f) rectRate = 0.0f;

	int startX = 242 / 2;
	startX += (int)((float)startX * rectRate);

	canvas.DrawRect(Rect(startX,0,242,173 / 2),paint);

	//残りの円形ゲージの切り落とし
	float circleRate = gaugeRate / 0.75f;
	if (gaugeRate >= 0.75f) circleRate = 1.0f;

	//扇描画が時計回りにしかできないので、
	//始点を移動させる形で反時計周りの切り落としを行う

	//始点
	float startAngle = 270.0f * circleRate;
	//切り落とす角度
	float sweepAngle = 270.0f - startAngle;

	if ( (int)sweepAngle > 0) {
	
		canvas.DrawArc(RectWH(0, 0, 242, 173), startAngle,sweepAngle,true,paint);

	}

	//********ここまで来たら完成*********//
	
	this->inGaugeBackBuffer_->ReleaseDC();

	//あとは描画
	GraphicsDevice.SetDefaultRenderTarget();

	SpriteBatch.Begin();
	SpriteBatch.Draw(*Ggauge::gaugeOutSp_, this->pos_);
	SpriteBatch.Draw(*this->inGaugeBackBuffer_, this->pos_);
	SpriteBatch.End();

	return SUCCESS;

}