#pragma once
#include "object.h"

namespace yk {
	class Plane : public Object {
	public:
		Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Plane();
	};
}