#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
void main()
{
    vec3 normal, lightDir;
    vec4 diffuse;
    float NdotL;

    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(vec3(gl_LightSource[0].position));
    NdotL = max(dot(normal, lightDir), 0.0);
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    gl_FrontColor =  NdotL * diffuse;
    gl_Position = mvp_matrix * gl_Vertex;
    v_texcoord = a_texcoord;
} 
