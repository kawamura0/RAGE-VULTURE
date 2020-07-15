#pragma once
#include <vector>
#include "../ESGLib.h"

namespace Fujimura {

	class Missile;

	class MissileManager final{

	public:

		static MissileManager& GetInstance() {

			static MissileManager instance;
			return instance;

		}

		~MissileManager();

		void Initialize();
		void Finalize();
		void Update();
		void Draw();

		void InstanceMissile(Vector3 startPos,Vector3 endPos);
		void RemoveMissile(Missile* missile);

	private:

		MissileManager();

		std::vector<Missile*> missiles_;
		std::vector<Missile*> instancedMissiles_;

	};

#define MISSILE_MANAGER MissileManager::GetInstance()

}