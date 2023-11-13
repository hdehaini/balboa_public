#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"
#include "image.h"

using namespace hw3;

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

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration remains the same (still one 3-float position vector per vertex)
    float vertices[] = {
        // first triangle
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
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

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
        // glBindVertexArray(0); // no need to unbind it every time 
 
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

Vector3 xform_point(const Matrix4x4 &m, const Vector3 &p) {
    Vector4 ph = Vector4{p.x, p.y, p.z, Real(1)};
    Vector4 mph = m * ph;
    return Vector3{mph.x / mph.w,
                   mph.y / mph.w,
                   mph.z / mph.w};
}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration remains the same (still one 3-float position vector per vertex)
    float vertices[] = {
        // first triangle
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    float colors[] = {
        // first triangle
        1.0f, 0.0f, 0.0f, // red
        0.0f, 1.0f, 0.0f, // green
        0.0f, 0.0f, 1.0f  // blue
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

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS); // The default behavior, fragments with smaller depth values pass.
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // 24-bit depth buffer (adjust to your needs)

    unsigned int VBO_vertex, VBO_color;
    glGenBuffers(1, &VBO_vertex);
    glGenBuffers(1, &VBO_color);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Vertex Position Attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // Vertex Color Attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        Image3 img(scene.camera.resolution.x,
                scene.camera.resolution.y);
        // Construct perspective projection matrix
        Real w = scene.camera.resolution.x;
        Real h = scene.camera.resolution.y;
        Real s = scene.camera.s;
        Real a = Real(w) / Real(h);
        Real z_near = scene.camera.z_near;
        Real z_far = scene.camera.z_far;

        Matrix4x4f projection = Matrix4x4f(
            1.0 / a * s, 0.0, 0.0, 0.0,
            0.0, 1.0 / s, 0.0, 0.0,
            0.0, 0.0, -z_far / (z_far - z_near), (-z_far * z_near) / (z_far - z_near),
            0.0, 0.0, -1.0, 0.0
        );
        Matrix4x4f camera_to_screen;
        camera_to_screen(0, 0) = w / (2 * s * a);
        camera_to_screen(0, 3) = w / 2;
        camera_to_screen(1, 1) = -h / (2 * s);
        camera_to_screen(1, 3) = h / 2;
        camera_to_screen(2, 2) = 1;
        camera_to_screen(3, 3) = 1;

        Matrix4x4f world_to_screen =
            camera_to_screen * projection * inverse(scene.camera.cam_to_world);

        for (const auto &mesh : scene.meshes) {
            GLuint VAO, VBO_vertex, VBO_color;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO_vertex);
            glGenBuffers(1, &VBO_color);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);

            // Vertex Position Attribute
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertex_colors.size() * sizeof(float), &mesh.vertex_colors[0], GL_STATIC_DRAW);

            // Vertex Color Attribute
            glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);

            // Store the VAO and VBOs for this mesh
            VAOs.push_back(VAO);
            VBOs.push_back(VBO_vertex);
            VBOs.push_back(VBO_color);
        }

        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                Vector3 sum{0, 0, 0};
                int n = 4;
                for (int dx = 0; dx < n; dx++) {
                    for (int dy = 0; dy < n; dy++) {
                        Real xoff = (dx + Real(0.5)) / Real(n);
                        Real yoff = (dy + Real(0.5)) / Real(n);
                        Vector2 pixel_center{x + xoff, y + yoff};
                        Vector3 color = Vector3{0.5, 0.5, 0.5}; // background color
                        Real z_min = infinity<Real>();
                        for (const auto &mesh : scene.meshes) {

                            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

                            // construct the MVP matrix
                            Matrix4x4 MVP = world_to_screen * mesh.model_matrix;
                            for (int i = 0; i < (int)mesh.faces.size(); i++) {
                                Vector3 p0 = mesh.vertices[mesh.faces[i][0]];
                                Vector3 p1 = mesh.vertices[mesh.faces[i][1]];
                                Vector3 p2 = mesh.vertices[mesh.faces[i][2]];
                                Vector3 screen_p0 = xform_point(MVP, p0);
                                Vector3 screen_p1 = xform_point(MVP, p1);
                                Vector3 screen_p2 = xform_point(MVP, p2);

                                // We don't implement clipping here, if even one of the 
                                // vertex is behind the clipping plane, we skip the triangle
                                // screen_pi.z stores 1/pi.z
                                Real inv_z_near = (1/scene.camera.z_near);
                                if (screen_p0.z > inv_z_near ||
                                        screen_p1.z > inv_z_near ||
                                        screen_p2.z > inv_z_near) {
                                    continue;
                                }

                                Vector2 sp0 = Vector2{screen_p0.x, screen_p0.y};
                                Vector2 sp1 = Vector2{screen_p1.x, screen_p1.y};
                                Vector2 sp2 = Vector2{screen_p2.x, screen_p2.y};

                                Vector2 e01 = sp1 - sp0, e12 = sp2 - sp1, e20 = sp0 - sp2;
                                auto normal = [](const Vector2 &v) {
                                    return Vector2{v.y, -v.x};
                                };
                                Vector2 n01 = normal(e01),
                                        n12 = normal(e12),
                                        n20 = normal(e20);

                                bool s01 = dot(pixel_center - sp0, n01) > 0;
                                bool s12 = dot(pixel_center - sp1, n12) > 0;
                                bool s20 = dot(pixel_center - sp2, n20) > 0;
                                if ((s01 && s12 && s20) || (!s01 && !s12 && !s20)) {
                                    // Inside the triangle
                                    // Interpolate Z
                                    
                                    // 1. convert pixel center from screen space to camera space
                                    auto to_camera = [&](const Vector2 &p) {
                                        // camera to screen:
                                        // x'' = w ((x' + sa)/ (2sa))
                                        // y'' = -h ((y' - s)/ (2s))
                                        // screen to camera:
                                        // x' = 2sa x'' / w - sa
                                        // y' = - 2s y'' / h + s
                                        return Vector2{(2 * s * a) * p.x / img.width - s * a,
                                                    - 2 * s * p.y / img.height + s};
                                    };
                                    Vector2 pp = to_camera(pixel_center);
                                    Vector2 pp0 = to_camera(sp0);
                                    Vector2 pp1 = to_camera(sp1);
                                    Vector2 pp2 = to_camera(sp2);

                                    // 2. compute projected barycentric coordinates
                                    auto area = [&](const Vector2 pp0,
                                                    const Vector2 pp1,
                                                    const Vector2 pp2) {
                                        // lift things to 3D to take cross product
                                        Vector3 p0 = Vector3{pp0.x, pp0.y, Real(0)};
                                        Vector3 p1 = Vector3{pp1.x, pp1.y, Real(0)};
                                        Vector3 p2 = Vector3{pp2.x, pp2.y, Real(0)};
                                        return length(cross(p1 - p0, p2 - p0)) / 2;
                                    };

                                    Real a0 = area(pp, pp1, pp2);
                                    Real a1 = area(pp0, pp, pp2);
                                    Real a2 = area(pp0, pp1, pp);
                                    Real denom = area(pp0, pp1, pp2);
                                    Real b0p = a0 / denom;
                                    Real b1p = a1 / denom;
                                    Real b2p = a2 / denom;

                                    // 3. compute the original barycentric coordinates
                                    b0p *= screen_p0.z;
                                    b1p *= screen_p1.z;
                                    b2p *= screen_p2.z;
                                    Real b0 = b0p / (b0p + b1p + b2p);
                                    Real b1 = b1p / (b0p + b1p + b2p);
                                    Real b2 = b2p / (b0p + b1p + b2p);

                                    // 4. interpolate and obtain Z
                                    Real z = b0 / screen_p0.z +
                                            b1 / screen_p1.z +
                                            b2 / screen_p2.z;
                                    // Remember to negate to make Z positive
                                    z = fabs(z);

                                    // Now, test if Z is smaller
                                    if (z < z_min) {
                                        // Interpolate vertex colors
                                        Vector3 C0 = mesh.vertex_colors[mesh.faces[i][0]];
                                        Vector3 C1 = mesh.vertex_colors[mesh.faces[i][1]];
                                        Vector3 C2 = mesh.vertex_colors[mesh.faces[i][2]];

                                        color = b0 * C0 + b1 * C1 + b2 * C2;
                                        z_min = z;
                                    }
                                }
                            }
                        }
                        sum += color;
                    }
                }
                img(x, y) = sum / Real(n * n);
            }
        }
        
        
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
        // glBindVertexArray(0); // no need to unbind it every time 
 
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

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}
