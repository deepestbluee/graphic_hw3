#version 410

layout(location = 0) in vec3 position;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
   
   // set gl_Position variable correctly to give the transformed vertex position

	gl_Position = vec4(position,0); // this is a placeholder. It does not correctly set the position 
	
	GLfloat x,y;
	x = 1-gl_Position.x/((float)widthTexture);
	y = 1-gl_Position.z/((float)heightTexture);
	textureCoordinate = vec2(x,y);

	vec3 colorValues = texture(rgbTexture,textureCoordinate); 
    gl_Position.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);


    // normal calculation vertexNormal;

    ToLightVector = -vec3(widthTexture/2,widthTexture+heightTexture,heightTexture/2)+gl_Position.xyz;
    ToCameraVector = vec3(cameraPosition)-gl_Position.xyz;

}
