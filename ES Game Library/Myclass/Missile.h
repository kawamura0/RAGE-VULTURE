#pragma once
#include "../ESGLib.h"
#include <random>

//�~�T�C���܂Ƃ߂�N��������O��Ƃ���
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
		//obb�̒��S�␳�p
		static const Vector3	  CENTER_RIVISION_;
		static std::mt19937 RANDOM_;
		//���ɂ������݂ň��邩
		static const std::uniform_real_distribution<float> VEZIER_HOR_TICK_DIST_;
		//���ɂǂꂭ�炢���邩
		static const std::uniform_int_distribution<int> VEZIER_HOR_DIST_;
		//�c�ɂǂꂭ�炢���邩
		static const std::uniform_int_distribution<int> VEZIER_VER_DIST_;

		Vector3 pos_;
		D3DXQUATERNION quaternion_;

		//�x�W�G�_�i�[�p�̔z��Ȃ̂łS�Œ�
		Vector3 vezierPoints_[4];
		float t_;

		bool isUsingFlag_;

	};

}