#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif


uniform sampler2D texture;
uniform bool enableLight;

varying vec2 texCoord;
varying vec4 frontColor;
void main()
{
    vec4 color;
    color = texture2D(texture, texCoord);
    if(enableLight)
    {
        vec3 ct,cf;
        vec4 texel;
        float at,af;

        cf = frontColor.rgb;
        af = frontColor.a;

        texel = texture2D(texture, texCoord);
        ct = texel.rgb;
        at = texel.a;
        color = vec4(ct * cf, at * af);
    }
    gl_FragColor = color;
}
