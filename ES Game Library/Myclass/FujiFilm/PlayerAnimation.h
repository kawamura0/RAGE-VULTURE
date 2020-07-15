#pragma once
#include "../../ESGLib.h"
#include <functional>

namespace FujiFilm {

	class PlayerAnimation final {

	public:

		PlayerAnimation();
		~PlayerAnimation();

		void Initialize();
		void Update();
		//0.016 = 1/60
		//animationTrack　に　-1を指定で　再生しません。
		void Play(int animationTrack,float animationSpeed = 1.0f, float animetionLength = 1.0f,std::function<void(float,float,float)> updatingMethod = nullptr);
		void Stop();

		void SetModel(ANIMATIONMODEL playerModel) { this->model_ = playerModel; }
		void SetCompletedMethod(std::function<void()> completedMethod) { this->completedMethod_ = completedMethod; }

		bool isPlaying() { return this->isPlaying_; }
		int GetNowPlayingTrack() { return this->track_; }

	private:

		bool isPlaying_;
		int track_;
		float time_;
		float animetionLength_;
		float speed_;

		std::function<void(float speed,float nowTime,float animeMaxTime)> updatingMethod_;
		std::function<void()> completedMethod_;;

		ANIMATIONMODEL model_;

	};

}