#include "game_context.h"
#include <iostream>
#include "element/object.h"
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

	void GameContext::DrawObjects() {
		std::cout << "our_objects_ size = " << our_objects_.size() << std::endl;
		for (auto iter = our_objects_.begin(); iter != our_objects_.end(); ) {
			if ((*iter)->destory_) {
				iter = our_objects_.erase(iter);
			}
			else {
				(*iter)->Paint();
				++iter;
			}
		}
	}
}