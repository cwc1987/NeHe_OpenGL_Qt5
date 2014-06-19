//attribute highp vec4 posAttr;
//attribute lowp vec4 colAttr;
//varying lowp vec4 col;
//uniform highp mat4 matrix;
//void main()
//{
//    col = colAttr;
//    gl_Position = matrix * posAttr;
//}
attribute vec4 colAttr;
varying vec4 col;
void main()
{
    col = colAttr;
    gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
}
