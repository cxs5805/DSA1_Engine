#include "Model.h"

Model::Model()
{
	// default values
	vertArr = 0;
	vertCount = 0;

	// load texture
	tex = Texture();
}


Model::~Model()
{
	tex.~Texture();
}

bool Model::buffer(std::string objFile)
{
	// Part 2
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> vertInds;

	// try to open file
	//const char* objFileCstr(objFile.c_str());
	FILE* file;// = fopen(objFileCstr, "r");
	errno_t error = fopen_s(&file, objFile.c_str(), "r");
	if (error)
	{
		std::cout << "Well, the file didn't open :/\n";
		return false;
	}

	/*
	if (file == NULL)
	{
		std::cout << "Well, the file didn't open :/\n";
		return false;
	}
	*/

	// loop through file
	int res;
	do
	{
		char line[128];
		res = fscanf_s(file, "%s", line, _countof(line));
		// end of file
		if (res == EOF)
			break;

		// vertex
		if (strcmp(line, "v") == 0)
		{
			glm::vec3 vert;
			fscanf_s(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
			locations.push_back(vert);
		}
		// uv
		else if (strcmp(line, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		// normal
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		// face
		else if (strcmp(line, "f") == 0)
		{
			VertInd vind[3];
			int matches = fscanf_s(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
				&vind[0].locInd, &vind[0].uvInd, &vind[0].normInd,
				&vind[1].locInd, &vind[1].uvInd, &vind[1].normInd,
				&vind[2].locInd, &vind[2].uvInd, &vind[2].normInd);

			// decrement b/c obj indices are 1-based
			for (int i = 0; i < 3; i++)
			{
				vind[i].locInd--;
				vind[i].uvInd--;
				vind[i].normInd--;
				/*std::cout << "loc = " << vind[i].locInd <<
				"\nuv = " << vind[i].uvInd <<
				"\nnorm = " << vind[i].normInd << "\n\n";*/
			}

			vertInds.push_back(vind[0]);
			vertInds.push_back(vind[1]);
			vertInds.push_back(vind[2]);
		}
	} while (res != EOF);

	// Part 1
	// ----------------------
	// COPIED FROM MAIN
	// ----------------------
	// generate vertices at 4 corners
	// OpenGL is RIGHT-handed coordinate system
	// so +Z comes out of screen, -Z into screen
	/*
	std::vector<glm::vec3> locs =
	{ { 0.9, 0.9, 0 }, // top right
	{ -0.9, 0.9, 0 }, // top left
	{ -0.9, -0.9, 0 }, // bottom left
	{ 0.9, -0.9, 0 } }; // bottom right
	*/
	// list vertices and get number of them
	// what order do we wanna connect the dots?
	// unfortunately, we have to duplicate vertices (for now) b/c graphics wants them in 1D array
	/*
	std::vector <unsigned int> locInds = {
		0, 1, 2,	// 3 numbers represent a triangle
		0, 2, 3 };	// notice how both triangles are drawn counter-CW
	*/

	vertCount = vertInds.size();

	// duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount); // size and capacity both = vertCount
	for (unsigned int i = 0; i < vertCount; ++i)
	{
		vertBufData[i].location = locations[vertInds[i].locInd];
		vertBufData[i].uv = uvs[vertInds[i].uvInd];
		vertBufData[i].normal = normals[vertInds[i].normInd];

		// **********************************************************
		// WEEK 15: FIGURE OUT MAXIMUM X- Y- AND Z- LOCATIONS
		// BY LOOPING THRU EACH VERTEX...
		// GETTING ABS VALUE OF CURRENT VERTEX
		glm::vec3 absVert = glm::abs(vertBufData[i].location);

		// AAAAAAND SETTING MAX_XYZ TO HIGHEST VALUES OF ALL 3
		maxXYZ = glm::max(maxXYZ, absVert);
	}
		//vertBufData[i] = locations[vertInds[i].locInd];
	// now vertBufData has vertices in correct locInds order (duplicated as well :/)

	// GET MAXIMUM RADIUS BY FIGURING OUT LARGEST MAX VALUE
	maxR = glm::max(glm::max(maxXYZ.x, maxXYZ.y), maxXYZ.z);

	// DEBUG
	std::cout << "MAXIMUM CALCULATED" <<
		"\nX = " << maxXYZ.x <<
		"\nY = " << maxXYZ.y <<
		"\nZ = " << maxXYZ.z <<
		"\nLARGEST = " << maxR << "\n-\n";

	// **********************************************************

	// OPTIONAL
	// element arrays
	// 2 buffers
	// 1 for points, another for connections
	// no duplicates: use locs and locInds directly
	// complicated atm b/c of .obj files

	// now, we have data
	// we gotta give it to OpenGL
	//	place to store data
	//	way to bind buffer
	//	upload data
	//	tell OpenGL how to interpret

	// create buffer-related uint identifiers
	vertArr; // Vertex array object (VAO)
	GLuint vertBuf; // vertex buffer object (VBO)
	// where we store data for models

	// create buffers for vertex arrays and buffers
	// pass them by ref to OpenGL, so it can set their values
	glGenVertexArrays(1, &vertArr); // 1 = # of buffers to generate and save in address
	glGenBuffers(1, &vertBuf);
	// now I can upload data to buffers

	// bind array and buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	// you need one of these for each MODEL (VAO and VBO)

	// upload model data into currently bound buffer
	glBufferData(GL_ARRAY_BUFFER,		// where to copy to (goes to vertBuf)
		sizeof(Vertex) * vertCount,	// # bytes to copy (now vertices, but later bigger with textures)
		&vertBufData[0],				// from where to copy (address)
		GL_STATIC_DRAW);				// "hint" to OpenGL, not gonna change the data

	// set up one vertex attribute - model location
	// enable attribute
	glEnableVertexAttribArray(0); // attribute index = 0
	// tells OpenGL how to interpret data
	
	// set it up
	glVertexAttribPointer(
		0,					// attribute index
		3,					//  # of components (x,y,z)
		GL_FLOAT,			// type of data
		GL_FALSE,			// normalize data?
		sizeof(Vertex),		// stride (bytes per vertex)
		0);					// attribute offset
	// this method sets up a single attribute
	// we're only calling it once b/c only one attribute

	// Week 11 - set up another vertex attribute - fragment normal
	// enable attribute
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(
		6,					// attribute index
		3,					//  # of components (x,y,z)
		GL_FLOAT,			// type of data
		GL_FALSE,			// normalize data?
		sizeof(Vertex),		// stride (bytes per vertex)
		(void*)(sizeof(float) * 5));	// attribute offset

	// Week 12 - fragment UV
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute index
		2,					//  # of components (u,v)
		GL_FLOAT,			// type of data
		GL_FALSE,			// normalize data?
		sizeof(Vertex),		// stride (bytes per vertex)
		(void*)(sizeof(float) * 3));	// attribute offset

	glBindVertexArray(0);	// unbind when finished editing

	return true;
}

void Model::render()
{
	// time to draw
	// first bind, then render game objects
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
	glBindVertexArray(0);
	// unbind an object after drawing it
}
