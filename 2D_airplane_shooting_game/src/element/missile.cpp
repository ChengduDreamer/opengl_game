#include "missile.h"


namespace yk {
	Missile::Missile(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
		Object(img_relative_path, vs_path, fs_path, x, y, width, height) {
		EElementType element_type_ = EElementType::kMissile;
		//modify missile speed
		unit_step_size_ = 0.03f;
		can_go_beyond_window_boundaries_ = true;
	}
	Missile::~Missile() {

	}

	void Missile::Paint() {
		Object::Paint();
	}
}