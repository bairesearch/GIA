/*******************************************************************************
 *
 * File Name: GIAEntityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l5b 03-June-2012
 *
 *******************************************************************************/


#include "GIAEntityNodeClass.h"


string quantityNumberLowNameArray[QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string quantityNumberTensNameArray[QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES] = {"zero", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string quantityMultiplierNameArray[QUANTITY_MULTIPLIER_NUMBER_OF_TYPES] = {"one", "ten", "hundred", "thousand", "million", "billion", "trillion"};
string quantityModifierNameArray[QUANTITY_MODIFIER_NUMBER_OF_TYPES] = {"almost"};

//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};


//~nouns
GIAEntityNode::GIAEntityNode(void)
{
	idActiveList = 0;	
	idActiveEntityTypeList = 0;	//temporary ID reserved for specific entity types; concept, action, property etc
	idActiveListReorderdIDforXMLsave = 0;
	idInstance = 0;
				
	entityName = "";
	confidence = 1.0;
	
	isConcept = false;
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
	
	//entityNodeContainingThisProperty = NULL;				//if property only:	//eg, Tom; OR;  Tom's Assets	//NB by definition, only 1 thing can contain any given property [considering a property is an instance of an entity] - therefore this is not a Basic
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
	grammaticalRelexPersonOrStanfordProperNounTemp = GRAMMATICAL_PERSON_UNDEFINED;		
	grammaticalGenderTemp = GRAMMATICAL_GENDER_UNDEFINED;
	//grammaticalCountTemp = GRAMMATICAL_COUNT_UNDEFINED;
	grammaticalPronounTemp = GRAMMATICAL_PRONOUN_UNDEFINED;
	isSubjectTemp = false;
	isObjectTemp = false;
	hasPropertyTemp = false;
	//hasQualityTemp = false;
	isSubjectTemp2 = false;
	isObjectTemp2 = false;
	hasPropertyTemp2 = false;
	entityIndexTemp = 0;
	sentenceIndexTemp = 0;
	
	//to minimise query/referencing code	
	ActionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS]);
	IncomingActionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS]);
	ConditionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS]);
	IncomingConditionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS]);
	PropertyNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES]);
	PropertyNodeReverseList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]);
	EntityNodeDefinitionList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS]);
	EntityNodeDefinitionReverseList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS]);
	AssociatedInstanceNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES]);
	actionSubjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT]);
	actionObjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT]);
	conditionSubjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT]);
	conditionObjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT]);
	entityNodeDefiningThisInstance = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE]);

	#ifdef GIA_USE_DATABASE
	DBsetEntityConnectionsLoaded(this, true);	//for now, assume that a new entity will be configured with its connections loaded into RAM
	#endif
	
	/*
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_DEFINITIONS] = EntityNodeDefinitionList;
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_PROPERTIES] = PropertyNodeList;
	*/
	
	#ifdef GIA_USE_ADVANCED_REFERENCING
	/* initialisation shouldnt be necessary...
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityVectorConnectionsParametersSameReferenceSetArray[i] = new vector<GIAEntityNode*>();
	}
	*/
	#endif
	
	
	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	CharacterOffsetBeginTemp = -1;
	CharacterOffsetEndTemp = -1;
	*/
	stanfordPOSTemp = "";
	NERTemp = FEATURE_NER_UNDEFINED;
	NormalizedNERTemp = "";
	TimexTemp = "";
	#endif
	
		
	entityAlreadyDeclaredInThisContext = false;
	
	hasAssociatedInstanceTemp = false;
	
	isQuery = false;
	isWhichQuery = false;
	isAnswerToQuery = false;
	testedForQueryComparison = false;
	testedForQueryComparisonTemp = false;

	negative = false;
	
	disableParsingAsAPrepositionRelationTemp = false;
	
	queryEntityTraced = false;
	
	disabled = false;
	permanentConcept = false;

	CXLdummyNode = false;	
	
	//firstSentenceToAppearInNetwork = true;
	
	wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;	
	
	#ifdef GIA_USE_ADVANCED_REFERENCING
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	#endif	
	
	#ifdef GIA_USE_DATABASE
	bool added = false;	//implies database Update is Required
	bool modified = false;	//implies database Update is Required
	#endif	
}
GIAEntityNode::~GIAEntityNode(void)
{
}

#ifdef GIA_USE_DATABASE
void DBsetEntityConnectionsLoaded(GIAEntityNode * entityNode, bool loaded)
{
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityNode->entityVectorConnectionsReferenceListLoadedArray[i] = loaded;
	}
}
#endif


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


