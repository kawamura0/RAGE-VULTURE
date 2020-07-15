#pragma once
#include "../../ESGLib.h"

namespace FujiFilm {

	class TransGauge final {

	public:

		TransGauge();
		~TransGauge();

		static void LoadResource();

		void Initialize();

		void Update();
		
		void Draw(Vector3 drawPos);

		int GetNowLevel()   { return this->score_ / TransGauge::LEVEL_UP_NORM_;}
		
		int GetMaxLevel()   { return TransGauge::MAX_LEVEL_; }

		int GetNowScore()   { return this->score_; }
		void SetScore(int score) {
			
			if (score > (TransGauge::LEVEL_UP_NORM_ * TransGauge::MAX_LEVEL_)) {

				score = TransGauge::LEVEL_UP_NORM_ * TransGauge::MAX_LEVEL_;

			}
			
			this->score_ = score; 
		}
		int GetLevelUpNorm() { return TransGauge::LEVEL_UP_NORM_; }

	private:

		static const std::wstring GAUGE_OUTLINE_PATH_;
		static const Vector2 GAUGE_OUTLINE_SIZE_;
		static const std::wstring GAUGE_INNER_PATH_;
		static const Vector2 GAUGE_INNER_SIZE_;
		static SPRITE outLineSprite_;
		static SPRITE innerSprite_;

		static const int LEVEL_UP_NORM_;
		static const int MAX_LEVEL_;

		static const std::vector<Color> LEVEL_COLOR_MASKS_;

		int score_;
		


	};

}