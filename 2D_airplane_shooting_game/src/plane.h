#pragma once
#include <string>
#include <cstdint>
#include <memory>

class Shader;

namespace yk {
	class Plane {
	public:
		
		Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Plane();
		void Init();
		void Paint();
		void MoveX(float x);
		void MoveY(float y);

		std::string img_relative_path_ = "";

		std::string shader_vs_path_ = "";

		std::string shader_fs_path_ = "";

		float width_ = 0.10f;

		float height_ = 0.12f;

		//×ø±êÎ»ÖÃ
		float x_ = 0.0f;

		float y_ = 0.0f;

		float x_offset_ = 0.0f;

		float y_offset_ = 0.0f;

		unsigned int VBO_ = 0, VAO_ = 0, EBO_ = 0;

		unsigned int texture0_ = 0;

		std::shared_ptr<Shader> sharder_program_ = nullptr;

	};
}