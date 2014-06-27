#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 posAttr;
uniform mat4 mvpMatrix;
void main()
{
    gl_Position = mvpMatrix * posAttr;
}
