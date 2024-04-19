#pragma once
#include <string>

#include "plane.h"


namespace yk {
	class MainPlane : public Plane{
	public:
		MainPlane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		~MainPlane();
		std::string img_name;

	};

}