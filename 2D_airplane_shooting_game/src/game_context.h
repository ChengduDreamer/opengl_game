#pragma once
#include <vector>
#include <memory>

namespace sf {
	class Music;
}

namespace yk {

	class Object;
	class EnemyPlane;
	class Missile;
	class MainPlane;

	class GameContext {
	public:
		static GameContext* GetInstance() {
			static GameContext self;
			return &self;
		}
		~GameContext();

		void AddEnemyPlaneObject(const std::shared_ptr<EnemyPlane>& obj);
		void AddEnemyMissileObject(const std::shared_ptr<Missile>& obj);
		void AddOurPlaneObject(const std::shared_ptr<MainPlane>& obj);
		void AddOurMissileObject(const std::shared_ptr<Missile>& obj);
		void DrawObjects();
		void EnemyAutoLanuchMissile();
		size_t GetEnemyPlaneObjectSize();
		void GenerateEnemyPlaneObjects();


		void PlayBackgroundMusic();
		void StopPlay();

		void ExecuCheckCollision();
		bool CheckCollision(std::shared_ptr<Object> one, std::shared_ptr<Object> two);

		void OurPlaneLanuchMissile();

		std::shared_ptr<MainPlane> GetFirstMainPlane();
		std::shared_ptr<MainPlane> GetSecondMainPlane();

		//因为有敌我双方，故这里用两个容器来存放 双方的 object
		//enemy
		std::vector<std::shared_ptr<EnemyPlane>> enemy_plane_objects_;
		std::vector<std::shared_ptr<Missile>> enemy_missile_objects_;

		//our
		std::vector<std::shared_ptr<MainPlane>> our_plane_objects_;
		std::vector<std::shared_ptr<Missile>> our_missile_objects_;
	private:
		GameContext();
		
		std::shared_ptr<sf::Music> bk_music_player_ = nullptr;
	};
}