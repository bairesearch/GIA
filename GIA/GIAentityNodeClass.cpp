/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAentityNodeClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11g 01-March-2019
 * /
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


GIAentityNode::GIAentityNode(void)
{
	initialiseEntity();
}
GIAentityNode::GIAentityNode(string newEntityName)
{
	initialiseEntity();
	entityName = newEntityName;
}
void GIAentityNode::initialiseEntity()
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
	entityType = GIA_ENTITY_TYPE_UNDEFINED;
	isActionConcept = false;
	hasAssociatedInstance = false;
	hasAssociatedTime = false;
	negative = false;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	isArtificialAuxiliary = false;
	#endif

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
	#ifdef GIA_PREDETERMINERS
	grammaticalPredeterminerTemp = GRAMMATICAL_PREDETERMINER_UNDEFINED;
	#endif
	stanfordPOStemp = "";
	#ifdef GIA_STANFORD_CORENLP
	/*
	CharacterOffsetBeginTemp = INT_DEFAULT_VALUE;
	CharacterOffsetEndTemp = INT_DEFAULT_VALUE;
	*/
	NERTemp = FEATURE_NER_UNDEFINED;
	NormalizedNERtemp = "";
	TimexTemp = "";
	#endif

	/*GIA Translator Temporary Variables*/
	entityIndexTemp = GIA_ENTITY_INDEX_UNDEFINED;	//was 0 before 11 October 2012
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED;	//was 0 before 11 October 2012
	isToBeComplimentOfActionTemp = false;
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	disableParsingAsPrepositionRelationTemp = false;
	#endif
	hasAssociatedInstanceTemp = false;
	#ifdef GIA_ALIASES
	isName = false;
	isNameQuery = false;
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
	isNumberOf = false;
	#endif
	
	/*GIA Connections*/
	//to minimise query/referencing code
	actionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION]);
	actionReverseNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE]);
	conditionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION]);
	conditionReverseNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]);
	propertyNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY]);
	propertyReverseNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE]);
	definitionNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION]);
	definitionReverseNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE]);
	relationshipSubjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT]);
	relationshipObjectEntity = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT]);	
	instanceNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE]);
	instanceReverseNodeList = &(entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE]);
	#ifdef GIA_DATABASE
	GIAentityNodeClassClass().DBsetEntityConnectionsReferenceListsLoaded(this, true);	//for now, assume that a new entity will be configured with its connections loaded into RAM
	#endif
	conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	timeConditionNode = NULL;


	/*Query Variables*/
	isQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	isAnswerToQuery = false;
	testedForQueryComparison = false;
	testedForQueryComparisonTemp = false;
	testedForQueryComparisonTemp2 = false;
	queryAnswerContext = false;
	queryEntityTraced = false;
	entityCorrespondingBestMatch = NULL;


	/*GIA Miscellaneous Internal Variables*/
	disabled = false;
	permanentNetworkIndex = false;
		//CXL:
	CXLdummyNode = false;
		//referencing:
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED;
	minimumEntityIndexOfReferenceSet = GIA_REFERENCE_SET_ID_UNDEFINED;
	#ifdef GIA_ADVANCED_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	wasReferenceTemp = false;
	#endif
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	wasReference = false;
	#endif
	#endif
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;		//#ifdef GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	mustSetIsConceptBasedOnApposRelation = false;
	isPronounReference = false;
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
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
	#ifdef GIA_TRANSLATOR_EXPLETIVES
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

	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	isAlias = false;
	#endif
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

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	conditionType2 = "";
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	conditionTwoWay = false;
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	inverseConditionTwoWay = false;
	#endif
	#endif
	#endif
	
	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	shortcutToNonspecificConceptEntity = NULL;
	//shortcutsToSpecificConceptEntities = NULL;
	#ifdef GIA_NEURAL_NETWORK
	entityShortcutToConceptNeuron = NULL;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR
	isLogicReferenceEntity = false;
	logicReferenceClass = INT_DEFAULT_VALUE;	//GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_UNDEFINED;
	logicReferenceClassType = "";	//GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK
	parsedForANNgeneration = false;
	/*
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	conceptIndexType = GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_UNKNOWN;
	conceptIndex = INT_DEFAULT_VALUE;
	#endif
	*/
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES
	semanticRelationReturnFunctionName = ""; 
	semanticRelationReturnFunctionNameIndexType = INT_DEFAULT_VALUE;
	//semanticRelationReturnFunctionNameSameReferenceSet = false;
	semanticRelationPreprocessorEntityIndex = GIA_ENTITY_INDEX_UNDEFINED;
	#endif
	semanticRelationWordPOStypeInferred = INT_DEFAULT_VALUE;	//GIA_SHARED_POS_TYPE_UNDEFINED;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	semanticRelationWordDeterminer = "";
	semanticRelationWordPredeterminer = "";
	semanticRelationEntityIsReferenced = false;
	#endif
}

GIAentityNode::~GIAentityNode(void)
{
	//delete all connections
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityVectorConnectionsArray[i].begin(); connectionIter != entityVectorConnectionsArray[i].end(); connectionIter++)
		{
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
	actionReverseNodeList->clear();

	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	actionReverseNodeList->clear();
	*/
}



int GIAentityNodeClassClass::calculateQuantityNumberInt(const string quantityNumberString)
{
	/*
	if(quantityNumberString == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		quantityNumberInt = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT;
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
	entityNode->aliasList = explode(aliasesString, GIA_ALIASES_CONVERT_TO_STRING_DELIMITER);
}

void GIAentityNodeClassClass::convertAliasesToAliasesString(GIAentityNode* entityNode, string* aliasesString)
{
	*aliasesString = "";
	for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
	{
		*aliasesString = *aliasesString +* aliasIter + GIA_ALIASES_CONVERT_TO_STRING_DELIMITER;
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
		if(testEntityCharacteristic(entity, entityCharacteristic))
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
		setEntityCharacteristic(entity, entityCharacteristic);
	}
}
bool GIAentityNodeClassClass::testEntityCharacteristic(const GIAentityNode* entity, const GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;
	bool illegalVariable = false;


	/*GIA Entity Type*/
	testEntityCharacteristicIterationint(entity->entityType, entityCharacteristic, "entityType", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	testEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
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
	testEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	#ifdef GIA_STANFORD_CORENLP
	testEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	testEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	testEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	testEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	testEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	testEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	testEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	testEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
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
	testEntityCharacteristicIterationbool(entity->mustSetIsConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	testEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	testEntityCharacteristicIterationbool(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation, entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	testEntityCharacteristicIterationbool(entity->convertToSubClass, entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	testEntityCharacteristicIterationbool(entity->addSubClass, entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	testEntityCharacteristicIterationbool(entity->isExpletive, entityCharacteristic, "isExpletive", &foundMatch);	
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
			*foundMatch = true;
		}
	}
}


bool GIAentityNodeClassClass::setEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;


	/*GIA Entity Type*/
	setEntityCharacteristicIterationint(&(entity->entityType), entityCharacteristic, "entityType", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isActionConcept), entityCharacteristic, "isActionConcept", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstance), entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedTime), entityCharacteristic, "hasAssociatedTime", &foundMatch);
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
	setEntityCharacteristicIterationstring(&(entity->stanfordPOStemp), entityCharacteristic, "stanfordPOStemp", &foundMatch);
	#ifdef GIA_STANFORD_CORENLP
	setEntityCharacteristicIterationstring(&(entity->NormalizedNERtemp), entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	setEntityCharacteristicIterationstring(&(entity->TimexTemp), entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	setEntityCharacteristicIterationint(&(entity->NERTemp), entityCharacteristic, "NERTemp", &foundMatch);


	/*GIA Translator Temporary Variables*/
	setEntityCharacteristicIterationint(&(entity->entityIndexTemp), entityCharacteristic, "entityIndexTemp", &foundMatch);
	setEntityCharacteristicIterationint(&(entity->sentenceIndexTemp), entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isToBeComplimentOfActionTemp), entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	setEntityCharacteristicIterationbool(&(entity->disableParsingAsPrepositionRelationTemp), entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	setEntityCharacteristicIterationbool(&(entity->hasAssociatedInstanceTemp), entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	setEntityCharacteristicIterationbool(&(entity->isName), entityCharacteristic, "isName", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isNameQuery), entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
	setEntityCharacteristicIterationbool(&(entity->isNumberOf), entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	setEntityCharacteristicIterationbool(&(entity->wasReference), entityCharacteristic, "wasReference", &foundMatch);
	#endif
	setEntityCharacteristicIterationbool(&(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp), entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->mustSetIsConceptBasedOnApposRelation), entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	setEntityCharacteristicIterationbool(&(entity->isPronounReference), entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	setEntityCharacteristicIterationbool(&(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation), entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	setEntityCharacteristicIterationbool(&(entity->convertToSubClass), entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	setEntityCharacteristicIterationbool(&(entity->addSubClass), entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	setEntityCharacteristicIterationbool(&(entity->isExpletive), entityCharacteristic, "isExpletive", &foundMatch);
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

		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void GIAentityNodeClassClass::setEntityCharacteristicIterationint(int* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		int entityCharacteristicSetValue = SHAREDvars.convertStringToInt(entityCharacteristicSet->value);
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}
void GIAentityNodeClassClass::setEntityCharacteristicIterationstring(string* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch)
{
	if(entityCharacteristicSet->name == iterationVariable)
	{
		string entityCharacteristicSetValue = entityCharacteristicSet->value;
		*entityVal = entityCharacteristicSetValue;
		*foundMatch = true;
	}
}

bool GIAentityNodeClassClass::getEntityCharacteristic(const GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic)
{
	bool foundMatch = false;

	/*GIA Entity Type*/
	getEntityCharacteristicIterationint(entity->entityType, entityCharacteristic, "entityType", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isActionConcept, entityCharacteristic, "isActionConcept", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstance, entityCharacteristic, "hasAssociatedInstance", &foundMatch);
	getEntityCharacteristicIterationbool(entity->hasAssociatedTime, entityCharacteristic, "hasAssociatedTime", &foundMatch);
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
	getEntityCharacteristicIterationstring(entity->stanfordPOStemp, entityCharacteristic, "stanfordPOStemp", &foundMatch);
	#ifdef GIA_STANFORD_CORENLP
	getEntityCharacteristicIterationstring(entity->NormalizedNERtemp, entityCharacteristic, "NormalizedNERtemp", &foundMatch);
	getEntityCharacteristicIterationstring(entity->TimexTemp, entityCharacteristic, "TimexTemp", &foundMatch);
	#endif
	getEntityCharacteristicIterationint(entity->NERTemp, entityCharacteristic, "NERTemp", &foundMatch);

	/*GIA Translator Temporary Variables*/
	getEntityCharacteristicIterationint(entity->entityIndexTemp, entityCharacteristic, "entityIndexTemp", &foundMatch);
	getEntityCharacteristicIterationint(entity->sentenceIndexTemp, entityCharacteristic, "sentenceIndexTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isToBeComplimentOfActionTemp, entityCharacteristic, "isToBeComplimentOfActionTemp", &foundMatch);
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	getEntityCharacteristicIterationbool(entity->disableParsingAsPrepositionRelationTemp, entityCharacteristic, "disableParsingAsPrepositionRelationTemp", &foundMatch);
	#endif
	getEntityCharacteristicIterationbool(entity->hasAssociatedInstanceTemp, entityCharacteristic, "hasAssociatedInstanceTemp", &foundMatch);
	#ifdef GIA_ALIASES
	getEntityCharacteristicIterationbool(entity->isName, entityCharacteristic, "isName", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isNameQuery, entityCharacteristic, "isNameQuery", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
	getEntityCharacteristicIterationbool(entity->isNumberOf, entityCharacteristic, "isNumberOf", &foundMatch);
	#endif
	
	/*GIA Miscellaneous Internal Variables*/
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	getEntityCharacteristicIterationbool(entity->wasReference, entityCharacteristic, "wasReference", &foundMatch);
	#endif
	getEntityCharacteristicIterationbool(entity->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp, entityCharacteristic, "alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", &foundMatch);
	getEntityCharacteristicIterationbool(entity->mustSetIsConceptBasedOnApposRelation, entityCharacteristic, "mustSetIsConceptBasedOnApposRelation", &foundMatch);
	getEntityCharacteristicIterationbool(entity->isPronounReference, entityCharacteristic, "isPronounReference", &foundMatch);
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	getEntityCharacteristicIterationbool(entity->mustNotSetIsConceptBasedOnPrenomonalModifierRelation, entityCharacteristic, "mustNotSetIsConceptBasedOnPrenomonalModifierRelation", &foundMatch);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	getEntityCharacteristicIterationbool(entity->convertToSubClass, entityCharacteristic, "convertToSubClass", &foundMatch);
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	getEntityCharacteristicIterationbool(entity->addSubClass, entityCharacteristic, "addSubClass", &foundMatch);
	#endif
	#endif
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	getEntityCharacteristicIterationbool(entity->isExpletive, entityCharacteristic, "isExpletive", &foundMatch);
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



/*
void deleteAllEntitiesInNetworkIndexEntityNodeList(unordered_map<string, GIAentityNode*>* networkIndexEntityNodesList);
{
	for(vector<GIAentityNode*>::iterator networkIndexEntityNodesListIter = networkIndexEntityNodesList.begin(); networkIndexEntityNodesListIter != networkIndexEntityNodesList.end(); networkIndexEntityNodesListIter++)
	{
		GIAentityNode* networkIndexEntityNode = sentenceNetworkIndexEntityNodesListTempNotUsedIter->second;

		for(vector<GIAentityNode*>::iterator instanceEntityNodesListIter = networkIndexEntityNode->instanceNodeList.begin(); instanceEntityNodesListIter != networkIndexEntityNode->instanceNodeList.end(); )
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

			instanceEntityNodesListIter = networkIndexEntityNode->instanceNodeList.erase(instanceEntityNodesListIter);
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

bool GIAentityNodeClassClass::entityIsRelationship(const GIAentityNode* entity)
{
	bool relationshipEntity = false;
	if(entityTypesIsRelationshipArray[entity->entityType])
	{
		relationshipEntity = true;
	}
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[entity->entityType])
	{
		cerr << "!GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: GIAentityNodeClassClass::entityIsRelationship error{}: entityTypesIsPropertyOrDefinitionRelationshipArray[entity->entityType]" << endl;
		exit(EXIT_ERROR);
	}
	#endif
	return relationshipEntity;
}

int GIAentityNodeClassClass::getRelationshipEntityRelativeTypeIndex(const GIAentityNode* entity)
{
	int relationshipEntityRelativeTypeIndex = entity->entityType - GIA_RELATIONSHIP_ENTITY_OFFSET_TO_FIRST_RELATIVE_TYPE_INDEX;
	return relationshipEntityRelativeTypeIndex;
}



