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
 * File Name: GIAtxtRelTranslatorNeuralNetworkPropagateCompact.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j6a 10-January-2020
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkPropagateCompact.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_COMPACT

//static int newNeuronIndex;

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocalCompact;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
static vector<GIAtxtRelTranslatorRulesGroupParseTree*> parseTreeGroupListCompact;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
vector<GIAtxtRelTranslatorRulesGroupParseTree*>* GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::getParseTreeGroupList()
{
	return &parseTreeGroupListCompact;
}
#endif

//if GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::defineFirstLayer(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		bool getFirstLayer = true;
		
		int w = i;

		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalDataNOTUSED;
	
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSignalDataNOTUSED, forwardPropogationSentenceData, getFirstLayer))
		{
			result = false;
		}
	}
	
	return result;
}	
	
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance);
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocalCompact = NULL;
	#endif

	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	//forwardPropogationSentenceData->toplevelGroupActivationFound = false;	//now done by performPropagation
	//forwardPropogationSentenceData->performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData->parserEnabled = parserEnabled;
	#endif
	/*
	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData->isQuery = GIAtxtRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData->parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes = GIAtxtRelTranslatorRulesGroupTypes;
	*/
	
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{	
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = new GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData();
		forwardPropogationSentenceData->forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	
	//OLD: defineFirstLayer(translatorVariables, forwardPropogationSentenceData, &firstLayer)
	
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(performPropagationTest(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData))
	{
		result = true;
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	if(result)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData->sentenceContents->size();
		#else
		*performance = 1;
		#endif
	}
	
	return result;
}
	





bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagationTest(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool deinitialiseParseTreeGroupAfterFinish = true;
	int firstIndexInSequence = 0;
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);
	#endif
	
	bool result = performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
{	
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1 - firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	return performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	int firstIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		firstIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	else
	{
		firstIndexInSequence = 0;
	}
	return performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);

}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSentenceData->toplevelGroupActivationFound = false;
	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkPropagateLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.initialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int iStart = firstIndexInSequence;
	#else
	int iStart = 0;
	#endif
	for(int i=iStart; i<forwardPropogationSentenceData->sentenceContents->size(); i++)	//OLD: for(int i=currentFirstInputNeuronIndexInSequence; ...
	{
		int firstLayerNeuronIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			firstLayerNeuronIndex = forwardPropogationSentenceData->sentenceContents->size()-1-i;
		}
		else
		{
			firstLayerNeuronIndex = i;
		}	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG	
		cout << "\n\n \e[33m GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
	
		//group specific variables:
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		#endif
		
		const bool getFirstLayer = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSentenceData, getFirstLayer))
		{
			//result = true;
		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
		#endif
	}
	
	if(forwardPropogationSentenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];

	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationWordData->w = w;

	//cout << "w = " << w << ", word = " << currentWord->tagName << endl;
	//cout << "w = " << w << endl;
	//cout << "forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
		
	if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		forwardPropogationSignalData->firstPOSval = false;
		#endif
		for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif	
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
				{
					result = false;
				}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
			}
		}
		#endif
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
			{
				result = false;
			}
		}
		else
		{
		#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
			{
				result = false;
			}
			#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		}
		#endif
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	inputLayerGroup->neuronReference->GIAentityName = forwardPropogationWordData->wordReference->tagName;
	inputLayerGroup->groupName = forwardPropogationWordData->wordReference->tagName;
	#endif
	*/
	//cout << "wordPOStype = " << wordPOStype << endl;
	forwardPropogationWordData->wordPOStype = wordPOStype;
	inputLayerGroup->wordPOStype = wordPOStype;
	//cout << "inputLayerGroup->wordPOStype = " << inputLayerGroup->wordPOStype << endl;
	if(getFirstLayer)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		(forwardPropogationSentenceData->firstLayer->at(w))->push_back(inputLayerGroup);
		#else
		forwardPropogationSentenceData->firstLayer->push_back(inputLayerGroup);
		#endif
	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
		#endif
	
		GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not properly used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR
		int layer = 0;
		propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup);
	}
			
	return result;
}	

		

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops	//only required for recursion (not yet supported by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR)
	{
		group->neuronPropagated = true;
	#endif
		
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network

	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if((forwardPropogationSentenceData->findingCandidateComponent2))
			{
				if(!(forwardPropogationSentenceData->foundCandidateComponent2))
				{
					if(currentComponent == forwardPropogationSentenceData->candidateComponent2ToFind)
					{
						forwardPropogationSentenceData->foundCandidateComponent2 = true;
						forwardPropogationSentenceData->candidateComponent2sourceParseTreeGroup = activationPathWordCurrentParseTreeGroup;
					}
				}
			}
			#endif
			/*
			cout << "ownerGroup:" << endl;
			cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
			cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
			*/
		
			int componentIndex = currentComponent->componentIndex;	
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);	
				cout << "2a: propagateWordThroughNetworkGroup: ownerGroup->groupIndex = " <<  ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
				#endif
				
				if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
					//cout << "currentComponent->candidateStringMatch = " << currentComponent->candidateStringMatch->tagName << endl;
				}
				/*
				if(currentComponent->semanticRelationReturnEntity)
				{
					ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
				}
				*/
	
				bool sequentialActivationFound = false;
				bool existingActivationFound = false;
				//TODO: determine if new/GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR group reset method sufficient; see GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFound))
				{
					result = true;
				}

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFound)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;

	/*	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	*/
	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	bool missingStartComponentsFound = false;
	bool missingOrVariableStartComponentFound = false;
	bool variableStartComponentFound = false;	//temp debug only
	int numberOfInactiveComponentsRemaining = 0;
	
	bool componentWordConnectivityTests = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, false))
	{
		componentWordConnectivityTests = true;
	}
	#endif
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent, existingActivationFound, &missingStartComponentsFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, &numberOfInactiveComponentsRemaining))
	{	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - missingStartComponentsFound = " << missingStartComponentsFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
		#endif
		
		bool variableStartComponentFoundNOTUSED = false;
		if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFound, variableStartComponentFoundNOTUSED))
		{
			*sequentialActivationFound = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				if(!missingStartComponentsFound)
				{
					if(missingOrVariableStartComponentFound)
					{
						missingOrVariableStartComponentFound = false;	//the first component found was variable and correct
						if(numberOfInactiveComponentsRemaining == 0)
						{
							activationSequenceCompleted = true;
						}
					}
				}
			}
			#endif
		}
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			if(missingOrVariableStartComponentFound)
			{
				bool firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = false; 
				GIAtxtRelTranslatorRulesGroupParseTree activatedNeuronCandidate;
				activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
				activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;				
				if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1)))	//ie if missing || variable start component
				{
					updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
					if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
					{
						firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = true;
					}
					else
					{
						missingOrVariableStartComponentFound = false;
					}
				}
				else
				{
					cerr << "potential algorithm error; if(missingOrVariableStartComponentFound); the following should always be true: if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))" << endl;
					exit(EXIT_ERROR);
				}

				if(missingOrVariableStartComponentFound)
				{
					if(!(*sequentialActivationFound))
					{
						if((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))	//only 1 component has been activated
						{
							//variable first component detected
							//word indices do not align, so assume first component was miscellaneously activated (mark it as variable) 

							if(firstLastWordIndexTestHypotheticalWithoutVariableStartComponent)
							{
								variableStartComponentFound = true;
								if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, &activatedNeuronCandidate, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFound, variableStartComponentFound))
								{
									*sequentialActivationFound = true;
									//ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound = true;
									ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
									ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
								}
								else
								{
									missingOrVariableStartComponentFound = false;
								}
							}
						}	
					}
				}

				if(missingOrVariableStartComponentFound)
				{
					ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound = true;
				}
			}
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		//#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ORIG
		//moved GIA3j5aTEMP36 from old GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent:if(activationSequenceCompleted), verifyActivatedNeuronWithMaxWordIndexCoverage checks
		bool strictStartingCondition = false;
		if(missingStartComponentsFound || (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound))	//or: || missingOrVariableStartComponentFound
		{
			strictStartingCondition = true;
		}
		GIAtxtRelTranslatorRulesGroupParseTree activatedNeuronCandidate = *(ownerGroup->currentParseTreeGroupTemp);
		if(firstActiveComponentInGroup)
		{
			activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		}
		updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
		if(!verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, strictStartingCondition))
		{
			(*sequentialActivationFound) = false;
		}	
		//#endif
		#endif

		if(*sequentialActivationFound)
		{			
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentsFound = " << missingStartComponentsFound << ", (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) = " << (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) << ", variableStartComponentFound = " << variableStartComponentFound << endl;	
			#else
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentsFound = " << missingStartComponentsFound << endl;				
			#endif
			#endif
		
			bool missingOrVariableStartComponentFoundStored = false;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				missingOrVariableStartComponentFoundStored = ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
			}
			#endif
			bool variableStartComponentFoundStoredNOTUSED = false;	//= variableStartComponentFound //variableStartComponentFound = ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound;
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFound, missingStartComponentsFound, missingOrVariableStartComponentFoundStored, variableStartComponentFoundStoredNOTUSED))
			{
				result = true;
			}
		}
	}

	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(!activationSequenceCompleted)
		{
			if(*sequentialActivationFound)
			{
				bool testWordIndicesAllowed = true;
				bool testMinNumComponents = true;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
				if(ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound)
				{
					testWordIndicesAllowed = true;
					testMinNumComponents = false;
				}
				#endif
				
				bool candidateCoveragePartial = true;
				if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, ownerGroup->currentParseTreeGroupTemp, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
					cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage" << endl;
					#endif
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = ownerGroup->currentParseTreeGroupTemp;
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = true;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
					if(ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound)
					{
						forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariable = true;
					}
					else
					{
						forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariable = false;
					}
					#endif
				}
				
			}
		}
	}
		
	return result;
}
	
		
		
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFound, const bool missingStartComponentsFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound)
{
	bool result = true;
	
	if(existingActivationFound)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
		cout << "******** resetGroupParseTreeGroupRef" << endl;
		#endif
		group->neuronActive = false;	//not used
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetGroupActivation(ownerGroup);	//OLD: component activations have already been reset by GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, false);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_FREE_MEMORY
		//cout << "ownerGroup->currentParseTreeGroupTemp->groupName = " << ownerGroup->currentParseTreeGroupTemp->groupName << endl;
		parseTreeGroupListCompact.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	//}
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorNeuralNetworkPropagateInverseClass::generateRulesGroupTreeComponents;	
	GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	/*
	if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		cout << "\tnewParseComponent->candidateStringMatch = " << newParseComponent->candidateStringMatch->tagName << endl;
	}	
	*/
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
						
	GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		//currentParseTreeGroupTemp->components.push_front(newParseComponent);
		currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	}
	else
	{
		currentParseTreeGroupTemp->components.push_back(newParseComponent);
	}
	#endif

	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	//cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex << endl;
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: newParseComponent->componentIndex = " << newParseComponent->componentIndex << ", ownerGroup->components.size() = " << ownerGroup->components.size() << ", groupIndex = " << ownerGroup->groupIndex << ", existingActivationFound = " << existingActivationFound << ", activationSequenceCompleted = " << activationSequenceCompleted << endl; 
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif

		
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
		//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
		//}
		#endif
	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
		cout << "topLevelGroup0" << endl;
		#endif
		
		/*
		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);
		*/
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;
		
	
		bool testWordIndicesAllowed = true;
		bool testMinNumComponents = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(missingOrVariableStartComponentFound)
		{
			testWordIndicesAllowed = true;
			testMinNumComponents = false;
		}
		#endif
		bool candidateCoveragePartial = false;	
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, activationPathWordCurrentParseTreeGroupOwner, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG		
				cout << "topLevelGroup1" << endl;
				#endif

				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = activationPathWordCurrentParseTreeGroupOwner;
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = false;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
				if(missingOrVariableStartComponentFound)
				{
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariable = true;
				}
				else
				{
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariable = false;
				}
				#endif
			}
		}
		
			
		bool topLevelGroup = false;
		//if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activationPathWordCurrentParseTreeGroupOwner))
		//{	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ORIG
		if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, activationPathWordCurrentParseTreeGroupOwner, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
		{	
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
		{			
		#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			int maxWeight;
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, &maxWeight);
			if(maxWeight >= parseTreeMaxWeight)
			{
			#endif
				//cout << "topLevelGroup2" << endl;

				int wComparisonInt;
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					wComparisonInt = 0;	//start of sentence found
				}
				else
				{
					wComparisonInt = forwardPropogationSentenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
				}
				if(forwardPropogationWordData->w == wComparisonInt)	//start of sentence found
				{

					//cout << "topLevelGroup3" << endl;

					int activatedNeuronWithMaxWordIndexCoverageCoverage = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner);
					//cout << "activatedNeuronWithMaxWordIndexCoverageCoverage = " << activatedNeuronWithMaxWordIndexCoverageCoverage << endl;

					if(activatedNeuronWithMaxWordIndexCoverageCoverage == forwardPropogationSentenceData->sentenceContents->size())
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
						cout << "topLevelGroup" << endl;
						#endif
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_WORDCONNECTIVITY_VERIFICATION
						if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.updatePerformanceGroup(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
						{
							cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: !GIAtxtRelTranslatorNeuralNetworkPropagateOperations.updatePerformance" << endl;
							cerr << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
							cerr << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
							exit(EXIT_ERROR);
						}
						#endif

						topLevelGroup = true;

						forwardPropogationSentenceData->finishedPassingSentenceWords = true;

						forwardPropogationSentenceData->toplevelGroupActivationFound = true;

						parseTreeMaxWeight = maxWeight;

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPLICATE_TOP_LEVEL_PARSE_TREE
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteParseTree(topLevelParseTreeGroupLocalCompact, 0);
						topLevelParseTreeGroupLocalCompact = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
						#else
						//OLD: topLevelParseTreeGroupLocalCompact = activationPathWordCurrentParseTreeGroupOwner;
						//copy currentParseTreeGroupTemp so it cant be overwritten;
						topLevelParseTreeGroupLocalCompact = GIAtxtRelTranslatorRules.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
						#endif
						#endif

						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
						cout << "topLevelGroup" << endl;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
						cout << "parseTreeMaxWeight1 = " << parseTreeMaxWeight << endl;
						#endif
						cout << "topLevelParseTreeGroupLocalCompact->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
						printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
						#endif
						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						*/
					}
				}

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			}
			#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ORIG
		}	
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(!missingOrVariableStartComponentFound)	//prevents propagation up if variableStartComponentFound
		{
		#endif	
			if(!topLevelGroup)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = true;
				#endif

				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
				{
					//result = true;
				}

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = false;
				#endif
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		}
		#endif
		//}
	}

	return result;
}




bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::sequentialActivationConnectivityTests(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound, const bool variableStartComponentFound)
{	
	bool sequentialActivationFound = false;
				
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	//DOESNTWORK if(componentWordConnectivityTests || *existingActivationFound) 	//if existingActivationFound: group activations will be reset so assume real ownerGroupParseTreeGroup->components.size() == 0 
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentWordConnectivityTestsWrapper(currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound) || variableStartComponentFound)
	{
		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		Note if variableStartComponentFound, then componentWordConnectivityTestsWrapper is not required to be reexecuted, since if the previous component was variable (ie miscellaneously activated) and components->size()==1, no word index connectivity test is required to be conducted. 
		If the first component was variable, but components->size() >= 2, then the previous component would be valid and would already have been tested when componentWordConnectivityTestsWrapper was executed the first time
		#endif
		*/
		//cout << "pass componentWordConnectivityTestsWrapper" << endl;
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
		if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationWordData, currentComponent, ownerGroup, activationPathWordCurrentParseTreeGroup, existingActivationFound))
		{
		#endif
			//cout << "pass upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper" << endl;
			sequentialActivationFound = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
		}
		#endif				
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	}
	#endif
	
	return sequentialActivationFound;
}	


	
void GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParent(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorRulesComponentParseTree* parentComponent, GIAtxtRelTranslatorRulesGroupParseTree* childGroup)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_GENERATE_PARSE_TREE
	if(parentComponent->parseTreeGroupRef != NULL)
	#else
	if(childGroup != NULL)
	#endif
	{	
		//has children
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_GENERATE_PARSE_TREE
		childGroup = parentComponent->parseTreeGroupRef;	//not required
		#endif
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
			//cout << "parentGroup->parseTreeMaxWordIndex = " << parentGroup->parseTreeMaxWordIndex << endl;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
			//cout << "parentGroup->parseTreeMinWordIndex = " << parentGroup->parseTreeMinWordIndex << endl;
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
	}
	else
	{
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			//cout << "parentGroup->parseTreeMaxWordIndex = " << parentGroup->parseTreeMaxWordIndex << endl;
		}
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			//cout << "parentGroup->parseTreeMinWordIndex = " << parentGroup->parseTreeMinWordIndex << endl;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}

void GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParentBasic(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesGroupParseTree* childGroup)
{
	if(childGroup != NULL)
	{	
		//has children
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
			//cout << "parentGroup->parseTreeMaxWordIndex = " << parentGroup->parseTreeMaxWordIndex << endl;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
			//cout << "parentGroup->parseTreeMinWordIndex = " << parentGroup->parseTreeMinWordIndex << endl;
		}
		
		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
		*/
	}
	else
	{
		if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			//cout << "parentGroup->parseTreeMaxWordIndex = " << parentGroup->parseTreeMaxWordIndex << endl;
		}
		if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			//cout << "parentGroup->parseTreeMinWordIndex = " << parentGroup->parseTreeMinWordIndex << endl;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}



bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}









/*
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable)
{
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	return verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, activatedNeuronBaseline, activatedNeuronCandidate, testWordIndicesAllowed, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, testMinNumComponents, candidateCoveragePartial, candidateCoverageVariable);
}
*/

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	bool candidateNeuronInCompleteHistory = false;
	for(int i=0; i<forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
	{
		if(activatedNeuronCandidate->groupRef == forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory[i])
		{
			//cout << "candidateNeuronInCompleteHistory" << endl;
			candidateNeuronInCompleteHistory = true;	
		}
	}	
	if(!candidateNeuronInCompleteHistory)
	{
	#endif
	#endif
		//if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)	//assumed true
		//{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		if(verifyNeverSplitGroupBetweenTwoIdenticalComponents(forwardPropogationSentenceData, activatedNeuronCandidate, candidateCoveragePartial))
		{
		#endif

			int activatedNeuronCandidateCoverage = 0;
			int activatedNeuronCandidateMaxWeight = 0;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			bool activatedNeuronCandidateMinNumComponentsTest = false;
			#endif
			if(activatedNeuronCandidate != NULL)
			{
				activatedNeuronCandidateCoverage = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(activatedNeuronCandidate);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronCandidate, forwardPropogationSentenceData, &activatedNeuronCandidateMaxWeight);
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
				if(activatedNeuronCandidate->components.size() >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)
				{
					activatedNeuronCandidateMinNumComponentsTest = true;
				}
				#endif

			}
			int activatedNeuronBaselineCoverage = 0;
			int activatedNeuronBaselineMaxWeight = 0;
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				if(activatedNeuronBaseline != NULL)
				{
					activatedNeuronBaselineCoverage = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(activatedNeuronBaseline);
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					//activatedNeuronBaselineMaxWeight = activatedNeuronBaselineCoverage->parseTreeMaxWeight;
					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronBaseline, forwardPropogationSentenceData, &activatedNeuronBaselineMaxWeight);
					#endif
				}
			}

			bool firstLastWordIndexTest = false;
			if(testWordIndicesAllowed)
			{	
				//cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage" << endl;
				bool strictStartingCondition = true;
				firstLastWordIndexTest = verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activatedNeuronCandidate, strictStartingCondition);
			}
			else
			{
				firstLastWordIndexTest = true;
			}

			//cout << "activatedNeuronCandidate->components.size() = " << activatedNeuronCandidate->components.size() << endl;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
			{
			#endif
				if(firstLastWordIndexTest)
				{
					bool passBaselineComparisonTests = true;
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{
						passBaselineComparisonTests = false;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;
						#endif

						bool passBaselineCoverageTests = false;
						if(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)
						{
							passBaselineCoverageTests = true;
						}
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial && !(candidateCoveragePartial) && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow fully active activatedNeuronCandidate to override partially active activatedNeuronBaseline, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariable && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableStartComponentFound to override activatedNeuronBaseline with missingOrVariableStartComponentFound, assuming they have the same coverage
							}
						}
						#endif

						if(passBaselineCoverageTests)
						{
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							if(activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight)
							{
								//GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setPerformanceWeightOfParseTree(activatedNeuronBaselineCoverage, maxWeight);
								//activatedNeuronBaselineCoverage->parseTreeMaxWeight = maxWeight;
							#endif
								passBaselineComparisonTests = true;
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							}
							#endif
						}

					}
					if(passBaselineComparisonTests)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						result = true;
					}

				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			}
			#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		}
		#endif
		//}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	}
	#endif
	#endif
		
	return result;
}
	
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexAllowed(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition)
{
	bool result = false;

	int activatedNeuronCandidateMinWordIndex = activatedNeuronCandidate->parseTreeMinWordIndex;
	int activatedNeuronCandidateMaxWordIndex = activatedNeuronCandidate->parseTreeMaxWordIndex;

	bool firstWordIndexTest = false;
	bool lastWordIndexTest = false;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
		cout << "activatedNeuronCandidateMinWordIndex = " << activatedNeuronCandidateMinWordIndex << endl;
		cout << "activatedNeuronCandidateMaxWordIndex = " << activatedNeuronCandidateMaxWordIndex << endl;
		cout << "forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed << endl;
		cout << "forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed << endl;
		#endif
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMaxWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
				//cout << "firstWordIndexTest" << endl;
			}
		}
		else
		{
			if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
				//cout << "firstWordIndexTest" << endl;
			}		
		}
		if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
			//cout << "lastWordIndexTest" << endl;
		}
	}
	else
	{
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMinWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}

	if(firstWordIndexTest && lastWordIndexTest)
	{
		result = true;
	}	

	return result;
}
		

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronsAtLeastOne(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)
		{			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
			if(newNeuronSequenceGroup1 == activatedNeuronWithMaxWordIndexCoverage->groupRef) 
			{
				//intrasentenceMatch = true;
				pass = false;
			}
			#endif
		}
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
			
		//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
		if(neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
		{
			//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
			pass = false;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: activatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoveragePartial = " << activatedNeuronWithMaxWordIndexCoverage->activatedNeuronWithMaxWordIndexCoveragePartial << endl;
			//exit(EXIT_ERROR);
			#endif
		}
			
		if(candidateCoveragePartial)
		{
			//condition2: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in activatedNeuronWithMaxWordIndexCoverage equals the next unactivated component groupRef in activatedNeuronWithMaxWordIndexCoverage
			int lastActivatedIndex = INT_DEFAULT_VALUE;
			int firstUnactivatedIndex = INT_DEFAULT_VALUE;
			bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
			
			/*
			for(int i=0; i<activatedNeuronWithMaxWordIndexCoverage->components.size(); i++)
			{
				cout << "activatedNeuronWithMaxWordIndexCoverage->components[i]->componentIndex = " << activatedNeuronWithMaxWordIndexCoverage->components[i]->componentIndex << endl;
			}
			for(int i=0; i<activatedNeuronWithMaxWordIndexCoverage->groupRef->components.size(); i++)
			{
				cout << "activatedNeuronWithMaxWordIndexCoverage->groupRef->components[i]->neuronComponentConnectionActive = " << activatedNeuronWithMaxWordIndexCoverage->groupRef->components[i]->neuronComponentConnectionActive << endl;
			}
			*/
			
			if(partialActivationConfirmed)	//OLD: partial activation test required in case where activatedNeuronWithMaxWordIndexCoverage is recorded by GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
			{
				//cout << "lastActivatedIndex = " << lastActivatedIndex << endl;
				//cout << "firstUnactivatedIndex = " << firstUnactivatedIndex << endl;
				//cout << "activatedNeuronWithMaxWordIndexCoverage->components.size() = " << activatedNeuronWithMaxWordIndexCoverage->components.size() << endl;
				if(lastActivatedIndex == firstUnactivatedIndex)
				{
					cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
					exit(EXIT_ERROR);
				}
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[lastActivatedIndex];		//BAD: activatedNeuronWithMaxWordIndexCoverage->components[lastActivatedIndex]->componentRef;
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstNonActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[firstUnactivatedIndex];	//BAD: activatedNeuronWithMaxWordIndexCoverage->components[firstUnactivatedIndex]->componentRef;
				if(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)
				{
					//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
					pass = false;	
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
					cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition2" << endl;
					//exit(EXIT_ERROR);
					#endif
				}
			}
			else
			{
				//cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents error: !partialActivationConfirmed" << endl;
				//exit(EXIT_ERROR);
			}
		}
		
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
	int currentMaxWordIndexInSequence;
	int nextWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	else
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	int currentMinWordIndexInSequence;
	int previousWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	else
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	cout << "\n\n activatedNeuronWithMaxWordIndexCoverage->components.size() = " << activatedNeuronWithMaxWordIndexCoverage->components.size() << endl;	
	cout << "currentMaxWordIndexInSequence = " << currentMaxWordIndexInSequence << endl;
	cout << "nextWordIndexInSequence = " << nextWordIndexInSequence << endl;
	cout << "currentMinWordIndexInSequence = " << currentMinWordIndexInSequence << endl;
	cout << "previousWordIndexInSequence = " << previousWordIndexInSequence << endl;
	#endif
	
	if(((currentMaxWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMaxWordIndexInSequence >= 0)) && ((nextWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (nextWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* currentMaxWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
		vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* nextWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];			
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];		
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);
		#else
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMaxWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* nextWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];	
		#endif
			if(currentMaxWordIndexInSequenceFirstLayerNeuron == nextWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	if(((currentMinWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMinWordIndexInSequence >= 0)) && ((previousWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (previousWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* currentMinWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* previousWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];			
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);		
		#else		
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];	
		#endif
			if(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)
			{
				//cout << "(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)" << endl;
				result = true;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronsAtLeastOneBasic(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	if(activatedNeuronWithMaxWordIndexCoverage != NULL)
	{
		result = true;
	}
	
	return result;
}



bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexLastActivatedIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		result = true;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
		}
	}
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//CHECKTHIS;
//ordered (do not assume normal order): "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentsFound, but all remaining components are active)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int firstActivatedIndexUnordered;
	int lastActivatedIndexUnordered;
	if(identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, parseTreeGroupNeuron, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered))
	{
		//cout << "firstActivatedIndexUnordered = " << firstActivatedIndexUnordered << endl;
		//cout << "lastActivatedIndexUnordered = " << lastActivatedIndexUnordered << endl;
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = firstActivatedIndexUnordered;
			*firstUnactivatedIndex = firstActivatedIndexUnordered-1;
			//cout << "*firstUnactivatedIndex = " << *firstUnactivatedIndex << endl;			
			if(*firstUnactivatedIndex >= 0)
			{
				partialActivationConfirmed = true;
			}
		}
		else
		{
			*lastActivatedIndex = lastActivatedIndexUnordered;
			*firstUnactivatedIndex = lastActivatedIndexUnordered+1;
			if(*firstUnactivatedIndex < groupOrig->components.size())
			{
				partialActivationConfirmed = true;
			}
		}
	}
	
	return partialActivationConfirmed;
}

//unordred (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexFirstAndLastActivatedIndexUnordered(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex)
{
	bool result = false;
	
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		*firstActivatedIndex = firstActivatedComponent->componentIndex;
		*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		
		result = true;
	}
	
	return result;
}
#else

//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxWordIndexCoverage (never fullyActivatedNeuronWithMaxWordIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentsFound, but all remaining components are active)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() < groupOrig->components.size())
	{
		partialActivationConfirmed = true;
		
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
			*firstUnactivatedIndex = groupOrig->components.size()-1-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
			*firstUnactivatedIndex = numberOfActivatedComponents;
		}
	}
	
	return partialActivationConfirmed;
}


//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		int firstActivatedIndex = firstActivatedComponent->componentIndex;
		if(firstActivatedIndex == 0)
		{
			result = true;
			*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		}
		else
		{
			cerr << "!GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION: GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered error:  (firstActivatedIndex != 0)" << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}
#endif



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	if((ownerGroupParseTreeGroup->components.size() == 0) || existingActivationFound)
	{
		//cout << "existingActivationFound = " << existingActivationFound << endl;
		
		int firstWordIndexInSequence;
		//expect to find upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent if current group->parseTreeMinWordIndex/parseTreeMaxWordIndex doesnt encapsulate firstWordIndexInSequence

		int wordIndexMax = -1;
		int wordIndexMin = REALLY_LARGE_INT;
		GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMin;
		GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax;
		int wordIndexMinMin;
		int wordIndexMaxMax;
		int wordIndexMinToFindAbove;		
		int wordIndexMaxToFindAbove;
		int wordIndexLastAboveEffectiveAfterGroupReset = INT_DEFAULT_VALUE;

		bool execute = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "\nGIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper start:" << endl;
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
			#endif

			parseTreeGroupToFindWordIndexMax = activationPathWordCurrentParseTreeGroup;
			wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMax != NULL)
			{
				if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
				{

				}
			}
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
			cout << "wordIndexMax = " << wordIndexMax << endl;
			#endif
			
			wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMax (maxInt) = " << wordIndexMax << endl;
			#endif
			
			if(wordIndexMax < firstWordIndexInSequence)
			{
				wordIndexMaxToFindAbove = firstWordIndexInSequence;
				wordIndexMinToFindAbove = wordIndexMax+1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFound)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMaxWordIndex+1;//CHECKTHIS; ??
				}
				
				execute = true;
			}
		}
		else
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = 0;
			#endif

			parseTreeGroupToFindWordIndexMin = activationPathWordCurrentParseTreeGroup;
			wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMin != NULL)
			{
				if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
				{

				}
			}
			wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);
			if(wordIndexMin > firstWordIndexInSequence)
			{
				wordIndexMinToFindAbove = firstWordIndexInSequence;
				wordIndexMaxToFindAbove = wordIndexMin-1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFound)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMinWordIndex-1;	
				}
				
				execute = true;
			}
		}	
		
		if(execute)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxToFindAbove = " << wordIndexMaxToFindAbove << endl;
			cout << "wordIndexMinToFindAbove = " << wordIndexMinToFindAbove << endl;
			cout << "wordIndexLastAboveEffectiveAfterGroupReset = " << wordIndexLastAboveEffectiveAfterGroupReset << endl;
			#endif
				
			if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, 0, ownerGroup, false, false))
			{
				result = true;
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "!upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent" << endl;
				#endif
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
						
	return result;
}

//bool GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool findMaxOrMinWordIndex, const int wordIndexMaxOrMinToFind, int level, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound)
{
	bool result = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
	bool keepExecuting = true;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
	wordIndexMinFound = false;
	wordIndexMaxFound = false;
	#endif
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		if(keepExecuting)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
			bool keepExecutingLocal = true;
			#endif
		#endif
			
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
			#endif

			GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
			int parseTreeMinWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMinWordIndex;
			int parseTreeMaxWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMaxWordIndex;

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndex = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndex = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif


			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
			bool groupHasPreceedingComponent = false;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				if(currentComponent->componentIndex < ownerGroup->components.size()-1)
				{
					groupHasPreceedingComponent = true;
				}
			}
			else
			{
				 if(currentComponent->componentIndex > 0)
				 {
			 		groupHasPreceedingComponent = true;
				 }
			}
			#endif

			if(currentComponent->neuronComponentConnectionActive)
			{//currentComponent was activated by the group being reset - so determine the ownerGroup's expected [older] parseTreeMinWordIndex/parseTreeMaxWordIndex values assuming the group is reset
				if(wordIndexLastAboveEffectiveAfterGroupReset != INT_DEFAULT_VALUE)
				{
					if(forwardPropogationSentenceData->parseSentenceReverse)
					{
						if(parseTreeMinWordIndexEffectiveAfterGroupReset < wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMinWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
					else
					{
						if(parseTreeMaxWordIndexEffectiveAfterGroupReset > wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMaxWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
				}
			}

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndexEffectiveAfterGroupReset = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndexEffectiveAfterGroupReset = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif

			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				//assume will find wordIndexMin match before wordIndexMax match when propagating up network - CHECKTHIS
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMinFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				if(wordIndexMinFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				#endif
			}
			else
			{
				//assume will find wordIndexMax match before wordIndexMin match when propagating up network - CHECKTHIS
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMaxFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				if(wordIndexMaxFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				#endif
			}


			if(wordIndexMinFound && wordIndexMaxFound)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - (wordIndexMinFound && wordIndexMaxFound); result == true" << endl;
				//exit(EXIT_ERROR);
				#endif
				result = true;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
				keepExecuting = false;
				#endif
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				if(keepExecutingLocal)
				{
				#endif
					if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, wordIndexMinFound, wordIndexMaxFound))
					{
						result = true;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				}
				#endif
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		}
		#endif
	}
	
	return result;
}

#endif



#endif
