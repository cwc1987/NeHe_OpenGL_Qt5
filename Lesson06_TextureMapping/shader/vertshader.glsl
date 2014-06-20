#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
void main()
{
    gl_Position = mvp_matrix * gl_Vertex;
    v_texcoord = a_texcoord;
}
