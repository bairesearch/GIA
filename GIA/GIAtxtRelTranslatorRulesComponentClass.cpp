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
 * File Name: GIAtxtRelTranslatorRulesComponentClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2b 10-August-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorRulesComponentClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES


GIAtxtRelTranslatorRulesComponent::GIAtxtRelTranslatorRulesComponent(void)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR
	//ANNbackGroupConnectionList = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	componentStrength = 0;
	#endif
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	referenceSetTypeHybrid = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN;
	#endif
	
	componentType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN;
	componentIndex = INT_DEFAULT_VALUE;
	
	groupTypeRefName = "";
	groupRefName = "";
	
	stringType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN;
	word = "";
	wordPOStype = "";
	tokenLayer = "";
	tokenClass = "";
	tokenType = "";
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
	missing = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	stringTypeExplicitAddToExplicitWordTempPOS = false;
	#endif
		
	semanticRelationIndexType.resize(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationIndexType[i] = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	}
	semanticRelationReturnEntity = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	semanticRelationReturnConnectionDynamic = false;
	#endif
	
	semanticRelationReturnFunctionName = "";
	semanticRelationReturnFunctionNameIndexType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	
	semanticRelationRecord = false;
	semanticRelationConnect = false;
	
	optional = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
	queryComparisonVariable = false;
	isAuxiliaryQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_ALIASES
	isNameQuery = false;
	isName = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
	isNumberOf = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantType = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantType = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	#endif		
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
	isExpletive = false;
	#endif	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	neuronComponentConnectionActive = false;
	neuronComponentConnectionActiveWordRecord = NULL;
	#endif
	candidateStringMatch = NULL;
	groupTypeRef = NULL;
	groupRef = NULL;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_BIO
	parserForwardPropogationSignalData = NULL;
	#endif
	
}
GIAtxtRelTranslatorRulesComponent::~GIAtxtRelTranslatorRulesComponent(void)
{
}


GIAtxtRelTranslatorRulesComponentNeuralNetwork::GIAtxtRelTranslatorRulesComponentNeuralNetwork(void)
{
	//subComponents = NULL;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	ownerGroup = NULL;	
	isSubcomponent = false;
	ownerComponent = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	//ANNbackNeuronConnectionList = NULL;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	//optimumPathwayWordList = NULL;
	#endif
	#endif
}
GIAtxtRelTranslatorRulesComponentNeuralNetwork::~GIAtxtRelTranslatorRulesComponentNeuralNetwork(void)
{
}


GIAtxtRelTranslatorRulesComponentParseTree::GIAtxtRelTranslatorRulesComponentParseTree(void)
{
	//parse tree variables:
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_GENERATE_PARSE_TREE
	parseTreeGroupRef = NULL;
	#endif
	componentRef = NULL;
	wordPOStypeInferred = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO2
	groupFrontComponentConnectionListIndex = INT_DEFAULT_VALUE;
	numberOfWordsCurrentlyParsed = INT_DEFAULT_VALUE;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	int wordIndex;
	#endif
}
GIAtxtRelTranslatorRulesComponentParseTree::~GIAtxtRelTranslatorRulesComponentParseTree(void)
{
}



bool GIAtxtRelTranslatorRulesComponentClass::componentHasSubcomponents(GIAtxtRelTranslatorRulesComponent* component)
{
	bool hasSubcomponents = false;
	if((component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	{
		hasSubcomponents = true;
	}
	return hasSubcomponents;
}



#endif



