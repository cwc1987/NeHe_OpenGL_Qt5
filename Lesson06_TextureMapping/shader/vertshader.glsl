#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvpMatrix;
attribute vec4 posAttr;
attribute vec2 texCoordAttr;
varying vec2 texCoord;
void main()
{
    gl_Position = mvpMatrix * posAttr;
    texCoord = texCoordAttr;
}
