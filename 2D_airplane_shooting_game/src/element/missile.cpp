#include "missile.h"


namespace yk {
	Missile::Missile(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
		Object(img_relative_path, vs_path, fs_path, x, y, width, height) {
		//modify missile speed
		unit_step_size_ = 0.006f;
	}
	Missile::~Missile() {

	}

	void Missile::Paint() {
		Move(static_cast<uint8_t>(yk::EDirection::kU));
		Object::Paint();
	}
}