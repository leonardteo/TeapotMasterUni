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
	this->rootNode = new Node();
}

/**
* getNode
* Returns a pointer to a node with the name
*/
Node* SceneGraph::getNode(string nodeName)
{
	//Search each of the nodes and return the node
	//@todo recursive search
	return NULL;


}


/**
* Destructor
*/
SceneGraph::~SceneGraph(void)
{
	delete this->rootNode;
}

