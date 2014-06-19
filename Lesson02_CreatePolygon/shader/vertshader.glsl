#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
void main()
{
    gl_Position = mvp_matrix * gl_Vertex;
}
