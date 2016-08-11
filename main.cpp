#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS // force everything in radian
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "lib/Shader.h"
#include "lib/Camera.h"
#include "lib/bitmap.h"
#include "lib/SnowSence.h"

#define SNOW_RANDOM 1
#define SNOW_GRAVITY 2
#define SNOW_GRAVITY_W_WIND 3

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement(SnowSence&);
GLuint loadCubemap(vector<const GLchar*> faces);
GLuint loadBaseTexture(GLchar*);
void load_obj(const char* filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
glm::vec3 lightPos(1.2f, -1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Crystal snow ball", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    int w,h;
    glfwGetFramebufferSize( window, &w, &h);

    // Define the viewport dimensions
    glViewport(0, 0, w, h);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader lightingShader("shader/phong.vs", "shader/phong.frag");
    Shader skyboxShader("shader/skybox.vs","shader/skybox.frag");
    Shader inShader("shader/inphong.vs","shader/inphong.frag");
    Shader snowShader("shader/snow.vs","shader/snow.frag");
    Shader baseShader("shader/base.vs","shader/base.frag");
    Shader floorShader("shader/floor.vs","shader/floor.frag");
    Shader bunnyShader("shader/bunny.vs","shader/bunny.frag");
//================================================================================================

    /*****************Sphere *****************/
    int i, j;
    int numbOfTri = 41*41*2;
    GLfloat vertices[numbOfTri*6];
    GLint indices[numbOfTri+100];

    int indicator = 0, cnt = 0, cntInd = 0, lats = 40, longs = 40;
    double radius = 0.3; // radius here!!
    for(i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for(j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            vertices[cnt++] = x * zr0 * radius;
            vertices[cnt++] = y * zr0 * radius;
            vertices[cnt++] = z0 * radius;
            vertices[cnt++] = x * zr0 * radius;
            vertices[cnt++] = y * zr0 * radius;
            vertices[cnt++] = z0 * radius;
            indices[cntInd++] = indicator ++;

            vertices[cnt++] = x * zr1 * radius;
            vertices[cnt++] = y * zr1 * radius;
            vertices[cnt++] = z1 * radius;
            vertices[cnt++] = x * zr1 * radius;
            vertices[cnt++] = y * zr1 * radius;
            vertices[cnt++] = z1 * radius;
            indices[cntInd++] = indicator ++;
       }
       indices[cntInd++] = GL_PRIMITIVE_RESTART_FIXED_INDEX;
    }
    /************** BASE *******************/
    GLint b_numbOfTri = 40*2;
    GLfloat b_Vertices[b_numbOfTri*3 *5];

    GLint b_VerCnt = 0, b_cnt = 0;
    GLfloat cut = 12.0;
    double base_radius = 0.15; // base radius here!!
    
    double upper = sqrt(radius*radius - base_radius*base_radius);
    double base_height = 0.4-upper;
    
    for (i=0; i<cut; i++){
        double pos = glm::pi<double>() * (i/cut) * 2;
        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper;
        b_Vertices[b_cnt++] = sin(pos) * base_radius;
        b_Vertices[b_cnt++] = 0.0; b_Vertices[b_cnt++] = 0.5;
        b_VerCnt++;


        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper + base_height;
        b_Vertices[b_cnt++] = sin(pos) * base_radius;

        b_Vertices[b_cnt++] = 0.0; b_Vertices[b_cnt++] = 0.0;
        b_VerCnt++;

        pos = glm::pi<double>() * ((i+1)/cut) * 2;
        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper;
        b_Vertices[b_cnt++] = sin(pos) * base_radius;
        
        b_Vertices[b_cnt++] = 0.5; b_Vertices[b_cnt++] = 0.5;
        b_VerCnt++;

        //
        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper + base_height;
        b_Vertices[b_cnt++] = sin(pos) * base_radius;

        b_Vertices[b_cnt++] = 0.5; b_Vertices[b_cnt++] = 0.0;
        b_VerCnt++;
        
        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper;
        b_Vertices[b_cnt++] = sin(pos) * base_radius;

        b_Vertices[b_cnt++] = 0.5; b_Vertices[b_cnt++] = 0.5;
        b_VerCnt++;

        pos = glm::pi<double>() * (i/cut) * 2;
        b_Vertices[b_cnt++] = cos(pos) * base_radius;
        b_Vertices[b_cnt++] = upper + base_height; 
        b_Vertices[b_cnt++] = sin(pos) * base_radius;
        b_Vertices[b_cnt++] = 0.0; b_Vertices[b_cnt++] = 0.0;
        b_VerCnt++;       

    }
    /***************** skybox *****************/
    GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLfloat invertices[] = {
        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
         0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
        -0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,

        -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
         0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
        -0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
        -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,

        -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,
        -0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
        -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
        -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
        -0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,
        -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,

         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,
         0.1f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
         0.1f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,

        -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,
         0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,
         0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,
         0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,
        -0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,
        -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,

        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
        -0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f
    };

    /***************** floor *****************/
    GLfloat floorVertices[]
    {
         -0.8f,  0.4f, -0.8f,  0.0f,  0.0f,
         -0.8f,  0.4f,  0.8f,  0.0f,  2.0f,
          0.8f,  0.4f,  0.8f,  2.0f,  2.0f,

         -0.8f,  0.4f, -0.8f,  0.0f,  0.0f,
          0.8f,  0.4f, -0.8f,  2.0f,  0.0f,
          0.8f,  0.4f,  0.8f,  2.0f,  2.0f,
        
    };

    /***************** bunny *****************/
    vector<glm::vec4> bunny_vertices;
    vector<glm::vec3> bunny_normals;
    vector<GLushort> bunny_elements;
    load_obj("bunny.obj", bunny_vertices, bunny_normals, bunny_elements);
    vector<GLfloat> bunny_info;
    bunny_info.reserve(bunny_vertices.size()*3 + bunny_normals.size()*3);
    for(int i=0;i<bunny_vertices.size();i++)
    {
        bunny_info.push_back(bunny_vertices[i].x*5);
        bunny_info.push_back(bunny_vertices[i].y*5);
        bunny_info.push_back(bunny_vertices[i].z*5);
        bunny_info.push_back(bunny_normals[i].x);
        bunny_info.push_back(bunny_normals[i].y);
        bunny_info.push_back(bunny_normals[i].z);
    }
    cout<<bunny_info.size()<<endl;
    cout<<bunny_elements.size()<<endl;
    GLfloat* bunny_info_array = &bunny_info[0];
    GLushort* bunny_elements_array = &bunny_elements[0];
    cout<<bunny_info[0]<<" "<<bunny_info[1]<<" "<<bunny_info[2]<<endl;


//================================================================================================

    /*****************bunny VBO *****************/
    GLuint bunnyVAO, bunnyVBO, bunnyEBO;
    glGenVertexArrays(1, &bunnyVAO);
    glGenBuffers(1, &bunnyVBO);
    glGenBuffers(1, &bunnyEBO);

    glBindVertexArray(bunnyVAO);

        glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*bunny_info.size(), bunny_info_array, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*bunny_elements.size(), bunny_elements_array, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0);




    /*****************floor VBO *****************/

    GLuint floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindVertexArray(floorVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /*****************base VBO *****************/
    GLuint baseVBO, basecontainerVAO;
    glGenVertexArrays(1, &basecontainerVAO);
    glGenBuffers(1, &baseVBO);
        
    glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
    glBufferData(GL_ARRAY_BUFFER, b_cnt * sizeof(GLfloat), b_Vertices, GL_STATIC_DRAW);

    glBindVertexArray(basecontainerVAO);        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /*****************sphere VBO *****************/

    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cnt * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindVertexArray(containerVAO);        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    /*****************skybox VBO *****************/
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glBindVertexArray(skyboxVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    

    /*****************light VBO *****************/
    GLuint lightVAO;
    GLuint m_vboIndex;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &m_vboIndex);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cntInd * sizeof(GLuint), indices, GL_STATIC_DRAW);
//================================================================================================

    vector<const GLchar*> faces;
    faces.push_back("img/skybox/right.bmp");
    faces.push_back("img/skybox/left.bmp");
    faces.push_back("img/skybox/bottom.bmp");
    faces.push_back("img/skybox/top.bmp");
    faces.push_back("img/skybox/back.bmp");
    faces.push_back("img/skybox/front.bmp");

    // faces.push_back("img/skybox/criminal-impact_ft.bmp");
    // faces.push_back("img/skybox/criminal-impact_bk.bmp");
    // faces.push_back("img/skybox/criminal-impact_dn.bmp");
    // faces.push_back("img/skybox/criminal-impact_up.bmp");
    // faces.push_back("img/skybox/criminal-impact_rt.bmp");
    // faces.push_back("img/skybox/criminal-impact_lf.bmp");

    // faces.push_back("img/skybox/cdruidcove_ft.bmp");
    // faces.push_back("img/skybox/druidcove_bk.bmp");
    // faces.push_back("img/skybox/druidcove_dn.bmp");
    // faces.push_back("img/skybox/druidcove_up.bmp");
    // faces.push_back("img/skybox/druidcove_rt.bmp");
    // faces.push_back("img/skybox/druidcove_lf.bmp");

    GLuint cubemapTexture = loadCubemap(faces);
    GLchar  * base = "img/base_texture_wood.bmp";
    GLuint base_texture = loadBaseTexture(base);
    GLchar *floor = "img/wood_pattern_texture_4.bmp";
    GLuint floorTexture = loadBaseTexture(floor);

    SnowSence snowSence(800, 0.02, 0, 0.3);//number, size, rage, range

    // Game loop
    while (!glfwWindowShouldClose(window)){
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement(snowSence);

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);


        //**************** Draw skybox object**********************
        glDepthMask(GL_FALSE);// Remember to turn depth writing off
        skyboxShader.Use();   
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera.GetViewMatrix()))));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        // glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        

        

        // Use cooresponding shader when setting uniforms/drawing objects
        // glDepthMask(GL_FALSE);
        lightingShader.Use();
        GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(lightingShader.Program, "lightColor");
        GLint lightPosLoc    = glGetUniformLocation(lightingShader.Program, "lightPos");
        GLint viewPosLoc     = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);


        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lightingShader.Program,  "view");
        GLint projLoc  = glGetUniformLocation(lightingShader.Program,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the container (using container's vertex attributes)
        glm::mat4 model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(containerVAO);        
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
         
         // glDrawArrays(GL_TRIANGLES, 0, 3600);
        glDrawElements(GL_TRIANGLE_STRIP, cntInd, GL_UNSIGNED_INT, NULL);
        // glDepthMask(GL_TRUE);
        
        

        
        //**************** Draw inside object**********************
        // glClear(GL_DEPTH_BUFFER_BIT);
        // inShader.Use();
        // // Get the uniform locations
        // modelLoc = glGetUniformLocation(inShader.Program, "model");
        // viewLoc  = glGetUniformLocation(inShader.Program,  "view");
        // projLoc  = glGetUniformLocation(inShader.Program,  "projection");
        // // Pass the matrices to the shader
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // // Draw the container (using container's vertex attributes)
        // glBindVertexArray(incontainerVAO);
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        
        

        //**************** Draw snow object**********************
        // glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        snowShader.Use(); 
        glUniformMatrix4fv(glGetUniformLocation(snowShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(snowShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(snowShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        GLuint center = glGetUniformLocation(snowShader.Program, "center");
        GLuint length = glGetUniformLocation(snowShader.Program, "sideLength");
        snowSence.show(center,length);
        glEnable(GL_DEPTH_TEST);
        

        //**************** Draw floor object**********************
        floorShader.Use();   
        glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(floorVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //**************** Draw base object**********************
        // glClear(GL_DEPTH_BUFFER_BIT);
        baseShader.Use();   
        glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(basecontainerVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, base_texture);
        glDrawArrays(GL_TRIANGLES, 0, 40*2*3);
        glBindVertexArray(0);


        //**************** Draw bunny object**********************
        bunnyShader.Use();
        // Get the uniform locations
        modelLoc = glGetUniformLocation(bunnyShader.Program, "model");
        viewLoc  = glGetUniformLocation(bunnyShader.Program,  "view");
        projLoc  = glGetUniformLocation(bunnyShader.Program,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
        lightColorLoc  = glGetUniformLocation(lightingShader.Program, "lightColor");
        lightPosLoc    = glGetUniformLocation(lightingShader.Program, "lightPos");
        viewPosLoc     = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
        // Draw the container (using container's vertex attributes)
        glBindVertexArray(bunnyVAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, bunny_elements.size()*3, GL_UNSIGNED_SHORT, 0);
        // cout<<bunny_elements.size()*3<<endl;
        glBindVertexArray(0);
        

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}



void do_movement(SnowSence& snowSence)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (keys[GLFW_KEY_1]) //1 randomly
        snowSence.setMode(SNOW_RANDOM);
    if (keys[GLFW_KEY_2]) //2 gravity
        snowSence.setMode(SNOW_GRAVITY);
    if (keys[GLFW_KEY_3]) //1 gravity with wind
        snowSence.setMode(SNOW_GRAVITY_W_WIND);


}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        // image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        // SOIL_free_image_data(image);
        image = readBMP(faces[i], width, height);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        delete image;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureID;
}


GLuint loadBaseTexture(GLchar* b_texture){
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_2D, textureID);
        // image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        // SOIL_free_image_data(image);
    image = readBMP(b_texture, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    delete image;


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void load_obj(const char* filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements)
{
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl; exit(1);
    }

    string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            istringstream s(line.substr(2));
            glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
            vertices.push_back(v);
        }
        else if (line.substr(0,2) == "f ")
        {
            istringstream s(line.substr(2));
            GLushort a,b,c;
            s >> a; s >> b; s >> c;
            a--; b--; c--;
           elements.push_back(a); elements.push_back(b); elements.push_back(c);
        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }

    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (int i = 0; i < elements.size(); i+=3)
    {
        GLushort ia = elements[i];
        GLushort ib = elements[i+1];
        GLushort ic = elements[i+2];
        glm::vec3 normal = glm::normalize(glm::cross(
        glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
        glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
        normals[ia] = normals[ib] = normals[ic] = normal;
    }
}
