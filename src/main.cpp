#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "ssd.h"
#include "volcano.h"
#include "bar.h"
#include "bomb.h"
#include "bullet.h"
#include "ring.h"
#include "cannon.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball plane;
Ground ground;
// SSD speed1;
// SSD speed2;
Bar alt;
Bar lives_bar;
vector <Volcano> volcanoes;
vector <Bomb> bombs;
vector <Ring> rings;
vector <Bullet> bullets;
vector <Cannon> cannons;
int lives;

float screen_zoom = 0.1f, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float eye_x, eye_y, eye_z, t = 90;
float up_x = 0, up_y = 1, up_z = 0;
int camera_view = 0;

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
    if(camera_view == 0)
    {   
        eye_x = plane.position.x + 3.0f * cos( t * M_PI / 180.0f);
        eye_y = plane.position.y + 3.0f;
        eye_z = plane.position.z - 3.0f * sin(t * M_PI / 180.0f);
        up_x = 0;
        up_y = 1;
        up_z = 0;
    }
    if(camera_view == 1)
    {
        eye_x = plane.position.x;
        eye_y = plane.position.y+20;
        eye_z = plane.position.z;
        up_x = 0;
        up_y = 0;
        up_z = 1;
    }
    glm::vec3 eye (eye_x, eye_y, eye_z);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (plane.position.x, plane.position.y, plane.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (up_x, up_y, up_z);
 
    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    glm::mat4 VP1 = Matrices.projection *  glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    plane.draw(VP);
    ground.draw(VP);
    for(vector<Volcano>::iterator volcano = volcanoes.begin();volcano!=volcanoes.end();volcano++) {
        volcano->draw(VP);
    }
    for(vector<Bomb>::iterator bomb = bombs.begin(); bomb != bombs.end();bomb++) {
        bomb->draw(VP);
    }
    for(vector<Ring>::iterator ring = rings.begin(); ring != rings.end() ; ring++) {
        ring->draw(VP);
    }
    for(vector<Bullet>::iterator bullet = bullets.begin(); bullet != bullets.end(); bullet++) {
        bullet->draw(VP);
    }
    for(vector<Cannon>::iterator cannon = cannons.begin(); cannon != cannons.end() ;cannon++)
    {
        cannon->draw(VP);
    }
    // Matrices.view = glm::lookAt( glm::vec3(0,0,-3), glm::vec3(0,0,0),glm::vec3(0,1,0) );
    // Matrices.projection = glm::ortho(-1,1,-1,1);
    // glm::mat4 VP2 = Matrices.projection * Matrices.view;    
    // speed1.draw(VP);
    // speed2.draw(VP);
    alt.draw(VP1);
    lives_bar.draw(VP1);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int c = glfwGetKey(window, GLFW_KEY_C);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    int mouse_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (d) {
        plane.yaw -= 1;
        t = plane.yaw + 90;
    }
    if(a){
        plane.yaw += 1;
        t = plane.yaw + 90;
    }
    if(up) {
        t+=1.0f;
    }
    if(down) {
        t-=1.0f;
    }
    if(left || a) {
        plane.roll = plane.roll <= -30 ? -30 : plane.roll - 1;
    }
    if(right || d ) {
        plane.roll = plane.roll >= 30 ? 30 : plane.roll + 1;
    }
    if(w) {
        plane.pitch = plane.pitch <= -30 ? -30 : plane.pitch - 0.2f;
        plane.position.y += 0.1f;
    }
    else if(s) {
        plane.pitch = plane.pitch >= 30 ? 30 : plane.pitch + 0.2f;
        plane.position.y -= 0.1f;
    }
    else
    {
        plane.pitch = plane.pitch >= 0 ? plane.pitch + 0.2f : 0;
        plane.pitch = plane.pitch <= 0 ? plane.pitch - 0.2f : 0;
    }
    
    // if(w) {
    //     plane.speed.x = 0.5 * sin(plane.yaw*M_PI / 180.0f);
    //     plane.speed.z = 0.5 * cos(plane.yaw*M_PI / 180.0f);
    // }
    if(shift) {
        bombs.push_back(Bomb(plane.position.x,plane.position.y,plane.position.z)); 
        cout<<"Bomb dropped"<<endl;
    }
    if(mouse_left) {
        cout<<"Left"<<endl;
        bullets.push_back(Bullet(plane.position.x,plane.position.y,plane.position.z,plane.yaw));
    }
    if(c) {
        camera_view = (camera_view+1)%2;
    }
}

void tick_elements() {
    plane.tick();
    plane.speed.x = 0.5 * sin(plane.yaw*M_PI / 180.0f);
    plane.speed.z = 0.5 * cos(plane.yaw*M_PI / 180.0f);
    // speed1.set_position(plane.position.x+2,plane.position.y+2,plane.position.z+3);
    // speed2.set_position(plane.position.x,plane.position.y+2,plane.position.z+3);
    // alt.set_position(plane.position.x,plane.position.y-4, plane.position.z);
    // alt.yaw = plane.yaw;
    int mag_speed = int(100 * sqrt(plane.speed.x*plane.speed.x + plane.speed.y*plane.speed.y + plane.speed.z*plane.speed.z));
    // speed1.set_score(mag_speed%10);
    // speed2.set_score((mag_speed/10)%10);
    alt.set_score(plane.position.y-10);
    lives_bar.set_score(30-lives);
    for(vector<Bomb>::iterator bomb=bombs.begin();bomb!=bombs.end();bomb++) {
        bomb->tick();
    }
    for(vector<Bullet>::iterator bullet = bullets.begin(); bullet!=bullets.end(); bullet++) {
        bullet->tick();
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    plane = Ball(0, 0, 0);
    ground = Ground(0,-10.0f,0);
    // speed1 = SSD(0,0);
    // speed2 = SSD(2,0);
    alt = Bar(-3,-3,0,10,COLOR_LAVAYELLOW);
    lives_bar = Bar(-3,-2,0,30,COLOR_BLUE);
    for(int i=0;i<50;i++) {
        volcanoes.push_back(Volcano(rand()%1000-500,rand()%500-250));
    }
    for(int i=0;i<30;i++) {
        rings.push_back(Ring(rand()%1000-50,rand()%20,rand()%500-250));
    }
    for(int i=0;i<15;i++) {
        cannons.push_back(Cannon(rand()%1000-50,rand()%500-250));
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
            check_collisions();
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
    // if(camera_view == 0)
    Matrices.projection = glm::perspective(float(M_PI_2), 1.0f, 0.1f, 500.0f);
    // if(camera_view == 1)
    //     Matrices.projection = glm::ortho(left,right,bottom,top,0.1f,100.0f);

}

void check_collisions() { 
    for(vector<Volcano>::iterator volcano = volcanoes.begin();volcano!=volcanoes.end();volcano++) {
        if(plane.position.y < 10  && sqrt((plane.position.x - volcano->position.x)*(plane.position.x - volcano->position.x)+(plane.position.z - volcano->position.z)*(plane.position.z - volcano->position.z))<8){
            cout<<"Quit "<<lives++<<endl;
            // quit(window);
        }
    }
    for(vector<Ring>::iterator ring = rings.begin();ring!=rings.end();ring++)
    {
        if(sqrt((plane.position.y - ring->position.y)*(plane.position.y - ring->position.y)+(plane.position.x - ring->position.x)*(plane.position.x - ring->position.x)+(plane.position.z - ring->position.z)*(plane.position.z - ring->position.z)) < 8) {
            lives--;
            rings.erase(ring);
            ring--;
        }
    }
}