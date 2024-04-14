#include "plane_factory.h"

namespace yk {

	std::shared_ptr<Plane> PlaneFactory::CreatePlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
		return  std::make_shared<Plane>(img_relative_path, vs_path, fs_path, x, y, width, height);
	}
}