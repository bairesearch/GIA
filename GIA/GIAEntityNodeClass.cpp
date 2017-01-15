/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 2-Apr-2012
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"

string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {"undefined", "present", "past", "future"};
	//OPENCOG RECOMMENDED; "future", "future_progressive", "imperative", "infinitive", "past", "past_infinitive", "past_progressive", "perfect", "present", "present_progressive", "progressive"
	//MINIMAL; "undefined", "future", "progressive", "imperative", "infinitive", "past", "perfect", "present"
	//ABSTRACTED; "present", "present_perfect", "present_passive", "present_infinitive", "present_progressive", "present_progressive_passive", "past", "past_passive", "past_infinitive", "past_progressive", "future", "future_progressive", "imperative", "progressive"
string grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES] = {"progressive", "perfect", "passive", "infinitive", "imperative"};
string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {"undefined", "uncountable", "singular", "plural"};
string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES] = {"undefined", "person", "masculine", "feminine"};

string quantityNumberLowNameArray[QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string quantityNumberTensNameArray[QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES] = {"zero", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string quantityMultiplierNameArray[QUANTITY_MULTIPLIER_NUMBER_OF_TYPES] = {"one", "ten", "hundred", "thousand", "million", "billion", "trillion"};
string quantityModifierNameArray[QUANTITY_MODIFIER_NUMBER_OF_TYPES] = {"almost"};

//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};

//~nouns
GIAEntityNode::GIAEntityNode(void)
{
	id = 0;	
	idSecondary = 0;	//temporary ID reserved for specific entity types; concept, action, property etc
	reorderdIDforXMLsave = 0;
				
	entityName = "";
	confidence = 1.0;
	
	isProperty = false;
	isAction = false;
	isCondition = false;
	hasAssociatedInstance = false;
	hasAssociatedInstanceIsAction = false;
	hasAssociatedInstanceIsCondition = false;
	hasAssociatedTime = false;
	hasProgressiveTemp = false;
	hasQuality = false;
	
	//type = undefinedEntityType;
	//instance = undefinedInstance;
	
	actionSubjectEntity = NULL;
	actionObjectEntity = NULL;

	conditionSubjectEntity = NULL;
	conditionObjectEntity = NULL;	
	conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	timeConditionNode = NULL;
	
	//entityNodeContainingThisProperty = NULL;				//if property only:	//eg, Tom; OR;  Tom's Assets	//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a vector
	entityNodeDefiningThisInstance = NULL; 		

	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	
	hasQuantity = false;
	quantityNumber = QUANTITY_NUMBER_UNDEFINED;
	quantityModifier = QUANTITY_MODIFIER_UNDEFINED;	//not yet implemented
	quantityModifierString = "";	//eg "almost"
	hasQuantityMultiplier = false;
	hasMeasure = false;
	measureType = MEASURE_TYPE_UNDEFINED;

	initialisedForPrinting = false;
	//printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	
	for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
	{
		grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = false;
	}
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	grammaticalNumberTemp = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalDefiniteTemp = GRAMMATICAL_DEFINITE_UNDEFINED;
	grammaticalPersonTemp = GRAMMATICAL_PERSON_UNDEFINED;		
	grammaticalGenderTemp = GRAMMATICAL_GENDER_UNDEFINED;
	//grammaticalCountTemp = GRAMMATICAL_COUNT_UNDEFINED;
	grammaticalPronounTemp = GRAMMATICAL_PRONOUN_UNDEFINED;
	isSubjectTemp = false;
	isObjectTemp = false;
	hasPropertyTemp = false;
	//hasQualityTemp = false;
	
	entityAlreadyDeclaredInThisContext = false;
	
	hasAssociatedInstanceTemp = false;
	
	isQuery = false;
	isWhichQuery = false;
	isAnswerToQuery = false;
	//#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES	
	isAnswerContextToQuery = false;
	//#endif
	testedForQueryComparison = false;

	negative = false;
	
	disableParsingAsAPrepositionRelationTemp = false;
	
	queryEntityTraced = false;
	
	disabled = false;

	CXLdummyNode = false;	
}
GIAEntityNode::~GIAEntityNode(void)
{
}

/*
GIAEntityNodeContainer::GIAEntityNodeContainer(void)
{
	id = 0;
	node = NULL; 			
	next = NULL;
}
GIAEntityNodeContainer::~GIAEntityNodeContainer(void)
{	
	//delete node		
	if(next != NULL)
	{
		delete next;
	}
}
*/

void disconnectNodeFromAllButDefinitions(GIAEntityNode * entityNode)
{
	cout << "warning: disconnectNodeFromAllButDefinitions() not yet coded" << endl; 
	/* need to delete its instance from the reverse lists of each node of each list of this entity...
	ActionNodeList->clear();
	IncomingActionNodeList->clear();

	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	IncomingActionNodeList->clear();
	*/
}



int calculateQuantityNumberInt(string quantityNumberString)
{
	/*
	if(quantityNumberString == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		quantityNumberInt = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT;
	}	
	else
	{
	*/
	
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
			quantityNumberInt = pow(10, double(i));
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
			quantityMultiplierInt = pow(10, double(i));
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

string printQuantityNumberString(GIAEntityNode * entityNode)
{
	string quantityNumberStringTemp;

	if(entityNode->hasQuantityMultiplier)
	{
		char quantityNumberStringcharstar[20];
		sprintf(quantityNumberStringcharstar, "%d", entityNode->quantityNumber);
		quantityNumberStringTemp = quantityNumberStringcharstar;		
	}
	else
	{
		quantityNumberStringTemp = entityNode->quantityNumberString;
	}

	return quantityNumberStringTemp;	
}





