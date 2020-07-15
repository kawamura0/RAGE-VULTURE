#include "MissileManager.h"
#include "Missile.h"

using namespace Fujimura;

MissileManager::MissileManager() {



}

MissileManager::~MissileManager() {

	this->Finalize();

}

void MissileManager::Initialize() {

	this->Finalize();

}

void MissileManager::Finalize() {

	this->missiles_.clear();

	for (auto data : this->instancedMissiles_) {

		delete data;

	}

	this->instancedMissiles_.clear();

}

void MissileManager::Update() {

	for(auto missile : this->missiles_){
	
		missile->Update();
	
	}

}

void MissileManager::Draw() {

	for (auto missile : this->missiles_) {

		missile->Draw();

	}

}

void MissileManager::InstanceMissile(Vector3 startPos, Vector3 endPos) {

	for (auto itr = this->instancedMissiles_.begin(); itr != this->instancedMissiles_.end(); itr++) {

		if (!(*itr)->IsUsing()) {

			(*itr)->Initialize(startPos,endPos);
			this->missiles_.push_back((*itr));

			return;

		}

	}

	//ここまで来たときは未使用のインスタンスがなかった

	Missile* instance = new Missile();
	instance->Initialize(startPos,endPos);

	this->instancedMissiles_.push_back(instance);
	this->missiles_.push_back(instance);

}

void MissileManager::RemoveMissile(Missile* missile) {

	auto itr = std::find(this->missiles_.begin(),this->missiles_.end(),missile);

	//そもそも入ってないわい
	if (itr == this->missiles_.end()) return;

	(*itr)->Used();

	this->missiles_.erase(itr);

}