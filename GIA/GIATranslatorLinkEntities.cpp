/*******************************************************************************
 *
 * File Name: GIATranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
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

			string propertyName = currentRelationInList->relationFunction; 
			string ownerName = currentRelationInList->relationArgument; 
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

			GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
			GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationArgumentIndex];
			//cout << "propertyName = " << propertyEntity->entityName << endl;
			//cout << "ownerName = " << ownerEntity->entityName << endl;

			addOrConnectPropertyToEntity(ownerEntity, propertyEntity);
		}			
		currentRelationInList = currentRelationInList->next;
	}
}


void linkPropertiesDescriptiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{	
		bool passed = false;
		for(int i=0; i<RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == relationTypeAdjectiveNameArray[i])
			{
				passed = true;
			}
		}						
		//if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_1) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
		if(passed)
		{				
			bool passed2 = isAdjectiveNotConnectedToObjectOrSubject(currentSentenceInList, currentRelationInList, NLPdependencyRelationsType);
			bool passed3 = isAdjectiveNotAnAdvmodAndRelationFunctionIsNotBe(currentRelationInList, GIAEntityNodeArray, currentRelationInList->relationFunctionIndex, NLPdependencyRelationsType);

			if(passed2 && passed3)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				string thingName = currentRelationInList->relationFunction;
				string propertyName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];

				#ifdef GIA_TRANSLATOR_DEBUG
				cout << "thingEntity = " << thingEntity->entityName << endl;
				cout << "propertyEntity = " << propertyEntity->entityName << endl;
				#endif
				
				addOrConnectPropertyToEntity(thingEntity, propertyEntity);		
			}			
		}			
		currentRelationInList = currentRelationInList->next;
	}		
}			

void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
		{
			string propertyName = currentRelationInList->relationFunction; 
			string entityName = currentRelationInList->relationArgument; 
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				

			GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
			GIAEntityNode * definitionEntity = GIAEntityNodeArray[relationArgumentIndex];

			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "propertyName = " << propertyEntity->entityName << endl;
			cout << "entityName = " << definitionEntity->entityName << endl;
			#endif

			addDefinitionToEntity(propertyEntity, definitionEntity);									
		}			
		currentRelationInList = currentRelationInList->next;
	}
}

void disableEntityAndInstance(GIAEntityNode * GIAEntityNode)
{
	cout << "GIAEntityNode->disabled = " << GIAEntityNode->entityName << endl;
	GIAEntityNode->disabled = true;		//remove redundant 'be' artefacts 

	if(GIAEntityNode->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the current context (eg sentence)
	//if(GIAEntityNode->AssociatedInstanceNodeList.size() >= 1)
	{		
		GIAEntityNode->AssociatedInstanceNodeList.back()->disabled = true;	//and disable their associated instances (property nodes)
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
		int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
		int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
		subjectObjectName = currentRelationInList->relationArgument;
		subjectObjectEntity = GIAEntityNodeArray[relationArgumentIndex];
		subjectObjectFunctionEntity = GIAEntityNodeArray[relationFunctionIndex]; 	

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
		for(int i=0; i<RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationFunction == relationFunctionCompositionNameArray[i])
			{
				passcomposition = true;
			}
		}

		bool passdefinition = false;
		for(int i=0; i<RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationFunction == relationFunctionDefinitionNameArray[i])
			{
				passdefinition = true;
			}
		}
		
		if(passed)
		{					
			if(!(currentRelationInList->subjObjRelationAlreadyAdded))
			{//add independent action if appropriate
				//cout << "subjectObjectName = " << subjectObjectName << endl;

				if(passdefinition)
				//if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
				{
				}
				//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
				else if(passcomposition)
				{
				}
				else
				{//assume that the subject-object relationships is an action
					string actionName = currentRelationInList->relationFunction;
					//cout << "2 actionName = " << actionName << endl;
					GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];


					bool subjectOrObjectIsConnectedToAnAdvMod = false;
					
					#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
					if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
					{
					#endif					
						#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
						//find out if the subject is connected to an _advmod, if so assign it as an action condition instead of a subject+action
						Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList3->next != NULL)
						{
							if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_3)
							{
								//cout << "ASD" << endl;

								GIAEntityNode * actionOrPropertyConditionEntity;
								if(passsubject && (subjectObjectEntity->entityName == currentRelationInList3->relationArgument))
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
								else if(passobject && (subjectObjectFunctionEntity->entityName == currentRelationInList3->relationArgument))
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

									GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[currentRelationInList3->relationFunctionIndex];
									GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationArgumentIndex];


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
							currentRelationInList3 = currentRelationInList3->next;
						}
						#endif
					#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
					}
					#endif						
					
					if(!subjectOrObjectIsConnectedToAnAdvMod)
					{
					
						if(passsubject)
						{//fired by joe..???? [is there a possible example of this?]

							//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
							GIAEntityNode * subjectEntityTemp = subjectObjectEntity;

							addActionToSubject(subjectObjectEntity, actionEntity);	

						}
						else if(passobject)
						{//eg the bow was fired

							//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
							GIAEntityNode * objectEntityTemp = subjectObjectEntity;

							addActionToObject(objectEntityTemp, actionEntity);
						}

					}
				}
			}
		}
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
					
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
		int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
		subjectObjectName[SUBJECT_INDEX] = currentRelationInList->relationArgument;
		subjectObjectEntityArray[SUBJECT_INDEX] = GIAEntityNodeArray[relationArgumentIndex];
		subjectObjectFunctionEntityArray[SUBJECT_INDEX] = GIAEntityNodeArray[relationFunctionIndex]; 	
						
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
		for(int i=0; i<RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationFunction == relationFunctionCompositionNameArray[i])
			{
				passcomposition = true;
			}
		}

		bool passdefinition = false;
		for(int i=0; i<RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationFunction == relationFunctionDefinitionNameArray[i])
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
				int relationFunctionIndex2 = currentRelationInList2->relationFunctionIndex;
				int relationArgumentIndex2 = currentRelationInList2->relationArgumentIndex;
				subjectObjectName[OBJECT_INDEX] = currentRelationInList2->relationArgument;
				subjectObjectEntityArray[OBJECT_INDEX] = GIAEntityNodeArray[relationArgumentIndex2];
				subjectObjectFunctionEntityArray[OBJECT_INDEX] = GIAEntityNodeArray[relationFunctionIndex2]; 	
									
				bool passed2 = false;
				bool partnerTypeObjectSpecialConditionMeasureDistanceFound = false;
				bool partnerTypeObjectSpecialConditionToDoPropertyFound = false;
				bool partnerTypeObjectSpecialConditionToBePropertyFound = false;

				for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
					{
						passed2 = true;
					}
				}		
				for(int i=0; i<RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList2->relationType == relationTypeObjectSpecialConditionMeasureDistanceNameArray[i])
					{
						passed2 = true;
						partnerTypeObjectSpecialConditionMeasureDistanceFound = true;
					}
				}		

				#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
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
				if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
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
					if(subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] != true)
					{
						subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] = true;
					#else
					if(!((currentRelationInList->subjObjRelationAlreadyAdded) && (currentRelationInList2->subjObjRelationAlreadyAdded)))
					{//do not use a subj-obj pair if the same subj or obj has already been used in the generation of another pair		
					#endif
						//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

						GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
						GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

						if(relationFunctionIndex == relationFunctionIndex2)
						{//found a matching object-subject relationship

							//cout << "\n\n relationFunctionIndex2 == relationFunctionIndex2 " << endl;
							//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;	

							//cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;	
							//cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;
							
							
							//find out if the subject is connected to an _advmod, if so create a dummy entity as the subject, and assign the subject as a condition [instead of forming default subject-action-object relationship]
							bool subjectIsConnectedToAnAdvMod = false;
							Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								if(currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_3)
								{
								
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
									{
									#endif									
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
										if(subjectEntityTemp->entityName == currentRelationInList3->relationArgument)
										{//subject is connected to an _advmod

											GIAEntityNode * actionOrPropertyConditionEntity;
											GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[currentRelationInList3->relationFunctionIndex];
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
											else if(passdefinition || partnerTypeObjectSpecialConditionMeasureDistanceFound)
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
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
									{
									#endif								
										#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
										//cout << "qsd0" << endl;
										if(currentRelationInList3->relationFunction == RELATION_FUNCTION_DEFINITION_1)
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
													GIAEntityNode * propertyEntity = GIAEntityNodeArray[currentRelationInList3->relationArgumentIndex];

													addOrConnectPropertyToEntity(baseEntity, propertyEntity);			
													addOrConnectPropertyToEntity(propertyEntity, objectEntityTemp);

													#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
													#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
													disableEntityAndInstance(GIAEntityNodeArray[currentRelationInList3->relationFunctionIndex]);
													#endif
													#endif

												#else

													cout << "warning: GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_... defined - source may require review" << endl;

													GIAEntityNode * baseEntity = subjectEntityTemp;
													GIAEntityNode * definitionEntity = GIAEntityNodeArray[currentRelationInList3->relationArgumentIndex];
													GIAEntityNode * propertyEntity = objectEntityTemp;

													addDefinitionToEntity(baseEntity, definitionEntity);

													addOrConnectPropertyToEntity(definitionEntity, propertyEntity);	

												#endif											

											}
										}
										#endif
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									}
									#endif									
									
								}
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
								//if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
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
									{
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
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
									GIAEntityNodeArray[currentRelationInList->relationFunctionIndex]->disabled = true;	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure	
									#endif									
								}
								#endif
								#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
								//NB this case may be redundant because Relex automatical interprets these cases as preadj [eg _predadj(tom[1], happy[3])]
								//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
								else if(passcomposition)
								{//subject-object relationship is a composition [property]
									addOrConnectPropertyToEntity(subjectEntityTemp, objectEntityTemp);
										//check can use properties for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
									//cout << "a" << endl;
										
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
									GIAEntityNodeArray[currentRelationInList->relationFunctionIndex]->disabled = true;	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure										
									#endif
								}
								#endif
								else if(partnerTypeObjectSpecialConditionMeasureDistanceFound)
								{
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
									{
									#endif									
									#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
										//eg The rabbit is 20 meters away.	[away is a property of rabbit, not a condition of rabbit]
										GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;	//aka subjectObjectEntityArray[SUBJECT_INDEX];
										GIAEntityNode * propertyEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

										addOrConnectPropertyToEntity(subjectEntityOrProperty, propertyEntity);
																												
									#else
										//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a property of rabbit]
									
										GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
										GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationFunctionIndex2];
										//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
										//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;	

										string conditionTypeName = "specialCondition";
										long entityIndex = -1;
										bool entityAlreadyExistant = false;	
										vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();	
										long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
										long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();																	
										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);

										addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity);

									#endif
									#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
									}
									else
									{
										//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a property of rabbit]
									
										GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
										GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationFunctionIndex2];
										//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
										//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;	

										string conditionTypeName = "specialCondition";
										long entityIndex = -1;
										bool entityAlreadyExistant = false;			
										vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();		
										long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
										long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();														
										GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);

										addOrConnectPropertyConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity);								
									}
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
									GIAEntityNode * ownerEntity = actionEntity;
									//cout << "propertyName = " << propertyEntity->entityName << endl;
									//cout << "ownerName = " << ownerEntity->entityName << endl;

									addOrConnectPropertyToEntity(ownerEntity, propertyEntity);
													
								}	
								#endif								
								else
								{//assume that the subject-object relationships is an action
									string actionName = currentRelationInList->relationFunction;
									//cout << "1 actionName = " << actionName << endl;
									GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];

									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)

									/*
									cout << "SUBJECT_INDEX = " << SUBJECT_INDEX << endl;
									cout << "OBJECT_INDEX = " << OBJECT_INDEX << endl;									
									cout << "subjectObjectName[SUBJECT_INDEX] = " << subjectObjectName[SUBJECT_INDEX] << endl;
									cout << "subjectObjectName[OBJECT_INDEX] = " << subjectObjectName[OBJECT_INDEX] << endl;
									cout << "subjectEntityTemp->entityName = " << subjectEntityTemp->entityName << endl;																	
									cout << "objectEntityTemp->entityName = " << objectEntityTemp->entityName << endl;
									cout << "relationArgumentIndex = " << relationArgumentIndex << endl;
									cout << "relationArgumentIndex2 = " << relationArgumentIndex2 << endl;
									*/
									
									addActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity);
								}
							}
							
							#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
							if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
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
							if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
							{
							#endif
								#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
								//[search for intermediary {ie redundant} relations, and if so create a condition link between subject and object] 
								if(!foundPartner)
								{//do not overwrite usage of subj/obj if a direct link [ie action] has been found (this condition probably/mau not be required)

									if(subjectObjectFunctionEntityArray[SUBJECT_INDEX]->entityName == RELATION_FUNCTION_DEFINITION_1)
									{
										//cout << "a" << endl;
										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationFunction == RELATION_FUNCTION_DEFINITION_1)
											{											
												//cout << "a2" << endl;
												if((currentRelationInList3->relationType == RELATION_TYPE_ADJECTIVE_3))	//OR if (currentRelationInList3->relationType == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
												{
													//cout << "b" << endl;
													if(currentRelationInList3->relationArgument == subjectObjectFunctionEntityArray[OBJECT_INDEX]->entityName)
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

														//create a condition link between the object and subject, based upon RELATION_FUNCTION_DEFINITION_1

														GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
														GIAEntityNode * specialConditionNode = objectEntityTemp;


														//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList3->relationFunctionIndex]->negative & GIAEntityNodeArray[currentRelationInList3->relationArgumentIndex]->negative;

														GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList3->relationArgumentIndex];

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
											GIAEntityNode * propertyEntity = objectEntityTemp;
											GIAEntityNode * ownerEntity = subjectEntityTemp;
											//cout << "propertyName = " << propertyEntity->entityName << endl;
											//cout << "ownerName = " << ownerEntity->entityName << endl;

											addOrConnectPropertyToEntity(ownerEntity, propertyEntity);

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
				currentRelationInList2 = currentRelationInList2->next;
			}
		}

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

		if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
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

					if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
					{//found a matching object-indirectobject relationship
						//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;


						GIAEntityNode * propertyEntity = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
						GIAEntityNode * thingEntity = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];

						addOrConnectPropertyToEntity(thingEntity, propertyEntity);
					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}			
}



void defineObjectSubjectOfPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "here1" << endl;
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		if(currentRelationInList->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
		{					
			//now find the associated object..
 			Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
			while(currentRelationInList2->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;					
				if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION)
				{
					partnerTypeRequiredFound = true;
				}

				if(partnerTypeRequiredFound)
				{		
					if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
					{//found a matching preposition of object-subject relationship
						/*
						cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
						cout << "currentRelationInList2->relationArgumentIndex = " << currentRelationInList2->relationArgumentIndex << endl;
						cout << "currentRelationInList->relationArgumentIndex = " << currentRelationInList->relationArgumentIndex << endl;
						cout << "currentRelationInList2->relationFunctionIndex = " << currentRelationInList2->relationFunctionIndex << endl;
						*/

						GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList2->relationArgumentIndex];
						GIAEntityNode * conditionEntityNode = GIAEntityNodeArray[currentRelationInList->relationArgumentIndex];						
						GIAEntityNode * conditionTypeConceptEntity = GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex];

						/*
						cout << "entityNode->entityName = " << entityNode->entityName << endl;
						cout << "conditionEntityNode->entityName = " << conditionEntityNode->entityName << endl;		//this is wrong
						cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl;
						cout << "currentRelationInList->relationArgumentIndex = " << currentRelationInList->relationArgumentIndex << endl;
						*/

						//should really take into account the boolean and of both values: bool relationNegative = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex]->negative & GIAEntityNodeArray[currentRelationInList2->relationFunctionIndex]->negative;

						addOrConnectPropertyConditionToEntity(entityNode, conditionEntityNode, conditionTypeConceptEntity);							

					}
				}

				currentRelationInList2 = currentRelationInList2->next;
			}
		}
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}


void defineActionPropertyConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{	
		//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

		int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
		int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
		string relationType = currentRelationInList->relationType;
		GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationFunctionIndex];				
		GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[relationArgumentIndex];
		
		bool passed = true;
		#ifdef GIA_IGNORE_MEANINGLESS_RELATIONS
		if(GIAEntityNodeArray[relationArgumentIndex]->entityName == relationType)
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

		#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
		for(int i=0; i<RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; i++)
		{
			if(relationType == relationTypeConjugationNameArray[i])
			{
				passed = false;
			}
		}
		#endif

		#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
		{
		#endif
			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_PROPERTY_LINK_DEPENDING_UPON_ACTION_OR_PROPERTY
			bool stanfordPrepositionFound = false;
			if(convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound) == RELATION_TYPE_OF)
			{
				passed = false;

				//cout << "a" << endl;

				GIAEntityNode * actionOrProperty = actionOrPropertyEntity;
				if(actionOrPropertyEntity->hasAssociatedInstanceTemp)
				{
					actionOrProperty = actionOrPropertyEntity->AssociatedInstanceNodeList.back();
				}

				if(actionOrProperty->PropertyNodeReverseList.size() >= 1)	//CHECKTHIS; NB only concerned if it was created in the current context (eg sentence) - how can test this?		
				{
					//cout << "b" << endl;
					if(actionOrProperty->PropertyNodeReverseList.back()->isAction)
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
					addOrConnectPropertyToEntity(actionOrPropertyConditionEntity, actionOrPropertyEntity);			
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
	bool passedPropositionUnknown = false;
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

	for(int i=0; i<RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionTimeNameArray[i])
		{
			if(actionOrPropertyConditionEntity->hasAssociatedTime)
			{
				passedPropositionTime = true;
				passedPreposition = true;
			}
			else if(prepositionName == REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN)
			{//NB time query entities obviously do not have associated times (time grammar flags generated by relex)
				passedPropositionTime = true;
				passedPreposition = true;
			}
		}
	}			
	for(int i=0; i<RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES; i++)
	{
		if(prepositionName == relationTypePropositionLocationNameArray[i])
		{
			if(!actionOrPropertyConditionEntity->hasAssociatedTime)	
			{//NB "at" and "on" are shared for location and time prepositions
				passedPropositionLocation = true;
				passedPreposition = true;
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
				passedPreposition = true;
			}
		}
	}
	
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_RELEX)
	{
		if(prepositionName[0] != RELATION_TYPE_PREPOSITION_FIRST_CHARACTER)
		{//not valid for REFERENCE_TYPE_QUESTION_QUERY_VARIABLEs... [but this is not a problem because passedPropositionUnknown is processed last in the if/else switch below]
			passedPropositionUnknown = true;
			passedPreposition = true;		
		}	
	}
	else if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATION_FORMATION_STANFORD)
	{
		if(stanfordPrepositionFound)
		{
			passedPropositionUnknown = true;
			passedPreposition = true;		
		}	
	}
	//cout << "prepositionName = " << prepositionName << endl;

		
	if(passedPreposition)
	{
		
		string conditionTypeName = prepositionName;
		long entityIndex = -1;
		bool entityAlreadyExistant = false;
		//cout << "prepositionName = " << prepositionName << endl;
		vector<GIAEntityNode*> * entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
		long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
		long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();		
		GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, &conditionTypeName, &entityAlreadyExistant, &entityIndex, true, currentEntityNodeIDInCompleteList, currentEntityNodeIDInConceptEntityNodesList);	
		//cout << "conditionTypeConceptEntity->entityName = " << conditionTypeConceptEntity->entityName << endl; 
	
		//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
		if(actionOrPropertyEntity->hasAssociatedInstanceTemp)
		{
			actionOrPropertyEntity = actionOrPropertyEntity->AssociatedInstanceNodeList.back();	
		}				

		//CHECK THIS; check order - either select action or property first; NB there should not be both an associated action and an associated property in a given "Temp" context
		if(actionOrPropertyConditionEntity->hasAssociatedInstanceTemp)
		{
			//cout << "actionOrPropertyConditionEntity->hasAssociatedInstanceTemp" << endl;
			actionOrPropertyConditionEntity = actionOrPropertyConditionEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
			//cout << "actionOrPropertyConditionEntity->entityName = " << actionOrPropertyConditionEntity->entityName << endl; 
		}

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
		else if(passedPropositionUnknown)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "actionOrPropertyConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity);				
			//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
		}	

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
	




/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
#endif
*/
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{	
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

