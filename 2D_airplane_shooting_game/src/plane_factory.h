#pragma once
#include <memory>
#include "plane.h"

namespace yk {
	class PlaneFactory {
	public:
		static std::shared_ptr<Plane> CreatePlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
	};

}

