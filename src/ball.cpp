#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static GLfloat vertex_buffer_data[3200];
    for(int i=0;i<100;i++){
        vertex_buffer_data[18*i] = 1*(cos(2*M_PI*i/100));;
		vertex_buffer_data[18*i+1] = 1*(sin(2*M_PI*i/100));;
		vertex_buffer_data[18*i+2] = 0.0f;
		vertex_buffer_data[18*i+3] = 1*(cos(2*M_PI*i/100));
		vertex_buffer_data[18*i+4] = 1*(sin(2*M_PI*i/100));
		vertex_buffer_data[18*i+5] = 1.0f;
		vertex_buffer_data[18*i+6] = 1*(cos(2*M_PI*(i+1)/100));
		vertex_buffer_data[18*i+7] = 1*(sin(2*M_PI*(i+1)/100));
		vertex_buffer_data[18*i+8] = 1.0f;

        vertex_buffer_data[18*i+9] = 1*(cos(2*M_PI*(i+1)/100));;
		vertex_buffer_data[18*i+10] = 1*(sin(2*M_PI*(i+1)/100));;
		vertex_buffer_data[18*i+11] = 1.0f;
		vertex_buffer_data[18*i+12] = 1*(cos(2*M_PI*i/100));
		vertex_buffer_data[18*i+13] = 1*(sin(2*M_PI*i/100));
		vertex_buffer_data[18*i+14] = 0.0f;
		vertex_buffer_data[18*i+15] = 1*(cos(2*M_PI*(i+1)/100));
		vertex_buffer_data[18*i+16] = 1*(sin(2*M_PI*(i+1)/100));
		vertex_buffer_data[18*i+17] = 0.0f;
    }
    for(int i=0;i<100;i++){
        vertex_buffer_data[1800+9*i] = 0.0f;
		vertex_buffer_data[1800+9*i+1] = 0.0f;
		vertex_buffer_data[1800+9*i+2] = 2.0f;
		vertex_buffer_data[1800+9*i+3] = 1*(cos(2*M_PI*i/100));
		vertex_buffer_data[1800+9*i+4] = 1*(sin(2*M_PI*i/100));
		vertex_buffer_data[1800+9*i+5] = 1.0f;
		vertex_buffer_data[1800+9*i+6] = 1*(cos(2*M_PI*(i+1)/100));
		vertex_buffer_data[1800+9*i+7] = 1*(sin(2*M_PI*(i+1)/100));
		vertex_buffer_data[1800+9*i+8] = 1.0f;
    }
    this->object = create3DObject(GL_TRIANGLES, 400*3, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Ball::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

