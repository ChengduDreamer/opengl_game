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
		//��Ϊ�е���˫������������������������� ˫���� object
		//enemy
		std::vector<std::shared_ptr<EnemyPlane>> enemy_plane_objects_;
		std::vector<std::shared_ptr<Missile>> enemy_missile_objects_;

		//our
		std::vector<std::shared_ptr<Object>> our_objects_;
	private:
		GameContext();
		
	};
}