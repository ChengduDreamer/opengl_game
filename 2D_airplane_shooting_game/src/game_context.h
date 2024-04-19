#pragma once
#include <vector>
#include <memory>

namespace yk {

	class Object;
	class EnemyPlane;
	class Missile;

	class GameContext {
	public:
		static GameContext* GetInstance() {
			static GameContext self;
			return &self;
		}
		~GameContext();

		void AddEnemyPlaneObject(const std::shared_ptr<EnemyPlane>& obj);
		void AddEnemyMissileObject(const std::shared_ptr<Missile>& obj);
		void AddOurObject(const std::shared_ptr<Object>& obj);
		void DrawObjects();
		void EnemyAutoLanuchMissile();
		size_t GetEnemyPlaneObjectSize();
		//因为有敌我双方，故这里用两个容器来存放 双方的 object
		//enemy
		std::vector<std::shared_ptr<EnemyPlane>> enemy_plane_objects_;
		std::vector<std::shared_ptr<Missile>> enemy_missile_objects_;

		//our
		std::vector<std::shared_ptr<Object>> our_objects_;
	private:
		GameContext();
		
	};
}