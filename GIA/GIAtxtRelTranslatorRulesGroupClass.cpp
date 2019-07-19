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
 * File Name: GIAtxtRelTranslatorRulesGroupClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i2b 27-May-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorRulesGroupClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES



GIAtxtRelTranslatorDebug::GIAtxtRelTranslatorDebug(void)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	activationMemoryGroupArrayIndex = INT_DEFAULT_VALUE;
	activationMemoryGroupArraySize = INT_DEFAULT_VALUE;
	#endif
	firstComponentActive = false;
	secondComponentActive = false;	
}
GIAtxtRelTranslatorDebug::~GIAtxtRelTranslatorDebug(void)
{

}

GIAtxtRelTranslatorParserForwardPropogationSignalData::GIAtxtRelTranslatorParserForwardPropogationSignalData(void)
{
	semanticRelationReturnEntity = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	semanticRelationReturnEntitySubject = NULL;
	semanticRelationReturnEntityDelimiter = NULL;
	semanticRelationReturnFunctionNameRecord = "";
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	//semanticRelationReturnFunctionEntityArray = NULL
	#endif
}
GIAtxtRelTranslatorParserForwardPropogationSignalData::~GIAtxtRelTranslatorParserForwardPropogationSignalData(void)
{

}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData::GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void)
{
	//group specific variables:
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordNounVariantType = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordVerbVariantType = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	foundPreviousActiveWord = false;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	//parserForwardPropogationSignalData = NULL;
	#endif
}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData(void)
{

}

GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData::GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void)
{
	//word specific variables:
	wordReference = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	#endif
	
	w = INT_DEFAULT_VALUE;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	expectToSeeConnectionWithPreviousWordTrace = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
	//previousWordConnections = NULL;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	foundPreviousActiveWord = false;
	#endif
}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void)
{

}
#endif


GIAtxtRelTranslatorRulesGroup::GIAtxtRelTranslatorRulesGroup(void)
{
	//shared variables;
	groupName = "";
	groupTypeName = "";
	groupTypeReferenceSetType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN;
	semanticRelationFunctionName.resize(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationFunctionName[i] = "";
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	semanticRelationFunctionConditionNewName = "";
	#endif
	//components = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	groupWeight = 0;
	#endif	
		
	
	//GIAtxtRelTranslatorRulesGroup variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	previousWordPOStype = "";
	existsPreceedingWordPOStype = "";
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK
	numberOfConsecutiveTimesPreviousGroupType = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantTypeDerived = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantTypeDerived = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;	
	#endif
	#endif
}
GIAtxtRelTranslatorRulesGroup::~GIAtxtRelTranslatorRulesGroup(void)
{
}


GIAtxtRelTranslatorRulesGroupNeuralNetwork::GIAtxtRelTranslatorRulesGroupNeuralNetwork(void)
{
	//GIAtxtRelTranslatorRulesGroupNeuralNetwork variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
	nonResetActivationFoundDuringPreprocess = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	newActivationFoundDuringPreprocess = false;
	#endif
	#endif
	//ANNfrontGroupConnectionList = NULL;
	//ANNbackGroupConnectionList = NULL;
	//ANNfrontComponentConnectionList = NULL;
	next = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	initiateANNneuron("");
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	neuronConnectivitySet = false;
	#endif
	GIAtokenLayerName = "";
	GIAtokenLayerClassName = "";
	GIAtokenLayerClassTypeName = "";
	GIAtokenLayerClassTypeInstanceName = "";
	neuronGenerated = false;
	neuronPropagated = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	neuronProcessed = false;
	#endif
	neuronPropagatedSave = false;
	neuronPreviousWordPOStypeTested = false;	//NOT USED
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK3
	solidified = false;
	#endif
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	//activationMemoryGroupArray = NULL;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	//currentParseTreeGroupArray = NULL;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
	currentParseTreeGroupTemp = NULL;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	#endif	
	//#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT
	neuronActive = false;
	//#endif	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REMOVE_LAST_OPTIONAL_COMPONENTS
	optionalComponentsWrapperGroup = NULL;
	#endif
	#endif	
	
}
GIAtxtRelTranslatorRulesGroupNeuralNetwork::~GIAtxtRelTranslatorRulesGroupNeuralNetwork(void)
{
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
void GIAtxtRelTranslatorRulesGroupNeuralNetwork::initiateANNneuron(const string groupName)	
{
	neuronReference = new ANNneuron();
	neuronReference->GIAentityName = groupName;
	neuronReference->spatialCoordinatesSet3D = true;	//required for draw
	neuronReference->hasBackLayer = true;	//required for draw
}
#endif
			



GIAtxtRelTranslatorRulesGroupActivationMemory::GIAtxtRelTranslatorRulesGroupActivationMemory(void)
{
	//GIAtxtRelTranslatorRulesGroupActivationMemory variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
	numberWordsInSet = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	//forwardPropogationSignalData = NULL;
	#endif
	//forwardPropogationWordData = NULL;
	layer = INT_DEFAULT_VALUE;
	inputGroupString = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	groupOrig = NULL;	//for memoryActivationGroups only
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	pointArrayGenerated = false;
	firstLevelActivationPointAdded = false;
	//activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	#endif
	wordGroupNeuron = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	lastWordIndexActivated = INT_DEFAULT_VALUE;
	#endif
	#endif
	#endif
	#endif
}
GIAtxtRelTranslatorRulesGroupActivationMemory::~GIAtxtRelTranslatorRulesGroupActivationMemory(void)
{
}

GIAtxtRelTranslatorRulesGroupParseTree::GIAtxtRelTranslatorRulesGroupParseTree(void)
{	
	//GIAtxtRelTranslatorRulesGroupParseTree variables;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	//activationPathWordFirstParseTreeGroupActivationPointArray = NULL;	
	#endif	
	#endif
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	parseTreeGroupRefReverse = NULL;
	parseTreeGroupRefReverseComponentIndex = INT_DEFAULT_VALUE;
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO
	solidified = false;
	#endif
	#endif
	neuronTraced = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	neuronActive = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	parseTreeMinWordIndex = INT_DEFAULT_VALUE;
	parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	parseTreeMaxWeight = 0.0;
	parseTreeMinWeight = 0.0;
	parseTreeTotalWeight = 0.0;
	#endif
	#endif
	#endif
}
GIAtxtRelTranslatorRulesGroupParseTree::~GIAtxtRelTranslatorRulesGroupParseTree(void)
{
}


GIAtxtRelTranslatorRulesGroupType::GIAtxtRelTranslatorRulesGroupType(void)
{
	groupTypeName = "";
	
	referenceSetType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	groupTypeXindex = INT_DEFAULT_VALUE;
	groupMaxY = 0;
	#endif
}
GIAtxtRelTranslatorRulesGroupType::~GIAtxtRelTranslatorRulesGroupType(void)
{
	//groups = NULL;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK

GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData::GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void)
{
	generateActivationPointArray = false;
	connectToPreviousActivationGroup = false;
	activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	activationPathWordFirstParseTreeGroupActivationPointArray = NULL;
	layer = 0;
	activationPathW = INT_DEFAULT_VALUE;
}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData(void)
{

}


GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData::GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void)
{
	//sentence specific variables:
	toplevelGroupActivationFound = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	parserEnabled = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE	
	performance = 0;
	#endif
	sentenceContents = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	isQuery = false;
	#endif
	//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	parseIsolatedSubreferenceSets = false;
	//#endif
	
	finishedPassingSentenceWords = false;
	
	GIAtxtRelTranslatorRulesGroupTypes = NULL;
	
	//forwardPropogationWordDataArray = NULL;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#else
	//activationPathWordFirstActivationMemoryGroupArray = NULL;
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#endif
	
	forwardPropogationActivationPointData = NULL;
}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void)
{

}

#endif



void GIAtxtRelTranslatorRulesGroupClass::copyParseGroupInfo(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup)	
{
	//NOT ALLOWED as will overwrite previous tree linking to currentParseTreeGroup:  *currentParseTreeGroup = currentParseTreeGroupTemp;
	
	currentParseTreeGroup->semanticRelationFunctionName = currentParseTreeGroupTemp->semanticRelationFunctionName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	currentParseTreeGroup->semanticRelationFunctionConditionNewName = currentParseTreeGroupTemp->semanticRelationFunctionConditionNewName;
	#endif
	currentParseTreeGroup->groupName = currentParseTreeGroupTemp->groupName;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	currentParseTreeGroup->previousWordPOStype = currentParseTreeGroupTemp->previousWordPOStype;
	currentParseTreeGroup->existsPreceedingWordPOStype = currentParseTreeGroupTemp->existsPreceedingWordPOStype;
	#endif
	currentParseTreeGroup->groupTypeName = currentParseTreeGroupTemp->groupTypeName;
	
	currentParseTreeGroup->components = currentParseTreeGroupTemp->components;	//CHECKTHIS
}



bool GIAtxtRelTranslatorRulesGroupClass::isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets)	
{
	bool topLevelGroup = false;
	
	if(groupTypeReferenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
		if(isQuery)
		{
			if(groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				topLevelGroup = true;
				//cout << "isQuery && 'queries' group found" << endl;
			}
		}
		else
		{
		#endif

			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			if(parseIsolatedSubreferenceSets)
			{
				if(groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
				{
					topLevelGroup = true;
				}
			}
			else
			{
			#endif
			
				if(groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
				{
					topLevelGroup = true;
				}
							
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			}
			#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
		}
		#endif
	}
	
	return topLevelGroup;
}


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
bool GIAtxtRelTranslatorRulesGroupClass::determineIsQuery(vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool isQuery = false;
	
	if((*sentenceContents)[sentenceContents->size()-1]->tagName == GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE_QUESTIONMARK)
	{
		isQuery = true;
	}
	
	return isQuery;
}
#endif



	
#endif
