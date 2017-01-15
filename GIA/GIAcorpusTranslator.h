/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2b6a 09-January-2014
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
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList);
#else
void convertSentenceSemanticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser);
#endif
	void locateAndAddAllConceptEntitiesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAconceptNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser);
	void fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
		void updateGrammaticalValuesBasedOnCompositionAuxiliary(GIAentityNode * entity, string compositionAuxiliaryString);
		void updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode * entity, string modalAuxiliaryString);	
	void defineSubstancesBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	void defineConnectionsBasedOnSemanticRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);

#endif

#endif
