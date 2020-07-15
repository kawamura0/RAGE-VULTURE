#include "PlayerAnimation.h"

using namespace FujiFilm;

PlayerAnimation::PlayerAnimation() {

	this->track_ = -1;
	this->time_ = 0.0f;
	this->animetionLength_ = 0.0f;
	this->speed_ = 0.0f;
	this->model_ = nullptr;
	this->updatingMethod_ = nullptr;
	this->completedMethod_ = nullptr;
	this->isPlaying_ = false;

}

PlayerAnimation::~PlayerAnimation() {



}

void PlayerAnimation::Initialize() {

	this->track_ = -1;
	this->time_ = 0.0f;
	this->animetionLength_ = 0.0f;
	this->speed_ = 0.0f;
	this->updatingMethod_ = nullptr;

	int count = this->model_->GetNumTracks();
	for (int i = 0; i < count; i++) {

		this->model_->SetTrackPosition(i, 0.0f);
		this->model_->SetTrackEnable(i, false);

	}

	this->isPlaying_ = false;

}

void PlayerAnimation::Update() {

	if (!this->isPlaying()) return;

	float felapsedTime = (float)GameTimer.GetElapsedMilliSecond() / 1000.0f;
	this->time_ += felapsedTime * this->speed_;

	if (this->updatingMethod_ != nullptr) this->updatingMethod_(this->speed_,this->time_,this->animetionLength_);

	if (this->time_ >= this->animetionLength_) {

		this->Stop();

		if (this->completedMethod_ != nullptr) {

			this->completedMethod_();

		}

		return;

	}

	if(this->track_ >= 0) this->model_->SetTrackPosition(this->track_,this->time_);

}

void PlayerAnimation::Play(int animationTrack,  float animationSpeed, float endTime, std::function<void(float,float,float)> updatingMethod) {

	if ((int)this->model_->GetNumTracks() < animationTrack + 1) return;
	if (this->isPlaying()) this->Stop();

	if (animationTrack >= 0) {

		this->model_->SetTrackEnable(animationTrack, true);
		this->model_->SetTrackPosition(animationTrack, 0.0f);

	}

	this->track_ = animationTrack;
	this->speed_ = animationSpeed;
	this->animetionLength_ = endTime;
	this->updatingMethod_ = updatingMethod;
	this->isPlaying_ = true;

}

void PlayerAnimation::Stop() {

	if (this->track_ >= 0) {

		this->model_->SetTrackEnable(this->track_, false);
		this->model_->SetTrackPosition(this->track_, 0.0f);

	}

	this->Initialize();

}