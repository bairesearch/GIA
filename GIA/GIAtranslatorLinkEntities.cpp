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
 * File Name: GIAtranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2i16a 27-January-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorLinkEntities.h"
#include "GIAdatabase.h"



#ifdef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
void linkEntitiesDynamicPrenominalModifierOfNoun(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	//dynamically determine type of linking implied by NN based on existence of previous definition/property/condition links between the NN dependent/governor
	/*
	Hamish smoked at the toy shop.	_nn(shop[6], toy[5])
	A pawn is a chess character.	_nn(character[6], chess[5])
	The chess game is good.		_nn(game[3], chess[2])
	The ball is near the goal line.	_nn(line[7], goal[6])
	*/

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(textInTextArray(currentRelationInList->relationType, relationTypePrenominalModifierNameArray, RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES))
			{
				//prenominal modifier found...
				
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
				bool direction1Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, entityNodesActiveListSentences, entity1, entity2, entity1Index, entity2Index, relationTypeIndex);
				bool direction2Found = false;
				if(!direction1Found)
				{
					direction2Found = linkEntitiesDynamicPrenominalModifierOfNounDirection(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, entityNodesActiveListSentences, entity2, entity1, entity2Index, entity1Index, relationTypeIndex);
				}

				if(!direction1Found && !direction2Found)
				{
					//use default linking (property link)
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "!previousRelationshipFound: creating default property link" << endl;
					#endif
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					bool sameReferenceSet = true;
					bool rcmodIndicatesSameReferenceSet = false;
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					bool rcmodIndicatesSameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet, rcmodIndicatesSameReferenceSet);
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}	

bool linkEntitiesDynamicPrenominalModifierOfNounDirection(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, GIAentityNode* entity1, GIAentityNode* entity2, int entity1Index, int entity2Index, int relationTypeIndex)
{
	bool previousRelationshipFound = false;
	bool previousDefinitionRelationshipFound = false;
	bool previousPropertyRelationshipFound = false;
	bool previousConditionRelationshipFound = false;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	bool sameReferenceSet = true;
	bool rcmodIndicatesSameReferenceSet = false;
	#else
	bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
	bool rcmodIndicatesSameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
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
						for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS].begin(); connectionIter2 != instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS].end(); connectionIter2++)
						{
							if(!previousRelationshipFound)
							{
								GIAentityNode* definitionEntity = (*connectionIter2)->entity;
								if(definitionEntity->entityName == entity2->entityName)
								{
									previousRelationshipFound = true;
									previousDefinitionRelationshipFound = true;

									#ifdef GIA_TRANSLATOR_DEBUG
									cout << "previousDefinitionRelationshipFound" << endl;
									#endif

									#ifdef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_SUBSTANCE_CONCEPTS
									if(((definitionEntity->isSubstanceConcept) && (entity2->isSubstanceConcept)) || (!(definitionEntity->isSubstanceConcept) && !(entity2->isSubstanceConcept)))
									{
									#endif
										addDefinitionToEntity(entity1, entity2, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
										GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, entity1Index, entity2Index, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#endif
									#ifdef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_SUBSTANCE_CONCEPTS
									}
									else
									{
										addDefinitionToEntity(entity2, entity1, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
										GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, entity2Index, entity1Index, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#endif
									}
									#endif
								}
							}
						}
						#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
						for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES].begin(); connectionIter2 != instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES].end(); connectionIter2++)
						{
							if(!previousRelationshipFound)
							{
								GIAentityNode* propertyEntity = (*connectionIter2)->entity;
								if(propertyEntity->entityName == entity2->entityName)
								{
									previousRelationshipFound = true;
									previousPropertyRelationshipFound = true;

									#ifdef GIA_TRANSLATOR_DEBUG
									cout << "previousPropertyRelationshipFound" << endl;
									#endif
									GIAentityNodeArray[entity2Index] = addOrConnectPropertyToEntity(entity1, entity2, sameReferenceSet, rcmodIndicatesSameReferenceSet);
									#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
									GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, entity1Index, entity2Index, sameReferenceSet, rcmodIndicatesSameReferenceSet);
									#endif
								}
							}
						}
						#else
						for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS].begin(); connectionIter2 != instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS].end(); connectionIter2++)
						{
							if(!previousRelationshipFound)
							{
								GIAentityNode* actionEntity = (*connectionIter2)->entity;
								if(actionEntity->entityName == RELATION_ENTITY_SPECIAL_POSSESSIVE)
								{
									GIAentityNode* actionObject = NULL;
									if(!(actionEntity->actionObjectEntity->empty()))
									{
										actionObject = (actionEntity->actionObjectEntity->back())->entity;
										if(actionObject->entityName == entity2->entityName)
										{
											previousRelationshipFound = true;
											previousPropertyRelationshipFound = true;

											#ifdef GIA_TRANSLATOR_DEBUG
											cout << "previousPropertyRelationshipFound" << endl;
											#endif

											GIAentityNode* actionConceptEntity;

											int featureIndexOfAction = currentSentenceInList->conditionEntityArtificialIndexCurrent;
											currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

											bool entityAlreadyExistant = false;
											string actionName = actionEntity->entityName;
											actionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfAction, &actionName, &entityAlreadyExistant, entityNodesActiveListConcepts);
											//Alternative (need to fill GIAentityNodeArrayFilled);
											//actionConceptEntity = (actionEntity->entityNodeDefiningThisInstance->back())->entity;
											//cout << "actionConceptEntity = " << actionConceptEntity->entityName << endl;

											GIAentityNodeArray[featureIndexOfAction] = addOrConnectActionToEntity(entity1, entity2, actionConceptEntity, sameReferenceSet, rcmodIndicatesSameReferenceSet);
											#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
											GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, entity1Index, featureIndexOfAction, sameReferenceSet, rcmodIndicatesSameReferenceSet);
											GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, entity2Index, featureIndexOfAction, sameReferenceSet, rcmodIndicatesSameReferenceSet);
											#endif
										}
									}
								}
							}
						}
						#endif
						for(vector<GIAentityConnection*>::iterator connectionIter2 = instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS].begin(); connectionIter2 != instanceEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS].end(); connectionIter2++)
						{
							if(!previousRelationshipFound)
							{
								GIAentityNode* conditionEntity = (*connectionIter2)->entity;
								GIAentityNode* conditionObject = NULL;

								if(!(conditionEntity->conditionObjectEntity->empty()))
								{
									conditionObject = (conditionEntity->conditionObjectEntity->back())->entity;
									if(conditionObject->entityName == entity2->entityName)
									{
										previousRelationshipFound = true;
										previousConditionRelationshipFound = true;

										#ifdef GIA_TRANSLATOR_DEBUG
										cout << "previousConditionRelationshipFound" << endl;
										#endif

										GIAentityNode* conditionConceptEntity;
										#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
										int featureIndexOfPreposition = relationTypeIndex;
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
										string conditionName = conditionEntity->entityName;
										conditionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfPreposition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);
										//Alternative (need to fill GIAentityNodeArrayFilled);
										//conditionConceptEntity = (conditionEntity->entityNodeDefiningThisInstance->back())->entity;

										GIAentityNodeArray[featureIndexOfPreposition] = addOrConnectConditionToEntity(entity1, entity2, conditionConceptEntity, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
										GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, entity1Index, featureIndexOfPreposition, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, entity2Index, featureIndexOfPreposition, sameReferenceSet, rcmodIndicatesSameReferenceSet);
										#endif
										#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
										if(currentRelationInList->relationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single GIArelation
										{	
											#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
											cout << "currentRelationInList->relationTwoWay detected" << endl;
											#endif
											GIAentityNodeArray[featureIndexOfPreposition]->conditionTwoWay = true;	//sets conditionTwoWay for condition substance not concept 
										}
										#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
										if(currentRelationInList->inverseRelationTwoWay)	//limitation only works when GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity is called based on a single GIArelation
										{	
											#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
											cout << "currentRelationInList->inverseRelationTwoWay detected" << endl;
											#endif
											GIAentityNodeArray[featureIndexOfPreposition]->inverseConditionTwoWay = true;	//sets inverseConditionTwoWay for condition substance not concept 
										}
										#endif
										#endif
									}
								}
								else
								{
									cout << "linkEntitiesDynamicPrenominalModifierOfNounDirection() error: condition entity has no condition object entity" << endl;
								}
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

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void linkEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool linkPreestablishedReferencesGIA)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2a pass; link properties (possessive relationships); eg Joe's bike is blue. _poss(bike[3], Joe[1]) / Hamish smoked at the toy shop. _nn(shop[6], toy[5])" << endl;
	#endif
	linkPropertiesPossessiveRelationships(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2b pass; link properties (descriptive relationships); eg Joe is happy.	_predadj(Joe[1], happy[3]) [NB Stanford nsubj(happy-3, Joe-1) + cop(happy-3, is-2) gets redistributed to _predadj(Joe[1], happy[3])]" << endl;
	#endif
	linkPropertiesDescriptiveRelationships(currentSentenceInList, GIAentityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2c pass; link entity definitions (appositive of nouns only); eg The fish, a carp, swam deeply. _appo(fish[2], carp[5])" << endl;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAentityNodeArray, linkPreestablishedReferencesGIA);
	#else
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE1
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//stanford only
		#ifdef GIA_TRANSLATOR_DEBUG
 		cout <<"2d pass; link conditions (dependent actions type 1), eg To swim to the beach requires strength. csubj(requires-6, swim-2) + dobj(requires-6, strength-7) " << endl;
		#endif
		linkDependentActionsType1(currentSentenceInList, GIAentityNodeArray);
	}
	#endif

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2e pass; link Having Substance Conditions And Being Definition Conditions" << endl;
	cout << "eg1; Space is saved through/by having a chicken. prepc/prep_through/by(saved-3, having-5) + dobj(having-5, chicken-7)" << endl;
	cout << "eg2; Space is saved through/by being a chicken. prep_through/by(saved-3, be-5) + dobj(be-5, chicken-7) 	[Relex Only - note Relex currently fails to parse 'through having' but can parse 'by having']" << endl;
	cout << "eg3; Space is saved through/by being a chicken. prepc_through/by(saved-3, chicken-7) + cop(chicken-7, being-5) 	[Stanford Only]" << endl;
	#endif
	linkHavingPropertyConditionsAndBeingDefinitionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	//26 July 2013 - shifted linkIndirectObjects before linkSubjectObjectRelationships for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK (as obj is disableRelationDuringLink by linkSubjectObjectRelationships) - note defineToBeAndToDoConditions is also reliant on at least a subj being active [but not a subj and obj combination like linkIndirectObjects]: check this does not require shifting either
	cout << "3a pass; link properties (define indirect objects); eg The officer gave the youth a ride. _iobj(give, youth) +  _obj(give[3], ride[7])" << endl;
	#endif
	linkIndirectObjects(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3b pass; link dependent subject-object definition/composition/action relationships;" << endl;
	cout << "eg1; The rabbit is 20 meters away. _subj(away[6], rabbit[2]) + _measure_distance(away[6], meter[5])" << endl;
	cout << "eg2; A bat is an animal. _subj(be[3], bat[2]) + _obj(be[3], animal[5])" << endl;
	cout << "OLD: eg3; The house has a table. _subj(have[3], house[2]) + _obj(have[3], table[5])" << endl;
	cout << "eg4; Tom rides the bike. _subj(ride[2], Tom[1]) + _obj(ride[2], bike[4])" << endl;
	#endif
	linkSubjectObjectRelationships(currentSentenceInList, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3c pass; link independent subject/object action relationships; eg Tom runs quickly. _subj(run[2], Tom[1]) / The bike was ridden. _obj(ride[4], bike[2])" << endl;
	#endif
	linkSubjectOrObjectRelationships(currentSentenceInList, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP	//why GIA_USE_STANFORD_CORENLP and not GIA_USE_STANFORD_DEPENDENCY_RELATIONS?
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "3dOLD pass; link Having Substance Conditions And Being Definition Conditions" << endl;
		#endif
		linkHavingPropertyConditionsAndBeingDefinitionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);
		#endif
	}
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3d pass; link object/subject of preposition; eg The garage is next to the house. _pobj(next_to, house)  + _psubj(next_to, garage) [appears to be Relex only]" << endl;
	#endif
	linkObjectSubjectOfPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3e pass; link conjunction conditions; eg Tom and/or Max eat the cake. conj_and(Tom[1], Max[3]) / conj_or(Tom[2], Max[4])" << endl;
	#endif
	linkConjunctionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3f pass; link conditions; eg Joe is sad at the park. at(sad[3], park[6])" << endl;
	#endif
	linkConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE2
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//stanford only
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "3g pass; link conditions (dependent actions type 2); eg To copy the files[, ]create a backup of the old file.	dep(create-6, copy-2)" << endl;
		#endif
		linkDependentActionsType2(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	}
	#endif
}

void linkPropertiesPossessiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	Joe's bike is blue.	_poss(bike[3], Joe[1])
	*/
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramA.numberOfRelations = 1;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypePossessiveNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty;
	paramA.functionEntityRelationID[FUNC_ENT1] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	paramA.functionEntityRelationID[FUNC_ENT2] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;
	#endif
	genericDependecyRelationInterpretation(&paramA, REL1);
	#else
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramA.numberOfRelations = 1;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypePossessiveNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES;
	paramA.mustGenerateConditionName = true; paramA.conditionEntityDefaultName = RELATION_ENTITY_SPECIAL_POSSESSIVE;	//create intermediary action 'have'
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity;
	paramA.functionEntityRelationID[FUNC_ENT1] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	paramA.functionEntityRelationID[FUNC_ENT2] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;
	#endif
	genericDependecyRelationInterpretation(&paramA, REL1);
	#endif

#ifndef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
	/*
	Hamish smoked at the toy shop.	_nn(shop[6], toy[5])
	*/
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramB.numberOfRelations = 1;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypePrenominalModifierNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	paramB.functionEntityRelationID[FUNC_ENT2] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;
	#endif
	genericDependecyRelationInterpretation(&paramB, REL1);
	#endif
#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypePossessiveNameArray[i])
				{
					passed = true;
				}
			}
			//if(currentRelationInList->relationType == RELATION_TYPE_POSSESSIVE)
			if(passed)
			{
				string substanceName = currentRelationInList->relationGovernor;
				string ownerName = currentRelationInList->relationDependent;
				int substanceIndex = currentRelationInList->relationGovernorIndex;
				int ownerIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];
				GIAentityNode* ownerEntity = GIAentityNodeArray[ownerIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "RELATION_TYPE_POSSESSIVE" << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;
				#endif

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(ownerEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}

	#ifndef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR
	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypePrenominalModifierNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				//create a new substance for the entity and assign a substance definition entity if not already created
				string thingName = currentRelationInList->relationGovernor;
				string substanceName = currentRelationInList->relationDependent;
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode* thingEntity = GIAentityNodeArray[thingIndex];
				GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "thingEntity = " << thingEntity->entityName << endl;
				cout << "substanceEntity = " << substanceEntity->entityName << endl;
				#endif

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(thingEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
	#endif
	#endif
#endif
}


void linkPropertiesDescriptiveRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], int NLPdependencyRelationsType)
{
	/*
	Joe is happy.	_predadj(Joe[1], happy[3])	[NB Stanford nsubj(happy-3, Joe-1) + cop(happy-3, is-2) gets redistributed to _predadj(Joe[1], happy[3])]
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
		GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
		param.numberOfRelations = 1;
		param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES;
		param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
		param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
		param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;
		#endif
		genericDependecyRelationInterpretation(&param, REL1);
	#else

		GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
		param.numberOfRelations = 1;
		param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveWhichImplySameReferenceSetNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_SAME_REFERENCE_SET_NUMBER_OF_TYPES;
		param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
		param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
		param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES;
		#endif
		genericDependecyRelationInterpretation(&param, REL1);

		GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, NULL, GIAentityNodeArray, true);
		paramB.numberOfRelations = 1;
		paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveWhichImplyDifferentReferenceSetNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_DIFFERENT_REFERENCE_SET_NUMBER_OF_TYPES;
		paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
		paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
		paramB.functionEntityRelationID[FUNC_ENT2] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		paramB.defaultSameSetRelationID = REL1; paramB.defaultSameSetReferenceValue = false;
		#endif
		genericDependecyRelationInterpretation(&paramB, REL1);
	#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
				{
					passed = true;
				}
			}
			//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_AMOD) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_PREDADJ) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD))
			if(passed)
			{
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				bool passed2 = true;
				bool passed3 = true;

				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
				{
				#endif
					passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
					passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAentityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				}
				#endif

				if(passed2 && passed3)
				{
				#endif
					//create a new substance for the entity and assign a substance definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string substanceName = currentRelationInList->relationDependent;
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;
					GIAentityNode* thingEntity = GIAentityNodeArray[thingIndex];
					GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];

					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "thingEntity = " << thingEntity->entityName << endl;
					cout << "substanceEntity = " << substanceEntity->entityName << endl;
					#endif

					/*
					bool sameReferenceSet = false;
					for(int i=0; i<RELATION_TYPE_ADJECTIVE_IMPLIES_SAME_SET_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationType == relationTypeAdjectiveImpliesSameSetNameArray[i])
						{
							sameReferenceSet = true;
						}
					}
					*/

					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(thingEntity, substanceEntity, sameReferenceSet);
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}


#ifdef GIA_USE_ADVANCED_REFERENCING
void linkEntityDefinitionsAppositiveOfNouns(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], bool linkPreestablishedReferencesGIA)
#else
void linkEntityDefinitionsAppositiveOfNouns(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[])
#endif
{
	/*
	The fish, a carp, swam deeply.	_appo(fish[2], carp[5])
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
	if(linkPreestablishedReferencesGIA)
	{
	#endif
		GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, true);
		paramA.numberOfRelations = 1;
		paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		GIAentityCharacteristic entityCharacteristicsTest1("isSubstance", "true");
		GIAentityCharacteristic entityCharacteristicsTest2("isNameQuery", "true");
		paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1);
		paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest2);
		paramA.specialCaseCharacteristicsTestOr2Vector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1);
		paramA.specialCaseCharacteristicsTestOr2Vector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest2);
		#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
		GIAentityCharacteristic entityCharacteristicsTest3("isSubstanceConcept", "false");
		paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest3);
		paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest3);
		#endif
		GIAentityCharacteristic entityCharacteristicsTest4("hasAssociatedTime", "false");
		paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest4);
		GIAentityCharacteristic entityCharacteristicsTest5("grammaticalProperNounTemp", "true");
		GIAentityCharacteristic entityCharacteristicsTest6("isNameQuery", "true");
		paramA.specialCaseCharacteristicsTestOr3Vector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest5);
		paramA.specialCaseCharacteristicsTestOr3Vector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest6);
		paramA.disableRelationDuringLink[REL1] = true;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = false;
		#endif
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias;				
		#else
		paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias;
		#endif		
		paramA.functionEntityRelationID[FUNC_ENT1] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
		paramA.functionEntityRelationID[FUNC_ENT2] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
		genericDependecyRelationInterpretation(&paramA, REL1);

		GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, NULL, GIAentityNodeArray, true);
		paramB.numberOfRelations = 1;
		paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		//GIAentityCharacteristic entityCharacteristicsTest1("isSubstance", "true");
		//GIAentityCharacteristic entityCharacteristicsTest2("isNameQuery", "true");
		paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1);
		paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest2);
		paramB.specialCaseCharacteristicsTestOr2Vector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1);
		paramB.specialCaseCharacteristicsTestOr2Vector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest2);
		#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
		//GIAentityCharacteristic entityCharacteristicsTest3("isSubstanceConcept", "false");
		paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest3);
		paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest3);
		#endif
		//GIAentityCharacteristic entityCharacteristicsTest4("hasAssociatedTime", "false");
		paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest4);
		paramB.disableRelationDuringLink[REL1] = true;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		paramB.defaultSameSetRelationID = REL1; paramB.defaultSameSetReferenceValue = false;
		#endif
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntityMarkConnectionAsAlias;			
		#else
		paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_mergeEntityNodesAddAlias;
		#endif	
		paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
		paramB.functionEntityRelationID[FUNC_ENT2] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
		genericDependecyRelationInterpretation(&paramB, REL1);
	#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
	}
	#endif

	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramC.numberOfRelations = 1;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramC.defaultSameSetRelationID = REL1; paramC.defaultSameSetReferenceValue = false;
	#endif
	paramC.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntity;
	paramC.functionEntityRelationID[FUNC_ENT1] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	paramC.functionEntityRelationID[FUNC_ENT2] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	genericDependecyRelationInterpretation(&paramC, REL1);

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))		//?this condition may be required to prevent redundant RELATION_TYPE_APPOSITIVE_OF_NOUN relations eg in 'Where is the ball?'
		{
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				string thingName = currentRelationInList->relationGovernor;
				string definitionName = currentRelationInList->relationDependent;
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int definitionIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode* thingEntity = GIAentityNodeArray[thingIndex];
				GIAentityNode* definitionEntity = GIAentityNodeArray[definitionIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "linkEntityDefinitionsAppositiveOfNouns:" << endl;
				cout << "thingName = " << thingEntity->entityName << endl;
				cout << "definitionEntity = " << definitionEntity->entityName << endl;
				cout << "thingEntity->isSubstance = " << thingEntity->isSubstance << endl;
				cout << "definitionEntity->isSubstance = " << definitionEntity->isSubstance << endl;
				#endif

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_DEFINITIONS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif

				#ifdef GIA_SUPPORT_ALIASES
				bool treatDefinitionAsEquality = false;
				bool treatDefinitionAsEqualityReversePrimary = false;
				#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
				#ifdef GIA_USE_ADVANCED_REFERENCING
				if(linkPreestablishedReferencesGIA)
				{
				#endif
				#endif
					//cout << "thingEntity->isNameQuery = " << thingEntity->isNameQuery << endl;
					//cout << "definitionEntity->isNameQuery = " << definitionEntity->isNameQuery << endl;
					//cout << "thingEntity->isSubstance = " << thingEntity->isSubstance << endl;
					//cout << "definitionEntity->isSubstance = " << definitionEntity->isSubstance << endl;
					if((thingEntity->isSubstance || thingEntity->isNameQuery) && (definitionEntity->isSubstance || definitionEntity->isNameQuery))
					{//equality link found - add alias instead

						//cout << "thingEntity->isSubstanceConcept = " << thingEntity->isSubstance << endl;
						//cout << "definitionEntity->isSubstanceConcept = " << definitionEntity->isSubstance << endl;
						#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
						if(!(thingEntity->isSubstanceConcept) && (!(definitionEntity->isSubstanceConcept))) 	//only required with GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PROPERNOUNS: || (definitionEntity->isName)))
						{
						#endif
							#ifdef GIA_TRANSLATOR_DEBUG
							cout << "treatDefinitionAsEquality" << endl;
							#endif

							if(!(definitionEntity->hasAssociatedTime))
							{
								treatDefinitionAsEquality = true;
								if(definitionEntity->grammaticalProperNounTemp || definitionEntity->isNameQuery)
								{
									#ifdef GIA_ALIASES_DEBUG
									cout << "linkEntityDefinitionsAppositiveOfNouns1" << endl;
									#endif
									//eg max = the brown dog
								}
								else if(thingEntity->grammaticalProperNounTemp || thingEntity->isNameQuery)
								{
									treatDefinitionAsEqualityReversePrimary = true;
									#ifdef GIA_ALIASES_DEBUG
									cout << "linkEntityDefinitionsAppositiveOfNouns2" << endl;
									#endif
									//eg max = the brown dog
								}
								else
								{
									#ifdef GIA_ALIASES_DEBUG
									cout << "linkEntityDefinitionsAppositiveOfNouns3" << endl;
									#endif
									//if no proper noun (or query) detected, each node is equal, eg the brown dog == the happy wolf]
								}
							}
						#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
						}
						#endif
					}
				#ifdef GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
				#ifdef GIA_USE_ADVANCED_REFERENCING
				}
				#endif
				#endif
				if(treatDefinitionAsEquality)
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "adding equality:" << endl;
					#endif

					if(definitionEntity->idActiveList == thingEntity->idActiveList)
					{
						#ifdef GIA_ALIASES_DEBUG
						cout << "treatDefinitionAsEquality: already merged" << endl;
						#endif
					}
					else
					{
						if(treatDefinitionAsEqualityReversePrimary)
						{
							#ifdef GIA_ALIASES_DEBUG
							cout << "treatDefinitionAsEquality: treatDefinitionAsEqualityReversePrimary" << endl;
							#endif
							//eg max = the brown dog
							mergeEntityNodesAddAlias(definitionEntity, thingEntity);		//less nodes to merge (more efficient)
							GIAentityNodeArray[thingIndex] = GIAentityNodeArray[definitionIndex];
						}
						else
						{
							#ifdef GIA_ALIASES_DEBUG
							cout << "treatDefinitionAsEquality: !treatDefinitionAsEqualityReversePrimary" << endl;
							#endif
							//if no proper noun (or query) detected, each node is equal, eg the brown dog == the happy wolf]
							mergeEntityNodesAddAlias(thingEntity, definitionEntity);
							GIAentityNodeArray[definitionIndex] = GIAentityNodeArray[thingIndex];
						}
					}
				}
				else
				{
				#endif

					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "adding definition:" << endl;
					#endif

					addDefinitionToEntity(thingEntity, definitionEntity, sameReferenceSet);
				#ifdef GIA_SUPPORT_ALIASES
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}


//Stanford Only
void linkDependentActionsType1(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[])
{
	/*
	To swim to the beach requires strength. csubj(requires-6, swim-2) + dobj(requires-6, strength-7)

		interpret as condition;
	To swim to the beach requires strength.
		csubj(requires-6, swim-2)
		dobj(requires-6, strength-7)
	To drive to the beach requires strength.
		csubj(requires-6, drive-2)
		dobj(requires-6, strength-7)
	*/

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CLAUSAL_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL2; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT3] = REL1; param.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS;
	#endif
	param.disableRelationDuringLink[REL2] = true;	//required to prevent action link from being created to object in secondary action interpretation function linkSubjectOrObjectRelationships() - added 17 July 2013
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_CLAUSAL_SUBJECT)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
						{
							if(currentRelationInList->relationGovernorIndex == currentRelationInList2->relationGovernorIndex)
							{//found a matching relationship

								string ownerName = currentRelationInList->relationDependent;
								string substanceName = currentRelationInList2->relationDependent;
								string conditionName = currentRelationInList->relationGovernor;
								int ownerIndex = currentRelationInList->relationDependentIndex;
								int substanceIndex = currentRelationInList2->relationDependentIndex;
								int conditionTypeIndex = currentRelationInList->relationGovernorIndex;

								GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[ownerIndex];
								GIAentityNode* actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[substanceIndex];
								GIAentityNode* conditionConceptEntity = GIAentityNodeArray[conditionTypeIndex];

								#ifdef GIA_TRANSLATOR_DEBUG
								cout << "RELATION_TYPE_DEPENDENT" << endl;
								cout << "actionOrSubstanceConditionSubjectEntity = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
								cout << "actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
								cout << "conditionConceptEntity = " << conditionConceptEntity->entityName << endl;
								#endif

								#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
								bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								currentRelationInList2->relationType = "dummyRelationNamePreventActionLinkFromBeingCreatedToObject";

								#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
								GIAentityNodeArray[conditionTypeIndex] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
								#else
								addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
								#endif
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
void linkHavingPropertyConditionsAndBeingDefinitionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	Stanford;
	Space is saved through/by having a chicken.	prepc_through/by(saved-3, having-5) + dobj(having-5, chicken-7) + nsubjpass(saved-3, Space-1) [IRRELEVANT]	+ auxpass(saved-3, is-2) [IRRELEVANT]
	Space is saved through/by being a chicken.	prepc_through/by(saved-3, chicken-7) + cop(chicken-7, being-5)  + nsubjpass(saved-3, Space-1) [IRRELEVANT] + auxpass(saved-3, is-2) [IRRELEVANT]

	Relex;
	Space is saved through/by having a chicken.	prep_through/by(saved-3, having-5) + dobj(having-5, chicken-7)	[check that through(save[3], chicken[7]) + _obj(save[3], space[1]) is first reduced by redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition]
	Space is saved through/by being a chicken.	prep_through/by(saved-3, be-5)  + dobj(be-5, chicken-7)		[check that  _obj(be[5], chicken[7]) + _to-do(through[4], be[5]) + _predadj(space[1], through[4]) is first reduced by redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition]
	*/

	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
	param.NLPdependencyRelationsType = NLPdependencyRelationsType;
	param.numberOfRelations = 2;
	param.disableRelationDuringLink[REL1] = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function linkConditions()
	param.disableRelationDuringLink[REL2] = true;	//required to prevent re-interpretation of prepositions in secondary action interpretation function linkSubjectOrObjectRelationships() - added 17 July 2013
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = linkHavingPropertyConditionsAndBeingDefinitionConditionsPrepositionsNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_HAVING_AND_BEING_CONDITIONS_PREPOSITIONS_NUMBER_OF_TYPES;
	param.expectToFindPrepositionTest[REL1] = true;	//redundant test (maintained for consistency only with old non-GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK code)
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;	//coincidentially this condition holds for all 3 cases
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;	//coincidentially this condition holds for all 3 cases
	param.functionEntityRelationID[FUNC_ENT2] = REL2; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT3] = REL1; param.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;	//coincidentially this condition holds for all 3 cases

	/*
	Space is saved through/by having a chicken.	prepc/prep_through/by(saved-3, having-5) + dobj(having-5, chicken-7) 	[note Relex currently fails to parse 'through having/being' but can parse 'by having/being']
	*/
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.functionEntityRelationID[FUNC_ENT4_SPECIAL] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT4_SPECIAL] = REL_ENT2;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramA.useRelationTest[REL1][REL_ENT2] = true; paramA.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_HAVE;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectHavingPropertyConditionToEntity;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramA.defaultSameSetRelationID = REL2; paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS;
	#endif
	paramA.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramA, REL1);

	/*
	Space is saved through/by being a chicken.		prep_through/by(saved-3, be-5) + dobj(be-5, chicken-7) [Relex Only - note Relex currently fails to parse 'through having/being' but can parse 'by having/being']
	*/
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.functionEntityRelationID[FUNC_ENT4_SPECIAL] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT4_SPECIAL] = REL_ENT2;
	paramB.useRelationTest[REL2][REL_ENT3] = true; paramB.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRelationTest[REL1][REL_ENT2] = true; paramB.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_BE;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramB.defaultSameSetRelationID = REL2; paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS;
	#endif
	paramB.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramB, REL1);

	/*
	Space is saved through/by being a chicken.		prepc_through/by(saved-3, chicken-7) + cop(chicken-7, being-5) 	[Stanford Only]
	*/
	GIAgenericDepRelInterpretationParameters paramC = param;
	paramC.functionEntityRelationID[FUNC_ENT2] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	paramC.functionEntityRelationID[FUNC_ENT4_SPECIAL] = REL2; paramC.functionEntityRelationEntityID[FUNC_ENT4_SPECIAL] = REL_ENT2;
	paramC.useRelationTest[REL2][REL_ENT3] = true; paramC.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COPULA;
	paramC.parseDisabledRelation[REL2] = true;	//copula will have been disabled, so must explicitly allow it to be parsed
	paramC.useRelationTest[REL2][REL_ENT2] = true; paramC.relationTest[REL2][REL_ENT2] = RELATION_ENTITY_BE;
	paramC.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectBeingDefinitionConditionToEntity;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramC.defaultSameSetRelationID = REL2; paramC.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS;
	#endif
	paramC.disableEntity[REL2][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramC, REL1);
#else

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif

			bool stanfordPrepositionFound = false;
			string prepositionName = convertPrepositionToRelex(&(currentRelationInList->relationType), &stanfordPrepositionFound);

			if(stanfordPrepositionFound)
			{
				bool prepositionNamePassed = false;
				for(int i=0; i<RELATION_TYPE_HAVING_AND_BEING_CONDITIONS_PREPOSITIONS_NUMBER_OF_TYPES; i++)
				{
					if(prepositionName == linkHavingPropertyConditionsAndBeingDefinitionConditionsPrepositionsNameArray[i])
					{
						prepositionNamePassed = true;
					}
				}
				if(prepositionNamePassed)
				{
 					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
							if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
								{//found a matching relationship
									if(currentRelationInList->relationDependent == RELATION_ENTITY_HAVE)
									{
										#ifdef GIA_TRANSLATOR_DEBUG
										//cout << "RELATION_ENTITY_HAVE" << endl;
										//cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
										//cout << currentRelationInList2->relationType << "(" << currentRelationInList2->relationGovernor << ", " << currentRelationInList2->relationDependent << ")" << endl;
										#endif

										currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function linkConditions

										string conditionName = prepositionName;
										bool entityAlreadyExistant = false;

										int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
										currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

										GIAentityNode* conditionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

										GIAentityNode* haveEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
										//cout << "haveEntity->negative = " << haveEntity->negative << endl;
										bool negative = haveEntity->negative;
										GIAentityNode* entityNode = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
										GIAentityNode* conditionSubstanceNode = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

										#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
										bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS, currentRelationInList2);	//linkHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through having a chicken is.... therefore default same set]
										//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS;	//more precisely
										#else
										bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
										#endif

										#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
										GIAentityNodeArray[featureIndexOfCondition] = addOrConnectHavingPropertyConditionToEntity(entityNode, conditionSubstanceNode, conditionConceptEntity, negative, sameReferenceSet);
										#else
										addOrConnectHavingPropertyConditionToEntity(entityNode, conditionSubstanceNode, conditionConceptEntity, negative, sameReferenceSet);
										#endif

										disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(haveEntity);
									}
								}
							}
							#endif
							#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_CONDITION_DEFINITION
							else if(currentRelationInList2->relationType == RELATION_TYPE_COPULA)
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
								{//found a matching relationship
									if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
									{
										#ifdef GIA_TRANSLATOR_DEBUG
										//cout << "RELATION_ENTITY_BE" << endl;
										//cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
										//cout << currentRelationInList2->relationType << "(" << currentRelationInList2->relationGovernor << ", " << currentRelationInList2->relationDependent << ")" << endl;
										#endif

										currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function linkConditions

										string conditionName = prepositionName;
										bool entityAlreadyExistant = false;

										int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
										currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

										GIAentityNode* conditionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

										GIAentityNode* beEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
										bool negative = beEntity->negative;
										GIAentityNode* entityNode = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
										GIAentityNode* conditionDefinitionNode = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

										#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
										bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS, currentRelationInList2);	//linkHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through being a chicken is.... therefore default same set]
										//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS;	//more precisely
										#else
										bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
										#endif

										#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
										GIAentityNodeArray[featureIndexOfCondition] = addOrConnectBeingDefinitionConditionToEntity(entityNode, conditionDefinitionNode, conditionConceptEntity, negative, sameReferenceSet);
										#else
										addOrConnectBeingDefinitionConditionToEntity(entityNode, conditionDefinitionNode, conditionConceptEntity, negative, sameReferenceSet);
										#endif

										disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(beEntity);

									}
								}
							}
							#endif

						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}
#endif

void linkIndirectObjects(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	eg The officer gave the youth a ride. _iobj(give, youth) +  _obj(give[3], ride[7])
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_INDIRECT_OBJECT;
	param.useRelationArrayTest[REL2][REL_ENT3] = true; param.relationArrayTest[REL2][REL_ENT3] = relationTypeObjectNameArray; param.relationArrayTestSize[REL2][REL_ENT3] = RELATION_TYPE_OBJECT_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;

	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL2; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE;	//old before 2h2c
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_INDIRECT_OBJECTS;
	param.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	param.useRelationIndexTest[REL3][REL_ENT2] = true; param.relationIndexTestRelationID[REL3][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
	param.useRelationIndexTest[REL3][REL_ENT2] = true; param.relationIndexTestRelationID[REL3][REL_ENT2] = REL2; param.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
	#endif
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{
				//now find the associated object..
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool partnerTypeRequiredFound = false;
						for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
							{
								partnerTypeRequiredFound = true;
							}
						}
						if(partnerTypeRequiredFound)
						{

							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-indirectobject relationship
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								#endif

								int substanceIndex = currentRelationInList2->relationDependentIndex;
								int thingIndex = currentRelationInList->relationDependentIndex;
								GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];
								GIAentityNode* thingEntity = GIAentityNodeArray[thingIndex];

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//eg the linebacker that gave the quarterback a push is blue. / the linebacker gave the quarterback a push
								GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(thingEntity, substanceEntity, sameReferenceSet);
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void linkSubjectObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK

	/*special case A: partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound
	eg The rabbit is 20 meters away.	_subj(away[6], rabbit[2]) + _measure_distance(away[6], meter[5])	[away is a substance of rabbit, not a condition of rabbit]
	_subj(away[6], rabbit[2])
	_measure_distance(away[6], meter[5])
	*/
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramA.numberOfRelations = 2;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramA.useRelationArrayTest[REL2][REL_ENT3] = true; paramA.relationArrayTest[REL2][REL_ENT3] = relationTypeObjectSpecialConditionMeasureDistanceOrStanfordUnknownNameArray; paramA.relationArrayTestSize[REL2][REL_ENT3] = RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES;
	paramA.useRelationIndexTest[REL1][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; paramA.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	paramA.functionEntityRelationID[FUNC_ENT1] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	paramA.functionEntityRelationID[FUNC_ENT2] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramA.defaultSameSetRelationID = REL1; paramA.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS;
	#endif
	paramA.disableRelationDuringLink[REL1] = true;	//required to prevent action link from being created to object in secondary action interpretation function linkSubjectOrObjectRelationships() - added 17 July 2013
	genericDependecyRelationInterpretation(&paramA, REL1);
	#else
	cout << "GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK" << endl;
	#endif

	/*
	//compilation checks:
	#ifdef GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE
	cout << "GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK" << endl;
	#endif
	#ifdef GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE
	cout << "!GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY (and !GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY) not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK" << endl;
	#endif
	*/

	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 2;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeSubjectNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES;
	param.useRelationArrayTest[REL2][REL_ENT3] = true; param.relationArrayTest[REL2][REL_ENT3] = relationTypeObjectNameArray; param.relationArrayTestSize[REL2][REL_ENT3] = RELATION_TYPE_OBJECT_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.disableRelationDuringLink[REL1] = true;	//required to prevent action link from being created to object in secondary action interpretation function linkSubjectOrObjectRelationships() - added 17 July 2013
	param.disableRelationDuringLink[REL2] = true;	//required to prevent action link from being created to object in secondary action interpretation function linkSubjectOrObjectRelationships() - added 17 July 2013
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL2; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_DEFINITION_BASIC
	/*special case B: passdefinition [Relex only? Note Stanford produces different relations that get interpreted as appo]
	eg A bat is an animal.	_subj(be[3], bat[2]) + _obj(be[3], animal[5])
	_obj(be[3], animal[5])
	_subj(be[3], bat[2])
	*/
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationArrayTest[REL1][REL_ENT1] = true; paramB.relationArrayTest[REL1][REL_ENT1] = relationGovernorDefinitionNameArray; paramB.relationArrayTestSize[REL1][REL_ENT1] = RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addDefinitionToEntity;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramB.defaultSameSetReferenceValue = false;
	#endif
	paramB.disableEntity[REL1][REL_ENT1] = true;	//disable "be" entity
	genericDependecyRelationInterpretation(&paramB, REL1);
	#endif

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	/*special case C: passcomposition [Relex and Stanford]
	eg1 Tom comprises happiness.	_subj(comprise[2], Tom[1]) + _obj(comprise[2], happiness[3])
	_obj(comprise[2], happiness[3])
	_subj(comprise[2], Tom[1])
	eg2 The house has a table.	_subj(have[3], house[2]) + _obj(have[3], table[5])
	_obj(have[3], table[5])
	_subj(have[3], house[2])
	*/
	GIAgenericDepRelInterpretationParameters paramC = param;
	paramC.useRelationArrayTest[REL1][REL_ENT1] = true; paramC.relationArrayTest[REL1][REL_ENT1] = relationGovernorCompositionNameArray; paramC.relationArrayTestSize[REL1][REL_ENT1] = RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES;
	paramC.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramC.defaultSameSetReferenceValue = false;
	#endif
	#ifndef GIA_USE_CORPUS_DATABASE
	paramC.disableEntity[REL1][REL_ENT1] = true;	//disable "have" entity
	#endif
	genericDependecyRelationInterpretation(&paramC, REL1);
	#ifdef GIA_USE_CORPUS_DATABASE
	GIAgenericDepRelInterpretationParameters paramC2 = paramC;
	paramC2.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addAuxiliaryToEntity;
	paramC2.disableEntity[REL1][REL_ENT1] = true;	//disable "have" entity
	paramC2.functionEntityRelationID[FUNC_ENT1] = REL2; paramC2.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	paramC2.functionEntityRelationID[FUNC_ENT2] = REL2; paramC2.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	genericDependecyRelationInterpretation(&paramC2, REL1);
	#endif
	#endif

	/*standard case D:
	eg1 Tom rides the bike.	_subj(ride[2], Tom[1]) + _obj(ride[2], bike[4])
	_obj(ride[2], bike[4])
	_subj(ride[2], Tom[1])
	*/
	GIAgenericDepRelInterpretationParameters paramD = param;
	paramD.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToEntity;
	paramD.functionEntityRelationID[FUNC_ENT3] = REL1; paramD.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramD.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS;	//defaultSameSetReferenceValues are handled specially by genericDependecyRelationInterpretation(); defaultSameSetRelationID not required to be set for subjectObject actions
	#endif
	genericDependecyRelationInterpretation(&paramD, REL1);


#else
 	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;

	#ifdef USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD
	bool subjectObjectRelationshipAlreadyAdded[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE;i++)
	{
		subjectObjectRelationshipAlreadyAdded[i] = false;
	}
	#endif

	string subjectObjectName[2];
	GIAentityNode* subjectObjectEntityArray[2];
	GIAentityNode* subjectObjectFunctionEntityArray[2];
	int subjectObjectEntityIndexArray[2];
	int subjectObjectFunctionEntityIndexArray[2];

	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			subjectObjectName[SUBJECT_INDEX] = currentRelationInList->relationDependent;
			subjectObjectEntityArray[SUBJECT_INDEX] = GIAentityNodeArray[relationDependentIndex];
			subjectObjectFunctionEntityArray[SUBJECT_INDEX] = GIAentityNodeArray[relationGovernorIndex];
			subjectObjectEntityIndexArray[SUBJECT_INDEX] = relationDependentIndex;
			subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX] = relationGovernorIndex;

			bool passed = false;
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}
			if(currentRelationInList->disabled)
			{//required for relations disabled for negative collapse purposes
				passed = false;
			}

			bool passcomposition = false;
			for(int i=0; i<RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == relationGovernorCompositionNameArray[i])
				{
					passcomposition = true;
				}
			}

			bool passdefinition = false;
			for(int i=0; i<RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == relationGovernorDefinitionNameArray[i])
				{
					passdefinition = true;
				}
			}

			if(passed)
			{
				bool foundPartner = false;

				//now find the associated object..
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
					if(!(currentRelationInList2->disabled))
					{
					#endif
						int relationGovernorIndex2 = currentRelationInList2->relationGovernorIndex;
						int relationDependentIndex2 = currentRelationInList2->relationDependentIndex;
						subjectObjectName[OBJECT_INDEX] = currentRelationInList2->relationDependent;
						subjectObjectEntityArray[OBJECT_INDEX] = GIAentityNodeArray[relationDependentIndex2];
						subjectObjectFunctionEntityArray[OBJECT_INDEX] = GIAentityNodeArray[relationGovernorIndex2];
						subjectObjectEntityIndexArray[OBJECT_INDEX] = relationDependentIndex2;
						subjectObjectFunctionEntityIndexArray[OBJECT_INDEX] = relationGovernorIndex2;

						bool passed2 = false;
						bool partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound = false;
						bool partnerTypeObjectSpecialConditionToDoSubstanceFound = false;
						bool partnerTypeObjectSpecialConditionToDoSubstanceFoundAssumeUsed = true;
						bool partnerTypeObjectSpecialConditionToBeSubstanceFound = false;

						for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
							{
								passed2 = true;
							}
						}
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionMeasureDistanceOrStanfordUnknownNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound = true;
									#ifdef GIA_TRANSLATOR_DEBUG
									/*
									cout << "partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound" << endl;
									cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
									cout << "currentRelationInList2->relationDependent = " << currentRelationInList2->relationDependent << endl;
									cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
									cout << "currentRelationInList2->relationGovernor = " << currentRelationInList2->relationGovernor << endl;
									*/
									#endif
								}
							}
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
						}
						#endif

					#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
						/*
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
						*/
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_WITH_BE_AS_DEFINITION_LINK
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_TO_DO_SUBSTANCE_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionToDoSubstanceNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionToDoSubstanceFound = true;
								}
							}
							#endif
						/*
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
						}
						#endif
						*/

					#ifdef GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK_AND_ACTION_DEFINITION
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_TO_BE_SUBSTANCE_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionToBeSubstanceNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionToBeSubstanceFound = true;
								}
							}
							#endif
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						}
						#endif
					#endif
					#endif

						if(currentRelationInList2->disabled)
						{//required for relations disabled for negative collapse purposes
							passed2 = false;
						}

						if(passed2)
						{
							bool foundPartner = false;
							#ifdef USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD
							if(subjectObjectRelationshipAlreadyAdded[relationGovernorIndex] != true)
							{
								subjectObjectRelationshipAlreadyAdded[relationGovernorIndex] = true;
							#else
							if(!((currentRelationInList->subjObjRelationAlreadyAdded) && (currentRelationInList2->subjObjRelationAlreadyAdded)))
							{//do not use a subj-obj pair if the same subj or obj has already been used in the generation of another pair
							#endif
								#ifdef GIA_TRANSLATOR_DEBUG
								//cout << "(!((currentRelationInList->subjObjRelationAlreadyAdded) && (currentRelationInList2->subjObjRelationAlreadyAdded)))" << endl;
								//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								#endif

								int objectEntityIndexTemp = subjectObjectEntityIndexArray[OBJECT_INDEX];
								int subjectEntityIndexTemp = subjectObjectEntityIndexArray[SUBJECT_INDEX];
								GIAentityNode* objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
								GIAentityNode* subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

								if(relationGovernorIndex == relationGovernorIndex2)
								{//found a matching object-subject relationship

									#ifdef GIA_TRANSLATOR_DEBUG
									//cout << "\n\n relationGovernorIndex2 == relationGovernorIndex2 " << endl;
									//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;
									//cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;
									//cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;
									#endif

									#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

									bool subjectIsConnectedToAnAdvMod = false;

									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
									{
									#endif
										//find out if the subject is connected to an _advmod, if so create a dummy entity as the subject, and assign the subject as a condition [instead of forming default subject-action-object relationship]

										GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											if(!(currentRelationInList3->disabled))
											{
											#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
												{

													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
													if(subjectEntityTemp->entityName == currentRelationInList3->relationDependent)
													{//subject is connected to an _advmod

														GIAentityNode* actionOrSubstanceConditionObjectEntity;
														GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
														GIAentityNode* conditionConceptEntity = subjectEntityTemp;

														conditionConceptEntity->isSubstanceQuality = false;	//added 5 July 2013	//eg Apples are used for making juice.	- to prevent for from being left marked as a quality based on the original _advmod relation

														subjectIsConnectedToAnAdvMod = true;

														/*eg;  Space is saved by having a chicken.
														_obj(save[3], space[1]) 	[IRRELEVANT]
														_advmod(save[3], by[4])
														_obj(have[5], chicken[7])
														_subj(have[5], by[4])
														*/

													#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION

														if(0)
														{

														}
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_CONDITION_DEFINITION
														else if(passdefinition || partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound)
														{
															bool negative = subjectObjectFunctionEntityArray[SUBJECT_INDEX]->negative;
															subjectIsConnectedToAnAdvMod = true;
															actionOrSubstanceConditionObjectEntity = objectEntityTemp;
															#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
															GIAentityNodeArray[subjectObjectEntityIndexArray[SUBJECT_INDEX]] = addOrConnectBeingDefinitionConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#else
															addOrConnectBeingDefinitionConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#endif
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
														else if(passcomposition)
														{
															bool negative = subjectObjectFunctionEntityArray[SUBJECT_INDEX]->negative;
															subjectIsConnectedToAnAdvMod = true;
															actionOrSubstanceConditionObjectEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
															#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
															GIAentityNodeArray[subjectObjectEntityIndexArray[SUBJECT_INDEX]] = addOrConnectHavingPropertyConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#else
															addOrConnectHavingPropertyConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#endif
														}
														#endif
														else
														{
															//standard action/substance condion (ie action condition in this context)
															actionOrSubstanceConditionObjectEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															cout << "actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															#ifdef GIA_TRANSLATOR_DEBUG
															//cout << "actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
															#endif
															GIAentityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionObjectEntity);	//not required is done later?
															actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[actionIndex];
															#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
															GIAentityNodeArray[subjectObjectEntityIndexArray[SUBJECT_INDEX]] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#else
															addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
															#endif
															GIAentityNode* actionEntity = actionOrSubstanceConditionObjectEntity;
															GIAentityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTemp, actionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}

													#else

														#ifdef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
														cout << "error: ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION not yet coded; in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing" << endl;
														#else
														//create arbitrarySubjectSpecialConceptEntityNode
														string arbitrarySubjectSpecialConceptEntityNodeName = ARBITRARY_SUBJECT_SPECIAL_CONCEPT_NODE_NAME;
														bool entityAlreadyExistant = false;
														GIAentityNode* arbitrarySubjectSpecialConceptEntityNode = findOrAddConceptEntityNodeByNameSimpleWrapper(&arbitrarySubjectSpecialConceptEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);
														subjectEntityTemp = arbitrarySubjectSpecialConceptEntityNode;		//overwrites subjectEntityTemp
														#endif

														if(0)
														{

														}
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
														else if(passdefinition)
														{
															actionOrSubstanceConditionObjectEntity = arbitrarySubjectSpecialConceptEntityNode;
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														else if(passcomposition)
														{
															actionOrSubstanceConditionObjectEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
														}
														#endif
														else
														{
															actionOrSubstanceConditionObjectEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															#ifdef GIA_TRANSLATOR_DEBUG
															//cout << "actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
															#endif
															GIAentityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionObjectEntity);	//not required is done later?
														}

														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														passdefinition = false;		//treat "being" as an action
														#endif
														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														passpasscomposition = false;		//treat "being" as an action
														#endif

														#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
														GIAentityNodeArray[subjectObjectEntityIndexArray[SUBJECT_INDEX]] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														#else
														addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														#endif
													#endif

													}
													#endif

													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
													if(currentRelationInList3->relationGovernor == RELATION_ENTITY_BE)
													{//subject is connected to an _advmod

														if(passdefinition)
														{
															subjectIsConnectedToAnAdvMod = true;

															/*eg; The chicken is 3 minutes late.
															_advmod(be[3], late[6])
															_obj(be[3], minutes[5])
															_subj(be[3], chicken[2])
															_quantity(minutes[5], 3[4]) [IRRELEVANT]
															*/

															#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO

																GIAentityNode* measureEntity = objectEntityTemp;
																GIAentityNode* baseEntity = subjectEntityTemp;
																int substanceIndex = currentRelationInList3->relationDependentIndex;
																GIAentityNode* substanceEntity = GIAentityNodeArray[substanceIndex];

																GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(baseEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
																GIAentityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(substanceEntity, objectEntityTemp, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
																#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
																disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList3->relationGovernorIndex]);
																#endif
																#endif

															#else

																cout << "warning: GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_... defined - source may require review" << endl;

																GIAentityNode* baseEntity = subjectEntityTemp;
																GIAentityNode* definitionEntity = GIAentityNodeArray[currentRelationInList3->relationDependentIndex];

																int substanceIndex = objectEntityIndexTemp;
																GIAentityNode* substanceEntity = objectEntityTemp;

																addDefinitionToEntity(baseEntity, definitionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(definitionEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

															#endif

														}
													}
													#endif
												}

											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											}
											#endif
											currentRelationInList3 = currentRelationInList3->next;
										}
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									}
									#endif

									#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
									if(!subjectIsConnectedToAnAdvMod)
									#else
									if(true)
									#endif
									{
									#endif

										if(false)
										{

										}
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_DEFINITION_BASIC
										//if(currentRelationInList->relationGovernor == RELATION_ENTITY_BE)
										else if(passdefinition)
										{//expected subject-object relationship is a definition "is"

											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "passdefinition:" << endl;
											//cout << "objectEntityTemp = " << objectEntityTemp->entityName << endl;
											//cout << "subjectEntityTemp = " << subjectEntityTemp->entityName << endl;
											#endif
											//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
												//NB definitions are only assigned to entities, not substances (instances of entities)


											#ifndef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
											if(currentSentenceInList->isQuestion == true)
											#else
											if(subjectEntityTemp->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
											#endif
											{//this section of code is no longer used for RelEx: NB both Relex and Stanford CoreNLP ouput "what is"/"who is" queries in appos format (not in obj/subj format)
											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
												//cout << "GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY" << endl;
												//switch object/subject variables [transform question into answer form]
												bool sameReferenceSet = false;
												addDefinitionToEntity(objectEntityTemp, subjectEntityTemp, sameReferenceSet);
											#else

												//added 20 October 2011 [what is the time?]
												GIAentityNode* actionOrSubstanceConditionSubjectEntity = objectEntityTemp;
												GIAentityNode* actionOrSubstanceConditionObjectEntity = subjectEntityTemp;
												GIAentityNode* conditionConceptEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
												GIAentityNodeArray[subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX]] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
												#else
												addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
												#endif
											#endif
											}
											else
											{
											#endif
												bool sameReferenceSet = false;
												addDefinitionToEntity(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
											#ifndef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
											}
											#endif

											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
										//NB this case may be redundant because Relex automatical interprets these cases as preadj [eg _predadj(tom[1], happy[3])]
										//else if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_1) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_2) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_3))
										else if(passcomposition)
										{//subject-object relationship is a composition [substance]
											bool sameReferenceSet = false;
											GIAentityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
												//check can use substances for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										else if(partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound)
										{//this was removed from Stanford execution 15 May 2012

											#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
												//eg The rabbit is 20 meters away.	[away is a substance of rabbit, not a condition of rabbit]
												GIAentityNode* subjectEntityOrSubstance = subjectEntityTemp;	//aka subjectObjectEntityArray[SUBJECT_INDEX];
												int substanceIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
												GIAentityNode* substanceEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												bool sameReferenceSet = false;	//CHECK THIS???
												GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(subjectEntityOrSubstance, substanceEntity, sameReferenceSet);

											#else
												//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a substance of rabbit]

												GIAentityNode* subjectEntityOrSubstance = subjectEntityTemp;
												GIAentityNode* specialConditionNode = GIAentityNodeArray[relationGovernorIndex2];

												#ifdef GIA_TRANSLATOR_DEBUG
												//cout << "partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound" << endl;
												//cout << "subjectEntityOrSubstance->entityName = " << subjectEntityOrSubstance->entityName << endl;
												//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;
												#endif

												string conditionName = "specialCondition";

												bool entityAlreadyExistant = false;
												#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
												int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
												currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

												GIAentityNode* conditionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);
												GIAentityNodeArray[featureIndexOfCondition] = addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
												#else
												GIAentityNode* conditionConceptEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(&conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);
												addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
												#endif
											#endif
										}
									#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
									#ifdef GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_WITH_BE_AS_DEFINITION_LINK
										else if(partnerTypeObjectSpecialConditionToDoSubstanceFound)
										{
											partnerTypeObjectSpecialConditionToDoSubstanceFoundAssumeUsed = false;
											//relevant code is executed in function defineToBeAndToDoConditions() instead
										}
										#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK_AND_ACTION_DEFINITION
										else if(partnerTypeObjectSpecialConditionToBeSubstanceFound)
										{
											/*
											Eg;	 The pastry tasted awesome.
											_to-be(taste[3], awesome[4])
											_subj(taste[3], pastry[2])
											*/

											//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
											GIAentityNode* objectEntityTempUpdated = subjectObjectEntityArray[SUBJECT_INDEX];
											GIAentityNode* actionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
											int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];

											#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
											bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);	//subject relation
											#else
											bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAentityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTempUpdated, actionEntity, sameReferenceSet);

											//create a substance link between the subject and object
											GIAentityNode* substanceEntity = subjectObjectEntityArray[OBJECT_INDEX];
											int substanceIndex = subjectObjectEntityIndexArray[OBJECT_INDEX];
											GIAentityNode* ownerEntity = actionEntity;

											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "#ifdef GIA_TRANSLATOR_DEBUG" << endl;
											//cout << "substanceName = " << substanceEntity->entityName << endl;
											//cout << "ownerName = " << ownerEntity->entityName << endl;
											#endif

											#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
											sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);	//subject relation
											#else
											sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(ownerEntity, substanceEntity, sameReferenceSet);

										}
										#endif
									#endif
									#endif
										else
										{//assume that the subject-object relationships is an action
											string actionName = currentRelationInList->relationGovernor;
											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "1 actionName = " << actionName << endl;
											#endif
											int actionIndex = relationGovernorIndex;
											GIAentityNode* actionEntity = GIAentityNodeArray[actionIndex];

											//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)

											#ifdef GIA_TRANSLATOR_DEBUG
											/*
											cout << "SUBJECT_INDEX = " << SUBJECT_INDEX << endl;
											cout << "OBJECT_INDEX = " << OBJECT_INDEX << endl;
											cout << "subjectObjectName[SUBJECT_INDEX] = " << subjectObjectName[SUBJECT_INDEX] << endl;
											cout << "subjectObjectName[OBJECT_INDEX] = " << subjectObjectName[OBJECT_INDEX] << endl;
											cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;
											cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;
											cout << "relationDependentIndex = " << relationDependentIndex << endl;
											cout << "relationDependentIndex2 = " << relationDependentIndex2 << endl;
											*/
											#endif

											#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
											bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
											bool sameReferenceSetObject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList2);
											#else
											bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAentityNodeArray[actionIndex] = addOrConnectActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity, sameReferenceSetSubject, sameReferenceSetObject);
										}
									#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
									}
									#endif

									#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
									bool passAlreadyAddedConditions = true;
									bool relexAdditionalRequirement = false;
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
									#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
									if(!subjectIsConnectedToAnAdvMod)
									{
										relexAdditionalRequirement = true;
									}
									#endif
									#endif
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_WITH_BE_AS_DEFINITION_LINK
									if(relexAdditionalRequirement)
									{
										if(partnerTypeObjectSpecialConditionToDoSubstanceFound)
										{
											if(!partnerTypeObjectSpecialConditionToDoSubstanceFoundAssumeUsed)
											{
												passAlreadyAddedConditions = false;
											}
										}
									}
									#endif
									if(passAlreadyAddedConditions)
									{
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;
									}
									#endif
								}
								#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
								else
								{//do not find matching object-subject relationship

									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
									{
									#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
										//[search for intermediary {ie redundant} relations, and if so create a condition link between subject and object]
										if(!foundPartner)
										{//do not overwrite usage of subj/obj if a direct link [ie action] has been found (this condition probably/mau not be required)

											if(subjectObjectFunctionEntityArray[SUBJECT_INDEX]->entityName == RELATION_ENTITY_BE)
											{
												GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
												while(currentRelationInList3->next != NULL)
												{
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
													if(!(currentRelationInList3->disabled))
													{
													#endif
														if(currentRelationInList3->relationGovernor == RELATION_ENTITY_BE)
														{
															if((currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD))	//OR if (currentRelationInList3->relationType == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
															{
																if(currentRelationInList3->relationDependent == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
																{
																	/*
																	Eg;	What are the patent claims on?
																	_obj(on[6], _$qVar[1])
																	_advmod(be[2], on[6])	+ on(be[2], on[6]) [DUPLICATE INFO: IGNORE]
																	_subj(be[2], claim[5])
																	*/

																	//create a condition link between the object and subject, based upon RELATION_ENTITY_BE

																	GIAentityNode* subjectEntityOrSubstance = subjectEntityTemp;
																	GIAentityNode* specialConditionNode = objectEntityTemp;


																	//should really take into account the boolean and of both values: bool relationNegative = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex]->negative & GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->negative;

																	GIAentityNode* conditionConceptEntity = GIAentityNodeArray[currentRelationInList3->relationDependentIndex];

																	#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
																	conditionConceptEntity->disableParsingAsPrepositionRelationTemp = true;		//Added 30 Oct 2011a
																	#endif

																	#ifdef GIA_TRANSLATOR_DEBUG
																	//cout << "subjectEntityOrSubstance->entityName = " << subjectEntityOrSubstance->entityName << endl;
																	//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;
																	//cout << "conditionConceptEntity->entityName = " << conditionConceptEntity->entityName << endl;
																	#endif

																	#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
																	GIAentityNodeArray[currentRelationInList3->relationDependentIndex] = addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
																	#else
																	addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
																	#endif

																	foundPartner = true;
																	currentRelationInList->subjObjRelationAlreadyAdded = true;
																	currentRelationInList2->subjObjRelationAlreadyAdded = true;
																}
															}
														}
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
													}
													#endif

													currentRelationInList3 = currentRelationInList3->next;
												}
											}
										}
										#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
										//[search for special relation _to-do AND _subj, and if so create a substance link between subject and object]
										if(!foundPartner)
										{//do not overwrite usage of subj/obj if a direct link [ie action] has been found (this condition probably/may not be required)

											if(partnerTypeObjectSpecialConditionToDoSubstanceFound)
											{
												if(subjectObjectFunctionEntityArray[SUBJECT_INDEX]->entityName == subjectObjectEntityArray[OBJECT_INDEX]->entityName)
												{
													/*
													Eg;	Luke is slow.
													_to-do(be[2], slow[3])
													_subj(slow[3], Luke[1])
													*/

													//create a substance link between the object and subject
													int substanceIndex = objectEntityIndexTemp;
													GIAentityNode* substanceEntity = objectEntityTemp;
													GIAentityNode* ownerEntity = subjectEntityTemp;
													#ifdef GIA_TRANSLATOR_DEBUG
													//cout << "substanceName = " << substanceEntity->entityName << endl;
													//cout << "ownerName = " << ownerEntity->entityName << endl;
													#endif

													GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(ownerEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

													foundPartner = true;
													currentRelationInList->subjObjRelationAlreadyAdded = true;
													currentRelationInList2->subjObjRelationAlreadyAdded = true;
												}
											}
										}
										#endif
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									}
									#endif
								}
								#endif
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
					}
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void linkSubjectOrObjectRelationships(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	/*
	Tom runs quickly. 	_subj(run[2], Tom[1])
	The bike was ridden.	_obj(ride[4], bike[2])
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT1] = true; param.relationArrayTest[REL1][REL_ENT1] = relationGovernorCompositionNameArray; param.relationArrayTestSize[REL1][REL_ENT1] = RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES; param.relationArrayTestIsNegative[REL1][REL_ENT1] = true;
	param.useRelationArrayTest[REL1][REL_ENT1] = true; param.relationArrayTest[REL1][REL_ENT1] = relationGovernorDefinitionNameArray; param.relationArrayTestSize[REL1][REL_ENT1] = RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES; param.relationArrayTestIsNegative[REL1][REL_ENT1] = true;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS;
	#endif
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;

	/*
	Tom runs quickly. 	_subj(run[2], Tom[1])
	*/
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToSubject;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypeSubjectNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES;
	genericDependecyRelationInterpretation(&paramA, REL1);

	/*
	The bike was ridden.	_obj(ride[4], bike[2])
	*/
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectActionToObject;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypeObjectNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_OBJECT_NUMBER_OF_TYPES;
	genericDependecyRelationInterpretation(&paramB, REL1);

#else
 	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;

	string subjectObjectName;
	GIAentityNode* subjectObjectEntity;
	GIAentityNode* subjectObjectFunctionEntity;

	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			subjectObjectName = currentRelationInList->relationDependent;
			int subjectObjectEntityIndex = relationDependentIndex;
			int subjectObjectFunctionEntityIndex = relationGovernorIndex;
			subjectObjectEntity = GIAentityNodeArray[subjectObjectEntityIndex];
			subjectObjectFunctionEntity = GIAentityNodeArray[subjectObjectFunctionEntityIndex];

			bool passed = false;
			bool passsubject = false;
			bool passobject = false;

			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
					passsubject = true;
				}
			}
			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
					passobject = true;
				}
			}

			bool passcomposition = false;
			for(int i=0; i<RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == relationGovernorCompositionNameArray[i])
				{
					passcomposition = true;
				}
			}

			bool passdefinition = false;
			for(int i=0; i<RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == relationGovernorDefinitionNameArray[i])
				{
					passdefinition = true;
				}
			}

			if(passed)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "subjectObjectName = " << subjectObjectName << endl;
				#endif
				if(!(currentRelationInList->subjObjRelationAlreadyAdded))
				{//add independent action if appropriate
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "(!(currentRelationInList->subjObjRelationAlreadyAdded))" << endl;
					#endif
					if(passdefinition)
					//if(currentRelationInList->relationGovernor == RELATION_ENTITY_BE)
					{
					}
					//else if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_1) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_2) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_3))
					else if(passcomposition)
					{
					}
					else
					{//assume that the subject-object relationships is an action
						string actionName = currentRelationInList->relationGovernor;
						#ifdef GIA_TRANSLATOR_DEBUG
						//cout << "2 actionName = " << actionName << endl;
						#endif
						int actionIndex = subjectObjectFunctionEntityIndex;	//relationGovernorIndex
						GIAentityNode* actionEntity = GIAentityNodeArray[actionIndex];

						#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
						bool subjectOrObjectIsConnectedToAnAdvMod = false;

						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
							//find out if the subject is connected to an _advmod, if so assign it as an action condition instead of a subject+action
							GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								if(!(currentRelationInList2->disabled))
								{
								#endif
									if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
									{
										GIAentityNode* actionOrSubstanceConditionObjectEntity;
										int actionIndex;
										if(passsubject && (subjectObjectEntity->entityName == currentRelationInList3->relationDependent))
										{//subject is connected to an _advmod
											/*eg 1 Space is saved by running fast.
											_obj(save[3], space[1])	[NOT MODIFIED]
											_advmod(save[3], by[4])
											_advmod(run[5], fast[6]) [NOT MODIFIED + IRRELEVANT]
											_subj(run[5], by[4])
											-> by(save[3], run[5])
											*/
											actionOrSubstanceConditionObjectEntity = subjectObjectFunctionEntity;	//_subj relationGoverorIndex = run
											actionIndex = subjectObjectFunctionEntityIndex;
											subjectOrObjectIsConnectedToAnAdvMod = true;

										}
										else if(passobject && (subjectObjectFunctionEntity->entityName == currentRelationInList3->relationDependent))
										{//object function is connected to an _advmod
											/*eg 2 What is the Co-cart designed for?
											for(design[5], _$qVar[1]) [NOT MODIFIED + IRRELEVANT]
											_obj(design[5], Co[4]) [NOT MODIFIED]
											_advmod(design[5], for[6])
											_obj(for[6], _$qVar[1])
											-> for(design[5], _$qVar[1])
											*/
											actionOrSubstanceConditionObjectEntity = subjectObjectEntity;	//_obj relationDependentIndex = $qVar
											actionIndex = subjectObjectEntityIndex;
											subjectOrObjectIsConnectedToAnAdvMod = true;
										}
										else
										{

										}

										if(subjectOrObjectIsConnectedToAnAdvMod)
										{

											GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
											GIAentityNode* conditionConceptEntity = GIAentityNodeArray[currentRelationInList3->relationDependentIndex];

											#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
											conditionConceptEntity->disableParsingAsPrepositionRelationTemp = true;		//Added 30 Oct 2011a
											#endif

											GIAentityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionObjectEntity);
											actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[actionIndex];

											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
											//cout << "actionOrSubstanceConditionSubjectEntity = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
											//cout << "conditionConceptEntity = " << conditionConceptEntity->entityName << endl;
											#endif

											#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC
											GIAentityNodeArray[currentRelationInList3->relationDependentIndex] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
											#else
											addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
											#endif
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								}
								#endif
								currentRelationInList3 = currentRelationInList3->next;
							}
							#endif
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						}
						#endif

						if(!subjectOrObjectIsConnectedToAnAdvMod)
						{
						#endif
							#ifdef GIA_TRANSLATOR_DEBUG
							/*
							cout << "relationGovernorIndex = " << relationGovernorIndex << endl;
							cout << "relationDependentIndex = " << relationDependentIndex << endl;
							cout << "subjectObjectName = " << subjectObjectName << endl;
							cout << "subjectObjectEntity->entityName = " << subjectObjectEntity->entityName << endl;
							cout << "subjectObjectFunctionEntity->entityName = " << subjectObjectFunctionEntity->entityName << endl;
							*/
							#endif

							if(passsubject)
							{//fired by joe..???? [is there a possible example of this?]

								#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
								bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
								GIAentityNode* subjectEntityTemp = subjectObjectEntity;
								GIAentityNodeArray[actionIndex] = addOrConnectActionToSubject(subjectObjectEntity, actionEntity, sameReferenceSetSubject);
							}
							else if(passobject)
							{//eg the bow was fired

								#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
								bool sameReferenceSetObject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
								GIAentityNode* objectEntityTemp = subjectObjectEntity;
								GIAentityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTemp, actionEntity, sameReferenceSetObject);
							}
						#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
						}
						#endif
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#elif defined GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

//is this still used by Stanford? [appears to be Relex only]
void linkObjectSubjectOfPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	The garage is next to the house.	_pobj(next_to, house)  + _psubj(next_to, garage)
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL2; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT3] = REL2; param.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;
	#endif
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
			{
				//now find the associated object..
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching preposition of object-subject relationship


								GIAentityNode* entityNode = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								GIAentityNode* conditionObjectEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
								GIAentityNode* conditionConceptEntity = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex];

								#ifdef GIA_TRANSLATOR_DEBUG
								/*
								cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								cout << "currentRelationInList2->relationDependentIndex = " << currentRelationInList2->relationDependentIndex << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								cout << "currentRelationInList2->relationGovernorIndex = " << currentRelationInList2->relationGovernorIndex << endl;
								cout << "entityNode->entityName = " << entityNode->entityName << endl;
								cout << "conditionObjectEntity->entityName = " << conditionObjectEntity->entityName << endl;		//this is wrong
								cout << "conditionConceptEntity->entityName = " << conditionConceptEntity->entityName << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								*/
								#endif

								//OLD: should really take into account the boolean and of both values: bool relationNegative = GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->negative & GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->negative;
								//OLD: createConditionBasedUponPreposition(entityNode, conditionObjectEntity, conditionConceptEntity->entityName, false, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);

								#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//CHECK THIS... linkObjectSubjectOfPreposition use DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS?
								GIAentityNodeArray[currentRelationInList2->relationGovernorIndex] = addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionConceptEntity, sameReferenceSet);
								#else
								addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
								#endif


								/*OLD:
								bool passedPrepositionTime = false;
								if(conditionObjectEntity->hasAssociatedTime)	//added 10 April 2012 to make stanford compatible
								{
									passedPrepositionTime = true;
								}
								if(entityNode->hasAssociatedInstanceTemp)
								{
									entityNode = entityNode->associatedInstanceNodeList.back();
								}
								if(conditionObjectEntity->hasAssociatedInstanceTemp)
								{
									conditionObjectEntity = conditionObjectEntity->associatedInstanceNodeList.back();
								}


								if(passedPrepositionTime)
								{
									addTimeConditionToEntity(entityNode, conditionObjectEntity, conditionConceptEntity);

								}
								else
								{
									addOrConnectConditionToEntity(entityNode, conditionObjectEntity, conditionConceptEntity);
								}
								*/

							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void linkConjunctionConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	/*
	Tom and/or Max eat the cake.	conj_and(Tom[1], Max[3]) / conj_or(Tom[2], Max[4])
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
	param.numberOfRelations = 1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT3] = REL1; param.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE;
	#endif

	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_AND;
	paramA.conditionEntityDefaultName = RELATION_TYPE_CONJUGATION_AND_BASIC; //change the conditionType name (_conj_and -> and)
	genericDependecyRelationInterpretation(&paramA, REL1);

	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_OR;
	paramB.conditionEntityDefaultName = RELATION_TYPE_CONJUGATION_OR_BASIC; //change the conditionType name (_conj_or -> or)
	genericDependecyRelationInterpretation(&paramB, REL1);

	GIAgenericDepRelInterpretationParameters paramC = param;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_BUT;
	paramC.conditionEntityDefaultName = RELATION_TYPE_CONJUGATION_BUT_BASIC; //change the conditionType name (_conj_but -> but)
	genericDependecyRelationInterpretation(&paramC, REL1);

	GIAgenericDepRelInterpretationParameters paramD = param;
	paramD.useRelationTest[REL1][REL_ENT3] = true; paramD.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_YET;
	paramD.conditionEntityDefaultName = RELATION_TYPE_CONJUGATION_YET_BASIC; //change the conditionType name (_conj_yet -> yet)
	genericDependecyRelationInterpretation(&paramD, REL1);

	GIAgenericDepRelInterpretationParameters paramE = param;
	paramE.useRelationTest[REL1][REL_ENT3] = true; paramE.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_NOR;
	paramE.conditionEntityDefaultName = RELATION_TYPE_CONJUGATION_NOR_BASIC; //change the conditionType name (_conj_nor -> nor)
	genericDependecyRelationInterpretation(&paramE, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[relationGovernorIndex];
			GIAentityNode* actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[relationDependentIndex];

			bool passed = false;
			for(int i=0; i<RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; i++)
			{
				if(relationType == relationTypeConjugationNameArray[i])
				{
					passed = true;
					#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
					relationType = relationTypeConjugationBasicNameArray[i];
					#endif
				}
			}

			if(passed)
			{
				string conditionName = relationType;

				bool entityAlreadyExistant = false;
				int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
				currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;	
				GIAentityNode* conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "actionOrSubstanceConditionSubjectEntity->entityName = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
				cout << "actionOrSubstanceConditionObjectEntity->entityName = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
				cout << "conditionEntity->entityName = " << conditionEntity->entityName << endl;
				#endif

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE, currentRelationInList);	//eg "and that has a house" versus "and has a house" ??? [untested]
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				GIAentityNodeArray[featureIndexOfCondition] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionEntity, sameReferenceSet);
				#else
				addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionEntity, sameReferenceSet);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void linkConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK

	#ifdef GIA_IGNORE_MEANINGLESS_RELATIONS
	//NB with GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK, meaningless relations are now disabled completely rather than just ignored
	//to prevent meaningless relex relations; eg "on(be[2], on[6])"
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramA.numberOfRelations = 1;
	paramA.useRelationIndexTest[REL1][REL_ENT3] = true; paramA.relationIndexTestRelationID[REL1][REL_ENT3] = REL1; paramA.relationIndexTestEntityID[REL1][REL_ENT3] = REL_ENT2;
	paramA.disableRelationDuringLink[REL1] = true;
	genericDependecyRelationInterpretation(&paramA, REL1);
	#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_SUBSTANCE_LINK_DEPENDING_UPON_ACTION_OR_SUBSTANCE
	/*
	Interpret "of" preposition as property
	NB this case "The house of Kriton is blue." should create 2 substance connections (not just 1)
	of(house[2], Kriton[4])
	_predadj(house[2], blue[6])
	*/
	/*but ignore cases where the subject of the preposition is an action;
	NB not in this case "She grew tired of the pie."
	Relex:
	of(tired[3], pie[6])
	_to-be(grow[2], tired[3])
	_subj(grow[2], she[1])
	Stanford:
	prep_of(grew-2, pie-6)
	acomp(grew-2, tired-3)
	nsubj(grew-2, She-1)
	*/
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	paramB.numberOfRelations = 1;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypePossessivePrepositionsNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES;
	GIAentityCharacteristic relationTestGrammaticalNumberB("grammaticalNumber", GRAMMATICAL_NUMBER_PLURAL_STRING, true);
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&relationTestGrammaticalNumberB);	//not plural: added 2h11e
	//paramB.relationTestSpecialCaseIsNotAction[REL1][REL_ENT1] = true;	//Stanford Condition
	//paramB.relationTestSpecialCaseIsNotToBeComplimentOfAction[REL1][REL_ENT1] = true;	//Relex Condition
	GIAentityCharacteristic relationTestSpecialCaseIsNotAction("isAction", "false");								//Stanford Condition
	paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseIsNotAction);			//Stanford Condition
	GIAentityCharacteristic relationTestSpecialCaseIsNotToBeComplimentOfAction("isToBeComplimentOfActionTemp", "false");			//Relex Condition
	paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseIsNotToBeComplimentOfAction);	//Relex Condition
	paramB.disableRelationDuringLink[REL1] = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function linkConditions
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectPropertyToEntity;
	paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	paramB.functionEntityRelationID[FUNC_ENT2] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramB.defaultSameSetRelationID = REL1; paramB.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;
	#endif
	genericDependecyRelationInterpretation(&paramB, REL1);
	#endif
	#endif

	/*standard preposition;
	Joe is sad at the park. at(sad[3], park[6])
	*/
	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	paramC.entityNodesActiveListConcepts = entityNodesActiveListConcepts;	//required for preposition entity node creation (findOrAddEntityNodeByNameSimpleWrapperCondition)
	paramC.numberOfRelations = 1;
	paramC.NLPdependencyRelationsType = NLPdependencyRelationsType;	//this is required for expectToFindPrepositionTest
	paramC.expectToFindPrepositionTest[REL1] = true;
	/*OLD: before updating GIA 1t2f preprocess relex conj_or/conj_and as _conj_or/_conj_and
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		paramC.useRelationArrayTest[REL1][REL_ENT3] = true; paramC.relationArrayTest[REL1][REL_ENT3] = relationTypeConjugationNameArray; paramC.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; paramC.relationArrayTestIsNegative[REL1][REL_ENT3] = true;	//NB this case is required because conjunctions relations in Relex/GIA are defined without a prepending "_", which means when (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX) they will be non-intentionally interpreted as prepositions
	}
	*/
	paramC.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	paramC.functionEntityRelationID[FUNC_ENT1] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	paramC.functionEntityRelationID[FUNC_ENT2] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT2;
	paramC.functionEntityRelationID[FUNC_ENT3] = REL1; paramC.functionEntityRelationEntityID[FUNC_ENT3] = REL_ENT3;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	paramC.defaultSameSetRelationID = REL1; paramC.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;
	#endif
	genericDependecyRelationInterpretation(&paramC, REL1);

	//interpret $qVar relationTypes as conditions:
	//this code should only be required for 'how' qvar, because the other qvars should be detected within one of the above loops
	GIAgenericDepRelInterpretationParameters paramC2 = paramC;
	paramC2.expectToFindPrepositionTest[REL1] = false;
	paramC2.useRelationArrayTest[REL1][REL_ENT3] = true; paramC2.relationArrayTest[REL1][REL_ENT3] = relationTypeQueryVariableNameArray; paramC2.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES; paramC.relationArrayTestIsNegative[REL1][REL_ENT3] = false;
	genericDependecyRelationInterpretation(&paramC2, REL1);

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function linkConditions
		{
		//#endif

			int actionOrSubstanceIndex = currentRelationInList->relationGovernorIndex;
			int actionOrSubstanceConditionIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[actionOrSubstanceIndex];
			GIAentityNode* actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[actionOrSubstanceConditionIndex];

			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			//cout << "linkConditions actionOrSubstanceConditionSubjectEntity = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
			//cout << "linkConditions actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
			#endif

			bool passed = true;
			#ifdef GIA_IGNORE_MEANINGLESS_RELATIONS
			bool prepositionFound1 = false;
			if(GIAentityNodeArray[actionOrSubstanceConditionIndex]->entityName == convertPrepositionToRelex(&relationType, &prepositionFound1))	//this was required to be updated for GIA 1t2f (preprocessing: relex preposition renaming to stanford)
			{//to prevent meaningless relex relations; eg "on(be[2], on[6])"
				passed = false;
			}
			#endif
			#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				if(GIAentityNodeArrayFilled[i])
				{
					if(GIAentityNodeArray[i]->entityName == relationType)
					{
						if(GIAentityNodeArray[i]->disableParsingAsPrepositionRelationTemp)
						{//to prevent duplicate relex relations;
							/*eg; "for"
							for(design[5], _$qVar[1])
							_obj(design[5], Co[4]) [IRRELEVANT]
							_advmod(design[5], for[6]) [IRRELEVANT]
							_obj(for[6], _$qVar[1])
							*/
							passed = false;
						}
					}
				}
			}
			#endif

			/* this is no longer required as of GIA 1t2f (preprocessing: relex conjunction dependency relations renaming)
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
			if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
			{
			#endif
				//NB this case is required because conjunctions relations in Relex/GIA are defined without a prepending "_", which means when (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX) they will be non-intentionally interpreted as prepositions
				#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
				for(int i=0; i<RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; i++)
				{
					if(relationType == relationTypeConjugationNameArray[i])
					{
						passed = false;
					}
				}
				#endif
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
			}
			#endif
			*/

			#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_SUBSTANCE_LINK_DEPENDING_UPON_ACTION_OR_SUBSTANCE
			bool prepositionFound = false;
			bool foundPossessivePreposition = false;
			for(int i=0; i<RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES; i++)
			{
				if(convertPrepositionToRelex(&relationType, &prepositionFound) == relationTypePossessivePrepositionsNameArray[i])
				{
					foundPossessivePreposition = true;
				}
			}
			if(foundPossessivePreposition)
			{
				bool foundAction = false;

				//this section was completely changed (perhaps fixed) in version 10 May 2012 - test this...
				if((actionOrSubstanceConditionSubjectEntity->isAction) || (actionOrSubstanceConditionSubjectEntity->isToBeComplimentOfActionTemp))
				{
					/*Relex:
					NB not in this case "She grew tired of the pie."
					of(tired[3], pie[6])
					_to-be(grow[2], tired[3])
					_subj(grow[2], she[1])
					*/
					/*Stanford:
					NB not in this case "She grew tired of the pie."
					prep_of(grew-2, pie-6)
					acomp(grew-2, tired-3)
					nsubj(grew-2, She-1)
					*/
					foundAction = true;
				}

				/*full list parse not necessarily, as only concerned about how the node has been defined within the given context/sentence
				vector<GIAentityNode*>::iterator entityIter;
				for(entityIter = entityNode->propertyNodeReverseList.begin(); entityIter != entityNode->propertyNodeReverseList.end(); entityIter++)
				{
					...
					currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, printType, currentReferenceInPrintList, writeFileObject);

				}
				*/

				if(!foundAction)
				{
					/*
					NB this case "The house of Kriton is blue." should create 2 substance connections (not just 1)
					of(house[2], Kriton[4])
					_predadj(house[2], blue[6])
					*/
					#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
					bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					GIAentityNodeArray[actionOrSubstanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(actionOrSubstanceConditionObjectEntity, actionOrSubstanceConditionSubjectEntity, sameReferenceSet);
					passed = false;
				}
			}
			#endif
			#endif

			if(passed)
			{
				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				createConditionBasedUponPreposition(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, relationType, false, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);
			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
void createConditionBasedUponPreposition(GIAentityNode* actionOrSubstanceConditionSubjectEntity, GIAentityNode* actionOrSubstanceConditionObjectEntity, GIArelation* currentRelationInList, bool negative, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet)
{
	string relationType = currentRelationInList->relationType;

	/* does not cover all grounds
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	relationType = performPrepositionReduction(relationType);
	#endif
	*/

	bool prepositionFound = false;
	string prepositionName = convertPrepositionToRelex(&relationType, &prepositionFound);

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "prepositionName = " << prepositionName << endl;
	#endif

	bool passedPrepositionTime = false;
	bool passedPrepositionLocation = false;
	bool passedPrepositionReasonOrCircumstances = false;
	bool passedPreposition = false;

	//parse specific relex questions: not required, as these have been added to the relationTypePreposition_NameArrays
	/*
	if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
	{
		passedPrepositionTime = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE)
	{
		passedPrepositionLocation = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY)
	{
		passedPrepositionReason = true;
	}
	*/

	#ifdef GIA_ENFORCE_USE_OF_RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES
	for(int i=0; i<RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionTimeNameArray[i])
		{
			if(actionOrSubstanceConditionObjectEntity->hasAssociatedTime)
			{
				passedPrepositionTime = true;
			}
			else if(prepositionName == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
			{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
				passedPrepositionTime = true;
			}
		}
	}
	#else
	if(actionOrSubstanceConditionObjectEntity->hasAssociatedTime)
	{
		passedPrepositionTime = true;
	}
	else if(prepositionName == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
	{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
		passedPrepositionTime = true;		//are time nodes required to be attached to when query nodes (REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)? Note this functionality has been removed with GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		passedPreposition = true;
	}
	#endif

	for(int i=0; i<RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionLocationNameArray[i])
		{
			if(!actionOrSubstanceConditionObjectEntity->hasAssociatedTime)
			{//NB "at" and "on" are shared for location and time prepositions
				passedPrepositionLocation = true;
			}
		}
	}
	for(int i=0; i<RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionReasonOrCircumstanceNameArray[i])
		{
			if(!actionOrSubstanceConditionObjectEntity->hasAssociatedTime)
			{//NB in case "_" and "_" are shared for reason and time prepositions
				passedPrepositionReasonOrCircumstances = true;
			}
		}
	}

	if(prepositionFound)
	{
		//passedPrepositionUnknown = true;		//removed 17 April 2012
		passedPreposition = true;
	}

	for(int i=0; i<RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES; i++)	//this code should only be required for 'how' qvar, because the other qvars should be detected within one of the above loops
	{
		if(prepositionName == relationTypeQueryVariableNameArray[i])
		{
			passedPreposition = true;
		}
	}

	if(passedPreposition)
	{
		//added 3 June 2012 for advanced referencing of prepositions
		GIAentityNode* conditionConceptEntity;
		#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
		int featureIndexOfPreposition = currentRelationInList->relationTypeIndex;
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
		string conditionName = prepositionName;
		conditionConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfPreposition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "prepositionFeatureFound = " << prepositionFeatureFound << endl;
		cout << "prepositionName = " << prepositionName << endl;
		cout << "featureIndexOfPreposition = " << featureIndexOfPreposition << endl;
		//cout << "conditionConceptEntity->entityName = " << conditionConceptEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrSubstanceConditionSubjectEntity = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrSubstanceConditionObjectEntity = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition conditionConceptEntity = " << conditionConceptEntity->entityName << endl;
		#endif

		if(passedPrepositionTime)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceConditionSubjectEntity->entityName = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
			cout << "timeConditionName = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAentityNodeArray[featureIndexOfPreposition] = addTimeConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#else
			addTimeConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#endif
		}
		else if(passedPrepositionLocation)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceConditionSubjectEntity->entityName = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
			cout << "locationConditionName = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAentityNodeArray[featureIndexOfPreposition] = addLocationConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#else
			addLocationConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#endif
		}
		else if(passedPrepositionReasonOrCircumstances)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceConditionSubjectEntity->entityName = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
			cout << "reasonConditionName = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAentityNodeArray[featureIndexOfPreposition] = addReasonConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#else
			addReasonConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#endif
		}
		//else if(passedPrepositionUnknown)		//removed 17 April 2012
		else
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceConditionSubjectEntity->entityName = " << actionOrSubstanceConditionSubjectEntity->entityName << endl;
			cout << "actionOrSubstanceConditionName = " << actionOrSubstanceConditionObjectEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAentityNodeArray[featureIndexOfPreposition] = addGenericConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#else
			addGenericConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
			#endif
			//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
		}

		//cout << "GIAentityNodeArray[featureIndexOfPreposition]->entityName = " << GIAentityNodeArray[featureIndexOfPreposition]->entityName << endl;

	}
	else if(passedPrepositionTime)
	{//required for extractDatesStanfordCoreNLP/extractDatesRelex

		addTimeToSubstance(actionOrSubstanceConditionObjectEntity);
	}
}

void addTimeToSubstance(GIAentityNode* timeConditionEntity)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeToSubstance timeConditionEntity->entityName = " << timeConditionEntity->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

}

/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAentityNode* substanceNode, GIAentityNode* timeConditionEntity, GIAentityNode* conditionConceptEntity, vector<GIAtimeConditionNode*>* timeConditionNodesActiveList, vector<long>* timeConditionNumbersActiveList)
#else
void addTimeConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* timeConditionEntity, GIAentityNode* conditionConceptEntity)
#endif
*/
GIAentityNode* addTimeConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* timeConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeConditionToEntity substanceNode->entityName = " << substanceNode->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = INT_DEFAULT_VALUE;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->entityName);
	GIAtimeConditionNode* newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();
	#endif
	*/

	GIAtimeConditionNode* newTimeCondition = new GIAtimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

	return addOrConnectConditionToEntity(substanceNode, timeConditionEntity, conditionConceptEntity, sameReferenceSet);
}

GIAentityNode* addLocationConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* locationConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet)
{
	locationConditionEntity->conditionType = CONDITION_NODE_TYPE_LOCATION;

	return addOrConnectConditionToEntity(substanceNode, locationConditionEntity, conditionConceptEntity, sameReferenceSet);
}

GIAentityNode* addReasonConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* reasonConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet)
{
	reasonConditionEntity->conditionType = CONDITION_NODE_TYPE_REASON;

	return addOrConnectConditionToEntity(substanceNode, reasonConditionEntity, conditionConceptEntity, sameReferenceSet);
}


GIAentityNode* addGenericConditionToEntity(GIAentityNode* substanceNode, GIAentityNode* substanceConditionEntity, GIAentityNode* conditionConceptEntity, bool sameReferenceSet)
{
	//timeConditionEntity->conditionType = CONDITION_NODE_TYPE_UNDEFINED;

	return addOrConnectConditionToEntity(substanceNode, substanceConditionEntity, conditionConceptEntity, sameReferenceSet);
}
#endif

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
string performPrepositionReduction(string relationType)
{
	string relationTypeModified = relationType;

	//perform preposition reduction based upon frenchEnglishPrepositions.ods;
	for(int i=0; i<RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES; i++)
	{
		int prepositionTypeNumberOfVariations = relationTypePropositionReductionNumberVariationsArray[i];
		for(int j=0; j<prepositionTypeNumberOfVariations; j++)
		{
			if(relationType == relationTypePropositionReductionNameArray[i][j])
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "performPrepositionReduction: matched preposition; " << relationType << endl;
				#endif
				relationTypeModified = relationTypePropositionReductionReplacementNamesArray[i];
			}
		}
	}

	return relationTypeModified;
}
#endif



//Stanford Only
void linkDependentActionsType2(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts)
{
	/*
	eg To copy the files[, ]create a backup of the old file.
	 use dep tag to connect dependent actions (interpret as condition - invent one)
	 dep(create-6, copy-2)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DEPENDENT;	//check this implementation is not conficting with any stanford dependency relation reductions as defined in RELATION_TYPE_DEPENDENT
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addOrConnectConditionToEntity;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	param.functionEntityRelationID[FUNC_ENT2] = REL1; param.functionEntityRelationEntityID[FUNC_ENT2] = REL_ENT1;
	//param.relationTestSpecialCaseIsAction[REL1][REL_ENT1] = true;	//added 28 July 2013
	//param.relationTestSpecialCaseIsAction[REL1][REL_ENT2] = true;	//added 28 July 2013
	GIAentityCharacteristic relationTestSpecialCaseIsAction("isAction", "true");
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseIsAction);
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&relationTestSpecialCaseIsAction);
	param.mustGenerateConditionName = true; param.conditionEntityDefaultName = GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_DEFAULT_CONDITION_NAME;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	param.defaultSameSetRelationID = REL1; param.defaultSameSetReferenceValue = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS;
	#endif
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)	//check this implementation is not conficting with any stanford dependency relation reductions as defined in RELATION_TYPE_DEPENDENT
			{
				string ownerName = currentRelationInList->relationGovernor;
				string substanceName = currentRelationInList->relationDependent;
				int ownerIndex = currentRelationInList->relationGovernorIndex;
				int substanceIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode* actionOrSubstanceConditionSubjectEntity = GIAentityNodeArray[substanceIndex];
				GIAentityNode* actionOrSubstanceConditionObjectEntity = GIAentityNodeArray[ownerIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "RELATION_TYPE_DEPENDENT" << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				#endif

				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif

				string conditionName = GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_DEFAULT_CONDITION_NAME;
				bool entityAlreadyExistant = false;
				int featureIndexOfCondition = currentSentenceInList->conditionEntityArtificialIndexCurrent;
				currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
				GIAentityNode* conditionEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAentityNodeArrayFilled, GIAentityNodeArray, featureIndexOfCondition, &conditionName, &entityAlreadyExistant, entityNodesActiveListConcepts);

				#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
				GIAentityNodeArray[featureIndexOfCondition] = addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionEntity, sameReferenceSet);
				#else
				addOrConnectConditionToEntity(actionOrSubstanceConditionSubjectEntity, actionOrSubstanceConditionObjectEntity, conditionConceptEntity, sameReferenceSet);
				#endif


				/*OLD linkPropertiesDependentActions():
				GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(ownerEntity, substanceEntity, sameReferenceSet);
				//GIAentityNodeArray[substanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(ownerEntity, substanceEntity, sameReferenceSet);	- check if this might ever be required instead
				*/
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

#endif

