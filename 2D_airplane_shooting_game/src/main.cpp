#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include "learnopengl/shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader* shader_ptr);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

std::filesystem::path g_resource_base_path = "G:/code/yuanqi/opengl_game/2D_airplane_shooting_game/res";

float g_pox_x_offset = 0.0f;
float g_pox_y_offset = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    std::string hero_vs_relaive_path_str = "shader/hero_b_1.vs";
    std::filesystem::path hero_vs_path = g_resource_base_path / hero_vs_relaive_path_str;
    std::string hero_vs_path_str = hero_vs_path.string();

    std::string hero_fs_relaive_path_str = "shader/hero_b_1.fs";
    std::filesystem::path hero_fs_path = g_resource_base_path / hero_fs_relaive_path_str;
    std::string hero_fs_path_str = hero_fs_path.string();
    

    Shader ourShader(hero_vs_path_str.c_str(), hero_fs_path_str.c_str());

   
    // 学习到这里，其实整个窗口 的坐标 就是 -1到1之间， 好像还没有用到后面学到的坐标系统， 这个等后面前后集合起来再看 ???
    float vertices[] = {
        // positions          // texture coords
         0.05f,  -0.83f, 0.0f,   1.0f, 1.0f, // top right
         0.05f, -0.95f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.05f, -0.95f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.05f,  -0.83f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //如果不启用以下两行代码，png图片透明的部分，就是黑色底
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

    std::string hero_img_relaive_path_str = "image/hero_b_1.png";
    std::filesystem::path hero_img_path = g_resource_base_path / hero_img_relaive_path_str;
    std::string hero_img_path_str = hero_img_path.string();
    unsigned char* data = stbi_load(hero_img_path_str.c_str(), &width, &height, &nrChannels, 0);
    std::cout << "nrChannels = " << nrChannels << std::endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
   
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture0"), 0);

    // render loop
    // -----------
    // 这种模式下游戏的帧率是多少呢？？？
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, &ourShader);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render container
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Shader* shader_ptr)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_pox_y_offset += 0.01f;
        if (g_pox_y_offset >= 1.8f) {
            g_pox_y_offset = 1.8f;
        }
        shader_ptr->setFloat("posYOffset", g_pox_y_offset);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_pox_y_offset -= 0.01f;
        if (g_pox_y_offset <= -0.02f) {
            g_pox_y_offset = -0.02f;
        }
        shader_ptr->setFloat("posYOffset", g_pox_y_offset);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_pox_x_offset -= 0.01f;
        if (g_pox_x_offset <= -0.95f) {
            g_pox_x_offset = -0.95f;
        }
        shader_ptr->setFloat("posXOffset", g_pox_x_offset);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_pox_x_offset += 0.01f;
        if (g_pox_x_offset >= 0.95f) {
            g_pox_x_offset = 0.95f;
        }
        shader_ptr->setFloat("posXOffset", g_pox_x_offset);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}