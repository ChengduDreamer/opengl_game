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

	enum class EDirection {
		kS = 0, //��ֹ
		kU = 1,
		kD = 2,
		kL = 4,
		kR = 8,
	};

	class Object {
	public:
		
		Object(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height);
		virtual ~Object();
		void Init();
		virtual void Paint();
		void Move(uint8_t direction_combination);
		void Move(float x, float y);
		void SetDirection(uint8_t direction_combination);
		Position GetCurrentPosition();
		Position GetCurrentHeadPosition(); // ��ȡͷ��λ��

		uint8_t direction_combination_ = static_cast<uint8_t>(EDirection::kS);
		


		std::string img_relative_path_ = "";

		std::string shader_vs_path_ = "";

		std::string shader_fs_path_ = "";

		float width_ = 0.10f;

		float height_ = 0.10f;

		//����λ��
		float x_ = 0.0f;

		float y_ = 0.0f;

		float x_offset_ = 0.0f;

		float y_offset_ = 0.0f;

		//����λ��ʹ��
		glm::mat4 translation_matrix_;

		// speed
		float unit_step_size_ = 0.02f;

		unsigned int VBO_ = 0, VAO_ = 0, EBO_ = 0;

		unsigned int texture0_ = 0;

		std::shared_ptr<Shader> sharder_program_ = nullptr;

	};
}