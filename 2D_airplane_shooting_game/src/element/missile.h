#pragma once
#include "object.h"

namespace yk {
	class Missile : public Object {
	public:
		Missile(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Missile();
		virtual void Paint() override;
	};
}