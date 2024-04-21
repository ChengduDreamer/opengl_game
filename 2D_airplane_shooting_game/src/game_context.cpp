#include "game_context.h"
#include <iostream>
#include <random>
#include <SFML/Audio.hpp>
#include "object.h"
#include "enemy_plane.h"
#include "missile.h"
#include "setting.h"
#include "element_factory.h"
namespace yk {
	GameContext::GameContext() {
		bk_music_player_ = std::make_shared<sf::Music>();
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

	void GameContext::GenerateEnemyPlaneObjects() {
		if (enemy_plane_objects_.size() < Setting::GetInstance()->max_enemy_plane_count_) {
			float yr = 1.0f;
			float xr = 1.0f;
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> distribution(10, 18);
				yr = distribution(gen);
			}
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> distribution(10, 30);
				xr = distribution(gen);
			}
	
			static yk::Position birth_pos;
			birth_pos.y = 0.78f * (yr / 10);
			if (birth_pos.x > 0.88f) {
				birth_pos.x = -0.88f;
			}
			float plane_width = 0.1f;
			float plane_height = 0.12f;
			std::shared_ptr<yk::EnemyPlane> plane_ptr = yk::ElementFactory::CreateEnemyPlane("image/enemy.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", birth_pos.x, birth_pos.y, plane_width, plane_height);
			plane_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kD));
			yk::GameContext::GetInstance()->AddEnemyPlaneObject(plane_ptr);
			birth_pos.x += (plane_width * (xr / 10));
		}
	}

	void GameContext::PlayBackgroundMusic() {
		auto bk_music_path = Setting::GetInstance()->GetBackgroundMusicPath();
		std::string bk_music_path_str = bk_music_path.string();
		// ������Ƶ�ļ�
		if (!bk_music_player_->openFromFile(bk_music_path_str)) {
			std::cerr << "load bk music error" << std::endl;
			return; // ����ʧ��
		}
		// ����ѭ������
		bk_music_player_->setLoop(true);
		// ��������
		bk_music_player_->play();
	}

	void GameContext::StopPlay() {
		bk_music_player_->stop();
	}
}