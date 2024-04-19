#include "game_context.h"
#include <iostream>
#include "object.h"
#include "enemy_plane.h"
#include "missile.h"
namespace yk {
	GameContext::GameContext() {
	
	}

	GameContext::~GameContext() {
	
	}

	void GameContext::AddEnemyPlaneObject(const std::shared_ptr<yk::EnemyPlane>& obj) {
		enemy_plane_objects_.emplace_back(obj);
		//std::cout << "AddEnemyObjected size = "<< enemy_objects_.size() << std::endl;
	}

	void GameContext::AddEnemyMissileObject(const std::shared_ptr<yk::Missile>& obj) {
		enemy_missile_objects_.emplace_back(obj);
		//std::cout << "AddEnemyObjected size = "<< enemy_objects_.size() << std::endl;
	}

	void GameContext::AddOurObject(const std::shared_ptr<Object>& obj) {
		our_objects_.emplace_back(obj);
	}

	void GameContext::DrawObjects() {
		//std::cout << "our_objects_ size = " << our_objects_.size() << std::endl;
		for (auto iter = our_objects_.begin(); iter != our_objects_.end(); ) {
			if ((*iter)->destory_) {
				iter = our_objects_.erase(iter);
			}
			else {
				(*iter)->Paint();
				++iter;
			}
		}
		for (auto iter = enemy_plane_objects_.begin(); iter != enemy_plane_objects_.end(); ) {
			if ((*iter)->destory_) {
				iter = enemy_plane_objects_.erase(iter);
			}
			else {
				(*iter)->Paint();
				++iter;
			}
		}
		for (auto iter = enemy_missile_objects_.begin(); iter != enemy_missile_objects_.end(); ) {
			if ((*iter)->destory_) {
				iter = enemy_missile_objects_.erase(iter);
			}
			else {
				(*iter)->Paint();
				++iter;
			}
		}
	}

	void GameContext::EnemyAutoLanuchMissile() {
		for (auto& plane : enemy_plane_objects_) {
			plane->JudgeLanuchMissile();
		}
	}

	size_t GameContext::GetEnemyPlaneObjectSize() {
		return  enemy_plane_objects_.size();
	}
}