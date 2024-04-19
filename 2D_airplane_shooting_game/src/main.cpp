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
#include "game_context.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::shared_ptr<yk::Plane> plane_ptr);

// settings
const unsigned int SCR_WIDTH = 300;
const unsigned int SCR_HEIGHT = 200;

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


    //主控飞机1
    float plane_width = 0.1f;
    float plane_height = 0.12f;
    std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2, -1.0f + plane_height, plane_width, plane_height);
    yk::GameContext::GetInstance()->AddOurObject(plane_ptr);
    
    
    //主控飞机2 to do



    // render loop
    // -----------
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
            plane_ptr->LaunchMissile();
        }

        // 构造敌方元素   
        if (yk::GameContext::GetInstance()->GetEnemyPlaneObjectSize() < 14) {
            static yk::Position birth_pos;
            birth_pos.y = 0.78f;
            if (birth_pos.x > 0.88f) {
                birth_pos.x = -0.88f;
            }
            float plane_width = 0.1f;
            float plane_height = 0.12f;
            std::shared_ptr<yk::EnemyPlane> plane_ptr = yk::ElementFactory::CreateEnemyPlane("image/enemy.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", birth_pos.x, birth_pos.y, plane_width, plane_height);
            plane_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kD));
            yk::GameContext::GetInstance()->AddEnemyPlaneObject(plane_ptr);
            birth_pos.x += (plane_width * 2.5);
        }

       yk::GameContext::GetInstance()->EnemyAutoLanuchMissile();


        yk::GameContext::GetInstance()->DrawObjects();
       
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
    uint8_t plane_direction_combination = static_cast<uint8_t>(yk::EDirection::kS);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kU);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kL);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        plane_direction_combination |= static_cast<uint8_t>(yk::EDirection::kR);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // 这里为什么摁空格键一次，反而会触发多次
        std::cout << "launch a guided missile" << std::endl;
        g_launch_missile = true;
    }
    plane_ptr->SetDirection(plane_direction_combination);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}