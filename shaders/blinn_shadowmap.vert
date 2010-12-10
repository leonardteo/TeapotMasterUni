#version 120

//Lame attempt to get ready for version #150 haha...
in vec3 vertex;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

varying vec3 surfaceNormal;
varying vec3 vertexPosition;

//varying vec4 shadowCoord;

void main()
{

	//straight transform to send out of shader
	//gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1);
	gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;	//Unfortunately we have a few objects that still need this depracated code

	//Texture Mapping
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
		
	//Get vertex unprojected
	vertexPosition = vec3(modelViewMatrix * vec4(vertex, 1));

	//Get the surface normal
	surfaceNormal = gl_NormalMatrix * gl_Normal;

	//Calculate vertex position in eye space
	vertexPosition = vec3(modelViewMatrix * vec4(vertex, 1));

	//Shadow mapping
	//vec4 texCoord = gl_TextureMatrix[7] * gl_Vertex;

}