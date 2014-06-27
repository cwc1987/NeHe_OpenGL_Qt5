#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 colAttr;
attribute vec4 posAttr;
varying vec4 col;
uniform mat4 mvpMatrix;
void main()
{
    col = colAttr;
    gl_Position = mvpMatrix * posAttr;
}
