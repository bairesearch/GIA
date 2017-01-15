/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2l 24-July-2013
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
	idActiveList = 0;
	idActiveEntityTypeList = 0;	//temporary ID reserved for specific entity types; concept, action, substance etc
	idActiveListReorderdIDforXMLsave = 0;
	idInstance = 0;		//set as concept by default (GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE)

	entityName = "";
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	wordOrig = "";		//this needs to be added to XML i/o + file system database i/o [used for NLG2 bug]
	#endif
	confidence = 1.0;

	isConcept = false;
	isSubstance = false;
	isAction = false;
	isCondition = false;
	hasAssociatedInstance = false;
	hasAssociatedInstanceIsAction = false;
	hasAssociatedInstanceIsCondition = false;
	hasAssociatedTime = false;
	hasProgressiveTemp = false;
	isSubstanceQuality = false;
	isSubstanceConcept = false;
	
	//type = undefinedEntityType;
	//instance = undefinedInstance;

	actionSubjectEntity = NULL;
	actionObjectEntity = NULL;

	conditionSubjectEntity = NULL;
	conditionObjectEntity = NULL;
	conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	timeConditionNode = NULL;

	entityNodeDefiningThisInstance = NULL;

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
	printCoordsAlreadyDefined = false;

	for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
	{
		grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = false;
	}
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	grammaticalNumber = GRAMMATICAL_NUMBER_UNDEFINED;
	grammaticalDefiniteTemp = GRAMMATICAL_DEFINITE_UNDEFINED;
	grammaticalRelexPersonOrStanfordProperNounTemp = GRAMMATICAL_PERSON_UNDEFINED;
	grammaticalGenderTemp = GRAMMATICAL_GENDER_UNDEFINED;
	//grammaticalCountTemp = GRAMMATICAL_COUNT_UNDEFINED;
	grammaticalPronounTemp = GRAMMATICAL_PRONOUN_UNDEFINED;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	grammaticalDefiniteIndexOfDeterminerTemp = GIA_ENTITY_INDEX_UNDEFINED;
	#endif
	isSubjectTemp = false;
	isObjectTemp = false;
	hasSubstanceTemp = false;
	//hasQualityTemp = false;
	isActionTemp = false;
	entityIndexTemp = GIA_ENTITY_INDEX_UNDEFINED;	//was 0 before 11 October 2012
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED;	//was 0 before 11 October 2012

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

	#ifdef GIA_USE_ADVANCED_REFERENCING
	entityCorrespondingBestMatch = NULL;
	#endif

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


	#ifdef GIA_USE_STANFORD_CORENLP
	/*
	CharacterOffsetBeginTemp = -1;
	CharacterOffsetEndTemp = -1;
	*/
	stanfordPOStemp = "";
	NERTemp = FEATURE_NER_UNDEFINED;
	NormalizedNERtemp = "";
	TimexTemp = "";
	#endif


	entityAlreadyDeclaredInThisContext = false;

	hasAssociatedInstanceTemp = false;

	isQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	#ifdef GIA_SUPPORT_ALIASES
	isName = false;	
	isNameQuery = false;
	#endif	
	isAnswerToQuery = false;
	testedForQueryComparison = false;
	testedForQueryComparisonTemp = false;
	queryAnswerContext = false;

	negative = false;

	disableParsingAsPrepositionRelationTemp = false;

	queryEntityTraced = false;

	disabled = false;
	permanentConcept = false;

	CXLdummyNode = false;

	firstSentenceToAppearInNetwork = true;

	wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;

	#ifdef GIA_USE_ADVANCED_REFERENCING
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	wasReferenceTemp = false;
	#endif	
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	wasReference = false;
	#endif		
	#endif

	#ifdef GIA_USE_DATABASE
	added = false;		//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012
	modified = false;	//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012

	#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	conceptEntityLoaded = NULL;
	#endif
	#endif

	#ifdef GIA_USE_NLG
	parsedForLanguageGeneration = false;
	sourceReferencedInLanguageGeneration = false;
	#endif

	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	isName = false;
	#endif
	
	isToBeComplimentOfActionTemp = false;
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

