#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include <tuple>
//�������ͷ�ļ�
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
		void Init();
		virtual void Paint();

		std::string img_relative_path_ = "";

		std::string shader_vs_path_ = "";

		std::string shader_fs_path_ = "";

		//����λ��ʹ��
		glm::mat4 translation_matrix_;

		// speed
		float unit_step_size_ = 0.01f;

		unsigned int VBO_ = 0, VAO_ = 0, EBO_ = 0;

		unsigned int texture0_ = 0;

		std::shared_ptr<Shader> sharder_program_ = nullptr;

	};
}