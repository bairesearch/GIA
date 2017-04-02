/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAquery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a5e 28-March-2017
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_QUERY
#define HEADER_GIA_QUERY

#include "GIAglobalDefs.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAdatabase.hpp"
	#include "GIAwordnet.hpp"
	#include "wn.h"
#include "GIAtranslatorOperations.hpp"	//required for getPrimaryNetworkIndexNodeDefiningInstance()

#define GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS		//added 13 July 2012
#define GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS					//default: enabled	//added 21 May 2012
#define GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES			//default: enabled
#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	#define GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES	//default: enabled
	//#define GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES			//default: disabled
#endif
#define GIA_QUERY_TRACE_PAST_ANSWER						//default: enabled [untested]

//#define GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS	//default: disabled
#ifdef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
	#define GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE (true)		//added with GIA_1N1ATEMP1TO8_CHANGES
	//#define GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
#else
	#ifndef GIA_ALIASES
		#define GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES	//removed 15 August 2012
	#endif
	#define GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE (false)		//added with GIA_1N1ATEMP1TO8_CHANGES
#endif
#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES
	#define GIA_QUERY_RELAX_CONFIDENCE_REQUIREMENTS_FOR_YES
#endif


#define GIA_QUERY_DEBUG_LEVEL
#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
	#define GIA_QUERY_DEBUG_LEVEL
#endif
#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	#define GIA_QUERY_DEBUG_LEVEL
#endif
#ifdef GIA_ADVANCED_REFERENCING_DEBUG_TOO_LARGE_REFERENCE_SET
	#define GIA_QUERY_DEBUG_LEVEL
#endif


//#define GIA_QUERY_USE_ARTIFICIALLY_ADJUSTED_MAX_CONFIDENCE

#define GIA_QUERY_USE_LONG_CONTEXT_TRACE
#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
	//#define GIA_QUERY_USE_EXTRA_LONG_CONTEXT_TRACE
	//#define GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
#endif
#define GIA_QUERY_FRACTION_MAX_CONFIDENCE_REQUIRED_FOR_UNPRECISE_ANSWER (0.5)

#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT (1)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES (2)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON (3)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY (4)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP (5)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION (6)

#define EXACT_MATCH_FAIL (0)
#define EXACT_MATCH_PASS (1)
#define EXACT_MATCH_OUT_OF_BOUNDS (2)
#define MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS (3)

#define TRACE_MODE_ONLY_RECORD_EXACT_MATCH (1)
#define TRACE_MODE_ONLY_RECORD_BEST_MATCH (2)
#define TRACE_MODE_ASSERT_SAME_REFERENCE_SET_ID_TRUE (true)
#define TRACE_MODE_ASSERT_SAME_REFERENCE_SET_ID_FALSE (false)

#define TRACE_MODE_IS_QUERY_TRUE (true)
#define TRACE_MODE_IS_QUERY_FALSE (false)

class GIAqueryTraceParameters
{
public:

	GIAqueryTraceParameters(void);
	GIAqueryTraceParameters(GIAqueryTraceParameters* queryTraceParametersToCopy);
	~GIAqueryTraceParameters(void);

	bool detectComparisonVariable;				//never changes
	GIAentityNode* comparisonVariableNode;			//never changes

	bool foundAnswer;
	GIAentityNode* queryAnswerNode;			//record best answer found
	string queryAnswerContext;				//record the query answer context for best answer
	#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
	int numberAnswersFound;					//record number of answers found (with equal number of matches to best answer node)
	vector<GIAentityNode*> queryAnswerNodes;		//record answers found (with equal number of matches to best answer node)
	vector<string> queryAnswerContexts;
	int numberOfMatchedNodesAtPreviousAnswerNode;		//added 29 May 2012
	#endif

	bool thisIsInstanceAndPreviousNodeWasDefinition;	//temp variable
	#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	bool nonMatchingSourceConditions;			//temp variable
	#endif

	bool isCondition;					//temp variable	//not required for testEntityNodeForQuery
	GIAentityNode* sourceEntityNode;			//temp variable	//not required for testEntityNodeForQuery
	string sourceContext;					//temp variable	//not required for testEntityNodeForQuery

	#ifdef GIA_QUERY_DEBUG_LEVEL
	int level;
	#endif
	
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
	bool skipOverPropertyAndDefinitionRelationshipEntities;
	#endif
};

//shared with GIAtranslatorDefineReferencing.cpp
class GIAreferenceTraceParameters
{
public:

	GIAreferenceTraceParameters(void);
	~GIAreferenceTraceParameters(void);

	bool traceMode;						//never changes
	bool traceModeAssertSameReferenceSetID;			//never changes
	int referenceSetID;					//never changes
	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	bool linkSpecificConceptsAndActions;			//never changes
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	bool intrasentenceReference;
	#endif

	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	bool doNotParseQuerySubnetsWithConcepts;
	#endif

	#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	bool testReferenceSetContext;
	GIAentityNode* referenceSetDefiniteEntity;
	//GIAsentence* firstSentenceInList;
	#endif
	bool ensureSameReferenceSetQueryConnections;	//added 2i29a	//CHECKTHIS; check ensureSameReferenceSetQueryConnections is not required for dreammode (identifyReferenceSetsSpecificConceptsAndLinkWithConcepts)

	#ifdef GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS
	bool sameReferenceSetTests;
	#endif

	#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
	bool traceConceptsOnly;
	#endif
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	bool logicalConditionDisableTraceConcepts;
	#endif
	
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
	bool skipOverPropertyAndDefinitionRelationshipEntities;
	#endif
};


class GIAqueryClass
{
	private: GIAdatabaseClass GIAdatabase;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAwordnetClass GIAwordnet;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	public: GIAentityNode* answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, const bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, bool* foundAnswer, GIAentityNode* queryAnswerNode, double* numberOfMatchedNodes, string* queryAnswerContext);
		public: bool testEntityNodeForQueryOrReferenceSet(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters);
			private: int testReferencedEntityNodeForExactNameMatch(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters);
				private: bool verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAentityNode* entityNode, const string comparisonVariableNodeName);
			private: bool determineMatchParameters(const bool exactMatchFoundTemp, const bool traceIsQuery, const int referenceTraceParametersTraceMode, const int numberOfMatchedNodesTempMax, bool* exactMatch);
			private: bool determineIfBestAnswerCandidate(const bool traceModeIsQuery, const bool queryTraceParametersTempFoundAnswer, const bool alreadyFoundAnAnswer, const int numberOfMatchedNodesTemp, const int numberOfMatchedNodesTempMax, const int numberOfMatchedNodesRequiredSynonymnDetectionTemp, const int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, const int referenceTraceParametersTraceMode, const int exactMatchTemp);
	#ifdef GIA_QUERY_SIMPLIFIED_SEARCH
	private: GIAentityNode* answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork2(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, const bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, const bool* foundAnswer, GIAentityNode* queryAnswerNode, const double* numberOfMatchedNodes, const string* queryAnswerContext);
		public: bool testEntityNodeForQueryOrReferenceSet2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters);
			public: bool testReferencedEntityNodeForExactNameMatch2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters);
	#endif

	#ifdef GIA_SYNONYMN_DETECTION
	private: bool compareEntitySynonyms(GIAentityNode* queryEntityNode, GIAentityNode* entityNode);
	#endif
	private: bool compareEntityAliases(GIAentityNode* queryEntityNode, GIAentityNode* entityNode);
	private: void generateTexualContextWithPreviousNodeForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode, GIAentityNode* entityNodePrevious);
		private: void generateTexualContextForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode);
		private: void generateTexualContextBackwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode);
		private: void generateTexualContextEntityStringForwards(string* queryAnswerContext, GIAentityNode* entityNode);
		private: void generateTexualContextEntityStringBackwards(string* queryAnswerContext, GIAentityNode* entityNode);
			private: void generateTexualContextEntityString(string* texualContextEntityString, GIAentityNode* entityNode);

	public: double determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery);
		public: void traceEntityNode(GIAentityNode* entityNode, const int function, int* numberOfMatchedNodes, string* printEntityNodeString, const bool thisIsInstanceAndPreviousNodeWasDefinition, const int referenceSetID, const bool traceInstantiations);
			private: void traceEntityNodeDetermineNextCourseOfAction(string* printEntityNodeString, GIAentityNode* entityNode, string context, const int function, int* numberOfMatchedNodes, const bool thisIsInstanceAndPreviousNodeWasDefinition, const int referenceSetID, const bool traceInstantiations);

	public: void printEntityNodeQualitiesOnly(GIAentityNode* entityNode, string* printEntityNodeString);
	public: void printEntityTimeConditionNodeOnly(GIAentityNode* entityNode, string* printEntityNodeString);

	private: void queryDebugIndentOutputForLevel(const int currentLevel);

	private: bool compareEntityStandard(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, const GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters);
	private: void compareEntityReferenceTrace(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters, const bool compareEntityNamesResult, bool* exactMatch, const bool simplifiedSearch, int* resultOld);
};

#endif
