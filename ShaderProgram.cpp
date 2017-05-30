#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	//unload();
}

bool ShaderProgram::load()
{
	// compile both shaders
	if (!compile(GL_VERTEX_SHADER) || !compile(GL_FRAGMENT_SHADER))
	{
		return false;
	}
	else
	{
		// create shader program, save result in program
		program = glCreateProgram();

		// attach shaders to shader program, then link
		glAttachShader(program, vprogram);
		glAttachShader(program, fprogram);
		glLinkProgram(program);

		// check if program linked properly
		GLint linkResult = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
		if (linkResult == GL_TRUE)
		{
			return true;
		}
		else
		{
			// get error log
			GLint logLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			char* logArray = new char[logLength + 1];
			logArray[logLength] = '\0';
			glGetProgramInfoLog(program, logLength, 0, logArray);

			// print it out
			cout << logArray;

			// delete program and log array now that they're done
			glDeleteProgram(program);
			delete[] logArray;

			return false;
		}
	}
}

bool ShaderProgram::compile(GLenum shaderType)
{
	// open vertex shader file in binary mode
	// first, check shaderType (in this case, vertex shader)
	if (shaderType == GL_VERTEX_SHADER)
	{
		ifstream instream(filenamev, ios::binary); // this constructor opens the file!
		// no need to call instream.open()

		if (instream.is_open())
		{
			// get length
			instream.seekg(0, ios::end);
			int length = (int)instream.tellg();
			instream.seekg(0, ios::beg);

			// read entire file
			char* myCharArray = new char[length + 1]; // plus one for null terminator
			myCharArray[length] = '\0';
			instream.read(myCharArray, length); // maybe do this BEFORE null character

			// close file
			instream.close();

			// create shader object and store index for future use
			vprogram = glCreateShader(GL_VERTEX_SHADER);

			// set source code, delete 
			glShaderSource(vprogram, 1, &myCharArray, 0);
			delete[] myCharArray;

			// compile shader
			glCompileShader(vprogram);

			// check that shader compiled
			GLint result = 0;
			glGetShaderiv(vprogram, GL_COMPILE_STATUS, &result);
			if (result == GL_TRUE)
			{
				return true;
			}
			else
			{
				// get info log
				GLint logLength = 0;
				glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &logLength);

				// create array to hold log
				GLchar* logArray = new GLchar[logLength + 1];
				logArray[logLength] = '\0';
				glGetShaderInfoLog(vprogram, logLength, 0, logArray);
				
				// print log array
				cout << logArray;

				// delete shader, then log array
				glDeleteShader(vprogram);
				delete[] logArray;

				return false;
			}
		}
		// now, error check
		else
		{
			cout << "Error. Could not read vertex shader file\n";
			return false;
		}
	}
	// now do the exact same for the fragment shader
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		ifstream instream(filenamef, ios::binary);

		if (instream.is_open())
		{
			// get length
			instream.seekg(0, ios::end);
			int length = (int)instream.tellg();
			instream.seekg(0, ios::beg);

			// read entire file
			char* myCharArray = new char[length + 1];
			myCharArray[length] = '\0';
			instream.read(myCharArray, length);
			instream.close();

			// create shader object and store index for future use
			fprogram = glCreateShader(GL_FRAGMENT_SHADER);

			// set source code, delete 
			glShaderSource(fprogram, 1, &myCharArray, 0);
			delete[] myCharArray;

			// compile shader
			glCompileShader(fprogram);

			// check that shader compiled
			GLint result = 0;
			glGetShaderiv(fprogram, GL_COMPILE_STATUS, &result);
			if (result == GL_TRUE)
			{
				return true;
			}
			else
			{
				// get info log
				GLint logLength = 0;
				glGetShaderiv(fprogram, GL_INFO_LOG_LENGTH, &logLength);

				// create array to hold log
				GLchar* logArray = new GLchar[logLength + 1];
				logArray[logLength] = '\0';
				glGetShaderInfoLog(fprogram, logLength, 0, logArray);

				// print log array
				cout << logArray;

				// delete shader, then log array
				glDeleteShader(fprogram);
				delete[] logArray;

				return false;
			}
		}
	}

	return false;
}

void ShaderProgram::use()
{
	glUseProgram(program);
}

void ShaderProgram::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);

	program = 0;
	vprogram = 0;
	fprogram = 0;
}
