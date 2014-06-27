#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

void main()
{
    gl_Position = ftransform();
}
