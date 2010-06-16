
#include "cinder/gl/gl.h"
#include "cinder/AxisAlignedBox.h"

#include "aaBox.h"

namespace Atelier {
    AABox::AABox() : ci::AxisAlignedBox3f() {
    }

    AABox::AABox(const Vec3D& min_vec, const Vec3D& max_vec) :
        ci::AxisAlignedBox3f(min_vec, max_vec) {
    }

    void AABox::glDraw() {
        for(int i = 0; i < 8; ++i) {
            ci::gl::drawCube( mVerts[i], Vec3D(0.5f, 0.5f, 0.5f));
        }
    }

    /* I don't know how to draw generate the correct lines from th 8 vectors
    void AABox::glDraw() {
        float line_verts[3*8];

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, line_verts);

        for(int i = 0, j = 0; i < 3*8; i += 3) {
            line_verts[i] = mVerts[j].x;
            line_verts[i+1] = mVerts[j].y;
            line_verts[i+2] = mVerts[j].z;
            ++j;
        }

        glDrawArrays(GL_LINES, 0, 8);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    */
}