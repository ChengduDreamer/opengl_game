#pragma once
#include <vector>
#include <string>
#include "object.h"

namespace sf {
	class Music;
}

namespace yk {
	class Plane : public Object {
	public:
		Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Plane();
		virtual void LaunchMissile();
		virtual void Paint() override;
		virtual void SetExplodeImages(const std::vector<std::string> images_path);
		virtual void Explode();
		
	private:
		std::vector<std::string> explode_images_;
		int current_explode_index_ = 0;
		float explod_duration = 1500; // ms
		float explod_interval = 10;
		uint64_t last_update_explode_image_time_ = 0;

		std::shared_ptr<sf::Music> explode_music_player_ = nullptr;
	};
}