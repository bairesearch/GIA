/*******************************************************************************
 *
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j5a 30-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"


static long currentEntityNodeIDInCompleteList;				//For GIA XML generation only
static long currentEntityNodeIDInConceptEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInPropertyEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInActionEntityNodesList;			//For GIA XML generation only
static long currentEntityNodeIDInConditionEntityNodesList;		//For GIA XML generation only

static vector<GIAEntityNode*> * entityNodesCompleteList;		//For GIA XML generation only
//static vector<GIAEntityNode*> * conceptEntityNodesList;
static vector<GIAEntityNode*> * propertyEntityNodesList;		//For GIA XML generation only
static vector<GIAEntityNode*> * actionEntityNodesList;			//For GIA XML generation only
static vector<GIAEntityNode*> * conditionEntityNodesList;		//For GIA XML generation only

static bool foundComparisonVariable;
static GIAEntityNode* comparisonVariableNode;




void initialiseGIATranslatorForTexualContextOperations()
{
	currentEntityNodeIDInCompleteList = 0;
	currentEntityNodeIDInConceptEntityNodesList = 0;
	currentEntityNodeIDInPropertyEntityNodesList = 0;
	currentEntityNodeIDInActionEntityNodesList = 0;
	currentEntityNodeIDInConditionEntityNodesList = 0;
	
	vector<GIAEntityNode*>::iterator indexOfEntityNodesIterator;
	vector<string*>::iterator indexOfEntityNamesIterator;
	vector<GIATimeConditionNode*>::iterator indexOfTimeNodesIterator;
	vector<long*>::iterator indexOfTimeNumbersIterator;	

	/*
	//initialise conceptEntityNodesList;	[should be moved elsewhere]
	string firstEntityNameInNetwork = "universe";
	GIAEntityNode * firstEntityNodeInNetwork = new GIAEntityNode();
	firstEntityNodeInNetwork->entityName = firstEntityNameInNetwork;
	conceptEntityNodesList->push_back(firstEntityNodeInNetwork);
	conceptEntityNamesList->push_back(firstEntityNameInNetwork);
	
	//DEBUG
	//conceptEntityNamesList->push_back("za");
	//conceptEntityNamesList->push_back("zb");
	//conceptEntityNamesList->push_back("zc");
	//conceptEntityNamesList->push_back("zd");
	//conceptEntityNamesList->push_back("ze");
	//conceptEntityNamesList->push_back("zf");
	//conceptEntityNamesList->push_back("zg");
	//conceptEntityNamesList->push_back("zh");
	//conceptEntityNamesList->push_back("zi");
	//conceptEntityNamesList->push_back("zj");
	
	
	//initialise timeConditionNodesList;		[should be moved elsewhere]	
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIATimeConditionNode * firstTimeNodeInNetwork = new GIATimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersList->push_back(firstTimeInNetwork);		
	
	
	//DEBUG	
	//bool resultTemp = true;
	//string tempName = "zh";
	//long findIndex = -1;
	//GIAEntityNode * tempEntity = findOrAddEntityNodeByName(conceptEntityNodesList, &tempName, &resultTemp, &findIndex);
	//if(resultTemp)
	//{
	//	cout << "tempEntity->entityName = " << tempEntity->entityName << endl;
	//}
	//exit(0);
	*/
}

					
bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType)
{
	bool result = true;
	
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
		if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD) && (GIAEntityNodeArray[relationGovernorIndex]->entityName == RELATION_ENTITY_BE))
		{//added condition Fri 27 Jan - remove 'be' node artefacts
			disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[relationGovernorIndex]);	
			result = false;
			//cout << "GIAEntityNodeArray[relationGovernorIndex]->disabled = true" << endl;		
		}
		#endif
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE	
	}
	#endif
	
	return result;
}

bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList, int NLPdependencyRelationsType)
{
	bool passed2 = true;

	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
	#endif			
		if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
		{
			Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
 			while(currentRelationInList3->next != NULL)
			{	
				bool partnerTypeRequiredFound = false;

				for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList3->relationType == relationTypeSubjectNameArray[i])
					{
						partnerTypeRequiredFound = true;
					}
				}
				for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList3->relationType == relationTypeObjectNameArray[i])
					{
						partnerTypeRequiredFound = true;
					}
				}

				if(partnerTypeRequiredFound)
				{
					if(currentRelationInList->relationDependentIndex == currentRelationInList3->relationDependentIndex)
					{//do not add property, if _advmod argument (eg 'by') is connected to a subject/object
						passed2 = false;
						//cout << "ASFS" << endl;
					}
				}
				currentRelationInList3 = currentRelationInList3->next;							
			}
		}
		else
		{
			bool passed2 = true;
		}
	#ifdef GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
	}
	#endif
			
	
	return passed2;
}

void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{	
	if(!(thingEntity->disabled))
	{			
	#endif	
			
		if(propertyEntity->entityAlreadyDeclaredInThisContext)
		{
			if(!(propertyEntity->hasAssociatedInstanceTemp))
			{
				propertyEntity->hasAssociatedInstanceTemp = true;
			}	
			//cout << "1. propertyEntity->entityName = " << propertyEntity->entityName << endl; 
		}

		if(thingEntity->entityAlreadyDeclaredInThisContext)
		{
			if(!(thingEntity->hasAssociatedInstanceTemp))
			{
				thingEntity->hasAssociatedInstanceTemp = true;
			}	
		}

		if(propertyEntity->hasAssociatedInstanceTemp)
		{
			GIAEntityNode * existingProperty  = propertyEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a

			/*
			if(propertyEntity->hasQualityTemp)
			{
				existingProperty->hasQuality = true;
			}
			*/

			if(thingEntity->hasAssociatedInstanceTemp)
			{		
				thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
			}

			//configure entity node containing this property
			thingEntity->PropertyNodeList.push_back(existingProperty);	

			thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			existingProperty->PropertyNodeReverseList.push_back(thingEntity);	
			//existingProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a, removed 8 Dec 2011

		}
		else
		{
			if(thingEntity->hasAssociatedInstanceTemp)
			{	
				//cout << "thingEntity = " << thingEntity->entityName << endl;
				//cout << "propertyEntity = " << propertyEntity->entityName << endl;

				thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
			}

			//cout << "thingEntity = " << thingEntity->entityName << endl;

			GIAEntityNode * newProperty = addProperty(propertyEntity);		

			/*
			if(propertyEntity->hasQualityTemp)
			{
				newProperty->hasQuality = true;
			}
			*/

			newProperty->PropertyNodeReverseList.push_back(thingEntity);
			//newProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a, removed 8 Dec 2011

			//configure entity node containing this property
			thingEntity->PropertyNodeList.push_back(newProperty);		

			thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
		}	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}			
	#endif			
}


void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{				
	#endif
	
		//cout << "\t\t addPropertyToPropertyDefinition; propertyEntity->entityName = " << propertyEntity->entityName << endl;

		GIAEntityNode * newOrExistingProperty;

		if(propertyEntity->entityAlreadyDeclaredInThisContext)
		{
			if(!(propertyEntity->hasAssociatedInstanceTemp))
			{
				propertyEntity->hasAssociatedInstanceTemp = true;
			}	
		}

		if(propertyEntity->hasAssociatedInstanceTemp)
		{	
			//cout << "break; propertyEntity->entityName = " << propertyEntity->entityName << endl;
			GIAEntityNode * newOrExistingProperty = propertyEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a

		}
		else
		{	
			GIAEntityNode * newProperty = addProperty(propertyEntity);	
		}
	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}			
	#endif		
}


GIAEntityNode * addProperty(GIAEntityNode * propertyEntity)
{	
	
	//configure property node
	GIAEntityNode * newProperty = new GIAEntityNode();
	newProperty->id = currentEntityNodeIDInCompleteList;
	newProperty->idSecondary = currentEntityNodeIDInPropertyEntityNodesList;

	entityNodesCompleteList->push_back(newProperty);
	currentEntityNodeIDInCompleteList++;
	propertyEntityNodesList->push_back(newProperty);
	currentEntityNodeIDInPropertyEntityNodesList++;

	newProperty->entityName = propertyEntity->entityName;
	newProperty->isProperty = true;
	//newProperty->entityNodeContainingThisProperty = NULL;
	newProperty->entityNodeDefiningThisInstance = propertyEntity;
	propertyEntity->hasAssociatedInstance = true;
	propertyEntity->hasAssociatedInstanceTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	newProperty->wordNetPOS = propertyEntity->wordNetPOS;
	
	//if(propertyEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
	//{
	newProperty->grammaticalNumber = propertyEntity->grammaticalNumberTemp;
	//}

	if(propertyEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newProperty->hasProgressiveTemp = true;
		//cout << "property has progressive (eg lying/sitting/being happy)" << endl;
	}

	if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newProperty->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp, newProperty->hasProgressiveTemp);
	}

	//configure property definition node
	propertyEntity->AssociatedInstanceNodeList.push_back(newProperty);

	propertyEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	//cout << "propertyEntity->entityName = " << propertyEntity->entityName << endl;
	if(propertyEntity->isQuery)
	{
		propertyEntity->isQuery = false;
		newProperty->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(propertyEntity->isWhichQuery)
		{
			propertyEntity->isWhichQuery = false;
			newProperty->isWhichQuery = true;			
		}
		#endif
	}
	#endif	

					
	return newProperty;	
}


GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction = NULL;
	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(actionEntity->disabled))
	{				
	#endif

		if(actionEntity->entityAlreadyDeclaredInThisContext)
		{//CHECK THIS; need to convert to action node here also? ie hasAssociatedInstanceIsAction->true? [must look at generated semanticNet.xml and see if any propertyNodeContainers contain action nodes..., or if any actionNodeContainers do not contain property nodes...]
			if(!(actionEntity->hasAssociatedInstanceTemp))
			{	
				//cout << "actionEntity->entityAlreadyDeclaredInThisContext as a property:" << actionEntity->entityName << endl;
				actionEntity->hasAssociatedInstanceTemp = true;
			}
		}

		//configure action node	
		if(actionEntity->hasAssociatedInstanceTemp)
		{
			newOrExistingAction = actionEntity->AssociatedInstanceNodeList.back();	

			if(actionEntity->hasAssociatedInstanceIsAction == false)
			{//upgrade associated property to action

				//CHECK THIS; must remove from property list, and add to action list 
				actionEntity->hasAssociatedInstanceIsAction = true;
				newOrExistingAction->isProperty = false;
				newOrExistingAction->isAction = true;

				//cout << "newOrExistingAction->idSecondary = " << newOrExistingAction->idSecondary << endl;

				vector<GIAEntityNode*>::iterator propertyEntityNodesListIterator = propertyEntityNodesList->begin();
				advance(propertyEntityNodesListIterator,newOrExistingAction->idSecondary);
				propertyEntityNodesList->erase(propertyEntityNodesListIterator);
				currentEntityNodeIDInPropertyEntityNodesList--;

				actionEntityNodesList->push_back(newOrExistingAction);
				currentEntityNodeIDInActionEntityNodesList++;
			}
			#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_CONCEPT_ENTITY_IN_A_GIVEN_SENTENCE
			else
			{
				newOrExistingAction = addAction(actionEntity);
			}
			#endif
		}
		else
		{
			newOrExistingAction = addAction(actionEntity);
		}
		
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}				
	#endif		
	
	return newOrExistingAction;
}


	//conditions required to be added [eg when, where, how, why]
GIAEntityNode * addAction(GIAEntityNode * actionEntity)
{	
				
	//cout << "as4" << endl;

	GIAEntityNode * newAction = new GIAEntityNode();
	newAction->id = currentEntityNodeIDInCompleteList;
	newAction->idSecondary = currentEntityNodeIDInActionEntityNodesList;

	entityNodesCompleteList->push_back(newAction);
	currentEntityNodeIDInCompleteList++;
	actionEntityNodesList->push_back(newAction);
	currentEntityNodeIDInActionEntityNodesList++;

	newAction->entityName = actionEntity->entityName;		
	newAction->entityNodeDefiningThisInstance = actionEntity;

	//cout << "as4b" << endl;

	actionEntity->AssociatedInstanceNodeList.push_back(newAction);
	actionEntity->hasAssociatedInstance = true;
	actionEntity->hasAssociatedInstanceIsAction = true;
	actionEntity->hasAssociatedInstanceTemp = true;
	newAction->wordNetPOS = actionEntity->wordNetPOS;
	newAction->isAction = true;
	//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisProperty = NULL;

	if(actionEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newAction->hasProgressiveTemp = true;
		//cout << "property has progressive (eg lying/sitting/being happy)" << endl;			
	}

	//cout << "actionEntity->grammaticalTenseTemp = " << actionEntity->grammaticalTenseTemp << endl;
	//cout << "actionEntity->entityName = " << actionEntity->entityName << endl;

	if(actionEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newAction->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		//cout << "hello" << endl;
		//exit(0);
		addTenseOnlyTimeConditionToProperty(newAction, actionEntity->grammaticalTenseTemp, newAction->hasProgressiveTemp);
	}	

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	if(actionEntity->isQuery)
	{
		actionEntity->isQuery = false;
		newAction->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(actionEntity->isWhichQuery)
		{
			actionEntity->isWhichQuery = false;
			newAction->isWhichQuery = true;			
		}
		#endif
	}
	#endif	

	actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed

	//cout << "as5" << endl;	

	return newAction;
}







/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList)
#else
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
#endif
*/
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, bool isProgressive)
{
	propertyNode->conditionType = CONDITION_NODE_TYPE_TIME;
	
	/*
	if(isProgressive)
	{
		cout << "isProgressive sdf" << endl;
	}
	*/
	
	/*
	#ifdef GIA_USE_TIME_NODE_INDEXING
	int timeConditionEntityIndex = -1;
	bool argumentEntityAlreadyExistant = false;
	long timeConditionTotalTimeInSeconds = 0; //cannot assign absolute time to an event which occurs in the past.... //calculateTotalTimeInSeconds();
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/
	
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	
	newTimeCondition->tense = tense;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}	
	propertyNode->timeConditionNode = newTimeCondition;
	

}

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(thingEntity->disabled))
	{
	if(!(definitionEntity->disabled))
	{					
	#endif
	
		if(thingEntity->hasAssociatedInstanceTemp)
		{
			//cout << "thingEntity = thingEntity->AssociatedInstanceNodeList.back()" << endl;
			thingEntity = thingEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
		}

		if(definitionEntity->hasAssociatedInstanceTemp)
		{
			//cout << "definitionEntity = definitionEntity->AssociatedInstanceNodeList.back()" << endl;
			definitionEntity = definitionEntity->AssociatedInstanceNodeList.back();	//added 4 May 11a
		}

		//configure entity node
		thingEntity->EntityNodeDefinitionList.push_back(definitionEntity);

		//configure entity definition node
		definitionEntity->EntityNodeDefinitionReverseList.push_back(thingEntity);	
		
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}					
	#endif			
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(objectEntity->disabled))
	{	
	if(!(actionEntity->disabled))
	{					
	#endif
	
	GIAEntityNode * newOrExistingAction;
	newOrExistingAction = addActionToActionDefinition(actionEntity);
	
	addActionInstanceToSubject(subjectEntity, newOrExistingAction);

	addActionInstanceToObject(objectEntity, newOrExistingAction);
	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}					
	#endif	
}


void addActionInstanceToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * newOrExistingAction)
{		
	if(subjectEntity->hasAssociatedInstanceTemp)
	{
		subjectEntity = subjectEntity->AssociatedInstanceNodeList.back();
	}
	
	//configure subject entity node
	subjectEntity->ActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionSubjectEntity = subjectEntity;
	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;
	
	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}


void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity)
{	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{					
	#endif
		GIAEntityNode * newOrExistingAction;
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		addActionInstanceToSubject(subjectEntity, newOrExistingAction);	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}					
	#endif		
}

void addActionInstanceToObject(GIAEntityNode * objectEntity, GIAEntityNode * newOrExistingAction)
{		
	if(objectEntity->hasAssociatedInstanceTemp)
	{
		objectEntity = objectEntity->AssociatedInstanceNodeList.back();
	}
		
	//configure object entity node
	objectEntity->IncomingActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionObjectEntity = objectEntity;
	
	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(objectEntity->disabled))
	{	
	if(!(actionEntity->disabled))
	{					
	#endif
		GIAEntityNode * newOrExistingAction;
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		addActionInstanceToObject(objectEntity, newOrExistingAction);	
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}					
	#endif
}


void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeConceptEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionEntityNode->disabled))
	{	
	if(!(conditionTypeConceptEntity->disabled))
	{					
	#endif
	
		if(entityNode->hasAssociatedInstanceTemp)
		{
			entityNode = entityNode->AssociatedInstanceNodeList.back();
		}
		if(conditionEntityNode->hasAssociatedInstanceTemp)
		{
			conditionEntityNode = conditionEntityNode->AssociatedInstanceNodeList.back();
		}										
		addConditionToProperty(entityNode, conditionEntityNode, conditionTypeConceptEntity);
		
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}					
	#endif		
}

void addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeConceptEntity, bool negative)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionDefinitionNode->disabled))
	{	
	if(!(conditionTypeConceptEntity->disabled))
	{					
	#endif
	
		if(entityNode->hasAssociatedInstanceTemp)
		{
			entityNode = entityNode->AssociatedInstanceNodeList.back();
		}
		if(conditionDefinitionNode->hasAssociatedInstanceTemp)
		{
			conditionDefinitionNode = conditionDefinitionNode->AssociatedInstanceNodeList.back();
		}

		GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);
		newCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
		//cout << "negative = " << negative;

		newCondition->conditionSubjectEntity = entityNode;	
		entityNode->ConditionNodeList.push_back(newCondition);

		newCondition->EntityNodeDefinitionList.push_back(conditionDefinitionNode);
		conditionDefinitionNode->EntityNodeDefinitionReverseList.push_back(newCondition);	
		
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}				
	#endif			
}

void addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionPropertyNode, GIAEntityNode * conditionTypeConceptEntity, bool negative)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionPropertyNode->disabled))
	{	
	if(!(conditionTypeConceptEntity->disabled))
	{					
	#endif
	
		if(entityNode->hasAssociatedInstanceTemp)
		{
			entityNode = entityNode->AssociatedInstanceNodeList.back();
		}
		if(conditionPropertyNode->hasAssociatedInstanceTemp)
		{
			conditionPropertyNode = conditionPropertyNode->AssociatedInstanceNodeList.back();
		}	

		GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);
		newCondition->negative = negative;	//overwrite negative with correct one from context; ie that from "having" entity node

		newCondition->conditionSubjectEntity = entityNode;
		entityNode->ConditionNodeList.push_back(newCondition);

		conditionPropertyNode->PropertyNodeReverseList.push_back(newCondition);
		//conditionPropertyNode->entityNodeContainingThisProperty = newCondition;	//added 26 Aug 11a, removed 8 Dec 2011

		newCondition->PropertyNodeList.push_back(conditionPropertyNode);	

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}					
	#endif		
}


void addConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyNode->disabled))
	{
	if(!(propertyConditionEntity->disabled))
	{	
	if(!(conditionTypeConceptEntity->disabled))
	{					
	#endif
		
		GIAEntityNode * newCondition = addCondition(conditionTypeConceptEntity);

		newCondition->conditionSubjectEntity = propertyNode;
		newCondition->conditionObjectEntity = propertyConditionEntity;

		propertyNode->ConditionNodeList.push_back(newCondition);
		propertyConditionEntity->IncomingConditionNodeList.push_back(newCondition);
		
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}				
	#endif		
}

GIAEntityNode * addCondition(GIAEntityNode * conditionEntity)
{
	GIAEntityNode * newCondition = new GIAEntityNode();
	newCondition->id = currentEntityNodeIDInCompleteList;
	newCondition->idSecondary = currentEntityNodeIDInConditionEntityNodesList;

	entityNodesCompleteList->push_back(newCondition);
	currentEntityNodeIDInCompleteList++;
	conditionEntityNodesList->push_back(newCondition);
	currentEntityNodeIDInConditionEntityNodesList++;

	newCondition->entityName = conditionEntity->entityName;		
	newCondition->entityNodeDefiningThisInstance = conditionEntity;

	conditionEntity->AssociatedInstanceNodeList.push_back(newCondition);
	conditionEntity->hasAssociatedInstance = true;
	conditionEntity->hasAssociatedInstanceIsCondition = true;
	conditionEntity->hasAssociatedInstanceTemp = true;
	newCondition->isCondition = true;
	newCondition->negative = conditionEntity->negative;
	
	return newCondition;	
}


string convertStanfordPrepositionToRelex(string * preposition, int NLPdependencyRelationsType, bool * stanfordPrepositionFound)
{
	string relexPreposition = *preposition;
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		int foundStanfordPrepositionPrepend = preposition->find(STANFORD_PARSER_PREPOSITION_PREPEND);	
		if(foundStanfordPrepositionPrepend == string::npos)
		{
			/*
			#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
			cout << "convertStanfordPrepositionToRelex(): error - preposition 'prep_...' not found" << endl;
			cout << "the following dependency relation was expected to be a preposition = " << *preposition << endl;
			exit(0);
			#endif
			*/
		}
		else
		{
			int indexOfFirstRealCharacterInPreposition = STANFORD_PARSER_PREPOSITION_PREPEND_LENGTH;
			int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
			relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
			*stanfordPrepositionFound = true;
		}
	}
	return relexPreposition;
}


void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList)
{
	entityNodesCompleteList = newEntityNodesCompleteList;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList)
{
	conceptEntityNodesList = newConceptEntityNodesList;
}
*/
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList)
{
	propertyEntityNodesList = newPropertyEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList)
{
	actionEntityNodesList = newActionEntityNodesList;
}
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList)
{
	conditionEntityNodesList = newConditionEntityNodesList;
}



vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList()
{
	return entityNodesCompleteList;
}
/*
vector<GIAEntityNode*> * getTranslatorConceptEntityNodesList()
{
	return conceptEntityNodesList;
}
*/
vector<GIAEntityNode*> * getTranslatorPropertyEntityNodesList()
{
	return propertyEntityNodesList;
}
vector<GIAEntityNode*> * getTranslatorActionEntityNodesList()
{
	return actionEntityNodesList;
}
vector<GIAEntityNode*> * getTranslatorConditionEntityNodesList()
{
	return conditionEntityNodesList;
}

	
bool getFoundComparisonVariable()
{
	return foundComparisonVariable;
}
GIAEntityNode* getComparisonVariableNode()
{
	return comparisonVariableNode;
}
void setFoundComparisonVariable(bool value)
{
	foundComparisonVariable = value;
}
void setComparisonVariableNode(GIAEntityNode* newComparisonVariableNode)
{
	comparisonVariableNode = newComparisonVariableNode;
}



/*
void setCurrentEntityNodeIDInCompleteList(long newCurrentEntityNodeIDInCompleteList)
{
	currentEntityNodeIDInCompleteList = newCurrentEntityNodeIDInCompleteList;
}
void setCurrentEntityNodeIDInConceptEntityNodesList(long newCurrentEntityNodeIDInConceptEntityNodesList)
{
	currentEntityNodeIDInConceptEntityNodesList = newCurrentEntityNodeIDInConceptEntityNodesList;
}
void setCurrentEntityNodeIDInPropertyEntityNodesList(long newCurrentEntityNodeIDInPropertyEntityNodesList)
{
	currentEntityNodeIDInPropertyEntityNodesList = newCurrentEntityNodeIDInPropertyEntityNodesList;
}
void setCurrentEntityNodeIDInActionEntityNodesList(long newCurrentEntityNodeIDInActionEntityNodesList)
{
	currentEntityNodeIDInActionEntityNodesList = newCurrentEntityNodeIDInActionEntityNodesList;
}
void setCurrentEntityNodeIDInConditionEntityNodesList(long newCurrentEntityNodeIDInConditionEntityNodesList)
{
	currentEntityNodeIDInConditionEntityNodesList = newCurrentEntityNodeIDInConditionEntityNodesList;
}
void incrementCurrentEntityNodeIDInCompleteList()
{
	currentEntityNodeIDInCompleteList = currentEntityNodeIDInCompleteList + 1;
}
void incrementCurrentEntityNodeIDInConceptEntityNodesList()
{
	currentEntityNodeIDInConceptEntityNodesList = currentEntityNodeIDInConceptEntityNodesList + 1;
}
void incrementCurrentEntityNodeIDInPropertyEntityNodesList()
{
	currentEntityNodeIDInPropertyEntityNodesList = currentEntityNodeIDInPropertyEntityNodesList + 1;
}
void incrementCurrentEntityNodeIDInActionEntityNodesList()
{
	currentEntityNodeIDInActionEntityNodesList = currentEntityNodeIDInActionEntityNodesList + 1;
}
void incrementCurrentEntityNodeIDInConditionEntityNodesList()
{
	currentEntityNodeIDInConditionEntityNodesList = currentEntityNodeIDInConditionEntityNodesList + 1;
}
long getCurrentEntityNodeIDInCompleteList()
{
	return currentEntityNodeIDInCompleteList;
}
long getCurrentEntityNodeIDInConceptEntityNodesList()
{
	return currentEntityNodeIDInConceptEntityNodesList;
}
long getCurrentEntityNodeIDInPropertyEntityNodesList()
{
	return currentEntityNodeIDInPropertyEntityNodesList;
}
long getCurrentEntityNodeIDInActionEntityNodesList()
{
	return currentEntityNodeIDInActionEntityNodesList;
}
long getCurrentEntityNodeIDInConditionEntityNodesList()
{
	return currentEntityNodeIDInConditionEntityNodesList;
}
*/
long * getCurrentEntityNodeIDInCompleteList()
{
	return &currentEntityNodeIDInCompleteList;
}
long * getCurrentEntityNodeIDInConceptEntityNodesList()
{
	return &currentEntityNodeIDInConceptEntityNodesList;
}
long * getCurrentEntityNodeIDInPropertyEntityNodesList()
{
	return &currentEntityNodeIDInPropertyEntityNodesList;
}
long * getCurrentEntityNodeIDInActionEntityNodesList()
{
	return &currentEntityNodeIDInActionEntityNodesList;
}
long * getCurrentEntityNodeIDInConditionEntityNodesList()
{
	return &currentEntityNodeIDInConditionEntityNodesList;
}


void applyEntityAlreadyExistsFunction(GIAEntityNode * entity)
{
	entity->disabled = false;	
}

void disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	/*not possible, as property construction routines rely upon concept entity disabled information;
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->disabled = true;
	}
	*/
	entity->disabled = true;
	#endif
}
				
void disableEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;
	
	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the current context (eg sentence)
	//if(entity->AssociatedInstanceNodeList.size() >= 1)
	{	
		//cout << "entity->AssociatedInstanceNodeList.back()->disabled = " << entity->AssociatedInstanceNodeList.back()->entityName << endl;	
		entity->AssociatedInstanceNodeList.back()->disabled = true;	//and disable their associated instances (property nodes)
	}	
	#endif
}


/*PLAN SOMETHING LIKE THIS FOR FUTURE;
void applyEntityAlreadyExistsFunction(GIAEntityNode * entity)
{
	entity->disabled = false;	//probably not necessary [added 13 April 2012 for uniformity]
	if(!(entity->globalDisabled))
	{
		entity->firstSentenceToAppearInNetwork = false;
	}
	else
	{
		entity->firstSentenceToAppearInNetwork = true;
		entity->globalDisabled = false;
	}	
}

void disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		entity->globalDisabled = true;
	}
	entity->disabled = true;
	#endif
}
				
void disableEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		//cout << "entity->disabled = " << entity->entityName << endl;
		entity->globalDisabled = true;
	}
	entity->disabled = true;
	
	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the current context (eg sentence)
	//if(entity->AssociatedInstanceNodeList.size() >= 1)
	{	
		//cout << "entity->AssociatedInstanceNodeList.back()->disabled = " << entity->AssociatedInstanceNodeList.back()->entityName << endl;	
		entity->AssociatedInstanceNodeList.back()->disabled = true;	//and disable their associated instances (property nodes)
	}	
	#endif
}
*/


void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{ //condition required as GIAEntityNodeArrayFilled is not always true. With GRAMMATICAL_DEFINITE, eg "Mr" of "Mr Smith" will still be interpreted as a definite
			GIAEntityNode * entity = GIAEntityNodeArray[i];
			if(!(entity->disabled))
			{
				entity->permanentConcept = true;
			}
		}
	}
}

//(used for printing/xml write purposes)
void recordConceptNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++) 
	{	
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->permanentConcept))
		{
			entityNode->disabled = true;
		}		
	}	
}

//(used for printing/xml write purposes)
void recordConceptNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++) 
	{	
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(entityNode->disabled)
		{
			entityNode->permanentConcept = false;
		}
		else
		{
			entityNode->permanentConcept = true;
		}		
	}	
}

void convertRelexPOSTypeToWordnetWordType(string * relexPOStype, int * wordNetPOS)
{
	*wordNetPOS = GRAMMATICAL_WORD_TYPE_UNDEFINED;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOSTypeArray[i] == *relexPOStype)
		{
			*wordNetPOS = featureRelexPOSTypeCrossReferenceWordnetWordTypeArray[i];
		}
	}
	
	/*
	#ifdef GIA_WORDNET_DEBUG
	cout << "convertRelexPOSTypeToWordnetWordType(): " << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;	
	#endif
	*/
}

void convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(string * POStag, string * relexPOStype, int * wordNetPOS)
{	
	*relexPOStype = FEATURE_RELEX_POS_TYPE_WORD;
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL; i++)
	{
		if(featurePOSTagMinimalArray[i] == *POStag)
		{
			*relexPOStype = featurePOSTagCrossReferenceRelexPOSTypeArray[i];
		}
	}
	
	convertRelexPOSTypeToWordnetWordType(relexPOStype, wordNetPOS);

	/*
	#ifdef GIA_WORDNET_DEBUG
	cout << "convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(): " << endl;
	cout << "POStag = " << *POStag << endl;
	cout << "relexPOStype = " << *relexPOStype << endl;
	cout << "wordNetPOS = " << *wordNetPOS << endl;
	#endif
	*/
}

void generateTempFeatureArray(Feature * firstFeatureInList, Feature * featureArrayTemp[])
{
	//generateFeatureArray;
	int featureIndex = 1;
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		featureArrayTemp[featureIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
		featureIndex++;
	}
}


