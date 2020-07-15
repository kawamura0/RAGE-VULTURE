#include "TransGauge.h"

using namespace FujiFilm;

//外枠のパスとサイズ
const std::wstring TransGauge::GAUGE_OUTLINE_PATH_ = _T("UI/waku_trans.png");
const Vector2	   TransGauge::GAUGE_OUTLINE_SIZE_ = Vector2(324.0f,42.0f);
SPRITE             TransGauge::outLineSprite_ = nullptr;
//中身のパスとサイズ
const std::wstring TransGauge::GAUGE_INNER_PATH_ = _T("UI/trans.png");
const Vector2	   TransGauge::GAUGE_INNER_SIZE_ = Vector2(201.0f, 24.0f);
SPRITE             TransGauge::innerSprite_ = nullptr;

//スコアいくつごとにレベルアップ？
const int TransGauge::LEVEL_UP_NORM_ = 3;
//最大レベル　－　１　の値にすること ex)最大レベルが５ならここは4
const int TransGauge::MAX_LEVEL_ = 2;

//一番上がレベル０
const std::vector<Color> TransGauge::LEVEL_COLOR_MASKS_ = {

	Color(0,128,255),
	Color(255,255,255),
	Color(255,0,0)

};

TransGauge::TransGauge() {

	this->score_ = 0;

}

TransGauge::~TransGauge() {



}

void TransGauge::LoadResource() {

	outLineSprite_ = GraphicsDevice.CreateSpriteFromFile(TransGauge::GAUGE_OUTLINE_PATH_.c_str());
	innerSprite_ = GraphicsDevice.CreateSpriteFromFile(TransGauge::GAUGE_INNER_PATH_.c_str());

}

void TransGauge::Initialize() {

	this->score_ = 0;

}

void TransGauge::Update() {



}

void TransGauge::Draw(Vector3 drawPos) {

	SpriteBatch.Draw(*this->outLineSprite_,drawPos,1.0f);

	//補正用のそれ　ほんとは数字直書きはよくないんだけど、
	//めんどくさいので現状はこれで
	Vector3 rivision = Vector3(102.0f,8.0f,0.0f);

	int copyScore = this->score_;

	for (int i = 0; i < this->MAX_LEVEL_;i++) {
		Color color;
		if (i > this->LEVEL_COLOR_MASKS_.size()) {
			color = this->LEVEL_COLOR_MASKS_.back();
		}
		else {
			color = this->LEVEL_COLOR_MASKS_[i];
		}

		int hatena = (copyScore > this->LEVEL_UP_NORM_) ? this->LEVEL_UP_NORM_ : copyScore;
		int right = (int)(this->GAUGE_INNER_SIZE_.x * ( (float)hatena / (float)this->LEVEL_UP_NORM_) );
		RectWH rect = RectWH(0,0,right,this->GAUGE_INNER_SIZE_.y);

		SpriteBatch.Draw(*this->innerSprite_, drawPos + rivision,rect,color);

		copyScore -= this->LEVEL_UP_NORM_;
		if (copyScore <= 0) break;

	}

}