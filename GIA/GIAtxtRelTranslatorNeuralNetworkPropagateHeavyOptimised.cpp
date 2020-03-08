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
 * File Name: GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j6a 10-January-2020
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_OPTIMISED


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocal;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
static bool maxIteration;
#endif
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocal = NULL;
	#endif
				
	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, &forwardPropogationSentenceData);
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	//forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData.isQuery = GIAtxtRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData.GIAtxtRelTranslatorRulesGroupTypes = GIAtxtRelTranslatorRulesGroupTypes;
	
	//memory clean forwardPropogationSentenceData.forwardPropogationWordDataArray [from previous POS permutation];
	for(int i=0; i<forwardPropogationSentenceData.forwardPropogationWordDataArray.size(); i++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData.forwardPropogationWordDataArray[i];
		delete forwardPropogationWordData;
	}
	forwardPropogationSentenceData.forwardPropogationWordDataArray.clear();

	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = new GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.resize(sentenceContents->size());
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());

	//GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	
	//GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		bool passThrough = false;
		int layer = 0;
		for(int w=0; w<sentenceContents->size(); w++)
		{
			if(!propagateWordThroughNetworkWordGroupIntro(translatorVariables, w, &forwardPropogationSentenceData, layer))
			{
				result = false;
			}
		}
		
		/*
		cout << "propagateWordThroughNetworkWordGroupIntro{} finished" << endl;
		exit(EXIT_ERROR);
		*/
		if(!propagateWordThroughNetworkGroupIntro(translatorVariables, &forwardPropogationSentenceData))
		{
			result = false;
		}
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		//cout << "topLevelParseTreeGroupLocal" << endl;
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "topLevelParseTreeGroupLocal->groupName = " << topLevelParseTreeGroupLocal->groupName << endl;
		#endif	
		#endif	
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData.performance;
		//cout << "forwardPropogationSentenceData.performance = " << forwardPropogationSentenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		//cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
		//exit(EXIT_ERROR);
	}
	else
	{
		*performance = 0;		
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		int performanceNOTUSED = 0;
		bool print = false;
		bool performancePreprocess = false;
		bool calculateMaxWeight = false;
		int maxWeightNOTUSED = 0;
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropParseTree(topLevelParseTreeGroupLocal, 1, print, performancePreprocess, &performanceNOTUSED, NULL, calculateMaxWeight, &maxWeightNOTUSED);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents
	}
	#endif
	for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
	{
		vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray[w];
		vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray[w];
		if(activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size())
		{	
			cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork error: activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size()" << endl;
			exit(EXIT_ERROR);
		} 
		for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 
		{
			GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
			GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetGroupParseTreeGroupRef(activationPathWordFirstActivationMemoryGroup, activationPathWordFirstParseTreeGroup, true);
		}
		activationPathWordFirstActivationMemoryGroupArray.clear();
		activationPathWordFirstParseTreeGroupArray.clear();
	}
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.clear();
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.clear();
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetNeuronBackprop(topLevelParseTreeGroupLocal, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	}
	#endif
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		cout << "w = " << w << endl;
		#endif
	}
	
		
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;

	/*
	more general algorithm;
	(1) start at every w in sentence
		propagate word up network as high as it goes
		store the parse tree group (and memory activation group) in memory
			store also the num words n encapsulated by the parse tree group (for efficiency)
		NO: parse to the right and generate sets of parse groups (limited by previousWordFound), including single words
	*/
		
	//GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup

	GIApreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << GIApreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif
	
	//prepare word specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	forwardPropogationWordData->w = w;
	//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
	
	//group specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
	forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
	#endif
				
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));
		
	
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
				if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
				{
					result = true;
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
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_WILDCARDS
		for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{		
			//cout << "isMidSentenceUppercaseWordLikelyProperNoun" << endl;
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
			{
				result = true;
			}
		}
		#endif
		#endif	
	}
	#endif
	
			
	
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}
	}
	#else
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}
	}
	#endif
		
	for(unordered_map<string, GIAtxtRelTranslatorRulesGroupNeuralNetwork*>::iterator iter1 = GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionTokensLayerMap()->begin(); iter1 != GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionTokensLayerMap()->end(); iter1++)
	{
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayer = iter1->second;
		for(int i=0; i<currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = (currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList)[i];
			for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList.size(); i++)
			{
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = (currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList)[i];
				for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList.size(); i++)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = (currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList)[i];
					if(currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName == wordLowerCase)	//NB this implementation is synced with GIAtxtRelTranslatorRulesClass::isClassTag: assume tokens always comprise wordLowerCase
					{
						string GIAtokenLayerClassName = currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName;
						int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
						if(SHAREDvars.textInTextArray(GIAtokenLayerClassName, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
						{
							forwardPropogationWordData->wordPOStype = wordPOStype;
						}
						else
						{
							cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
						{
							result = true;
						}
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))	//what is this for?
						{
							result = true;
						}
					}
				}
			}
		}
	}
		
	return result;
}

	
bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
	{
		result = true;
	}
	
	return result;
}	
	
bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "activationPathWordFirstParseTreeGroupNext->inputGroupString" << endl;
	
	GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = NULL;
	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
	generateMemoryActiveGroup(inputLayerGroup, inputLayerGroup, inputLayerGroup, &activationPathWordFirstActivationMemoryGroup, &activationPathWordFirstParseTreeGroup, false);
	/*
	GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = GIAtxtRelTranslatorRules.copyGroup(inputLayerGroup);
	activationPathWordFirstActivationMemoryGroup->groupOrig = inputLayerGroup;
	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = new GIAtxtRelTranslatorRulesGroupParseTree(*inputLayerGroup);
	*/
	
	forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w].push_back(activationPathWordFirstActivationMemoryGroup);
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w].push_back(activationPathWordFirstParseTreeGroup);
	
	activationPathWordFirstActivationMemoryGroup->layer = layer;
	activationPathWordFirstActivationMemoryGroup->numberWordsInSet = 1;
	activationPathWordFirstActivationMemoryGroup->forwardPropogationSignalData = *forwardPropogationSignalData;
	activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData = *forwardPropogationWordData;	//(forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	activationPathWordFirstActivationMemoryGroup->neuronActive = true;
	activationPathWordFirstActivationMemoryGroup->wordGroupNeuron = true;
	activationPathWordFirstActivationMemoryGroup->inputGroupString = true;
		//TODO: ensure activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData is always set correctly
		
	return result;
}				
	

bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	/*
	more general algorithm;
	(2) repeat until found top level group with all encapsulated words:
		start at every w in sentence
			for each existing parseTreeGroup stored at w
				for each adjacent parseTreeGroup available (that does not skip an encapsulate word), ie stored at w+n (where n is number of words in the left parse group)
					activate their appropriate top level components in the memory group
					see if the second activated group propagates up into the second
	*/
	
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = forwardPropogationSentenceData->sentenceContents;

	int maxIterationIndex = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_MAX_ITERATIONS;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	parseTreeMaxWeight = 0;
	#endif
	maxIteration = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
	bool limitedIterations = false;
	#endif
	#endif
	bool foundTopLevelGroup = false;
	bool stillGeneratingSets = true;
	int connectIterationIndex = 1;
	while(!foundTopLevelGroup && stillGeneratingSets)	//repeat until found top level group with all encapsulated words:
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		//GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "\nconnectIterationIndex = " << connectIterationIndex << endl;
		#endif
			
		for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
		{
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
			cout << "w = " << w << endl;
			#endif
			*/
			
			vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w];
			vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_STRICT_ITERATION_INDICES
			int groupArraySize = activationPathWordFirstActivationMemoryGroupArray.size();
			for(int i=0; i<groupArraySize; i++)
			#else
			for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 	//for each existing parseTreeGroup stored at w
			#endif
			{
				GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
				GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];
				
				int wNext = w + activationPathWordFirstActivationMemoryGroup->numberWordsInSet;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
				cout << "w = " << w << endl;
				cout << "wNext = " << wNext << endl;
				#endif
				
				if(wNext < sentenceContents->size())
				{	
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroup->layer);
					cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroup: " <<  activationPathWordFirstActivationMemoryGroup->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroup->groupName << endl;		
					#endif

					//activate their appropriate top level components in the memory group
					int layer = activationPathWordFirstActivationMemoryGroup->layer;
					GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = &(activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData);
					//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
					GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData = &(activationPathWordFirstActivationMemoryGroup->forwardPropogationSignalData);
					GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData forwardPropogationActivationPointData;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray = &(activationPathWordFirstActivationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray);
					forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray = &(activationPathWordFirstParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray);
					#else
					vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
					vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArray;	
					forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray = &activationPathWordFirstActivationMemoryGroupActivationPointArray;
					forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray = &activationPathWordFirstParseTreeGroupActivationPointArray;
					#endif
					forwardPropogationSentenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointData;
					forwardPropogationActivationPointData.generateActivationPointArray = true;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					if(!(activationPathWordFirstActivationMemoryGroup->pointArrayGenerated))
					{
					#endif	
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!activationPathWordFirstActivationMemoryGroup->firstLevelActivationPointAdded)
						{
							activationPathWordFirstActivationMemoryGroup->firstLevelActivationPointAdded = true;
						#endif
							//add the first level activationPathWordFirstActivationMemoryGroup to activationPathWordFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
							forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(activationPathWordFirstActivationMemoryGroup);
							forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordFirstParseTreeGroup);
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif

						if(activationPathWordFirstActivationMemoryGroup->neuronActive)
						{
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
							activationPathWordFirstActivationMemoryGroup->pointArrayGenerated = true;
							#endif
							
							GIAtxtRelTranslatorRulesGroupNeuralNetwork* activationPathWordFirstActivationMemoryGroupBase = activationPathWordFirstActivationMemoryGroup;
							if(propagateWordThroughNetworkGroup(translatorVariables, activationPathWordFirstActivationMemoryGroupBase, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
							{
								result = true;
							}
						}

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
					}
					#endif


					vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[wNext];
					vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[wNext];
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_STRICT_ITERATION_INDICES
					int groupArrayNextSize = activationPathWordFirstActivationMemoryGroupArrayNext.size();
					for(int i2=0; i2<groupArrayNextSize; i2++)
					#else
					for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupArrayNext.size(); i2++) 	//for each adjacent parseTreeGroup available (that does not skip an encapsulate word), ie stored at w+n (where n is number of words in the left parse group)
					#endif
					{
						GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext = activationPathWordFirstActivationMemoryGroupArrayNext[i2];
						GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupNext = activationPathWordFirstParseTreeGroupArrayNext[i2];

						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroupNext->layer);
						cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroupNext: " <<  activationPathWordFirstActivationMemoryGroupNext->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupNext->groupName << endl;		
						#endif

						//activate their appropriate top level components in the memory group
						int layerNext = activationPathWordFirstActivationMemoryGroupNext->layer;
						GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataNext = &(activationPathWordFirstActivationMemoryGroupNext->forwardPropogationWordData);
						//cout << "forwardPropogationWordDataNext->w = " << forwardPropogationWordDataNext->w << endl;
						GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalDataNext = &(activationPathWordFirstActivationMemoryGroupNext->forwardPropogationSignalData);
						GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData forwardPropogationActivationPointDataNext;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray = &(activationPathWordFirstActivationMemoryGroupNext->activationPathWordFirstActivationMemoryGroupActivationPointArray);
						forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray = &(activationPathWordFirstParseTreeGroupNext->activationPathWordFirstParseTreeGroupActivationPointArray);
						#else
						vector<GIAtxtRelTranslatorRulesGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						vector<GIAtxtRelTranslatorRulesGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArrayNext;	
						forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray = &activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray = &activationPathWordFirstParseTreeGroupActivationPointArrayNext;
						#endif
						forwardPropogationSentenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointDataNext;
						forwardPropogationActivationPointDataNext.generateActivationPointArray = true;

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!(activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated))
						{
						#endif
							if(activationPathWordFirstActivationMemoryGroupNext->neuronActive)
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated = true;
								#endif

								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								if(!activationPathWordFirstActivationMemoryGroupNext->firstLevelActivationPointAdded)
								{
									activationPathWordFirstActivationMemoryGroupNext->firstLevelActivationPointAdded = true;
								#endif
									//if(activationPathWordFirstActivationMemoryGroupNext->inputGroupString)
									//{	
									//add the first level activationPathWordFirstActivationMemoryGroupNext to activationPathWordFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
									forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(activationPathWordFirstActivationMemoryGroupNext);	//OLD: add original unpropagated group to forwardPropogationActivationPointDataNext (in case of string)
									forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordFirstParseTreeGroupNext);
									//}
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
								}
								#endif
								
								if(propagateWordThroughNetworkGroup(translatorVariables, activationPathWordFirstActivationMemoryGroupNext, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSentenceData, layerNext, activationPathWordFirstParseTreeGroupNext))
								{
									result = true;
								}
							}
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif


						if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
						{
							//see if the second activated set point propagates up immediately into the first				
							if(connectToPreviousActivationGroup(translatorVariables, activationPathWordFirstActivationMemoryGroup, activationPathWordFirstActivationMemoryGroupNext, &forwardPropogationActivationPointData, &forwardPropogationActivationPointDataNext, forwardPropogationSignalDataNext, forwardPropogationWordData, forwardPropogationWordDataNext, forwardPropogationSentenceData, layerNext))
							{
								result = true;
							}
						}
						
						#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArrayNext.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
						{
							GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationPathWordFirstActivationMemoryGroupActivationPointArrayNext[i];
							GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = activationPathWordFirstParseTreeGroupActivationPointArrayNext[i];
							if(!activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
							{
								GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);
								if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
								{
									GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
								}
							}
						}
						#endif
					}
				}
				
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
				for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
				{
					GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
					GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = activationPathWordFirstParseTreeGroupActivationPointArray[i];
					if(!activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
					{
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);
						if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
						{
							GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
						}
					}
				}
				#endif
			}
		}
		
		if(forwardPropogationSentenceData->finishedPassingSentenceWords)
		{
			stillGeneratingSets = false;
		}
		if(connectIterationIndex >= maxIterationIndex)
		{
			stillGeneratingSets = false;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
		if(maxIteration)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
			maxIteration = false;
			if(!limitedIterations)
			{
				limitedIterations = true;
				maxIterationIndex = connectIterationIndex+GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK_OFFSET;	//allow 1 more iteration once maxIteration detected
			}
			#else
			stillGeneratingSets = false;
			#endif
		}
		#endif
							
		connectIterationIndex++;
	}
	
	return result;
	
}

//CHECKTHIS; check parameter forwardPropogationWordData
bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::connectToPreviousActivationGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	for(int i1=0; i1<forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i1++)
	{
		GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i1];
		GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstParseTreeGroupActivationPointArray)[i1];

		if(activationPathWordFirstActivationMemoryGroupNextPoint->neuronActive)	//added 3g8aTEMP22
		{			
			for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupNextPoint->ANNfrontComponentConnectionList.size(); i2++)
			{
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponentNext = (activationPathWordFirstActivationMemoryGroupNextPoint->ANNfrontComponentConnectionList)[i2];
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupNext = currentComponentNext->ownerGroup;
				
				for(int i3=0; i3<forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i3++)
				{
					GIAtxtRelTranslatorRulesGroupActivationMemory* activationPathWordFirstActivationMemoryGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i3];
					GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray)[i3];
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig = activationPathWordFirstActivationMemoryGroupPoint->groupOrig;

					if(groupOrig == ownerGroupNext)
					{
						//found matching group names between set and next set (see if their memory groups are connected)

						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupPoint: " <<  activationPathWordFirstActivationMemoryGroupPoint->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupPoint->groupName << endl;		
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupNextPoint: " <<  activationPathWordFirstActivationMemoryGroupNextPoint->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupNextPoint->groupName << endl;		
						#endif

						GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponentNext = currentComponentNext;
						int componentIndex = currentComponentNext->componentIndex;
						int ownerComponentIndex = componentIndex;
						if(currentComponentNext->isSubcomponent)
						{
							ownerComponentNext = currentComponentNext->ownerComponent;
							ownerComponentIndex = ownerComponentNext->componentIndex;
						}

						GIAtxtRelTranslatorRulesGroupActivationMemory* ownerGroup = activationPathWordFirstActivationMemoryGroupPoint;
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent = ownerGroup->components[ownerComponentIndex];
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = ownerComponent;
						if(currentComponentNext->isSubcomponent)
						{
							currentComponent = ownerComponent->subComponents[componentIndex];
						}

						forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup = true;
						forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray = false;	//CHECKTHIS
						forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = forwardPropogationWordData->w;
						
						
						//forwardPropogationActivationPointDataNext->connectToPreviousActivationGroup = true;
						
						if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
						{
							if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup->groupOrig, currentComponent, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroupNextPoint, activationPathWordFirstParseTreeGroupPoint))
							{
								result = true;

								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
								cout << "connectToPreviousActivationGroup: propagateWordThroughNetworkGroupSelect passed" << endl;
								#endif

								//FUTURE: ideally this should be done (set correctly originally) in GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent:
								GIAtxtRelTranslatorRulesGroupActivationMemory* newGroup = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationWordData->w].back();	//forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW
								newGroup->numberWordsInSet = activationPathWordFirstActivationMemoryGroup->numberWordsInSet + activationPathWordFirstActivationMemoryGroupNext->numberWordsInSet;	//CHECKTHIS
								newGroup->forwardPropogationWordData.w = forwardPropogationWordData->w;
								
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
								cout << "\tnewGroup: " <<  newGroup->groupTypeName << ":" << newGroup->groupName << endl;	
								cout << "\tnewGroup->pointArrayGenerated: " <<  newGroup->pointArrayGenerated << endl;
								cout << "\tnewGroup->numberWordsInSet = " << newGroup->numberWordsInSet << endl;
								cout << "\tforwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
								cout << "\tnewGroup->forwardPropogationWordData.w = " << newGroup->forwardPropogationWordData.w << endl;
								cout << "\tforwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = " << forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW << endl;
								#endif
							}
						}
					}
				}
			}
		}
	}
		
	return result;
}
	


bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	//cout << "layer = " << layer << endl;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
			{
				GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
				GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToParseTreeGroupNew(ownerGroup);
				activationPathWordCurrentParseTreeGroupOwner->components.clear();	//dont wish to copy subcomponents into currentParseTreeGroupNew;
				
				if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup, currentComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationPathWordCurrentParseTreeGroupOwner);
				#endif				
			}
		}
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
	
		
bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	 		
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent = currentComponent;
	int componentIndex = currentComponent->componentIndex;	
	int ownerComponentIndex = componentIndex;
	if(currentComponent->isSubcomponent)
	{
		ownerComponent = currentComponent->ownerComponent;
		ownerComponentIndex = ownerComponent->componentIndex;
	}
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponentOrig = ownerComponent;
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponentOrig = currentComponent;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!ownerGroup->neuronPropagated)	//prevent circular loops
	{
	#endif	
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "2a: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
		//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
		{
		#endif
		
			//cout << "ownerGroup->groupTypeName = " << ownerGroup->groupTypeName  << endl;
		
			GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroupNew = NULL;
			GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupNew = NULL;
			
			generateMemoryActiveGroup(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, &activationMemoryGroupNew, &currentParseTreeGroupNew, true);			
			
			//required because ownerGroup is often equivalent to ownerGroupOrig:
			activationMemoryGroupNew->forwardPropogationWordData = *forwardPropogationWordData;
			//activationMemoryGroupNew->forwardPropogationSignalData = *forwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective
			activationMemoryGroupNew->layer = layer;
			activationMemoryGroupNew->neuronActive = false;
			//activationMemoryGroupNew->pointArrayGenerated = false;
			activationMemoryGroupNew->firstLevelActivationPointAdded = false;
			activationMemoryGroupNew->activationPathWordFirstActivationMemoryGroupActivationPointArray.clear();
			currentParseTreeGroupNew->activationPathWordFirstParseTreeGroupActivationPointArray.clear();
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupOwnerComponent = activationMemoryGroupNew->components[ownerComponentIndex];
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
			if(currentComponentOrig->isSubcomponent)
			{
				if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
				{
					/*
					cout << "propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
					cout << "componentIndex = " << componentIndex << endl;
					*/
					activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
				}
			}
			ownerGroup = activationMemoryGroupNew;
			currentComponent = activationMemoryGroupCurrentComponent;
			ownerComponent = activationMemoryGroupOwnerComponent;
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
			activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupNew;	//use at top of parseTreeGroup at top of stack
			#endif

			if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
			}

			GIAtxtRelTranslatorDebug debug;

			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
			cout << "2b: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
			#endif

			if(currentComponent->semanticRelationReturnEntity)
			{
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
			}

			int indexOfFirstComponentInGroup = 0;
			int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

			/*
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
			bool lastComponentFuzzyAndCompatible = false;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
			bool lastComponentOptionalAndInactiveAndCompatible = false;
			bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
			if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.isLastComponentFuzzy(ownerGroup))
			{
				if(lastComponent == ownerComponent)
				{
					if(lastComponent->neuronComponentConnectionActive)
					{
						if(lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
						{
							if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->w - 1)
							{
								lastComponentRepeatAndActivatedByPreviousWordAndCompatible = true;
								lastComponentFuzzyAndCompatible = true;
								//cout << "lastComponentRepeatAndActivatedByPreviousWord" << endl;
							}
						}
					}
					else
					{
						if(lastComponent->optional)
						{
							lastComponentOptionalAndInactiveAndCompatible = true;
							lastComponentFuzzyAndCompatible = true;
							//cout << "lastComponentFuzzyAndCompatible" << endl;
						}
					}
				}
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
			if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.isLastComponentFuzzy(ownerGroup))
			{
				if(lastComponent == ownerComponent)
				{
					if(lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
					{
						lastComponentFuzzyAndCompatible = true;
					}
				}
				else 
				{
					lastComponentFuzzyAndCompatible = true;
				}
			}
			#endif
			#else
			bool lastComponentFuzzyAndCompatible = false;
			#endif
			*/


			bool deleteNewlyCreatedMemoryGroup = false;

			if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, &debug))
			{
				result = true;
			}
			else
			{
				deleteNewlyCreatedMemoryGroup = true;
			}

			if(deleteNewlyCreatedMemoryGroup)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(currentParseTreeGroupNew);
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(activationMemoryGroupNew);
				#endif
			}

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif	
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;
	if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
					#endif
				}
			}
			else
			{
				subComponentsPassed = true;
			}
		}
		if(subComponentsPassed)
		{
			sequentialActivationFound = true;
		
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(!firstActiveComponentInGroup || GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				if(propagateWordThroughNetworkGroupComponent(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, debug))
				{
					result = true;
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	//cout << "4: GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
		
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedInverseClass::generateRulesGroupTreeComponents;
	GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;	
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	if(newParseComponent->groupRefName == "")
	{
		newParseComponent->groupRefName = activationPathWordCurrentParseTreeGroup->groupName;
	}
	//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
	#endif

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroupComponent: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "propagateWordThroughNetworkGroupComponent: activationSequenceCompleted = " << activationSequenceCompleted << endl;
	#endif

	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
	{
		result = true;
		
		forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(activationPathWordCurrentParseTreeGroupOwner);	//OLD: activationPathWordCurrentParseTreeGroup
	}
	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
	{
		result = true;
	}
				
	if(activationSequenceCompleted)
	{
		//enable connections to fuzzy groups
		if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			//result = true;

			//cout << "propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
			//cout << "(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)" << endl;
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
		}
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;

		activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup)->forwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective

		//record variables for GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedParse;
		ownerGroup->neuronActive = true;
		//activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
		
		bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{	
			//cout << "topLevelGroup forwardPropogationSentenceData->parseIsolatedSubreferenceSets = " << forwardPropogationSentenceData->parseIsolatedSubreferenceSets << endl;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
			if(forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW == 0) //forwardPropogationWordData->w == 0)	//verify that all words are captured by the tree
			{
			#endif
				if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
					maxIteration = true;	//3g8h	//CHECKTHIS //always take successfully parses that require the least number of iterations
					#endif
					#else
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;
					#endif
					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					bool print = false;
					bool performancePreprocess = false;
					int performanceNOTUSED = 0;
					bool calculateMaxWeight = true;
					int maxWeight = 0;
					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1, print, performancePreprocess, &performanceNOTUSED, forwardPropogationSentenceData->sentenceContents, calculateMaxWeight, &maxWeight);
					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetNeuronBackprop(activationPathWordCurrentParseTreeGroupOwner, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
					if(maxWeight >= parseTreeMaxWeight)
					{
						parseTreeMaxWeight = maxWeight;
					#endif
					
						forwardPropogationSentenceData->toplevelGroupActivationFound = true;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
						topLevelParseTreeGroupLocal = activationPathWordCurrentParseTreeGroupOwner;
						#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE_RECORD_PERFORMANCE
						if(!GIAtxtRelTranslatorNeuralNetworkPropagateOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
						{
							//result = false;
						}
						#endif
						
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						cout << "topLevelGroup found" << endl;
						printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
						cout << "end printBackpropParseTree" << endl;
						#endif

						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						*/
					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					}
					#endif
				}
				
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY	
			}
			#endif
		}
		else
		{
			if(!(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup))	//CHECKTHIS
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))	//OLD: ownerGroupOrig
				{
					result = true;
				}
			}
		}
	}
	else
	{
		if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
		}	
	}
	
	return result;
}







bool GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}














//copyParseTreeGroupComponents = false
void GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::generateMemoryActiveGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentParseTreeGroupTemplate, GIAtxtRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAtxtRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents)
{
	*activationMemoryGroupNew = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroupNew(activationMemoryGroup);
	
	if(copyParseTreeGroupComponents == true)
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::generateMemoryActiveGroup error{}: (copyParseTreeGroupComponents == true)" << endl;
		exit(EXIT_ERROR);
	}
	
	(*activationMemoryGroupNew)->groupOrig = groupOrig;
	*currentParseTreeGroupNew = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToParseTreeGroupNew(currentParseTreeGroupTemplate);
}

//copyParseTreeGroupComponents = true
void GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::generateMemoryActiveGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* activationMemoryGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* groupOrig, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupActivationMemory** activationMemoryGroupNew, GIAtxtRelTranslatorRulesGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents)
{
	*activationMemoryGroupNew = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.convertNeuralNetworkGroupToMemoryActivationGroupNew(activationMemoryGroup);
	
	if(copyParseTreeGroupComponents == false)
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::generateMemoryActiveGroup error{}: (copyParseTreeGroupComponents == false)" << endl;
		exit(EXIT_ERROR);
	}
	
	(*activationMemoryGroupNew)->groupOrig = groupOrig;
	*currentParseTreeGroupNew = GIAtxtRelTranslatorRules.copyGroup(currentParseTreeGroup);
}


void GIAtxtRelTranslatorNeuralNetworkPropagateHeavyOptimisedClass::printComponents(vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components)
{		
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (*components)[i];
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentComponent, 0);
		cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			printComponents(&(currentComponent->subComponents)); 
		}
	}
}

#endif
#endif
