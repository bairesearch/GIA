/*******************************************************************************
 *
 * File Name: GIATranslatorLinkEntities.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l5b 03-June-2012
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
				//cout << "RELATION_TYPE_POSSESSIVE" << endl;

				string propertyName = currentRelationInList->relationGovernor; 
				string ownerName = currentRelationInList->relationDependent; 
				int propertyIndex = currentRelationInList->relationGovernorIndex;
				int ownerIndex = currentRelationInList->relationDependentIndex;				

				GIAEntityNode * propertyEntity = GIAEntityNodeArray[propertyIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[ownerIndex];
				//cout << "propertyName = " << propertyEntity->entityName << endl;
				//cout << "ownerName = " << ownerEntity->entityName << endl;
				
				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity, sameReferenceSet);
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
					GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);		
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
				cout << "definitionEntity = " << definitionEntity->entityName << endl;
				#endif
				
				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_APPOS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				addDefinitionToEntity(propertyEntity, definitionEntity, sameReferenceSet);									
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif				
		currentRelationInList = currentRelationInList->next;
	}
}


void defineSubjectOrObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
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
										//cout << "ASD" << endl;

										GIAEntityNode * actionOrPropertyConditionEntity;
										int actionIndex;
										if(passsubject && (subjectObjectEntity->entityName == currentRelationInList3->relationDependent))
										{//subject is connected to an _advmod
											/*eg 1 Space is saved by running fast.
											_obj(save[3], space[1])	[IRRELEVANT]
											_advmod(save[3], by[4])
											_advmod(run[5], fast[6]) [IRRELEVANT]
											_subj(run[5], by[4])											
											*/
											actionOrPropertyConditionEntity = subjectObjectFunctionEntity;
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
											actionOrPropertyConditionEntity = subjectObjectEntity;
											actionIndex = subjectObjectEntityIndex;
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
											
											GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrPropertyConditionEntity);
											actionOrPropertyConditionEntity = GIAEntityNodeArray[actionIndex];
											
											//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
											//cout << "actionOrPropertyEntity = " << actionOrPropertyEntity->entityName << endl;
											//cout << "conditionTypeConceptEntity = " << conditionTypeConceptEntity->entityName << endl;

											addOrConnectConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
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
								
								#ifdef GIA_USE_ADVANCED_REFERENCING
								bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif
								
								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * subjectEntityTemp = subjectObjectEntity;
								//cout << "vf1" << endl;
								GIAEntityNodeArray[actionIndex] = addActionToSubject(subjectObjectEntity, actionEntity, sameReferenceSetSubject);	
								//cout << "vf2" << endl;
							}
							else if(passobject)
							{//eg the bow was fired
								
								#ifdef GIA_USE_ADVANCED_REFERENCING
								bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
								#else
								bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
								#endif
								
								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * objectEntityTemp = subjectObjectEntity;

								//cout << "vf3" << endl;
								GIAEntityNodeArray[actionIndex] = addActionToObject(objectEntityTemp, actionEntity, sameReferenceSetSubject);
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

void defineSubjectObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
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
									/*
									cout << "partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound" << endl;
									cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
									cout << "currentRelationInList2->relationDependent = " << currentRelationInList2->relationDependent << endl;
									cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
									cout << "currentRelationInList2->relationGovernor = " << currentRelationInList2->relationGovernor << endl;						
									*/
								}
							}		
						#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
						}
						#endif	
						
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
															addOrConnectBeingDefinitionConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}
														#endif
														#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
														else if(passcomposition)
														{
															bool negative = subjectObjectFunctionEntityArray[SUBJECT_INDEX]->negative;
															subjectIsConnectedToAnAdvMod = true;
															actionOrPropertyConditionEntity = objectEntityTemp;	//= subjectObjectEntityArray[SUBJECT_INDEX], = old subjectEntityTemp;
															addOrConnectHavingPropertyConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, negative, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
														}
														#endif
														else
														{
															//standard action/property condion (ie action condition in this context)
															actionOrPropertyConditionEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
															GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrPropertyConditionEntity);	//not required is done later?
															actionOrPropertyConditionEntity = GIAEntityNodeArray[actionIndex];
															addOrConnectConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

															GIAEntityNode * actionEntity = actionOrPropertyConditionEntity;
															GIAEntityNodeArray[actionIndex] = addActionToObject(objectEntityTemp, actionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);																			
														}									

													#else

														#ifdef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
														cout << "error: ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION not yet coded; in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing" << endl;
														#else
														//create arbitrarySubjectSpecialConceptEntityNode
														string arbitrarySubjectSpecialConceptEntityNodeName = ARBITRARY_SUBJECT_SPECIAL_CONCEPT_NODE_NAME;
														bool entityAlreadyExistant = false;
														GIAEntityNode * arbitrarySubjectSpecialConceptEntityNode = findOrAddEntityNodeByNameSimpleWrapper(&arbitrarySubjectSpecialConceptEntityNodeName, &entityAlreadyExistant, entityNodesActiveListConcepts);
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
															int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
															//cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity->entityName << endl;
															GIAEntityNodeArray[actionIndex] = addActionToActionDefinition(actionOrPropertyConditionEntity);	//not required is done later?
														}

														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION
														passdefinition = false;		//treat "being" as an action
														#endif
														#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
														passpasscomposition = false;		//treat "being" as an action
														#endif

														addOrConnectConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
													#endif	

													}
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

																GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(baseEntity, propertyEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);			
																GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(propertyEntity, objectEntityTemp, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

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

																addDefinitionToEntity(baseEntity, definitionEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

																GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(definitionEntity, propertyEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);	

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
												bool sameReferenceSet = false;
												addDefinitionToEntity(objectEntityTemp, subjectEntityTemp, sameReferenceSet);
											#else

												//added 20 October 2011 [what is the time?]
												GIAEntityNode * actionOrPropertyEntity = objectEntityTemp;				
												GIAEntityNode * actionOrPropertyConditionEntity = subjectEntityTemp;
												GIAEntityNode * conditionTypeConceptEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												addOrConnectConditionToEntity(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);
											#endif	
											}
											else
											{
												//cout << "h2" << endl;
												bool sameReferenceSet = false;
												addDefinitionToEntity(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
											}

											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure	
										}
										#endif
										#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
										//NB this case may be redundant because Relex automatical interprets these cases as preadj [eg _predadj(tom[1], happy[3])]
										//else if((currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_1) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_2) || (currentRelationInList->relationGovernor == RELATION_GOVERNOR_COMPOSITION_3))
										else if(passcomposition)
										{//subject-object relationship is a composition [property]
											bool sameReferenceSet = false;
											GIAEntityNodeArray[objectEntityIndexTemp] = addOrConnectPropertyToEntity(subjectEntityTemp, objectEntityTemp, sameReferenceSet);
												//check can use properties for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
											//cout << "a" << endl;

											disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]);	//remove lone 'be' artefacts (blue entity nodes). NB these occur because of the nature of the 'is' -> entity definitional substitution procedure
										}
										#endif
										else if(partnerTypeObjectSpecialConditionMeasureDistanceOrStanfordFound)
										{//this was removed from Stanford execution 15 May 2012		
															
											#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
												//eg The rabbit is 20 meters away.	[away is a property of rabbit, not a condition of rabbit]
												GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;	//aka subjectObjectEntityArray[SUBJECT_INDEX];
												int propertyIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
												GIAEntityNode * propertyEntity = subjectObjectFunctionEntityArray[SUBJECT_INDEX];

												bool sameReferenceSet = false;	//CHECK THIS???
												GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(subjectEntityOrProperty, propertyEntity, sameReferenceSet);

											#else
												//eg The rabbit is 20 meters away.	[away is a condition of rabbit, not a property of rabbit]

												GIAEntityNode * subjectEntityOrProperty = subjectEntityTemp;
												GIAEntityNode * specialConditionNode = GIAEntityNodeArray[relationGovernorIndex2];
												//cout << "subjectEntityOrProperty->entityName = " << subjectEntityOrProperty->entityName << endl;
												//cout << "specialConditionNode->entityName = " << specialConditionNode->entityName << endl;	

												string conditionTypeName = "specialCondition";

												bool entityAlreadyExistant = false;
												GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

												addOrConnectConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

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
											int actionIndex = subjectObjectFunctionEntityIndexArray[SUBJECT_INDEX];
											
											#ifdef GIA_USE_ADVANCED_REFERENCING
											bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);	//subject relation
											#else
											bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAEntityNodeArray[actionIndex] = addActionToObject(objectEntityTempUpdated, actionEntity, sameReferenceSet);

											//create a property link between the subject and object
											GIAEntityNode * propertyEntity = subjectObjectEntityArray[OBJECT_INDEX];
											int propertyIndex = subjectObjectEntityIndexArray[OBJECT_INDEX];
											GIAEntityNode * ownerEntity = actionEntity;
											//cout << "propertyName = " << propertyEntity->entityName << endl;
											//cout << "ownerName = " << ownerEntity->entityName << endl;
											
											#ifdef GIA_USE_ADVANCED_REFERENCING
											sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES, currentRelationInList);	//subject relation
											#else
											sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											#endif
											GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity, sameReferenceSet);

										}	
										#endif								
										else
										{//assume that the subject-object relationships is an action
											string actionName = currentRelationInList->relationGovernor;
											//cout << "1 actionName = " << actionName << endl;
											int actionIndex = relationGovernorIndex;
											GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];

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
											
											#ifdef GIA_USE_ADVANCED_REFERENCING
											bool sameReferenceSetSubject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList);
											bool sameReferenceSetObject = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS, currentRelationInList2);
											#else
											bool sameReferenceSetSubject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
											bool sameReferenceSetObject = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;											
											#endif
											GIAEntityNodeArray[actionIndex] = addActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity, sameReferenceSetSubject, sameReferenceSetObject);
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

																	addOrConnectConditionToEntity(subjectEntityOrProperty, specialConditionNode, conditionTypeConceptEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);														

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

													GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(ownerEntity, propertyEntity, OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET);

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

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE;	//eg the linebacker that gave the quarterback a push is blue. / the linebacker gave the quarterback a push
								GIAEntityNodeArray[propertyIndex] = addOrConnectPropertyToEntity(thingEntity, propertyEntity, sameReferenceSet);
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
void defineHavingPropertyConditionsAndBeingDefinitionConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
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
	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			
			if(stanfordPrepositionFound)
			{		
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				
				while(currentRelationInList2->next != NULL)
				{					
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{			
					#endif	
						//cout << "AS1" << endl;
				
						#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
						if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
						{
							//cout << "AS2" << endl;	
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching relationship	
								//cout << "AS3" << endl;
								if(currentRelationInList->relationDependent == RELATION_ENTITY_HAVE)
								{
									//cout << "AS4" << endl;
									currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition 
									
									string conditionTypeName = prepositionName;
									bool entityAlreadyExistant = false;
									GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);

									GIAEntityNode * haveEntity = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
									bool negative = haveEntity->negative;
									GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
									GIAEntityNode * conditionPropertyNode = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
									
									#ifdef GIA_USE_ADVANCED_REFERENCING
									bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_PROPERTY_CONDITIONS, currentRelationInList2);	//defineHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through having a chicken is.... therefore default same set]
									//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_PROPERTY_CONDITIONS;	//more precisely
									#else
									bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									#endif
									addOrConnectHavingPropertyConditionToEntity(entityNode, conditionPropertyNode, conditionTypeConceptEntity, negative, sameReferenceSet);	
									
									disableEntityBasedUponFirstSentenceToAppearInNetwork(haveEntity);								
								}
							}
						}
						#endif
						#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_CONDITION_DEFINITION
						else if(currentRelationInList2->relationType == RELATION_TYPE_COPULA)
						{	
							//cout << "AS2" << endl;
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching relationship	
								//cout << "AS3" << endl;
								if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
								{
									//cout << "AS4" << endl;
									currentRelationInList->disabled = true;	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
									
									string conditionTypeName = prepositionName;
									bool entityAlreadyExistant = false;
									GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);
																											
									GIAEntityNode * beEntity = GIAEntityNodeArray[currentRelationInList2->relationDependentIndex];
									bool negative = beEntity->negative;
									GIAEntityNode * entityNode = GIAEntityNodeArray[currentRelationInList->relationGovernorIndex];
									GIAEntityNode * conditionDefinitionNode = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
									
									#ifdef GIA_USE_ADVANCED_REFERENCING
									bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS, currentRelationInList2);	//defineHavingPropertyConditionsAndBeingDefinitionConditions check relation to use here... [the chicken saved through being a chicken is.... therefore default same set]								
									//sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS;	//more precisely
									#else
									bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
									#endif
									addOrConnectBeingDefinitionConditionToEntity(entityNode, conditionDefinitionNode, conditionTypeConceptEntity, negative, sameReferenceSet);																
								
									disableEntityBasedUponFirstSentenceToAppearInNetwork(beEntity);

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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}		
		#endif
			//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

//is this still used by Stanford?
void defineObjectSubjectOfPreposition(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
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

								bool sameReferenceSet = DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS;	//CHECK THIS... defineObjectSubjectOfPreposition use DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS?
								createConditionBasedUponPreposition(entityNode, conditionEntityNode, conditionTypeConceptEntity->entityName, false, currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);

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
		//cout << "here2" << endl;
		currentRelationInList = currentRelationInList->next;
	}
}


void defineActionPropertyConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{						
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition		
		{			
		//#endif
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
						cout << "ASF" << endl;
						/*
						NB this case "The house of Kriton is blue." should create 2 property connections (not just 1)
						of(house[2], Kriton[4])
						_predadj(house[2], blue[6])
						*/	
						#ifdef GIA_USE_ADVANCED_REFERENCING
						bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);			
						#else
						bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
						#endif
						GIAEntityNodeArray[actionOrPropertyIndex] = addOrConnectPropertyToEntity(actionOrPropertyConditionEntity, actionOrPropertyEntity, sameReferenceSet);			
					}			
				}		
				#endif
			#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
			}
			#endif

			if(passed)
			{
				#ifdef GIA_USE_ADVANCED_REFERENCING
				bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
				#else
				bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
				#endif
				createConditionBasedUponPreposition(actionOrPropertyEntity, actionOrPropertyConditionEntity, relationType, false, currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, sameReferenceSet);
			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		//#endif
		
		currentRelationInList = currentRelationInList->next;
	}
}

bool determineFeatureIndexOfPreposition(Sentence * currentSentenceInList, string * prepositionName, int * indexOfPreposition)
{
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

void createConditionBasedUponPreposition(GIAEntityNode * actionOrPropertyEntity, GIAEntityNode * actionOrPropertyConditionEntity, string relationType, bool negative, Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, bool sameReferenceSet)
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

		//cout << "3prepositionName = " << prepositionName << endl;
		
		#ifdef GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
		//added 3 June 2012 for advanced referencing of prepositions
		GIAEntityNode * conditionTypeConceptEntity;
		int featureIndexOfPreposition = -1;
		
		bool prepositionFeatureFound = determineFeatureIndexOfPreposition(currentSentenceInList, &prepositionName, &featureIndexOfPreposition);
		if(prepositionFeatureFound)
		{
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			cout << "prepositionFeatureFound" << endl;
			cout << "prepositionName = " << prepositionName << endl;
			cout << "featureIndexOfPreposition = " << featureIndexOfPreposition << endl;
			#endif
			if(GIAEntityNodeArrayFilled[featureIndexOfPreposition])
			{
				conditionTypeConceptEntity = GIAEntityNodeArray[featureIndexOfPreposition];
				/*OLD: not required any more as take last word in multword prepositions (eg 'to' in near_to)
				//conditionTypeConceptEntity->disabled = false;	
					//required in the case a preposition word has been added in initialisation becuase it occurs as a governor/dependent of a relation within the sentence (as opposed to being initialised as a reference by GIA in its advanced references)
					//eg He rode the carriage that is near to the horse. nsubj(near-7, carriage-4) / prep_to(near-7, horse-10) -> prep_near_to(carriage-4, horse-10)
				*/
			}
			else
			{
				string conditionTypeName = prepositionName;
				bool entityAlreadyExistant = false;
				conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);				
				GIAEntityNodeArrayFilled[featureIndexOfPreposition] = true;
				//cout << "2prepositionName = " << prepositionName << endl;
			}
		}
		else
		{
			cout << "error: !prepositionFeatureFound" << endl;
			exit(0);
		}		
		#else
		string conditionTypeName = prepositionName;
		bool entityAlreadyExistant = false;
		GIAEntityNode * conditionTypeConceptEntity = findOrAddEntityNodeByNameSimpleWrapper(&conditionTypeName, &entityAlreadyExistant, entityNodesActiveListConcepts);
		#endif
		

													
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

			#ifdef GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addTimeConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);				
			#else
			addTimeConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
			#endif
		}
		else if(passedPropositionLocation)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "locationConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif
			
			#ifdef GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addLocationConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);	
			#else
			addLocationConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);				
			#endif
		}		
		else if(passedPropositionReasonOrCircumstances)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "reasonConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addReasonConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);		
			#else
			addReasonConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);		
			#endif
		}
		//else if(passedPropositionUnknown)		//removed 17 April 2012
		else
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "actionOrPropertyEntity->entityName = " << actionOrPropertyEntity->entityName << endl;
			cout << "actionOrPropertyConditionName = " << actionOrPropertyConditionEntity->entityName << endl;
			#endif

			#ifdef GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
			GIAEntityNodeArray[featureIndexOfPreposition] = addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);				
			#else
			addPropertyConditionToProperty(actionOrPropertyEntity, actionOrPropertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);				
			#endif
			//currentRelationInList->relationType.substr(1, currentRelationInList->relationType.length()-1)
		}
		
		//cout << "GIAEntityNodeArray[featureIndexOfPreposition]->entityName = " << GIAEntityNodeArray[featureIndexOfPreposition]->entityName << endl;	

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
	//cout << "addTimeToProperty timeConditionEntity->entityName = " << timeConditionEntity->entityName << endl;
	timeConditionEntity->conditionType = CONDITION_NODE_TYPE_TIME;
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	timeConditionEntity->timeConditionNode = newTimeCondition;	
	
}



/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, vector<GIATimeConditionNode*> *timeConditionNodesActiveList, vector<long> *timeConditionNumbersActiveList)
#else
void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
#endif
*/
GIAEntityNode * addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{	
	//cout << "addTimeConditionToProperty propertyNode->entityName = " << propertyNode->entityName << endl;
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
	
	return addOrConnectConditionToEntity(propertyNode, timeConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

GIAEntityNode * addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{	
	locationConditionEntity->conditionType = CONDITION_NODE_TYPE_LOCATION;
	
	return addOrConnectConditionToEntity(propertyNode, locationConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

GIAEntityNode * addReasonConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * reasonConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{	
	reasonConditionEntity->conditionType = CONDITION_NODE_TYPE_REASON;
	
	return addOrConnectConditionToEntity(propertyNode, reasonConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}


GIAEntityNode * addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity, bool sameReferenceSet)
{
	//timeConditionEntity->conditionType = CONDITION_NODE_TYPE_UNDEFINED;
	
	return addOrConnectConditionToEntity(propertyNode, propertyConditionEntity, conditionTypeConceptEntity, sameReferenceSet);
}

