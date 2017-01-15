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
 * File Name: GIATranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q4c 14-October-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorLinkEntities.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"






void linkPropertiesPossessiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[ownerIndex];
				
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "RELATION_TYPE_POSSESSIVE" << endl;				
				//cout << "substanceName = " << substanceEntity->entityName << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;
				#endif
				
				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(ownerEntity, substanceEntity, sameReferenceSet);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void linkPropertiesDescriptiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

				bool passed2 = true;
				bool passed3 = true;

				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
				{
				#endif
					passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
					passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAEntityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);
				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
				}
				#endif

				if(passed2 && passed3)
				{
					//create a new substance for the entity and assign a substance definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string substanceName = currentRelationInList->relationDependent;
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;
					GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
					GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

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

					#ifdef GIA_USE_ADVANCED_REFERENCING
					bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
					#else
					bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
					#endif
					GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(thingEntity, substanceEntity, sameReferenceSet);
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

#ifdef GIA_USE_ADVANCED_REFERENCING
void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], bool linkPreestablishedReferencesGIA)
#else
void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
#endif
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))		//?this condition may be required to prevent redundant RELATION_TYPE_APPOSITIVE_OF_NOUN relations eg in 'Where is the ball?'
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				string thingName = currentRelationInList->relationGovernor;
				string definitionName = currentRelationInList->relationDependent;
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int definitionIndex = currentRelationInList->relationDependentIndex;

				GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
				GIAEntityNode * definitionEntity = GIAEntityNodeArray[definitionIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "thingName = " << thingEntity->entityName << endl;
				cout << "definitionEntity = " << definitionEntity->entityName << endl;
				#endif

				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_APPOS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif

				#ifdef GIA_SUPPORT_ALIASES
				bool treatDefinitionAsEquality = false;
				bool treatDefinitionAsEqualityReversePrimary = false;
				#ifdef GIA_USE_ADVANCED_REFERENCING
				if(linkPreestablishedReferencesGIA)
				{
				#endif
					if((thingEntity->isSubstance || thingEntity->isNameQuery) && (definitionEntity->isSubstance || definitionEntity->isNameQuery))
					{//equality link found - add alias instead

						cout << "thingEntity->entityName = " << thingEntity->entityName << endl;
						cout << "definitionEntity->entityName = " << definitionEntity->entityName << endl;
						cout << "thingEntity->isSubstance = " << thingEntity->isSubstance << endl;
						cout << "definitionEntity->isSubstance = " << definitionEntity->isSubstance << endl;
						
						#ifndef GIA_SUPPORT_WHAT_IS_THE_TIME_QUERY_ALIAS_ANSWERS
						if(!(definitionEntity->hasAssociatedTime))
						{
						#endif
							treatDefinitionAsEquality = true;
							if(definitionEntity->grammaticalRelexPersonOrStanfordProperNounTemp || definitionEntity->isNameQuery)
							{
								#ifdef GIA_ALIASES_DEBUG
								cout << "linkEntityDefinitionsAppositiveOfNouns1" << endl;
								#endif
								//eg max = the brown dog
							}
							else if(thingEntity->grammaticalRelexPersonOrStanfordProperNounTemp || thingEntity->isNameQuery)
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
						#ifndef GIA_SUPPORT_WHAT_IS_THE_TIME_QUERY_ALIAS_ANSWERS
						}
						#endif
					}
					#ifdef GIA_SUPPORT_WHAT_IS_THE_TIME_QUERY_ALIAS_ANSWERS
					else if((thingEntity->isSubstance && thingEntity->entityName == FEATURE_RELEX_FLAG_TIME_NAME) && (definitionEntity->isQuery))
					{
						#ifdef GIA_ALIASES_DEBUG
						cout << "linkEntityDefinitionsAppositiveOfNouns4" << endl;
						#endif
						//eg what is the time
						treatDefinitionAsEquality = true;
						definitionEntity->isNameQuery = true;
					}
					#endif
				#ifdef GIA_USE_ADVANCED_REFERENCING
				}
				#endif
				if(treatDefinitionAsEquality)
				{
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
							GIAEntityNodeArray[thingIndex] = GIAEntityNodeArray[definitionIndex];
						}
						else
						{
							#ifdef GIA_ALIASES_DEBUG
							cout << "treatDefinitionAsEquality: !treatDefinitionAsEqualityReversePrimary" << endl;
							#endif
							//if no proper noun (or query) detected, each node is equal, eg the brown dog == the happy wolf]
							mergeEntityNodesAddAlias(thingEntity, definitionEntity);
							GIAEntityNodeArray[definitionIndex] = GIAEntityNodeArray[thingIndex];
						}
					}
				}
				else
				{
				#endif
					addDefinitionToEntity(thingEntity, definitionEntity, sameReferenceSet);
				#ifdef GIA_SUPPORT_ALIASES
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void linkSubjectOrObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
 	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;

	string subjectObjectName;
	GIAEntityNode * subjectObjectEntity;
	GIAEntityNode * subjectObjectFunctionEntity;

	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			subjectObjectName = currentRelationInList->relationDependent;
			int subjectObjectEntityIndex = relationDependentIndex;
			int subjectObjectFunctionEntityIndex = relationGovernorIndex;
			subjectObjectEntity = GIAEntityNodeArray[subjectObjectEntityIndex];
			subjectObjectFunctionEntity = GIAEntityNodeArray[subjectObjectFunctionEntityIndex];

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
						GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];


						bool subjectOrObjectIsConnectedToAnAdvMod = false;

						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
							//find out if the subject is connected to an _advmod, if so assign it as an action condition instead of a subject+action
							Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
								if(!(currentRelationInList2->disabled))
								{
								#endif
									if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
									{
										GIAEntityNode * actionOrSubstanceConditionEntity;
										int actionIndex;
										if(passsubject && (subjectObjectEntity->entityName == currentRelationInList3->relationDependent))
										{//subject is connected to an _advmod
											/*eg 1 Space is saved by running fast.
											_obj(save[3], space[1])	[IRRELEVANT]
											_advmod(save[3], by[4])
											_advmod(run[5], fast[6]) [IRRELEVANT]
											_subj(run[5], by[4])
											*/
											actionOrSubstanceConditionEntity = subjectObjectFunctionEntity;
											actionIndex = subjectObjectFunctionEntityIndex;
											subjectOrObjectIsConnectedToAnAdvMod = true;

										}
										else if(passobject && (subjectObjectFunctionEntity->entityName == currentRelationInList3->relationDependent))
										{//object function is connected to an _advmod
											/*eg 2 What is the Co-cart designed for?
											for(design[5], _$qVar[1]) [IRRELEVANT]
											_obj(design[5], Co[4]) [IRRELEVANT]
											_advmod(design[5], for[6])
											_obj(for[6], _$qVar[1])
											*/
											actionOrSubstanceConditionEntity = subjectObjectEntity;
											actionIndex = subjectObjectEntityIndex;
											subjectOrObjectIsConnectedToAnAdvMod = true;
										}
										else
										{

										}

										if(subjectOrObjectIsConnectedToAnAdvMod)
										{

											GIAEntityNode * actionOrSubstanceEntity = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex];
											GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];

											#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
											conditionTypeConceptEntity->disableParsingAsAPrepositionRelationTemp = true;		//Added 30 Oct 2011a
											#endif

											GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionEntity);
											actionOrSubstanceConditionEntity = GIAEntityNodeArray[actionIndex];

											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "actionOrSubstanceConditionEntity = " << actionOrSubstanceConditionEntity->entityName << endl;
											//cout << "actionOrSubstanceEntity = " << actionOrSubstanceEntity->entityName << endl;
											//cout << "conditionTypeConceptEntity = " << conditionTypeConceptEntity->entityName << endl;
											#endif
											
											addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

								#ifdef GIA_USE_ADVANCED_REFERENCING
								bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
								GIAEntityNode * subjectEntityTemp = subjectObjectEntity;
								GIAEntityNodeArray[actionIndex] = addOrConnectActionToSubject(subjectObjectEntity, actionEntity, sameReferenceSetSubject);
							}
							else if(passobject)
							{//eg the bow was fired

								#ifdef GIA_USE_ADVANCED_REFERENCING
								bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif

								//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
								GIAEntityNode * objectEntityTemp = subjectObjectEntity;
								GIAEntityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTemp, actionEntity, sameReferenceSetSubject);
							}

						}
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void linkSubjectObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
 	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;

	#ifdef USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD
	bool subjectObjectRelationshipAlreadyAdded[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE;i++)
	{
		subjectObjectRelationshipAlreadyAdded[i] = false;
	}
	#endif

	string subjectObjectName[2];
	GIAEntityNode * subjectObjectEntityArray[2];
	GIAEntityNode * subjectObjectFunctionEntityArray[2];
	int subjectObjectEntityIndexArray[2];
	int subjectObjectFunctionEntityIndexArray[2];

	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
			int relationDependentIndex = currentRelationInList->relationDependentIndex;
			subjectObjectName[SUBJECT_INDEX] = currentRelationInList->relationDependent;
			subjectObjectEntityArray[SUBJECT_INDEX] = GIAEntityNodeArray[relationDependentIndex];
			subjectObjectFunctionEntityArray[SUBJECT_INDEX] = GIAEntityNodeArray[relationGovernorIndex];
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
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						int relationGovernorIndex2 = currentRelationInList2->relationGovernorIndex;
						int relationDependentIndex2 = currentRelationInList2->relationDependentIndex;
						subjectObjectName[OBJECT_INDEX] = currentRelationInList2->relationDependent;
						subjectObjectEntityArray[OBJECT_INDEX] = GIAEntityNodeArray[relationDependentIndex2];
						subjectObjectFunctionEntityArray[OBJECT_INDEX] = GIAEntityNodeArray[relationGovernorIndex2];
						subjectObjectEntityIndexArray[OBJECT_INDEX] = relationDependentIndex2;
						subjectObjectFunctionEntityIndexArray[OBJECT_INDEX] = relationGovernorIndex2;

						bool passed2 = false;
						bool partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound = false;
						bool partnerTypeObjectSpecialConditionToDoSubstanceFound = false;
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

						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_TO_DO_SUBSTANCE_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionToDoSubstanceNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionToDoSubstanceFound = true;
								}
							}
							#endif
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						}
						#endif


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
								GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
								GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

								if(relationGovernorIndex == relationGovernorIndex2)
								{//found a matching object-subject relationship

									#ifdef GIA_TRANSLATOR_DEBUG
									//cout << "\n\n relationGovernorIndex2 == relationGovernorIndex2 " << endl;
									//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;
									//cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;
									//cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;
									#endif
									
									bool subjectIsConnectedToAnAdvMod = false;
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
									{
									#endif
										//find out if the subject is connected to an _advmod, if so create a dummy entity as the subject, and assign the subject as a condition [instead of forming default subject-action-object relationship]

										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											if(!(currentRelationInList3->disabled))
											{
											#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
												{

													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
													if(subjectEntityTemp->entityName == currentRelationInList3->relationDependent)
													{//subject is connected to an _advmod

														GIAEntityNode * actionOrSubstanceConditionEntity;
														GIAEntityNode * actionOrSubstanceEntity = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex];
														GIAEntityNode * conditionTypeConceptEntity = subjectEntityTemp;

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
															actionOrSubstanceConditionEntity = objectEntityTemp;
															addOrConnectBeingDefinitionConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
														else if(passcomposition)
														{
															bool negative = subjectObjectFunctionEntityArray[SUBJECT_INDEX]->negative;
															subjectIsConnectedToAnAdvMod = true;
															actionOrSubstanceConditionEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
															addOrConnectHavingPropertyConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}
														#endif
														else
														{
															//standard action/substance condion (ie action condition in this context)
															actionOrSubstanceConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															#ifdef GIA_TRANSLATOR_DEBUG
															//cout << "actionOrSubstanceConditionEntity = " << actionOrSubstanceConditionEntity->entityName << endl;
															#endif
															GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionEntity);	//not required is done later?
															actionOrSubstanceConditionEntity = GIAEntityNodeArray[actionIndex];
															addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

															GIAEntityNode * actionEntity = actionOrSubstanceConditionEntity;
															GIAEntityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTemp, actionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}

													#else

														#ifdef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
														cout << "error: ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION not yet coded; in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing" << endl;
														#else
														//create arbitrarySubjectSpecialConceptEntityNode
														string arbitrarySubjectSpecialConceptEntityNodeName = ARBITRARY_SUBJECT_SPECIAL_CONCEPT_NODE_NAME;
														bool entityAlreadyExistant = false;
														GIAEntityNode * arbitrarySubjectSpecialConceptEntityNode = findOrAddConceptEntityNodeByNameSimpleWrapper(&arbitrarySubjectSpecialConceptEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);
														subjectEntityTemp = arbitrarySubjectSpecialConceptEntityNode;		//overwrites subjectEntityTemp
														#endif

														if(0)
														{

														}
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
														else if(passdefinition)
														{
															actionOrSubstanceConditionEntity = arbitrarySubjectSpecialConceptEntityNode;
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														else if(passcomposition)
														{
															actionOrSubstanceConditionEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
														}
														#endif
														else
														{
															actionOrSubstanceConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															#ifdef GIA_TRANSLATOR_DEBUG
															//cout << "actionOrSubstanceConditionEntity = " << actionOrSubstanceConditionEntity->entityName << endl;
															#endif
															GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrSubstanceConditionEntity);	//not required is done later?
														}

														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														passdefinition = false;		//treat "being" as an action
														#endif
														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														passpasscomposition = false;		//treat "being" as an action
														#endif

														addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
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

																GIAEntityNode * measureEntity = objectEntityTemp;
																GIAEntityNode * baseEntity = subjectEntityTemp;
																int substanceIndex = currentRelationInList3->relationDependentIndex;
																GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

																GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(baseEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
																GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(substanceEntity, objectEntityTemp, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
																#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
																disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex]);
																#endif
																#endif

															#else

																cout << "warning: GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_... defined - source may require review" << endl;

																GIAEntityNode * baseEntity = subjectEntityTemp;
																GIAEntityNode * definitionEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];

																int substanceIndex = objectEntityIndexTemp;
																GIAEntityNode * substanceEntity = objectEntityTemp;

																addDefinitionToEntity(baseEntity, definitionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(definitionEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

															#endif

														}
													}
													#endif
												}

											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
											if(currentSentenceInList->isQuestion == true)
											#else
											if(subjectEntityTemp->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
											#endif
											{//this section of code is only used for RelEx: NB Stanford CoreNLP ouputs "what is"/"who is" queries in appos format (not in obj/subj format)
											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
												//switch object/subject variables [transform question into answer form]
												bool sameReferenceSet = false;
												addDefinitionToEntity(objectEntityTemp, subjectEntityTemp, sameReferenceSet);
											#else

												//added 20 October 2011 [what is the time?]
												GIAEntityNode * actionOrSubstanceEntity = objectEntityTemp;
												GIAEntityNode * actionOrSubstanceConditionEntity = subjectEntityTemp;
												GIAEntityNode * conditionTypeConceptEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												addOrConnectConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
											#endif
											}
											else
											{
												bool sameReferenceSet = false;
												addDefinitionToEntity(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
											}

											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
										//NB this case may be redundant because Relex automatical interprets these cases as preadj [eg _predadj(tom[1], happy[3])]
										//else if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_1) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_2) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_3))
										else if(passcomposition)
										{//subject-object relationship is a composition [substance]
											bool sameReferenceSet = false;
											GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
												//check can use substances for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
											disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										else if(partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound)
										{//this was removed from Stanford execution 15 May 2012

											#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
												//eg The rabbit is 20 meters away.	[away is a substance of rabbit, not a condition of rabbit]
												GIAEntityNode * subjectEntityOrSubstance = subjectEntityTemp;	//aka subjectObjectEntityArray[SUBJECT_INDEX];
												int substanceIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
												GIAEntityNode * substanceEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												bool sameReferenceSet = false;	//CHECK THIS???
												GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(subjectEntityOrSubstance, substanceEntity, sameReferenceSet);

											#else
												//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a substance of rabbit]

												GIAEntityNode * subjectEntityOrSubstance = subjectEntityTemp;
												GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationGovernorIndex2];
												
												#ifdef GIA_TRANSLATOR_DEBUG
												//cout << "partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound" << endl;
												//cout << "subjectEntityOrSubstance->entityName = " << subjectEntityOrSubstance->entityName << endl;
												//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;
												#endif
												
												string conditionTypeName = "specialCondition";

												bool entityAlreadyExistant = false;
												GIAEntityNode * conditionTypeConceptEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

												addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

											#endif
										}
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
										else if(partnerTypeObjectSpecialConditionToDoSubstanceFound)
										{

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
											GIAEntityNode * objectEntityTempUpdated = subjectObjectEntityArray[SUBJECT_INDEX];
											GIAEntityNode * actionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
											int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];

											#ifdef GIA_USE_ADVANCED_REFERENCING
											bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);	//subject relation
											#else
											bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAEntityNodeArray[actionIndex] = addOrConnectActionToObject(objectEntityTempUpdated, actionEntity, sameReferenceSet);

											//create a substance link between the subject and object
											GIAEntityNode * substanceEntity = subjectObjectEntityArray[OBJECT_INDEX];
											int substanceIndex = subjectObjectEntityIndexArray[OBJECT_INDEX];
											GIAEntityNode * ownerEntity = actionEntity;
											
											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "#ifdef GIA_TRANSLATOR_DEBUG" << endl;
											//cout << "substanceName = " << substanceEntity->entityName << endl;
											//cout << "ownerName = " << ownerEntity->entityName << endl;
											#endif
											
											#ifdef GIA_USE_ADVANCED_REFERENCING
											sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);	//subject relation
											#else
											sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(ownerEntity, substanceEntity, sameReferenceSet);

										}
										#endif
										else
										{//assume that the subject-object relationships is an action
											string actionName = currentRelationInList->relationGovernor;
											#ifdef GIA_TRANSLATOR_DEBUG
											//cout << "1 actionName = " << actionName << endl;
											#endif
											int actionIndex = relationGovernorIndex;
											GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];

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

											#ifdef GIA_USE_ADVANCED_REFERENCING
											bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
											bool sameReferenceSetObject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList2);
											#else
											bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAEntityNodeArray[actionIndex] = addOrConnectActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity, sameReferenceSetSubject, sameReferenceSetObject);
										}
									}

									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
									{
									#endif
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
										if(!partnerTypeObjectSpecialConditionToDoSubstanceFound || subjectIsConnectedToAnAdvMod)
										{
											foundPartner = true;
											currentRelationInList->subjObjRelationAlreadyAdded = true;
											currentRelationInList2->subjObjRelationAlreadyAdded = true;
										}
										#else
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;
										#endif
									#else
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;
									#endif
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									}
									else
									{
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;
									}
									#endif
								}
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
												Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
												while(currentRelationInList3->next != NULL)
												{
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
																	_advmod(be[2], on[6])
																	_subj(be[2], claim[5])
																	and/or;
																	_obj(on[6], _$qVar[1])
																	on(be[2], on[6])
																	_subj(be[2], claim[5])
																	*/

																	//create a condition link between the object and subject, based upon RELATION_ENTITY_BE

																	GIAEntityNode * subjectEntityOrSubstance = subjectEntityTemp;
																	GIAEntityNode * specialConditionNode = objectEntityTemp;


																	//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex]->negative & GIAEntityNodeArray[currentRelationInList3->relationDependentIndex]->negative;

																	GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];

																	#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
																	conditionTypeConceptEntity->disableParsingAsAPrepositionRelationTemp = true;		//Added 30 Oct 2011a
																	#endif
																	
																	#ifdef GIA_TRANSLATOR_DEBUG
																	//cout << "subjectEntityOrSubstance->entityName = " << subjectEntityOrSubstance->entityName << endl;
																	//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;
																	//cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
																	#endif

																	addOrConnectConditionToEntity(subjectEntityOrSubstance, specialConditionNode, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																	foundPartner = true;
																	currentRelationInList->subjObjRelationAlreadyAdded = true;
																	currentRelationInList2->subjObjRelationAlreadyAdded = true;
																}
															}
														}
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
													}
													#endif

													currentRelationInList3 = currentRelationInList3->next;
												}
											}
										}
										#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
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
													GIAEntityNode * substanceEntity = objectEntityTemp;
													GIAEntityNode * ownerEntity = subjectEntityTemp;
													#ifdef GIA_TRANSLATOR_DEBUG
													//cout << "substanceName = " << substanceEntity->entityName << endl;
													//cout << "ownerName = " << ownerEntity->entityName << endl;
													#endif
													
													GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntity(ownerEntity, substanceEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

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
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}



void linkIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
								GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];
								GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//eg the linebacker that gave the quarterback a push is blue. / the linebacker gave the quarterback a push
								GIAEntityNodeArray[substanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(thingEntity, substanceEntity, sameReferenceSet);
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void linkHavingPropertyConditionsAndBeingDefinitionConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	/*
			Space is saved through having a chicken.
			Space is saved by having a chicken.
			Space is saved by being a chicken.
				nsubjpass(saved-3, Space-1) / prepc_through(saved-3, having-5) / dobj(having-5, chicken-7)	[auxpass(saved-3, is-2)]
				nsubjpass(saved-3, Space-1) / agent(saved-3, having-5) / dobj(having-5, chicken-7)
				nsubjpass(saved-3, Space-1) / agent(saved-3, chicken-7) / cop(chicken-7, being-5) [auxpass(saved-3, is-2)]

	*/

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif

			bool stanfordPrepositionFound = false;
			string prepositionName = convertStanfordPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

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
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
										
										currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition

										string conditionTypeName = prepositionName;
										bool entityAlreadyExistant = false;

										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_HAVING_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

										GIAEntityNode * haveEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
										bool negative = haveEntity->negative;
										GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
										GIAEntityNode * conditionSubstanceNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];

										#ifdef GIA_USE_ADVANCED_REFERENCING
										bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS, currentRelationInList2);	//linkHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through having a chicken is.... therefore default same set]
										//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS;	//more precisely
										#else
										bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
										#endif

										#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
										GIAEntityNodeArray[FEATURE_INDEX_OF_HAVING_UNKNOWN] = addOrConnectHavingPropertyConditionToEntity(entityNode, conditionSubstanceNode, conditionTypeConceptEntity, negative, sameReferenceSet);
										#else
										addOrConnectHavingPropertyConditionToEntity(entityNode, conditionSubstanceNode, conditionTypeConceptEntity, negative, sameReferenceSet);
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
										
										currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition

										string conditionTypeName = prepositionName;
										bool entityAlreadyExistant = false;

										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, FEATURE_INDEX_OF_BEING_UNKNOWN, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

										GIAEntityNode * beEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
										bool negative = beEntity->negative;
										GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
										GIAEntityNode * conditionDefinitionNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];

										#ifdef GIA_USE_ADVANCED_REFERENCING
										bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS, currentRelationInList2);	//linkHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through being a chicken is.... therefore default same set]
										//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS;	//more precisely
										#else
										bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
										#endif

										#ifdef GIA_ADVANCED_REFERENCING_CONDITIONS
										GIAEntityNodeArray[FEATURE_INDEX_OF_BEING_UNKNOWN] = addOrConnectBeingDefinitionConditionToEntity(entityNode, conditionDefinitionNode, conditionTypeConceptEntity, negative, sameReferenceSet);
										#else
										addOrConnectBeingDefinitionConditionToEntity(entityNode, conditionDefinitionNode, conditionTypeConceptEntity, negative, sameReferenceSet);
										#endif

										disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(beEntity);

									}
								}
							}
							#endif

						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

//is this still used by Stanford?
void linkObjectSubjectOfPreposition(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching preposition of object-subject relationship
								
								GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
								GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
								GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex];

								#ifdef GIA_TRANSLATOR_DEBUG
								/*
								cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								cout << "currentRelationInList2->relationDependentIndex = " << currentRelationInList2->relationDependentIndex << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								cout << "currentRelationInList2->relationGovernorIndex = " << currentRelationInList2->relationGovernorIndex << endl;
								cout << "entityNode->entityName = " << entityNode->entityName << endl;
								cout << "conditionEntityNode->entityName = " << conditionEntityNode->entityName << endl;		//this is wrong
								cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								*/
								#endif
								
								//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->negative & GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->negative;

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//CHECK THIS... linkObjectSubjectOfPreposition use DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS?
								createConditionBasedUponPreposition(entityNode, conditionEntityNode, conditionTypeConceptEntity->entityName, false, currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);

								/*OLD:
								bool passedPropositionTime = false;
								if(conditionEntityNode->hasAssociatedTime)	//added 10 April 2012 to make stanford compatible
								{
									passedPropositionTime = true;
								}
								if(entityNode->hasAssociatedInstanceTemp)
								{
									entityNode = entityNode->AssociatedInstanceNodeList.back();
								}
								if(conditionEntityNode->hasAssociatedInstanceTemp)
								{
									conditionEntityNode = conditionEntityNode->AssociatedInstanceNodeList.back();
								}


								if(passedPropositionTime)
								{
									addTimeConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);

								}
								else
								{
									addOrConnectConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);
								}
								*/

							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void linkConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif
			int actionOrSubstanceIndex = currentRelationInList->relationGovernorIndex;
			int actionOrSubstanceConditionIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrSubstanceEntity = GIAEntityNodeArray[actionOrSubstanceIndex];
			GIAEntityNode * actionOrSubstanceConditionEntity = GIAEntityNodeArray[actionOrSubstanceConditionIndex];

			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			//cout << "linkConditions actionOrSubstanceEntity = " << actionOrSubstanceEntity->entityName << endl;
			//cout << "linkConditions actionOrSubstanceConditionEntity = " << actionOrSubstanceConditionEntity->entityName << endl;
			#endif
			
			bool passed = true;
			#ifdef GIA_IGNORE_MEANINGLESS_RELATIONS
			if(GIAEntityNodeArray[actionOrSubstanceConditionIndex]->entityName == relationType)
			{//to prevent meaningless relex relations; eg "on(be[2], on[6])"
				passed = false;
			}
			#endif
			#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				if(GIAEntityNodeArrayFilled[i])
				{
					if(GIAEntityNodeArray[i]->entityName == relationType)
					{
						if(GIAEntityNodeArray[i]->disableParsingAsAPrepositionRelationTemp)
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

			/*
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
			if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
			{
			#endif
			*/
			#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE
				#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_SUBSTANCE_LINK_DEPENDING_UPON_ACTION_OR_SUBSTANCE
				bool stanfordPrepositionFound = false;
				if(convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound) == RELATION_TYPE_OF)
				{
					passed = false;

					//this section was completely changed (perhaps fixed) in version 10 May 2012 - test this...
					if(actionOrSubstanceEntity->isAction)
					{
						/*Relex:
						NB not in this case "She grew tired of the pie."
						of(tired[3], pie[6])
						_to-be(grow[2], tired[3])
						_subj(grow[2], she[1])
						*/
						/*Stanford:
						prep_of(grew-2, pie-6)
						acomp(grew-2, tired-3)
						nsubj(grew-2, She-1)
						*/

						passed = true;
					}

					/*full list parse not necessarily, as only concerned about how the node has been defined within the given context/sentence
					vector<GIAEntityNode*>::iterator entityIter;
					for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++)
					{
						...
						currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);

					}
					*/

					if(!passed)
					{
						/*
						NB this case "The house of Kriton is blue." should create 2 substance connections (not just 1)
						of(house[2], Kriton[4])
						_predadj(house[2], blue[6])
						*/
						#ifdef GIA_USE_ADVANCED_REFERENCING
						bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
						#else
						bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
						#endif
						GIAEntityNodeArray[actionOrSubstanceIndex] = addOrConnectPropertyToEntityAddOnlyIfOwnerIsProperty(actionOrSubstanceConditionEntity, actionOrSubstanceEntity, sameReferenceSet);
					}
				}
				#endif
			#endif
			/*
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
			}
			#endif
			*/

			if(passed)
			{
				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				createConditionBasedUponPreposition(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, relationType, false, currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);
			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
}

bool determineFeatureIndexOfPreposition(Sentence * currentSentenceInList, string * prepositionName, int * indexOfPreposition)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "*prepositionName = " << *prepositionName << endl;
	#endif
	
	bool prepositionFeatureFound = false;
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		string singleWordPreposition = *prepositionName;
		int indexOfPrepositionDelimiter = prepositionName->rfind(STANFORD_PARSER_PREPOSITION_DELIMITER);	//find last occurance
		if(indexOfPrepositionDelimiter != string::npos)
		{
			int lengthOfSingleWordPreposition = prepositionName->length() - indexOfPrepositionDelimiter - 1;
			singleWordPreposition = prepositionName->substr(indexOfPrepositionDelimiter+1, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == prepositionName->length()-1)
			{
				cout << "determineFeatureIndexOfPreposition: illegal multiword preposition; (indexOfPrepositionDelimiter == prepositionName->length()-1)" << endl;
				cout << "prepositionName = " << *prepositionName << endl;
				exit(0);
			}
		}

		/*OLD: find first occurance; not possible as the first word in multiword prepositions has often already been assigned an entity array index, eg He rode the carriage that is near to the horse. nsubj(near-7, carriage-4) / prep_to(near-7, horse-10) -> prep_near_to(carriage-4, horse-10)
		string singleWordPreposition = *prepositionName;
		int indexOfPrepositionDelimiter = prepositionName->find(STANFORD_PARSER_PREPOSITION_DELIMITER);
		if(indexOfPrepositionDelimiter != string::npos)
		{
			int lengthOfSingleWordPreposition = indexOfPrepositionDelimiter;
			singleWordPreposition = prepositionName->substr(0, lengthOfSingleWordPreposition);
			if(indexOfPrepositionDelimiter == 0)
			{
				cout << "determineFeatureIndexOfPreposition: illegal multiword preposition; indexOfPrepositionDelimiter == 0" << endl;
				cout << "prepositionName = " << *prepositionName << endl;
				exit(0);
			}
		}
		*/

		if(currentFeatureInList->lemma == singleWordPreposition)
		{
			prepositionFeatureFound = true;
			*indexOfPreposition = currentFeatureInList->entityIndex;
		}
		currentFeatureInList = currentFeatureInList->next;
	}
	return prepositionFeatureFound;
}

void createConditionBasedUponPreposition(GIAEntityNode * actionOrSubstanceEntity, GIAEntityNode * actionOrSubstanceConditionEntity, string relationType, bool negative, Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet)
{
	/* does not cover all grounds
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	relationType = performPrepositionReduction(relationType);
	#endif
	*/

	bool stanfordPrepositionFound = false;
	string prepositionName = convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound);

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "prepositionName = " << prepositionName << endl;
	#endif
	
	bool passedPropositionTime = false;
	bool passedPropositionLocation = false;
	bool passedPropositionReasonOrCircumstances = false;
	bool passedPreposition = false;

	//parse specific relex questions: not required, as these have been added to the relationTypePreposition_NameArrays
	/*
	if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
	{
		passedPropositionTime = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE)
	{
		passedPropositionLocation = true;
	}
	else if(currentRelationInList->relationType == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY)
	{
		passedPropositionReason = true;
	}
	*/

	#ifdef GIA_ENFORCE_USE_OF_RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES
	for(int i=0; i<RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionTimeNameArray[i])
		{
			if(actionOrSubstanceConditionEntity->hasAssociatedTime)
			{
				passedPropositionTime = true;
			}
			else if(prepositionName == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
			{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
				passedPropositionTime = true;
			}
		}
	}
	#else
	if(actionOrSubstanceConditionEntity->hasAssociatedTime)
	{
		passedPropositionTime = true;
	}
	else if(prepositionName == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
	{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
		passedPropositionTime = true;
		passedPreposition = true;
	}
	#endif

	for(int i=0; i<RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionLocationNameArray[i])
		{
			if(!actionOrSubstanceConditionEntity->hasAssociatedTime)
			{//NB "at" and "on" are shared for location and time prepositions
				passedPropositionLocation = true;
			}
		}
	}
	for(int i=0; i<RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionReasonOrCircumstanceNameArray[i])
		{
			if(!actionOrSubstanceConditionEntity->hasAssociatedTime)
			{//NB in case "_" and "_" are shared for reason and time prepositions
				passedPropositionReasonOrCircumstances = true;
			}
		}
	}

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		if(prepositionName[0] != RELATION_TYPE_PREPOSITION_FIRST_CHARACTER)
		{//not valid for REFERENCE_TYPE_QUESTION_QUERY_VARIABLEs... [but this is not a problem because passedPropositionUnknown is processed last in the if/else switch below]
			//passedPropositionUnknown = true;		//removed 17 April 2012
			passedPreposition = true;
		}
	}
	else if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		if(stanfordPrepositionFound)
		{
			//passedPropositionUnknown = true;		//removed 17 April 2012
			passedPreposition = true;
		}
	}
	for(int i=0; i<RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES; i++)	//this code should only be required for 'how' qvar, because the other qvars should be detected within one of the above loops
	{
		if(prepositionName == relationTypeQVariableNameArray[i])
		{
			passedPreposition = true;
		}
	}

	if(passedPreposition)
	{
		//added 3 June 2012 for advanced referencing of prepositions
		GIAEntityNode * conditionTypeConceptEntity;
		int featureIndexOfPreposition = -1;
		bool prepositionFeatureFound = determineFeatureIndexOfPreposition(currentSentenceInList, &prepositionName, &featureIndexOfPreposition);
		if(!prepositionFeatureFound)
		{
			featureIndexOfPreposition = FEATURE_INDEX_OF_PREPOSITION_UNKNOWN;
		}

		bool entityAlreadyExistant = false;
		string conditionTypeName = prepositionName;
		conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapperCondition(GIAEntityNodeArrayFilled, GIAEntityNodeArray, featureIndexOfPreposition, &conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "prepositionFeatureFound = " << prepositionFeatureFound << endl;
		cout << "prepositionName = " << prepositionName << endl;
		cout << "featureIndexOfPreposition = " << featureIndexOfPreposition << endl;		
		//cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrSubstanceEntity = " << actionOrSubstanceEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrSubstanceConditionEntity = " << actionOrSubstanceConditionEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition conditionTypeConceptEntity = " << conditionTypeConceptEntity->entityName << endl;
		#endif
		
		if(passedPropositionTime)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
			cout << "timeConditionName = " << actionOrSubstanceConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addTimeConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#else
			addTimeConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#endif
		}
		else if(passedPropositionLocation)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
			cout << "locationConditionName = " << actionOrSubstanceConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addLocationConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#else
			addLocationConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#endif
		}
		else if(passedPropositionReasonOrCircumstances)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
			cout << "reasonConditionName = " << actionOrSubstanceConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addReasonConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#else
			addReasonConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#endif
		}
		//else if(passedPropositionUnknown)		//removed 17 April 2012
		else
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
			cout << "actionOrSubstanceConditionName = " << actionOrSubstanceConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addGenericConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#else
			addGenericConditionToEntity(actionOrSubstanceEntity, actionOrSubstanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#endif
			//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
		}

		//cout << "GIAEntityNodeArray[featureIndexOfPreposition]->entityName = " << GIAEntityNodeArray[featureIndexOfPreposition]->entityName << endl;

	}
	else if(passedPropositionTime)
	{//required for extractDatesStanfordCoreNLP

		addTimeToSubstance(actionOrSubstanceConditionEntity);
	}
}


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





void addTimeToSubstance(GIAEntityNode * timeConditionEntity)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeToSubstance timeConditionEntity->entityName = " << timeConditionEntity->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

}



/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToSubstance(GIAEntityNode * substanceNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, vector<GIATimeConditionNode*> *timeConditionNodesActiveList, vector<long> *timeConditionNumbersActiveList)
#else
void addTimeConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
#endif
*/
GIAEntityNode * addTimeConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "addTimeConditionToEntity substanceNode->entityName = " << substanceNode->entityName << endl;
	#endif
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;

	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->entityName);
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/

	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();

	timeConditionEntity->timeConditionNode = newTimeCondition;

	return addOrConnectConditionToEntity(substanceNode, timeConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

GIAEntityNode * addLocationConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * locationConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{
	locationConditionEntity->conditionType = CONDITION_NODE_TYPE_LOCATION;

	return addOrConnectConditionToEntity(substanceNode, locationConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

GIAEntityNode * addReasonConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * reasonConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{
	reasonConditionEntity->conditionType = CONDITION_NODE_TYPE_REASON;

	return addOrConnectConditionToEntity(substanceNode, reasonConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}


GIAEntityNode * addGenericConditionToEntity(GIAEntityNode * substanceNode, GIAEntityNode * substanceConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{
	//timeConditionEntity->conditionType = CONDITION_NODE_TYPE_UNDEFINED;

	return addOrConnectConditionToEntity(substanceNode, substanceConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

