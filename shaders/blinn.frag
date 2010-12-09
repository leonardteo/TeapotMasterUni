#version 120

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
	vec4 textureColor = texture2D(texture, gl_TexCoord[0].st);
	//textureColor *= shadow2DProj(shadowMap, projShadow).r;

	vec3 normal = normalize(surfaceNormal);		//Ridiculous -- you can't assign to varying variable

	vec4 globalAmbientTerm;
	vec4 ambientTerm;
	vec4 diffuseTerm;
	vec4 specularTerm;

	//Calculate global ambient term
	globalAmbientTerm = gl_FrontMaterial.ambient.rgba * gl_LightModel.ambient.rgba * textureColor.rgba;

	//Calculate per light contributions
	for (int i=0; i < numLights; i++)
	{
		vec3 lightDirection = normalize(gl_LightSource[i].position.xyz - vertexPosition);
		vec3 eyeDirection = normalize(-vertexPosition);
		vec3 halfVector = normalize(lightDirection + eyeDirection);

		//Calculate ambient term
		ambientTerm += gl_FrontMaterial.ambient.rgba * gl_LightSource[i].ambient.rgba * textureColor.rgba;

		//Calculate diffuse term
		diffuseTerm += gl_FrontMaterial.diffuse.rgba * gl_LightSource[i].diffuse.rgba * textureColor.rgba * clamp(dot(lightDirection, normal), 0.0, 1.0);

		//Blinn-Phong Shading specular term
		specularTerm += gl_FrontMaterial.specular.rgba * gl_LightSource[i].specular.rgba * pow( max(dot(normal, halfVector), 0.0), gl_FrontMaterial.shininess);

	}

	//Sum of all the lights
	gl_FragColor = globalAmbientTerm + ambientTerm + diffuseTerm + specularTerm;
}



