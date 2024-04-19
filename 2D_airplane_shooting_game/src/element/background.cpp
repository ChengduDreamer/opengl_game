#include "background.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include "learnopengl/shader_s.h"
#include "setting.h"
#include "const_def.h"


namespace yk {

    Background::Background(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) {
        img_relative_path_ = img_relative_path;
        shader_vs_path_ = (Setting::GetInstance()->resource_base_path_ / vs_path).string();
        shader_fs_path_ = (Setting::GetInstance()->resource_base_path_ / fs_path).string();
        sharder_program_ = std::make_shared<Shader>(shader_vs_path_.c_str(), shader_fs_path_.c_str());
        Init();
    }

    Background::~Background()
    {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
        glDeleteTextures(1, &texture0_); //如果不删除, 会导致内存泄露
    }

    void Background::Init() {
        float vertices[] = {
            // positions             // texture coords
            1.00f,  1.00f,   0.0f,   1.0f, 1.0f, // top right
            1.00f, -1.00f,   0.0f,   1.0f, 0.0f, // bottom right
           -1.00f, -1.00f,   0.0f,   0.0f, 0.0f, // bottom left
           -1.00f,  1.00f,   0.0f,   0.0f, 1.0f  // top left 
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

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
        unsigned int model_loc = glGetUniformLocation(sharder_program_->ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(translation_matrix_));
    }

    void Background::Paint() {
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0_);
        // render container
        sharder_program_->use();
        glBindVertexArray(VAO_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

   
   
}
