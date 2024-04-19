#pragma once
#include <memory>
#include "plane.h"
#include "missile.h"
#include "main_plane.h"
#include "enemy_plane.h"

namespace yk {
	class ElementFactory {
	public:
		static std::shared_ptr<Plane> CreatePlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);

		static std::shared_ptr<Missile> CreateMissile(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);

		static std::shared_ptr<MainPlane> CreateMainPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);

		static std::shared_ptr<EnemyPlane> CreateEnemyPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
	};



}

