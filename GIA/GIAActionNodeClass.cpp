/*******************************************************************************
 *
 * File Name: GIAActionNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b7a 01-Sept-11
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
		
	/*
	next = NULL;
	*/
}
GIAActionNode::~GIAActionNode(void)
{
	/*
	if(next != NULL)
	{
		delete next;
	}
	*/

}




