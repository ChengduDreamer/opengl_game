#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include <tuple>
//矩阵相关头文件
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "common/common.h"

class Shader;

namespace yk {
	class Background {
	public:
		Background(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Background();
		// top right
		// bottom right
		// bottom left
		// top left 
		bool first = false;
		void Init(Position top_right, Position bottom_right, Position bottom_left, Position top_left);
		virtual void Paint();

		std::string img_relative_path_ = "";

		std::string shader_vs_path_ = "";

		std::string shader_fs_path_ = "";

		//操作位移使用
		glm::mat4 translation_matrix_;

		// speed
		float unit_step_size_ = 0.01f;

		unsigned int VBO_ = 0, VAO_ = 0, EBO_ = 0;

		unsigned int texture0_ = 0;

		std::shared_ptr<Shader> sharder_program_ = nullptr;

		float x_ = 0.0f;
		float y_ = 0.0f;

		void Move(float x_offset, float y_offset);
	};
}