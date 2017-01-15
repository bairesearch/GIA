/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"

string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {"undefined", "present", "past", "future"};
int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};
string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {"undefined", "uncountable", "singular", "plural"};
int grammaticalNumberNameLengthsArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {9, 11, 8, 6};

//~nouns
GIAEntityNode::GIAEntityNode(void)
{
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	grammaticalNumberTemp = GRAMMATICAL_NUMBER_UNDEFINED;
	definiteTemp = GRAMMATICAL_DEFINITE_UNDEFINED;
	hasAssociatedPropertyTemp = false;
	hasAssociatedActionTemp = false;
	
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	
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

