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


int count = 0;




vec3 norm(vec3 v1,vec3 v2,vec3 v3)
{
    return cross(v2-v1,v3-v1);
}

bool validCheck(float x, float z)
{
   return  (x < 0 || z < 0 || z > heightTexture+1 || x > widthTexture+1 ) ; 

}
void normalFunc(float x, float y, float z)
{   
    vec3 sumVector = vec3(0.0f,0.0f,0.0f);
    vec3 pos = vec3(x,y,z);
    vec3 north,northeast,east,west,southwest,south;

    bool arr[6] ;
    for (int i = 0; i < 6; ++i)
    {
        arr[i] = false;
    }
    
    if(!validCheck(x,z-1))
    {   
        
        float a,b;
        a = 1-x/(1.0f*widthTexture);
        b = 2-z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate); 
        north = vec3(x,y,z-1);
        north.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);
        arr[0] = true;
    }
    if(!validCheck(x+1,z-1))
    {
        float a;
        float b;
        a = -x/(1.0f*widthTexture);
        b = 2-z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate);
        northeast = vec3(x+1,y,z-1);

        northeast.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);
        arr[1] = true;
    }
    if(!validCheck(x+1,z))
    {
        float a,b;
        a = -x/(1.0f*widthTexture);
        b = 1-z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate);

        east = vec3(x+1,y,z);
        east.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);

        arr[2] = true;
    }    
    if(!validCheck(x,z+1))
    {
        float a,b;
        a = 1-x/(1.0f*widthTexture);
        b = -z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate);


        south = vec3(x,y,z+1);
        south.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);
        arr[3] = true;        
    }
    if(!validCheck(x-1,z+1))
    {
        float a,b;
        a = 2-x/(1.0f*widthTexture);
        b = -z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate);

        southwest = vec3(x-1,y,z+1);
        southwest.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);
        arr[4] = true;
    }
    if(!validCheck(x-1,z))
    {
        float a,b;
        a = 2-x/(1.0f*widthTexture);
        b = 1-z/(1.0f*heightTexture);
        textureCoordinate = vec2(a,b);
        vec4 colorValues = texture(rgbTexture,textureCoordinate);

        west = vec3(x-1,y,z);
        west.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);
        arr[5] = true;
    }

    // edges
    // north - x,y,z ,northeast - x,y,z / northeast - x,y,z ,east - x,y,z / east - x,y,z ,south - x,y,z/ 
    // south - x,y,z ,south west - x,y,z/ southwest - x,y,z ,west - x,y,z/  west - x,y,z ,north - x,y,z/ 


    if(arr[0] && arr[1])
    {
        sumVector = sumVector + norm(pos,north,northeast);
        count++;
    }
    if(arr[1] && arr[2])
    {
        sumVector = sumVector + norm(pos,northeast,east);
        count++;
    }

    if(arr[2] && arr[3])
    {    
        sumVector = sumVector + norm(pos,east,south);
        count++;
    }  
    if(arr[3] && arr[4])
    {
        sumVector = sumVector + norm(pos,south,southwest);
        count++;
    }    
    if(arr[4] && arr[5])
    {
        sumVector = sumVector + norm(pos,southwest,west); 
        count++;
    }    

    if(arr[5] && arr[0])
    {
        sumVector = sumVector + norm(pos,west,north);
        count++;
    }   



   vertexNormal = sumVector/(1.0f*count); 

   normalize(vertexNormal);

}





void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
   
   // set gl_Position variable correctly to give the transformed vertex position

	
    vec4 pos = vec4(position,1.0f); // this is a placeholder. It does not correctly set the position 
    // normal calculation vertexNormal;

    float x,y;
    x = 1-pos.x/(1.0f*widthTexture);
    y = 1-pos.z/(1.0f*heightTexture);
    textureCoordinate = vec2(x,y);
	vec4 colorValues = texture(rgbTexture,textureCoordinate); 
    pos.y = heightFactor*(0.2126*colorValues.x +0.7152*colorValues.y+0.0722*colorValues.z);

    normalFunc(pos.x,pos.y,pos.z);



    ToLightVector = -vec3(widthTexture/2,widthTexture+heightTexture,heightTexture/2)+pos.xyz;
    ToCameraVector = vec3(cameraPosition)-pos.xyz;

    gl_Position = MVP*pos;



}
