/*******************************************************************************
 *
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/

#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"

//#define GIA_QUERY_DEBUG
#define GIA_QUERY_ADVANCED_DEBUG
#define GIA_QUERY_FRACTION_MAX_CONFIDENCE_REQUIRED_FOR_UNPRECISE_ANSWER (0.5)

GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, vector<GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * confidence);
	GIAEntityNode * testEntityNodeForQuery(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters);
		GIAEntityNode * testReferencedEntityNodeForNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode * comparisonVariableNode,  bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, bool isSuitableNodeTypeForInexactAnswer, bool isCondition, bool sourceWasConditionAndSourceHadComparisonVariableAttached, bool isSuitableNodeTypeForExactAnswerFromCondition);
		
