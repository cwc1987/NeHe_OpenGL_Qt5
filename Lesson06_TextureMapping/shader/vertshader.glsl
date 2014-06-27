#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
