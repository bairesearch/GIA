/*******************************************************************************
 *
 * File Name: GIATranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j8a 10-May-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
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
				//cout << "RELATION_TYPE_POSSESSIVE" << endl;

				string propertyName = currentRelationInList->relationGovernor; 
				string ownerName = currentRelationInList->relationDependent; 
				int propertyIndex = currentRelationInList->relationGovernorIndex;
				int ownerIndex = currentRelationInList->relationDependentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[ownerIndex];
				//cout << "propertyName = " << propertyEntity->entityName << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;

				GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity);
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
				bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
				bool passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAEntityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);

				if(passed2 && passed3)
				{
					//create a new property for the entity and assign a property definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string propertyName = currentRelationInList->relationDependent; 
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int propertyIndex = currentRelationInList->relationDependentIndex;				
					GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

					#ifdef GIA_TRANSLATOR_DEBUG
					cout << "thingEntity = " << thingEntity->entityName << endl;
					cout << "propertyEntity = " << propertyEntity->entityName << endl;
					#endif

					GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(thingEntity, propertyEntity);		
				}			
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif				
		currentRelationInList = currentRelationInList->next;
	}		
}			

void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif		
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				string propertyName = currentRelationInList->relationGovernor; 
				string entityName = currentRelationInList->relationDependent; 
				int propertyIndex = currentRelationInList->relationGovernorIndex;
				int definitionIndex = currentRelationInList->relationDependentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];
				GIAEntityNode * definitionEntity = GIAEntityNodeArray[definitionIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "propertyName = " << propertyEntity->entityName << endl;
				cout << "entityName = " << definitionEntity->entityName << endl;
				#endif

				addDefinitionToEntity(propertyEntity, definitionEntity);									
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif				
		currentRelationInList = currentRelationInList->next;
	}
}


void defineSubjectOrObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
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
			subjectObjectEntity = GIAEntityNodeArray[relationDependentIndex];
			subjectObjectFunctionEntity = GIAEntityNodeArray[relationGovernorIndex]; 
			
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

				//cout << "subjectObjectName = " << subjectObjectName << endl;

				if(!(currentRelationInList->subjObjRelationAlreadyAdded))
				{//add independent action if appropriate
					//cout << "subjectObjectName = " << subjectObjectName << endl;

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
						//cout << "2 actionName = " << actionName << endl;
						GIAEntityNode * actionEntity = GIAEntityNodeArray[relationGovernorIndex];


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
										//cout << "ASD" << endl;

										GIAEntityNode * actionOrPropertyConditionEntity;
										if(passsubject && (subjectObjectEntity->entityName == currentRelationInList3->relationDependent))
										{//subject is connected to an _advmod
											/*eg 1 Space is saved by running fast.
											_obj(save[3], space[1])	[IRRELEVANT]
											_advmod(save[3], by[4])
											_advmod(run[5], fast[6]) [IRRELEVANT]
											_subj(run[5], by[4])											
											*/
											actionOrPropertyConditionEntity = subjectObjectFunctionEntity;
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
											actionOrPropertyConditionEntity = subjectObjectEntity;
											subjectOrObjectIsConnectedToAnAdvMod = true;
										}
										else
										{

										}

										if(subjectOrObjectIsConnectedToAnAdvMod)
										{

											GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex];
											GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];


											#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
											conditionTypeConceptEntity->disableParsingAsAPrepositionRelationTemp = true;		//Added 30 Oct 2011a
											#endif

											//cout << "actionOrPropertyConditionEntity= " << actionOrPropertyConditionEntity->entityName << endl;

											addActionToActionDefinition(actionOrPropertyConditionEntity);

											//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
											//cout << "actionOrPropertyEntity = " << actionOrPropertyEntity->entityName << endl;
											//cout << "conditionTypeConceptEntity = " << conditionTypeConceptEntity->entityName << endl;

											addOrConnectPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);
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
							/*
							cout << "relationGovernorIndex = " << relationGovernorIndex << endl;	
							cout << "relationDependentIndex = " << relationDependentIndex << endl;
							cout << "subjectObjectName = " << subjectObjectName << endl;
							cout << "subjectObjectEntity->entityName = " << subjectObjectEntity->entityName << endl;
							cout << "subjectObjectFunctionEntity->entityName = " << subjectObjectFunctionEntity->entityName << endl;
							*/
							
							if(passsubject)
							{//fired by joe..???? [is there a possible example of this?]

								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * subjectEntityTemp = subjectObjectEntity;
								//cout << "vf1" << endl;
								addActionToSubject(subjectObjectEntity, actionEntity);	
								//cout << "vf2" << endl;
							}
							else if(passobject)
							{//eg the bow was fired

								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * objectEntityTemp = subjectObjectEntity;

								//cout << "vf3" << endl;
								addActionToObject(objectEntityTemp, actionEntity);
								//cout << "vf4" << endl;
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

void defineSubjectObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
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
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

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
						bool partnerTypeObjectSpecialConditionToDoPropertyFound = false;
						bool partnerTypeObjectSpecialConditionToBePropertyFound = false;

						for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
							{
								passed2 = true;
							}
						}		
						for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionMeasureDistanceOrStanfordUnknownNameArray[i])
							{
								passed2 = true;
								partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound = true;
								/*
								cout << "partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound" << endl;
								cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
								cout << "currentRelationInList2->relationDependent = " << currentRelationInList2->relationDependent << endl;
								cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
								cout << "currentRelationInList2->relationGovernor = " << currentRelationInList2->relationGovernor << endl;						
								*/
							}
						}		

						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
						if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
						{
						#endif	
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_TO_DO_PROPERTY_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionToDoPropertyNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionToDoPropertyFound = true;
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
							#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_PROPERTY_LINK_AND_ACTION_DEFINITION
							for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_TO_BE_PROPERTY_NUMBER_OF_TYPES; i++)						
							{
								if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionToBePropertyNameArray[i])
								{
									passed2 = true;
									partnerTypeObjectSpecialConditionToBePropertyFound = true;
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
								//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								
								int objectEntityIndexTemp = subjectObjectEntityIndexArray[OBJECT_INDEX];
								int subjectEntityIndexTemp = subjectObjectEntityIndexArray[SUBJECT_INDEX];								
								GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
								GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

								if(relationGovernorIndex == relationGovernorIndex2)
								{//found a matching object-subject relationship

									//cout << "\n\n relationGovernorIndex2 == relationGovernorIndex2 " << endl;
									//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;	

									//cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;	
									//cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;


									//find out if the subject is connected to an _advmod, if so create a dummy entity as the subject, and assign the subject as a condition [instead of forming default subject-action-object relationship]
									bool subjectIsConnectedToAnAdvMod = false;
									Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
									while(currentRelationInList3->next != NULL)
									{
										#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
										if(!(currentRelationInList3->disabled))
										{			
										#endif										
											if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
											{

												#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
												if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
												{
												#endif									
													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
													if(subjectEntityTemp->entityName == currentRelationInList3->relationDependent)
													{//subject is connected to an _advmod

														GIAEntityNode * actionOrPropertyConditionEntity;
														GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex];
														GIAEntityNode * conditionTypeConceptEntity = subjectEntityTemp;										

														subjectIsConnectedToAnAdvMod = true;

														//cout << "here15" << endl;

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
															actionOrPropertyConditionEntity = objectEntityTemp;
															addOrConnectBeingDefinitionConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, negative);
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
														else if(passcomposition)
														{
															bool negative = subjectObjectFunctionEntityArray[SUBJECT_INDEX]->negative;
															subjectIsConnectedToAnAdvMod = true;
															actionOrPropertyConditionEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
															addOrConnectHavingPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, negative);
														}
														#endif
														else
														{
															//standard action/property condion (ie action condition in this context)
															actionOrPropertyConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
															addActionToActionDefinition(actionOrPropertyConditionEntity);	//not required is done later?
															addOrConnectPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);

															GIAEntityNode * actionEntity = actionOrPropertyConditionEntity;
															addActionToObject(objectEntityTemp, actionEntity);																			
														}									

													#else

														#ifdef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
														cout << "error: ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION not yet coded; in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing" << endl;
														#else
														//create arbitrarySubjectSpecialConceptEntityNode
														string arbitrarySubjectSpecialConceptEntityNodeName = ARBITRARY_SUBJECT_SPECIAL_CONCEPT_NODE_NAME;
														long entityIndex = -1;
														bool entityAlreadyExistant = false;		
														vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();	
														long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
														long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();																			
														GIAEntityNode * arbitrarySubjectSpecialConceptEntityNode = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &arbitrarySubjectSpecialConceptEntityNodeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
														if(entityAlreadyExistant)
														{
															applyEntityAlreadyExistsFunction(arbitrarySubjectSpecialConceptEntityNode);				
														}
														subjectEntityTemp = arbitrarySubjectSpecialConceptEntityNode;		//overwrites subjectEntityTemp
														#endif

														if(0)
														{

														}
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
														else if(passdefinition)
														{
															actionOrPropertyConditionEntity = arbitrarySubjectSpecialConceptEntityNode;
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														else if(passcomposition)
														{
															actionOrPropertyConditionEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
														}
														#endif
														else
														{
															actionOrPropertyConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
															addActionToActionDefinition(actionOrPropertyConditionEntity);	//not required is done later?
														}

														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														passdefinition = false;		//treat "being" as an action
														#endif
														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														passpasscomposition = false;		//treat "being" as an action
														#endif

														addOrConnectPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);
													#endif	

													}
													#endif
												#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
												}
												#endif									

												#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
												if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
												{
												#endif								
													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
													//cout << "qsd0" << endl;
													if(currentRelationInList3->relationGovernor == RELATION_ENTITY_BE)
													{//subject is connected to an _advmod

														//cout << "qsd1" << endl;

														if(passdefinition)
														{
															//cout << "qsd2" << endl;

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
																int propertyIndex = currentRelationInList3->relationDependentIndex;
																GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];

																GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(baseEntity, propertyEntity);			
																GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(propertyEntity, objectEntityTemp);

																#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
																#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
																disableEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex]);
																#endif
																#endif

															#else

																cout << "warning: GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_... defined - source may require review" << endl;

																GIAEntityNode * baseEntity = subjectEntityTemp;
																GIAEntityNode * definitionEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];
																
																int propertyIndex = objectEntityIndexTemp;
																GIAEntityNode * propertyEntity = objectEntityTemp;

																addDefinitionToEntity(baseEntity, definitionEntity);

																GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(definitionEntity, propertyEntity);	

															#endif											

														}
													}
													#endif
												#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
												}
												#endif	
											}								

										#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
										}			
										#endif	
										currentRelationInList3 = currentRelationInList3->next;
									}	


									#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
									if(!subjectIsConnectedToAnAdvMod)
									//cout << "h-2" << endl;
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

											//cout << "here: objectEntityTemp = " << objectEntityTemp->entityName << endl;
											//cout << "here: subjectEntityTemp = " << subjectEntityTemp->entityName << endl;

											//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
												//NB definitions are only assigned to entities, not properties (instances of entities)

											
											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
											if(currentSentenceInList->isQuestion == true)
											#else
											if(subjectEntityTemp->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
											#endif
											{//this section of code is only used for RelEx: NB Stanford CoreNLP ouputs "what is"/"who is" queries in appos format (not in obj/subj format)
											#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY									
												//switch object/subject variables [transform question into answer form]
												addDefinitionToEntity(objectEntityTemp, subjectEntityTemp);
											#else

												//added 20 October 2011 [what is the time?]
												GIAEntityNode * actionOrPropertyEntity = objectEntityTemp;				
												GIAEntityNode * actionOrPropertyConditionEntity = subjectEntityTemp;
												GIAEntityNode * conditionTypeConceptEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												addOrConnectPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);
											#endif	
											}
											else
											{
												//cout << "h2" << endl;
												addDefinitionToEntity(subjectEntityTemp, objectEntityTemp);
											}

											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure	
										}
										#endif
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
										//NB this case may be redundant because Relex automatical interprets these cases as preadj [eg _predadj(tom[1], happy[3])]
										//else if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_1) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_2) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_3))
										else if(passcomposition)
										{//subject-object relationship is a composition [property]
											GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(subjectEntityTemp, objectEntityTemp);
												//check can use properties for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
											//cout << "a" << endl;

											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										else if(partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound)
										{								
											#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
												//eg The rabbit is 20 meters away.	[away is a property of rabbit, not a condition of rabbit]
												GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;	//aka subjectObjectEntityArray[SUBJECT_INDEX];
												int propertyIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
												GIAEntityNode * propertyEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(subjectEntityOrProperty, propertyEntity);

											#else
												//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a property of rabbit]

												GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
												GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationGovernorIndex2];
												//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
												//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;	

												string conditionTypeName = "specialCondition";
												long entityIndex = -1;
												bool entityAlreadyExistant = false;	
												vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();	
												long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
												long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();																	
												GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);
												if(entityAlreadyExistant)
												{
													applyEntityAlreadyExistsFunction(arbitrarySubjectSpecialConceptEntityNode);					
												}

												addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity);

											#endif								
										}
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
										else if(partnerTypeObjectSpecialConditionToDoPropertyFound)
										{

										}
										#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_PROPERTY_LINK_AND_ACTION_DEFINITION
										else if(partnerTypeObjectSpecialConditionToBePropertyFound)
										{

											//cout << "c" << endl;
											/* 
											Eg;	 The pastry tasted awesome.							
											_to-be(taste[3], awesome[4])
											_subj(taste[3], pastry[2])											
											*/

											//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
											GIAEntityNode * objectEntityTempUpdated = subjectObjectEntityArray[SUBJECT_INDEX];
											GIAEntityNode * actionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

											addActionToObject(objectEntityTempUpdated, actionEntity);

											//create a property link between the subject and object
											GIAEntityNode * propertyEntity = subjectObjectEntityArray[OBJECT_INDEX];
											int propertyIndex = subjectObjectEntityIndexArray[OBJECT_INDEX];
											GIAEntityNode * ownerEntity = actionEntity;
											//cout << "propertyName = " << propertyEntity->entityName << endl;
											//cout << "ownerName = " << ownerEntity->entityName << endl;

											GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity);

										}	
										#endif								
										else
										{//assume that the subject-object relationships is an action
											string actionName = currentRelationInList->relationGovernor;
											//cout << "1 actionName = " << actionName << endl;
											GIAEntityNode * actionEntity = GIAEntityNodeArray[relationGovernorIndex];

											//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)

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

											addActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity);
										}
									}

									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
									{
									#endif						
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
										if(!partnerTypeObjectSpecialConditionToDoPropertyFound || subjectIsConnectedToAnAdvMod)
										{
											foundPartner = true;
											currentRelationInList->subjObjRelationAlreadyAdded = true;
											currentRelationInList2->subjObjRelationAlreadyAdded = true;
											//cout << "a" << endl;								
										}
										#else
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;								
										//cout << "b" << endl;
										#endif							
									#else
										foundPartner = true;
										currentRelationInList->subjObjRelationAlreadyAdded = true;
										currentRelationInList2->subjObjRelationAlreadyAdded = true;								
										//cout << "c" << endl;
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
												//cout << "a" << endl;
												Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
												while(currentRelationInList3->next != NULL)
												{
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
													if(!(currentRelationInList3->disabled))
													{			
													#endif													
														if(currentRelationInList3->relationGovernor == RELATION_ENTITY_BE)
														{																						
															//cout << "a2" << endl;
															if((currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD))	//OR if (currentRelationInList3->relationType == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
															{
																//cout << "b" << endl;
																if(currentRelationInList3->relationDependent == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
																{	
																	//cout << "c" << endl;
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

																	GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
																	GIAEntityNode * specialConditionNode = objectEntityTemp;


																	//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList3->relationGovernorIndex]->negative & GIAEntityNodeArray[currentRelationInList3->relationDependentIndex]->negative;

																	GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationDependentIndex];

																	#ifdef GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
																	conditionTypeConceptEntity->disableParsingAsAPrepositionRelationTemp = true;		//Added 30 Oct 2011a
																	#endif

																	/*
																	cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
																	cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;
																	cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
																	*/

																	addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity);														

																	foundPartner = true;
																	currentRelationInList->subjObjRelationAlreadyAdded = true;
																	currentRelationInList2->subjObjRelationAlreadyAdded = true;
																	//cout << "d" << endl;														
																}
															}
														}
													#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
													}		
													#endif		

													//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
													currentRelationInList3 = currentRelationInList3->next;											
												}
											}
										}
										#endif
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
										//[search for special relation _to-do AND _subj, and if so create a property link between subject and object] 
										if(!foundPartner)
										{//do not overwrite usage of subj/obj if a direct link [ie action] has been found (this condition probably/may not be required)

											if(partnerTypeObjectSpecialConditionToDoPropertyFound)
											{
												//cout << "b" << endl;
												if(subjectObjectFunctionEntityArray[SUBJECT_INDEX]->entityName == subjectObjectEntityArray[OBJECT_INDEX]->entityName)
												{
													//cout << "c" << endl;
													/* 
													Eg;	Luke is slow.							
													_to-do(be[2], slow[3])
													_subj(slow[3], Luke[1])												
													*/

													//create a property link between the object and subject
													int propertyIndex = objectEntityIndexTemp;
													GIAEntityNode * propertyEntity = objectEntityTemp;
													GIAEntityNode * ownerEntity = subjectEntityTemp;
													//cout << "propertyName = " << propertyEntity->entityName << endl;
													//cout << "ownerName = " << ownerEntity->entityName << endl;

													GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity);

													foundPartner = true;
													currentRelationInList->subjObjRelationAlreadyAdded = true;
													currentRelationInList2->subjObjRelationAlreadyAdded = true;
													//cout << "e" << endl;
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



void defineIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
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
								//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

								int propertyIndex = currentRelationInList2->relationDependentIndex;
								int thingIndex = currentRelationInList->relationDependentIndex;
								GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];
								GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];

								GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(thingEntity, propertyEntity);
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



void defineObjectSubjectOfPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
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
								/*
								cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								cout << "currentRelationInList2->relationDependentIndex = " << currentRelationInList2->relationDependentIndex << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								cout << "currentRelationInList2->relationGovernorIndex = " << currentRelationInList2->relationGovernorIndex << endl;
								*/

								GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
								GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];						
								GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex];

								/*
								cout << "entityNode->entityName = " << entityNode->entityName << endl;
								cout << "conditionEntityNode->entityName = " << conditionEntityNode->entityName << endl;		//this is wrong
								cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
								cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
								*/

								//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->negative & GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->negative;

								createConditionBasedUponPreposition(entityNode, conditionEntityNode, conditionTypeConceptEntity->entityName, false, conceptEntityNodesList, NLPdependencyRelationsType);

								/*
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
									addTimeConditionToProperty(entityNode, conditionEntityNode, conditionTypeConceptEntity);				

								}
								else
								{
									addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);							
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
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}


void defineActionPropertyConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif
			int actionOrPropertyIndex = currentRelationInList->relationGovernorIndex;
			int actionOrPropertyConditionIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[actionOrPropertyIndex];				
			GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[actionOrPropertyConditionIndex];

			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;	      
			//cout << "defineActionPropertyConditions actionOrPropertyEntity = " << actionOrPropertyEntity->entityName << endl;
			//cout << "defineActionPropertyConditions actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;

			bool passed = true;
			#ifdef GIA_IGNORE_MEANINGLESS_RELATIONS
			if(GIAEntityNodeArray[actionOrPropertyConditionIndex]->entityName == relationType)
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

			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
			if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
			{
			#endif
				#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_PROPERTY_LINK_DEPENDING_UPON_ACTION_OR_PROPERTY
				bool stanfordPrepositionFound = false;
				if(convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound) == RELATION_TYPE_OF)
				{
					passed = false;

					//cout << "a" << endl;

					//this section was completely changed (perhaps fixed) in version 10 May 2012 - test this...
					//cout << "b" << endl;
					if(actionOrPropertyEntity->isAction)
					{
						/*
						NB not in this case "She grew tired of the pie." 
						of(tired[3], pie[6])
						_to-be(grow[2], tired[3])
						_subj(grow[2], she[1])
						*/

						//cout << "c" << endl;
						passed = true;
					}
							
					/*full list parse not necessarily, as only concerned about how the node has been defined within the given context/sentence
					vector<GIAEntityNode*>::iterator entityIter;				
					for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++) 
					{
						...
						//cout << "a32" << endl;	
						currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);

					}
					*/

					if(!passed)
					{
						/*
						NB this case "The house of Kriton is blue." should create 2 property connections (not just 1)
						of(house[2], Kriton[4])
						_predadj(house[2], blue[6])
						*/				
						GIAEntityNodeArray[actionOrPropertyIndex] = addOrConnectPropertyToEntity(actionOrPropertyConditionEntity, actionOrPropertyEntity);			
					}			
				}		
				#endif
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
			}
			#endif

			if(passed)
			{
				createConditionBasedUponPreposition(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType, false, conceptEntityNodesList, NLPdependencyRelationsType);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		
		currentRelationInList = currentRelationInList->next;
	}
}


void createConditionBasedUponPreposition(GIAEntityNode * actionOrPropertyEntity, GIAEntityNode * actionOrPropertyConditionEntity, string relationType, bool negative, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{	
	/* does not cover all grounds	
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	relationType = performPrepositionReduction(relationType);
	#endif
	*/
	
	bool stanfordPrepositionFound = false;
	string prepositionName = convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound);
	
	//cout << "prepositionName = " << prepositionName << endl;
	
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
			if(actionOrPropertyConditionEntity->hasAssociatedTime)
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
	if(actionOrPropertyConditionEntity->hasAssociatedTime)
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
			if(!actionOrPropertyConditionEntity->hasAssociatedTime)	
			{//NB "at" and "on" are shared for location and time prepositions
				passedPropositionLocation = true;
			}
		}
	}
	for(int i=0; i<RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionReasonOrCircumstanceNameArray[i])
		{
			if(!actionOrPropertyConditionEntity->hasAssociatedTime)	
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
	//cout << "prepositionName = " << prepositionName << endl;

		
	if(passedPreposition)
	{

		//cout << "prepositionName = " << prepositionName << endl;
		
		string conditionTypeName = prepositionName;
		long entityIndex = -1;
		bool entityAlreadyExistant = false;
		//cout << "prepositionName = " << prepositionName << endl;
		vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
		long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
		long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();		
		GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);	
		if(entityAlreadyExistant)
		{
			applyEntityAlreadyExistsFunction(conditionTypeConceptEntity);
		}		
		//cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl; 
	
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrPropertyEntity = " << actionOrPropertyEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
		//cout << "createConditionBasedUponPreposition passedPreposition conditionTypeConceptEntity = " << conditionTypeConceptEntity->entityName << endl;
			
		if(passedPropositionTime)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "timeConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			addTimeConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);				
		}
		else if(passedPropositionLocation)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "locationConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			addLocationConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);	
		}		
		else if(passedPropositionReasonOrCircumstances)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "reasonConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			addReasonConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);		
		}
		//else if(passedPropositionUnknown)		//removed 17 April 2012
		else
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "actionOrPropertyConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);				
			//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
		}	

	}
	else if(passedPropositionTime)
	{//required for extractDatesStanfordCoreNLP 
					
		addTimeToProperty(actionOrPropertyConditionEntity);	
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
		//cout << "prepositionTypeNumberOfVariations = " << prepositionTypeNumberOfVariations << endl;
		for(int j=0; j<prepositionTypeNumberOfVariations; j++)
		{	
			if(relationType == relationTypePropositionReductionNameArray[i][j])
			{
				//cout << "matched preposition; " << relationType << endl;
				relationTypeModified = relationTypePropositionReductionReplacementNamesArray[i];
			}
		}
	}
	
	return relationTypeModified;
}
#endif
	




void addTimeToProperty(GIAEntityNode * timeConditionEntity)
{	
	//cout << "addTimeConditionToProperty propertyNode->entityName = " << propertyNode->entityName << endl;
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	timeConditionEntity->timeConditionNode = newTimeCondition;	
	
}



/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
#endif
*/
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{	
	//cout << "addTimeConditionToProperty propertyNode->entityName = " << propertyNode->entityName << endl;
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;
	
	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = calculateTotalTimeInSeconds(timeConditionEntity->entityName);
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	timeConditionEntity->timeConditionNode = newTimeCondition;	
	
	addConditionToProperty(propertyNode, timeConditionEntity, conditionTypeConceptEntity);
}

void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{	
	locationConditionEntity->conditionType = CONDITION_NODE_TYPE_LOCATION;
	
	addConditionToProperty(propertyNode, locationConditionEntity, conditionTypeConceptEntity);
}

void addReasonConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * reasonConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{	
	reasonConditionEntity->conditionType = CONDITION_NODE_TYPE_REASON;
	
	addConditionToProperty(propertyNode, reasonConditionEntity, conditionTypeConceptEntity);
}


void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{
	//timeConditionEntity->conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	
	addConditionToProperty(propertyNode, propertyConditionEntity, conditionTypeConceptEntity);
}

