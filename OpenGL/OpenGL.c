#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 640
#define HEIGHT 480


//Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main(){
  // Initiate glfw
  if(!glfwInit()){
    printf("Error initializing GLFW");
    return 1;
  }

  // Create window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Chess", NULL, NULL);
  if(!window){
    glfwTerminate();

    printf("Error initializing window");
    return 1;
  }

  // Make the window he current context (probably to make sure if u have more windows or more windows profiles)
  glfwMakeContextCurrent(window);

  // Load glad
  int version_glad = gladLoadGL( glfwGetProcAddress );
  if ( version_glad == 0 ) {
    glfwDestroyWindow(window);
    glfwTerminate();

    fprintf( stderr, "ERROR: Failed to initialize OpenGL context.\n" );
    return 1;
  }
  // DEBUG info
  printf( "Loaded OpenGL %i.%i\n", GLAD_VERSION_MAJOR( version_glad ), GLAD_VERSION_MINOR( version_glad ) );
  printf( "Renderer: %s.\n", glGetString( GL_RENDERER ) );
  printf( "OpenGL version supported %s.\n", glGetString( GL_VERSION ) );

  // Set up viewport and resize the viewport when user resize window
  glViewport(0,0,WIDTH,HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  // Shader  #######################################################################
  //TODO: maybe learn a way to outsource the shader code in a file
  // Shader code  ------------------------------------------------------------------
  // Vertex shader  ................................................................
  // vertex_shader.glsl
  const char* VERTEX_SHADER_SOURCE = "#version 330 core\n\
    layout (location = 0) in vec3 aPos;\n\
    void main(){ \n\
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
    }\0\
  ";
  // Compile the shader code
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertexShader);

  //Check for compilaion succes
  int  success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

    glDeleteShader(vertexShader);
    glfwDestroyWindow(window);
    glfwTerminate();

    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
    return 1;
  }

  // Fragment shader  ...............................................................
  const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n\
    out vec4 FragColor;\n\
    void main(){\n\
      FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
    }\0\
  ";
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragmentShader);

  //Check for compilaion succes
  #define INFO_LOG_SIZE 512
  char infoLog[INFO_LOG_SIZE];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {

    glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, NULL, infoLog);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwDestroyWindow(window);
    glfwTerminate();

    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
    return 1;
  }

  // Compile shader program (both shaders together) ---------------------------------
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // Check for errors on linking shaders
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, INFO_LOG_SIZE, NULL, infoLog);

    glfwDestroyWindow(window);
    glfwTerminate();

    printf("ERROR::PROGAM::COMPILATION_FAILED\n%s\n",infoLog);
  }

  // Memory  ########################################################################
  // Info about vertices  -----------------------------------------------------------
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // Array objects (VAO) 1  -------------------------------------------------------
  // Stores the VBO and the layout of the buffer
  // Create the VAO and bind it (bind to configure the VBO inside the VAO)
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);


  // Buffers (VBO)  ------------------------------------------------------------------
  // Generate a buffer (we can do it before the VAO)
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // Bind the buffer to the specific type of OpenGL buffer (¡¡AFTER!! the VAO)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Give data to buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Tell OpenGL how the vertex data is structured  ---------------------------------
  // 1: The number of the attribute u are gonna describe in the vertex (Ex: 1º attrib is position, 2º is color, ...)
  // 2: How many values is the attrib is gonna use ( postion is gonna use 2 if 2D, 3 if 3D or color, 4 if color with alpha, ...)
  // 3: Type of the elements
  // 4: If the data is normalized
  // 5: Offset between attribs from different vertex (counting it self)(if only one attrib is present u can set it to 0(tightly packed)) (rare will be its not the same as the sum of the size of attrib of vertex)
  // 6: Offset to the first attrib on the array (offset of that attrib on the first element (Ex: first 2D pos float then color, for color attrib will '(GLvoid*)(2*sizeof(GLfloat)' )
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
  glEnableVertexAttribArray(0);



  // Do things on the window  #######################################################
  while(!glfwWindowShouldClose(window)){
    // Process the events
    glfwPollEvents();

    // Clear the screen (RGBA)
    glClearColor(0.9f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Use shader and bind the VAO we are gonna use to render
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //Tell the shaders (probably not he ones we programmed now) the mode, the first vertex, and the count of vertex we want to render
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Render the frame
    glfwSwapBuffers(window);
  }

  // Close all objects
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
