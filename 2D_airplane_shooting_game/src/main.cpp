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

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 900;

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
    glfwMaximizeWindow(window);

    yk::GameContext::GetInstance()->PlayBackgroundMusic();


    //主控飞机1
    {
        float plane_width = 0.1f;
        float plane_height = 0.12f;
        std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 - plane_width * 3, -1.0f + plane_height, plane_width, plane_height);

        plane_ptr->SetExplodeImages({
            "image/explode/hero/explode01.png",
            "image/explode/hero/explode02.png",
            "image/explode/hero/explode03.png",
            "image/explode/hero/explode04.png",
            "image/explode/hero/explode05.png",
            "image/explode/hero/explode06.png"
        });

        yk::GameContext::GetInstance()->AddOurFirstPlaneObject(plane_ptr);
    }
    //主控飞机2 
    {
        float plane_width = 0.1f;
        float plane_height = 0.12f;
        std::shared_ptr<yk::MainPlane> plane_ptr = yk::ElementFactory::CreateMainPlane("image/hero_b_2.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1 * plane_width / 2 + plane_width * 3, -1.0f + plane_height, plane_width, plane_height);

        plane_ptr->SetExplodeImages({
            "image/explode/hero/explode01.png",
            "image/explode/hero/explode02.png",
            "image/explode/hero/explode03.png",
            "image/explode/hero/explode04.png",
            "image/explode/hero/explode05.png",
            "image/explode/hero/explode06.png"
            });

        yk::GameContext::GetInstance()->AddOurSecondPlaneObject(plane_ptr);
    }

    // background
    std::shared_ptr<yk::Background> background_ptr = std::make_shared<yk::Background>("image/bg_512x768.jpg", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1.0f, 1.0f, 2.0f, 2.0f);

    background_ptr->Init({1.0f, 1.0f}, {1.0, -1.0f}, {-1.0f, -1.0f}, {-1.0f, 1.0f});

    background_ptr->first = true;

    std::shared_ptr<yk::Background> background_ptr2 = std::make_shared<yk::Background>("image/bg_512x768.jpg", "shader/hero_b_1.vs", "shader/hero_b_1.fs", -1.0f, 3.0f, 2.0f, 2.0f);

    background_ptr2->Init({ 1.0f, 1.0f }, { 1.0, -1.0f }, { -1.0f, -1.0f }, { -1.0f, 1.0f });

    // 设置键盘回调函数
    glfwSetKeyCallback(window, keyCallback);

    // 设置游戏手柄回调函数
    glfwSetJoystickCallback(gamepadCallback);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        uint64_t before_time = yk::GetCurrentTimestamp();
        yk::GameContext::GetInstance()->ProcessInput(window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        background_ptr->Paint();

        background_ptr2->Paint();

        yk::GameContext::GetInstance()->OurPlaneLanuchMissile();

        // 构造敌方元素   
        yk::GameContext::GetInstance()->GenerateEnemyPlaneObjects();

        yk::GameContext::GetInstance()->EnemyAutoLanuchMissile();

        yk::GameContext::GetInstance()->DrawObjects();

        yk::GameContext::GetInstance()->ExecuCheckCollision();
       
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}