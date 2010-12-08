uniform sampler2D Checker;
uniform sampler2DShadow ShadowMap;

varying vec4 ProjShadow;
varying float Diffuse;
varying vec2  TexCoord;

void main (void)
{
	vec3 color = texture2D(Checker, TexCoord).stp;
	color *= shadow2DProj(ShadowMap, ProjShadow).r;
	color *= Diffuse;
	gl_FragColor = vec4(color, 1);
}