 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorReferencing.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by X Parser
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_REFERENCING
#define HEADER_GIA_TRANSLATOR_REFERENCING

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAquery.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OLD
#include "LRPpreprocessorWordIdentification.hpp"
#endif
#include "SHAREDvars.hpp"


class GIAtranslatorReferencingClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAqueryClass GIAquery;
	private: GIAdatabaseClass GIAdatabase;
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OLD
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#endif
	
	public: void identifyComparisonVariableAlternateMethod(const GIAtranslatorVariablesClass* translatorVariables);				//Stanford Compatible / Relex Specific [$qvars not supported. What is the 'attr' stanford dependency relation?]
	//void identifyEntityTypesLocal(GIArelation* currentRelationInList, int NLPdependencyRelationsType, GIAentityNode* governer, GIAentityNode* dependent);

	#ifdef GIA_TRANSLATOR_WITHOUT_SYN_REL_TRANSLATOR_FEATURES
	public: bool entityPosInferredIsName(GIAentityNode* currentGIAEntityNode);
	public: bool entityPosInferredIsPronoun(GIAentityNode* currentGIAEntityNode);
	public: bool entityPosInferredIsPronounPossessive(GIAentityNode* currentGIAEntityNode);
	public: void linkPronounReferencesTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables);
	#endif
	public: void linkPronounReferencesRelex(GIAtranslatorVariablesClass* translatorVariables);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
	#ifdef GIA_STANFORD_CORENLP
	public: void linkPronounAndTextualContextReferencesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables, const GIAstanfordCoreNLPcoreference* firstCoreferenceInList);	//Stanford Compatible
	#endif

	public: void fillExplicitReferenceSameSetTags(GIAsentence* currentSentenceInList);

	public: int identifyReferenceSets(unordered_map<string,GIAentityNode*>* sentenceNetworkIndexEntityNodesList, const bool NLPdependencyRelationsType, vector<GIAentityNode*>* referenceSetDefiniteEntityList);
		private: void resetReferenceSets(unordered_map<string,GIAentityNode*>* sentenceNetworkIndexEntityNodesList);
		private: void identifyReferenceSetNetworkIndexEntityEntrance(GIAentityNode* entityNode, int* referenceSetID, const bool haveSentenceEntityIndexOfDeterminers, vector<GIAentityNode*>* referenceSetDefiniteEntityList);

	#ifdef GIA_ADVANCED_REFERENCING
	public: void createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(unordered_map<string,GIAentityNode*>* sentenceNetworkIndexEntityNodesList, unordered_map<string,GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAcoreference* firstGIAcoreferenceInList, vector<GIAentityNode*>* referenceSetDefiniteEntityList, const int sentenceIndex);
		private: void createGIAcoreferenceInListBasedUponIdentifiedReferenceSet(unordered_map<string,GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, unordered_map<string,GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAreferenceTraceParameters* referenceTraceParameters, int* maxNumberOfMatchedNodes, constEffective GIAentityNode** queryEntityWithMaxNumberNodesMatched, GIAentityNode** networkEntityWithMaxNumberNodesMatched, bool* foundAtLeastOneMatch);
		private: GIAcoreference* generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAentityNode* entityNode, GIAcoreference* currentGIAcoreferenceInList, bool intrasentenceReference);
	public: void linkAdvancedReferencesGIA(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstCoreferenceInList);
	#endif

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	public: void identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(GIAtranslatorVariablesClass* translatorVariables);
	#endif
	private: bool identifyReferenceSetDetermineNextCourseOfAction(GIAentityNode* entityNode, const bool sameReferenceSet, int referenceSetID, int minimumEntityIndexOfReferenceSet, const GIAentityConnection* connection);
		private: void identifyReferenceSet(GIAentityNode* entityNode, int referenceSetID, int minimumEntityIndexOfReferenceSet);
};


#endif
