#include "MethodTimer.h"
#include "../ESGLib.h"

using namespace Fujimura;

MethodTimer::MethodTimer() {



}

MethodTimer::~MethodTimer() {



}

void MethodTimer::Initialize() {

	this->methods_.clear();

}

void MethodTimer::Update() {

	this->AddNewMethod();

	int elapsedTime = GameTimer.GetElapsedMilliSecond();

	auto itr = this->methods_.begin();
	while (itr != this->methods_.end()) {

		itr->timeLimit_ -= elapsedTime;

		if (itr->timeLimit_ <= 0) {

			itr->method_(this);

			itr = this->methods_.erase(itr);
			continue;

		}

		itr++;

	}

}

void MethodTimer::EntryMethod(MethodData data) {

	if (this->nextAddMethods_.size() >= 10) {

		 int aa = 10;

	}

	this->nextAddMethods_.push_back(data);

}

void MethodTimer::AddNewMethod() {

	if (this->nextAddMethods_.size() <= 0) return;

	if (this->nextAddMethods_.size() > 10) {

		int aa = 10;

	}

	auto na_itr = this->nextAddMethods_.begin();

	//TODO:‘½•ª‚¨‚©‚µ‚¢@20190215
	while (na_itr != this->nextAddMethods_.end()) {

		this->methods_.push_back((*na_itr));
		na_itr++;

	}

	this->nextAddMethods_.clear();

}