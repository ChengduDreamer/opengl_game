#pragma once
#include <vector>
#include <memory>

struct GLFWwindow;

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
		void AddOurFirstPlaneObject(const std::shared_ptr<MainPlane>& obj);
		void AddOurSecondPlaneObject(const std::shared_ptr<MainPlane>& obj);

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

		void ProcessInput(GLFWwindow* window);

		//因为有敌我双方，故这里用两个容器来存放 双方的 object
		//enemy
		std::vector<std::shared_ptr<EnemyPlane>> enemy_plane_objects_;
		std::vector<std::shared_ptr<Missile>> enemy_missile_objects_;

		//our
		std::vector<std::shared_ptr<Missile>> our_missile_objects_;

		std::shared_ptr<MainPlane> first_plane_objects_;

		std::shared_ptr<MainPlane> second_plane_objects_;
	private:
		GameContext();
		
		std::shared_ptr<sf::Music> bk_music_player_ = nullptr;

		uint64_t first_plane_last_launch_missile_time_ = 0;

		uint64_t second_plane_last_launch_missile_time_ = 0;

		bool first_plane_launch_missile_ = false;

		bool second_plane_launch_missile_ = false;
	};
}