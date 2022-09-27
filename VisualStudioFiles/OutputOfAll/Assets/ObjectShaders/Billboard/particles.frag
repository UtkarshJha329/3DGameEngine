
in vec2 oTexCoord;
//uniform sampler2D particleTexture;

uniform float entityID;
uniform float totalNumEntities;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 ColourBuffer;
layout (location = 2) out int thisEntityID;

void main()
{
	//vec4 colour = texture(billboardTexture, oTexCoord);

	//if(colour.a < 0.5) {
	//	discard;
	//}

	FragColor = vec4(1.0);

	ColourBuffer = vec4(vec3(entityID / totalNumEntities), 1.0);

	thisEntityID = int(entityID);
}