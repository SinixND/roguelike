// https://raw.githubusercontent.com/raysan5/raylib/master/examples/shaders/resources/shaders/glsl330/base.vs

// VERTEX SHADER
// Transforms verticies ("edges of triangles")

// VAO (vertex array object)
// Stores where to find the data for each input (vertex) attribute

// VAO is needed BEFORE VBO!

// VBO (vertex buffer object)
// This is an object that holds/buffers verticies to be passed to the vertex shader

// Required 
// GLSL version
#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;

// NOTE: Add here your custom variables

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // Reqired
    // Calculate final vertex position
    gl_POSITION_ = mvp*vec4(vertexPosition, 1.0);
}
