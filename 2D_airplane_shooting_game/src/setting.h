#pragma once

#include <filesystem>

namespace yk {
	class Setting {
	public:
		static Setting* GetInstance() {
			static Setting self;
			return &self;
		}
		int fps_ = 60;
		std::filesystem::path resource_base_path_ = "";
	private:
		Setting();
		virtual ~Setting();
	};
}