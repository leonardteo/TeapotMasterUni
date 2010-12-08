//Blinn Shader
varying vec3 halfVector;
varying vec3 lightDirection;
varying vec3 surfaceNormal;
varying vec3 vertexPosition;

void main()
{
	//straight transform
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//Texture Mapping
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
		
	//Get vertex unprojected
	vertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	//Get half vector
	halfVector = gl_LightSource[0].halfVector.xyz;

	//Get the light direction
	lightDirection = gl_LightSource[0].position - vertexPosition;

	//Get the surface normal
	surfaceNormal = gl_NormalMatrix * gl_Normal;

	vertexPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

}