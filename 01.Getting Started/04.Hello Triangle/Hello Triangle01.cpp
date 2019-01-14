#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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
const char * fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    int success;            // 判断着色器是否编译成功的标识
    char infoLog[512];      // 捕获到的错误信息字符串

    // 顶点着色器：创建 -> 源码附加 -> 编译 -> 检查
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // 链接着色器：创建 -> 附加 -> 链接
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定义顶点数据，一个三角形
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,     // 左底边
             0.5f, -0.5f, 0.0f,     // 右底边
             0.0f,  0.5f, 0.0f      // 顶点
    };

    // 创建顶点缓冲和顶点数组
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);     // 1代表VAO数量
    glGenBuffers(1, &VBO);
    // 绑定先绑定VAO，然后绑定并设置顶点缓冲（VBO），最后设置顶点属性
    glBindVertexArray(VAO);
    // 设置顶点缓冲对象，将顶点数据存储到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性，让OpenGL可以解析缓冲内容
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

        // 绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: 交换颜色缓冲（存储着每个像素颜色的大缓冲），在本轮迭代中用来绘制窗口
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        // 事件触发检测
        glfwPollEvents();
    }

    // 可选，删除VAO，VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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