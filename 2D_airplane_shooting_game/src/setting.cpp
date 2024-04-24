#include "setting.h"
#include <iostream>
namespace yk {
	Setting::Setting() {
	
	}

	Setting::~Setting() {
	
	}

	std::filesystem::path Setting::GetBackgroundMusicPath() {
		std::cout << "resource_base_path_ = " << resource_base_path_.string() << std::endl;
		auto ms_path = resource_base_path_ / background_music_path_;
		std::cout << "ms_path = " << ms_path.string() << std::endl;
		return ms_path;
	}

	std::filesystem::path Setting::GetExplodeMusicPath() {
		auto ms_path = resource_base_path_ / explode_music_path_;
		return ms_path;
	}
}