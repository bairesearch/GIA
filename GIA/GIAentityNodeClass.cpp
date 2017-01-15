/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAentityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h1c 14-November-2014
 *
 *******************************************************************************/


#include "GIAentityNodeClass.h"


string quantityNumberLowNameArray[QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string quantityNumberTensNameArray[QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES] = {"zero", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string quantityMultiplierNameArray[QUANTITY_MULTIPLIER_NUMBER_OF_TYPES] = {"one", "ten", "hundred", "thousand", "million", "billion", "trillion"};
string quantityModifierNameArray[QUANTITY_MODIFIER_NUMBER_OF_TYPES] = {"almost"};

//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};

#ifdef GIA_USE_DATABASE
#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
GIAconceptEntityLoaded::GIAconceptEntityLoaded(void)
{
	loaded = false;
	numberOfInstances = 0;
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	disabled = false;
	#endif
}
GIAconceptEntityLoaded::~GIAconceptEntityLoaded(void)
{
}
#endif
#endif

//~nouns
GIAentityNode::GIAentityNode(void)
{
	/*GIA Internal Entity Referencing*/
	idActiveList = 0;
	idActiveEntityTypeList = 0;	//temporary ID reserved for specific entity types; concept, action, substance etc
	idActiveListReorderdIDforXMLsave = 0;
	idInstance = 0;		//set as concept by default (GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE)


	/*GIA Entity Name*/
	entityName = "";
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	wordOrig = "";		//this needs to be added to XML i/o + file system database i/o [used for NLG2 bug]
	#endif
	confidence = 1.0;


	/*GIA Entity Type*/
	isConcept = false;
	isSubstance = false;
	isAction = false;
	isCondition = false;
	hasAssociatedInstance = false;
	hasAssociatedInstanceIsAction = false;
	hasAssociatedInstanceIsCondition = false;
	hasAssociatedTime = false;
	isSubstanceQuality = false;
	isSubstanceConcept = false;
	isActionConcept = false;
	negative = false;


	/*GIA Special Variables (Quantities/Measures)*/
	hasQuantity = false;
	quantityNumber = QUANTITY_NUMBER_UNDEFINED;
	quantityModifier = QUANTITY_MODIFIER_UNDEFINED;	//not yet implemented
	quantityModifierString = "";	//eg "almost"
	hasQuantityMultiplier = false;
	hasMeasure = false;
	measureType = MEASURE_TYPE_UNDEFINED;


	/*GIA Draw Variables*/
	initialisedForPrinting = false;
	//printed = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	printCoordsAlreadyDefined = false;


	/*GIA Translator Temporary Variables - Grammar*/
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalWordTypeTemp = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
	{
		grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = false;
	}
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	grammaticalDefiniteTemp = false;
	grammaticalIndefinitePluralTemp = false;
	grammaticalProperNounTemp = false;
	grammaticalGenderTemp = GRAMMATICAL_GENDER_UNDEFINED;
	//grammaticalCountTemp = GRAMMATICAL_COUNT_UNDEFINED;
	grammaticalPronounTemp = false;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	grammaticalIndexOfDeterminerTemp = GIA_ENTITY_INDEX_UNDEFINED;
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	CharacterOffsetBeginTemp = INT_DEFAULT_VALUE;
	CharacterOffsetEndTemp = INT_DEFAULT_VALUE;
	*/
	stanfordPOStemp = "";
	NERTemp = FEATURE_NER_UNDEFINED;
	NormalizedNERtemp = "";
	TimexTemp = "";
	#endif

	/*GIA Translator Temporary Variables*/
	isSubjectTemp = false;
	isObjectTemp = false;
	hasSubstanceTemp = false;
	//hasQualityTemp = false;
	isActionTemp = false;
	entityIndexTemp = GIA_ENTITY_INDEX_UNDEFINED;	//was 0 before 11 October 2012
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED;	//was 0 before 11 October 2012
	isToBeComplimentOfActionTemp = false;
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	disableParsingAsPrepositionRelationTemp = false;
	#endif
	entityAlreadyDeclaredInThisContext = false;
	hasAssociatedInstanceTemp = false;
	#ifdef GIA_SUPPORT_ALIASES
	isName = false;
	isNameQuery = false;
	#endif
	#ifdef GIA_SUPPORT_NUMBER_OF
	isNumberOf = false;
	#endif
	
	/*GIA Connections*/
	//to minimise query/referencing code
	actionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS]);
	incomingActionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS]);
	conditionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS]);
	incomingConditionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS]);
	propertyNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES]);
	propertyNodeReverseList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES]);
	entityNodeDefinitionList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS]);
	entityNodeDefinitionReverseList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS]);
	associatedInstanceNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES]);
	actionSubjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT]);
	actionObjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT]);
	conditionSubjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT]);
	conditionObjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT]);
	entityNodeDefiningThisInstance = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE]);
	#ifdef GIA_USE_DATABASE
	DBsetEntityConnectionsReferenceListsLoaded(this, true);	//for now, assume that a new entity will be configured with its connections loaded into RAM
	#endif
	/*
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_DEFINITIONS] = entityNodeDefinitionList;
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_SUBSTANCES] = propertyNodeList;
	*/
	#ifdef GIA_USE_ADVANCED_REFERENCING
	/* initialisation shouldnt be necessary...
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityVectorConnectionsParametersSameReferenceSetArray[i] = new vector<GIAentityNode*>();
	}
	*/
	#endif
	conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	timeConditionNode = NULL;


	/*Query Variables*/
	isQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	isAnswerToQuery = false;
	testedForQueryComparison = false;
	testedForQueryComparisonTemp = false;
	queryAnswerContext = false;
	queryEntityTraced = false;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	entityCorrespondingBestMatch = NULL;
	#endif


	/*GIA Miscellaneous Internal Variables*/
	disabled = false;
	permanentConcept = false;
	firstSentenceToAppearInNetwork = true;
		//CXL:
	CXLdummyNode = false;
		//referencing:
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	minimumEntityIndexOfReferenceSet = GIA_REFERENCE_SET_ID_UNDEFINED;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	wasReferenceTemp = false;
	#endif
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	wasReference = false;
	#endif
	#endif
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsSubstanceConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#endif
		//databasing:
	#ifdef GIA_USE_DATABASE
	added = false;		//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012
	modified = false;	//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012
	#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	conceptEntityLoaded = NULL;
	#endif
	#endif
		//nlg:
	#ifdef GIA_USE_NLG
	parsedForLanguageGeneration = false;
	sourceAddedInLanguageGeneration = false;		//added 3 Aug 2013 - why wasn't this being initialised?
	sourceReferencedInLanguageGeneration = false;
	#endif

	#ifdef USE_NLC
	NLCparsedForCodeBlocks = false;
	//parsedForNLCcodeBlocksActionRound = false;
	//parsedForNLCclassHeirarchy = false;
	NLCisSingularArgument = false;
	NLClocalListVariableHasBeenDeclared = false;
	NLClocalListVariableHasBeenInitialised = false;
	NLClogicalConditionOperation = false;		//required as some logical condition operation conditions (prepositions) require sentence context for detection (eg "for each/all/every")
	NLCparsedForlogicalConditionOperations = false;
	NLCconjunctionCondition = false;
	NLClogicalConditionConjunctionIndex = INT_DEFAULT_VALUE;
	NLCcontextGenerated = false;
	NLCoriginalNumericalVariableName = "";
	NLCcontextGeneratedTemp = false;
	NLCcategoryListCreatedTemp = false;
	#endif
	
	#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS
	conditionType2 = ""; 
	#endif
}
GIAentityNode::~GIAentityNode(void)
{
	//delete all connections
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityVectorConnectionsArray[i].begin(); connectionIter != entityVectorConnectionsArray[i].end(); connectionIter++)
		{
			#ifdef GIA_FREE_MEMORY_DEBUG
			//cout << "deleting: vector connection: " << (*connectionIter)->entityName << endl;
			#endif
			delete *connectionIter;
		}
		entityVectorConnectionsArray[i].clear();
	}

	#ifdef GIA_USE_DATABASE
	#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	delete conceptEntityLoaded;
	#endif
	#endif
}

void deleteEntitiesInEntityNodeList(vector<GIAentityNode*> * entityNodesActiveListComplete)
{
	for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
	{
		GIAentityNode * entityNode = *entityIter;
		#ifdef GIA_FREE_MEMORY_DEBUG
		cout << "deleting: entityNode: " << entityNode->entityName << endl;
		#endif
		delete entityNode;
	}
}



#ifdef GIA_USE_DATABASE
void DBsetEntityConnectionsReferenceListsLoaded(GIAentityNode * entityNode, bool loaded)
{
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityNode->entityVectorConnectionsReferenceListLoadedArray[i] = loaded;
	}
}
#endif


void disconnectNodeFromAllButDefinitions(GIAentityNode * entityNode)
{
	cout << "warning: disconnectNodeFromAllButDefinitions() not yet coded" << endl;
	/* need to delete its instance from the reverse lists of each node of each list of this entity...
	actionNodeList->clear();
	incomingActionNodeList->clear();

	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
	incomingActionNodeList->clear();
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

string printQuantityNumberString(GIAentityNode * entityNode)
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

#ifdef GIA_SUPPORT_ALIASES

void convertAliasesStringToAliases(GIAentityNode * entityNode, string aliasesString)
{
	entityNode->aliasList = explode(aliasesString, GIA_ALIASES_CONVERT_TO_STRING_DELIMITER);
}

void convertAliasesToAliasesString(GIAentityNode * entityNode, string * aliasesString)
{
	*aliasesString = "";
	for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
	{
		*aliasesString = *aliasesString + *aliasIter + GIA_ALIASES_CONVERT_TO_STRING_DELIMITER;
		#ifdef GIA_ALIASES_DEBUG
		cout << "*aliasesString = " << *aliasesString << endl;
		#endif
	}
}

//http://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array
vector<string> explode(const string& str, const char& ch)
{
	string next = "";
	vector<string> result;

	// For each character in the string
	for(string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		// If we've hit the terminal character
		if(*it == ch)
		{
			// If we have some characters accumulated
			if(next.length() > 0)
			{
				// Add them to the result vector
				#ifdef GIA_ALIASES_DEBUG
				cout << "found alias: " << next << endl;
				#endif
				result.push_back(next);
				next = "";
			}
		}
		else
		{
			// Accumulate the next character into the sequence
			next += *it;
		}
	}

	return result;
}

string * convertDelimitedStringToArray(string str, char delimiter)
{
	int delimiterCount = 0;

	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == delimiter)
		{
			delimiterCount++;
		}
	}
	string * stringArray = new string[delimiterCount+1];
	string currentString = "";
	delimiterCount = 0;
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == delimiter)
		{
			stringArray[delimiterCount] = currentString;
			//cout << "convertDelimitedStringToArray: " << currentString << endl;
			currentString = "";
			delimiterCount++;
		}
		else
		{
			currentString = currentString + str[i];
		}
	}
	//add last array item;
	stringArray[delimiterCount] = currentString;
	//cout << "convertDelimitedStringToArray: " << currentString << endl;
	currentString = "";
	delimiterCount++;

	return stringArray;
}

#endif



#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
EntityCharacteristic::EntityCharacteristic()
{
	name = "";
	value = "";
	arrayIndex = 0;
	isNegative = false;	//only used for test (not assign)
}
EntityCharacteristic::EntityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = 0;
	isNegative = false;
}
EntityCharacteristic::EntityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, bool isNegativeNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = 0;
	isNegative = isNegativeNew;
}
EntityCharacteristic::EntityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = entityPropertyArrayIndexNew;
	isNegative = false;
}
EntityCharacteristic::EntityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew, bool isNegativeNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = entityPropertyArrayIndexNew;
	isNegative = isNegativeNew;
}
EntityCharacteristic::~EntityCharacteristic(void)
{
}

bool testEntityCharacteristics(GIAentityNode * entity, vector<EntityCharacteristic*> * redistributeSpecialCasePropertiesTestVector, bool andOrOr)
{
	bool passFound = false;
	bool failureFound = false;
	bool vectorHasItems = false;
	for(vector<EntityCharacteristic*>::iterator entityCharacteristicIter = redistributeSpecialCasePropertiesTestVector->begin(); entityCharacteristicIter != redistributeSpecialCasePropertiesTestVector->end(); entityCharacteristicIter++)
	{
		vectorHasItems = true;
		EntityCharacteristic * entityCharacteristic = *entityCharacteristicIter;
		if(testEntityCharacteristic(entity, entityCharacteristic))
		{
			//cout << "passFound" << endl;
			passFound = true;
		}
		else
		{
			failureFound = true;
		}
		//cout << "done" << endl;
	}
	//cout << "done" << endl;

	bool passedRelation = true;
	if(vectorHasItems)
	{
		if(andOrOr)
		{
			if(failureFound)
			{
				passedRelation = false;
			}
			else
			{
				passedRelation = true;
			}
		}
		else
		{
			if(passFound)
			{
				passedRelation = true;
			}
			else
			{
				passedRelation = false;
			}
		}
	}
	return passedRelation;
}
void setEntityCharacteristics(GIAentityNode * entity, vector<EntityCharacteristic*> * redistributeSpecialCasePropertiesAssignmentVector)
{
	for(vector<EntityCharacteristic*>::iterator entityCharacteristicIter = redistributeSpecialCasePropertiesAssignmentVector->begin(); entityCharacteristicIter != redistributeSpecialCasePropertiesAssignmentVector->end(); entityCharacteristicIter++)
	{
		EntityCharacteristic * entityCharacteristic = *entityCharacteristicIter;
		setEntityCharacteristic(entity, entityCharacteristic);
	}
}
bool testEntityCharacteristic(GIAentityNode * entity, EntityCharacteristic * entityCharacteristic)
{
	bool foundMatch = false;
	bool illegalVariable = false;
	
	//cout << "testEntityCharacteristic():" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;

	/*GIA Entity Type*/
	testEntityCharacteristicIterationbool(entity->isConcept, entityCharacteristic, "isConcept", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isSubstance, entityCharacteristic, "isSubstance", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isAction, entityCharacteristic, "isAction", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isCondition, entityCharacteristic, "isCondition", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstanceIsAction, entityCharacteristic, "hasAssociatedInstanceIsAction", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstanceIsCondition, entityCharacteristic, "hasAssociatedInstanceIsCondition", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isSubstanceQuality, entityCharacteristic, "isSubstanceQuality", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isSubstanceConcept, entityCharacteristic, "isSubstanceConcept", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	testEntityCharacteristicIterationbool(entity->negative, entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	testEntityCharacteristicIterationint(entity->conditionType, entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	testEntityCharacteristicIterationbool(entity->hasQuantity, entityCharacteristic, "hasQuantity", &foundMatch);
	testEntityCharacteristicIterationint(entity->quantityNumber, entityCharacteristic, "quantityNumber", &foundMatch);
	testEntityCharacteristicIterationstring(entity->quantityNumberString, entityCharacteristic, "quantityNumberString", &foundMatch);
	testEntityCharacteristicIterationint(entity->quantityModifier, entityCharacteristic, "quantityModifier", &foundMatch);
	testEntityCharacteristicIterationstring(entity->quantityModifierString, entityCharacteristic, "quantityModifierString", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasQuantityMultiplier, entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasMeasure, entityCharacteristic, "hasMeasure", &foundMatch);
	testEntityCharacteristicIterationint(entity->measureType, entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	testEntityCharacteristicIterationint(entity->grammaticalNumber, entityCharacteristic, "grammaticalNumber", &foundMatch);
	testEntityCharacteristicIterationint(entity->grammaticalWordTypeTemp, entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex], entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	testEntityCharacteristicIterationint(entity->grammaticalTenseTemp, entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->grammaticalDefiniteTemp, entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->grammaticalIndefinitePluralTemp, entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->grammaticalProperNounTemp, entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	testEntityCharacteristicIterationint(entity->grammaticalGenderTemp, entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->grammaticalPronounTemp, entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	testEntityCharacteristicIterationint(entity->grammaticalIndexOfDeterminerTemp, entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	testEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	testEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	testEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	testEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	testEntityCharacteristicIterationbool(entity->isSubjectTemp, entityCharacteristic, "isSubjectTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isObjectTemp, entityCharacteristic, "isObjectTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasSubstanceTemp, entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isActionTemp, entityCharacteristic, "isActionTemp", &foundMatch);
	testEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	testEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	testEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	testEntityCharacteristicIterationbool(entity->entityAlreadyDeclaredInThisContext, entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_SUPPORT_ALIASES
	testEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_SUPPORT_NUMBER_OF
	testEntityCharacteristicIterationbool(entity->isNumberOf, entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	testEntityCharacteristicIterationbool(entity->wasReference, entityCharacteristic, "wasReference", &foundMatch);
	#else
	if(entityCharacteristic->name == "wasReference")
	{
		illegalVariable = true;	//ignore illegal variable - added 2g6a
	}
	#endif
	testEntityCharacteristicIterationbool(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp, entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->mustSetIsSubstanceConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsSubstanceConceptBasedOnApposRelation", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);


	bool result = false;
	if(entityCharacteristic->isNegative)
	{
		if(foundMatch)
		{
			result = false;
		}
		else
		{
			result = true;
		}
	}
	else
	{
		if(foundMatch)
		{
			result = true;
		}
		else
		{
			result = false;
		}
	}
	
	if(illegalVariable)
	{
		result = true;
	}

	return result;
}
void testEntityCharacteristicIterationbool(bool entityVal, EntityCharacteristic * entityCharacteristicTest, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicTest->name == iterationVariable)
	{
		bool entityCharacteristicTestValue;
		if(entityCharacteristicTest->value == "true")
		{
			entityCharacteristicTestValue = true;
		}
		else if(entityCharacteristicTest->value == "false")
		{
			entityCharacteristicTestValue = false;
		}
		else
		{
			cout << "testEntityCharacteristicIterationbool() error: illegal entityCharacteristicTestValue for " << iterationVariable << ": " << entityCharacteristicTest->value << endl;
		}
		//bool entityCharacteristicTestValue = atoi(entityCharacteristicTest->value.c_str());		//if GIA Translator XML file booleans are defined as '1'/'0' instead of 'true'/'false'
		if(entityCharacteristicTestValue == entityVal)
		{
			//cout << "testEntityCharacteristicIterationbool(): " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			*foundMatch = true;
		}
	}
}
void testEntityCharacteristicIterationint(int entityVal, EntityCharacteristic * entityCharacteristicTest, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicTest->name == iterationVariable)
	{
		int entityCharacteristicTestValue = atoi(entityCharacteristicTest->value.c_str());
		if(entityCharacteristicTestValue == entityVal)
		{
			//cout << "testEntityCharacteristicIterationint(): " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			*foundMatch = true;
		}
	}
}
void testEntityCharacteristicIterationstring(string entityVal, EntityCharacteristic * entityCharacteristicTest, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicTest->name == iterationVariable)
	{
		string entityCharacteristicTestValue = entityCharacteristicTest->value;
		if(entityCharacteristicTestValue == entityVal)
		{
			//cout << "testEntityCharacteristicIterationstring(): " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			*foundMatch = true;
		}
	}
}


bool setEntityCharacteristic(GIAentityNode * entity, EntityCharacteristic * entityCharacteristic)
{
	bool foundMatch = false;

	//cout << "setEntityCharacteristic():" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;

	/*GIA Entity Type*/
	setEntityCharacteristicIterationbool(&(entity->isConcept), entityCharacteristic, "isConcept", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isSubstance), entityCharacteristic, "isSubstance", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isAction), entityCharacteristic, "isAction", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isCondition), entityCharacteristic, "isCondition", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstance), entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstanceIsAction), entityCharacteristic, "hasAssociatedInstanceIsAction", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstanceIsCondition), entityCharacteristic, "hasAssociatedInstanceIsCondition", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedTime), entityCharacteristic, "hasAssociatedTime", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isSubstanceQuality), entityCharacteristic, "isSubstanceQuality", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isSubstanceConcept), entityCharacteristic, "isSubstanceConcept", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isActionConcept), entityCharacteristic, "isActionConcept", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->negative), entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	setEntityCharacteristicIterationint(&(entity->conditionType), entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	setEntityCharacteristicIterationbool(&(entity->hasQuantity), entityCharacteristic, "hasQuantity", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->quantityNumber), entityCharacteristic, "quantityNumber", &foundMatch);
	setEntityCharacteristicIterationstring(&(entity->quantityNumberString), entityCharacteristic, "quantityNumberString", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->quantityModifier), entityCharacteristic, "quantityModifier", &foundMatch);
	setEntityCharacteristicIterationstring(&(entity->quantityModifierString), entityCharacteristic, "quantityModifierString", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasQuantityMultiplier), entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasMeasure), entityCharacteristic, "hasMeasure", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->measureType), entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	setEntityCharacteristicIterationint(&(entity->grammaticalNumber), entityCharacteristic, "grammaticalNumber", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->grammaticalWordTypeTemp), entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex]), entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->grammaticalTenseTemp), entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->grammaticalDefiniteTemp), entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->grammaticalIndefinitePluralTemp), entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->grammaticalProperNounTemp), entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->grammaticalGenderTemp), entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->grammaticalPronounTemp), entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	setEntityCharacteristicIterationint(&(entity->grammaticalIndexOfDeterminerTemp), entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	setEntityCharacteristicIterationstring(&(entity->stanfordPOStemp), entityCharacteristic, "stanfordPOStemp", &foundMatch);
	setEntityCharacteristicIterationstring(&(entity->NormalizedNERtemp), entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	setEntityCharacteristicIterationstring(&(entity->TimexTemp), entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	setEntityCharacteristicIterationint(&(entity->NERTemp), entityCharacteristic, "NERTemp", &foundMatch);


	/*GIA Translator Temporary Variables*/
	setEntityCharacteristicIterationbool(&(entity->isSubjectTemp), entityCharacteristic, "isSubjectTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isObjectTemp), entityCharacteristic, "isObjectTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasSubstanceTemp), entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isActionTemp), entityCharacteristic, "isActionTemp", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->entityIndexTemp), entityCharacteristic, "entityIndexTemp", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->sentenceIndexTemp), entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isToBeComplimentOfActionTemp), entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	setEntityCharacteristicIterationbool(&(entity->disableParsingAsPrepositionRelationTemp), entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	setEntityCharacteristicIterationbool(&(entity->entityAlreadyDeclaredInThisContext), entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstanceTemp), entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_SUPPORT_ALIASES
	setEntityCharacteristicIterationbool(&(entity->isName), entityCharacteristic, "isName", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isNameQuery), entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_SUPPORT_NUMBER_OF
	setEntityCharacteristicIterationbool(&(entity->isNumberOf), entityCharacteristic, "isNumberOf", &foundMatch);
	#endif

	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	setEntityCharacteristicIterationbool(&(entity->wasReference), entityCharacteristic, "wasReference", &foundMatch);
	#endif
	setEntityCharacteristicIterationbool(&(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp), entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->mustSetIsSubstanceConceptBasedOnApposRelation), entityCharacteristic, "mustSetIsSubstanceConceptBasedOnApposRelation", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isPronounReference), entityCharacteristic, "isPronounReference", &foundMatch);

	if(!foundMatch)
	{
		cout << "setEntityCharacteristic() error: entityCharacteristic not found:" << entityCharacteristic->name << endl;
	}
	return foundMatch;
}

void setEntityCharacteristicIterationbool(bool * entityVal, EntityCharacteristic * entityCharacteristicSet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		bool entityCharacteristicSetValue;
		if(entityCharacteristicSet->value == "true")
		{
			entityCharacteristicSetValue = true;
		}
		else if(entityCharacteristicSet->value == "false")
		{
			entityCharacteristicSetValue = false;
		}
		else
		{
			cout << "testEntityCharacteristicIterationbool() error: illegal entityCharacteristicTestValue for " << iterationVariable << ": " << entityCharacteristicSet->value << endl;
		}
		//cout << "setEntityCharacteristicIterationbool(): " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;

		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void setEntityCharacteristicIterationint(int * entityVal, EntityCharacteristic * entityCharacteristicSet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		int entityCharacteristicSetValue = atoi(entityCharacteristicSet->value.c_str());
		//cout << "setEntityCharacteristicIterationint(): " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void setEntityCharacteristicIterationstring(string * entityVal, EntityCharacteristic * entityCharacteristicSet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		string entityCharacteristicSetValue = entityCharacteristicSet->value;
		//cout << "setEntityCharacteristicIterationstring(): " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}

bool getEntityCharacteristic(GIAentityNode * entity, EntityCharacteristic * entityCharacteristic)
{
	bool foundMatch = false;
	//cout << "testEntityCharacteristic():" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;

	/*GIA Entity Type*/
	getEntityCharacteristicIterationbool(entity->isConcept, entityCharacteristic, "isConcept", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isSubstance, entityCharacteristic, "isSubstance", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isAction, entityCharacteristic, "isAction", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isCondition, entityCharacteristic, "isCondition", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstanceIsAction, entityCharacteristic, "hasAssociatedInstanceIsAction", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstanceIsCondition, entityCharacteristic, "hasAssociatedInstanceIsCondition", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isSubstanceQuality, entityCharacteristic, "isSubstanceQuality", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isSubstanceConcept, entityCharacteristic, "isSubstanceConcept", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	getEntityCharacteristicIterationbool(entity->negative, entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	getEntityCharacteristicIterationint(entity->conditionType, entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	getEntityCharacteristicIterationbool(entity->hasQuantity, entityCharacteristic, "hasQuantity", &foundMatch);
	getEntityCharacteristicIterationint(entity->quantityNumber, entityCharacteristic, "quantityNumber", &foundMatch);
	getEntityCharacteristicIterationstring(entity->quantityNumberString, entityCharacteristic, "quantityNumberString", &foundMatch);
	getEntityCharacteristicIterationint(entity->quantityModifier, entityCharacteristic, "quantityModifier", &foundMatch);
	getEntityCharacteristicIterationstring(entity->quantityModifierString, entityCharacteristic, "quantityModifierString", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasQuantityMultiplier, entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasMeasure, entityCharacteristic, "hasMeasure", &foundMatch);
	getEntityCharacteristicIterationint(entity->measureType, entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	getEntityCharacteristicIterationint(entity->grammaticalNumber, entityCharacteristic, "grammaticalNumber", &foundMatch);
	getEntityCharacteristicIterationint(entity->grammaticalWordTypeTemp, entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex], entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	getEntityCharacteristicIterationint(entity->grammaticalTenseTemp, entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->grammaticalDefiniteTemp, entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->grammaticalIndefinitePluralTemp, entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->grammaticalProperNounTemp, entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	getEntityCharacteristicIterationint(entity->grammaticalGenderTemp, entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->grammaticalPronounTemp, entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	getEntityCharacteristicIterationint(entity->grammaticalIndexOfDeterminerTemp, entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	getEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	getEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	getEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	getEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	getEntityCharacteristicIterationbool(entity->isSubjectTemp, entityCharacteristic, "isSubjectTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isObjectTemp, entityCharacteristic, "isObjectTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasSubstanceTemp, entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isActionTemp, entityCharacteristic, "isActionTemp", &foundMatch);
	getEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	getEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	getEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	getEntityCharacteristicIterationbool(entity->entityAlreadyDeclaredInThisContext, entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_SUPPORT_ALIASES
	getEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_SUPPORT_NUMBER_OF
	getEntityCharacteristicIterationbool(entity->isNumberOf, entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	getEntityCharacteristicIterationbool(entity->wasReference, entityCharacteristic, "wasReference", &foundMatch);
	#endif
	getEntityCharacteristicIterationbool(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp, entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->mustSetIsSubstanceConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsSubstanceConceptBasedOnApposRelation", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);

	if(!foundMatch)
	{
		cout << "getEntityCharacteristic() error: entityCharacteristic not found:" << entityCharacteristic->name << endl;
	}
	return foundMatch;
}
void getEntityCharacteristicIterationbool(bool entityVal, EntityCharacteristic * entityCharacteristicGet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicGet->name == iterationVariable)
	{
		string entityCharacteristicGetValue = "";
		if(entityVal == true)
		{
			entityCharacteristicGetValue = "true";
		}
		else
		{
			entityCharacteristicGetValue = "false";
		}
		entityCharacteristicGet->value = entityCharacteristicGetValue;
		*foundMatch = true;
	}
}
void getEntityCharacteristicIterationint(int entityVal, EntityCharacteristic * entityCharacteristicGet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicGet->name == iterationVariable)
	{
		char tempString[ENTITY_CHARACTERISTIC_MAX_VALUE_SIZE];
		sprintf(tempString, "%d", entityVal);
		string entityCharacteristicGetValue = string(tempString);
		entityCharacteristicGet->value = entityCharacteristicGetValue;
		*foundMatch = true;
	}
}
void getEntityCharacteristicIterationstring(string entityVal, EntityCharacteristic * entityCharacteristicGet, string iterationVariable, bool *foundMatch)
{
	if(entityCharacteristicGet->name == iterationVariable)
	{
		string entityCharacteristicGetValue = entityVal;
		entityCharacteristicGet->value = entityCharacteristicGetValue;
		*foundMatch = true;
	}
}
#endif





/*
void deleteAllEntitiesInConceptEntityNodeList(unordered_map<string, GIAentityNode*> * conceptEntityNodesList);
{
	for(vector<GIAentityNode*>::iterator conceptEntityNodesListIter = conceptEntityNodesList.begin(); conceptEntityNodesListIter != conceptEntityNodesList.end(); conceptEntityNodesListIter++)
	{
		GIAentityNode * conceptEntityNode = sentenceConceptEntityNodesListTempNotUsedIter->second;

		for(vector<GIAentityNode*>::iterator instanceEntityNodesListIter = conceptEntityNode->associatedInstanceNodeList.begin(); instanceEntityNodesListIter != conceptEntityNode->associatedInstanceNodeList.end(); )
		{
			GIAentityNode * instanceEntityNode = *instanceEntityNodesListIter;
			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntityNode->entityVectorConnectionsArray[i].begin(); connectionIter != instanceEntityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					delete *connectionIter;
				}
				instanceEntityNode->entityVectorConnectionsArray[i].clear();
			}
			#ifdef GIA_SUPPORT_ALIASES
			delete aliasList;
			#endif
			#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
			delete conceptEntityLoaded;
			#endif

			instanceEntityNodesListIter = conceptEntityNode->associatedInstanceNodeList.erase(instanceEntityNodesListIter);
		}


		GIAentityNode * currentInstanceInConcept = conceptEntityNodeTemp->
		string entityNodeNameTemp = conceptEntityNodeTemp->entityName;
		sentenceConceptEntityNodesListTempNotUsedMap.insert(pair<string, GIAentityNode*>(entityNodeNameTemp, conceptEntityNodeTemp));

		#ifdef GIA_SUPPORT_ALIASES
		delete aliasList;
		#endif
		#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
		delete conceptEntityLoaded;
		#endif

	}
}
*/

