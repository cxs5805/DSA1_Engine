#version 430

layout (location = 0) in vec3 position; // attribute, fragment location(?)
layout (location = 6) in vec3 vertexNorm;

out float red; // varying
out float blue;
layout (location = 3) uniform float scale; // uniform 

// week 9
layout (location = 4) uniform mat4 worldView; // identity matrix?
// camera matrix

// week 11
out vec3 fragNorm;
out vec3 fragLoc;

// week 12
//layout (location = 1) in vec2 texCoord;
layout (location = 1) in vec2 modelUV;
out vec2 fragUV;

// week 13
layout (location = 2) uniform mat4 modelWorld; // the MODEL'S world view matrix
// week 14 - can the CAMERA go here too??? Answer =...

void main()
{
	fragUV = modelUV;
	
	// transform from model to world (naive, week 11)
	fragLoc = position;
	fragNorm = vertexNorm;
	
	// set position
    //gl_Position = vec4(position, 1);
	gl_Position = worldView * vec4(position, 1); // week 9
	
	// week 13 - put model location into world space, then set position via new value
	gl_Position = worldView * modelWorld * vec4(position, 1);
	
	red = (position.y) / 0.2;
	blue = position.x * 2;
	
	// put fragNorm in world space
	//vec4(fragNorm, 0) * transpose(inverse(worldView));
	// **** WEEK 11 - SAME CONCERN AS ABOVE! ALSO WHY W-VALUE IF VEC3?
	// SHOULD I SKIP?
}
