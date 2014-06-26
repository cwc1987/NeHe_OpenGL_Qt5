#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec2 a_texcoord;
varying vec2 v_texcoord;
varying vec3 lightDir,normal;
void main()
{
    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(vec3(gl_LightSource[0].position));

    gl_Position = ftransform();
    v_texcoord = a_texcoord;
}
