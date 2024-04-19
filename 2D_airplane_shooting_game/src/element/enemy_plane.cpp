#include "enemy_plane.h"
#include <random>
#include <iostream>
#include "common/time_util.h"
namespace yk {

	EnemyPlane::EnemyPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
		Plane(img_relative_path, vs_path, fs_path, x, y, width, height) {
		can_go_beyond_window_boundaries_ = true;
		camp_type_ = ECampType::kEnemy;
		last_lanuch_time_ = direction_update_time_ = yk::GetCurrentTimestamp();
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> distribution(2000, 4500);
			direction_update_interval_time_ = distribution(gen);
		}
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> distribution(1, 10);
			unit_step_size_ = 0.001f *  distribution(gen);
		}
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> distribution(1000, 3000);
			lanuch_interval_time_ = distribution(gen);
		}
	}

	void EnemyPlane::Paint() {
		auto current_time = yk::GetCurrentTimestamp();
		if (current_time - direction_update_time_ > direction_update_interval_time_) {
			direction_combination_ = static_cast<uint8_t>(yk::EDirection::kD);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> distribution(0, 2);
			int random_number = distribution(gen);
			if (0 == random_number) {
			}
			else if (1 == random_number) {
				direction_combination_ |= static_cast<uint8_t>(yk::EDirection::kL);
			}
			else if (2 == random_number) {
				direction_combination_ |= static_cast<uint8_t>(yk::EDirection::kR);
			}

			auto pos = GetCurrentPosition();

			if (pos.x < -1.0f + width_) {
				direction_combination_ &= (~static_cast<uint8_t>(yk::EDirection::kL));
				direction_combination_ |= static_cast<uint8_t>(yk::EDirection::kR);
			}
			else if (pos.x > 1.0f -width_) {
				direction_combination_ &= (~static_cast<uint8_t>(yk::EDirection::kR));
				direction_combination_ |= static_cast<uint8_t>(yk::EDirection::kL);
			}
			direction_update_time_ = yk::GetCurrentTimestamp();
		}
		Plane::Paint();
	}

	void EnemyPlane::JudgeLanuchMissile() {
		auto current_time = yk::GetCurrentTimestamp();
		if (current_time - last_lanuch_time_ > lanuch_interval_time_) {
			direction_combination_ = static_cast<uint8_t>(yk::EDirection::kD);
			this->LaunchMissile();
			last_lanuch_time_ = yk::GetCurrentTimestamp();
		}
	}

	EnemyPlane::~EnemyPlane() {

	}

}
