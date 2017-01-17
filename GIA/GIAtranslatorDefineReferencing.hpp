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
 * File Name: GIAtranslatorDefineReferencing.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4a 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING
#define HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAquery.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"

class GIAtranslatorDefineReferencingClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAqueryClass GIAquery;
	private: GIAdatabaseClass GIAdatabase;
	public: void identifyComparisonVariableAlternateMethod(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser);				//Stanford Compatible / Relex Specific [$qvars not supported. What is the 'attr' stanford dependency relation?]
	public: void identifyEntityTypes(const GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], const int NLPdependencyRelationsType);									//Stanford Compatible
	//void identifyEntityTypesLocal(GIArelation* currentRelationInList, int NLPdependencyRelationsType, GIAentityNode* governer, GIAentityNode* dependent);

	public: void linkPronounReferencesRelex(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAfeatureTempEntityNodeArray[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAfeature* featureArrayTemp[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_STANFORD_CORENLP
	public: void linkPronounAndTextualContextReferencesStanfordCoreNLP(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAfeatureTempEntityNodeArray[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const GIAstanfordCoreNLPcoreference* firstCoreferenceInList, GIAfeature* featureArrayTemp[]);	//Stanford Compatible
#endif

#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	public: void fillExplicitReferenceSameSetTags(GIAsentence* currentSentenceInList);

	public: int identifyReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList, const bool NLPdependencyRelationsType, vector<GIAentityNode*>* referenceSetDefiniteEntityList);
		private: void resetReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList);
		private: void identifyReferenceSetNetworkIndexEntityEntrance(GIAentityNode* entityNode, int* referenceSetID, const bool haveSentenceEntityIndexOfDeterminers, vector<GIAentityNode*>* referenceSetDefiniteEntityList);
#endif

#ifdef GIA_ADVANCED_REFERENCING
	public: void createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAcoreference* firstGIAcoreferenceInList, vector<GIAentityNode*>* referenceSetDefiniteEntityList, const GIAsentence* currentSentenceInList);
		private: void createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const GIAreferenceTraceParameters* referenceTraceParameters, int* maxNumberOfMatchedNodes, GIAentityNode* *queryEntityWithMaxNumberNodesMatched, GIAentityNode* *networkEntityWithMaxNumberNodesMatched, bool* foundAtLeastOneMatch);
		private: GIAcoreference* generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAentityNode* entityNode, GIAcoreference* currentGIAcoreferenceInList, bool intrasentenceReference);
	public: void linkAdvancedReferencesGIA(const GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAcoreference* firstCoreferenceInList, GIAfeature* featureArrayTemp[], GIAentityNode* GIAfeatureTempEntityNodeArray[], GIAentityNode* GIAnetworkIndexNodeArray[]);
#endif

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	public: void identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(vector<GIAentityNode*>* entityNodesActiveListComplete);
#endif
#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	private: bool identifyReferenceSetDetermineNextCourseOfAction(GIAentityNode* entityNode, const bool sameReferenceSet, int referenceSetID, int minimumEntityIndexOfReferenceSet, const bool isProperty);
		private: void identifyReferenceSet(GIAentityNode* entityNode, int referenceSetID, int minimumEntityIndexOfReferenceSet);
#endif
};


#endif
