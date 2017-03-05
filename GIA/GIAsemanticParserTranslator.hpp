/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4c 17-January-2017
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
#include "GIAlrp.hpp"

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

class GIAsemanticParserTranslatorClass
{
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorRedistributeRelationsClass GIAtranslatorRedistributeRelations;
	private: GIAtranslatorDefineReferencingClass GIAtranslatorDefineReferencing;
	private: GIAtranslatorLinkEntitiesDynamicClass GIAtranslatorLinkEntitiesDynamic;
	private: GIAbotClass GIAbot;
	private: GIAtranslatorDefineSubstancesClass GIAtranslatorDefineSubstances;
	private: GIAlrpClass GIAlrp;
	private: GIAtranslatorApplyAdvancedFeaturesClass GIAtranslatorApplyAdvancedFeatures;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	private: GIAnlpClass GIAnlp;
	public: void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, const GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, const int NLPfeatureParser, const bool linkPreestablishedReferencesGIA,  GIAcoreference* firstGIAcoreferenceInList);
		private: void locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAnetworkIndexNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, const int NLPfeatureParser);
		private: void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);
		#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
			private: void updateGrammaticalValuesBasedOnCompositionAuxiliary(GIAentityNode* entity, const string compositionAuxiliaryString);
		#endif
			private: void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string modalAuxiliaryString);
		private: void defineSubstancesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA2_SUPPORT_QUERIES
		private: void identifyComparisonVariableBasedOnSemanticRelations(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
		private: void defineConnectionsBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[]);
		#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
			private: void invertOrDuplicateConditionsIfRequiredSemantic(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[], GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
			private: GIAentityNode* createNewInverseConditionEntitySemantic(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], string inverseConditionName, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[]);
		#endif
		private: void applyAdvancedFeaturesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser);
			private: void defineQuantitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser);
			private: void defineQualitiesBasedOnSemanticRelations(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser);
#ifdef GIA2_SEMANTIC_PARSER
	public: bool generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
#endif
};

#endif

#endif
