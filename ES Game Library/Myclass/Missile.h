#pragma once
#include "../ESGLib.h"
#include <random>

//ミサイルまとめる誰かがいる前提とする
namespace Fujimura {

	class Missile final {

	public:

		Missile();
		~Missile();

		static void LoadModel();

		void Initialize(Vector3 startPoint,Vector3 endPoint);
		void Finalize();
		void Update();
		void Draw();

		bool IsUsing() { return this->isUsingFlag_; }
		void Used() { this->isUsingFlag_ = false; }

		OrientedBoundingBox GetOBB();

		void SetVezierEndPos(Vector3 pos) {

			this->vezierPoints_[3] = pos;

		}

	private:

		static const std::wstring MODEL_PATH_;
		static MODEL model_;
		//obbの中心補正用
		static const Vector3	  CENTER_RIVISION_;
		static std::mt19937 RANDOM_;
		//横にいくつ刻みで逸れるか
		static const std::uniform_real_distribution<float> VEZIER_HOR_TICK_DIST_;
		//横にどれくらい逸れるか
		static const std::uniform_int_distribution<int> VEZIER_HOR_DIST_;
		//縦にどれくらい逸れるか
		static const std::uniform_int_distribution<int> VEZIER_VER_DIST_;

		Vector3 pos_;
		D3DXQUATERNION quaternion_;

		//ベジエ点格納用の配列なので４固定
		Vector3 vezierPoints_[4];
		float t_;

		bool isUsingFlag_;

	};

}