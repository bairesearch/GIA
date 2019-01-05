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
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1m 24-April-2018
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: NLG - natural language generation
 * /
 *******************************************************************************/


#include "GIAnlg.hpp"

#ifdef GIA_NLG

GIANLGentity::GIANLGentity(void)
{
	entityGrammatisedText = "";

	next = NULL;
}

string GIAnlgClass::generateLanguageFromQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode)
{
	string answerString = "";

	GIAentityConnection* queryAnswerNodeConnectionArtificial = new GIAentityConnection();
	queryAnswerNodeConnectionArtificial->entity = queryAnswerNode;
	queryAnswerNodeConnectionArtificial->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION;	//artificially set to ensure that connection is not interpreted as relationship

	GIANLGentity* firstNLGentityInSourceContext = new GIANLGentity();
	GIANLGentity* firstNLGentityInRelationshipNode = new GIANLGentity();
	GIANLGentity* firstNLGentityInTargetContext = new GIANLGentity();
	GIANLGentity* currentNLGentityInSourceContext = firstNLGentityInSourceContext;
	GIANLGentity* currentNLGentityInRelationshipNode = firstNLGentityInRelationshipNode;
	GIANLGentity* currentNLGentityInTargetContext = firstNLGentityInTargetContext;
	
	bool comparisonVariableNodeHasAlias = false;
	if(!(comparisonVariableNode->aliasList.empty()))
	{
		comparisonVariableNodeHasAlias = true;
	}
	
	//find the !sameReferencceSet connection of the entityNode (comparisonVariableNode) - there should only be 1 !sameReferencceSet connection and this should correspond to the previous word in the question
	GIAentityConnection* relationshipConnection = NULL;
	GIAentityNode* relationshipEntitySubjectOrObject = NULL;	//lastNodeInQuestionContextReferenceSet
	GIAentityConnection* relationshipEntitySubjectOrObjectConnection = NULL;
	bool relationshipEntitySubjectFound = false;	//lastNodeInQuestionContextReferenceSetFound
	bool relationshipEntityObjectFound = false;
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		bool relationshipEntitySubjectFoundTemp = false;
		bool relationshipEntityObjectFoundTemp = false;
		//if(connectionType != GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
		if(entityVectorConnectionGoToParentArray[connectionType])
		{
			relationshipEntitySubjectFoundTemp = true;
		}
		#ifdef GIA_NLG_REPRESENT_WHO_WHAT_QUERIES_ADVANCED
		else if(entityVectorConnectionGoToChildArray[connectionType])
		{
			if(!comparisonVariableNodeHasAlias)
			{
				relationshipEntityObjectFoundTemp = true;
			}
		}
		#endif
		if(relationshipEntitySubjectFoundTemp || relationshipEntityObjectFoundTemp)
		{
			GIAentityNode* relationshipEntityQuery = NULL;
			
			for(vector<GIAentityConnection*>::iterator connectionIter = comparisonVariableNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != comparisonVariableNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				GIAentityConnection* connection = *connectionIter;
				if(!(connection->sameReferenceSet))
				{
					relationshipEntityQuery = connection->entity;
					if(relationshipEntitySubjectFoundTemp)
					{
						#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
						relationshipEntitySubjectOrObjectConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(relationshipEntityQuery);
						#else
						if(entityVectorConnectionIsRelationshipPropertyOrDefinitionReverseArray[connection->connectionType])
						{
							relationshipEntitySubjectOrObjectConnection = connection;
						}
						else
						{
							relationshipEntitySubjectOrObjectConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(relationshipEntityQuery);
						}
						#endif
						relationshipEntitySubjectOrObject = GIAtranslatorOperations.getRelationshipSubjectEntity(connection);
						//cout << "relationshipEntitySubjectOrObject = " << relationshipEntitySubjectOrObject->entityName << endl;
					}
					#ifdef GIA_NLG_REPRESENT_WHO_WHAT_QUERIES_ADVANCED
					else if(relationshipEntityObjectFoundTemp)
					{
						#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
						relationshipEntitySubjectOrObjectConnection = GIAtranslatorOperations.getRelationshipObjectEntityConnection(relationshipEntityQuery);
						#else
						if(entityVectorConnectionIsRelationshipPropertyOrDefinitionForwardArray[connection->connectionType])	//OLD: entityVectorConnectionIsRelationshipPropertyOrDefinitionArray	!//entityVectorConnectionIsPropertyOrDefinitionArray
						{
							relationshipEntitySubjectOrObjectConnection = connection;
						}
						else
						{
							relationshipEntitySubjectOrObjectConnection = GIAtranslatorOperations.getRelationshipObjectEntityConnection(relationshipEntityQuery);
						}
						#endif
						relationshipEntitySubjectOrObject = GIAtranslatorOperations.getRelationshipObjectEntity(connection);
						//cout << "relationshipEntitySubjectOrObject = " << relationshipEntitySubjectOrObject->entityName << endl;
					}
					#endif
			
					if(relationshipEntitySubjectFoundTemp)
					{
						relationshipEntitySubjectFound = true;
					}
					#ifdef GIA_NLG_REPRESENT_WHO_WHAT_QUERIES_ADVANCED
					else if(relationshipEntityObjectFoundTemp)
					{
						relationshipEntityObjectFound = true;
					}
					#endif
				}
			}
			
			if(relationshipEntityQuery != NULL)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = queryAnswerNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != queryAnswerNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityConnection* connection = *connectionIter;
					if(connection->entity->queryAnswerContext)
					{
						relationshipConnection = connection;
					}
				}
			}
		}
	}
	
	if(relationshipEntitySubjectFound)
	{
		//where, why, how many, how, who, which, etc query
		if(!generateLanguageFromTextIteration(relationshipEntitySubjectOrObjectConnection, &currentNLGentityInSourceContext, true, 0, false, false, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(relationshipEntitySubjectOrObjectConnection, currentNLGentityInSourceContext, true..." << endl;
			exit(EXIT_ERROR);		
		}

		if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false, false, true))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
			exit(EXIT_ERROR);	
		}
		
		if(!generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, &currentNLGentityInTargetContext, false, 0, false, false, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, currentNLGentityInTargetContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	}
	#ifdef GIA_NLG_REPRESENT_WHO_WHAT_QUERIES_ADVANCED
	else if(relationshipEntityObjectFound)
	{
		//what, which, who etc query
		if(!generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, &currentNLGentityInSourceContext, false, 0, false, false, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, currentNLGentityInSourceContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	
		if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false, false, true))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
			exit(EXIT_ERROR);	
		}
		
		if(!generateLanguageFromTextIteration(relationshipEntitySubjectOrObjectConnection, &currentNLGentityInTargetContext, true, 0, false, false, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(relationshipEntitySubjectOrObjectConnection, currentNLGentityInTargetContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	}
	#endif
	else
	{		
		//assume who query; e.g. That is Jim. Who is that?
		
		GIAentityConnection* comparisonVariableNodeConnectionArtificial = new GIAentityConnection();
		comparisonVariableNodeConnectionArtificial->entity = comparisonVariableNode;
		comparisonVariableNodeConnectionArtificial->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION;	//artificially set to ensure that connection is not interpreted as relationship
		
		if(!generateLanguageFromTextIteration(comparisonVariableNodeConnectionArtificial, &currentNLGentityInSourceContext, false, 0, false, false, false))	//NB parseSameReferenceSetOnly changed to false @3c5c
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(comparisonVariableNode, currentNLGentityInSourceContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	
		GIAentityNode* relationshipEntity = new GIAentityNode();
		relationshipConnection = new GIAentityConnection();
		relationshipConnection->entity = relationshipEntity;
		relationshipEntity->entityType = GIA_ENTITY_TYPE_DEFINITION;
		relationshipConnection->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION;
		if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false, false, true))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
			exit(EXIT_ERROR);	
		}
		
		
		/*
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !relationshipEntitySubjectFound" << endl;
		exit(EXIT_ERROR);
		*/
		
		//where, why, how many, how, who, etc query
		if(!generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, &currentNLGentityInTargetContext, false, 0, false, false, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, firstNLGentityInTargetContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	}
	
	
				
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInSourceContext);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInRelationshipNode);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInTargetContext);
	
	return answerString;
}

string GIAnlgClass::generateLanguageFromNLGentityList(GIANLGentity* firstNLGentityInContext)
{
	string languageString = "";

	GIANLGentity* currentNLGentity = firstNLGentityInContext;
	while(currentNLGentity->next != NULL)
	{
		languageString = languageString + currentNLGentity->entityGrammatisedText;
		currentNLGentity = currentNLGentity->next;
	}
	
	return languageString;
}



bool GIAnlgClass::generateLanguageFromTextIteration(GIAentityConnection* currentConnection, GIANLGentity** currentNLGentity, const bool parseSameReferenceSetOnly, const int interationIndex, const bool isRelationshipReverseIterationAction, const bool isRelationshipReverseIterationProperty, const bool isRelationshipReverseIterationPropertyPossessive)
{
	bool result = true;
	
	GIAentityNode* currentEntity = currentConnection->entity;
	
	if(!(currentEntity->parsedForLanguageGeneration))
	{
		//cout << "currentEntity->entityName = " << currentEntity->entityName << endl;
		
		currentEntity->parsedForLanguageGeneration = true;
		
		bool qualityDetected = false;
		if(currentEntity->entityType == GIA_ENTITY_TYPE_QUALITY)
		{
			qualityDetected = true;
		}
		
		#ifdef GIA_NLG_POSSESSIVE_ADVANCED
		bool firstIteration = false;
		if(interationIndex == 0)
		{
			if(!parseSameReferenceSetOnly)
			{
				firstIteration = true;
			}
		}
			
		//special exception; parse reverse properties (possessive)
		bool isRelationshipReverseIterationPropertyChild = false;
		bool isRelationshipReverseIterationPropertyChildPossessive = false;
		bool isRelationshipReverseIterationPropertyChildQuality = false;
		int subphraseCount = 0;
		vector<GIANLGentity*> NLGentitiesInSection;
		for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE].end(); connectionIter++)
		{
			GIAentityConnection* connection = *connectionIter;
			if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
			{
				GIAentityNode* relationshipEntity = connection->entity;
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				GIAentityConnection* relationshipEntitySubjectConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(relationshipEntity);
				#else
				GIAentityConnection* relationshipEntitySubjectConnection = connection;
				#endif

				if(generateLanguageFromTextIterationPerformChecks(relationshipEntitySubjectConnection, false))
				{
					//cout << "generateLanguageFromTextIterationPerformChecks: currentEntity= " << currentEntity->entityName << endl;
					//cout << "generateLanguageFromTextIterationPerformChecks: relationshipEntitySubjectConnection = " << relationshipEntitySubjectConnection->entity->entityName << endl;

					subphraseCount++;
					NLGentitiesInSection.push_back(*currentNLGentity);
					
					bool prependRcmod = false;
					if(qualityDetected)
					{
						if(firstIteration)
						{
							//e.g. Tom proposed that the apple is happy. What did Tom propose?
							isRelationshipReverseIterationPropertyChildQuality = true;
							prependRcmod = true;
						}
						else
						{
							cout << "GIAnlgClass::generateLanguageFromTextIteration warning: GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE && qualityDetected && !firstIteration" << endl;
							//e.g. Tom proposed that the apple is near the bike is happy. (grammatically illegal)
						}
					}
					else
					{
						//e.g. Max has a bike. Tom proposed the bike. What did Tom propose?
						isRelationshipReverseIterationPropertyChildPossessive = true;
						if(!parseSameReferenceSetOnly || connection->sameReferenceSet)
						{
							//prependRcmod = true;
						}
					}
					
					isRelationshipReverseIterationPropertyChild = true;
					
					if(prependRcmod)
					{						
						//prepend "that" to answer; e.g. Tom proposed that the apple is happy
						string relationshipEntityText = "";
						relationshipEntityText = relationshipEntityText + GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET;
						(*currentNLGentity)->entityGrammatisedText = relationshipEntityText;
						(*currentNLGentity)->next = new GIANLGentity();
						(*currentNLGentity) = (*currentNLGentity)->next;
					}
					
					if(!generateLanguageFromTextIteration(relationshipEntitySubjectConnection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, false, true, isRelationshipReverseIterationPropertyChildPossessive))
					{
						result = false;
					}
					
				}
			}
		}
		if(!generateConjunctionOfNLGentitiesInSection(subphraseCount, &NLGentitiesInSection))
		{
			result = false;	//e.g. Tom's and the rabbit's hat
		}
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		//#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(interationIndex == 0)
		{
			if(!parseSameReferenceSetOnly)
			{
				if(currentEntity->entityName == RELATION_ENTITY_HAVE)
				{
					for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].end(); connectionIter++)
					{
						GIAentityConnection* relationshipEntitySubjectConnection = *connectionIter;
						if(relationshipEntitySubjectConnection->sameReferenceSet || !parseSameReferenceSetOnly)
						{
							if(generateLanguageFromTextIterationPerformChecks(relationshipEntitySubjectConnection, false))
							{
								//cout << "generateLanguageFromTextIterationPerformChecks: currentEntity= " << currentEntity->entityName << endl;
								//cout << "generateLanguageFromTextIterationPerformChecks: relationshipEntitySubjectConnection = " << relationshipEntitySubjectConnection->entity->entityName << endl;

								isRelationshipReverseIterationPropertyChild = true;
								bool prependRcmod = true;
								//e.g. Tom proposed that Max has a bike. What did Tom propose?
								
								if(prependRcmod)
								{
									//prepend "that" to answer; e.g. Tom proposed that Max has a bike.
									string relationshipEntityText = "";
									relationshipEntityText = relationshipEntityText + GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET;
									(*currentNLGentity)->entityGrammatisedText = relationshipEntityText;
									(*currentNLGentity)->next = new GIANLGentity();
									(*currentNLGentity) = (*currentNLGentity)->next;
								}
					
								if(!generateLanguageFromTextIteration(relationshipEntitySubjectConnection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, false, true, false))
								{
									result = false;
								}

							}
						}
					}
				}
			}
		}
		//#endif
		#endif
		#endif
		
		#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
		#ifdef GIA_NLG_POSSESSIVE_ADVANCED
		if(!qualityDetected || isRelationshipReverseIterationPropertyChildQuality)	//isRelationshipReverseIterationPropertyChildQuality: e.g. Tom proposed that the apple is happy. What did Tom propose?
		#else
		if(!qualityDetected)			
		#endif
		{
			
		#endif
			if(GIAtranslatorOperations.connectionTargetIsRelationship(currentConnection))
			{
				bool isSameReferenceSetIteration = true;
				if(parseSameReferenceSetOnly)
				{
					isSameReferenceSetIteration = currentConnection->sameReferenceSet;
				}
				
				if(isRelationshipReverseIterationAction)
				{
					generateRelationshipEntityLanguageActionReverse(currentConnection, currentNLGentity, isSameReferenceSetIteration);
				}
				else
				{	
					if(currentEntity->entityType == GIA_ENTITY_TYPE_ACTION)
					{
						//special exception; parse action subject
						for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].end(); connectionIter++)
						{	
							GIAentityConnection* connection = *connectionIter;
							#ifdef GIA_NLG_EXPECT_TRANSLATOR_QUERY_ACTION_AND_CONDITION_SAME_REFERENCE_SET_ASSIGNMENT_BUG_FIXED
							if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
							{
							#endif
								GIAentityNode* entity = connection->entity;
								if(generateLanguageFromTextIterationPerformChecks(connection, false))
								{
									if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, false, false, false))
									{
										result = false;
									}
									if(parseSameReferenceSetOnly)
									{
										isSameReferenceSetIteration = connection->sameReferenceSet;
									}
									else
									{
										isSameReferenceSetIteration = false;
									}
								}
							#ifdef GIA_NLG_EXPECT_TRANSLATOR_QUERY_ACTION_AND_CONDITION_SAME_REFERENCE_SET_ASSIGNMENT_BUG_FIXED	
							}
							#endif
						}
					}

					#ifdef GIA_NLG_POSSESSIVE_ADVANCED
					if(!isRelationshipReverseIterationPropertyPossessive)
					{
					#endif
						//cout << "generateRelationshipEntityLanguage currentEntity->entityName = " << currentEntity->entityName << endl;
						generateRelationshipEntityLanguage(currentConnection, currentNLGentity, isSameReferenceSetIteration, isRelationshipReverseIterationProperty&&!isRelationshipReverseIterationPropertyPossessive, !isRelationshipReverseIterationProperty&&!isRelationshipReverseIterationPropertyChild);
					#ifdef GIA_NLG_POSSESSIVE_ADVANCED
					}
					#endif
				}
			}
			else
			{	
				bool definite = true;	//CHECKTHIS
				if(!generateNounEntityLanguage(currentEntity, currentNLGentity, definite, parseSameReferenceSetOnly, isRelationshipReverseIterationPropertyPossessive, !isRelationshipReverseIterationPropertyChildPossessive))
				{
					result = false;
				}
			}

			if(parseSameReferenceSetOnly || (interationIndex < GIA_NLG_GENERATE_LANGUAGE_MAX_NUM_ITERATIONS))
			{
				if(isRelationshipReverseIterationAction)
				{
					//special exception; parse action subject
					for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].end(); connectionIter++)
					{	
						GIAentityConnection* connection = *connectionIter;
						if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
						{
							GIAentityNode* entity = connection->entity;
							if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, false, false, false))
							{
								result = false;
							}
						}
					}	
				}
				else
				{
					int subphraseCount = 0;
					vector<GIANLGentity*> NLGentitiesInSection;
						
					//special exception; parse reverse actions
					for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE].end(); connectionIter++)
					{
						GIAentityConnection* connection = *connectionIter;
						if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
						{
							GIAentityNode* actionRelationshipEntity = connection->entity;
							if(generateLanguageFromTextIterationPerformChecks(connection, false))
							{
								subphraseCount++;
								NLGentitiesInSection.push_back(*currentNLGentity);
								
								if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, true, false, false))
								{
									result = false;
								}
							}
						}
					}

					for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
					{
						if(entityVectorConnectionGoToChildArray[connectionType])
						{
							for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
							{
								GIAentityConnection* connection = *connectionIter;
								if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
								{
									GIAentityNode* entity = connection->entity;
									if(generateLanguageFromTextIterationPerformChecks(connection, isRelationshipReverseIterationProperty))
									{										
										if(!entityVectorConnectionIsRelationshipSubjectObjectArray[connectionType])
										{
											subphraseCount++;
											NLGentitiesInSection.push_back(*currentNLGentity);
											//cout << "entity = " << entity->entityName << endl;
											//cout << "\tsubphraseCount = " << subphraseCount << endl;
										}

										if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, interationIndex+1, false, isRelationshipReverseIterationProperty, isRelationshipReverseIterationPropertyPossessive))
										{
											result = false;
										}
									}
								}
							}
						}
					}

					if(!generateConjunctionOfNLGentitiesInSection(subphraseCount, &NLGentitiesInSection))
					{
						result = false;
					}
				}
			}
		#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
		}
		#endif
	}
		
	return result;
}

bool GIAnlgClass::generateLanguageFromTextIterationPerformChecks(GIAentityConnection* connection, const bool parseQualities)
{
	bool result = true;
	
	GIAentityNode* entity = connection->entity;
	if(entity->parsedForLanguageGeneration)
	{
		result = false;
	}
	if(entity->queryAnswerContext)
	{
		result = false;
	}
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	if(compareRelationshipConnectionType(connection, GIA_ENTITY_TYPE_CONDITION))	//if(currentEntity->entityType == GIA_ENTITY_TYPE_CONDITION)
	{
		GIAentityConnection* conditionObjectConnection = GIAtranslatorOperations.getRelationshipObjectEntityConnection(entity);
		if(!generateLanguageFromTextIterationPerformChecks(conditionObjectConnection))
		{
			result = false;	
		}
	}
	#endif
	#endif
	#endif
	
	#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
	if(entity->entityType == GIA_ENTITY_TYPE_QUALITY)
	{
		result = false;	
	}
	if(!parseQualities)
	{
		if(compareRelationshipConnectionType(connection, GIA_ENTITY_TYPE_PROPERTY))
		{
			GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(connection);
			if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
			{
				result = false;
			}
		}
	}
	#endif
			
	return result;
}

bool GIAnlgClass::generateQualityPrependText(GIAentityNode* currentEntity, GIANLGentity** currentNLGentity, const bool testSameReferenceSet, const bool generateNewNLGentities)	//specific to english
{
	bool result = true;
	
	string qualityText = "";	//eg the blue apple / the blue and happy apple / the blue, happy and bright apple
	int subphraseCount = 0;
	//GIANLGentity* firstNLGentityInSection = *currentNLGentity;
	vector<GIANLGentity*> NLGentitiesInSection;
	for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY].end(); connectionIter++)
	{
		GIAentityConnection* connection = *connectionIter;
		GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getRelationshipObjectEntity(connection);
		if(!(relationshipEntityObject->queryAnswerContext))
		{
			if(!(relationshipEntityObject->parsedForLanguageGeneration))
			{
				if(connection->sameReferenceSet || !testSameReferenceSet)
				{
					if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
					{				
						//sub qualities (eg the very big...)
						if(!generateQualityPrependText(relationshipEntityObject, currentNLGentity, true, false))
						{
							result = false;
						}
	
						#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
						(*currentNLGentity)->entityGrammatisedText = (*currentNLGentity)->entityGrammatisedText + generateMorphology(relationshipEntityObject) + GIA_NLG_TEXT_SPACE;
						#else
						(*currentNLGentity)->entityGrammatisedText = (*currentNLGentity)->entityGrammatisedText + relationshipEntityObject->entityName + GIA_NLG_TEXT_SPACE;	//qualities/adjectives don't have lemmas and so don't require morphological generation
						#endif
						relationshipEntityObject->parsedForLanguageGeneration = true;
						if(generateNewNLGentities)
						{
							NLGentitiesInSection.push_back((*currentNLGentity));
							(*currentNLGentity)->next = new GIANLGentity();
							(*currentNLGentity) = (*currentNLGentity)->next;
							subphraseCount++;
							//cout << "subphraseCount++ relationshipEntityObject = " << relationshipEntityObject->entityName << endl;
						}
					}		
				}
			}
		}
	}
	
	if(!generateConjunctionOfNLGentitiesInSection(subphraseCount, &NLGentitiesInSection))
	{
		result = false;
	}
	
	return result;
}

bool GIAnlgClass::generateConjunctionOfNLGentitiesInSection(int subphraseCountMax, vector<GIANLGentity*>* NLGentitiesInSection)
{
	bool result = true;
	
	int subphraseCount = 1;
	for(vector<GIANLGentity*>::iterator iter = NLGentitiesInSection->begin(); iter != NLGentitiesInSection->end(); iter++)
	{
		GIANLGentity* currentNLGentityInSection = *iter;
		if(subphraseCount == 1)
		{
			//do not add conjunction text for first subphrase
		}
		else if(subphraseCount < subphraseCountMax)
		{
			currentNLGentityInSection->entityGrammatisedText = string(GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND_IMPLICIT) + currentNLGentityInSection->entityGrammatisedText;		
		}
		else if(subphraseCount == subphraseCountMax)
		{
			currentNLGentityInSection->entityGrammatisedText = string(GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND) + currentNLGentityInSection->entityGrammatisedText;
		}
		subphraseCount++;
	}	
	
	return result;	
}

/*
bool GIAnlgClass::generateConjunctionOfNLGentitiesInSection(int subphraseCountMax, GIANLGentity* firstNLGentityInSection)
{
	bool result = true;
	
	GIANLGentity* currentNLGentityInSection = firstNLGentityInSection;
	subphraseCount = 1;
	while(currentNLGentityInSection->next != NULL)
	{
		if(subphraseCount < subphraseCountMax-1)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND_IMPLICIT;		
		}
		else if(subphraseCount < subphraseCountMax)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND;
		}
		else if(subphraseCount == subphraseCountMax)
		{

		}
		subphraseCount++;
		currentNLGentityInSection = currentNLGentityInSection->next;
	}	
	
	return result;	
}
*/







bool GIAnlgClass::generateRelationshipEntityLanguage(GIAentityConnection* relationshipConnection, GIANLGentity** currentNLGentity, const bool sameReferenceSet, bool addAuxiliaryToQualities, bool addRCmodToProperties)
{
	bool result = true;
	
	string relationshipEntityText = "";

	bool generateAuxiliary = true;
		
	if(sameReferenceSet)
	{
		#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_CONDITIONS
		if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_CONDITION))
		{
			generateAuxiliary = false;
		}
		#endif
		#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
		if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_PROPERTY))
		{
			GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(relationshipConnection);
			if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
			{
				if(!addAuxiliaryToQualities)
				{
					generateAuxiliary = false;	
				}
			}
		}
		#endif
		if(generateAuxiliary)
		{
			if(addRCmodToProperties)
			{
				relationshipEntityText = relationshipEntityText + GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET;
			}
		}
	}
	
	if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_ACTION))
	{
		relationshipEntityText = relationshipEntityText + generateMorphologyAction(relationshipConnection);
	}
	else
	{
		if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_CONDITION))
		{
			if(generateAuxiliary)
			{
				relationshipEntityText = relationshipEntityText + generateMorphologyRelationshipAuxiliaryBe(relationshipConnection);
			}
			relationshipEntityText = relationshipEntityText + generateMorphologyCondition(relationshipConnection);
		}
		else if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_PROPERTY))
		{
			if(generateAuxiliary)
			{
				//if property is a quality, then use "is" as auxiliary
				GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(relationshipConnection);
				if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
				{
					relationshipEntityText = relationshipEntityText + generateMorphologyRelationshipAuxiliaryBe(relationshipConnection);
				}
				else
				{
					relationshipEntityText = relationshipEntityText + generateMorphologyRelationshipAuxiliaryHave(relationshipConnection);
				}
			}
		}
		if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_DEFINITION))
		{
			if(generateAuxiliary)
			{
				relationshipEntityText = relationshipEntityText + generateMorphologyRelationshipAuxiliaryBe(relationshipConnection);
			}
		}	
	}

	(*currentNLGentity)->entityGrammatisedText = relationshipEntityText;
	(*currentNLGentity)->next = new GIANLGentity();
	(*currentNLGentity) = (*currentNLGentity)->next;
	
	return result;
}

bool GIAnlgClass::generateRelationshipEntityLanguageActionReverse(GIAentityConnection* relationshipConnection, GIANLGentity** currentNLGentity, const bool sameReferenceSet)
{
	bool result = true;
	
	string relationshipEntityText = "";
	
	bool generateAuxiliary = true;
	if(sameReferenceSet)
	{		
		if(generateAuxiliary)
		{
			relationshipEntityText = relationshipEntityText + GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET;
		}
	}
	relationshipEntityText = relationshipEntityText + generateMorphologyRelationshipAuxiliaryBe(relationshipConnection);
	relationshipEntityText = relationshipEntityText + generateMorphologyActionReverse(relationshipConnection);
	relationshipEntityText = relationshipEntityText + GIA_NLG_RELATIONSHIP_ENTITY_ACTION_REVERSE;
	
	(*currentNLGentity)->entityGrammatisedText = relationshipEntityText;
	(*currentNLGentity)->next = new GIANLGentity();
	(*currentNLGentity) = (*currentNLGentity)->next;
	
	return result;
}

bool GIAnlgClass::compareRelationshipConnectionType(GIAentityConnection* relationshipConnection, int entityType)
{
	bool result = false;
	
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	if(relationshipEntity->entityType == entityType)
	{
		result = true;
	}
	#else
	if(entityTypesIsPropertyOrDefinitionRelationshipArray[entityType])
	{ 
		if(entityVectorConnectionIsPropertyOrDefinitionArray[relationshipConnection->connectionType])
		{
			result = true;
		}
	}
	else
	{
		GIAentityNode* relationshipEntity = relationshipConnection->entity;
		if(relationshipEntity->entityType == entityType)
		{
			result = true;
		}
	}
	#endif
	
	return result;
}



string GIAnlgClass::generateMorphologyAction(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	relationshipEntityText = generateMorphology(relationshipEntity);	//OR generateVerbCase
	//consider; that did ride VS that rode 
	#else
	relationshipEntityText = relationshipEntity->entityName;
	/*
	if(entityNode->wordOrig != "")
	{
		relationshipEntityText = relationshipEntity->wordOrig;
	}
	else
	{
		relationshipEntityText = relationshipEntity->entityName;
	}
	*/
	#endif
	relationshipEntityText = relationshipEntityText + GIA_NLG_TEXT_SPACE;
	return relationshipEntityText;
}

string GIAnlgClass::generateMorphologyActionReverse(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	relationshipEntityText = generateMorphologyPastParticiple(relationshipEntity);
	//eg that was ridden by
	#else
	relationshipEntityText = relationshipEntity->entityName;
	#endif
	relationshipEntityText = relationshipEntityText + GIA_NLG_TEXT_SPACE;
	return relationshipEntityText;
}

string GIAnlgClass::generateMorphologyCondition(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	relationshipEntityText = relationshipEntityText + relationshipEntity->entityName;	//eg "near"
	relationshipEntityText = relationshipEntityText + GIA_NLG_TEXT_SPACE;
	return relationshipEntityText;
}



string GIAnlgClass::generateMorphologyRelationshipAuxiliaryBe(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	relationshipEntityText = generateMorphology(relationshipEntity);
	#else
	relationshipEntityText = generateMorphologyConnection(relationshipConnection);
	#endif	
	#else
	int grammaticalNumberIndex = GRAMMATICAL_NUMBER_SINGULAR;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	if(!(relationshipEntity->relationshipSubjectEntity->empty()))
	{
		GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getDefinitionRelationshipSubjectEntity(relationshipConnection);
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][grammaticalNumberIndex];
	#else
	if(relationshipConnection->entityOrigin != NULL)
	{
		GIAentityNode* relationshipEntitySubject = relationshipConnection->entityOrigin;
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[relationshipConnection->grammaticalTenseTemp][grammaticalNumberIndex];	
	#endif
	#endif
	return relationshipEntityText;
}

string GIAnlgClass::generateMorphologyRelationshipAuxiliaryHave(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	relationshipEntityText = generateMorphology(relationshipEntity);
	#else
	relationshipEntityText = generateMorphologyConnection(relationshipConnection);
	#endif
	#else
	int grammaticalNumberIndex = GRAMMATICAL_NUMBER_SINGULAR;
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	if(!(relationshipEntity->relationshipSubjectEntity->empty()))
	{
		GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getPropertyRelationshipSubjectEntity(relationshipConnection);
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][grammaticalNumberIndex];
	#else
	if(relationshipConnection->entityOrigin != NULL)
	{
		GIAentityNode* relationshipEntitySubject = relationshipConnection->entityOrigin;
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipConnection->grammaticalTenseTemp][grammaticalNumberIndex];	
	#endif
	#endif
	return relationshipEntityText;
}





bool GIAnlgClass::generateNounEntityLanguage(GIAentityNode* nounEntity, GIANLGentity** currentNLGentity, const bool definite, const bool parseSameReferenceSetOnly, const bool possessive, const bool determiner)
{
	bool result = true;
	
	if(determiner)
	{
		string determinateText = "";
		determinateText = determinateText + calcDeterminate(nounEntity, definite);
		(*currentNLGentity)->entityGrammatisedText = determinateText;
		(*currentNLGentity)->next = new GIANLGentity();
		(*currentNLGentity) = (*currentNLGentity)->next;	
	}
		
	if(!generateQualityPrependText(nounEntity, currentNLGentity, parseSameReferenceSetOnly, true))
	{
		result = false;
	}
	
	string nounEntityText = "";
	nounEntityText = nounEntityText + calcNounWord(nounEntity, possessive);
	(*currentNLGentity)->entityGrammatisedText = nounEntityText;
	(*currentNLGentity)->next = new GIANLGentity();
	(*currentNLGentity) = (*currentNLGentity)->next;
	
	return result;
}

string GIAnlgClass::calcDeterminate(GIAentityNode* entityNode, const bool definite)
{
	//first letter is vowel		//added 03 August 2012
	bool firstLetterIsVowel = false;
	string word = entityNode->entityName;
	for(int i=0; i<NLG_NUMBER_OF_VOWELS; i++)
	{
		if(tolower(word[0]) == vowelArray[i])
		{
		 	firstLetterIsVowel = true;
		}
	}

	//noun_number
	bool isPlural = false;	//added 03 August 2012
	if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
	{
		isPlural = true;
	}

	#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
	//gender
	bool isPerson = false;
	bool hasGender = false;
	string genderString = "";
	if(entityNode->grammaticalGenderTemp != GRAMMATICAL_GENDER_UNDEFINED)
	{
		//relex compatible only
		if(entityNode->grammaticalGenderTemp == GRAMMATICAL_GENDER_PERSON)
		{
			isPerson = true;
		}
		else
		{
			isPerson = true;
			genderString = grammaticalGenderNameArray[entityNode->grammaticalGenderTemp];
			hasGender = true;
		}
	}
	else
	{
		//stanford/relex compatible
		//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()	[see GIAtranslatorGrammar.cpp]
		if(entityNode->grammaticalProperNounTemp)
		{
			isPerson = true;
		}

	}

	bool isPronoun = false;
	if(entityNode->grammaticalPronounTemp)
	{
		isPronoun = true;
	}
	#endif

	//determinate
	bool addDeterminate = false;
	string determinate = "";
	if(GIAentityNodeClass.entityIsRelationship(entityNode))
	{
		cout << " GIAnlgClass::calcDeterminate{} error: entityIsRelationship(entityNode)" << endl;
	}
	else if((entityNode->entityType == GIA_ENTITY_TYPE_CONCEPT) || (entityNode->entityType == GIA_ENTITY_TYPE_QUALITY) || (entityNode->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX))
	{
		
	}
	else if(entityNode->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
	{
		if(!(entityNode->hasQuantity))
		{
			if(definite)
			{
				determinate = GRAMMATICAL_DETERMINER_DEFINITE;	//the
				addDeterminate = true;
			}
			else
			{	
				if(isPlural)	//added 03 August 2012
				{
					determinate = GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL;	//some
				}
				else
				{
					if(firstLetterIsVowel)
					{
						determinate = GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL;	//an
					}
					else
					{
						determinate = GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR;	//a
					}
				}
				addDeterminate = true;
			}
		}	
	}

	//that
	if(entityNode->entityName == GIA_SYN_REL_TRANSLATOR_RELATION_DEPENDENT_THAT)
	{
		addDeterminate = false;
	}
	
	//has alias (so proper noun)
	if(!(entityNode->aliasList.empty()))
	{
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
		{
			if(*aliasIter != GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
			{
				addDeterminate = false;
			}
		}
	}
	
	//time
	#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
	bool isTime = false;
	if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)	//added 4 August 2012
	{
		if(entityNode->timeConditionNode != NULL)
		{
			if(!(entityNode->timeConditionNode->tenseOnlyTimeCondition))
			{
				isTime = true;
			}
		}
	}
	#endif

	string determinateFinal = "";
	if(addDeterminate)
	{
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
		if(!isPerson)
		{
			if(!isPronoun)
			{
		#endif
				#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
				if(!isTime)
				{
				#endif

					determinateFinal = determinate + GIA_NLG_TEXT_SPACE;
				#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
				}
				#endif
		#ifdef GIA_NLG_SUPPORT_PERSON_AND_GENDER
			}
		}
		#endif
	}
	return determinateFinal;
}


string GIAnlgClass::calcNounWord(GIAentityNode* entityNode, const bool possessive)
{
	string nounWord = "";
	
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	nounWord = generateMorphology(entityNode, possessive);
	#else
	if(entityNode->wordOrig != "")
	{
		nounWord = entityNode->wordOrig;
	}
	else
	{
		nounWord = entityNode->entityName;
	}
	#ifdef GIA_NLG_POSSESSIVE_ADVANCED
	if(possessive)
	{
		if(nounWord[nounWord.length()-1] == GIA_NLG_POSSESSIVE_ADVANCED_ENTITY_POSSESSIVEENDING_TYPE_DETERMINER_CHARACTER)
		{	
			//last character of word is an s
			#ifdef GIA_NLG_POSSESSIVE_ADVANCED_ENTITY_POSSESSIVEENDING_TYPE_DEPENDENT_ON_PROPERNOUNS
			if(entityNode->grammaticalProperNounTemp)
			{
				nounWord = nounWord + ENTITY_POSSESSIVEENDING_APOSTROPHE_S;
			}
			else
			{
			#endif
				nounWord = nounWord + ENTITY_POSSESSIVEENDING_APOSTROPHE;
			#ifdef GIA_NLG_POSSESSIVE_ADVANCED_ENTITY_POSSESSIVEENDING_TYPE_DEPENDENT_ON_PROPERNOUNS
			}
			#endif
		}
		else
		{
			nounWord = nounWord + ENTITY_POSSESSIVEENDING_APOSTROPHE_S;
		}
	}
	#endif
	#endif

	//prepend quantity/negative indicator - added 25 July 2012
	if(entityNode->hasQuantity)
	{
		string quantityNumberStringTemp;
		if(entityNode->isQuery)
		{
			quantityNumberStringTemp = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
		}
		else
		{
			quantityNumberStringTemp = GIAentityNodeClass.printQuantityNumberString(entityNode);
		}
		nounWord = quantityNumberStringTemp + GIA_NLG_TEXT_SPACE + nounWord;

	}
	else if(entityNode->negative)
	{
		nounWord = string(GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_NEGATIVE_CONTEXT_1) + GIA_NLG_TEXT_SPACE + nounWord;
	}
	/*
	#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
	else if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)	//added 4 August 2012
	{
		if(entityNode->timeConditionNode != NULL)
		{
			if(!(entityNode->timeConditionNode->tenseOnlyTimeCondition))
			{
				nounWord = entityNode->timeConditionNode->conditionName + GIA_NLG_TEXT_SPACE + nounWord;
			}
		}
	}
	#endif
	*/
	#ifdef GIA_ALIASES
	//if(comparisonVariableNode->isNameQuery)	//who query
	else if(!(entityNode->aliasList.empty()))
	{
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
		{
			if(*aliasIter != GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
			{
				nounWord = *aliasIter;
			}		
		}
	}
	//}
	#endif
	else
	{

	}
	
	nounWord = nounWord + GIA_NLG_TEXT_SPACE;

	return nounWord;
}


string GIAnlgClass::generatePlurality(const string entityName, const int grammaticalNumber)
{
	string entityMorph = "";
	if(grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
	{
		entityMorph = entityName + GIA_NLG_MORPH_PLURALITY_APPEND;
	}
	else
	{
		entityMorph = entityName;
	}
	return entityMorph;
}




#endif

