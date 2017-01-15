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
 * Project Version: 1q6b 28-October-2012
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
	foundAnswer = false;
	queryAnswerNode = NULL;
	queryAnswerContext = "";
	#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
	numberAnswersFound = 0;
	numberOfMatchedNodesAtPreviousAnswerNode = 0;
	#endif
	thisIsInstanceAndPreviousNodeWasDefinition = false;
	#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
	nonMatchingSourceConditions = false;
	#endif

	#ifdef GIA_QUERY_DEBUG_LEVEL
	level = 0;
	#endif

	/*
	queryTraceParametersTemp.isCondition;					//not required for testEntityNodeForQuery
	queryTraceParametersTemp.sourceEntityNode;				//not required for testEntityNodeForQuery
	queryTraceParametersTemp.sourceContext;					//not required for testEntityNodeForQuery
	*/
}
GIAQueryTraceParameters::~GIAQueryTraceParameters(void)
{
}

GIAQueryTraceParameters::GIAQueryTraceParameters(GIAQueryTraceParameters * queryTraceParametersToCopy)
{

	detectComparisonVariable = queryTraceParametersToCopy->detectComparisonVariable;		//never changes
	comparisonVariableNode = queryTraceParametersToCopy->comparisonVariableNode;			//never changes

	foundAnswer = false;
	queryAnswerNode = NULL;
	queryAnswerContext = "";
	#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
	//numberAnswersFound = 0;				//not used for queryTraceParametersToCopyTemp
	//numberOfMatchedNodesAtPreviousAnswerNode = 0;		//not used for queryTraceParametersToCopyTemp
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

	#ifdef GIA_QUERY_DEBUG_LEVEL
	level = queryTraceParametersToCopy->level;
	#endif

}



GIAReferenceTraceParameters::GIAReferenceTraceParameters(void)
{
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	intrasentenceReference = false;
	#endif
}
GIAReferenceTraceParameters::~GIAReferenceTraceParameters(void)
{
}


GIAEntityNode * answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConceptsQuery, bool detectComparisonVariable, GIAEntityNode* comparisonVariableNode, bool * foundAnswer, GIAEntityNode* queryAnswerNode, double * numberOfMatchedNodes, string * queryAnswerContext)
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

	for(unordered_map<string, GIAEntityNode*>::iterator entityIterQuery = entityNodesActiveListConceptsQuery->begin(); entityIterQuery != entityNodesActiveListConceptsQuery->end(); entityIterQuery++)
	{//for each node in query semantic net;

		#ifdef GIA_QUERY_DEBUG
		cout << "\n\t\t\t**********************Query Trace Start: entityIterQuery->second->entityName = " << entityIterQuery->second->entityName << endl;
		#endif
		GIAEntityNode* currentQueryEntityNode = entityIterQuery->second;

		if(currentQueryEntityNode->entityName != REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)	//added 22 August 2012
		{
			bool foundQueryEntityNodeName = false;
			long queryEntityNodeIndex = -1;
			string queryEntityNodeName = currentQueryEntityNode->entityName;

			GIAEntityNode * conceptEntityMatchingCurrentQueryEntity = findOrAddConceptEntityNodeByName(NULL, entityNodesActiveListConcepts, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL, false);

			if(foundQueryEntityNodeName)
			{
				#ifdef GIA_QUERY_DEBUG
				cout << "foundQueryEntityNodeName" << endl;
				cout << "\tcurrentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
				#endif

				//now start matching structure search for all substances of the identical concept node (to current query entity name) in Semantic Network

				int numberOfMatchedNodesTemp = 0;
				int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

				GIAQueryTraceParameters queryTraceParametersTemp;
				queryTraceParametersTemp.detectComparisonVariable = detectComparisonVariable;
				queryTraceParametersTemp.comparisonVariableNode = comparisonVariableNode;

				GIAReferenceTraceParameters referenceTraceParameters;	//irrelevant

				bool exactMatchIrrelevant = testEntityNodeForQueryOrReferenceSet(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, &referenceTraceParameters);
				//queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, conceptEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, false, &queryPreviousAnswerNodeTemp, &queryAnswerContextTemp, false, false);
				//NB entityNode->isAnswerContextToQuery is identical to entityNode->testedForQueryComparison

				#ifdef GIA_QUERY_DEBUG
				cout << "queryTraceParametersTemp.foundAnswer = " << queryTraceParametersTemp.foundAnswer << endl;
				cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
				#endif

				bool irrelevantBool;
				bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, foundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, irrelevantBool, exactMatchIrrelevant);
				//NB are only interested in answers if they give the max or equal to max node matches.. (otherwise answers are now disgarded; as of GIA1l...)
				if(bestAnswerCandidate)
				{
					foundAtLeastOneMatch = true;

					numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
					numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;

					networkEntityWithMaxNumberNodesMatched = conceptEntityMatchingCurrentQueryEntity;
					queryEntityWithMaxNumberNodesMatched = currentQueryEntityNode;

					#ifdef GIA_QUERY_DEBUG
					cout << "bestAnswerCandidate" << endl;
					#endif

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
				bool traceInstantiations = GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS_VALUE;
				traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
				traceEntityNode(conceptEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
			}
		}
	}

	#ifdef GIA_QUERY_DEBUG
	cout << "finished query round 1" << endl;
	#endif
	#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
	cout << "\n\n\nfinished query round 1" << endl;
	#endif

	if(foundAtLeastOneMatch)
	{
		#ifdef GIA_QUERY_DEBUG
		cout << "\n\n\n queryEntityWithMaxNumberNodesMatched->entityName = " << queryEntityWithMaxNumberNodesMatched->entityName << endl;
		cout << "\n\n\n numberOfMatchedNodesTempMax = " << numberOfMatchedNodesTempMax << endl;
		#endif

		int numberOfMatchedNodesTemp = 0;
		int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

		GIAQueryTraceParameters queryTraceParameters;
		queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
		queryTraceParameters.comparisonVariableNode = comparisonVariableNode;

		GIAReferenceTraceParameters referenceTraceParameters;	//irrelevant

		testEntityNodeForQueryOrReferenceSet(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);
		//queryAnswerNodeTemp = testEntityNodeForQuery(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, true, &queryAnswerContextTemp, false, false);

		#ifdef GIA_QUERY_DEBUG
		cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
		#endif

		/*
		#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
		//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
		int irrelevant;
		string printEntityNodeString = "";
		bool traceInstantiations = GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS_VALUE;
		traceEntityNode(queryEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);		//is this required?	//added 13 July 2012
		traceEntityNode(networkEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);				//added 13 July 2012
		#endif
		*/

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

				#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
				#ifndef GIA_DO_NOT_PRINT_RESULTS
				if(queryTraceParameters.queryAnswerContexts.size() > 1)
				{
					vector<string>::iterator entityAnswerContextsIter = queryTraceParameters.queryAnswerContexts.begin();
					for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParameters.queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters.queryAnswerNodes.end(); entityAnswerIter++)
					{//for each node in answer list;
						cout << "(Multiple) Answer: " << (*entityAnswerIter)->entityName << endl;
						cout << "(Multiple) Answer Context: " << (*entityAnswerContextsIter) << endl;
						entityAnswerContextsIter++;
					}
				}
				#endif
				#endif
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
	string sourceContextCurrent = queryTraceParameters->sourceContext;
	#ifdef GIA_QUERY_DEBUG
	cout << "testReferencedEntityNodeForExactNameMatch: queryEntityNode = " << queryEntityNode->entityName << ", entityNode = " << entityNode->entityName << endl;
	#endif
	/*
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
	*/
	if((entityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparison))
	{
		//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if((entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparisonTemp))
	{
		//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if((queryEntityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparison))
	{
		//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if((queryEntityNode->testedForQueryComparisonTemp) && !(entityNode->testedForQueryComparisonTemp))
	{
		//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		result = MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS;
	}
	else if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp))	//added '!(entityNode->testedForQueryComparisonTemp)' 18 May 2012
	{
		#ifdef GIA_QUERY_DEBUG
		/*DEBUG [check only that never true]:
		if((queryEntityNode->testedForQueryComparison) || (queryEntityNode->testedForQueryComparisonTemp))
		{
			cout << "as2" << endl;
			exit(0);
		}
		*/
		#endif

		bool compareEntityNamesResult = false;
		#ifdef GIA_SUPPORT_ALIASES
		if(compareEntityAliases(queryEntityNode, entityNode))
		#else
		if(queryEntityNode->entityName == entityNode->entityName)
		#endif
		{
			compareEntityNamesResult = true;
		}
		#ifdef GIA_USE_SYNONYMN_DETECTION
		else
		{
			int synonymnDetectionStatus = getSynonymnDetectionStatus();
			if(synonymnDetectionStatus != SYNONYMN_DETECTION_STATUS_OFF)
			{
				if(traceModeIsQuery || (synonymnDetectionStatus == SYNONYMN_DETECTION_STATUS_QUERIES_AND_ADVANCED_REFERENCING))
				{
					if(compareEntitySynonyms(queryEntityNode, entityNode))
					{
						compareEntityNamesResult = true;
						*numberOfMatchedNodesRequiredSynonymnDetection = *numberOfMatchedNodesRequiredSynonymnDetection + 1;
					}
				}
			}
		}
		#endif

		if(traceModeIsQuery)
		{
			#ifdef GIA_QUERY_DEBUG
			//cout << "\tqueryEntityNode = " << queryEntityNode->entityName << endl;
			//cout << "\tentityNode = " << entityNode->entityName << endl;
			//cout << "queryTraceParameters->queryAnswerContext = " << queryTraceParameters->queryAnswerContext << endl;
			#endif
			
			bool foundMatch = false;

			if(compareEntityNamesResult)
			{
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
					#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES		//also declared suitable to support name queries GIA_SUPPORT_ALIASES
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
			}
			else if(queryTraceParameters->detectComparisonVariable)
			{
				/*
				#ifdef GIA_SUPPORT_ALIASES
				if(compareEntityAliases(entityNode, queryTraceParameters->comparisonVariableNode->entityName))
				#else
				*/
				if(queryEntityNode->entityName == queryTraceParameters->comparisonVariableNode->entityName)
				//#endif
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
							#ifdef GIA_QUERY_DEBUG
							//cout << "verifyThatAnswerEntityIsDefinedByComparisonVariableNode" << endl;
							#endif
						}
					}
					#endif
				}
			}

			if(foundMatch)
			{
				if(knownBestMatch)	//condition added 18 May 2012: only trace the best path (max number of nodes)
				{
					entityNode->isAnswerToQuery = true;
				}

				/*artefact:
				#ifndef GIA_QUERY_USE_LONG_CONTEXT_TRACE
				generateTexualContextWithPreviousNodeForwards(&(queryTraceParameters->queryAnswerContext), sourceContextCurrent, entityNode, queryTraceParameters->sourceEntityNode);
				#endif
				*/

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

				#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
				if(!knownBestMatch)
				{
					bool answerAlreadyAddedToMultipleAnswers = false;
					for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
					{
						//if(((*entityAnswerIter)->entityName == entityNode->entityName) && ((*entityAnswerIter)->idInstance == entityNode->idInstance))
						if((*entityAnswerIter)->idActiveList == entityNode->idActiveList)		//modified 14 August 2012
						{
							answerAlreadyAddedToMultipleAnswers = true;
							#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
							cout << "2answerAlreadyAddedToMultipleAnswers: " << entityNode->entityName << endl;
							#endif
						}
					}
					if(!answerAlreadyAddedToMultipleAnswers)
					{
						queryTraceParameters->queryAnswerNodes.push_back(entityNode);
						queryTraceParameters->queryAnswerContexts.push_back("");
						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						for(int level=0; level<queryTraceParameters->level+1; level++)
						{
							cout << "\t";
						}
						cout << "addingAnswer:" << entityNode->entityName << endl;
						#endif
					}
				}
				#endif

				#ifdef GIA_QUERY_DEBUG
				cout << "\nfoundmatch AnumberOfMatchedNodes = " << *numberOfMatchedNodes << "\n" << endl;
				#endif
			}

		#ifdef GIA_QUERY_ALLOW_ANSWER_OF_NON_IDENTICAL_CONDITION_TYPES
			bool nonMatchingConditions = false;
			if(!compareEntityNamesResult && queryTraceParameters->isCondition)		//now supports unimplemented preposition reduction (need to modify compareEntitySynonyms() to support synonomous prepositions), 	OLD: before 21 May 2012: if((queryEntityNode->entityName != entityNode->entityName) && queryTraceParameters->isCondition)
			{
				nonMatchingConditions = true;
			}
			#ifdef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if(compareEntityNamesResult || nonMatchingConditions || foundMatch)	//allow trace past the answer
			#else
			if(compareEntityNamesResult || nonMatchingConditions)	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)  //only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer
			#endif
			{
			#else
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if((compareEntityNamesResult || nonMatchingConditions || foundMatch) && !(queryTraceParameters->nonMatchingSourceConditions))	//do not continue trace if non-matching source conditions	//allow trace past the answer
			#else
			if((compareEntityNamesResult || nonMatchingConditions) && (!(queryTraceParameters->nonMatchingSourceConditions)))	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)	//do not continue trace if non-matching source conditions	//only trace if matching entity names [or if it is a condition; allows for non-matching conditions to be compared], and do not trace past the answer
			#endif
			{
				queryTraceParameters->nonMatchingSourceConditions = nonMatchingConditions;
			#endif
		#else
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER
			if(compareEntityNamesResult || foundMatch)	//allow trace past the answer
			#else
			if(compareEntityNamesResult)	//redundant (implied/assumed true considering query node will not equal entity node if found answer): && !(queryTraceParameters->foundAnswer)
			#endif
			{
		#endif
				#ifdef GIA_QUERY_DEBUG
				//cout << "compareEntityNamesResult, testEntityNodeForQueryOrReferenceSet" << endl;
				#endif
				if(testEntityNodeForQueryOrReferenceSet(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
				{
					result = EXACT_MATCH_PASS;
				}
				else
				{
					result = EXACT_MATCH_FAIL;
				}
			}
			#ifndef GIA_QUERY_TRACE_PAST_NON_IDENTICAL_CONDITION_TYPES
			#ifdef GIA_QUERY_TRACE_PAST_ANSWER	//changed to Temp 21 May 2012
			else
			{
				#ifdef GIA_QUERY_DEBUG
				//cout << "!compareEntityNamesResult" << endl;
				#endif			
				if(foundMatch)
				{
					entityNode->testedForQueryComparisonTemp = true;
					*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				}
			}
			#endif
			#endif

			#ifdef GIA_QUERY_USE_LONG_CONTEXT_TRACE
			if(queryTraceParameters->foundAnswer)
			{
				if(knownBestMatch)	//is this condition required?
				{
					generateTexualContextBackwards(&(queryTraceParameters->queryAnswerContext), sourceContextCurrent, entityNode);
					#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
					cout << "queryTraceParameters->queryAnswerContext = " << queryTraceParameters->queryAnswerContext << endl;
					#endif
					entityNode->queryAnswerContext = true;
				}

				#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
				if(!knownBestMatch)
				{
					/*
					string queryAnswerContextTemp = queryTraceParameters->queryAnswerContexts.back();
					generateTexualContextBackwards(&queryAnswerContextTemp, sourceContextCurrent, entityNode);
					*/
					for(vector<string>::iterator entityAnswerContextsIter = queryTraceParameters->queryAnswerContexts.begin(); entityAnswerContextsIter != queryTraceParameters->queryAnswerContexts.end(); entityAnswerContextsIter++)
					{
						generateTexualContextBackwards(&(*entityAnswerContextsIter), sourceContextCurrent, entityNode);
						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						cout << "queryTraceParameters->queryAnswerContexts = " << (*entityAnswerContextsIter) << endl;
						#endif
					}
				}
				#endif
			}
			#endif

			#ifdef GIA_QUERY_DEBUG
			if(foundMatch)
			{
				cout << "\n\tfoundmatch BnumberOfMatchedNodes = " << *numberOfMatchedNodes << "\n" << endl;
			}
			else
			{
				//cout << "!foundMatch" << endl;
			}
			#endif

		}
		#ifdef GIA_USE_ADVANCED_REFERENCING
		else
		{
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			//cout << "queryEntityNode->referenceSetID = " << queryEntityNode->referenceSetID << endl;
			//cout << "referenceTraceParameters->referenceSetID = " << referenceTraceParameters->referenceSetID << endl;
			/*
			cout << "A1:" << endl;
			cout << "queryEntityNode->referenceSetID = " << queryEntityNode->referenceSetID << endl;
			cout << "referenceTraceParameters->referenceSetID = " << referenceTraceParameters->referenceSetID << endl;
			cout << "(queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) = " << (queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) << endl;
			cout << "(referenceTraceParameters->traceModeAssertSameReferenceSetID) = " << (referenceTraceParameters->traceModeAssertSameReferenceSetID) << endl;
			*/
			#endif
			if((queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))	//only trace paths of same reference set ID
			{
				#ifdef GIA_ADVANCED_REFERENCING_DEBUG
				//cout << "A2: ((queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))" << endl;
				#endif
				
				#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
				if(queryEntityNode->referenceSetID != GIA_REFERENCE_SET_ID_UNDEFINED)		//added 13 July 2012
				{
				#endif					
					#ifndef GIA_ADVANCED_REFERENCING_ORIGINAL
					if(queryEntityNode->idActiveList != entityNode->idActiveList)	//else they are exactly the same [NB with new implementation of GIA_USE_ADVANCED_REFERENCING, it will detect the same nodes as a reference match, so they need to be ignored when this happens]
					{
					#endif						
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						bool passIntrasentenceReferenceRequirements = true;
						if(referenceTraceParameters->intrasentenceReference)
						{
							passIntrasentenceReferenceRequirements = false;
							if(entityNode->entityIndexTemp < queryEntityNode->minimumEntityIndexOfReferenceSet)
							{
								passIntrasentenceReferenceRequirements = true;
							}
						}

						if(passIntrasentenceReferenceRequirements)
						{
						#endif							
							if(compareEntityNamesResult)
							{								
								#ifdef GIA_ADVANCED_REFERENCING_DEBUG
								for(int level=0; level<queryTraceParameters->level+1; level++)
								{
									cout << "\t";
								}
								cout << "compareEntityNamesResult: queryEntityNode->entityName = " << queryEntityNode->entityName << ", entityNode->entityName = " << entityNode->entityName << endl;
								#endif
								
								#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
								if((!(queryEntityNode->isSubstance) && !(entityNode->isSubstance)) || ((queryEntityNode->isSubstanceConcept) && (entityNode->isSubstanceConcept)) || (!(queryEntityNode->isSubstanceConcept) && !(entityNode->isSubstanceConcept)))
								{
								#endif
									if(testEntityNodeForQueryOrReferenceSet(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
									{
										result = EXACT_MATCH_PASS;

										#ifdef GIA_ADVANCED_REFERENCING_DEBUG
										/*
										#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
										if(referenceTraceParameters->intrasentenceReference)
										{
											cout << "EXACT_MATCH_PASS" << endl;
										}
										#endif
										*/
										#endif
									}
									else
									{
										result = EXACT_MATCH_FAIL;
									}
								#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
								}
								#endif
							}
							else
							{
								//cout << "!compareEntityNamesResult" << endl;
								result = EXACT_MATCH_FAIL;
							}
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
						}
						else
						{
							result = EXACT_MATCH_FAIL;	//CHECKTHIS
						}
						#endif
					#ifndef GIA_ADVANCED_REFERENCING_ORIGINAL
					}
					else
					{
						result = EXACT_MATCH_FAIL;	//CHECKTHIS
					}
					#endif
				#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
				}
				else
				{
					#ifdef GIA_ADVANCED_REFERENCING_DEBUG
					//cout << "(queryEntityNode->referenceSetID == GIA_REFERENCE_SET_ID_UNDEFINED)" << endl;
					#endif
					result = EXACT_MATCH_OUT_OF_BOUNDS;	//CHECKTHIS
				}
				#endif
			}
			else
			{
				#ifdef GIA_ADVANCED_REFERENCING_DEBUG
				//cout << "EXACT_MATCH_OUT_OF_BOUNDS1" << endl;
				#endif
				result = EXACT_MATCH_OUT_OF_BOUNDS;
			}
		}
		#endif
	}
	else
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		//cout << "EXACT_MATCH_OUT_OF_BOUNDS2" << endl;
		#endif
		result = EXACT_MATCH_OUT_OF_BOUNDS;
	}

	return result;
}





bool testEntityNodeForQueryOrReferenceSet(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode, int * numberOfMatchedNodes, bool knownBestMatch, int * numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAQueryTraceParameters * queryTraceParameters, GIAReferenceTraceParameters * referenceTraceParameters)
{
	bool exactMatch = true;

	bool pass = false;

	#ifdef GIA_QUERY_DEBUG
	//cout << "testEntityNodeForQueryOrReferenceSet" << endl;
	/*
	cout << "entityNode->testedForQueryComparison = " << entityNode->testedForQueryComparison << endl;
	cout << "entityNode->testedForQueryComparisonTemp = " << entityNode->testedForQueryComparisonTemp << endl;
	cout << "queryEntityNode->testedForQueryComparison = " << queryEntityNode->testedForQueryComparison << endl;
	cout << "queryEntityNode->testedForQueryComparisonTemp = " << queryEntityNode->testedForQueryComparisonTemp << endl;	
	*/
	#endif

	
	if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparisonTemp))
	{
		#ifdef GIA_QUERY_DEBUG_LEVEL
		queryTraceParameters->level = queryTraceParameters->level+1;
		int currentLevel = queryTraceParameters->level;
		#endif

		/* this is set elsewhere;
		if(knownBestMatch)
		{
			queryEntityNode->entityCorrespondingBestMatches.push_back(entityNode);
		}
		*/

		entityNode->testedForQueryComparison = true;
		queryEntityNode->testedForQueryComparison = true;
		#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
		entityNode->testedForQueryComparisonTemp = false;		//added 13 July 2012 [Should not be required - redundant]
		queryEntityNode->testedForQueryComparisonTemp = false;		//added 13 July 2012 [Should not be required - redundant]
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_TOO_LARGE_REFERENCE_SET
		if(queryTraceParameters->level < 10)
		{
			for(int level=0; level<queryTraceParameters->level+1; level++)
			{
				cout << "\t";
			}
			cout << queryEntityNode->entityName << ", entityNode->entityName = " << entityNode->entityName << endl;
		}
		#endif


		if(traceModeIsQuery)
		{
			#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
			if(!(queryTraceParameters->nonMatchingSourceConditions))
			{
			#endif
				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES
				if(GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS_VALUE)
				{
				#endif
					*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES
				}
				else
				{
					if(!(queryTraceParameters->thisIsInstanceAndPreviousNodeWasDefinition))
					{
						*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
					}
				}
				#endif
			#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
			}
			#endif
		}
		else
		{
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			for(int level=0; level<currentLevel; level++)
			{
				cout << "\t";
			}
			cout << "numberOfMatchedNodes = " << *numberOfMatchedNodes << ", entityNode->entityName = " << entityNode->entityName << endl;
			#endif
		}

		#ifdef GIA_QUERY_DEBUG
		cout << "\ntestEntityNodeForQueryOrReferenceSet:" << endl;
		cout << "entityNode = " << entityNode->entityName << endl;
		#endif		
		
		#ifdef GIA_QUERY_DEBUG
		//cout << "\tqueryEntityNode->entityName = " << queryEntityNode->entityName << endl;
		if(entityNode->isConcept)
		{
			cout << "entityNode = " << entityNode->entityName << " (is concept)" << endl;
		}		
		if(entityNode->isSubstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (is substance)" << endl;
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
			cout << "entityNode = " << entityNode->entityName << " (has associated instance is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance is condition)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << " (undefined/concept)" << endl;
		}
		cout << "numberOfMatchedNodes = " << *numberOfMatchedNodes << endl;
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isSubstance = " << entityNode->isSubstance << endl;
		cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;
		*/		
		#endif


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
						vector<GIAEntityNode*>::iterator connectionIter;
						for(connectionIter = entityNode->EntityNodeDefinitionList.begin(); connectionIter != entityNode->EntityNodeDefinitionList.end(); connectionIter++)
						{
							queryTraceParameters->sourceContext = "being ";
							//cout << "\t 2A sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*connectionIterQuery, *connectionIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);

							queryTraceParameters->queryAnswerNode = queryTraceParametersTemp
						}

						for(connectionIter = entityNode->PropertyNodeList.begin(); connectionIter != entityNode->PropertyNodeList.end(); connectionIter++)
						{
							//cout << "a31" << endl;
							queryTraceParameters->sourceContext = "having ";
							//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*connectionIterQuery, *connectionIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);
						}
					}
				}
			}
		}
		*/


		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS
			if((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE) && (traceModeIsQuery))	//check: do not trace instantinations for queries only
			{
				pass = false;
			}
			#endif
			if(pass)
			{
				#ifdef GIA_QUERY_DEBUG
				//cout << "i = " << i << endl;
				#endif
				for(vector<GIAEntityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[i].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[i].end(); connectionIterQuery++)
				{
					#ifdef GIA_QUERY_DEBUG
					//cout << "connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif
										
					bool alreadyFoundAnAnswer = false;
					if(traceModeIsQuery)
					{
						queryTraceParameters->sourceContext = entityVectorConnectionSourceContextArray[i];
						#ifdef GIA_QUERY_DEBUG
						/*
						if(knownBestMatch)
						{
							cout << "********* \t\t i=  " << i << ", queryTraceParameters->sourceContext = " << queryTraceParameters->sourceContext << endl;
						}
						*/
						#endif

						queryTraceParameters->isCondition = entityVectorConnectionIsConditionArray[i];
						queryTraceParameters->sourceEntityNode = entityNode;
						if(queryTraceParameters->foundAnswer)
						{
							alreadyFoundAnAnswer = true;
						}
						#ifndef GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS
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

					#ifdef GIA_USE_DATABASE
					if(getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_ACTIVE)
					{
						#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
						cout << "GIAquery; entityNode->isConcept = " << entityNode->isConcept << endl;
						cout << "GIAquery; entityNode->isSubstance = " << entityNode->isSubstance << endl;
						#endif
						DBreadVectorConnections(entityNode, i);		//this is important, as it will read all of the vector connections from the database for this node (conferred 25 May 2012)
					}
					#endif

					#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
					for(int level=0; level<currentLevel; level++)
					{
						cout << "\t";
					}
					cout << "A. SDGG connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif

					//for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
					for(vector<GIAEntityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[i].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[i].rend(); connectionIter++)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected)
					{
						#ifdef GIA_QUERY_DEBUG
						//cout << "connectionIter = " << (*connectionIter)->entity->entityName << endl;
						#endif
											
						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						for(int level=0; level<currentLevel; level++)
						{
							cout << "\t";
						}
						cout << "A2. SDGG connectionIter = " << (*connectionIter)->entity->entityName << endl;
						#endif
						#ifdef GIA_ADVANCED_REFERENCING_DEBUG
						cout << "" << endl;
						for(int level=0; level<currentLevel; level++)
						{
							cout << "\t";
						}
						cout << "tracing: (*connectionIterQuery) = " << (*connectionIterQuery)->entity->entityName << ", (*connectionIter) = " << (*connectionIter)->entity->entityName << ", i = " << i <<  endl;
						#endif
						
						GIAQueryTraceParameters queryTraceParametersTemp(queryTraceParameters);

						int numberOfMatchedNodesTemp = 0;
						int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

						int exactMatchTemp = testReferencedEntityNodeForExactNameMatch((*connectionIterQuery)->entity, (*connectionIter)->entity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, referenceTraceParameters);
						#ifdef GIA_QUERY_DEBUG
						//cout << "exactMatchTemp = " << exactMatchTemp << endl;
						#endif
						if(exactMatchTemp == EXACT_MATCH_PASS)
						{
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG
							/*
							#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
							if(referenceTraceParameters->intrasentenceReference)
							{
								cout << "foundExactMatchPass" << endl;
							}
							#endif
							*/
							#endif
							#ifndef GIA_USE_ADVANCED_REFERENCING_UPDAT
							foundExactMatchPass = true;
							#endif
						}
						else if(exactMatchTemp == EXACT_MATCH_FAIL)
						{
							#ifdef GIA_QUERY_DEBUG
							//cout << "foundExactMatchFail" << endl;
							#endif
							foundExactMatchFail = true;
						}

						#ifdef GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS
						if(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)
						{
						#endif
							#ifdef GIA_QUERY_DEBUG
							//cout << "(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)" << endl;
							#endif
							
							bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, alreadyFoundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, referenceTraceParameters->traceMode, exactMatchTemp);
							if(bestAnswerCandidate)
							{
								#ifdef GIA_ADVANCED_REFERENCING_UPDATE_NOT_NECESSARY_OR_TESTED
								foundExactMatchPass = true;
								#endif

								#ifdef GIA_ADVANCED_REFERENCING_DEBUG
								/*
								#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
								if(referenceTraceParameters->intrasentenceReference)
								{
									cout << "\n\nbestAnswerCandidate" << endl;
								}
								#endif
								*/
								#endif

								#ifdef GIA_ADVANCED_REFERENCING_DEBUG
								for(int level=0; level<currentLevel; level++)
								{
									cout << "\t";
								}
								cout << "bestAnswerCandidate: (*connectionIter)->entity->entityName = " << (*connectionIter)->entity->entityName << endl;
								#endif
								queryEntityCorrespondingBestMatch = (*connectionIter)->entity;
								numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
								numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
							}
							else
							{
								#ifdef GIA_ADVANCED_REFERENCING_DEBUG
								for(int level=0; level<currentLevel; level++)
								{
									cout << "\t";
								}
								cout << "nobestAnswerCandidate: (*connectionIter) = " << (*connectionIter)->entity->entityName << ", exactMatchTemp = " << exactMatchTemp << ", numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
								#endif
							}

							#ifdef GIA_QUERY_SUPPORT_MULTIPLE_ANSWERS
							if(knownBestMatch)
							{//only are concerned about recording multiple answers for known best match...
								if(traceModeIsQuery)
								{
									//if(queryTraceParametersTemp.detectComparisonVariable)
									//{
										if(queryTraceParametersTemp.foundAnswer)
										{
											#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
											for(int level=0; level<currentLevel; level++)
											{
												cout << "\t";
											}
											cout << "queryTraceParametersTemp.foundAnswer: answer = " << queryTraceParametersTemp.queryAnswerNode->entityName << endl;
											#endif

											/*
											#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
											for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIter != queryTraceParametersTemp.queryAnswerNodes.end(); entityAnswerIter++)
											{
												for(int level=0; level<currentLevel; level++)
												{
													cout << "\t";
												}
												cout << "Multiple Answer Found:" << (*entityAnswerIter)->entityName << endl;
											}
											#endif
											*/

											if(alreadyFoundAnAnswer)
											{
												cout << "error: query error 1b: answer already found on alternate trace branch" << endl;
											}
											else
											{
												#ifdef GIA_QUERY_DEBUG
												//cout << "!alreadyFoundAnAnswer" << endl;
												#endif
												if(numberOfMatchedNodesTemp > queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)		//do not record deficient answers [those are known to provide less node matches than the previously recorded answer(s)]
												{
													#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
													vector<string>::iterator entityAnswerContextsIter = queryTraceParameters->queryAnswerContexts.begin();
													for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
													{
														for(int level=0; level<currentLevel; level++)
														{
															cout << "\t";
														}
														cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << ", numberOfMatchedNodesAtPreviousAnswerNode = " << queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode << endl;

														for(int level=0; level<currentLevel; level++)
														{
															cout << "\t";
														}
														cout << "Multiple Answer Cleared:" << (*entityAnswerIter)->entityName << endl;
														for(int level=0; level<currentLevel; level++)
														{
															cout << "\t";
														}
														cout << "Multiple Answer Context Cleared:" << (*entityAnswerContextsIter) << endl;
														entityAnswerContextsIter++;
													}
													#endif

													#ifdef GIA_QUERY_DEBUG
													//cout << "(numberOfMatchedNodesTemp > queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)" << endl;
													#endif
													//current answer provides better match... clear previous answer nodes...
													queryTraceParameters->queryAnswerNodes.clear();
													queryTraceParameters->queryAnswerContexts.clear();
													queryTraceParameters->numberAnswersFound = 0;
												}

												if(numberOfMatchedNodesTemp >= queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)
												{
													#ifdef GIA_QUERY_DEBUG
													//cout << "\t(numberOfMatchedNodesTemp >= queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)" << endl;
													#endif
													vector<string>::iterator entityAnswerContextsIterTemp = queryTraceParametersTemp.queryAnswerContexts.begin();
													for(vector<GIAEntityNode*>::iterator entityAnswerIterTemp = queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIterTemp != queryTraceParametersTemp.queryAnswerNodes.end(); entityAnswerIterTemp++)
													{
														bool answerAlreadyAddedToMultipleAnswers = false;
														vector<string>::iterator entityAnswerContextsIter = queryTraceParameters->queryAnswerContexts.begin();
														for(vector<GIAEntityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
														{
															//if(((*entityAnswerIter)->entityName == (*entityAnswerIterTemp)->entityName) && ((*entityAnswerIter)->idInstance == (*entityAnswerIterTemp)->idInstance))	//((*entityAnswerContextsIter) == (*entityAnswerContextsIterTemp)) ?
															if((*entityAnswerIter)->idActiveList == (*entityAnswerIterTemp)->idActiveList)	//modified 14 August 2012
															{
																answerAlreadyAddedToMultipleAnswers = true;
																#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
																cout << "answerAlreadyAddedToMultipleAnswers: " << (*entityAnswerIterTemp)->entityName << endl;
																cout << "answerAlreadyAddedToMultipleAnswers context: " << (*entityAnswerContextsIterTemp) << endl;
																#endif
															}
															/*
															#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
															else if((*entityAnswerIter)->entityName == (*entityAnswerIterTemp)->entityName)
															{
																cout << "((*entityAnswerIter)->entityName == (*entityAnswerIterTemp)->entityName)" << endl;
															}
															#endif
															*/
															entityAnswerContextsIter++;
														}
														if(!answerAlreadyAddedToMultipleAnswers)
														{
															queryTraceParameters->queryAnswerNodes.push_back(*entityAnswerIterTemp);
															queryTraceParameters->queryAnswerContexts.push_back(*entityAnswerContextsIterTemp);
															queryTraceParameters->numberAnswersFound = queryTraceParameters->numberAnswersFound + 1;

															#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
															for(int level=0; level<currentLevel; level++)
															{
																cout << "\t";
															}
															cout << "Multiple Answer Found:" << (*entityAnswerIterTemp)->entityName << endl;
															for(int level=0; level<currentLevel; level++)
															{
																cout << "\t";
															}
															cout << "Multiple Answer Context Found:" << (*entityAnswerContextsIterTemp) << endl;
															#endif
														}
														entityAnswerContextsIterTemp++;
													}
													queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode = numberOfMatchedNodesTemp;
												}
											}
										}
									//}
								}
							}
							#endif
							#ifdef GIA_QUERY_DEBUG
							//cout << "finish: (exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)" << endl;
							#endif
						#ifdef GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS
						}
						else
						{
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG
							for(int level=0; level<currentLevel; level++)
							{
								cout << "\t";
							}
							cout << "MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS: (*connectionIter)->entity->entityName = " << (*connectionIter)->entity->entityName << endl;
							#endif
						}
						#endif
						//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
						int irrelevantInt;
						string irrelevantString = "";
						bool traceInstantiations = GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS_VALUE;		//clear all (why is this still required if GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS is off? - it is based on testing, but unknown as to why)
						traceEntityNode((*connectionIter)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
						traceEntityNode((*connectionIterQuery)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);

						#ifdef GIA_QUERY_DEBUG
						//cout << "finished: connectionIter = " << (*connectionIter)->entity->entityName << endl;
						#endif
					}

					#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
					for(int level=0; level<currentLevel; level++)
					{
						cout << "\t";
					}
					cout << "B. SDGG connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif
					
					bool exactMatchFoundTemp = false;
					#ifdef GIA_ADVANCED_REFERENCING_UPDATE_NOT_NECESSARY_OR_TESTED
					if(foundExactMatchPass)
					{
						exactMatchFoundTemp = true;
					}
					#else
					if(!foundExactMatchFail || foundExactMatchPass)		//OLD before 1 June 2012: (!foundExactMatchFail && foundExactMatchPass)
					{
						exactMatchFoundTemp = true;
					}
					#endif

					#ifdef GIA_ADVANCED_REFERENCING_DEBUG
					/*
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					if(referenceTraceParameters->intrasentenceReference)
					{
						if(!foundExactMatchFail)
						{
							cout << "\n\n !foundExactMatchFail" << endl;
						}
					}
					#endif
					*/
					#endif

					bool matchFound = determineMatchParameters(exactMatchFoundTemp, traceModeIsQuery, referenceTraceParameters->traceMode, numberOfMatchedNodesTempMax, &exactMatch);

					#ifdef GIA_ADVANCED_REFERENCING_DEBUG
					/*
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					if(referenceTraceParameters->intrasentenceReference)
					{
						if(exactMatch)
						{
							cout << "\n\n exactMatch" << endl;
						}
						if(exactMatchFoundTemp)
						{
							cout << "\n\n exactMatchFoundTemp" << endl;
						}
					}
					#endif
					*/
					#endif

					if(matchFound)
					{
						#ifdef GIA_ADVANCED_REFERENCING_DEBUG
						for(int level=0; level<currentLevel; level++)
						{
							cout << "\t";
						}
						cout << "matchFound" << endl;
						#endif

						#ifdef GIA_QUERY_DEBUG
						//cout << "matchFound i = " << i << endl;
						//cout << "matchFound" << endl;
						//cout << "numberOfMatchedNodesTempMax = " << numberOfMatchedNodesTempMax << endl;
						#endif
						
						#ifdef GIA_USE_ADVANCED_REFERENCING
						if(knownBestMatch)
						{
							//cout << "knownBestMatch" << endl;
							(*connectionIterQuery)->entity->entityCorrespondingBestMatch = queryEntityCorrespondingBestMatch;		//this shouldn't be required for queries....
						}
						#endif
						//now set the matched nodes as already passed (required such that they are not retraced...)

						int numberOfMatchedNodesTemp = *numberOfMatchedNodes;
						int numberOfMatchedNodesRequiredSynonymnDetectionTemp = *numberOfMatchedNodesRequiredSynonymnDetection;

						int exactMatchTemp = testReferencedEntityNodeForExactNameMatch((*connectionIterQuery)->entity, queryEntityCorrespondingBestMatch, &numberOfMatchedNodesTemp, knownBestMatch, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);			//numberOfMatchedNodesTemp, numberOfMatchedNodesRequiredSynonymnDetectionTemp

						*numberOfMatchedNodes = numberOfMatchedNodesTemp;
						*numberOfMatchedNodesRequiredSynonymnDetection = numberOfMatchedNodesRequiredSynonymnDetectionTemp;

						#ifdef GIA_QUERY_DEBUG
						//cout << "finished: matchFound" << endl;
						#endif
						
						#ifdef GIA_ADVANCED_REFERENCING_DEBUG
						for(int level=0; level<currentLevel; level++)
						{
							cout << "\t";
						}
						cout << "numberOfMatchedNodes = " << *numberOfMatchedNodes << endl;
						#endif
					}
					else
					{
						#ifdef GIA_ADVANCED_REFERENCING_DEBUG
						for(int level=0; level<currentLevel; level++)
						{
							cout << "\t";
						}
						cout << "nomatchFound" << endl;
						#endif
					}
					
					#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
					for(int level=0; level<currentLevel; level++)
					{
						cout << "\t";
					}
					cout << "C. SDGG connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif
					
					#ifdef GIA_QUERY_DEBUG
					//cout << "finished: connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif					
				}
				#ifdef GIA_QUERY_DEBUG
				//cout << "finished: i = " << i << endl;
				#endif				
			}
		}

		#ifdef GIA_SUPPORT_NAMES_IMPLEMENTATION_2_NOT_USED
		//direct pass through for equality links - added 13 August 2012
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES; i++)
		{
			for(vector<GIAEntityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[entityVectorConnectionEqualitiesArray[i]].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[entityVectorConnectionEqualitiesArray[i]].end(); connectionIterQuery++)
			{
				if(entityNode->isSubstance && (*connectionIter)->entity->isSubstance)
				{
					//equality link found - enable pass through
					testEntityNodeForQueryOrReferenceSet((*connectionIter)->entity, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

				}

			}
			for(vector<GIAEntityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[i].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[i].rend(); connectionIter++)	//reverse iterator not required here
			{
				if(entityNode->isSubstance && (*connectionIter)->entity->isSubstance)
				{
					//equality link found - enable pass through
					testEntityNodeForQueryOrReferenceSet(queryEntityNode, (*connectionIter)->entity, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);
				}
			}
		}
		#endif

		#ifdef GIA_QUERY_DEBUG
		if(entityNode->isSubstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is substance)" << endl;
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
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance is condition)" << endl;
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
		queryEntityNode->testedForQueryComparison = false;
		entityNode->testedForQueryComparisonTemp = true;
		queryEntityNode->testedForQueryComparisonTemp = true;

	}
	#ifdef GIA_QUERY_DEBUG
	/*
	else
	{
		cout << "DEBUG: already set" << endl;
		cout << "entityNode->testedForQueryComparison = " << entityNode->testedForQueryComparison << endl;
		cout << "queryEntityNode->testedForQueryComparison = " << queryEntityNode->testedForQueryComparison << endl;
		cout << "entityNode->testedForQueryComparisonTemp = " << entityNode->testedForQueryComparisonTemp << endl;
		cout << "queryEntityNode->testedForQueryComparisonTemp = " << queryEntityNode->testedForQueryComparisonTemp << endl;
	}
	*/
	#endif

	#ifdef GIA_QUERY_DEBUG
	//cout << "finished: testEntityNodeForQueryOrReferenceSet" << endl;
	#endif
	return exactMatch;


}

bool determineIfBestAnswerCandidate(bool traceModeIsQuery, bool queryTraceParametersTempFoundAnswer, bool alreadyFoundAnAnswer, int numberOfMatchedNodesTemp, int numberOfMatchedNodesTempMax, int numberOfMatchedNodesRequiredSynonymnDetectionTemp, int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, int referenceTraceParametersTraceMode, int exactMatchTemp)
{
	bool bestAnswerCandidate = false;

	if(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)
	{
		if(traceModeIsQuery)
		{
			#ifdef GIA_QUERY_DEBUG
			//cout << "determineIfBestAnswerCandidate: numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
			//cout << "determineIfBestAnswerCandidate: numberOfMatchedNodesTempMax = " << numberOfMatchedNodesTempMax << endl;
			#endif
			
			if(numberOfMatchedNodesTemp > numberOfMatchedNodesTempMax)
			{//first traces (ie traces of newest instances/substances) take priority
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
		}
		else
		{
			bool passExactMatchRequirements = false;
			if(referenceTraceParametersTraceMode == TRACE_MODE_ONLY_RECORD_EXACT_MATCH)
			{
				if(exactMatchTemp == EXACT_MATCH_PASS)
				{
					passExactMatchRequirements = true;
				}
			}
			else if(referenceTraceParametersTraceMode != TRACE_MODE_ONLY_RECORD_EXACT_MATCH)
			{
				passExactMatchRequirements = true;
			}
			if(passExactMatchRequirements)
			{
				if(numberOfMatchedNodesTemp > numberOfMatchedNodesTempMax)
				{//first traces (ie traces of newest instances/substances) take priority
					bestAnswerCandidate = true;
				}
				else if((numberOfMatchedNodesRequiredSynonymnDetectionTemp < numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax) && (numberOfMatchedNodesTemp == numberOfMatchedNodesTempMax))
				{
					//traces with least degree of synonymn detection required take priority (if equal number of matched nodes)
					bestAnswerCandidate = true;
				}
			}
		}
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
				if(numberOfMatchedNodesTempMax > 0)
				{
					matchFound = true;
				}
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
		if(!(entityNode->entityNodeDefiningThisInstance->empty()))
		{
			if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode((entityNode->entityNodeDefiningThisInstance->back())->entity, comparisonVariableNodeName))
			{
				definitionFound = true;
			}
		}

		if(!(entityNode->EntityNodeDefinitionList->empty()))
		{
			for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->EntityNodeDefinitionList->begin(); connectionIter < entityNode->EntityNodeDefinitionList->end(); connectionIter++)
			{
				if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode((*connectionIter)->entity, comparisonVariableNodeName))
				{
					definitionFound = true;
				}
			}
		}
	}

	return definitionFound;
}


#ifdef GIA_USE_SYNONYMN_DETECTION
bool compareEntitySynonyms(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode)
{
	bool entityNamesAreSynonymous = false;

	#ifndef USE_WORDNET
	cout << "compareEntitySynonyms() error: requires USE_WORDNET" << endl;
	exit(0);
	#endif

	#ifdef GIA_WORDNET_DEBUG
	//cout << "\t compareEntitySynonyms: queryEntityNode " << queryEntityNode->entityName << ", with entityNode " << entityNode->entityName << endl;
	#endif
	
	/*
	#ifdef GIA_SUPPORT_ALIASES
	if(compareEntityAliases(queryEntityNode, entityNode))
	#else
	if(queryEntityNode->entityName == entityNode->entityName)
	#endif
	{
		entityNamesAreSynonymous = true;
	}
	else
	{
	*/
	
	if(queryEntityNode->wordNetPOS != entityNode->wordNetPOS)	//error checking
	{
		#ifdef GIA_WORDNET_DEBUG
		/*
		cout << "compareEntitySynonyms() warning: (queryEntityNode->wordNetPOS != entityNode->wordNetPOS)" << endl;
		cout << "queryEntityNode->wordNetPOS " << queryEntityNode->entityName << " = " << queryEntityNode->wordNetPOS << endl;
		cout << "entityNode->wordNetPOS = " << entityNode->entityName << " = " << entityNode->wordNetPOS << endl;
		*/
		#endif
	}
	else
	{
		#ifdef GIA_WORDNET_DEBUG
		cout << "WN compare: queryEntityNode = " << queryEntityNode->entityName << ", with " << entityNode->entityName << endl;
		#endif
		if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(queryEntityNode->entityName), entityNode->wordNetPOS))
		{
			entityNamesAreSynonymous = true;
		}

		#ifdef GIA_SUPPORT_ALIASES
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
			{
				if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(*aliasIterQuery), entityNode->wordNetPOS))
				{
					entityNamesAreSynonymous = true;
				}
			}
		}
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(queryEntityNode->entityName), entityNode->wordNetPOS))
			{
				entityNamesAreSynonymous = true;
			}
		}
		for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
		{
			if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(*aliasIterQuery), entityNode->wordNetPOS))
			{
				entityNamesAreSynonymous = true;
			}
		}
		#endif
		#ifdef GIA_WORDNET_DEBUG
		cout << "WN finish compare: queryEntityNode = " << queryEntityNode->entityName << ", with " << entityNode->entityName << endl;		
		if(entityNamesAreSynonymous)
		{
			cout << "entityNamesAreSynonymous" << endl;
		}
		#endif
	}
	
	/*
	}
	*/

	return entityNamesAreSynonymous;
}
#endif

#ifdef GIA_SUPPORT_ALIASES
bool compareEntityAliases(GIAEntityNode * queryEntityNode, GIAEntityNode * entityNode)
{
	bool aliasMatchFound = false;

	if(queryEntityNode->entityName == entityNode->entityName)
	{
		aliasMatchFound = true;
	}

	for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
	{
		for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
		{
			if(*aliasIter == *aliasIterQuery)
			{
				aliasMatchFound = true;
			}
		}
	}
	for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
	{
		if(*aliasIter == queryEntityNode->entityName)
		{
			aliasMatchFound = true;
		}
	}
	for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
	{
		if(*aliasIterQuery == entityNode->entityName)
		{
			aliasMatchFound = true;
		}
	}
	return aliasMatchFound;
}
#endif


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

	if(entityNode->hasQuantity)
	{
		string quantityNumberStringTemp = printQuantityNumberString(entityNode);
		entityPretext = entityPretext + quantityNumberStringTemp + " ";
	}
	else if(entityNode->isSubstance)
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




double determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConceptsQuery)
{
	double maxNumberOfMatchedNodes = 0.0;

	unordered_map<string, GIAEntityNode*>::iterator connectionIterQuery;
	for(connectionIterQuery = entityNodesActiveListConceptsQuery->begin(); connectionIterQuery != entityNodesActiveListConceptsQuery->end(); connectionIterQuery++)
	{//for each node in query semantic net;

		GIAEntityNode* currentQueryEntityNode = connectionIterQuery->second;

		#ifdef GIA_QUERY_DEBUG
		cout << "determineMaxConfidenceOfQuerySemanticNetwork" << endl;
		cout << "\currentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
		#endif

		int numberOfMatchedNodes = 0;
		string queryAnswerContextTemp = "";
		bool traceInstantiations = GIA_QUERY_TRACE_CONCEPT_NODES_DEFINING_INSTANTIATIONS_VALUE;
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
	#ifdef GIA_QUERY_DEBUG
	//cout << "printEntityNodeQualitiesOnly: entityNode->entityName = " << entityNode->entityName << endl;
	#endif
	for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->PropertyNodeList->begin(); connectionIter < entityNode->PropertyNodeList->end(); connectionIter++)
	{
		GIAEntityNode * substanceNode = (*connectionIter)->entity;
		if(substanceNode->isSubstanceQuality)
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

			*printEntityNodeString = *printEntityNodeString + "Type: " + substanceNode->entityName;
		}
	}

	if(numberQualities > 0)
	{
		*printEntityNodeString = *printEntityNodeString + ")\n";
	}
}

void printEntityTimeConditionNodeOnly(GIAEntityNode * entityNode, string * printEntityNodeString)
{
	#ifdef GIA_QUERY_DEBUG
	//cout << "printEntityTimeConditionNodeOnly: entityNode->entityName = " << entityNode->entityName << endl;
	#endif
	
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
		*printEntityNodeString = *printEntityNodeString + "\nContext: " + context + " = " + entityNode->entityName;
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
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION)
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
		if((entityNode->testedForQueryComparisonTemp))	//&& !(entityNode->testedForQueryComparison))?
		{
			pass = true;
			entityNode->testedForQueryComparisonTemp = false;
		}
	}
	#ifdef GIA_USE_NLG
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION)
	{
		if((entityNode->parsedForLanguageGeneration))
		{
			pass = true;
			entityNode->parsedForLanguageGeneration = false;
		}
	}
	#endif
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
		#ifdef GIA_QUERY_DEBUG
		//cout << "entityNode being traced = " << entityNode->entityName << endl;
		//cout << "*numberOfMatchedNodes = " << *numberOfMatchedNodes << endl;
		#endif

		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES
		if(traceInstantiations)
		{
		#endif
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		#ifdef GIA_QUERY_TRACE_INSTANTIATIONS_DO_NOT_INCREMENT_NUMBER_OF_MATCHED_NODES
		}
		else
		{
			if(!thisIsInstanceAndPreviousNodeWasDefinition)
			{
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
			}
		}
		#endif

		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			bool pass2 = true;
			if(!traceInstantiations)
			{
				if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
				{
					pass2 = false;
				}
			}
			if(pass2)
			{
				for(vector<GIAEntityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*connectionIter)->entity, entityVectorConnectionContextArray[i], function, numberOfMatchedNodes, entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[i], referenceSetID, traceInstantiations);
				}
			}
		}
	}
}











