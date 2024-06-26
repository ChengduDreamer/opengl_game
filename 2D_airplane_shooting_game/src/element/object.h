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

	enum class EDirection {
		kS = 0, //静止
		kU = 1,
		kD = 2,
		kL = 4,
		kR = 8,
	};

	enum class EElementType {
		kPlane,
		kMissile,
	};

	enum class ECampType {
		kOur,
		kEnemy,
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
		Position GetCurrentHeadPosition(); // 获取头部位置
		bool GoBeyondWindowBoundaries();
		bool can_go_beyond_window_boundaries_ = false; //是否可以超出窗口边界
		bool destory_ = false;

		uint8_t direction_combination_ = static_cast<uint8_t>(EDirection::kS);

		EElementType element_type_ = EElementType::kPlane;

		ECampType camp_type_ = ECampType::kOur;
		
		std::string img_relative_path_ = "";

		std::string shader_vs_path_ = "";

		std::string shader_fs_path_ = "";

		float width_ = 0.10f;

		float height_ = 0.10f;

		//坐标位置
		float x_ = 0.0f;

		float y_ = 0.0f;

		float x_offset_ = 0.0f;

		float y_offset_ = 0.0f;

		//操作位移使用
		glm::mat4 translation_matrix_;

		// speed
		float unit_step_size_ = 0.01f;

		unsigned int VBO_ = 0, VAO_ = 0, EBO_ = 0;

		unsigned int texture0_ = 0;

		std::shared_ptr<Shader> sharder_program_ = nullptr;

		bool explode_ = false;
	};
}