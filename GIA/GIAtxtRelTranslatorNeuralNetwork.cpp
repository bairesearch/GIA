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
 * Project Version: 3g5a 27-October-2018
 * Requirements: 
 * Description: Textual Relation Translator Neural Network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetwork.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
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
	
	bool sentenceValidActivationPath = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
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
		measurePerformance(topLevelParseTreeGroupLocal, &forwardPropogationSentenceData, GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LAYER_START);
		cout << "forwardPropogationSentenceData.toplevelGroupActivationFound, performance = " << forwardPropogationSentenceData.performance << endl;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TEMPORARY_WORKAROUND_TAKE_POS_PERMUTATION_CORRESPONDING_TO_SUCCESSFUL_PARSE_TREE
		if(forwardPropogationSentenceData.performance == sentenceContents->size())
		{
		#endif
			sentenceValidActivationPath = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
			*topLevelParseTreeGroup = topLevelParseTreeGroupLocal;
			cout << "sentenceValidActivationPath = true" << endl;
			#endif	

			/*
			cout << "topLevelGroup [FIRST]" << endl;
			printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
			cout << "end printBackpropParseTree" << endl;
			*/
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_TEMPORARY_WORKAROUND_TAKE_POS_PERMUTATION_CORRESPONDING_TO_SUCCESSFUL_PARSE_TREE
		}
		#endif
	}

	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	if(sentenceValidActivationPath)
	{
		resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		//keep existing valid parseTreeGroup heirachy in memory
	}
	else
	{
		resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF_ENFORCE_MEM_CLEAR);	//clear parseTreeGroup heirachy memory
	}
	#endif
	resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE);
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
	int layerStart = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LAYER_START;
	propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layerStart, measureActivationRecencyOnly, activationPathWordFirstParseTreeGroup, false);
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
	resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED);
	#endif	
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureActivationRecencyOnly, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = true;
		
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
	if(measureActivationRecencyOnly)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "\e[31m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;
		#endif
	}
	else
	{
		if(layer > GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LAYER_START)
		{
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeNameBackup << ":" << group->groupName << " \e[0m" << endl;
		
			/*
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA6
			cout << "printBackpropParseTree" << endl;
			printBackpropParseTree(group->currentParseTreeGroup, 0);
			#endif
			*/
		}		
	}
	//cout << "measureActivationRecencyOnly = " << measureActivationRecencyOnly << endl;
	#endif
			
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
		
		vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(!propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureActivationRecencyOnlyNew, measureActivationRecencyOnlyFirstBifurcation, &frontLayerActivationPathMostRecentContribution, activationPathWordCurrentParseTreeGroup, passThrough))
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
				
			//activate optimum pathway;
			for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
			{
				bool foundMostRecentContribution = false;
				GIAactivationPathMostRecentContribution* mostRecentContribution = NULL;
				for(int q=0; q<frontLayerActivationPathMostRecentContribution.size(); q++)
				{
					GIAactivationPathMostRecentContribution* currentMostRecentContribution = &(frontLayerActivationPathMostRecentContribution[q]);
					if(currentMostRecentContribution->mostRecentContributionIndex == i)
					{
						mostRecentContribution = currentMostRecentContribution;
						foundMostRecentContribution = true;
					}
				}
				if(foundMostRecentContribution)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[34m foundMostRecentContribution: i = " << i << " \e[0m" << endl;
					#endif
					
					GIAtxtRelTranslatorRulesComponent* currentComponent = (group->ANNfrontComponentConnectionList)[i];
					GIAtxtRelTranslatorRulesGroup* ownerGroup = currentComponent->ownerGroup;	
					GIAtxtRelTranslatorRulesComponent* ownerComponent = currentComponent;
					if(currentComponent->isSubcomponent)
					{
						ownerComponent = currentComponent->ownerComponent;
					}

					if(mostRecentContribution->mostRecentContributionRequireResetGroupActivation)
					{
						resetGroupActivation(ownerGroup);
					}

					bool measureSequentialActivationOnly = false;
					bool measureActivationRecencyOnly = false;
					bool requireResetGroupActivation = false;
					propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, &requireResetGroupActivation, activationPathWordCurrentParseTreeGroup);
				}
			}
		}
		#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureActivationRecencyOnly, const bool measureActivationRecencyOnlyFirstBifurcation, vector<GIAactivationPathMostRecentContribution>* frontLayerActivationPathMostRecentContribution, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, const bool passThrough)
{
	bool result = true;
	
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

				if(componentTests(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
				{
					if(currentComponent->semanticRelationReturnEntity)
					{
						ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
					}

					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[36m i = " << i << " \e[0m" << endl;
					#endif
					
					int indexOfFirstComponentInGroup = 0;
					int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
					GIAtxtRelTranslatorRulesComponent* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
					GIAtxtRelTranslatorRulesComponent* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];
					
					/*
					//none of this code is presently used;
					//changed 3g5a (change 3g1o code to 3g3a code);
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
									//if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex == forwardPropogationWordData->w - 1)
									//{
										lastComponentRepeatAndActivatedByPreviousWordAndCompatible = true;
										lastComponentFuzzyAndCompatible = true;
										//cout << "lastComponentRepeatAndActivatedByPreviousWord" << endl;
									//}
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
						cout << "requireResetGroupActivation" << endl;

						//2. If fail due to find sequentialActivationFound, then resetGroupActivation
						if(!measureActivationRecencyOnly)
						{
							//cout << "resetGroupActivation" << endl;
							resetGroupActivation(ownerGroup);
							requireResetGroupActivation = false;
						}
					}


					bool measureSequentialActivationOnly = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
					if(measureActivationRecencyOnlyFirstBifurcation)
					{
						//cout << "measureActivationRecencyOnlyFirstBifurcation" << endl;
						forwardPropogationWordData->foundMostRecentContribution = false;
						forwardPropogationWordData->mostRecentContributionWordIndex = INT_DEFAULT_VALUE;
					}
					#endif

					if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, measureSequentialActivationOnly, measureActivationRecencyOnly, &requireResetGroupActivation, activationPathWordCurrentParseTreeGroup))
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
						if(measureActivationRecencyOnlyFirstBifurcation)
						{
							if(forwardPropogationWordData->foundMostRecentContribution)
							{
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
								GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
								cout << "\e[36m i = " << i << ", forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << "\e[0m" << endl;
								#endif

								if(forwardPropogationWordData->mostRecentContributionWordIndex >= mostRecentContributionWordIndex)
								{
									//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
									//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
									//cout << "*foundFrontLayerActivationPathMostRecentContribution = true" << endl;
									//cout << "requireResetGroupActivation = " << requireResetGroupActivation << endl;
									GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
									//firstBifurcation propagateWordThroughNetworkGroupSelect: 
									cout << "\e[36m " << ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", i = " << i << ", forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << "\e[0m" << endl;
									//#endif

									mostRecentContributionWordIndex = forwardPropogationWordData->mostRecentContributionWordIndex;

									GIAactivationPathMostRecentContribution currentMostRecentContribution;	//not on heap
									currentMostRecentContribution.mostRecentContributionIndex = i;
									currentMostRecentContribution.mostRecentContributionWordIndex = forwardPropogationWordData->mostRecentContributionWordIndex;
									currentMostRecentContribution.mostRecentContributionRequireResetGroupActivation = requireResetGroupActivation;
									frontLayerActivationPathMostRecentContribution->push_back(currentMostRecentContribution);
								}
							}
							#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREFERENCE_MOST_RECENT_ACTIVATED_PATHWAY_WEAK
							else
							{
								#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA3
								GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
								cout << "!(forwardPropogationWordData->foundMostRecentContribution)" << endl;
								#endif

								GIAactivationPathMostRecentContribution currentMostRecentContribution;	//not on heap
								currentMostRecentContribution.mostRecentContributionIndex = i;
								currentMostRecentContribution.mostRecentContributionWordIndex = 0;
								currentMostRecentContribution.mostRecentContributionRequireResetGroupActivation = requireResetGroupActivation;
								frontLayerActivationPathMostRecentContribution->push_back(currentMostRecentContribution);
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
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	if(measureActivationRecencyOnlyFirstBifurcation)
	{
		removeLessRecentContributionIndices(frontLayerActivationPathMostRecentContribution);
	}
	#endif
	
	return result;
}	


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
								

bool GIAtxtRelTranslatorNeuralNetworkClass::removeLessRecentContributionIndices(vector<GIAactivationPathMostRecentContribution>* frontLayerActivationPathMostRecentContribution)
{
	int maxMostRecentContributionWordIndex = 0;
	for(int q=0; q<frontLayerActivationPathMostRecentContribution->size(); q++)
	{
		int mostRecentContributionWordIndex = ((*frontLayerActivationPathMostRecentContribution)[q]).mostRecentContributionWordIndex;
		if(mostRecentContributionWordIndex > maxMostRecentContributionWordIndex)
		{
			maxMostRecentContributionWordIndex = mostRecentContributionWordIndex;
		}
	}
	//cout << "maxMostRecentContributionWordIndex = " << maxMostRecentContributionWordIndex << endl;
	
	if(maxMostRecentContributionWordIndex > 0)
	{
		int vectorSize = frontLayerActivationPathMostRecentContribution->size();
		int q=0;
		for(vector<GIAactivationPathMostRecentContribution>::iterator iter1 = frontLayerActivationPathMostRecentContribution->begin(); iter1 < frontLayerActivationPathMostRecentContribution->end(); )
		{
			bool erase = false;
			GIAactivationPathMostRecentContribution* currentMostRecentContribution = &(*iter1);
			int mostRecentContributionWordIndex = currentMostRecentContribution->mostRecentContributionWordIndex;
			
			if(mostRecentContributionWordIndex < maxMostRecentContributionWordIndex)
			{
				//delete less recent index reference;
				iter1 = frontLayerActivationPathMostRecentContribution->erase(iter1);
				//cout << "erasing" << endl;
				erase = true;
			}
			
			if(!erase)
			{
				iter1++;
				q++;
			}
		}	
	}
}		


bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool measureSequentialActivationOnly, const bool measureActivationRecencyOnly, bool* frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	bool activationSequenceCompleted = false;
	bool repeatDetected = false;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA5
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "\e[35m 3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << " \e[0m" << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ACTIVATION_STACK
	GIAtxtRelTranslatorRulesGroup* activationStackParseTreeGroupBackup = NULL;
	bool restoredActivationsFromStack = false;
	bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivationBackup = *frontLayerActivationPathMostRecentContributionRequireResetGroupActivation;
	#endif
		
	bool componentSequenceActivationReady = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(group, currentComponent, ownerGroup, ownerComponent, activationPathWordCurrentParseTreeGroup, &(ownerGroup->components), forwardPropogationWordData, &activationSequenceCompleted, *frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, &repeatDetected, layer))
	{
		componentSequenceActivationReady = true;
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ACTIVATION_STACK
	else
	{
		if(ownerGroup->activationStackParseTreeGroup.size() > 0)
		{
			//cout << "setCurrentActivationValues" << endl; 
			
			*frontLayerActivationPathMostRecentContributionRequireResetGroupActivation = false;
			
			//CHECKTHIS; always check top of stack for snapshots of neuron in previously activated states (before reset)

			//temporarily store activations;
			getCurrentActivationValues(ownerGroup, &activationStackParseTreeGroupBackup);

			//temporarily restore activations from top of stack;
			setCurrentActivationValues(ownerGroup, ownerGroup->activationStackParseTreeGroup.back());

			if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(group, currentComponent, ownerGroup, ownerComponent, activationPathWordCurrentParseTreeGroup, &(ownerGroup->components), forwardPropogationWordData, &activationSequenceCompleted, *frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, &repeatDetected, layer))
			{
				restoredActivationsFromStack = true;
				componentSequenceActivationReady = true;
			}
		}
	}
	#endif
	
	if(componentSequenceActivationReady)
	{
		//cout << "componentSequenceActivationReady" << endl;
		
		/*
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		//cout << "3 propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady" << endl;
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << endl;	
		GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
		#endif
		*/
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
		if(!(measureActivationRecencyOnly && forwardPropogationWordData->foundMostRecentContribution))	//prevents wasting computational resources continuing to find activation path
		{
		#endif	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS
			if(!measureSequentialActivationOnly)
			{
			#endif
				if(!propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, measureActivationRecencyOnly, repeatDetected, activationPathWordCurrentParseTreeGroup))
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
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ACTIVATION_STACK
	if(measureActivationRecencyOnly)
	{
		if(restoredActivationsFromStack)
		{
			//restore activations
			*frontLayerActivationPathMostRecentContributionRequireResetGroupActivation = frontLayerActivationPathMostRecentContributionRequireResetGroupActivationBackup;
			setCurrentActivationValues(group, activationStackParseTreeGroupBackup);
		}
	}
	#endif
	
	return componentSequenceActivationReady;
}



		
					
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSignalData* forwardPropogationSignalData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool measureActivationRecencyOnly, const bool repeatDetected, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	GIAtxtRelTranslatorRulesGroup* ownerGroupCurrentParseTreeGroup = NULL;
	
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
		
		ownerGroupCurrentParseTreeGroup = ownerGroup->currentParseTreeGroup;
		
		/*
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << endl;	
		GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
		#endif
		*/
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE_TRIAL
		ownerGroupCurrentParseTreeGroup = new GIAtxtRelTranslatorRulesGroup(*(ownerGroup->currentParseTreeGroup));
		#endif
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	//create new parseTreeGroup
	GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//create a new component for every repeated component
	if(measureActivationRecencyOnly)
	{
		newParseComponent->hypotheticalActivation = true;
		newParseComponent->neuronComponentConnectionActive = true;
		newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE_REVERSE_REFERENCES
	newParseComponent->originalComponentRef = currentComponent;
	newParseComponent->ownerGroup = ownerGroupCurrentParseTreeGroup;
	activationPathWordCurrentParseTreeGroup->reverseComponentRefs.push_back(newParseComponent);
	#endif
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	ownerGroupCurrentParseTreeGroup->components.push_back(newParseComponent);
	#endif

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA4
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeNameBackup << ":" << ownerGroup->groupName << ", forwardPropogationWordData->mostRecentContributionWordIndex = " << forwardPropogationWordData->mostRecentContributionWordIndex << endl;	
	GIAtxtRelTranslatorRules.printComponent(currentComponent, layer+1);
	#endif
		
	if(activationSequenceCompleted)
	{
		//cout << "activationSequenceCompleted, repeatDetected = " << repeatDetected << endl;
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
			
		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required regardless of measureActivationRecencyOnly to pass wordNounVariantType/wordVerbVariantType

		GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroupOwner = ownerGroupCurrentParseTreeGroup;
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		if(!measureActivationRecencyOnly)
		{
		#endif
			//record variables for GIAtxtRelTranslatorNeuralNetworkParse;	
			ownerGroup->neuronActive = true;	//temporarily set true
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY
		}
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
		//if(layer > 1)	//ensures that group activation is complete
		//{
			if(!measureActivationRecencyOnly)
			{
				//cout << "DEBUG printBackpropParseTree:" << endl;
				//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

				bool topLevelGroup = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeNameBackup, ownerGroup->groupTypeReferenceSetTypeBackup, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
				if(topLevelGroup)
				{
					forwardPropogationSentenceData->toplevelGroupActivationFound = true;
					topLevelParseTreeGroupLocal = ownerGroup->currentParseTreeGroup;

					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					cout << "topLevelGroup" << endl;
					printBackpropParseTree(topLevelParseTreeGroupLocal, 1);
					cout << "end printBackpropParseTree" << endl;
					#endif

					//cout << "finishedPassingSentenceWords (temp exit)" << endl;
					//exit(0);
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;
				}
			}
		//}
		#endif

		//activation sequence completed, propagate next layer up
		bool passThrough = true;
		bool ownerGroupPropagated = ownerGroup->neuronPropagated;
		if(!propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), measureActivationRecencyOnly, activationPathWordCurrentParseTreeGroupOwner, passThrough))
		{
			result = false;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		if(measureActivationRecencyOnly)
		{
			if(!ownerGroupPropagated)
			{
				ownerGroup->neuronPropagated = false;	//restore to original state after test pass (measureActivationRecencyOnly)	//3g1k
			}
		}
		#endif
	}


	if(measureActivationRecencyOnly)
	{
		//delete temporary newParseComponent from parseTree
		
		/*
		for(int i=0; i<ownerGroupCurrentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* component = ownerGroupCurrentParseTreeGroup->components[i];
		*/

		for(vector<GIAtxtRelTranslatorRulesComponent*>::iterator iter = activationPathWordCurrentParseTreeGroup->reverseComponentRefs.begin(); iter < activationPathWordCurrentParseTreeGroup->reverseComponentRefs.end(); )
		{
			GIAtxtRelTranslatorRulesComponent* component = *iter;
			if(component == newParseComponent)
			{
				iter = activationPathWordCurrentParseTreeGroup->reverseComponentRefs.erase(iter);
			}
			else
			{
				iter++;
			}
		} 

		//redundant:
		for(vector<GIAtxtRelTranslatorRulesComponent*>::iterator iter = ownerGroupCurrentParseTreeGroup->components.begin(); iter < ownerGroupCurrentParseTreeGroup->components.end(); )
		{
			GIAtxtRelTranslatorRulesComponent* component = *iter;
			if(component == newParseComponent)
			{
				iter = ownerGroupCurrentParseTreeGroup->components.erase(iter);
			}
			else
			{
				iter++;
			}
		}
				
		delete newParseComponent;		
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
		//cout << "group->previousWordPOStype = " << group->previousWordPOStype << endl;
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, previousWordPOStype))		
		{
			//cout << "componentTests = true" << endl;
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED);
		#endif
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, forwardPropogationWordData->wordReference, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PREVENT_CIRCULAR_CONNECTION_LOOPS
		resetNeuronForwardProp(group, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED);
		#endif
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

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* activationSequenceCompleted, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, bool* repeatDetected, int layer)
{
	bool componentSequenceActivationReady = false;
	int mostRecentContributionWordIndex = forwardPropogationWordData->mostRecentContributionWordIndex;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyBasic(ownerComponent, components, ownerGroup, forwardPropogationWordData, activationSequenceCompleted, frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, &mostRecentContributionWordIndex, layer))
	{
		bool subComponentsPassed = true;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyOr(currentComponent, &(ownerComponent->subComponents), frontLayerActivationPathMostRecentContributionRequireResetGroupActivation))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady: propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyOr" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyRepeat(currentComponent, &(ownerComponent->subComponents), forwardPropogationWordData, frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
					//cout << "GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady: propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyRepeat" << endl;
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
			//cout << "propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady subComponentsPassed" << endl;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
			bool print = false;
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
			//if(ownerGroup->groupName == "actionReverse2" && ownerGroup->groupTypeNameBackup == "subReferenceSetsAppend")
			if(ownerGroup->groupName == "action" && ownerGroup->groupTypeNameBackup == "subReferenceSetsDelimiterAndObject")
			{
				print = true;
			}
			#else
			print = true;	//TEMP
			#endif
			if(frontLayerActivationPathMostRecentContributionRequireResetGroupActivation || !laterComponentContainsWordReferencesFromEarlierComponents(group, currentComponent, ownerGroup, ownerComponent, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup, layer, print))
			{
			#endif
				componentSequenceActivationReady = true;
				forwardPropogationWordData->mostRecentContributionWordIndex = mostRecentContributionWordIndex;	//CHECKTHIS
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
			}
			else
			{
				cout << "GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS debug: laterComponentContainsWordReferencesFromEarlierComponents" << endl;
			}
			#endif	
		}
		else
		{
			//cout << "propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady !subComponentsPassed" << endl;
		}
	}
	
	return componentSequenceActivationReady;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyBasic(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, bool* activationSequenceCompleted, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, int* mostRecentContributionWordIndex, int layer)
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
					
						int mostRecentContributionWordIndexCurrent = previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_TEST_FULL_PARSE_TREES
						getMostRecentContributionWordIndexInParseTree(ownerGroup->currentParseTreeGroup, &mostRecentContributionWordIndexCurrent, layer);
						#endif
						
						#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_ONLY_TEST_FIRST_ENCOUNTERED_ACTIVE_COMPONENT
						if(!forwardPropogationWordData->foundMostRecentContribution)
						#else
						if(mostRecentContributionWordIndexCurrent > *mostRecentContributionWordIndex)
						#endif
						{
							forwardPropogationWordData->foundMostRecentContribution = true;
							#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
							//cout << "frontLayerActivationPathMostRecentContributionRequireResetGroupActivation = " << frontLayerActivationPathMostRecentContributionRequireResetGroupActivation << endl;
							cout << "previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex = " << previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex << endl;
							#endif
							*mostRecentContributionWordIndex = previousActiveComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
						}

					}
				}
				#endif
			}
			else
			{
				//cout << "!(component == testComponent)" << endl;
				
				/*
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_RESTRICT_GROUP_ACTIVATION_TO_SINGLE_COMPONENT_PER_WORD
				if(component->neuronComponentConnectionActiveWordRecord == forwardPropogationWordData->wordReference)
				{
					stillParsingActiveComponents = false;
					//component already activated via another forward pass pathway
					cout << "component already activated via another forward pass pathway" << endl;
				}
				#endif
				*/
				
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
					//cout << "!sequentialActivationFound" << endl;
					
					if(!frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
					{
						sequentialActivationFound = false;	//found an activated component following the newly activated (current) component - this is illegal; this should never have been activated
					}
					
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
						//do not throw error in case the first tag was optional and inactive, and the second tag was active, but now the first tag is activated
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
			//cout << "*activationSequenceCompleted" << endl;
			
			*activationSequenceCompleted = true;
		}
	}

	return sequentialActivationFound;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_TEST_FULL_PARSE_TREES
bool GIAtxtRelTranslatorNeuralNetworkClass::getMostRecentContributionWordIndexInParseTree(GIAtxtRelTranslatorRulesGroup* parseTreeGroup, int* mostRecentContributionWordIndexCurrent, int level)
{
	for(int i=0; i<parseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentComponent = (parseTreeGroup->components)[i];

		if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex > *mostRecentContributionWordIndexCurrent)
		{
			*mostRecentContributionWordIndexCurrent = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceEntityIndex;
		}
								
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA
			if(print)
			{
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
				cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->groupName = " << currentComponent->groupName << endl;
			}
			#endif

			getMostRecentContributionWordIndexInParseTree(currentComponent->parseTreeGroupRef, mostRecentContributionWordIndexCurrent, level+1);
		}		
	}	
}
#endif
						

bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyOr(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
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
bool GIAtxtRelTranslatorNeuralNetworkClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReadyRepeat(GIAtxtRelTranslatorRulesComponent* testComponent, vector<GIAtxtRelTranslatorRulesComponent*>* components, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, const bool frontLayerActivationPathMostRecentContributionRequireResetGroupActivation, bool* repeatDetected)
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
			if(!frontLayerActivationPathMostRecentContributionRequireResetGroupActivation)
			{
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
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				if(group->neuronActive)
				{
					resetGroupParseTreeGroupRef(group, false);
				}
				else
				{
					resetGroupParseTreeGroupRef(group, true);
				}
			}
			if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF_ENFORCE_MEM_CLEAR)
			{
				resetGroupParseTreeGroupRef(group, true);
			}
			#endif
		}
	}
	
	return result;
}



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ACTIVATION_STACK
bool GIAtxtRelTranslatorNeuralNetworkClass::getCurrentActivationValues(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup** activationStackParseTreeGroup)
{
	bool result = true;
	
	*activationStackParseTreeGroup = (group->currentParseTreeGroup);
			
	return result;
}		

bool GIAtxtRelTranslatorNeuralNetworkClass::setCurrentActivationValues(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesGroup* activationStackParseTreeGroup)
{
	bool result = true;
		
	group->currentParseTreeGroup = activationStackParseTreeGroup;
	
	for(int i1=0; i1<activationStackParseTreeGroup->components.size(); i1++)
	{
		GIAtxtRelTranslatorRulesComponent* currentActivationStackComponent = (activationStackParseTreeGroup->components)[i1];
		GIAtxtRelTranslatorRulesComponent* originalComponent = currentActivationStackComponent->originalComponentRef;

		for(int i2=0; i2<group->components.size(); i2++)
		{
			GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i2];
			if(currentComponent == originalComponent)
			{
				currentComponent->neuronComponentConnectionActive = true;
			}
			if(currentComponent->isSubcomponent)
			{
				if((currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (currentComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
				{
					//NB GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR: parseTreeGroup info has already been generated, so it doesnt matter if set the OR subcomponent active or not (NB it is not recorded whether a previously OR subcomponent was activated)
					for(int i3=0; i3<currentComponent->subComponents.size(); i3++)
					{
						GIAtxtRelTranslatorRulesComponent* currentSubComponent = (currentComponent->subComponents)[i3];
						if(currentSubComponent == originalComponent)
						{
							currentSubComponent->neuronComponentConnectionActive = true;
							currentComponent->neuronComponentConnectionActive = true;	//activate componentOwner
						}
					}
				}
			}
		}
	}
	
	bool stillReadingActiveComponents = true;	
	for(int i1=0; i1<group->components.size(); i1++)
	{	
		GIAtxtRelTranslatorRulesComponent* currentComponent = (group->components)[i1];
	
		if(!(currentComponent->neuronComponentConnectionActive))
		{		
			if(stillReadingActiveComponents)
			{
				if(!(currentComponent->optional))
				{
					stillReadingActiveComponents = false;
				}
			}
		}
	}
	if(stillReadingActiveComponents)
	{
		group->neuronActive = true;	//reactivate neuron (assume all of its necessary components were active)
	}
	
	return result;
}		
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::resetGroupActivation(GIAtxtRelTranslatorRulesGroup* group)
{	
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ACTIVATION_STACK
	GIAtxtRelTranslatorRulesGroup* activationStackParseTreeGroup = NULL;
	if(getCurrentActivationValues(group, &activationStackParseTreeGroup))
	{
		group->activationStackParseTreeGroup.push_back(activationStackParseTreeGroup);
	}
	resetGroupParseTreeGroupRef(group, false);	//always leave existing parseTreeGroupRef in memory (because even if neuronActive==true, it may have an optional component which has not yet been activated
	#else
	if(group->neuronActive)
	{
		resetGroupParseTreeGroupRef(group, false);	//leave existing parseTreeGroupRef in memory
	}
	else
	{
		resetGroupParseTreeGroupRef(group, true);	//clear existing parseTreeGroupRef from memory (as it is either deactivated or incompletely activated)
	}	
	#endif
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE
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



#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS


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







#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SAVE_PARSE_TREE

bool GIAtxtRelTranslatorNeuralNetworkClass::printBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level)
{
	/*
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkClass::printBackpropParseTree{}: " << endl;
	*/
	bool print = true;
	int performanceNOTUSED = 0;
	traceBackpropParseTree(group, level, print, &performanceNOTUSED, NULL);
}

bool GIAtxtRelTranslatorNeuralNetworkClass::measurePerformance(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int level)
{
	bool result = true;
	
	/*
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "GIAtxtRelTranslatorNeuralNetworkClass::printBackpropParseTree{}: " << endl;
	*/
	bool print = false;
	int performance = 0;
	
	traceBackpropParseTree(group, level, print, &performance, forwardPropogationSentenceData->sentenceContents);
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		GIApreprocessorPlainTextWord* currentWord = (forwardPropogationSentenceData->sentenceContents)->at(i);
		if(currentWord->alreadyFoundMatch)
		{
			currentWord->alreadyFoundMatch = false;
			performance++;
		}
	}
	#endif
	
	forwardPropogationSentenceData->performance = performance;
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::traceBackpropParseTree(GIAtxtRelTranslatorRulesGroup* group, int level, bool print, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;

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

		//CHECKTHIS;
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
		if(!print)
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
			else
			{
				*performance = *performance + 1;
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
	
	return result;
}

#endif

/*
bool GIAtxtRelTranslatorNeuralNetworkClass::resetNeuronBackprop(GIAtxtRelTranslatorRulesGroup* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_PRINTED)
	{
		boolPointer = &(group->neuronPrinted);
	}
	//else if(groupBoolIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	//{
	//	boolPointer = &(group->neuronSolidified);
	//}
				
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
*/



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


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
bool GIAtxtRelTranslatorNeuralNetworkClass::laterComponentContainsWordReferencesFromEarlierComponents(GIAtxtRelTranslatorRulesGroup* group, GIAtxtRelTranslatorRulesComponent* currentComponent, GIAtxtRelTranslatorRulesGroup* ownerGroup, GIAtxtRelTranslatorRulesComponent* ownerComponent, GIAtxtRelTranslatorNeuralNetworkForwardPropogationWordData* forwardPropogationWordData, GIAtxtRelTranslatorRulesGroup* activationPathWordCurrentParseTreeGroup, int layer, bool print)
{
	bool result = false;

	//cout << "executing laterComponentContainsWordReferencesFromEarlierComponents" << endl;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
	if(print)
	{
		cout << "laterComponentContainsWordReferencesFromEarlierComponents:" << endl;
		if(activationPathWordCurrentParseTreeGroup == NULL)
		{
			cout << "ERROR: activationPathWordCurrentParseTreeGroup == NULL" << endl;
		}
	}
	#endif
	
	GIAtxtRelTranslatorRulesComponent* newParseTreeComponent = new GIAtxtRelTranslatorRulesComponent(*currentComponent);	//create a new temporary component
	newParseTreeComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	newParseTreeComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		
	for(int i=0; i<ownerGroup->currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponent = (ownerGroup->currentParseTreeGroup->components)[i];
		if(laterComponentContainsWordReferencesFromEarlierComponent(earlierParseTreeComponent, newParseTreeComponent, layer, print))
		{
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "laterComponentContainsWordReferencesFromEarlierComponents1" << endl;
			
			result = true;
		}
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS_REVERSE
	//now if the newly activated component is optional and the group is already fired then ensure that higher level groups in parseTree do not contain previously activated components that have wordRefs within the newly activated component's parseTree:
	if(ownerComponent->optional && ownerGroup->neuronActive)	//REDUNDANT: && !(currentComponent->neuronComponentConnectionActive)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
		if(print)
		{
			cout << "\townerGroup = " << ownerGroup->groupName << ", groupType = " << ownerGroup->groupTypeNameBackup << endl;
			GIAtxtRelTranslatorRules.printComponent(ownerComponent, 1);
		}
		#endif
		
		GIAtxtRelTranslatorRulesComponent artificialComponent;
		artificialComponent.ownerGroup = ownerGroup->currentParseTreeGroup;

		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
		if(print)
		{
			cout << "\tGIAtxtRelTranslatorNeuralNetworkClass::laterComponentContainsWordReferencesFromEarlierComponents: (currentComponent->optional && ownerGroup->neuronActivated && !(currentComponent->neuronComponentConnectionActive))" << endl;
		}
		#endif
		bool firstLevel = true;
		if(laterComponentContainsWordReferencesFromHigherLevelParseTreeComponents(&artificialComponent, newParseTreeComponent, layer, print, firstLevel))
		{
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "ownerGroup = " << ownerGroup->groupName << ", groupType = " << ownerGroup->groupTypeNameBackup << endl;
			GIAtxtRelTranslatorRules.printComponent(ownerComponent, layer);
			
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "laterComponentContainsWordReferencesFromEarlierComponents2" << endl;
			
			result = true;
		}
	}
	#endif
	
	delete newParseTreeComponent;
		
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS_REVERSE
bool GIAtxtRelTranslatorNeuralNetworkClass::laterComponentContainsWordReferencesFromHigherLevelParseTreeComponents(GIAtxtRelTranslatorRulesComponent* higherParseTreeComponent, GIAtxtRelTranslatorRulesComponent* newParseTreeComponent, int level, bool print, bool firstLevel)
{
	bool result = false;
	bool laterComponentContainsWordReferencesFromEarlierComponentCurrentLevel = false;
	bool foundAtLeastOneNullResultAcrossReverseComponents = false;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
	if(print)
	{
		cout << "\tlaterComponentContainsWordReferencesFromHigherLevelParseTreeComponents:" << endl;
	}
	#endif
	
	//cout << "laterComponentContainsWordReferencesFromHigherLevelParseTreeComponents:" << endl;

	if(!firstLevel)
	{
		for(int i=0; i<higherParseTreeComponent->ownerGroup->components.size(); i++)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
			if(print)
			{
				cout << "\thigherParseTreeComponent->ownerGroup = " << higherParseTreeComponent->ownerGroup->groupName << ", groupType = " << higherParseTreeComponent->ownerGroup->groupTypeNameBackup << endl;
				GIAtxtRelTranslatorRules.printComponent(higherParseTreeComponent, 1);
			}
			#endif
	
			GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponent = (higherParseTreeComponent->ownerGroup->components)[i];
			if(earlierParseTreeComponent != higherParseTreeComponent)
			{
				if(laterComponentContainsWordReferencesFromEarlierComponent(earlierParseTreeComponent, newParseTreeComponent, level, print))
				{
					//result = true;
					laterComponentContainsWordReferencesFromEarlierComponentCurrentLevel = true;
					
					if(print)
					{
						GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(level);
						cout << "laterComponentContainsWordReferencesFromEarlierComponentCurrentLevel: higherParseTreeComponent->ownerGroup = " << higherParseTreeComponent->ownerGroup->groupName << ", groupType = " << higherParseTreeComponent->ownerGroup->groupTypeNameBackup << endl;
						GIAtxtRelTranslatorRules.printComponent(higherParseTreeComponent, 1);
					}
				}
			}
			/*
			else
			{
				cout << "earlierParseTreeComponent == higherParseTreeComponent" << endl;
			}
			*/
		}
	}

	for(int i=0; i<higherParseTreeComponent->ownerGroup->reverseComponentRefs.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* higherParseTreeComponentRecurse = (higherParseTreeComponent->ownerGroup->reverseComponentRefs)[i];
		if(laterComponentContainsWordReferencesFromHigherLevelParseTreeComponents(higherParseTreeComponentRecurse, newParseTreeComponent, level-1, print, false))
		{
			//result = true;
		}
		else
		{
			foundAtLeastOneNullResultAcrossReverseComponents = true;
		}
	}
	
	if(!laterComponentContainsWordReferencesFromEarlierComponentCurrentLevel && foundAtLeastOneNullResultAcrossReverseComponents)
	{

	}
	else
	{
		result = true;		
	}
	
	return result;
}
#endif

bool GIAtxtRelTranslatorNeuralNetworkClass::laterComponentContainsWordReferencesFromEarlierComponent(GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponent, GIAtxtRelTranslatorRulesComponent* newParseTreeComponent, int level, bool print)
{
	bool result = false;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
	if(print)
	{
		cout << "\tlaterComponentContainsWordReferencesFromEarlierComponent:" << endl;
		cout << "\t\tnewParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName = " << newParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName << endl;
		cout << "\t\tearlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName = " << earlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName << endl;
	}
	#endif
	
	if(newParseTreeComponent->neuronComponentConnectionActiveWordRecord == earlierParseTreeComponent->neuronComponentConnectionActiveWordRecord)
	{
		if(!(newParseTreeComponent->hypotheticalActivation) && !(earlierParseTreeComponent->hypotheticalActivation))
		//if(!(earlierParseTreeComponent->hypotheticalActivation))
		{
			result = true;

			/*
			if(print)
			{
				cout << "\tlaterComponentContainsWordReferencesFromEarlierComponent (earlierParseTreeComponent, newParseTreeComponent):" << endl;
				GIAtxtRelTranslatorRules.printComponent(earlierParseTreeComponent, 1);
				GIAtxtRelTranslatorRules.printComponent(newParseTreeComponent, 1);
				cout << "\t\tnewParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName = " << newParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName << endl;
				cout << "\t\tearlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName = " << earlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName << endl;		
			}
			*/		
		}
	}
	
	if(laterComponentRecursedHasWordReferenceFromEarlierComponent(earlierParseTreeComponent, newParseTreeComponent, level, print))
	{
		result = true;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::laterComponentRecursedHasWordReferenceFromEarlierComponent(GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponent, GIAtxtRelTranslatorRulesComponent* newParseTreeComponentRecursed, int level, bool print)
{
	bool result = false;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
	if(print)
	{
		cout << "\t\tlaterComponentRecursedHasWordReferenceFromEarlierComponent:" << endl;
		cout << "\t\t\tnewParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName = " << newParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName << endl;
		cout << "\t\t\tearlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName = " << earlierParseTreeComponent->neuronComponentConnectionActiveWordRecord->tagName << endl;
	}
	#endif
	
	if(laterComponentRecursedHasWordReferenceFromEqualierComponentRecursed(earlierParseTreeComponent, newParseTreeComponentRecursed, level, print))
	{
		result = true;
	}

	if(newParseTreeComponentRecursed->parseTreeGroupRef != NULL)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
		if(print)
		{	
			cout << "\t\t\t (newParseTreeComponentRecursed->parseTreeGroupRef != NULL)" << endl;	
		}
		#endif
		for(int i=0; i<newParseTreeComponentRecursed->parseTreeGroupRef->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* newParseTreeComponentRecursed2 = (newParseTreeComponentRecursed->parseTreeGroupRef->components)[i];

			if(laterComponentRecursedHasWordReferenceFromEarlierComponent(earlierParseTreeComponent, newParseTreeComponentRecursed2, level+1, print))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkClass::laterComponentRecursedHasWordReferenceFromEqualierComponentRecursed(GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponentRecursed, GIAtxtRelTranslatorRulesComponent* newParseTreeComponentRecursed, int level, bool print)
{
	bool result = false;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENSURE_LATER_COMPONENTS_DO_NOT_CONTAIN_WORD_REFERENCES_FROM_EARLIER_COMPONENTS
	if(print)
	{
		cout << "\t\t\tlaterComponentRecursedHasWordReferenceFromEqualierComponentRecursed:" << endl;
		cout << "\t\t\t\tnewParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName = " << newParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName << endl;
		cout << "\t\t\t\tearlierParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName = " << earlierParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName << endl;
	}
	#endif
	
	if(newParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord == earlierParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord)
	{
		if(!(newParseTreeComponentRecursed->hypotheticalActivation) && !(earlierParseTreeComponentRecursed->hypotheticalActivation))
		//if(!(earlierParseTreeComponent->hypotheticalActivation))
		{
			result = true;
		}
		
		/*
		if(print)
		{
			cout << "\tlaterComponentRecursedHasWordReferenceFromEqualierComponentRecursed (earlierParseTreeComponentRecursed, newParseTreeComponentRecursed):" << endl;
			GIAtxtRelTranslatorRules.printComponent(earlierParseTreeComponentRecursed, 1);
			GIAtxtRelTranslatorRules.printComponent(newParseTreeComponentRecursed, 1);
			cout << "\tnewParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName = " << newParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName << endl;
			cout << "\tearlierParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName = " << earlierParseTreeComponentRecursed->neuronComponentConnectionActiveWordRecord->tagName << endl;	    
		}
		*/
	}

	if(earlierParseTreeComponentRecursed->parseTreeGroupRef != NULL)
	{	
		for(int i=0; i<earlierParseTreeComponentRecursed->parseTreeGroupRef->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* earlierParseTreeComponentRecursed2 = (earlierParseTreeComponentRecursed->parseTreeGroupRef->components)[i];

			if(laterComponentRecursedHasWordReferenceFromEqualierComponentRecursed(earlierParseTreeComponentRecursed2, newParseTreeComponentRecursed, level+1, print))
			{
				result = true;
			}
		}
	}
	
	return result;
}


#endif



#endif
