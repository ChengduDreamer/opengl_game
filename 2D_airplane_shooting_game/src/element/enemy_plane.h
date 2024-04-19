#pragma once
#include <string>
#include "plane.h"


namespace yk {
	class EnemyPlane : public Plane {
	public:
		EnemyPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		~EnemyPlane();
		virtual void Paint() override;

		void JudgeLanuchMissile();

		uint64_t direction_update_time_ = 0;
		uint64_t direction_update_interval_time_ = 3000;

		uint64_t last_lanuch_time_ = 0;
		uint64_t lanuch_interval_time_ = 1500;
	};

}