#include "ReadFireTimeMap.h"
#include "MethodTimer.h"
#include "MissileManager.h"

#include <random>
#include <fstream>

using namespace Fujimura;

int ReadFireTimeMap::enableCount = 0;

std::function<void(MethodTimer*)> ReadFireTimeMap::missileInstanceMethod_ = nullptr;

void ReadFireTimeMap::Import(std::wstring filePath) {

	//if (enableCount > 0) return;
	this->enableCount++;

	if (missileInstanceMethod_ == nullptr) {

		return;

	}

	//::OutputDebugString(_T("Ç´ÇΩÇÊÅ`\n"));
	StreamReader file;
	file.Open(filePath.c_str());

	//TODO:getlineïsãÔçáÅ@20190215
	TCHAR buffer[256] = {};
	while (true) {

		if (file.IsEOF()) break;

		file.ReadLine(buffer);

		int time = 0;
		int num = 0;

		_stscanf_s(buffer,_T("%d %d"), &time,&num);

		for (int i = 0; i < num;i++) {

			auto method = missileInstanceMethod_;
			int instantTime = time + i * 200;

			METHOD_TIMER.EntryMethod(MethodData(instantTime,method));
			
		}

	}

}