#pragma once

#include <filesystem>

namespace yk {
	class Setting {
	public:
		static Setting* GetInstance() {
			static Setting self;
			return &self;
		}
		int max_enemy_plane_count_ = 14;
		int fps_ = 60;
		std::filesystem::path resource_base_path_ = "";
	private:
		Setting();
		virtual ~Setting();
	};
}