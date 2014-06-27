#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 colAttr;
varying vec4 col;
void main()
{
    col = colAttr;
    gl_Position = ftransform();
}
