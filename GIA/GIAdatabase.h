/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/

#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"

//#define GIA_DATABASE_DEBUG

//#define GIA_USE_TIME_NODE_INDEXING

GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList);
#ifdef GIA_USE_TIME_NODE_INDEXING
GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant, GIATimeConditionNode * prexistingTimeConditionNode);
#endif

GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList);

long maximumLong(long a, long b);
