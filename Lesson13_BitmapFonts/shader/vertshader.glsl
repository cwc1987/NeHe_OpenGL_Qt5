#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

attribute vec4 coord;
uniform mat4 mvpMatrix;

varying vec2 texcoord;
void main()
{
  gl_Position = mvpMatrix * vec4(coord.xy, 0, 1);
  texcoord = coord.zw;
}
