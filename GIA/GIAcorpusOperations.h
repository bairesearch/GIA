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
 * File Name: GIAcorpusOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h7b 19-January-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CORPUS_OPERATIONS
#define HEADER_GIA_CORPUS_OPERATIONS

#include "GIAglobalDefs.h"
#include "GIAtranslatorDefs.h"

#ifdef GIA_USE_CORPUS_DATABASE

void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNode ** GIAentityNodeArray, Sentence * currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNode ** GIAentityNodeArray, Sentence * currentSentenceInList, bool linkPreestablishedReferencesGIA, int NLPdependencyRelationsType);
	string generateGIA2semanticDependencyRelation(GIAentityNode ** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
		string generateGIA2semanticDependencyRelationSimple(string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);
			string createSameReferenceSetRecord(bool sameReferenceSet);
string regenerateSentenceText(Feature * firstFeatureInSentence, bool addPOSinfo, int NLPfeatureParser);
void determineGIAconnectionistNetworkPOStypeNames(Feature * firstFeatureInList, int NLPfeatureParser);
	void determineGIAconnectionistNetworkPOStypeNameStanford(Feature * currentFeatureInSentence);
	void determineGIAconnectionistNetworkPOStypeNameRelex(Feature * currentFeatureInSentence);
#endif

#endif
