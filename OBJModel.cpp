#include "OBJModel.h"

using namespace std;

//Constructor
OBJModel::OBJModel(const char* fileName)
{
	this->init();
	this->load(fileName);
}

//Free memory
OBJModel::~OBJModel(void)
{
	//Free all vertices and normals
	delete [] this->vertices;
	delete [] this->normals;
	delete [] this->uvs;
	
	delete [] this->index;
	delete [] this->normalsIndex;
	delete [] this->uvIndex;

	delete [] this->vertexArray;
	delete [] this->normalsArray;
	delete [] this->uvArray;
	delete [] this->indexArray;
}

//General class initialization
void OBJModel::init()
{
	this->numVertices = 0;
	this->numFaces = 0;
	this->numUVs = 0;
	this->numNormals = 0;

	//Set all arrays to null
	this->vertices = NULL;
	this->uvs = NULL;
	this->normals = NULL;
	this->index = NULL;
	this->uvIndex = NULL;
	this->normalsIndex = NULL;
	this->vertexArray = NULL;
	this->normalsArray = NULL;
	this->uvArray = NULL;
	this->indexArray = NULL;

	//VBO
	this->vbo_vertices = 0;
	this->vbo_normals = 0;
	this->vbo_uvs = 0;
	this->vbo_index = 0;
}

/**
* Load the OBJ Model into memory
**/

void OBJModel::load(const char* fileName)
{
	char buffer[255];

	//Open file for reading
	ifstream file;
	file.open(fileName);

	if (!file.is_open())
	{
		//If file isn't open
		cout << "Error loading file: " << fileName << endl;
		return;
	} else {

		//First pass of reading the file counts the data
		while ( !file.eof() ){

			//Read the line
			file.getline(buffer, 255);

			//Read the line
			switch(buffer[0]) {

			//Vertex
			case 'v':
				switch(buffer[1]) {
				case ' ':
					this->numVertices++;
					break;
				case 'n':
					this->numNormals++;
					break;
				case 't':
					this->numUVs++;
					break;
				}
				break;

			//Face
			case 'f':
				this->numFaces++;
				break;

			default:
				break;
			}

		} //end of file

		//Echo the stats
		/*
		printf("Loading OBJ file\n");
		printf("fileName: %s \n", fileName);
		printf("Vertices: %d \n", this->numVertices);
		printf("Vertex Normals: %d \n", this->numNormals);
		printf("Faces: %d \n", this->numFaces);
		printf("\n");
		*/

		//Allocate memory for data

		//Init vertices
		this->vertices = new Vector3*[this->numVertices];
		this->index = new int*[this->numFaces];

		//Init normals
		this->normals = new Vector3*[this->numNormals];
		this->normalsIndex = new int*[this->numFaces];	//Triangles
		
		//Init UV's
		this->uvs = new UV*[this->numUVs];
		this->uvIndex = new int*[this->numFaces];

		//Initialize each of the indices
		for (int i=0; i<this->numFaces; i++){
			this->index[i] = new int[3];
			this->normalsIndex[i] = new int[3];
			this->uvIndex[i] = new int[3];
			for (int j=0; j<3; j++){
				this->index[i][j] = 0;
				this->normalsIndex[i][j] = 0;
				this->uvIndex[i][j] = 0;
			}
		}

		//Second pass of file to load data
		file.clear();				//Clear eof status
		file.seekg(0, ios::beg);	//Move cursor to beginning

		int currentVertex = 0;
		int currentIndex = 0;
		int currentNormal = 0;
		int currentUV = 0;
		int currentUVIndex = 0;
		int currentNormalIndex = 0;

		while (!file.eof()){

			//Read line
			file.getline(buffer, 255);

			switch (buffer[0]){

			//Vertex
			case 'v':
				switch(buffer[1]){
				case ' ':
					//Create new vertex
					this->vertices[currentVertex] = new Vector3();
					sscanf(buffer, "v %f %f %f", &this->vertices[currentVertex]->x, &this->vertices[currentVertex]->y, &this->vertices[currentVertex]->z);
					currentVertex++;
					break;
				case 'n':
					//Create new vertex normal
					this->normals[currentNormal] = new Vector3();
					sscanf(buffer, "vn %f %f %f", &this->normals[currentNormal]->x, &this->normals[currentNormal]->y, &this->normals[currentNormal]->z);
					currentNormal++;
					break;
				case 't':
					//Create new UV
					this->uvs[currentUV] = new UV();
					sscanf(buffer, "vt %f %f", &this->uvs[currentUV]->u, &this->uvs[currentUV]->v);
					currentUV++;
					break;
				}
				break;

			case 'f':
				sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &this->index[currentIndex][0], &this->uvIndex[currentUVIndex][0], &this->normalsIndex[currentNormalIndex][0], &this->index[currentIndex][1], &this->uvIndex[currentUVIndex][1], &this->normalsIndex[currentNormalIndex][1], &this->index[currentIndex][2], &this->uvIndex[currentUVIndex][2], &this->normalsIndex[currentNormalIndex][2]);
				currentIndex++;
				currentNormalIndex++;
				currentUVIndex++;
				break;
			}

		} //end of file

		//Release file pointer
		file.close();
	} 

	//decrement all indices to cater for OpenGL (OBJ indices start at 1)
	for(int i = 0; i < this->numFaces; i++)
	{
		for(int j=0; j<3; j++){
			this->index[i][j]--;
			this->normalsIndex[i][j]--;
			this->uvIndex[i][j]--;
		}
	}

	//Prepare the arrays for drawing
	this->prepArrays();

}

/**
Takes the vertex, normals and texture coords and turns them into straight march arrays for OpenGL to render
**/
void OBJModel::prepArrays()
{

	//Initialize the arrays
	this->vertexArray = new GLfloat[this->numFaces *3 * 3];	//3 vertices to a face, 3 floats to a vertex
	this->normalsArray = new GLfloat[this->numFaces *3 * 3];
	this->indexArray = new GLuint[this->numFaces * 3];
	this->uvArray = new GLfloat[this->numFaces * 3 * 2];	//3 vertices to a face, 2 floats to a UV

	//Turn index into a straight march
	int numElements = this->numFaces * 3;
	for (int i=0; i<numElements; i++)
	{
		this->indexArray[i] = i;
	}

	//Figure out vertices, normals and UVs
	for (int face = 0; face < this->numFaces; face++)
	{

		//Get vertex numbers
		int v1, v2, v3;
		v1 = this->index[face][0];
		v2 = this->index[face][1];
		v3 = this->index[face][2];

		//Get Normal numbers
		int n1, n2, n3;
		n1 = this->normalsIndex[face][0];
		n2 = this->normalsIndex[face][1];
		n3 = this->normalsIndex[face][2];

		//Get UVs
		int uv1, uv2, uv3;
		uv1 = this->uvIndex[face][0];
		uv2 = this->uvIndex[face][1];
		uv3 = this->uvIndex[face][2];

		//Element 1
		this->uvArray[6*face] = (GLfloat) this->uvs[uv1]->u;
		this->uvArray[6*face + 1] = (GLfloat) this->uvs[uv1]->v;

		this->normalsArray[9*face] = (GLfloat) this->normals[n1]->x;
		this->normalsArray[9*face + 1] = (GLfloat) this->normals[n1]->y;
		this->normalsArray[9*face + 2] = (GLfloat) this->normals[n1]->z;

		this->vertexArray[9*face] = (GLfloat) this->vertices[v1]->x;
		this->vertexArray[9*face + 1] = (GLfloat) this->vertices[v1]->y;
		this->vertexArray[9*face + 2] = (GLfloat) this->vertices[v1]->z;

		//Element 2
		this->uvArray[6*face + 2] = (GLfloat) this->uvs[uv2]->u;
		this->uvArray[6*face + 3] = (GLfloat) this->uvs[uv2]->v;

		this->normalsArray[9*face + 3] = (GLfloat) this->normals[n2]->x;
		this->normalsArray[9*face + 4] = (GLfloat) this->normals[n2]->y;
		this->normalsArray[9*face + 5] = (GLfloat) this->normals[n2]->z;

		this->vertexArray[9*face + 3] = (GLfloat) this->vertices[v2]->x;
		this->vertexArray[9*face + 4] = (GLfloat) this->vertices[v2]->y;
		this->vertexArray[9*face + 5] = (GLfloat) this->vertices[v2]->z;

		//Element 3
		this->uvArray[6*face + 4] = (GLfloat) this->uvs[uv3]->u;
		this->uvArray[6*face + 5] = (GLfloat) this->uvs[uv3]->v;

		this->normalsArray[9*face + 6] = (GLfloat)this->normals[n3]->x;
		this->normalsArray[9*face + 7] = (GLfloat)this->normals[n3]->y;
		this->normalsArray[9*face + 8] = (GLfloat)this->normals[n3]->z;

		this->vertexArray[9*face + 6] = (GLfloat)this->vertices[v3]->x;
		this->vertexArray[9*face + 7] = (GLfloat)this->vertices[v3]->y;
		this->vertexArray[9*face + 8] = (GLfloat)this->vertices[v3]->z;	
	}

	/*
	//Release all the object memory - we only need the arrays
	delete [] this->vertices;
	delete [] this->normals;
	delete [] this->uvs;
	
	delete [] this->index;
	delete [] this->normalsIndex;
	delete [] this->uvIndex;
	*/

	//this->debugArrays();

	//Check vertex buffer object extension
	if (glewGetExtension("GL_ARB_vertex_buffer_object"))
	{
		//cout << "Vertex buffer objects supported!" << endl;
		
		//Load vertices
		glGenBuffers(1, &this->vbo_vertices);	//Get name/ID
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);	//Bind the buffer to the name
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numFaces*3*3, this->vertexArray, GL_STATIC_DRAW);

		//Load normals
		glGenBuffers(1, &this->vbo_normals);	//Get name/ID
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);	//Bind the buffer to the name
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numFaces*3*3, this->normalsArray, GL_STATIC_DRAW);

		//Load UVs
		glGenBuffers(1, &this->vbo_uvs);	//Get name/ID
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_uvs);	//Bind the buffer to the name
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numFaces*3*2, this->uvArray, GL_STATIC_DRAW);

		//Load index
		glGenBuffers(1, &this->vbo_index);	//Get name/ID
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_index);	//Bind the buffer to the name
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->numFaces*3, this->indexArray, GL_STATIC_DRAW_ARB);
		
		//Clear bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//cout << "Loaded model into GPU memory" << endl;
		
	}
	
}

//Show all the array data
void OBJModel::debugArrays()
{
	FILE *file = fopen("debug.txt", "w");
	if (file == NULL)
	{
		printf("Couldn't open debug file.");
		return;
	}
	
	fprintf(file, "Element Index: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%d %d %d \n", this->indexArray[3*face], this->indexArray[3*face + 1], this->indexArray[3*face + 2]);
	}

	fprintf(file, "\nVertices: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%f %f %f   %f %f %f   %f %f %f   \n", 
			this->vertexArray[3*face], this->vertexArray[3*face + 1], this->vertexArray[3*face + 2],
			this->vertexArray[3*face+3], this->vertexArray[3*face + 4], this->vertexArray[3*face + 5],
			this->vertexArray[3*face+6], this->vertexArray[3*face + 7], this->vertexArray[3*face + 8]
		);
	}

	fprintf(file, "\nNormals: \n");
	for (int face=0; face < this->numFaces; face++)
	{
		fprintf(file, "%f %f %f   %f %f %f   %f %f %f   \n", 
			this->normalsArray[3*face], this->normalsArray[3*face + 1], this->normalsArray[3*face + 2],
			this->normalsArray[3*face+3], this->normalsArray[3*face + 4], this->normalsArray[3*face + 5],
			this->normalsArray[3*face+6], this->normalsArray[3*face + 7], this->normalsArray[3*face + 8]
		);
	}

	fclose(file);
}

