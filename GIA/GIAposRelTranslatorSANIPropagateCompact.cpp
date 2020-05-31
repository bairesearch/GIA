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
 * File Name: GIAposRelTranslatorSANIPropagateCompact.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l1b 28-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIPropagateCompact.hpp"
#include "SHAREDvars.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
#include "GIAneuralNetworkOperations.hpp"
#include "ANNdisplay.hpp"
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_COMPACT

//static int newNeuronIndex;

#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
GIAposRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocalCompact;
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
static vector<GIAposRelTranslatorRulesGroupParseTree*> parseTreeGroupListCompact;
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
vector<GIAposRelTranslatorRulesGroupParseTree*>* GIAposRelTranslatorSANIPropagateCompactClass::getParseTreeGroupList()
{
	return &parseTreeGroupListCompact;
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
static int incrementalGenerationIndex;
void GIAposRelTranslatorSANIPropagateCompactClass::initialiseIncrementalGeneration()
{
	incrementalGenerationIndex = 0;
}
#endif

//if GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
bool GIAposRelTranslatorSANIPropagateCompactClass::defineFirstLayer(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
		
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		bool getFirstLayer = true;
		
		int w = i;

		GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalDataNOTUSED;
	
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSignalDataNOTUSED, forwardPropogationSentenceData, getFirstLayer))
		{
			result = false;
		}
	}
	
	return result;
}	
	
bool GIAposRelTranslatorSANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	GIAposRelTranslatorRulesGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAposRelTranslatorRulesGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance);
}

bool GIAposRelTranslatorSANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	GIAposRelTranslatorSANIForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, GIAposRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, GIAposRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	return result;
}


bool GIAposRelTranslatorSANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
	//cout << "GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE: topLevelParseTreeGroupLocalCompact = NULL" << endl;
	topLevelParseTreeGroupLocalCompact = NULL;
	#endif
	
	//sentence specific variables:
	GIAposRelTranslatorSANIPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	//forwardPropogationSentenceData->toplevelGroupActivationFound = false;	//now done by performPropagation
	//forwardPropogationSentenceData->performance = performance;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData->parserEnabled = parserEnabled;
	#endif
	/*
	//not used by GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData->isQuery = GIAposRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData->parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes = GIAposRelTranslatorRulesGroupTypes;
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes = GIAposRelTranslatorRulesGroupTypes;
	#endif
	
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{	
		GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = new GIAposRelTranslatorSANIForwardPropogationWordData();
		forwardPropogationSentenceData->forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	//OLD: defineFirstLayer(translatorVariables, forwardPropogationSentenceData, &firstLayer)
	
	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(performPropagationTest(translatorVariables, GIAposRelTranslatorRulesGroupTypes, forwardPropogationSentenceData))
	{
		result = true;
		//printBackpropParseTree(topLevelParseTreeGroupLocalCompact, 3);
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
		//cout << "GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE: *topLevelParseTreeGroup = topLevelParseTreeGroupLocalCompact;" << endl;
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocalCompact;
		#endif
	}
	#endif
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateCompactClass::executeTxtRelTranslatorNeuralNetworkPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{
		GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	if(result)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData->sentenceContents->size();
		#else
		*performance = 1;
		#endif
	}
	
	return result;
}
	





bool GIAposRelTranslatorSANIPropagateCompactClass::performPropagationTest(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	bool resetAllNeuronComponents = true;
	bool deinitialiseParseTreeGroupAfterFinish = true;
	int firstIndexInSequence = 0;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAposRelTranslatorSANIPropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);
	#endif
	
	if(performPropagation(translatorVariables, GIAposRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence))
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "toplevelGroupActivationFound: groupIndexes = " << GIAposRelTranslatorSANIPropagateOperations.printParseTreeGroupIndices(topLevelParseTreeGroupLocalCompact) << endl;
		#endif
		result = true;
	}
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	GIAposRelTranslatorSANIPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#endif
	
	return result;
}


bool GIAposRelTranslatorSANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
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
	return performPropagation(translatorVariables, GIAposRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
}


bool GIAposRelTranslatorSANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
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
	return performPropagation(translatorVariables, GIAposRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);

}

bool GIAposRelTranslatorSANIPropagateCompactClass::performPropagation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSentenceData->toplevelGroupActivationFound = false;
	
	if(resetAllNeuronComponents)
	{
		GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED
		GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAposRelTranslatorSANIPropagateLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	}
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
	GIAposRelTranslatorSANIPropagateOperations.initialiseParseTreeGroupList(GIAposRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
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
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR	
		cout << "\n\n \e[33m GIAposRelTranslatorSANIPropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
	
		//group specific variables:
		GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		#endif
		
		const bool getFirstLayer = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSentenceData, getFirstLayer))
		{

		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
		GIAposRelTranslatorSANIPropagateOperations.deinitialiseParseTreeGroupList(GIAposRelTranslatorRulesGroupTypes, &parseTreeGroupListCompact);
		#endif
	}
	
	if(forwardPropogationSentenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];

	GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationWordData->w = w;

	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
		
	if(!GIAposRelTranslatorSANIPropagateOperations.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
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
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		int wordPOStype = INT_DEFAULT_VALUE;
		bool pass = GIAposRelTranslatorSANIPropagateOperations.getWordPOStypeFromAmbiguousWord(currentWord, &wordPOStype);
		if(pass)
		{
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
			{
				result = false;
			}		
		}
	}
	#endif
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAposRelTranslatorSANIFormation.getInputGroupLayerSection(GIAposRelTranslatorSANIFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	

	forwardPropogationWordData->wordPOStype = wordPOStype;
	inputLayerGroup->wordPOStype = wordPOStype;

	if(getFirstLayer)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		(forwardPropogationSentenceData->firstLayer->at(w))->push_back(inputLayerGroup);
		#else
		forwardPropogationSentenceData->firstLayer->push_back(inputLayerGroup);
		#endif
	}
	else
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		cout << "GIAposRelTranslatorSANIPropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
		#endif
	
		GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not properly used by GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
		int layer = 0;
		propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup);
	}
			
	return result;
}	

		

bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << "group->groupName << group->groupIndex = " << group->groupIndex << " \e[0m" << endl;
	#endif
			
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops	//only required for recursion (not yet supported by GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR)
	{
		group->neuronPropagated = true;
	#endif
		
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network

	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifndef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
	
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
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
		
			int componentIndex = currentComponent->componentIndex;	
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA2
				GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);	
				cout << "2a: propagateWordThroughNetworkGroup: ownerGroup->groupIndex = " <<  ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
				#endif
				
				if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}
				/*
				if(currentComponent->semanticRelationReturnEntity)
				{
					ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
				}
				*/
	
				bool sequentialActivationFound = false;
				bool existingActivationFound = false;
				//TODO: determine if new/GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR group reset method sufficient; see GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFound))
				{
					result = true;
				}

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif
		#ifndef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFound)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3_: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", ownerGroup->groupIndex: " << ownerGroup->groupIndex << endl;	
	#endif
	
	
	GIAposRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	
	GIAposRelTranslatorRulesComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	bool missingStartComponentsFound = false;
	bool missingOrVariableStartComponentFound = false;
	bool variableStartComponentFound = false;	//temp debug only
	int numberOfInactiveComponentsRemaining = 0;
	
	//cout << "\townerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
	bool componentWordConnectivityTestsPrelimResult = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(GIAposRelTranslatorSANIPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, false))
	{
		componentWordConnectivityTestsPrelimResult = true;
	}
	#endif
	
	if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent, existingActivationFound, &missingStartComponentsFound, &missingOrVariableStartComponentFound, &numberOfInactiveComponentsRemaining, ownerGroup, componentWordConnectivityTestsPrelimResult))
	{	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3_PRIMARY
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - missingStartComponentsFound = " << missingStartComponentsFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "ownerGroup->currentParseTreeGroupTemp->components.size() = " << ownerGroup->currentParseTreeGroupTemp->components.size() << endl;
		#endif
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupBackup = ownerGroup->currentParseTreeGroupTemp;
		GIApreprocessorPlainTextWord* previousActiveComponentActiveWordRecordBackup = NULL;
		if(previousActiveComponent != NULL)
		{
			previousActiveComponentActiveWordRecordBackup = previousActiveComponent->neuronComponentConnectionActiveWordRecord;
		}
		#endif
		
		//execute sequentialActivationConnectivityTests at start to save multiple executions [OPTIONAL];
		bool componentWordConnectivityTests = false;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS
		bool skipWordConnectivityTests = missingStartComponentsFound;
		#else
		bool skipWordConnectivityTests = false;
		#endif
		if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFound, skipWordConnectivityTests))
		{
			componentWordConnectivityTests = true;
			*sequentialActivationFound = true;
			//cout << "sequentialActivationConnectivityTests" << endl;
		}
		
			
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		verifyMissingOrVariableStartComponentFound(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFound, &missingStartComponentsFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		verifyMissingOrVariableStartComponentFoundAllowedWordIndices(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFound, &missingStartComponentsFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		#endif

		if(*sequentialActivationFound)
		{	
			//cout << "sequentialActivationFound: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
					
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentsFound = " << missingStartComponentsFound << ", (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) = " << (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound) << ", variableStartComponentFound = " << variableStartComponentFound << endl;	
			#else
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentsFound = " << missingStartComponentsFound << endl;				
			#endif
			#endif
		
			bool missingOrVariableStartComponentFoundStored = false;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
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
		else
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
			//cout << "restoring currentParseTreeGroupBackup" << endl;
			ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupBackup;
			if(previousActiveComponent != NULL)
			{
				previousActiveComponent->neuronComponentConnectionActiveWordRecord = previousActiveComponentActiveWordRecordBackup;
			}
			#endif
		}
	}

	#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(!activationSequenceCompleted)
		{
			if(*sequentialActivationFound)
			{	
				bool candidateCoveragePartial = true;
				updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentsFound, missingOrVariableStartComponentFound, variableStartComponentFound, candidateCoveragePartial);
			}
		}
	}
	#endif
		
	return result;
}




	
	
bool GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFound, const bool missingStartComponentsFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound)
{
	bool result = true;
		
	if(existingActivationFound)
	{		
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE	//GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
		cout << "******** resetGroupParseTreeGroupRef, groupIndex = " << ownerGroup->groupIndex << endl;
		#endif
		ownerGroup->neuronActive = false;	//required for GIA_POS_REL_TRANSLATOR_SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		GIAposRelTranslatorSANIPropagateOperations.resetGroupActivation(ownerGroup);	//OLD: component activations have already been reset by GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
		GIAposRelTranslatorSANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, false);
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_FREE_MEMORY
		parseTreeGroupListCompact.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
	//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	//}
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	currentComponent->neuronComponentConnectionActiveWordRecordMemory.push_back(currentComponent->neuronComponentConnectionActiveWordRecord);	//note neuronComponentConnectionActiveWordRecordMemory is stored in currentComponent not newParseComponent such that it can be retained across executions of resetGroupParseTreeGroupRef(ownerGroup, false)
	#endif
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	//create new parseTreeGroup
	//copied from GIAposRelTranslatorSANIPropagateInverseClass::generateRulesGroupTreeComponents;	
	GIAposRelTranslatorRulesComponentParseTree* newParseComponent = GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAposRelTranslatorRulesComponentParseTree(*GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component

	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
						
	GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;

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

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences/2)
		{
		#endif
			printSANInetworkSVG(translatorVariables, forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes);
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		}
		#endif
	}
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA4
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << ", newParseComponent->componentIndex = " << newParseComponent->componentIndex << ", ownerGroup->components.size() = " << ownerGroup->components.size() << ", groupIndex = " << ownerGroup->groupIndex << ", existingActivationFound = " << existingActivationFound << ", activationSequenceCompleted = " << activationSequenceCompleted << endl; 
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	

	#ifdef GIA_DEBUG_GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	GIAposRelTranslatorSANIPropagateOperations.verifyWordIndexCoverageIntegrity(forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData);
	#endif

	if(activationSequenceCompleted)
	{			
		ownerGroup->neuronActive = true;	//required for GIA_POS_REL_TRANSLATOR_SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WEIGHTS
		//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
		//}
		#endif
	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
		cout << "topLevelGroup0" << endl;
		#endif
		
		GIAposRelTranslatorSANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;
		
	
		bool candidateCoveragePartial = false;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
		cout << "activationSequenceCompleted" << endl;
		#endif
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)" << endl;
			#endif
			//consider relying on: activationPathWordCurrentParseTreeGroupOwner->missingOrVariableStartComponentFound/missingStartComponentsFound, rather than passing missingOrVariableStartComponentFound/missingStartComponentsFound
			updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentsFound, missingOrVariableStartComponentFound, variableStartComponentFound, candidateCoveragePartial);
		}
		
			
		bool topLevelGroup = false;
		//if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activationPathWordCurrentParseTreeGroupOwner))
		//{	
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ORIG
		if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, activationPathWordCurrentParseTreeGroupOwner, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
		{	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
		{			
		#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			int maxWeight;
			GIAposRelTranslatorSANIPropagateOperations.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, &maxWeight);
			if(maxWeight >= parseTreeMaxWeight)
			{
			#endif
				
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
					int topLevelParseTreeGroupWordIndexCoverage = GIAposRelTranslatorSANIPropagateOperations.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner);

					if(topLevelParseTreeGroupWordIndexCoverage == forwardPropogationSentenceData->sentenceContents->size())
					{
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
						int leafSize = GIAposRelTranslatorSANIPropagateOperations.countParseTreeLeafSize(activationPathWordCurrentParseTreeGroupOwner);
						int maxLeafSize = 0;
						int maxDepth = 0;
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
						GIAposRelTranslatorSANIPropagateOperations.getNeuralNetworkDepth(ownerGroup, &maxDepth);
						#else
						GIAposRelTranslatorSANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(ownerGroup, &maxLeafSize, &maxDepth);
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
						GIAposRelTranslatorSANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(ownerGroup);
						#endif
						#endif
						cout << "topLevelGroup" << endl;
						cout << "\tparseTree leafSize = " << leafSize << endl;
						cout << "\tneuralNetwork maxLeafSize = " << maxLeafSize << endl;
						cout << "\tneuralNetwork maxDepth = " << maxDepth << endl;
						#endif
						

						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION
						if(!GIAposRelTranslatorSANIPropagateOperations.updatePerformanceGroup(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
						{
							cerr << "GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION; GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent error: !GIAposRelTranslatorSANIPropagateOperations.updatePerformance" << endl;
							cerr << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
							cerr << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
							GIAposRelTranslatorSANIPropagateOperations.printParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
							exit(EXIT_ERROR);
						}
						#endif

						topLevelGroup = true;

						forwardPropogationSentenceData->finishedPassingSentenceWords = true;

						forwardPropogationSentenceData->toplevelGroupActivationFound = true;

						parseTreeMaxWeight = maxWeight;

						#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_REPLICATE_TOP_LEVEL_PARSE_TREE
						GIAposRelTranslatorSANIPropagateOperations.deleteParseTree(topLevelParseTreeGroupLocalCompact, 0);
						topLevelParseTreeGroupLocalCompact = GIAposRelTranslatorSANIPropagateOperations.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
						//printBackpropParseTree(topLevelParseTreeGroupLocalCompact, 3);
						#endif
						#else
						//OLD: topLevelParseTreeGroupLocalCompact = activationPathWordCurrentParseTreeGroupOwner;
						//copy currentParseTreeGroupTemp so it cant be overwritten;
						topLevelParseTreeGroupLocalCompact = GIAposRelTranslatorRules.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
						#endif
						#endif

						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
						cout << "topLevelGroup" << endl;
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
						cout << "parseTreeMaxWeight1 = " << parseTreeMaxWeight << endl;
						#endif
						cout << "topLevelParseTreeGroupLocalCompact->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
						printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
						#endif
					}
				}

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			}
			#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ORIG
		}	
		#endif
				
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(!missingOrVariableStartComponentFound)	//prevents propagation up if variableStartComponentFound
		{
		#endif	

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
			if(activationPathWordCurrentParseTreeGroupOwner->components.size() == 1)
			{
				cout << "GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent: activationSequenceCompleted; activationPathWordCurrentParseTreeGroupOwner->components.size() == 1" << endl;
				cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
				exit(EXIT_ERROR);
			}
			#endif

			if(!topLevelGroup)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = true;
				#endif
								
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
				{
					//result = true;
				}

				#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = false;
				#endif
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		}
		#endif
		//}
	}

	return result;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
bool GIAposRelTranslatorSANIPropagateCompactClass::verifyMissingOrVariableStartComponentFound(GIAtranslatorVariablesClass* translatorVariables, int layer, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//consider moving componentWordConnectivityTests test into GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady setting of missingOrVariableStartComponentFound
	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			if(!*missingStartComponentsFound)
			{
				if(*missingOrVariableStartComponentFound)
				{
					//guaranteed: testComponent->w != 0 [ie second+ component]

					*missingOrVariableStartComponentFound = false;	//the first component found was variable and correct
					if(numberOfInactiveComponentsRemaining == 0)
					{
						if(*activationSequenceCompleted != true)
						{
							cout << "GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper warning: (numberOfInactiveComponentsRemaining == 0) && (activationSequenceCompleted != true)" << endl;
						}
						*activationSequenceCompleted = true;	//this be already marked as true by GIAposRelTranslatorSANIPropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
					}
				}
			}
		}
		#endif
	}

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(*missingOrVariableStartComponentFound)
		{
			//guaranteed: (missingStartComponentsFound || missingStartComponentsFound) && testComponent->w != 0 [ie second+ component]

			bool firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = false; 
			GIAposRelTranslatorRulesGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxWordIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex/parseTreeMaxWordIndex in the case a variable first component is detected
			activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			//updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only (existing group activation parseTreeMaxWordIndex/parseTreeMinWordIndex will be disgarded as the first component is variable)			
			if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1)))	//ie if missing start component || variable start component	//ie no components have previously been activated (firstActiveComponentInGroup) || 1 component has previously been activated ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))
			{
				updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
				if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
				{
					firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = true;
				}
				else
				{
					*missingOrVariableStartComponentFound = false;
				}
			}
			else
			{
				cerr << "potential algorithm error; if(missingOrVariableStartComponentFound); the following should always be true: if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))" << endl;
				exit(EXIT_ERROR);
			}

			if(firstLastWordIndexTestHypotheticalWithoutVariableStartComponent)
			{
				if(*missingOrVariableStartComponentFound)
				{
					if(!(*sequentialActivationFound))
					{
						if((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))	//only 1 component has been activated
						{
							//variable first component detected
							//word indices do not align (ie !sequentialActivationFound), so assume first component was miscellaneously activated (mark it as variable) 

							*variableStartComponentFound = true;
							if(sequentialActivationConnectivityTests(translatorVariables, testComponent, ownerGroup, &activatedNeuronCandidate, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFound, variableStartComponentFound))	//CHECKTHIS
							{
								/*update sequentialActivationFound value->true under the assumption the first component is being modified with a new (variable) connection [therefore wordIndex alignment with second component is not required]. 
								This is only required for propagateWordThroughNetworkGroupComponent:updateActivatedNeuronWithMaxWordIndexCoverage to execute [ie the detection/designation of a fully activatedNeuronWithMaxWordIndexCoverage with variable first component]*/
								*sequentialActivationFound = true;	
								
								//ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound = true;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
							}
							else
							{
								*missingOrVariableStartComponentFound = false;
							}
						}
					}	
				}
			}
		}

		if(*missingOrVariableStartComponentFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound = true;
		}
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(missingStartComponentsFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingStartComponentFound = true;
		}
		#endif
		*/
	}
	#endif
	
	return result;
}
bool GIAposRelTranslatorSANIPropagateCompactClass::verifyMissingOrVariableStartComponentFoundAllowedWordIndices(GIAtranslatorVariablesClass* translatorVariables, int layer, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* testComponent, vector<GIAposRelTranslatorRulesComponentNeuralNetwork*>* components, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent, GIAposRelTranslatorRulesComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFound, bool* missingStartComponentsFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ORIG
	//moved GIA3j5aTEMP36 from old GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent:if(activationSequenceCompleted), verifyActivatedNeuronWithMaxWordIndexCoverage checks
	//CHECKTHIS - the activation tree being fed into the missing or variable start component must encapsulate the entire word index segment from the start of the global propagation required wordIndex (activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed):
	bool strictStartingCondition = false;
	if(*missingStartComponentsFound || (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound))	//or: || missingOrVariableStartComponentFound
	{
		strictStartingCondition = true;	
	}
	GIAposRelTranslatorRulesGroupParseTree activatedNeuronCandidate = *(ownerGroup->currentParseTreeGroupTemp);	//activatedNeuronCandidate is only used for verifyActivatedNeuronWithMaxWordIndexAllowed purposes
	if(firstActiveComponentInGroup)
	{
		activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		//updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only
	}
	updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
	if(!verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, strictStartingCondition))
	{
		(*sequentialActivationFound) = false;
	}	
	//#endif
	
	return result;
}
#endif




bool GIAposRelTranslatorSANIPropagateCompactClass::updateActivatedNeuronWithMaxWordIndexCoverage(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool missingStartComponentsFound, const bool missingOrVariableStartComponentFound, const bool variableStartComponentFound, const bool candidateCoveragePartial)
{
	bool result = true;
		
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS_SIMPLIFY
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;	//or missingOrVariableStartComponentFound?
	#else
	if(candidateCoveragePartial)
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	}
	#endif
	#endif
	
	bool testWordIndicesAllowed = true;
	bool testMinNumComponents = true;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
	if(activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet)
	{
		testWordIndicesAllowed = true;
		testMinNumComponents = false;
	}
	#endif
	
	if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, currentParseTreeGroupTemp, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound))
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
		cout << "verifyActivatedNeuronWithMaxWordIndexCoverage" << endl;
		#endif

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
		if(!missingStartComponentsFound)
		{
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
		if(!missingOrVariableStartComponentFound)
		{
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)
		{
		#endif
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)" << endl;
			#endif
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_SIZEABLE_SUBTREES
			if(!missingOrVariableStartComponentFound || GIAposRelTranslatorSANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) >= GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_SIZEABLE_SUBTREES_MIN_NEURONS)
			{
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH	
			int variableComponentMaxDepth = 0;
			int variableComponentMaxLeafSize = 0;
			if(missingOrVariableStartComponentFound)
			{
				GIAposRelTranslatorRulesComponentNeuralNetwork* startComponent = GIAposRelTranslatorSANIPropagateOperations.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true);
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				GIAposRelTranslatorSANIPropagateOperations.getNeuralNetworkDepth(startComponent, &variableComponentMaxDepth);
				#else
				GIAposRelTranslatorSANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(startComponent, &variableComponentMaxLeafSize, &variableComponentMaxDepth);
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
				GIAposRelTranslatorSANIPropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(startComponent);
				#endif
				#endif
			}
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
			cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSentenceData->variableFirstComponentMaxDepth << endl;
			cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;
			//cout << "GIAposRelTranslatorSANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << GIAposRelTranslatorSANIPropagateOperations.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
			#endif
			if(!missingOrVariableStartComponentFound || variableComponentMaxDepth == forwardPropogationSentenceData->variableFirstComponentMaxDepth)	//ORIGSPEC //intermediary: variableComponentMaxDepth < forwardPropogationSentenceData->variableFirstComponentMaxDepth
			{
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
			double randomProb = GIAposRelTranslatorSANIPropagateOperations.generateRandomNumber();
			if(randomProb > GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE_PROB)
			{
			#endif
				
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
				cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage" << endl;
				cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
				#endif
				//cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
				cout << "\tcandidateCoveragePartial = " << candidateCoveragePartial << endl;
				cout << "\tforwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
				#endif
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = currentParseTreeGroupTemp;
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = candidateCoveragePartial;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
				#endif
				/*
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
				forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageMissingStartComponent = activatedNeuronWithMaxWordIndexCoverageMissingStartComponentSet;	//NOTUSED
				#endif
				*/
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_RANDOMISE
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_MATCHING_DEPTH
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_REQUIRE_SIZEABLE_SUBTREES
			}
			#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
		}
		#endif
	}
	
	return result;
}



bool GIAposRelTranslatorSANIPropagateCompactClass::sequentialActivationConnectivityTests(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound, const bool skipWordConnectivityTests)
{	
	bool sequentialActivationFound = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationWordData, currentComponent, ownerGroup, activationPathWordCurrentParseTreeGroup, existingActivationFound))
	{
	#endif
		bool passBasicWordConnectivityTest = true;
		if(!skipWordConnectivityTests)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS		
			if(!existingActivationFound)
			{
				bool groupHasPreceedingComponent = false;
				if(currentParseTreeGroupTemp->components.size() > 0)
				{
					bool lastActiveComponentInParseTreeIndexCheck = false;
					
					//this check is presumably required in the event GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponent was previously called with missingStartComponentsFound in order to save forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage via updateActivatedNeuronWithMaxWordIndexCoverage 
					int lastActiveComponentInParseTreeIndex;
					GIAposRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = NULL;
					GIAposRelTranslatorRulesComponent* lastActiveComponent = NULL;
					if(forwardPropogationSentenceData->parseSentenceReverse)
					{
						lastActiveComponentInParseTreeIndex = 0;
						lastActiveComponentInParseTree = (currentParseTreeGroupTemp->components)[lastActiveComponentInParseTreeIndex];
						lastActiveComponent = lastActiveComponentInParseTree->componentRef;

						if(lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL)
						{
							cerr << "GIAposRelTranslatorSANIPropagateCompactClass::sequentialActivationConnectivityTests error: lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL" << endl;
							exit(EXIT_ERROR);
						}
						
						if(lastActiveComponentInParseTree->componentIndex == currentComponent->componentIndex+1)
						{
							if(lastActiveComponent->neuronComponentConnectionActive)
							{
								lastActiveComponentInParseTreeIndexCheck = true;
							}
						}
					}
					else
					{
						lastActiveComponentInParseTreeIndex = currentParseTreeGroupTemp->components.size() - 1;
						lastActiveComponentInParseTree = (currentParseTreeGroupTemp->components)[lastActiveComponentInParseTreeIndex];
						lastActiveComponent = lastActiveComponentInParseTree->componentRef;
						
						if(lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL)
						{
							cerr << "GIAposRelTranslatorSANIPropagateCompactClass::sequentialActivationConnectivityTests error: lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL" << endl;
							exit(EXIT_ERROR);
						}
						
						if(lastActiveComponentInParseTree->componentIndex == currentComponent->componentIndex-1)
						{
							if(lastActiveComponent->neuronComponentConnectionActive)
							{
								lastActiveComponentInParseTreeIndexCheck = true;
							}
						}
					}

					if(lastActiveComponentInParseTreeIndexCheck)
					{
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
					}
				}
				if(groupHasPreceedingComponent)
				{
					//cout << "groupHasPreceedingComponent" << endl;
					//DOESNTWORK if(componentWordConnectivityTests || *existingActivationFound) 	//if existingActivationFound: group activations will be reset so assume real ownerGroupParseTreeGroup->components.size() == 0 
			#endif
					passBasicWordConnectivityTest = false;
					if(GIAposRelTranslatorSANIPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound))
					{
						//cout << "passBasicWordConnectivityTest" << endl;
						passBasicWordConnectivityTest = true;
					}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS			
				}
			}
			#endif
			#endif
		}
		
		if(passBasicWordConnectivityTest)
		{			
			/*
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
			Note if variableStartComponentFound, then componentWordConnectivityTestsWrapper is not required to be reexecuted, since if the previous component was variable (ie miscellaneously activated) and components->size()==1, no word index connectivity test is required to be conducted. 
			If the first component was variable, but components->size() >= 2, then the previous component would be valid and would already have been tested when componentWordConnectivityTestsWrapper was executed the first time
			#endif
			*/
		
			sequentialActivationFound = true;

		}
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	}
	#endif	
	
	return sequentialActivationFound;
}	


	
void GIAposRelTranslatorSANIPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParent(GIAposRelTranslatorRulesGroupParseTree* parentGroup, GIAposRelTranslatorRulesComponentParseTree* parentComponent, GIAposRelTranslatorRulesGroupParseTree* childGroup)
{
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_GENERATE_PARSE_TREE
	if(parentComponent->parseTreeGroupRef != NULL)
	#else
	if(childGroup != NULL)
	#endif
	{	
		//has children
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_GENERATE_PARSE_TREE
		childGroup = parentComponent->parseTreeGroupRef;	//not required
		#endif
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
		}
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
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
		}
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
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

void GIAposRelTranslatorSANIPropagateCompactClass::updateParseTreeMaxMinWordIndexOfParentBasic(GIAposRelTranslatorRulesGroupParseTree* parentGroup, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorRulesGroupParseTree* childGroup)
{
	if(childGroup != NULL)
	{	
		//has children
		if(childGroup->parseTreeMaxWordIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = childGroup->parseTreeMaxWordIndex;
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
		}
		
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
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
		}
		if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
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



bool GIAposRelTranslatorSANIPropagateCompactClass::printBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAposRelTranslatorSANIPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAposRelTranslatorSANIPropagateOperations.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}









bool GIAposRelTranslatorSANIPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexCoverage(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronBaseline, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
					
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	bool candidateNeuronInCompleteHistory = false;
	if(activatedNeuronCandidate->groupRef->marked)
	{
		candidateNeuronInCompleteHistory = true;
	}
	#else
	bool candidateNeuronInCompleteHistory = false;
	for(int i=0; i<forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
	{
		if(activatedNeuronCandidate->groupRef == forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory[i])
		{
			candidateNeuronInCompleteHistory = true;	
		}
	}
	#endif
	if(!candidateNeuronInCompleteHistory)
	{
	#endif
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
		cout << "!candidateNeuronInCompleteHistory" << endl;
		#endif

		//if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)	//assumed true
		//{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		if(verifyNeverSplitGroupBetweenTwoIdenticalComponents(forwardPropogationSentenceData, activatedNeuronCandidate, candidateCoveragePartial))
		{
		#endif
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
			cout << "pass verifyNeverSplitGroupBetweenTwoIdenticalComponents" << endl;
			#endif
			
			int activatedNeuronCandidateCoverage = 0;
			int activatedNeuronCandidateMaxWeight = 0;
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			bool activatedNeuronCandidateMinNumComponentsTest = false;
			#endif
			if(activatedNeuronCandidate != NULL)
			{
				activatedNeuronCandidateCoverage = GIAposRelTranslatorSANIPropagateOperations.calculateCoverage(activatedNeuronCandidate);
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
				GIAposRelTranslatorSANIPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronCandidate, forwardPropogationSentenceData, &activatedNeuronCandidateMaxWeight);
				#endif
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
				if(activatedNeuronCandidate->components.size() >= GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)
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
					activatedNeuronBaselineCoverage = GIAposRelTranslatorSANIPropagateOperations.calculateCoverage(activatedNeuronBaseline);
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					//activatedNeuronBaselineMaxWeight = activatedNeuronBaselineCoverage->parseTreeMaxWeight;
					GIAposRelTranslatorSANIPropagateOperations.calculatePerformanceWeightOfParseTree(activatedNeuronBaseline, forwardPropogationSentenceData, &activatedNeuronBaselineMaxWeight);
					#endif
				}
			}

			bool firstLastWordIndexTest = false;
			if(testWordIndicesAllowed)
			{	
				bool strictStartingCondition = true;
				firstLastWordIndexTest = verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activatedNeuronCandidate, strictStartingCondition);
			}
			else
			{
				firstLastWordIndexTest = true;
			}

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
			{
			#endif
				if(firstLastWordIndexTest)
				{
					bool passBaselineComparisonTests = true;
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{
						passBaselineComparisonTests = false;
						
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;		
						#endif
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;
						#endif

						bool passBaselineCoverageTests = false;
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_PREFERENCE_THEM
						if(candidateCoverageVariable && !candidateCoveragePartial)
						{	
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;
							}
						}
						#endif
						if(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)
						{
							passBaselineCoverageTests = true;
						}
						#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial && !(candidateCoveragePartial) && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow fully active activatedNeuronCandidate to override partially active activatedNeuronBaseline, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_FIRST_COMPONENTS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent && !(candidateCoverageVariable))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableStartComponentFound to override activatedNeuronBaseline with missingOrVariableStartComponentFound, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif

						if(passBaselineCoverageTests)
						{
							#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_FIX
							if((activatedNeuronCandidateCoverage != activatedNeuronBaselineCoverage) || (activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight))
							#else
							if(activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight)
							#endif
							{
								//GIAposRelTranslatorSANIPropagateOperations.setPerformanceWeightOfParseTree(activatedNeuronBaselineCoverage, maxWeight);
								//activatedNeuronBaselineCoverage->parseTreeMaxWeight = maxWeight;
							#endif
								passBaselineComparisonTests = true;
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							}
							#endif
						}

					}
					if(passBaselineComparisonTests)
					{
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						result = true;
					}

				}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			}
			#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		}
		#endif
		//}
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	}
	#endif
		
	return result;
}
	
bool GIAposRelTranslatorSANIPropagateCompactClass::verifyActivatedNeuronWithMaxWordIndexAllowed(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition)
{
	bool result = false;

	int activatedNeuronCandidateMinWordIndex = activatedNeuronCandidate->parseTreeMinWordIndex;
	int activatedNeuronCandidateMaxWordIndex = activatedNeuronCandidate->parseTreeMaxWordIndex;

	bool firstWordIndexTest = false;
	bool lastWordIndexTest = false;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
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
			}
		}
		else
		{
			if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
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
		

bool GIAposRelTranslatorSANIPropagateCompactClass::verifyActivatedNeuronsAtLeastOne(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, GIAposRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)
		{			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
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

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
bool GIAposRelTranslatorSANIPropagateCompactClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial)
{
	bool result = false;
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
	cout << "verifyNeverSplitGroupBetweenTwoIdenticalComponents START" << endl;
	#endif
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
		cout << "verifyActivatedNeuronsAtLeastOneBasic" << endl;
		#endif
		
		//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
		if(neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
		{
			//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
			pass = false;
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_LIMIT_NUM_COMPONENTS
			cout << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence"<< endl;		
			#endif
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			cout << "GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: activatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoveragePartial = " << activatedNeuronWithMaxWordIndexCoverage->activatedNeuronWithMaxWordIndexCoveragePartial << endl;
			//exit(EXIT_ERROR);
			#endif
		}
			
		if(candidateCoveragePartial)
		{
			//condition2: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in activatedNeuronWithMaxWordIndexCoverage equals the next unactivated component groupRef in activatedNeuronWithMaxWordIndexCoverage
			int lastActivatedIndex = INT_DEFAULT_VALUE;
			int firstUnactivatedIndex = INT_DEFAULT_VALUE;
			bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
			
			if(partialActivationConfirmed)	//OLD: partial activation test required in case where activatedNeuronWithMaxWordIndexCoverage is recorded by GIAposRelTranslatorSANIPropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
			{
				if(lastActivatedIndex == firstUnactivatedIndex)
				{
					cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
					exit(EXIT_ERROR);
				}
				GIAposRelTranslatorRulesComponentNeuralNetwork* lastActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[lastActivatedIndex];		//BAD: activatedNeuronWithMaxWordIndexCoverage->components[lastActivatedIndex]->componentRef;
				GIAposRelTranslatorRulesComponentNeuralNetwork* firstNonActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[firstUnactivatedIndex];	//BAD: activatedNeuronWithMaxWordIndexCoverage->components[firstUnactivatedIndex]->componentRef;
				if(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)
				{
					//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
					pass = false;	
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
					cout << "GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition2" << endl;
					#endif
				}
			}
		}
		
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateCompactClass::neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
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
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	cout << "\n\n activatedNeuronWithMaxWordIndexCoverage->components.size() = " << activatedNeuronWithMaxWordIndexCoverage->components.size() << endl;	
	cout << "currentMaxWordIndexInSequence = " << currentMaxWordIndexInSequence << endl;
	cout << "nextWordIndexInSequence = " << nextWordIndexInSequence << endl;
	cout << "currentMinWordIndexInSequence = " << currentMinWordIndexInSequence << endl;
	cout << "previousWordIndexInSequence = " << previousWordIndexInSequence << endl;
	#endif
	
	if(((currentMaxWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMaxWordIndexInSequence >= 0)) && ((nextWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (nextWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>* currentMaxWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
		vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>* nextWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];			
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			GIAposRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];		
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);
		#else
			GIAposRelTranslatorRulesGroupNeuralNetwork* currentMaxWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
			GIAposRelTranslatorRulesGroupNeuralNetwork* nextWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];	
		#endif
			if(currentMaxWordIndexInSequenceFirstLayerNeuron == nextWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	if(((currentMinWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMinWordIndexInSequence >= 0)) && ((previousWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (previousWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>* currentMinWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		vector<GIAposRelTranslatorRulesGroupNeuralNetwork*>* previousWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			GIAposRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];			
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);		
		#else		
		GIAposRelTranslatorRulesGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		GIAposRelTranslatorRulesGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];	
		#endif
			if(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	
	return result;
}
#endif

bool GIAposRelTranslatorSANIPropagateCompactClass::verifyActivatedNeuronsAtLeastOneBasic(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	if(activatedNeuronWithMaxWordIndexCoverage != NULL)
	{
		result = true;
	}
	
	return result;
}



bool GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexLastActivatedIndex(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
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

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//CHECKTHIS;
//ordered (do not assume normal order): "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentsFound, but all remaining components are active)
bool GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int firstActivatedIndexUnordered;
	int lastActivatedIndexUnordered;
	if(identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, parseTreeGroupNeuron, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered))
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

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
bool GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexFirstAndLastActivatedIndexUnordered(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex)
{
	bool result = false;
	
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		GIAposRelTranslatorRulesComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		*firstActivatedIndex = firstActivatedComponent->componentIndex;
		*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		
		result = true;
	}
	
	return result;
}
#else

//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxWordIndexCoverage (never fullyActivatedNeuronWithMaxWordIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentsFound, but all remaining components are active)
bool GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	GIAposRelTranslatorRulesGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
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
bool GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		GIAposRelTranslatorRulesComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		int firstActivatedIndex = firstActivatedComponent->componentIndex;
		if(firstActivatedIndex == 0)
		{
			result = true;
			*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		}
		else
		{
			cerr << "!GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION: GIAposRelTranslatorSANIPropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered error:  (firstActivatedIndex != 0)" << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}
#endif



#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
bool GIAposRelTranslatorSANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorRulesComponentNeuralNetwork* component, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFound)
{
	bool result = false;
	
	GIAposRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	if((ownerGroupParseTreeGroup->components.size() == 0) || existingActivationFound)
	{		
		int firstWordIndexInSequence;
		//expect to find upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent if current group->parseTreeMinWordIndex/parseTreeMaxWordIndex doesnt encapsulate firstWordIndexInSequence

		int wordIndexMax = -1;
		int wordIndexMin = REALLY_LARGE_INT;
		GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMin;
		GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax;
		int wordIndexMinMin;
		int wordIndexMaxMax;
		int wordIndexMinToFindAbove;		
		int wordIndexMaxToFindAbove;
		int wordIndexLastAboveEffectiveAfterGroupReset = INT_DEFAULT_VALUE;

		bool execute = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "\nGIAposRelTranslatorSANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper start:" << endl;
			#endif
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
			#endif

			parseTreeGroupToFindWordIndexMax = activationPathWordCurrentParseTreeGroup;
			wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMax != NULL)
			{
				if(!GIAposRelTranslatorSANIPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
				{

				}
			}
			
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
			cout << "wordIndexMax = " << wordIndexMax << endl;
			#endif
			
			wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);
			
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
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
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = 0;
			#endif

			parseTreeGroupToFindWordIndexMin = activationPathWordCurrentParseTreeGroup;
			wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMin != NULL)
			{
				if(!GIAposRelTranslatorSANIPropagateOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
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
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
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
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
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

//bool GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool findMaxOrMinWordIndex, const int wordIndexMaxOrMinToFind, int level, GIAposRelTranslatorRulesGroupNeuralNetwork* group)
bool GIAposRelTranslatorSANIPropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, GIAposRelTranslatorRulesGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound)
{
	bool result = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
	bool keepExecuting = true;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
	wordIndexMinFound = false;
	wordIndexMaxFound = false;
	#endif
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		if(keepExecuting)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
			bool keepExecutingLocal = true;
			#endif
		#endif
			
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
			#endif

			GIAposRelTranslatorRulesGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
			int parseTreeMinWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMinWordIndex;
			int parseTreeMaxWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMaxWordIndex;

			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndex = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndex = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif


			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
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

			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndexEffectiveAfterGroupReset = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndexEffectiveAfterGroupReset = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif

			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				//assume will find wordIndexMin match before wordIndexMax match when propagating up network - CHECKTHIS
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMinFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				if(wordIndexMinFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				#endif
			}
			else
			{
				//assume will find wordIndexMax match before wordIndexMin match when propagating up network - CHECKTHIS
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMaxFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				if(wordIndexMaxFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
				#ifndef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				#endif
			}


			if(wordIndexMinFound && wordIndexMaxFound)
			{
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "GIAposRelTranslatorSANIPropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - (wordIndexMinFound && wordIndexMaxFound); result == true" << endl;
				#endif
				result = true;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
				keepExecuting = false;
				#endif
			}
			else
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				if(keepExecutingLocal)
				{
				#endif
					if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, wordIndexMinFound, wordIndexMaxFound))
					{
						result = true;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				}
				#endif
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		}
		#endif
	}
	
	return result;
}

#endif


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool GIAposRelTranslatorSANIPropagateCompactClass::printSANInetworkSVG(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	string outputTextSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + "IncrementalGeneration" + SHAREDvars.convertIntToString(incrementalGenerationIndex) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	cout << "GIA_POS_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK: outputTextSVGFileName = " << outputTextSVGFileName << endl;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	GIAposRelTranslatorSANIFormation.createANNconnectivity(GIAposRelTranslatorRulesGroupTypes);
	#endif
	
	GIAneuralNetworkOperationsClass().generateNeuralNetFromGIAposRelTranslatorNet(translatorVariables);	//generate GIA NLP neural network

	int rasterImageWidth = 0;
	int rasterImageHeight = 0;
	bool ANNdisplayInOpenGL = false;
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = true;
	string ANNoutputSVGFileName = outputTextSVGFileName;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseSprites = true;
	
	//int widthSVG = 3840;	//1920
	//int heightSVG = 2160;	//1080	//1400
	int widthSVG = 14400;
	int heightSVG = 2430;
	
	ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight, widthSVG, heightSVG);

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	GIAposRelTranslatorSANIFormation.createANNconnectivityReset(GIAposRelTranslatorRulesGroupTypes);
	#endif
	
	incrementalGenerationIndex++;
	
	return result;
}
#endif


#endif
