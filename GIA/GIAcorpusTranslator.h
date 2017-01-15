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
 * File Name: GIAcorpusTranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h17b 27-January-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CORPUS_TRANSLATOR
#define HEADER_GIA_CORPUS_TRANSLATOR

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"

#ifdef GIA_USE_CORPUS_DATABASE

#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceConceptEntityNodesList, int NLPfeatureParser, bool linkPreestablishedReferencesGIA,  GIACoreference* firstGIACoreferenceInList);
#else
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceConceptEntityNodesList, int NLPfeatureParser);
#endif
	void locateAndAddAllConceptEntitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAconceptNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* sentenceConceptEntityNodesList, int NLPfeatureParser);
	void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
		#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
		void updateGrammaticalValuesBasedOnCompositionAuxiliary(GIAentityNode* entity, string compositionAuxiliaryString);
		#endif
		void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, string modalAuxiliaryString);
	void defineSubstancesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA2_SUPPORT_QUERIES
	void identifyComparisonVariableBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	void defineConnectionsBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	void applyAdvancedFeaturesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
		void defineQuantitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
		void defineQualitiesBasedOnSemanticRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);
#ifdef GIA2_CONNECTIONIST_NETWORK
bool generateAllPermutationsFromSemanticRelationsFile(string corpusFileName, int NLPfeatureParser);
#endif

#endif

#endif
