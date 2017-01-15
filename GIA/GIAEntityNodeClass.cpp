/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"



//~nouns
GIAEntityNode::GIAEntityNode(void)
{
	tenseTemp = TENSE_UNDEFINED;
	
	initialisedForPrinting = false;
	printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	
	entityName = "";
	confidence = 1.0;
	isProperty = false;
	hasAssociatedProperty = false;
	hasAssociatedAction = false;
	hasAssociatedTime = false;
	
	type = undefinedEntityType;
	//instance = undefinedInstance;

	entityNodeContainingThisProperty = NULL;				//if property only:	//eg, Tom; OR;  Tom's Assets	//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	entityNodeDefiningThisProperty = NULL; 		
		
	/*	
	next = NULL;
	*/
}
GIAEntityNode::~GIAEntityNode(void)
{

	/*			
	if(next != NULL)
	{
		delete next;
	}
	*/
}

