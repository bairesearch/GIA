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
 * File Name: GIAtranslatorDefineReferencing.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2f3a 02-July-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING
#define HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAquery.h"
#include "GIAtranslatorOperations.h"

void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser);				//Stanford Compatible / Relex Specific [$qvars not supported. What is the 'attr' stanford dependency relation?]
void identifyEntityTypes(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType);									//Stanford Compatible
	//void identifyEntityTypesLocal(Relation * currentRelationInList, int NLPdependencyRelationsType, GIAentityNode * governer, GIAentityNode * dependent);

void linkPronounReferencesRelex(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_USE_STANFORD_CORENLP
void linkPronounAndTextualContextReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, StanfordCoreNLPcoreference * firstCoreferenceInList, Feature * featureArrayTemp[]);	//Stanford Compatible
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
void fillExplicitReferenceSameSetTags(Sentence * currentSentenceInList);

int identifyReferenceSets(unordered_map<string, GIAentityNode*> *sentenceConceptEntityNodesList, bool NLPdependencyRelationsType);
	void resetReferenceSets(unordered_map<string, GIAentityNode*> *sentenceConceptEntityNodesList);
	void identifyReferenceSetConceptEntityEntrance(GIAentityNode * entityNode, int * referenceSetID, bool haveSentenceEntityIndexOfDeterminers);

void createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(unordered_map<string, GIAentityNode*> *sentenceConceptEntityNodesList, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, GIACoreference * firstGIACoreferenceInList, int numberReferenceSets);	//bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]
	void createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(unordered_map<string, GIAentityNode*> *entityNodesActiveListConceptsQuery, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, GIAreferenceTraceParameters *referenceTraceParameters, int *maxNumberOfMatchedNodes, GIAentityNode **queryEntityWithMaxNumberNodesMatched, GIAentityNode **networkEntityWithMaxNumberNodesMatched, bool *foundAtLeastOneMatch);
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	GIACoreference * generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAentityNode * entityNode, GIACoreference * currentGIAcoreferenceInList, bool intrasentenceReference);
	#else
	GIACoreference * generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAentityNode * entityNode, GIACoreference * currentGIAcoreferenceInList);
	#endif
void linkAdvancedReferencesGIA(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, GIACoreference * firstCoreferenceInList, Feature * featureArrayTemp[], GIAentityNode * GIAfeatureTempEntityNodeArray[], GIAentityNode * GIAconceptNodeArray[]);

#endif

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void identifyReferenceSetsSpecificConceptsAndLinkWithSubstanceConcepts(vector<GIAentityNode*> * entityNodesActiveListComplete);
#endif
#ifdef GIA_USE_ADVANCED_REFERENCING_SEARCH_CODE
bool identifyReferenceSetDetermineNextCourseOfAction(GIAentityNode * entityNode, bool sameReferenceSet, int referenceSetID, int minimumEntityIndexOfReferenceSet, bool isProperty);
	void identifyReferenceSet(GIAentityNode * entityNode, int referenceSetID, int minimumEntityIndexOfReferenceSet);
#endif

#endif
