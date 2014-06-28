#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec4 starColor;
varying vec2 texCoord;
void main()
{
    vec3 ct,cf;
    vec4 texel;
    float at,af;

    cf = starColor.rgb;
    af = starColor.a;

    texel = texture2D(texture, texCoord);
    ct = texel.rgb;
    at = texel.a;

    gl_FragColor = vec4(ct * cf, at * af);
}
