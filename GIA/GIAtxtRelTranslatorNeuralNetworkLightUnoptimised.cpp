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
 * File Name: GIAtxtRelTranslatorNeuralNetworkLightUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11g 01-March-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Light Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkLightUnoptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_UNOPTIMISED



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO
vector<GIAtxtRelTranslatorRulesGroup*> parseTreeGroupsArray;
#endif


bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	topLevelParseTreeGroupLocal = NULL;
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO
	parseTreeGroupsArray.clear();
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
	
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
	firstExecution = false;
	#endif
	
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		int w = sentenceContents->size()-1;
		#else
		int w = 0;
		#endif
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup))
		{
			result = false;
		}
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
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
		cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
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
		
		/*
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
		}
		*/
	}
	else
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	//cout << "w = " << w << endl;
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

	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
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
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorPOStagger.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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
		cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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
							cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
						{
							result = true;
						}
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))	//what is this for?
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

	
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroup* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}	
	
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroup* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}				
				

bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;
	#endif
			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
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
		
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
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
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
				GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
				cout << "2a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
				#endif

				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				if(GIAtxtRelTranslatorNeuralNetworkOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
				{
				#endif
						
					if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
					}

					GIAtxtRelTranslatorDebug debug;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
					debug.firstComponentActive = ownerGroup->components[0]->neuronComponentConnectionActive;
					if(ownerGroup->components.size() >= 2)
					{
						debug.secondComponentActive = ownerGroup->components[1]->neuronComponentConnectionActive;
					}
					#endif

					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
					GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
					cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
					//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
					GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
					#endif

					if(currentComponent->semanticRelationReturnEntity)
					{
						ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
					}

					bool deleteNewlyCreatedMemoryGroup = false;
					if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &debug))
					{
						result = true;
					}

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
	


bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, int groupFrontComponentConnectionListIndex, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
	#endif
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	*/
	
	GIAtxtRelTranslatorRulesComponent* previousActiveComponent = NULL;	//NOTUSED
	GIAtxtRelTranslatorRulesComponent* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
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
					//cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(componentTests2(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3c: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(GIAtxtRelTranslatorNeuralNetworkOperations.componentWordConnectivityTests(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, debug))
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
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	if(!result)
	{
		forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
	}
	#endif
	*/
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, int groupFrontComponentConnectionListIndex, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_MEM
	GIAtxtRelTranslatorRulesGroup* ownerGroupOrig = GIAtxtRelTranslatorRules.copyGroup(ownerGroup);
	#else
	GIAtxtRelTranslatorRulesGroup* ownerGroupOrig = NULL;
	#endif
	#endif

	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponents;
	GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO2
	//newParseComponent->componentRef = currentComponent;
	newParseComponent->groupFrontComponentConnectionListIndex = groupFrontComponentConnectionListIndex;
	newParseComponent->numberOfWordsCurrentlyParsed = forwardPropogationWordData->w;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO
	if(currentParseTreeGroupTemp->components.size() == 0)
	{
		parseTreeGroupsArray.push_back(currentParseTreeGroupTemp);
	}
	#endif
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	newParseComponent->parseTreeGroupRef->solidified = true;
	#endif
	*/
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	//currentParseTreeGroupTemp->components.push_front(newParseComponent);
	currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	#else
	currentParseTreeGroupTemp->components.push_back(newParseComponent);
	#endif
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", firstComponentActive = " << debug->firstComponentActive << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif

	/*
	if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
		exit(EXIT_ERROR);
	}
	*/
				
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
		GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupActivation(ownerGroup);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		//create new currentParseTreeGroupTemp for ownerGroup;
		//cout << "GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupParseTreeGroupRef" << endl;
		GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupParseTreeGroupRef(ownerGroup, false);
		/*
		if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
		{
			cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
			exit(EXIT_ERROR);
		}
		*/
		#endif
		#endif	
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);

		bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeNameBackup, ownerGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			if(forwardPropogationWordData->w == 0)	//start of sentence found
			#else
			if(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents->size()-1)	//end of sentence punctuation mark found
			#endif
			{
			#endif
				if(GIAtxtRelTranslatorNeuralNetworkOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
				{
					/*
					cout << "topLevelGroup && GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
					exit(EXIT_ERROR);
					*/

					result = true;
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
					forwardPropogationSentenceData->toplevelGroupActivationFound = true;
					topLevelParseTreeGroupLocal = activationPathWordCurrentParseTreeGroupOwner;
					#endif
					/*
					cout << "topLevelGroup" << endl;
					cout << "finishedPassingSentenceWords (temp exit)" << endl;
					exit(0);
					*/
				}
				else
				{
					//fail
					restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, true);
				}	

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
			}
			else
			{
				//activation sequence completed (and top level group), propagate next word
				if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				else
				{
					//ownerGroupOrig->neuronPropagated = false;
					restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, true);
					
					if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
					{
						cerr << "4 (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
						exit(EXIT_ERROR);
					}
				}
				
				/*
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
				cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == 0)" << endl;
				#else
				cerr << "GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
				#endif
				cerr << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
				exit(EXIT_ERROR);
				*/
			}
			#endif
		}
		else
		{
			//activation sequence completed (and not top level group), propagate next layer up
			if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
			{
				result = true;
			}
			else
			{
				//ownerGroupOrig->neuronPropagated = false;
				restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, true);
				
				if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
				{
					cerr << "5 (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
					exit(EXIT_ERROR);
				}

			}
		}
	}
	else
	{
		//activation sequence completed, propagate next word
		if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
		{
			result = true;
		}
		else
		{
			//#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_MEM
			restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, false);
			//#endif
		}
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_MEM
	GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(ownerGroupOrig);
	#endif
	#endif


	return result;
}


void GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::restoreGroupActivations(GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool restoreCurrentParseTreeGroupTemp)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_NEW
	if(restoreCurrentParseTreeGroupTemp)
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.restoreGroupParseTreeGroupRef(ownerGroup, activationPathWordCurrentParseTreeGroupOwner);	//restore original group from memory
	}
	//cout << "ownerGroup->currentParseTreeGroupTemp = " << ownerGroup->currentParseTreeGroupTemp->groupName << endl;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_MEM
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	//ownerGroup->currentParseTreeGroupTemp.pop_front();
	GIAtxtRelTranslatorRulesComponent* latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[0];
	ownerGroup->currentParseTreeGroupTemp->components.erase(ownerGroup->currentParseTreeGroupTemp->components.begin());
	#else
	GIAtxtRelTranslatorRulesComponent* latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[ownerGroup->currentParseTreeGroupTemp->components.size()-1];
	ownerGroup->currentParseTreeGroupTemp->components.pop_back();
	#endif
	delete latestParseTreeComponent;
	GIAtxtRelTranslatorNeuralNetworkOperations.restoreGroupActivation(ownerGroup, ownerGroupOrig);
	//NOTNECESSARY: GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupParseTreeGroupRef(ownerGroup, true);
	#endif
		
	#else
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_BIO
	for(int i2=0; i2<parseTreeGroupsArray.size(); i2++)
	{
		GIAtxtRelTranslatorRulesGroup* parseTreeGroup = parseTreeGroupsArray[i2];
		if(parseTreeGroup == activationPathWordCurrentParseTreeGroupOwner)	//this check is only allowed if GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::restoreGroupActivations is always executed when if(!propagateWordThroughNetworkGroup) / if(!propagateWordThroughNetworkGroupComponentPassNextWord)
		{
			for(int i1=0; i1<parseTreeGroup->components.size(); i1++)
			{
				GIAtxtRelTranslatorRulesComponent* parseTreeComponent = (parseTreeGroup->components)[i1];
				int parseTreeComponentWordIndex = GIAtxtRelTranslatorNeuralNetworkOperations.convertSentenceEntityIndexToWordIndex(parseTreeComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
				if(parseTreeComponentWordIndex <= forwardPropogationWordData->w)
				#else
				if(parseTreeComponentWordIndex >= forwardPropogationWordData->w)
				#endif
				{
					GIAtxtRelTranslatorRulesComponent* component = parseTreeComponent->componentRef;
					component->neuronComponentConnectionActive = false;
					parseTreeGroup->components.erase(parseTreeGroup->components.begin() + i1); //delete parseTreeComponent from parseTreeGroup
					i1--;
					delete parseTreeComponent;
				}
			}
			if(parseTreeGroup->components.size() == 0)
			{
				parseTreeGroupsArray.erase(parseTreeGroupsArray.begin() + i2); //delete parseTreeGroup from array
				i2--;
				//NOTNECESSARY: GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupParseTreeGroupRef(ownerGroup, true);
			}
		}
	}
	#else
	
	#endif
	
	
	#endif
}

bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	int wNext = forwardPropogationWordData->w - 1;
	#else
	int wNext = forwardPropogationWordData->w + 1;
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
	cout << "forwardPropogationWordData->w+1 = " << forwardPropogationWordData->w+1 << endl;
	#endif
	
	bool passThroughNew = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	if(wNext >= 0)
	#else
	if(wNext < forwardPropogationSentenceData->sentenceContents.size())
	#endif
	{
		if(propagateWordThroughNetworkIntro(translatorVariables, wNext, forwardPropogationSentenceData, layer+1, NULL))
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






#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::componentTests2(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype
	OLD:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the wordPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous word POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	int currentWordIndex = forwardPropogationWordData->w;
	if(currentWordIndex > 0)	//CHECKTHIS
	{
		if(group->previousWordPOStype != "")
		{
			componentTests = false;
			int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);

			if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, currentWordIndex, previousWordPOStype))		
			{
				//cout << "componentTests = true" << endl;
				componentTests = true;
			}
		}
		if(group->existsPreceedingWordPOStype != "")
		{
			componentTests = false;
			int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);

			if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, currentWordIndex, existsPreceedingWordPOStype))		
			{
				componentTests = true;
			}
		}
	}
	
	return componentTests;
}

bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	GIApreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentWordIndex)
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
		if(GIAtxtRelTranslatorNeuralNetworkOperations.checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentWordIndex)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}
#endif





bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAtxtRelTranslatorNeuralNetworkLightUnoptimisedClass::groupActivationCompleted(GIAtxtRelTranslatorRulesGroup* group)
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
				






#endif
