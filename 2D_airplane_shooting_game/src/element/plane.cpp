#include "plane.h"

namespace yk {
	Plane::Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) : 
	Object(img_relative_path, vs_path, fs_path, x, y, width, height) {
	
	}
	Plane::~Plane() {
	
	}
}