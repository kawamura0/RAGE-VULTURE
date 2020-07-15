#pragma once
#include <string>
#include <functional>

namespace Fujimura {

	class MethodTimer;

	class ReadFireTimeMap final {

	public:

		ReadFireTimeMap() {};
		~ReadFireTimeMap() {};

		static void Reset() { enableCount = 0; }
		void Import(std::wstring filePath);

		static std::function<void(MethodTimer*)> missileInstanceMethod_;

	private:

		static int enableCount;

	};

}