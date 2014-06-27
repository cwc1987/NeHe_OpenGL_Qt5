#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform bool enableLight;
varying vec3 lightDir,normal;
void main()
{
    if(enableLight)
    {
        vec3 ct,cf;
        vec4 texel;
        float intensity, at, af;

        intensity = max(dot(lightDir,normalize(normal)),0.0);
        cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
        af = gl_FrontMaterial.diffuse.a;

        texel = texture2D(texture, gl_TexCoord[0].st);
        ct = texel.rgb;
        at = texel.a;

        gl_FragColor = vec4(ct * cf, at * af);
    }
    else
    {
        gl_FragColor = texture2D(texture,gl_TexCoord[0].st);
    }
}
