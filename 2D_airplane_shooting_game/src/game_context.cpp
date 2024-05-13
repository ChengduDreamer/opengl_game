#include "game_context.h"
#include <iostream>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SFML/Audio.hpp>
#include "object.h"
#include "enemy_plane.h"
#include "missile.h"
#include "setting.h"
#include "element_factory.h"
#include "joystick.h"
#include "common/time_util.h"
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

	void GameContext::AddOurFirstPlaneObject(const std::shared_ptr<MainPlane>& obj) {
		first_plane_objects_ = obj;
	}

	void GameContext::AddOurSecondPlaneObject(const std::shared_ptr<MainPlane>& obj) {
		second_plane_objects_ = obj;
	}

	void GameContext::AddOurMissileObject(const std::shared_ptr<Missile>& obj) {
		our_missile_objects_.emplace_back(obj);
	}

	void GameContext::DrawObjects() {
		if (first_plane_objects_) {
			if (first_plane_objects_->destory_) {
				first_plane_objects_ = nullptr;
			}
			else {
				first_plane_objects_->Paint();
			}
		}

		if (second_plane_objects_) {
			if (second_plane_objects_->destory_) {
				second_plane_objects_ = nullptr;
			}
			else {
				second_plane_objects_->Paint();
			}
		}

		for (auto iter = our_missile_objects_.begin(); iter != our_missile_objects_.end(); ) {
			if ((*iter)->destory_) {
				iter = our_missile_objects_.erase(iter);
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
			if (!plane->explode_) {
				plane->JudgeLanuchMissile();
			}
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

			plane_ptr->SetExplodeImages({
				"image/explode/enemy/explode01.png",
				"image/explode/enemy/explode02.png",
				"image/explode/enemy/explode03.png",
				"image/explode/enemy/explode04.png"
			});

			yk::GameContext::GetInstance()->AddEnemyPlaneObject(plane_ptr);
			birth_pos.x += (plane_width * (xr / 10));
		}
	}

	void GameContext::PlayBackgroundMusic() {
		auto bk_music_path = Setting::GetInstance()->GetBackgroundMusicPath();
		std::string bk_music_path_str = bk_music_path.string();
		// 加载音频文件
		if (!bk_music_player_->openFromFile(bk_music_path_str)) {
			std::cerr << "load bk music error" << std::endl;
			return; // 加载失败
		}
		// 设置循环播放
		bk_music_player_->setLoop(true);
		// 播放音乐
		bk_music_player_->play();
	}

	void GameContext::StopPlay() {
		bk_music_player_->stop();
	}

	bool GameContext::CheckCollision(std::shared_ptr<Object> one, std::shared_ptr<Object> two) {
		// x轴方向碰撞？
		auto one_pos = one->GetCurrentPosition();
		auto two_pos = two->GetCurrentPosition();
		bool collision_x = one_pos.x + one->width_ >= two_pos.x && two_pos.x + two->width_ >= one_pos.x;
		// y轴方向碰撞？
		bool collision_y = one_pos.y + one->height_ >= two_pos.y && two_pos.y + two->height_ >= one_pos.y;
		// 只有两个轴向都有碰撞时才碰撞
		return collision_x && collision_y;
	}

	void GameContext::ExecuCheckCollision() {
		if(first_plane_objects_) {
			for (auto enemy_plane_iter = enemy_plane_objects_.begin(); enemy_plane_iter != enemy_plane_objects_.end(); ++enemy_plane_iter) {
				if (first_plane_objects_->explode_ || (*enemy_plane_iter)->explode_) {
					continue;
				}
				if (CheckCollision(first_plane_objects_, *enemy_plane_iter)) {
					first_plane_objects_->explode_ = true;
					(*enemy_plane_iter)->explode_ = true;
				}
			}

			for (auto enemy_missile_iter = enemy_missile_objects_.begin(); enemy_missile_iter != enemy_missile_objects_.end(); ++enemy_missile_iter) {
				if (CheckCollision(first_plane_objects_, *enemy_missile_iter)) {
					first_plane_objects_->explode_ = true;
					(*enemy_missile_iter)->destory_ = true;
				}
			}
		}

		if (second_plane_objects_) {
			for (auto enemy_plane_iter = enemy_plane_objects_.begin(); enemy_plane_iter != enemy_plane_objects_.end(); ++enemy_plane_iter) {
				if (second_plane_objects_->explode_ || (*enemy_plane_iter)->explode_) {
					continue;
				}
				if (CheckCollision(second_plane_objects_, *enemy_plane_iter)) {
					second_plane_objects_->explode_ = true;
					(*enemy_plane_iter)->explode_ = true;
				}
			}

			for (auto enemy_missile_iter = enemy_missile_objects_.begin(); enemy_missile_iter != enemy_missile_objects_.end(); ++enemy_missile_iter) {
				if (CheckCollision(second_plane_objects_, *enemy_missile_iter)) {
					second_plane_objects_->explode_ = true;
					(*enemy_missile_iter)->destory_ = true;
				}
			}
		}

		for (auto our_missile_iter = our_missile_objects_.begin(); our_missile_iter != our_missile_objects_.end(); ++our_missile_iter) {
			for (auto enemy_plane_iter = enemy_plane_objects_.begin(); enemy_plane_iter != enemy_plane_objects_.end(); ++enemy_plane_iter) {
				if (CheckCollision(*our_missile_iter, *enemy_plane_iter)) {
					(*our_missile_iter)->destory_ = true;
					(*enemy_plane_iter)->explode_ = true;
				}
			}
		}
	}

	void GameContext::OurPlaneLanuchMissile() {
		if (!first_plane_launch_missile_ && !second_plane_launch_missile_) { 
			return; 
		}
		auto first_plane = GetFirstMainPlane();
		if (first_plane && first_plane_launch_missile_ && !first_plane->explode_) {
			first_plane->LaunchMissile();
			first_plane_launch_missile_ = false;
		}

		auto second_plane = GetSecondMainPlane();
		if (second_plane && second_plane_launch_missile_ && !second_plane->explode_) {
			second_plane->LaunchMissile();
			second_plane_launch_missile_ = false;
		}
	}

	std::shared_ptr<MainPlane> GameContext::GetFirstMainPlane() {
		return first_plane_objects_;
	}


	std::shared_ptr<MainPlane> GameContext::GetSecondMainPlane() {
		return second_plane_objects_;
	}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
	void GameContext::ProcessInput(GLFWwindow* window) {
		{
			uint8_t first_plane_direction_combination = static_cast<uint8_t>(yk::EDirection::kS);

			uint8_t second_plane_direction_combination = static_cast<uint8_t>(yk::EDirection::kS);

			// keyboard control
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kU);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kD);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kL);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kR);
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				auto current_time = yk::GetCurrentTimestamp();
				if (current_time - first_plane_last_launch_missile_time_ > 60) {
					first_plane_launch_missile_ = true;
					first_plane_last_launch_missile_time_ = current_time;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				auto first_plane = yk::GameContext::GetInstance()->GetFirstMainPlane();
				if (!first_plane) {
					float plane_width = 0.1f;
					float plane_height = 0.12f;
					std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 - plane_width * 3, -1.0f + plane_height, plane_width, plane_height);
					plane_ptr->SetExplodeImages({
						"image/explode/hero/explode01.png",
						"image/explode/hero/explode02.png",
						"image/explode/hero/explode03.png",
						"image/explode/hero/explode04.png",
						"image/explode/hero/explode05.png",
						"image/explode/hero/explode06.png"
						});

					yk::GameContext::GetInstance()->AddOurFirstPlaneObject(plane_ptr);
				}
			}

			//  joystick control
			if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE) {
				const char* first_joystick_name = glfwGetJoystickName(GLFW_JOYSTICK_1);
				//printf("first_joystick_name = %s\n", first_joystick_name);
				// 获取手柄的按钮状态
				int count = 0;
				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
				if (buttons != nullptr && count > 0)
				{
#if 0
					for (size_t i = 0; i < count; ++i)
					{
						printf("i = %d ,", (int)i);
						std::cout << std::hex << std::setw(2) << std::setfill('0')
							<< static_cast<int>(buttons[i]) << ";";
					}
					std::cout << std::endl;
#endif
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionUp)]) {
						first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kU);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kkDirectionDown)]) {
						first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kD);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionLeft)]) {
						first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kL);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionRight)]) {
						first_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kR);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kA)]) {
						auto current_time = yk::GetCurrentTimestamp();
						if (current_time - first_plane_last_launch_missile_time_ > 60) {
							first_plane_launch_missile_ = true;
							first_plane_last_launch_missile_time_ = current_time;
						}
					}

					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kY)]) {
						auto first_plane = yk::GameContext::GetInstance()->GetFirstMainPlane();
						if (!first_plane) {
							float plane_width = 0.1f;
							float plane_height = 0.12f;
							std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 - plane_width * 3, -1.0f + plane_height, plane_width, plane_height);
							plane_ptr->SetExplodeImages({
								"image/explode/hero/explode01.png",
								"image/explode/hero/explode02.png",
								"image/explode/hero/explode03.png",
								"image/explode/hero/explode04.png",
								"image/explode/hero/explode05.png",
								"image/explode/hero/explode06.png"
								});

							yk::GameContext::GetInstance()->AddOurFirstPlaneObject(plane_ptr);
							
						}
					}
				}
			}

			// keyboard control
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kU);
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kD);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kL);
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kR);
			}
			if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS) {
				auto current_time = yk::GetCurrentTimestamp();
				if (current_time - second_plane_last_launch_missile_time_ > 60) {
					second_plane_launch_missile_ = true;
					second_plane_last_launch_missile_time_ = current_time;
				}
			}

			if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS) {
				auto second_plane = yk::GameContext::GetInstance()->GetSecondMainPlane();
				if (!second_plane) {
					float plane_width = 0.1f;
					float plane_height = 0.12f;
					std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_2.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 + plane_width * 3, -1.0f + plane_height, plane_width, plane_height);
					plane_ptr->SetExplodeImages({
						"image/explode/hero/explode01.png",
						"image/explode/hero/explode02.png",
						"image/explode/hero/explode03.png",
						"image/explode/hero/explode04.png",
						"image/explode/hero/explode05.png",
						"image/explode/hero/explode06.png"
						});
					yk::GameContext::GetInstance()->AddOurSecondPlaneObject(plane_ptr);
				}
			}

			if (glfwJoystickPresent(GLFW_JOYSTICK_2) == GLFW_TRUE) {
				const char* second_joystick_name = glfwGetJoystickName(GLFW_JOYSTICK_2);
				printf("second_joystick_name = %s\n", second_joystick_name);
				// 获取手柄的按钮状态
				int count = 0;
				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &count);
				if (buttons != nullptr && count > 0)
				{
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionUp)]) {
						second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kU);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kkDirectionDown)]) {
						second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kD);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionLeft)]) {
						second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kL);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kDirectionRight)]) {
						second_plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kR);
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kA)]) {
						auto current_time = yk::GetCurrentTimestamp();
						if (current_time - second_plane_last_launch_missile_time_ > 60) {
							second_plane_launch_missile_ = true;
							second_plane_last_launch_missile_time_ = current_time;
						}
					}
					if (GLFW_PRESS == buttons[static_cast<int>(Joystick::EKey::kY)]) {
						auto second_plane = yk::GameContext::GetInstance()->GetSecondMainPlane();
						if (!second_plane) {
							float plane_width = 0.1f;
							float plane_height = 0.12f;
							std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_2.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 + plane_width * 3, -1.0f + plane_height, plane_width, plane_height);
							plane_ptr->SetExplodeImages({
								"image/explode/hero/explode01.png",
								"image/explode/hero/explode02.png",
								"image/explode/hero/explode03.png",
								"image/explode/hero/explode04.png",
								"image/explode/hero/explode05.png",
								"image/explode/hero/explode06.png"
								});
							yk::GameContext::GetInstance()->AddOurSecondPlaneObject(plane_ptr);
						}
					}
				}
			}

			auto first_plane = yk::GameContext::GetInstance()->GetFirstMainPlane();
			if (first_plane) {
				first_plane->SetDirection(first_plane_direction_combination);
			}

			auto second_plane = yk::GameContext::GetInstance()->GetSecondMainPlane();
			if (second_plane) {
				second_plane->SetDirection(second_plane_direction_combination);
			}
		}
	}
}