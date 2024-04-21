#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>
#include "setting.h"
#include "common/time_util.h"
#include "element/element_factory.h"
#include "learnopengl/shader_s.h"
#include "game_context.h"
#include "background.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::shared_ptr<yk::Plane> plane_ptr);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

std::vector<std::shared_ptr<yk::Object>> g_object_vector;

bool g_launch_missile = false;
uint64_t g_last_launch_missile_time = 0;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        std::cout << "按下的按键键值: " << key << std::endl;
    }
}

void gamepadCallback(int jid, int event)
{
    std::cout << "joystick id = " << jid << " event = " << event << std::endl;
    // 在此处理游戏手柄事件
}


int main()
{
    
    yk::Setting::GetInstance()->resource_base_path_ = YK_RESOURCE_PATH;

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


    yk::GameContext::GetInstance()->PlayBackgroundMusic();


    //主控飞机1
    float plane_width = 0.1f;
    float plane_height = 0.12f;
    std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2, -1.0f + plane_height, plane_width, plane_height);
    yk::GameContext::GetInstance()->AddOurObject(plane_ptr);
    
    
    //主控飞机2 to do



    // background
    std::shared_ptr<yk::Background> background_ptr = std::make_shared<yk::Background>("image/bg_512x768.jpg", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1.0f, 1.0f, 2.0f, 2.0f);

    // 设置键盘回调函数
    glfwSetKeyCallback(window, keyCallback);

    // 设置游戏手柄回调函数
    glfwSetJoystickCallback(gamepadCallback);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        uint64_t before_time = yk::GetCurrentTimestamp();
        // input
        // -----
        processInput(window, plane_ptr);


        // 获取手柄的按钮状态
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
        if (buttons != nullptr && count > 0)
        {
            // 检查按钮 0 是否按下
            if (buttons[0] == GLFW_PRESS)
            {
                std::cout << "按钮 0 被按下" << std::endl;

                
            }
            //std::cout << "count = " << count << std::endl;
            for (size_t i = 0; i < count; ++i)
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(buttons[i]) << ' ';
            }
            std::cout << std::endl;
        }




        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        background_ptr->Paint();
        
        plane_ptr->Paint();

        if (g_launch_missile) {
            g_launch_missile = false;
            plane_ptr->LaunchMissile();
        }

        // 构造敌方元素   
        yk::GameContext::GetInstance()->GenerateEnemyPlaneObjects();

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
    yk::GameContext::GetInstance()->StopPlay();
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
        auto current_time = yk::GetCurrentTimestamp();
        if (current_time - g_last_launch_missile_time > 60) {
            g_launch_missile = true;
            g_last_launch_missile_time = current_time;
        }
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