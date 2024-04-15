#include "game_context.h"

namespace yk {
	GameContext::GameContext() {
	
	}

	GameContext::~GameContext() {
	
	}

	void GameContext::AddEnemyObject(const std::shared_ptr<Object>& obj) {
		enemy_objects_.emplace_back(obj);
	}

	void GameContext::AddOurObject(const std::shared_ptr<Object>& obj) {
		our_objects_.emplace_back(obj);
	}
}