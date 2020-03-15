/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAquery.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k3a 10-March-2020
 * Requirements: requires a GIA network created for both existing knowledge and the query (question)
 * Description: Query - locates (and tags for highlighting) a given query GIA network (subnet) within a larger GIA network of existing knowledge, and identifies the exact answer if applicable (if a comparison variable has been defined within the GIA query network)
 * ?Limitations: will only locate a exact answer (based upon a comparison node) if it provides the maximum number of matched nodes
 * /
 *******************************************************************************/


#include "GIAquery.hpp"
#ifdef USE_WORDNET
#endif


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


	/*
	queryTraceParametersTemp.isCondition;					//not required for testEntityNodeForQuery
	queryTraceParametersTemp.sourceEntityNode;				//not required for testEntityNodeForQuery
	queryTraceParametersTemp.sourceContext;					//not required for testEntityNodeForQuery
	*/
	
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES_QUERIES
	skipOverPropertyAndDefinitionRelationshipEntities = true;
	#else
	skipOverPropertyAndDefinitionRelationshipEntities = false;
	#endif
	#endif
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

	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES_QUERIES
	skipOverPropertyAndDefinitionRelationshipEntities = true;
	#else
	skipOverPropertyAndDefinitionRelationshipEntities = false;
	#endif
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

	referenceSetDefiniteEntity = NULL;
	#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
	testReferenceSetContext = false;
	//firstSentenceInList = NULL;
	#endif
	ensureSameReferenceSetQueryConnections = false;

	#ifdef GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS
	sameReferenceSetTests = false;
	#endif

	#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
	traceConceptsOnly = false;
	#endif
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	logicalConditionDisableTraceConcepts = false;
	#endif
	
	#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
	skipOverPropertyAndDefinitionRelationshipEntities = true;
	#endif
	
	#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
	exactReferenceSetSearch = false;
	#endif
}
GIAreferenceTraceParameters::~GIAreferenceTraceParameters(void)
{
}


#ifdef GIA_QUERY_SIMPLIFIED_SEARCH
GIAentityNode* GIAqueryClass::answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork2(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery, const bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, const bool* foundAnswer, GIAentityNode* queryAnswerNode, const double* numberOfMatchedNodes, const string* queryAnswerContext)
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
			if(currentQueryEntityNode->entityName != GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)	//added 22 August 2012
			{
				bool foundQueryEntityNodeName = false;
				string queryEntityNodeName = currentQueryEntityNode->entityName;

				GIAentityNode* networkIndexEntityMatchingCurrentQueryEntity = GIAdatabase.findOrAddNetworkIndexEntityNodeByName(NULL, entityNodesActiveListNetworkIndexes, &queryEntityNodeName, &foundQueryEntityNodeName, false, NULL, NULL, false);

				if(foundQueryEntityNodeName)
				{

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
					traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);
					traceEntityNode(networkIndexEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);
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



bool GIAqueryClass::testEntityNodeForQueryOrReferenceSet2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool exactMatch = true;
						
	if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparisonTemp))
	{
		entityNode->testedForQueryComparison = true;
		queryEntityNode->testedForQueryComparison = true;
		entityNode->testedForQueryComparisonTemp = false;
		queryEntityNode->testedForQueryComparisonTemp = false;

		if(entityNode->entityType != GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
		}

		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
			if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)	//Removed && (traceModeIsQuery) GIA 2f19b 23-July-2014 (do not trace instantinations for queries only)
			{
				pass = false;
			}
			#endif
			if(pass)
			{
				for(vector<GIAentityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[connectionType].end(); connectionIterQuery++)
				{
					GIAentityNode* queryEntityNodeTarget = (*connectionIterQuery)->entity;
					if(traceModeIsQuery || !(referenceTraceParameters->ensureSameReferenceSetQueryConnections) || (*connectionIterQuery)->sameReferenceSet)
					{
						#ifdef GIA_DATABASE
						#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
						if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
						{
							GIAdatabase.DBreadVectorConnections(entityNode, connectionType);		//this is important, as it will read all of the vector connections from the database for this node (conferred 25 May 2012)
						}
						#endif
						#endif

						int maxNumberMatchedNodes = 0;
						bool foundBestAnswerCandidate = false;
						GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
						#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
						bool atLeastOneExactMatch = false;
						#endif
						
						for(vector<GIAentityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].rend(); connectionIter++)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected)
						{
							GIAentityNode* entityNodeTarget = (*connectionIter)->entity;
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
								#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
								if((!traceModeIsQuery && referenceTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities) || (traceModeIsQuery && queryTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities))
								{
									//GIA3a1a: skip over property/definition enities (go directly to their object)
									bool skipRelationshipEntity = false;
									if(((queryEntityNodeTarget->entityType == GIA_ENTITY_TYPE_PROPERTY) && (entityNodeTarget->entityType == GIA_ENTITY_TYPE_PROPERTY)) ||
									((queryEntityNodeTarget->entityType == GIA_ENTITY_TYPE_DEFINITION) && (entityNodeTarget->entityType == GIA_ENTITY_TYPE_DEFINITION)))
									{
										if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION))
										{
											//cout << "skipping2" << endl;
											#ifdef GIA_DATABASE
											#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
											if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
											{
												GIAdatabase.DBreadVectorConnections(entityNodeTarget, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT);
											}
											#endif
											#endif
											//cout << "skipRelationshipEntity" << endl;
											skipRelationshipEntity = true;
											queryEntityNodeTarget = GIAtranslatorOperations.getRelationshipObjectEntity(queryEntityNodeTarget);
											entityNodeTarget = GIAtranslatorOperations.getRelationshipObjectEntity(entityNodeTarget);
										}
										else if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE))
										{
											//cout << "skipping2" << endl;
											#ifdef GIA_DATABASE
											#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
											if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
											{
												GIAdatabase.DBreadVectorConnections(entityNodeTarget, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT);
											}
											#endif
											#endif
											//cout << "skipRelationshipEntity" << endl;
											skipRelationshipEntity = true;
											queryEntityNodeTarget = GIAtranslatorOperations.getRelationshipSubjectEntity(queryEntityNodeTarget);
											entityNodeTarget = GIAtranslatorOperations.getRelationshipSubjectEntity(entityNodeTarget);							
										}
									}
								}
								#endif							
						
								int numberOfMatchedNodesTemp = 0;
								int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

								#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
								bool exactReferenceSetSearch = false;
								int maxNumberOfMatchedNodesPossibleQuery = 0;
								int maxNumberOfMatchedNodesPossible = 0;
								if((!traceModeIsQuery && referenceTraceParameters->exactReferenceSetSearch))
								{	
									exactReferenceSetSearch = true;
									int irrelevantInt;
									string irrelevantString = "";
									bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
									traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY, &maxNumberOfMatchedNodesPossibleQuery, NULL, queryEntityNodeTarget->referenceSetID, traceInstantiations);
									traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
									traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_INDEPENDENT, &maxNumberOfMatchedNodesPossible, NULL, entityNodeTarget->referenceSetID, traceInstantiations);
									traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
									#ifdef GIA_DEBUG_QUERY2
									if(entityNodeTarget->entityName == queryEntityNodeTarget->entityName)
									{
										cout << "\t\t\t\t entityNodeTarget = " << entityNodeTarget->entityName << ", " << entityNodeTarget->entityType << endl;
										cout << "\t\t\t\t queryEntityNodeTarget = " << queryEntityNodeTarget->entityName << ", " << entityNodeTarget->entityType << endl;
										cout << "\t\t\t\t\t maxNumberOfMatchedNodesPossible = " << maxNumberOfMatchedNodesPossible << endl;
										cout << "\t\t\t\t\t maxNumberOfMatchedNodesPossibleQuery = " << maxNumberOfMatchedNodesPossibleQuery << endl;
									}
									#endif
								}
								#endif
								
								bool exactMatchTemp = testReferencedEntityNodeForExactNameMatch2(queryEntityNodeTarget, entityNodeTarget, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

								#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
								if(!exactReferenceSetSearch || (maxNumberOfMatchedNodesPossible == maxNumberOfMatchedNodesPossibleQuery))
								{
								#endif
									if(numberOfMatchedNodesTemp > maxNumberMatchedNodes)
									{
										if(traceModeIsQuery || exactMatchTemp)
										{
											//cout << "numberOfMatchedNodesTemp = " << numberOfMatchedNodesTemp << endl;
											maxNumberMatchedNodes = numberOfMatchedNodesTemp;
											foundBestAnswerCandidate = true;
											networkEntityWithMaxNumberNodesMatched = entityNodeTarget;
										}
									}
									#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
									if(exactMatchTemp)
									{
										atLeastOneExactMatch = true;
									}
									#endif
								#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
								}
								#endif
											
								//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
								int irrelevantInt;
								string irrelevantString = "";
								bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;		//clear all (why is this still required if GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS is off? - it is based on testing, but unknown as to why)
								traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
								traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);

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
								cout << "foundBestAnswerCandidate: queryEntityNodeTarget->entityName = " << queryEntityNodeTarget->entityName << endl;
								cout << "foundBestAnswerCandidate: (*connectionIterQuery) idInstance = " << queryEntityNodeTarget->idInstance << endl;
								cout << "foundBestAnswerCandidate: (*connectionIterQuery) entityIndexTemp = " << queryEntityNodeTarget->entityIndexTemp << endl;
							}
							*/

							if(knownBestMatch)
							{
								queryEntityNodeTarget->entityCorrespondingBestMatch = networkEntityWithMaxNumberNodesMatched;		//this shouldn't be required for queries....
							}

							int numberOfMatchedNodesTemp = *numberOfMatchedNodes;
							int numberOfMatchedNodesRequiredSynonymnDetectionTemp = *numberOfMatchedNodesRequiredSynonymnDetection;
							if(!testReferencedEntityNodeForExactNameMatch2(queryEntityNodeTarget, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, knownBestMatch, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters))
							{
								//for advanced referencing this should never be the case (it should always refind what was found originally)
								exactMatch = false;
							}
							*numberOfMatchedNodes = numberOfMatchedNodesTemp;
							*numberOfMatchedNodesRequiredSynonymnDetection = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
						}

						#ifndef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH
						if((queryEntityNodeTarget->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))
						{
							if(!atLeastOneExactMatch)
							{
								exactMatch = false;
							}
						}
						#endif
					}
				}
			}
		}

		entityNode->testedForQueryComparison = false;
		queryEntityNode->testedForQueryComparison = false;
		entityNode->testedForQueryComparisonTemp = true;
		queryEntityNode->testedForQueryComparisonTemp = true;
	}

	return exactMatch;
}



bool GIAqueryClass::testReferencedEntityNodeForExactNameMatch2(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool exactMatch = false;
	string sourceContextCurrent = queryTraceParameters->sourceContext;
	if(entityNode != queryEntityNode)	//added 2i19c
	{
		if((entityNode->testedForQueryComparison) && !(queryEntityNode->testedForQueryComparison))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		}
		else if((entityNode->testedForQueryComparisonTemp) && !(queryEntityNode->testedForQueryComparisonTemp))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		}
		else if((queryEntityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparison))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		}
		else if((queryEntityNode->testedForQueryComparisonTemp) && !(entityNode->testedForQueryComparisonTemp))
		{
			//KB entity traced, but query entity not traced... implies have chosen wrong trace path
		}
		else if(!(entityNode->testedForQueryComparison) && !(entityNode->testedForQueryComparisonTemp))
		{
			bool compareEntityNamesResult = compareEntityStandard(queryEntityNode, entityNode, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

			if(traceModeIsQuery)
			{
				bool foundMatch = false;

				if(compareEntityNamesResult)
				{
					if(queryTraceParameters->detectComparisonVariable)
					{
						if(queryTraceParameters->comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
						{//exact match found [NB if a quantity, the queryEntityNode's entityName will not have the comparisonVariable name (_$qVar) specified, and therefore a matched entity node entityName is required]
							foundMatch = true;
						}
						#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES		//also declared suitable to support name queries GIA_ALIASES
						else if(queryEntityNode->isQuery)
						{//added support for which query (alternate method of comparison node detection/designation/definition)
							foundMatch = true;
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

						#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
						if(queryEntityNode->isWhichOrEquivalentWhatQuery)
						{
							/*
							if a 'which' query, then verify that the entityNode is defined by the comparisonVariableNode [ie has a definition corresponding to the comparisonVariableNode]
							eg1 a dog eats the mud. dogs are animals. / which animal eats the mud?	[answer: 'dog' - which is an instance of 'dog' networkIndex node, where the 'dog' networkIndex node is defined by 'animal'
								NB answer context text = "eat mud is done by dog" ['eat' is the first node traced, and 'dog' is the answer found'. NB the reason 'mud' is added to the answer context text, is because it is the actionRelationshipObjectEntity, which is parsed after actionRelationshipSubjectEntity in testEntityNodeForQuery {ie, after answer 'dog' has already been found}]
									for this example, need to then verify that the answer 'dog' is defined in the primary semantic network as an animal
							*/
							if(!verifyThatAnswerEntityIsDefinedByComparisonVariableNode(entityNode, queryTraceParameters->comparisonVariableNode->entityName))
							{
								foundMatch = false;
							}
							else
							{
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

				}

				bool nonMatchingConditions = false;
				if(!compareEntityNamesResult && queryTraceParameters->isCondition)		//now supports unimplemented preposition reduction (need to modify compareEntitySynonyms() to support synonomous prepositions), 	OLD: before 21 May 2012: if((queryEntityNode->entityName != entityNode->entityName) && queryTraceParameters->isCondition)
				{
					nonMatchingConditions = true;
				}

				if((compareEntityNamesResult || nonMatchingConditions || foundMatch) && !(queryTraceParameters->nonMatchingSourceConditions))	//do not continue trace if non-matching source conditions	//allow trace past the answer
				{
					queryTraceParameters->nonMatchingSourceConditions = nonMatchingConditions;

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
						entityNode->queryAnswerContext = true;
					}
				}
				#endif
			}
			else
			{
				int resultOldTemp = EXACT_MATCH_OUT_OF_BOUNDS;
				compareEntityReferenceTrace(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters, compareEntityNamesResult, &exactMatch, true, &resultOldTemp);
			}
		}
		else
		{
			exactMatch = true;
		}
	}

	return exactMatch;
}
#endif


GIAentityNode* GIAqueryClass::answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(GIAtranslatorVariablesClass* translatorVariables, GIAtranslatorVariablesClass* translatorVariablesQuery, const bool detectComparisonVariable, GIAentityNode* comparisonVariableNode, bool* foundAnswer, GIAentityNode* queryAnswerNode, double* numberOfMatchedNodes, string* queryAnswerContext)
{
	unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes = translatorVariables->entityNodesActiveListNetworkIndexes;
	unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery = translatorVariablesQuery->entityNodesActiveListNetworkIndexes;
							
	bool traceModeIsQuery = TRACE_MODE_IS_QUERY_TRUE;

	int numberOfMatchedNodesTempMax = 0;
	int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = 0;
	bool foundAnAnswer = false;	//this is not really used, and is more of an artefact...

	GIAentityNode* networkEntityWithMaxNumberNodesMatched = NULL;
	GIAentityNode* queryEntityWithMaxNumberNodesMatched = NULL;

	bool foundAtLeastOneMatch = false;

	for(unordered_map<string, GIAentityNode*>::iterator entityIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); entityIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); entityIterQuery++)
	{//for each node in query semantic net;

		GIAentityNode* currentQueryEntityNode = entityIterQuery->second;

		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		if(!(currentQueryEntityNode->disabled))
		{
		#endif		
						
			if(currentQueryEntityNode->entityName != GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)	//added 22 August 2012
			{
				bool foundQueryEntityNodeName = false;
				string queryEntityNodeName = currentQueryEntityNode->entityName;
				
				GIAentityNode* networkIndexEntityMatchingCurrentQueryEntity = GIAdatabase.findOrAddNetworkIndexEntityNodeByName(NULL, entityNodesActiveListNetworkIndexes, &queryEntityNodeName, &foundQueryEntityNodeName, false, NULL, NULL, false);

				if(foundQueryEntityNodeName)
				{
				
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


						if(queryTraceParametersTemp.foundAnswer)
						{//if comparisonVariable is identified within the query, then an exact answer is required...

							foundAnAnswer = true;
						}
					}

					//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
					int irrelevant;
					string printEntityNodeString = "";
					bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
					traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);
					traceEntityNode(networkIndexEntityMatchingCurrentQueryEntity, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);
				}
			}
		#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
		}
		#endif
	}

	if(foundAtLeastOneMatch)
	{
		int numberOfMatchedNodesTemp = 0;
		int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

		GIAqueryTraceParameters queryTraceParameters;
		queryTraceParameters.detectComparisonVariable = detectComparisonVariable;
		queryTraceParameters.comparisonVariableNode = comparisonVariableNode;

		GIAreferenceTraceParameters referenceTraceParameters;	//irrelevant

		testEntityNodeForQueryOrReferenceSet(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, &numberOfMatchedNodesTemp, true, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParameters, &referenceTraceParameters);
		//queryAnswerNodeTemp = testEntityNodeForQuery(queryEntityWithMaxNumberNodesMatched, networkEntityWithMaxNumberNodesMatched, detectComparisonVariable, comparisonVariableNode, &foundAnswerTemp, queryAnswerNodeTemp, &numberOfMatchedNodesTemp, true, &queryAnswerContextTemp, false, false);


		/*
		#ifdef GIA_1N1ATEMP1TO8_CHANGES
		//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
		int irrelevant;
		string printEntityNodeString = "";
		bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
		traceEntityNode(queryEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);		//is this required?	//added 13 July 2012
		traceEntityNode(networkEntityWithMaxNumberNodesMatched, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);				//added 13 July 2012
		#endif
		*/

		if(detectComparisonVariable)
		{
			if(queryTraceParameters.foundAnswer)
			{

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

	return queryAnswerNode;
}




int GIAqueryClass::testReferencedEntityNodeForExactNameMatch(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters)
{
	int result = EXACT_MATCH_OUT_OF_BOUNDS;
	string sourceContextCurrent = queryTraceParameters->sourceContext;
	if(entityNode != queryEntityNode)	//added 2i19c
	{
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

			bool compareEntityNamesResult = compareEntityStandard(queryEntityNode, entityNode, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);

			if(traceModeIsQuery)
			{

				bool foundMatch = false;

				if(compareEntityNamesResult)
				{
					if(queryTraceParameters->detectComparisonVariable)
					{
						if(queryTraceParameters->comparisonVariableNode->hasQuantity && queryEntityNode->hasQuantity && entityNode->hasQuantity)
						{//exact match found [NB if a quantity, the queryEntityNode's entityName will not have the comparisonVariable name (_$qVar) specified, and therefore a matched entity node entityName is required]
							foundMatch = true;
						}
						#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES		//also declared suitable to support name queries GIA_ALIASES
						else if(queryEntityNode->isQuery)
						{//added support for which query (alternate method of comparison node detection/designation/definition)
							foundMatch = true;
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

						#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
						if(queryEntityNode->isWhichOrEquivalentWhatQuery)
						{
							/*
							if a 'which' query, then verify that the entityNode is defined by the comparisonVariableNode [ie has a definition corresponding to the comparisonVariableNode]
							eg1 a dog eats the mud. dogs are animals. / which animal eats the mud?	[answer: 'dog' - which is an instance of 'dog' networkIndex node, where the 'dog' networkIndex node is defined by 'animal'
								NB answer context text = "eat mud is done by dog" ['eat' is the first node traced, and 'dog' is the answer found'. NB the reason 'mud' is added to the answer context text, is because it is the actionRelationshipObjectEntity, which is parsed after actionRelationshipSubjectEntity in testEntityNodeForQuery {ie, after answer 'dog' has already been found}]
									for this example, need to then verify that the answer 'dog' is defined in the primary semantic network as an animal

							*/
							if(!verifyThatAnswerEntityIsDefinedByComparisonVariableNode(entityNode, queryTraceParameters->comparisonVariableNode->entityName))
							{
								foundMatch = false;
							}
							else
							{
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

					//CHECKTHIS; need to take into account vector of answers (not just a single answer)

					result = EXACT_MATCH_PASS;

					#ifndef GIA_QUERY_TRACE_PAST_ANSWER	//changed to Temp 21 May 2012
					entityNode->testedForQueryComparisonTemp = true;
					if(entityNode->entityType != GIA_ENTITY_TYPE_NETWORK_INDEX)
					{
						*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
					}
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
							}
						}
						if(!answerAlreadyAddedToMultipleAnswers)
						{
							queryTraceParameters->queryAnswerNodes.push_back(entityNode);
							queryTraceParameters->queryAnswerContexts.push_back("");
						}
					}
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
					if(foundMatch)
					{
						entityNode->testedForQueryComparisonTemp = true;
						if(entityNode->entityType != GIA_ENTITY_TYPE_NETWORK_INDEX)
						{
							*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
						}
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
						}
					}
					#endif
				}
				#endif


			}
			else
			{
				bool exactMatchTemp = false;
				compareEntityReferenceTrace(queryEntityNode, entityNode, numberOfMatchedNodes, knownBestMatch, numberOfMatchedNodesRequiredSynonymnDetection, traceModeIsQuery, queryTraceParameters, referenceTraceParameters, compareEntityNamesResult, &exactMatchTemp, false, &result);
			}
		}
		else
		{
			result = EXACT_MATCH_OUT_OF_BOUNDS;
		}
	}

	return result;
}





bool GIAqueryClass::testEntityNodeForQueryOrReferenceSet(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters)
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
		entityNode->testedForQueryComparisonTemp = false;		//added 13 July 2012 [Should not be required - redundant]
		queryEntityNode->testedForQueryComparisonTemp = false;		//added 13 July 2012 [Should not be required - redundant]



		if(entityNode->entityType != GIA_ENTITY_TYPE_NETWORK_INDEX)
		{
			if(traceModeIsQuery)
			{
				#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
				if(!(queryTraceParameters->nonMatchingSourceConditions))
				{
				#endif
					*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
				#ifdef GIA_QUERY_DO_NOT_ASSIGN_MATCH_POINTS_FOR_NON_IDENTICAL_CONDITION_TYPES
				}
				#endif
			}
			else
			{
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
			}
		}



		/*NOT FINISHED AND WILL NOT BE IMPLEMENTED - this is only required where there exists a discrepancy between the way in which the query net and the text net have been formed;
		//special: having and being connections to conditions....
		if(traceModeIsQuery)
		{
			if(queryTraceParameters->nonMatchingSourceConditions)
			{
				if(queryTraceParameters->detectComparisonVariable)
				{
					if(queryEntityNode->relationshipObjectEntity != NULL)
					{
						queryTraceParameters->isCondition = false;
						queryTraceParameters->sourceEntityNode = entityNode;

						//NB entityNode and queryEntityNode are conditions
						for(vector<GIAentityNode*>::iterator connectionIter = entityNode->definitionNodeList.begin(); connectionIter != entityNode->definitionNodeList.end(); connectionIter++)
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


		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			bool pass = true;
			#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
			if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)	//Removed && (traceModeIsQuery) GIA 2f19b 23-July-2014 (do not trace instantinations for queries only)
			{
				pass = false;
			}
			#endif
			if(pass)
			{
				for(vector<GIAentityConnection*>::iterator connectionIterQuery = queryEntityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIterQuery != queryEntityNode->entityVectorConnectionsArray[connectionType].end(); connectionIterQuery++)
				{	
					GIAentityNode* queryEntityNodeTarget = (*connectionIterQuery)->entity;
					
					if(traceModeIsQuery || !(referenceTraceParameters->ensureSameReferenceSetQueryConnections) || (*connectionIterQuery)->sameReferenceSet)
					{
						bool alreadyFoundAnAnswer = false;
						if(traceModeIsQuery)
						{
							queryTraceParameters->sourceContext = entityVectorConnectionSourceContextArray[connectionType];

							queryTraceParameters->isCondition = entityVectorConnectionIsConditionArray[connectionType];
							queryTraceParameters->sourceEntityNode = entityNode;
							if(queryTraceParameters->foundAnswer)
							{
								alreadyFoundAnAnswer = true;
							}
							#ifndef GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS
							if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE)	//check: do not trace instantinations for queries only
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

						#ifdef GIA_DATABASE
						#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
						if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
						{
							GIAdatabase.DBreadVectorConnections(entityNode, connectionType);		//this is important, as it will read all of the vector connections from the database for this node (conferred 25 May 2012)
						}
						#endif
						#endif


						//for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
						for(vector<GIAentityConnection*>::reverse_iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].rbegin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].rend(); connectionIter++)	//always search from end position first (to take the latest/newest reference/answer, if equal number of matched nodes is detected)
						{
							GIAentityNode* entityNodeTarget = (*connectionIter)->entity;
							
							#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
							if((!traceModeIsQuery && referenceTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities) || (traceModeIsQuery && queryTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities))
							{
								//GIA3a1a: skip over property/definition enities (go directly to their object)
								bool skipRelationshipEntity = false;
								if(((queryEntityNodeTarget->entityType == GIA_ENTITY_TYPE_PROPERTY) && (entityNodeTarget->entityType == GIA_ENTITY_TYPE_PROPERTY)) ||
								((queryEntityNodeTarget->entityType == GIA_ENTITY_TYPE_DEFINITION) && (entityNodeTarget->entityType == GIA_ENTITY_TYPE_DEFINITION)))
								{
									if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION))
									{
										//cout << "skipping3" << endl;
										#ifdef GIA_DATABASE
										#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
										if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
										{
											GIAdatabase.DBreadVectorConnections(entityNodeTarget, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT);
										}
										#endif
										#endif
										skipRelationshipEntity = true;
										queryEntityNodeTarget = GIAtranslatorOperations.getRelationshipObjectEntity(queryEntityNodeTarget);
										entityNodeTarget = GIAtranslatorOperations.getRelationshipObjectEntity(entityNodeTarget);
									}
									else if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE))
									{
										//cout << "skipping3" << endl;
										#ifdef GIA_DATABASE
										#ifndef GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ
										if(GIAdatabase.getUseDatabase() == GIA_DATABASE_TRUE_READ_ACTIVE)
										{
											GIAdatabase.DBreadVectorConnections(entityNodeTarget, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT);
										}
										#endif
										#endif
										skipRelationshipEntity = true;
										queryEntityNodeTarget = GIAtranslatorOperations.getRelationshipSubjectEntity(queryEntityNodeTarget);
										entityNodeTarget = GIAtranslatorOperations.getRelationshipSubjectEntity(entityNodeTarget);							
									}
								}
							}
							#endif
									

							GIAqueryTraceParameters queryTraceParametersTemp(queryTraceParameters);

							int numberOfMatchedNodesTemp = 0;
							int numberOfMatchedNodesRequiredSynonymnDetectionTemp = 0;

							#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
							int maxNumberOfMatchedNodesPossibleQuery = 0;
							int maxNumberOfMatchedNodesPossible = 0;
							bool exactReferenceSetSearch = false;
							if((!traceModeIsQuery && referenceTraceParameters->exactReferenceSetSearch))
							{
								exactReferenceSetSearch = true;
								int irrelevantInt;
								string irrelevantString = "";
								bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
								traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY, &maxNumberOfMatchedNodesPossibleQuery, NULL, queryEntityNodeTarget->referenceSetID, traceInstantiations);
								traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
								traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_INDEPENDENT, &maxNumberOfMatchedNodesPossible, NULL, entityNodeTarget->referenceSetID, traceInstantiations);
								traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
							}
							#endif
								
							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(queryEntityNodeTarget, entityNodeTarget, &numberOfMatchedNodesTemp, false, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, &queryTraceParametersTemp, referenceTraceParameters);
							
							if(exactMatchTemp == EXACT_MATCH_FAIL)
							{
								foundExactMatchFail = true;
							}

							#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
							if(!exactReferenceSetSearch || (maxNumberOfMatchedNodesPossible == maxNumberOfMatchedNodesPossibleQuery))
							{
							#endif
								if(exactMatchTemp == EXACT_MATCH_PASS)
								{
									#ifndef GIA_ADVANCED_REFERENCING_UPDATE	//CHECKTHIS, OLD: GIA_ADVANCED_REFERENCING_UPDAT
									foundExactMatchPass = true;
									#endif
								}
								
								#ifdef GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS
								if(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)
								{
								#endif
									bool bestAnswerCandidate = determineIfBestAnswerCandidate(traceModeIsQuery, queryTraceParametersTemp.foundAnswer, alreadyFoundAnAnswer, numberOfMatchedNodesTemp, numberOfMatchedNodesTempMax, numberOfMatchedNodesRequiredSynonymnDetectionTemp, numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, referenceTraceParameters->traceMode, exactMatchTemp);
									if(bestAnswerCandidate)
									{
										#ifdef GIA_ADVANCED_REFERENCING_UPDATE
										foundExactMatchPass = true;
										#endif

										queryEntityCorrespondingBestMatch = entityNodeTarget;
										numberOfMatchedNodesTempMax = numberOfMatchedNodesTemp;
										numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
									}
									else
									{
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
														if(numberOfMatchedNodesTemp > queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)		//do not record deficient answers [those are known to provide less node matches than the previously recorded answer(s)]
														{

															//current answer provides better match... clear previous answer nodes...
															queryTraceParameters->queryAnswerNodes.clear();
															queryTraceParameters->queryAnswerContexts.clear();
															queryTraceParameters->numberAnswersFound = 0;
														}

														if(numberOfMatchedNodesTemp >= queryTraceParameters->numberOfMatchedNodesAtPreviousAnswerNode)
														{
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
								#ifdef GIA_QUERY_DO_NOT_RECORD_IF_COMPLETELY_MISMATCHED_TRACE_PATHS
								}
								else
								{
								}
								#endif
							#ifdef GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH_CONCEPTS
							}
							#endif
							
							//now reset the matched nodes as unpassed (required such that they are retracable using a the different path)
							int irrelevantInt;
							string irrelevantString = "";
							bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;		//clear all (why is this still required if GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS is off? - it is based on testing, but unknown as to why)
							traceEntityNode(entityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);
							traceEntityNode(queryEntityNodeTarget, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP, &irrelevantInt, &irrelevantString, NULL, traceInstantiations);

						}


						bool exactMatchFoundTemp = false;
						#ifdef GIA_ADVANCED_REFERENCING_UPDATE
						if(foundExactMatchPass)
						{
							exactMatchFoundTemp = true;
						}
						#else
						if(!foundExactMatchFail || foundExactMatchPass)
						{
							exactMatchFoundTemp = true;
						}
						#endif
						
						#ifdef GIA_ADVANCED_REFERENCING_UPDATE_HYBRID
						if(!foundExactMatchFail && !foundExactMatchPass)
						#else
						if(!exactMatchFoundTemp)
						#endif
						{
							exactMatch = false;	//query was not perfectly tracable at/from this node (queryEntityNode)
						}

						bool matchFound = determineMatchParameters(exactMatchFoundTemp, traceModeIsQuery, referenceTraceParameters->traceMode, numberOfMatchedNodesTempMax);

						if(matchFound)
						{
							if(knownBestMatch)
							{
								queryEntityNodeTarget->entityCorrespondingBestMatch = queryEntityCorrespondingBestMatch;		//this shouldn't be required for queries....
							}
							//now set the matched nodes as already passed (required such that they are not retraced...)

							int numberOfMatchedNodesTemp = *numberOfMatchedNodes;
							int numberOfMatchedNodesRequiredSynonymnDetectionTemp = *numberOfMatchedNodesRequiredSynonymnDetection;

							int exactMatchTemp = testReferencedEntityNodeForExactNameMatch(queryEntityNodeTarget, queryEntityCorrespondingBestMatch, &numberOfMatchedNodesTemp, knownBestMatch, &numberOfMatchedNodesRequiredSynonymnDetectionTemp, traceModeIsQuery, queryTraceParameters, referenceTraceParameters);			//numberOfMatchedNodesTemp, numberOfMatchedNodesRequiredSynonymnDetectionTemp

							*numberOfMatchedNodes = numberOfMatchedNodesTemp;
							*numberOfMatchedNodesRequiredSynonymnDetection = numberOfMatchedNodesRequiredSynonymnDetectionTemp;
						}
						else
						{
						}
					}
				}
			}
		}

		entityNode->testedForQueryComparison = false;
		queryEntityNode->testedForQueryComparison = false;
		entityNode->testedForQueryComparisonTemp = true;
		queryEntityNode->testedForQueryComparisonTemp = true;
	}

	return exactMatch;
}

bool GIAqueryClass::determineIfBestAnswerCandidate(const bool traceModeIsQuery, const bool queryTraceParametersTempFoundAnswer, const bool alreadyFoundAnAnswer, const int numberOfMatchedNodesTemp, const int numberOfMatchedNodesTempMax, const int numberOfMatchedNodesRequiredSynonymnDetectionTemp, const int numberOfMatchedNodesRequiredSynonymnDetectionTempAtMax, const int referenceTraceParametersTraceMode, const int exactMatchTemp)
{
	bool bestAnswerCandidate = false;

	if(exactMatchTemp != MATCH_FAIL_COMPLETELY_MISMATCHED_TRACE_PATHS)
	{
		if(traceModeIsQuery)
		{
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

bool GIAqueryClass::determineMatchParameters(const bool exactMatchFoundTemp, const bool traceIsQuery, const int referenceTraceParametersTraceMode, const int numberOfMatchedNodesTempMax)
{
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

bool GIAqueryClass::verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAentityNode* entityNode, const string comparisonVariableNodeName)
{
	bool definitionFound = false;

	if(entityNode->entityName == comparisonVariableNodeName)
	{
		definitionFound = true;
	}
	else
	{
		if(!(entityNode->instanceReverseNodeList->empty()))
		{
			if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entityNode), comparisonVariableNodeName))
			{
				definitionFound = true;
			}
		}

		if(!(entityNode->definitionNodeList->empty()))
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->definitionNodeList->begin(); connectionIter < entityNode->definitionNodeList->end(); connectionIter++)
			{
				GIAentityNode* relationshipObjectEntity = GIAtranslatorOperations.getDefinitionRelationshipTargetEntity((*connectionIter)->entity);
				if(verifyThatAnswerEntityIsDefinedByComparisonVariableNode(relationshipObjectEntity, comparisonVariableNodeName))
				{
					definitionFound = true;
				}
			}
		}
	}

	return definitionFound;
}


#ifdef GIA_SYNONYMN_DETECTION
bool GIAqueryClass::compareEntitySynonyms(GIAentityNode* queryEntityNode, GIAentityNode* entityNode)
{
	bool entityNamesAreSynonymous = false;

	#ifndef USE_WORDNET
	cerr << "compareEntitySynonyms{} error: requires USE_WORDNET" << endl;
	exit(EXIT_ERROR);
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
	}
	else
	{
		if(GIAwordnet.checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(queryEntityNode->entityName), entityNode->grammaticalWordTypeTemp))
		{
			entityNamesAreSynonymous = true;
		}

		#ifdef GIA_ALIASES
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
			{
				if(GIAwordnet.checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(*aliasIterQuery), entityNode->grammaticalWordTypeTemp))
				{
					entityNamesAreSynonymous = true;
				}
			}
		}
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter < entityNode->aliasList.end(); aliasIter++)
		{
			if(GIAwordnet.checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(*aliasIter), &(queryEntityNode->entityName), entityNode->grammaticalWordTypeTemp))
			{
				entityNamesAreSynonymous = true;
			}
		}
		for(vector<string>::iterator aliasIterQuery = queryEntityNode->aliasList.begin(); aliasIterQuery < queryEntityNode->aliasList.end(); aliasIterQuery++)
		{
			if(GIAwordnet.checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(&(entityNode->entityName), &(*aliasIterQuery), entityNode->grammaticalWordTypeTemp))
			{
				entityNamesAreSynonymous = true;
			}
		}
		#endif
	}

	/*
	}
	*/

	return entityNamesAreSynonymous;
}
#endif

bool GIAqueryClass::compareEntityAliases(GIAentityNode* queryEntityNode, GIAentityNode* entityNode)
{
	bool aliasMatchFound = false;

	#ifdef GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES
	if(!(queryEntityNode->disabled) && !(entityNode->disabled))
	{
	#endif

		#ifdef GIA_ALIASES

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


void GIAqueryClass::generateTexualContextWithPreviousNodeForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode, GIAentityNode* entityNodePrevious)
{
	generateTexualContextEntityStringForwards(queryAnswerContext, entityNodePrevious);

	*queryAnswerContext = *queryAnswerContext + sourceContext;

	generateTexualContextEntityStringForwards(queryAnswerContext, entityNode);

	*queryAnswerContext = *queryAnswerContext + ". ";
}

void GIAqueryClass::generateTexualContextForwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = *queryAnswerContext + sourceContext + texualContextEntityString;
}

void GIAqueryClass::generateTexualContextBackwards(string* queryAnswerContext, string sourceContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = sourceContext + texualContextEntityString +* queryAnswerContext;
}

void GIAqueryClass::generateTexualContextEntityStringForwards(string* queryAnswerContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = *queryAnswerContext + texualContextEntityString;
}

void GIAqueryClass::generateTexualContextEntityStringBackwards(string* queryAnswerContext, GIAentityNode* entityNode)
{
	string texualContextEntityString = "";
	generateTexualContextEntityString(&texualContextEntityString, entityNode);

	*queryAnswerContext = texualContextEntityString +* queryAnswerContext;
}

void GIAqueryClass::generateTexualContextEntityString(string* texualContextEntityString, GIAentityNode* entityNode)
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
		string quantityNumberStringTemp = GIAentityNodeClass.printQuantityNumberString(entityNode);
		entityPretext = entityPretext + quantityNumberStringTemp + " ";
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_ACTION)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_CONDITION)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_PROPERTY)
	{
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_CONDITION)
	{
	}
	else
	{//networkIndex
		entityPretext = entityPretext + "a ";
	}

	*texualContextEntityString =  entityPretext + entityNode->entityName + " " + entityPosttext;
}




double GIAqueryClass::determineMaxConfidenceOfQuerySemanticNetwork(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexesQuery)
{
	double maxNumberOfMatchedNodes = 0.0;

	for(unordered_map<string, GIAentityNode*>::iterator connectionIterQuery = entityNodesActiveListNetworkIndexesQuery->begin(); connectionIterQuery != entityNodesActiveListNetworkIndexesQuery->end(); connectionIterQuery++)
	{//for each node in query semantic net;

		GIAentityNode* currentQueryEntityNode = connectionIterQuery->second;

		int numberOfMatchedNodes = 0;
		string queryAnswerContextTemp = "";
		bool traceInstantiations = GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS_VALUE;
		traceEntityNode(currentQueryEntityNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES, &numberOfMatchedNodes, &queryAnswerContextTemp, NULL, traceInstantiations);

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


void GIAqueryClass::printEntityNodeQualitiesOnly(GIAentityNode* entityNode, string* printEntityNodeString)
{
	int numberQualities = 0;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->propertyNodeList->begin(); connectionIter < entityNode->propertyNodeList->end(); connectionIter++)
	{
		GIAentityNode* substanceNode = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(*connectionIter);
		if(substanceNode->entityType == GIA_ENTITY_TYPE_QUALITY)
		{
			if(numberQualities > 0)
			{
				*printEntityNodeString = *printEntityNodeString + "\n";
			}
			else
			{
				*printEntityNodeString = *printEntityNodeString + " (";
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

void GIAqueryClass::printEntityTimeConditionNodeOnly(GIAentityNode* entityNode, string* printEntityNodeString)
{

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


void GIAqueryClass::traceEntityNodeDetermineNextCourseOfAction(string* printEntityNodeString, GIAentityNode* entityNode, string context, const int function, int* numberOfMatchedNodes, const int referenceSetID, const bool traceInstantiations)
{
	if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT)
	{
		*printEntityNodeString = *printEntityNodeString + "\nContext: " + context + " = " + entityNode->entityName;
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISON)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_TESTEDFORQUERYCOMPARISONTEMP)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY)
	{
		if(entityNode->referenceSetID == referenceSetID)
		{
			traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
		}
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_INDEPENDENT)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET)
	{
		traceEntityNode(entityNode, function, numberOfMatchedNodes, printEntityNodeString, referenceSetID, traceInstantiations);
	}
	else
	{
		cerr << "error: illegal trace entity nodes function" << endl;
		exit(EXIT_ERROR);
	}

}
void GIAqueryClass::traceEntityNode(GIAentityNode* entityNode, const int function, int* numberOfMatchedNodes, string* printEntityNodeString, const int referenceSetID, const bool traceInstantiations)
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
	else if((function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY) || (function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_INDEPENDENT))
	{
		if(!(entityNode->testedForQueryComparisonTemp2))	//&& !(entityNode->testedForQueryComparison))?
		{
			pass = true;
			entityNode->testedForQueryComparisonTemp2 = true;
		}
	}
	else if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_RESET)
	{
		if((entityNode->testedForQueryComparisonTemp2))
		{
			pass = true;
			entityNode->testedForQueryComparisonTemp2 = false;
		}
	}
	#ifdef GIA_NLG
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
		bool passMatchNodesIncrement = true;
		
		#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES_QUERIES
		if(entityTypesIsPropertyOrDefinitionRelationshipArray[entityNode->entityType])
		{
			passMatchNodesIncrement = false;
		}
		#endif
	
		if(passMatchNodesIncrement)
		{
			if(!(entityNode->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX))
			{
				*numberOfMatchedNodes = *numberOfMatchedNodes + 1;
			}
		}

		for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
		{
			bool pass2 = true;
			if(!traceInstantiations)
			{
				if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
				{
					pass2 = false;
				}
			}
			if(pass2)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityConnection* connection = (*connectionIter);
					
					bool pass3 = true;
					if(function == GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY_INDEPENDENT)
					{
						pass3 = false;
						if(connection->sameReferenceSet)
						{
							pass3 = true;
						}
					}
					if(pass3)
					{
						traceEntityNodeDetermineNextCourseOfAction(printEntityNodeString, (*connectionIter)->entity, entityVectorConnectionContextArray[connectionType], function, numberOfMatchedNodes, referenceSetID, traceInstantiations);
					}
				}
			}
		}
	}
}

void GIAqueryClass::queryDebugIndentOutputForLevel(const int currentLevel)
{
	for(int level=0; level<currentLevel; level++)
	{
		cout << "\t";
	}
}





bool GIAqueryClass::compareEntityStandard(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, const GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters)
{
	bool compareEntityNamesResult = false;

	if(queryEntityNode->entityName == entityNode->entityName)
	{
		compareEntityNamesResult = true;
	}
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if(!compareEntityNamesResult)
	{
		#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
		if((!traceModeIsQuery && !(referenceTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities)) || (traceModeIsQuery && !(queryTraceParameters->skipOverPropertyAndDefinitionRelationshipEntities)))
		{
			//cout << "skipping1" << endl;
		#endif
			//this code could be moved to a separate function eg in GIAentityNode to make use of entityTypesIsRelationshipArray
			if(((queryEntityNode->entityType == GIA_ENTITY_TYPE_PROPERTY) && (queryEntityNode->entityType == GIA_ENTITY_TYPE_PROPERTY)) ||
			((queryEntityNode->entityType == GIA_ENTITY_TYPE_DEFINITION) && (queryEntityNode->entityType == GIA_ENTITY_TYPE_DEFINITION)))
			{
				compareEntityNamesResult = true;
			}
		#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
		}
		#endif
	}
	#endif
	
	if(!compareEntityNamesResult)
	{
		if(compareEntityAliases(queryEntityNode, entityNode))
		{
			compareEntityNamesResult = true;
		}
	}
	
	#ifdef GIA_SYNONYMN_DETECTION
	if(!compareEntityNamesResult)
	{
		int synonymnDetectionStatus = GIAwordnet.getSynonymnDetectionStatus();
		if(synonymnDetectionStatus != SYNONYMN_DETECTION_STATUS_OFF)
		{
			if(traceModeIsQuery || (synonymnDetectionStatus == SYNONYMN_DETECTION_STATUS_QUERIES_AND_ADVANCED_REFERENCING))
			{
				#ifdef GIA_SYNONYMN_DETECTION_DISABLE_DURING_SPECIFIC_CONCEPT_ACTION_LINKING
				if(traceModeIsQuery || !(referenceTraceParameters->linkSpecificConceptsAndActions))
				{
				#endif
					if(compareEntitySynonyms(queryEntityNode, entityNode))
					{
						compareEntityNamesResult = true;
						if(entityNode->entityType != GIA_ENTITY_TYPE_NETWORK_INDEX)
						{
							*numberOfMatchedNodesRequiredSynonymnDetection = *numberOfMatchedNodesRequiredSynonymnDetection + 1;
						}
					}
				#ifdef GIA_SYNONYMN_DETECTION_DISABLE_DURING_SPECIFIC_CONCEPT_ACTION_LINKING
				}
				#endif
			}
		}
	}
	#endif

	#ifdef GIA_REFERENCING_WILD_CARDS
	if(!compareEntityNamesResult)
	{
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
						if(entityNode->entityType == GIA_ENTITY_TYPE_ACTION)
						{
							if(SHAREDvars.textInTextArray(queryEntityNode->entityName, giaReferencingWildCardActionArray, GIA_REFERENCING_WILD_CARDS_ACTIONS_NUMBER_OF_TYPES))
							{
								compareEntityNamesResult = true;
							}
						}
						else
						{
							if(SHAREDvars.textInTextArray(queryEntityNode->entityName, giaReferencingWildCardSubstanceArray, GIA_REFERENCING_WILD_CARDS_SUBSTANCES_NUMBER_OF_TYPES))
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
	}
	#endif

	return compareEntityNamesResult;
}

void GIAqueryClass::compareEntityReferenceTrace(GIAentityNode* queryEntityNode, GIAentityNode* entityNode, int* numberOfMatchedNodes, const bool knownBestMatch, int* numberOfMatchedNodesRequiredSynonymnDetection, const bool traceModeIsQuery, GIAqueryTraceParameters* queryTraceParameters, const GIAreferenceTraceParameters* referenceTraceParameters, const bool compareEntityNamesResult, bool* exactMatch, const bool simplifiedSearch, int* resultOld)
{
	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	//cout << "referenceTraceParameters->doNotParseQuerySubnetsWithConcepts = " << referenceTraceParameters->doNotParseQuerySubnetsWithConcepts << endl;
	//cout << "queryEntityNode->isConcept = " << queryEntityNode->isConcept << endl;
	if(!(referenceTraceParameters->doNotParseQuerySubnetsWithConcepts) || !(queryEntityNode->entityType == GIA_ENTITY_TYPE_CONCEPT))
	{
	#endif
		if((queryEntityNode->referenceSetID == referenceTraceParameters->referenceSetID) || !(referenceTraceParameters->traceModeAssertSameReferenceSetID))	//only trace paths of same reference set ID
		{
			if(queryEntityNode->referenceSetID != GIA_REFERENCE_SET_ID_UNDEFINED)		//added 13 July 2012
			{
				#ifndef GIA_ADVANCED_REFERENCING_ORIGINAL
				if(queryEntityNode->idActiveList != entityNode->idActiveList)	//else they are exactly the same [NB with new implementation of GIA_RECORD_SAME_REFERENCE_SET_INFORMATION, it will detect the same nodes as a reference match, so they need to be ignored when this happens]
				{
				#endif
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					bool passIntrasentenceReferenceRequirements = true;
					if(referenceTraceParameters->intrasentenceReference)
					{
						passIntrasentenceReferenceRequirements = false;

						#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
						if(!entityTypesIsPropertyOrDefinitionRelationshipArray[entityNode->entityType])
						{
						#endif
							if(entityNode->entityIndexTemp < queryEntityNode->minimumEntityIndexOfReferenceSet)
							{
								passIntrasentenceReferenceRequirements = true;
							}
						#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
						}
						#endif
					}

					if(passIntrasentenceReferenceRequirements)
					{
					#endif
						if(compareEntityNamesResult)
						{

							#ifdef GIA_SPECIFIC_CONCEPTS
							bool passSpecificConcepts = true;
							if(((queryEntityNode->entityType == GIA_ENTITY_TYPE_CONCEPT) && !(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)) ||
							((entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT) && !(queryEntityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)))
							{
								passSpecificConcepts = false;
							}
							#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
							if(referenceTraceParameters->linkSpecificConceptsAndActions)
							{
								//override passSpecificConcepts value:
								#ifndef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED
								if(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)
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
								bool passPluralityMatch = true;
								#ifdef GIA_ADVANCED_REFERENCING_ENSURE_PLURALITY_MATCHES
								if(((queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && !(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)) ||
								((entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && !(queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)))
								{
									if(!(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT && queryEntityNode->entityType == GIA_ENTITY_TYPE_CONCEPT))	//condition added 29 Sept 2013
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
								#endif
								#ifdef GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES
								#ifdef GIA_TRANSLATOR_NUMBER_OF
								if(queryEntityNode->isNumberOf || entityNode->isNumberOf)
								{
									passPluralityMatch = false;
								}
								#endif
								if(queryEntityNode->hasQuantity)
								{
									if(entityNode->hasQuantity)
									{
										if(queryEntityNode->quantityNumber != entityNode->quantityNumber)
										{
											passPluralityMatch = false;
										}
									}
									else
									{
										passPluralityMatch = false;
									}
								}
								else if(entityNode->hasQuantity)
								{
									#ifdef GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES_SUPPORT_QUERY_PLURAL_TO_QUANTITY_MATCH
									if(queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
									{
										passPluralityMatch = true;
									}
									else
									{
									#endif
										passPluralityMatch = false;
									#ifdef GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES_SUPPORT_QUERY_PLURAL_TO_QUANTITY_MATCH
									}
									#endif
								}
								#endif
								#ifdef GIA_ADVANCED_REFERENCING_PREVENT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
								if(GIAentityNodeClass.detectPredeterminerNonReference(entityNode) || GIAentityNodeClass.detectPredeterminerNonReference(queryEntityNode))
								{
									passPluralityMatch = false;
								}
								#endif
								#ifdef GIA_ADVANCED_REFERENCING_PREVENT_REFERENCING_OF_PLURAL_ENTITIES
								if(((queryEntityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && (queryEntityNode->grammaticalDefiniteTemp)) || ((entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) && (queryEntityNode->grammaticalIndefinitePluralTemp)))
								{
									passPluralityMatch = false;
								}
								#endif
								if(passPluralityMatch)
								{
									#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
									bool passReferenceContextMatch = true;
									if(referenceTraceParameters->testReferenceSetContext)
									{
										if(queryEntityNode == referenceTraceParameters->referenceSetDefiniteEntity)
										{
											passReferenceContextMatch = false;
											int indentationDifferenceFound = INT_DEFAULT_VALUE;	//not used
											if(GIAtranslatorOperations.checkIndefiniteEntityCorrespondingToDefiniteEntityInSameContextGIA(entityNode, queryEntityNode, &indentationDifferenceFound))
											{
												passReferenceContextMatch = true;
											}
										}
									}
									if(passReferenceContextMatch)
									{
									#endif
										#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
										if(!(entityNode->NLCmathTextParsablePhraseEntity))
										{
										#endif
											#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
											bool passConceptOnlyTraceRequirements = true;
											if(referenceTraceParameters->traceConceptsOnly)
											{
												passConceptOnlyTraceRequirements = false;
												if(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT || entityNode->entityType == GIA_ENTITY_TYPE_QUALITY)
												{
													passConceptOnlyTraceRequirements = true;
													//rely on previous testReferencedEntityNodeForExactNameMatch2() passSpecificConcepts checks to test entityNode target (connectionIter) for isConcept also
												}
											}
											if(passConceptOnlyTraceRequirements)
											{
											#endif
												#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
												bool passLogicalConditionRequirements = true;
												if(referenceTraceParameters->logicalConditionDisableTraceConcepts)
												{
													if(entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT)
													{
														passLogicalConditionRequirements = false;
													}
												}
												if(passLogicalConditionRequirements)
												{
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

														}
														else
														{
															*resultOld = EXACT_MATCH_FAIL;
														}
													}
												#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
												}
												#endif
											#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
											}
											#endif
										#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
										}
										#endif
									#ifdef GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION
									}
									#endif
								}
								else
								{
								}
							#ifdef GIA_SPECIFIC_CONCEPTS
							}
							else
							{
							}
							#endif
						}
						else
						{
							*resultOld = EXACT_MATCH_FAIL;
						}
					#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
					}
					else
					{
						*resultOld = EXACT_MATCH_FAIL;	//CHECKTHIS
					}
					#endif
				#ifndef GIA_ADVANCED_REFERENCING_ORIGINAL
				}
				else
				{
					*resultOld = EXACT_MATCH_FAIL;	//CHECKTHIS
				}
				#endif
			}
			else
			{
				*resultOld = EXACT_MATCH_OUT_OF_BOUNDS;	//CHECKTHIS
			}
		}
		else
		{
			*resultOld = EXACT_MATCH_OUT_OF_BOUNDS;
		}
	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
	}
	#endif
}




