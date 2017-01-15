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
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n9a 07-August-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_QUERY
#define HEADER_GIA_QUERY


#include "GIAglobalDefs.h"
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"
#include "GIAConditionNodeClass.h"

#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
	#define GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS		//added 13 July 2012
#endif
#define GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS					//default: enabled	//added 21 May 2012
#define GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES			//default: enabled
#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	#define GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES	//default: enabled
	//#define GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES			//default: disabled
#endif
#define GIA_QUERY_TRACE_PAST_ANSWER						//default: enabled [untested]

//#define GIA_QUERY_TRACE_INSTANTIATIONS	//default: disabled
#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
	#define GIA_QUERY_TRACE_INSTANTIATIONS_VALUE (true)		//added with GIA_USE_1N1ATEMP1TO8_CHANGES
	//#define GIA_QUERY_TRACE_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
#else
	#define GIA_QUERY_TRACE_INSTANTIATIONS_VALUE (false)		//added with GIA_USE_1N1ATEMP1TO8_CHANGES
#endif


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
	//#define GIA_QUERY_TRACE_INSTANTIATIONS
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

class GIAQueryTraceParameters
{
public:

	GIAQueryTraceParameters(void);
	GIAQueryTraceParameters(GIAQueryTraceParameters * queryTraceParametersToCopy);
	~GIAQueryTraceParameters(void);

	bool detectComparisonVariable;				//never changes
	GIAEntityNode * comparisonVariableNode;			//never changes

	bool foundAnswer;
	GIAEntityNode * queryAnswerNode;			//record best answer found
	string queryAnswerContext;				//record the query answer context for best answer
	#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
	int numberAnswersFound;					//record number of answers found (with equal number of matches to best answer node)
	vector<GIAEntityNode*> queryAnswerNodes;		//record answers found (with equal number of matches to best answer node)
	vector<string> queryAnswerContexts;
	int numberOfMatchedNodesAtPreviousAnswerNode;		//added 29 May 2012
	#endif

	bool thisIsInstanceAndPreviousNodeWasDefinition;	//temp variable
	#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	bool nonMatchingSourceConditions;			//temp variable
	#endif

	bool isCondition;					//temp variable	//not required for testEntityNodeForQuery
	GIAEntityNode* sourceEntityNode;			//temp variable	//not required for testEntityNodeForQuery
	string sourceContext;					//temp variable	//not required for testEntityNodeForQuery

	#ifdef GIA_QUERY_DEBUG_LEVEL
	int level;
	#endif
};

//shared with GIATranslatorDefineReferencing.cpp
class GIAReferenceTraceParameters
{
public:

	GIAReferenceTraceParameters(void);
	~GIAReferenceTraceParameters(void);

	bool traceMode;						//never changes
	bool traceModeAssertSameReferenceSetID;			//never changes
	bool referenceSetID;					//never changes

	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	bool intrasentenceReference;
	#endif
};


GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConceptsQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * numberOfMatchedNodes, string * queryAnswerContext);
	bool testEntityNodeForQueryOrReferenceSet(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, int * numberOfMatchedNodes, bool knownBestMatch, int * numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAQueryTraceParameters * queryTraceParameters, GIAReferenceTraceParameters * referenceTraceParameters);
		int testReferencedEntityNodeForExactNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, int * numberOfMatchedNodes, bool knownBestMatch, int * numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAQueryTraceParameters * queryTraceParameters, GIAReferenceTraceParameters * referenceTraceParameters);
			bool verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAEntityNode * entityNode, string comparisonVariableNodeName);
		bool determineMatchParameters(bool exactMatchFoundTemp, bool traceIsQuery, int referenceTraceParametersTraceMode, int numberOfMatchedNodesTempMax, bool * exactMatch);
		bool determineIfBestAnswerCandidate(bool traceModeIsQuery, bool queryTraceParametersTempFoundAnswer, bool alreadyFoundAnAnswer, int numberOfMatchedNodesTemp, int numberOfMatchedNodesTempMax, int numberOfMatchedNodesRequiredSynonymnDetectionTemp, int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, int referenceTraceParametersTraceMode, int exactMatchTemp);

bool compareEntityNames(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode);
void generateTexualContextWithPreviousNodeForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode, GIAEntityNode* entityNodePrevious);
	void generateTexualContextForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode);
	void generateTexualContextBackwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode);
	void generateTexualContextEntityStringForwards(string * queryAnswerContext, GIAEntityNode* entityNode);
	void generateTexualContextEntityStringBackwards(string * queryAnswerContext, GIAEntityNode* entityNode);
		void generateTexualContextEntityString(string * texualContextEntityString, GIAEntityNode* entityNode);

double determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConceptsQuery);
	void traceEntityNode(GIAEntityNode * entityNode, int function, int * numberOfMatchedNodes, string * printEntityNodeString, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations);
		void traceEntityNodeDetermineNextCourseOfAction(string * printEntityNodeString, GIAEntityNode * entityNode, string context, int function, int * numberOfMatchedNodes, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations);

void printEntityNodeQualitiesOnly(GIAEntityNode * entityNode, string * printEntityNodeString);
void printEntityTimeConditionNodeOnly(GIAEntityNode * entityNode, string * printEntityNodeString);



#endif
