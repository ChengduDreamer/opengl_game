#include "plane.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include "learnopengl/shader_s.h"
#include "setting.h"
#include "const_def.h"


namespace yk {

    Object::Object(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height)	{
		img_relative_path_ = img_relative_path;
		shader_vs_path_ = (Setting::GetInstance()->resource_base_path_ / vs_path).string();
		shader_fs_path_ = (Setting::GetInstance()->resource_base_path_ / fs_path).string();
		x_ = x;
		y_ = y;
		width_ = width;
		height_ = height;
        sharder_program_ = std::make_shared<Shader>(shader_vs_path_.c_str(), shader_fs_path_.c_str());
		Init();
	}

	Object::~Object()
	{
        //std::cout << "~------------------~Object" << std::endl;
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
        glDeleteTextures(1, &texture0_); //如果不删除, 会导致内存泄露
	}

	void Object::Init() {
		float vertices[] = {
			// positions                                 // texture coords
            0.00f + width_,  0.00f,             0.0f,   1.0f, 1.0f, // top right
			0.00f + width_,  0.00f - height_,   0.0f,   1.0f, 0.0f, // bottom right
			0.00f,           0.00f - height_,   0.0f,   0.0f, 0.0f, // bottom left
            0.00f,           0.00f,             0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

        glm::vec3 position = glm::vec3(x_, y_, 0.0f);
            
        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);

        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        // load and create a texture 
        glGenTextures(1, &texture0_);
        glBindTexture(GL_TEXTURE_2D, texture0_);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //如果不启用以下两行代码，png图片透明的部分，就是黑色底
#if 1
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

        std::filesystem::path hero_img_path = Setting::GetInstance()->resource_base_path_ / img_relative_path_;
        std::string hero_img_path_str = hero_img_path.string();
        unsigned char* data = stbi_load(hero_img_path_str.c_str(), &width, &height, &nrChannels, 0);
        std::cout << "nrChannels = " << nrChannels << std::endl;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        sharder_program_->use(); // don't forget to activate/use the shader before setting uniforms!
        // either set it manually like so:
        glUniform1i(glGetUniformLocation(sharder_program_->ID, "texture0"), 0);

        translation_matrix_ = glm::mat4(1.0f);
        translation_matrix_ = glm::translate(translation_matrix_, position);

        unsigned int model_loc = glGetUniformLocation(sharder_program_->ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(translation_matrix_));
        HAVE_QUESTIONS("其实这里还是处于标准化设备坐标系？ 怎么理解当前的坐标模式与 后面所学的 坐标系统")
	}

    void Object::Paint() {
        Move(direction_combination_);
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0_);
        // render container
        sharder_program_->use();
        glBindVertexArray(VAO_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (GoBeyondWindowBoundaries()) {
            destory_ = true;
        }
    }

    void Object::Move(uint8_t direction_combination) {
        float x_offset = 0.0f;
        float y_offset = 0.0f;
        if (direction_combination & static_cast<uint8_t>(yk::EDirection::kU)) {
            y_offset = unit_step_size_;
        }

        if (direction_combination & static_cast<uint8_t>(yk::EDirection::kD)) {
            y_offset = -1 * unit_step_size_;
        }

        if (direction_combination & static_cast<uint8_t>(yk::EDirection::kL)) {
            x_offset = -1 * unit_step_size_;
        }
        
        if (direction_combination & static_cast<uint8_t>(yk::EDirection::kR)) {
            x_offset = unit_step_size_;
        }
        Move(x_offset, y_offset);
    }

    void Object::Move(float x_offset, float y_offset) {
        glm::vec3 offset = glm::vec3(x_offset, y_offset, 0.0f);
        GLfloat position[4] = {0.0f, };
        //禁止越界(超越窗口)
        HAVE_QUESTIONS("这里是怎么判断越界的，等后面再研究下");
        if (!can_go_beyond_window_boundaries_) {
            glm::vec3 object_position = glm::vec3(translation_matrix_ * glm::vec4(x_offset, y_offset, 0.0f, 1.0f));
            //std::cout << "object_position x = " << object_position.x << " object_position y = " << object_position.y << std::endl;
            if (object_position.x < -1.0f || object_position.x >(1.0f - width_)) {
                offset.x = 0;
            }
            if (object_position.y < (-1.0 + height_) || object_position.y > 1.0f) {
                offset.y = 0;
            }
        }
        sharder_program_->use();
        translation_matrix_ = glm::translate(translation_matrix_, offset);
        unsigned int model_loc = glGetUniformLocation(sharder_program_->ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(translation_matrix_));
    }

    void Object::SetDirection(uint8_t direction_combination) {
        direction_combination_ = direction_combination;
    }

    HAVE_QUESTIONS("获取位置这个地方的逻辑，等后面再研究下");
    Position Object::GetCurrentPosition() {
        glm::vec3 object_position = glm::vec3(translation_matrix_ * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        return { object_position.x, object_position.y};
    }

    Position Object::GetCurrentHeadPosition() {
        auto pos = GetCurrentPosition();
        pos.x += (width_ / 2);
        if (direction_combination_ & static_cast<uint8_t>(yk::EDirection::kU)) {
            pos.y = pos.y + 0.0f;
        }
        else {
            pos.y = pos.y - height_;
        }
        return pos;
    }

    bool Object::GoBeyondWindowBoundaries() {
        auto current_pos = GetCurrentPosition();
        if (current_pos.x < (-1.0f - width_)) {
            return true;
        }
        if (current_pos.x > 1.0f) {
            return true;
        }
        if (current_pos.y < -1.0f) {
            return true;
        }
        if (current_pos.y > (1.0f + height_)) {
            return true;
        }
        return false;
    }
}
