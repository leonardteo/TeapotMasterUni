/*
 *  SceneGraph.cpp
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-10-31.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "SceneGraph.h"

/**
 * Constructor
 */
SceneGraph::SceneGraph(void)
{
	this->rootNode = new Node("root");
}

/**
 Render the scene
 **/
void SceneGraph::render(RenderType renderType)
{
	this->rootNode->render(renderType);
}

/**
* getNode
* Returns a pointer to a node with the name
*/
Node* SceneGraph::getNode(string nodeName)
{
	return this->rootNode->getNode(nodeName);
}


/**
* Destructor
*/
SceneGraph::~SceneGraph(void)
{
	delete this->rootNode;
}

