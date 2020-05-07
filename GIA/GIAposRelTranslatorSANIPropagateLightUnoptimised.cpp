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
 * File Name: GIAposRelTranslatorSANIPropagateLightUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k8a 06-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Light Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIPropagateLightUnoptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_UNOPTIMISED



#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
GIAposRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocal;
#endif

#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_BIO
vector<GIAposRelTranslatorRulesGroupParseTree*> parseTreeGroupsArray;
#endif


bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocal = NULL;
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_BIO
	parseTreeGroupsArray.clear();
	#endif

	//sentence specific variables:
	GIAposRelTranslatorSANIForwardPropogationSentenceData forwardPropogationSentenceData;
	GIAposRelTranslatorSANIPropagateOperations.setParseSentenceReverse(true, &forwardPropogationSentenceData);
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData.isQuery = GIAposRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData.GIAposRelTranslatorRulesGroupTypes = GIAposRelTranslatorRulesGroupTypes;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = new GIAposRelTranslatorSANIForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA8
	firstExecution = false;
	#endif
	
	GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			w = sentenceContents->size()-1;
		}
		else
		{
			w = 0;
		}
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup))
		{
			result = false;
		}
	}
	else
	{
		cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		//cout << "topLevelParseTreeGroupLocal" << endl;
		
		//#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "topLevelParseTreeGroupLocal->groupName = " << topLevelParseTreeGroupLocal->groupName << endl;
		//#endif	
		#endif	
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData.performance;
		//cout << "forwardPropogationSentenceData.performance = " << forwardPropogationSentenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		/*
		cout << "DEBUG: finished executing GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
		exit(EXIT_ERROR);
		*/
	}
	else
	{
		*performance = 0;		
	}
		
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		int performanceNOTUSED = 0;
		bool print = false;
		bool performancePreprocess = false;
		GIAposRelTranslatorSANIPropagateOperations.traceBackpropParseTree(topLevelParseTreeGroupLocal, 1, print, performancePreprocess, &performanceNOTUSED, NULL);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents
		GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
		GIAposRelTranslatorSANIPropagateOperations.resetNeuronBackprop(topLevelParseTreeGroupLocal, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	//added GIA3g6aTEMP32 
		
		/*
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
		}
		*/
	}
	else
	{
		GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	cout << "DEBUG: finished executing GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	//cout << "w = " << w << endl;
	GIApreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << GIApreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif

	//word specific variables:
	GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);

	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	
	forwardPropogationWordData->w = w;
	//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
	
	//group specific variables:
	GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalData;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
	forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
	#endif
				
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));
		
	
	if(!GIAposRelTranslatorSANIPropagateOperations.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
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
				
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_WILDCARDS
		for(int wordPOStype=0; wordPOStype<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
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
	
			
	GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAposRelTranslatorSANIFormation.findWordInGroupMap(wordLowerCase, GIAposRelTranslatorSANIFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	#else
	if(GIAposRelTranslatorSANIFormation.findWordInGroupMap(wordLowerCase, GIAposRelTranslatorSANIFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	else if(GIAposRelTranslatorSANIFormation.findWordInGroupMap(currentWord->tagName, GIAposRelTranslatorSANIFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	#endif
		
	for(unordered_map<string, GIAposRelTranslatorRulesGroupNeuralNetwork*>::iterator iter1 = GIAposRelTranslatorSANIFormation.getInputLayerSectionTokensLayerMap()->begin(); iter1 != GIAposRelTranslatorSANIFormation.getInputLayerSectionTokensLayerMap()->end(); iter1++)
	{
		GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayer = iter1->second;
		for(int i=0; i<currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList.size(); i++)
		{
			GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = (currentGroupInInputLayerSectionTokensLayer->ANNbackGroupConnectionList)[i];
			for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList.size(); i++)
			{
				GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = (currentGroupInInputLayerSectionTokensLayerClass->ANNbackGroupConnectionList)[i];
				for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList.size(); i++)
				{
					GIAposRelTranslatorRulesGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = (currentGroupInInputLayerSectionTokensLayerClassType->ANNbackGroupConnectionList)[i];
					if(currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName == wordLowerCase)	//NB this implementation is synced with GIAposRelTranslatorRulesClass::isClassTag: assume tokens always comprise wordLowerCase
					{
						string GIAtokenLayerClassName = currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName;
						int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
						if(SHAREDvars.textInTextArray(GIAtokenLayerClassName, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
						{
							forwardPropogationWordData->wordPOStype = wordPOStype;
						}
						else
						{
							cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
						GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
						cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
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

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
	
	return result;
}

	
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAposRelTranslatorSANIFormation.getInputGroupLayerSection(GIAposRelTranslatorSANIFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}	
	
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}				
				

bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
			
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
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
		
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
			GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent = currentComponent;
			int componentIndex = currentComponent->componentIndex;	
			int ownerComponentIndex = componentIndex;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
				ownerComponentIndex = ownerComponent->componentIndex;
			}
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA2
				GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
				cout << "2a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
				#endif

				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				if(GIAposRelTranslatorSANIPropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
				{
				#endif
						
					if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
					}

					GIAposRelTranslatorDebug debug;
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA4
					debug.firstComponentActive = ownerGroup->components[0]->neuronComponentConnectionActive;
					if(ownerGroup->components.size() >= 2)
					{
						debug.secondComponentActive = ownerGroup->components[1]->neuronComponentConnectionActive;
					}
					#endif

					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
					cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
					//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
					GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
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

				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				}
				#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif	
		}
	}
	
	return result;
}	
	


bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, int groupFrontComponentConnectionListIndex, GIAposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAposRelTranslatorSANIForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	*/
	
	GIAposRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAposRelTranslatorRulesComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
					//cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
					//cout << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(componentTests2(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3c: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	#endif
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(GIAposRelTranslatorSANIPropagateOperations.componentWordConnectivityTestsWrapper(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, debug))
					{
						result = true;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				}
				#endif
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	if(!result)
	{
		forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
	}
	#endif
	*/
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, int groupFrontComponentConnectionListIndex, GIAposRelTranslatorDebug* debug)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_MEM
	GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig = GIAposRelTranslatorRules.copyGroup(ownerGroup);
	#else
	GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig = NULL;
	#endif
	#endif

	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	//create new parseTreeGroup
	//copied from GIAposRelTranslatorSANIPropagateInverseClass::generateRulesGroupTreeComponents;
	GIAposRelTranslatorRulesComponentParseTree* newParseComponent = GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAposRelTranslatorRulesComponentParseTree(*GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_BIO2
	//newParseComponent->componentRef = currentComponent;
	newParseComponent->groupFrontComponentConnectionListIndex = groupFrontComponentConnectionListIndex;
	newParseComponent->numberOfWordsCurrentlyParsed = forwardPropogationWordData->w;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_BIO
	if(currentParseTreeGroupTemp->components.size() == 0)
	{
		parseTreeGroupsArray.push_back(currentParseTreeGroupTemp);
	}
	#endif
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
	newParseComponent->parseTreeGroupRef->solidified = true;
	#endif
	*/
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
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA4
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", firstComponentActive = " << debug->firstComponentActive << endl;	
	GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
	GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif

	/*
	if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
	{
		cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
		exit(EXIT_ERROR);
	}
	*/
				
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
		GIAposRelTranslatorSANIPropagateOperations.resetGroupActivation(ownerGroup);
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
		//create new currentParseTreeGroupTemp for ownerGroup;
		//cout << "GIAposRelTranslatorSANIPropagateOperations.resetGroupParseTreeGroupRef" << endl;
		GIAposRelTranslatorSANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, false);
		/*
		if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
		{
			cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
			exit(EXIT_ERROR);
		}
		*/
		#endif
		#endif	
		
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		GIAposRelTranslatorSANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);

		bool topLevelGroup = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
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
			#endif
				if(GIAposRelTranslatorSANIPropagateOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
				{
					/*
					cout << "topLevelGroup && GIAposRelTranslatorSANIPropagateOperations.isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
					exit(EXIT_ERROR);
					*/

					result = true;
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;

					forwardPropogationSentenceData->toplevelGroupActivationFound = true;
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
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

			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_REVERSE_DIRECTION
				cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == 0)" << endl;
				#else
				cerr << "GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
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
			//#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_MEM
			restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData, false);
			//#endif
		}
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_MEM
	GIAposRelTranslatorSANIPropagateOperations.deleteGroup(ownerGroupOrig);
	#endif
	#endif


	return result;
}


void GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::restoreGroupActivations(GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, bool restoreCurrentParseTreeGroupTemp)
{
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_NEW
	if(restoreCurrentParseTreeGroupTemp)
	{
		GIAposRelTranslatorSANIPropagateOperations.restoreGroupParseTreeGroupRef(ownerGroup, activationPathWordCurrentParseTreeGroupOwner);	//restore original group from memory
	}
	//cout << "ownerGroup->currentParseTreeGroupTemp = " << ownerGroup->currentParseTreeGroupTemp->groupName << endl;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_MEM
	GIAposRelTranslatorRulesComponentParseTree* latestParseTreeComponent = NULL;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		//ownerGroup->currentParseTreeGroupTemp.pop_front();
		latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[0];
		ownerGroup->currentParseTreeGroupTemp->components.erase(ownerGroup->currentParseTreeGroupTemp->components.begin());
	}
	else
	{
		latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[ownerGroup->currentParseTreeGroupTemp->components.size()-1];
		ownerGroup->currentParseTreeGroupTemp->components.pop_back();
	}
	delete latestParseTreeComponent;
	GIAposRelTranslatorSANIPropagateOperations.restoreGroupActivation(ownerGroup, ownerGroupOrig);
	//NOTNECESSARY: GIAposRelTranslatorSANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, true);
	#endif
		
	#else
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_LIGHT_BIO
	for(int i2=0; i2<parseTreeGroupsArray.size(); i2++)
	{
		GIAposRelTranslatorRulesGroupParseTree* parseTreeGroup = parseTreeGroupsArray[i2];
		if(parseTreeGroup == activationPathWordCurrentParseTreeGroupOwner)	//this check is only allowed if GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::restoreGroupActivations is always executed when if(!propagateWordThroughNetworkGroup) / if(!propagateWordThroughNetworkGroupComponentPassNextWord)
		{
			for(int i1=0; i1<parseTreeGroup->components.size(); i1++)
			{
				GIAposRelTranslatorRulesComponentParseTree* parseTreeComponent = (parseTreeGroup->components)[i1];
				int parseTreeComponentWordIndex = parseTreeComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				
				bool parseTreeComponentWordIndexComparisonBool = false;
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					if(parseTreeComponentWordIndex <= forwardPropogationWordData->w)
					{
						parseTreeComponentWordIndexComparisonBool = true;
					}
				}
				else
				{
					if(parseTreeComponentWordIndex >= forwardPropogationWordData->w)
					{
						parseTreeComponentWordIndexComparisonBool = true;
					}
				}
				if(parseTreeComponentWordIndexComparisonBool)
				{
					GIAposRelTranslatorRulesComponentParseTree* component = parseTreeComponent->componentRef;
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
				//NOTNECESSARY: GIAposRelTranslatorSANIPropagateOperations.resetGroupParseTreeGroupRef(ownerGroup, true);
			}
		}
	}
	#else
	
	#endif
	
	
	#endif
}

bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	
	int wNext;
	bool wNextComparison = false;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		wNext = forwardPropogationWordData->w - 1;
		if(wNext >= 0)
		{
			wNextComparison = true;
		}
	}
	else
	{
		wNext = forwardPropogationWordData->w + 1;
		if(wNext < forwardPropogationSentenceData->sentenceContents.size())
		{
			wNextComparison = true;
		}
	}
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
	cout << "forwardPropogationWordData->w+1 = " << forwardPropogationWordData->w+1 << endl;
	#endif
	
	bool passThroughNew = false;
	if(wNextComparison)
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






#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::componentTests2(GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
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

bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype)
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
		if(GIAposRelTranslatorSANIPropagateOperations.checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype)
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
				if(GIAposRelTranslatorSANIPropagateOperations.checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}
#endif





bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::printBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAposRelTranslatorSANIPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAposRelTranslatorSANIPropagateOperations.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAposRelTranslatorSANIPropagateLightUnoptimisedClass::groupActivationCompleted(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
		/*
		cout << "\tcurrentComponent2->neuronComponentConnectionActive = " << currentComponent2->neuronComponentConnectionActive << endl;
		cout << "\tcurrentComponent2->optional = " << currentComponent2->optional << endl;
		cout << "\tcurrentComponent2->hasSubComponents = " << GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent2) << endl;
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
