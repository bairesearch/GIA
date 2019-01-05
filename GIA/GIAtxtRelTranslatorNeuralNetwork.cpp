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
 * Project Version: 3g2c 21-May-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif
	
GIAactivationPathMostRecentContribution::GIAactivationPathMostRecentContribution(void)
{
	mostRecentContributionIndex = INT_DEFAULT_VALUE;
	mostRecentContributionWordIndex = 0;
	mostRecentContributionRequireResetGroupActivation = false;	
}
GIAactivationPathMostRecentContribution::~GIAactivationPathMostRecentContribution(void)
{

}	

bool GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = true;

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
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData = new GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	#endif
	
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group
	
	GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroupNOTUSED = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w = 0;
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroupNOTUSED, false))
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		//cout << "topLevelParseTreeGroupLocal" << endl;
		
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
		cout << "end printBackpropParseTree" << endl;
		//#endif		
		#endif	
	}
	else
	{
		sentenceValidActivationPath = false;
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	resetAllNeurons(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_ALL_COMPONENTS_ACTIVATED_AT_LEAST_ONCE);
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE);	//note this will leak memory for unused parseTreeGroups
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
	#endif
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	if(sentenceValidActivationPath)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		*performance = forwardPropogationSentenceData.performance;
		//cout << "forwardPropogationSentenceData.performance = " << forwardPropogationSentenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		result = true;
	}
	else
	{
		*performance = 0;
	}
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "DEBUG: finished executing GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
		
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
	cout << "currentWord = " << currentWord->tagName << endl;
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
	forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = expectToSeeConnectionWithPreviousWordTrace;
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

				if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
				{
					result = true;
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
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
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
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
		
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		inputLayerGroup->neuronPropagated = false;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
						cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						currentGroupInInputLayerSectionTokensLayerClassType->neuronPropagated = false;
						currentGroupInInputLayerSectionTokensLayerClass->neuronPropagated = false;
						if(propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
						{
							result = true;
						}
						if(propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
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

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED)
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
		
		vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupNOTUSED))
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
		
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif			
				if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}

				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE4
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
				cout << "2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
				#endif

				if(currentComponent->semanticRelationReturnEntity)
				{
					ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
				}

				if(componentTests(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
				{
					bool lastComponentFuzzyAndCompatible = false;
					bool lastComponentOptionalAndInactiveAndCompatible = false;
					bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
					int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
					GIAtxtRelTranslatorRulesComponent* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];
					if(isLastComponentFuzzy(ownerGroup))
					{
						if(lastComponent == ownerComponent)
						{
							if(lastComponent->neuronComponentConnectionActive)
							{
								if(lastComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
								{
									if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->w - 1)
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
					if(groupActivationCompleted(ownerGroup))
					{
						//cout << "groupActivationCompleted(ownerGroup)" << endl;
					}
					if(groupActivationCompleted(ownerGroup) && !lastComponentFuzzyAndCompatible)
					{
						//cout << "resetGroupActivation: ownerGroup = " << ownerGroup->groupName << endl;
						resetGroupActivation(ownerGroup);
					}

					if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupNOTUSED, lastComponentFuzzyAndCompatible))
					{
						result = true;
					}
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif		
		}
	}
	
	return result;
}	
	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED, const bool lastComponentFuzzyAndCompatible)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted))
	{
		/*
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		//cout << "3 propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady" << endl;
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
		GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
		#endif
		*/
		
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, &repeatDetected))
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

			GIAtxtRelTranslatorRulesGroup ownerGroupSave = *ownerGroup;
			GIAtxtRelTranslatorRulesComponent ownerComponentSave = *ownerComponent;
			GIAtxtRelTranslatorRulesComponent currentComponentSave = *currentComponent;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
			GIAtxtRelTranslatorRulesGroup ownerGroupCurrentParseTreeGroupSave = *(ownerGroup->currentParseTreeGroup);
			#endif
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroupNOTUSED, lastComponentFuzzyAndCompatible))
			{
				result = true;
			}
			else
			{
				//reset group to previous state;
				*ownerGroup = ownerGroupSave;
				*ownerComponent = ownerComponentSave;
				*currentComponent = currentComponentSave;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
				*(ownerGroup->currentParseTreeGroup) = ownerGroupCurrentParseTreeGroupSave;
				#endif
			}
		}
	}
	
	return result;
}
			
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED, const bool lastComponentFuzzyAndCompatible)
{
	bool result = false;
	
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	//cout << "forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	//create new parseTreeGroup
	GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//create a new component for every repeated component
	newParseComponent->parseTreeGroupRef = group->currentParseTreeGroup;
	ownerGroup->currentParseTreeGroup->components.push_back(newParseComponent);
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
	
	if(lastComponentFuzzyAndCompatible)
	{
		result = propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupNOTUSED);
	}
	else
	{
		if(activationSequenceCompleted)
		{
			//cout << "activationSequenceCompleted, repeatDetected = " << repeatDetected << endl;
			
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "activationSequenceCompleted" << endl;
			#endif
				
			ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
			ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
			#endif
				
			//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
			ownerGroup->allComponentsActivatedAtLeastOnce = true;
			ownerGroup->neuronActive = true;
	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
			GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroup;
			activationPathWordCurrentParseTreeGroupOwner->allComponentsActivatedAtLeastOnce = true;
			#endif
	
			//cout << "DEBUG printBackpropParseTree:" << endl;
			//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeNameBackup, ownerGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
				if(forwardPropogationWordData->w == forwardPropogationSentenceData->sentenceContents->size()-1)
				{
					if(isSentenceWordDataFullyConnected(forwardPropogationSentenceData))
					{
						cout << "topLevelGroup" << endl;
						exit(0);
				#endif

						result = true;
						//cout << "topLevelGroup" << endl;
						//exit(0);

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

						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						forwardPropogationSentenceData->finishedPassingSentenceWords = true;

				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
					}
					else
					{
						result = false;
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
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupNOTUSED))
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
			//cout << "propagateWordThroughNetworkGroupComponentPassNextWord" << endl;
			result = propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroupNOTUSED);
		}
	}
	
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
bool GIAtxtRelTranslatorNeuralNetworkClass::isSentenceWordDataFullyConnected(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
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
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupNOTUSED)
{
	bool result = false;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	bool pass = true;
	//cout << "forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace = " << forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace << ", forwardPropogationSignalData->foundPreviousActiveWord = " << forwardPropogationSignalData->foundPreviousActiveWord << endl;
	if(forwardPropogationWordData->expectToSeeConnectionWithPreviousWordTrace)
	{
		pass = false;
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			pass = true;
		}
	}
	if(pass)
	{
	#endif		
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroup = " << ownerGroup->groupName << ", ownerGroup->groupTypeNameBackup = " << ownerGroup->groupTypeNameBackup << endl;
					
		//pass next word though network
		int w = forwardPropogationWordData->w + 1;
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			expectToSeeConnectionWithPreviousWordTrace = false;
		}
		else
		{
			expectToSeeConnectionWithPreviousWordTrace = true;
		}
		if(propagateWordThroughNetworkIntro(translatorVariables, w, forwardPropogationSentenceData, layer+1, activationPathWordCurrentParseTreeGroupNOTUSED, expectToSeeConnectionWithPreviousWordTrace))
		{
			result = true;
		}
		else
		{
			//ownerGroup->neuronPropagated = false;
		}	
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	}
	#endif	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::componentTests(GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	if(currentComponent->wordNounVariantType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordNounVariantType == currentComponent->wordNounVariantType)
		{
			componentTests = true;
		}
	}
	if(currentComponent->wordVerbVariantType != GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordVerbVariantType == currentComponent->wordVerbVariantType)
		{
			componentTests = true;
		}
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

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted)
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
		/*
		cout << "component->neuronComponentConnectionActive = " << component->neuronComponentConnectionActive << endl;
		cout << "component->optional = " << component->optional << endl;
		cout << "component->hasSubComponents = " << GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component) << endl;
		*/
		if(stillParsingActiveComponents)
		{
			if(component == testComponent)
			{
				//cout << "(component == testComponent)" << endl;

				stillParsingActiveComponents = false;
				
				if(component->neuronComponentConnectionActive)
				{
					if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)	//if the component contains repeats, then accept it if it is already active
					{
						//sequential activation found
						//component->neuronComponentConnectionActive = true;
						
						sequentialActivationFound = true;
					}
					else
					{
						//component already activated
					}
				}
				else
				{
					//sequential activation found
					sequentialActivationFound = true;
					//cout << "activate" << endl;
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
							int wPrevious = forwardPropogationWordData->w - 1;
							if(wPrevious >= 0)	//redundant
							{
								//cout << "wPrevious = " << wPrevious << endl;
								if(previousActiveComponent->neuronComponentConnectionActiveWordRecord == (*(forwardPropogationSentenceData->sentenceContents))[wPrevious])
								{
									forwardPropogationSignalData->foundPreviousActiveWord = true;
									cout << "foundPreviousActiveWord" << endl;
									//exit(0);
								}
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_POSHOC
								GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordDataPrevious = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[wPrevious]);
								//cout << "forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
								//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
								if(find(forwardPropogationWordData->previousWordConnections.begin(), forwardPropogationWordData->previousWordConnections.end(), forwardPropogationWordDataPrevious) == forwardPropogationWordData->previousWordConnections.end())
								{
									forwardPropogationWordData->previousWordConnections.push_back(forwardPropogationWordDataPrevious);
								}
								#endif
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
				if(component->neuronComponentConnectionActive)
				{
					//cout << "!sequentialActivationFound" << endl;
					
					sequentialActivationFound = false;	//found an activated component following the newly activated (current) component - this is illegal; this should never have been activated
				}
				else
				{
					if(!(component->optional))
					{
						numberOfInactiveComponentsRemaining++;	
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
	}

	return sequentialActivationFound;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components)
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
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
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
			else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_ALL_COMPONENTS_ACTIVATED_AT_LEAST_ONCE)
			{
				boolPointer = &(group->allComponentsActivatedAtLeastOnce);
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
	int performanceNOTUSED = 0;
	traceBackpropParseTree(group, 1, true, &performanceNOTUSED, NULL);
	resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED);
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

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA2
	cout << "resetGroupActivation: group->groupName = " << group->groupName << ", group->groupTypeNameBackup = " << group->groupTypeNameBackup << endl;
	#endif
	
	group->neuronActive = false;
	for(int i1=0; i1<group->components.size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
		resetComponentActivation(currentComponent);
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::resetComponentActivation(GIAtxtRelTranslatorRulesComponent* currentComponent)
{	
	bool result = true;

	/*
	if(currentComponent->parseTreeGroupRef != NULL)
	{
		cout << "resetComponentActivation: currentComponent->parseTreeGroupRef = " << currentComponent->parseTreeGroupRef->groupName << endl;
		cout << "resetComponentActivation: currentComponent->parseTreeGroupRef->groupTypeNameBackup = " << currentComponent->parseTreeGroupRef->groupTypeNameBackup << endl;
	}
	*/
		
	currentComponent->neuronComponentConnectionActive = false;
	if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
	{
		for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
			subComponent->neuronComponentConnectionActive = false;
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
		if(group->currentParseTreeGroup != NULL)
		{
			delete group->currentParseTreeGroup;
		}
	}
	group->currentParseTreeGroup = new GIAtxtRelTranslatorRulesGroup(*group);
	group->currentParseTreeGroup->components.clear();	//added 3g1h
	
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
		traceBackpropParseTree(currentParseTreeGroup, 1, false, &performanceTemp, forwardPropogationSentenceData->sentenceContents);
		resetNeuronBackprop(currentParseTreeGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED);

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

bool GIAtxtRelTranslatorNeuralNetworkClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level, bool print, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;
	
	if(!group->neuronPrinted)
	{
		if(!print)
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
		
		group->neuronPrinted = true;
		
		if(group->allComponentsActivatedAtLeastOnce)
		{			
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
				
				if(!print)
				{
					#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
					if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
					{
						currentComponent->candidateStringMatch->alreadyFoundMatch = true;
						//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
							//"the" and "a" will use identical neural groups and so will only count to +1 performance total
					}
					#endif
				}
		
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
						
						traceBackpropParseTree(currentComponent->parseTreeGroupRef, level+1, print, performance, sentenceContents);
					}
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

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED)
	{
		boolPointer = &(group->neuronPrinted);
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





#endif
