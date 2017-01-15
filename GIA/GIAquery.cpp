/*******************************************************************************
 *
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1d4b 01-Nov-2011
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/


#include "GIAquery.h"
#include "GIAdatabase.h"

GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesList, vector<string> *conceptEntityNamesList, vector<GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * confidence, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext)
{
	double bestConfidence = 0.0;
	double bestConfidenceAssumingFoundAnswer = 0.0;
	
	#ifdef GIA_QUERY_DEBUG
	if(detectComparisonVariable)
	{
		cout << "comparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;
	}
	#endif
	
	GIAEntityNode * networkEntityNodeWhenSearchedResultsInBestConfidence = NULL;
	GIAEntityNode * queryEntityNodeWhenSearchedResultsInBestConfidence = NULL;
		
	vector<GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		GIAEntityNode* currentQueryEntityNode = *entityIterQuery;
		
		bool foundQueryEntityNodeName = false;
		long queryEntityNodeIndex = -1;
		string queryEntityNodeName = currentQueryEntityNode->entityName;
		GIAEntityNode * conceptEntityMatchingCurrentQueryEntity = findOrAddEntityNodeByName(NULL, conceptEntityNodesList, conceptEntityNamesList, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL);
		
		if(foundQueryEntityNodeName)
		{
			#ifdef GIA_QUERY_DEBUG
			cout << "currentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
			#endif
				
			//now start matching structure search for all properties of the identical concept node (to current query entity name) in Semantic Network
			
			int numberOfMatchedNodes = 0;	
			bool foundAnswerTemp = false;
			GIAEntityNode* queryAnswerNodeTemp = NULL;
			GIAEntityNode* queryPeviousAnswerNodeTemp = NULL;
			string queryAnswerContextTemp = "";

			queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, false, &queryPeviousAnswerNodeTemp, &queryAnswerContextTemp);

			double currentConfidence = (double)numberOfMatchedNodes;	//NB confidence value definition for query network structure matching is currently very simple; it is just equal to the number of matched nodes found 

			//cout << "currentConfidence = " << currentConfidence << endl;
			
			if(detectComparisonVariable)
			{
				if(foundAnswerTemp)
				{//if comparisonVariable is identified within the query, then an exact answer is required... 
					if(currentConfidence > bestConfidenceAssumingFoundAnswer)
					{
						bestConfidenceAssumingFoundAnswer = currentConfidence;
						queryAnswerNode = queryAnswerNodeTemp;
						*queryAnswerPreviousNode = queryPeviousAnswerNodeTemp;
						*queryAnswerContext = queryAnswerContextTemp;
						*foundAnswer = true;
						*confidence = bestConfidenceAssumingFoundAnswer;
						
						#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
						if(*foundAnswer)
						{
							//add first node to trace;
							generateTexualContextEntityStringBackwards(queryAnswerContext, conceptEntityMatchingCurrentQueryEntity);
							#ifdef GIA_QUERY_DEBUG
							cout << "queryAnswerContext = " << *queryAnswerContext << endl;
							#endif
						}
						#endif							
					}
				}
			}

			if(currentConfidence > bestConfidence)
			{
				bestConfidence = currentConfidence;
				//cout << "bestConfidence = " << bestConfidence << endl;
				networkEntityNodeWhenSearchedResultsInBestConfidence = conceptEntityMatchingCurrentQueryEntity;
				queryEntityNodeWhenSearchedResultsInBestConfidence = currentQueryEntityNode;
			}				

			
		}
	}
	
	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 1" << endl;
	#endif
	
	if(!detectComparisonVariable || !(*foundAnswer))
	{//now set draw parameters for optimium solution...
	
		//cout << "asf2" << endl;
		//cout << "queryEntityNodeWhenSearchedResultsInBestConfidence->entityName = " << queryEntityNodeWhenSearchedResultsInBestConfidence->entityName << endl; 
		bool foundAnswerTemp = false;
		GIAEntityNode* queryAnswerNodeTemp = NULL;
		GIAEntityNode* queryPeviousAnswerNodeTemp = NULL;
		string queryAnswerContextTemp = "";
		int numberOfMatchedNodes = 0;
		
		queryAnswerNodeTemp = testEntityNodeForQuery(queryEntityNodeWhenSearchedResultsInBestConfidence, networkEntityNodeWhenSearchedResultsInBestConfidence, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, true, queryAnswerPreviousNode, &queryAnswerContextTemp);
	
		//cout << "numberOfMatchedNodes = " << numberOfMatchedNodes << endl;
		
		if(!detectComparisonVariable && foundAnswerTemp)
		{
			//cout << "asf1" << endl;
			*foundAnswer = true;
			queryAnswerNode = queryAnswerNodeTemp;
			*queryAnswerPreviousNode = queryPeviousAnswerNodeTemp;
			*queryAnswerContext = queryAnswerContextTemp;
		}
		*confidence = (double)numberOfMatchedNodes;
	}
	

		
	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 2" << endl;
	#endif
	return queryAnswerNode;
				
}





GIAEntityNode * testReferencedEntityNodeForNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode * comparisonVariableNode,  bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, bool isSuitableNodeTypeForInexactAnswer, bool isCondition, GIAEntityNode** queryAnswerPreviousNode, GIAEntityNode* sourceEntityNode, bool sourceIsConditionAndHasComparisonVariableAttached, string * queryAnswerContext, string sourceContext)
{
	if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	{
		
		//cout << "\tqueryEntityNode = " << queryEntityNode->entityName << endl;
		//cout << "\tentityNode = " << entityNode->entityName << endl;
			
		/*	
		#ifdef GIA_QUERY_USE_EXTRA_LONG_CONTEXT_TRACE
		generateTexualContextBackwards(queryAnswerContext, sourceContext, entityNode);	
		#endif
		*/
		
		//cout << "*queryAnswerContext = " << *queryAnswerContext << endl;
		

		bool foundMatch = false;
		if(sourceIsConditionAndHasComparisonVariableAttached)
		{
			//cout << "HERE" << endl;
			
			if(!(*foundAnswer))
			{//do not overwrite match, if already found match 

				//cout << "queryEntityNode->entityName = " << queryEntityNode->entityName << endl;
				//cout << "entityNode->entityName = " << entityNode->entityName << endl;			
				//this case is required in the case an answer is connected to a condition property or a condition definition (both new additions to the algorithm)
				foundMatch = true;
			}
		}
		else
		{
			if(queryEntityNode->entityName == entityNode->entityName)	//allow non-equal conditions to be matched during network comparison
			{
				if(detectComparisonVariable && comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
				{//exact match found [NB if a quantity, the queryEntityNode's entityName will not have the comparisonVariable name (_$qVar) specified, and therefore a matched entity node entityName is required]
					foundMatch = true;
					#ifdef GIA_QUERY_DEBUG
					cout << "entityNode->quantityNumber = " << entityNode->quantityNumber << endl;
					#endif
				}
				else
				{
					//cout << "IE1" << endl;
					if(isSuitableNodeTypeForInexactAnswer)
					{
						//cout << "IE2" << endl;
						
						if(findBestInexactAnswerAndSetDrawParameters)
						{
							cout << "IE3" << endl;
							foundMatch = true;
							#ifdef GIA_QUERY_DEBUG
							cout << "foundBestInexactAnswerAndSetDrawParameters:" << entityNode->entityName << endl;
							#endif
							//set queryAnswerNode if entityNode is an object;
							/*eg;
							Which house does did Jane buy?
							Which house is that?
							What day is it?
							What house did Jane buy?
							*/

						}			
					}
				}
			}
			else if(detectComparisonVariable)
			{
				if(queryEntityNode->entityName == comparisonVariableNode->entityName)
				{//exact match found
					foundMatch = true;
				}
			}
		}
				
		if(foundMatch)
		{
			//OLD: if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters))

			*queryAnswerPreviousNode = sourceEntityNode;
			#ifndef GIA_QUERY_USE_LONG_CONTEXT_TRACE
			generateTexualContextWithPreviousNodeForwards(queryAnswerContext, sourceContext, entityNode, *queryAnswerPreviousNode);	//*queryAnswerContext = sourceContext;
			#endif

			entityNode->isAnswerToQuery = true;
			*foundAnswer = true; 
			queryAnswerNode = entityNode;
			#ifdef GIA_QUERY_DEBUG
			cout << "foundAnswer:" << entityNode->entityName << endl;
			#endif
			//CHECKTHIS; need to take into account vector of answers (not just a single answer)

			if(findBestInexactAnswerAndSetDrawParameters)
			{
				entityNode->isAnswerContextToQuery = true;			 
			}
			else
			{
				entityNode->testedForQueryComparison = true;		//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]				
			}	

		}
		
		string queryAnswerContextTemp = "";
		#ifndef GIA_QUERY_TRACE_PAST_ANSWER
		if(((queryEntityNode->entityName == entityNode->entityName) || isCondition) && !(*foundAnswer))
		{//only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer
		#endif
			queryAnswerNode = testEntityNodeForQuery(queryEntityNode, entityNode, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, queryAnswerPreviousNode, &queryAnswerContextTemp);
		#ifndef GIA_QUERY_TRACE_PAST_ANSWER
		}
		#endif
		
		#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
		if(*foundAnswer)
		{
			*queryAnswerContext = *queryAnswerContext + queryAnswerContextTemp;
			generateTexualContextBackwards(queryAnswerContext, sourceContext, entityNode);
			/*
			#ifndef GIA_QUERY_USE_EXTRA_LONG_CONTEXT_TRACE
			generateTexualContextBackwards(queryAnswerContext, sourceContext, entityNode);
			#endif
			*/
		}	
		#endif

	}
	
	return queryAnswerNode;
	
	
}


				
//current queryEntityNode, currentEntityNode
GIAEntityNode * testEntityNodeForQuery(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext)
{
	*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		
	/*
	bool searchingForExactMatchAtThisQueryEntityNode = false;
	//test for exact match
	if(detectComparisonVariable)
	{
		if(comparisonVariableNode->entityName == queryEntityNode->entityName)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "comparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;	//should equal something like _$qVar
			#endif
			searchingForExactMatchAtThisQueryEntityNode = true;
		}
	}
	*/
						
	if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	{//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]
	
		if(findBestInexactAnswerAndSetDrawParameters)
		{
			entityNode->isAnswerContextToQuery = true;			 
		}
		else
		{
			entityNode->testedForQueryComparison = true;		//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]				
		}
		
		#ifdef GIA_QUERY_DEBUG
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}
		else if(entityNode->isCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;		
		}			
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		} 		 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		#endif
		
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;
		*/
		
		bool foundAnswerTemp = false;
	
		//this case structure supports the scenario where a "being"/"having" condition needs to be compared to and matched with an ordinary condition (containing the query comparison variable)
		if(entityNode->isCondition)
		{
			if(detectComparisonVariable)
			{
				bool sourceIsConditionAndHasComparisonVariableAttached = false;
				//cout << "\tentityNode = " << entityNode->entityName << " (is condition)" << endl;	

				vector<GIAEntityNode*>::iterator entityIterQuery;	
				for(entityIterQuery = queryEntityNode->PropertyNodeList.begin(); entityIterQuery != queryEntityNode->PropertyNodeList.end(); entityIterQuery++) 
				{//DRAW SHOULD NOT BE REQUIRED	

					//cout << "a31" << endl;
					if((*entityIterQuery)->entityName == comparisonVariableNode->entityName)
					{//exact match found
						sourceIsConditionAndHasComparisonVariableAttached = true;
						//cout << "\t A sourceIsConditionAndHasComparisonVariableAttached" << endl;
					}								
				}
				for(entityIterQuery = queryEntityNode->EntityNodeDefinitionList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionList.end(); entityIterQuery++) 
				{
					if((*entityIterQuery)->entityName == comparisonVariableNode->entityName)
					{//exact match found
						sourceIsConditionAndHasComparisonVariableAttached = true;
						//cout << "\t B sourceIsConditionAndHasComparisonVariableAttached" << endl;
					}	
				}
					
				//probably not required;			
				if(queryEntityNode->conditionSubjectEntity != NULL)
				{	
					//cout << "as" << endl;
					if(queryEntityNode->conditionSubjectEntity->entityName == comparisonVariableNode->entityName)
					{//exact match found
						sourceIsConditionAndHasComparisonVariableAttached = true;
						//cout << "\t C sourceIsConditionAndHasComparisonVariableAttached" << endl;
						//cout << "queryEntityNode->entityName = " << queryEntityNode->entityName << endl;
						//cout << "entityNode->entityName = " << entityNode->entityName << endl;						
					}					
					//cout << "b6" << endl;								
				}			
				if(queryEntityNode->conditionObjectEntity != NULL)
				{	
					//cout << "as2" << endl;	
					if(queryEntityNode->conditionObjectEntity->entityName == comparisonVariableNode->entityName)
					{//exact match found
						sourceIsConditionAndHasComparisonVariableAttached = true;
						//cout << "\t D sourceIsConditionAndHasComparisonVariableAttached" << endl;
						//cout << "queryEntityNode->entityName = " << queryEntityNode->entityName << endl;
						//cout << "entityNode->entityName = " << entityNode->entityName << endl;
					}						
				}
				
				if(sourceIsConditionAndHasComparisonVariableAttached)
				{
					vector<GIAEntityNode*>::iterator entityIter;
					for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
					{
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "being ";
						//cout << "\t 2A sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIter, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext);				
					}
	
					for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
					{//DRAW SHOULD NOT BE REQUIRED	
						//cout << "a31" << endl;
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "having ";
						//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIter, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext);
					}
					
					//CHECK THIS; need to take into account condition types for match
						
					vector<GIAEntityNode*>::iterator ConditionIter;	
					for(ConditionIter = entityNode->ConditionNodeList.begin(); ConditionIter != entityNode->ConditionNodeList.end(); ConditionIter++) 
					{	
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "";
						//cout << "\t 2C sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIter, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext);
					}			
									
					//CHECK THIS; need to take into account condition types for match		
					//go reverse also...
						
					for(ConditionIter = entityNode->IncomingConditionNodeList.begin(); ConditionIter != entityNode->IncomingConditionNodeList.end(); ConditionIter++) 
					{	
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "";
						//cout << "\t 2D sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIter, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext);
					}							
				}
			}
		}
			
		vector<GIAEntityNode*>::iterator actionIterQuery;
		vector<GIAEntityNode*>::iterator actionIter;
		for(actionIterQuery = queryEntityNode->IncomingActionNodeList.begin(); actionIterQuery != queryEntityNode->IncomingActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->IncomingActionNodeList.begin(); actionIter != entityNode->IncomingActionNodeList.end(); actionIter++) 
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				//cout << "A" << endl;
				string sourceContext = "is ";	//is done to 
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			
				//cout << "AA" << endl;
			}
		}			
		for(actionIterQuery = queryEntityNode->ActionNodeList.begin(); actionIterQuery != queryEntityNode->ActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->ActionNodeList.begin(); actionIter != entityNode->ActionNodeList.end(); actionIter++) 
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "";
				//cout << "AAA" << endl;	
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);				
				//cout << "AAAA" << endl;
			}
		}	

		if(queryEntityNode->actionSubjectEntity != NULL)
		{		
			if(entityNode->actionSubjectEntity != NULL)
			{		
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "is done by ";
				
				
				//cout << "b5" << endl;
				//cout << "entityNode->actionSubjectEntity->testedForQueryComparison = " << entityNode->actionSubjectEntity->testedForQueryComparison << endl;
				//cout << "entityNode->actionSubjectEntity->entityName = " << entityNode->actionSubjectEntity->entityName << endl;
				//cout << "entityNode->actionObjectEntity->entityName = " << entityNode->actionObjectEntity->entityName << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionSubjectEntity, entityNode->actionSubjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
							
				//cout << "b6" << endl;		
			}	
		}			
		if(queryEntityNode->actionObjectEntity != NULL)
		{		
			if(entityNode->actionObjectEntity != NULL)
			{	
				bool isSuitableNodeTypeForInexactAnswer = true;
				string sourceContext = "";	
				//cout << "b7" << endl;
				
				
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionObjectEntity, entityNode->actionObjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
				
				//cout << "b8" << endl;		
			}	
		}
					
		
		//cout << "a2" << endl;
		
		//conditions connections
		vector<GIAEntityNode*>::iterator ConditionIterQuery;
		vector<GIAEntityNode*>::iterator ConditionIter;
		
		//CHECK THIS; need to take into account condition types for match
		for(ConditionIterQuery = queryEntityNode->ConditionNodeList.begin(); ConditionIterQuery != queryEntityNode->ConditionNodeList.end(); ConditionIterQuery++) 
		{	
			for(ConditionIter = entityNode->ConditionNodeList.begin(); ConditionIter != entityNode->ConditionNodeList.end(); ConditionIter++) 
			{	
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			}			
		}				
		//CHECK THIS; need to take into account condition types for match		
		//go reverse also...
		for(ConditionIterQuery = queryEntityNode->IncomingConditionNodeList.begin(); ConditionIterQuery != queryEntityNode->IncomingConditionNodeList.end(); ConditionIterQuery++) 
		{	
			for(ConditionIter = entityNode->IncomingConditionNodeList.begin(); ConditionIter != entityNode->IncomingConditionNodeList.end(); ConditionIter++) 
			{	
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			}	
		}
		
		if(queryEntityNode->conditionSubjectEntity != NULL)
		{		
			if(entityNode->conditionSubjectEntity != NULL)
			{	
				bool isSuitableNodeTypeForInexactAnswer = false;
				
				string sourceContext = "";	
				//cout << "b5" << endl;
				//cout << "entityNode->conditionSubjectEntity->testedForQueryComparison = " << entityNode->conditionSubjectEntity->testedForQueryComparison << endl;
				//cout << "entityNode->conditionSubjectEntity->entityName = " << entityNode->conditionSubjectEntity->entityName << endl;
				//cout << "entityNode->conditionObjectEntity->entityName = " << entityNode->conditionObjectEntity->entityName << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionSubjectEntity, entityNode->conditionSubjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
							
				//cout << "b6" << endl;		
			}	
		}			
		if(queryEntityNode->conditionObjectEntity != NULL)
		{		
			if(entityNode->conditionObjectEntity != NULL)
			{	
				string sourceContext = "";	
				//cout << "b7" << endl;
				bool isSuitableNodeTypeForInexactAnswer = true;
				
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionObjectEntity, entityNode->conditionObjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
				
				//cout << "b8" << endl;		
			}	
		}
			
					
				
		
		
		//cout << "a3" << endl;
		
		//property connections	
		vector<GIAEntityNode*>::iterator entityIterQuery;
		vector<GIAEntityNode*>::iterator entityIter;	
		for(entityIterQuery = queryEntityNode->PropertyNodeList.begin(); entityIterQuery != queryEntityNode->PropertyNodeList.end(); entityIterQuery++) 
		{//DRAW SHOULD NOT BE REQUIRED	
			for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
			{//DRAW SHOULD NOT BE REQUIRED	
				//cout << "a31" << endl;
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "has ";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			}		
		}
		/*this has been removed 25 Sept - use entityNodeContainingThisProperty instead
		//go reverse also...		
		for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++) 
		{//DRAW SHOULD NOT BE REQUIRED
			//cout << "a32" << endl;	
			currentReferenceInPrintList = testEntityNodeForQuery((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
		}
		*/	
		//cout << "a3b" << endl;
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		//go upwards also...	
		if(queryEntityNode->entityNodeDefiningThisInstance != NULL)
		{
			if(entityNode->entityNodeDefiningThisInstance != NULL)
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "is ";
				//cout << "a33" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeDefiningThisInstance, entityNode->entityNodeDefiningThisInstance, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			}		
		}
		#endif
		//cout << "a3c" << endl;
		if(queryEntityNode->entityNodeContainingThisProperty != NULL)
		{
			if(entityNode->entityNodeContainingThisProperty != NULL)
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "possessed by ";
				//cout << "a34" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeContainingThisProperty, entityNode->entityNodeContainingThisProperty, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);
			}
		}
		//cout << "a4" << endl;
		

		
		//cout << "a5" << endl;				
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionList.end(); entityIterQuery++) 
		{
			for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "is ";	//is defined by
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);				
			}
		}
		//go reverse also...	
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionReverseList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionReverseList.end(); entityIterQuery++) 
		{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
			for(entityIter = entityNode->EntityNodeDefinitionReverseList.begin(); entityIter != entityNode->EntityNodeDefinitionReverseList.end(); entityIter++) 
			{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "defines ";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);						
			}
		}
		//cout << "a6" << endl;
		
				
		//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
		//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]	
		for(entityIterQuery = queryEntityNode->AssociatedInstanceNodeList.begin(); entityIterQuery != queryEntityNode->AssociatedInstanceNodeList.end(); entityIterQuery++) 
		{
			for(entityIter = entityNode->AssociatedInstanceNodeList.begin(); entityIter != entityNode->AssociatedInstanceNodeList.end(); entityIter++) 
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
				string sourceContext = "is seen in ";	//is realised in/defines/is instantiated in
				#else
				string sourceContext = "";
				#endif
				//cout << "as0" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext);						
				//cout << "as1" << endl;
			}
		}	
		
		if(foundAnswerTemp)
		{
			*foundAnswer = true;
		}
		
		//cout << "a7" << endl;
		
		/*
		if(initialiseOrPrint == DRAW_PRINT)
		{	
			//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
			
			int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
			
			int entityColour;
			if(entityNode->hasAssociatedInstanceIsAction)
			{
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR;	//clearly identify the definition of the action
				}
			}			
			else if(entityNode->isProperty)
			{
				if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
				{
					boxThickness = GIA_DRAW_THICKNESS_THICK;
				}
				
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else if(entityNode->hasQuantity)
				{
					entityColour = GIA_DRAW_PROPERTY_QUANTITY_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_PROPERTY_NODE_COLOUR;
				}
			}
			else if(entityNode->isAction)
			{
				entityColour = GIA_DRAW_ACTION_NODE_COLOUR;	
			}			
			else if(entityNode->hasAssociatedTime)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
			}
			//else if(entityNode->hasConditionTime)
			//{
			//	entityColour = GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR;	//clear identify a time node
			//}		
			else if(entityNode->hasAssociatedInstance)
			{//the original spec seemed to imply that entities that have associated properties (ie, that define properties) are special but they don't appear to be
				if(!(entityNode->isProperty))
				{		
					//added 2 May 11a (highlight entities which define property nodes)
					entityColour = GIA_DRAW_PROPERTY_DEFINITION_NODE_COLOUR;	//OLD: no colour modifier, just use basic entity colour; GIA_DRAW_BASICENTITY_NODE_COLOUR;
				}
			}					
			else
			{	
				entityColour = GIA_DRAW_BASICENTITY_NODE_COLOUR;
			}
										
			//first, print this action node.
			string nameOfBox = "";
			if(entityNode->hasQuantity)
			{
				char quantityNumberStringcharstar[20];
				if(entityNode->isQuery)
				{
					strcpy(quantityNumberStringcharstar, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_TEMP_FOR_DISPLAY_ONLY);
				}
				else
				{
					
					sprintf(quantityNumberStringcharstar, "%d", entityNode->quantityNumber);
					
				}
				nameOfBox = nameOfBox + quantityNumberStringcharstar + " " + entityNode->entityName;
				
			}
			else
			{
				nameOfBox = entityNode->entityName;
			}
			currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, writeFileObject, boxThickness);

		}
		*/
		
		//cout << "a8" << endl;	
		#ifdef GIA_QUERY_DEBUG
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}	
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}
		else if(entityNode->isCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;		
		}					
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		} 		
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
		#endif
							
	}
	//cout << "a0c" << endl;
	
	return queryAnswerNode;
		
}



void generateTexualContextWithPreviousNodeForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode, GIAEntityNode* entityNodePrevious)
{
	generateTexualContextEntityStringForwards(queryAnswerContext, entityNodePrevious);

	*queryAnswerContext = *queryAnswerContext + sourceContext;
	
	generateTexualContextEntityStringForwards(queryAnswerContext, entityNode);
	
	*queryAnswerContext = *queryAnswerContext + ". ";
}

void generateTexualContextForwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode)
{		
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);
	
	*queryAnswerContext = *queryAnswerContext + sourceContext + texualContextEntityString;		
}

void generateTexualContextBackwards(string * queryAnswerContext, string sourceContext, GIAEntityNode* entityNode)
{		
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);
	
	*queryAnswerContext = sourceContext + texualContextEntityString + *queryAnswerContext;	
}

void generateTexualContextEntityStringForwards(string * queryAnswerContext, GIAEntityNode* entityNode)
{	
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);
	
	*queryAnswerContext = *queryAnswerContext + texualContextEntityString;	
}

void generateTexualContextEntityStringBackwards(string * queryAnswerContext, GIAEntityNode* entityNode)
{	
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);
	
	*queryAnswerContext = texualContextEntityString + *queryAnswerContext;	
}

void generateTexualContextEntityString(string * texualContextEntityString, GIAEntityNode* entityNode)
{
	string entityPretext = "";
	string entityPosttext = "";
	
	if(entityNode->negative)
	{
		entityPosttext = entityPosttext + "(not) ";
	}
	
	/*
	if(entityNode->isDefinite)
	{//ie definite noun
		entityPretext = entityPretext + "the ";
	}
	*/
	
	if(entityNode->isProperty)
	{
	}
	else if(entityNode->isAction)
	{	
	}
	else if(entityNode->isCondition)
	{
	}
	else
	{//concept
		entityPretext = entityPretext + "a ";
	}
	
	*texualContextEntityString =  entityPretext + entityNode->entityName + " " + entityPosttext;
}




double determineMaxConfidenceOfQuerySemanticNetwork(vector<GIAEntityNode*> *conceptEntityNodesListQuery)
{
	double bestConfidence = 0.0;
				
	vector<GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		GIAEntityNode* currentQueryEntityNode = *entityIterQuery;
		
		int numberOfMatchedNodes = 0;	
		string queryAnswerContextTemp = "";
		traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES, &numberOfMatchedNodes, &queryAnswerContextTemp);

		double currentConfidence = (double)numberOfMatchedNodes;	//NB confidence value definition for query network structure matching is currently very simple; it is just equal to the number of matched nodes found 

		if(currentConfidence > bestConfidence)
		{
			bestConfidence = currentConfidence;
		}				
	}
	
	return bestConfidence-1.0;

}


void traceEntityNodeDetermineNextCourseOfAction(string * printEntityNodeString, GIAEntityNode * entityNode, string context, int function, int * numberOfMatchedNodes)
{
	if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT)
	{
		#ifdef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
		*printEntityNodeString = *printEntityNodeString + "\nContext: = " + entityNode->entityName;			
		#else
		*printEntityNodeString = *printEntityNodeString + "\nContext: " + context + " = " + entityNode->entityName;		
		#endif	
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString);
	}
	else
	{
		cout << "error: illegal trace entity nodes function" << endl;
		exit(0);
	}

}
void traceEntityNode(GIAEntityNode * entityNode, int function, int * numberOfMatchedNodes, string * printEntityNodeString)
{
	if(!(entityNode->queryEntityTraced))
	{
		//cout << "entityNode being traced = " << entityNode->entityName << endl;
		//cout << "*numberOfMatchedNodes = " << *numberOfMatchedNodes << endl;
		
		entityNode->queryEntityTraced = true;
		*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
	

		//if(!(entityNode->hasAssociatedInstanceIsAction))
		//{
			if(entityNode->ActionNodeList.begin() != entityNode->ActionNodeList.end())
			{
				for(entityNode->ActionNodeListIterator = entityNode->ActionNodeList.begin(); entityNode->ActionNodeListIterator < entityNode->ActionNodeList.end(); entityNode->ActionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->ActionNodeListIterator)), "outgoingAction(s)", function, numberOfMatchedNodes);
				}				
			}
			if(entityNode->IncomingActionNodeList.begin() != entityNode->IncomingActionNodeList.end())
			{
				for(entityNode->IncomingActionNodeListIterator = entityNode->IncomingActionNodeList.begin(); entityNode->IncomingActionNodeListIterator < entityNode->IncomingActionNodeList.end(); entityNode->IncomingActionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->IncomingActionNodeListIterator)), "incomingAction(s)", function, numberOfMatchedNodes);			
				}				
			}
		//}
		//if(entityNode->hasAssociatedInstanceIsAction)
		//{
			if(entityNode->actionSubjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->actionSubjectEntity, "actionSubjectEntity", function, numberOfMatchedNodes);
			}
			if(entityNode->actionObjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->actionObjectEntity, "actionObjectEntity", function, numberOfMatchedNodes);
			}
		//}

		//if(!(entityNode->hasAssociatedInstanceIsCondition))	//CHECKTHIS; doesnt support recursive conditions at the moment (conditions of conditions)
		//{
			if(entityNode->ConditionNodeList.begin() != entityNode->ConditionNodeList.end())
			{
				for(entityNode->ConditionNodeListIterator = entityNode->ConditionNodeList.begin(); entityNode->ConditionNodeListIterator < entityNode->ConditionNodeList.end(); entityNode->ConditionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->ConditionNodeListIterator)), "conditionNode(s)", function, numberOfMatchedNodes);
				}				
			}
			if(entityNode->IncomingConditionNodeList.begin() != entityNode->IncomingConditionNodeList.end())
			{
				for(entityNode->IncomingConditionNodeListIterator = entityNode->IncomingConditionNodeList.begin(); entityNode->IncomingConditionNodeListIterator < entityNode->IncomingConditionNodeList.end(); entityNode->IncomingConditionNodeListIterator++)
				{		
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->IncomingConditionNodeListIterator)), "incomingConditionNode(s)", function, numberOfMatchedNodes);
				}				
			}
		//}
				
		//if(entityNode->hasAssociatedInstanceIsCondition)
		//{
			if(entityNode->conditionSubjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->conditionSubjectEntity, "conditionSubjectEntity", function, numberOfMatchedNodes);
			}
			if(entityNode->conditionObjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->conditionObjectEntity, "conditionObjectEntity", function, numberOfMatchedNodes);
			}
		//} 
		//if(entityNode->PropertyNodeList.begin() != entityNode->PropertyNodeList.end())
		//{
			for(entityNode->PropertyNodeListIterator = entityNode->PropertyNodeList.begin(); entityNode->PropertyNodeListIterator < entityNode->PropertyNodeList.end(); entityNode->PropertyNodeListIterator++)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->PropertyNodeListIterator)), "propertyNode(s)", function, numberOfMatchedNodes);	
			}				
		//}

		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		if(entityNode->entityNodeDefiningThisInstance != NULL)
		{
			traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->entityNodeDefiningThisInstance, "entityNodeDefiningThisInstance", function, numberOfMatchedNodes);
		}
		#endif
		//if(entityNode->isProperty)
		//{
			if(entityNode->entityNodeContainingThisProperty != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->entityNodeContainingThisProperty, "entityNodeContainingThisProperty (parent)", function, numberOfMatchedNodes);
			}	
		//}
				
		//if(!(entityNode->isProperty))
		//{
			if(entityNode->EntityNodeDefinitionList.begin() != entityNode->EntityNodeDefinitionList.end())
			{
				for(entityNode->EntityNodeDefinitionListIterator = entityNode->EntityNodeDefinitionList.begin(); entityNode->EntityNodeDefinitionListIterator < entityNode->EntityNodeDefinitionList.end(); entityNode->EntityNodeDefinitionListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->EntityNodeDefinitionListIterator)), "incomingEntityNodeDefinition", function, numberOfMatchedNodes);	
				}				
			}
			if(entityNode->EntityNodeDefinitionReverseList.begin() != entityNode->EntityNodeDefinitionReverseList.end())
			{
				for(entityNode->EntityNodeDefinitionReverseListIterator = entityNode->EntityNodeDefinitionReverseList.begin(); entityNode->EntityNodeDefinitionReverseListIterator < entityNode->EntityNodeDefinitionReverseList.end(); entityNode->EntityNodeDefinitionReverseListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->EntityNodeDefinitionReverseListIterator)), "incomingEntityNodeDefinition(s)", function, numberOfMatchedNodes);	
				}				
			}
		//}
		
		if(entityNode->AssociatedInstanceNodeList.begin() != entityNode->AssociatedInstanceNodeList.end())
		{	
			for(entityNode->AssociatedInstanceNodeListIterator = entityNode->AssociatedInstanceNodeList.begin(); entityNode->AssociatedInstanceNodeListIterator < entityNode->AssociatedInstanceNodeList.end(); entityNode->AssociatedInstanceNodeListIterator++)
			{			
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->AssociatedInstanceNodeListIterator)), "associatedInstanceNodes(s)", function, numberOfMatchedNodes);	
			}
		}
					

		/*
		//if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
		//{
			if(entityNode->timeConditionNode != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->timeConditionNode->conditionName, "timeConditionNode", function, numberOfMatchedNodes);
			}
		//}
		*/
	}	

}
	
	
	


