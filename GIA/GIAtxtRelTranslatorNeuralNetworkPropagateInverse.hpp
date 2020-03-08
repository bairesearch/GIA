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
 * File Name: GIAtxtRelTranslatorNeuralNetworkPropagateInverse.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j6c 10-January-2020
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Neural Network Inverse
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_INVERSE_PROPAGATE_NEURAL_NETWORK
#define HEADER_GIA_TXT_REL_TRANSLATOR_INVERSE_PROPAGATE_NEURAL_NETWORK

#include "GIAglobalDefs.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
#ifdef GIA_TXT_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK

#include "GIAtranslatorOperations.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkPropagateOperations.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "SHAREDvars.hpp"

class GIAtxtRelTranslatorNeuralNetworkPropagateInverseClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorNeuralNetworkPropagateOperationsClass GIAtxtRelTranslatorNeuralNetworkPropagateOperations;	//required for printComponent/printParseTreeDebugIndentation
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: SHAREDvarsClass SHAREDvars;

	public: bool generateParseTreeIntro(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets);
		private: bool generateParseTreeGroupType(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupType* groupType, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, GIAtxtRelTranslatorRulesComponentParseTree* previousParseTreeComponent);
			private: bool generateParseTreeGroup(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
				private: bool generateRulesGroupTreeComponents(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, bool subcomponents, int subcomponentsType, bool subcomponentsOptional, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
					private: bool generateRulesGroupTreeComponent(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		private: bool forwardNounVerbVariantRequirementsComponentToGroup(GIAtxtRelTranslatorRulesComponentParseTree* component, GIAtxtRelTranslatorRulesGroupParseTree* newParseGroup);
		private: bool forwardNounVerbVariantRequirementsGroupToComponent(GIAtxtRelTranslatorRulesGroupParseTree* currentParseGroup, GIAtxtRelTranslatorRulesComponentParseTree* currentComponent);
		#endif
		private: bool findStringMatch(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIApreprocessorPlainTextWord* currentWord, GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent);
		private: bool verifyPOStype(GIApreprocessorPlainTextWord* currentWord, unsigned char wordPOStype);
		public: bool updatePerformance(const int performanceTemp, int* performance, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, GIAtxtRelTranslatorRulesComponentParseTree* previousParseTreeComponent);
			private: bool deleteAllSubgroupsRecurse(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int layer);
			private: bool deleteParseComponent(GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent);
		public: void clearAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup);
		public: void restoreAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew);
	private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables);
	private: int getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables);

	private: bool printParseTreeDebugIndentation(int layer);

};
#endif
#endif



#endif
