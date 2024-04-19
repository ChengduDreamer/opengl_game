#include "element_factory.h"

namespace yk {

	std::shared_ptr<Plane> ElementFactory::CreatePlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
		return  std::make_shared<Plane>(img_relative_path, vs_path, fs_path, x, y, width, height);
	}

	std::shared_ptr<Missile> ElementFactory::CreateMissile(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
		return  std::make_shared<Missile>(img_relative_path, vs_path, fs_path, x, y, width, height);
	}

	std::shared_ptr<MainPlane> ElementFactory::CreateMainPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
		return  std::make_shared<MainPlane>(img_relative_path, vs_path, fs_path, x, y, width, height);
	}


	std::shared_ptr<EnemyPlane> ElementFactory::CreateEnemyPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
		return  std::make_shared<EnemyPlane>(img_relative_path, vs_path, fs_path, x, y, width, height);
	}

}