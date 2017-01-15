/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserTranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2n1g 12-September-2016
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR
#define HEADER_GIA_SEMANTIC_PARSER_TRANSLATOR

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, int NLPfeatureParser, bool linkPreestablishedReferencesGIA,  GIACoreference* firstGIACoreferenceInList);
	void locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAnetworkIndexNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, int NLPfeatureParser);
	void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
		#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
		void updateGrammaticalValuesBasedOnCompositionAuxiliary(GIAentityNode* entity, string compositionAuxiliaryString);
		#endif
		void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, string modalAuxiliaryString);
	void defineSubstancesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA2_SUPPORT_QUERIES
	void identifyComparisonVariableBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	void defineConnectionsBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[]);
		#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
		void invertOrDuplicateConditionsIfRequiredSemantic(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[], GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet);
		GIAentityNode* createNewInverseConditionEntitySemantic(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], string inverseConditionName, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[]);
		#endif
	void applyAdvancedFeaturesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
		void defineQuantitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
		void defineQualitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
#ifdef GIA2_SEMANTIC_PARSER
bool generateAllPermutationsFromSemanticRelationsFile(GIAfeature* firstFeatureInList, int NLPfeatureParser);
#endif

#endif

#endif
