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
 * File Name: GIAsemanticParserOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k8a 06-September-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_OPERATIONS
#define HEADER_GIA_SEMANTIC_PARSER_OPERATIONS

#include "GIAglobalDefs.h"
#include "GIAtranslatorDefs.h"

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, bool rcmodIndicatesSameReferenceSet);
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, bool linkPreestablishedReferencesGIA, int NLPdependencyRelationsType);
	void generateGIA2semanticDependencyRelation(GIAentityNode** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, bool rcmodIndicatesSameReferenceSet, string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation);
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
		string generateGIA2semanticDependencyRelationText(string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet, bool rcmodIndicatesSameReferenceSet);
			string createSameReferenceSetRecord(bool sameReferenceSet);
			#ifdef GIA_RECORD_RCMOD_SET_INFORMATION
			string createRcmodIndicatesSameReferenceSetRecord(bool rcmodIndicatesSameReferenceSet);
			#endif
string generateCorpusFileHeaderText(GIAfeature* firstFeatureInSentence, bool addPOSinfo);
#endif
void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, int NLPfeatureParser);
	void determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence);
	void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence);
#ifdef GIA2_SEMANTIC_PARSER
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
GIAfeature* generateOptimisedFeatureSubsetBasedOnContextualConjunctions(GIAfeature* firstFeatureInSentenceSubset, int centralWord, bool* optimisedBasedOnContextualConjunctions);
#endif
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
int calculateFirstWordInTupleIndexRelative(int firstWordInTupleIndex, int firstWordInSentenceSubsetIndex);
#endif
#endif

#endif

#endif
