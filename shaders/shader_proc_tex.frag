#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D samp;

in vec3 interpNormal;
in vec2 CoordMap;
in vec3 positionTop;



void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	vec4 texC = texture2D(samp, CoordMap);
	gl_FragColor = texC * diffuse;
}


