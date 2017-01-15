/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b7a 01-Sept-11
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"

string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {"undefined", "present", "past", "future"};
//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};
string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {"undefined", "uncountable", "singular", "plural"};
//int grammaticalNumberNameLengthsArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {9, 11, 8, 6};
string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES] = {"undefined", "person", "masculine", "feminine"};
//int grammaticalGenderNameLengthsArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES] = {9, 6, 9, 8};

string quantityNumberLowNameArray[QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string quantityNumberTensNameArray[QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES] = {"zero", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string quantityMultiplierNameArray[QUANTITY_MULTIPLIER_NUMBER_OF_TYPES] = {"one", "ten", "hundred", "thousand", "million", "billion", "trillion"};
string quantityModifierNameArray[QUANTITY_MODIFIER_NUMBER_OF_TYPES] = {"almost"};

//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};

//~nouns
GIAEntityNode::GIAEntityNode(void)
{
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	grammaticalNumberTemp = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalDefiniteTemp = GRAMMATICAL_DEFINITE_UNDEFINED;
	grammaticalPersonTemp = GRAMMATICAL_PERSON_UNDEFINED;		
	grammaticalGenderTemp = GRAMMATICAL_GENDER_UNDEFINED;
	//grammaticalCountTemp = GRAMMATICAL_COUNT_UNDEFINED;
	isSubjectTemp = false;
	isObjectTemp = false;
	hasPropertyTemp = false;
	
	entityAlreadyDeclaredInThisContext = false;
	
	hasAssociatedPropertyTemp = false;
	hasAssociatedActionTemp = false;
	
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	
	hasQuantity = false;
	quantityNumber = QUANTITY_NUMBER_UNDEFINED;
	quantityModifier = QUANTITY_MODIFIER_UNDEFINED;	//not yet implemented
	quantityModifierString = "";	//eg "almost"
	hasMeasure = false;
	measureType = MEASURE_TYPE_UNDEFINED;
			
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
	
	//type = undefinedEntityType;
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



int calculateQuantityNumberInt(string quantityNumberString)
{
	int quantityNumberInt = 1;
	bool found = false;
	for(int i=0; i<QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES; i++)
	{
		if(quantityNumberString == quantityNumberLowNameArray[i])
		{
			quantityNumberInt = i;
			found = true;
		}
	}
	for(int i=0; i<QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES; i++)
	{
		if(quantityNumberString == quantityNumberTensNameArray[i])
		{
			quantityNumberInt = i*10;
			found = true;
		}
	}
	for(int i=0; i<QUANTITY_MULTIPLIER_NUMBER_OF_TYPES; i++)
	{
		if(quantityNumberString == quantityMultiplierNameArray[i])
		{
			quantityNumberInt = pow(10, i);
			found = true;
		}
	}		
	if(!found)
	{//parse as simple number
		char * quantityNumberStringcharstar = const_cast<char*>(quantityNumberString.c_str());
		quantityNumberInt = atoi(quantityNumberStringcharstar);	
	}	
	return quantityNumberInt;
}


int calculateQuantityMultiplierInt(string quantityMultiplierString)
{
	int quantityMultiplierInt = 1;
	bool found = false;
	for(int i=0; i<QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES; i++)
	{
		if(quantityMultiplierString == quantityNumberLowNameArray[i])
		{
			quantityMultiplierInt = i;
			found = true;
		}
	}
	for(int i=0; i<QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES; i++)
	{
		if(quantityMultiplierString == quantityNumberTensNameArray[i])
		{
			quantityMultiplierInt = i*10;
			found = true;
		}
	}	
	for(int i=0; i<QUANTITY_MULTIPLIER_NUMBER_OF_TYPES; i++)
	{
		if(quantityMultiplierString == quantityMultiplierNameArray[i])
		{
			quantityMultiplierInt = pow(10, i);
			found = true;
		}
	}
	if(!found)
	{//parse as simple number
	
		char * quantityMultiplierStringcharstar = const_cast<char*>(quantityMultiplierString.c_str());
		quantityMultiplierInt = atoi(quantityMultiplierStringcharstar);
	}
	
	return quantityMultiplierInt;
}

int calculateQuantityModifierInt(string quantityModifierString)
{
	cout << "warning: calculateQuantityModifierInt() not yet implemented" << endl;
	
	int quantityModifierInt = 1;
	bool found = false;
	for(int i=0; i<QUANTITY_MODIFIER_NUMBER_OF_TYPES; i++)
	{
		if(quantityModifierString == quantityModifierNameArray[i])
		{
			quantityModifierInt = i;
			found = true;
		}
	}
	return quantityModifierInt;	
}

