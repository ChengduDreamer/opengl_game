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
		std::filesystem::path GetExplodeMusicPath();
		int max_enemy_plane_count_ = 4;
		int fps_ = 60;
		std::filesystem::path resource_base_path_ = "";

		//Ïà¶ÔÂ·¾¶
		std::filesystem::path background_music_path_ = "media/bk_audio.wav";

		std::filesystem::path explode_music_path_ = "media/explode.wav";
	private:
		Setting();
		virtual ~Setting();
	};
}