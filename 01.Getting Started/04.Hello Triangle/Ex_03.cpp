// 使用两个着色器输出两个不同颜色的三角形
// EX_03.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void compileCheck(unsigned int, bool);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器
const char * vertexShaderSource = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "}\0";
// 片段着色器
const char * fragmentShader1Source = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";
const char * fragmentShader2Source = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                     "}\n\0";


int main()
{
    using std::cout;
    using std::endl;

    // glfw: 初始化设置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);     // 设置OpenGL上下文
    // 设置回调函数，当窗口大小改变的时候会调用，使图像适应窗口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 加载OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        exit(EXIT_FAILURE);
    }

    // 配置并编译着色器程序
    // ---------------------------------------------------------------

    // 顶点着色器：创建 -> 源码附加 -> 编译 -> 检查
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram1 = glCreateProgram();
    unsigned int shaderProgram2 = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    compileCheck(vertexShader, false);

    glShaderSource(fragmentShader1, 1, &fragmentShader1Source, nullptr);
    glCompileShader(fragmentShader1);
    compileCheck(fragmentShader1, false);

    glShaderSource(fragmentShader2, 1, &fragmentShader2Source, nullptr);
    glCompileShader(fragmentShader2);
    compileCheck(fragmentShader2, false);

    // 链接着色器：创建 -> 附加 -> 链接
    // First
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    // 检查链接错误
    compileCheck(shaderProgram1, true);

    // Second
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    compileCheck(shaderProgram2, true);

    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);


    // 定义顶点数据
    float firstTriangle[] = {
            -0.9f, -0.5f, 0.0f,  // left
            -0.0f, -0.5f, 0.0f,  // right
            -0.45f, 0.5f, 0.0f,  // top
    };
    float secondTriangle[] = {
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top
    };


    // 创建顶点缓冲和顶点数组
    unsigned int VAO[2], VBO[2];
    glGenVertexArrays(2, VAO);     // 1代表VAO数量
    glGenBuffers(2, VBO);

    // First Triangle
    // ---------------------------------------------------
    // 绑定先绑定VAO，然后绑定并设置顶点缓冲（VBO），最后设置顶点属性
    glBindVertexArray(VAO[0]);
    // 设置顶点缓冲对象，将顶点数据存储到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // 设置顶点属性，让OpenGL可以解析缓冲内容
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    // Second Triangle
    // ---------------------------------------------------
    // 绑定先绑定VAO，然后绑定并设置顶点缓冲（VBO），最后设置顶点属性
    glBindVertexArray(VAO[1]);
    // 设置顶点缓冲对象，将顶点数据存储到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    // 设置顶点属性，让OpenGL可以解析缓冲内容
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);


    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入，这里如果是ESCAP的话就退出窗口
        // -----
        processInput(window);

        // 渲染，设置背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // First Triangle
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Second Triangle
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: 交换颜色缓冲（存储着每个像素颜色的大缓冲），在本轮迭代中用来绘制窗口
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        // 事件触发检测
        glfwPollEvents();
    }

    // 可选，删除VAO，VBO
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    
    // glfw: 终结窗口显示，并释放资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保viewport匹配调整后的窗口大小
    glViewport(0, 0, width, height);
}

// 着色器编译检查
void compileCheck(unsigned int shader, bool isProgram) {
    int success;            // 判断着色器是否编译成功的标识
    char infoLog[512];      // 捕获到的错误信息字符串

    if (isProgram) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}