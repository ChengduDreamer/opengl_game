#pragma once

#include <filesystem>
#include <string>

namespace yk {
	class Setting {
	public:
		static Setting* GetInstance() {
			static Setting self;
			return &self;
		}
		std::filesystem::path GetBackgroundMusicPath();
		int max_enemy_plane_count_ = 10;
		int fps_ = 60;
		std::filesystem::path resource_base_path_ = "";

		//Ïà¶ÔÂ·¾¶
		std::filesystem::path background_music_path_ = "media/bk_audio.mp3";
	private:
		Setting();
		virtual ~Setting();
	};
}