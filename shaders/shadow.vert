varying float Diffuse;
varying vec2  TexCoord;
varying vec4 ProjShadow;
//uniform vec3 LightDirection;

void main(void)
{
	vec3 realNorm   = gl_NormalMatrix * gl_Normal;

	vec3 LightDirection = normalize(vec3(0,-1,1));

	Diffuse         = max(dot(-LightDirection, realNorm), 0.0);
	TexCoord        = gl_MultiTexCoord0;
	ProjShadow	= gl_TextureMatrix[1] * gl_Vertex;
	gl_Position	= ftransform();
}