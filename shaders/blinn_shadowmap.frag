#version 120

//Fragment Shader

//Inputs from vertex shader
varying vec3 surfaceNormal;
varying vec3 vertexPosition;
varying vec4 projShadow;

//Built-in texture sampler
uniform sampler2D texture;

//Shadow Map
/*
uniform sampler2D shadowMap;
varying vec4 shadowCoord;
*/

int numLights = 2;	//We'll use a uniform var later



void main()
{

	//Get texture color
	vec4 textureColor = texture2D(texture, gl_TexCoord[0].st);
	//textureColor *= shadow2DProj(shadowMap, projShadow).r;

	vec3 normal = normalize(surfaceNormal);

	vec4 globalAmbientTerm = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambientTerm = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 diffuseTerm = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularTerm = vec4(0.0, 0.0, 0.0, 0.0);

	//Calculate global ambient term
	globalAmbientTerm = gl_FrontMaterial.ambient.rgba * gl_LightModel.ambient.rgba * textureColor.rgba;

	//Calculate per light contributions
	for (int i=0; i < numLights; i++)
	{
		vec3 lightDirection = normalize(gl_LightSource[i].position.xyz - vertexPosition);
		vec3 eyeDirection = normalize(-vertexPosition);	//Looking at YOU!
		vec3 halfVector = normalize(lightDirection + eyeDirection);

		//For spotlight, check if the light direction is within the cone
		float normaldotL = max(dot(normal,normalize(lightDirection)),0.0);
		if (normaldotL > 0.0)	//If the surface is pointing within 90" to the light
		{
			float spotEffect = dot(normalize(gl_LightSource[i].spotDirection), -lightDirection);
			if (spotEffect > gl_LightSource[i].spotCosCutoff)
			{

				//Calculate ambient term
				ambientTerm += gl_FrontMaterial.ambient.rgba * gl_LightSource[i].ambient.rgba * textureColor.rgba;

				//Calculate diffuse term
				diffuseTerm += gl_FrontMaterial.diffuse.rgba * gl_LightSource[i].diffuse.rgba * textureColor.rgba * clamp(dot(lightDirection, normal), 0.0, 1.0);

				//Blinn-Phong Shading specular term
				specularTerm += gl_FrontMaterial.specular.rgba * gl_LightSource[i].specular.rgba * pow( max(dot(normal, halfVector), 0.0), gl_FrontMaterial.shininess);

			}
		}

	}

	//output color
	vec4 color;

	//Sum of all the lights
	color = globalAmbientTerm + ambientTerm + diffuseTerm + specularTerm;

	//Shadow Mapping
	/*
	vec4 shadowCoordinateWdivide = shadowCoord / shadowCoord.w ;
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;
	float distanceFromLight = texture2D(shadowMap,shadowCoordinateWdivide.st).z;
	float shadow = 1.0;
	if (shadowCoord.w > 0.0)
	 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
	*/

	gl_FragColor = color;
}



