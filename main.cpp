#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "ESC detectado!" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    //Inicializar GLFW
    if (!glfwInit())
    {
        std::cout << "Error al iniciar GLFW" << std::endl;
        return -1;
    }

    //Configurar la version de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Needed for MacOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Crear la ventana
    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "Triangulo OpenGL",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Error al crear la ventana" << std::endl;
         glfwTerminate();
        return -1;
    }

    // Usar el contexto de OpenGL de esta ventana
    glfwMakeContextCurrent(window);
    
    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error al iniciar GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

   /* Old Vertex array
    // Vertices del triangulo
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
*/

 // Posicion             // Coordenadas de textura
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Superior derecha
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Inferior derecha
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Inferior izquierda
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Superior izquierda
    };

    // Dos triangulos forman el rectangulo
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };


    /* Old shader
    // Vertex shader
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n";
    */

        // Vertex shader
    /*const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";
*/
        //vertexShaderSource
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";

    // Compilar el vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    // Verificar si el vertex shader compilo
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error en vertex shader:\n"
                  << infoLog << std::endl;
    }


    /*
    // Fragment shader
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

        */

       //fragmentShaderSource 
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture1;\n"
        "uniform sampler2D texture2;\n"
        "void main()\n"
        "{\n"
        "    vec4 crate = texture(texture1, TexCoord);\n"
        "    vec4 face = texture(texture2, TexCoord);\n"
        "    FragColor = vec4(mix(crate.rgb, face.rgb, 0.2), 1.0);\n"
        "}\n";

    // Compilar el fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Crear el shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  
    /*
    // Crear el VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Crear el VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Conectar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copiar los vertices al buffer
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Decirle a OpenGL como leer los vertices
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
    */

        // Crear los buffers
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Guardar los vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Guardar los indices de los dos triangulos
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // Posicion: primeros 3 floats de cada vertice
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Textura: siguientes 2 floats de cada vertice
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Crear dos texturas
    unsigned int textures[2];
    glGenTextures(2, textures);

    const char* imagePaths[] = {
        "container.jpg",
        "awesomeface.png"
    };

    stbi_set_flip_vertically_on_load(true);

    for (int i = 0; i < 2; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT
        );
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT
        );
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR
        );
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        int width, height, nrChannels;

        // Cargar ambas imagenes con 4 canales
        unsigned char* data = stbi_load(
            imagePaths[i],
            &width,
            &height,
            &nrChannels,
            STBI_rgb_alpha
        );

        if (data == nullptr)
        {
            std::cout << "Error al cargar "
                      << imagePaths[i] << ": "
                      << stbi_failure_reason() << std::endl;

            glDeleteTextures(2, textures);
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteProgram(shaderProgram);
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        // Enviar los pixeles a OpenGL
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    // Conectar cada sampler con su unidad de textura
    glUseProgram(shaderProgram);

    glUniform1i(
        glGetUniformLocation(shaderProgram, "texture1"), 0
    );
    glUniform1i(
        glGetUniformLocation(shaderProgram, "texture2"), 1
    );

        // Mantener la ventana abierta
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);
 

        // Ajustar el viewport para la pantalla
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Limpiar el fondo
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Conectar las dos texturas
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // Dibujar el rectangulo
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        // Mostrar el resultado
        glfwSwapBuffers(window);
    }

    // Limpiar los recursos al cerrar la ventana
    glDeleteTextures(2, textures);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


