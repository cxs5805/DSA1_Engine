#version 430

out vec4 fragColor;
in float red;
in float blue;
layout (location = 5) uniform float time;

// Week 11 - Lighting
in vec3 fragLoc; // position in world space of the pixel
in vec3 fragNorm;

layout (location = 7) uniform vec3 cameraLoc;
layout (location = 8) uniform vec3 lightLoc;

// week 12
in vec2 fragUV;
uniform sampler2D myTexture;

void main()
{	
	// calculate unit vectors
	vec3 N = normalize (fragNorm);
	vec3 L = normalize(lightLoc - fragLoc);
	vec3 V = normalize(cameraLoc - fragLoc);
	vec3 H = normalize(L + V);
	
	// calculate light values and apply to material
	float ambient = 0.3f;
	float diffuse = 0.95 * max(dot(L, N), 0);
	float specular = max(dot(H, N), 0);
	float specAlpha = 32f;
	specular = 1.5f * pow(specular, specAlpha);
	
	// calculate brightness (to be used for fragment color)
	float brightness = ambient + diffuse + specular;
	
	// glfrag position and varying variables lerped
	// but this is automatic, don't have to write any code
	
	float green = 0.3 + sin(time) / 2;
	
	// scale color channels by light
	fragColor = vec4(red, green, blue, 1) * brightness; //rgba
	fragColor.a = 1;
	
	// week 12 - sample texture for frag shader
	fragColor = texture(myTexture, fragUV) * brightness; 
	
	// DEBUGGING LINES
	//fragColor = vec4(0.2) * brightness;
	//fragColor = vec4(cameraLoc, 1);
	//fragColor = vec4(fragLoc, 1);
	//gl_FragColor = vec4(1,0,0,1);
	//fragColor = vec4(fragNorm, 1);
	//fragColor = vec4(lightLoc, 1);
	//fragColor = vec4(fragUV, 1, 1);
}
