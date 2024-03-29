 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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
#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
#include "LRPpreprocessorWordIdentification.hpp"
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
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
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
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelationsMultiwordAuxiliary(GIAtranslatorVariablesClass* translatorVariables, const int modalAuxiliaryIndex, const GIAentityNode* entity);
		#endif
		private: void defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
		private: void identifyComparisonVariableBasedOnSemanticRelations(const GIAtranslatorVariablesClass* translatorVariables);
		#endif
		private: void defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: bool findMatchingObject(constEffective GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationName, const int entity2Index, GIArelation** currentRelationInList2, int* entity2Index2);
			#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
			private: void invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
			private: GIAentityNode* createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
			#endif
		private: void applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQuantitiesBasedOnSemanticRelations(const GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
	public: bool generateAllPermutationsFromSemanticRelationsFile(GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif

	public: void defineQualitiesBasedOnSemanticRelations(const GIAtranslatorVariablesClass* translatorVariables);
	public: bool convertSentenceSemanticRelationsIntoGIAnetworkNodesEndPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);

};



#endif
