#include "main_plane.h"

namespace yk {

	MainPlane::MainPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
		Plane(img_relative_path, vs_path, fs_path, x, y, width, height) {
		camp_type_ = ECampType::kOur;
	}
	MainPlane::~MainPlane() {
	
	}

}
