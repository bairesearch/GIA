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
 * File Name: GIAsemanticParserTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e10a 15-January-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR
#define HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAtranslatorDefineGrammar.hpp"
#include "GIAtranslatorRedistributeRelations.hpp"
#include "GIAtranslatorDefineReferencing.hpp"
#include "GIAtranslatorLinkEntitiesDynamic.hpp"
#include "GIAtranslatorDefineSubstances.hpp"
#include "GIAtranslatorApplyAdvancedFeatures.hpp"
#include "GIAnlp.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAbot.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"

#ifdef GIA_SEMANTIC_PARSER

class GIAsemanticParserTranslatorClass
{
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorRedistributeRelationsClass GIAtranslatorRedistributeRelations;
	private: GIAtranslatorDefineReferencingClass GIAtranslatorDefineReferencing;
	private: GIAtranslatorLinkEntitiesDynamicClass GIAtranslatorLinkEntitiesDynamic;
	private: GIAbotClass GIAbot;
	private: GIAtranslatorDefineSubstancesClass GIAtranslatorDefineSubstances;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIAtranslatorApplyAdvancedFeaturesClass GIAtranslatorApplyAdvancedFeatures;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	private: GIAnlpClass GIAnlp;
	public: void convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: void locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string modalAuxiliaryString);
		private: void defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
		private: void identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		private: void defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
			private: void invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
			private: GIAentityNode* createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
			#endif
		private: GIAentityNode* createNewRelationshipEntitySemantic(const int relationshipEntityType, const string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
		private: void applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQuantitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
			private: void defineQualitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables);
	#ifdef GIA_SEMANTIC_PARSER_SUBSETS
	public: bool generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
	#endif
};

#endif

#endif
