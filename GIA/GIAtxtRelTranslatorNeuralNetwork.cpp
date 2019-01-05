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
 * Project Version: 3g1d 24-April-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK




bool GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;

	*performance = 0;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	bool sentenceValidActivationPath = true;
	#endif
			
	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	forwardPropogationSentenceData.topLevelParseTreeGroup = NULL;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData.isQuery = GIAtxtRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
		
	GIApreprocessorPlainTextWord* previousSentenceWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
		cout << "\n\ncurrentWord = " << currentWord->tagName << endl;
		#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		cout << "currentWord->POSambiguityInfo = " << GIApreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
		#endif
		
		//word specific variables:
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData forwardPropogationWordData;
		forwardPropogationWordData.wordReference = currentWord;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
		//forwardPropogationWordData.wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		forwardPropogationWordData.previousSentenceWord = previousSentenceWord;
		forwardPropogationWordData.validActivationPathWordConnectivity = true;
		forwardPropogationWordData.foundFirstPreviousActivatedComponent = false;
		#endif	
	
		//group specific variables:
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
		forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = new GIAtxtRelTranslatorRulesGroup();
		forwardPropogationSignalData.currentParseTreeGroup = activationPathWordFirstParseTreeGroup;
		#endif
		
		if(!propagateWordThroughNetworkIntro(translatorVariables, &forwardPropogationSignalData, &forwardPropogationWordData, &forwardPropogationSentenceData))
		{
			result = false;
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		if(!forwardPropogationWordData.validActivationPathWordConnectivity)
		{
			sentenceValidActivationPath = false;
		}
		#endif
		
		previousSentenceWord = currentWord;
		
		//currentWord->wordPropagated = true;
	}
	
	if(!forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = false;
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	resetAllNeurons(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_ALL_COMPONENTS_ACTIVATED_AT_LEAST_ONCE);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE);
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
	#endif
	#endif
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	if(sentenceValidActivationPath)
	{
		*performance = forwardPropogationSentenceData.performance;
	}
	else
	{
		*performance = 0;
	}
	
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	cout << "exiting normally" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
		
	return result;
}

	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;

	GIApreprocessorPlainTextWord* currentWord = forwardPropogationWordData->wordReference;
	
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
				cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, i = " << i << endl;
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
				forwardPropogationWordData->wordPOStype = i;
				#endif

				propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
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
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
	}
	#else
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
		
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
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
						cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClassType, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
						propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClass, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData);
					}
				}
			}
		}
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	if(toplevelGroupActivationFound)
	{
		*topLevelParseTreeGroup = forwardPropogationSignalData.topLevelParseTreeGroup;
	}
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, 1);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED);
	#endif	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << endl;	
	#endif
	
	if(layer > 1)	//ensures that group activation is complete
	{
		if(group->allComponentsActivatedAtLeastOnce)
		{
			bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(group->groupTypeNameBackup, group->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
				forwardPropogationSentenceData->toplevelGroupActivationFound = true;
				//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
				cout << "topLevelGroup" << endl;
				printBackpropActivationPath(group, 1);
				GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PRINTED);
				//#endif
			}
		}
	}

			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
	
		/*
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "neuronPropagated" << endl;
		*/
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
		int activationMostRecentContribution = 0;	//higher value equals more recent
		int numberOfRecencyGradations = 0;
		#endif
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
			ownerComponent->passActivationRecencyPrelimTestsTemp = false;	//reset
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif			
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
				if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}
				#endif
				
				/*
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
				cout << "2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
				#endif
				*/
				
				if(currentComponent->semanticRelationReturnEntity)
				{
					ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
				}
				
				if(componentTests(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
					//support trailing (final) optional components:
					//1. execute propagateWordThroughNetworkGroupComponentWrapper first for a) first parse or b) final optional component parse.
					bool sequentialActivationFound = false;
					if(groupActivationCompleted(ownerGroup))
					{	
						bool requireResetGroupActivation = true;
						
						/*
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
						cout << "groupActivationCompleted(ownerGroup)" << endl;
						*/

						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PRECISE
						if(isLastComponentFuzzy(ownerGroup))
						{
						#endif		
							bool measureSequentialActivationOnly = true;
							bool measureActivationRecencyOnly = false;
							int ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED = 0;		
							if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, &ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED))
							{
								requireResetGroupActivation = false;

								/*
								GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
								cout << "executeAgain = false" << endl;
								*/
							}
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PRECISE
						}
						#endif

						if(requireResetGroupActivation)
						{
							/*
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_BASIC_UPON_ACTIVATION_RESET
							if(!GIAtxtRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsWrapper(translatorVariables, ownerGroup, layer))
							{
								result = false;
							}
							#endif
							*/
							
							//2. If fail due to find sequentialActivationFound, then resetGroupActivation
							resetGroupActivation(ownerGroup);
						}
					}
					#endif
					
					bool measureSequentialActivationOnly = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
					bool measureActivationRecencyOnly = true;
					#else
					bool measureActivationRecencyOnly = false;
					#endif
					int ownerGroupActivationMostRecentComponentContribution = 0;
					if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, &ownerGroupActivationMostRecentComponentContribution))
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
						ownerComponent->ownerGroupActivationMostRecentComponentContribution = ownerGroupActivationMostRecentComponentContribution;
						ownerComponent->passActivationRecencyPrelimTestsTemp = true;
						//cout << "ownerComponent->passActivationRecencyPrelimTestsTemp = " << ownerComponent->passActivationRecencyPrelimTestsTemp << endl;
						if(ownerGroupActivationMostRecentComponentContribution > activationMostRecentContribution)
						{
							numberOfRecencyGradations++;
							activationMostRecentContribution = ownerGroupActivationMostRecentComponentContribution;
						}
						#endif
					}
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif		
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			if(ownerComponent->passActivationRecencyPrelimTestsTemp)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD_WEAK
				if((numberOfRecencyGradations < GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD_WEAK_MIN_NUMBER_OF_RECENCY_GRADATIONS) || (ownerComponent->ownerGroupActivationMostRecentComponentContribution == activationMostRecentContribution))
				#else
				if(ownerComponent->ownerGroupActivationMostRecentComponentContribution == activationMostRecentContribution)
				#endif
				{
					bool measureSequentialActivationOnly = false;
					bool measureActivationRecencyOnly = false;
					int ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED = 0;		
					propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, &ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED);
				}
			}
			
			/*
			//DEBUG (this works):
			if(!ownerGroup->neuronPropagated)
			{
				bool measureActivationRecencyOnly = false;
				int ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED = 0;		
				propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, false, &ownerGroupActivationMostRecentComponentContributionTEMPNOTUSED);			
			}
			*/
			
			
		}
		#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureSequentialActivationOnly, const bool measureActivationRecencyOnly, int* ownerGroupActivationMostRecentComponentContribution)
{
	bool result = true;
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationWordData, &activationSequenceCompleted))
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
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents)))
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

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
			if(!measureSequentialActivationOnly)
			{
			#endif
				if(!propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, measureActivationRecencyOnly, ownerGroupActivationMostRecentComponentContribution))
				{
					result = false;
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS	
			}
			#endif
		}
	}
	return sequentialActivationFound;
}
			
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool measureActivationRecencyOnly, int* ownerGroupActivationMostRecentComponentContribution)
{
	bool result = true;
		
	ownerComponent->neuronComponentConnectionActive = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_DO_NOT_SET_SUBCOMPONENT_ACTIVATION_UNLESS_SET_COMPONENT_ACTIVATION
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	ownerComponent->neuronComponentConnectionActiveWordPOStypeRecord = forwardPropogationWordData->wordPOStype;
	#endif
	#endif
	currentComponent->parseTreeGroupRef = group;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_PARSETREE_GROUP_REFS
	currentComponent->parseTreeGroupRefs.push_back(group);
	#endif

	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);		
	#endif
	*/

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
	if(measureActivationRecencyOnly)
	{
		for(int i=0; i<ownerGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* tempComponent = (ownerGroup->components)[i];
			if(tempComponent->neuronComponentConnectionActive)
			{
				if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(tempComponent))
				{
					for(int i1=0; i1<tempComponent->subComponents.size(); i1++)
					{
						GIAtxtRelTranslatorRulesComponent* subComponent = (tempComponent->subComponents)[i1];
						if(subComponent->neuronComponentConnectionActive)
						{
							if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > *ownerGroupActivationMostRecentComponentContribution)
							{
								*ownerGroupActivationMostRecentComponentContribution = subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
								cout << "subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex << endl;
							}	
						}
					}
				}
				else
				{
					if(tempComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > *ownerGroupActivationMostRecentComponentContribution)
					{
						*ownerGroupActivationMostRecentComponentContribution = tempComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
						cout << "tempComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << tempComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex << endl;
					}
				}
			}
		}
	}
	else
	{
	#endif	
		if(activationSequenceCompleted)
		{
			ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;
			
			//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
			ownerGroup->allComponentsActivatedAtLeastOnce = true;
			ownerGroup->neuronActive = true;	//temporarily set true

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SEPARATE
			if(forwardPropogationSentenceData->parserEnabled)
			{
				//NO: only execute this function when reached the highest layer in activation tree;
				if(!GIAtxtRelTranslatorNeuralNetworkParser.generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(translatorVariables, ownerGroup, layer))
				{
					result = false;
				}
			}
			#endif
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE
			if(!GIAtxtRelTranslatorNeuralNetworkParser.updatePerformance(ownerGroup, forwardPropogationSentenceData, layer)
			{
				result = false;
			}
			#endif

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET
			//upon competing activation of group solidify neural net - all previous components and children are deactivated 
			solidifyNeuralNetIntro(ownerGroup, layer+1);
			#endif

			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
			cout << "5 activationSequenceCompleted" << endl;
			#endif
			*/

			//activation sequence completed, propagate next layer up
			if(!propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1)))
			{
				result = false;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
			GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(ownerGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED);
			#endif

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
			/*
			//upon competing activation of group solidify neural net - all previous components and children are deactivated 
			solidifyNeuralNetIntro(ownerGroup, layer+1);
			*/
			#else
			//reset ownerGroup activation sequence
			resetGroupActivation(ownerGroup);
			#endif
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_OLD
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
		//cout << "(group->previousWordPOStype != '')" << endl;
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
		if(findPreviousActivatedComponentInGroupWrapper(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, forwardPropogationWordData->wordPOStype, previousWordPOStype))		
		#else
		if(findPreviousActivatedComponentInGroupWrapper(group, currentComponent, previousWordPOStype))
		#endif
		{
			//cout << "componentTests = true" << endl;
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED);
		#endif
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
		if(findPreviousActivatedComponentInGroupWrapper(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, forwardPropogationWordData->wordPOStype, existsPreceedingWordPOStype))		
		#else
		if(findPreceedingActivatedComponentInGroupWrapper(group, currentComponent, existsPreceedingWordPOStype))
		#endif
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED);
		#endif
	}
	#endif
	
	return componentTests;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousActivatedComponentInGroupWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	int previousComponentWordPOStypeRecord = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(contextWord == currentWord)
		{
			stillReadingActivatedWords = false;
		}
		else
		{
			previousComponentWordPOStypeRecord = wordPOStype;	//NB if GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START can use currentWord->unambiguousPOSindex
		}
	}
	if(previousWordPOStype == previousComponentWordPOStypeRecord)
	{
		result = true;
	}
		
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::findPreceedingActivatedComponentInGroupWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(contextWord == currentWord)
		{
			stillReadingActivatedWords = false;
		}
		else
		{
			if(existsPreceedingWordPOStype == wordPOStype)	//NB if GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START can use currentWord->unambiguousPOSindex
			{
				result = true;
			}
		}
	}
	
	return result;
}



#else
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousOrPreceedingActivatedComponentInHigherLayerNetwork(GIAtxtRelTranslatorRulesGroup* group, int wordPOStype, bool findFirstComponent)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPreviousWordPOStypeTested)	//prevent circular loops
	{
		group->neuronPreviousWordPOStypeTested = true;
	#endif
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			GIAtxtRelTranslatorRulesGroup* ownerGroup = ownerComponent->ownerGroup;

			if(findFirstComponent)
			{
				if(findPreviousActivatedComponentInGroupWrapper(ownerGroup, ownerComponent, wordPOStype))
				{
					result = true;
				}
			}
			else
			{
				if(findPreceedingActivatedComponentInGroupWrapper(ownerGroup, ownerComponent, wordPOStype))
				{
					result = true;
				}	
			}
		}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousActivatedComponentInGroupWrapper(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int previousWordPOStype)
{
	bool result = false;
	
	//i)
	bool foundPreviousActivatedWord = false;
	if(findPreviousActivatedComponentInGroup(group, currentComponent, previousWordPOStype, &foundPreviousActivatedWord))
	{
		cout << "findPreviousActivatedComponentInGroup" << endl;
		result = true;
	}
	else
	{
		if(!foundPreviousActivatedWord)
		{
			//ii)
			if(findPreviousOrPreceedingActivatedComponentInHigherLayerNetwork(group, previousWordPOStype, true))
			{
				//cout << "findPreviousOrPreceedingActivatedComponentInHigherLayerNetwork" << endl;
				result = true;
			}
		}	
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousActivatedComponentInGroup(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int previousWordPOStype, bool* foundPreviousActivatedWord)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	int previousComponentWordPOStypeRecord = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent2 = (group->components)[i2];
		if(stillReadingActivatedWords)
		{	
			if(currentComponent2 == currentComponent)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				previousComponentWordPOStypeRecord = currentComponent2->neuronComponentConnectionActiveWordPOStypeRecord;
			}
		}
	}
	if(previousWordPOStype == previousComponentWordPOStypeRecord)
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::findPreceedingActivatedComponentInGroupWrapper(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	//i)
	if(findPreceedingActivatedComponentInGroup(group, currentComponent, existsPreceedingWordPOStype))
	{
		result = true;
	}
	else
	{
		//ii)
		if(findPreviousOrPreceedingActivatedComponentInHigherLayerNetwork(group, existsPreceedingWordPOStype, false))
		{
			result = true;
		}
	}
	
	return result;
}
	
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreceedingActivatedComponentInGroup(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent2 = (group->components)[i2];
		if(stillReadingActivatedWords)
		{
			if(currentComponent2 == currentComponent)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(existsPreceedingWordPOStype == currentComponent2->neuronComponentConnectionActiveWordPOStypeRecord)
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}
			
		
#endif
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* activationSequenceCompleted)
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
				if(!(forwardPropogationWordData->foundFirstPreviousActivatedComponent))
				{
					if(previousActiveComponent != NULL)
					{
						if(forwardPropogationWordData->previousSentenceWord != NULL)
						{
							forwardPropogationWordData->foundFirstPreviousActivatedComponent = true;
							if(forwardPropogationWordData->previousSentenceWord != previousActiveComponent->neuronComponentConnectionActiveWordRecord)
							{
								forwardPropogationWordData->validActivationPathWordConnectivity = false;
								sequentialActivationFound = false;	//CHECKTHIS
							}
							else
							{
								//forwardPropogationWordData->foundFirstPreviousActivatedComponent = true;
							}
						}
					}
				}
				#endif
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
				if(component->neuronComponentConnectionActive)
				{
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
					sequentialActivationFound = false;	//found an activated component following the newly activated (current) component - this is illegal; this should never have been activated
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PRECISE
					cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: found an activated component following the newly activated (current) component - this is illegal; this should never have been activated" << endl;
					exit(EXIT_ERROR);
					#else
					//alternate case: the first tag was optional and inactive, and the second tag was active, but now the first tag is activated
					#endif
					#else
					cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: found an activated component following the newly activated (current) component - this is illegal; this should never have been activated" << endl;
					exit(EXIT_ERROR);
					#endif	
					
					//Note if the lower layer neuron/group (e.g. noun sequence) is connected to two components in the higher layer neuron/group, only activate the component which is next in line for activation.
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
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_DO_NOT_SET_SUBCOMPONENT_ACTIVATION_UNLESS_SET_COMPONENT_ACTIVATION
				component->neuronComponentConnectionActive = true;
				#endif
				sequentialActivationFound = true;
			}
		}
	}
	
	return sequentialActivationFound;
}

//precondition: repeat sequences can contain only 1 component
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			//sequential activation found
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_DO_NOT_SET_SUBCOMPONENT_ACTIVATION_UNLESS_SET_COMPONENT_ACTIVATION
			component->neuronComponentConnectionActive = true;
			#endif
			sequentialActivationFound = true;
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
				resetGroupParseTreeGroupRef(group);
			}
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::printBackpropActivationPath(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	int performanceNOTUSED = 0;
	GIAtxtRelTranslatorNeuralNetworkParser.traceBackpropActivationPath(group, 1, true, &performanceNOTUSED, NULL);
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

bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool result = true;
	
	group->neuronActive = false;
	/*
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	if(group->groupName == "action" && group->groupTypeNameBackup == "subReferenceSetsDelimiterAndObject")
	{
		cout << "3 ******************************************************** subReferenceSetsDelimiterAndObject:action deactivated ********************************************************" << endl;
	}
	#endif
	*/
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

bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupParseTreeGroupRef(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool result = true;
	
	for(int i1=0; i1<group->components.size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
		currentComponent->parseTreeGroupRef = NULL;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_PARSETREE_GROUP_REFS
		currentComponent->parseTreeGroupRefs.clear();
		#endif
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
			{
				GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
				subComponent->parseTreeGroupRef = NULL;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_PARSETREE_GROUP_REFS
				subComponent->parseTreeGroupRefs.clear();
				#endif
			}
		}
	}
	
	return result;
}






/*
bool GIAtxtRelTranslatorNeuralNetworkClass::setParseTreeGroupRefOfAllComponents(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* parseTreeGroupRef)
{	
	bool result = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent2 = (group->components)[i2];
		currentComponent2->parseTreeGroupRef = parseTreeGroupRef;
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent2))
		{
			for(int i3=0; i3<currentComponent2->subComponents.size(); i3++)
			{
				GIAtxtRelTranslatorRulesComponent* currentComponent3 = (currentComponent2->subComponents)[i3];
				currentComponent3->parseTreeGroupRef = parseTreeGroupRef;
			}
		}
	}
	
	return result;
}
*/


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET
bool GIAtxtRelTranslatorNeuralNetworkClass::solidifyNeuralNetIntro(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool result = true;
	
	//upon competing activation of group solidify neural net - all previous components and children are deactivated 
	
	if(group->neuronActive)	
	{
		solidifyNeuralNetGroupLastComponent(group, level);
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkClass::solidifyNeuralNetIntro{} error: function called with !(group->neuronActive)" << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::solidifyNeuralNetGroupLastComponent(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool result = true;
		
	if(group->neuronActive)
	{
		int indexOfLastComponentInGroup = group->components.size()-1;
		bool lastComponentIsFuzzy = isLastComponentFuzzy(group);
		if(!lastComponentIsFuzzy)
		{
			resetGroupActivation(group);
		}
		for(int i1=0; i1<group->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
			bool solidyComponent = true;
			bool solidyLowerLevelComponents = true;
			if(lastComponentIsFuzzy)
			{
				solidyComponent = false;
				if(i1 == indexOfLastComponentInGroup)
				{
					solidyLowerLevelComponents = false;
				}
			}
			if(solidyLowerLevelComponents)
			{
				if(i1 == indexOfLastComponentInGroup)
				{
					solidifyNeuralNetComponent(currentComponent, level, true, solidyComponent);
				}
				else
				{
					solidifyNeuralNetComponent(currentComponent, level, false, solidyComponent);
				}
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::solidifyNeuralNetGroup(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	bool result = true;
		
	if(group->neuronActive)
	{
		group->neuronActive = false;	//OLD: resetGroupActivation(group);
		for(int i1=0; i1<group->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
			solidifyNeuralNetComponent(currentComponent, level, false, true);
		}
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET_BREAK_ALL_CONNECTIONS_TO_INACTIVE_GROUPS
	else
	{
		resetGroupActivation(group);		
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::solidifyNeuralNetComponent(GIAtxtRelTranslatorRulesComponent* currentComponent, int level, const bool lastComponent, const bool solidifyComponent)
{
	bool result = true;
	
	if(solidifyComponent)
	{
		currentComponent->neuronComponentConnectionActive = false;
	}
	/*
	if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		//do nothing?
	}
	*/
	if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
	{
		for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
			if(solidifyComponent)
			{
				subComponent->neuronComponentConnectionActive = false;
			}
			if(subComponent->parseTreeGroupRef != NULL)
			{
				if(lastComponent)
				{
					solidifyNeuralNetGroupLastComponent(subComponent->parseTreeGroupRef, level+1);
				}
				else
				{
					solidifyNeuralNetGroup(subComponent->parseTreeGroupRef, level+1);
				}
			}
		}
	}
	else
	{
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			if(lastComponent)
			{
				solidifyNeuralNetGroupLastComponent(currentComponent->parseTreeGroupRef, level+1);
			}
			else
			{
				solidifyNeuralNetGroup(currentComponent->parseTreeGroupRef, level+1);
			}
		}	
	}
	
	return result;
}	
	
			
#endif

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


#endif
