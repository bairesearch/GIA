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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2f 10-August-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkPropagateCompact.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR

//static int newNeuronIndex;

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocalCompact;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
static vector<GIAtxtRelTranslatorRulesGroupParseTree*> parseTreeGroupListCompact;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
vector<GIAtxtRelTranslatorRulesGroupParseTree*>* GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::getParseTreeGroupList()
{
	return &parseTreeGroupListCompact;
}
#endif


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance, createNewConnections, firstLayer);
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool result = false;

	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance, createNewConnections, firstLayer);
	#endif
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance, createNewConnections, firstLayer);
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
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
	

	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		int w = i;
		/*
		int w;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			w = forwardPropogationSentenceData->sentenceContents->size()-1-i;
		}
		else
		{
			w = i;
		}
		*/

		//if(!propagateWordThroughNetworkIntro(translatorVariables, w, forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup, createNewConnections))
		if(!defineFirstLayer(translatorVariables, w, forwardPropogationSentenceData, firstLayer))
		{
			result = false;
		}
	}
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(performPropagationTest(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer))
	{
		result = true;
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
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
	





bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagationTest(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool deinitialiseParseTreeGroupAfterFinish = true;
	int firstIndexInSequence = 0;
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);
	#endif
	/*
	//not required;
	forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	*/
	bool result = performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
{	
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = firstLayer->size()-1 - firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	return performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	int firstIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		firstIndexInSequence = firstLayer->size()-1;
	}
	else
	{
		firstIndexInSequence = 0;
	}
	return performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);

}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSentenceData->toplevelGroupActivationFound = false;
	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkPropagateLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.initialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int iStart = firstIndexInSequence;
	#else
	int iStart = 0;
	#endif
	for(int i=iStart; i<firstLayer->size(); i++)	//OLD: for(int i=currentFirstInputNeuronIndexInSequence; ...
	{
		int firstLayerNeuronIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			firstLayerNeuronIndex = firstLayer->size()-1-i;
		}
		else
		{
			firstLayerNeuronIndex = i;
		}	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG	
		cout << "\n\n \e[33m GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
		
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstLayerNeuronGroup = (*firstLayer)[firstLayerNeuronIndex]; 

		/*
		cout << "firstLayerNeuronGroup->groupIndex = " << firstLayerNeuronGroup->groupIndex << endl;
		cout << "firstLayerNeuronGroup->components.size() = " << firstLayerNeuronGroup->components.size() << endl;
		*/
	
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[firstLayerNeuronIndex];	//OLD: firstLayerNeuronGroup->wordDataTemp;
		//cout << "forwardPropogationWordData->wordDataTemp->tagName = " << forwardPropogationWordData->wordReference->tagName << endl;

		//group specific variables:
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		#endif
		
		GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR

		int layer = 0;
		if(propagateWordThroughNetworkGroup(translatorVariables, firstLayerNeuronGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			//result = true;
		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
		#endif
	}
	
	if(forwardPropogationSentenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::defineFirstLayer(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
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
	
	if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif	
				if(!propagateWordThroughNetworkGroupInitSequenceGrammar(translatorVariables, w, wordPOStype, forwardPropogationWordData, forwardPropogationSentenceData, firstLayer))
				{
					result = false;
				}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
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
			if(!propagateWordThroughNetworkGroupInitSequenceGrammar(translatorVariables, w, wordPOStype, forwardPropogationWordData, forwardPropogationSentenceData, firstLayer))
			{
				result = false;
			}
		}
		else
		{
		#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
			if(!propagateWordThroughNetworkGroupInitSequenceGrammar(translatorVariables, w, wordPOStype, forwardPropogationWordData, forwardPropogationSentenceData, firstLayer))
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

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupInitSequenceGrammar(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool result = true;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	inputLayerGroup->neuronReference->GIAentityName = forwardPropogationWordData->wordReference->tagName;
	inputLayerGroup->groupName = forwardPropogationWordData->wordReference->tagName;
	#endif
	*/
	forwardPropogationWordData->wordPOStype = wordPOStype;
	inputLayerGroup->wordDataTemp = forwardPropogationWordData;
	firstLayer->push_back(inputLayerGroup);
				
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
	
	bool componentWordConnectivityTests = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, false))
	{
		componentWordConnectivityTests = true;
	}
	#endif
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent, existingActivationFound, &missingStartComponentsFound, componentWordConnectivityTests))
	{	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - PASS1" << endl;
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		if(missingStartComponentsFound)
		{
			activationSequenceCompleted = false;
		}
		#endif
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
		//DOESNTWORK if(componentWordConnectivityTests || *existingActivationFound) 	//if existingActivationFound: group activations will be reset so assume real ownerGroupParseTreeGroup->components.size() == 0 
		if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound))
		{
		#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
			if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationWordData, currentComponent, ownerGroup, activationPathWordCurrentParseTreeGroup, *existingActivationFound))
			{
			#endif
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
				cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
				#endif

				*sequentialActivationFound = true;

				if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFound, missingStartComponentsFound))
				{
					result = true;
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
			}
			/*
			else
			{
				cout << "!upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper2" << endl;
			}
			*/
			#endif				
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
		}
		#endif
	}

	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(*sequentialActivationFound && !activationSequenceCompleted)
		{
			//cout << "forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed << endl;
			//cout << "forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed << endl;
			if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, true))
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
				cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage" << endl;
				#endif
				forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage = ownerGroup->currentParseTreeGroupTemp;
			}
		}
	}
		
	return result;
}
					
	
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFound, const bool missingStartComponentsFound)
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
		//cout << "ownerGroup->currentParseTreeGroupTemp->groupName = " << ownerGroup->currentParseTreeGroupTemp->groupName << endl;
		parseTreeGroupListCompact.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	
	#endif
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorNeuralNetworkPropagateInverseClass::generateRulesGroupTreeComponents;
	GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
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

	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
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
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
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
		
		bool topLevelGroup = false;
		if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, true))
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG		
			cout << "topLevelGroup1" << endl;
			#endif
			
			forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage = activationPathWordCurrentParseTreeGroupOwner;
						
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
					
					int activatedNeuronWithMaxWordIndexCoverageCoverage = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage);
					//cout << "activatedNeuronWithMaxWordIndexCoverageCoverage = " << activatedNeuronWithMaxWordIndexCoverageCoverage << endl;
					
					/*
					cout << "activationPathWordCurrentParseTreeGroupOwner coverage = " << GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner) << endl;
					cout << "forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage coverage = " << GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage) << endl;
					*/

					if(activatedNeuronWithMaxWordIndexCoverageCoverage == forwardPropogationSentenceData->sentenceContents->size())
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
						cout << "topLevelGroup" << endl;
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
		}
		
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
	}

	return result;
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



bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}










bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const bool testMinNumComponents)
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
	return verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, activatedNeuronBaseline, activatedNeuronCandidate, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, testMinNumComponents);
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, const bool testMinNumComponents)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
	
	int activatedNeuronCandidateCoverage = 0;
	int activatedNeuronCandidateMinWordIndex = 0;
	int activatedNeuronCandidateMaxWordIndex = 0;
	int activatedNeuronCandidateMaxWeight = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
	bool activatedNeuronCandidateMinNumComponentsTest = false;
	#endif
	if(activatedNeuronCandidate != NULL)
	{
		activatedNeuronCandidateCoverage = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateCoverage(activatedNeuronCandidate);
		activatedNeuronCandidateMinWordIndex = activatedNeuronCandidate->parseTreeMinWordIndex;
		activatedNeuronCandidateMaxWordIndex = activatedNeuronCandidate->parseTreeMaxWordIndex;
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

	bool firstWordIndexTest = false;
	bool lastWordIndexTest = false;
	//cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage" << endl;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_WORDINDEX_VERIFICATION
		cout << "activatedNeuronCandidateMinWordIndex = " << activatedNeuronCandidateMinWordIndex << endl;
		cout << "activatedNeuronCandidateMaxWordIndex = " << activatedNeuronCandidateMaxWordIndex << endl;
		cout << "activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = " << activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed << endl;
		cout << "activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = " << activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed << endl;
		#endif
		if(activatedNeuronCandidateMaxWordIndex == activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
		{
			firstWordIndexTest = true;
			//cout << "firstWordIndexTest" << endl;
		}
		if(activatedNeuronCandidateMinWordIndex >= activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
			//cout << "lastWordIndexTest" << endl;
		}
	}
	else
	{
		if(activatedNeuronCandidateMinWordIndex == activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
		{
			firstWordIndexTest = true;
		}
		if(activatedNeuronCandidateMaxWordIndex <= activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}	
	
	//cout << "activatedNeuronCandidate->components.size() = " << activatedNeuronCandidate->components.size() << endl;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
	if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
	{
	#endif
		if(firstWordIndexTest && lastWordIndexTest)
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
				if(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)
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
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronsAtLeastOne(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, GIAtxtRelTranslatorRulesGroupParseTree** fullyActivatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupParseTree** partiallyActivatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, *fullyActivatedNeuronWithMaxWordIndexCoverage, *partiallyActivatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		if((*partiallyActivatedNeuronWithMaxWordIndexCoverage) != NULL)
		{			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
			if(newNeuronSequenceGroup1 == (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->groupRef) 
			{
				//intrasentenceMatch = true;
				pass = false;
			}
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			//condition1: dont select a matched neuron partiallyActivatedNeuronWithMaxWordIndexCoverage/fullyActivatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
			if(neuronFirstOrLastWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, firstLayer, *partiallyActivatedNeuronWithMaxWordIndexCoverage))
			{
				//prevent partiallyActivatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
				*partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
				cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: partiallyActivatedNeuronWithMaxWordIndexCoverage" << endl;
				//exit(EXIT_ERROR);
				#endif
				if(*fullyActivatedNeuronWithMaxWordIndexCoverage == NULL)
				{
					//fullyActivatedNeuronWithMaxWordIndexCoverage is not available for use (add low level POS word neurons directly to grammaticalSentenceNeuron; do not rely on a previously matched sequence)
					pass = false;					
				}
			}
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			//condition2: dont select a matched neuron partiallyActivatedNeuronWithMaxWordIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in partiallyActivatedNeuronWithMaxWordIndexCoverage equals the next unactivated component groupRef in partiallyActivatedNeuronWithMaxWordIndexCoverage
			if(*partiallyActivatedNeuronWithMaxWordIndexCoverage != NULL)
			{
				int lastActivatedIndex = INT_DEFAULT_VALUE;
				int firstUnactivatedIndex = INT_DEFAULT_VALUE;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
				bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, *partiallyActivatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
				#else
				identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, *partiallyActivatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex);
				bool partialActivationConfirmed = identifyComponentIndexFirstUnactivatedIndex(forwardPropogationSentenceData, *partiallyActivatedNeuronWithMaxWordIndexCoverage, &firstUnactivatedIndex);
				#endif
								
				if(partialActivationConfirmed)	//partial activation test required in case where partiallyActivatedNeuronWithMaxWordIndexCoverage is recorded by GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
				{
					//cout << "lastActivatedIndex = " << lastActivatedIndex << endl;
					//cout << "firstUnactivatedIndex = " << firstUnactivatedIndex << endl;
					//cout << "(*partiallyActivatedNeuronWithMaxWordIndexCoverage)->components.size() = " << (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->components.size() << endl;
					if(lastActivatedIndex == firstUnactivatedIndex)
					{
						cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
						exit(EXIT_ERROR);
					}
					GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActivatedComponent = ((*partiallyActivatedNeuronWithMaxWordIndexCoverage)->groupRef)->components[lastActivatedIndex];		//BAD: (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->components[lastActivatedIndex]->componentRef;
					GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstNonActivatedComponent = ((*partiallyActivatedNeuronWithMaxWordIndexCoverage)->groupRef)->components[firstUnactivatedIndex];	//BAD: (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->components[firstUnactivatedIndex]->componentRef;
					if(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)
					{
						//prevent partiallyActivatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
						*partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
						cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition2" << endl;
						//exit(EXIT_ERROR);
						#endif
						if(*fullyActivatedNeuronWithMaxWordIndexCoverage == NULL)
						{
							//fullyActivatedNeuronWithMaxWordIndexCoverage is not available for use (add low level POS word neurons directly to grammaticalSentenceNeuron; do not rely on a previously matched sequence)
							pass = false;					
						}
					}
				}
			}
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_SPLIT_OLD
			if(*partiallyActivatedNeuronWithMaxWordIndexCoverage != NULL)
			{
				bool passMinComponentsChecks = true;

				int numberOfActivatedComponents = (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->components.size();
				int numberOfComponents = (*partiallyActivatedNeuronWithMaxWordIndexCoverage)->groupRef->components.size();

				int firstActivatedIndexUnordered = INT_DEFAULT_VALUE;				
				int lastActivatedIndexUnordered = INT_DEFAULT_VALUE;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
				identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, *partiallyActivatedNeuronWithMaxWordIndexCoverage, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered);
				if(!((firstActivatedIndexUnordered == 0) || (firstActivatedIndexUnordered >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)))
				{
					passMinComponentsChecks = false;
				}
				if(!(lastActivatedIndexUnordered-firstActivatedIndexUnordered+1 >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS))
				{
					passMinComponentsChecks = false;
				}
				if(!((lastActivatedIndexUnordered == numberOfComponents-1) || (numberOfComponents-lastActivatedIndexUnordered >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)))
				{
					passMinComponentsChecks = false;
				}				
				#else
				firstActivatedIndexUnordered = 0;
				identifyComponentIndexLastActivatedIndexUnordered(forwardPropogationSentenceData, *partiallyActivatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndexUnordered);
				int numberOfInactivatedComponents = numberOfComponents - numberOfActivatedComponents;
				if(!(numberOfActivatedComponents >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS))
				{
					passMinComponentsChecks = false;
				}
				if(!(numberOfInactivatedComponents >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS))
				{
					passMinComponentsChecks = false;
				}
				#endif

				if(!passMinComponentsChecks)
				{
					pass = false;
				}
			}	
			#endif
		} 
		if(*fullyActivatedNeuronWithMaxWordIndexCoverage != NULL)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			//condition1: dont select a matched neuron partiallyActivatedNeuronWithMaxWordIndexCoverage/fullyActivatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
			if(neuronFirstOrLastWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, firstLayer, *fullyActivatedNeuronWithMaxWordIndexCoverage))
			{
				//prevent fullyActivatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
				*fullyActivatedNeuronWithMaxWordIndexCoverage = NULL;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
				cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: fullyActivatedNeuronWithMaxWordIndexCoverage" << endl;
				//exit(EXIT_ERROR);
				#endif
				if(*partiallyActivatedNeuronWithMaxWordIndexCoverage == NULL)
				{
					//partiallyActivatedNeuronWithMaxWordIndexCoverage is not available for use (add low level POS word neurons directly to grammaticalSentenceNeuron; do not rely on a previously matched sequence)
					pass = false;					
				}
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
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::neuronFirstOrLastWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
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
	
	if(((currentMaxWordIndexInSequence < firstLayer->size()) && (currentMaxWordIndexInSequence >= 0)) && ((nextWordIndexInSequence < firstLayer->size()) && (nextWordIndexInSequence >= 0)))
	{//boundary test
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMaxWordIndexInSequenceFirstLayerNeuron = (*firstLayer)[currentMaxWordIndexInSequence];
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* nextWordIndexInSequenceFirstLayerNeuron = (*firstLayer)[nextWordIndexInSequence];	
		if(currentMaxWordIndexInSequenceFirstLayerNeuron == nextWordIndexInSequenceFirstLayerNeuron)
		{
			result = true;
		}
	}
	if(((currentMinWordIndexInSequence < firstLayer->size()) && (currentMinWordIndexInSequence >= 0)) && ((previousWordIndexInSequence < firstLayer->size()) && (previousWordIndexInSequence >= 0)))
	{//boundary test
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*firstLayer)[currentMinWordIndexInSequence];
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*firstLayer)[previousWordIndexInSequence];	
		if(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)
		{
			//cout << "(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)" << endl;
			result = true;
		}
	}
	
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::verifyActivatedNeuronsAtLeastOneBasic(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* fullyActivatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	if(!((fullyActivatedNeuronWithMaxWordIndexCoverage == NULL) && (partiallyActivatedNeuronWithMaxWordIndexCoverage == NULL)))
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
//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxWordIndexCoverage (never fullyActivatedNeuronWithMaxWordIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentsFound, but all remaining components are active)
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactClass::identifyComponentIndexFirstUnactivatedIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() < groupOrig->components.size())
	{
		partialActivationConfirmed = true;
		
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*firstUnactivatedIndex = groupOrig->components.size()-1-numberOfActivatedComponents;
		}
		else
		{
			*firstUnactivatedIndex = numberOfActivatedComponents;
		}
	}
	
	return partialActivationConfirmed;
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
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = firstActivatedIndexUnordered;
			*firstUnactivatedIndex = firstActivatedIndexUnordered-1;
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
//unordred (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
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
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMaxWordIndex+1;
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
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		if(keepExecuting)
		{
			bool keepExecutingLocal = true;
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
				if(!wordIndexMinFound)
				{
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif


				}
				if(wordIndexMinFound)
				{
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
				}
			}
			else
			{
				//assume will find wordIndexMax match before wordIndexMin match when propagating up network - CHECKTHIS
				if(!wordIndexMaxFound)
				{
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				}
				if(wordIndexMaxFound)
				{
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
				}		
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
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
				if(keepExecutingLocal)
				{
				#endif
					if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, wordIndexMinFound, wordIndexMaxFound))
					{
						result = true;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
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
