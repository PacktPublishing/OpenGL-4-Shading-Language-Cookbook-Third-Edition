#include "grid.h"

Grid::Grid(float size, int nDivisions) {
    float size2 = size / 2.0f;
    float divisionSize = size / nDivisions;
    nVerts = 4 * (nDivisions + 1);
    std::vector<GLfloat> p;

    GLfloat x, y = 0.0f, z;
    for( int row = 0; row <= nDivisions; row++ ) {
        GLfloat z = (row * divisionSize) - size2;
        p.push_back(-size2);
        p.push_back(0);
        p.push_back(z);
        p.push_back(size2);
        p.push_back(0);
        p.push_back(z);
    }

    for( int col = 0; col <= nDivisions; col++ ) {
        GLfloat x = (col * divisionSize) - size2;
        p.push_back(x);
        p.push_back(0);
        p.push_back(-size2);
        p.push_back(x);
        p.push_back(0);
        p.push_back(size2);
    }

    GLuint posBuf = 0;
    glGenBuffers(1, &posBuf);
    buffers.push_back(posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glBufferData(GL_ARRAY_BUFFER, p.size() * sizeof(GLfloat), p.data(), GL_STATIC_DRAW);

    glGenVertexArrays( 1, &vao );
    glBindVertexArray(vao);
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position
    glBindVertexArray(0);
}

Grid::~Grid() {
    if( buffers.size() > 0 ) {
        glDeleteBuffers( (GLsizei)buffers.size(), buffers.data() );
        buffers.clear();
    }

    if( vao != 0 ) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

void Grid::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, nVerts);
    glBindVertexArray(0);
}
