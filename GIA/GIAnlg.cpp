/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAnlg.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3c5c 21-June-2017
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
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

	GIANLGentity* firstNLGentityInQuestionContext = new GIANLGentity();
	GIANLGentity* firstNLGentityInRelationshipNode = new GIANLGentity();
	GIANLGentity* firstNLGentityInAnswerNode = new GIANLGentity();
	GIANLGentity* currentNLGentityInQuestionContext = firstNLGentityInQuestionContext;
	GIANLGentity* currentNLGentityInRelationshipNode = firstNLGentityInRelationshipNode;
	GIANLGentity* currentNLGentityInAnswerNode = firstNLGentityInAnswerNode;
	
	//find the !sameReferencceSet connection of the entityNode (comparisonVariableNode) - there should only be 1 !sameReferencceSet connection and this should correspond to the previous word in the question
	GIAentityNode* relationshipEntity = NULL;
	GIAentityConnection* relationshipConnection = NULL;
	GIAentityNode* relationshipEntitySubject = NULL;	//lastNodeInQuestionContextReferenceSet
	GIAentityConnection* relationshipEntitySubjectConnection = NULL;
	bool relationshipEntitySubjectFound = false;	//lastNodeInQuestionContextReferenceSetFound
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		//if(connectionType != GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
		if(entityVectorConnectionGoToParentArray[connectionType])
		{
			GIAentityNode* relationshipEntityQuery = NULL;
			
			for(vector<GIAentityConnection*>::iterator connectionIter = comparisonVariableNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != comparisonVariableNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				GIAentityConnection* connection = *connectionIter;
				if(!(connection->sameReferenceSet))
				{
					relationshipEntityQuery = connection->entity;
					#ifdef GIA_NLG_DISPLAY_QUERY_CONDITIONS
					relationshipConnection = connection;
					relationshipEntity = connection->entity;
					#endif
					#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
					relationshipEntitySubjectConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(relationshipEntityQuery);
					#else
					if(entityVectorConnectionIsRelationshipPropertyOrDefinitionReverseArray[connection->connectionType])
					{
						relationshipEntitySubjectConnection = connection;
					}
					else
					{
						relationshipEntitySubjectConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(relationshipEntityQuery);
					}
					#endif
					relationshipEntitySubject = GIAtranslatorOperations.getRelationshipSubjectEntity(connection);
					relationshipEntitySubjectFound =  true;
				}
			}
			
			#ifndef GIA_NLG_DISPLAY_QUERY_CONDITIONS
			if(relationshipEntityQuery != NULL)
			{
				for(vector<GIAentityConnection*>::iterator connectionIter = queryAnswerNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != queryAnswerNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityConnection* connection = *connectionIter;
					if(connection->entity->queryAnswerContext)
					{
						relationshipConnection = connection;
						relationshipEntity = connection->entity;
					}
				}
			}
			#endif
		}
	}
	if(relationshipEntitySubjectFound)
	{
		if(!generateLanguageFromTextIteration(relationshipEntitySubjectConnection, &currentNLGentityInQuestionContext, true, true, 0, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(relationshipEntitySubjectConnection, currentNLGentityInQuestionContext, true..." << endl;
			exit(EXIT_ERROR);		
		}

		
		#ifdef GIA_NLG_DISPLAY_QUERY_CONDITIONS
		int entityRelationshipConditionQueryType = INT_DEFAULT_VALUE;
		bool entityRelationshipConditionQueryFound = false;
		if(SHAREDvars.textInTextArray(relationshipEntity->entityName, entityRelationshipConditionQueryArray, GIA_ENTITY_RELATIONSHIP_CONDITION_QUERY_NUMBER_OF_TYPES, &entityRelationshipConditionQueryType))
		{
			//temporarily replace condition query name with natural language condition name
			relationshipEntity->entityName = entityRelationshipConditionQueryCrossReferenceConditionArray[entityRelationshipConditionQueryType];
			entityRelationshipConditionQueryFound = true;
		}
		#endif
		if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
			exit(EXIT_ERROR);	
		}
		#ifdef GIA_NLG_DISPLAY_QUERY_CONDITIONS
		if(entityRelationshipConditionQueryFound)
		{
			relationshipEntity->entityName = entityRelationshipConditionQueryArray[entityRelationshipConditionQueryType];
		}
		#endif
	
	}
	else
	{		
		//assume who query; e.g. That is Jim. Who is that?
		
		GIAentityConnection* comparisonVariableNodeConnectionArtificial = new GIAentityConnection();
		comparisonVariableNodeConnectionArtificial->entity = comparisonVariableNode;
		comparisonVariableNodeConnectionArtificial->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION;	//artificially set to ensure that connection is not interpreted as relationship
		
		if(!generateLanguageFromTextIteration(comparisonVariableNodeConnectionArtificial, &currentNLGentityInQuestionContext, false, true, 0, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(comparisonVariableNode, currentNLGentityInQuestionContext, true..." << endl;
			exit(EXIT_ERROR);		
		}
	
		relationshipEntity = new GIAentityNode();
		relationshipConnection = new GIAentityConnection();
		relationshipConnection->entity = relationshipEntity;
		relationshipEntity->entityType = GIA_ENTITY_TYPE_DEFINITION;
		relationshipConnection->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION;
		if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
			exit(EXIT_ERROR);	
		}
		
		
		/*
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !relationshipEntitySubjectFound" << endl;
		exit(EXIT_ERROR);
		*/
	}
	
	
	//where, why, how many, how, who, etc query
	if(!generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, &currentNLGentityInAnswerNode, false, true, 0, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, firstNLGentityInAnswerNode, true..." << endl;
		exit(EXIT_ERROR);		
	}
				
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInQuestionContext);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInRelationshipNode);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInAnswerNode);
	
	return answerString;
}

/*	
string GIAnlgClass::generateLanguageFromWhichQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode)
{
	string answerString = "";
	
	GIAentityConnection* comparisonVariableNodeConnectionArtificial = new GIAentityConnection();
	GIAentityConnection* queryAnswerNodeConnectionArtificial = new GIAentityConnection();
	comparisonVariableNodeConnectionArtificial->entity = comparisonVariableNode;
	comparisonVariableNodeConnectionArtificial->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION;	//artificially set to ensure that connection is not interpreted as relationship
	queryAnswerNodeConnectionArtificial->entity = queryAnswerNode;
	queryAnswerNodeConnectionArtificial->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION;	//artificially set to ensure that connection is not interpreted as relationship
	
	GIANLGentity* firstNLGentityInAnswerContext = new GIANLGentity();
	GIANLGentity* firstNLGentityInRelationshipNode = new GIANLGentity();
	GIANLGentity* firstNLGentityInQuestionContext = new GIANLGentity();
	GIANLGentity* currentNLGentityInAnswerContext = firstNLGentityInAnswerContext;
	GIANLGentity* currentNLGentityInRelationshipNode = firstNLGentityInRelationshipNode;
	GIANLGentity* currentNLGentityInQuestionContext = firstNLGentityInQuestionContext;
		
	if(!generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, &currentNLGentityInAnswerContext, false, true, 0, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNodeConnectionArtificial, firstNLGentityInAnswerContext, false..." << endl;
		exit(EXIT_ERROR);			
	}
	
	GIAentityConnection* relationshipConnection = new GIAentityConnection();	//temporary auxiliary entity
	GIAentityNode* relationshipEntity = new GIAentityNode();
	relationshipConnection->entity = relationshipEntity;
	relationshipEntity->entityType = GIA_ENTITY_TYPE_DEFINITION;
	relationshipConnection->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION;
	if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
		exit(EXIT_ERROR);	
	}

	if(!generateLanguageFromTextIteration(comparisonVariableNodeConnectionArtificial, &currentNLGentityInQuestionContext, true, true, 0, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromWhichQuery error: !generateLanguageFromTextIteration(comparisonVariableNodeConnectionArtificial, firstNLGentityInQuestionContext, true..." << endl;
		exit(EXIT_ERROR);			
	}
	
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInAnswerContext);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInRelationshipNode);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInQuestionContext);
	
	return answerString;
}
*/

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



bool GIAnlgClass::generateLanguageFromTextIteration(GIAentityConnection* currentConnection, GIANLGentity** currentNLGentity, const bool parseSameReferenceSetOnly, const bool isSameReferenceSetIteration, const int interationIndex, const bool isRelationshipReverseIteration)
{
	bool result = true;
	
	GIAentityNode* currentEntity = currentConnection->entity;
	
	if(!(currentEntity->parsedForLanguageGeneration))
	{
		//cout << "currentEntity->entityName = " << currentEntity->entityName << endl;
		
		currentEntity->parsedForLanguageGeneration = true;
		
		#ifndef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
		bool passQualityPrelimChecks = true;
		//if(isSameReferenceSetIteration)
		//{
			if(currentEntity->entityType == GIA_ENTITY_TYPE_QUALITY)
			{
				passQualityPrelimChecks = false;
			}
		//}
		if(passQualityPrelimChecks)	
		{
		#endif

			if(GIAtranslatorOperations.connectionIsRelationship(currentConnection))
			{
				if(isRelationshipReverseIteration)
				{
					generateRelationshipEntityLanguageActionReverse(currentConnection, currentNLGentity, isSameReferenceSetIteration);
				}
				else
				{
					bool sameReferenceSet = isSameReferenceSetIteration;
					
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
								if(generateLanguageFromTextIterationPerformChecks(connection))
								{
									if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, false))
									{
										result = false;
									}
									sameReferenceSet = connection->sameReferenceSet;
								}
							#ifdef GIA_NLG_EXPECT_TRANSLATOR_QUERY_ACTION_AND_CONDITION_SAME_REFERENCE_SET_ASSIGNMENT_BUG_FIXED	
							}
							#endif
						}
					}

					generateRelationshipEntityLanguage(currentConnection, currentNLGentity, sameReferenceSet);
				}
			}
			else
			{		
				bool definite = true;	//CHECKTHIS
				if(!generateNounEntityLanguage(currentEntity, currentNLGentity, definite, parseSameReferenceSetOnly))
				{
					result = false;
				}
			}

			if(parseSameReferenceSetOnly || (interationIndex < GIA_NLG_GENERATE_LANGUAGE_MAX_NUM_ITERATIONS))
			{
				if(isRelationshipReverseIteration)
				{
					//special exception; parse action subject
					for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT].end(); connectionIter++)
					{	
						GIAentityConnection* connection = *connectionIter;
						if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
						{
							GIAentityNode* entity = connection->entity;
							if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, false))
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
						int subphraseCount = 0;
						vector<GIANLGentity*> NLGentitiesInSection;
						GIAentityConnection* connection = *connectionIter;
						if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
						{
							GIAentityNode* actionRelationshipEntity = connection->entity;
							if(generateLanguageFromTextIterationPerformChecks(connection))
							{
								subphraseCount++;
								NLGentitiesInSection.push_back(*currentNLGentity);
								
								if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, true))
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
									if(generateLanguageFromTextIterationPerformChecks(connection))
									{
										if(!entityVectorConnectionIsRelationshipSubjectObjectArray[connectionType])
										{
											subphraseCount++;
											NLGentitiesInSection.push_back(*currentNLGentity);
											//cout << "entity = " << entity->entityName << endl;
											//cout << "\tsubphraseCount = " << subphraseCount << endl;
										}

										if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, false))
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

bool GIAnlgClass::generateLanguageFromTextIterationPerformChecks(GIAentityConnection* connection)
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
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
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
	//if(isSameReferenceSetIteration)
	//{
		if(entity->entityType == GIA_ENTITY_TYPE_QUALITY)
		{
			result = false;	
		}
		if(compareRelationshipConnectionType(connection, GIA_ENTITY_TYPE_PROPERTY))
		{
			GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(connection);
			if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
			{
				result = false;
			}
		}
	//}
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







bool GIAnlgClass::generateRelationshipEntityLanguage(GIAentityConnection* relationshipConnection, GIANLGentity** currentNLGentity, const bool sameReferenceSet)
{
	bool result = true;
	
	string relationshipEntityText = "";

	bool generateAuxiliary = true;	//this variable is only relevant for non-actions
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
				generateAuxiliary = false;	
			}
		}
		#endif
		
		if(generateAuxiliary)
		{
			relationshipEntityText = relationshipEntityText + GIA_NLG_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET;
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
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	int grammaticalNumberIndex = GRAMMATICAL_NUMBER_SINGULAR;
	if(!(relationshipEntity->relationshipSubjectEntity->empty()))
	{
		GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getDefinitionRelationshipSubjectEntity(relationshipConnection);
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][grammaticalNumberIndex];
	#else
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
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	int grammaticalNumberIndex = GRAMMATICAL_NUMBER_SINGULAR;
	if(!(relationshipEntity->relationshipSubjectEntity->empty()))
	{
		GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getPropertyRelationshipSubjectEntity(relationshipConnection);
		grammaticalNumberIndex = relationshipEntitySubject->grammaticalNumber;
	}
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][grammaticalNumberIndex];
	#else
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipConnection->grammaticalTenseTemp][grammaticalNumberIndex];	
	#endif
	#endif
	return relationshipEntityText;
}





bool GIAnlgClass::generateNounEntityLanguage(GIAentityNode* nounEntity, GIANLGentity** currentNLGentity, const bool definite, const bool parseSameReferenceSetOnly)
{
	bool result = true;
	
	string determinateText = "";
	determinateText = determinateText + calcDeterminate(nounEntity, definite);
	(*currentNLGentity)->entityGrammatisedText = determinateText;
	(*currentNLGentity)->next = new GIANLGentity();
	(*currentNLGentity) = (*currentNLGentity)->next;	
	
	if(!generateQualityPrependText(nounEntity, currentNLGentity, parseSameReferenceSetOnly, true))
	{
		result = false;
	}
	
	string nounEntityText = "";
	nounEntityText = nounEntityText + calcNounWord(nounEntity);
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
		//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()	[see GIAtranslatorDefineGrammar.cpp]
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
	if(entityNode->entityName == RELATION_DEPENDENT_THAT)
	{
		addDeterminate = false;
	}
	
	//has alias (so proper noun)
	if(!(entityNode->aliasList.empty()))
	{
		for(vector<string>::iterator aliasIter = entityNode->aliasList.begin(); aliasIter != entityNode->aliasList.end(); aliasIter++)
		{
			if(*aliasIter != REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
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


string GIAnlgClass::calcNounWord(GIAentityNode* entityNode)
{
	string nounWord = "";
	
	#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
	nounWord = generateMorphology(entityNode);
	#else
	if(entityNode->wordOrig != "")
	{
		nounWord = entityNode->wordOrig;
	}
	else
	{
		nounWord = entityNode->entityName;
	}
	#endif

	//prepend quantity/negative indicator - added 25 July 2012
	if(entityNode->hasQuantity)
	{
		string quantityNumberStringTemp;
		if(entityNode->isQuery)
		{
			quantityNumberStringTemp = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
		}
		else
		{
			quantityNumberStringTemp = GIAentityNodeClass.printQuantityNumberString(entityNode);
		}
		nounWord = quantityNumberStringTemp + GIA_NLG_TEXT_SPACE + nounWord;

	}
	else if(entityNode->negative)
	{
		nounWord = string(RELATION_TYPE_NEGATIVE_CONTEXT_1) + GIA_NLG_TEXT_SPACE + nounWord;
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
			if(*aliasIter != REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
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

