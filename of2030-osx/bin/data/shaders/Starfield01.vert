// uniform mat4 modelViewProjectionMatrix;
// in vec4 position;

void main(void)
{
   // vec4 pos = gl_Vertex;
   // vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   // gl_Position = modelViewProjectionMatrix * position;
}
