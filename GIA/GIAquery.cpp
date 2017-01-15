/*******************************************************************************
 *
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i9a 11-Apr-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 *
 *******************************************************************************/


#include "GIAquery.h"
#include "GIAdatabase.h"


GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * confidence, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext)
{
	double bestConfidence = 0.0;
	double bestConfidenceAssumingFoundAnswer = 0.0;
	
	#ifdef GIA_QUERY_DEBUG
	if(detectComparisonVariable)
	{
		cout << "answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork" << endl;
		cout << "detectComparisonVariable" << endl;
		cout << "\tcomparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;
	}
	#endif
	
	GIAEntityNode * networkEntityNodeWhenSearchedResultsInBestConfidence = NULL;
	GIAEntityNode * queryEntityNodeWhenSearchedResultsInBestConfidence = NULL;
	
	bool foundAtLeastOneMatch = false;
		
	unordered_map<string, GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		//cout << "entityIterQuery->second->entityName = " << entityIterQuery->second->entityName << endl;
		GIAEntityNode* currentQueryEntityNode = entityIterQuery->second;
		
		bool foundQueryEntityNodeName = false;
		long queryEntityNodeIndex = -1;
		string queryEntityNodeName = currentQueryEntityNode->entityName;
		//cout << "saf1" << endl;
		
		GIAEntityNode * conceptEntityMatchingCurrentQueryEntity = findOrAddEntityNodeByName(NULL, conceptEntityNodesList, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL);
		
		//cout << "saf2" << endl;
		
		if(foundQueryEntityNodeName)
		{
			#ifdef GIA_QUERY_DEBUG
			cout << "foundQueryEntityNodeName" << endl;
			cout << "\tcurrentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
			#endif
				
			//now start matching structure search for all properties of the identical concept node (to current query entity name) in Semantic Network
			
			int numberOfMatchedNodes = 0;	
			bool foundAnswerTemp = false;
			GIAEntityNode* queryAnswerNodeTemp = NULL;
			GIAEntityNode* queryPeviousAnswerNodeTemp = NULL;
			string queryAnswerContextTemp = "";

			/*
			NB bool thisIsInstanceAndPreviousNodeWasDefinition could be replaced, and instead a simple condition could be applied here;
			if(currentQueryEntityNode->AssociatedInstanceNodeList.begin() != currentQueryEntityNode->AssociatedInstanceNodeList.end())
			{	
				if(conceptEntityMatchingCurrentQueryEntity->AssociatedInstanceNodeList.begin() != conceptEntityMatchingCurrentQueryEntity->AssociatedInstanceNodeList.end())
				{
					if(*(entityNode->AssociatedInstanceNodeList.begin())->entityName == *(entityNode->conceptEntityMatchingCurrentQueryEntity.begin())->entityName)
					{
						numberOfMatchedNodes = numberOfMatchedNodes-1;
					}
				}
			}
			*/
			
			queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, false, &queryPeviousAnswerNodeTemp, &queryAnswerContextTemp, false, false);

			#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
			//resetQuerySemanticNetworkAlreadyTracedNodes;
			int irrelevant;
			string printEntityNodeString = "";
			traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON, &irrelevant, &printEntityNodeString, false);				
			#endif
			
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
						
						networkEntityNodeWhenSearchedResultsInBestConfidence = conceptEntityMatchingCurrentQueryEntity;
						queryEntityNodeWhenSearchedResultsInBestConfidence = currentQueryEntityNode;
					
						foundAtLeastOneMatch = true;
						
						#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
						#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
						if(*foundAnswer)
						{
							//add first node to trace;
							generateTexualContextEntityStringBackwards(queryAnswerContext, conceptEntityMatchingCurrentQueryEntity);
							#ifdef GIA_QUERY_DEBUG
							cout << "queryAnswerContext = " << *queryAnswerContext << endl;
							#endif
						}
						#endif
						#endif							
						
					}
				}
			}

			if(currentConfidence > bestConfidence)
			{
				bestConfidence = currentConfidence;
				//cout << "bestConfidence = " << bestConfidence << endl;
				if(!detectComparisonVariable || (detectComparisonVariable && (*foundAnswer == false)))
				{
					networkEntityNodeWhenSearchedResultsInBestConfidence = conceptEntityMatchingCurrentQueryEntity;
					queryEntityNodeWhenSearchedResultsInBestConfidence = currentQueryEntityNode;
					
					foundAtLeastOneMatch = true;
				}
			}				

			
		}
	}
	
	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 1" << endl;
	#endif
	
	if(foundAtLeastOneMatch)
	{
		#ifndef GIA_QUERY_PRINT_CONTEXT_EVEN_WHEN_EXACT_ANSWER_FOUND
		if(!detectComparisonVariable || (detectComparisonVariable && !(*foundAnswer)))
		{//now set draw parameters for optimium solution...
		#endif
			//cout << "asf2" << endl;
			//cout << "queryEntityNodeWhenSearchedResultsInBestConfidence->entityName = " << queryEntityNodeWhenSearchedResultsInBestConfidence->entityName << endl; 
			bool foundAnswerTemp = false;
			GIAEntityNode* queryAnswerNodeTemp = NULL;
			GIAEntityNode* queryPeviousAnswerNodeTemp = NULL;
			string queryAnswerContextTemp = "";
			int numberOfMatchedNodes = 0;

			//cout << "asf3" << endl;

			queryAnswerNodeTemp = testEntityNodeForQuery(queryEntityNodeWhenSearchedResultsInBestConfidence, networkEntityNodeWhenSearchedResultsInBestConfidence, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodes, true, queryAnswerPreviousNode, &queryAnswerContextTemp, false, false);

			//cout << "numberOfMatchedNodes = " << numberOfMatchedNodes << endl;

			#ifdef GIA_QUERY_PRINT_CONTEXT_EVEN_WHEN_EXACT_ANSWER_FOUND
			if(!detectComparisonVariable || (detectComparisonVariable && !(*foundAnswer)))
			{//now set draw parameters for optimium solution...
			#endif

				if(!detectComparisonVariable && foundAnswerTemp)
				{
					//cout << "asf1" << endl;
					*foundAnswer = true;
					queryAnswerNode = queryAnswerNodeTemp;
					*queryAnswerPreviousNode = queryPeviousAnswerNodeTemp;
					*queryAnswerContext = queryAnswerContextTemp;
				}
				*confidence = (double)numberOfMatchedNodes;

			#ifdef GIA_QUERY_PRINT_CONTEXT_EVEN_WHEN_EXACT_ANSWER_FOUND
			}
			#endif		

		#ifndef GIA_QUERY_PRINT_CONTEXT_EVEN_WHEN_EXACT_ANSWER_FOUND	
		}
		#endif
	}


		
	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 2" << endl;
	#endif
	return queryAnswerNode;
				
}


bool verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAEntityNode * entityNode, string comparisonVariableNodeName)
{
	bool definitionFound = false;
	
	if(entityNode->entityName == comparisonVariableNodeName)
	{
		definitionFound = true;
	}	
	else
	{
		if(entityNode->entityNodeDefiningThisInstance != NULL)
		{
			if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(entityNode->entityNodeDefiningThisInstance, comparisonVariableNodeName))
			{
				definitionFound = true;
			}		
		}
			
		if(entityNode->EntityNodeDefinitionList.begin() != entityNode->EntityNodeDefinitionList.end())
		{
			for(entityNode->EntityNodeDefinitionListIterator = entityNode->EntityNodeDefinitionList.begin(); entityNode->EntityNodeDefinitionListIterator < entityNode->EntityNodeDefinitionList.end(); entityNode->EntityNodeDefinitionListIterator++)
			{
				if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(*(entityNode->EntityNodeDefinitionListIterator), comparisonVariableNodeName))
				{
					definitionFound = true;
				}
			}				
		}
	}
			
	return definitionFound;
}




GIAEntityNode * testReferencedEntityNodeForNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode * comparisonVariableNode,  bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, bool isSuitableNodeTypeForInexactAnswer, bool isCondition, GIAEntityNode** queryAnswerPreviousNode, GIAEntityNode* sourceEntityNode, bool sourceIsConditionAndHasComparisonVariableAttached, string * queryAnswerContext, string sourceContext, bool thisIsInstanceAndPreviousNodeWasDefinition, bool nonMatchingSourceConditions)
{
	//if(findBestInexactAnswerAndSetDrawParameters)
	//{
		/*
		cout << "queryEntityNode->entityName = " << queryEntityNode->entityName << endl;
		cout << "entityNode->entityName = " << entityNode->entityName << endl;
		cout << "findBestInexactAnswerAndSetDrawParameters = " << findBestInexactAnswerAndSetDrawParameters << endl;
		cout << "entityNode->testedForQueryComparison = " << entityNode->testedForQueryComparison << endl;
		cout << "findBestInexactAnswerAndSetDrawParameters = " << findBestInexactAnswerAndSetDrawParameters << endl;
		cout << "entityNode->isAnswerContextToQuery = " << entityNode->isAnswerContextToQuery << endl;
		*/
	//}
	
	#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
	#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
	if((!findBestInexactAnswerAndSetDrawParameters && !(queryEntityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	#else
	if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	#endif	
	#else
	#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
	if(!(queryEntityNode->testedForQueryComparison))	
	#else
	if(!(entityNode->testedForQueryComparison))	
	#endif
	#endif
	{
		//cout << "IE-2" << endl;

		/*
		cout << "\tqueryEntityNode = " << queryEntityNode->entityName << endl;
		cout << "\tentityNode = " << entityNode->entityName << endl;
		*/
		
		/*	
		#ifdef GIA_QUERY_USE_EXTRA_LONG_CONTEXT_TRACE
		generateTexualContextBackwards(queryAnswerContext, sourceContext, entityNode);	
		#endif
		*/
		
		//cout << "*queryAnswerContext = " << *queryAnswerContext << endl;
		

		bool foundMatch = false;
		#ifdef GIA_QUERY_USE_OLD_REDUNDANT_CODE_SOURCE_IS_CONDITION_AND_HAS_COMPARISON_VARIABLE_ATTACHED
		if(sourceIsConditionAndHasComparisonVariableAttached)
		{
			//cout << "IE-1a" << endl;
			//cout << "HERE" << endl;
			
			if(!(*foundAnswer))
			{//do not overwrite match, if already found match 

				//cout << "queryEntityNode->entityName = " << queryEntityNode->entityName << endl;
				//cout << "entityNode->entityName = " << entityNode->entityName << endl;			
				//this case is required in the case an answer is connected to a condition property or a condition definition (both new additions to the algorithm)
				#ifdef GIA_QUERY_DEBUG
				cout << "sourceIsConditionAndHasComparisonVariableAttached" << endl;
				cout << "\t foundMatch:" << entityNode->entityName << endl;
				#endif
				foundMatch = true;
			}
		}
		else
		{
		#endif
			//cout << "IE-1b" << endl;

			if(queryEntityNode->entityName == entityNode->entityName)	//allow non-equal conditions to be matched during network comparison
			{
				//cout << "IE0" << endl;

				if(detectComparisonVariable)
				{
					if(comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
					{//exact match found [NB if a quantity, the queryEntityNode's entityName will not have the comparisonVariable name (_$qVar) specified, and therefore a matched entity node entityName is required]
						foundMatch = true;
						#ifdef GIA_QUERY_DEBUG
						cout << "detectComparisonVariable && (queryEntityNode->entityName == entityNode->entityName) && (comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)" << endl;					
						cout << "\t foundMatch:" << entityNode->entityName << endl;
						cout << "entityNode->quantityNumberString = " << entityNode->quantityNumberString << endl;
						#endif
					}
					#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
					else if(queryEntityNode->isQuery)				
					{//added support for which query (alternate method of comparison node detection/designation/definition) 
						foundMatch = true;
						#ifdef GIA_QUERY_DEBUG
						cout << "detectComparisonVariable && (queryEntityNode->entityName == entityNode->entityName) && (queryEntityNode->isQuery)" << endl;					
						cout << "\t foundMatch:" << entityNode->entityName << endl;
						#endif					
					}
					#endif
				}
				#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
				else
				{
					//cout << "IE1" << endl;
					if(isSuitableNodeTypeForInexactAnswer)
					{
						//cout << "IE2" << endl;
						
						if(findBestInexactAnswerAndSetDrawParameters)
						{
							//cout << "IE3" << endl;
							foundMatch = true;
							#ifdef GIA_QUERY_DEBUG
							cout << "(queryEntityNode->entityName == entityNode->entityName) && isSuitableNodeTypeForInexactAnswer" << endl;
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
				#endif
				//cout << "dg0" << endl;
			}
			else if(detectComparisonVariable)
			{
				//cout << "IE0b" << endl;
				if(queryEntityNode->entityName == comparisonVariableNode->entityName)	//implied: (queryEntityNode->isQuery)
				{//exact match found
					foundMatch = true;
					#ifdef GIA_QUERY_DEBUG
					cout << "detectComparisonVariable && (queryEntityNode->entityName == comparisonVariableNode->entityName)" << endl;
					cout << "\t foundMatch:" << queryEntityNode->entityName << endl;
					#endif	
					
					#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
					if(queryEntityNode->isWhichQuery)
					{
						/*
						if a 'which' query, then verify that the entityNode is defined by the comparisonVariableNode [ie has a definition corresponding to the comparisonVariableNode]
						eg1 a dog eats the mud. dogs are animals. / which animal eats the mud?	[answer: 'dog' - which is an instance of 'dog' concept node, where the 'dog' concept node is defined by 'animal' 
							NB answer context text = "eat mud is done by dog" ['eat' is the first node traced, and 'dog' is the answer found'. NB sthe reason 'mud' is added to the answer context text, is because it is the actionObject, which is parsed after actionSubject in testEntityNodeForQuery {ie, after answer 'dog' has already been found}] 
								for this example, need to then verify that the answer 'dog' is defined in the primary semantic network as an animal

						*/
						if(!verifyThatAnswerEntityIsDefinedByComparisonVariableNode(entityNode, comparisonVariableNode->entityName))
						{
							foundMatch = false;
						}
						else
						{
							//cout << "verifyThatAnswerEntityIsDefinedByComparisonVariableNode" << endl;
						}
					}
					#endif									
				}
			}
		#ifdef GIA_QUERY_USE_OLD_REDUNDANT_CODE_SOURCE_IS_CONDITION_AND_HAS_COMPARISON_VARIABLE_ATTACHED			
		}
		#endif
				
		if(foundMatch)
		{	
			//cout << "dg1" << endl;
		
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
	
			#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
			if(findBestInexactAnswerAndSetDrawParameters)
			{
				entityNode->isAnswerContextToQuery = true;			 
			}
			else
			{
			#endif
				#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
				queryEntityNode->testedForQueryComparison = true;	
				#else
				entityNode->testedForQueryComparison = true;	//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]		
				#endif			
			#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
			}	
			#endif

			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}
		
		//cout << "dg2" << endl;
		
		string queryAnswerContextTemp = "";
		
		bool nonMatchingConditions = false;
		if((queryEntityNode->entityName != entityNode->entityName) && isCondition)
		{
			nonMatchingConditions = true;
		}

		#ifdef GIA_QUERY_DO_NOT_TRACE_NON_IDENTICAL_CONDITION_TYPES
			#ifndef GIA_QUERY_TRACE_PAST_ANSWER
			if(((queryEntityNode->entityName == entityNode->entityName) || nonMatchingConditions) && !(*foundAnswer) && (!nonMatchingSourceConditions))	//do not continue trace if non-matching source conditions	//only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer
			#else
			if(((queryEntityNode->entityName == entityNode->entityName) || nonMatchingConditions) && !nonMatchingSourceConditions)	//do not continue trace if non-matching source conditions	//allow trace past the answer		
			#endif
		#else	
			#ifndef GIA_QUERY_TRACE_PAST_ANSWER
			if(((queryEntityNode->entityName == entityNode->entityName) || nonMatchingConditions) && !(*foundAnswer))	//only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer
			#else
			if(((queryEntityNode->entityName == entityNode->entityName) || nonMatchingConditions))	//allow trace past the answer		
			#endif
		#endif
		{
			queryAnswerNode = testEntityNodeForQuery(queryEntityNode, entityNode, detectComparisonVariable, comparisonVariableNode, foundAnswer, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, queryAnswerPreviousNode, &queryAnswerContextTemp, thisIsInstanceAndPreviousNodeWasDefinition, nonMatchingConditions);
		}
		
		#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
		if(*foundAnswer)
		{
			//cout << "adding; " << queryAnswerContextTemp << " to backwards trace..." << endl;
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
GIAEntityNode * testEntityNodeForQuery(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, int * numberOfMatchedNodes, bool findBestInexactAnswerAndSetDrawParameters, GIAEntityNode** queryAnswerPreviousNode, string * queryAnswerContext, bool thisIsInstanceAndPreviousNodeWasDefinition, bool nonMatchingConditions)
{
	#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
	if(!nonMatchingConditions)
	{
	#endif
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		#else
		if(!thisIsInstanceAndPreviousNodeWasDefinition)
		{
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}
		#endif
	#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
	}
	#endif
				
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
	
	#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES					
	#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
	if((!findBestInexactAnswerAndSetDrawParameters && !(queryEntityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	#else
	if((!findBestInexactAnswerAndSetDrawParameters && !(entityNode->testedForQueryComparison)) || (findBestInexactAnswerAndSetDrawParameters && !(entityNode->isAnswerContextToQuery)))
	#endif	
	#else
	#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
	if(!(queryEntityNode->testedForQueryComparison))	
	#else
	if(!(entityNode->testedForQueryComparison))	
	#endif
	#endif
	{//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]
	
		#ifdef GIA_QUERY_DEBUG	
		cout << "\t\ttestEntityNodeForQuery:" << endl;
		cout << "\t\tentityNode = " << entityNode->entityName << endl;
		cout << "\t\tnonMatchingConditions = " << (int)nonMatchingConditions << endl;
		#endif
		
		#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
		if(findBestInexactAnswerAndSetDrawParameters)
		{
			//cout << "TEMP: findBestInexactAnswerAndSetDrawParameters: entityNode = " << entityNode->entityName << endl;
			entityNode->isAnswerContextToQuery = true;			 
		}
		else
		{
		#endif
			#ifdef GIA_QUERY_TRACE_FIND_MAXIMUM_NUMBER_OF_MATCHED_NODES
			queryEntityNode->testedForQueryComparison = true;	
			#else
			entityNode->testedForQueryComparison = true;	//CHECK THIS - may not be appropriate to ensure this... [eg if a query has 2 properties of the same name...?]		
			#endif			
		#ifdef GIA_QUERY_SUPPORT_NON_EXACT_QUERIES
		}
		#endif
		
		#ifdef GIA_QUERY_DEBUG
		//cout << "\tfindBestInexactAnswerAndSetDrawParameters = " << findBestInexactAnswerAndSetDrawParameters << endl;
		//cout << "\tqueryEntityNode->entityName = " << queryEntityNode->entityName << endl;
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
			cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;	
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
			cout << "entityNode = " << entityNode->entityName << " (undefined/concept)" << endl;
		}
		#endif
		
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;
		*/
		
		bool foundAnswerTemp = false;
	
		#ifdef GIA_QUERY_USE_OLD_REDUNDANT_CODE_SOURCE_IS_CONDITION_AND_HAS_COMPARISON_VARIABLE_ATTACHED
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
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIter, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);				
					}
	
					for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
					{//DRAW SHOULD NOT BE REQUIRED	
						//cout << "a31" << endl;
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "having ";
						//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIter, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);
					}
					
					//CHECK THIS; need to take into account condition types for match
						
					vector<GIAEntityNode*>::iterator ConditionIter;	
					for(ConditionIter = entityNode->ConditionNodeList.begin(); ConditionIter != entityNode->ConditionNodeList.end(); ConditionIter++) 
					{	
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "";
						//cout << "\t 2C sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIter, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);
					}			
									
					//CHECK THIS; need to take into account condition types for match		
					//go reverse also...
						
					for(ConditionIter = entityNode->IncomingConditionNodeList.begin(); ConditionIter != entityNode->IncomingConditionNodeList.end(); ConditionIter++) 
					{	
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "";
						//cout << "\t 2D sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIter, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);
					}							
				}
			}
		}
		#else
		if(nonMatchingConditions)
		{
			if(detectComparisonVariable)
			{
				if(queryEntityNode->conditionObjectEntity != NULL)
				{	
					//NB entityNode and queryEntityNode are conditions			
					vector<GIAEntityNode*>::iterator entityIter;
					for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
					{
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "being ";
						//cout << "\t 2A sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionObjectEntity, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);				
					}
				
					for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
					{
						//cout << "a31" << endl;
						bool isSuitableNodeTypeForInexactAnswer = false;
						string sourceContext = "having ";
						//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
						queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionObjectEntity, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, true, queryAnswerContext, sourceContext, false, nonMatchingConditions);
					}
				}			
			}		
		}	
		#endif
		
		/*
		#ifdef GIA_QUERY_DO_NOT_TRACE_NON_IDENTICAL_CONDITION_TYPES
		if(!nonMatchingConditions)
		{
		#endif
		*/
				
		vector<GIAEntityNode*>::iterator actionIterQuery;
		vector<GIAEntityNode*>::iterator actionIter;
		for(actionIterQuery = queryEntityNode->IncomingActionNodeList.begin(); actionIterQuery != queryEntityNode->IncomingActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->IncomingActionNodeList.begin(); actionIter != entityNode->IncomingActionNodeList.end(); actionIter++) 
			{	
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;
				#endif
				//cout << "A" << endl;
				string sourceContext = "is ";	//is done to 
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);

				//cout << "AA" << endl;
			}
		}			
		for(actionIterQuery = queryEntityNode->ActionNodeList.begin(); actionIterQuery != queryEntityNode->ActionNodeList.end(); actionIterQuery++) 
		{
			for(actionIter = entityNode->ActionNodeList.begin(); actionIter != entityNode->ActionNodeList.end(); actionIter++) 
			{
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;
				#endif				
				string sourceContext = "";
				//cout << "AAA" << endl;	
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*actionIterQuery, *actionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);				
				//cout << "AAAA" << endl;
			}
		}	

		if(queryEntityNode->actionSubjectEntity != NULL)
		{		
			if(entityNode->actionSubjectEntity != NULL)
			{		
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;
				#endif				
				string sourceContext = "is done by ";


				//cout << "b5" << endl;
				//cout << "entityNode->actionSubjectEntity->testedForQueryComparison = " << entityNode->actionSubjectEntity->testedForQueryComparison << endl;
				//cout << "entityNode->actionSubjectEntity->entityName = " << entityNode->actionSubjectEntity->entityName << endl;
				//cout << "entityNode->actionObjectEntity->entityName = " << entityNode->actionObjectEntity->entityName << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionSubjectEntity, entityNode->actionSubjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);

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


				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->actionObjectEntity, entityNode->actionObjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);

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
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
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
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*ConditionIterQuery, *ConditionIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
			}	
		}

		//cout << "b5z" << endl;
		
		if(queryEntityNode->conditionSubjectEntity != NULL)
		{		
			//cout << "b5a" << endl;
			if(entityNode->conditionSubjectEntity != NULL)
			{	
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;
				#endif

				string sourceContext = "";	
				//cout << "b5" << endl;
				//cout << "entityNode->conditionSubjectEntity->testedForQueryComparison = " << entityNode->conditionSubjectEntity->testedForQueryComparison << endl;
				//cout << "entityNode->conditionSubjectEntity->entityName = " << entityNode->conditionSubjectEntity->entityName << endl;
				//cout << "entityNode->conditionObjectEntity->entityName = " << entityNode->conditionObjectEntity->entityName << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionSubjectEntity, entityNode->conditionSubjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);

				//cout << "b6" << endl;		
			}	
		}			
		if(queryEntityNode->conditionObjectEntity != NULL)
		{		
			//cout << "b7a" << endl;
			//cout << "entityNode->entityName = " << entityNode->entityName << endl;
			if(entityNode->conditionObjectEntity != NULL)
			{	
				string sourceContext = "";	
				//cout << "b7" << endl;
				bool isSuitableNodeTypeForInexactAnswer = true;

				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->conditionObjectEntity, entityNode->conditionObjectEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, true, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);

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
				if((((*entityIter)->hasQuality) && ((*entityIterQuery)->hasQuality)) || (!((*entityIter)->hasQuality) && !((*entityIterQuery)->hasQuality)))
				{//condition added 03 Feb 2012 - only accept matched properties if they are either both qualities, or both not qualities		

					//cout << "a31" << endl;
					#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
					bool isSuitableNodeTypeForInexactAnswer = true;
					#else
					bool isSuitableNodeTypeForInexactAnswer = false;
					#endif				
					string sourceContext = "has ";
					queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
				}
			}		
		}
		//this was removed 25 Sept 2011 - assume using entityNodeContainingThisProperty instead, and was restored on 8 Dec 2011
		//go reverse also...
		for(entityIterQuery = queryEntityNode->PropertyNodeReverseList.begin(); entityIterQuery != queryEntityNode->PropertyNodeReverseList.end(); entityIterQuery++) 
		{	
			for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++) 
			{	
				//cout << "a31" << endl;
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;
				#endif				
				string sourceContext = "possessed by ";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
			}		
		}

		//cout << "a3b" << endl;
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		//go upwards also...	
		if(queryEntityNode->entityNodeDefiningThisInstance != NULL)
		{
			if(entityNode->entityNodeDefiningThisInstance != NULL)
			{
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
				bool isSuitableNodeTypeForInexactAnswer = true;	//???
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;				
				#endif

				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
				string sourceContext = "is ";	
				#else
				string sourceContext = "";
				#endif				
				//cout << "a33" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeDefiningThisInstance, entityNode->entityNodeDefiningThisInstance, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
			}		
		}
		#endif
		/*removed 8 Dec 2011
		//cout << "a3c" << endl;
		if(queryEntityNode->entityNodeContainingThisProperty != NULL)
		{
			if(entityNode->entityNodeContainingThisProperty != NULL)
			{
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "possessed by ";
				//cout << "a34" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(queryEntityNode->entityNodeContainingThisProperty, entityNode->entityNodeContainingThisProperty, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);
			}
		}
		*/
		//cout << "a4" << endl;



		//cout << "a5" << endl;				
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionList.end(); entityIterQuery++) 
		{
			for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
			{
				//cout << "DEBUG: expect here 2" << endl;
				//cout << "*entityIter = " << (*entityIter)->entityName << endl;
				//cout << "*entityIterQuery = " << (*entityIterQuery)->entityName << endl;

				bool isSuitableNodeTypeForInexactAnswer = true;
				string sourceContext = "is ";	//is defined by
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);				

				//cout << "done" << endl;

			}
		}
		//go reverse also...	
		for(entityIterQuery = queryEntityNode->EntityNodeDefinitionReverseList.begin(); entityIterQuery != queryEntityNode->EntityNodeDefinitionReverseList.end(); entityIterQuery++) 
		{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
			for(entityIter = entityNode->EntityNodeDefinitionReverseList.begin(); entityIter != entityNode->EntityNodeDefinitionReverseList.end(); entityIter++) 
			{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
				bool isSuitableNodeTypeForInexactAnswer = false;
				string sourceContext = "defines ";
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, false, nonMatchingConditions);						
			}
		}
		//cout << "a6" << endl;


		//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
		//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]	
		for(entityIterQuery = queryEntityNode->AssociatedInstanceNodeList.begin(); entityIterQuery != queryEntityNode->AssociatedInstanceNodeList.end(); entityIterQuery++) 
		{			
			for(entityIter = entityNode->AssociatedInstanceNodeList.begin(); entityIter != entityNode->AssociatedInstanceNodeList.end(); entityIter++) 
			{				
				#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER
					#ifdef GIA_QUERY_RELEX_REQUIREMENTS_TO_FIND_INEXACT_ANSWER_ALLOW_SINGLE_ENTITY_MATCHES
					bool isSuitableNodeTypeForInexactAnswer = true;
					#else
					bool isSuitableNodeTypeForInexactAnswer = false;					
					#endif
				#else
				bool isSuitableNodeTypeForInexactAnswer = false;				
				#endif
				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
					#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_OLD_TEXTUAL_OUTPUT
					string sourceContext = "is seen in ";	//is realised in/defines/is instantiated in				
					#else
					string sourceContext = "";
					#endif
				#else
				string sourceContext = "";
				#endif
				
				//cout << "DEBUG: expect here 1" << endl;
				
				//cout << "as0" << endl;
				queryAnswerNode = testReferencedEntityNodeForNameMatch(*entityIterQuery, *entityIter, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNode, numberOfMatchedNodes, findBestInexactAnswerAndSetDrawParameters, isSuitableNodeTypeForInexactAnswer, false, queryAnswerPreviousNode, entityNode, false, queryAnswerContext, sourceContext, true, nonMatchingConditions);						
				//cout << "as1" << endl;
			}
		}
		
		/*	
		#ifdef GIA_QUERY_DO_NOT_TRACE_NON_IDENTICAL_CONDITION_TYPES
		}
		#endif			
		*/
		
		if(foundAnswerTemp)
		{
			*foundAnswer = true;
		}
		
		//cout << "a7" << endl;
				
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

	//cout << "HERE2" << endl;
	
	/*
	if(entityNode->isDefinite)
	{//ie definite noun
		entityPretext = entityPretext + "the ";
	}
	*/
	
	if(entityNode->hasQuantity)
	{
		string quantityNumberStringTemp = printQuantityNumberString(entityNode);	
		entityPretext = entityPretext + quantityNumberStringTemp + " ";
	}	
	else if(entityNode->isProperty)
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




double determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListQuery)
{
	double bestConfidence = 0.0;
				
	unordered_map<string, GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		GIAEntityNode* currentQueryEntityNode = entityIterQuery->second;

		#ifdef GIA_QUERY_DEBUG
		cout << "determineMaxConfidenceOfQuerySemanticNetwork" << endl;
		cout << "\currentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
		#endif
		
		int numberOfMatchedNodes = 0;	
		string queryAnswerContextTemp = "";
		traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES, &numberOfMatchedNodes, &queryAnswerContextTemp, false);

		double currentConfidence = (double)numberOfMatchedNodes;	//NB confidence value definition for query network structure matching is currently very simple; it is just equal to the number of matched nodes found 

		if(currentConfidence > bestConfidence)
		{
			bestConfidence = currentConfidence;
		}				
	}
	
	#ifdef GIA_QUERY_USE_ARTIFICIALLY_ADJUSTED_MAX_CONFIDENCE
	return bestConfidence-1.0;
	#else
	return bestConfidence;
	#endif
}


void printEntityNodeQualitiesOnly(GIAEntityNode * entityNode, string * printEntityNodeString)
{	
	int numberQualities = 0;
	//cout << "entityNode->entityName = " << entityNode->entityName << endl;
	for(entityNode->PropertyNodeListIterator = entityNode->PropertyNodeList.begin(); entityNode->PropertyNodeListIterator < entityNode->PropertyNodeList.end(); entityNode->PropertyNodeListIterator++)
	{
		GIAEntityNode * propertyNode = *(entityNode->PropertyNodeListIterator);
		if(propertyNode->hasQuality)
		{
			if(numberQualities > 0)
			{
				*printEntityNodeString = *printEntityNodeString + "\n";	
			}
			else
			{
				*printEntityNodeString = *printEntityNodeString + "(";	
			}		
			numberQualities++;
			
			*printEntityNodeString = *printEntityNodeString + "Type: " + propertyNode->entityName;
		}	
	}
	
	if(numberQualities > 0)
	{
		*printEntityNodeString = *printEntityNodeString + ")\n";	
	}
}

void printEntityTimeConditionNodeOnly(GIAEntityNode * entityNode, string * printEntityNodeString)
{	
	//cout << "entityNode->entityName = " << entityNode->entityName << endl;
	
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		if(entityNode->timeConditionNode != NULL)
		{
			*printEntityNodeString = *printEntityNodeString + "(";	

			*printEntityNodeString = *printEntityNodeString + "Time: " + entityNode->timeConditionNode->conditionName;
			
			*printEntityNodeString = *printEntityNodeString + ")\n";
		}	
	}	

}


void traceEntityNodeDetermineNextCourseOfAction(string * printEntityNodeString, GIAEntityNode * entityNode, string context, int function, int * numberOfMatchedNodes, bool thisIsInstanceAndPreviousNodeWasDefinition)
{
	if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT)
	{
		/*
		#ifdef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
		*printEntityNodeString = *printEntityNodeString + "\nContext: = " + entityNode->entityName;			
		#else
		*/
		*printEntityNodeString = *printEntityNodeString + "\nContext: " + context + " = " + entityNode->entityName;		
		//#endif	
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition);
	}	
	else
	{
		cout << "error: illegal trace entity nodes function" << endl;
		exit(0);
	}

}
void traceEntityNode(GIAEntityNode * entityNode, int function, int * numberOfMatchedNodes, string * printEntityNodeString, bool thisIsInstanceAndPreviousNodeWasDefinition)
{
	bool pass = false;
	if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON)
	{
		if(entityNode->testedForQueryComparison)
		{	
			pass = true;
			entityNode->testedForQueryComparison = false;
		}		
	}
	else
	{
		if(!(entityNode->queryEntityTraced))
		{	
			pass = true;
			entityNode->queryEntityTraced = true;
		}
	}
	
	if(pass)
	{
		/*
		cout << "entityNode being traced = " << entityNode->entityName << endl;
		cout << "*numberOfMatchedNodes = " << *numberOfMatchedNodes << endl;
		*/
		
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		#else
		if(!thisIsInstanceAndPreviousNodeWasDefinition)
		{
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}
		#endif

		//if(!(entityNode->hasAssociatedInstanceIsAction))
		//{
			if(entityNode->ActionNodeList.begin() != entityNode->ActionNodeList.end())
			{
				for(entityNode->ActionNodeListIterator = entityNode->ActionNodeList.begin(); entityNode->ActionNodeListIterator < entityNode->ActionNodeList.end(); entityNode->ActionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->ActionNodeListIterator)), "outgoingAction(s)", function, numberOfMatchedNodes, false);
				}				
			}
			if(entityNode->IncomingActionNodeList.begin() != entityNode->IncomingActionNodeList.end())
			{
				for(entityNode->IncomingActionNodeListIterator = entityNode->IncomingActionNodeList.begin(); entityNode->IncomingActionNodeListIterator < entityNode->IncomingActionNodeList.end(); entityNode->IncomingActionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->IncomingActionNodeListIterator)), "incomingAction(s)", function, numberOfMatchedNodes, false);			
				}				
			}
		//}
		//if(entityNode->hasAssociatedInstanceIsAction)
		//{
			if(entityNode->actionSubjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->actionSubjectEntity, "actionSubjectEntity", function, numberOfMatchedNodes, false);
			}
			if(entityNode->actionObjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->actionObjectEntity, "actionObjectEntity", function, numberOfMatchedNodes, false);
			}
		//}

		//if(!(entityNode->hasAssociatedInstanceIsCondition))	//CHECKTHIS; doesnt support recursive conditions at the moment (conditions of conditions)
		//{
			if(entityNode->ConditionNodeList.begin() != entityNode->ConditionNodeList.end())
			{
				for(entityNode->ConditionNodeListIterator = entityNode->ConditionNodeList.begin(); entityNode->ConditionNodeListIterator < entityNode->ConditionNodeList.end(); entityNode->ConditionNodeListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->ConditionNodeListIterator)), "conditionNode(s)", function, numberOfMatchedNodes, false);
				}				
			}
			if(entityNode->IncomingConditionNodeList.begin() != entityNode->IncomingConditionNodeList.end())
			{
				for(entityNode->IncomingConditionNodeListIterator = entityNode->IncomingConditionNodeList.begin(); entityNode->IncomingConditionNodeListIterator < entityNode->IncomingConditionNodeList.end(); entityNode->IncomingConditionNodeListIterator++)
				{		
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->IncomingConditionNodeListIterator)), "incomingConditionNode(s)", function, numberOfMatchedNodes, false);
				}				
			}
		//}
				
		//if(entityNode->hasAssociatedInstanceIsCondition)
		//{
			if(entityNode->conditionSubjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->conditionSubjectEntity, "conditionSubjectEntity", function, numberOfMatchedNodes, false);
			}
			if(entityNode->conditionObjectEntity != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->conditionObjectEntity, "conditionObjectEntity", function, numberOfMatchedNodes, false);
			}
		//} 
		//if(entityNode->PropertyNodeList.begin() != entityNode->PropertyNodeList.end())
		//{
			for(entityNode->PropertyNodeListIterator = entityNode->PropertyNodeList.begin(); entityNode->PropertyNodeListIterator < entityNode->PropertyNodeList.end(); entityNode->PropertyNodeListIterator++)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->PropertyNodeListIterator)), "propertyNode(s)", function, numberOfMatchedNodes, false);	
			}	
			//added 8 Dec 2011
			for(entityNode->PropertyNodeReverseListIterator = entityNode->PropertyNodeReverseList.begin(); entityNode->PropertyNodeReverseListIterator < entityNode->PropertyNodeReverseList.end(); entityNode->PropertyNodeReverseListIterator++)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->PropertyNodeReverseListIterator)), "propertyNode(s)", function, numberOfMatchedNodes, false);	
			}						
		//}

		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		if(entityNode->entityNodeDefiningThisInstance != NULL)
		{
			traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->entityNodeDefiningThisInstance, "entityNodeDefiningThisInstance", function, numberOfMatchedNodes, false);
		}
		#endif
		/*	//removed 8 Dec 2011
		//if(entityNode->isProperty)
		//{
			if(entityNode->entityNodeContainingThisProperty != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->entityNodeContainingThisProperty, "entityNodeContainingThisProperty (parent)", function, numberOfMatchedNodes, false);
			}	
		//}
		*/
				
		//if(!(entityNode->isProperty))
		//{
			if(entityNode->EntityNodeDefinitionList.begin() != entityNode->EntityNodeDefinitionList.end())
			{
				for(entityNode->EntityNodeDefinitionListIterator = entityNode->EntityNodeDefinitionList.begin(); entityNode->EntityNodeDefinitionListIterator < entityNode->EntityNodeDefinitionList.end(); entityNode->EntityNodeDefinitionListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->EntityNodeDefinitionListIterator)), "incomingEntityNodeDefinition", function, numberOfMatchedNodes, false);	
				}				
			}
			if(entityNode->EntityNodeDefinitionReverseList.begin() != entityNode->EntityNodeDefinitionReverseList.end())
			{
				for(entityNode->EntityNodeDefinitionReverseListIterator = entityNode->EntityNodeDefinitionReverseList.begin(); entityNode->EntityNodeDefinitionReverseListIterator < entityNode->EntityNodeDefinitionReverseList.end(); entityNode->EntityNodeDefinitionReverseListIterator++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->EntityNodeDefinitionReverseListIterator)), "incomingEntityNodeDefinition(s)", function, numberOfMatchedNodes, false);	
				}				
			}
		//}
		
		if(entityNode->AssociatedInstanceNodeList.begin() != entityNode->AssociatedInstanceNodeList.end())
		{	
			for(entityNode->AssociatedInstanceNodeListIterator = entityNode->AssociatedInstanceNodeList.begin(); entityNode->AssociatedInstanceNodeListIterator < entityNode->AssociatedInstanceNodeList.end(); entityNode->AssociatedInstanceNodeListIterator++)
			{			
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityNode->AssociatedInstanceNodeListIterator)), "associatedInstanceNodes(s)", function, numberOfMatchedNodes, true);	
			}
		}
					

		/*
		//if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
		//{
			if(entityNode->timeConditionNode != NULL)
			{
				traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, entityNode->timeConditionNode->conditionName, "timeConditionNode", function, numberOfMatchedNodes, false);
			}
		//}
		*/
	}	

}
	
	
	


