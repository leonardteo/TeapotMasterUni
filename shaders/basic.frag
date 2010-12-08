//Fragment Shader

//Inputs from vertex shader
varying vec3 halfVector;
varying vec3 lightDirection;
varying vec3 surfaceNormal;
varying vec3 vertexPosition;

//Built-in texture sampler
uniform sampler2D texture;


void main()
{
	
	//Get texture color
	vec4 textureColor = texture2D(texture, gl_TexCoord[0]);

	//Normalize all input vectors
	surfaceNormal = normalize(surfaceNormal);
	//lightDirection = normalize(lightDirection);
	lightDirection = normalize(gl_LightSource[0].position.xyz - vertexPosition);
	vec3 eyeDirection = normalize(-vertexPosition);

	halfVector = normalize(halfVector);

	//Calculate diffuse term
	//vec4 diffuseTerm = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * textureColor * clamp(dot(lightDirection, surfaceNormal), 0.0, 1.0);
	vec3 diffuseTerm = gl_FrontMaterial.diffuse.xyz * gl_LightSource[0].diffuse.xyz * textureColor * clamp(dot(lightDirection, surfaceNormal), 0.0, 1.0);

	//Blinn-Phong Shading specular term
	//vec3 specularTerm = gl_FrontMaterial.specular.xyz * gl_LightSource[0].specular.xyz * pow( max(dot(surfaceNormal, halfVector), 0.0), gl_FrontMaterial.shininess);

	//Phong Shading
	vec3 reflectionVector = normalize(-reflect(lightDirection, surfaceNormal));
	vec3 specularTerm = gl_FrontMaterial.specular.xyz * gl_LightSource[0].specular.xyz * pow( max(dot(reflectionVector, eyeDirection), 0.0), gl_FrontMaterial.shininess);

	gl_FragColor = vec4(diffuseTerm, 1) + vec4(specularTerm, 1);
}



