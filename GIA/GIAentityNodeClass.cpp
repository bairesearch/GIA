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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4c 17-January-2017
 *
 *******************************************************************************/


#include "GIAentityNodeClass.hpp"


string quantityNumberLowNameArray[QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
string quantityNumberTensNameArray[QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES] = {"zero", "ten", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
string quantityMultiplierNameArray[QUANTITY_MULTIPLIER_NUMBER_OF_TYPES] = {"one", "ten", "hundred", "thousand", "million", "billion", "trillion"};
string quantityModifierNameArray[QUANTITY_MODIFIER_NUMBER_OF_TYPES] = {"almost"};

//int grammaticalTenseNameLengthsArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {9, 7, 4, 6};

#ifdef GIA_DATABASE
#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
GIAnetworkIndexEntityLoaded::GIAnetworkIndexEntityLoaded(void)
{
	loaded = false;
	numberOfInstances = 0;
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	disabled = false;
	#endif
}
GIAnetworkIndexEntityLoaded::~GIAnetworkIndexEntityLoaded(void)
{
}
#endif
#endif

//~nouns
GIAentityNode::GIAentityNode(void)
{
	/*GIA Internal Entity Referencing*/
	idActiveList = 0;
	idActiveListReorderdIDforXMLsave = 0;
	idInstance = GIA_ENTITY_ID_INSTANCE_NETWORK_INDEX_ENTITY;		//set as networkIndex by default (GIA_DATABASE_NODE_NETWORK_INDEX_ID_INSTANCE)


	/*GIA Entity Name*/
	entityName = "";
	#ifdef GIA_WORD_ORIG
	wordOrig = "";
	#endif
	confidence = 1.0;


	/*GIA Entity Type*/
	entityType = GIA_ENTITY_TYPE_TYPE_UNDEFINED;
	isActionConcept = false;
	hasAssociatedInstance = false;
	hasAssociatedTime = false;
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
	#ifdef GIA_PREDETERMINERS
	grammaticalPredeterminerTemp = GRAMMATICAL_PREDETERMINER_UNDEFINED;
	#endif
	#ifdef GIA_STANFORD_CORENLP
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
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	disableParsingAsPrepositionRelationTemp = false;
	#endif
	entityAlreadyDeclaredInThisContext = false;
	hasAssociatedInstanceTemp = false;
	#ifdef GIA_ALIASES
	isName = false;
	isNameQuery = false;
	#endif
	#ifdef GIA_NUMBER_OF
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
	#ifdef GIA_DATABASE
	GIAentityNodeClassClass().DBsetEntityConnectionsReferenceListsLoaded(this, true);	//for now, assume that a new entity will be configured with its connections loaded into RAM
	#endif
	/*
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_DEFINITIONS] = entityNodeDefinitionList;
	entityVectorConnectionsSpecialConditionsHavingBeingArray[GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPE_SUBSTANCES] = propertyNodeList;
	*/
	#ifdef GIA_ADVANCED_REFERENCING
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
	permanentNetworkIndex = false;
	firstSentenceToAppearInNetwork = true;
		//CXL:
	CXLdummyNode = false;
		//referencing:
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	minimumEntityIndexOfReferenceSet = GIA_REFERENCE_SET_ID_UNDEFINED;
	#endif
	#ifdef GIA_ADVANCED_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	wasReferenceTemp = false;
	#endif
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	wasReference = false;
	#endif
	#endif
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	mustNotSetIsConceptBasedOnPrenomonalModifierRelation = false;
	#endif
		//subclasses:
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	isSubClass = false;
	#endif
	convertToSubClass = false;
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	addSubClass = false;
	#endif
	#endif
	#endif
		//expletives:
	#ifdef GIA_EXPLETIVES
	isExpletive = false;
	#endif
		//databasing:
	#ifdef GIA_DATABASE
	added = false;		//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012
	modified = false;	//implies database Update is Required		//CHECKTHIS removed 'bool' 21 July 2012
	#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	networkIndexEntityLoaded = NULL;
	#endif
	#endif
		//nlg:
	#ifdef GIA_NLG
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
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	NLCmathTextParsablePhraseEntity = false;
	#endif
	NLCisAlias = false;
	NLCidInstanceBackup = INT_DEFAULT_VALUE;
	NLCfirstInstanceOfProperNounInContext = false;
	#endif

	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
	conditionType2 = "";
	#endif
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
	conditionTwoWay = false;
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	inverseConditionTwoWay = false;
	#endif
	#endif
	#endif
}
/*
#ifdef USE_NLC
//#ifdef NLC_NONOO
GIAentityNode::GIAentityNode(string newEntityName)	//must be synced with the above constructor - NB must create GIAentityNode::initialiseVariables if this constructor is actually in use
{

}
//#endif
#endif
*/
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
			delete* connectionIter;
		}
		entityVectorConnectionsArray[i].clear();
	}

	#ifdef GIA_DATABASE
	#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	delete networkIndexEntityLoaded;
	#endif
	#endif
}

void GIAentityNodeClassClass::deleteEntitiesInEntityNodeList(vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
	{
		GIAentityNode* entityNode = *entityIter;
		#ifdef GIA_FREE_MEMORY_DEBUG
		cout << "deleting: entityNode: " << entityNode->entityName << endl;
		#endif
		delete entityNode;
	}
}



#ifdef GIA_DATABASE
void GIAentityNodeClassClass::DBsetEntityConnectionsReferenceListsLoaded(GIAentityNode* entityNode, bool loaded)
{
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityNode->entityVectorConnectionsReferenceListLoadedArray[i] = loaded;
	}
}
#endif


void GIAentityNodeClassClass::disconnectNodeFromAllButDefinitions(const GIAentityNode* entityNode)
{
	cout << "warning: disconnectNodeFromAllButDefinitions{} not yet coded" << endl;
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



int GIAentityNodeClassClass::calculateQuantityNumberInt(const string quantityNumberString)
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
		quantityNumberInt = SHAREDvars.convertStringToInt(quantityNumberString);
	}

	return quantityNumberInt;
}


int GIAentityNodeClassClass::calculateQuantityMultiplierInt(const string quantityMultiplierString)
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

		quantityMultiplierInt = SHAREDvars.convertStringToInt(quantityMultiplierString);
	}

	return quantityMultiplierInt;
}

int GIAentityNodeClassClass::calculateQuantityModifierInt(const string quantityModifierString)
{
	cout << "warning: calculateQuantityModifierInt{} not yet implemented" << endl;

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

string GIAentityNodeClassClass::printQuantityNumberString(const GIAentityNode* entityNode)
{
	string quantityNumberStringTemp;

	if(entityNode->hasQuantityMultiplier)
	{
		quantityNumberStringTemp = SHAREDvars.convertIntToString(entityNode->quantityNumber);
	}
	else
	{
		quantityNumberStringTemp = entityNode->quantityNumberString;
	}

	return quantityNumberStringTemp;
}

#ifdef GIA_ALIASES

void GIAentityNodeClassClass::convertAliasesStringToAliases(GIAentityNode* entityNode, string aliasesString)
{
	entityNode->aliasList = this->explode(aliasesString, GIA_ALIASES_CONVERT_TO_STRING_DELIMITER);
}

void GIAentityNodeClassClass::convertAliasesToAliasesString(GIAentityNode* entityNode, string* aliasesString)
{
	*aliasesString = "";
	for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
	{
		*aliasesString = *aliasesString +* aliasIter + GIA_ALIASES_CONVERT_TO_STRING_DELIMITER;
		#ifdef GIA_ALIASES_DEBUG
		cout << "*aliasesString = " <<* aliasesString << endl;
		#endif
	}
}

//http://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array
vector<string> GIAentityNodeClassClass::explode(const string& str, const char& ch)
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

string* GIAentityNodeClassClass::convertDelimitedStringToArray(const string str, const char delimiter)
{
	int delimiterCount = 0;

	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == delimiter)
		{
			delimiterCount++;
		}
	}
	string* stringArray = new string[delimiterCount+1];
	string currentString = "";
	delimiterCount = 0;
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == delimiter)
		{
			stringArray[delimiterCount] = currentString;
			#ifdef GIA_DEBUG
			//cout << "convertDelimitedStringToArray: " << currentString << endl;
			#endif
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
	#ifdef GIA_DEBUG
	//cout << "convertDelimitedStringToArray: " << currentString << endl;
	#endif
	currentString = "";
	delimiterCount++;

	return stringArray;
}

#endif



#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
GIAentityCharacteristic::GIAentityCharacteristic()
{
	name = "";
	value = "";
	arrayIndex = 0;
	isNegative = false;	//only used for test (not assign)
}
GIAentityCharacteristic::GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = 0;
	isNegative = false;
}
GIAentityCharacteristic::GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, bool isNegativeNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = 0;
	isNegative = isNegativeNew;
}
GIAentityCharacteristic::GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = entityPropertyArrayIndexNew;
	isNegative = false;
}
GIAentityCharacteristic::GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew, bool isNegativeNew)
{
	name = entityPropertyNameNew;
	value = entityPropertyValueNew;
	arrayIndex = entityPropertyArrayIndexNew;
	isNegative = isNegativeNew;
}
GIAentityCharacteristic::~GIAentityCharacteristic(void)
{
}

bool GIAentityNodeClassClass::testEntityCharacteristics(const GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesTestVector, const bool andOrOr)
{
	bool passFound = false;
	bool failureFound = false;
	bool vectorHasItems = false;
	for(vector<GIAentityCharacteristic*>::iterator entityCharacteristicIter = redistributeSpecialCasePropertiesTestVector->begin(); entityCharacteristicIter != redistributeSpecialCasePropertiesTestVector->end(); entityCharacteristicIter++)
	{
		vectorHasItems = true;
		GIAentityCharacteristic* entityCharacteristic = *entityCharacteristicIter;
		if(this->testEntityCharacteristic(entity, entityCharacteristic))
		{
			passFound = true;
		}
		else
		{
			failureFound = true;
		}
	}

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
void GIAentityNodeClassClass::setEntityCharacteristics(GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesAssignmentVector)
{
	for(vector<GIAentityCharacteristic*>::iterator entityCharacteristicIter = redistributeSpecialCasePropertiesAssignmentVector->begin(); entityCharacteristicIter != redistributeSpecialCasePropertiesAssignmentVector->end(); entityCharacteristicIter++)
	{
		GIAentityCharacteristic* entityCharacteristic = *entityCharacteristicIter;
		this->setEntityCharacteristic(entity, entityCharacteristic);
	}
}
bool GIAentityNodeClassClass::testEntityCharacteristic(const GIAentityNode* entity, const GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;
	bool illegalVariable = false;

	#ifdef GIA_DEBUG
	//cout << "testEntityCharacteristic{}:" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;
	#endif

	/*GIA Entity Type*/
	this->testEntityCharacteristicIterationint(entity->entityType, entityCharacteristic, "entityType", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->negative, entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	this->testEntityCharacteristicIterationint(entity->conditionType, entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	this->testEntityCharacteristicIterationbool(entity->hasQuantity, entityCharacteristic, "hasQuantity", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->quantityNumber, entityCharacteristic, "quantityNumber", &foundMatch);
	this->testEntityCharacteristicIterationstring(entity->quantityNumberString, entityCharacteristic, "quantityNumberString", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->quantityModifier, entityCharacteristic, "quantityModifier", &foundMatch);
	this->testEntityCharacteristicIterationstring(entity->quantityModifierString, entityCharacteristic, "quantityModifierString", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasQuantityMultiplier, entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasMeasure, entityCharacteristic, "hasMeasure", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->measureType, entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	this->testEntityCharacteristicIterationint(entity->grammaticalNumber, entityCharacteristic, "grammaticalNumber", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->grammaticalWordTypeTemp, entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex], entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->grammaticalTenseTemp, entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->grammaticalDefiniteTemp, entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->grammaticalIndefinitePluralTemp, entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->grammaticalProperNounTemp, entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->grammaticalGenderTemp, entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->grammaticalPronounTemp, entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	this->testEntityCharacteristicIterationint(entity->grammaticalIndexOfDeterminerTemp, entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_STANFORD_CORENLP
	this->testEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	this->testEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	this->testEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	this->testEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	this->testEntityCharacteristicIterationbool(entity->isSubjectTemp, entityCharacteristic, "isSubjectTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isObjectTemp, entityCharacteristic, "isObjectTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasSubstanceTemp, entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isActionTemp, entityCharacteristic, "isActionTemp", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	this->testEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	this->testEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	this->testEntityCharacteristicIterationbool(entity->entityAlreadyDeclaredInThisContext, entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	this->testEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_NUMBER_OF
	this->testEntityCharacteristicIterationbool(entity->isNumberOf, entityCharacteristic, "isNumberOf", &foundMatch);
	#endif

	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	this->testEntityCharacteristicIterationbool(entity->wasReference, entityCharacteristic, "wasReference", &foundMatch);
	#else
	if(entityCharacteristic->name == "wasReference")
	{
		illegalVariable = true;	//ignore illegal variable - added 2g6a
	}
	#endif
	this->testEntityCharacteristicIterationbool(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp, entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->mustSetIsConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	this->testEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	this->testEntityCharacteristicIterationbool(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation, entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	this->testEntityCharacteristicIterationbool(entity->convertToSubClass, entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	this->testEntityCharacteristicIterationbool(entity->addSubClass, entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_EXPLETIVES
	this->testEntityCharacteristicIterationbool(entity->isExpletive, entityCharacteristic, "isExpletive", &foundMatch);	
	#endif	
	
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
void GIAentityNodeClassClass::testEntityCharacteristicIterationbool(const bool entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch)
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
			cout << "testEntityCharacteristicIterationbool{} error: illegal entityCharacteristicTestValue for " << iterationVariable << ": " << entityCharacteristicTest->value << endl;
		}
		//bool entityCharacteristicTestValue = convertStringToInt(entityCharacteristicTest->value);		//if GIA Translator XML file booleans are defined as '1'/'0' instead of 'true'/'false'
		if(entityCharacteristicTestValue == entityVal)
		{
			#ifdef GIA_DEBUG
			//cout << "testEntityCharacteristicIterationbool{}: " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			#endif
			*foundMatch = true;
		}
	}
}
void GIAentityNodeClassClass::testEntityCharacteristicIterationint(const int entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicTest->name == iterationVariable)
	{
		int entityCharacteristicTestValue = SHAREDvars.convertStringToInt(entityCharacteristicTest->value);
		if(entityCharacteristicTestValue == entityVal)
		{
			#ifdef GIA_DEBUG
			//cout << "testEntityCharacteristicIterationint{}: " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			#endif
			*foundMatch = true;
		}
	}
}
void GIAentityNodeClassClass::testEntityCharacteristicIterationstring(const string entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicTest->name == iterationVariable)
	{
		string entityCharacteristicTestValue = entityCharacteristicTest->value;
		if(entityCharacteristicTestValue == entityVal)
		{
			#ifdef GIA_DEBUG
			//cout << "testEntityCharacteristicIterationstring{}: " << entityCharacteristicTest->name << " = " << entityCharacteristicTestValue << endl;
			#endif
			*foundMatch = true;
		}
	}
}


bool GIAentityNodeClassClass::setEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;

	#ifdef GIA_DEBUG
	//cout << "setEntityCharacteristic{}:" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;
	#endif

	/*GIA Entity Type*/
	this->setEntityCharacteristicIterationint(&(entity->entityType), entityCharacteristic, "entityType", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isActionConcept), entityCharacteristic, "isActionConcept", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstance), entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasAssociatedTime), entityCharacteristic, "hasAssociatedTime", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->negative), entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	this->setEntityCharacteristicIterationint(&(entity->conditionType), entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	this->setEntityCharacteristicIterationbool(&(entity->hasQuantity), entityCharacteristic, "hasQuantity", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->quantityNumber), entityCharacteristic, "quantityNumber", &foundMatch);
	this->setEntityCharacteristicIterationstring(&(entity->quantityNumberString), entityCharacteristic, "quantityNumberString", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->quantityModifier), entityCharacteristic, "quantityModifier", &foundMatch);
	this->setEntityCharacteristicIterationstring(&(entity->quantityModifierString), entityCharacteristic, "quantityModifierString", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasQuantityMultiplier), entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasMeasure), entityCharacteristic, "hasMeasure", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->measureType), entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	this->setEntityCharacteristicIterationint(&(entity->grammaticalNumber), entityCharacteristic, "grammaticalNumber", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->grammaticalWordTypeTemp), entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex]), entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->grammaticalTenseTemp), entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->grammaticalDefiniteTemp), entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->grammaticalIndefinitePluralTemp), entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->grammaticalProperNounTemp), entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->grammaticalGenderTemp), entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->grammaticalPronounTemp), entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	this->setEntityCharacteristicIterationint(&(entity->grammaticalIndexOfDeterminerTemp), entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_STANFORD_CORENLP
	this->setEntityCharacteristicIterationstring(&(entity->stanfordPOStemp), entityCharacteristic, "stanfordPOStemp", &foundMatch);
	this->setEntityCharacteristicIterationstring(&(entity->NormalizedNERtemp), entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	this->setEntityCharacteristicIterationstring(&(entity->TimexTemp), entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	this->setEntityCharacteristicIterationint(&(entity->NERTemp), entityCharacteristic, "NERTemp", &foundMatch);


	/*GIA Translator Temporary Variables*/
	this->setEntityCharacteristicIterationbool(&(entity->isSubjectTemp), entityCharacteristic, "isSubjectTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isObjectTemp), entityCharacteristic, "isObjectTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasSubstanceTemp), entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isActionTemp), entityCharacteristic, "isActionTemp", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->entityIndexTemp), entityCharacteristic, "entityIndexTemp", &foundMatch);
	this->setEntityCharacteristicIterationint(&(entity->sentenceIndexTemp), entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isToBeComplimentOfActionTemp), entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	this->setEntityCharacteristicIterationbool(&(entity->disableParsingAsPrepositionRelationTemp), entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	this->setEntityCharacteristicIterationbool(&(entity->entityAlreadyDeclaredInThisContext), entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstanceTemp), entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	this->setEntityCharacteristicIterationbool(&(entity->isName), entityCharacteristic, "isName", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isNameQuery), entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_NUMBER_OF
	this->setEntityCharacteristicIterationbool(&(entity->isNumberOf), entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	this->setEntityCharacteristicIterationbool(&(entity->wasReference), entityCharacteristic, "wasReference", &foundMatch);
	#endif
	this->setEntityCharacteristicIterationbool(&(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp), entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->mustSetIsConceptBasedOnApposRelation), entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	this->setEntityCharacteristicIterationbool(&(entity->isPronounReference), entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	this->setEntityCharacteristicIterationbool(&(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation), entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	this->setEntityCharacteristicIterationbool(&(entity->convertToSubClass), entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	this->setEntityCharacteristicIterationbool(&(entity->addSubClass), entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_EXPLETIVES
	this->setEntityCharacteristicIterationbool(&(entity->isExpletive), entityCharacteristic, "isExpletive", &foundMatch);
	#endif	


	if(!foundMatch)
	{
		cout << "setEntityCharacteristic{} error: entityCharacteristic not found:" << entityCharacteristic->name << endl;
	}
	return foundMatch;
}

void GIAentityNodeClassClass::setEntityCharacteristicIterationbool(bool* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch)
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
			cout << "testEntityCharacteristicIterationbool{} error: illegal entityCharacteristicTestValue for " << iterationVariable << ": " << entityCharacteristicSet->value << endl;
		}
		#ifdef GIA_DEBUG
		//cout << "setEntityCharacteristicIterationbool{}: " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;
		#endif

		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void GIAentityNodeClassClass::setEntityCharacteristicIterationint(int* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		int entityCharacteristicSetValue = SHAREDvars.convertStringToInt(entityCharacteristicSet->value);
		#ifdef GIA_DEBUG
		//cout << "setEntityCharacteristicIterationint{}: " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;
		#endif
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void GIAentityNodeClassClass::setEntityCharacteristicIterationstring(string* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		string entityCharacteristicSetValue = entityCharacteristicSet->value;
		#ifdef GIA_DEBUG
		//cout << "setEntityCharacteristicIterationstring{}: " << entityCharacteristicSet->name << " = " << entityCharacteristicSetValue << endl;
		#endif
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}

bool GIAentityNodeClassClass::getEntityCharacteristic(const GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;
	#ifdef GIA_DEBUG
	//cout << "testEntityCharacteristic{}:" << entityCharacteristic->name << ": " << entityCharacteristic->value << endl;
	#endif

	/*GIA Entity Type*/
	this->getEntityCharacteristicIterationint(entity->entityType, entityCharacteristic, "entityType", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->negative, entityCharacteristic, "negative", &foundMatch);

	/*GIA Connections*/
	this->getEntityCharacteristicIterationint(entity->conditionType, entityCharacteristic, "conditionType", &foundMatch);

	/*GIA Special Variables (Quantities/Measures)*/
	this->getEntityCharacteristicIterationbool(entity->hasQuantity, entityCharacteristic, "hasQuantity", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->quantityNumber, entityCharacteristic, "quantityNumber", &foundMatch);
	this->getEntityCharacteristicIterationstring(entity->quantityNumberString, entityCharacteristic, "quantityNumberString", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->quantityModifier, entityCharacteristic, "quantityModifier", &foundMatch);
	this->getEntityCharacteristicIterationstring(entity->quantityModifierString, entityCharacteristic, "quantityModifierString", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasQuantityMultiplier, entityCharacteristic, "hasQuantityMultiplier", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasMeasure, entityCharacteristic, "hasMeasure", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->measureType, entityCharacteristic, "measureType", &foundMatch);

	/*GIA Translator Temporary Variables - Grammar*/
	this->getEntityCharacteristicIterationint(entity->grammaticalNumber, entityCharacteristic, "grammaticalNumber", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->grammaticalWordTypeTemp, entityCharacteristic, "grammaticalWordTypeTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->grammaticalTenseModifierArrayTemp[entityCharacteristic->arrayIndex], entityCharacteristic, "grammaticalTenseModifierArrayTemp", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->grammaticalTenseTemp, entityCharacteristic, "grammaticalTenseTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->grammaticalDefiniteTemp, entityCharacteristic, "grammaticalDefiniteTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->grammaticalIndefinitePluralTemp, entityCharacteristic, "grammaticalIndefinitePluralTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->grammaticalProperNounTemp, entityCharacteristic, "grammaticalProperNounTemp", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->grammaticalGenderTemp, entityCharacteristic, "grammaticalGenderTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->grammaticalPronounTemp, entityCharacteristic, "grammaticalPronounTemp", &foundMatch);
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	this->getEntityCharacteristicIterationint(entity->grammaticalIndexOfDeterminerTemp, entityCharacteristic, "grammaticalIndexOfDeterminerTemp", &foundMatch);
	#endif
	#ifdef GIA_STANFORD_CORENLP
	this->getEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	this->getEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	this->getEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	this->getEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	this->getEntityCharacteristicIterationbool(entity->isSubjectTemp, entityCharacteristic, "isSubjectTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isObjectTemp, entityCharacteristic, "isObjectTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasSubstanceTemp, entityCharacteristic, "hasSubstanceTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isActionTemp, entityCharacteristic, "isActionTemp", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	this->getEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	this->getEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	this->getEntityCharacteristicIterationbool(entity->entityAlreadyDeclaredInThisContext, entityCharacteristic, "entityAlreadyDeclaredInThisContext", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	this->getEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_NUMBER_OF
	this->getEntityCharacteristicIterationbool(entity->isNumberOf, entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	this->getEntityCharacteristicIterationbool(entity->wasReference, entityCharacteristic, "wasReference", &foundMatch);
	#endif
	this->getEntityCharacteristicIterationbool(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp, entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->mustSetIsConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	this->getEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	this->getEntityCharacteristicIterationbool(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation, entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	this->getEntityCharacteristicIterationbool(entity->convertToSubClass, entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	this->getEntityCharacteristicIterationbool(entity->addSubClass, entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_EXPLETIVES
	this->getEntityCharacteristicIterationbool(entity->isExpletive, entityCharacteristic, "isExpletive", &foundMatch);
	#endif
	
	if(!foundMatch)
	{
		cout << "getEntityCharacteristic{} error: entityCharacteristic not found:" << entityCharacteristic->name << endl;
	}
	return foundMatch;
}
void GIAentityNodeClassClass::getEntityCharacteristicIterationbool(const bool entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch)
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
void GIAentityNodeClassClass::getEntityCharacteristicIterationint(const int entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicGet->name == iterationVariable)
	{
		string entityCharacteristicGetValue = SHAREDvars.convertIntToString(entityVal);
		entityCharacteristicGet->value = entityCharacteristicGetValue;
		*foundMatch = true;
	}
}
void GIAentityNodeClassClass::getEntityCharacteristicIterationstring(const string entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicGet->name == iterationVariable)
	{
		string entityCharacteristicGetValue = entityVal;
		entityCharacteristicGet->value = entityCharacteristicGetValue;
		*foundMatch = true;
	}
}
#endif

#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
bool GIAentityNodeClassClass::isActionSpecialPossessive(const GIAentityNode* actionEntity)
{
	bool result = false;
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION_GENERAL_IMPLEMENTATION
	if(SHAREDvars.textInTextArray(actionEntity->entityName, relationEntitySpecialActionNameForEffectivePropertiesArray, RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_NUMBER_OF_TYPES))
	{
		result == true;
	}
	#else
	if(actionEntity->entityName == RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES)
	{
		result = true;
	}
	#endif
	return result;
}
#endif



/*
void deleteAllEntitiesInNetworkIndexEntityNodeList(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesList);
{
	for(vector<GIAentityNode*>::iterator networkIndexEntityNodesListIter = networkIndexEntityNodesList.begin(); networkIndexEntityNodesListIter != networkIndexEntityNodesList.end(); networkIndexEntityNodesListIter++)
	{
		GIAentityNode* networkIndexEntityNode = sentenceNetworkIndexEntityNodesListTempNotUsedIter->second;

		for(vector<GIAentityNode*>::iterator instanceEntityNodesListIter = networkIndexEntityNode->associatedInstanceNodeList.begin(); instanceEntityNodesListIter != networkIndexEntityNode->associatedInstanceNodeList.end(); )
		{
			GIAentityNode* instanceEntityNode = *instanceEntityNodesListIter;
			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntityNode->entityVectorConnectionsArray[i].begin(); connectionIter != instanceEntityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					delete* connectionIter;
				}
				instanceEntityNode->entityVectorConnectionsArray[i].clear();
			}
			#ifdef GIA_ALIASES
			delete aliasList;
			#endif
			#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
			delete networkIndexEntityLoaded;
			#endif

			instanceEntityNodesListIter = networkIndexEntityNode->associatedInstanceNodeList.erase(instanceEntityNodesListIter);
		}


		GIAentityNode* currentInstanceInNetworkIndex = networkIndexEntityNodeTemp->
		string entityNodeNameTemp = networkIndexEntityNodeTemp->entityName;
		sentenceNetworkIndexEntityNodesListTempNotUsedMap.insert(pair<string, GIAentityNode*>(entityNodeNameTemp, networkIndexEntityNodeTemp));

		#ifdef GIA_ALIASES
		delete aliasList;
		#endif
		#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
		delete networkIndexEntityLoaded;
		#endif

	}
}
*/


#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
string GIAentityNodeClassClass::getParentClassEntityNameFromSubClassEntityName(string subClassEntityName)
{
	string parentEntityName = "";
	int index = subClassEntityName.find(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER);
	if(index == CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		cout << "getParentClassEntityNameFromSubClassEntityName{} error: failed to find GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER" << endl;
	}
	else if(index < subClassEntityName.length()-1)
	{
		parentEntityName = subClassEntityName.substr(index+string(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER).length());
	}
	return parentEntityName;
}

string GIAentityNodeClassClass::getChildClassEntityNameFromSubClassEntityName(const string subClassEntityName)
{
	string childEntityName = "";
	int index = subClassEntityName.find(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER);
	if(index == CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		cout << "getChildClassEntityNameFromSubClassEntityName{} error: failed to find GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER" << endl;
	}
	else
	{
		childEntityName = subClassEntityName.substr(0, index);
	}
	return childEntityName;
}

string GIAentityNodeClassClass::createSubClassEntityName(const string childEntityName, const string parentEntityName)
{
	string subClassEntityName = childEntityName + GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER + parentEntityName;
	return subClassEntityName;
}

bool GIAentityNodeClassClass::detectPredeterminerNonReference(const GIAentityNode* entity)
{
	bool predeterminerDetected = false;
	predeterminerDetected = SHAREDvars.intInIntArray(entity->grammaticalPredeterminerTemp, entityPredeterminerSmallArray, GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES);
	return predeterminerDetected;
}


#endif
