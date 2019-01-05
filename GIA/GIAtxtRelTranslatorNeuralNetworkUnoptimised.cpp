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
 * File Name: GIAtxtRelTranslatorNeuralNetworkUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g7a 12-December-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkUnoptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_UNOPTIMISED



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
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
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());
	
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w = 0;
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		bool passThrough = false;
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup, expectToSeeConnectionWithPreviousWordTrace, passThrough))
		{
			result = false;
		}
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
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
		
		/*
		cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
		exit(EXIT_ERROR);
		*/
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
		resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
		resetNeuronBackprop(topLevelParseTreeGroupLocal, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	//added GIA3g6aTEMP32 
		
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
			cout << "w = " << w << ", forwardPropogationWordData->foundPreviousActiveWord = " << forwardPropogationWordData->foundPreviousActiveWord << endl;
			#endif
		}
	}
	else
	{
		resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough)
{
	bool result = false;

	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	GIApreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << GIApreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif

	//word specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	forwardPropogationWordData->previousWordConnections.clear();
	#endif
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	//forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = expectToSeeConnectionWithPreviousWordTrace;
	#endif
	
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
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorPOStagger.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
		cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
		cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
		{
			result = true;
		}
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
							cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
						{
							result = true;
						}
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))	//what is this for?
						{
							result = true;
						}
					}
				}
			}
		}
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
		
	return result;
}

	
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroup* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_CONSECUTIVE_WORDS_SAME_POSTYPE
	if(w > 0)
	{
		int currentWordPosType = forwardPropogationSentenceData->forwardPropogationWordDataArray[w]->wordPOStype;
		int previousWordPosType = forwardPropogationSentenceData->forwardPropogationWordDataArray[w-1]->wordPOStype;
		if((currentWordPosType == previousWordPosType) && (currentWordPosType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED))
		{
			forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = true;
		}
	}
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}	
	
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroup* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
	activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	else
	{
		delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
	}
	
	return result;
}				
				

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{
	#endif
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
	#endif
	/*
	if(passThrough)
	{
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;		
	}
	*/
			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, passThrough))
		{
			result = true;
		}
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	else
	{
		cout << "FAIL: neuronPropagated" << endl;
		//result = true;
	}
	#endif
	
	return result;
}
		
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
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
				GIAtxtRelTranslatorRulesGroup* ownerGroupOrig = ownerGroup;

				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
				cout << "2a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
				#endif

				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				if(componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
				{
				#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION
					//this is required to support GIAtxtRelTranslatorRules.xml recursive groups, e.g. logicReferenceSets: 
					int activationMemoryGroupArraySizeOrig = ownerGroupOrig->activationMemoryGroupArray.size();
					int currentParseTreeGroupArraySizeOrig = ownerGroupOrig->currentParseTreeGroupArray.size();
					int index = 0;
					for(int activationMemoryGroupArrayIndex=ownerGroupOrig->activationMemoryGroupArray.size()-1; activationMemoryGroupArrayIndex>=0; activationMemoryGroupArrayIndex--)
					{
						index++;
					#endif	
						int activationMemoryGroupArraySize = ownerGroupOrig->activationMemoryGroupArray.size();
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION
						//cout << "activationMemoryGroupArrayIndex = " << activationMemoryGroupArrayIndex << endl;
						//cout << "ownerGroup->currentParseTreeGroupArray.size() = " << ownerGroupOrig->currentParseTreeGroupArray.size() << endl;
						GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray.back();
						#endif
						#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION
						GIAtxtRelTranslatorRulesGroup* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAtxtRelTranslatorRulesGroup* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray.back();
						#endif
						#endif
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						if(activationMemoryGroup->lastWordIndexActivated != convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex))
						{
						#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
							if(!(currentParseTreeGroup->solidified))	//this saves time by considering only recently activated groups
							{
							#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
								GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = NULL;
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
								GIAtxtRelTranslatorRulesGroup* activationMemoryGroupNew = NULL;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
								int activationMemoryGroupLastWordIndexActivatedOrig = INT_DEFAULT_VALUE;
								#endif
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
								GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupNew = NULL;
								#endif
								
								GIAtxtRelTranslatorRulesGroup* activationMemoryGroupTemp = NULL;
								GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp = NULL;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
									activationMemoryGroupNew = copyGroup(activationMemoryGroup);
									ownerGroupOrig->activationMemoryGroupArray.push_back(activationMemoryGroupNew);
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroupLastWordIndexActivatedOrig = activationMemoryGroup->lastWordIndexActivated;
									activationMemoryGroup->lastWordIndexActivated = convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
									//NOTREQUIRED: activationMemoryGroupNew->lastWordIndexActivated = convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex); 
									#endif
									#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
									currentParseTreeGroupNew = copyGroup(currentParseTreeGroup);	//create a new parse tree group (in case of 1. first encounter with group or 2. recursion into different group)
									ownerGroupOrig->currentParseTreeGroupArray.push_back(currentParseTreeGroupNew);
									#endif

									/*
									if(activationMemoryGroupNew->neuronPropagated)
									{
										cout << "activationMemoryGroupNew->neuronPropagated" << endl;
										exit(EXIT_ERROR);
									}
									if(currentParseTreeGroupNew->neuronPropagated)
									{
										cout << "activationMemoryGroupNew->neuronPropagated" << endl;
										exit(EXIT_ERROR);
									}
									*/					

									activationMemoryGroupTemp = activationMemoryGroupNew;
									currentParseTreeGroupTemp = currentParseTreeGroupNew;
									
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
									if(activationMemoryGroupArrayIndex != 0)	//never solidify first group in array (non-activated template)
									{
										currentParseTreeGroup->solidified = true;
									}
									#endif

								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								else
								{
									activationMemoryGroupTemp = activationMemoryGroup;
									currentParseTreeGroupTemp = currentParseTreeGroup;

									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
									currentParseTreeGroup->solidified = true;	//prevent recursion during passThrough
									#endif
								}
								#endif
								
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS		
								GIAtxtRelTranslatorRulesComponent* activationMemoryGroupOwnerComponent = activationMemoryGroupTemp->components[ownerComponentIndex];
								GIAtxtRelTranslatorRulesComponent* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
								if(currentComponentOrig->isSubcomponent)
								{
									if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
									{
										activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
									}
									else
									{
										cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupSelect{} error: (currentComponent->isSubcomponent) && !(activationMemoryGroupOwnerComponent->hasSubComponents)" << endl;
										cout << "index = " << index << endl;
										cout << "ownerComponentIndex = " << ownerComponentIndex << endl;
										cout << "componentIndex = " << componentIndex << endl;
										GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  group->groupTypeNameBackup << ":" << group->groupName << endl;	
										GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;
										cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
										cout << "ownerComponent->componentType = " << ownerComponent->componentType << endl;
										GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);	
										GIAtxtRelTranslatorRules.printComponent(ownerComponent, layer+1);
										exit(EXIT_ERROR);
									}
								}
								ownerGroup = activationMemoryGroupTemp;
								currentComponent = activationMemoryGroupCurrentComponent;
								ownerComponent = activationMemoryGroupOwnerComponent;
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
								activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupTemp;	//use at top of parseTreeGroup at top of stack
								#endif
									
													
								if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
								{
									currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
								}


								GIAtxtRelTranslatorDebug debug;
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
								debug.activationMemoryGroupArrayIndex = activationMemoryGroupArrayIndex;
								debug.activationMemoryGroupArraySize = activationMemoryGroupArraySize;
								debug.firstComponentActive = ownerGroup->components[0]->neuronComponentConnectionActive;
								if(ownerGroup->components.size() >= 2)
								{
									debug.secondComponentActive = ownerGroup->components[1]->neuronComponentConnectionActive;
								}
								#endif



								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
								GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
								cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
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


								#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION
								/*
								if(groupActivationCompleted(ownerGroup))
								{
									//cout << "groupActivationCompleted(ownerGroup)" << endl;
								}
								if(groupActivationCompleted(ownerGroup) && !lastComponentFuzzyAndCompatible)
								{
									//cout << "resetGroupActivation: ownerGroup = " << ownerGroup->groupName << endl;
									resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								*/
								
								//changed 3g5a (introduce more simple reset detection method);
								bool requireResetGroupActivation = false;
								if(firstComponent == ownerComponent)
								{
									if(doesRequireResetGroupActivation(firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationWordData))
									{
										requireResetGroupActivation = true;
									}
								}
								else if(ownerGroup->components.size() >= 1)
								{
									int indexOfSecondComponentInGroup = 1;
									GIAtxtRelTranslatorRulesComponent* secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];

									if((secondComponent == ownerComponent) && (firstComponent->optional))
									{
										if(doesRequireResetGroupActivation(secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationWordData))
										{
											requireResetGroupActivation = true;
										}
									}
								}

								if(requireResetGroupActivation)
								{
									#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
									cout << "requireResetGroupActivation" << endl;
									#endif
									resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								#endif


								bool deleteNewlyCreatedMemoryGroup = false;

								if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough, &debug))
								{
									result = true;
								}
								else
								{
									deleteNewlyCreatedMemoryGroup = true;
								}



								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									if(deleteNewlyCreatedMemoryGroup)
									{
										//cout << "deleteNewlyCreatedMemoryGroup" << endl;

										/*
										if(ownerGroupOrig->currentParseTreeGroupArray.size() != sizeOrig+1)
										{
											cerr << "(ownerGroupOrig->currentParseTreeGroupArray.size() != sizeOrig+1)" << endl;
											cout << "currentParseTreeGroupArraySizeOrig = " << currentParseTreeGroupArraySizeOrig << endl;
											cout << "activationMemoryGroupArraySizeOrig = " << activationMemoryGroupArraySizeOrig << endl;
											cout << "ownerGroupOrig->currentParseTreeGroupArray.size() = " << ownerGroupOrig->currentParseTreeGroupArray.size() << endl;
											cout << "ownerGroupOrig->activationMemoryGroupArray.size() = " << ownerGroupOrig->activationMemoryGroupArray.size() << endl;
											exit(EXIT_ERROR);
										}
										*/

										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
										deleteGroup(currentParseTreeGroupNew);
										ownerGroupOrig->currentParseTreeGroupArray.pop_back();
										#endif
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
										deleteGroup(activationMemoryGroupNew);
										ownerGroupOrig->activationMemoryGroupArray.pop_back();
										#endif
									}

									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroup->lastWordIndexActivated = activationMemoryGroupLastWordIndexActivatedOrig;
									#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								#endif

							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									if(activationMemoryGroupArrayIndex != 0)
									{
										currentParseTreeGroup->solidified = false;
									}
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								else
								{
									currentParseTreeGroup->solidified = false;
								}
								#endif
							}
							#endif

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						}
						#endif

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION	
					}
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif	
		}
	}
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	
	bool firstActiveComponentInGroup = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, lastComponentFuzzyAndCompatible, &firstActiveComponentInGroup, passThrough))
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
					//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			if(!firstActiveComponentInGroup || componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(componentWordConnectivityTests(activationPathWordCurrentParseTreeGroupOwner, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough, debug))
					{
						result = true;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	if(!result)
	{
		forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
	}
	#endif
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPreviousRecord = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		GIAtxtRelTranslatorRulesComponent* previousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components[activationPathWordCurrentParseTreeGroupOwner->components.size()-1];
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
		int wPrevious2 = forwardPropogationWordData->w - 1;
		if(wPrevious2 >= 0)	//redundant
		{
			if(previousActiveComponent->neuronComponentConnectionActiveWordRecord == (*(forwardPropogationSentenceData->sentenceContents))[wPrevious2])
			{
				forwardPropogationSignalData->foundPreviousActiveWord = true;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
				cout << "foundPreviousActiveWord" << endl;
				#endif
			}
		}
		else
		{
			cerr << "wPrevious2 = " << wPrevious2 << endl;
			exit(EXIT_ERROR);
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
		if(!passThrough)
		{
		#endif
			int wPrevious = convertSentenceEntityIndexToWordIndex(previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
			if(wPrevious >= 0)	//redundant
			{
				GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPrevious = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[wPrevious]);
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
				cout << "foundPreviousActiveWord, forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				#endif
				//cout << "forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
				if(lastComponentFuzzyAndCompatible)
				{
					*forwardPropogationWordDataPreviousRecord = forwardPropogationWordDataPrevious;
					/*
					if(find(forwardPropogationWordDataPrevious->previousWordConnections.begin(), forwardPropogationWordDataPrevious->previousWordConnections.end(), forwardPropogationWordData) == forwardPropogationWordDataPrevious->previousWordConnections.end())
					{
						forwardPropogationWordDataPrevious->previousWordConnections.push_back(forwardPropogationWordData);
					}	
					*/
				}
				else
				{
				#endif
					if(find(forwardPropogationWordData->previousWordConnections.begin(), forwardPropogationWordData->previousWordConnections.end(), forwardPropogationWordDataPrevious) == forwardPropogationWordData->previousWordConnections.end())
					{
						forwardPropogationWordData->previousWordConnections.push_back(forwardPropogationWordDataPrevious);
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
				}
				#endif
			}
			else
			{
				cerr << "wPrevious = " << wPrevious << endl;
				exit(EXIT_ERROR);
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
		}
		#endif
		#endif					
	}
	#endif
		
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{	
	#endif
		
		//cout << "forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;
		/*
		if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
		{
			cout << "(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)" << endl;
		}
		*/
		
		ownerComponent->neuronComponentConnectionActive = true;
		currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
		ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
		vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
		#endif
		#endif
		*/

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

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
		vector<GIAtxtRelTranslatorRulesGroup*> parseTreeGroupRefParseTreeGroupRefReverseOrig;
		vector<int> parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse;
		int activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex;
		activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
		activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex = activationPathWordCurrentParseTreeGroupOwner->components.size()-1;
		if(activationSequenceCompleted)
		{
		#endif
			//update the parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex of every component->parseTreeGroupRef in activationPathWordCurrentParseTreeGroupOwner (with the latest activationPathWordCurrentParseTreeGroupOwner memory container):
				//note this will be reexecuted accordingly if lastComponentFuzzyAndCompatible detected:
			for(int i=0; i<activationPathWordCurrentParseTreeGroupOwner->components.size(); i++)
			{
				GIAtxtRelTranslatorRulesComponent* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
				GIAtxtRelTranslatorRulesGroup* parseTreeGroupRef = component->parseTreeGroupRef;
				//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
				parseTreeGroupRefParseTreeGroupRefReverseOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverse);
				parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex);
				parseTreeGroupRef->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
				parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = i;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		}
		#endif
		GIAtxtRelTranslatorRulesComponent* higherLevelComponent = NULL;
		GIAtxtRelTranslatorRulesGroup* higherLevelComponentParseTreeGroupRefOrig = NULL;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
		//bool foundPreviousActiveWordOrig = forwardPropogationSignalData->foundPreviousActiveWord;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
		vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig;
		#endif
		#endif
		if(activationSequenceCompleted)
		{
			if(lastComponentFuzzyAndCompatible)
			{
				if(activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse != NULL)
				{
					//cout << "lastComponentFuzzyAndCompatible" << endl;
					//exit(EXIT_ERROR);

					//replace parseTreeGroupRef of previously activated parent component
					/*
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->groupName = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->groupName << endl;
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex << endl;
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components.size() = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components.size() << endl;
					*/
					higherLevelComponent = activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components[activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex];
					/*
					cout << "higherLevelComponent->groupRefName = " << higherLevelComponent->groupRefName << endl;
					cout << "done" << endl;
					*/
					higherLevelComponentParseTreeGroupRefOrig = higherLevelComponent->parseTreeGroupRef;
					higherLevelComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroupOwner;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = true;
					#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
					previousWordConnectionsOrig = forwardPropogationWordDataPreviousRecord->previousWordConnections;
					if(find(forwardPropogationWordDataPreviousRecord->previousWordConnections.begin(), forwardPropogationWordDataPreviousRecord->previousWordConnections.end(), forwardPropogationWordData) == forwardPropogationWordDataPreviousRecord->previousWordConnections.end())
					{
						forwardPropogationWordDataPreviousRecord->previousWordConnections.push_back(forwardPropogationWordData);
						//cout << "forwardPropogationWordDataPreviousRecord->previousWordConnections.push_back(forwardPropogationWordData); forwardPropogationWordDataPreviousRecord->wordReference->tagName = " << forwardPropogationWordData->wordReference->tagName << endl;
						//exit(EXIT_ERROR);
					}
					#endif
					#endif					
				}
			}
		}
		#endif
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
		//b) enforce solidification of previous active component subgroup
		if(activationPathWordCurrentParseTreeGroupOwner->components.size() >= 2)
		{
			int indexOfPreviousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components.size()-2;
			GIAtxtRelTranslatorRulesComponent* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
			solidifyNeuralNetBackpropGroupLastComponent(component->parseTreeGroupRef, layer); 
		}
		#else
		newParseComponent->parseTreeGroupRef->solidified = true;
		#endif
		#endif
		
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(lastComponentFuzzyAndCompatible && activationSequenceCompleted)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible && activationSequenceCompleted" << endl;
			#endif

			//both a) propagate up and  b) pass next word

			//activation sequence completed (and not top level group), propagate next layer up
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}	
			else
			{
				/*
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
				forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
				#endif
				*/

				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif

				//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used	

				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					result = true;
				}
			}	
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(lastComponentFuzzyAndCompatible)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible" << endl;
			#endif
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
		#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
			
		
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	bool sentenceWordDataFullyConnected = isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeNameBackup << ":" << ownerGroupOrig->groupName << ", activationMemoryGroupArrayIndex = " << debug->activationMemoryGroupArrayIndex << ", firstComponentActive = " << debug->firstComponentActive << ", activationMemoryGroupArraySize = " << debug->activationMemoryGroupArraySize << ", isSentenceWordDataFullyConnected(forwardPropogationSentenceData) = " << sentenceWordDataFullyConnected << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	
	bool passTemp = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(passThrough)
	{
		passTemp = true;
	}
	else
	{
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(!(lastComponentFuzzyAndCompatible && activationSequenceCompleted))
		{
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!lastComponentFuzzyAndCompatible)
		{
		#endif
			passTemp = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		}
		#endif	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
	if(passTemp)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "propagateWordThroughNetworkGroupComponent: !lastComponentFuzzyAndCompatible, activationSequenceCompleted = " << activationSequenceCompleted << endl;
		#endif
		if(activationSequenceCompleted)
		{
			//cout << "propagateWordThroughNetworkGroupComponent: repeatDetected = " << repeatDetected << endl;

			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "activationSequenceCompleted" << endl;
			#endif

			GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
			if(!passThrough)
			{	
			#endif
			
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif
				semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);

				//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
			}
			else
			{
				semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;	//only used for forwardPropogationSignalData->foundPreviousActiveWord
			}
			#endif
			
			//cout << "DEBUG printBackpropParseTree:" << endl;
			//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeNameBackup, ownerGroupOrig->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
				if(passThrough)
				{	
					result = true;
				}
				else
				{
				#endif
					//cout << "topLevelGroup" << endl;
					//exit(0);

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
					if(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents->size()-1)	//end of sentence punctuation mark found
					{
						if(isSentenceWordDataFullyConnected(forwardPropogationSentenceData))
						{
							/*
							cout << "topLevelGroup && isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
							exit(EXIT_ERROR);
							*/
					#endif

							result = true;
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

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
						}
						else
						{
							result = false;

							/*
							cout << "topLevelGroup && !isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
							exit(EXIT_ERROR);
							*/
						}
					}
					else
					{
						cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
				}	
				#endif
			}
			else
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					result = true;
				}
				else
				{
					//ownerGroupOrig->neuronPropagated = false;
				}
			}
		}
		else
		{
			/*
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeNameBackup << ":" << ownerGroupOrig->groupName << endl;	
			GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
			*/

			//cout << "propagateWordThroughNetworkGroupComponentPassNextWord" << endl;
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
	}


	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{	
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!result)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig;
			activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig;
			if(activationSequenceCompleted)
			{
			#endif
				for(int i=0; i<activationPathWordCurrentParseTreeGroupOwner->components.size(); i++)
				{
					GIAtxtRelTranslatorRulesComponent* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
					GIAtxtRelTranslatorRulesGroup* parseTreeGroupRef = component->parseTreeGroupRef;
					parseTreeGroupRef->parseTreeGroupRefReverse = parseTreeGroupRefParseTreeGroupRefReverseOrig[i];
					parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig[i];
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			}
			#endif
			if(activationSequenceCompleted)
			{
				if(lastComponentFuzzyAndCompatible && (higherLevelComponent != NULL))
				{
					higherLevelComponent->parseTreeGroupRef = higherLevelComponentParseTreeGroupRefOrig;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = foundPreviousActiveWordOrig;
					#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
					forwardPropogationWordDataPreviousRecord->previousWordConnections = previousWordConnectionsOrig;
					#endif
					#endif
				}
			}
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}	
	#endif
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level)
{
	bool result = true;

	currentParseTreeGroup->solidified = true;
	
	if(currentParseTreeGroup->components.size() > 0)
	{
		int indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
		GIAtxtRelTranslatorRulesComponent* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
		solidifyNeuralNetBackpropGroupLastComponent(lastComponent->parseTreeGroupRef, level+1);
	}
		
	return result;
}
#endif
#endif



/*
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::findComponentRef(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesComponent* componentRef, GIAtxtRelTranslatorRulesComponent** componentFound)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
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

void GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* currentWordData)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough)
{
	bool result = false;
	bool pass = true;
	bool expectToSeeConnectionWithPreviousWordTrace = BOOL_IRRELEVANT;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(passThrough)
	{
		if(forwardPropogationSignalData->foundPreviousActiveWord)	//FUTURE GIA? - change to; found any previous active word
		{
			pass = false;
			result = true;	//foundPreviousActiveWord in hypothetical activation path 
			//cout << "forwardPropogationSignalData->foundPreviousActiveWord = " << forwardPropogationSignalData->foundPreviousActiveWord << endl;
		}
		else
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES
			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeNameBackup, ownerGroupOrig->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(!topLevelGroup)
			{
				//hypothesise that activation sequence is completed (and not top level group), propagate next layer up
				bool passThroughNew = true;
				
				//cout << "propagateWordThroughNetworkGroup START: [GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
				GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData; //GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, forwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					//cout << "propagateWordThroughNetworkGroup pass [GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
					pass = false;
					result = true;
				}
				else
				{
					//cout << "propagateWordThroughNetworkGroup fail [GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
					pass = false;
					result = false;
				}
			}
			else
			{
				pass = false;
				result = false;			
			}				
			#else
			pass = false;
			result = false;	//irrelevant
			#endif
		}
	}
	else
	{
		if(forwardPropogationWordData->w > 0)	//first word does not require previousWord connectivity
		{
			if(forwardPropogationSignalData->foundPreviousActiveWord)	//FUTURE GIA? - change to; found any previous active word
			{
				pass = true;
			}
			else
			{	
				//activate passThrough
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
				if(isWordPOStypeNoun(forwardPropogationWordData->wordPOStype))
				{
				#endif
				
					bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeNameBackup, ownerGroupOrig->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
					if(!topLevelGroup)
					{
						//hypothesise that activation sequence is completed (and not top level group), propagate next layer up
						bool passThroughNew = true;
						
						/*
						if(lastComponentFuzzyAndCompatible)
						{
							cout << "propagateWordThroughNetworkGroup: lastComponentFuzzyAndCompatible, ownerGroupOrig->groupName = " << ownerGroupOrig->groupName << endl;
						}
						*/
						
						//cout << "propagateWordThroughNetworkGroup START:" << endl;
						GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData;	//GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
						if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, forwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroup, passThroughNew))
						{
							//cout << "propagateWordThroughNetworkGroup pass" << endl;
							//exit(EXIT_ERROR);
							
							pass = true;
						}
						else
						{
							//cout << "propagateWordThroughNetworkGroup fail" << endl;
							pass = false;
							result = false;
						}
					}
					else
					{
						pass = false;
						result = false;		
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
				}
				else
				{
					pass = true;
				}
				#endif
			}		
		}
		else
		{
			pass = true;
		}
	}
	#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
	if(lastComponentFuzzyAndCompatible)
	{
		forwardPropogationSignalData->foundPreviousActiveWord = true;	
	}
	#endif
	#endif
	//cout << "forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = " << forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace << ", forwardPropogationSignalData->foundPreviousActiveWord = " << forwardPropogationSignalData->foundPreviousActiveWord << endl;
	if(forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace)
	{
		pass = false;
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			pass = true;
		}
		else
		{
			//cout << "!pass" << endl;
		}
	}
	if(pass)
	{
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
		
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroupOrig = " << ownerGroupOrig->groupName << ", ownerGroupOrig->groupTypeNameBackup = " << ownerGroupOrig->groupTypeNameBackup << endl;
				
		//pass next word though network
		expectToSeeConnectionWithPreviousWordTrace = false;
		forwardPropogationWordData->foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			expectToSeeConnectionWithPreviousWordTrace = false;
		}
		else
		{
			if(forwardPropogationWordData->w > 0)
			{
				GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPrevious = (forwardPropogationSentenceData->forwardPropogationWordDataArray[forwardPropogationWordData->w-1]);
				if(forwardPropogationWordDataPrevious->foundPreviousActiveWord)
				{
					expectToSeeConnectionWithPreviousWordTrace = false;	//allow single (max 1) isolated words
				}
				else
				{
					expectToSeeConnectionWithPreviousWordTrace = true;
				}
			}
			else
			{
				expectToSeeConnectionWithPreviousWordTrace = false;
			}
		}
		#endif
	}
	#endif
		
	if(pass)
	{
		int w = forwardPropogationWordData->w + 1;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		cout << "forwardPropogationWordData->w+1 = " << forwardPropogationWordData->w+1 << endl;
		#endif
		
		bool passThroughNew = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, w, forwardPropogationSentenceData, layer+1, activationPathWordCurrentParseTreeGroupOwner, expectToSeeConnectionWithPreviousWordTrace, passThroughNew))
		{
			result = true;
		}
		else
		{
			//ownerGroupOrig->neuronPropagated = false;
		}
	}
		
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentWordConnectivityTests(GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;
		
	GIAtxtRelTranslatorRulesGroup* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		GIAtxtRelTranslatorRulesComponent* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		GIAtxtRelTranslatorRulesGroup* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
				
		int wordIndexMax = -1;
		if(!getMinMaxWordIndexInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, true, &wordIndexMax))
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentWordConnectivityTests error{}: !getMinMaxWordIndexInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, true)" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentWordConnectivityTests pass{}: !getMinMaxWordIndexInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, true)" << endl;	
			//exit(EXIT_ERROR);
		}
		wordIndexMax = SHAREDvars.maxInt(wordIndexMax, lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
		
		int wordIndexMin = REALLY_LARGE_INT;
		if(!getMinMaxWordIndexInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, false, &wordIndexMin))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentWordConnectivityTests error{} !getMinMaxWordIndexInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, false) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentWordConnectivityTests pass{} !getMinMaxWordIndexInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, false) && " << endl;
			//exit(EXIT_ERROR);
		}
		wordIndexMin = SHAREDvars.minInt(wordIndexMin, forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);

		//cout << "wordIndexMax = " << wordIndexMax << endl;
		//cout << "wordIndexMin = " << wordIndexMin << endl;
		if(wordIndexMin-wordIndexMax== 1)
		{
			result = true;
			//cout << "result == true" << endl;
			//exit(EXIT_ERROR);
		}
	}
	else
	{
		result = true;
	}
	
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::getMinMaxWordIndexInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin)
{
	bool result = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (currentParseTreeGroup->components)[i];
		if(findMaxOrMinWordIndex)
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
				result = true;
			}
		}
		else
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex < *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
				result = true;
			}
		}
						
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getMinMaxWordIndexInParseTree(currentComponent->parseTreeGroupRef, findMaxOrMinWordIndex, wordIndexMaxOrMin))
			{
				result = true;
			}
		}
	}
	
	return result;	
}
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentTests1(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::componentTests2(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::getFirstWordInParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceEntityIndexMin, int* numComponentsFound)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool lastComponentFuzzyAndCompatible, bool* firstActiveComponentInGroup, const bool passThrough)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	int numberOfInactiveComponentsRemaining = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	GIAtxtRelTranslatorRulesComponent* previousActiveComponent = NULL;
	#endif
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


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
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
			cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}














bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetAllNeurons(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetAllNeuronComponents(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, const int groupBoolIndexType)
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
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				resetGroupParseTreeGroupRef(group, true);
			}
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


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::isLastComponentFuzzy(GIAtxtRelTranslatorRulesGroup* group)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::groupPartiallyOrFullyActivated(GIAtxtRelTranslatorRulesGroup* group)
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
				
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group)
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetComponentsActivation(vector<GIAtxtRelTranslatorRulesComponent*>* components)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	//CHECKTHIS;
	GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = new GIAtxtRelTranslatorRulesGroup(*group);
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	GIAtxtRelTranslatorRulesGroup* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		GIAtxtRelTranslatorRulesGroup* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	GIAtxtRelTranslatorRulesGroup* tempMemoryGroup = copyGroup(group);
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif










#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::updatePerformance(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	//cout << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::updatePerformance" << endl;
		 
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

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int level, bool print, bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
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
			else
			{					
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
			}
		}	
	}
	
	return result;
}



bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, const int groupBoolIndexType)
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
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::resetNeuronForwardProp(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
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



bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::deleteGroup(GIAtxtRelTranslatorRulesGroup* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::deleteComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components)
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
GIAtxtRelTranslatorRulesGroup* GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::copyGroup(GIAtxtRelTranslatorRulesGroup* group)
{		
	GIAtxtRelTranslatorRulesGroup* newGroup = new GIAtxtRelTranslatorRulesGroup(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));
	
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	
	return newGroup;
}
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::copyComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIAtxtRelTranslatorRulesComponent*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (*components)[i];
		GIAtxtRelTranslatorRulesComponent* newComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);
		componentsNew->push_back(newComponent);
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			newComponent->subComponents.clear();
			copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
		}
	}
	
	return result;
}
#endif


bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::doesRequireResetGroupActivation(GIAtxtRelTranslatorRulesComponent* Xcomponent, int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
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
				cerr << "GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
			}
		}
		else
		{
			requireResetGroupActivation = true;
		}
	}
	
	return requireResetGroupActivation;
}
						

bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::findNextAvailableComponent(int indexOfXcomponentInGroup, GIAtxtRelTranslatorRulesGroup* ownerGroup)
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
	
	
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
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


int GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool GIAtxtRelTranslatorNeuralNetworkUnoptimisedClass::isWordPOStypeNoun(int wordPOStype)
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



#endif
