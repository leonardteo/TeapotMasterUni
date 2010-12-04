/*
 *  PolyMesh.cpp
 *  OBJViewer
 *
 *  Created by Leonard Teo on 10-10-05.
 *
 */

#include "PolyMeshNode.h"

void PolyMeshNode::init()
{
	this->mesh = NULL;
	this->texture = NULL;
	
	//Create a default material and attach it
	Material* material = new Material();
	this->material = material;

	this->collisionTriangles = NULL;
	this->staticCollider = false;
	this->activeCollider = false;
	this->colliderSphereRadius = 0.0f;

	this->numFaces = 0;
}

//Constructor
PolyMeshNode::PolyMeshNode(string id) : Node(id)
{
	this->init();
}

//Convenience constructor
PolyMeshNode::PolyMeshNode(string id, string filename) : Node(id)
{
	this->init();

	//Load OBJ file
	OBJModel* obj = new OBJModel(filename.c_str());
	this->attachModel(obj);
	
}

//Destructor
PolyMeshNode::~PolyMeshNode()
{
	delete this->mesh;
	delete this->texture;
	delete this;
}

//attachModel
void PolyMeshNode::attachModel(OBJModel* model)
{
	this->mesh = model;
	this->numFaces = model->numFaces;
}

//attachTexture
void PolyMeshNode::attachTexture(Texture* texture)
{
	this->texture = texture;
}

void PolyMeshNode::attachTexture(string filename)
{
	Texture* textureNode = new Texture(filename.c_str());
	this->attachTexture(textureNode);
}

//Attach material
void PolyMeshNode::attachMaterial(Material* material)
{
	this->material = material;
}

void PolyMeshNode::draw()
{

	//Load texture
	if (this->texture != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture->textureID);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

	//Load material
	if (this->material != NULL)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, this->material->ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, this->material->diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, this->material->specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, this->material->shininess);
	}

	//Load Shader
	if (this->shader != NULL)
	{
		glUseProgram(this->shader->shaderProgram);
	} else {
		glUseProgram(NULL);
	}

	//Check vertex buffer object extension
	
	if (glewGetExtension("GL_ARB_vertex_buffer_object"))
	{
		//Load vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_vertices);
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);

		//Load normals
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_normals);
		glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_NORMAL_ARRAY);

		//Load UVs
		glBindBuffer(GL_ARRAY_BUFFER, this->mesh->vbo_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mesh->vbo_index);

		glDrawElements(GL_TRIANGLES, 3*this->mesh->numFaces, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	} else {
	

		//Drawing with vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, this->mesh->vertexArray);
		glNormalPointer(GL_FLOAT, 0, this->mesh->normalsArray);
		glTexCoordPointer(2, GL_FLOAT, 0, this->mesh->uvArray);

		glDrawElements(GL_TRIANGLES, 3*this->mesh->numFaces, GL_UNSIGNED_INT, this->mesh->indexArray);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}


/*

	//Old immediate mode drawing code

	glBegin(GL_TRIANGLES);
	for (int face = 0; face < this->numFaces; face++)
	{

		//Draw the vertex at each of the vertex numbders
		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][0]]->u, (GLfloat)this->uvs[this->uvIndex[face][0]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][0]]->x, (GLfloat)this->normals[this->normalsIndex[face][0]]->y, (GLfloat)this->normals[this->normalsIndex[face][0]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][0]]->x, (GLfloat)this->vertices[this->index[face][0]]->y, (GLfloat)this->vertices[this->index[face][0]]->z);

		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][1]]->u, (GLfloat)this->uvs[this->uvIndex[face][1]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][1]]->x, (GLfloat)this->normals[this->normalsIndex[face][1]]->y, (GLfloat)this->normals[this->normalsIndex[face][1]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][1]]->x, (GLfloat)this->vertices[this->index[face][1]]->y, (GLfloat)this->vertices[this->index[face][1]]->z);

		glTexCoord2f((GLfloat)this->uvs[this->uvIndex[face][2]]->u, (GLfloat)this->uvs[this->uvIndex[face][2]]->v);
		glNormal3f((GLfloat)this->normals[this->normalsIndex[face][2]]->x, (GLfloat)this->normals[this->normalsIndex[face][2]]->y, (GLfloat)this->normals[this->normalsIndex[face][2]]->z);
		glVertex3f((GLfloat)this->vertices[this->index[face][2]]->x, (GLfloat)this->vertices[this->index[face][2]]->y, (GLfloat)this->vertices[this->index[face][2]]->z);

		/* Non-Optimized code for debugging
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

		//Draw the vertex at each of the vertex numbders
		glTexCoord2f((GLfloat)this->uvs[uv1]->u, (GLfloat)this->uvs[uv1]->v);
		glNormal3f((GLfloat)this->normals[n1]->x, (GLfloat)this->normals[n1]->y, (GLfloat)this->normals[n1]->z);
		glVertex3f((GLfloat)this->vertices[v1]->x, (GLfloat)this->vertices[v1]->y, (GLfloat)this->vertices[v1]->z);

		glTexCoord2f((GLfloat)this->uvs[uv2]->u, (GLfloat)this->uvs[uv2]->v);
		glNormal3f((GLfloat)this->normals[n2]->x, (GLfloat)this->normals[n2]->y, (GLfloat)this->normals[n2]->z);
		glVertex3f((GLfloat)this->vertices[v2]->x, (GLfloat)this->vertices[v2]->y, (GLfloat)this->vertices[v2]->z);

		glTexCoord2f((GLfloat)this->uvs[uv3]->u, (GLfloat)this->uvs[uv3]->v);
		glNormal3f((GLfloat)this->normals[n3]->x, (GLfloat)this->normals[n3]->y, (GLfloat)this->normals[n3]->z);
		glVertex3f((GLfloat)this->vertices[v3]->x, (GLfloat)this->vertices[v3]->y, (GLfloat)this->vertices[v3]->z);
		*/
	/*
	}
	glEnd();
	*/

	//Unload shader so we don't interfere with other objects
	if (this->shader != NULL)
	{
		glUseProgram(NULL);
	}

	//Unload texture so that we don't interfere with other objects
	if (this->texture != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	} 

}

void PolyMeshNode::render(enum RenderType renderType)
{

	
	//Draw this
	glPushMatrix();
		//Model transform
		this->modelTransform();

		//Drawing

		//Figure out if this is an opaque or transparent object, and draw appropriately
		if (renderType == ALL_OBJECTS)
		{
			this->draw();
		} else {
			if (this->material != NULL)
			{
			//Check material
			if (this->material->diffuse[3] < 1.0f || this->material->ambient[3] < 1.0f)
			{
				//If there is transparency

				//Check the render type. If transparent, draw
				if (renderType == TRANSPARENT_OBJECTS)
				{
					this->draw();
				}
			} else {
				//If there is no transparency
				if (renderType == OPAQUE_OBJECTS)
				{
					this->draw();
				}
			}

			} else {
			//No material -- assume that it is opaque, so draw
			this->draw();
			}

		}
		
		//Draw children
		for (list<Node*>::iterator child = this->children->begin(); child != this->children->end(); child++)
		{
			(*child)->render(renderType);
		}


	glPopMatrix();	
}

//Initialize static collision
void PolyMeshNode::initStaticCollider()
{
	//Error Check
	if (this->mesh == NULL || !this->staticCollider) 
	{
		return;
	}

	//Initialize collision plane array
	this->collisionTriangles = new CollisionTriangle*[this->mesh->numFaces];

	//For each face, generate a plane
	for (int face=0; face<this->mesh->numFaces; face++)
	{
		//Get vertex numbers
		int v1, v2, v3;
		v1 = this->mesh->index[face][0];
		v2 = this->mesh->index[face][1];
		v3 = this->mesh->index[face][2];

		//Initialize actual vertices
		Vector3 vert1(this->mesh->vertices[v1]->x, this->mesh->vertices[v1]->y, this->mesh->vertices[v1]->z);
		Vector3 vert2(this->mesh->vertices[v2]->x, this->mesh->vertices[v2]->y, this->mesh->vertices[v2]->z);
		Vector3 vert3(this->mesh->vertices[v3]->x, this->mesh->vertices[v3]->y, this->mesh->vertices[v3]->z);

		//Initialize plane
		CollisionTriangle* plane = new CollisionTriangle(vert1, vert2, vert3);
		this->collisionTriangles[face] = plane;


		//echo the normal for debugging
		/*
		cout << "Created Plane: " << endl;
		cout << "V1: " << vert1 << endl;
		cout << "V2: " << vert2 << endl;
		cout << "V3: " << vert3 << endl;
		cout << "Normal: " << plane->normal << endl;
		cout << "Equation: " << plane->normal.x << "x + " << plane->normal.y << "y + " << plane->normal.z << "z + " << plane->d << " = 0" << endl;
		cout << endl;
		*/
	}
}