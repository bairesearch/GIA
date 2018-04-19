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
 * File Name: GIAsynRelTranslatorLinkEntitiesDynamic.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1g 22-February-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAsynRelTranslatorLinkEntitiesDynamic.hpp"



void GIAsynRelTranslatorLinkEntitiesDynamicClass::linkEntitiesDynamic(GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
	linkEntitiesDynamicPrenominalModifierOfNoun(translatorVariables);
	#endif

	#ifdef GIA_SYN_REL_TRANSLATOR_DYNAMICALLY_LINK_FROM_CONDITIONS
	linkEntitiesDynamicFromConditions(translatorVariables);
	#endif
}


#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
void GIAsynRelTranslatorLinkEntitiesDynamicClass::linkEntitiesDynamicPrenominalModifierOfNoun(GIAtranslatorVariablesClass* translatorVariables)
{
	//dynamically determine type of linking implied by NN based on existence of previous definition/property/condition links between the NN dependent/governor
	/*
	Hamish smoked at the toy shop.	_nn(shop[6], toy[5])	//direction=true (same with respect to property-child connection)
	A pawn is a chess character.	_nn(character[6], chess[5])
	The chess game is good.		_nn(game[3], chess[2])
	The ball is near the goal line.	_nn(line[7], goal[6])	//direction=false (reverse with respect to property-child connection)
		NB compare ordering with Tom's boat is happy.	poss(boat-3, Tom-1)	//direction=false (reverse with respect to property-child connection)
	*/

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool prenominalModifierFound = false;
			#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER])
			{
				prenominalModifierFound = true;
			}
			#endif
			if(SHAREDvars.textInTextArray(currentRelationInList->relationType, relationTypePrenominalModifierNameArray, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES))
			{
				prenominalModifierFound = true;
			}
			if(prenominalModifierFound)
			{
				//prenominal modifier found...

				string entity1Name = currentRelationInList->relationGovernor;
				string entity2Name = currentRelationInList->relationDependent;
				int entity1Index = currentRelationInList->relationGovernorIndex;
				int entity2Index = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity1 = (*translatorVariables->GIAentityNodeArray)[entity1Index];
				GIAentityNode* entity2 = (*translatorVariables->GIAentityNodeArray)[entity2Index];

				int relationTypeIndex = currentRelationInList->relationTypeIndex;


				//check if chess (dependent) [primary] is a game (governor), or if goal (dependent) has a line (governor) [primary], or if shop (governor) has toy (dependent), or if line (dependent) is in the goal (dependent)
				bool direction1Found = false;
				bool direction2Found = false;
				direction1Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, translatorVariables, entity1, entity2, entity1Index, entity2Index, relationTypeIndex, true);
				if(!direction1Found)
				{
					direction2Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, translatorVariables, entity2, entity1, entity2Index, entity1Index, relationTypeIndex, false);
				}

				if(!direction1Found && !direction2Found)
				{
					#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_DEFINITE
					if(entity2->grammaticalDefiniteTemp)
					{
						entity1->grammaticalDefiniteTemp = true;
						entity2->grammaticalDefiniteTemp = false;
					}
					#endif

					/*//removed 2i30c - the rationale behind this change is that if the correct relationship between the items in the _nn case is unknown, then it is best not to reassign concepts thereof
					#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_CONCEPT_IF_NECESSARY
					if(entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)
					{
						entity1->entityType = GIA_ENTITY_TYPE_CONCEPT;	//this may not be used currently because concept prenominal modifiers are interpreted by at least stanford NLP as _amod not _nn (_amod(line[2], goal[1]))
					}
					else
					{
						entity1->entityType = GIA_ENTITY_TYPE_SUBSTANCE;
					}
					#endif
					*/

					//use default linking (property link)
					bool sameReferenceSet = true;

					connectPropertyToEntityFull(translatorVariables, entity1, entity2, entity1Index, entity2Index, sameReferenceSet, true);
					//OLD: (*translatorVariables->GIAentityNodeArray)[entity2Index] = connectPropertyToEntity(entity1, entity2, sameReferenceSet);
				}
			}
		#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

bool GIAsynRelTranslatorLinkEntitiesDynamicClass::linkEntitiesDynamicPrenominalModifierOfNounDirection(const GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, int relationTypeIndex, const bool direction)
{
	bool previousRelationshipFound = false;
	bool previousDefinitionRelationshipFound = false;
	bool previousPropertyRelationshipFound = false;
	bool previousConditionRelationshipFound = false;

	bool sameReferenceSet = true;

	//find the most recent reference (latest sentence);
	for(map<int, vector<GIAentityNode*>*>::reverse_iterator sentenceIter = entityNodesActiveListSentences->rbegin(); sentenceIter != entityNodesActiveListSentences->rend(); sentenceIter++)
	{
		if(!previousRelationshipFound)
		{
			int sentenceIndex = sentenceIter->first;
			vector<GIAentityNode*>* entityNodesActiveListSentence = sentenceIter->second;

			for(vector<GIAentityNode*>::iterator entityNodesActiveListSentenceIterator = entityNodesActiveListSentence->begin(); entityNodesActiveListSentenceIterator != entityNodesActiveListSentence->end(); entityNodesActiveListSentenceIterator++)
			{
				if(!previousRelationshipFound)
				{
					GIAentityNode* instanceEntity = *entityNodesActiveListSentenceIterator;

					if(instanceEntity->entityName == entity1->entityName)
					{
						GIAentityNode* targetEntityFound = NULL;
						if(findPreviousRelationship(instanceEntity, entity2, &targetEntityFound, &previousDefinitionRelationshipFound, &previousPropertyRelationshipFound, &previousConditionRelationshipFound))
						{
							previousRelationshipFound = true;
							if(previousDefinitionRelationshipFound)
							{
								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_CONCEPTS
								if(((targetEntityFound->entityType == GIA_ENTITY_TYPE_CONCEPT) && (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)) || (!(targetEntityFound->entityType == GIA_ENTITY_TYPE_CONCEPT) && !(entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)))
								{
								#endif
									connectDefinitionToEntityFull(translatorVariables, entity1, entity2, entity1Index, entity2Index, sameReferenceSet);
								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_CONCEPTS
								}
								else
								{
									connectDefinitionToEntityFull(translatorVariables, entity2, entity1, entity2Index, entity1Index, sameReferenceSet);
								}
								#endif
							}
							else if(previousPropertyRelationshipFound)
							{

								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_DEFINITE
								if(entity2->grammaticalDefiniteTemp)
								{
									entity1->grammaticalDefiniteTemp = true;
									entity2->grammaticalDefiniteTemp = false;
								}
								#endif
								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_CONCEPT_IF_NECESSARY
								if(entity2->entityType == GIA_ENTITY_TYPE_CONCEPT)
								{
									entity1->entityType = GIA_ENTITY_TYPE_CONCEPT;	//this may not be used currently because concept prenominal modifiers are interpreted by at least stanford NLP as _amod not _nn (_amod(line[2], goal[1]))
								}
								else
								{
									entity1->entityType = GIA_ENTITY_TYPE_SUBSTANCE;
								}
								#endif

								connectPropertyToEntityFull(translatorVariables, entity1, entity2, entity1Index, entity2Index, sameReferenceSet, direction);

							}
							else if(previousConditionRelationshipFound)
							{
								int featureIndexOfPreposition = connectConditionToEntityFull(translatorVariables, entity1, entity2, entity1Index, entity2Index, targetEntityFound->entityName, relationTypeIndex, sameReferenceSet);

								#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
								if(currentRelationInList->relationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_conditionToEntity is called based on a single GIArelation
								{
									(*translatorVariables->GIAentityNodeArray)[featureIndexOfPreposition]->conditionTwoWay = true;	//sets conditionTwoWay for condition substance not networkIndex
								}
								#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
								if(currentRelationInList->inverseRelationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_conditionToEntity is called based on a single GIArelation
								{
									(*translatorVariables->GIAentityNodeArray)[featureIndexOfPreposition]->inverseConditionTwoWay = true;	//sets inverseConditionTwoWay for condition substance not networkIndex
								}
								#endif
								#endif
							}
						}
					}
				}
			}
		}
	}
	return previousRelationshipFound;
}
#endif

#ifdef GIA_SYN_REL_TRANSLATOR_DYNAMICALLY_LINK_FROM_CONDITIONS
//preconditions: requires redistributeStanfordRelationsConnectToAndFromConditions{} and redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject{} to have been executed
void GIAsynRelTranslatorLinkEntitiesDynamicClass::linkEntitiesDynamicFromConditions(GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	Move the chicken from the pie to the apple.
	Move the chicken from the red pie to the apple.
	Move the red chicken from the pie to the apple.
	*/

	bool sameReferenceSet = true;

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if(SHAREDvars.textInTextArray((*translatorVariables->GIAentityNodeArray)[w]->entityName, relationTypePrepositionFromNameArray, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PREPOSITION_FROM_NUMBER_OF_TYPES))
			{
				GIAentityNode* fromCondition = (*translatorVariables->GIAentityNodeArray)[w];

				if(!(fromCondition->relationshipObjectEntity->empty()))
				{
					GIAentityNode* fromConditionObject = (fromCondition->relationshipObjectEntity->back())->entity;
					GIAentityNode* sourceLocationEntity = fromConditionObject;

					if(!(fromCondition->relationshipSubjectEntity->empty()))
					{
						bool foundFromConditionParent = false;
						GIAentityNode* fromConditionParent = NULL;
						GIAentityNode* fromConditionSubject = (fromCondition->relationshipSubjectEntity->back())->entity;

						//now see if from condition subject is an action (if so, perform the relinking to the action object instead of the from condition subject)
						if(fromConditionSubject->entityType == GIA_ENTITY_TYPE_ACTION)
						{
							#ifndef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES
							if(SHAREDvars.textInTextArray(fromConditionSubject->entityName, dynamicallyLinkFromConditionsActionTypesArray, GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES))
							{
							#endif
								/*
								NB not parsed this way by Stanford (but has been changed to this using redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject) :
								eg move -> chicken
									\
									from
									  \
									   pie
								*/

								if(!(fromConditionSubject->relationshipObjectEntity->empty()))
								{
									GIAentityNode* fromConditionSubjectActionObject = (fromConditionSubject->relationshipObjectEntity->back())->entity;
									fromConditionParent = fromConditionSubjectActionObject;
									foundFromConditionParent = true;
								}
							#ifndef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES
							}
							#endif
						}
						/*no longer supported with redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject;
						else
						{

							//NB parsed this way by Stanford:
							//eg move -> chicken
							//		\
							//		from
							//		  \
							//		   pie
							fromConditionParent = fromConditionSubject;
							foundFromConditionParent = true;
						}
						*/

						if(foundFromConditionParent)
						{
							GIAentityNode* sourceEntity = fromConditionParent;

							//now see if there exists a previous property or condition relationship between sourceEntity/fromConditionObject->entityName and sourceLocationEntity/fromConditionParent->entityName
							bool previousRelationshipFound = false;
							bool previousDefinitionRelationshipFound = false;
							bool previousPropertyRelationshipFound = false;
							bool previousConditionRelationshipFound = false;
							GIAentityNode* targetEntityFound = NULL;

							//find the most recent reference (latest sentence);
							for(map<int, vector<GIAentityNode*>*>::reverse_iterator sentenceIter = translatorVariables->entityNodesActiveListSentences->rbegin(); sentenceIter != translatorVariables->entityNodesActiveListSentences->rend(); sentenceIter++)
							{
								if(!previousRelationshipFound)
								{
									int sentenceIndex = sentenceIter->first;
									vector<GIAentityNode*>* entityNodesActiveListSentence = sentenceIter->second;

									for(vector<GIAentityNode*>::iterator entityNodesActiveListSentenceIterator = entityNodesActiveListSentence->begin(); entityNodesActiveListSentenceIterator != entityNodesActiveListSentence->end(); entityNodesActiveListSentenceIterator++)
									{
										if(!previousRelationshipFound)
										{
											GIAentityNode* instanceEntity = *entityNodesActiveListSentenceIterator;
											bool previousDefinitionRelationshipFoundTemp = false;
											bool previousPropertyRelationshipFoundTemp = false;
											bool previousConditionRelationshipFoundTemp = false;
											if(instanceEntity->entityName == sourceLocationEntity->entityName)
											{
												if(findPreviousRelationship(instanceEntity, sourceEntity, &targetEntityFound, &previousDefinitionRelationshipFoundTemp, &previousPropertyRelationshipFoundTemp, &previousConditionRelationshipFoundTemp))
												{
													if(previousPropertyRelationshipFoundTemp)
													{
														previousRelationshipFound = true;
														previousPropertyRelationshipFound = true;
													}
												}
											}
											else if(instanceEntity->entityName == sourceEntity->entityName)
											{
												if(findPreviousRelationship(instanceEntity, sourceLocationEntity, &targetEntityFound, &previousDefinitionRelationshipFoundTemp, &previousPropertyRelationshipFoundTemp, &previousConditionRelationshipFoundTemp))
												{
													if(previousConditionRelationshipFoundTemp)
													{
														#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ONLY_ACCEPT_AT_CONDITIONS

														#endif
															previousRelationshipFound = true;
															previousConditionRelationshipFound = true;
														#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ONLY_ACCEPT_AT_CONDITIONS

														#endif
													}
												}
											}
										}
									}
								}
							}

							if(previousPropertyRelationshipFound || !previousRelationshipFound)
							{

								bool direction = false;	//always define from condition artificial property parent relationship as a poss relationship (this is done for NLC compatibility; such that NLC will parse the same reference set child entity)
								int sourceLocationEntityIndex = getEntityIndex(translatorVariables, sourceLocationEntity);
								int sourceEntityIndex = getEntityIndex(translatorVariables, sourceEntity);
								connectPropertyToEntityFull(translatorVariables, sourceLocationEntity, sourceEntity, sourceLocationEntityIndex, sourceEntityIndex, sameReferenceSet);
							}
							else if(previousConditionRelationshipFound)
							{

								int sourceLocationEntityIndex = getEntityIndex(translatorVariables, sourceLocationEntity);
								int sourceEntityIndex = getEntityIndex(translatorVariables, sourceEntity);
								connectConditionToEntityFull(translatorVariables, sourceEntity, sourceLocationEntity, sourceEntityIndex, sourceLocationEntityIndex, targetEntityFound->entityName, INT_DEFAULT_VALUE, sameReferenceSet);
							}
						}
					}
				}
			}
		}
	}
}

#endif

bool GIAsynRelTranslatorLinkEntitiesDynamicClass::findPreviousRelationship(GIAentityNode* instanceEntity, const GIAentityNode* comparisonEntity, GIAentityNode** targetEntityFound, bool* previousDefinitionRelationshipFound, bool* previousPropertyRelationshipFound, bool* previousConditionRelationshipFound)
{
	bool previousRelationshipFound = false;

	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->definitionNodeList->begin(); connectionIter2 != instanceEntity->definitionNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* definitionRelationshipObjectEntity = GIAtranslatorOperations.getDefinitionRelationshipObjectEntity(*connectionIter2);
			if(definitionRelationshipObjectEntity->entityName == comparisonEntity->entityName)
			{
				previousRelationshipFound = true;
				*previousDefinitionRelationshipFound = true;

				*targetEntityFound = definitionRelationshipObjectEntity;

			}
		}
	}
	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->propertyNodeList->begin(); connectionIter2 != instanceEntity->propertyNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* propertyRelationshipObjectEntity = GIAtranslatorOperations.getPropertyRelationshipObjectEntity(*connectionIter2);
			if(propertyRelationshipObjectEntity->entityName == comparisonEntity->entityName)
			{
				previousRelationshipFound = true;
				*previousPropertyRelationshipFound = true;

				*targetEntityFound = propertyRelationshipObjectEntity;	//REDUNDANT

			}
		}
	}

	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->conditionNodeList->begin(); connectionIter2 != instanceEntity->conditionNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* conditionRelationshipEntity = (*connectionIter2)->entity;
			GIAentityNode* conditionRelationshipObjectEntity = NULL;

			if(!(conditionRelationshipEntity->relationshipObjectEntity->empty()))
			{
				conditionRelationshipObjectEntity = (conditionRelationshipEntity->relationshipObjectEntity->back())->entity;
				if(conditionRelationshipObjectEntity->entityName == comparisonEntity->entityName)
				{
					previousRelationshipFound = true;
					*previousConditionRelationshipFound = true;

					*targetEntityFound = conditionRelationshipEntity;

					//#ifdef GIA_TRANSLATOR_DEBUG
					cout << "previousConditionRelationshipFound" << endl;
					//#endif

				}
			}
			else
			{
				cout << "findPreviousRelationship{} error: condition entity has no condition object entity" << endl;
			}
		}
	}
	return previousRelationshipFound;
}

void GIAsynRelTranslatorLinkEntitiesDynamicClass::connectDefinitionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet)
{
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addDefinitionRelationshipToEntity(entity1, entity2, sameReferenceSet, translatorVariables);

	/*OLD:
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(translatorVariables);
	int relationshipEntityIndex = relationshipEntity->entityIndexTemp;
	(*translatorVariables->GIAentityNodeArray)[relationshipEntityIndex] = GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2, relationshipEntity, sameReferenceSet);
	*/
	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT, entity1Index, entity2Index, sameReferenceSet);
	#endif
	#endif
}

void GIAsynRelTranslatorLinkEntitiesDynamicClass::connectPropertyToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet)
{
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addPropertyRelationshipToEntity(entity1, entity2, sameReferenceSet, translatorVariables);
	
	/*OLD:
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(translatorVariables);
	int relationshipEntityIndex = relationshipEntity->entityIndexTemp;
	(*translatorVariables->GIAentityNodeArray)[relationshipEntityIndex] = GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2, relationshipEntity, sameReferenceSet);
	*/
	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, entity1Index, entity2Index, sameReferenceSet);
	#endif
	#endif
}

int GIAsynRelTranslatorLinkEntitiesDynamicClass::connectConditionToEntityFull(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, const string conditionEntityName, int conditionIndex, bool sameReferenceSet)
{
	GIAentityNode* conditionNetworkIndexEntity;
	int featureIndexOfPreposition = conditionIndex;
	if(featureIndexOfPreposition == INT_DEFAULT_VALUE)
	{
		featureIndexOfPreposition = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
		translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
	}

	bool entityAlreadyExistant = false;
	string conditionName = conditionEntityName;	//conditionRelationshipEntity->entityName;
	conditionNetworkIndexEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(featureIndexOfPreposition, &conditionName, &entityAlreadyExistant, translatorVariables);
	//Alternative (need to fill GIAentityNodeArrayFilled);
	//conditionNetworkIndexEntity = (conditionRelationshipEntity->instanceReverseNodeList->back())->entity;

	GIAtranslatorOperations.connectConditionToEntity(entity1, entity2, conditionNetworkIndexEntity, sameReferenceSet, translatorVariables);
	if(conditionIndex != INT_DEFAULT_VALUE)
	{
		#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
		#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, entity1Index, featureIndexOfPreposition, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables->GIAentityNodeArray, translatorVariables->currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, entity2Index, featureIndexOfPreposition, sameReferenceSet);
		#endif
		#endif
	}
	return featureIndexOfPreposition;
}

int GIAsynRelTranslatorLinkEntitiesDynamicClass::getEntityIndex(GIAtranslatorVariablesClass* translatorVariables, const GIAentityNode* instanceEntity)
{
	bool foundEntityIndex = false;
	int instanceEntityIndex = 0;

	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if((*translatorVariables->GIAentityNodeArray)[w] == instanceEntity)
			{
				foundEntityIndex = true;
				instanceEntityIndex = w;
			}
		}
	}
	if(!foundEntityIndex)
	{
		cerr << "getEntityIndex{} error: !foundEntityIndex, entityName = " << instanceEntity->entityName << ", entityIndexTemp = " << instanceEntity->entityIndexTemp << endl;
		exit(EXIT_ERROR);
	}
	return instanceEntityIndex;
}



