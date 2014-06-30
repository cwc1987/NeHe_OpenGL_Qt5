#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif


uniform sampler2D texture;
uniform vec4 matrialColor;

varying vec2 texCoord;
varying vec4 lightColor;
void main()
{
    vec4 texel;
    vec3 ct,cf;
    float at,af;

    texel = texture2D(texture, texCoord);
    ct = texel.rgb;
    at = texel.a;
    cf = matrialColor.rgb;
    af = matrialColor.a;
    texel = vec4(ct * cf, at * af);

    ct = texel.rgb;
    at = texel.a;
    cf = lightColor.rgb;
    af = lightColor.a;
    gl_FragColor = vec4(ct * cf, at * af);
}
