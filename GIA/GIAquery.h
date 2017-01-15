/*******************************************************************************
 *
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1f1c 23-Jan-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/

#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"

//#define GIA_QUERY_DEBUG

//#define GIA_QUERY_TRACE_PAST_ANSWER	//default: disabled [temporarily testing enabled]
//#define GIA_QUERY_TRACE_INSTANTIATIONS	//default: disabled
#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
	//#define GIA_QUERY_TRACE_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
#endif
//#define GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER	//default: disabled //does not work with GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER, when the inexact answer expected is the object of the question, and not an arbitrary position within the question. need to identify answer entities in the question/text as a separate variable (eg object - but not necessarily) - not based on their incoming links.
#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
	#define GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER_ALLOW_SINGLE_ENTITY_MATCHES
#endif

//#define GIA_QUERY_SUPPORT_NON_EXACT_QUERIES

//#define GIA_QUERY_PRINT_CONTEXT_EVEN_WHEN_EXACT_ANSWER_FOUND
//#define GIA_QUERY_USE_ARTIFICIALLY_ADJUSTED_MAX_CONFIDENCE

#define GIA_QUERY_USE_LONG_CONTEXT_TRACE
#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
	//#define GIA_QUERY_USE_EXTRA_LONG_CONTEXT_TRACE
	//#define GIA_QUERY_TRACE_INSTANTIATIONS
#endif
#define GIA_QUERY_ADVANCED_DEBUG
#define GIA_QUERY_FRACTION_MAX_CONFIDENCE_REQUIRED_FOR_UNPRECISE_ANSWER (0.5)

#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT (1)
#define GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES (2)

GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, vector<GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * confidence, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext);
	GIAEntityNode * testEntityNodeForQuery(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext, bool thisIsInstanceAndPreviousNodeWasDefinition);
		GIAEntityNode * testReferencedEntityNodeForNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode * comparisonVariableNode,  bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, bool isSuitableNodeTypeForInexactAnswer, bool isCondition, GIAEntityNode** queryAnswerPreviousNode, GIAEntityNode* sourceEntityNode, bool sourceIsConditionAndHasComparisonVariableAttached, string * queryAnswerContext, string sourceContext, bool thisIsInstanceAndPreviousNodeWasDefinition);		

void generateTexualContextWithPreviousNodeForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode, GIAEntityNode* entityNodePrevious);
	void generateTexualContextForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode);
	void generateTexualContextBackwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode);
	void generateTexualContextEntityStringForwards(string * queryAnswerContext, GIAEntityNode* entityNode);
	void generateTexualContextEntityStringBackwards(string * queryAnswerContext, GIAEntityNode* entityNode);
		void generateTexualContextEntityString(string * texualContextEntityString, GIAEntityNode* entityNode);

double determineMaxConfidenceOfQuerySemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesListQuery);
	void traceEntityNode(GIAEntityNode * entityNode, int function, int * numberOfMatchedNodes, string * printEntityNodeString, bool thisIsInstanceAndPreviousNodeWasDefinition);
		void traceEntityNodeDetermineNextCourseOfAction(string * printEntityNodeString, GIAEntityNode * entityNode, string context, int function, int * numberOfMatchedNodes, bool thisIsInstanceAndPreviousNodeWasDefinition);

void printEntityNodeQualitiesOnly(GIAEntityNode * entityNode, string * printEntityNodeString);
