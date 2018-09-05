#include "skybox.h"

#include "cookbookogl.h"
#include <vector>

SkyBox::SkyBox(float size)
{
    float side2 = size * 0.5f;
    std::vector<GLfloat> v = {
        // Front
       -side2, -side2, side2,
        side2, -side2, side2,
        side2,  side2, side2,
       -side2,  side2, side2,
       // Right
        side2, -side2, side2,
        side2, -side2, -side2,
        side2,  side2, -side2,
        side2,  side2, side2,
       // Back
       -side2, -side2, -side2,
       -side2,  side2, -side2,
        side2,  side2, -side2,
        side2, -side2, -side2,
       // Left
       -side2, -side2, side2,
       -side2,  side2, side2,
       -side2,  side2, -side2,
       -side2, -side2, -side2,
       // Bottom
       -side2, -side2, side2,
       -side2, -side2, -side2,
        side2, -side2, -side2,
        side2, -side2, side2,
       // Top
       -side2,  side2, side2,
        side2,  side2, side2,
        side2,  side2, -side2,
       -side2,  side2, -side2
    };

    // We don't shade a sky box, so normals aren't used.
    std::vector<GLfloat> n(v.size(), 0.0f);

    std::vector<GLuint> el = {
        0,2,1,0,3,2,
        4,6,5,4,7,6,
        8,10,9,8,11,10,
        12,14,13,12,15,14,
        16,18,17,16,19,18,
        20,22,21,20,23,22
    };

    initBuffers(&el, &v, &n);
}
