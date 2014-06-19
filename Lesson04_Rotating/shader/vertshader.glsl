#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 colAttr;
varying vec4 col;
uniform mat4 mvp_matrix;
void main()
{
    col = colAttr;
    gl_Position = mvp_matrix * gl_Vertex;
}
