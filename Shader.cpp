#include "Shader.h"


Shader::Shader(void)
{
	this->vertexShaderSource = NULL;
	this->fragmentShaderSource = NULL;
	this->fragmentShader = NULL;
	this->vertexShader = NULL;
	this->shaderProgram = NULL;
}


Shader::~Shader(void)
{
}


/**
Load vertex shader
**/
void Shader::loadVertexShader(char* filename)
{
	this->vertexShaderSource = this->readTextFile(filename);
}


/**
Load fragment shader
**/
void Shader::loadFragmentShader(char* filename)
{
	this->fragmentShaderSource = this->readTextFile(filename);
}

/**
Compilation
**/
bool Shader::compile()
{
	//If sources aren't there, return false
	if (this->vertexShaderSource == NULL || this->fragmentShaderSource == NULL)
		return false;

	//Reserve identifiers
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//Set constant strings
	const char* vertexShaderStr = this->vertexShaderSource;
	const char* fragmentShaderStr = this->fragmentShaderSource;

	//Set sources
	glShaderSource(this->vertexShader, 1, &vertexShaderStr, NULL);
	glShaderSource(this->fragmentShader, 1, &fragmentShaderStr, NULL);

	//Compile
	GLint vertexShaderCompiled;
	GLint fragmentShaderCompiled;

	//Compile vertex shader
	glCompileShader(this->vertexShader);

	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
	if (!vertexShaderCompiled)
	{
		GLint length;
		GLchar* log;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &length);
		log = (GLchar*) malloc(length);
		glGetShaderInfoLog(this->vertexShader, length, &length, log);
		cout << "There was an error compiling vertex shader: " << log;
		free(log);
		return false;
	}

	//Compile fragment shader
	glCompileShader(this->fragmentShader);
	glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
	if (!fragmentShaderCompiled)
	{
		GLint length;
		GLchar* log;
		glGetShaderiv(this->fragmentShader, GL_INFO_LOG_LENGTH, &length);
		log = (GLchar*) malloc(length);
		glGetShaderInfoLog(this->fragmentShader, length, &length, log);
		cout << "There was an error compiling fragment shader: " << log;
		free(log);
		return false;
	}

	//Create main shader program
	this->shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Link
	GLint linked;

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint length;
		GLchar* log;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

		log = (GLchar*) malloc(length);
		glGetProgramInfoLog(shaderProgram, length, &length, log);
		cout << "Error linking shader: " << log << endl;
		free(log);
		return false;
	}

	//cout << "Successfully compiled and linked shaders." << endl;

	//Get Uniform Locations
	this->vertices = glGetAttribLocation(shaderProgram, "vertex");
	this->modelViewMatrix = glGetUniformLocation(shaderProgram, "modelViewMatrix");
	this->projectionMatrix = glGetUniformLocation(shaderProgram, "projectionMatrix");
	this->shadowMap = glGetUniformLocation(shaderProgram, "shadowMap");

	/*
	cout << endl << "Uniform Locations: " << endl;
	cout << "Shadow Map: " << this->shadowMap << endl;
	cout << "MV Matrix: " << this->modelViewMatrix << endl;
	cout << "Proj Matrix: " << this->projectionMatrix << endl;
	cout << "Vertices: " << this->vertices << endl;
	*/

	return true;

}



/**
Reads a text file and returns a pointer to the char string
**/
char* Shader::readTextFile(char* filename)
{
	FILE* file;
	char* content = NULL;

	int charcount = 0;

	if (filename != NULL)
	{
		//Create file handler
		file = fopen(filename, "rt");

		//First figure out how much memory to allocate
		fseek(file, 0, SEEK_END);
		charcount = ftell(file);

		//Rewind
		rewind(file);

		//Copy
		if (charcount > 0)
		{
			content = (char*)malloc(sizeof(char) * (charcount+1));
			charcount = fread(content, sizeof(char), charcount, file);
			content[charcount] = '\0';	//String termination
		}

		fclose(file);

	}

	return content;

}

