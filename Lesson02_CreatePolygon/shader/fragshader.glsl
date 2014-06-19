#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

void main()
{
    // Set fragment color from texture
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
