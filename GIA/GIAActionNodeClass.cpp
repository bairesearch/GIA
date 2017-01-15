/*******************************************************************************
 *
 * File Name: GIAActionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
 *
 *******************************************************************************/


#include "GIAActionNodeClass.h"



//~verbs
GIAActionNode::GIAActionNode(void)
{
	actionName = "";
	confidence = 1.0;

	initialisedForPrinting = false;
	printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;

	entityNodeDefiningThisAction = NULL;
	actionSubjectEntity = NULL;
	actionObjectEntity = NULL;
	
	id = 0;	
}
GIAActionNode::~GIAActionNode(void)
{
}

/*
GIAActionNodeContainer::GIAActionNodeContainer(void)
{
	id = 0;
	node = NULL; 			
	next = NULL;
}
GIAActionNodeContainer::~GIAActionNodeContainer(void)
{	
	//delete node		
	if(next != NULL)
	{
		delete next;
	}
}
*/



