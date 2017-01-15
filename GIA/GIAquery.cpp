/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAquery.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2n3d 21-September-2016
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
#include "GIAtranslatorOperations.h"	//required for getPrimaryNetworkIndexNodeDefiningInstance()


GIAqueryTraceParameters::GIAqueryTraceParameters(void)
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
GIAqueryTraceParameters::~GIAqueryTraceParameters(void)
{
}

GIAqueryTraceParameters::GIAqueryTraceParameters(GIAqueryTraceParameters* queryTraceParametersToCopy)
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
	vector<GIAentityNode*> queryAnswerNodes;
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



GIAreferenceTraceParameters::GIAreferenceTraceParameters(void)
{
	traceMode = TRACE_MODE_ONLY_RECORD_EXACT_MATCH;						//initialisation added 29 Sept 2013
	traceModeAssertSameReferenceSetID = TRACE_MODE_ASSERT_SAME_REFERENCE_SET_ID_TRUE;	//initialisation added 29 Sept 2013
	referenceSetID = GIA_REFERENCE_SET_ID_UNDEFINED; 					//initialisation added 29 Sept 2013
	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	linkSpecificConceptsAndActions	= false;						//initialisation added 29 Sept 2013
	#endif
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	intrasentenceReference = false;
	#endif
	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	doNotParseQuerySubnetsWithConcepts = false;
	#endif

	#ifdef GIA_SUPPORT_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	testReferenceSetContext = false;
	referenceSetDefiniteEntity = NULL;
	//firstSentenceInList = NULL;
	#endif
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	ensureSameReferenceSetQueryConnections = false;
	#endif

	#ifdef GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS
	sameReferenceSetTests = false;
	#endif

	#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
	traceConceptsOnly = false;
	#endif
	#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	logicalConditionDisableTraceConcepts = false;
	#endif
}
GIAreferenceTraceParameters::~GIAreferenceTraceParameters(void)
{
}


#ifdef GIA_QUERY_SIMPLIFIED_SEARCH
GIAentityNode* answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork2(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, bool* foundAnswer, GIAentityNode* queryAnswerNode, double* numberOfMatchedNodes, string* queryAnswerContext)
{
	bool traceModeIsQuery = TRACE_MODE_IS_QUERY_TRUE;

	GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
	GIAentityNode* queryEntityWithMaxNumberNodesMatched = NULL;
	bool foundBestAnswerCandidate = false;
	int maxNumberMatchedNodes = 0;

	for(unordered_map<string, GIAentityNode*>::iterator entityIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); entityIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); entityIterQuery++)
	{//for each node in query semantic net;

		GIAentityNode* currentQueryEntityNode = entityIterQuery->second;

		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		if(!(currentQueryEntityNode->disabled))
		{
		#endif
			if(currentQueryEntityNode->entityName != REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)	//added 22 August 2012
			{
				bool foundQueryEntityNodeName = false;
				long queryEntityNodeIndex = INT_DEFAULT_VALUE;
				string queryEntityNodeName = currentQueryEntityNode->entityName;

				GIAentityNode* networkIndexEntityMatchingCurrentQueryEntity = findOrAddNetworkIndexEntityNodeByName(NULL, entityNodesActiveListNetworkIndexes, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL, false);

				if(foundQueryEntityNodeName)
				{
					#ifdef GIA_QUERY_DEBUG
					cout << "foundQueryEntityNodeName" << endl;
					cout << "\tcurrentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
					#endif

					GIAqueryTraceParameters queryTraceParameters;
					queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
					queryTraceParameters.comparisonVariableNode = comparisonVariableNode;

					GIAreferenceTraceParameters referenceTraceParameters;	//not used


					int numberOfMatchedNodesTemp = 0;
					int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;
					bool exactMatch = testEntityNodeForQueryOrReferenceSet2(currentQueryEntityNode, networkIndexEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);

					if(numberOfMatchedNodesTemp > maxNumberMatchedNodes)
					{
						maxNumberMatchedNodes = numberOfMatchedNodesTemp;
						foundBestAnswerCandidate = true;
						networkEntityWithMaxNumberNodesMatched = networkIndexEntityMatchingCurrentQueryEntity;
						queryEntityWithMaxNumberNodesMatched = currentQueryEntityNode;
					}

					//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
					int irrelevant;
					string printEntityNodeString = "";
					bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
					traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
					traceEntityNode(networkIndexEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
				}
			}
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		}
		#endif
	}

	if(foundBestAnswerCandidate)
	{
		GIAreferenceTraceParameters referenceTraceParameters;	//not used
		GIAqueryTraceParameters queryTraceParameters;
		queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
		queryTraceParameters.comparisonVariableNode = comparisonVariableNode;

		int numberOfMatchedNodesTemp = 0;
		int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;
		bool exactMatch = testEntityNodeForQueryOrReferenceSet2(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);
	}

	return queryAnswerNode;
}



bool testEntityNodeForQueryOrReferenceSet2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool exactMatch = true;

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
	if(!(referenceTraceParameters->intrasentenceReference))
	{
		queryDebugIndentOutputForLevel(queryTraceParameters->level);
		cout << "testEntityNodeForQueryOrReferenceSet2: entityNode = " << entityNode->entityName << convertIntToString(entityNode->idInstance) << ", queryEntityNode = " << queryEntityNode->entityName << convertIntToString(queryEntityNode->idInstance) << endl;

	}
	#endif

	if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparisonTemp))
	{
		#ifdef GIA_QUERY_DEBUG_LEVEL
		queryTraceParameters->level = queryTraceParameters->level+1;
		#endif

		entityNode->testedForQueryComparison = true;
		queryEntityNode->testedForQueryComparison = true;
		entityNode->testedForQueryComparisonTemp = false;
		queryEntityNode->testedForQueryComparisonTemp = false;

		*numberOfMatchedNodes = *numberOfMatchedNodes + 1;

		#ifdef GIA_QUERY_DEBUG
		queryDebugIndentOutputForLevel(queryTraceParameters->level);
		cout << "PASS testEntityNodeForQueryOrReferenceSet2: entityNode = " << entityNode->entityName << convertIntToString(entityNode->idInstance) << ", queryEntityNode = " << queryEntityNode->entityName << convertIntToString(queryEntityNode->idInstance) << endl;
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
		if(!(referenceTraceParameters->intrasentenceReference))
		{
			queryDebugIndentOutputForLevel(queryTraceParameters->level);
			cout << "PASS testEntityNodeForQueryOrReferenceSet2: entityNode = " << entityNode->entityName << convertIntToString(entityNode->idInstance) << ", queryEntityNode = " << queryEntityNode->entityName << convertIntToString(queryEntityNode->idInstance) << endl;
		}
		#endif


		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
			if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)	//Removed && (traceModeIsQuery) GIA 2f19b 23-July-2014 (do not trace instantinations for queries only)
			{
				pass = false;
			}
			#endif
			if(pass)
			{
				#ifdef GIA_QUERY_DEBUG
				//cout << "i = " << i << endl;
				#endif
				for(vector<GIAentityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[i].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[i].end(); connectionIterQuery++)
				{
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					if(traceModeIsQuery || !(referenceTraceParameters->ensureSameReferenceSetQueryConnections) || (*connectionIterQuery)->sameReferenceSet)
					{
					#endif
						#ifdef GIA_QUERY_DEBUG
						cout << "\n\nconnectionIterQuery = " << (*connectionIterQuery)->entity->entityName << ", entityType = " << (*connectionIterQuery)->entity->entityType << endl;
						cout << "connectionIterQuery idInstance = " << (*connectionIterQuery)->entity->idInstance << endl;
						cout << "connectionIterQuery entityIndexTemp = " << (*connectionIterQuery)->entity->entityIndexTemp << endl;
						#endif

						#ifdef GIA_USE_DATABASE
						#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
						if(getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_ACTIVE)
						{
							#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
							cout << "GIAquery; entityNode->entityType = " << entityNode->entityType << endl;
							//cout << "DBreadVectorConnections: " << entityNode->entityName << ", " << entityNode->idInstance << ", i=" << i << endl;
							#endif
							DBreadVectorConnections(entityNode, i);		//this is important, as it will read all of the vector connections from the database for this node (conferred 25 May 2012)
						}
						#endif
						#endif

						int maxNumberMatchedNodes = 0;
						bool foundBestAnswerCandidate = false;
						GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
						#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
						bool atLeastOneExactMatch = false;
						#endif

						for(vector<GIAentityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[i].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[i].rend(); connectionIter++)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected)
						{
							#ifdef GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS
							bool sameReferenceSetTest = true;
							if(referenceTraceParameters->sameReferenceSetTests)
							{
								sameReferenceSetTest = false;
								if((*connectionIterQuery)->sameReferenceSet == (*connectionIter)->sameReferenceSet)
								{
									sameReferenceSetTest = true;
								}
							}
							if(sameReferenceSetTest)
							{
							#endif
								#ifdef GIA_QUERY_DEBUG
								cout << "connectionIter = " << (*connectionIter)->entity->entityName << ", entityType = " << (*connectionIter)->entity->entityType << endl;
								cout << "connectionIter idInstance = " << (*connectionIter)->entity->idInstance << endl;
								cout << "connectionIter entityIndexTemp = " << (*connectionIter)->entity->entityIndexTemp << endl;
								#endif

								int numberOfMatchedNodesTemp = 0;
								int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

								#ifdef GIA_DEBUG
								//cout << "\t\ttesting:" << endl;
								//cout << "\t\tconnectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
								//cout << "\t\tconnectionIter = " << (*connectionIter)->entity->entityName << endl;
								#endif

								bool exactMatchTemp = testReferencedEntityNodeForExactNameMatch2((*connectionIterQuery)->entity, (*connectionIter)->entity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

								if(numberOfMatchedNodesTemp > maxNumberMatchedNodes)
								{
									if(traceModeIsQuery || exactMatchTemp)
									{
										maxNumberMatchedNodes = numberOfMatchedNodesTemp;
										foundBestAnswerCandidate = true;
										networkEntityWithMaxNumberNodesMatched = (*connectionIter)->entity;
										#ifdef GIA_DEBUG
										//if(knownBestMatch)
										//{
											//cout << "exactMatchTemp = " << exactMatchTemp << endl;
											//cout << "foundBestAnswerCandidate = " << (*connectionIter)->entity->entityName << endl;
										//}
										#endif
									}
								}
								#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
								if(exactMatchTemp)
								{
									atLeastOneExactMatch = true;
								}
								#endif

								//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
								int irrelevantInt;
								string irrelevantString = "";
								bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;		//clear all (why is this still required if GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS is off? - it is based on testing, but unknown as to why)
								traceEntityNode((*connectionIter)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
								traceEntityNode((*connectionIterQuery)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);

								#ifdef GIA_QUERY_DEBUG
								//cout << "finished: connectionIter = " << (*connectionIter)->entity->entityName << endl;
								#endif
							#ifdef GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS
							}
							#endif
						}

						if(foundBestAnswerCandidate)
						{
							/*
							if(knownBestMatch)
							{
								cout << "foundBestAnswerCandidate: networkEntityWithMaxNumberNodesMatched->entityName = " << networkEntityWithMaxNumberNodesMatched->entityName << endl;
								cout << "foundBestAnswerCandidate: networkEntityWithMaxNumberNodesMatched idInstance = " << networkEntityWithMaxNumberNodesMatched->idInstance << endl;
								cout << "foundBestAnswerCandidate: networkEntityWithMaxNumberNodesMatched entityIndexTemp = " << networkEntityWithMaxNumberNodesMatched->entityIndexTemp << endl;
								cout << "foundBestAnswerCandidate: (*connectionIterQuery)->entity->entityName = " << (*connectionIterQuery)->entity->entityName << endl;
								cout << "foundBestAnswerCandidate: (*connectionIterQuery) idInstance = " << (*connectionIterQuery)->entity->idInstance << endl;
								cout << "foundBestAnswerCandidate: (*connectionIterQuery) entityIndexTemp = " << (*connectionIterQuery)->entity->entityIndexTemp << endl;
							}
							*/

							#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
							if(knownBestMatch)
							{
								#ifdef GIA_DEBUG
								//cout << "knownBestMatch: (*connectionIterQuery)->entity->entityName = " << (*connectionIterQuery)->entity->entityName << endl;
								#endif
								(*connectionIterQuery)->entity->entityCorrespondingBestMatch = networkEntityWithMaxNumberNodesMatched;		//this shouldn't be required for queries....
							}
							#endif

							int numberOfMatchedNodesTemp = *numberOfMatchedNodes;
							int numberOfMatchedNodesRequiredSynonymnDetectionTemp = *numberOfMatchedNodesRequiredSynonymnDetection;
							if(!testReferencedEntityNodeForExactNameMatch2((*connectionIterQuery)->entity, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, knownBestMatch, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
							{
								//for advanced referencing this should never be the case (it should always refind what was found originally)
								exactMatch = false;
								#ifdef GIA_DEBUG
								//cout << "!exactMatch1: (*connectionIterQuery)->entity->entityName = " << (*connectionIterQuery)->entity->entityName << endl;
								#endif
							}
							*numberOfMatchedNodes = numberOfMatchedNodesTemp;
							*numberOfMatchedNodesRequiredSynonymnDetection = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
						}

						#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
						if(((*connectionIterQuery)->entity->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))
						{
							if(!atLeastOneExactMatch)
							{
								#ifdef GIA_DEBUG
								/*
								cout << "!atLeastOneExactMatch" << endl;
								cout << "(*connectionIterQuery)->entity->referenceSetID  = " << (*connectionIterQuery)->entity->referenceSetID << endl;
								cout << "(*connectionIterQuery)->entity->entityName  = " << (*connectionIterQuery)->entity->entityName << endl;
								cout << "(*connectionIterQuery)->entity->isNetworkIndex  = " << (*connectionIterQuery)->entity->isNetworkIndex << endl;
								*/
								#endif
								exactMatch = false;
							}
						}
						#endif
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					}
					#endif
				}
				#ifdef GIA_QUERY_DEBUG
				//cout << "finished: i = " << i << endl;
				#endif
			}
		}

		entityNode->testedForQueryComparison = false;
		queryEntityNode->testedForQueryComparison = false;
		entityNode->testedForQueryComparisonTemp = true;
		queryEntityNode->testedForQueryComparisonTemp = true;

		#ifdef GIA_QUERY_DEBUG_LEVEL
		queryTraceParameters->level = queryTraceParameters->level-1;
		#endif
	}
	#ifdef GIA_DEBUG
	/*
	else
	{
		cout << "no entry:" << endl;
		cout << "\tentityNode->testedForQueryComparison = " <<  entityNode->testedForQueryComparison << endl;
		cout << "\tentityNode->testedForQueryComparisonTemp = " <<  entityNode->testedForQueryComparisonTemp << endl;
		cout << "\tqueryEntityNode->testedForQueryComparison = " <<  queryEntityNode->testedForQueryComparison << endl;
		cout << "\tqueryEntityNode->testedForQueryComparisonTemp = " <<  queryEntityNode->testedForQueryComparisonTemp << endl;
	}
	*/
	#endif

	return exactMatch;
}



bool testReferencedEntityNodeForExactNameMatch2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool exactMatch = false;
	string sourceContextCurrent = queryTraceParameters->sourceContext;
	if(entityNode != queryEntityNode)	//added 2i19c
	{
		#ifdef GIA_QUERY_DEBUG
		cout << "testReferencedEntityNodeForExactNameMatch2: queryEntityNode = " << queryEntityNode->entityName << ", entityNode = " << entityNode->entityName << endl;
		#endif
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
		if(!(referenceTraceParameters->intrasentenceReference))
		{
			queryDebugIndentOutputForLevel(queryTraceParameters->level);
			cout << "testReferencedEntityNodeForExactNameMatch2: entityNode = " << entityNode->entityName << convertIntToString(entityNode->idInstance) << ", queryEntityNode = " << queryEntityNode->entityName << convertIntToString(queryEntityNode->idInstance) << endl;

		}
		#endif

		if((entityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparison))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
			#ifdef GIA_DEBUG
			//cout << "((entityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparison))" << endl;
			#endif
		}
		else if((entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparisonTemp))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
			#ifdef GIA_DEBUG
			//cout << "((entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparisonTemp))" << endl;
			#endif
		}
		else if((queryEntityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparison))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
			#ifdef GIA_DEBUG
			//cout << "((queryEntityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparison))" << endl;
			#endif
		}
		else if((queryEntityNode->testedForQueryComparisonTemp) && !(entityNode->testedForQueryComparisonTemp))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
			#ifdef GIA_DEBUG
			//cout << "((queryEntityNode->testedForQueryComparisonTemp) && !(entityNode->testedForQueryComparisonTemp))" << endl;
			#endif
		}
		else if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp))
		{
			bool compareEntityNamesResult = compareEntityStandard(queryEntityNode, entityNode, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

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
					if(queryEntityNode->entityName == queryTraceParameters->comparisonVariableNode->entityName)
					//#endif
					{//exact match found
						foundMatch = true;
						#ifdef GIA_QUERY_DEBUG
						cout << "detectComparisonVariable && (queryEntityNode->entityName == comparisonVariableNode->entityName)" << endl;
						cout << "\t foundMatch:" << queryEntityNode->entityName << endl;
						#endif

						#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
						if(queryEntityNode->isWhichOrEquivalentWhatQuery)
						{
							/*
							if a 'which' query, then verify that the entityNode is defined by the comparisonVariableNode [ie has a definition corresponding to the comparisonVariableNode]
							eg1 a dog eats the mud. dogs are animals. / which animal eats the mud?	[answer: 'dog' - which is an instance of 'dog' networkIndex node, where the 'dog' networkIndex node is defined by 'animal'
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

					#ifdef GIA_QUERY_DEBUG
					cout << "foundAnswer:" << entityNode->entityName << endl;
					#endif
				}

				bool nonMatchingConditions = false;
				if(!compareEntityNamesResult && queryTraceParameters->isCondition)		//now supports unimplemented preposition reduction (need to modify compareEntitySynonyms() to support synonomous prepositions), 	OLD: before 21 May 2012: if((queryEntityNode->entityName != entityNode->entityName) && queryTraceParameters->isCondition)
				{
					nonMatchingConditions = true;
				}

				if((compareEntityNamesResult || nonMatchingConditions || foundMatch) && !(queryTraceParameters->nonMatchingSourceConditions))	//do not continue trace if non-matching source conditions	//allow trace past the answer
				{
					queryTraceParameters->nonMatchingSourceConditions = nonMatchingConditions;

					#ifdef GIA_QUERY_DEBUG
					//cout << "compareEntityNamesResult, testEntityNodeForQueryOrReferenceSet" << endl;
					#endif
					if(testEntityNodeForQueryOrReferenceSet2(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
					{
						exactMatch = true;
					}
				}

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
				}
				#endif
			}
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			else
			{
				int resultOldTemp = EXACT_MATCH_OUT_OF_BOUNDS;
				compareEntityReferenceTrace(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters, compareEntityNamesResult, &exactMatch, true, &resultOldTemp);
			}
			#endif
		}
		else
		{
			exactMatch = true;
			#ifdef GIA_DEBUG
			cout << "already successfully parsed this route" << endl;
			#endif
		}
	}

	return exactMatch;
}
#endif


GIAentityNode* answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, bool* foundAnswer, GIAentityNode* queryAnswerNode, double* numberOfMatchedNodes, string* queryAnswerContext)
{
	bool traceModeIsQuery = TRACE_MODE_IS_QUERY_TRUE;

	int numberOfMatchedNodesTempMax = 0;
	int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = 0;
	bool foundAnAnswer = false;	//this is not really used, and is more of an artefact...

	#ifdef GIA_QUERY_DEBUG
	if(detectComparisonVariable)
	{
		cout << "answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork" << endl;
		cout << "detectComparisonVariable" << endl;
		cout << "\tcomparisonVariableNode->entityName = " << comparisonVariableNode->entityName << endl;
	}
	#endif

	GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
	GIAentityNode* queryEntityWithMaxNumberNodesMatched = NULL;

	bool foundAtLeastOneMatch = false;

	for(unordered_map<string, GIAentityNode*>::iterator entityIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); entityIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); entityIterQuery++)
	{//for each node in query semantic net;

		#ifdef GIA_QUERY_DEBUG
		cout << "\n\t\t\t**********************Query Trace Start: entityIterQuery->second->entityName = " << entityIterQuery->second->entityName << endl;
		#endif
		#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
		cout << "\n\t\t\t**********************Query Trace Start: entityIterQuery->second->entityName = " << entityIterQuery->second->entityName << endl;
		#endif
		GIAentityNode* currentQueryEntityNode = entityIterQuery->second;

		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		if(!(currentQueryEntityNode->disabled))
		{
		#endif

			if(currentQueryEntityNode->entityName != REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)	//added 22 August 2012
			{
				bool foundQueryEntityNodeName = false;
				long queryEntityNodeIndex = INT_DEFAULT_VALUE;
				string queryEntityNodeName = currentQueryEntityNode->entityName;

				GIAentityNode* networkIndexEntityMatchingCurrentQueryEntity = findOrAddNetworkIndexEntityNodeByName(NULL, entityNodesActiveListNetworkIndexes, &queryEntityNodeName, &foundQueryEntityNodeName, &queryEntityNodeIndex, false, NULL, NULL, false);

				if(foundQueryEntityNodeName)
				{
					#ifdef GIA_QUERY_DEBUG
					cout << "foundQueryEntityNodeName" << endl;
					cout << "\tcurrentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
					#endif

					//now start matching structure search for all substances of the identical networkIndex node (to current query entity name) in Semantic Network

					int numberOfMatchedNodesTemp = 0;
					int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

					GIAqueryTraceParameters queryTraceParametersTemp;
					queryTraceParametersTemp.detectComparisonVariable = detectComparisonVariable;
					queryTraceParametersTemp.comparisonVariableNode = comparisonVariableNode;

					GIAreferenceTraceParameters referenceTraceParameters;	//irrelevant

					bool exactMatchIrrelevant = testEntityNodeForQueryOrReferenceSet(currentQueryEntityNode, networkIndexEntityMatchingCurrentQueryEntity, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, &referenceTraceParameters);
					//queryAnswerNodeTemp = testEntityNodeForQuery(currentQueryEntityNode, networkIndexEntityMatchingCurrentQueryEntity, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, false, &queryPreviousAnswerNodeTemp, &queryAnswerContextTemp, false, false);
					//NB entityNode->isAnswerContextToQuery is identical to entityNode->testedForQueryComparison

					#ifdef GIA_QUERY_DEBUG
					cout << "queryTraceParametersTemp.foundAnswer = " << queryTraceParametersTemp.foundAnswer << endl;
					cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
					#endif

					bool irrelevantBool = false;
					bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, foundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, irrelevantBool, exactMatchIrrelevant);
					//NB are only interested in answers if they give the max or equal to max node matches.. (otherwise answers are now disgarded; as of GIA1l...)
					if(bestAnswerCandidate)
					{
						foundAtLeastOneMatch = true;

						numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
						numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;

						networkEntityWithMaxNumberNodesMatched = networkIndexEntityMatchingCurrentQueryEntity;
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
					bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
					traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
					traceEntityNode(networkIndexEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);
				}
			}
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		}
		#endif
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

		GIAqueryTraceParameters queryTraceParameters;
		queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
		queryTraceParameters.comparisonVariableNode = comparisonVariableNode;

		GIAreferenceTraceParameters referenceTraceParameters;	//irrelevant

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
		bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
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
					for(vector<GIAentityNode*>::iterator entityAnswerIter = queryTraceParameters.queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters.queryAnswerNodes.end(); entityAnswerIter++)
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




int testReferencedEntityNodeForExactNameMatch(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters)
{
	int result = EXACT_MATCH_OUT_OF_BOUNDS;
	string sourceContextCurrent = queryTraceParameters->sourceContext;
	if(entityNode != queryEntityNode)	//added 2i19c
	{
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

			bool compareEntityNamesResult = compareEntityStandard(queryEntityNode, entityNode, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

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
					if(queryEntityNode->entityName == queryTraceParameters->comparisonVariableNode->entityName)
					//#endif
					{//exact match found
						foundMatch = true;
						#ifdef GIA_QUERY_DEBUG
						cout << "detectComparisonVariable && (queryEntityNode->entityName == comparisonVariableNode->entityName)" << endl;
						cout << "\t foundMatch:" << queryEntityNode->entityName << endl;
						#endif

						#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
						if(queryEntityNode->isWhichOrEquivalentWhatQuery)
						{
							/*
							if a 'which' query, then verify that the entityNode is defined by the comparisonVariableNode [ie has a definition corresponding to the comparisonVariableNode]
							eg1 a dog eats the mud. dogs are animals. / which animal eats the mud?	[answer: 'dog' - which is an instance of 'dog' networkIndex node, where the 'dog' networkIndex node is defined by 'animal'
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
						for(vector<GIAentityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
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
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "addingAnswer:" << entityNode->entityName << endl;
							#endif
						}
					}
					#endif

					#ifdef GIA_QUERY_DEBUG
					cout << "\nfoundmatch AnumberOfMatchedNodes = " <<* numberOfMatchedNodes << "\n" << endl;
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
					cout << "\n\tfoundmatch BnumberOfMatchedNodes = " <<* numberOfMatchedNodes << "\n" << endl;
				}
				else
				{
					//cout << "!foundMatch" << endl;
				}
				#endif

			}
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			else
			{
				bool exactMatchTemp = false;
				compareEntityReferenceTrace(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters, compareEntityNamesResult, &exactMatchTemp, false, &result);
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
	}

	return result;
}





bool testEntityNodeForQueryOrReferenceSet(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters)
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
			queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
				if(GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE)
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
			queryDebugIndentOutputForLevel(queryTraceParameters->level);
			cout << "numberOfMatchedNodes = " <<* numberOfMatchedNodes << ", entityNode->entityName = " << entityNode->entityName << endl;
			#endif
		}

		#ifdef GIA_QUERY_DEBUG
		cout << "\ntestEntityNodeForQueryOrReferenceSet:" << endl;
		cout << "entityNode = " << entityNode->entityName << endl;
		#endif

		#ifdef GIA_QUERY_DEBUG
		//cout << "\tqueryEntityNode->entityName = " << queryEntityNode->entityName << endl;
		if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
		{
			cout << "entityNode = " << entityNode->entityName << " (is networkIndex)" << endl;
		}
		if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
		{
			cout << "entityNode = " << entityNode->entityName << " (is substance)" << endl;
		}
		else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
		{
			cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
		}
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << " (undefined/networkIndex)" << endl;
		}
		cout << "numberOfMatchedNodes = " <<* numberOfMatchedNodes << endl;
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isSubstance = " << entityNode->isSubstance << endl;
		cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
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
						for(vector<GIAentityNode*>::iterator connectionIter = entityNode->entityNodeDefinitionList.begin(); connectionIter != entityNode->entityNodeDefinitionList.end(); connectionIter++)
						{
							queryTraceParameters->sourceContext = "being ";
							//cout << "\t 2A sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*connectionIterQuery,* connectionIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);

							queryTraceParameters->queryAnswerNode = queryTraceParametersTemp
						}

						for(connectionIter = entityNode->propertyNodeList.begin(); connectionIter != entityNode->propertyNodeList.end(); connectionIter++)
						{
							queryTraceParameters->sourceContext = "having ";
							//cout << "\t 2B sourceIsConditionAndHasComparisonVariableAttached" << endl;
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(*connectionIterQuery,* connectionIter, &numberOfMatchedNodesTemp, traceMode, traceModeAssertSameReferenceSetID, referenceSetID, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters);
						}
					}
				}
			}
		}
		*/


		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
			if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)	//Removed && (traceModeIsQuery) GIA 2f19b 23-July-2014 (do not trace instantinations for queries only)
			{
				pass = false;
			}
			#endif
			if(pass)
			{
				#ifdef GIA_QUERY_DEBUG
				//cout << "i = " << i << endl;
				#endif
				for(vector<GIAentityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[i].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[i].end(); connectionIterQuery++)
				{
					#ifdef GIA_QUERY_DEBUG
					//cout << "connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
					#endif
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					if(traceModeIsQuery || !(referenceTraceParameters->ensureSameReferenceSetQueryConnections) || (*connectionIterQuery)->sameReferenceSet)
					{
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
							#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
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
						GIAentityNode* queryEntityCorrespondingBestMatch;

						#ifdef GIA_USE_DATABASE
						#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
						if(getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_ACTIVE)
						{
							#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
							cout << "GIAquery; entityNode->isNetworkIndex = " << entityNode->isNetworkIndex << endl;
							cout << "GIAquery; entityNode->isSubstance = " << entityNode->isSubstance << endl;
							//cout << "DBreadVectorConnections: " << entityNode->entityName << ", " << entityNode->idInstance << ", i=" << i << endl;
							#endif
							DBreadVectorConnections(entityNode, i);		//this is important, as it will read all of the vector connections from the database for this node (conferred 25 May 2012)
						}
						#endif
						#endif

						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						queryDebugIndentOutputForLevel(queryTraceParameters->level);
						cout << "A. SDGG connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
						#endif

						//for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
						for(vector<GIAentityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[i].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[i].rend(); connectionIter++)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected)
						{
							#ifdef GIA_QUERY_DEBUG
							//cout << "connectionIter = " << (*connectionIter)->entity->entityName << endl;
							#endif

							#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "A2. SDGG connectionIter = " << (*connectionIter)->entity->entityName << endl;
							#endif
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG
							cout << "" << endl;
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "tracing: (*connectionIterQuery) = " << (*connectionIterQuery)->entity->entityName << ", (*connectionIter) = " << (*connectionIter)->entity->entityName << ", i = " << i <<  endl;
							#endif

							GIAqueryTraceParameters queryTraceParametersTemp(queryTraceParameters);

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
									queryDebugIndentOutputForLevel(queryTraceParameters->level);
									cout << "bestAnswerCandidate: (*connectionIter)->entity->entityName = " << (*connectionIter)->entity->entityName << endl;
									#endif
									queryEntityCorrespondingBestMatch = (*connectionIter)->entity;
									numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
									numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
								}
								else
								{
									#ifdef GIA_ADVANCED_REFERENCING_DEBUG
									queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
												queryDebugIndentOutputForLevel(queryTraceParameters->level);
												cout << "queryTraceParametersTemp.foundAnswer: answer = " << queryTraceParametersTemp.queryAnswerNode->entityName << endl;
												#endif

												/*
												#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
												for(vector<GIAentityNode*>::iterator entityAnswerIter = queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIter != queryTraceParametersTemp.queryAnswerNodes.end(); entityAnswerIter++)
												{
													queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
														for(vector<GIAentityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
														{
															queryDebugIndentOutputForLevel(queryTraceParameters->level);
															cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << ", numberOfMatchedNodesAtPreviousAnswerNode = " << queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode << endl;

															queryDebugIndentOutputForLevel(queryTraceParameters->level);
															cout << "Multiple Answer Cleared:" << (*entityAnswerIter)->entityName << endl;
															queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
														for(vector<GIAentityNode*>::iterator entityAnswerIterTemp = queryTraceParametersTemp.queryAnswerNodes.begin(); entityAnswerIterTemp != queryTraceParametersTemp.queryAnswerNodes.end(); entityAnswerIterTemp++)
														{
															bool answerAlreadyAddedToMultipleAnswers = false;
															vector<string>::iterator entityAnswerContextsIter = queryTraceParameters->queryAnswerContexts.begin();
															for(vector<GIAentityNode*>::iterator entityAnswerIter = queryTraceParameters->queryAnswerNodes.begin(); entityAnswerIter != queryTraceParameters->queryAnswerNodes.end(); entityAnswerIter++)
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
																queryDebugIndentOutputForLevel(queryTraceParameters->level);
																cout << "Multiple Answer Found:" << (*entityAnswerIterTemp)->entityName << endl;
																queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
								queryDebugIndentOutputForLevel(queryTraceParameters->level);
								cout << "MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS: (*connectionIter)->entity->entityName = " << (*connectionIter)->entity->entityName << endl;
								#endif
							}
							#endif
							//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
							int irrelevantInt;
							string irrelevantString = "";
							bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;		//clear all (why is this still required if GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS is off? - it is based on testing, but unknown as to why)
							traceEntityNode((*connectionIter)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);
							traceEntityNode((*connectionIterQuery)->entity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, false, NULL, traceInstantiations);

							#ifdef GIA_QUERY_DEBUG
							//cout << "finished: connectionIter = " << (*connectionIter)->entity->entityName << endl;
							#endif
						}

						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						queryDebugIndentOutputForLevel(queryTraceParameters->level);
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
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "matchFound" << endl;
							#endif

							#ifdef GIA_QUERY_DEBUG
							//cout << "matchFound i = " << i << endl;
							//cout << "matchFound" << endl;
							//cout << "numberOfMatchedNodesTempMax = " << numberOfMatchedNodesTempMax << endl;
							#endif

							#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
							if(knownBestMatch)
							{
								#ifdef GIA_DEBUG
								//cout << "knownBestMatch" << endl;
								#endif
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
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "numberOfMatchedNodes = " <<* numberOfMatchedNodes << endl;
							#endif
						}
						else
						{
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG
							queryDebugIndentOutputForLevel(queryTraceParameters->level);
							cout << "nomatchFound" << endl;
							#endif
						}

						#ifdef GIA_QUERY_MULTIPLE_ANSWERS_DEBUG
						queryDebugIndentOutputForLevel(queryTraceParameters->level);
						cout << "C. SDGG connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
						#endif

						#ifdef GIA_QUERY_DEBUG
						//cout << "finished: connectionIterQuery = " << (*connectionIterQuery)->entity->entityName << endl;
						#endif
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					}
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
			for(vector<GIAentityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[entityVectorConnectionEqualitiesArray[i]].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[entityVectorConnectionEqualitiesArray[i]].end(); connectionIterQuery++)
			{
				if(entityNode->isSubstance && (*connectionIter)->entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
				{
					//equality link found - enable pass through
					testEntityNodeForQueryOrReferenceSet((*connectionIter)->entity, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

				}

			}
			for(vector<GIAentityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[i].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[i].rend(); connectionIter++)	//reverse iterator not required here
			{
				if(entityNode->isSubstance && (*connectionIter)->entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
				{
					//equality link found - enable pass through
					testEntityNodeForQueryOrReferenceSet(queryEntityNode, (*connectionIter)->entity, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);
				}
			}
		}
		#endif

		#ifdef GIA_QUERY_DEBUG
		if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is substance)" << endl;
		}
		else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;
		}
		else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;
		}
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
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

		#ifdef GIA_QUERY_DEBUG_LEVEL
		queryTraceParameters->level = queryTraceParameters->level-1;
		#endif

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

bool determineMatchParameters(bool exactMatchFoundTemp, bool traceIsQuery, int referenceTraceParametersTraceMode, int numberOfMatchedNodesTempMax, bool* exactMatch)
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

bool verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAentityNode* entityNode, string comparisonVariableNodeName)
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
			if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(getPrimaryNetworkIndexNodeDefiningInstance(entityNode), comparisonVariableNodeName))
			{
				definitionFound = true;
			}
		}

		if(!(entityNode->entityNodeDefinitionList->empty()))
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityNodeDefinitionList->begin(); connectionIter < entityNode->entityNodeDefinitionList->end(); connectionIter++)
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
bool compareEntitySynonyms(GIAentityNode* queryEntityNode, GIAentityNode* entityNode)
{
	bool entityNamesAreSynonymous = false;

	#ifndef USE_WORDNET
	cout << "compareEntitySynonyms{} error: requires USE_WORDNET" << endl;
	exit(0);
	#endif

	#ifdef GIA_WORDNET_DEBUG
	//cout << "\t compareEntitySynonyms: queryEntityNode " << queryEntityNode->entityName << ", with entityNode " << entityNode->entityName << endl;
	#endif

	/*
	if(compareEntityAliases(queryEntityNode, entityNode))
	{
		entityNamesAreSynonymous = true;
	}
	else
	{
	*/

	if(queryEntityNode->grammaticalWordTypeTemp != entityNode->grammaticalWordTypeTemp)	//error checking
	{
		#ifdef GIA_WORDNET_DEBUG
		/*
		cout << "compareEntitySynonyms() warning: (queryEntityNode->grammaticalWordTypeTemp != entityNode->grammaticalWordTypeTemp)" << endl;
		cout << "queryEntityNode->grammaticalWordTypeTemp " << queryEntityNode->entityName << " = " << queryEntityNode->grammaticalWordTypeTemp << endl;
		cout << "entityNode->grammaticalWordTypeTemp = " << entityNode->entityName << " = " << entityNode->grammaticalWordTypeTemp << endl;
		*/
		#endif
	}
	else
	{
		#ifdef GIA_WORDNET_DEBUG
		cout << "WN compare: queryEntityNode = " << queryEntityNode->entityName << ", with " << entityNode->entityName << endl;
		#endif
		if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(queryEntityNode->entityName), entityNode->grammaticalWordTypeTemp))
		{
			entityNamesAreSynonymous = true;
		}

		#ifdef GIA_SUPPORT_ALIASES
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
			{
				if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(*aliasIterQuery), entityNode->grammaticalWordTypeTemp))
				{
					entityNamesAreSynonymous = true;
				}
			}
		}
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(queryEntityNode->entityName), entityNode->grammaticalWordTypeTemp))
			{
				entityNamesAreSynonymous = true;
			}
		}
		for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
		{
			if(checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(*aliasIterQuery), entityNode->grammaticalWordTypeTemp))
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

bool compareEntityAliases(GIAentityNode* queryEntityNode, GIAentityNode* entityNode)
{
	bool aliasMatchFound = false;

	#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
	if(!(queryEntityNode->disabled) && !(entityNode->disabled))
	{
	#endif
		#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION_GENERAL_IMPLEMENTATION
		//match "have" and "poss" special possessive actions
		if(isActionSpecialPossessive(queryEntityNode) && isActionSpecialPossessive(entityNode))
		{
			aliasMatchFound = true;
		}
		#endif

		if(queryEntityNode->entityName == entityNode->entityName)
		{
			aliasMatchFound = true;
		}

		#ifdef GIA_SUPPORT_ALIASES

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
				/*
				if(queryEntityNode->entityName == "making")
				{
					cout << "entityNode->entityName = " << entityNode->entityName << endl;
				}
				*/
				aliasMatchFound = true;
			}
		}

		#endif

	#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
	}
	#endif

	return aliasMatchFound;
}


void generateTexualContextWithPreviousNodeForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode, GIAentityNode* entityNodePrevious)
{
	generateTexualContextEntityStringForwards(queryAnswerContext, entityNodePrevious);

	*queryAnswerContext = *queryAnswerContext + sourceContext;

	generateTexualContextEntityStringForwards(queryAnswerContext, entityNode);

	*queryAnswerContext = *queryAnswerContext + ". ";
}

void generateTexualContextForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = *queryAnswerContext + sourceContext + texualContextEntityString;
}

void generateTexualContextBackwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = sourceContext + texualContextEntityString +* queryAnswerContext;
}

void generateTexualContextEntityStringForwards(string* queryAnswerContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = *queryAnswerContext + texualContextEntityString;
}

void generateTexualContextEntityStringBackwards(string* queryAnswerContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = texualContextEntityString +* queryAnswerContext;
}

void generateTexualContextEntityString(string* texualContextEntityString, GIAentityNode* entityNode)
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
	else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
	{
	}
	else
	{//networkIndex
		entityPretext = entityPretext + "a ";
	}

	*texualContextEntityString =  entityPretext + entityNode->entityName + " " + entityPosttext;
}




double determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery)
{
	double maxNumberOfMatchedNodes = 0.0;

	for(unordered_map<string, GIAentityNode*>::iterator connectionIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); connectionIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); connectionIterQuery++)
	{//for each node in query semantic net;

		GIAentityNode* currentQueryEntityNode = connectionIterQuery->second;

		#ifdef GIA_QUERY_DEBUG
		cout << "determineMaxConfidenceOfQuerySemanticNetwork" << endl;
		cout << "\currentQueryEntityNode->entityName = " << currentQueryEntityNode->entityName << endl;
		#endif

		int numberOfMatchedNodes = 0;
		string queryAnswerContextTemp = "";
		bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
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


void printEntityNodeQualitiesOnly(GIAentityNode* entityNode, string* printEntityNodeString)
{
	int numberQualities = 0;
	#ifdef GIA_QUERY_DEBUG
	//cout << "printEntityNodeQualitiesOnly: entityNode->entityName = " << entityNode->entityName << endl;
	#endif
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->propertyNodeList->begin(); connectionIter < entityNode->propertyNodeList->end(); connectionIter++)
	{
		GIAentityNode* substanceNode = (*connectionIter)->entity;
		if(substanceNode->entityType == GIA_ENTITY_TYPE_TYPE_QUALITY)
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

void printEntityTimeConditionNodeOnly(GIAentityNode* entityNode, string* printEntityNodeString)
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


void traceEntityNodeDetermineNextCourseOfAction(string* printEntityNodeString, GIAentityNode* entityNode, string context, int function, int* numberOfMatchedNodes, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations)
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
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
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
void traceEntityNode(GIAentityNode* entityNode, int function, int* numberOfMatchedNodes, string* printEntityNodeString, bool thisIsInstanceAndPreviousNodeWasDefinition, int referenceSetID, bool traceInstantiations)
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
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY)
	{
		if(!(entityNode->testedForQueryComparisonTemp))	//&& !(entityNode->testedForQueryComparison))?
		{
			pass = true;
			entityNode->testedForQueryComparisonTemp = true;
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
		//cout << "*numberOfMatchedNodes = " <<* numberOfMatchedNodes << endl;
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
				for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*connectionIter)->entity, entityVectorConnectionContextArray[i], function, numberOfMatchedNodes, entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[i], referenceSetID, traceInstantiations);
				}
			}
		}
	}
}

void queryDebugIndentOutputForLevel(int currentLevel)
{
	for(int level=0; level<currentLevel; level++)
	{
		cout << "\t";
	}
}





bool compareEntityStandard(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool compareEntityNamesResult = false;

	if(compareEntityAliases(queryEntityNode, entityNode))
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
				#ifdef GIA_USE_SYNONYMN_DETECTION_DISABLE_DURING_SPECIFIC_CONCEPT_ACTION_LINKING
				if(traceModeIsQuery || !(referenceTraceParameters->linkSpecificConceptsAndActions))
				{
				#endif
					if(compareEntitySynonyms(queryEntityNode, entityNode))
					{
						compareEntityNamesResult = true;
						*numberOfMatchedNodesRequiredSynonymnDetection = *numberOfMatchedNodesRequiredSynonymnDetection + 1;
					}
				#ifdef GIA_USE_SYNONYMN_DETECTION_DISABLE_DURING_SPECIFIC_CONCEPT_ACTION_LINKING
				}
				#endif
			}
		}
	}
	#endif

	#ifdef GIA_REFERENCING_WILD_CARDS
 	if(!traceModeIsQuery)
	{
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		if(!(queryEntityNode->disabled) && !(entityNode->disabled))
		{
		#endif
			#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
			if(!(referenceTraceParameters->intrasentenceReference))
			{
			#endif
				#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
				if(!(referenceTraceParameters->linkSpecificConceptsAndActions))
				{
				#endif
					if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
					{
						if(textInTextArray(queryEntityNode->entityName, giaReferencingWildCardActionArray, GIA_REFERENCING_WILD_CARDS_ACTIONS_NUMBER_OF_TYPES))
						{
							compareEntityNamesResult = true;
						}
					}
					else
					{
						if(textInTextArray(queryEntityNode->entityName, giaReferencingWildCardSubstanceArray, GIA_REFERENCING_WILD_CARDS_SUBSTANCES_NUMBER_OF_TYPES))
						{
							compareEntityNamesResult = true;
						}
					}
				#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
				}
				#endif
			#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
			}
			#endif

		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		}
		#endif
	}
	#endif

	return compareEntityNamesResult;
}

void compareEntityReferenceTrace(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, GIAreferenceTraceParameters* referenceTraceParameters, bool compareEntityNamesResult, bool* exactMatch, bool simplifiedSearch, int* resultOld)
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
	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	//cout << "referenceTraceParameters->doNotParseQuerySubnetsWithConcepts = " << referenceTraceParameters->doNotParseQuerySubnetsWithConcepts << endl;
	//cout << "queryEntityNode->isConcept = " << queryEntityNode->isConcept << endl;
	if(!(referenceTraceParameters->doNotParseQuerySubnetsWithConcepts) || !(queryEntityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))
	{
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
				if(queryEntityNode->idActiveList != entityNode->idActiveList)	//else they are exactly the same [NB with new implementation of GIA_RECORD_SAME_REFERENCE_SET_INFORMATION, it will detect the same nodes as a reference match, so they need to be ignored when this happens]
				{
				#endif
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					bool passIntrasentenceReferenceRequirements = true;
					if(referenceTraceParameters->intrasentenceReference)
					{
						passIntrasentenceReferenceRequirements = false;

						#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
						cout << "\nqueryEntityNode->entityName = " << queryEntityNode->entityName << endl;
						cout << "queryEntityNode->referenceSetID = " << queryEntityNode->referenceSetID << endl;
						cout << "queryEntityNode->minimumEntityIndexOfReferenceSet = " << queryEntityNode->minimumEntityIndexOfReferenceSet << endl;
						cout << "entityNode->entityIndexTemp = " << entityNode->entityIndexTemp << endl;
						#endif

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
							cout << "compareEntityNamesResult: queryEntityNode->entityName = " << queryEntityNode->entityName << ", entityNode->entityName = " << entityNode->entityName << endl;

							cout << "queryEntityNode->entityType = " << queryEntityNode->entityType << endl;
							cout << "entityNode->entityType = " << entityNode->entityType << endl;
							#endif

							#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
							bool passSpecificConcepts = true;
							if(((queryEntityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && !(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)) ||
							((entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && !(queryEntityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)))
							{
								passSpecificConcepts = false;
								#ifdef GIA_DEBUG
								//cout << "\t\t\t !passSpecificConcepts" << endl;
								#endif
							}
							#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
							if(referenceTraceParameters->linkSpecificConceptsAndActions)
							{
								//override passSpecificConcepts value:
								#ifndef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED
								if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
								{
									passSpecificConcepts = false;
								}
								else
								{
								#endif
									passSpecificConcepts = true;
								#ifndef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED
								}
								#endif
							}
							#endif
							if(passSpecificConcepts)
							{
							#endif
								#ifdef GIA_ADVANCED_REFERENCING_ENSURE_PLURALITY_MATCHES
								bool passPluralityMatch = true;
								if(((queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && !(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)) ||
								((entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && !(queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)))
								{
									if(!(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT && queryEntityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))	//condition added 29 Sept 2013
									{//if they are concepts, ignore plural (in fact concepts should not be assigned plural in the first place; this is an artefact of the english grammmar system: eg "blue chickens are strong")
										passPluralityMatch = false;
									}
								}
								#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
								if(referenceTraceParameters->linkSpecificConceptsAndActions)	//removed "|| referenceTraceParameters->referenceSetDefiniteEntity" - findIndefiniteEntityCorrespondingToDefiniteEntityInSameContext() referenceSetDefiniteEntity now uses testReferencedEntityNodeForExactNameMatch2 to perform plurality tests
								{
									passPluralityMatch = true;
								}
								#endif
								if(passPluralityMatch)
								{
								#endif
									#ifdef GIA_DEBUG
									//cout << "\tpassed isConcept tests" << endl;
									#endif
									#ifdef GIA_SUPPORT_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
									bool passReferenceContextMatch = true;
									if(referenceTraceParameters->testReferenceSetContext)
									{
										if(queryEntityNode == referenceTraceParameters->referenceSetDefiniteEntity)
										{
											passReferenceContextMatch = false;
											int indentationDifferenceFound = INT_DEFAULT_VALUE;	//not used
											if(checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContext(entityNode, queryEntityNode, &indentationDifferenceFound))
											{
												passReferenceContextMatch = true;
											}
										}
									}
									if(passReferenceContextMatch)
									{
									#endif
										#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
										if(!(entityNode->NLCmathTextParsablePhraseEntity))
										{
										#endif
											#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
											bool passConceptOnlyTraceRequirements = true;
											if(referenceTraceParameters->traceConceptsOnly)
											{
												passConceptOnlyTraceRequirements = false;
												if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT || entityNode->entityType == GIA_ENTITY_TYPE_TYPE_QUALITY)
												{
													passConceptOnlyTraceRequirements = true;
													//rely on previous testReferencedEntityNodeForExactNameMatch2() passSpecificConcepts checks to test entityNode target (connectionIter) for isConcept also
												}
											}
											if(passConceptOnlyTraceRequirements)
											{
											#endif
												#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
												bool passLogicalConditionRequirements = true;
												if(referenceTraceParameters->logicalConditionDisableTraceConcepts)
												{
													if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
													{
														passLogicalConditionRequirements = false;
													}
												}
												if(passLogicalConditionRequirements)
												{
												#endif
													#ifdef GIA_DEBUG
													//cout << "\texactMatch" << endl;
													#endif
													if(simplifiedSearch)
													{
														if(testEntityNodeForQueryOrReferenceSet2(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
														{
															*exactMatch = true;
														}
													}
													else
													{
														if(testEntityNodeForQueryOrReferenceSet(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
														{
															*resultOld = EXACT_MATCH_PASS;

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
															*resultOld = EXACT_MATCH_FAIL;
														}
													}
												#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
												}
												#endif
											#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
											}
											#endif
										#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
										}
										#endif
									#ifdef GIA_SUPPORT_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
									}
									#endif
								#ifdef GIA_ADVANCED_REFERENCING_ENSURE_PLURALITY_MATCHES
								}
								else
								{
									#ifdef GIA_DEBUG
									//cout << "!passPluralityMatch" << endl;
									#endif
								}
								#endif
							#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
							}
							else
							{
								#ifdef GIA_DEBUG
								//cout << "!passSpecificConcepts" << endl;
								//cout << "compareEntityNamesResult: queryEntityNode->entityName = " << queryEntityNode->entityName << ", entityNode->entityName = " << entityNode->entityName << endl;
								#endif
							}
							#endif
						}
						else
						{
							#ifdef GIA_DEBUG
							//cout << "!compareEntityNamesResult" << endl;
							#endif
							*resultOld = EXACT_MATCH_FAIL;
						}
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					}
					else
					{
						#ifdef GIA_DEBUG
						//cout << "!passIntrasentenceReferenceRequirements" << endl;
						//cout << "\t!(entityNode->entityIndexTemp < queryEntityNode->minimumEntityIndexOfReferenceSet)" << endl;
						//cout << "\tentityNode->entityIndexTemp = " << entityNode->entityIndexTemp << endl;
						//cout << "\tqueryEntityNode->minimumEntityIndexOfReferenceSet = " << queryEntityNode->minimumEntityIndexOfReferenceSet << endl;
						#endif
						*resultOld = EXACT_MATCH_FAIL;	//CHECKTHIS
					}
					#endif
				#ifndef GIA_ADVANCED_REFERENCING_ORIGINAL
				}
				else
				{
					#ifdef GIA_DEBUG
					//cout << "(queryEntityNode->idActiveList == entityNode->idActiveList)" << endl;
					#endif
					*resultOld = EXACT_MATCH_FAIL;	//CHECKTHIS
				}
				#endif
			#ifdef GIA_USE_1N1ATEMP1TO8_CHANGES
			}
			else
			{
				#ifdef GIA_DEBUG
				//cout << "(queryEntityNode->referenceSetID == GIA_REFERENCE_SET_ID_UNDEFINED)" << endl;
				#endif
				*resultOld = EXACT_MATCH_OUT_OF_BOUNDS;	//CHECKTHIS
			}
			#endif
		}
		else
		{
			#ifdef GIA_DEBUG
			//cout << "!((queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))" << endl;
			//cout << "\t(referenceTraceParameters->traceModeAssertSameReferenceSetID) = " << (referenceTraceParameters->traceModeAssertSameReferenceSetID) << endl;
			//cout << "\t(queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID)  = " << (queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID)  << endl;
			//cout << "\tqueryEntityNode->referenceSetID = " << queryEntityNode->referenceSetID  << endl;
			//cout << "\treferenceTraceParameters->referenceSetID = " << referenceTraceParameters->referenceSetID  << endl;
			#endif
			*resultOld = EXACT_MATCH_OUT_OF_BOUNDS;
		}
	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	}
	#endif
}




