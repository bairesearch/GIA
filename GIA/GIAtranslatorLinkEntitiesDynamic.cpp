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
 * File Name: GIAtranslatorLinkEntitiesDynamic.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o2a 12-October-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorLinkEntitiesDynamic.h"
#include "GIAdatabase.h"
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
#include "GIAsemanticParserOperations.h"
#endif


void linkEntitiesDynamic(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B3a; linkEntitiesDynamicPrenominalModifierOfNoun{}:" << endl;
	#endif
	linkEntitiesDynamicPrenominalModifierOfNoun(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences);
	#endif

	#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B3b; linkEntitiesDynamicFromConditions{}:" << endl;
	#endif
	linkEntitiesDynamicFromConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences);
	#endif
}


#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
void linkEntitiesDynamicPrenominalModifierOfNoun(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	//dynamically determine type of linking implied by NN based on existence of previous definition/property/condition links between the NN dependent/governor
	/*
	Hamish smoked at the toy shop.	_nn(shop[6], toy[5])	//direction=true (same with respect to property-child connection)
	A pawn is a chess character.	_nn(character[6], chess[5])
	The chess game is good.		_nn(game[3], chess[2])
	The ball is near the goal line.	_nn(line[7], goal[6])	//direction=false (reverse with respect to property-child connection)
		NB compare ordering with Tom's boat is happy.	poss(boat-3, Tom-1)	//direction=false (reverse with respect to property-child connection)
	*/

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DEBUG
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
		#endif
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool prenominalModifierFound = false;
			#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER])
			{
				prenominalModifierFound = true;
			}
			#endif
			if(textInTextArray(currentRelationInList->relationType, relationTypePrenominalModifierNameArray, RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES))
			{
				prenominalModifierFound = true;
			}
			if(prenominalModifierFound)
			{
				//prenominal modifier found...
				#ifdef GIA_DEBUG
				//cout << "prenominal modifier found" << endl;
				#endif

				string entity1Name = currentRelationInList->relationGovernor;
				string entity2Name = currentRelationInList->relationDependent;
				int entity1Index = currentRelationInList->relationGovernorIndex;
				int entity2Index = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity1 = GIAentityNodeArray[entity1Index];
				GIAentityNode* entity2 = GIAentityNodeArray[entity2Index];

				#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
				int relationTypeIndex = currentRelationInList->relationTypeIndex;
				#else
				int relationTypeIndex = INT_DEFAULT_VALUE;
				#endif

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "entity1 = " << entity1->entityName << endl;
				cout << "entity2 = " << entity2->entityName << endl;
				#endif

				//check if chess (dependent) [primary] is a game (governor), or if goal (dependent) has a line (governor) [primary], or if shop (governor) has toy (dependent), or if line (dependent) is in the goal (dependent)
				bool direction1Found = false;
				bool direction2Found = false;
				direction1Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, entity1, entity2, entity1Index, entity2Index, relationTypeIndex, true);
				if(!direction1Found)
				{
					direction2Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, entity2, entity1, entity2Index, entity1Index, relationTypeIndex, false);
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
					if(entity2->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
					{
						entity1->entityType = GIA_ENTITY_TYPE_TYPE_CONCEPT;	//this may not be used currently because concept prenominal modifiers are interpreted by at least stanford NLP as _amod not _nn (_amod(line[2], goal[1]))
					}
					else
					{
						entity1->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;
					}
					#endif
					*/

					//use default linking (property link)
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "!previousRelationshipFound: creating default property link" << endl;
					#endif
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					bool sameReferenceSet = true;
					#else
					bool sameReferenceSet = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif

					connectPropertyToEntityFull(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entity1, entity2, entity1Index, entity2Index, sameReferenceSet, true);
					//OLD: GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet);
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

bool linkEntitiesDynamicPrenominalModifierOfNounDirection(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, int relationTypeIndex, bool direction)
{
	bool previousRelationshipFound = false;
	bool previousDefinitionRelationshipFound = false;
	bool previousPropertyRelationshipFound = false;
	bool previousConditionRelationshipFound = false;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	bool sameReferenceSet = true;
	#else
	bool sameReferenceSet = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
	#endif

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
								if(((targetEntityFound->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && (entity2->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)) || (!(targetEntityFound->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && !(entity2->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)))
								{
								#endif
									addDefinitionToEntity(entity1, entity2, sameReferenceSet);
									#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, entity1Index, entity2Index, sameReferenceSet);
									#endif
									#endif
								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_CONCEPTS
								}
								else
								{
									addDefinitionToEntity(entity2, entity1, sameReferenceSet);
									#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, entity2Index, entity1Index, sameReferenceSet);
									#endif
									#endif
								}
								#endif
							}
							else if(previousPropertyRelationshipFound)
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "entity1 = " << entity1->entityName << endl;
								cout << "entity2 = " << entity2->entityName << endl;
								cout << "entity1->grammaticalDefiniteTemp = " << entity1->grammaticalDefiniteTemp << endl;
								cout << "entity2->grammaticalDefiniteTemp = " << entity2->grammaticalDefiniteTemp << endl;
								#endif

								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_DEFINITE
								if(entity2->grammaticalDefiniteTemp)
								{
									entity1->grammaticalDefiniteTemp = true;
									entity2->grammaticalDefiniteTemp = false;
								}
								#endif
								#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_CONCEPT_IF_NECESSARY
								if(entity2->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
								{
									entity1->entityType = GIA_ENTITY_TYPE_TYPE_CONCEPT;	//this may not be used currently because concept prenominal modifiers are interpreted by at least stanford NLP as _amod not _nn (_amod(line[2], goal[1]))
								}
								else
								{
									entity1->entityType = GIA_ENTITY_TYPE_TYPE_SUBSTANCE;
								}
								#endif

								connectPropertyToEntityFull(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entity1, entity2, entity1Index, entity2Index, sameReferenceSet, direction);

							}
							else if(previousConditionRelationshipFound)
							{
								int featureIndexOfPreposition = connectConditionToEntityFull(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entity1, entity2, entity1Index, entity2Index, targetEntityFound->entityName, relationTypeIndex, sameReferenceSet);

								#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
								if(currentRelationInList->relationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single GIArelation
								{
									#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
									cout << "currentRelationInList->relationTwoWay detected" << endl;
									#endif
									GIAentityNodeArray[featureIndexOfPreposition]->conditionTwoWay = true;	//sets conditionTwoWay for condition substance not networkIndex
								}
								#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
								if(currentRelationInList->inverseRelationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single GIArelation
								{
									#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
									cout << "currentRelationInList->inverseRelationTwoWay detected" << endl;
									#endif
									GIAentityNodeArray[featureIndexOfPreposition]->inverseConditionTwoWay = true;	//sets inverseConditionTwoWay for condition substance not networkIndex
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

#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS
//preconditions: requires redistributeStanfordRelationsConnectToAndFromConditions{} and redistributeStanfordRelationsConnectPrepositionsToActionRatherThanActionObject{} to have been executed
void linkEntitiesDynamicFromConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	/*
	Move the chicken from the pie to the apple.
	Move the chicken from the red pie to the apple.
	Move the red chicken from the pie to the apple.
	*/

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	bool sameReferenceSet = true;
	#else
	bool sameReferenceSet = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
	#endif

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(textInTextArray(GIAentityNodeArray[w]->entityName, relationTypePrepositionFromNameArray, RELATION_TYPE_PREPOSITION_FROM_NUMBER_OF_TYPES))
			{
				#ifdef GIA_DEBUG
				//cout << "fromCondition" << endl;
				#endif
				GIAentityNode* fromCondition = GIAentityNodeArray[w];

				if(!(fromCondition->conditionObjectEntity->empty()))
				{
					GIAentityNode* fromConditionObject = (fromCondition->conditionObjectEntity->back())->entity;
					GIAentityNode* sourceLocationEntity = fromConditionObject;

					if(!(fromCondition->conditionSubjectEntity->empty()))
					{
						bool foundFromConditionParent = false;
						GIAentityNode* fromConditionParent = NULL;
						GIAentityNode* fromConditionSubject = (fromCondition->conditionSubjectEntity->back())->entity;

						//now see if from condition subject is an action (if so, perform the relinking to the action object instead of the from condition subject)
						if(fromConditionSubject->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
						{
							#ifndef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES
							if(textInTextArray(fromConditionSubject->entityName, dynamicallyLinkFromConditionsActionTypesArray, GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ACTION_TYPES_NUMBER_OF_TYPES))
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
								#ifdef GIA_DEBUG
								//cout << "fromConditionSubject->isAction" << endl;
								#endif

								if(!(fromConditionSubject->actionObjectEntity->empty()))
								{
									GIAentityNode* fromConditionSubjectActionObject = (fromConditionSubject->actionObjectEntity->back())->entity;
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
											bool previousDefinitionRelationshipFoundTemp = false;
											bool previousPropertyRelationshipFoundTemp = false;
											bool previousConditionRelationshipFoundTemp = false;
											if(instanceEntity->entityName == sourceLocationEntity->entityName)
											{
												if(findPreviousRelationship(instanceEntity, sourceEntity, &targetEntityFound, &previousDefinitionRelationshipFoundTemp, &previousPropertyRelationshipFoundTemp, &previousConditionRelationshipFoundTemp))
												{
													if(previousPropertyRelationshipFoundTemp)
													{
														#ifdef GIA_DEBUG
														//cout << "previousPropertyRelationshipFound" << endl;
														#endif
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
															#ifdef GIA_DEBUG
															//cout << "previousConditionRelationshipFound" << endl;
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
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "linkEntitiesDynamicFromConditions{}: (previousPropertyRelationshipFound || !previousRelationshipFound)" << endl;
								#endif

								bool direction = false;	//always define from condition artificial property parent relationship as a poss relationship (this is done for NLC compatibility; such that NLC will parse the same reference set child entity)
								int sourceLocationEntityIndex = getEntityIndex(GIAentityNodeArrayFilled, GIAentityNodeArray, sourceLocationEntity);
								int sourceEntityIndex = getEntityIndex(GIAentityNodeArrayFilled, GIAentityNodeArray, sourceEntity);
								connectPropertyToEntityFull(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, sourceLocationEntity, sourceEntity, sourceLocationEntityIndex, sourceEntityIndex, sameReferenceSet, direction);
							}
							else if(previousConditionRelationshipFound)
							{
								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "linkEntitiesDynamicFromConditions{}: previousConditionRelationshipFound" << endl;
								#endif

								int sourceLocationEntityIndex = getEntityIndex(GIAentityNodeArrayFilled, GIAentityNodeArray, sourceLocationEntity);
								int sourceEntityIndex = getEntityIndex(GIAentityNodeArrayFilled, GIAentityNodeArray, sourceEntity);
								connectConditionToEntityFull(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, sourceEntity, sourceLocationEntity, sourceEntityIndex, sourceLocationEntityIndex, targetEntityFound->entityName, INT_DEFAULT_VALUE, sameReferenceSet);
							}
						}
					}
				}
			}
		}
	}
}

#endif

bool findPreviousRelationship(GIAentityNode* instanceEntity, GIAentityNode* comparisonEntity, GIAentityNode** targetEntityFound, bool* previousDefinitionRelationshipFound, bool* previousPropertyRelationshipFound, bool* previousConditionRelationshipFound)
{
	bool previousRelationshipFound = false;

	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->entityNodeDefinitionList->begin(); connectionIter2 != instanceEntity->entityNodeDefinitionList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* definitionEntity = (*connectionIter2)->entity;
			if(definitionEntity->entityName == comparisonEntity->entityName)
			{
				previousRelationshipFound = true;
				*previousDefinitionRelationshipFound = true;

				*targetEntityFound = definitionEntity;

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "previousDefinitionRelationshipFound" << endl;
				#endif
			}
		}
	}
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->propertyNodeList->begin(); connectionIter2 != instanceEntity->propertyNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* propertyEntity = (*connectionIter2)->entity;
			if(propertyEntity->entityName == comparisonEntity->entityName)
			{
				previousRelationshipFound = true;
				*previousPropertyRelationshipFound = true;

				*targetEntityFound = propertyEntity;	//REDUNDANT

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "previousPropertyRelationshipFound" << endl;
				#endif
			}
		}
	}
	#else
	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->actionNodeList->begin(); connectionIter2 != instanceEntity->actionNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* actionEntity = (*connectionIter2)->entity;
			if(isActionSpecialPossessive(actionEntity))
			{
				GIAentityNode* actionObject = NULL;
				if(!(actionEntity->actionObjectEntity->empty()))
				{
					actionObject = (actionEntity->actionObjectEntity->back())->entity;
					if(actionObject->entityName == comparisonEntity->entityName)
					{
						previousRelationshipFound = true;
						*previousPropertyRelationshipFound = true;

						*targetEntityFound = actionObject;	//REDUNDANT

						#ifdef GIA_TRANSLATOR_DEBUG
						cout << "previousPropertyRelationshipFound" << endl;
						#endif
					}
				}
			}
		}
	}
	#endif
	for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->conditionNodeList->begin(); connectionIter2 != instanceEntity->conditionNodeList->end(); connectionIter2++)
	{
		if(!previousRelationshipFound)
		{
			GIAentityNode* conditionEntity = (*connectionIter2)->entity;
			GIAentityNode* conditionObject = NULL;

			if(!(conditionEntity->conditionObjectEntity->empty()))
			{
				conditionObject = (conditionEntity->conditionObjectEntity->back())->entity;
				if(conditionObject->entityName == comparisonEntity->entityName)
				{
					previousRelationshipFound = true;
					*previousConditionRelationshipFound = true;

					*targetEntityFound = conditionEntity;

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

void connectPropertyToEntityFull(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, bool sameReferenceSet, bool direction)
{
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC

	GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet);
	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, entity1Index, entity2Index, sameReferenceSet);
	#endif
	#endif

	#elif defined GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_HYBRID

	GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet);
	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, entity1Index, entity2Index, sameReferenceSet);
	#endif
	#endif

	#else

	GIAentityNode* actionNetworkIndexEntity;

	int featureIndexOfAction = currentSentenceInList->conditionEntityArtificialIndexCurrent;
	currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

	bool entityAlreadyExistant = false;
	string actionName = "";
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION_GENERAL_IMPLEMENTATION
	if(direction)
	{
	#endif
		string actionName = RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES;	//actionEntity->entityName
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION_GENERAL_IMPLEMENTATION
	}
	else
	{
		string actionName = RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_POSS;	//actionEntity->entityName
	}
	#endif
	actionNetworkIndexEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfAction, &actionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);
	//Alternative (need to fill GIAentityNodeArrayFilled);
	//actionNetworkIndexEntity = (actionEntity->entityNodeDefiningThisInstance->back())->entity;
	#ifdef GIA_DEBUG
	//cout << "actionNetworkIndexEntity = " << actionNetworkIndexEntity->entityName << endl;
	//cout << "featureIndexOfAction = " << featureIndexOfAction << endl;
	#endif
	GIAentityNodeArray[featureIndexOfAction] = addOrConnectActionToEntity(entity1, entity2, actionNetworkIndexEntity, sameReferenceSet);

	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, entity1Index, featureIndexOfAction, sameReferenceSet);
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, entity2Index, featureIndexOfAction, sameReferenceSet);
	#endif
	#endif

	#endif
}

int connectConditionToEntityFull(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, string conditionEntityName, int conditionIndex, bool sameReferenceSet)
{
	GIAentityNode* conditionNetworkIndexEntity;
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
	int featureIndexOfPreposition = conditionIndex;
	if(featureIndexOfPreposition == INT_DEFAULT_VALUE)
	{
		featureIndexOfPreposition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
		currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
	}
	#else
	int featureIndexOfPreposition = INT_DEFAULT_VALUE;
	bool prepositionFeatureFound = determineFeatureIndexOfPreposition(currentSentenceInList, currentRelationInList, &featureIndexOfPreposition);
	if(!prepositionFeatureFound)
	{
		featureIndexOfPreposition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
		currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
	}
	#endif

	bool entityAlreadyExistant = false;
	string conditionName = conditionEntityName;	//conditionEntity->entityName;
	conditionNetworkIndexEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfPreposition, &conditionName, &entityAlreadyExistant, entityNodesActiveListNetworkIndexes);
	//Alternative (need to fill GIAentityNodeArrayFilled);
	//conditionNetworkIndexEntity = (conditionEntity->entityNodeDefiningThisInstance->back())->entity;

	GIAentityNodeArray[featureIndexOfPreposition] = addOrConnectConditionToEntity(entity1, entity2, conditionNetworkIndexEntity, sameReferenceSet);
	#ifndef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, entity1Index, featureIndexOfPreposition, sameReferenceSet);
	GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, entity2Index, featureIndexOfPreposition, sameReferenceSet);
	#endif
	#endif
	return featureIndexOfPreposition;
}

int getEntityIndex(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* instanceEntity)
{
	bool foundEntityIndex = false;
	int instanceEntityIndex = 0;

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(GIAentityNodeArray[w] == instanceEntity)
			{
				foundEntityIndex = true;
				instanceEntityIndex = w;
			}
		}
	}
	if(!foundEntityIndex)
	{
		cout << "getEntityIndex{} error: !foundEntityIndex, entityName = " << instanceEntity->entityName << ", entityIndexTemp = " << instanceEntity->entityIndexTemp << endl;
		exit(0);
	}
	return instanceEntityIndex;
}



