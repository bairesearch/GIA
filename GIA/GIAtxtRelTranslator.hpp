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
 * File Name: GIAtxtRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1v 22-February-2018
 * Requirements: requires plain text file
 * Description: Textual relation translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR
#define HEADER_GIA_TXT_REL_TRANSLATOR

#include "GIAglobalDefs.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3

//#include "SHAREDvars.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
#include "GIAtxtRelTranslatorHybrid.hpp"
#endif
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "GIApreprocessorPOStaggerDatabase.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"
#include "GIApreprocessorMultiwordReductionClass.hpp"
#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
#include "GIAnlp.hpp"
#endif
#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"

class GIAtxtRelTranslatorClass
{
	//private: SHAREDvarsClass SHAREDvars;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	private: GIAtxtRelTranslatorHybridClass GIAtxtRelTranslatorHybrid;
	#endif
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	private: GIAnlpClass GIAnlp;
	#endif
	private: GIAtranslatorClass GIAtranslator;
	private: SHAREDvarsClass SHAREDvars;

	
	public: bool parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName);
		public: bool executeTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables);
			public: bool generateParseTreeIntro(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup, int* performance);
				private: bool generateParseTreeGroupType(GIAtxtRelTranslatorRulesGroupType* groupType, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, GIAtxtRelTranslatorRulesComponent* previousParseTreeComponent);
					private: bool generateParseTreeGroup(GIAtxtRelTranslatorRulesGroup* group, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
						private: bool generateRulesGroupTreeComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, bool subcomponents, int subcomponentsType, bool subcomponentsOptional, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
							private: bool generateRulesGroupTreeComponent(GIAtxtRelTranslatorRulesComponent* component, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
				private: bool findStringMatch(GIAtxtRelTranslatorRulesComponent* component, GIApreprocessorWord* currentWord);
				private: bool verifyPOStype(GIApreprocessorWord* currentWord, unsigned char wordPOStype);
				private: bool updatePerformance(const int performanceTemp, int* performanceMax, int* performance, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, GIAtxtRelTranslatorRulesComponent* previousParseTreeComponent);
				private: int calculateMinIndexOfMatchesFound(vector<GIApreprocessorWord*>* sentenceContentsSubset);
				private: void clearAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup);
				private: void restoreAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew);
		private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables);
		private: int getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables);
	
	private: bool printParseTreeDebug(GIAtranslatorVariablesClass* translatorVariables);
	private: bool printParseTreeDebug(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int layer);
	private: bool printParseTreeDebugIndentation(int layer);

};
#endif




#endif
