#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>
#include "setting.h"
#include "common/time_util.h"
#include "element/element_factory.h"
#include "learnopengl/shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::shared_ptr<yk::Plane> plane_ptr);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

std::vector<std::shared_ptr<yk::Object>> g_object_vector;

bool g_launch_missile = false;

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
    std::shared_ptr<yk::Plane> plane_ptr = yk::ElementFactory::CreatePlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2, -1.0f + plane_height, plane_width, plane_height);

 


    



    //g_object_vector.emplace_back(missile_ptr);

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

        if (g_launch_missile) {
            g_launch_missile = false;

#if 0
            static std::once_flag flag;
            std::call_once(flag, []() {
                float missile_width = 0.017f;
                float missile_height = 0.035f;
                std::shared_ptr<yk::Missile> missile_ptr = yk::ElementFactory::CreateMissile("image/missile_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", 0.0f, 0.0f, missile_width, missile_height);
                g_object_vector.emplace_back(missile_ptr);
            });
#endif

            float missile_width = 0.017f;
            float missile_height = 0.035f;
            std::shared_ptr<yk::Missile> missile_ptr = yk::ElementFactory::CreateMissile("image/missile_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", 0.0f, 0.0f, missile_width, missile_height);
            g_object_vector.emplace_back(missile_ptr);
            
        }

        for (auto& obj : g_object_vector) {
            std::cout << "obj paint" << std::endl;
            obj->Paint();
        }


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
    uint8_t plane_direction = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        plane_direction |= static_cast<uint8_t>(yk::EDirection::kU);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        plane_direction |= static_cast<uint8_t>(yk::EDirection::kD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        plane_direction |= static_cast<uint8_t>(yk::EDirection::kL);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        plane_direction |= static_cast<uint8_t>(yk::EDirection::kR);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // 这里为什么摁空格键一次，反而会触发多次
        std::cout << "launch a guided missile" << std::endl;
        g_launch_missile = true;
    }
    plane_ptr->Move(plane_direction);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}