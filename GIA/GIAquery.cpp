/*******************************************************************************
 *
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1f 23-May-2012
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 * ?Limitations: will only locate a exact answer (based upon a comparison node) if it provides the maximum number of matched nodes 
 *
 *******************************************************************************/


#include "GIAquery.h"
#include "GIAdatabase.h"
#ifdef USE_WORDNET
	#include "GIAwordnet.h"
	#include "wn.h"
#endif


GIAQueryTraceParameters::GIAQueryTraceParameters(void)
{
	
}
GIAQueryTraceParameters::~GIAQueryTraceParameters(void)
{
}

GIAQueryTraceParameters::GIAQueryTraceParameters(GIAQueryTraceParameters * queryTraceParametersToCopy)
{
	
	bool detectComparisonVariable = queryTraceParametersToCopy->detectComparisonVariable;				//never changes
	GIAEntityNode * comparisonVariableNode = queryTraceParametersToCopy->comparisonVariableNode;			//never changes
						
	foundAnswer = false;
	queryAnswerNode = NULL;
	queryAnswerContext = "";	
	#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
	int numberAnswersFound = 0;
	/*
	vector<GIAEntityNode*> queryAnswerNodes;
	//vector<string> queryAnswerContexts;
	*/
	#endif
	
	thisIsInstanceAndPreviousNodeWasDefinition = queryTraceParametersToCopy->thisIsInstanceAndPreviousNodeWasDefinition;
	#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	#ifndef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
	nonMatchingSourceConditions = queryTraceParametersToCopy->nonMatchingSourceConditions;
	#endif
	#endif
	
	isCondition = queryTraceParametersToCopy->isCondition;
	sourceEntityNode = queryTraceParametersToCopy->sourceEntityNode;
	sourceContext = queryTraceParametersToCopy->sourceContext;
}



GIAReferenceTraceParameters::GIAReferenceTraceParameters(void)
{
}
GIAReferenceTraceParameters::~GIAReferenceTraceParameters(void)
{
}


GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * numberOfMatchedNodes, string * queryAnswerContext)
{
	bool traceModeIsQuery = TRACE_MODE_IS_QUERY_TRUE;
	
	int numberOfMatchedNodesTempMax = 0;
	int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax;
	bool foundAnAnswer = false;	//this is not really used, and is more of an artefact...
	
	#ifdef GIA_QUERY_DEBUG
	if(detectComparisonVariable)
	{
		cout << "answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork" << endl;
		cout << "detectComparisonVariable" << endl;
		cout << "\tcomparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;
	}
	#endif
	
	GIAEntityNode * networkEntityWithMaxNumberNodesMatched = NULL;
	GIAEntityNode * queryEntityWithMaxNumberNodesMatched = NULL;
	
	bool foundAtLeastOneMatch = false;
		
	unordered_map<string, GIAEntityNode*>::iterator entityIterQuery;
	for(entityIterQuery = conceptEntityNodesListQuery->begin(); entityIterQuery != conceptEntityNodesListQuery->end(); entityIterQuery++) 
	{//for each node in query semantic net;
		
		#ifdef GIA_QUERY_DEBUG
		cout << "Query Trace Start: entityIterQuery->second->entityName = " << entityIterQuery->second->entityName << endl;
		#endif
		GIAEntityNode* currentQueryEntityNode = entityIterQuery->second;
		
		bool foundQueryEntityNodeName = false;
		long queryEntityNodeIndex = -1;
		string queryEntityNodeName = currentQueryEntityNode->entityName;
		//cout << "saf1" << endl;
		
		GIAEntityNode * conceptEntityMatchingCurrentQueryEntity = findOrAddEntityNodeByName(NULL, conceptEntityNodesList, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL, false);
		
		//cout << "saf2" << endl;
		
		if(foundQueryEntityNodeName)
		{
			#ifdef GIA_QUERY_DEBUG
			cout << "foundQueryEntityNodeName" << endl;
			cout << "\tcurrentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
			#endif
				
			//now start matching structure search for all properties of the identical concept node (to current query entity name) in Semantic Network
			
			int numberOfMatchedNodesTemp = 0;	
			int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;			
				
			GIAQueryTraceParameters queryTraceParametersTemp;
			queryTraceParametersTemp.detectComparisonVariable = detectComparisonVariable;
			queryTraceParametersTemp.comparisonVariableNode = comparisonVariableNode;
			queryTraceParametersTemp.foundAnswer = false;
			queryTraceParametersTemp.queryAnswerNode = NULL;
			queryTraceParametersTemp.queryAnswerContext = "";
			#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
			queryTraceParametersTemp.numberAnswersFound = 0;
			#endif
			queryTraceParametersTemp.thisIsInstanceAndPreviousNodeWasDefinition = false;
			#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
			#ifndef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
			queryTraceParametersTemp.nonMatchingSourceConditions = false;
			#endif
			#endif
			queryTraceParametersTemp.isCondition;					//not required for testEntityNodeForQuery
			queryTraceParametersTemp.sourceEntityNode;					//not required for testEntityNodeForQuery
			queryTraceParametersTemp.sourceContext;					//not required for testEntityNodeForQuery
			GIAReferenceTraceParameters referenceTraceParameters;	//irrelevant
					
					
			testEntityNodeForReferenceSet(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, &referenceTraceParameters);
			//queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, false, &queryPreviousAnswerNodeTemp, &queryAnswerContextTemp, false, false);
			//NB entityNode->isAnswerContextToQuery is identical to entityNode->testedForQueryComparison
			
			bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, foundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax);
			//NB are only interested in answers if they give the max or equal to max node matches..
			if(bestAnswerCandidate)
			{	
				foundAtLeastOneMatch = true;	
						
				numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
				numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
				
				networkEntityWithMaxNumberNodesMatched = conceptEntityMatchingCurrentQueryEntity;
				queryEntityWithMaxNumberNodesMatched = currentQueryEntityNode;
									
				if(queryTraceParametersTemp.foundAnswer)
				{//if comparisonVariable is identified within the query, then an exact answer is required... 
					
					foundAnAnswer = true;
					#ifdef GIA_QUERY_DEBUG
					cout << "found answer during query Round 1" << endl;
					#endif
				}				
			}

			//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
			int irrelevant;
			string printEntityNodeString = "";
			#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
			bool traceInstantiations = true;
			#else
			bool traceInstantiations = false;
			#endif				
			traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);		
			traceEntityNode(conceptEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);		
		}
	}
	
	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 1" << endl;
	#endif
	
	if(foundAtLeastOneMatch)
	{

		//cout << "asf2" << endl;
		//cout << "queryEntityWithMaxNumberNodesMatched->entityName = " << queryEntityWithMaxNumberNodesMatched->entityName << endl; 
		int numberOfMatchedNodesTemp = 0;
		int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;
		
		GIAQueryTraceParameters queryTraceParameters;
		queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
		queryTraceParameters.comparisonVariableNode = comparisonVariableNode;
		queryTraceParameters.foundAnswer = false;
		queryTraceParameters.queryAnswerNode = NULL;
		queryTraceParameters.queryAnswerContext = "";			
		#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
		queryTraceParameters.numberAnswersFound = 0;
		#endif	
		queryTraceParameters.thisIsInstanceAndPreviousNodeWasDefinition = false;
		#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
		#ifndef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
		queryTraceParameters.nonMatchingSourceConditions = false;
		#endif
		#endif
		queryTraceParameters.isCondition;					//not required for testEntityNodeForQuery
		queryTraceParameters.sourceEntityNode;					//not required for testEntityNodeForQuery
		queryTraceParameters.sourceContext;					//not required for testEntityNodeForQuery
		GIAReferenceTraceParameters referenceTraceParameters;	//irrelevant

		//cout << "asf3" << endl;

		testEntityNodeForReferenceSet(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);
		//queryAnswerNodeTemp = testEntityNodeForQuery(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, true, &queryAnswerContextTemp, false, false);

		//cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;

		if(detectComparisonVariable)
		{
			if(queryTraceParameters.foundAnswer)
			{
				#ifdef GIA_QUERY_DEBUG
				cout << "found answer during query Round 2" << endl;
				#endif

				queryAnswerNode = queryTraceParameters.queryAnswerNode;
				*queryAnswerContext = queryTraceParameters.queryAnswerContext;
				*foundAnswer = true;
			}
		}

		*numberOfMatchedNodes = (double)numberOfMatchedNodesTemp;
	}

	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 2" << endl;
	#endif
	return queryAnswerNode;
				
}




int testReferencedEntityNodeForExactNameMatch(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, int * numberOfMatchedNodes, bool knownBestMatch, int * numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAQueryTraceParameters * queryTraceParameters, GIAReferenceTraceParameters * referenceTraceParameters)
{
	int result = EXACT_MATCH_OUT_OF_BOUNDS;
	
	if(((entityNode->testedForQueryComparison) || (entityNode->testedForQueryComparisonTemp)) && !((queryEntityNode->testedForQueryComparison) || (queryEntityNode->testedForQueryComparisonTemp)))
	{
		//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if(((queryEntityNode->testedForQueryComparison) || (queryEntityNode->testedForQueryComparisonTemp)) && !((entityNode->testedForQueryComparison) || (entityNode->testedForQueryComparisonTemp)))
	{
		//query entity traced, but KB entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp))	//added '!(entityNode->testedForQueryComparisonTemp)' 18 May 2012
	{	
		bool compareEntityNamesResult = false;
		if(queryEntityNode->entityName == entityNode->entityName)
		{
			compareEntityNamesResult = true;
		}
		#ifdef GIA_USE_SYNONYMN_DETECTION
		else
		{
			if(compareEntityNames(queryEntityNode, entityNode))
			{
				compareEntityNamesResult = true;
				*numberOfMatchedNodesRequiredSynonymnDetection = *numberOfMatchedNodesRequiredSynonymnDetection + 1;
			}
		}
		#endif

		if(traceModeIsQuery)
		{
			//cout << "IE-2" << endl;
			//cout << "\tqueryEntityNode = " << queryEntityNode->entityName << endl;
			//cout << "\tentityNode = " << entityNode->entityName << endl;
			//cout << "*queryAnswerContext = " << *queryAnswerContext << endl;

			bool foundMatch = false;

			if(compareEntityNamesResult)
			{
				//cout << "IE0" << endl;

				if(queryTraceParameters->detectComparisonVariable)
				{
					if(queryTraceParameters->comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
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
				else
				{
					//if not detecting a comparison variable, then not looking for an exact answer...
				}
				//cout << "dg0" << endl;
			}
			else if(queryTraceParameters->detectComparisonVariable)
			{
				//cout << "IE0b" << endl;			
				if(queryEntityNode->entityName == queryTraceParameters->comparisonVariableNode->entityName)
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
							NB answer context text = "eat mud is done by dog" ['eat' is the first node traced, and 'dog' is the answer found'. NB the reason 'mud' is added to the answer context text, is because it is the actionObject, which is parsed after actionSubject in testEntityNodeForQuery {ie, after answer 'dog' has already been found}] 
								for this example, need to then verify that the answer 'dog' is defined in the primary semantic network as an animal

						*/
						if(!verifyThatAnswerEntityIsDefinedByComparisonVariableNode(entityNode, queryTraceParameters->comparisonVariableNode->entityName))
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


			if(foundMatch)
			{	
				//cout << "dg1" << endl;

				if(knownBestMatch)	//condition added 18 May 2012: only trace the best path (max number of nodes)
				{
					entityNode->isAnswerToQuery = true;
				}
				
				#ifndef GIA_QUERY_USE_LONG_CONTEXT_TRACE
				generateTexualContextWithPreviousNodeForwards(&(queryTraceParameters->queryAnswerContext), queryTraceParameters->sourceContext, entityNode, queryTraceParameters->sourceEntityNode);
				#endif				
				
				if(queryTraceParameters->foundAnswer)
				{
					cout << "error: query error 1a: answer already found on this trace branch" << endl;
				}
				queryTraceParameters->foundAnswer = true;
				queryTraceParameters->queryAnswerNode = entityNode;
				
				#ifdef GIA_QUERY_DEBUG
				cout << "foundAnswer:" << entityNode->entityName << endl;
				#endif
				//CHECKTHIS; need to take into account vector of answers (not just a single answer)

				result = EXACT_MATCH_PASS;

				#ifndef GIA_QUERY_TRACE_PAST_ANSWER	//changed to Temp 21 May 2012	
				entityNode->testedForQueryComparisonTemp = true;	
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				#endif				
			}



		#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
			bool nonMatchingConditions = false;
			if(!compareEntityNamesResult && queryTraceParameters->isCondition)		//now supports unimplemented preposition reduction (need to modify compareEntityNames() to support synonomous prepositions), 	OLD: before 21 May 2012: if((queryEntityNode->entityName != entityNode->entityName) && queryTraceParameters->isCondition)
			{
				nonMatchingConditions = true;
			}		
			#ifdef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if(compareEntityNamesResult || nonMatchingConditions || queryTraceParameters->foundAnswer)	//allow trace past the answer		
			#else
			if(compareEntityNamesResult || nonMatchingConditions)	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)  //only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer			
			#endif		
			{
			#else
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if((compareEntityNamesResult || nonMatchingConditions || queryTraceParameters->foundAnswer) && !(queryTraceParameters->nonMatchingSourceConditions))	//do not continue trace if non-matching source conditions	//allow trace past the answer		
			#else
			if((compareEntityNamesResult || nonMatchingConditions) && (!(queryTraceParameters->nonMatchingSourceConditions)))	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)	//do not continue trace if non-matching source conditions	//only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer			
			#endif
			{
				queryTraceParameters->nonMatchingSourceConditions = nonMatchingConditions;
			#endif
		#else
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if(compareEntityNamesResult || queryTraceParameters->foundAnswer)	//allow trace past the answer		
			#else
			if(compareEntityNamesResult)	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)			
			#endif		
			{
		#endif	
				if(testEntityNodeForReferenceSet(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
				{
					result = EXACT_MATCH_PASS;
				}
				else
				{
					result = EXACT_MATCH_FAIL;
				}
			}
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER	//changed to Temp 21 May 2012	
			else
			{
				if(queryTraceParameters->foundAnswer)
				{
					entityNode->testedForQueryComparisonTemp = true;			
					*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				}
			}
			#endif
			
			#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
			if(queryTraceParameters->foundAnswer)
			{
				generateTexualContextBackwards(&(queryTraceParameters->queryAnswerContext), queryTraceParameters->sourceContext, entityNode);
			}	
			#endif

		}
		else
		{
			if((queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))	//only trace paths of same reference set ID
			{
				if(compareEntityNamesResult)
				{
					if(testEntityNodeForReferenceSet(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
					{
						result = EXACT_MATCH_PASS;
					}
					else
					{
						result = EXACT_MATCH_FAIL;
					}
				}
				else
				{
					result = EXACT_MATCH_FAIL;
				}
			}
			else
			{
				 result = EXACT_MATCH_OUT_OF_BOUNDS;
			}
		}
	}
	else
	{
		 result = EXACT_MATCH_OUT_OF_BOUNDS;
	}

	return result;
}

					
					
					
//based on testEntityNodeForQuery
bool testEntityNodeForReferenceSet(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, int * numberOfMatchedNodes, bool knownBestMatch, int * numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAQueryTraceParameters * queryTraceParameters, GIAReferenceTraceParameters * referenceTraceParameters)
{	
	bool exactMatch = true;
	
	bool pass = false;

	if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparisonTemp))
	{
		/* this is set elsewhere;
		if(knownBestMatch)
		{
			queryEntityNode->entityCorrespondingBestMatches.push_back(entityNode);
		}
		*/
		
		entityNode->testedForQueryComparison = true;
		queryEntityNode->testedForQueryComparison = true;

		if(traceModeIsQuery)
		{
			#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
			if(!(queryTraceParameters->nonMatchingSourceConditions))
			{
			#endif
				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				#else
				if(!(queryTraceParameters->thisIsInstanceAndPreviousNodeWasDefinition))
				{
					*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				}
				#endif
			#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
			}
			#endif
		}
		else
		{
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}
		
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG	
		cout << "\t\t testEntityNodeForReferenceSet:" << endl;
		cout << "\t\t entityNode = " << entityNode->entityName << endl;
		#endif
				
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
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
		
		/*NOT FINISHED AND WILL NOT BE IMPLEMENTED - this is only required where there exists a discrepancy between the way in which the query net and the text net have been formed;
		//special: having and being connections to conditions....
		if(traceModeIsQuery)
		{
			if(queryTraceParameters->nonMatchingSourceConditions)
			{
				if(queryTraceParameters->detectComparisonVariable)
				{
					if(queryEntityNode->conditionObjectEntity != NULL)
					{	
						queryTraceParameters->isCondition = false;
						queryTraceParameters->sourceEntityNode = entityNode;
									
						//NB entityNode and queryEntityNode are conditions			
						vector<GIAEntityNode*>::iterator entityIter;
						for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
						{
							queryTraceParameters->sourceContext = "being ";
							//cout << "\t 2A sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*entityIterQuery, *entityIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);
							
							queryTraceParameters->queryAnswerNode = queryTraceParametersTemp
						}

						for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
						{
							//cout << "a31" << endl;
							queryTraceParameters->sourceContext = "having ";
							//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*entityIterQuery, *entityIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);
						}
					}			
				}		
			}
		}
		*/
				
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_INSTANTIATIONS
			if((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE) && (traceModeIsQuery))	//check: do not trace instantinations for queries only
			{
				pass = false;
			}
			#endif
			if(pass)
			{
							
				for(vector<GIAEntityNode*>::iterator entityIterQuery = queryEntityNode->entityVectorConnectionsArray[i].begin(); entityIterQuery != queryEntityNode->entityVectorConnectionsArray[i].end(); entityIterQuery++) 
				{
					bool alreadyFoundAnAnswer = false;
					if(traceModeIsQuery)
					{
						queryTraceParameters->sourceContext = entityVectorConnectionSourceContextArray[i];
						queryTraceParameters->isCondition = entityVectorConnectionIsConditionArray[i];
						queryTraceParameters->sourceEntityNode = entityNode;	
						if(queryTraceParameters->foundAnswer)
						{
							alreadyFoundAnAnswer = true;
						}
						#ifndef GIA_QUERY_TRACE_INSTANTIATIONS
						if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES)	//check: do not trace instantinations for queries only
						{
							queryTraceParameters->thisIsInstanceAndPreviousNodeWasDefinition = true;
						}
						else
						{
							queryTraceParameters->thisIsInstanceAndPreviousNodeWasDefinition = false;
						}
						#endif					
					}

					bool foundExactMatchFail = false;
					bool foundExactMatchPass = false;

					int numberOfMatchedNodesTempMax = 0;
					int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = 0;
					GIAEntityNode * queryEntityCorrespondingBestMatch;
					int numberOfMatchedNodesAtPreviousAnswerNode = 0;
					readVectorConnection(entityNode, i);
					for(vector<GIAEntityNode*>::iterator entityIter = entityNode->entityVectorConnectionsArray[i].end(); entityIter != entityNode->entityVectorConnectionsArray[i].begin(); entityIter--)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected) 
					{
						GIAQueryTraceParameters queryTraceParametersTemp(queryTraceParameters);

						int numberOfMatchedNodesTemp = 0;
						int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

						int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*entityIterQuery, *entityIter, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, referenceTraceParameters);
						if(exactMatchTemp == EXACT_MATCH_PASS)
						{
							foundExactMatchPass = true;
						}
						else if((exactMatchTemp == EXACT_MATCH_FAIL) || (exactMatchTemp == MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS))
						{
							foundExactMatchFail = true;
						}

						if(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)
						{
							bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, alreadyFoundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax);
							if(bestAnswerCandidate)
							{			
								queryEntityCorrespondingBestMatch = *entityIter;
								numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
								numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
							}

							#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS												
							if(traceModeIsQuery)
							{	
								//if(queryTraceParametersTemp.detectComparisonVariable)
								//{ 						
									if(queryTraceParametersTemp.foundAnswer)
									{
										if(alreadyFoundAnAnswer)
										{
											cout << "error: query error 1b: answer already found on alternate trace branch" << endl;							
										}
										else
										{
											if(numberOfMatchedNodesTemp > numberOfMatchedNodesAtPreviousAnswerNode)		//do not record deficient answers [those are known to provide less node matches than the previously recorded answer(s)]
											{
												//current answer provides better match... clear previous answer nodes...
												queryTraceParameters->queryAnswerNodes.clear();
												queryTraceParameters->numberAnswersFound = 0;

											}
											if(numberOfMatchedNodesTemp >= numberOfMatchedNodesAtPreviousAnswerNode)
											{
												for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIter != queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIter++)
												{							
													queryTraceParameters->queryAnswerNodes.push_back(*entityAnswerIter);
													queryTraceParameters->numberAnswersFound = queryTraceParameters->numberAnswersFound + 1;
												}
												numberOfMatchedNodesAtPreviousAnswerNode = numberOfMatchedNodesTemp;	
											}
										}
									}
								//}
							}
							#endif

							//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
							int irrelevantInt;
							string irrelevantString = "";
							#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
							bool traceInstantiations = true;	
							#else
							bool traceInstantiations = false;
							#endif				
							traceEntityNode(*entityIter, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
							traceEntityNode(*entityIterQuery, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
						}
					}

					bool exactMatchFoundTemp = false;
					if(!foundExactMatchFail && foundExactMatchPass)
					{	
						exactMatchFoundTemp = true;	
					}

					bool matchFound = determineMatchParameters(exactMatchFoundTemp, traceModeIsQuery, referenceTraceParameters->traceMode, numberOfMatchedNodesTempMax, &exactMatch);

					if(matchFound)
					{	
						if(knownBestMatch)
						{
							(*entityIterQuery)->entityCorrespondingBestMatch = queryEntityCorrespondingBestMatch;		//this shouldn't be required for queries....
						}
						//now set the matched nodes as already passed (required such that they are not retraced...)	

						int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*entityIterQuery, queryEntityCorrespondingBestMatch, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);			//numberOfMatchedNodesTemp, numberOfMatchedNodesRequiredSynonymnDetectionTemp			
					}
				}
			}
		}

		//cout << "a7" << endl;
				
		//cout << "a8" << endl;	
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
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
		
		entityNode->testedForQueryComparison = false;
		entityNode->testedForQueryComparisonTemp = true;		
		queryEntityNode->testedForQueryComparison = false;
		queryEntityNode->testedForQueryComparisonTemp = true;	
									
	}
	
	
	return exactMatch;
	//cout << "a0c" << endl;
	
		
}

bool determineIfBestAnswerCandidate(bool traceModeIsQuery, int queryTraceParametersTempFoundAnswer, bool alreadyFoundAnAnswer, bool numberOfMatchedNodesTemp, int numberOfMatchedNodesTempMax, int numberOfMatchedNodesRequiredSynonymnDetectionTemp, int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax)
{
	bool bestAnswerCandidate = false;

	if(numberOfMatchedNodesTemp > numberOfMatchedNodesTempMax)
	{//first traces (ie traces of newest instances/properties) take priority
		bestAnswerCandidate = true;
	}
	else if(traceModeIsQuery && !alreadyFoundAnAnswer && queryTraceParametersTempFoundAnswer && (numberOfMatchedNodesTemp == numberOfMatchedNodesTempMax))
	{//answer found, where not previously found
		//traces with answers take priority (if equal number of matched nodes)
		bestAnswerCandidate = true;
	}
	else if((numberOfMatchedNodesRequiredSynonymnDetectionTemp < numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax) && (numberOfMatchedNodesTemp == numberOfMatchedNodesTempMax))
	{
		//traces with least degree of synonymn detection required take priority (if equal number of matched nodes)
		bestAnswerCandidate = true;
	}
					
	return bestAnswerCandidate;
}	

bool determineMatchParameters(bool exactMatchFoundTemp, bool traceIsQuery, int referenceTraceParametersTraceMode, int numberOfMatchedNodesTempMax, bool * exactMatch)
{
	if(!exactMatchFoundTemp)
	{
		*exactMatch = false;	//query was not perfectly tracable at/from this node (queryEntityNode)
	}

	bool matchFound = false;				
	if(!traceIsQuery)
	{		
		if(referenceTraceParametersTraceMode == TRACE_MODE_ONLY_RECORD_EXACT_MATCH)
		{
			if(exactMatchFoundTemp)
			{
				matchFound = true;			
			}
		}
		else if(referenceTraceParametersTraceMode != TRACE_MODE_ONLY_RECORD_EXACT_MATCH)
		{
			if(numberOfMatchedNodesTempMax > 0)
			{	
				matchFound = true;				
			}			
		}
	}
	else
	{
		if(numberOfMatchedNodesTempMax > 0)
		{				
			matchFound = true;
		}
	}
	
	return matchFound;
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
			for(vector<GIAEntityNode*>::iterator entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter < entityNode->EntityNodeDefinitionList.end(); entityIter++)
			{
				if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(*(entityIter), comparisonVariableNodeName))
				{
					definitionFound = true;
				}
			}				
		}
	}
			
	return definitionFound;
}



bool compareEntityNames(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode)
{
	bool entityNamesAreSynonymous = false;
	
	#ifndef USE_WORDNET
	cout << "compareEntityNames() error: requires USE_WORDNET" << endl;
	exit(0);
	#endif
	
	//cout << "\t compareEntityNames: queryEntityNode " << queryEntityNode->entityName << ", with entityNode " << entityNode->entityName << endl; 
		
	if(queryEntityNode->entityName == entityNode->entityName)
	{
		entityNamesAreSynonymous = true;
	}	
	else
	{
		if(queryEntityNode->wordNetPOS != entityNode->wordNetPOS)	//error checking
		{
			/*
			#ifdef GIA_WORDNET_DEBUG
			cout << "compareEntityNames() warning: (queryEntityNode->wordNetPOS != entityNode->wordNetPOS)" << endl;
			cout << "queryEntityNode->wordNetPOS " << queryEntityNode->entityName << " = " << queryEntityNode->wordNetPOS << endl;
			cout << "entityNode->wordNetPOS = " << entityNode->entityName << " = " << entityNode->wordNetPOS << endl;
			#endif
			*/
		}
		else
		{		
			if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(queryEntityNode->entityName), entityNode->wordNetPOS))
			{
				entityNamesAreSynonymous = true;
			}
		}
	}
	
	return entityNamesAreSynonymous;
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
	double maxNumberOfMatchedNodes = 0.0;
				
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
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
		bool traceInstantiations = true;
		#else
		bool traceInstantiations = false;
		#endif
		 
		traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES, &numberOfMatchedNodes, &queryAnswerContextTemp, false, NULL, traceInstantiations);

		if((double)numberOfMatchedNodes > maxNumberOfMatchedNodes)
		{
			maxNumberOfMatchedNodes = numberOfMatchedNodes;
		}				
	}
	
	#ifdef GIA_QUERY_USE_ARTIFICIALLY_ADJUSTED_MAX_CONFIDENCE
	return maxNumberOfMatchedNodes-1.0;
	#else
	return maxNumberOfMatchedNodes;
	#endif
}


void printEntityNodeQualitiesOnly(GIAEntityNode * entityNode, string * printEntityNodeString)
{	
	int numberQualities = 0;
	//cout << "entityNode->entityName = " << entityNode->entityName << endl;
	for(vector<GIAEntityNode*>::iterator entityIter = entityNode->PropertyNodeList.begin(); entityIter < entityNode->PropertyNodeList.end(); entityIter++)
	{
		GIAEntityNode * propertyNode = *(entityIter);
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


void traceEntityNodeDetermineNextCourseOfAction(string * printEntityNodeString, GIAEntityNode * entityNode, string context, int function, int * numberOfMatchedNodes, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations)
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
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition, referenceSetID, traceInstantiations);
	}	
	#ifdef GIA_USE_ADVANCED_REFERENCING
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY)
	{
		if(entityNode->referenceSetID == referenceSetID)
		{
			traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, thisIsInstanceAndPreviousNodeWasDefinition, referenceSetID, traceInstantiations);
		}
	}			
	#endif
	else
	{
		cout << "error: illegal trace entity nodes function" << endl;
		exit(0);
	}

}
void traceEntityNode(GIAEntityNode * entityNode, int function, int * numberOfMatchedNodes, string * printEntityNodeString, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations)
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
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP)
	{
		if(entityNode->testedForQueryComparisonTemp)
		{	
			pass = true;
			entityNode->testedForQueryComparisonTemp = false;
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
		
		if(traceInstantiations)
		{		
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}
		else
		{
			if(!thisIsInstanceAndPreviousNodeWasDefinition)
			{
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
			}
		}

		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			if(traceInstantiations)
			{
				if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
				{
					pass = false;
				}
			}
			if(pass)
			{			
				for(vector<GIAEntityNode*>::iterator entityIter = entityNode->entityVectorConnectionsArray[i].begin(); entityIter != entityNode->entityVectorConnectionsArray[i].end(); entityIter++) 
				{		
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*(entityIter)), entityVectorConnectionContextArray[i], function, numberOfMatchedNodes, entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[i], referenceSetID, traceInstantiations);			
				}
			}
		}
	}	
}







	
	


