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
 * File Name: GIAposRelTranslatorRulesGroupClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k13a 11-May-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator Rules
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorRulesGroupClass.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_RULES



GIAposRelTranslatorDebug::GIAposRelTranslatorDebug(void)
{
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	activationMemoryGroupArrayIndex = INT_DEFAULT_VALUE;
	activationMemoryGroupArraySize = INT_DEFAULT_VALUE;
	#endif
	firstComponentActive = false;
	secondComponentActive = false;	
}
GIAposRelTranslatorDebug::~GIAposRelTranslatorDebug(void)
{

}

GIAposRelTranslatorParserForwardPropogationSignalData::GIAposRelTranslatorParserForwardPropogationSignalData(void)
{
	semanticRelationReturnEntity = NULL;
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	semanticRelationReturnConnectionDynamic = false;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	semanticRelationReturnEntitySubject = NULL;
	semanticRelationReturnEntityDelimiter = NULL;
	semanticRelationReturnFunctionNameRecord = "";
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	//semanticRelationReturnFunctionEntityArray = NULL
	#endif
}
GIAposRelTranslatorParserForwardPropogationSignalData::~GIAposRelTranslatorParserForwardPropogationSignalData(void)
{

}

#ifdef GIA_POS_REL_TRANSLATOR_SANI
GIAposRelTranslatorSANIForwardPropogationSignalData::GIAposRelTranslatorSANIForwardPropogationSignalData(void)
{
	//group specific variables:
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	firstIndexInSequence = 0;
	#endif
	//createNewConnections = false;
	//maxLayerToCreateNewConnections = 0;
	activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = 0;
	activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordNounVariantType = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordVerbVariantType = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
	foundPreviousActiveWord = false;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	//parserForwardPropogationSignalData = NULL;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	firstPOSval = true;
	#endif
}
GIAposRelTranslatorSANIForwardPropogationSignalData::~GIAposRelTranslatorSANIForwardPropogationSignalData(void)
{

}

GIAposRelTranslatorSANIForwardPropogationWordData::GIAposRelTranslatorSANIForwardPropogationWordData(void)
{
	//word specific variables:
	wordReference = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	#endif
	
	w = INT_DEFAULT_VALUE;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	expectToSeeConnectionWithPreviousWordTrace = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
	//previousWordConnections = NULL;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	foundPreviousActiveWord = false;
	#endif
}
GIAposRelTranslatorSANIForwardPropogationWordData::~GIAposRelTranslatorSANIForwardPropogationWordData(void)
{

}

#endif


GIAposRelTranslatorRulesGroup::GIAposRelTranslatorRulesGroup(void)
{
	//shared variables;
	groupName = "";
	groupTypeName = "";
	groupTypeReferenceSetType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN;
	semanticRelationFunctionName.resize(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationFunctionName[i] = "";
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	semanticRelationFunctionConditionNewName = "";
	#endif
	//components = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	groupWeight = 0;
	#endif	
		
	
	//GIAposRelTranslatorRulesGroup variables;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	previousWordPOStype = "";
	existsPreceedingWordPOStype = "";
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK
	numberOfConsecutiveTimesPreviousGroupType = 0;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantTypeDerived = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantTypeDerived = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;	
	#endif
	#endif
}
GIAposRelTranslatorRulesGroup::~GIAposRelTranslatorRulesGroup(void)
{
}


GIAposRelTranslatorRulesGroupNeuralNetwork::GIAposRelTranslatorRulesGroupNeuralNetwork(void)
{
	//GIAposRelTranslatorRulesGroupNeuralNetwork variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	networkDepth = 0;
	networkLeafSize = 0;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	//parseTreeGroupMemory = NULL;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	marked = false;
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	counted = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
	verified = false;
	#endif
	inputLayerNeuron = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
	firstHiddenLayerNeuron = false;
	#endif
	groupTypeIsString = false;
	//wordDataTemp = NULL;
	wordPOStype = INT_DEFAULT_VALUE;
	groupIndex = INT_DEFAULT_VALUE;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
	groupStrength = 0;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
	topLevelSentenceNeuron = false;
	#endif
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
	nonResetActivationFoundDuringPreprocess = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	newActivationFoundDuringPreprocess = false;
	#endif
	#endif
	//ANNfrontGroupConnectionList = NULL;
	//ANNbackGroupConnectionList = NULL;
	//ANNfrontComponentConnectionList = NULL;
	next = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	initiateANNneuron("");
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	neuronConnectivitySet = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_CENTRE_NEURONS
	neuronDisplayPositionXcentred = INT_DEFAULT_VALUE;
	neuronDisplayPositionYcentred = INT_DEFAULT_VALUE;	
	#endif
	#endif
	GIAtokenLayerName = "";
	GIAtokenLayerClassName = "";
	GIAtokenLayerClassTypeName = "";
	GIAtokenLayerClassTypeInstanceName = "";
	neuronGenerated = false;
	neuronPropagated = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	neuronProcessed = false;
	#endif
	neuronPropagatedSave = false;
	neuronPreviousWordPOStypeTested = false;	//NOT USED
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_BIO_WEAK3
	solidified = false;
	#endif
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	//activationMemoryGroupArray = NULL;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	//currentParseTreeGroupArray = NULL;
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
	currentParseTreeGroupTemp = NULL;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	//#endif	
	//#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT
	neuronActive = false;
	//#endif	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	optionalComponentsWrapperGroup = NULL;
	#endif
	#endif	
	
}
GIAposRelTranslatorRulesGroupNeuralNetwork::~GIAposRelTranslatorRulesGroupNeuralNetwork(void)
{
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
void GIAposRelTranslatorRulesGroupNeuralNetwork::initiateANNneuron(const string groupName)	
{
	neuronReference = new ANNneuron();
	neuronReference->GIAentityName = groupName;
	neuronReference->spatialCoordinatesSet3D = true;	//required for draw
	neuronReference->hasBackLayer = true;	//required for draw
}
#endif
			



GIAposRelTranslatorRulesGroupActivationMemory::GIAposRelTranslatorRulesGroupActivationMemory(void)
{
	//GIAposRelTranslatorRulesGroupActivationMemory variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	numberWordsInSet = 0;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	//forwardPropogationSignalData = NULL;
	#endif
	//forwardPropogationWordData = NULL;
	layer = INT_DEFAULT_VALUE;
	inputGroupString = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	groupOrig = NULL;	//for memoryActivationGroups only
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	pointArrayGenerated = false;
	firstLevelActivationPointAdded = false;
	//activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	#endif
	wordGroupNeuron = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	lastWordIndexActivated = INT_DEFAULT_VALUE;
	#endif
	#endif
	#endif
	#endif
}
GIAposRelTranslatorRulesGroupActivationMemory::~GIAposRelTranslatorRulesGroupActivationMemory(void)
{
}

GIAposRelTranslatorRulesGroupParseTree::GIAposRelTranslatorRulesGroupParseTree(void)
{	
	//GIAposRelTranslatorRulesGroupParseTree variables;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	groupRef = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	//variableStartComponentFound = false;
	missingOrVariableStartComponentFound = false;
	#endif
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	missingStartComponentFound = false;
	#endif
	*/
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_OPTIMISED
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	//activationPathWordFirstParseTreeGroupActivationPointArray = NULL;	
	#endif	
	#endif
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	parseTreeGroupRefReverse = NULL;
	parseTreeGroupRefReverseComponentIndex = INT_DEFAULT_VALUE;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_BIO
	solidified = false;
	#endif
	#endif
	neuronTraced = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	neuronActive = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	parseTreeMinWordIndex = INT_DEFAULT_VALUE;
	parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	parseTreeMaxWeight = 0.0;
	parseTreeMinWeight = 0.0;
	parseTreeTotalWeight = 0.0;
	#endif
	#endif
	#endif
}
GIAposRelTranslatorRulesGroupParseTree::~GIAposRelTranslatorRulesGroupParseTree(void)
{
}


GIAposRelTranslatorRulesGroupType::GIAposRelTranslatorRulesGroupType(void)
{
	groupTypeName = "";
	
	referenceSetType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	groupTypeXindex = INT_DEFAULT_VALUE;
	groupMaxY = 0;
	#endif
}
GIAposRelTranslatorRulesGroupType::~GIAposRelTranslatorRulesGroupType(void)
{
	//groups = NULL;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI

GIAposRelTranslatorSANIForwardPropogationActivationPointData::GIAposRelTranslatorSANIForwardPropogationActivationPointData(void)
{
	generateActivationPointArray = false;
	connectToPreviousActivationGroup = false;
	activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	activationPathWordFirstParseTreeGroupActivationPointArray = NULL;
	layer = 0;
	activationPathW = INT_DEFAULT_VALUE;
}
GIAposRelTranslatorSANIForwardPropogationActivationPointData::~GIAposRelTranslatorSANIForwardPropogationActivationPointData(void)
{

}


GIAposRelTranslatorSANIForwardPropogationSentenceData::GIAposRelTranslatorSANIForwardPropogationSentenceData(void)
{
	parseSentenceReverse = false;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	recordActivatedNeuronWithMaxWordIndexCoverage = false;
	activatedNeuronWithMaxWordIndexCoverage = NULL;
	activatedNeuronWithMaxWordIndexCoveragePartial = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH
	variableFirstComponentMaxDepth = 0;
	#endif
	#endif
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	//listOfHighLevelNeuronsCompleteHistory = NULL;
	#endif	
	#endif
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
	activatedNeuronWithMaxWordIndexCoverageMissingStartComponent = false;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	findingCandidateComponent2 = false;
	candidateComponent2ToFind = NULL;
	foundCandidateComponent2 = false;
	candidateComponent2sourceParseTreeGroup = NULL;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	firstLayer = NULL;
	#else
	firstLayer = NULL;
	#endif
	#endif
	
	//sentence specific variables:
	toplevelGroupActivationFound = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	parserEnabled = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE	
	performance = 0;
	#endif
	sentenceContents = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	isQuery = false;
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	parseIsolatedSubreferenceSets = false;
	//#endif
	
	finishedPassingSentenceWords = false;
	
	GIAposRelTranslatorRulesGroupTypes = NULL;
	
	//forwardPropogationWordDataArray = NULL;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#else
	//activationPathWordFirstActivationMemoryGroupArray = NULL;
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#endif
	
	forwardPropogationActivationPointData = NULL;
}
GIAposRelTranslatorSANIForwardPropogationSentenceData::~GIAposRelTranslatorSANIForwardPropogationSentenceData(void)
{

}

#endif



void GIAposRelTranslatorRulesGroupClass::copyParseGroupInfo(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup)	
{
	//NOT ALLOWED as will overwrite previous tree linking to currentParseTreeGroup:  *currentParseTreeGroup = currentParseTreeGroupTemp;
	
	currentParseTreeGroup->semanticRelationFunctionName = currentParseTreeGroupTemp->semanticRelationFunctionName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	currentParseTreeGroup->semanticRelationFunctionConditionNewName = currentParseTreeGroupTemp->semanticRelationFunctionConditionNewName;
	#endif
	currentParseTreeGroup->groupName = currentParseTreeGroupTemp->groupName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	currentParseTreeGroup->previousWordPOStype = currentParseTreeGroupTemp->previousWordPOStype;
	currentParseTreeGroup->existsPreceedingWordPOStype = currentParseTreeGroupTemp->existsPreceedingWordPOStype;
	#endif
	currentParseTreeGroup->groupTypeName = currentParseTreeGroupTemp->groupTypeName;
	
	currentParseTreeGroup->components = currentParseTreeGroupTemp->components;	//CHECKTHIS
}



bool GIAposRelTranslatorRulesGroupClass::isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets)	
{
	bool topLevelGroup = false;
	
	if(groupTypeReferenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
		if(isQuery)
		{
			if(groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				topLevelGroup = true;
				//cout << "isQuery && 'queries' group found" << endl;
			}
		}
		else
		{
		#endif

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			if(parseIsolatedSubreferenceSets)
			{
				if(groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
				{
					topLevelGroup = true;
				}
			}
			else
			{
			#endif
			
				if(groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
				{
					topLevelGroup = true;
				}
							
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			}
			#endif

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
		}
		#endif
	}
	
	return topLevelGroup;
}


#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
bool GIAposRelTranslatorRulesGroupClass::determineIsQuery(vector<GIApreprocessorPlainTextWord*>* sentenceContents)
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
