#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

varying vec3 lightDir,normal;
void main()
{
    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(vec3(gl_LightSource[0].position));

    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
