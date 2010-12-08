//Fragment Shader

//Inputs from vertex shader
varying vec3 surfaceNormal;
varying vec3 vertexPosition;
varying vec4 projShadow;

//Built-in texture sampler
uniform sampler2D texture;
uniform sampler2DShadow shadowMap;

int numLights = 3;	//We'll use a uniform var later

void main()
{
	
	//Get texture color
	vec4 textureColor = texture2D(texture, gl_TexCoord[0]);
	//textureColor *= shadow2DProj(shadowMap, projShadow).r;

	surfaceNormal = normalize(surfaceNormal);

	vec3 globalAmbientTerm = vec3(0,0,0);
	vec3 ambientTerm = vec3(0,0,0);
	vec3 diffuseTerm = vec3(0,0,0);
	vec3 specularTerm = vec3(0,0,0);

	//Calculate global ambient term
	globalAmbientTerm = gl_FrontMaterial.ambient.xyz * gl_LightModel.ambient * textureColor;

	//Calculate per light contributions
	for (int i=0; i < numLights; i++)
	{
		vec3 lightDirection = normalize(gl_LightSource[i].position.xyz - vertexPosition);
		vec3 eyeDirection = normalize(-vertexPosition);
			vec3 halfVector = normalize(lightDirection + eyeDirection);

		//Calculate ambient term
		ambientTerm += gl_FrontMaterial.ambient.xyz * gl_LightSource[i].ambient.xyz * textureColor;

		//Calculate diffuse term
		diffuseTerm += gl_FrontMaterial.diffuse.xyz * gl_LightSource[i].diffuse.xyz * textureColor * clamp(dot(lightDirection, surfaceNormal), 0.0, 1.0);

		//Blinn-Phong Shading specular term
		specularTerm += gl_FrontMaterial.specular.xyz * gl_LightSource[i].specular.xyz * pow( max(dot(surfaceNormal, halfVector), 0.0), gl_FrontMaterial.shininess);

	}

	//Sum of all the lights
	gl_FragColor = vec4(globalAmbientTerm, 1) + vec4(ambientTerm, 1) + vec4(diffuseTerm, 1) + vec4(specularTerm, 1);
}



