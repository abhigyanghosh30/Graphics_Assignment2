#include "indicator.h"
#include "main.h"

Indicator::Indicator(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    static GLfloat vertex_buffer_data[2700];
    
    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Indicator::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}