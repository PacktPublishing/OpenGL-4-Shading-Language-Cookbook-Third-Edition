#include "plane.h"
#include "glutils.h"
#include "cookbookogl.h"
#include "glutils.h"

#include <cstdio>
#include <cmath>

Plane::Plane(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax)
{
	int nPoints = (xdivs + 1) * (zdivs + 1);
    std::vector<GLfloat> p(3 * nPoints);
	std::vector<GLfloat> n(3 * nPoints);
    std::vector<GLfloat> tex(2 * nPoints);
	std::vector<GLfloat> tang(4 * nPoints);
    std::vector<GLuint> el(6 * xdivs * zdivs);

    float x2 = xsize / 2.0f;
    float z2 = zsize / 2.0f;
    float iFactor = (float)zsize / zdivs;
    float jFactor = (float)xsize / xdivs;
    float texi = smax / xdivs;
    float texj = tmax / zdivs;
    float x, z;
    int vidx = 0, tidx = 0;
    for( int i = 0; i <= zdivs; i++ ) {
        z = iFactor * i - z2;
        for( int j = 0; j <= xdivs; j++ ) {
            x = jFactor * j - x2;
            p[vidx] = x;
            p[vidx+1] = 0.0f;
            p[vidx+2] = z;
			n[vidx] = 0.0f;
			n[vidx+1] = 1.0f;
			n[vidx+2] = 0.0f;

            tex[tidx] = j * texi;
            tex[tidx+1] = (zdivs - i) * texj;

            vidx += 3;
            tidx += 2;
        }
    }

	for (int i = 0; i < nPoints; i++) {
		tang[i * 4 + 0] = 1.0f;
		tang[i * 4 + 1] = 0.0f;
		tang[i * 4 + 2] = 0.0f;
		tang[i * 4 + 3] = 1.0f;
	}

    GLuint rowStart, nextRowStart;
    int idx = 0;
    for( int i = 0; i < zdivs; i++ ) {
        rowStart = (GLuint)( i * (xdivs+1) );
        nextRowStart = (GLuint)( (i+1) * (xdivs+1));
        for( int j = 0; j < xdivs; j++ ) {
            el[idx] = rowStart + j;
            el[idx+1] = nextRowStart + j;
            el[idx+2] = nextRowStart + j + 1;
            el[idx+3] = rowStart + j;
            el[idx+4] = nextRowStart + j + 1;
            el[idx+5] = rowStart + j + 1;
            idx += 6;
        }
    }

    initBuffers(&el, &p, &n, &tex, &tang);
}
