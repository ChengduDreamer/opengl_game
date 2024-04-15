#pragma once
#include <vector>
#include <memory>

namespace yk {

	class Object;

	class GameContext {
	public:
		static GameContext* GetInstance() {
			static GameContext self;
			return &self;
		}
		~GameContext();

		void AddEnemyObject(const std::shared_ptr<Object>& obj);
		void AddOurObject(const std::shared_ptr<Object>& obj);

		//因为有敌我双方，故这里用两个容器来存放 双方的 object
		//enemy
		std::vector<std::shared_ptr<Object>> enemy_objects_;

		//our
		std::vector<std::shared_ptr<Object>> our_objects_;
	private:
		GameContext();
		
	};
}