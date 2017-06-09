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
 * Project Version: 3c1d 01-June-2017
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

string GIAnlgClass::generateLanguageFromNonWhichQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode)
{
	string answerString = "";
	GIANLGentity* firstNLGentityInQuestionContext = new GIANLGentity();
	GIANLGentity* firstNLGentityInRelationshipNode = new GIANLGentity();
	GIANLGentity* firstNLGentityInAnswerNode = new GIANLGentity();
	GIANLGentity* currentNLGentityInQuestionContext = firstNLGentityInQuestionContext;
	GIANLGentity* currentNLGentityInRelationshipNode = firstNLGentityInRelationshipNode;
	
	//find the !sameReferencceSet connection of the entityNode (comparisonVariableNode) - there should only be 1 !sameReferencceSet connection and this should correspond to the previous word in the question
	GIAentityNode* relationshipEntity = NULL;
	GIAentityConnection* relationshipConnection = NULL;
	GIAentityNode* relationshipEntitySubject = NULL;	//lastNodeInQuestionContextReferenceSet
	bool relationshipEntitySubjectFound = false;	//lastNodeInQuestionContextReferenceSetFound
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		//if(connectionType != GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
		if(entityVectorConnectionIsRelationshipReverseArray[connectionType])
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = comparisonVariableNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != comparisonVariableNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				GIAentityConnection* connection = *connectionIter;
				if(!(connection->sameReferenceSet))
				{
					relationshipEntity = connection->entity;
					relationshipConnection = connection;
					relationshipEntitySubject = GIAtranslatorOperations.getRelationshipSubjectEntity(connection);
					relationshipEntitySubjectFound =  true;
				}
			}
		}
	}
	if(relationshipEntitySubjectFound)
	{
		if(!generateLanguageFromTextIteration(relationshipEntitySubject, &currentNLGentityInQuestionContext, true, true, 0, false))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromTextIteration" << endl;
			exit(EXIT_ERROR);		
		}

		/*
		if(!generateLanguageFromSameSentenceReferenceSet(relationshipEntitySubject, firstNLGentityInQuestionContext, true))
		{
			cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateLanguageFromSameSentenceReferenceSet(relationshipEntitySubject, firstNLGentityInQuestionContext)" << endl;
			exit(EXIT_ERROR);			
		}
		*/
	}
	else
	{
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !lastNodeInQuestionContextReferenceSetFound" << endl;
		exit(EXIT_ERROR);
	}
	
	if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
		exit(EXIT_ERROR);	
	}

	#ifdef GIA_ALIASES
	if(comparisonVariableNode->isNameQuery)	//who query
	{
		for(vector<string>::iterator aliasIter = queryAnswerNode->aliasList.begin(); aliasIter != queryAnswerNode->aliasList.end(); aliasIter++)
		{
			firstNLGentityInAnswerNode->entityGrammatisedText = firstNLGentityInAnswerNode->entityGrammatisedText + *aliasIter;	//CHECKTHIS; this is not working yet?
		}
	}
	else
	{
	#endif
		if(comparisonVariableNode->hasQuantity)	//how many query
		{
			firstNLGentityInAnswerNode->entityGrammatisedText = firstNLGentityInAnswerNode->entityGrammatisedText + SHAREDvars.convertIntToString(queryAnswerNode->quantityNumber) + GIA_NLG_TEXT_SPACE;
			//queryAnswerNode->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;	//should already be true
		}
		
		firstNLGentityInAnswerNode->entityGrammatisedText = firstNLGentityInAnswerNode->entityGrammatisedText + calcNounWord(queryAnswerNode);
		
	#ifdef GIA_ALIASES
	}
	#endif
				
	
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInQuestionContext);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInRelationshipNode);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInAnswerNode);
	
	return answerString;
}
	
string GIAnlgClass::generateLanguageFromWhichQuery(GIAentityNode* comparisonVariableNode, GIAentityNode* queryAnswerNode)
{
	string answerString = "";
	
	GIANLGentity* firstNLGentityInAnswerContext = new GIANLGentity();
	GIANLGentity* firstNLGentityInRelationshipNode = new GIANLGentity();
	GIANLGentity* firstNLGentityInQuestionContext = new GIANLGentity();
	GIANLGentity* currentNLGentityInAnswerContext = firstNLGentityInAnswerContext;
	GIANLGentity* currentNLGentityInRelationshipNode = firstNLGentityInRelationshipNode;
	GIANLGentity* currentNLGentityInQuestionContext = firstNLGentityInQuestionContext;
		
	if(!generateLanguageFromTextIteration(queryAnswerNode, &currentNLGentityInAnswerContext, false, true, 0, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromWhichQuery error: !generateLanguageFromTextIteration(queryAnswerNode, firstNLGentityInAnswerContext, false..." << endl;
		exit(EXIT_ERROR);			
	}
	
	GIAentityConnection* relationshipConnection = new GIAentityConnection();	//temporary auxiliary entity
	relationshipConnection->connectionType = GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION;
	if(!generateRelationshipEntityLanguage(relationshipConnection, &currentNLGentityInRelationshipNode, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromNonWhichQuery error: !generateRelationshipEntityLanguage(relationshipConnection, currentNLGentityInRelationshipNode, false))" << endl;
		exit(EXIT_ERROR);	
	}

	if(!generateLanguageFromTextIteration(comparisonVariableNode, &currentNLGentityInQuestionContext, true, true, 0, false))
	{
		cerr << "GIAnlgClass::generateLanguageFromWhichQuery error: !generateLanguageFromTextIteration(comparisonVariableNode, firstNLGentityInQuestionContext, true..." << endl;
		exit(EXIT_ERROR);			
	}
	
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInAnswerContext);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInRelationshipNode);
	answerString = answerString + generateLanguageFromNLGentityList(firstNLGentityInQuestionContext);
	
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


/*
bool GIAnlgClass::generateLanguageFromSameSentenceReferenceSetParent(GIAentityNode* firstEntityInSameReferenceSet, GIANLGentity* firstNLGentity)
{
	bool result = true;
	GIAentityNode* parentEntity = getParentEntityInSameReferenceSet(firstEntityInSameReferenceSet);
	if(!generateLanguageFromTextIteration(parentEntity, firstNLGentity, true, true, 0, false))
	{
		result = false;
	}
	return result;
}

bool GIAnlgClass::getParentEntityInSameReferenceSet(GIAentityNode* currentEntityInSameReferenceSet, GIAentityNode** parentEntity)
{
	bool foundParentEntity = false;
	for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		if(entityVectorConnectionGoToParentArray[connectionType])
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = currentEntityInSameReferenceSet->entityVectorConnectionsArray[connectionType].begin(); connectionIter != currentEntityInSameReferenceSet->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				GIAentityConnection* connection = *connectionIter;
				if(connection->sameReferenceSet)
				{
					GIAentityNode* parentEntityTemp = NULL;
					if(getParentEntityInSameReferenceSet(connection->entity, &parentEntityTemp))
					{
						*parentEntity = parentEntityTemp;
						foundParentEntity = true;
					}
				}
			}
		}
	}
	
	return foundParentEntity;
}
*/

bool GIAnlgClass::generateLanguageFromTextIteration(GIAentityConnection* currentConnection, GIANLGentity** currentNLGentity, bool parseSameReferenceSetOnly, bool isSameReferenceSetIteration, int interationIndex, bool isRelationshipReverseIteration)
{
	bool result = true;
	
	GIAentityNode* currentEntity = currentConnection->entity;
	
	#ifdef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
	bool passQualityPrelimChecks = true;
	if(isSameReferenceSetIteration)
	{
		if(currentEntity->entityType == GIA_ENTITY_TYPE_QUALITY)
		{
			passQualityPrelimChecks = false;
		}
	}
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
				generateRelationshipEntityLanguage(currentConnection, currentNLGentity, isSameReferenceSetIteration);
			}
		}
		else
		{						
			if(!generateQualityPrependText(currentConnection, currentNLGentity, true))
			{
				result = false;
			}
			bool definite = true;	//CHECKTHIS
			if(!generateNounEntityLanguage(currentEntity, currentNLGentity, definite))
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
				//special exception; parse reverse conditions
				for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE].end(); connectionIter++)
				{
					int qualityCount = 0;
					vector<GIANLGentity*> NLGentitiesInSection;
					GIAentityConnection* connection = *connectionIter;
					if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
					{
						GIAentityNode* actionRelationshipEntity = connection->entity;
						qualityCount++;
						NLGentitiesInSection.push_back(*currentNLGentity);
						if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, true))
						{
							result = false;
						}
					}
					if(!generateConjunctionOfNLGentitiesInSection(qualityCount, &NLGentitiesInSection))
					{
						result = false;
					}
				}

				for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
				{
					int qualityCount = 0;
					vector<GIANLGentity*> NLGentitiesInSection;
					if(entityVectorConnectionGoToChildArray[connectionType])
					{
						for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
						{
							GIAentityConnection* connection = *connectionIter;
							if(connection->sameReferenceSet || !parseSameReferenceSetOnly)
							{
								GIAentityNode* entity = connection->entity;
								qualityCount++;
								NLGentitiesInSection.push_back(*currentNLGentity);
								if(!generateLanguageFromTextIteration(connection, currentNLGentity, parseSameReferenceSetOnly, true, interationIndex+1, false))
								{
									result = false;
								}
							}
						}
					}
					if(!entityVectorConnectionIsRelationshipSubjectObjectArray[connectionType])
					{
						if(!generateConjunctionOfNLGentitiesInSection(qualityCount, &NLGentitiesInSection))
						{
							result = false;
						}
					}
				}
			}
		}
	#ifdef GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES
	}
	#endif
		
	return result;
}

bool GIAnlgClass::generateQualityPrependText(GIAentityNode* currentEntity, GIANLGentity** currentNLGentity, bool testSameReferenceSet)	//specific to english
{
	bool result = true;
	
	string qualityText = "";	//eg the blue apple / the blue and happy apple / the blue, happy and bright apple
	int qualityCount = 0;
	//GIANLGentity* firstNLGentityInSection = *currentNLGentity;
	vector<GIANLGentity*> NLGentitiesInSection;
	for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[GIA_ENTITY_TYPE_PROPERTY].begin(); connectionIter != currentEntity->entityVectorConnectionsArray[GIA_ENTITY_TYPE_PROPERTY].end(); connectionIter++)
	{
		GIAentityConnection* connection = *connectionIter;
		GIAentityNode* relationshipEntityObject = GIAtranslatorOperations.getRelationshipObjectEntity(connection);
		if(connection->sameReferenceSet || !testSameReferenceSet)
		{
			if(relationshipEntityObject->entityType == GIA_ENTITY_TYPE_QUALITY)
			{
				NLGentitiesInSection.push_back((*currentNLGentity));
				#ifdef GIA_NLG_MORPHOLOGY_GENERATOR
				(*currentNLGentity)->entityGrammatisedText = generateMorphology(relationshipEntityObject);
				#else
				(*currentNLGentity)->entityGrammatisedText = relationshipEntityObject->entityName;	//qualities/adjectives don't have lemmas and so don't require morphological generation
				#endif
				(*currentNLGentity)->next = new GIANLGentity();
				(*currentNLGentity) = (*currentNLGentity)->next;
				qualityCount++;
			}		
		}
	}
	
	if(!generateConjunctionOfNLGentitiesInSection(qualityCount, &NLGentitiesInSection))
	{
		result = false;
	}
	
	return result;
}

bool GIAnlgClass::generateConjunctionOfNLGentitiesInSection(int qualityCountMax, vector<GIANLGentity*>* NLGentitiesInSection)
{
	bool result = true;
	
	int qualityCount = 1;
	for(vector<GIANLGentity*>::iterator iter = NLGentitiesInSection->begin(); iter != NLGentitiesInSection->end(); iter++)
	{
		GIANLGentity* currentNLGentityInSection = *iter;
		if(qualityCount < qualityCountMax-1)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND_IMPLICIT;		
		}
		else if(qualityCount < qualityCountMax)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND;
		}
		else if(qualityCount == qualityCountMax)
		{

		}
		qualityCount++;
	}	
	
	return result;	
}

/*
bool GIAnlgClass::generateConjunctionOfNLGentitiesInSection(int qualityCountMax, GIANLGentity* firstNLGentityInSection)
{
	bool result = true;
	
	GIANLGentity* currentNLGentityInSection = firstNLGentityInSection;
	qualityCount = 1;
	while(currentNLGentityInSection->next != NULL)
	{
		if(qualityCount < qualityCountMax-1)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND_IMPLICIT;		
		}
		else if(qualityCount < qualityCountMax)
		{
			currentNLGentityInSection->entityGrammatisedText = currentNLGentityInSection->entityGrammatisedText + GIA_NLG_RELATIONSHIP_ENTITY_CONJUNCTION_AND;
		}
		else if(qualityCount == qualityCountMax)
		{

		}
		qualityCount++;
		currentNLGentityInSection = currentNLGentityInSection->next;
	}	
	
	return result;	
}
*/







bool GIAnlgClass::generateRelationshipEntityLanguage(GIAentityConnection* relationshipConnection, GIANLGentity** currentNLGentity, bool sameReferenceSet)
{
	bool result = false;
	
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
		else if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_PROPERTY))
		{
			//if property is a quality, then use "is" as auxiliary
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
		else if(compareRelationshipConnectionType(relationshipConnection, GIA_ENTITY_TYPE_PROPERTY)
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

bool GIAnlgClass::generateRelationshipEntityLanguageActionReverse(GIAentityNode* relationshipConnection, GIANLGentity** currentNLGentity, bool sameReferenceSet)
{
	bool result = false;
	
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



string GIAnlgClass::generateMorphologyAction(GIAentityNode* relationshipConnection)
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

string GIAnlgClass::generateMorphologyActionReverse(GIAentityNode* relationshipConnection)
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

string GIAnlgClass::generateMorphologyCondition(GIAentityNode* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntity = relationshipConnection->entity;
	relationshipEntityText = relationshipEntityText + relationshipEntity->entityName;	//eg "near"
	relationshipEntityText = relationshipEntityText + GIA_NLG_TEXT_SPACE;
	return relationshipEntityText;
}



string GIAnlgClass::generateMorphologyRelationshipAuxiliaryBe(GIAentityNode* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getDefinitionRelationshipSubjectEntity(relationshipConnection);
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
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][relationshipEntitySubject->grammaticalNumber];
	#else
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceBeAuxiliaryArray[relationshipConnection->grammaticalTenseTemp][relationshipEntitySubject->grammaticalNumber];	
	#endif
	#endif
	return relationshipEntityText;
}

string GIAnlgClass::generateMorphologyRelationshipAuxiliaryHave(GIAentityConnection* relationshipConnection)
{
	string relationshipEntityText = "";
	GIAentityNode* relationshipEntitySubject = GIAtranslatorOperations.getPropertyRelationshipSubjectEntity(relationshipConnection);
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
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipEntity->grammaticalTenseTemp][relationshipEntitySubject->grammaticalNumber];
	#else
	relationshipEntityText = relationshipEntityText + entityRelationshipTenseNumerosityCrossReferenceHaveAuxiliaryArray[relationshipConnection->grammaticalTenseTemp][relationshipEntitySubject->grammaticalNumber];	
	#endif
	#endif
	return relationshipEntityText;
}





bool GIAnlgClass::generateNounEntityLanguage(GIAentityNode* nounEntity, GIANLGentity** currentNLGentity, bool definite)
{
	bool result = false;
	
	string nounEntityText = "";
	nounEntityText = nounEntityText + calcDeterminate(nounEntity, definite);
	nounEntityText = nounEntityText + calcNounWord(nounEntity);
	
	(*currentNLGentity)->entityGrammatisedText = nounEntityText;
	(*currentNLGentity)->next = new GIANLGentity();
	(*currentNLGentity) = (*currentNLGentity)->next;
	
	return result;
}

string GIAnlgClass::calcDeterminate(const GIAentityNode* entityNode, const bool definite)
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
		if(definite)
		{
			determinate = GRAMMATICAL_DETERMINER_DEFINITE;	//the
			addDeterminate = true;
		}
		else
		{	if(!(entityNode->hasQuantity))
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


string GIAnlgClass::calcNounWord(const GIAentityNode* entityNode)
{
	string nounWord = "";
	#ifdef GIA_NLG_NO_MORPHOLOGY_GENERATOR
	if(entityNode->wordOrig != "")
	{
		nounWord = entityNode->wordOrig;		//IMPORTANT; due to bug in nlg2, nlg2 currently requires the original word, not the lemma
	}
	else
	{
		nounWord = entityNode->entityName;
	}
	#else
	nounWord = generateMorphology(entityNode);
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
		nounWord = string(RELATION_TYPE_NEGATIVE_CONTEXT_1) + " " + nounWord;
	}
	#ifdef GIA_NLG_SUPPORT_TIME_CONDITIONS
	else if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)	//added 4 August 2012
	{
		if(entityNode->timeConditionNode != NULL)
		{
			if(!(entityNode->timeConditionNode->tenseOnlyTimeCondition))
			{
				nounWord = entityNode->timeConditionNode->conditionName;
			}
		}
	}
	#endif
	else
	{

	}

	return nounWord;
}


string GIAnlgClass::generatePlurality(string entityName, int grammaticalNumber)
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

