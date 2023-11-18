#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"
#include "image.h"
#include "vector.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "cameraGL.h"

using namespace hw3;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight;


bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void hw_3_1(const std::vector<std::string> &params) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Balboa", nullptr, nullptr);
    
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void hw_3_2(const std::vector<std::string> &params) {
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    const char *vertexShaderSource ="#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model; // rotation matrix\n"
        "void main()\n"
        "{\n"
        "   gl_Position = model * vec4(aPos, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = ourColor;\n"
        "}\n\0";
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
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f   // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);

    float angle = 0.0f;
    glm::mat4 rotationMatrix = glm::mat4(1.0f);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // be sure to activate the shader before any calls to glUniform
        glUseProgram(shaderProgram);

        angle += 0.01f;
        rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

        // be sure to activate the shader before any calls to glUniform
        glUseProgram(shaderProgram);

        // update shader uniform
        int modelLocation = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}










// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    const unsigned int SCR_WIDTH = scene.camera.resolution.x;
    const unsigned int SCR_HEIGHT = scene.camera.resolution.y;

    cameraPos = {
        scene.camera.cam_to_world(0,3),
        scene.camera.cam_to_world(1,3),
        scene.camera.cam_to_world(2,3)
    };

    cameraFront.x   = -scene.camera.cam_to_world(0,2);
    cameraFront.y   = -scene.camera.cam_to_world(1,2);
    cameraFront.z   = -scene.camera.cam_to_world(2,2);
    
    cameraUp.x   = scene.camera.cam_to_world(0,1);
    cameraUp.y   = scene.camera.cam_to_world(1,1);
    cameraUp.z   = scene.camera.cam_to_world(2,1);

    cameraRight.x   = scene.camera.cam_to_world(0,0);
    cameraRight.y   = scene.camera.cam_to_world(1,0);
    cameraRight.z   = scene.camera.cam_to_world(2,0);

    const char *vertexShaderSource ="#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec3 ourColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(ourColor, 1.0f);\n"
        "}\n\0";
        
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Balboa", NULL, NULL);
    std::cout << "SCR_WIDTH: " << SCR_WIDTH << std::endl;
    std::cout << "SCR_HEIGHT: " << SCR_HEIGHT << std::endl;
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAOs;
    GLuint VBO_vertex;
    GLuint VBO_color;
    GLuint EBOs;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);
        Image3 img(SCR_WIDTH, SCR_HEIGHT);
        // Construct perspective projection matrix
        Real w = Real(scene.camera.resolution.x);
        Real h = Real(scene.camera.resolution.y);
        Real s = Real(scene.camera.s);
        Real a = Real(w) / Real(h);
        Real z_near = Real(scene.camera.z_near);
        Real z_far = Real(scene.camera.z_far);

        Matrix4x4f projection = Matrix4x4f(
            1.0 / (a * s), 0.0, 0.0, 0.0,
            0.0, 1.0 / s, 0.0, 0.0,
            0.0, 0.0, -z_far / (z_far - z_near), (-z_far * z_near) / (z_far - z_near),
            0.0, 0.0, -1.0, 0.0
        );

        // std::cout << "projection: " << projection << std::endl;

        // render
        // ------
        glClearColor(Vector3f (scene.background).x, Vector3f(scene.background).y, Vector3f(scene.background).z, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_FRAMEBUFFER_SRGB); 

        for (std::size_t i = 0; i < scene.meshes.size(); ++i) {
            auto mesh = scene.meshes[i];

            std::vector<float> vertex_array = {};
            std::vector<float> color_array = {};
            std::vector<unsigned int> face_array = {};

            for(int j = 0; j < mesh.vertices.size(); ++j) {

                auto vertex = Vector3f(mesh.vertices[j]);

                vertex_array.push_back(vertex.x);
                vertex_array.push_back(vertex.y);
                vertex_array.push_back(vertex.z);
            }
            
            float vertices_size [vertex_array.size()];
            for(int j = 0; j < vertex_array.size(); ++j) {
                vertices_size[j] = vertex_array[j];
            }



            for(int j = 0; j < mesh.vertex_colors.size(); ++j) {

                auto color = Vector3f(mesh.vertex_colors[j]);

                color_array.push_back(color.x);
                color_array.push_back(color.y);
                color_array.push_back(color.z);
            }
            
            float colors_size [color_array.size()];
            for(int j = 0; j < color_array.size(); ++j) {
                colors_size[j] = color_array[j];
            }

            

            for(int j = 0; j < mesh.faces.size(); ++j) {

                auto face = Vector3i(mesh.faces[j]);

                face_array.push_back(face.x);
                face_array.push_back(face.y);
                face_array.push_back(face.z);
            }
            
            unsigned int face_size [face_array.size()];
            for(int j = 0; j < face_array.size(); ++j) {
                face_size[j] = face_array[j];
            }

            
        
            glGenVertexArrays(1, &VAOs);
            glBindVertexArray(VAOs);

            // Create VBO for vertices
            glGenBuffers(1, &VBO_vertex);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
            glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(float), vertices_size, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Create VBO for colors
            glGenBuffers(1, &VBO_color);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
            glBufferData(GL_ARRAY_BUFFER, color_array.size() * sizeof(float), colors_size, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);

            glGenBuffers(1, &EBOs);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_array.size() * sizeof(unsigned int), face_size, GL_STATIC_DRAW);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);

            // Set shader program
            glUseProgram(shaderProgram);

            // Set uniforms
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.ptr());
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, scene.meshes[i].model_matrix.ptr());

            // Bind VAO
            glBindVertexArray(VAOs);

            // Draw the mesh using indexed rendering
            glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

            // i++;
            glBindVertexArray(0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // Delete VAOs
    glDeleteVertexArrays(1, &VAOs);

    // Delete VBOs
    glDeleteBuffers(1, &VBO_vertex);
    glDeleteBuffers(1, &VBO_color);
    glDeleteBuffers(1, &EBOs);

    // Delete shader program
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}















void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    const unsigned int SCR_WIDTH = scene.camera.resolution.x;
    const unsigned int SCR_HEIGHT = scene.camera.resolution.y;

    // cameraPos.x   = scene.camera.cam_to_world(0,3);
    // cameraPos.y   = scene.camera.cam_to_world(1,3);
    // cameraPos.z   = scene.camera.cam_to_world(2,3);

    cameraPos = {
        scene.camera.cam_to_world(0,3),
        scene.camera.cam_to_world(1,3),
        scene.camera.cam_to_world(2,3)
    };

    cameraFront.x   = -scene.camera.cam_to_world(0,2);
    cameraFront.y   = -scene.camera.cam_to_world(1,2);
    cameraFront.z   = -scene.camera.cam_to_world(2,2);
    
    cameraUp.x   = scene.camera.cam_to_world(0,1);
    cameraUp.y   = scene.camera.cam_to_world(1,1);
    cameraUp.z   = scene.camera.cam_to_world(2,1);

    cameraRight.x   = scene.camera.cam_to_world(0,0);
    cameraRight.y   = scene.camera.cam_to_world(1,0);
    cameraRight.z   = scene.camera.cam_to_world(2,0);

    std::cout << (scene.camera.cam_to_world) << std::endl;

    const char *vertexShaderSource ="#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec3 aNormal;\n"
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "out vec3 ourColor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        // "    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "    ourColor = aColor;\n"
        "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
            
        "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "in vec3 ourColor;\n"

        "out vec4 FragColor;\n"

        "uniform vec3 viewPos;\n"

        "void main()\n"
        "{\n"
        "   // ambient\n"
        "   float ambientStrength = 0.1;\n"
        "   vec3 ambient = ambientStrength * ourColor;\n"
        
        "   // diffuse\n"
        "   vec3 lightColor = vec3(1, 1, 1);\n"
        "   vec3 norm = normalize(Normal);\n"
        "   vec3 lightDir = normalize(vec3(1, 1, 1));\n"
        "   float diff = max(dot(norm, lightDir), 0.0);\n"
        "   vec3 diffuse = diff * lightColor;\n"
        
        "   // specular\n"
        "   float specularStrength = 0.5;\n"
        "   vec3 viewDir = normalize(viewPos - FragPos);\n"
        "   vec3 reflectDir = reflect(-lightDir, norm);\n"
        "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n"
        "   vec3 specular = specularStrength * spec * lightColor;\n"

        "   vec3 result = (ambient + diffuse + specular) * ourColor;\n"
        "   FragColor = vec4(result, 1.0f);\n"
        "}\0";
        
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Balboa", NULL, NULL);
    std::cout << "SCR_WIDTH: " << SCR_WIDTH << std::endl;
    std::cout << "SCR_HEIGHT: " << SCR_HEIGHT << std::endl;
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader(vertexShaderSource, fragmentShaderSource);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAOs;
    GLuint VBO_vertex;
    GLuint VBO_color;
    GLuint VBO_normal;
    GLuint EBOs;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);
        Image3 img(SCR_WIDTH, SCR_HEIGHT);
        // Construct perspective projection matrix
        Real w = Real(scene.camera.resolution.x);
        Real h = Real(scene.camera.resolution.y);
        Real s = Real(scene.camera.s);
        Real a = Real(w) / Real(h);
        Real z_near = Real(scene.camera.z_near);
        Real z_far = Real(scene.camera.z_far);

        Matrix4x4f projection = Matrix4x4f(
            1.0 / (a * s), 0.0, 0.0, 0.0,
            0.0, 1.0 / s, 0.0, 0.0,
            0.0, 0.0, -z_far / (z_far - z_near), (-z_far * z_near) / (z_far - z_near),
            0.0, 0.0, -1.0, 0.0
        );

        // std::cout << "projection: " << projection << std::endl;

        // render
        // ------
        glClearColor(Vector3f (scene.background).x, Vector3f(scene.background).y, Vector3f(scene.background).z, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_FRAMEBUFFER_SRGB); 

        for (std::size_t i = 0; i < scene.meshes.size(); ++i) {
            auto mesh = scene.meshes[i];

            std::vector<float> vertex_vector = {};
            std::vector<float> color_vector = {};
            std::vector<float> normal_vector = {};
            std::vector<unsigned int> face_vector = {};

            for(int j = 0; j < mesh.vertices.size(); ++j) {

                auto vertex = Vector3f(mesh.vertices[j]);

                vertex_vector.push_back(vertex.x);
                vertex_vector.push_back(vertex.y);
                vertex_vector.push_back(vertex.z);
            }
            
            float vertices_array [vertex_vector.size()];
            for(int j = 0; j < vertex_vector.size(); ++j) {
                vertices_array[j] = vertex_vector[j];
            }



            for(int j = 0; j < mesh.vertex_colors.size(); ++j) {

                auto color = Vector3f(mesh.vertex_colors[j]);

                color_vector.push_back(color.x);
                color_vector.push_back(color.y);
                color_vector.push_back(color.z);
            }
            
            float colors_array [color_vector.size()];
            for(int j = 0; j < color_vector.size(); ++j) {
                colors_array[j] = color_vector[j];
            }

            

            for(int j = 0; j < mesh.faces.size(); ++j) {

                auto face = Vector3i(mesh.faces[j]);

                face_vector.push_back(face.x);
                face_vector.push_back(face.y);
                face_vector.push_back(face.z);
            }
            
            unsigned int face_array [face_vector.size()];
            for(int j = 0; j < face_vector.size(); ++j) {
                face_array[j] = face_vector[j];
            }




            
            for(int j = 0; j < mesh.vertex_normals.size(); ++j) {

                auto normal = Vector3f(mesh.vertex_normals[j]);

                normal_vector.push_back(normal.x);
                normal_vector.push_back(normal.y);
                normal_vector.push_back(normal.z);
            }
            
            float normal_array [normal_vector.size()];
            for(int j = 0; j < normal_vector.size(); ++j) {
                normal_array[j] = normal_vector[j];
            }



            glGenVertexArrays(1, &VAOs);
            glBindVertexArray(VAOs);

            // Create VBO for vertices
            glGenBuffers(1, &VBO_vertex);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
            glBufferData(GL_ARRAY_BUFFER, vertex_vector.size() * sizeof(float), vertices_array, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Create VBO for colors
            glGenBuffers(1, &VBO_color);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
            glBufferData(GL_ARRAY_BUFFER, color_vector.size() * sizeof(float), colors_array, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);

            // Create VBO for normals
            glGenBuffers(1, &VBO_normal);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
            glBufferData(GL_ARRAY_BUFFER, normal_vector.size() * sizeof(float), normal_array, GL_STATIC_DRAW);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(2);

            glGenBuffers(1, &EBOs);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_vector.size() * sizeof(unsigned int), face_array, GL_STATIC_DRAW);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);

            // Set shader program
            glUseProgram(shaderProgram);

            // be sure to activate shader when setting uniforms/drawing objects
            lightingShader.use();
            // lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            // lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            // lightingShader.setVec3("lightPos", lightPos);
            // lightingShader.setVec3("viewPos", cameraPos);

            // Set uniforms
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.ptr());
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, inverse(scene.camera.cam_to_world).ptr());
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, scene.meshes[i].model_matrix.ptr());
            // glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, GL_FALSE, glm::value_ptr(cameraPos));
            GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
            glUniform3fv(viewPosLoc, 1, glm::value_ptr(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z)));


            // Bind VAO
            glBindVertexArray(VAOs);

            // Draw the mesh using indexed rendering
            glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

            // i++;
            glBindVertexArray(0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // Delete VAOs
    glDeleteVertexArrays(1, &VAOs);

    // Delete VBOs
    glDeleteBuffers(1, &VBO_vertex);
    glDeleteBuffers(1, &VBO_color);
    glDeleteBuffers(1, &VBO_normal);
    glDeleteBuffers(1, &EBOs);

    // Delete shader program
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
