#include "plane.h"
#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "element_factory.h"
#include "game_context.h"
#include "setting.h"
#include "common/time_util.h"
namespace yk {
	Plane::Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
	Object(img_relative_path, vs_path, fs_path, x, y, width, height) {
		EElementType element_type_ = EElementType::kPlane;
	}

	void Plane::Paint() {
		if (explode_) {
			Explode();
		}
		Object::Paint();
	}

	Plane::~Plane() {
	
	}
	void Plane::LaunchMissile() {
		yk::Position plane_pos = this->GetCurrentHeadPosition();
		float missile_width = 0.017f;
		float missile_height = 0.035f;
		std::shared_ptr<yk::Missile> missile_ptr = yk::ElementFactory::CreateMissile("image/missile_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", plane_pos.x - missile_width / 2, 
			camp_type_ == ECampType::kOur ? plane_pos.y + missile_height : plane_pos.y, missile_width, missile_height);

		switch (camp_type_)
		{
		case yk::ECampType::kOur:
			missile_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kU));
			yk::GameContext::GetInstance()->AddOurMissileObject(missile_ptr);
			break;
		case yk::ECampType::kEnemy:
			missile_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kD));
			yk::GameContext::GetInstance()->AddEnemyMissileObject(missile_ptr);
			break;
		}
	}

	void Plane::SetExplodeImages(const std::vector<std::string> images_path) {
		explode_images_ = images_path;
		explod_interval = explod_duration / explode_images_.size();
	}

	void Plane::Explode() {
		auto current_time = yk::GetCurrentTimestamp();
		if (current_time - last_update_explode_image_time_ < explod_interval) {
			return;
		}
		last_update_explode_image_time_ = current_time;
		if (current_explode_index_ >= explode_images_.size()) {
			destory_ = true;
			return;
		}
		glDeleteTextures(1, &texture0_);
		glGenTextures(1, &texture0_);
		glBindTexture(GL_TEXTURE_2D, texture0_);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

		std::filesystem::path hero_img_path = Setting::GetInstance()->resource_base_path_ / explode_images_[current_explode_index_];
		std::string hero_img_path_str = hero_img_path.string();
		unsigned char* data = stbi_load(hero_img_path_str.c_str(), &width, &height, &nrChannels, 0);
		//std::cout << "nrChannels = " << nrChannels << std::endl;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		
		++current_explode_index_;
	}
}