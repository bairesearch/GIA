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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1g 24-April-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Rules
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorRulesGroupClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES

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
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	//parserForwardPropogationSignalData = NULL;
	#endif
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
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	foundMostRecentContribution = false;
	mostRecentContributionWordIndex = INT_DEFAULT_VALUE;
	#endif
}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData(void)
{

}

GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData::GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void)
{
	//sentence specific variables:
	toplevelGroupActivationFound = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	parserEnabled = false;
	performance = 0;
	#endif
	sentenceContents = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	isQuery = false;
	#endif
	//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	parseIsolatedSubreferenceSets = false;
	//#endif

}
GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData::~GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData(void)
{

}
#endif

GIAtxtRelTranslatorRulesGroup::GIAtxtRelTranslatorRulesGroup(void)
{
	semanticRelationFunctionName.resize(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationFunctionName[i] = "";
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	semanticRelationFunctionConditionNewName = "";
	#endif
	groupName = "";
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	previousWordPOStype = "";
	existsPreceedingWordPOStype = "";
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantTypeDerived = GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantTypeDerived = GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;	
	#endif
		
	groupTypeNameBackup = "";
	groupTypeReferenceSetTypeBackup = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN;
	
	numberOfConsecutiveTimesPreviousGroupType = 0;
	
	//components = NULL;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	//ANNfrontGroupConnectionList = NULL;
	//ANNbackGroupConnectionList = NULL;
	//ANNfrontComponentConnectionList = NULL;
	next = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	neuronReference = new ANNneuron();
	#endif
	GIAtokenLayerName = "";
	GIAtokenLayerClassName = "";
	GIAtokenLayerClassTypeName = "";
	GIAtokenLayerClassTypeInstanceName = "";
	neuronGenerated = false;
	neuronPropagated = false;
	neuronPreviousWordPOStypeTested = false;
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	currentParseTreeGroup = NULL;
	#endif	
	neuronPrinted = false;
	neuronActive = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET
	//neuronSolidified = false;
	#endif
	#endif
}
GIAtxtRelTranslatorRulesGroup::~GIAtxtRelTranslatorRulesGroup(void)
{
}

GIAtxtRelTranslatorRulesGroupType::GIAtxtRelTranslatorRulesGroupType(void)
{
	groupTypeName = "";
	
	referenceSetType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES;
}
GIAtxtRelTranslatorRulesGroupType::~GIAtxtRelTranslatorRulesGroupType(void)
{
	//groups = NULL;
}


void GIAtxtRelTranslatorRulesGroupClass::copyParseGroupInfo(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup)	
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
	currentParseTreeGroup->groupTypeNameBackup = currentParseTreeGroupTemp->groupTypeNameBackup;
	
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
