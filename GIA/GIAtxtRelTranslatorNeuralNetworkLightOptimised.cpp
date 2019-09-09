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
 * File Name: GIAtxtRelTranslatorNeuralNetworkLightOptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j1a 03-August-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkLightOptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocal;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
static int parseTreeMaxWeight;
#endif

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
vector<GIAtxtRelTranslatorRulesGroupParseTree*> parseTreeGroupList;
vector<GIAtxtRelTranslatorRulesComponentParseTree*> parseTreeComponentListExtra;
#endif


bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocal = NULL;
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	parseTreeMaxWeight = 0;
	#endif
	
	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData.parserEnabled = parserEnabled;
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
	
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork)


	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			parseTreeGroupList.push_back(group->currentParseTreeGroupTemp);
		}
	}
	#endif


	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	for(int w=sentenceContents->size()-1; w>=0; w--)
	#else
	for(int w=0; w<sentenceContents->size(); w++)
	#endif
	{
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup))
		{
			result = false;
		}
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData.GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY);
	#endif
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		//cout << "topLevelParseTreeGroupLocal" << endl;
		
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
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
		
		/*
		cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
		exit(EXIT_ERROR);
		*/
	}
	else
	{
		*performance = 0;		
	}
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPLICATE_TOP_LEVEL_PARSE_TREE	
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
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	for(int i2=0; i2<parseTreeGroupList.size(); i2++)
	{
		GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupTemp = parseTreeGroupList[i2];
		GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(parseTreeGroupTemp);
	}
	parseTreeGroupList.clear();
	for(int i2=0; i2<parseTreeComponentListExtra.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* parseTreeComponentTemp = parseTreeComponentListExtra[i2];
		delete parseTreeComponentTemp;
	}
	parseTreeComponentListExtra.clear();
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
			group->currentParseTreeGroupTemp = NULL;
		}
	}
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData.forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	

	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
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
		
	
			
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
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
		cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	bool currentWordIsExplicitPOStypeTemp = false;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_EXPLICITWORDTEMP)
	{
		currentWordIsExplicitPOStypeTemp = true;
	}
	#else
	if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, GIA_PREPROCESSOR_POS_TYPE_EXPLICITWORDTEMP))	//CHECKTHIS
	{
		currentWordIsExplicitPOStypeTemp = true;
	}
	#endif
	if(!currentWordIsExplicitPOStypeTemp)
	{
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
								cerr << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
								exit(EXIT_ERROR);
							}

							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
							GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
							cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
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
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	GIAtxtRelTranslatorNeuralNetworkOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED);
	#endif
	
		
	return result;
}

	
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
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
	
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}				
				

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		
		bool preprocessSearchForNonResetActivations = false;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
		preprocessSearchForNonResetActivations = true;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, preprocessSearchForNonResetActivations))
		{
			result = true;
		}
		#endif

		preprocessSearchForNonResetActivations = false;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, preprocessSearchForNonResetActivations))
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
		
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif

	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
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
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			if(isWordInGroupOptimumPathwayWordList(forwardPropogationSentenceData, currentComponent, forwardPropogationWordData))
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
				if(!ownerGroup->neuronPropagated)	//prevent circular loops
				{
				#endif	
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

						//cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
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
						cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
						//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
						GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
						#endif

						if(currentComponent->semanticRelationReturnEntity)
						{
							ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
						}

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig = GIAtxtRelTranslatorRules.copyGroup(ownerGroup);
						GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupOrig = ownerGroupOrig->currentParseTreeGroupTemp;
						#endif

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
						int indexOfFirstComponentInGroup = ownerGroup->components.size()-1;
						int indexOfLastComponentInGroup = 0;
						#else
						int indexOfFirstComponentInGroup = 0;
						int indexOfLastComponentInGroup = ownerGroup->components.size()-1;				
						#endif
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

						int indexOfSecondComponentInGroup = INT_DEFAULT_VALUE;
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* secondComponent = NULL;
						if(ownerGroup->components.size() >= 1)
						{
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
							indexOfSecondComponentInGroup = indexOfFirstComponentInGroup-1;
							#else
							indexOfSecondComponentInGroup = 1;
							#endif
							secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];
						}

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

						bool measureSequentialActivationOnly = false;
						bool sequentialActivationFound = false;
						bool existingActivationFound = false;	
						bool resetUnallowed = false;

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
						if(preprocessSearchForNonResetActivations)
						{
							measureSequentialActivationOnly = true;
							sequentialActivationFound = false;
							existingActivationFound = false;
							resetUnallowed = false;
							propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFound, &resetUnallowed, &debug);
							if(sequentialActivationFound)
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
								ownerGroup->nonResetActivationFoundDuringPreprocess = true;
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
								if(!existingActivationFound)
								{
									ownerGroup->newActivationFoundDuringPreprocess = true;
									//cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupSelect{}: ownerGroup->newActivationFoundDuringPreprocess" << endl;
								}
								#endif
							}
						}
						else
						{
						#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
							bool nonResetActivationFoundDuringPreprocessLocal = false;
							bool newActivationFoundDuringPreprocessLocal = false;

							measureSequentialActivationOnly = true;
							sequentialActivationFound = false;
							existingActivationFound = false;
							resetUnallowed = false;	
							propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFound, &resetUnallowed, &debug);
							if(sequentialActivationFound)
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
								nonResetActivationFoundDuringPreprocessLocal = true;
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
								if(!existingActivationFound)
								{
									newActivationFoundDuringPreprocessLocal = true;
								}
								#endif
							}
							#endif

							//changed 3g5a (introduce more simple reset detection method);
							bool requireResetGroupActivation = false;
							bool requireResetGroupActivationGroupWasActive = false;
							if(firstComponent == ownerComponent)
							{

								bool firstMandatoryComponentActive = false;
								if(firstComponent->neuronComponentConnectionActive)
								{
									firstMandatoryComponentActive = true;
								}
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_ENSURE_FIRST_COMPONENT_ACTIVE_NEW
								else if(secondComponent != NULL)
								{
									if((firstComponent->optional) && (secondComponent->neuronComponentConnectionActive))
									{
										firstMandatoryComponentActive = true;		//changed @GIA3g11aTEMP70
									}
								}
								#endif

								if(firstMandatoryComponentActive)
								{
									if(GIAtxtRelTranslatorNeuralNetworkOperations.doesRequireResetGroupActivation(firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationWordData))
									{
										requireResetGroupActivation = true;
										//cout << "requireResetGroupActivation" << endl;
									}
								}
							}
							else if(ownerGroup->components.size() >= 1)
							{
								if((secondComponent == ownerComponent) && (firstComponent->optional))
								{
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_ENSURE_FIRST_COMPONENT_ACTIVE_NEW
									if((secondComponent->neuronComponentConnectionActive) || (firstComponent->neuronComponentConnectionActive))
									#else
									if(secondComponent->neuronComponentConnectionActive)
									#endif
									{
										if(GIAtxtRelTranslatorNeuralNetworkOperations.doesRequireResetGroupActivation(secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationWordData))
										{
											requireResetGroupActivation = true;
										}
									}
								}
							}


							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD
							if(requireResetGroupActivation)
							{	
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
								cout << "resetUnallowed = " << resetUnallowed << endl;
								#endif

								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
								if(!(ownerGroup->neuronPropagated))	//CHECKTHIS
								{
								#endif	
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
									if(ownerGroup->nonResetActivationFoundDuringPreprocess)
									{
										//cout << "nonResetActivationFoundDuringPreprocess" << endl;
										requireResetGroupActivation = false;
									}
									#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
									if(resetUnallowed)
									{
										//cout << "ownerGroup->resetUnallowed" << endl;
										//exit(EXIT_ERROR);
										requireResetGroupActivation = false;
									}
									#endif
									#endif

									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
									if(ownerGroup->neuronProcessed)	//note if sequentialActivationFound && existingActivationFound during preprocessSearchForNonResetActivations, then ownerGroup->neuronProcessed
									{
										requireResetGroupActivation = false;
										//cout << "ownerGroup->neuronProcessed" << endl;
									}		
									#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
								}
								#endif
							}
							#endif

							if(requireResetGroupActivation)
							{
								requireResetGroupActivationGroupWasActive = ownerGroup->neuronActive;

								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
								cout << "resetGroupActivation - " << ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
								#endif
								bool clearParseTreeGroup = false;
								#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(!(ownerGroup->neuronActive))
								{
									clearParseTreeGroup = true;
								}
								#endif

								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
								clearParseTreeGroup = false;
								#endif
								GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupActivation(ownerGroup);
								GIAtxtRelTranslatorNeuralNetworkOperations.resetGroupParseTreeGroupRef(ownerGroup, clearParseTreeGroup);
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
								parseTreeGroupList.push_back(ownerGroup->currentParseTreeGroupTemp);
								#endif
							}
							#endif


							/*
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
							GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
							if(currentParseTreeGroupTemp->solidified)
							{
								cout << "solidified" << endl;
								exit(EXIT_ERROR);
							}
							else
							{
							#endif
							*/

							measureSequentialActivationOnly = false;
							sequentialActivationFound = false;
							existingActivationFound = false;
							resetUnallowed = false;	//irrelevant
							if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFound, &resetUnallowed, &debug))
							{
								result = true;
								#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(requireResetGroupActivation)
								{
									if(!requireResetGroupActivationGroupWasActive)
									{
										GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(currentParseTreeGroupOrig);
									}
								}
								#endif
								#endif
							}
							else
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(requireResetGroupActivation)
								{
									#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
									cout << "restoreGroupActivation - " << ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
									#endif
									GIAtxtRelTranslatorNeuralNetworkOperations.restoreGroupActivation(ownerGroup, ownerGroupOrig);
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
									ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupOrig;
									#else
									GIAtxtRelTranslatorNeuralNetworkOperations.restoreGroupParseTreeGroupRef(ownerGroup, currentParseTreeGroupOrig);
									#endif
								}
								#endif
							}

							/*
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
							}
							#endif
							*/

							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_RESET
							//if(nonResetActivationFoundDuringPreprocessLocal)
							//{
								ownerGroup->nonResetActivationFoundDuringPreprocess = false;
							//}
							#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
							if(newActivationFoundDuringPreprocessLocal)
							{
								ownerGroup->newActivationFoundDuringPreprocess = false;
							}
							#endif

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS
						}
						#endif

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
						GIAtxtRelTranslatorNeuralNetworkOperations.deleteGroup(ownerGroupOrig);
						#endif
						#endif

					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
					}
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			}
			#endif
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFound, bool* resetUnallowed, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;

	/*	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
	#endif
	*/
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	*/
	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	
	if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent, existingActivationFound))
	{	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - PASS1" << endl;
		GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
		#endif
	
		//cout << "existingActivationFound = " << *existingActivationFound << endl;
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			//*existingActivationFound = false;	//added GIA3g11aTEMP62
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents), existingActivationFound))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAtxtRelTranslatorNeuralNetworkOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected, existingActivationFound))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
			cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(componentTests2(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted))
			{
			#endif
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
				GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
				cout << "3c: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
				#endif
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
				//determines if reset is allowed
				/*//OLD method;
				if(measureSequentialActivationOnly)
				{
					*resetUnallowed = false;
					if(!resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound, forwardPropogationSentenceData))
					{
						*resetUnallowed = true;
					}
				}
				*/
				*resetUnallowed = false;
				if(resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound, forwardPropogationSentenceData, firstActiveComponentInGroup))
				{
				#endif
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
					GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
					cout << "3c1: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
					#endif
					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
					if(GIAtxtRelTranslatorNeuralNetworkOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound))
					{
					#endif
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
						GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
						cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
						#endif

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
						if(verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound))
						{
						#endif
							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
							GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
							cout << "3d: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
							#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
							if(existingActivationVerifyMaxNumberWordsInParseTree(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSentenceData, layer, *existingActivationFound))
							{
							#endif
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
								GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
								cout << "3d2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
								#endif
								
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
								if(verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(ownerGroup, activationPathWordCurrentParseTreeGroup, layer))
								{
								#endif
									#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
									GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
									cout << "3d3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
									#endif

									*sequentialActivationFound = true;
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
									if(!measureSequentialActivationOnly)
									{
									#endif
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
										if(!(*existingActivationFound && ownerGroup->newActivationFoundDuringPreprocess))
										{
										#endif
											if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFound, debug))
											{
												result = true;
											}
										#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
										}
										else
										{
											cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper{}: existingActivationFound && ownerGroup->newActivationFoundDuringPreprocess" << endl;
										}
										#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
									}
									#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
								}
								#endif

							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
							}
							#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
						}
						#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
					}
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
				}
				else
				{
					*resetUnallowed = true;
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - FAIL1" << endl;	
		GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
		#endif	
	}
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
	if(!resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFound, forwardPropogationSentenceData, firstActiveComponentInGroup))
	{
		*resetUnallowed = true;
	}
	#endif
				
	
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
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool existingActivationFound, GIAtxtRelTranslatorDebug* debug)
{
	bool result = true;
	
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	if(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex < 0)
	{
		cout << "forwardPropogationWordData->wordReference->translatorSentenceEntityIndex = " << forwardPropogationWordData->wordReference->translatorSentenceEntityIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	//copied from GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponents;
	GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_BIO
	if(newParseComponent->parserForwardPropogationSignalData != NULL)
	{
		delete newParseComponent->parserForwardPropogationSignalData;
	}
	newParseComponent->parserForwardPropogationSignalData = new GIAtxtRelTranslatorParserForwardPropogationSignalData(forwardPropogationSignalData->parserForwardPropogationSignalData);
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_STREAMLINED
	forwardPropogationWordData->wordReference->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//!GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC: this will be set later by GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	#else
	//DOESNTWORK;
	if(newParseComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(newParseComponent->candidateStringMatch != NULL)
		{	
			cout << "forwardPropogationWordData->wordPOStype = " << forwardPropogationWordData->wordPOStype << endl;
			newParseComponent->candidateStringMatch->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//note this is only compatible with PARSE_SIMULTANEOUS because there is no guarantee the candidateStringMatch->wordPOStypeInferred from a good/successful propagation will not be overwritten with the data from a future bad/unsuccessful propagation
		}
	}
	#endif
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	if(currentComponent->semanticRelationReturnConnectionDynamic)
	{
		
	}
	#endif
	GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;


	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS
	if(existingActivationFound)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_TAKE_LAST_ACTIVATION_AS_PARSETREE
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		//currentParseTreeGroupTemp->components.pop_front();
		//cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
		//cout << "currentParseTreeGroupTemp->components.size() = " << currentParseTreeGroupTemp->components.size() << endl;
		GIAtxtRelTranslatorRulesComponentParseTree* oldParseComponent = currentParseTreeGroupTemp->components.front();
		currentParseTreeGroupTemp->components.erase(currentParseTreeGroupTemp->components.begin());
		#else
		GIAtxtRelTranslatorRulesComponentParseTree* oldParseComponent = currentParseTreeGroupTemp->components.back();
		currentParseTreeGroupTemp->components.pop();
		#endif
		//delete oldParseComponent;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
		parseTreeComponentListExtra.push_back(oldParseComponent);	//mark overwritten component for deletion
		#endif
		#else
		cerr << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupComponent: GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS: !GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_TAKE_LAST_ACTIVATION_AS_PARSETREE is not coded (planned implementation: store multiple parseTrees for each component activation)" << endl; 
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
		if(currentParseTreeGroupTemp->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			GIAtxtRelTranslatorRulesComponentParseTree* previousParseComponent = currentParseTreeGroupTemp->components.front();
			currentParseTreeGroupTemp->parseTreeMinWordIndex = previousParseComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;	//CHECKTHIS
			#else
			GIAtxtRelTranslatorRulesComponentParseTree* previousParseComponent = currentParseTreeGroupTemp->components.back();
			currentParseTreeGroupTemp->parseTreeMaxWordIndex = previousParseComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;	//CHECKTHIS
			#endif
			//updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, previousParseComponent);
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			GIAtxtRelTranslatorRulesGroupParseTree* childGroup = previousParseComponent->parseTreeGroupRef;
			if(childGroup != NULL)
			{
				currentParseTreeGroupTemp->parseTreeTotalWeight = currentParseTreeGroupTemp->parseTreeTotalWeight - childGroup->parseTreeTotalWeight;
			}
			#endif
		}
		else
		{
			currentParseTreeGroupTemp->parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
			currentParseTreeGroupTemp->parseTreeMinWordIndex = INT_DEFAULT_VALUE;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			currentParseTreeGroupTemp->parseTreeMaxWeight = 0.0;
			currentParseTreeGroupTemp->parseTreeMinWeight = 0.0;
			currentParseTreeGroupTemp->parseTreeTotalWeight = 0.0;
			#endif
		}
		#endif
	}
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	//currentParseTreeGroupTemp->components.push_front(newParseComponent);
	currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	#else
	currentParseTreeGroupTemp->components.push_back(newParseComponent);
	#endif
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	//cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex << endl;
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", activationSequenceCompleted = " << activationSequenceCompleted << endl;	
	GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	ownerGroup->neuronProcessed = true;
	#endif
	

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	//b) enforce solidification of previous active component subgroup
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() >= 2)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		int indexOfPreviousActiveComponent = 0+1;
		#else
		int indexOfPreviousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components.size()-2;
		#endif
		GIAtxtRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK1
		if(component->parseTreeGroupRef != NULL)
		{
			component->parseTreeGroupRef->solidified = true;
		}
		#else
		solidifyNeuralNetBackpropGroupLastComponent(component->parseTreeGroupRef, layer); 
		#endif
	}
	#else
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK3
	newParseComponent->solidified = true;
	#else
	newParseComponent->parseTreeGroupRef->solidified = true;
	#endif
	#endif
	#endif
					
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
		if(forwardPropogationSentenceData->parserEnabled)
		{
			//NO: only execute this function when reached the highest layer in activation tree;
			if(!GIAtxtRelTranslatorNeuralNetworkParser.generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(translatorVariables, activationPathWordCurrentParseTreeGroupOwner, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective), layer))
			{
				result = false;
			}
		}
		#endif
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);

		bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
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
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE
				if(GIAtxtRelTranslatorNeuralNetworkOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
				{
					//cout << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
				#endif
				
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					int maxWeight;
					GIAtxtRelTranslatorNeuralNetworkOperations.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, &maxWeight);
					/*
					cout << "maxWeight = " << maxWeight << endl;
					cout << "parseTreeMaxWeight2 = " << parseTreeMaxWeight << endl;
					*/
					if(maxWeight >= parseTreeMaxWeight)
					{
						parseTreeMaxWeight = maxWeight;
					#endif
						/*
						cout << "topLevelGroup && GIAtxtRelTranslatorNeuralNetworkOperations.isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
						exit(EXIT_ERROR);
						*/
	
						//result = true;
						forwardPropogationSentenceData->finishedPassingSentenceWords = true;
	
						forwardPropogationSentenceData->toplevelGroupActivationFound = true;
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_REPLICATE_TOP_LEVEL_PARSE_TREE
						GIAtxtRelTranslatorNeuralNetworkOperations.deleteParseTree(topLevelParseTreeGroupLocal, 0);
						topLevelParseTreeGroupLocal = GIAtxtRelTranslatorNeuralNetworkOperations.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
						#else
						//OLD: topLevelParseTreeGroupLocal = activationPathWordCurrentParseTreeGroupOwner;
						//copy currentParseTreeGroupTemp so it cant be overwritten;
						topLevelParseTreeGroupLocal = GIAtxtRelTranslatorRules.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
						#endif
						#endif
						
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
						cout << "1topLevelGroup" << endl;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
						cout << "parseTreeMaxWeight1 = " << parseTreeMaxWeight << endl;
						#endif
						cout << "topLevelParseTreeGroupLocal->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
						printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
						#endif
						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						*/
						
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					}
					else
					{
						//cout << "parseTreeMaxWeight2 = " << parseTreeMaxWeight << endl;
					}
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_PERFORMANCE
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
			}
			else
			{
				//cout << endl;
			}
			#endif
		}
		else
		{
			//cout << endl;
		
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


bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
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




#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK3
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, int level)
{
	bool result = true;

	if(group != NULL)
	{
		group->solidified = true;
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(level);
		cout << "group->solidified: " << group->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
		#endif
		
		if(group->components.size() > 0)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* lastComponent = getLastActiveComponent(group);
			solidifyNeuralNetBackpropGroupLastComponent(lastComponent->groupRef, level+1);
		}
	}
		
	return result;
}
#else
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;

	if(currentParseTreeGroup != NULL)
	{
		currentParseTreeGroup->solidified = true;
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(level);
		cout << "currentParseTreeGroup->solidified: " << currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
		#endif
		
		if(currentParseTreeGroup->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			int indexOfLastComponentInGroup = 0;
			#else
			int indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
			#endif
			GIAtxtRelTranslatorRulesComponentParseTree* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
			solidifyNeuralNetBackpropGroupLastComponent(lastComponent->parseTreeGroupRef, level+1);
		}
	}
		
	return result;
}
#endif
#endif
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::componentTests2(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted)
{
	bool componentTests = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	if(activationSequenceCompleted)
	{
	#endif
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
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	}
	#endif
	
	return componentTests;
}

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype)
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
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype)
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




#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
//determines if reset is allowed
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::resetAllowed(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup)
{
	bool result = false;
	
	if((ownerGroup->groupName == "general") && (ownerGroup->groupTypeName == "referenceSets"))	//CHECKTHIS magic strings
	{
		if(existingActivatedComponentCapturesLastWordInSentence(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFound, forwardPropogationSentenceData, firstActiveComponentInGroup))
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

//requires GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE;
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup)
{
	bool result = false;

	GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = NULL;
	GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_BIO_WEAK3
	if(!(ownerGroup->solidified))
	#else
	if(!(ownerGroupParseTreeGroup->solidified))
	#endif
	{
	#endif
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
			if(firstActiveComponentInGroup)
			#else
			if(existingActivationFound)
			#endif
			{
				//cout << "firstActiveComponentInGroup" << endl;
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
				int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				#else
				int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				#endif
				#else
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
				int lastActiveComponentInParseTreeIndex = 0;
				#else
				int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				#endif
				#endif

				GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];

				lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;

				if(existingActivatedComponentCapturesLastWordInSentence(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFound, forwardPropogationSentenceData))
				{
					result = true;
				}
			}
			else
			{
				//cout << "!firstActiveComponentInGroup" << endl;
				result = true;
			}
		}
		else
		{
			result = true;
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SOLIDIFY_NET_BACKPROP
	}
	else
	{
		//cout << "ownerGroupParseTreeGroup->solidified" << endl;
		result = true;
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence(GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//cout << "\t GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence{}: " << endl;
	
	bool result = false;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	bool calculateMaxOrMin = true;
	int wordIndexMax1 = -1;
	int wordIndexMax2 = -1;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax1 = lastActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax2 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMaxMin1 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;	//CHECKTHIS
	int wordIndexMaxMin2 = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;	//CHECKTHIS
	#else
	bool calculateMaxOrMin = false;
	int wordIndexMax1 = REALLY_LARGE_INT;
	int wordIndexMax2 = REALLY_LARGE_INT;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax1 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
	GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFindWordIndexMax2 = lastActiveComponentInParseTreeParseTreeGroupRef;
	int wordIndexMaxMin1 = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;
	int wordIndexMaxMin2 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
	#endif

	if(parseTreeGroupToFindWordIndexMax1 != NULL)
	{
		if(!GIAtxtRelTranslatorNeuralNetworkOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, calculateMaxOrMin, &wordIndexMax1, 0))
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax1 = SHAREDvars.maxInt(wordIndexMax1, wordIndexMaxMin1);
	
	if(parseTreeGroupToFindWordIndexMax2 != NULL)
	{
		if(!GIAtxtRelTranslatorNeuralNetworkOperations.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, calculateMaxOrMin, &wordIndexMax2, 0))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax2 = SHAREDvars.maxInt(wordIndexMax2, wordIndexMaxMin2);

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
	cout << "wordIndexMax1 = " << wordIndexMax1 << endl;
	cout << "wordIndexMax2 = " << wordIndexMax2 << endl;
	//cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	if(wordIndexMax1 == forwardPropogationSentenceData->sentenceContents->size()-1)	//ignore full stop
	#else
	if(wordIndexMax1 == 0)
	#endif
	{
		//existing activated component's parseTree contains last word in sentence 
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		if(wordIndexMax2 == forwardPropogationSentenceData->sentenceContents->size()-1)	//ignore full stop
		#else
		if(wordIndexMax2 == 0)
		#endif
		{
			//overwritten/newly activated component's parseTree contains last word in sentence 
			result = true;
		}
		
		/*
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD2
		if(wordIndexMax2 < wordIndexMax1-1)
		{
			result = true;	//entity refers to separate referenceSet:general group
		}
		#endif
		*/
	}
	else
	{
		result = true;
	}

	return result;
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFound)
{
	bool result = false;

	if(existingActivationFound)
	{
		GIAtxtRelTranslatorRulesGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = NULL;
		if(activationPathWordCurrentParseTreeGroup != NULL)
		{
			prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
		}
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree: existingActivationFound" << endl;
		#endif
		
		GIAtxtRelTranslatorRulesGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
		GIAtxtRelTranslatorRulesComponentParseTree* lastActiveComponentInParseTree = NULL;
		GIAtxtRelTranslatorRulesGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
			int lastActiveComponentInParseTreeIndex = 0;
			#else
			int lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
			#endif
			lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			if(lastActiveComponentInParseTree->parseTreeGroupRef != NULL)
			{
				lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			}
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
			int prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin;
			int prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax;
			int lastActiveComponentInParseTreeParseTreeWordIndexMin;
			int lastActiveComponentInParseTreeParseTreeWordIndexMax;
			if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->parseTreeMaxWordIndex;
			}
			else
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = forwardPropogationWordData->wordReference->translatorSentenceEntityIndex;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY_WEAK
			if(lastActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				lastActiveComponentInParseTreeParseTreeWordIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
				lastActiveComponentInParseTreeParseTreeWordIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef->parseTreeMaxWordIndex;
			}
			else
			{
				lastActiveComponentInParseTreeParseTreeWordIndexMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
				lastActiveComponentInParseTreeParseTreeWordIndexMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
			}
			#else
			lastActiveComponentInParseTreeParseTreeWordIndexMin = ownerGroupParseTreeGroup->parseTreeMinWordIndex;
			lastActiveComponentInParseTreeParseTreeWordIndexMax = ownerGroupParseTreeGroup->parseTreeMaxWordIndex;			
			#endif	
			
			/*
			cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin << endl;
			cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax << endl;
			cout << "lastActiveComponentInParseTreeParseTreeWordIndexMin = " << lastActiveComponentInParseTreeParseTreeWordIndexMin << endl;
			cout << "lastActiveComponentInParseTreeParseTreeWordIndexMax = " << lastActiveComponentInParseTreeParseTreeWordIndexMax << endl;
			*/
			
			result = false;
			if((prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin <= lastActiveComponentInParseTreeParseTreeWordIndexMin) && (prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax >= lastActiveComponentInParseTreeParseTreeWordIndexMax))
			{
				result = true;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			if((lastActiveComponentInParseTreeParseTreeWordIndexMin == prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin) && (lastActiveComponentInParseTreeParseTreeWordIndexMax == prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax))
			{
				bool foundMatchingSetOfLastActiveAndProspectiveActiveComponentParseTreeWords = true;
				if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)
				{
					//cout << "(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)" << endl;
					result = false;	
				}
			}
			#endif
				
		#else

			vector<int> prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList;
			vector<int> lastActiveComponentInParseTreeParseTreeWordIndexList;
			if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, &prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList, 0);
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
				//prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(convertWordIndexToSentenceEntityIndex(forwardPropogationWordData->w));
			}
			else
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceEntityIndex);
			}
			if(lastActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, &lastActiveComponentInParseTreeParseTreeWordIndexList, 0);	
			}
			else
			{
				lastActiveComponentInParseTreeParseTreeWordIndexList.push_back(lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex);
			}

			result = true;
			
			
			bool foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord = true;
			for(int i=0; i<lastActiveComponentInParseTreeParseTreeWordIndexList.size(); i++)
			{
				bool foundLastActiveComponentParseTreeWord = false;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
				cout << "lastActiveComponentInParseTreeParseTreeWordIndexList[i] = " << lastActiveComponentInParseTreeParseTreeWordIndexList[i] << endl;
				#endif
				
				for(int j=0; j<prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.size(); j++)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
					cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					#endif
					
					if(prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] == lastActiveComponentInParseTreeParseTreeWordIndexList[i])
					{
						foundLastActiveComponentParseTreeWord = true;
						//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree debug: foundLastActiveComponentParseTreeWord; prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					}
				}
				if(!foundLastActiveComponentParseTreeWord)
				{
					result = false;	
					foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord = false;
				}
			}
		
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
			if(foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord)
			{
				if((prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL) && (lastActiveComponentInParseTreeParseTreeGroupRef != NULL))
				{
					bool foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord = true;

					for(int j=0; j<prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.size(); j++)
					{
						bool foundProspectiveActiveComponentParseTreeWord = false;

						for(int i=0; i<lastActiveComponentInParseTreeParseTreeWordIndexList.size(); i++)
						{
							if(prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] == lastActiveComponentInParseTreeParseTreeWordIndexList[i])
							{
								foundProspectiveActiveComponentParseTreeWord = true;
								//cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree debug: foundLastActiveComponentParseTreeWord; prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
							}
						}
						if(!foundProspectiveActiveComponentParseTreeWord)
						{
							foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord = false;
						}
					}
					
					if(foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord)
					{
						bool foundMatchingSetOfLastActiveAndProspectiveActiveComponentParseTreeWords = true;
						if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)
						{
							//cout << "(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)" << endl;
							result = false;	
						}
					}
				}
			}
			#endif
		#endif
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

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::getWordIndicesInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, vector<int>* wordIndexList, int level)
{
	bool result = true;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
	printParseTreeDebugIndentation(level);
	cout << "GIAtxtRelTranslatorNeuralNetworkOperationsClass::getWordIndicesInParseTree: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;	
	#endif
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		wordIndexList->push_back(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex); 
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
		printComponent(currentComponent, level);
		#endif
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getWordIndicesInParseTree(currentComponent->parseTreeGroupRef, wordIndexList, level+1))
			{
				result = true;
			}
		}
	}
	
	return result;	
}
#endif




	

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool existingActivationFound)
{
	bool result = true;

	if(existingActivationFound)
	{
		GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
		GIAtxtRelTranslatorRulesComponentParseTree* oldParseComponent = currentParseTreeGroupTemp->components.front();
		#else
		GIAtxtRelTranslatorRulesComponentParseTree* oldParseComponent = currentParseTreeGroupTemp->components.back();
		#endif
		
		int oldParseComponentNumberOfWordsInParseTree = 0;
		if(oldParseComponent->parseTreeGroupRef == NULL)
		{
			oldParseComponentNumberOfWordsInParseTree = 1;
			//cout << "currentParseTreeGroupTemp->components.size() = " << currentParseTreeGroupTemp->components.size() << endl;
			//cerr << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree error: (oldParseComponent->parseTreeGroupRef == NULL)" << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			oldParseComponentNumberOfWordsInParseTree = countNumberWordsInParseTree(oldParseComponent->parseTreeGroupRef, forwardPropogationSentenceData, layer);
		}
		
		int newParseComponentNumberOfWordsInParseTree = 0;
		if(activationPathWordCurrentParseTreeGroup == NULL)
		{
			newParseComponentNumberOfWordsInParseTree = 1;		
		}
		else
		{
			newParseComponentNumberOfWordsInParseTree = countNumberWordsInParseTree(activationPathWordCurrentParseTreeGroup, forwardPropogationSentenceData, layer);
		}
		
	
		result = false;

		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
		cout << "newParseComponentNumberOfWordsInParseTree = " << newParseComponentNumberOfWordsInParseTree << endl;
		cout << "oldParseComponentNumberOfWordsInParseTree = " << oldParseComponentNumberOfWordsInParseTree << endl;
		#endif
		
		if(newParseComponentNumberOfWordsInParseTree >= oldParseComponentNumberOfWordsInParseTree)
		{
			result = true;
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
			cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree debug: newParseComponentNumberOfWordsInParseTree >= oldParseComponentNumberOfWordsInParseTree" << endl;
			//exit(EXIT_ERROR);
			#endif
		}
		else
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA9
			cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree debug: newParseComponentNumberOfWordsInParseTree < oldParseComponentNumberOfWordsInParseTree" << endl;
			//exit(EXIT_ERROR);
			#endif
		}
	}
	
	return result;
}

int GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::countNumberWordsInParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	int performance = 0;
	if(currentParseTreeGroup->components.size() > 0)
	{
		performance = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
		//cout << "performance = " << performance << endl;
	}
	#else
	int performanceOrig = forwardPropogationSentenceData->performance;
	
	updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	int performance = forwardPropogationSentenceData->performance;

	forwardPropogationSentenceData->performance = performanceOrig;
	#endif
	
	return performance;
}


#endif
	

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAtxtRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, int layer)
{
	bool result = true;

	GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
	if((currentParseTreeGroupTemp != NULL) && (activationPathWordCurrentParseTreeGroup != NULL))
	{
		/*
		if(parseTreeContainsGroup(currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, layer))
		{
			result = false;
		}
		*/
		if(parseTreeContainsGroup(activationPathWordCurrentParseTreeGroup, currentParseTreeGroupTemp, layer))
		{
			result = false;
		}
	}	
	
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::parseTreeContainsGroup(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroupToFind, int level)
{
	bool result = false;

	if(currentParseTreeGroup == parseTreeGroupToFind)
	{
		result = true;
	}
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			if(parseTreeContainsGroup(currentComponent->parseTreeGroupRef, parseTreeGroupToFind, level+1))
			{
				result = true;
			}	
		}
	}
	
	return result;
}
#endif

	
void GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::updateParseTreeMaxMinWordIndexOfParent(GIAtxtRelTranslatorRulesGroupParseTree* parentGroup, GIAtxtRelTranslatorRulesComponentParseTree* parentComponent, GIAtxtRelTranslatorRulesGroupParseTree* childGroup)
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
		}
		if(childGroup->parseTreeMinWordIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = childGroup->parseTreeMinWordIndex;
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
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > parentGroup->parseTreeMaxWordIndex)
		{
			parentGroup->parseTreeMaxWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		}
		if(parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex < parentGroup->parseTreeMinWordIndex)
		{
			parentGroup->parseTreeMinWordIndex = parentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
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



bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAtxtRelTranslatorNeuralNetworkOperations.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::groupActivationCompleted(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
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
	
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::demarkateOptimumPathwayBackprop(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup)
{
	bool result = true;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
	printParseTreeDebugIndentation(level);
	cout << "GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::demarkateOptimumPathwayBackprop: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;	
	#endif
	
	/*
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentParseTreeGroup->groupOrig;
	ownerGroup->optimumPathwayWordList.push_back(currentParseTreeGroup->wordIndex);
	*/
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* componentOrig = currentComponent->componentRef;
		componentOrig->optimumPathwayWordList.push_back(currentComponent->wordIndex);
			
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(demarkateOptimumPathwayBackprop(currentComponent->parseTreeGroupRef))
			{
				result = true;
			}
		}
	}
	
	return result;	
}

bool GIAtxtRelTranslatorNeuralNetworkLightOptimisedClass::isWordInGroupOptimumPathwayWordList(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	if(forwardPropogationSentenceData->parserEnabled)
	{
		//cout << "currentComponent->optimumPathwayWordList.size() = " << currentComponent->optimumPathwayWordList.size() << endl;
		//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
		for(int i=0; i<currentComponent->optimumPathwayWordList.size(); i++)
		{
			//cout << "currentComponent->optimumPathwayWordList[i] = " << currentComponent->optimumPathwayWordList[i] << endl;
		}
		
		if(find(currentComponent->optimumPathwayWordList.begin(), currentComponent->optimumPathwayWordList.end(), forwardPropogationWordData->w) != currentComponent->optimumPathwayWordList.end())
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
			
#endif


		

#endif
