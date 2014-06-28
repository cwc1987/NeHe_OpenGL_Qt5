#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 texCoord;
void main()
{
    gl_FragColor = texture2D(texture, texCoord);
}
