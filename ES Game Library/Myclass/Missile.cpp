#include "Missile.h"

using namespace Fujimura;

const std::wstring Missile::MODEL_PATH_ = _T("Missile/Missile.X");
MODEL Missile::model_ = nullptr;
const Vector3 Missile::CENTER_RIVISION_ = Vector3(2.0f,5.0f,5.0f);

std::mt19937 Missile::RANDOM_ = std::mt19937();
const std::uniform_real_distribution<float> Missile::VEZIER_HOR_TICK_DIST_ = std::uniform_real_distribution<float>(-40.0f, 40.0f);
const std::uniform_int_distribution<int>    Missile::VEZIER_HOR_DIST_	   = std::uniform_int_distribution<int>(-10,10);
const std::uniform_int_distribution<int>    Missile::VEZIER_VER_DIST_	   = std::uniform_int_distribution<int>(-5,5);

Missile::Missile() {

	this->pos_ = Vector3_Zero;
	this->quaternion_ = Quaternion_Identity;
	
	for (int i = 0; i < 4;i++) {

		this->vezierPoints_[i] = Vector3_Zero;

	}
	this->t_ = 0.0f;

	this->isUsingFlag_ = false;

}

Missile::~Missile() {



}

void Missile::LoadModel() {

	Missile::model_ = GraphicsDevice.CreateModelFromFile(Missile::MODEL_PATH_.c_str());

	std::random_device seed;
	Missile::RANDOM_ = std::mt19937(seed());

}

void Missile::Initialize(Vector3 startPoint, Vector3 endPoint) {

	this->pos_ = startPoint;

	{//�x�W�G�ݒ�]�[��

		Vector3 dir = endPoint - startPoint;

		float horTickValue = Missile::VEZIER_HOR_TICK_DIST_(Missile::RANDOM_);
		int dx = Missile::VEZIER_HOR_DIST_(Missile::RANDOM_);
		int dy = Missile::VEZIER_VER_DIST_(Missile::RANDOM_);

		this->vezierPoints_[0] = startPoint;

		//���˒n�_�@����@�S�[���@�܂�1/3�̂Ƃ���ɐ���X��ł�����
		this->vezierPoints_[1] = startPoint + (dir * 0.33f);
		//TODO: �����̒��ł����ǂ��ɂ�����
		this->vezierPoints_[1].y += 30.0f* dy;
		this->vezierPoints_[1].x += horTickValue * dx;

		//���˒n�_�@����@�S�[���@��2/3�̂Ƃ���ɐ���X��ł�����
		this->vezierPoints_[2] = startPoint + (dir * 0.66f);
		//TODO: �����̒��ł����ǂ��ɂ�����
		this->vezierPoints_[2].y += 30.0f* dy;
		this->vezierPoints_[2].x += horTickValue * dx;

		this->vezierPoints_[3] = endPoint;

	}//�x�W�G�ݒ�]�[���@�R�R�}�f

	this->isUsingFlag_ = true;

}

void Missile::Finalize() {



}

void Missile::Update() {

	//�e�̑��x����
	this->t_ += 0.005f;

	Vector3 prevPos = this->pos_;

	this->pos_ = Vector3_Bezier(this->vezierPoints_[0],
								this->vezierPoints_[1],
								this->vezierPoints_[2],
								this->vezierPoints_[3], 
								this->t_);

	Matrix  miss_rot;
	miss_rot = Matrix_CreateLookAt(prevPos, this->pos_, Vector3_Up);
	miss_rot = Matrix_Invert(miss_rot);
	this->quaternion_ = Quaternion_CreateFromRotationMatrix(miss_rot);

}

void Missile::Draw() {

	Missile::model_->SetPosition(this->pos_);
	Missile::model_->SetDirection(this->quaternion_);

	Missile::model_->Draw();

}

OrientedBoundingBox Missile::GetOBB() {

	Missile::model_->SetPosition(this->pos_);
	Missile::model_->SetDirection(this->quaternion_);

	OrientedBoundingBox obb = Missile::model_->GetOBB();

	Vector3 centerRivision = Vector3_Zero;
	centerRivision += Missile::model_->GetRightVector() * this->CENTER_RIVISION_.x;
	centerRivision += Missile::model_->GetUpVector()	* this->CENTER_RIVISION_.y;
	centerRivision += Missile::model_->GetFrontVector() * this->CENTER_RIVISION_.z;

	obb.Center += centerRivision;

	return obb;

}