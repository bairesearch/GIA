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
 * File Name: GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3h3b 24-April-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Heavy Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimised.hpp"
#include "SHAREDvars.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_HEAVY_UNOPTIMISED



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocal;
#endif


bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
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
	
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	
	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
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
		cerr << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
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
		cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
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
		GIAtxtRelTranslatorNeuralNetworkOperations.traceBackpropParseTree(topLevelParseTreeGroupLocal, 1, print, performancePreprocess, &performanceNOTUSED, NULL);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents
		GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
		GIAtxtRelTranslatorNeuralNetworkOperations.resetNeuronBackprop(topLevelParseTreeGroupLocal, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	//added GIA3g6aTEMP32 
		
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
		GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough)
{
	bool result = false;

	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	GIApreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
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
		
	
	if(!GIAtxtRelTranslatorNeuralNetworkOperations.currentWordAmbiguous(currentWord))
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
	
			
	
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
							cerr << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
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
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
		
	return result;
}

	
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
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
	
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroupNew(inputLayerGroup);	//new GIAtxtRelTranslatorRulesGroupParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroup(inputLayerGroup));	//CHECKTHIS
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
				

bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{
	#endif
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
	#endif
	/*
	if(passThrough)
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;		
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
		
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
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
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig = ownerGroup;

				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
				GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
				cout << "2a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
				#endif

				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				if(GIAtxtRelTranslatorNeuralNetworkOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
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
						GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray.back();
						#endif
						#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION
						GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray.back();
						#endif
						#endif
						
						/*
						cout << "activationMemoryGroup->components.size() = " << activationMemoryGroup->components.size() << endl;
						cout << "activationMemoryGroup->lastWordIndexActivated = " << activationMemoryGroup->lastWordIndexActivated << endl;
						cout << "forwardPropogationWordData->wordReference->translatorSentenceEntityIndex = " << forwardPropogationWordData->wordReference->translatorSentenceEntityIndex << endl;
						*/
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						if(activationMemoryGroup->lastWordIndexActivated != GIAtxtRelTranslatorNeuralNetworkOperations.convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex))
						{
						#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
							if(!(currentParseTreeGroup->solidified))	//this saves time by considering only recently activated groups
							{
							#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
								GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = NULL;
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
								GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroupNew = NULL;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
								int activationMemoryGroupLastWordIndexActivatedOrig = INT_DEFAULT_VALUE;
								#endif
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
								GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupNew = NULL;
								#endif
								
								GIAtxtRelTranslatorRulesGroupActivationMemory* activationMemoryGroupTemp = NULL;
								GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = NULL;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
									activationMemoryGroupNew = GIAtxtRelTranslatorRules.copyGroup(activationMemoryGroup);
									ownerGroupOrig->activationMemoryGroupArray.push_back(activationMemoryGroupNew);
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroupLastWordIndexActivatedOrig = activationMemoryGroup->lastWordIndexActivated;
									activationMemoryGroup->lastWordIndexActivated = GIAtxtRelTranslatorNeuralNetworkOperations.convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
									//NOTREQUIRED: activationMemoryGroupNew->lastWordIndexActivated = GIAtxtRelTranslatorNeuralNetworkOperations.convertSentenceEntityIndexToWordIndex(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex); 
									#endif
									#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
									currentParseTreeGroupNew = GIAtxtRelTranslatorRules.copyGroup(currentParseTreeGroup);	//create a new parse tree group (in case of 1. first encounter with group or 2. recursion into different group)
									ownerGroupOrig->currentParseTreeGroupArray.push_back(currentParseTreeGroupNew);
									#endif				

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
								GIAtxtRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupOwnerComponent = activationMemoryGroupTemp->components[ownerComponentIndex];
								GIAtxtRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
								if(currentComponentOrig->isSubcomponent)
								{
									if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
									{
										activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
									}
									else
									{
										cerr << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect{} error: (currentComponent->isSubcomponent) && !(activationMemoryGroupOwnerComponent->hasSubComponents)" << endl;
										cout << "index = " << index << endl;
										cout << "ownerComponentIndex = " << ownerComponentIndex << endl;
										cout << "componentIndex = " << componentIndex << endl;
										GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  group->groupTypeName << ":" << group->groupName << endl;	
										GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
										cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
										cout << "ownerComponent->componentType = " << ownerComponent->componentType << endl;
										GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);	
										GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(ownerComponent, layer+1);
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
								GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
								cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
								//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
								GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
								#endif

								//int currentMemoryGroupArrayNewlyCreatedIndex = ownerGroup->currentParseTreeGroupArray.size()-1;	//CHECKTHIS; is this required for groupArray deletion?


								if(currentComponent->semanticRelationReturnEntity)
								{
									activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
								}

								int indexOfFirstComponentInGroup = 0;
								int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
								GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
								GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
								bool lastComponentFuzzyAndCompatible = false;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
								bool lastComponentOptionalAndInactiveAndCompatible = false;
								bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
								if(GIAtxtRelTranslatorNeuralNetworkOperations.isLastComponentFuzzy(ownerGroup))
								{
									if(lastComponent == ownerComponent)
									{
										if(lastComponent->neuronComponentConnectionActive)
										{
											if(lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
											{
												//OLD bug: if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->w - 1)
												if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == GIAtxtRelTranslatorNeuralNetworkOperations.convertWordIndexToSentenceEntityIndex(forwardPropogationWordData->w - 1))
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
								if(GIAtxtRelTranslatorNeuralNetworkOperations.isLastComponentFuzzy(ownerGroup))
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
									//cout << "GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupActivation: ownerGroup = " << ownerGroup->groupName << endl;
									GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								*/
								
								//changed 3g5a (introduce more simple reset detection method);
								bool requireResetGroupActivation = false;
								if(firstComponent == ownerComponent)
								{
									if(GIAtxtRelTranslatorNeuralNetworkOperations.doesRequireResetGroupActivation(firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationWordData))
									{
										requireResetGroupActivation = true;
									}
								}
								else if(ownerGroup->components.size() >= 1)
								{
									int indexOfSecondComponentInGroup = 1;
									GIAtxtRelTranslatorRulesComponentNeuralNetwork* secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];

									if((secondComponent == ownerComponent) && (firstComponent->optional))
									{
										if(GIAtxtRelTranslatorNeuralNetworkOperations.doesRequireResetGroupActivation(secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationWordData))
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
									GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupActivation(ownerGroup);
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
										GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(currentParseTreeGroupNew);
										ownerGroupOrig->currentParseTreeGroupArray.pop_back();
										#endif
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
										GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(activationMemoryGroupNew);
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
	


bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;
	if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			if(!firstActiveComponentInGroup || GIAtxtRelTranslatorNeuralNetworkOperations.componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(GIAtxtRelTranslatorNeuralNetworkOperations.componentWordConnectivityTestsWrapper(activationPathWordCurrentParseTreeGroupOwner, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData))
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
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPreviousRecord = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* previousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components[activationPathWordCurrentParseTreeGroupOwner->components.size()-1];
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
			int wPrevious = GIAtxtRelTranslatorNeuralNetworkOperations.convertSentenceEntityIndexToWordIndex(previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
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
		//copied from GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponents;
		GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
		newParseComponent->componentRef = currentComponent;
		newParseComponent->neuronComponentConnectionActive = true;
		newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
		//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
		newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
		activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
		vector<GIAtxtRelTranslatorRulesGroupParseTree*> parseTreeGroupRefParseTreeGroupRefReverseOrig;
		vector<int> parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse;
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
				GIAtxtRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
				GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
				//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
				parseTreeGroupRefParseTreeGroupRefReverseOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverse);
				parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex);
				parseTreeGroupRef->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
				parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = i;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		}
		#endif
		GIAtxtRelTranslatorRulesComponentParseTree* higherLevelComponent = NULL;
		GIAtxtRelTranslatorRulesGroupParseTree* higherLevelComponentParseTreeGroupRefOrig = NULL;
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
			GIAtxtRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
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
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
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
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
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
	//bool sentenceWordDataFullyConnected = GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	bool sentenceWordDataFullyConnected = false;
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << ", activationMemoryGroupArrayIndex = " << debug->activationMemoryGroupArrayIndex << ", firstComponentActive = " << debug->firstComponentActive << ", activationMemoryGroupArraySize = " << debug->activationMemoryGroupArraySize << ", GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData) = " << sentenceWordDataFullyConnected << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		cout << "propagateWordThroughNetworkGroupComponent: !lastComponentFuzzyAndCompatible, activationSequenceCompleted = " << activationSequenceCompleted << endl;
		#endif
		if(activationSequenceCompleted)
		{
			//cout << "propagateWordThroughNetworkGroupComponent: repeatDetected = " << repeatDetected << endl;

			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
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

			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
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
					#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_OLD
						if(GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData))
						{
						#endif
					
							/*
							cout << "topLevelGroup && GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
							exit(EXIT_ERROR);
							*/

							result = true;
							forwardPropogationSentenceData->finishedPassingSentenceWords = true;

							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
							forwardPropogationSentenceData->toplevelGroupActivationFound = true;
							topLevelParseTreeGroupLocal = activationPathWordCurrentParseTreeGroupOwner;

							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
							if(!GIAtxtRelTranslatorNeuralNetworkOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
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

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_OLD
						}
						else
						{
							result = false;
						}
						#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
					}
					else
					{
						cerr << "GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
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
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
			cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;	
			GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
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
					GIAtxtRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
					GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
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
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;

	currentParseTreeGroup->solidified = true;
	
	if(currentParseTreeGroup->components.size() > 0)
	{
		int indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
		GIAtxtRelTranslatorRulesComponentParseTree* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
		solidifyNeuralNetBackpropGroupLastComponent(lastComponent->parseTreeGroupRef, level+1);
	}
		
	return result;
}
#endif
#endif



/*
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::findComponentRef(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesComponentParseTree* componentRef, GIAtxtRelTranslatorRulesComponentParseTree** componentFound)
{
	bool result = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		if(currentParseTreeComponent->componentRef == componentRef)
		{
			result = true;
			*componentFound = currentParseTreeComponent;
		}
	}
	
	return result;
}
*/


bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough)
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
			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
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
				if(GIAtxtRelTranslatorNeuralNetworkOperations.isWordPOStypeNoun(forwardPropogationWordData->wordPOStype))
				{
				#endif
				
					bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
		
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroupOrig = " << ownerGroupOrig->groupName << ", ownerGroupOrig->groupTypeName = " << ownerGroupOrig->groupTypeName << endl;
				
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











bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::groupActivationCompleted(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
bool GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::groupPartiallyOrFullyActivated(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool groupPartiallyActivated = false;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
		if(currentComponent2->neuronComponentConnectionActive)
		{
			groupPartiallyActivated = true;
		}
	}
	
	return groupPartiallyActivated;
}
#endif
				










/*
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkHeavyUnoptimisedClass::copyGroup(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{		
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newGroup = GIAtxtRelTranslatorRules.copyGroup(group);
	
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	
	return newGroup;
}
#endif
*/

#endif


#endif
