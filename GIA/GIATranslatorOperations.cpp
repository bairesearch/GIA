/*******************************************************************************
 *
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4a 21-July-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"

static long currentEntityNodeIDInCompleteList;				//For GIA XML generation only
static long currentEntityNodeIDInConceptEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInPropertyEntityNodesList;		//For GIA XML generation only
static long currentEntityNodeIDInActionEntityNodesList;			//For GIA XML generation only
static long currentEntityNodeIDInConditionEntityNodesList;		//For GIA XML generation only

static vector<GIAEntityNode*> * entityNodesActiveListComplete;		//For GIA XML generation only
//static vector<GIAEntityNode*> * entityNodesActiveListConcepts;
static vector<GIAEntityNode*> * entityNodesActiveListProperties;		//For GIA XML generation only
static vector<GIAEntityNode*> * entityNodesActiveListActions;			//For GIA XML generation only
static vector<GIAEntityNode*> * entityNodesActiveListConditions;		//For GIA XML generation only

static bool saveNetwork; 						//For GIA XML generation only / GIA Advanced referencing only
static long currentEntityNodeIDInSentenceCompleteList;			//For GIA Advanced referencing only
static long currentEntityNodeIDInSentenceConceptEntityNodesList;	//For GIA Advanced referencing only

static bool foundComparisonVariable;
static GIAEntityNode* comparisonVariableNode;




void initialiseGIATranslatorForTexualContextOperations()
{
	saveNetwork = true;

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
	//initialise entityNodesActiveListConcepts;	[should be moved elsewhere]
	string firstEntityNameInNetwork = "universe";
	GIAEntityNode * firstEntityNodeInNetwork = new GIAEntityNode();
	firstEntityNodeInNetwork->entityName = firstEntityNameInNetwork;
	entityNodesActiveListConcepts->push_back(firstEntityNodeInNetwork);
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


	//initialise timeConditionNodesActiveList;		[should be moved elsewhere]
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIATimeConditionNode * firstTimeNodeInNetwork = new GIATimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	timeConditionNodesActiveList->push_back(firstTimeNodeInNetwork);
	timeConditionNumbersActiveList->push_back(firstTimeInNetwork);


	//DEBUG
	//bool resultTemp = true;
	//string tempName = "zh";
	//long findIndex = -1;
	//GIAEntityNode * tempEntity = findOrAddConceptEntityNodeByName(entityNodesActiveListConcepts, &tempName, &resultTemp, &findIndex);
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
			disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[relationGovernorIndex]);
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



GIAEntityNode * addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingProperty = propertyEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{
	if(!(thingEntity->disabled))
	{
	#endif
		if(!(propertyEntity->isConcept))
		{
			GIAEntityNode * existingProperty = propertyEntity;

			/*
			if(propertyEntity->hasQualityTemp)
			{
				existingProperty->hasQuality = true;
			}
			*/

			thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this property
			writeVectorConnection(thingEntity, existingProperty, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(existingProperty, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);

			newOrExistingProperty = existingProperty;

		}
		else
		{
			//cout << "thingEntity = " << thingEntity->entityName << endl;

			GIAEntityNode * newProperty = addProperty(propertyEntity);

			/*
			if(propertyEntity->hasQualityTemp)
			{
				newProperty->hasQuality = true;
			}
			*/

			writeVectorConnection(newProperty, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(thingEntity, newProperty, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingProperty = newProperty;
		}
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingProperty;
}

GIAEntityNode * addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{
	//cout << "df1" << endl;
	GIAEntityNode * newOrExistingProperty = propertyEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(propertyEntity->disabled))
	{
	#endif
		if(propertyEntity->isConcept)
		{
			//cout << "\t\t addPropertyToPropertyDefinition; propertyEntity->entityName = " << propertyEntity->entityName << endl;

			#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
				/*
				NB GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING is not perfect; it cannot link the blue chicken references together
				A blue chicken is small.
				A red chicken is fat.
				The green chicken ate the pie.
				The blue chicken is late.
				*/
				#ifdef GIA_ENABLE_REFERENCE_LINKING_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
				if(checkEntityHasPropertyThatWasDeclaredInContext(propertyEntity) && (propertyEntity->grammaticalDefiniteTemp || propertyEntity->grammaticalRelexPersonOrStanfordProperNounTemp))	//NB the grammaticalRelexPersonOrStanfordProperNounTemp condition should only be required here if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS is set to true
				#else
				if(checkEntityHasPropertyThatWasDeclaredInContext(propertyEntity))
				#endif
				{
					//cout << "\tbreak; assigning: propertyEntity->entityName = " << propertyEntity->entityName << endl;
					newOrExistingProperty = getEntityPropertyThatWasDeclaredInContext(propertyEntity);
					/*
					if(newOrExistingProperty->isConcept)
					{
						cout << "\tERROR" << endl;
					}
					*/
				}
				else
				{
					//cout << "\tbreak; adding: propertyEntity->entityName = " << propertyEntity->entityName << endl;
					newOrExistingProperty = addProperty(propertyEntity);
				}
			#else
				//cout << "\tbreak2; propertyEntity->entityName = " << propertyEntity->entityName << endl;
				newOrExistingProperty = addProperty(propertyEntity);

				/*
				cout << "addPropertyToPropertyDefinition():" << endl;
				cout << "\tnewOrExistingProperty->entityName = " << newOrExistingProperty->entityName << endl;
				*/
			#endif
		}
		else
		{
			newOrExistingProperty = propertyEntity;
		}

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif
	//cout << "df2" << endl;

	return newOrExistingProperty;
}

void forwardTimeInfoToNewProperty(GIAEntityNode * entity, GIAEntityNode * newProperty)
{
	if(entity->hasAssociatedTime)
	{
		newProperty->hasAssociatedTime = true;
	}
	newProperty->NormalizedNERTemp = entity->NormalizedNERTemp;	//always required (not just for time info / time condition related)
	newProperty->NERTemp = entity->NERTemp;

	if(entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true)
	{
		newProperty->hasProgressiveTemp = true;
		//cout << "property has progressive (eg lying/sitting/being happy)" << endl;
	}

	if(entity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT || newProperty->hasProgressiveTemp)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToProperty(newProperty, entity->grammaticalTenseTemp, newProperty->hasProgressiveTemp);
	}
}

GIAEntityNode * addProperty(GIAEntityNode * entity)
{

	//configure property node
	GIAEntityNode * newProperty = new GIAEntityNode();
	#ifdef GIA_USE_DATABASE
	newProperty->added = true;
	#endif

	if(saveNetwork)
	{
		newProperty->idActiveList = currentEntityNodeIDInCompleteList;
		newProperty->idActiveEntityTypeList = currentEntityNodeIDInPropertyEntityNodesList;

		entityNodesActiveListComplete->push_back(newProperty);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListProperties->push_back(newProperty);
		currentEntityNodeIDInPropertyEntityNodesList++;
	}
	else
	{
		newProperty->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newProperty->entityName = entity->entityName;
	newProperty->idInstance = determineNextIdInstance(entity);
	newProperty->isProperty = true;

	entity->hasAssociatedInstance = true;
	entity->hasAssociatedInstanceTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	newProperty->wordNetPOS = entity->wordNetPOS;
	//if(entity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
	//{
	newProperty->grammaticalNumber = entity->grammaticalNumberTemp;
	//}
	forwardTimeInfoToNewProperty(entity, newProperty);

	writeVectorConnection(newProperty, entity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(entity, newProperty, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	entity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	//cout << "entity->entityName = " << entity->entityName << endl;
	if(entity->isQuery)
	{
		entity->isQuery = false;
		newProperty->isQuery = true;
		#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED
		if(entity->isWhichQuery)
		{
			entity->isWhichQuery = false;
			newProperty->isWhichQuery = true;
		}
		#endif
	}
	#endif


	return newProperty;
}


void upgradePropertyToAction(GIAEntityNode * property)
{
	GIAEntityNode * existingAction = property;

	//CHECK THIS; must remove from property list, and add to action list
	(existingAction->entityNodeDefiningThisInstance->back())->entity->hasAssociatedInstanceIsAction = true;
	existingAction->isProperty = false;
	existingAction->isAction = true;

	if(saveNetwork)
	{
		//cout << "existingAction->idActiveEntityTypeList = " << existingAction->idActiveEntityTypeList << endl;
		int i=0;
		bool propertyEntityNodesListIteratorIsFound = false;
 		vector<GIAEntityNode*>::iterator propertyEntityNodesListIteratorFound;
		for(vector<GIAEntityNode*>::iterator propertyEntityNodesListIterator = entityNodesActiveListProperties->begin(); propertyEntityNodesListIterator != entityNodesActiveListProperties->end(); propertyEntityNodesListIterator++)
		{
			if((*propertyEntityNodesListIterator)->idActiveEntityTypeList == existingAction->idActiveEntityTypeList)
			{
				propertyEntityNodesListIteratorFound = propertyEntityNodesListIterator;
				propertyEntityNodesListIteratorIsFound = true;
			}
			//cout << "i = " << i << endl;
			//cout << "(*propertyEntityNodesListIterator)->entityName = " << (*propertyEntityNodesListIterator)->entityName << endl;
			i++;
		}
		if(!propertyEntityNodesListIteratorIsFound)
		{
			cout << "error: !...EntityNodesListIteratorIsFound" << endl;
			exit(0);
		}
		entityNodesActiveListProperties->erase(propertyEntityNodesListIteratorFound);

		/*//removed 8 May 2012
		vector<GIAEntityNode*>::iterator propertyEntityNodesListIterator = entityNodesActiveListProperties->begin();
		advance(propertyEntityNodesListIterator,existingAction->idActiveEntityTypeList);
		entityNodesActiveListProperties->erase(propertyEntityNodesListIterator);
		currentEntityNodeIDInPropertyEntityNodesList--;
		*/

		entityNodesActiveListActions->push_back(existingAction);
		currentEntityNodeIDInActionEntityNodesList++;
	}
}

GIAEntityNode * addActionToActionDefinitionDefineProperties(GIAEntityNode * actionEntity)
{
	//cout << "SD1" << endl;

	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(actionEntity->disabled))
	{
	#endif
		//configure action node
		if(actionEntity->isConcept)
		{
			#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
				#ifdef GIA_ENABLE_REFERENCE_LINKING_DO_NOT_USE_IF_REFERENCE_IS_NOT_DEFINITE_OR_PROPER_NOUN
				if(checkEntityHasPropertyThatWasDeclaredInContext(actionEntity) && (actionEntity->grammaticalDefiniteTemp || actionEntity->grammaticalRelexPersonOrStanfordProperNounTemp))	//NB the grammaticalRelexPersonOrStanfordProperNounTemp condition should only be required here if GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS is set to true
				#else
				if(checkEntityHasPropertyThatWasDeclaredInContext(actionEntity))
				#endif
				{
					//cout << "\tbreak; actionEntity->entityName = " << actionEntity->entityName << endl;
					newOrExistingAction = getEntityPropertyThatWasDeclaredInContext(actionEntity);
				}
				else
				{
					newOrExistingAction = addAction(actionEntity);
				}
			#else
				//cout << "\tbreak2; actionEntity->entityName = " << actionEntity->entityName << endl;
				newOrExistingAction = addAction(actionEntity);
			#endif
		}
		else
		{
			if(actionEntity->isAction == false)
			{//upgrade associated property to action
				upgradePropertyToAction(actionEntity);
				newOrExistingAction = actionEntity;
			}
			else
			{
				#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
				newOrExistingAction = addAction(actionEntity);
				#else
				newOrExistingAction = actionEntity;
				#endif
			}
		}

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	#endif

	//cout << "SD2" << endl;

	return newOrExistingAction;
}


GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(actionEntity->disabled))
	{
	#endif
		//configure action node
		if(actionEntity->isConcept)
		{
			//always add new actions per sentence (never use actions defined from previous sentences)
			newOrExistingAction = addAction(actionEntity);
		}
		else
		{
			if(actionEntity->isAction == false)
			{//upgrade associated property to action
				upgradePropertyToAction(actionEntity);
				newOrExistingAction = actionEntity;
			}
			else
			{
				#ifdef USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_ENTITY_INDEX_IN_A_GIVEN_SENTENCE
				newOrExistingAction = addAction(actionEntity);
				#else
				newOrExistingAction = actionEntity;
				#endif
			}
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
	#ifdef GIA_USE_DATABASE
	newAction->added = true;
	#endif

	if(saveNetwork)
	{
		newAction->idActiveList = currentEntityNodeIDInCompleteList;
		newAction->idActiveEntityTypeList = currentEntityNodeIDInActionEntityNodesList;

		entityNodesActiveListComplete->push_back(newAction);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListActions->push_back(newAction);
		currentEntityNodeIDInActionEntityNodesList++;
	}
	else
	{
		newAction->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newAction->entityName = actionEntity->entityName;
	newAction->idInstance = determineNextIdInstance(actionEntity);

	writeVectorConnection(newAction, actionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(actionEntity, newAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	actionEntity->hasAssociatedInstance = true;
	actionEntity->hasAssociatedInstanceIsAction = true;
	actionEntity->hasAssociatedInstanceTemp = true;
	newAction->wordNetPOS = actionEntity->wordNetPOS;
	newAction->isAction = true;
	//WHY WOULD THIS EVER BE REQURIED?; newAction->entityNodeContainingThisProperty = NULL;

	forwardTimeInfoToNewProperty(actionEntity, newAction);

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

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	actionEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	#endif

	//cout << "as5" << endl;

	return newAction;
}







/*
#ifdef GIA_USE_TIME_NODE_INDEXING
void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, vector<GIATimeConditionNode*> *timeConditionNodesActiveList, vector<long> *timeConditionNumbersActiveList)
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
	GIATimeConditionNode * newTimeCondition = findOrAddTimeNodeByNumber(timeConditionNodesActiveList, conceptEntityNamesList, timeConditionAbsoluteTimeValue, &argumentEntityAlreadyExistant, &timeConditionEntityIndex, true);
	#else
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	#endif
	*/

	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();

	newTimeCondition->tense = tense;
	newTimeCondition->tenseOnlyTimeCondition = true;
	newTimeCondition->conditionName = grammaticalTenseNameArray[tense];
	if(isProgressive)
	{
		newTimeCondition->isProgressive = true;
	}
	propertyNode->timeConditionNode = newTimeCondition;


}

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity, bool sameReferenceSet)
{
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(thingEntity->disabled))
	{
	if(!(definitionEntity->disabled))
	{
	#endif

		//configure entity node and entity definition node
		writeVectorConnection(thingEntity, definitionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
		writeVectorConnection(definitionEntity, thingEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
GIAEntityNode * addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity, bool sameReferenceSetSubject, bool sameReferenceSetObject)
{
	GIAEntityNode * newOrExistingAction = actionEntity;
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif

	newOrExistingAction = addActionToActionDefinition(actionEntity);

	addActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSetSubject);

	addActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSetObject);

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif
	return newOrExistingAction;
}


void addActionInstanceToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure subject entity node
	writeVectorConnection(subjectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, subjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, sameReferenceSet);

	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;

	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}


GIAEntityNode * addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(subjectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		addActionInstanceToSubject(subjectEntity, newOrExistingAction, sameReferenceSet);
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}

void addActionInstanceToObject(GIAEntityNode * objectEntity, GIAEntityNode * newOrExistingAction, bool sameReferenceSet)
{
	//configure object entity node
	writeVectorConnection(objectEntity, newOrExistingAction, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, sameReferenceSet);
	writeVectorConnection(newOrExistingAction, objectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, sameReferenceSet);

	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

GIAEntityNode * addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingAction = actionEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(objectEntity->disabled))
	{
	if(!(actionEntity->disabled))
	{
	#endif
		//cout << "vf3a" << endl;
		newOrExistingAction = addActionToActionDefinition(actionEntity);
		//cout << "vf3b" << endl;
		addActionInstanceToObject(objectEntity, newOrExistingAction, sameReferenceSet);
	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	#endif

	return newOrExistingAction;
}


GIAEntityNode * addOrConnectConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeEntity, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionEntityNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif
		if(!(conditionTypeEntity->isConcept))
		{
			//cout << "existingCondition" << endl;

			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this property
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionEntityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionEntityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			//cout << "newCondition" << endl;

			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(newCondition, conditionEntityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionEntityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, sameReferenceSet);

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	/*
	}
	else
	{
		cout << "(conditionTypeConceptEntity->disabled)" << endl;
	}
	}
	else
	{
		cout << "(conditionEntityNode->disabled)" << endl;
	}
	}
	else
	{
		cout << "(entityNode->disabled)" << endl;
	}
	*/
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionDefinitionNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif

		if(!(conditionTypeEntity->isConcept))
		{
			//cout << "existingCondition" << endl;

			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this property
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionDefinitionNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
			writeVectorConnection(conditionDefinitionNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			//cout << "newCondition" << endl;

			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);
			newCondition->negative = negative;	//overwrite negative with orrect one from context; ie that from "being" entity node
			//cout << "negative = " << negative;

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(newCondition, conditionDefinitionNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, sameReferenceSet);
			writeVectorConnection(conditionDefinitionNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, sameReferenceSet);

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionPropertyNode, GIAEntityNode * conditionTypeEntity, bool negative, bool sameReferenceSet)
{
	GIAEntityNode * newOrExistingCondition = conditionTypeEntity;

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	if(!(entityNode->disabled))
	{
	if(!(conditionPropertyNode->disabled))
	{
	if(!(conditionTypeEntity->disabled))
	{
	#endif

		if(!(conditionTypeEntity->isConcept))
		{
			//cout << "existingCondition" << endl;

			GIAEntityNode * existingCondition = conditionTypeEntity;

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			//configure entity node containing this property
			writeVectorConnection(existingCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionPropertyNode, existingCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(existingCondition, conditionPropertyNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			newOrExistingCondition = existingCondition;

		}
		else
		{
			//cout << "newCondition" << endl;

			GIAEntityNode * newCondition = addCondition(conditionTypeEntity);
			newCondition->negative = negative;	//overwrite negative with correct one from context; ie that from "having" entity node

			writeVectorConnection(newCondition, entityNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, sameReferenceSet);
			writeVectorConnection(entityNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, sameReferenceSet);
			writeVectorConnection(conditionPropertyNode, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, sameReferenceSet);
			writeVectorConnection(newCondition, conditionPropertyNode, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, sameReferenceSet);

			//entityNode->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

			newOrExistingCondition = newCondition;
		}

	#ifdef GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES
	}
	}
	}
	#endif

	return newOrExistingCondition;
}

GIAEntityNode * addCondition(GIAEntityNode * conditionEntity)
{
	/*
	if(!(conditionEntity->isConcept))
	{
		cout << "error: !(conditionEntity->isConcept)" << endl;
	}
	*/

	GIAEntityNode * newCondition = new GIAEntityNode();
	#ifdef GIA_USE_DATABASE
	newCondition->added = true;
	#endif

	if(saveNetwork)
	{
		newCondition->idActiveList = currentEntityNodeIDInCompleteList;
		newCondition->idActiveEntityTypeList = currentEntityNodeIDInConditionEntityNodesList;

		entityNodesActiveListComplete->push_back(newCondition);
		currentEntityNodeIDInCompleteList++;
		entityNodesActiveListConditions->push_back(newCondition);
		currentEntityNodeIDInConditionEntityNodesList++;
	}
	else
	{
		newCondition->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}

	newCondition->entityName = conditionEntity->entityName;
	newCondition->idInstance = determineNextIdInstance(conditionEntity);

	writeVectorConnection(newCondition, conditionEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);
	writeVectorConnection(conditionEntity, newCondition, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES, VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN);

	conditionEntity->hasAssociatedInstance = true;
	conditionEntity->hasAssociatedInstanceIsCondition = true;
	conditionEntity->hasAssociatedInstanceTemp = true;
	newCondition->isCondition = true;
	newCondition->negative = conditionEntity->negative;

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	conditionEntity->entityAlreadyDeclaredInThisContext = true;	//added 9 May 2012
	#endif

	return newCondition;
}


string convertStanfordPrepositionToRelex(string * preposition, int NLPdependencyRelationsType, bool * stanfordPrepositionFound)
{
	string relexPreposition = *preposition;
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		string stanfordPrepositionPrependFound = "";
		bool multiwordPrepositionIntermediaryRelationTypeBFound = false;
		for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
		{
			string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
			int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

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
				int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
				int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
				relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
				*stanfordPrepositionFound = true;
			}
		}
	}
	return relexPreposition;
}


void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList)
{
	entityNodesActiveListComplete = newEntityNodesCompleteList;
}
/*
void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList)
{
	entityNodesActiveListConcepts = newConceptEntityNodesList;
}
*/
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList)
{
	entityNodesActiveListProperties = newPropertyEntityNodesList;
}
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList)
{
	entityNodesActiveListActions = newActionEntityNodesList;
}
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList)
{
	entityNodesActiveListConditions = newConditionEntityNodesList;
}



vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList()
{
	return entityNodesActiveListComplete;
}
/*
vector<GIAEntityNode*> * getTranslatorConceptEntityNodesList()
{
	return entityNodesActiveListConcepts;
}
*/
vector<GIAEntityNode*> * getTranslatorPropertyEntityNodesList()
{
	return entityNodesActiveListProperties;
}
vector<GIAEntityNode*> * getTranslatorActionEntityNodesList()
{
	return entityNodesActiveListActions;
}
vector<GIAEntityNode*> * getTranslatorConditionEntityNodesList()
{
	return entityNodesActiveListConditions;
}

void setSaveNetwork(bool val)
{
	saveNetwork = val;
}
bool getSaveNetwork()
{
	return saveNetwork;
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

long * getCurrentEntityNodeIDInSentenceCompleteList()
{
	return &currentEntityNodeIDInSentenceCompleteList;
}
long * getCurrentEntityNodeIDInSentenceConceptEntityNodesList()
{
	return &currentEntityNodeIDInSentenceConceptEntityNodesList;
}



void applyConceptEntityAlreadyExistsFunction(GIAEntityNode * entity, bool entityAlreadyExistant)
{
	if(entityAlreadyExistant)
	{
		//cout << "entityAlreadyExistant" << endl;
		entity->disabled = false;
		entity->firstSentenceToAppearInNetwork = false;
	}
	else
	{
		//cout << "!entityAlreadyExistant" << endl;
		entity->firstSentenceToAppearInNetwork	= true;
	}
}

void disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	if(entity->firstSentenceToAppearInNetwork)
	{
		//cout << "firstSentenceToAppearInNetwork" << endl;
		entity->disabled = true;
	}
	#endif
}

void disableEntity(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;
	#endif
}


void disableConceptEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(entity);	//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)

	if(entity->hasAssociatedInstanceTemp)	//CHECKTHIS; only disable the instance if it was created in the immediate context (eg sentence)
	//if(entity->AssociatedInstanceNodeList.size() >= 1)
	{
		GIAEntityNode * associateInstance = (entity->AssociatedInstanceNodeList->back())->entity;
		//cout << "associateInstance->disabled = " << associateInstance->entityName << endl;
		associateInstance->disabled = true;	//and disable their associated instances (property nodes)
	}
	#endif
}

void disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity)
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
	entity->disabled = true;
	
	if(!(entity->entityNodeDefiningThisInstance->empty()))	
	{
		//CHECKTHIS; only disable the concept if it was created in the immediate context (eg sentence)
		GIAEntityNode * conceptEntity = (entity->entityNodeDefiningThisInstance->back())->entity;
		
		//cout << "disableConceptEntityBasedUponFirstSentenceToAppearInNetwork( = " << conceptEntity->entityName << endl;
		disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(conceptEntity);
	}
	#endif
}



void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap)
{
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		if(!(entityNode->disabled))
		{
			entityNode->permanentConcept = true;
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
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		//cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
		featureArrayTemp[currentFeatureInList->entityIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
	}

	/*OLD: is not compatible with Relex query nodes (set to new entity index eg "199")
	//generateFeatureArray;
	int featureIndex = 1;
	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		featureArrayTemp[featureIndex] = currentFeatureInList;
		currentFeatureInList = currentFeatureInList->next;
		featureIndex++;
	}
	*/
}

#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING

/*
//NOT REQUIRED: redundant: this unique check is redundant considering if a concept entity has a property that was declared in the immediate context, ie sentence, then the entity node being queried will be the property itself (and not its concept)
bool checkEntityHasPropertyThatWasDeclaredInContextAndIsUnique(GIAEntityNode * entityNode, int entityIndexTemp, int sentenceIndexTemp)
{
	bool result = false;

	if(checkEntityHasPropertyThatWasDeclaredInContext(entityNode))
	{
		GIAEntityNode * entityPropertyDeclaredInContext = getEntityPropertyThatWasDeclaredInContext(entityNode);
		if(!((entityPropertyDeclaredInContext->entityIndexTemp == entityIndexTemp) && (entityPropertyDeclaredInContext->sentenceIndexTemp == sentenceIndexTemp)))
		{
			result = true;
		}
	}

	return result;
}
*/

bool checkEntityHasPropertyThatWasDeclaredInContext(GIAEntityNode * entityNode)
{
	bool result = false;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		result = true;
	}

	return result;
}

GIAEntityNode * getEntityPropertyThatWasDeclaredInContext(GIAEntityNode * entityNode)
{
	GIAEntityNode * entityNodeAssociatedInstance = entityNode;

	if(entityNode->entityAlreadyDeclaredInThisContext)
	{
		if(!(entityNode->AssociatedInstanceNodeList->empty()))
		{
			entityNodeAssociatedInstance = (entityNode->AssociatedInstanceNodeList->back())->entity;
		}
		else
		{
			cout << "error: entityAlreadyDeclaredInThisContext && (entityNode->AssociatedInstanceNodeList.size() == 0)" << endl;
			cout << "entityNode->entityName = " <<  entityNode->entityName << endl;
			exit(0);
		}
	}

	return entityNodeAssociatedInstance;
}
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
bool determineSameReferenceSetValue(bool defaultSameSetValueForRelation, Relation * relation)
{

	bool auxillaryIndicatesDifferentReferenceSet = relation->auxillaryIndicatesDifferentReferenceSet;
	bool rcmodIndicatesSameReferenceSet = relation->rcmodIndicatesSameReferenceSet;

	bool sameReferenceSet = defaultSameSetValueForRelation;
	if(auxillaryIndicatesDifferentReferenceSet)
	{
		sameReferenceSet = false;
	}
	if(rcmodIndicatesSameReferenceSet)
	{
		sameReferenceSet = true;
	}


	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\ndetermineSameReferenceSetValue():" << endl;
	cout << "\t" << relation->relationType << "(" << relation->relationGovernor << ", " << relation->relationDependent << ")" << endl;
	cout << "\tauxillaryIndicatesDifferentReferenceSet = " << auxillaryIndicatesDifferentReferenceSet << endl;
	cout << "\trcmodIndicatesSameReferenceSet = " << rcmodIndicatesSameReferenceSet << endl;
	cout << "\tdefaultSameSetValueForRelation = " << defaultSameSetValueForRelation << endl;
	cout << "\tsameReferenceSet = " << sameReferenceSet << endl;
	#endif


	return sameReferenceSet;
}
#endif


GIAEntityNode * findOrAddEntityNodeByNameSimpleWrapperCondition(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int featureIndex, string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	GIAEntityNode * conditionTypeEntity;
	#ifdef GIA_USE_ADVANCED_REFERENCING_CONDITIONS
	if(GIAEntityNodeArrayFilled[featureIndex])
	{
		conditionTypeEntity = GIAEntityNodeArray[featureIndex];
	}
	else
	{
		conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
		GIAEntityNodeArrayFilled[featureIndex] = true;
	}
	#else
	conditionTypeEntity = findOrAddConceptEntityNodeByNameSimpleWrapper(entityNodeName, entityAlreadyExistant, entityNodesActiveListConcepts);
	#endif

	return conditionTypeEntity;
}

GIAEntityNode * findOrAddConceptEntityNodeByNameSimpleWrapper(string * entityNodeName, bool * entityAlreadyExistant, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{
	GIAEntityNode * entityNodeFound = NULL;

	/*already available locally...
	long * currentEntityNodeIDInCompleteList;
	long * currentEntityNodeIDInConceptEntityNodesList;
	vector<GIAEntityNode*> * entityNodesActiveListComplete;
	if(saveNetwork)
	{
		entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();
		currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
		currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
	}
	else
	{
		currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInSentenceCompleteList();
		currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInSentenceConceptEntityNodesList();
	}
	*/

	long entityIndex = -1;
	entityNodeFound = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodeName, entityAlreadyExistant, &entityIndex, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList, saveNetwork);
	
	applyConceptEntityAlreadyExistsFunction(entityNodeFound, *entityAlreadyExistant);

	return entityNodeFound;
}



/*these functions have been added for GIA Database compatibility*/
//this function does write to database, but prepares data structures for write to database (at the end of the user sequence, writeDatabase() is written...)
void writeVectorConnection(GIAEntityNode * entityNode, GIAEntityNode * entityNodeToAdd, int connectionType, bool sameReferenceSet)
{
	vector<GIAEntityConnection*> * vectorConnection = &(entityNode->entityVectorConnectionsArray[connectionType]);
	if(entityVectorConnectionIsBasicArray[connectionType])
	{
		vectorConnection->clear();	//clear the vector (basic connections only support 1 node)

	}

	GIAEntityConnection * newConnection = new GIAEntityConnection();
	newConnection->entity = entityNodeToAdd;
	vectorConnection->push_back(newConnection);

	#ifdef GIA_USE_ADVANCED_REFERENCING
	newConnection->sameReferenceSet = sameReferenceSet;
	/*
	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "writeVectorConnection: newConnection->sameReferenceSet = " << sameReferenceSet << endl;
	#endif
	*/
	#endif

	#ifdef GIA_USE_DATABASE
	if((getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_ACTIVE) || (getUseDatabase() == GIA_USE_DATABASE_TRUE_READ_INACTIVE))	//NB even if not accessing the database for new information (read), still prepare nodes for database write
	{
		//#ifdef GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS		//why is this preprocessor check not required???
		//required for database syncronisation with RAM
		if(!(entityNode->entityVectorConnectionsReferenceListLoadedArray[connectionType]))
		{
			cout << "error: writeVectorConnection called, but entityVectorConnectionsReferenceListLoadedArray set to false" << endl;
			exit(0);
		}
		//#endif

		newConnection->entityName = entityNodeToAdd->entityName;
		newConnection->idInstance = entityNodeToAdd->idInstance;
		newConnection->loaded = true;
		newConnection->modified = false;
		newConnection->added = true;		//this allows for fast update of the DB (append reference connections)
	}
	#endif
}

long determineNextIdInstance(GIAEntityNode * entity)
{
	long nextIdInstance;
	#ifdef GIA_DATABASE_DEBUG
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->entityName = " << entity->entityName << endl;
	cout << "\t\tDEBUG: determineNextIdInstance(); 0. entity->idInstance = " << entity->idInstance << endl;
	#endif
	GIAEntityNode * conceptEntity;
	if(!(entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE].empty()))
	{
		//the current entity is a property of a concept entity
		conceptEntity = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE].back()->entity;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 1a. conceptEntity->entityName = " << conceptEntity->entityName << endl;
		cout << "\t\tDEBUG: determineNextIdInstance(); 1a. conceptEntity->idInstance = " << conceptEntity->idInstance << endl;
		#endif
	}
	else
	{
		//the current entity is a concept entity
		conceptEntity = entity;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->entityName = " << conceptEntity->entityName << endl;
		cout << "\t\tDEBUG: determineNextIdInstance(); 1b. conceptEntity->idInstance = " << conceptEntity->idInstance << endl;
		#endif
	}

#if defined(GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS) || !defined(GIA_USE_DATABASE)
	#ifdef GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
	if(!(conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].empty()))
	{
		long previousIdInstance = conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].back()->entity->idInstance;
		nextIdInstance = previousIdInstance + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 2a. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	else
	{
		nextIdInstance = GIA_DATABASE_NODE_CONCEPT_ID_INSTANCE + 1;
		#ifdef GIA_DATABASE_DEBUG
		cout << "\t\tDEBUG: determineNextIdInstance(); 2b. nextIdInstance = " << nextIdInstance << endl;
		#endif
	}
	#else
	long numberOfInstances =  conceptEntity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES].size();
	nextIdInstance = numberOfInstances;
	#endif
#else
	if(conceptEntity->conceptEntityLoaded == NULL)
	{
		cout <<"ERROR: (conceptEntity->conceptEntityLoaded == NULL)" << endl;
	}
	GIAconceptEntityLoaded * conceptEntityLoaded = conceptEntity->conceptEntityLoaded;
	#ifdef GIA_DATABASE_DEBUG
	cout << "conceptEntityLoaded->numberOfInstances = " << conceptEntityLoaded->numberOfInstances << endl;
	#endif
	conceptEntityLoaded->numberOfInstances = conceptEntityLoaded->numberOfInstances + 1;
	nextIdInstance = conceptEntityLoaded->numberOfInstances;
#endif

	return nextIdInstance;
}


#ifdef GIA_USE_DATABASE
void addEntityNodeToActiveLists(GIAEntityNode * entity, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts)
{//NB add reference set entity to active list complete + appropriate list (property/action/condition) [NB the reference set entity will already be added to concept active list entityNodesActiveListConcepts...] - this function enables references to be written to XML

	/*already available locally...
	long * currentEntityNodeIDInCompleteList = getCurrentEntityNodeIDInCompleteList();
	long * currentEntityNodeIDInConceptEntityNodesList = getCurrentEntityNodeIDInConceptEntityNodesList();
	long * currentEntityNodeIDInPropertyEntityNodesList = getCurrentEntityNodeIDInPropertyEntityNodesList();
	long * currentEntityNodeIDInActionEntityNodesList = getCurrentEntityNodeIDInActionEntityNodesList();
	long * currentEntityNodeIDInConditionEntityNodesList = getCurrentEntityNodeIDInConditionEntityNodesList();
	*/

	if(saveNetwork)
	{
		if(entity->isConcept)
		{
			//do nothing - assume reference set entity already added to concept active list (in createGIACoreferenceInListBasedUponIdentifiedReferenceSets())
		}
		else if(entity->isAction)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInActionEntityNodesList;
			entityNodesActiveListActions->push_back(entity);
			currentEntityNodeIDInActionEntityNodesList++;
		}
		else if(entity->isCondition)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInConditionEntityNodesList;
			entityNodesActiveListConditions->push_back(entity);
			currentEntityNodeIDInConditionEntityNodesList++;
		}
		else if(entity->isProperty)
		{
			entity->idActiveEntityTypeList = currentEntityNodeIDInPropertyEntityNodesList;
			entityNodesActiveListProperties->push_back(entity);
			currentEntityNodeIDInPropertyEntityNodesList++;
		}

		entity->idActiveList = currentEntityNodeIDInCompleteList;
		entityNodesActiveListComplete->push_back(entity);
		currentEntityNodeIDInCompleteList++;
	}
	else
	{//this case should never be used...
		entity->idActiveList = currentEntityNodeIDInSentenceCompleteList;
		currentEntityNodeIDInSentenceCompleteList++;
	}
}
#endif

