//Blinn Shader
varying vec3 surfaceNormal;
varying vec3 vertexPosition;
varying vec4 projShadow;

void main()
{
	//straight transform
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//Texture Mapping
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
		
	//Get vertex unprojected
	vertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	//Get the surface normal
	surfaceNormal = gl_NormalMatrix * gl_Normal;

	//Calculate vertex position
	vertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	//projShadow = gl_TextureMatrix[1] * gl_Vertex;

}