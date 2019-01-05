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
 * File Name: GIAsemRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1g 24-April-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator Parser
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SEM_REL_TRANSLATOR_PARSER
#define HEADER_GIA_SEM_REL_TRANSLATOR_PARSER

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAsemRelTranslatorOperations.hpp"
#include "GIAtranslatorGrammar.hpp"
#include "GIAsynRelTranslatorRedistributeRelations.hpp"
#include "GIAtranslatorReferencing.hpp"
#include "GIAsynRelTranslatorLinkEntitiesDynamic.hpp"
#include "GIAsynRelTranslatorDefineSubstances.hpp"
#include "GIAsynRelTranslatorApplyAdvancedFeatures.hpp"
#include "GIAnlp.hpp"
#include "GIAsemRelTranslatorDatabase.hpp"
#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
#include "GIAbot.hpp"
#endif
#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
#include "GIApreprocessorWordIdentification.hpp"
#endif



class GIAsemRelTranslatorParserClass
{
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsynRelTranslatorRedistributeRelationsClass GIAsynRelTranslatorRedistributeRelations;
	private: GIAtranslatorReferencingClass GIAtranslatorReferencing;
	private: GIAsynRelTranslatorLinkEntitiesDynamicClass GIAsynRelTranslatorLinkEntitiesDynamic;
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	private: GIAbotClass GIAbot;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	#endif
	private: GIAsynRelTranslatorDefineSubstancesClass GIAsynRelTranslatorDefineSubstances;
	private: GIAsynRelTranslatorApplyAdvancedFeaturesClass GIAsynRelTranslatorApplyAdvancedFeatures;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	private: GIAsemRelTranslatorDatabaseClass GIAsemRelTranslatorDatabase;
	private: GIAnlpClass GIAnlp;
	#ifdef GIA_SEM_REL_TRANSLATOR
	public: bool convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: void locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelationsMultiwordAuxiliary(GIAtranslatorVariablesClass* translatorVariables, int modalAuxiliaryIndex, GIAentityNode* entity);
		#endif
		private: void defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
		private: void identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		private: void defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: bool findMatchingObject(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationName, const int entity2Index, GIArelation** currentRelationInList2, int* entity2Index2);
			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
			private: void invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
			private: GIAentityNode* createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
			#endif
		private: void applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQuantitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
	public: bool generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
	#endif
	#endif
	private: int getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables);

	public: void defineQualitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
	public: bool convertSentenceSemanticRelationsIntoGIAnetworkNodesEndPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);

};



#endif
