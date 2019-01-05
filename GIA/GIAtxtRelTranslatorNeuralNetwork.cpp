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
 * Project Version: 3g1e 24-April-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
GIAtxtRelTranslatorRulesGroup* topLevelParseTreeGroupLocal;
#endif
	

bool GIAtxtRelTranslatorNeuralNetworkClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = true;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	topLevelParseTreeGroupLocal = NULL;
	#endif
				
	//sentence specific variables:
	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
	forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;	
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
	
		//group specific variables:
		GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData forwardPropogationSignalData;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
		forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = new GIAtxtRelTranslatorRulesGroup();
		#else
		GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup = NULL;	//not used
		#endif
		
		if(!propagateWordThroughNetworkIntro(translatorVariables, &forwardPropogationSignalData, &forwardPropogationWordData, &forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup))
		{
			result = false;
		}
		
		previousSentenceWord = currentWord;
		
		//currentWord->wordPropagated = true;
	}
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
		#endif	
	}
	else
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
		*performance = forwardPropogationSentenceData.performance;
		#else
		*performance = 1;
		#endif
		result = true;
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

	


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
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

				propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
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
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
	}
	#else
	if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(wordLowerCase, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
		
	}
	else if(GIAtxtRelTranslatorNeuralNetworkFormation.findWordInGroupMap(currentWord->tagName, GIAtxtRelTranslatorNeuralNetworkFormation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkIntro: GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
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
						propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClassType, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
						propagateWordThroughNetworkGroupIntro(translatorVariables, currentGroupInInputLayerSectionTokensLayerClass, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordFirstParseTreeGroup);
					}
				}
			}
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroup* activationPathWordFirstParseTreeGroup)
{
	bool result = true;
	
	bool measureActivationRecencyOnly = false;
	propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, 1, measureActivationRecencyOnly);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED);
	#endif	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureActivationRecencyOnly)
{
	bool result = true;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << endl;
	if(measureActivationRecencyOnly)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[31m measureActivationRecencyOnly \e[0m" << endl;
		#endif
	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[32m !measureActivationRecencyOnly \e[0m" << endl;
		#endif
	}
	//cout << "measureActivationRecencyOnly = " << measureActivationRecencyOnly << endl;
	#endif
	
	if(layer > 1)	//ensures that group activation is complete
	{
		if(!measureActivationRecencyOnly)
		{
			if(group->allComponentsActivatedAtLeastOnce)
			{
				bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(group->groupTypeNameBackup, group->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
				if(topLevelGroup)
				{
					forwardPropogationSentenceData->toplevelGroupActivationFound = true;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
					topLevelParseTreeGroupLocal = group->currentParseTreeGroup;
					#endif

					//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					cout << "topLevelGroup" << endl;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
					printBackpropActivationPath(topLevelParseTreeGroupLocal, 1);
					#else
					printBackpropActivationPath(group, 1);			
					#endif
					cout << "end printBackpropActivationPath" << endl;
					//#endif
				}
			}
		}
	}

			
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
	
		/*
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "neuronPropagated" << endl;
		#endif
		*/
		
		bool measureActivationRecencyOnlyFirstBifurcation = false;
		bool measureActivationRecencyOnlyNew = false;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		measureActivationRecencyOnlyNew = true;
		if(!measureActivationRecencyOnly)
		{
			//perform test activation (identify optimum path/s)
			measureActivationRecencyOnlyFirstBifurcation = true;
			
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "measureActivationRecencyOnlyFirstBifurcation = " << measureActivationRecencyOnlyFirstBifurcation << endl;
			*/
		}
		else
		{
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "measureActivationRecencyOnlyFirstBifurcation = " << measureActivationRecencyOnlyFirstBifurcation << endl;			
			*/
		}
		#endif
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[34m propagateWordThroughNetworkGroupSelect: \e[0m" << endl;
		#endif
		
		bool foundFrontLayerActivationPathMostRecentContribution = false;
		vector<int> frontLayerActivationPathMostRecentContributionIndex;
		vector<bool> frontLayerActivationPathMostRecentContributionRequireResetGroupActivation;
		if(!propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureActivationRecencyOnlyNew, measureActivationRecencyOnlyFirstBifurcation, &foundFrontLayerActivationPathMostRecentContribution, &frontLayerActivationPathMostRecentContributionIndex, &frontLayerActivationPathMostRecentContributionRequireResetGroupActivation))
		{
			result = false;
		}
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		cout << "\n" << endl;
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		//perform real activation (activate optimum path/s)
		if(measureActivationRecencyOnlyFirstBifurcation)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[34m measureActivationRecencyOnlyFirstBifurcation: \e[0m" << endl;
			#endif
			
			if(foundFrontLayerActivationPathMostRecentContribution)
			{	
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				cout << "\e[33m !activate all pathways \e[0m" << endl;
				#endif
				
				//activate optimum pathway;
				for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
				{
					bool foundMostRecentContribution = false;
					int mostRecentContributionNumber = 0;
					for(int q=0; q<frontLayerActivationPathMostRecentContributionIndex.size(); q++)
					{
						if(frontLayerActivationPathMostRecentContributionIndex[q] == i)
						{
							foundMostRecentContribution = true;
							mostRecentContributionNumber = q;
						}
					}
					if(foundMostRecentContribution)
					{
						GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
						GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
						GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
						if(currentComponent->isSubcomponent)
						{
							ownerComponent = currentComponent->ownerComponent;
						}

						if(frontLayerActivationPathMostRecentContributionRequireResetGroupActivation[mostRecentContributionNumber])
						{
							resetGroupActivation(ownerGroup);
						}

						bool measureSequentialActivationOnly = false;
						bool measureActivationRecencyOnly = false;
						propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, false);
					}
				}
			}
			else
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				cout << "\e[33m activate all pathways \e[0m" << endl;
				#endif
			
				//activate all pathways;
				bool measureActivationRecencyOnly = false;
				bool measureActivationRecencyOnlyFirstBifurcation = false;
				bool foundFrontLayerActivationPathMostRecentContribution = false;	//NOT USED
				vector<int> frontLayerActivationPathMostRecentContributionIndex;	//NOT USED
				vector<bool> frontLayerActivationPathMostRecentContributionRequireResetGroupActivation;	//NOT USED
				if(!propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureActivationRecencyOnly, measureActivationRecencyOnlyFirstBifurcation, &foundFrontLayerActivationPathMostRecentContribution, &frontLayerActivationPathMostRecentContributionIndex, &frontLayerActivationPathMostRecentContributionRequireResetGroupActivation))
				{
					result = false;
				}			
			}
		}
		#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif
	
	return result;
}
		
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureActivationRecencyOnly, const bool measureActivationRecencyOnlyFirstBifurcation, bool* foundFrontLayerActivationPathMostRecentContribution, vector<int>* frontLayerActivationPathMostRecentContributionIndex, vector<bool>* frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	int frontLayerActivationPathMostRecentContributionWordIndex = INT_DEFAULT_VALUE;
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

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		if(!ownerGroup->neuronPropagated)	//prevent circular loops
		{
		#endif			
			//#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE
			if(currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
			}
			//#endif

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
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				cout << "\e[36m i = " << i << " \e[0m" << endl;
				#endif
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
				//support trailing (final) optional components:
				//1. execute propagateWordThroughNetworkGroupComponentWrapper first for a) first parse or b) final optional component parse.
				bool sequentialActivationFound = false;
				bool requireResetGroupActivation = false;
				if(groupActivationCompleted(ownerGroup))
				{	
					requireResetGroupActivation = true;

					/*
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
					cout << "groupActivationCompleted(ownerGroup)" << endl;
					#endif
					*/

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PRECISE
					if(isLastComponentFuzzy(ownerGroup))
					{
					#endif		
						bool measureSequentialActivationOnly = true;
						if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, false))
						{
							requireResetGroupActivation = false;

							/*
							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
							GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
							cout << "executeAgain = false" << endl;
							#endif
							*/
						}
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PRECISE
					}
					#endif

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_PREVENT_RESET_OF_NEWLY_ACTIVATED_COMPONENT_GROUP
					//do not execute resetGroupActivation if the last component to be activated corresponds to the same word being fed into the neural net (in case a higher level group can be activated by more than 1 lower level pathway)
					if(requireResetGroupActivation)
					{
						bool foundLastActivatedComponent = 0;
						for(int i2=ownerGroup->components.size()-1; i2>=0; i2--)
						{
							GIAtxtRelTranslatorRulesComponent* component = (ownerGroup->components)[i2];
							if(!foundLastActivatedComponent)
							{
								if(component->neuronComponentConnectionActive)
								{
									foundLastActivatedComponent = true;
									if(component->neuronComponentConnectionActiveWordRecord == forwardPropogationWordData->wordReference)
									{
										requireResetGroupActivation = false;
									}
								}
							}
						}
					}
					#endif
					
					if(requireResetGroupActivation)
					{
						//2. If fail due to find sequentialActivationFound, then resetGroupActivation
						if(!measureActivationRecencyOnly)
						{
							resetGroupActivation(ownerGroup);
						}
					}
				}
				#endif

				bool measureSequentialActivationOnly = false;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
				if(measureActivationRecencyOnlyFirstBifurcation)
				{
					forwardPropogationWordData->foundMostRecentContribution = false;
					forwardPropogationWordData->mostRecentContributionWordIndex = INT_DEFAULT_VALUE;
				}
				#endif
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, requireResetGroupActivation))
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					if(measureActivationRecencyOnlyFirstBifurcation)
					{
						if(forwardPropogationWordData->foundMostRecentContribution)
						{
							if(forwardPropogationWordData->mostRecentContributionWordIndex >= frontLayerActivationPathMostRecentContributionWordIndex)
							{
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
								GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
								cout << "forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << ", i = " << i << endl;
								#endif
								
								frontLayerActivationPathMostRecentContributionWordIndex = forwardPropogationWordData->mostRecentContributionWordIndex;
								*foundFrontLayerActivationPathMostRecentContribution = true;
								frontLayerActivationPathMostRecentContributionIndex->push_back(i);
								frontLayerActivationPathMostRecentContributionRequireResetGroupActivation->push_back(requireResetGroupActivation);
							}
						}
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_WEAK
						else
						{
							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
							GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
							cout << "!(forwardPropogationWordData->foundMostRecentContribution)" << endl;
							#endif
							
							*foundFrontLayerActivationPathMostRecentContribution = true;
							frontLayerActivationPathMostRecentContributionIndex->push_back(i);
							frontLayerActivationPathMostRecentContributionRequireResetGroupActivation->push_back(requireResetGroupActivation);
						}
						#endif
					}
					#endif
				}
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		}
		#endif		
	}
}	
		

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureSequentialActivationOnly, const bool measureActivationRecencyOnly, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
{
	bool result = true;
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(ownerComponent, &(ownerGroup->components), forwardPropogationWordData, &activationSequenceCompleted, frontLayerActivationPathMostRecentContributionRequireResetGroupActivation))
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
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(currentComponent, &(ownerComponent->subComponents), frontLayerActivationPathMostRecentContributionRequireResetGroupActivation))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(currentComponent, &(ownerComponent->subComponents), frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, &repeatDetected))
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

			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
			if(!(measureActivationRecencyOnly && forwardPropogationWordData->foundMostRecentContribution))	//prevents wasting computational resources continuing to find activation path
			{
			#endif	
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
				if(!measureSequentialActivationOnly)
				{
				#endif
					if(!propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, measureActivationRecencyOnly, repeatDetected))
					{
						result = false;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS	
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
			}
			#endif
		}
	}
	return sequentialActivationFound;
}
			
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool measureActivationRecencyOnly, const bool repeatDetected)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	if(!measureActivationRecencyOnly)
	{
	#endif
		ownerComponent->neuronComponentConnectionActive = true;
		currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY
		currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		//cout << "forwardPropogationWordData->wordReference = " << forwardPropogationWordData->wordReference->tagName << endl;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_BACK_GROUP_REF_REPEAT_COUNTER
		bool repeatedComponentFound = false;
		if(currentComponent->parseTreeGroupRef == group)
		{
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(repeatDetected)
				{
					ownerComponent->parseTreeGroupRefRepeatCounter = ownerComponent->parseTreeGroupRefRepeatCounter+1;
				}
				else
				{
					cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent error{}: (currentComponent->parseTreeGroupRef == group) && !repeatDetected " << endl;
				}
			}
			else
			{
				cerr << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent error{}: (currentComponent->parseTreeGroupRef == group) && (ownerComponent->componentType != GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT) " << endl;	
			}	
		}
		#endif
		currentComponent->parseTreeGroupRef = group;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
		currentComponent->parseTreeGroupRefs.push_back(group);
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
		//upon competing activation of component a) solidify neural net subgroup - all children deactivated 
		//solidifyNeuralNetGroupLastComponent(group, layer+1);	//removed GIA3g1eTEMP42
		//b) enforce solidification of previous active component subgroup
		bool foundCurrentActiveComponent = false;
		bool foundPreviousActiveComponent = false;
		for(int i=ownerGroup->components.size()-1; i>=0; i--)
		{
			GIAtxtRelTranslatorRulesComponent* component = (ownerGroup->components)[i];
			if(foundCurrentActiveComponent)
			{
				if(!foundPreviousActiveComponent)
				{
					if(component->neuronComponentConnectionActive)
					{
						foundPreviousActiveComponent = true;
						cout << "solidifyNeuralNetGroup:, foundPreviousActiveComponent: ownerGroup = " << ownerGroup->groupName << endl;
						GIAtxtRelTranslatorRules.printComponent(component, 0);
						if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component))
						{
							int numberActiveSubcomponents = 0;
							for(int i1=0; i1<component->subComponents.size(); i1++)
							{
								GIAtxtRelTranslatorRulesComponent* subComponent = (component->subComponents)[i1];
								if(subComponent->neuronComponentConnectionActive)
								{
									#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
									for(int i3=0; i3<subComponent->parseTreeGroupRefs.size(); i3++)
									{
										GIAtxtRelTranslatorRulesGroup* subComponentGroupRef = (subComponent->parseTreeGroupRefs)[i3];
										solidifyNeuralNetGroup(subComponentGroupRef, layer+1);	
									}	
									#else
									solidifyNeuralNetGroup(subComponent->parseTreeGroupRef, layer+1);		
									#endif
								}
							}
						}
						else
						{
							if(component->parseTreeGroupRef != NULL)
							{
								solidifyNeuralNetGroup(component->parseTreeGroupRef, layer+1);	
							}
						}
						cout << "end solidifyNeuralNetGroup" << endl;
					}
				}
			}
			
			if(component == ownerComponent)
			{
				foundCurrentActiveComponent = true;
			}
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	}
	#endif
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);		
	#endif

	if(activationSequenceCompleted)
	{
	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
			
		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required regardless of measureActivationRecencyOnly to pass wordNounVariantType/wordVerbVariantType

		GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = NULL;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		if(!measureActivationRecencyOnly)
		{
		#endif
			
			//record variables for GIAtxtRelTranslatorNeuralNetworkParse;
			ownerGroup->allComponentsActivatedAtLeastOnce = true;
				
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
			if(ownerGroup->neuronActive)
			{
				//append newly found optional component to existing group;
				GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//create a new component for every repeated component
				newParseComponent->parseTreeGroupRef = currentComponent->parseTreeGroupRef->currentParseTreeGroup;
				ownerGroup->currentParseTreeGroup->components.push_back(newParseComponent);
				activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroup;	//change activationPathWordCurrentParseTreeGroupOwner to existing ownerGroup->currentParseTreeGroup
				//OLD: stop word path activation here (as going into existing tree)? - no because want to update higher level neuronComponentConnectionActiveWordRecord for GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY?
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS_CLEAR_UPON_GROUP_ACTIVATION
				bool lastComponentIsFuzzy = isLastComponentFuzzy(ownerGroup);
				#endif
				
				//replicate ownerGroup
				activationPathWordCurrentParseTreeGroupOwner = new GIAtxtRelTranslatorRulesGroup();
				*activationPathWordCurrentParseTreeGroupOwner = *(ownerGroup);	//copyParseGroupInfo(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup)	
				//reforge component list (remove all inactive optional components)
				activationPathWordCurrentParseTreeGroupOwner->components.clear();
				for(int i=0; i<ownerGroup->components.size(); i++)
				{
					GIAtxtRelTranslatorRulesComponent* component = (ownerGroup->components)[i];

					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS_CLEAR_UPON_GROUP_ACTIVATION
					bool isLastComponent = false;
					if(i == ownerGroup->components.size()-1)
					{
						isLastComponent = true;
					}
					#endif
							
					if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component))
					{
						int numberActiveSubcomponents = 0;
						for(int i1=0; i1<component->subComponents.size(); i1++)
						{
							GIAtxtRelTranslatorRulesComponent* subComponent = (component->subComponents)[i1];
							if(subComponent->neuronComponentConnectionActive)
							{	
								numberActiveSubcomponents++;
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
								for(int i2=0; i2<subComponent->parseTreeGroupRefs.size(); i2++)
								{
									GIAtxtRelTranslatorRulesGroup* tempGroup = subComponent->parseTreeGroupRefs[i2];
									GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*subComponent);	//create a new component for every repeated component
									newParseComponent->parseTreeGroupRef = tempGroup->currentParseTreeGroup;
									activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
								}
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS_CLEAR_UPON_GROUP_ACTIVATION
								if(!(isLastComponent && lastComponentIsFuzzy))
								{
									subComponent->parseTreeGroupRefs.clear();	//3g1eTEMP38
									subComponent->parseTreeGroupRef = NULL;
								}
								#else
								//subComponent->parseTreeGroupRefs.clear();	//added 3g1eTEMP34	//removed 3g1eTEMP38 - dont clear parseTreeGroupRef/parseTreeGroupRefs upon activationSequenceCompleted (in case a higher level function needs to solidify through them) [alt implementation: dont execute parseTreeGroupRefs.clear() for last component for last component if it is fuzzy] 				
								#endif
								#else
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_BACK_GROUP_REF_REPEAT_COUNTER
								//added 3g1eTEMP35
								if(component->parseTreeGroupRefRepeatCounter > 0)
								{
									for(int i2=0; i2<component->parseTreeGroupRefRepeatCounter; i2++)
									{
										GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*subComponent);	//create a new component for every repeated component
										newParseComponent->parseTreeGroupRef = subComponent->parseTreeGroupRef->currentParseTreeGroup;
										activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
									}
									component->parseTreeGroupRefRepeatCounter = 0;	//reset
								}
								else
								{
								#endif
									GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*subComponent);	//copy rules component
									newParseComponent->parseTreeGroupRef = subComponent->parseTreeGroupRef->currentParseTreeGroup;
									activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
								#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_BACK_GROUP_REF_REPEAT_COUNTER	
								}
								#endif
								#endif
							}
						}
						if(numberActiveSubcomponents > 1)
						{
							cerr << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE: GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent{} error: if(numberActiveSubcomponents > 1)" << endl;
							exit(EXIT_ERROR);
						}
					}
					else
					{
						if(component->neuronComponentConnectionActive)	//CHECKTHIS
						{
							GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*component);	//copy rules component
							newParseComponent->parseTreeGroupRef = component->parseTreeGroupRef->currentParseTreeGroup;
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS_CLEAR_UPON_GROUP_ACTIVATION
							if(!(isLastComponent && lastComponentIsFuzzy))
							{
								component->parseTreeGroupRefs.clear();	//3g1eTEMP38
								component->parseTreeGroupRef = NULL;
							}
							#else
							//component->parseTreeGroupRefs.clear();	//added 3g1eTEMP34	//removed 3g1eTEMP38 - dont clear parseTreeGroupRef/parseTreeGroupRefs upon activationSequenceCompleted (in case a higher level function needs to solidify through them) [alt implementation: dont execute parseTreeGroupRefs.clear() for last component for last component if it is fuzzy] 				
							#endif
							#endif
							activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
						}
					}
				}
				
				ownerGroup->currentParseTreeGroup = activationPathWordCurrentParseTreeGroupOwner;
			}
			#endif
			
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
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
			if(!GIAtxtRelTranslatorNeuralNetworkParser.updatePerformance(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
			#else
			if(!GIAtxtRelTranslatorNeuralNetworkParser.updatePerformance(ownerGroup, forwardPropogationSentenceData, layer))
			#endif
			{
				result = false;
			}
			#endif
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_SOLIDIFY_NET_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
			//upon competing activation of group solidify neural net - all previous components and children are deactivated 
			solidifyNeuralNetIntro(ownerGroup, layer+1);
			#endif		
			#endif

			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		}
		#endif
		
		//activation sequence completed, propagate next layer up
		if(!propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), measureActivationRecencyOnly))
		{
			result = false;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronForwardProp(ownerGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED);
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		if(!measureActivationRecencyOnly)
		{
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		}
		#endif
	}

	
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
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, forwardPropogationWordData->wordPOStype, previousWordPOStype))		
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
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, forwardPropogationWordData->wordPOStype, existsPreceedingWordPOStype))		
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
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int previousWordPOStype)
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
bool GIAtxtRelTranslatorNeuralNetworkClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int wordPOStype, int existsPreceedingWordPOStype)
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
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* activationSequenceCompleted, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
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
					else if(frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
					{
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
				if(sequentialActivationFound)
				{
					if(previousActiveComponent != NULL)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
						if(!forwardPropogationWordData->foundMostRecentContribution)
						{
						#endif
							forwardPropogationWordData->foundMostRecentContribution = true;
							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
							//cout << "frontLayerActivationPathMostRecentContributionRequireResetGroupActivation = " << frontLayerActivationPathMostRecentContributionRequireResetGroupActivation << endl;
							cout << "previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex << endl;
							#endif
							forwardPropogationWordData->mostRecentContributionWordIndex = previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
						}
						#endif
					}
				}
				#endif
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;

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
					
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					if(frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
					{
						if(!(component->optional))
						{
							numberOfInactiveComponentsRemaining++;	
						}
					}
					else
					{
					#endif
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
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					}
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


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
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
				if(frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
				{
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
			}
		}
	}
	
	return sequentialActivationFound;
}

//precondition: repeat sequences can contain only 1 component
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, bool* repeatDetected)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* component = (*components)[i];
		if(component == testComponent)
		{
			//sequential activation found
			if(component->neuronComponentConnectionActive)
			{
				*repeatDetected = true;
			}
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
	GIAtxtRelTranslatorNeuralNetworkParser.resetNeuronBackprop(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED);
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
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	if(group->currentParseTreeGroup != NULL)
	{
		group->currentParseTreeGroup = NULL;
	}
	#endif
	*/
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::resetComponentActivation(GIAtxtRelTranslatorRulesComponent* currentComponent)
{	
	bool result = true;
	
	if(currentComponent->parseTreeGroupRef != NULL)
	{
		cout << "resetComponentActivation: currentComponent->parseTreeGroupRef = " << currentComponent->parseTreeGroupRef->groupName << endl;
		cout << "resetComponentActivation: currentComponent->parseTreeGroupRef->groupTypeNameBackup = " << currentComponent->parseTreeGroupRef->groupTypeNameBackup << endl;
	}
	currentComponent->neuronComponentConnectionActive = false;
	currentComponent->parseTreeGroupRef = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
	currentComponent->parseTreeGroupRefs.clear();
	#endif
	if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
	{
		for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
			subComponent->neuronComponentConnectionActive = false;
			subComponent->parseTreeGroupRef = NULL;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
			subComponent->parseTreeGroupRefs.clear();
			#endif
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
		currentComponent->parseTreeGroupRefs.clear();
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_BACK_GROUP_REF_REPEAT_COUNTER
		currentComponent->parseTreeGroupRefRepeatCounter = 0;
		#endif
		if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			for(int i1=0; i1<currentComponent->subComponents.size(); i1++)
			{
				GIAtxtRelTranslatorRulesComponent* subComponent = (currentComponent->subComponents)[i1];
				subComponent->parseTreeGroupRef = NULL;
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
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
		/*
		//removed @3g1eTEMP39
		if(!lastComponentIsFuzzy)
		{
			//resetGroupActivation(group);
		}
		*/
		for(int i1=0; i1<group->components.size(); i1++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
			bool solidifyComponent = true;
			bool solidifyLowerLevelComponents = true;
			if(lastComponentIsFuzzy)
			{
				GIAtxtRelTranslatorRulesComponent* lastComponent = (group->components)[i1];
				if(!(lastComponent->neuronComponentConnectionActive))		//added @3g1eTEMP41;
				{
					solidifyComponent = false;
				}
				
				if(i1 == indexOfLastComponentInGroup)
				{
					if(!(currentComponent->neuronComponentConnectionActive))	//added @3g1eTEMP40
					{
						solidifyLowerLevelComponents = false;
					}
				}
			}
			if(solidifyLowerLevelComponents)
			{
				if(i1 == indexOfLastComponentInGroup)
				{
					solidifyNeuralNetComponent(currentComponent, level, true, solidifyComponent);
				}
				else
				{
					solidifyNeuralNetComponent(currentComponent, level, false, solidifyComponent);
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
		resetGroupActivation(group);
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
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
			for(int i2=0; i2<subComponent->parseTreeGroupRefs.size(); i2++)
			{
				GIAtxtRelTranslatorRulesGroup* tempGroup = subComponent->parseTreeGroupRefs[i2];
				
				if(lastComponent)
				{
					solidifyNeuralNetGroupLastComponent(tempGroup, level+1);
				}
				else
				{
				
					solidifyNeuralNetGroup(tempGroup, level+1);
				}
			}
			#endif
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
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_STORE_MULTIPLE_BACK_GROUP_REFS
		for(int i2=0; i2<currentComponent->parseTreeGroupRefs.size(); i2++)
		{
			GIAtxtRelTranslatorRulesGroup* tempGroup = currentComponent->parseTreeGroupRefs[i2];
			
			if(lastComponent)
			{
				solidifyNeuralNetGroupLastComponent(tempGroup, level+1);
			}
			else
			{
				solidifyNeuralNetGroup(tempGroup, level+1);
			}
		}
		#endif	
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
