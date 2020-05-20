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
 * File Name: GIAposRelTranslatorSANIPropagateHeavyUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k15d 14-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Heavy Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorSANIPropagateHeavyUnoptimised.hpp"
#include "SHAREDvars.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY
#ifdef GIA_POS_REL_TRANSLATOR_SANI_HEAVY_UNOPTIMISED



#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
GIAposRelTranslatorRulesGroupParseTree* topLevelParseTreeGroupLocal;
#endif


bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAposRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocal = NULL;
	#endif
				
	//sentence specific variables:
	GIAposRelTranslatorSANIForwardPropogationSentenceData forwardPropogationSentenceData;
	GIAposRelTranslatorSANIPropagateOperations.setParseSentenceReverse(true, &forwardPropogationSentenceData);
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS
	//forwardPropogationSentenceData.parserEnabled = parserEnabled;
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
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());
	
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork)
	
	GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
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
		cerr << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
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
		cout << "DEBUG: finished executing GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
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
		
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
			cout << "w = " << w << ", forwardPropogationWordData->foundPreviousActiveWord = " << forwardPropogationWordData->foundPreviousActiveWord << endl;
			#endif
		}
	}
	else
	{
		GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	cout << "DEBUG: finished executing GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough)
{
	bool result = false;

	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
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
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	forwardPropogationWordData->previousWordConnections.clear();
	#endif
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	//forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = expectToSeeConnectionWithPreviousWordTrace;
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
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
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
	
			
	
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAposRelTranslatorSANIFormation.findWordInGroupMap(wordLowerCase, GIAposRelTranslatorSANIFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
		cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
		{
			result = true;
		}
	}
	else if(GIAposRelTranslatorSANIFormation.findWordInGroupMap(currentWord->tagName, GIAposRelTranslatorSANIFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
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
							cerr << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
						GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
						cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
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

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAposRelTranslatorSANIPropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->GIAposRelTranslatorRulesGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
		
	return result;
}

	
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup = GIAposRelTranslatorSANIFormation.getInputGroupLayerSection(GIAposRelTranslatorSANIFormation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_CONSECUTIVE_WORDS_SAME_POSTYPE
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
	
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAposRelTranslatorRulesGroupNeuralNetwork* inputLayerGroup, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkGroupToParseTreeGroupNew(inputLayerGroup);	//new GIAposRelTranslatorRulesGroupParseTree(*GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkGroupToParseTreeGroup(inputLayerGroup));	//CHECKTHIS
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
				

bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}

bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{
	#endif
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
	#endif
	/*
	if(passThrough)
	{
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;		
	}
	*/
			
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, passThrough))
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
		
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough)
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
			GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponentOrig = ownerComponent;
			GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponentOrig = currentComponent;
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig = ownerGroup;

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
				
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_RECURSION
					//this is required to support GIAposRelTranslatorRules.xml recursive groups, e.g. logicReferenceSets: 
					int activationMemoryGroupArraySizeOrig = ownerGroupOrig->activationMemoryGroupArray.size();
					int currentParseTreeGroupArraySizeOrig = ownerGroupOrig->currentParseTreeGroupArray.size();
					int index = 0;
					for(int activationMemoryGroupArrayIndex=ownerGroupOrig->activationMemoryGroupArray.size()-1; activationMemoryGroupArrayIndex>=0; activationMemoryGroupArrayIndex--)
					{
						index++;
					#endif	
						int activationMemoryGroupArraySize = ownerGroupOrig->activationMemoryGroupArray.size();
						
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_RECURSION
						//cout << "activationMemoryGroupArrayIndex = " << activationMemoryGroupArrayIndex << endl;
						//cout << "ownerGroup->currentParseTreeGroupArray.size() = " << ownerGroupOrig->currentParseTreeGroupArray.size() << endl;
						GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray.back();
						#endif
						#endif
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_RECURSION
						GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray[activationMemoryGroupArrayIndex];
						#else
						GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray.back();
						#endif
						#endif
						
						/*
						cout << "activationMemoryGroup->components.size() = " << activationMemoryGroup->components.size() << endl;
						cout << "activationMemoryGroup->lastWordIndexActivated = " << activationMemoryGroup->lastWordIndexActivated << endl;
						cout << "forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
						*/
						
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						if(activationMemoryGroup->lastWordIndexActivated != forwardPropogationWordData->wordReference->translatorSentenceWordIndex)
						{
						#endif
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
							if(!(currentParseTreeGroup->solidified))	//this saves time by considering only recently activated groups
							{
							#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
								GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = NULL;
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
								GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroupNew = NULL;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
								int activationMemoryGroupLastWordIndexActivatedOrig = INT_DEFAULT_VALUE;
								#endif
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
								GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupNew = NULL;
								#endif
								
								GIAposRelTranslatorRulesGroupActivationMemory* activationMemoryGroupTemp = NULL;
								GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = NULL;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
									activationMemoryGroupNew = GIAposRelTranslatorRules.copyGroup(activationMemoryGroup);
									ownerGroupOrig->activationMemoryGroupArray.push_back(activationMemoryGroupNew);
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroupLastWordIndexActivatedOrig = activationMemoryGroup->lastWordIndexActivated;
									activationMemoryGroup->lastWordIndexActivated = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
									//NOTREQUIRED: activationMemoryGroupNew->lastWordIndexActivated = forwardPropogationWordData->wordReference->translatorSentenceWordIndex; 
									#endif
									#endif
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
									currentParseTreeGroupNew = GIAposRelTranslatorRules.copyGroup(currentParseTreeGroup);	//create a new parse tree group (in case of 1. first encounter with group or 2. recursion into different group)
									ownerGroupOrig->currentParseTreeGroupArray.push_back(currentParseTreeGroupNew);
									#endif				

									activationMemoryGroupTemp = activationMemoryGroupNew;
									currentParseTreeGroupTemp = currentParseTreeGroupNew;
									
									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
									if(activationMemoryGroupArrayIndex != 0)	//never solidify first group in array (non-activated template)
									{
										currentParseTreeGroup->solidified = true;
									}
									#endif

								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								else
								{
									activationMemoryGroupTemp = activationMemoryGroup;
									currentParseTreeGroupTemp = currentParseTreeGroup;

									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
									currentParseTreeGroup->solidified = true;	//prevent recursion during passThrough
									#endif
								}
								#endif
								
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS		
								GIAposRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupOwnerComponent = activationMemoryGroupTemp->components[ownerComponentIndex];
								GIAposRelTranslatorRulesComponentNeuralNetwork* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
								if(currentComponentOrig->isSubcomponent)
								{
									if(GIAposRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
									{
										activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
									}
									else
									{
										cerr << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect{} error: (currentComponent->isSubcomponent) && !(activationMemoryGroupOwnerComponent->hasSubComponents)" << endl;
										cout << "index = " << index << endl;
										cout << "ownerComponentIndex = " << ownerComponentIndex << endl;
										cout << "componentIndex = " << componentIndex << endl;
										GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  group->groupTypeName << ":" << group->groupName << endl;	
										GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
										cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
										cout << "ownerComponent->componentType = " << ownerComponent->componentType << endl;
										GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);	
										GIAposRelTranslatorSANIPropagateOperations.printComponent(ownerComponent, layer+1);
										exit(EXIT_ERROR);
									}
								}
								ownerGroup = activationMemoryGroupTemp;
								currentComponent = activationMemoryGroupCurrentComponent;
								ownerComponent = activationMemoryGroupOwnerComponent;
								#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
								activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupTemp;	//use at top of parseTreeGroup at top of stack
								#endif
									
													
								if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
								{
									currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
								}


								GIAposRelTranslatorDebug debug;
								#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA4
								debug.activationMemoryGroupArrayIndex = activationMemoryGroupArrayIndex;
								debug.activationMemoryGroupArraySize = activationMemoryGroupArraySize;
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

								//int currentMemoryGroupArrayNewlyCreatedIndex = ownerGroup->currentParseTreeGroupArray.size()-1;	//CHECKTHIS; is this required for groupArray deletion?


								if(currentComponent->semanticRelationReturnEntity)
								{
									activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
								}

								int indexOfFirstComponentInGroup = 0;
								int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
								GIAposRelTranslatorRulesComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
								GIAposRelTranslatorRulesComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS
								bool lastComponentFuzzyAndCompatible = false;
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
								bool lastComponentOptionalAndInactiveAndCompatible = false;
								bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
								if(GIAposRelTranslatorSANIPropagateOperations.isLastComponentFuzzy(ownerGroup))
								{
									if(lastComponent == ownerComponent)
									{
										if(lastComponent->neuronComponentConnectionActive)
										{
											if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
											{
												if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == (forwardPropogationWordData->w - 1))
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
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
								if(GIAposRelTranslatorSANIPropagateOperations.isLastComponentFuzzy(ownerGroup))
								{
									if(lastComponent == ownerComponent)
									{
										if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
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


								#ifndef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_RECURSION
								/*
								if(groupActivationCompleted(ownerGroup))
								{
									//cout << "groupActivationCompleted(ownerGroup)" << endl;
								}
								if(groupActivationCompleted(ownerGroup) && !lastComponentFuzzyAndCompatible)
								{
									//cout << "GIAposRelTranslatorSANIPropagateOperations.resetGroupActivation: ownerGroup = " << ownerGroup->groupName << endl;
									GIAposRelTranslatorSANIPropagateOperations.resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								*/
								
								//changed 3g5a (introduce more simple reset detection method);
								bool requireResetGroupActivation = false;
								if(firstComponent == ownerComponent)
								{
									if(GIAposRelTranslatorSANIPropagateOperations.doesRequireResetGroupActivation(firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationWordData))
									{
										requireResetGroupActivation = true;
									}
								}
								else if(ownerGroup->components.size() >= 1)
								{
									int indexOfSecondComponentInGroup = 1;
									GIAposRelTranslatorRulesComponentNeuralNetwork* secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];

									if((secondComponent == ownerComponent) && (firstComponent->optional))
									{
										if(GIAposRelTranslatorSANIPropagateOperations.doesRequireResetGroupActivation(secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationWordData))
										{
											requireResetGroupActivation = true;
										}
									}
								}

								if(requireResetGroupActivation)
								{
									#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
									cout << "requireResetGroupActivation" << endl;
									#endif
									GIAposRelTranslatorSANIPropagateOperations.resetGroupActivation(ownerGroup);
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



								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
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

										#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
										GIAposRelTranslatorSANIPropagateOperations.deleteGroup(currentParseTreeGroupNew);
										ownerGroupOrig->currentParseTreeGroupArray.pop_back();
										#endif
										#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
										GIAposRelTranslatorSANIPropagateOperations.deleteGroup(activationMemoryGroupNew);
										ownerGroupOrig->activationMemoryGroupArray.pop_back();
										#endif
									}

									#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroup->lastWordIndexActivated = activationMemoryGroupLastWordIndexActivatedOrig;
									#endif
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								#endif

							#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								if(!passThrough)
								{
								#endif
									if(activationMemoryGroupArrayIndex != 0)
									{
										currentParseTreeGroup->solidified = false;
									}
								#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
								}
								else
								{
									currentParseTreeGroup->solidified = false;
								}
								#endif
							}
							#endif

						#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						}
						#endif

					#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_RECURSION	
					}
					#endif
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
	


bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA3
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	vector<GIAposRelTranslatorSANIForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	
	GIAposRelTranslatorRulesComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	GIAposRelTranslatorRulesComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;
	if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent))
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
					//cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(GIAposRelTranslatorSANIPropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE
					//cout << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(!firstActiveComponentInGroup || GIAposRelTranslatorSANIPropagateOperations.componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(GIAposRelTranslatorSANIPropagateOperations.componentWordConnectivityTestsWrapper(activationPathWordCurrentParseTreeGroupOwner, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough, debug))
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
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
	if(!result)
	{
		forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
	}
	#endif
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* group, GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroup, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorRulesComponentNeuralNetwork* ownerComponent, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, GIAposRelTranslatorDebug* debug)
{
	bool result = false;
	
	GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordDataPreviousRecord = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		GIAposRelTranslatorRulesComponentParseTree* previousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components[activationPathWordCurrentParseTreeGroupOwner->components.size()-1];
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
		int wPrevious2 = forwardPropogationWordData->w - 1;
		if(wPrevious2 >= 0)	//redundant
		{
			if(previousActiveComponent->neuronComponentConnectionActiveWordRecord == (*(forwardPropogationSentenceData->sentenceContents))[wPrevious2])
			{
				forwardPropogationSignalData->foundPreviousActiveWord = true;
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
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
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
		if(!passThrough)
		{
		#endif
			int wPrevious = previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			if(wPrevious >= 0)	//redundant
			{
				GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordDataPrevious = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[wPrevious]);
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
				cout << "foundPreviousActiveWord, forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				#endif
				//cout << "forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
				}
				#endif
			}
			else
			{
				cerr << "wPrevious = " << wPrevious << endl;
				exit(EXIT_ERROR);
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
		}
		#endif
		#endif					
	}
	#endif
		
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{	
	#endif
		
		//cout << "forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;
		/*
		if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
		{
			cout << "(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)" << endl;
		}
		*/
		
		ownerComponent->neuronComponentConnectionActive = true;
		currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
		ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

		/*
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
		vector<GIAposRelTranslatorSANIForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
		#endif
		#endif
		*/

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE
		//create new parseTreeGroup
		//copied from GIAposRelTranslatorSANIPropagateInverseClass::generateRulesGroupTreeComponents;
		GIAposRelTranslatorRulesComponentParseTree* newParseComponent = GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new GIAposRelTranslatorRulesComponentParseTree(*GIAposRelTranslatorSANIPropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
		newParseComponent->componentRef = currentComponent;
		newParseComponent->neuronComponentConnectionActive = true;
		newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
		//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
		newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
		activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
		vector<GIAposRelTranslatorRulesGroupParseTree*> parseTreeGroupRefParseTreeGroupRefReverseOrig;
		vector<int> parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse;
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
				GIAposRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
				GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
				//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
				parseTreeGroupRefParseTreeGroupRefReverseOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverse);
				parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex);
				parseTreeGroupRef->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
				parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = i;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		}
		#endif
		GIAposRelTranslatorRulesComponentParseTree* higherLevelComponent = NULL;
		GIAposRelTranslatorRulesGroupParseTree* higherLevelComponentParseTreeGroupRefOrig = NULL;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
		//bool foundPreviousActiveWordOrig = forwardPropogationSignalData->foundPreviousActiveWord;
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
		vector<GIAposRelTranslatorSANIForwardPropogationWordData*> previousWordConnectionsOrig;
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

					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = true;
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
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

		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
		//b) enforce solidification of previous active component subgroup
		if(activationPathWordCurrentParseTreeGroupOwner->components.size() >= 2)
		{
			int indexOfPreviousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components.size()-2;
			GIAposRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
			solidifyNeuralNetBackpropGroupLastComponent(component->parseTreeGroupRef, layer); 
		}
		#else
		newParseComponent->parseTreeGroupRef->solidified = true;
		#endif
		#endif
		
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(lastComponentFuzzyAndCompatible && activationSequenceCompleted)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
				forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
				#endif
				*/

				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif

				//record variables for GIAposRelTranslatorSANIParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used	

				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					result = true;
				}
			}	
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(lastComponentFuzzyAndCompatible)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
			cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible" << endl;
			#endif
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
		#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
			
		
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = GIAposRelTranslatorSANIPropagateOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	bool sentenceWordDataFullyConnected = false;
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << ", activationMemoryGroupArrayIndex = " << debug->activationMemoryGroupArrayIndex << ", firstComponentActive = " << debug->firstComponentActive << ", activationMemoryGroupArraySize = " << debug->activationMemoryGroupArraySize << ", GIAposRelTranslatorSANIPropagateOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData) = " << sentenceWordDataFullyConnected << endl;	
	GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
	#endif
	
	
	bool passTemp = false;
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(passThrough)
	{
		passTemp = true;
	}
	else
	{
	#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(!(lastComponentFuzzyAndCompatible && activationSequenceCompleted))
		{
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!lastComponentFuzzyAndCompatible)
		{
		#endif
			passTemp = true;
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		}
		#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}
	#endif
	if(passTemp)
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		cout << "propagateWordThroughNetworkGroupComponent: !lastComponentFuzzyAndCompatible, activationSequenceCompleted = " << activationSequenceCompleted << endl;
		#endif
		if(activationSequenceCompleted)
		{
			//cout << "propagateWordThroughNetworkGroupComponent: repeatDetected = " << repeatDetected << endl;

			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
			cout << "activationSequenceCompleted" << endl;
			#endif

			GIAposRelTranslatorSANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
			if(!passThrough)
			{	
			#endif
			
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif
				semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);

				//record variables for GIAposRelTranslatorSANIParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
			}
			else
			{
				semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;	//only used for forwardPropogationSignalData->foundPreviousActiveWord
			}
			#endif
			
			//cout << "DEBUG printBackpropParseTree:" << endl;
			//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

			bool topLevelGroup = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
				if(passThrough)
				{	
					result = true;
				}
				else
				{
				#endif
					//cout << "topLevelGroup" << endl;
					//exit(0);

					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
					if(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents->size()-1)	//end of sentence punctuation mark found
					{
					#endif
						#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_OLD
						if(GIAposRelTranslatorSANIPropagateOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData))
						{
						#endif
					
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
							#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
							if(!GIAposRelTranslatorSANIPropagateOperations.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
							{
								//result = false;
							}
							#endif

							/*
							cout << "topLevelGroup" << endl;
							cout << "finishedPassingSentenceWords (temp exit)" << endl;
							exit(0);
							*/

						#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_OLD
						}
						else
						{
							result = false;
						}
						#endif
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC
					}
					else
					{
						cerr << "GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
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
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
			cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;	
			GIAposRelTranslatorSANIPropagateOperations.printComponent(currentComponent, layer+1);
			*/

			//cout << "propagateWordThroughNetworkGroupComponentPassNextWord" << endl;
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
	}


	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	if(!passThrough)
	{	
	#endif
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!result)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig;
			activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig;
			if(activationSequenceCompleted)
			{
			#endif
				for(int i=0; i<activationPathWordCurrentParseTreeGroupOwner->components.size(); i++)
				{
					GIAposRelTranslatorRulesComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
					GIAposRelTranslatorRulesGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
					parseTreeGroupRef->parseTreeGroupRefReverse = parseTreeGroupRefParseTreeGroupRefReverseOrig[i];
					parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig[i];
				}
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			}
			#endif
			if(activationSequenceCompleted)
			{
				if(lastComponentFuzzyAndCompatible && (higherLevelComponent != NULL))
				{
					higherLevelComponent->parseTreeGroupRef = higherLevelComponentParseTreeGroupRefOrig;

					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = foundPreviousActiveWordOrig;
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC	
					forwardPropogationWordDataPreviousRecord->previousWordConnections = previousWordConnectionsOrig;
					#endif
					#endif
				}
			}
		}
		#endif
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
	}	
	#endif
	
	return result;
}


#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;

	currentParseTreeGroup->solidified = true;
	
	if(currentParseTreeGroup->components.size() > 0)
	{
		int indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
		GIAposRelTranslatorRulesComponentParseTree* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
		solidifyNeuralNetBackpropGroupLastComponent(lastComponent->parseTreeGroupRef, level+1);
	}
		
	return result;
}
#endif
#endif



/*
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::findComponentRef(GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorRulesComponentParseTree* componentRef, GIAposRelTranslatorRulesComponentParseTree** componentFound)
{
	bool result = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		if(currentParseTreeComponent->componentRef == componentRef)
		{
			result = true;
			*componentFound = currentParseTreeComponent;
		}
	}
	
	return result;
}
*/


bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupNeuralNetwork* ownerGroupOrig, GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalData, GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordData, GIAposRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroup, GIAposRelTranslatorRulesGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough)
{
	bool result = false;
	bool pass = true;
	bool expectToSeeConnectionWithPreviousWordTrace = BOOL_IRRELEVANT;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD
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
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES
			bool topLevelGroup = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(!topLevelGroup)
			{
				//hypothesise that activation sequence is completed (and not top level group), propagate next layer up
				bool passThroughNew = true;
				
				//cout << "propagateWordThroughNetworkGroup START: [GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
				GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData; //GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, forwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					//cout << "propagateWordThroughNetworkGroup pass [GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
					pass = false;
					result = true;
				}
				else
				{
					//cout << "propagateWordThroughNetworkGroup fail [GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
				if(GIAposRelTranslatorSANIPropagateOperations.isWordPOStypeNoun(forwardPropogationWordData->wordPOStype))
				{
				#endif
				
					bool topLevelGroup = GIAposRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
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
						GIAposRelTranslatorSANIForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData;	//GIAposRelTranslatorSANIForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
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
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
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
		
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
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
		GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer+1);
		//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
		
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroupOrig = " << ownerGroupOrig->groupName << ", ownerGroupOrig->groupTypeName = " << ownerGroupOrig->groupTypeName << endl;
				
		//pass next word though network
		expectToSeeConnectionWithPreviousWordTrace = false;
		forwardPropogationWordData->foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
		
		#ifdef GIA_POS_REL_TRANSLATOR_SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			expectToSeeConnectionWithPreviousWordTrace = false;
		}
		else
		{
			if(forwardPropogationWordData->w > 0)
			{
				GIAposRelTranslatorSANIForwardPropogationWordData* forwardPropogationWordDataPrevious = (forwardPropogationSentenceData->forwardPropogationWordDataArray[forwardPropogationWordData->w-1]);
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
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PROPAGATE_EXTRA5
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











bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::printBackpropParseTree(GIAposRelTranslatorRulesGroupParseTree* group, int level)
{
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	GIAposRelTranslatorSANIPropagateOperations.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	GIAposRelTranslatorSANIPropagateOperations.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}




bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::groupActivationCompleted(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
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

#ifdef GIA_POS_REL_TRANSLATOR_SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
bool GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::groupPartiallyOrFullyActivated(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{	
	bool groupPartiallyActivated = false;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAposRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
		if(currentComponent2->neuronComponentConnectionActive)
		{
			groupPartiallyActivated = true;
		}
	}
	
	return groupPartiallyActivated;
}
#endif
				










/*
#ifdef GIA_POS_REL_TRANSLATOR_SANI_SAVE_MEMORY_GROUPS
GIAposRelTranslatorRulesGroupNeuralNetwork* GIAposRelTranslatorSANIPropagateHeavyUnoptimisedClass::copyGroup(GIAposRelTranslatorRulesGroupNeuralNetwork* group)
{		
	GIAposRelTranslatorRulesGroupNeuralNetwork* newGroup = GIAposRelTranslatorRules.copyGroup(group);
	
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	
	return newGroup;
}
#endif
*/

#endif


#endif
