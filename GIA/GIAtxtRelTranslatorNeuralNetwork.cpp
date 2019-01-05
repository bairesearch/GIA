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
 * File Name: GIAtxtRelTranslatorNeuralNetwork.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g8a 19-December-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_UNOPTIMISED


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif


bool GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	topLevelParseTreeGroupLocal = NULL;
	#endif
				
	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
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
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = new GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.resize(sentenceContents->size());
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());

	//resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork)
	
	//GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;
		
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
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, &forwardPropogationSentenceData))
		{
			result = true;
		}
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		//cout << "topLevelParseTreeGroupLocal" << endl;
		
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "topLevelParseTreeGroupLocal->groupName = " << topLevelParseTreeGroupLocal->groupName << endl;
		//#endif	
		#endif	
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		*performance = forwardPropogationSentenceData.performance;
		//cout << "forwardPropogationSentenceData.performance = " << forwardPropogationSentenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		//cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
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
		traceBackpropParseTree(topLevelParseTreeGroupLocal, 1, print, performancePreprocess, &performanceNOTUSED, NULL);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during resetAllNeuronComponents
	}
	#endif
	for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
	{
		vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray[w];
		vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray[w];
		if(activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size())
		{	
			cerr << "GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork error: activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size()" << endl;
			exit(EXIT_ERROR);
		} 
		for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 
		{
			GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
			GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];			
			resetGroupParseTreeGroupRef(activationPathWordFirstActivationMemoryGroup, activationPathWordFirstParseTreeGroup, true);
		}
		activationPathWordFirstActivationMemoryGroupArray.clear();
		activationPathWordFirstParseTreeGroupArray.clear();
	}
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.clear();
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.clear();
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		resetNeuronBackprop(topLevelParseTreeGroupLocal, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	}
	#endif
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		cout << "w = " << w << ", forwardPropogationWordData->foundPreviousActiveWord = " << forwardPropogationWordData->foundPreviousActiveWord << endl;
		#endif
	}
	
		
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkWordGroupIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
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
		
	//GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup

	GIApreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
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
		
	
	if(!currentWordAmbiguous(currentWord))
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
		if(GIApreprocessorPOStagger.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
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
	
			
	
	
	GIAtxtRelTranslatorRulesGroup* inputLayerGroup = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}
	}
	#endif
		
	for(unordered_map<string, GIAtxtRelTranslatorRulesGroup*>::iterator iter1 = GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionTokensLayerMap()->begin(); iter1 != GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionTokensLayerMap()->end(); iter1++)
	{
		GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionTokensLayer = iter1->second;
		for(int i=0; i<currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionTokensLayerClass = (currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList)[i];
			for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList.size(); i++)
			{
				GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionTokensLayerClassType = (currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList)[i];
				for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList.size(); i++)
				{
					GIAtxtRelTranslatorRulesGroup* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = (currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList)[i];
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
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
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

	
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroup* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
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
	
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkWordGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroup* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	//cout << "activationPathWordFirstParseTreeGroupNext->inputGroupString" << endl;
	
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup = NULL;
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;
	generateMemoryActiveGroup(inputLayerGroup, inputLayerGroup, inputLayerGroup, &activationPathWordFirstActivationMemoryGroup, &activationPathWordFirstParseTreeGroup, false);
	/*
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup = copyGroup(inputLayerGroup);
	activationPathWordFirstActivationMemoryGroup->groupOrig = inputLayerGroup;
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
	*/
	
	forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w].push_back(activationPathWordFirstActivationMemoryGroup);
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w].push_back(activationPathWordFirstParseTreeGroup);
	
	//cout << "layer = " << layer << endl;
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
	

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
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

	bool foundTopLevelGroup = false;
	bool stillGeneratingSets = true;
	int connectIterationIndex = 1;
	while(!foundTopLevelGroup && stillGeneratingSets)	//repeat until found top level group with all encapsulated words:
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\nconnectIterationIndex = " << connectIterationIndex << endl;
		#endif
		
		//forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes = GIAtxtRelTranslatorRulesGroupTypes;
	
		
		for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
		{
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
			cout << "w = " << w << endl;
			#endif
			*/
			
			vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w];
			vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];

			for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 	//for each existing parseTreeGroup stored at w
			{
				GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
				GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];
				
				int wNext = w + activationPathWordFirstActivationMemoryGroup->numberWordsInSet;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
				cout << "w = " << w << endl;
				cout << "wNext = " << wNext << endl;
				#endif
				
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroup->layer);
				cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroup: " <<  activationPathWordFirstActivationMemoryGroup->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroup->groupName << endl;		
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
				vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
				vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupActivationPointArray;	
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
						
						//cout << "activationPathWordFirstActivationMemoryGroup = " << activationPathWordFirstActivationMemoryGroup << endl;
						//cout << "activationPathWordFirstActivationMemoryGroup->wordGroupNeuron = " << activationPathWordFirstActivationMemoryGroup->wordGroupNeuron << endl;	
						//cout << "i = " << i << endl;

						//cout << "propagateWordThroughNetworkGroup start" << endl;
						if(propagateWordThroughNetworkGroup(translatorVariables, activationPathWordFirstActivationMemoryGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
						{
							result = true;
						}
						//cout << "propagateWordThroughNetworkGroup end" << endl;
					}
					else
					{
						//cout << endl;
					}

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
				}
				#endif

				if(wNext < sentenceContents->size())
				{
					vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[wNext];
					vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[wNext];
					for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupArrayNext.size(); i2++) 	//for each adjacent parseTreeGroup available (that does not skip an encapsulate word), ie stored at w+n (where n is number of words in the left parse group)
					{
						GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroupNext = activationPathWordFirstActivationMemoryGroupArrayNext[i2];
						GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroupNext = activationPathWordFirstParseTreeGroupArrayNext[i2];

						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroupNext->layer);
						cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroupNext: " <<  activationPathWordFirstActivationMemoryGroupNext->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroupNext->groupName << endl;		
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
						vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						vector<GIAtxtRelTranslatorRulesGroup*> activationPathWordFirstParseTreeGroupActivationPointArrayNext;	
						forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray = &activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray = &activationPathWordFirstParseTreeGroupActivationPointArrayNext;
						#endif
						forwardPropogationSentenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointDataNext;
						forwardPropogationActivationPointDataNext.generateActivationPointArray = true;


						//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroup->layer);
						//cout << "activationPathWordFirstParseTreeGroup->components.size() = " << activationPathWordFirstParseTreeGroup->components.size() << endl;
	
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!(activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated))
						{
						#endif
							if(activationPathWordFirstActivationMemoryGroupNext->neuronActive)
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated = true;
								#endif
								/*
								cout << "activationPathWordFirstActivationMemoryGroupNext = " << activationPathWordFirstActivationMemoryGroupNext << endl;
								//cout << "activationPathWordFirstActivationMemoryGroupNext->wordGroupNeuron = " << activationPathWordFirstActivationMemoryGroupNext->wordGroupNeuron << endl;	
								cout << "i2 = " << i2 << endl;
								*/

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
					}
				}
			}
		}
		
		if(forwardPropogationSentenceData->finishedPassingSentenceWords)
		{
			stillGeneratingSets = false;
		}
		if(connectIterationIndex >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_MAX_ITERATIONS)
		{
			stillGeneratingSets = false;
		}
							
		connectIterationIndex++;
	}
	
	return result;
	
}

//CHECKTHIS; check parameter forwardPropogationWordData
bool GIAtxtRelTranslatorNeuralNetworkClass::connectToPreviousActivationGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroupNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataNext, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	//cout << "forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray.size() = " << forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray->size() << endl;
	for(int i1=0; i1<forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i1++)
	{
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i1];
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstParseTreeGroupActivationPointArray)[i1];
		//cout << "connectToPreviousActivationGroup: activationPathWordFirstActivationMemoryGroupNextPoint: " <<  activationPathWordFirstActivationMemoryGroupNextPoint->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroupNextPoint->groupName << endl;

		if(activationPathWordFirstActivationMemoryGroupNextPoint->neuronActive)	//added 3g8aTEMP22
		{
			//cout << "\tactivationPathWordFirstActivationMemoryGroupNextPoint->neuronActive" << endl;
			
			for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupNextPoint->ANNfrontComponentConnectionList.size(); i2++)
			{
				GIAtxtRelTranslatorRulesComponent* currentComponentNext = (activationPathWordFirstActivationMemoryGroupNextPoint->ANNfrontComponentConnectionList)[i2];
				GIAtxtRelTranslatorRulesGroup* ownerGroupNext = currentComponentNext->ownerGroup;
				//cout << "connectToPreviousActivationGroup: ownerGroupNext: " <<  ownerGroupNext->groupTypeNameBackup << ":" << ownerGroupNext->groupName << endl;
				
				for(int i3=0; i3<forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i3++)
				{
					GIAtxtRelTranslatorRulesGroup* activationPathWordFirstActivationMemoryGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i3];
					GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray)[i3];
					GIAtxtRelTranslatorRulesGroup* groupOrig = activationPathWordFirstActivationMemoryGroupPoint->groupOrig;
					//cout << "connectToPreviousActivationGroup: activationPathWordFirstActivationMemoryGroupPoint: " <<  activationPathWordFirstActivationMemoryGroupPoint->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroupPoint->groupName << endl;
					//cout << "activationPathWordFirstParseTreeGroupPoint->components.size() = " << activationPathWordFirstParseTreeGroupPoint->components.size() << endl;

					if(groupOrig == ownerGroupNext)
					{
						//found matching group names between set and next set (see if their memory groups are connected)

						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupPoint: " <<  activationPathWordFirstActivationMemoryGroupPoint->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroupPoint->groupName << endl;		
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupNextPoint: " <<  activationPathWordFirstActivationMemoryGroupNextPoint->groupTypeNameBackup << ":" << activationPathWordFirstActivationMemoryGroupNextPoint->groupName << endl;		
						#endif

						GIAtxtRelTranslatorRulesComponent* ownerComponentNext = currentComponentNext;
						int componentIndex = currentComponentNext->componentIndex;
						int ownerComponentIndex = componentIndex;
						if(currentComponentNext->isSubcomponent)
						{
							ownerComponentNext = currentComponentNext->ownerComponent;
							ownerComponentIndex = ownerComponentNext->componentIndex;
						}

						GIAtxtRelTranslatorRulesGroup* ownerGroup = activationPathWordFirstActivationMemoryGroupPoint;
						GIAtxtRelTranslatorRulesComponent* ownerComponent = ownerGroup->components[ownerComponentIndex];
						GIAtxtRelTranslatorRulesComponent* currentComponent = ownerComponent;
						if(currentComponentNext->isSubcomponent)
						{
							currentComponent = ownerComponent->subComponents[componentIndex];
						}

						//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						//cout << "ownerComponentIndex = " << ownerComponentIndex << endl;
						//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						//cout << "activationPathWordFirstParseTreeGroupPoint->components.size() = " << activationPathWordFirstParseTreeGroupPoint->components.size() << endl;
					
						//cout << "currentComponent = " << endl;
						//GIAtxtRelTranslatorRules.printComponent(currentComponent, layer);
						//cout << "ownerGroup->groupName = " << ownerGroup->groupName << endl;

						forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup = true;
						forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray = false;	//CHECKTHIS
						forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = forwardPropogationWordData->w;
						

						/*
						cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
						cout << "ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
						*/
						
						//forwardPropogationActivationPointDataNext->connectToPreviousActivationGroup = true;

						
						if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
						{
							if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup->groupOrig, currentComponent, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroupNextPoint, activationPathWordFirstParseTreeGroupPoint))
							{
								result = true;

								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
								cout << "connectToPreviousActivationGroup: propagateWordThroughNetworkGroupSelect passed" << endl;
								#endif

								/*
								if(result)
								{
									cerr << "GIAtxtRelTranslatorNeuralNetworkClass::connectToPreviousActivationGroup found 2 or more connections" << endl;
									exit(EXIT_ERROR);
								}
								*/

								//FUTURE: ideally this should be done (set correctly originally) in GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent:
								GIAtxtRelTranslatorRulesGroup* newGroup = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationWordData->w].back();	//forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW
								newGroup->numberWordsInSet = activationPathWordFirstActivationMemoryGroup->numberWordsInSet + activationPathWordFirstActivationMemoryGroupNext->numberWordsInSet;	//CHECKTHIS
								newGroup->forwardPropogationWordData.w = forwardPropogationWordData->w;
								
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
								cout << "\tnewGroup: " <<  newGroup->groupTypeNameBackup << ":" << newGroup->groupName << endl;	
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
	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	//cout << "layer = " << layer << endl;
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;
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
				GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
				GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;
				GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = new GIAtxtRelTranslatorRulesGroup(*ownerGroup);
				activationPathWordCurrentParseTreeGroupOwner->components.clear();	//dont wish to copy subcomponents into currentParseTreeGroupNew;
				
				/*
				cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup: ownerGroup = " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;
				cout << "start printComponents" << endl;
				printComponents(&(ownerGroup->components));	//DEBUG
				cout << "end printComponents" << endl;
				*/
				
				if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup, currentComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
				deleteGroup(activationPathWordCurrentParseTreeGroupOwner);
				#endif				
			}
		}
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
	
		
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 		
	//cout << "ownerGroup->groupName = " << ownerGroup->groupName << endl;

	GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
	int componentIndex = currentComponent->componentIndex;	
	int ownerComponentIndex = componentIndex;
	if(currentComponent->isSubcomponent)
	{
		ownerComponent = currentComponent->ownerComponent;
		ownerComponentIndex = ownerComponent->componentIndex;
	}
	GIAtxtRelTranslatorRulesComponent* ownerComponentOrig = ownerComponent;
	GIAtxtRelTranslatorRulesComponent* currentComponentOrig = currentComponent;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!ownerGroup->neuronPropagated)	//prevent circular loops
	{
	#endif	
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "2a: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
		//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
		GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		if(componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
		{
		#endif
		
			GIAtxtRelTranslatorRulesGroup* activationMemoryGroupNew = NULL;
			GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupNew = NULL;
			
			/*
			cout << "propagateWordThroughNetworkGroupSelect: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
			cout << "2b: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;
			*/
			
			generateMemoryActiveGroup(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, &activationMemoryGroupNew, &currentParseTreeGroupNew, true);			
			
			//required because ownerGroup is often equivalent to ownerGroupOrig:
			activationMemoryGroupNew->forwardPropogationWordData = *forwardPropogationWordData;
			//activationMemoryGroupNew->forwardPropogationSignalData = *forwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective
			activationMemoryGroupNew->layer = layer;
			activationMemoryGroupNew->neuronActive = false;
			//activationMemoryGroupNew->pointArrayGenerated = false;
			activationMemoryGroupNew->firstLevelActivationPointAdded = false;
				
			//cout << "activationMemoryGroupNew->groupName = " << activationMemoryGroupNew->groupName << endl;
			//cout << "activationMemoryGroupNew->components.size() = " << activationMemoryGroupNew->components.size() << endl;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
			GIAtxtRelTranslatorRulesComponent* activationMemoryGroupOwnerComponent = activationMemoryGroupNew->components[ownerComponentIndex];
			GIAtxtRelTranslatorRulesComponent* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
			if(currentComponentOrig->isSubcomponent)
			{
				if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
				{
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
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
			debug.activationMemoryGroupArrayIndex = activationMemoryGroupArrayIndex;
			debug.activationMemoryGroupArraySize = activationMemoryGroupArraySize;
			debug.firstComponentActive = ownerGroup->components[0]->neuronComponentConnectionActive;
			if(ownerGroup->components.size() >= 2)
			{
				debug.secondComponentActive = ownerGroup->components[1]->neuronComponentConnectionActive;
			}
			#endif
			*/

			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "2b: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
			//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
			GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
			#endif

			//int currentMemoryGroupArrayNewlyCreatedIndex = ownerGroup->currentParseTreeGroupArray.size()-1;	//CHECKTHIS; is this required for groupArray deletion?


			if(currentComponent->semanticRelationReturnEntity)
			{
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
			}

			int indexOfFirstComponentInGroup = 0;
			int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
			GIAtxtRelTranslatorRulesComponent* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
			GIAtxtRelTranslatorRulesComponent* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
			bool lastComponentFuzzyAndCompatible = false;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
			bool lastComponentOptionalAndInactiveAndCompatible = false;
			bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
			if(isLastComponentFuzzy(ownerGroup))
			{
				if(lastComponent == ownerComponent)
				{
					if(lastComponent->neuronComponentConnectionActive)
					{
						if(lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
						{
							//OLD bug: if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->w - 1)
							if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == convertWordIndexToSentenceEntityIndex(forwardPropogationWordData->w - 1))
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
			if(isLastComponentFuzzy(ownerGroup))
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


			bool deleteNewlyCreatedMemoryGroup = false;

			if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, &debug))
			{
				result = true;
			}
			else
			{
				deleteNewlyCreatedMemoryGroup = true;
			}

			if(deleteNewlyCreatedMemoryGroup)
			{
				if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
					deleteGroup(currentParseTreeGroupNew);
					#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
					/*
					cout << "activationMemoryGroupNew->wordGroupNeuron = " << activationMemoryGroupNew->wordGroupNeuron << endl;
					cout << "activationMemoryGroupNew->groupName = " << activationMemoryGroupNew->groupName << endl;
					cout << "activationMemoryGroupNew->components.size() = " << activationMemoryGroupNew->components.size() << endl;
					*/
					deleteGroup(activationMemoryGroupNew);
					#endif
				}
			}

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif	
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	
	//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	//cout << "activationPathWordCurrentParseTreeGroupOwner->components.size() = " << activationPathWordCurrentParseTreeGroupOwner->components.size() << endl;
	#endif
	
	bool firstActiveComponentInGroup = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, lastComponentFuzzyAndCompatible, &firstActiveComponentInGroup))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents), activationPathWordCurrentParseTreeGroupOwner))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
		
			//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			//cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(!firstActiveComponentInGroup || componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				if(propagateWordThroughNetworkGroupComponent(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, debug))
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
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
		
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorInverseNeuralNetworkClass::generateRulesGroupTreeComponents;
	GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
	#endif

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroupComponent: " <<  ownerGroupOrig->groupTypeNameBackup << ":" << ownerGroupOrig->groupName << endl;
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	//cout << "activationPathWordCurrentParseTreeGroupOwner->components.size() = " << activationPathWordCurrentParseTreeGroupOwner->components.size() << endl;
	
	

	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "propagateWordThroughNetworkGroupComponent: !lastComponentFuzzyAndCompatible, activationSequenceCompleted = " << activationSequenceCompleted << endl;
	#endif

	
	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
	{
		result = true;
		
		/*
		cout << "4: propagateWordThroughNetworkGroupComponent: activationPathWordCurrentParseTreeGroupOwner " <<  activationPathWordCurrentParseTreeGroupOwner->groupTypeNameBackup << ":" << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
		cout << "forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = " << forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW << endl;
		cout << "forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray.size() = " << forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray.size() << endl;
		*/
		
		forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(ownerGroup);
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(activationPathWordCurrentParseTreeGroupOwner);	//OLD: activationPathWordCurrentParseTreeGroup
	}
	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
	{
		result = true;
	}
	
	/*
	//debug;
	if(ownerGroup->forwardPropogationWordData.w == -1)
	{
		cout << "ERROR: propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "4: propagateWordThroughNetworkGroupComponent: " <<  ownerGroupOrig->groupTypeNameBackup << ":" << ownerGroupOrig->groupName << endl;
		GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);

		exit(EXIT_ERROR);
	}
	*/
				
	if(activationSequenceCompleted)
	{
		//if(lastComponentFuzzyAndCompatible)
		//{
			//enable connections to fuzzy groups
			if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
			{
				//result = true;
				
				//cout << "propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
				//cout << "(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)" << endl;
				forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(ownerGroup);
				forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
			}
		//}
		
		//cout << "propagateWordThroughNetworkGroupComponent: repeatDetected = " << repeatDetected << endl;

		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;


		activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);
		ownerGroup->forwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective

		//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
		ownerGroup->neuronActive = true;
		activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
		

		bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeNameBackup, ownerGroupOrig->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
			if(forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW == 0) //forwardPropogationWordData->w == 0)	//verify that all words are captured by the tree
			{
			#endif
				if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
				{
					//cout << "forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = " << forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW << endl;

					//result = true;
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
					forwardPropogationSentenceData->toplevelGroupActivationFound = true;
					topLevelParseTreeGroupLocal = activationPathWordCurrentParseTreeGroupOwner;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
					if(!updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
					{
						//result = false;
					}
					#endif
					#endif
					
					/*
					cout << "topLevelGroup" << endl;
					cout << "finishedPassingSentenceWords (temp exit)" << endl;
					exit(0);
					*/
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
			//cout << "propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(ownerGroup);
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
		}	
	}
	
	return result;
}




/*
bool GIAtxtRelTranslatorNeuralNetworkClass::findComponentRef(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesComponent* componentRef, GIAtxtRelTranslatorRulesComponent** componentFound)
{
	bool result = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		if(currentParseTreeComponent->componentRef == componentRef)
		{
			result = true;
			*componentFound = currentParseTreeComponent;
		}
	}
	
	return result;
}
*/


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
bool GIAtxtRelTranslatorNeuralNetworkClass::isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last word as found 
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* lastWordData = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[numberOfWords-1]);
	isSentenceWordDataFullyConnected(&previousWordConnectionsFound, lastWordData);
	for(int w = 0; w<previousWordConnectionsFound.size(); w++)
	{
		if(!previousWordConnectionsFound[w])
		{
			result = false;
		}
	}
	
	return result;
}

void GIAtxtRelTranslatorNeuralNetworkClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* currentWordData)
{
	for(int i = 0; i<currentWordData->previousWordConnections.size(); i++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* previousWordData = (currentWordData->previousWordConnections)[i];
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		#endif
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif



#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAtxtRelTranslatorNeuralNetworkClass::componentTests1(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	if(currentComponent->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		
		/*
		cout << "componentTests; (currentComponent->wordNounVariantType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)" << endl;
		cout << "forwardPropogationSignalData->wordNounVariantType = " << forwardPropogationSignalData->wordNounVariantType << endl;
		cout << "currentComponent->wordNounVariantType = " << currentComponent->wordNounVariantType << endl;
		*/
		
		if(forwardPropogationSignalData->wordNounVariantType == currentComponent->wordNounVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
		else if(forwardPropogationSignalData->wordNounVariantType == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)	
		{
			componentTests = true;
		}
		#endif
	}
	if(currentComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordVerbVariantType == currentComponent->wordVerbVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE))
		{
			componentTests = true;
		}
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))
		{
			componentTests = true;
		}
		#endif
	}
	
	return componentTests;
}
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorNeuralNetworkClass::componentTests2(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the wordPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous word POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	if(group->previousWordPOStype != "")
	{
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		int translatorSentenceEntityIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		bool pass = false;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceEntityIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
			pass = true;
			//cout << "firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex << endl;
			//exit(EXIT_ERROR);
		}

		/*
		if(pass)
		{
			cout << "group->previousWordPOStype = " << group->previousWordPOStype << ", getFirstWordInParseTree pass, firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex = " << firstWordInCurrentParseTreeGroupParseTree->translatorSentenceEntityIndex << ", numComponentsFound = " << numComponentsFound << endl;
		}
		else
		{
			cout << "group->previousWordPOStype = " << group->previousWordPOStype << ", getFirstWordInParseTree !pass" << endl;
		}
		*/
		
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, previousWordPOStype))		
		{
			//cout << "componentTests = true" << endl;
			componentTests = true;
		}
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		int translatorSentenceEntityIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceEntityIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
		
		}
		
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
	}
	
	return componentTests;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceEntityIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
		
		//cout << "currentComponent" << endl;
		//if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		//{
			*numComponentsFound = *numComponentsFound + 1;
		//}
		
		int translatorSentenceEntityIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		if(translatorSentenceEntityIndex < *translatorSentenceEntityIndexMin)
		{
			*translatorSentenceEntityIndexMin = translatorSentenceEntityIndex;
			*firstWordInCurrentParseTreeGroupParseTree = currentComponent->neuronComponentConnectionActiveWordRecord;
			result = true;
		}
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			if(getFirstWordInParseTree(currentComponent->parseTreeGroupRef, firstWordInCurrentParseTreeGroupParseTree, translatorSentenceEntityIndexMin, numComponentsFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}

#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	GIApreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				previousComponentWord = contextWord;
			}
		}
	}
	if(previousComponentWord != NULL)
	{
		if(checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
{
	bool result = false;	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == unambiguousPOSindex)
	{
		result = true;
	}
	#else
	if(!currentWordAmbiguous(currentWord))
	{
		//for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		//{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, unambiguousPOSindex))
			{
				result = true;
			}
		//}
	}
	#endif
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool lastComponentFuzzyAndCompatible, bool* firstActiveComponentInGroup)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	int numberOfInactiveComponentsRemaining = 0;
	GIAtxtRelTranslatorRulesComponent* previousActiveComponent = NULL;	//not used

	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];

		if(stillParsingActiveComponents)
		{
			/*
			cout << "component->neuronComponentConnectionActive = " << component->neuronComponentConnectionActive << endl;
			cout << "component->optional = " << component->optional << endl;
			cout << "component->hasSubComponents = " << GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component) << endl;
			*/
			if(component == testComponent)
			{
				//cout << "(component == testComponent)" << endl;

				if(previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;	
				}
				
				stillParsingActiveComponents = false;

				if(component->neuronComponentConnectionActive)
				{
					if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)	//if the component contains repeats, then accept it if it is already active
					{
						//sequential activation found
						sequentialActivationFound = true;
					}
					else
					{
						//component already activated
					}
				}
				else
				{
					sequentialActivationFound = true;
				}
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
				if(component->neuronComponentConnectionActive)
				{
					//cout << i << endl;
					
					//cout << "previousActiveComponent = component" << endl;
					previousActiveComponent = component;
				}
				#endif

				if(component->optional)
				{
					//ignore optional components
				}
				else
				{
					if(!(component->neuronComponentConnectionActive))
					{
						stillParsingActiveComponents = false;
					}						
				}
			}
		}
		else
		{
			if(sequentialActivationFound)
			{
				if(!(component->optional))
				{
					if(!(component->neuronComponentConnectionActive))
					{
						numberOfInactiveComponentsRemaining++;	
					}
					else
					{
						//cout << "group not suitable for activation, following component is active" << endl;
						bool foundFollowingActiveComponent = true;
						sequentialActivationFound = false;
					}
				}
			}
		}
	}
	
	if(sequentialActivationFound)
	{
		if(numberOfInactiveComponentsRemaining == 0)
		{
			//cout << "*activationSequenceCompleted" << endl;
			
			*activationSequenceCompleted = true;
		}
		//cout << "numberOfInactiveComponentsRemaining = " << numberOfInactiveComponentsRemaining << endl;
	}

	return sequentialActivationFound;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			if(component->neuronComponentConnectionActive)
			{
				//component already activated
			}	
			else
			{
				//sequential activation found
				sequentialActivationFound = true;
			}
		}
	}
	
	return sequentialActivationFound;
}

//precondition: repeat sequences can contain only 1 component
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			bool pass = true;
			if(testComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(component->neuronComponentConnectionActive)
				{
					pass = false;
					if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->wordReference->translatorSentenceEntityIndex - 1)
					{
						pass = true;
						//cout << "propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady pass" << endl;
						//exit(EXIT_ERROR);
					}
				}
			}
			if(pass)
			{
			#endif
				//sequential activation found
				if(component->neuronComponentConnectionActive)
				{
					*repeatDetected = true;
				}
				sequentialActivationFound = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			}
			#endif
		}
		else
		{
			cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}














bool GIAtxtRelTranslatorNeuralNetworkClass::resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroup* group = (groupType->groups)[i2];

			bool* boolPointer = NULL;
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED)
			{
				boolPointer = &(group->neuronGenerated);
			}

			if(*boolPointer)
			{
				*boolPointer = false;
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroup* group = (groupType->groups)[i2];

			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE)
			{
				resetGroupActivation(group);
			}
			/*
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				resetGroupParseTreeGroupRef(group, true);
			}
			*/
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED)
			{
				//cout << "group->neuronPropagated = false" << endl;
				group->neuronPropagated = false;
			}
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE)
			{
				group->neuronPropagatedSave = group->neuronPropagated;
			}
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET)
			{
				group->neuronPropagated = group->neuronPropagatedSave;
			}
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAtxtRelTranslatorNeuralNetworkClass::groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent2 = (group->components)[i2];
		/*
		cout << "\tcurrentComponent2->neuronComponentConnectionActive = " << currentComponent2->neuronComponentConnectionActive << endl;
		cout << "\tcurrentComponent2->optional = " << currentComponent2->optional << endl;
		cout << "\tcurrentComponent2->hasSubComponents = " << GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent2) << endl;
		*/
		if(!(currentComponent2->neuronComponentConnectionActive))
		{
			if(!(currentComponent2->optional))
			{
				groupActivationCompleted = false;
			}
		}
	}
	
	return groupActivationCompleted;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
bool GIAtxtRelTranslatorNeuralNetworkClass::isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group)
{
	bool lastComponentIsFuzzy = false;

	int indexOfLastComponentInGroup = group->components.size()-1;
	GIAtxtRelTranslatorRulesComponent* lastComponent = (group->components)[indexOfLastComponentInGroup];
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_TREAT_OR_AS_FUZZY
	if(lastComponent->optional || GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(lastComponent))
	#else
	if(lastComponent->optional || (lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	#endif
	{
		lastComponentIsFuzzy = true;
	}
		
	return lastComponentIsFuzzy;	
}
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
bool GIAtxtRelTranslatorNeuralNetworkClass::groupPartiallyOrFullyActivated(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool groupPartiallyActivated = false;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent2 = (group->components)[i2];
		if(currentComponent2->neuronComponentConnectionActive)
		{
			groupPartiallyActivated = true;
		}
	}
	
	return groupPartiallyActivated;
}
#endif
				
bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool result = true;

	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	#endif
	*/
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
	cout << "resetGroupActivation: group->groupName = " << group->groupName << ", group->groupTypeNameBackup = " << group->groupTypeNameBackup << endl;
	#endif
	
	group->neuronActive = false;	//not used
	resetComponentsActivation(&(group->components));
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i1];
		currentComponent->neuronComponentConnectionActive = false;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsActivation(&(currentComponent->subComponents));
		}
	}
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* activationMemoryGroup, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(!(currentParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
		{
			//delete currentParseTreeGroup;
			deleteGroup(currentParseTreeGroup);
		}
	}	
	
	deleteGroup(activationMemoryGroup);
	//delete activationMemoryGroup;
	
	return result;
}
#endif









#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
bool GIAtxtRelTranslatorNeuralNetworkClass::updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "GIAtxtRelTranslatorNeuralNetworkClass::updatePerformance" << endl;
		 
	bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeNameBackup, currentParseTreeGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		int performanceTemp = 0;
		bool print = false;
		bool performancePreprocess = true;
		traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents);
		resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		forwardPropogationSentenceData->performance = performanceTemp;
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
		forwardPropogationSentenceData->performance = 0;
		for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
		{
			GIApreprocessorPlainTextWord* currentWord = (forwardPropogationSentenceData->sentenceContents)->at(i);
			if(currentWord->alreadyFoundMatch)
			{
				forwardPropogationSentenceData->performance = forwardPropogationSentenceData->performance + 1;
				currentWord->alreadyFoundMatch = false;
			}
		}
		#endif
		#endif
	}
			
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, bool print, bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;
	
	if(!currentParseTreeGroup->neuronTraced)
	{
	
		currentParseTreeGroup->neuronTraced = true;

		if(performancePreprocess)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			*performance = *performance + 1;
			//won't calculate performance perfectly because some groups are reused
			#else
			/*
			if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentParseTreeGroup->semanticRelationReturnEntityForwardPropogationSignalData->wordReference))
			{
				//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
					//"dog" and "apple" will use identical neural groups and so will only count to +1 performance total
			}
			*/
			#endif
		}
				
		if(print)
		{
			//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeNameBackup = " << currentParseTreeGroup->groupTypeNameBackup << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			//#endif
		}

		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
			
			//GIAtxtRelTranslatorRules.printComponent(currentComponent, level);
			
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			if(performancePreprocess)
			{
				if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
				{
					currentComponent->candidateStringMatch->alreadyFoundMatch = true;
					//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
						//"the" and "a" will use identical neural groups and so will only count to +1 performance total
				}
			}
			#endif
	
			if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(print)
				{
					GIAtxtRelTranslatorRules.printComponent(currentComponent, level);
				}
			}
			//else
			//{					
				if(currentComponent->parseTreeGroupRef != NULL)
				{							
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
					if(print)
					{
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
						cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->parseTreeGroupRef->groupName = " << currentComponent->parseTreeGroupRef->groupName << endl;
					}
					#endif
					
					traceBackpropParseTree(currentComponent->parseTreeGroupRef, level+1, print, performancePreprocess, performance, sentenceContents);
				}
			//}
		}	
	}
	
	return result;
}



bool GIAtxtRelTranslatorNeuralNetworkClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(currentParseTreeGroup->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(currentParseTreeGroup->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<currentParseTreeGroup->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i1];
			if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
			{
				for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
				{
					GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
					if(subComponent->parseTreeGroupRef != NULL)
					{
						resetNeuronBackprop(subComponent->parseTreeGroupRef, groupBoolIndexType);
					}
				}
			}
			else
			{
				if(currentComponent->parseTreeGroupRef != NULL)
				{
					resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
				}	
			}
		}
	}
	
	return result;
}

#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool GIAtxtRelTranslatorNeuralNetworkClass::resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED)
	{
		boolPointer = &(group->neuronPropagated);
	}
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED)
	{
		boolPointer = &(group->neuronPreviousWordPOStypeTested);
	}
			
	if(*boolPointer)
	{
		//cout << "resetNeuronForwardProp: group = " << group->groupName << endl;
		*boolPointer = false;
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			GIAtxtRelTranslatorRulesGroup* ownerGroup = ownerComponent->ownerGroup;
			resetNeuronForwardProp(ownerGroup, groupBoolIndexType);
		}
	}
	
	return result;
}

#endif



bool GIAtxtRelTranslatorNeuralNetworkClass::deleteGroup(GIAtxtRelTranslatorRulesGroup* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAtxtRelTranslatorNeuralNetworkClass::deleteComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i];
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			deleteComponents(&(currentComponent->subComponents));
		}
		delete currentComponent;
	}
	
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
GIAtxtRelTranslatorRulesGroup* GIAtxtRelTranslatorNeuralNetworkClass::copyGroup(GIAtxtRelTranslatorRulesGroup* group)
{		
	GIAtxtRelTranslatorRulesGroup* newGroup = new GIAtxtRelTranslatorRulesGroup(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));
		
	/*
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	*/
	
	return newGroup;
}
bool GIAtxtRelTranslatorNeuralNetworkClass::copyComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIAtxtRelTranslatorRulesComponent*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		//cout << "copyComponents1" << endl;
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i];
		GIAtxtRelTranslatorRulesComponent* newComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);
		componentsNew->push_back(newComponent);
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			newComponent->subComponents.clear();
			copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
		}
		//cout << "copyComponents2" << endl;
	}
	
	return result;
}
#endif


bool GIAtxtRelTranslatorNeuralNetworkClass::doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponent* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool requireResetGroupActivation = false;
	
	if(Xcomponent->neuronComponentConnectionActive)
	{
		if(Xcomponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
		{
			if(Xcomponent->subComponents.size() > 0)
			{
				GIAtxtRelTranslatorRulesComponent* subComponent = (Xcomponent->subComponents)[0];
				if(subComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					//expect repeated strings to occur consecutively
					if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex != forwardPropogationWordData->wordReference->translatorSentenceEntityIndex - 1)
					{
						requireResetGroupActivation = true;
					}
				}
				else
				{
					if(findNextAvailableComponent(indexOfXcomponentInGroup, ownerGroup))
					{
						requireResetGroupActivation = true;
					}
					else
					{
						//FUTURE: what if still need to reset however; what if it was activated incorrectly, or already activated from a previous successful parse (e.g. <group groupName="nounMultiword" semanticRelationFunctionName="multiwordNoun">)?
					}
				}
			}
			else
			{
				cerr << "GIAtxtRelTranslatorNeuralNetworkClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
			}
		}
		else
		{
			requireResetGroupActivation = true;
		}
	}
	
	return requireResetGroupActivation;
}
						

bool GIAtxtRelTranslatorNeuralNetworkClass::findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup)
{
	bool foundNextAvailableComponent = false;
	bool findingNextAvailableComponent = true;
	
	for(int i2=indexOfXcomponentInGroup; i2<ownerGroup->components.size(); i2++)
	{
		if(findingNextAvailableComponent)
		{
			if(i2 > indexOfXcomponentInGroup)
			{
				GIAtxtRelTranslatorRulesComponent* component = (ownerGroup->components)[i2];
				if(!(component->neuronComponentConnectionActive))
				{
					if(!component->optional)
					{
						findingNextAvailableComponent = false;
						foundNextAvailableComponent = true;
					}
				}
				else
				{
					findingNextAvailableComponent = false;
				}
			}
		}
	}
	
	return foundNextAvailableComponent;
}
	
	
bool GIAtxtRelTranslatorNeuralNetworkClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	#else
	if(currentWord->POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	#endif
	{
		result = true;
	}
	
	return result;
}


int GIAtxtRelTranslatorNeuralNetworkClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int GIAtxtRelTranslatorNeuralNetworkClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool GIAtxtRelTranslatorNeuralNetworkClass::isWordPOStypeNoun(int wordPOStype)
{
	bool result = false;
	
	if(wordPOStype == GIA_SHARED_POS_TYPE_NOUN)	//what about GIA_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE etc from GIAsemRelTranslatorDefs.hpp?
	{
		result = true;
	}
	//sync with (ie based on) <group groupName="nounMultiwordPropernoun"> [includes: <group groupName="nounMultiwordName">]
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_MALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_FEMALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FAMILY)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_PLACE)
	{
		result = true;
	}	
	//sync with (ie based on) <group groupName="pronoun">		
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_INDEFINITE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_REFLEXIVE)
	{
		result = true;
	}	
	
	else if(wordPOStype == GIA_SHARED_POS_TYPE_VERB)
	{
		result = true;
	}
	
	return result;
}
#endif

void GIAtxtRelTranslatorNeuralNetworkClass::generateMemoryActiveGroup(GIAtxtRelTranslatorRulesGroup* activationMemoryGroup, GIAtxtRelTranslatorRulesGroup* groupOrig, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroup** activationMemoryGroupNew, GIAtxtRelTranslatorRulesGroup** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents)
{
	//cout << "activationMemoryGroup->groupName = " << activationMemoryGroup->groupName << endl;
	*activationMemoryGroupNew = copyGroup(activationMemoryGroup);
	(*activationMemoryGroupNew)->groupOrig = groupOrig;
	if(copyParseTreeGroupComponents)
	{
		*currentParseTreeGroupNew = copyGroup(currentParseTreeGroup);
	}
	else
	{
		*currentParseTreeGroupNew = new GIAtxtRelTranslatorRulesGroup(*currentParseTreeGroup);
	}
}


#endif

void GIAtxtRelTranslatorNeuralNetworkClass::printComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components)
{		
	for(int i=0; i<components->size(); i++)
	{
		//cout << "printComponents1" << endl;
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i];
		GIAtxtRelTranslatorRules.printComponent(currentComponent, 0);
		cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			printComponents(&(currentComponent->subComponents)); 
		}
		//cout << "printComponents2" << endl;
	}
}
#endif
