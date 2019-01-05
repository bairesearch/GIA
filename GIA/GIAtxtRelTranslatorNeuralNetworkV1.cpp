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
 * Project Version: 3g6a 24-November-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif

/*
GIAactivationPathMostRecentContribution::GIAactivationPathMostRecentContribution(void)
{
	mostRecentContributionIndex = INT_DEFAULT_VALUE;
	mostRecentContributionWordIndex = 0;
	mostRecentContributionRequireResetGroupActivation = false;
}
GIAactivationPathMostRecentContribution::~GIAactivationPathMostRecentContribution(void)
{

}	
*/

GIAtxtRelTranslatorDebug::GIAtxtRelTranslatorDebug(void)
{
	activationMemoryGroupArrayIndex = INT_DEFAULT_VALUE;
	activationMemoryGroupArraySize = INT_DEFAULT_VALUE;
	firstComponentActive = false;
	secondComponentActive = false;	
}
GIAtxtRelTranslatorDebug::~GIAtxtRelTranslatorDebug(void)
{

}


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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = new GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
		#endif
	}
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());
	
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork)
	
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w = 0;
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup, false))
		{
			result = false;
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
		
		/*
		cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
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
	}
	else
	{
		resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace)
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
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);	
	forwardPropogationWordData->previousWordConnections.clear();
	#else
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData forwardPropogationWordDataBase;
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = &forwardPropogationWordDataBase;
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
		
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	{
		int i = currentWord->unambiguousPOSindex;
	#else
	if(currentWord->POSambiguityInfo != GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	{
		for(int i=0; i<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; i++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, i))
			{
	#endif
				GIAtxtRelTranslatorRulesGroup* inputLayerGroup = GIAtxtRelTranslatorNeuralNetworkFormation.getInputGroupLayerSection(GIAtxtRelTranslatorNeuralNetworkFormation.getFirstGroupInInputLayerSectionWordPOStype(), i);
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, i = " << i << endl;
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
				forwardPropogationWordData->wordPOStype = i;
				#endif
				inputLayerGroup->neuronPropagated = false;
				
				forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
				activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
				if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
				{
					result = true;
				}
				else
				{
					delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
				}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	}	
	#else
			}
		}
	}
	#endif
	
	GIAtxtRelTranslatorRulesGroup* inputLayerGroup = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		inputLayerGroup->neuronPropagated = false;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
		activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
		else
		{
			delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
		}
	}
	#else
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		inputLayerGroup->neuronPropagated = false;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
		activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
		else
		{
			delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
		}
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		inputLayerGroup->neuronPropagated = false;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*inputLayerGroup);
		activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
		else
		{
			delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
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
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						currentGroupInInputLayerSectionTokensLayerClassType->neuronPropagated = false;
						currentGroupInInputLayerSectionTokensLayerClass->neuronPropagated = false;
						forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*currentGroupInInputLayerSectionTokensLayerClassType);
						activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
						if(propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
						{
							result = true;
						}
						else
						{
							delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
						}
						forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w] = new GIAtxtRelTranslatorRulesGroup(*currentGroupInInputLayerSectionTokensLayerClass);
						activationPathWordFirstParseTreeGroup = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
						if(propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
						{
							result = true;
						}
						else
						{
							delete forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];
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

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
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
		
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
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

				if(componentTests(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
				{
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
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS
						GIAtxtRelTranslatorRulesGroup* activationMemoryGroupNew = copyGroup(activationMemoryGroup);
						ownerGroupOrig->activationMemoryGroupArray.push_back(activationMemoryGroupNew);
						#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
						GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupNew = copyGroup(currentParseTreeGroup);	//create a new parse tree group (in case of 1. first encounter with group or 2. recursion into different group)
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
	
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_MEMORY_GROUPS		
						GIAtxtRelTranslatorRulesComponent* activationMemoryGroupOwnerComponent = activationMemoryGroupNew->components[ownerComponentIndex];
						GIAtxtRelTranslatorRulesComponent* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
						if(currentComponentOrig->isSubcomponent)
						{
							if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
							{
								activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
							}
							else
							{
								cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect{} error: (currentComponent->isSubcomponent) && !(activationMemoryGroupOwnerComponent->hasSubComponents)" << endl;
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
						ownerGroup = activationMemoryGroupNew;
						currentComponent = activationMemoryGroupCurrentComponent;
						ownerComponent = activationMemoryGroupOwnerComponent;
						#endif
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
						GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupNew;	//use at top of parseTreeGroup at top of stack
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
										if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->w + GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET)
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
						#endif



						/*
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
						*/


						bool deleteNewlyCreatedMemoryGroup = false;

						if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, &debug))
						{
							result = true;
						}
						else
						{
							deleteNewlyCreatedMemoryGroup = true;
						}


			

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
						


					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_RECURSION	
					}
					#endif
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif	
		}
	}
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, GIAtxtRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPreviousRecord;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, lastComponentFuzzyAndCompatible, &forwardPropogationWordDataPreviousRecord))
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
			
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, debug, forwardPropogationWordDataPreviousRecord))
			{
				result = true;
			}
		}
	}
	
	return result;
}
		
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroupOrig, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, GIAtxtRelTranslatorDebug* debug, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPreviousRecord)
{
	bool result = false;
	
	GIAtxtRelTranslatorRulesGroup* ownerGroup = ownerGroupOrig;
	
	//cout << "forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;

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
	activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
	//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
	vector<GIAtxtRelTranslatorRulesGroup*> parseTreeGroupRefParseTreeGroupRefReverseOrig;
	vector<int> parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
	GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = NULL;
	int activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig = INT_DEFAULT_VALUE;
	activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverse;
	activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig = activationPathWordCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex;
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
				higherLevelComponent = activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components[activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex];
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
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	bool sentenceWordDataFullyConnected = isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", activationMemoryGroupArrayIndex = " << debug->activationMemoryGroupArrayIndex << ", firstComponentActive = " << debug->firstComponentActive << ", activationMemoryGroupArraySize = " << debug->activationMemoryGroupArraySize << ", isSentenceWordDataFullyConnected(forwardPropogationSentenceData) = " << sentenceWordDataFullyConnected << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
	if(lastComponentFuzzyAndCompatible)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible" << endl;
		#endif
		result = propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupOwner);
	}
	else
	{
	#endif
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
				
			activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
			activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
			#endif
				
			//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
			
			activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
	
			//cout << "DEBUG printBackpropParseTree:" << endl;
			//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeNameBackup, ownerGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
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

						//cout << "topLevelGroup" << endl;
						//cout << "finishedPassingSentenceWords (temp exit)" << endl;
						//exit(0);

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
					cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents.size()-1)" << endl;
					exit(EXIT_ERROR);
				}
				#endif
			}
			else
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				else
				{
					//ownerGroup->neuronPropagated = false;
				}
			}
		}
		else
		{
			/*
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
			GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
			*/
			
			//cout << "propagateWordThroughNetworkGroupComponentPassNextWord" << endl;
			result = propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupOwner);
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
	}
	#endif
	
	if(!result)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_FUZZY_LAST_COMPONENTS
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
		#endif
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

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool pass = true;
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
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroup = " << ownerGroup->groupName << ", ownerGroup->groupTypeNameBackup = " << ownerGroup->groupTypeNameBackup << endl;
					
		//pass next word though network
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			expectToSeeConnectionWithPreviousWordTrace = false;
		}
		else
		{
			expectToSeeConnectionWithPreviousWordTrace = true;
		}
	#else
	bool expectToSeeConnectionWithPreviousWordTrace = BOOL_IRRELEVANT;
	#endif
		int w = forwardPropogationWordData->w + 1;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
		cout << "forwardPropogationWordData->w+1 = " << forwardPropogationWordData->w+1 << endl;
		#endif
		
		if(propagateWordThroughNetworkIntro(translatorVariables, w, forwardPropogationSentenceData, layer+1, activationPathWordCurrentParseTreeGroup, expectToSeeConnectionWithPreviousWordTrace))
		{
			result = true;
		}
		else
		{
			//ownerGroup->neuronPropagated = false;
		}	
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
	}
	#endif	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::componentTests(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	if(currentComponent->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		
		cout << "componentTests; (currentComponent->wordNounVariantType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)" << endl;
		cout << "forwardPropogationSignalData->wordNounVariantType = " << forwardPropogationSignalData->wordNounVariantType << endl;
		cout << "currentComponent->wordNounVariantType = " << currentComponent->wordNounVariantType << endl;
		
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
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
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
		//cout << "group->previousWordPOStype = " << group->previousWordPOStype << endl;
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, previousWordPOStype))		
		{
			//cout << "componentTests = true" << endl;
			componentTests = true;
		}
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
	}
	#endif
	
	return componentTests;
}

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
	if(currentWord->POSambiguityInfo != GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	{
		for(int i=0; i<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; i++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, unambiguousPOSindex))
			{
				result = true;
			}
		}
	}
	#endif
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool lastComponentFuzzyAndCompatible, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData** forwardPropogationWordDataPreviousRecord)
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


				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
				//if(forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace)
				//{
					if(sequentialActivationFound)
					{
						//cout << "sequentialActivationFound" << endl;
						if(previousActiveComponent != NULL)
						{	
							//cout << "(previousActiveComponent != NULL)" << endl;
							//int wPrevious = forwardPropogationWordData->w - 1;
							int wPrevious = previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex - GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;	//OLD: forwardPropogationWordData->w - 1;
							if(wPrevious >= 0)	//redundant
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BASIC
								//cout << "wPrevious = " << wPrevious << endl;
								if(previousActiveComponent->neuronComponentConnectionActiveWordRecord == (*(forwardPropogationSentenceData->sentenceContents))[wPrevious])
								{
									forwardPropogationSignalData->foundPreviousActiveWord = true;
									#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
									cout << "foundPreviousActiveWord" << endl;
									#endif
								}
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
								GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPrevious = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[wPrevious]);
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
								cout << "foundPreviousActiveWord, forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
								#endif
								//cout << "forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
								//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
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
									if(find(forwardPropogationWordData->previousWordConnections.begin(), forwardPropogationWordData->previousWordConnections.end(), forwardPropogationWordDataPrevious) == forwardPropogationWordData->previousWordConnections.end())
									{
										forwardPropogationWordData->previousWordConnections.push_back(forwardPropogationWordDataPrevious);
									}
								}
								#endif
							}
							else
							{
								cerr << "wPrevious = " << wPrevious << endl;
								exit(EXIT_ERROR);
							}
						}
					}
				//}
				#endif
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
bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group, const bool deleteExistingParseTreeGroupRef)
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

bool GIAtxtRelTranslatorNeuralNetworkClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level, bool print, bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;
	
	if(!group->neuronTraced)
	{
	
		group->neuronTraced = true;

		if(performancePreprocess)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			*performance = *performance + 1;
			//won't calculate performance perfectly because some groups are reused
			#else
			/*
			if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, group->semanticRelationReturnEntityForwardPropogationSignalData->wordReference))
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
			cout << "traceBackpropParseTree: group->groupName = " << group->groupName << ", group->groupTypeNameBackup = " << group->groupTypeNameBackup << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			//#endif
		}

		for(int i=0; i<group->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i];
			
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



bool GIAtxtRelTranslatorNeuralNetworkClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(group->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(group->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<group->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
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
	
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	
	return newGroup;
}
bool GIAtxtRelTranslatorNeuralNetworkClass::copyComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIAtxtRelTranslatorRulesComponent*>* componentsNew)
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
	
	




#endif
