/*******************************************************************************
 *
 * File Name: GIAdatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i14c 17-Apr-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: performs simple GIA database functions (storing nodes in ordered arrays/vectors/maps)
 *
 *******************************************************************************/

#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"
#include "GIAglobalDefs.h"




//#define GIA_USE_TIME_NODE_INDEXING

#ifdef GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR
	GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList);
#else
	GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, string * entityNodeName, bool * found, long * index, bool addIfNonexistant, long * currentEntityNodeIDInCompleteList, long * currentEntityNodeIDInConceptEntityNodesList);
	#ifdef GIA_USE_TIME_NODE_INDEXING
	GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant, GIATimeConditionNode * prexistingTimeConditionNode);
	#endif
#endif
GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList);



long maximumLong(long a, long b);



