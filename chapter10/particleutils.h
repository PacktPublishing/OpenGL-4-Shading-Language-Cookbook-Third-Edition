#pragma once

#include "random.h"
#include "cookbookogl.h"
#include <glm/glm.hpp>

class ParticleUtils {

public:
    /**
     * Return a rotation matrix that rotates the y-axis to be parallel to dir.
     *
     * @param dir
     * @return
     */
    static glm::mat3 makeArbitraryBasis( const glm::vec3 & dir ) {
        glm::mat3 basis;
        glm::vec3 u, v, n;
        v = dir;
        n = glm::cross(glm::vec3(1,0,0), v);
        if( glm::length(n) < 0.00001f ) {
            n = glm::cross(glm::vec3(0,1,0), v);
        }
        u = glm::cross(v,n);
        basis[0] = glm::normalize(u);
        basis[1] = glm::normalize(v);
        basis[2] = glm::normalize(n);
        return basis;
    }

    /**
     * Creates a 1D texture of random floating point values in the range [0, 1)
     * @param size number of values
     * @return the texture id
     */
    static GLuint createRandomTex1D( int size ) {
        Random rand;

        std::vector<GLfloat> randData(size);
        for( int i = 0; i < randData.size(); i++ ) {
            randData[i] = rand.nextFloat();
        }

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_1D, texId);
        glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return texId;
    }
};
