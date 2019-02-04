#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "ssd.h"
#include "volcano.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball plane;
Ground ground;
SSD speed1;
SSD speed2;
SSD alt;
vector <Volcano> volcanoes;


float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float eye_x, eye_y, eye_z, t=90;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    eye_x = plane.position.x + 3.0f * cos( t * M_PI / 180.0f);
    eye_y = plane.position.y + 3.0f;
    eye_z = plane.position.z - 3.0f * sin(t * M_PI / 180.0f);
    glm::vec3 eye (eye_x, eye_y, eye_z);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (plane.position.x, plane.position.y, plane.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    plane.draw(VP);
    ground.draw(VP);
    speed1.draw(VP);
    speed2.draw(VP);
    alt.draw(VP);
    for(vector<Volcano>::iterator volcano = volcanoes.begin();volcano!=volcanoes.end();volcano++) {
        volcano->draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    if (right) {
        plane.pitch -= 1;
    }
    if(left){
        plane.pitch += 1;
        
    }
    if(up) {
        t+=1.0f;
    }
    if(down) {
        t-=1.0f;
    }
    if(a) {
        plane.yaw = plane.yaw <= -30 ? -30 : plane.yaw - 1;
    }
    if(d) {
        plane.yaw = plane.yaw >= 30 ? 30 : plane.yaw + 1;
    }
    if(space) {
        plane.roll = plane.roll <= -30 ? -30 : plane.roll - 0.2f;
        plane.position.y += 0.1f;
    }
    else
    {
        plane.roll = plane.roll >= 0 ? 0 : plane.roll + 0.2f;
    }
    
    if(w) {
        plane.speed.x = 0.5 * sin(plane.pitch*M_PI / 180.0f);
        plane.speed.z = 0.5 * cos(plane.pitch*M_PI / 180.0f);
    }
    // else{
    //     plane.speed = glm::vec3(0,0,0);
    // }
}

void tick_elements() {
    plane.tick();
    plane.speed.x = 0.5 * sin(plane.pitch*M_PI / 180.0f);
    plane.speed.z = 0.5 * cos(plane.pitch*M_PI / 180.0f);
    speed1.set_position(plane.position.x+2,plane.position.y+2,plane.position.z+3);
    speed2.set_position(plane.position.x,plane.position.y+2,plane.position.z+3);
    alt.set_position(plane.position.x-3,plane.position.y+2, plane.position.z+3);
    int mag_speed = int(100 * sqrt(plane.speed.x*plane.speed.x + plane.speed.y*plane.speed.y + plane.speed.z*plane.speed.z));
    speed1.set_score(mag_speed%10);
    speed2.set_score((mag_speed/10)%10);
    alt.set_score(int(plane.position.y)%10);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    plane = Ball(0, 0, 0);
    ground = Ground(0,-10.0f,0);
    speed1 = SSD(0,0);
    speed2 = SSD(2,0);
    alt = SSD(-1,0);
    for(int i=0;i<10;i++) {
        volcanoes.push_back(Volcano(rand()%1000,rand()%500));
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1080;
    int height = 1080;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }
        if(plane.position.y == -9){
            quit(window);
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::perspective(float(M_PI_2), 1.0f, 0.1f, 500.0f);
}
