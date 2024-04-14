#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>
#include "setting.h"
#include "common/time_util.h"
#include "plane_factory.h"
#include "learnopengl/shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::shared_ptr<yk::Plane> plane_ptr);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

int main()
{
    yk::Setting::GetInstance()->resource_base_path_ = "G:/code/yuanqi/opengl_game/2D_airplane_shooting_game/res";

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

    /*std::shared_ptr<yk::Plane> plane_ptr = yk::PlaneFactory::CreatePlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -0.05f, -0.83f, 0.1f, 0.12f);*/


    float plane_width = 0.1f;
    float plane_height = 0.12f;



    std::shared_ptr<yk::Plane> plane_ptr = yk::PlaneFactory::CreatePlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", 0.0f , 0.0f, plane_width, plane_height);

 

    // render loop
    // -----------
    // 这种模式下游戏的帧率是多少呢？？？
    while (!glfwWindowShouldClose(window))
    {
        uint64_t before_time = yk::GetCurrentTimestamp();
        // input
        // -----
        processInput(window, plane_ptr);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        plane_ptr->Paint();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        uint64_t after_time = yk::GetCurrentTimestamp();
        int diff_time = after_time - before_time;
        int per_frame_time = 1000 / yk::Setting::GetInstance()->fps_;
        if (per_frame_time > diff_time) {
            std::this_thread::sleep_for(std::chrono::milliseconds(per_frame_time - diff_time));
        }
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, std::shared_ptr<yk::Plane> plane_ptr)
{
    float unit_step_size = 0.02f;
    float x_offset = 0.0f;
    float y_offset = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y_offset = unit_step_size;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y_offset = -1 * unit_step_size;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x_offset = -1 * unit_step_size;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x_offset = unit_step_size;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // 这里为什么摁空格键一次，反而会触发多次
        std::cout << "launch a guided missile" << std::endl;
    }
    plane_ptr->Move(x_offset, y_offset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}