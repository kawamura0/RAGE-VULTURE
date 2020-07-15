#pragma once
#include <functional>
#include <list>

namespace Fujimura {

	class MethodTimer;

	struct MethodData {
		
		//ƒ~ƒŠ’PˆÊ‚Ë
		int timeLimit_;
		std::function<void(MethodTimer*)> method_;

		MethodData(int timeLimit, std::function<void(MethodTimer*)> method) {

			this->timeLimit_ = timeLimit;
			this->method_ = method;

		}

	};

#define SEE_CLASS_MEMBERS [this]
#define COPY_OUT_VARIABLE [=]
#define SEE_OUT_VARIABLE  [&]

#define NEED_ARGUMENT (MethodTimer* methodTimer)

	class MethodTimer final{

	public:

		static MethodTimer& GetInstance() {

			static MethodTimer instance;
			return instance;

		}

		~MethodTimer();

		void Initialize();
		void Update();

		void EntryMethod(MethodData data);

	private:

		void AddNewMethod();

		MethodTimer();

		std::list<MethodData> methods_;
		std::list<MethodData> nextAddMethods_;

	};

}
#define METHOD_TIMER MethodTimer::GetInstance()