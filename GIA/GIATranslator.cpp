/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors indexOfEntityNodes/indexOfEntityNames with a map, and replace vectors GIATimeConditionNode/indexOfTimeNumbers with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslator.h"


string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_TO, RELATION_TYPE_OBJECT_TO_BE, RELATION_TYPE_OBJECT_TO_DO, RELATION_TYPE_OBJECT_THAT};
//int relationTypeObjectNameLengthsArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {4, 2, 6, 6};
string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
//int relationTypeSubjectNameLengthsArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {5, 5};
string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_2, RELATION_TYPE_ADJECTIVE_3};
//int relationTypeAdjectiveNameLengthsArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {5, 8, 7};
string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE, RELATION_TYPE_PRENOMIAL_MODIFIER};
//int relationTypePossessiveNameLengthsArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {5, 3};

string relationFunctionCompositionNameArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_COMPOSITION_1, RELATION_FUNCTION_COMPOSITION_2, RELATION_FUNCTION_COMPOSITION_3, RELATION_FUNCTION_COMPOSITION_4};
//int relationFunctionCompositionNameLengthsArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {8, 9, 3};
string relationFunctionDefinitionNameArray[RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_DEFINITION_1};



int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};
string relationTypeAdjectiveWhichImplyEntityInstanceNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_3};
string relationTypeRequireSwitchingNameArray[RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT_THAT};

string relationTypeQuantityOrMeasureNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY, RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};
string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME};


//int timeMonthIntArray[TIME_MONTH_NUMBER_OF_TYPES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
string timeMonthStringArray[TIME_MONTH_NUMBER_OF_TYPES] = {TIME_MONTH_JANUARY, TIME_MONTH_FEBRUARY, TIME_MONTH_MARCH, TIME_MONTH_APRIL, TIME_MONTH_MAY, TIME_MONTH_JUNE, TIME_MONTH_JULY, TIME_MONTH_AUGUST, TIME_MONTH_SEPTEMBER, TIME_MONTH_OCTOBER, TIME_MONTH_NOVEMBER, TIME_MONTH_DECEMBER};
	


/* ORIGINAL v1a;
string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_TO};
//int relationTypeObjectNameLengthsArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {4, 2, 6, 6};
string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT};
//int relationTypeSubjectNameLengthsArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {5, 5};
string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_1, RELATION_TYPE_ADJECTIVE_2, RELATION_TYPE_ADJECTIVE_3};
//int relationTypeAdjectiveNameLengthsArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {5, 8, 7};
string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE};
//int relationTypePossessiveNameLengthsArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {5, 3};
string relationFunctionCompositionNameArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_COMPOSITION_1, RELATION_FUNCTION_COMPOSITION_2, RELATION_FUNCTION_COMPOSITION_3};
//int relationFunctionCompositionNameLengthsArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {8, 9, 3};
*/


string referenceTypePossessiveNameArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {"undefined", "his", "her", "them", "its"};
//int referenceTypePossessiveNameLengthsArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {9, 3, 3, 4, 3};
string referenceTypePersonNameArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {"undefined", "he", "she", "they", "it"};
//int referenceTypePersonNameLengthsArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {9, 2, 3, 4, 2};

int referenceTypePersonCrossReferenceNumberArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_UNDEFINED, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_PLURAL, GRAMMATICAL_NUMBER_SINGULAR};
int referenceTypePersonCrossReferenceGenderArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_MASCULINE, GRAMMATICAL_GENDER_FEMININE, GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_UNDEFINED};
bool referenceTypePersonCrossReferenceDefiniteArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {false, true, true, true, true};
bool referenceTypePersonCrossReferencePersonArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON_UNDEFINED};


static long currentEntityNodeIDInCompleteList;
static long currentActionNodeIDInCompleteList;
static long currentConditionNodeIDInCompleteList;
static vector<GIAEntityNode*> entityNodesCompleteList;
static vector<GIAActionNode*> actionNodesCompleteList;
static vector<GIAConditionNode*> conditionNodesCompleteList;
/*
GIAEntityNode * currentEntityNodeInCompleteList;
GIAActionNode * currentActionNodeInCompleteList;
GIAConditionNode * currentConditionNodeInCompleteList;
*/


vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList()
{
	return &entityNodesCompleteList;
}
vector<GIAActionNode*> * getTranslatorActionNodesCompleteList()
{
	return &actionNodesCompleteList;
}	
vector<GIAConditionNode*> * getTranslatorConditionNodesCompleteList()
{
	return &conditionNodesCompleteList;
}
	
	
void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	if(propertyEntity->hasAssociatedPropertyTemp)
	{
		GIAEntityNode * existingProperty  = propertyEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a

		if(thingEntity->hasAssociatedPropertyTemp)
		{
			thingEntity = thingEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
		}

		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(existingProperty);	

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed

		existingProperty->entityNodeContainingThisProperty = thingEntity;		//added 26 Aug 11a	
	}
	else
	{
		if(thingEntity->hasAssociatedPropertyTemp)
		{
			thingEntity = thingEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
		}
	
		//configure property node
		GIAEntityNode * newProperty = new GIAEntityNode();
		entityNodesCompleteList.push_back(newProperty);
		newProperty->id = currentEntityNodeIDInCompleteList++;
		newProperty->entityName = propertyEntity->entityName;
		newProperty->isProperty = true;
		newProperty->entityNodeContainingThisProperty = thingEntity;
		newProperty->entityNodeDefiningThisProperty = propertyEntity;
		propertyEntity->hasAssociatedProperty = true;
		propertyEntity->hasAssociatedPropertyTemp = true;	////temporary: used for GIA translator only - overwritten every time a new sentence is parsed
		
		//if(propertyEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
		//{
		newProperty->grammaticalNumber = propertyEntity->grammaticalNumberTemp;
		//}
			
		if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
		{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
			addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp);
		}

		//configure property definition node
		propertyEntity->AssociatedPropertyNodeList.push_back(newProperty);
		
		//configure entity node containing this property
		thingEntity->PropertyNodeList.push_back(newProperty);		

		thingEntity->hasPropertyTemp = true;		//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	}
	
}

void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{
	if(propertyEntity->hasAssociatedPropertyTemp && (GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_TO_DEFINITIVE_NOUNS != 1) && (propertyEntity->entityAlreadyDeclaredInThisContext))
	{
		propertyEntity = propertyEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
	else
	{	
		//configure property node
		GIAEntityNode * newProperty = new GIAEntityNode();
		entityNodesCompleteList.push_back(newProperty);
		newProperty->id = currentEntityNodeIDInCompleteList++;
		newProperty->entityName = propertyEntity->entityName;
		newProperty->isProperty = true;
		newProperty->entityNodeContainingThisProperty = NULL;
		newProperty->entityNodeDefiningThisProperty = propertyEntity;
		propertyEntity->hasAssociatedProperty = true;
		propertyEntity->hasAssociatedPropertyTemp = true;

		//if(propertyEntity->grammaticalNumberTemp > GRAMMATICAL_NUMBER_SINGULAR)
		//{
		newProperty->grammaticalNumber = propertyEntity->grammaticalNumberTemp;
		//}				

		if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
		{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
			addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp);
		}

		//configure property definition node
		propertyEntity->AssociatedPropertyNodeList.push_back(newProperty);	
		
		propertyEntity->entityAlreadyDeclaredInThisContext = true;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	}	
}

/*not used
void connnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntityAssociatedProperty)
{
	//configure property node
	propertyEntityAssociatedProperty->entityNodeContainingThisProperty = thingEntity;
	
	//configure entity node containing this property
	thingEntity->PropertyNodeList.push_back(propertyEntityAssociatedProperty);
		
}
*/



void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = grammaticalTenseNameArray[tense];
	newCondition->conditionEntity = NULL;
	newCondition->parentProperty = propertyNode;
	newCondition->parentIsAction = false;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_TIME;
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->tense = tense;
	newCondition->timeConditionNode = newTimeCondition;
	
	propertyNode->ConditionNodeList.push_back(newCondition);
}

void addTenseOnlyTimeConditionToAction(GIAActionNode * actionNode, int tense)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = grammaticalTenseNameArray[tense];
	newCondition->conditionEntity = NULL;
	newCondition->parentAction = actionNode;
	newCondition->parentIsAction = true;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_TIME;
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->tense = tense;
	newCondition->timeConditionNode = newTimeCondition;
		
	actionNode->ConditionNodeList.push_back(newCondition);
}


/*unfinished - needed any more?;
void convertEntityToProperty(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	//configure property node
	propertyEntity->isProperty = true;
	propertyEntity->entityNodeContainingThisProperty = thingEntity;
	newProperty->entityNodeDefiningThisProperty = propertyEntity;
	
	//configure property definition node
	propertyEntity->AssociatedPropertyNodeList.push_back(newProperty);
	
	//configure entity node containing this property
	thingEntity->PropertyNodeList.push_back(newProperty);
}
*/


void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity)
{
	if(definitionEntity->hasAssociatedPropertyTemp)
	{
		definitionEntity = definitionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
	
	//configure entity node
	thingEntity->EntityNodeDefinitionList.push_back(definitionEntity);
	
	//configure entity definition node
	definitionEntity->EntityNodeDefinitionReverseList.push_back(thingEntity);		
}

	//conditions required to be added [eg when, where, how, why]
	//replace action if already existant
void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{
	GIAActionNode * newOrExistingAction;
	
	if(!(actionEntity->hasAssociatedActionTemp))
	{
		newOrExistingAction = addAction(actionEntity);
	}
	else
	{
		if(GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME == 1)
		{
			//cout << "GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME == 1" << endl;
			newOrExistingAction = addAction(actionEntity);
		}
		else
		{
			newOrExistingAction = actionEntity->AssociatedActionNodeList.back();
		}
	}

	addActionToSubject(subjectEntity, actionEntity);

	addActionToObject(objectEntity, actionEntity);

}

	//conditions required to be added [eg when, where, how, why]
GIAActionNode * addAction(GIAEntityNode * actionEntity)
{
	if(actionEntity->hasAssociatedPropertyTemp)
	{
		actionEntity = actionEntity->AssociatedPropertyNodeList.back();	//added 27 Aug 11a
	}
	
		
	//configure action node
	GIAActionNode * newAction = new GIAActionNode();
	actionNodesCompleteList.push_back(newAction);
	newAction->id = currentActionNodeIDInCompleteList++;
	newAction->actionName = actionEntity->entityName;
	newAction->entityNodeDefiningThisAction = actionEntity;
	actionEntity->AssociatedActionNodeList.push_back(newAction);
	actionEntity->hasAssociatedAction = true;
	actionEntity->hasAssociatedActionTemp = true;
	
	//cout << "actionEntity->grammaticalTenseTemp = " << actionEntity->grammaticalTenseTemp << endl;
	//cout << "actionEntity->entityName = " << actionEntity->entityName << endl;

	if(actionEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		//cout << "hello" << endl;
		//exit(0);
		addTenseOnlyTimeConditionToAction(newAction, actionEntity->grammaticalTenseTemp);
	}
	
	return newAction;
}

void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity)
{	
	GIAActionNode * newOrExistingAction;
	if(!(actionEntity->hasAssociatedActionTemp))
	{
		newOrExistingAction = addAction(actionEntity);
	}
	else
	{
		newOrExistingAction = actionEntity->AssociatedActionNodeList.back();
	}

	if(subjectEntity->hasAssociatedPropertyTemp)
	{
		subjectEntity = subjectEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
		
	//configure subject entity node
	subjectEntity->ActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionSubjectEntity = subjectEntity;
	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;
	
	subjectEntity->isSubjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	
}

void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity)
{	
	GIAActionNode * newOrExistingAction;
	if(!(actionEntity->hasAssociatedActionTemp))
	{
		newOrExistingAction = addAction(actionEntity);
	}
	else
	{
		newOrExistingAction = actionEntity->AssociatedActionNodeList.back();
	}

	if(objectEntity->hasAssociatedPropertyTemp)
	{
		objectEntity = objectEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
	}
		
	//configure object entity node
	objectEntity->IncomingActionNodeList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionObjectEntity = objectEntity;
	
	objectEntity->isObjectTemp = true; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
}

void addLocationConditionToAction(GIAActionNode * actionNode, GIAEntityNode * locationConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = locationConditionEntity->entityName;
	newCondition->conditionEntity = locationConditionEntity;
	newCondition->parentAction = actionNode;
	newCondition->parentIsAction = true;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_LOCATION;
	
	actionNode->ConditionNodeList.push_back(newCondition);
	locationConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}

void addTimeConditionToAction(GIAActionNode * actionNode, GIAEntityNode * timeConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = timeConditionEntity->entityName;
	//TO DO: parse time info here [into seconds since start of universe, totalTimeInSeconds]
	newCondition->conditionEntity = timeConditionEntity;
	newCondition->parentAction = actionNode;
	newCondition->parentIsAction = true;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_TIME;
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newCondition->timeConditionNode = newTimeCondition;
		
	actionNode->ConditionNodeList.push_back(newCondition);
	timeConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}

void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = locationConditionEntity->entityName;
	newCondition->conditionEntity = locationConditionEntity;
	newCondition->parentProperty = propertyNode;
	newCondition->parentIsAction = false;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_LOCATION;
	
	propertyNode->ConditionNodeList.push_back(newCondition);
	locationConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}

void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = timeConditionEntity->entityName;
	//TO DO: parse time info here [into seconds since start of universe, totalTimeInSeconds]
	newCondition->conditionEntity = timeConditionEntity;
	newCondition->parentProperty = propertyNode;
	newCondition->parentIsAction = false;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_TIME;
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newCondition->timeConditionNode = newTimeCondition;	
	
	propertyNode->ConditionNodeList.push_back(newCondition);
	timeConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}




void addActionConditionToAction(GIAActionNode * actionNode, GIAActionNode * actionConditionActionNode)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = actionConditionActionNode->actionName;
	newCondition->conditionAction = actionConditionActionNode;
	newCondition->parentAction = actionNode;
	newCondition->parentIsAction = true;
	newCondition->conditionIsAction = true;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_ACTION;
	
	actionNode->ConditionNodeList.push_back(newCondition);
	actionConditionActionNode->ConditionNodeReverseList.push_back(newCondition);
}

void addPropertyConditionToAction(GIAActionNode * actionNode, GIAEntityNode * propertyConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = propertyConditionEntity->entityName;
	newCondition->conditionEntity = propertyConditionEntity;
	newCondition->parentAction = actionNode;
	newCondition->parentIsAction = true;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_PROPERTY;
	
	actionNode->ConditionNodeList.push_back(newCondition);
	propertyConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}

void addActionConditionToProperty(GIAEntityNode * propertyNode, GIAActionNode * actionConditionActionNode)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = actionConditionActionNode->actionName;
	newCondition->conditionAction = actionConditionActionNode;
	newCondition->parentProperty = propertyNode;
	newCondition->parentIsAction = false;
	newCondition->conditionIsAction = true;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_ACTION;
	
	propertyNode->ConditionNodeList.push_back(newCondition);
	actionConditionActionNode->ConditionNodeReverseList.push_back(newCondition);
}

void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity)
{
	GIAConditionNode * newCondition = new GIAConditionNode();
	conditionNodesCompleteList.push_back(newCondition);
	newCondition->id = currentConditionNodeIDInCompleteList++;
	newCondition->conditionName = propertyConditionEntity->entityName;
	newCondition->conditionEntity = propertyConditionEntity;
	newCondition->parentProperty = propertyNode;
	newCondition->parentIsAction = false;
	newCondition->conditionIsAction = false;
	
	newCondition->conditionType = CONDITION_NODE_TYPE_PROPERTY;
	
	propertyNode->ConditionNodeList.push_back(newCondition);
	propertyConditionEntity->ConditionNodeReverseList.push_back(newCondition);
}






void convertSentenceRelationsIntoGIAnetworkNodes(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, Sentence * firstSentenceInList)
{
	
	currentEntityNodeIDInCompleteList = 0;
	currentActionNodeIDInCompleteList = 0;
	currentConditionNodeIDInCompleteList = 0;
	/*
	currentEntityNodeInCompleteList = new GIAEntityNode();
	currentActionNodeInCompleteList = new GIAActionNode();
	currentConditionNodeInCompleteList = new GIAConditionNode();
	*/
	
	vector<GIAEntityNode*>::iterator indexOfEntityNodesIterator;
	vector<string*>::iterator indexOfEntityNamesIterator;
	vector<GIATimeConditionNode*>::iterator indexOfTimeNodesIterator;
	vector<long*>::iterator indexOfTimeNumbersIterator;	


	/*
	//initialise indexOfEntityNodes;	[should be moved elsewhere]
	string firstEntityNameInNetwork = "universe";
	GIAEntityNode * firstEntityNodeInNetwork = new GIAEntityNode();
	firstEntityNodeInNetwork->entityName = firstEntityNameInNetwork;
	indexOfEntityNodes->push_back(firstEntityNodeInNetwork);
	indexOfEntityNames->push_back(firstEntityNameInNetwork);
	
	//DEBUG
	//indexOfEntityNames->push_back("za");
	//indexOfEntityNames->push_back("zb");
	//indexOfEntityNames->push_back("zc");
	//indexOfEntityNames->push_back("zd");
	//indexOfEntityNames->push_back("ze");
	//indexOfEntityNames->push_back("zf");
	//indexOfEntityNames->push_back("zg");
	//indexOfEntityNames->push_back("zh");
	//indexOfEntityNames->push_back("zi");
	//indexOfEntityNames->push_back("zj");
	
	
	//initialise indexOfTimeNodes;		[should be moved elsewhere]	
	long firstTimeInNetwork = -14*(10^9)*SECONDS_IN_YEAR;
	string firstTimeNameInNetwork = "beginning";
	GIATimeConditionNode * firstTimeNodeInNetwork = new GIATimeConditionNode();
	firstTimeNodeInNetwork->conditionName = firstTimeNameInNetwork;
	firstTimeNodeInNetwork->totalTimeInSeconds = firstTimeInNetwork;
	indexOfTimeNodes->push_back(firstTimeNodeInNetwork);
	indexOfTimeNumbers->push_back(firstTimeInNetwork);		
	
	
	//DEBUG	
	//bool resultTemp = true;
	//string tempName = "zh";
	//long findIndex = -1;
	//GIAEntityNode * tempEntity = findOrAddEntityNodeByName(indexOfEntityNodes, indexOfEntityNames, &tempName, &resultTemp, &findIndex);
	//if(resultTemp)
	//{
	//	cout << "tempEntity->entityName = " << tempEntity->entityName << endl;
	//}
	//exit(0);
	*/

	
	Sentence * currentSentenceInList = firstSentenceInList;
	
	while(currentSentenceInList->next != NULL)
	{
		Relation * currentRelationInList;
		
		
		bool GIAEntityNodeIsDate[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//not properly implemented yet
		int GIAEntityNodeGrammaticalTenseArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		int GIAEntityNodeGrammaticalNumberArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalIsDefiniteArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		bool GIAEntityNodeGrammaticalIsPersonArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		int GIAEntityNodeGrammaticalGenderArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		//bool GIAEntityNodeGrammaticalHasCountArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		
		bool GIAEntityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
		GIAEntityNode * GIAEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

		//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
		//bool GIAEntityNodeArrayHasAssociatedProperty[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

	
		for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
		{					
			GIAEntityNodeIsDate[w] = false;
			GIAEntityNodeGrammaticalTenseArray[w] = GRAMMATICAL_TENSE_UNDEFINED;
			GIAEntityNodeGrammaticalNumberArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			GIAEntityNodeGrammaticalIsDefiniteArray[w] = false;
			GIAEntityNodeGrammaticalIsPersonArray[w] = false;
			GIAEntityNodeGrammaticalGenderArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			//GIAEntityNodeGrammaticalHasCountArray[w] = GRAMMATICAL_NUMBER_UNDEFINED;
			
			GIAEntityNodeArrayFilled[w] = false;

			//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
			//GIAEntityNodeArrayHasAssociatedProperty[w] = false;
			
			//GIAActionNodeArrayFilled[w] = false;
		}
		Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;
			
			//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
			if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_TENSE_DATE) != -1)
			{
				GIAEntityNodeIsDate[currentFeatureInList->entityIndex] = true;
				cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			}
			
			for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the tense type name 
				if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != -1) 
				//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex]) 
				{
					GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] = grammaticalTenseIndex;
					cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseIndex = " << grammaticalTenseNameArray[grammaticalTenseIndex] << endl;
				}			
			}
			for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name 
				if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != -1) 				
				{
					GIAEntityNodeGrammaticalNumberArray[currentFeatureInList->entityIndex] = grammaticalNumberIndex;
					cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalNumberIndex = " << grammaticalNumberNameArray[grammaticalNumberIndex] << endl;
				}			
			}
			if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsDefiniteArray[currentFeatureInList->entityIndex] = GRAMMATICAL_DEFINITE;
				cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}			

			if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsPersonArray[currentFeatureInList->entityIndex] = GRAMMATICAL_PERSON;
				cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}
			for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
			{
				//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name 
				if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != -1) 				
				{
					//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
					GIAEntityNodeGrammaticalGenderArray[currentFeatureInList->entityIndex] = grammaticalGenderIndex;
					cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalGenderIndex = " << grammaticalGenderNameArray[grammaticalGenderIndex] << endl;
				}			
			}
			
			/*
			//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
			if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != -1)
			{
				GIAEntityNodeGrammaticalHasCountArray[currentFeatureInList->entityIndex] = true;
				//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			}	
			*/		
												 
			currentFeatureInList = currentFeatureInList->next;
		}
		//exit(0);
		
		cout << "pass A;" << endl;
		cout << "pass 1; locate/add all entities" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			

			string functionName = currentRelationInList->relationFunction;
			string argumentName = currentRelationInList->relationArgument; 
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			//cout << "functionName = " << functionName << endl;
			//cout << "argumentName = " << argumentName << endl;

			long functionEntityIndex = -1;
			long argumentEntityIndex = -1;
			bool functionEntityAlreadyExistant = false;
			bool argumentEntityAlreadyExistant = false;

			//cout << "relationFunctionIndex = " << relationFunctionIndex << endl;
			//cout << "relationArgumentIndex = " << relationArgumentIndex << endl;

			if(!GIAEntityNodeArrayFilled[relationFunctionIndex])
			{
				GIAEntityNode * functionEntity = findOrAddEntityNodeByName(indexOfEntityNodes, indexOfEntityNames, &functionName, &functionEntityAlreadyExistant, &functionEntityIndex, true);
				GIAEntityNodeArrayFilled[relationFunctionIndex] = true;
				GIAEntityNodeArray[relationFunctionIndex] = functionEntity;
				functionEntity->hasAssociatedTime = GIAEntityNodeIsDate[relationFunctionIndex]; 
				//cout << "functionEntity->hasAssociatedTime = " << functionEntity->hasAssociatedTime << endl;
				//cout << "relationFunctionIndex = " << relationFunctionIndex << endl;	
				
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalPersonTemp = GIAEntityNodeGrammaticalIsPersonArray[relationFunctionIndex];
				GIAEntityNodeArray[relationFunctionIndex]->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[relationFunctionIndex];
				 
				GIAEntityNodeArray[relationFunctionIndex]->hasAssociatedPropertyTemp = false;
				GIAEntityNodeArray[relationFunctionIndex]->hasAssociatedActionTemp = false;
				
			}
			if(!GIAEntityNodeArrayFilled[relationArgumentIndex])
			{
				GIAEntityNode * argumentEntity = findOrAddEntityNodeByName(indexOfEntityNodes, indexOfEntityNames, &argumentName, &argumentEntityAlreadyExistant, &argumentEntityIndex, true);
				GIAEntityNodeArrayFilled[relationArgumentIndex] = true;
				GIAEntityNodeArray[relationArgumentIndex] = argumentEntity;				
				argumentEntity->hasAssociatedTime = GIAEntityNodeIsDate[relationArgumentIndex]; 
				//cout << "argumentEntity->hasAssociatedTime = " << argumentEntity->hasAssociatedTime << endl;	
				//cout << "relationArgumentIndex = " << relationArgumentIndex << endl;
			
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalTenseTemp = GIAEntityNodeGrammaticalTenseArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalNumberTemp = GIAEntityNodeGrammaticalNumberArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalDefiniteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalPersonTemp = GIAEntityNodeGrammaticalIsPersonArray[relationArgumentIndex];
				GIAEntityNodeArray[relationArgumentIndex]->grammaticalGenderTemp = GIAEntityNodeGrammaticalGenderArray[relationArgumentIndex];
								
				GIAEntityNodeArray[relationArgumentIndex]->hasAssociatedPropertyTemp = false;	
				GIAEntityNodeArray[relationFunctionIndex]->hasAssociatedActionTemp = false;		
			}
			
			currentRelationInList = currentRelationInList->next;
		}
		//cout << "as2" <<endl;
	
		cout << "pass 1b; switch argument/functions where necessary" << endl;
		if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY)
		{
			currentRelationInList = currentSentenceInList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{	
				//cout << "here1" << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

				bool passed = false;
				for(int i=0; i<RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES; i++)
				{
					//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

					if(currentRelationInList->relationType == relationTypeRequireSwitchingNameArray[i])
					{
						passed = true;
					}
				}
				if(passed)
				{	
					bool passed2 = false;
									
					if(GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED)
					{						
						//now find the associated object..
 						Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
						while(currentRelationInList2->next != NULL)
						{	
							
							/*
							bool partnerTypeRequiredFoundObj = false;
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									partnerTypeRequiredFoundObj = true;
								}
							}
							*/
							bool partnerTypeRequiredFoundSubj = false;
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									partnerTypeRequiredFoundSubj = true;
								}
							}								
							if(partnerTypeRequiredFoundSubj)
							{
								if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationArgumentIndex)
								{//found a matching subject-that[obj] relationship that requires function/argument switching

									//cout << "found a matching subject-that[obj] relationship that requires function/argument switching" << endl;
									passed2 = true;
									//cout << "partnerTypeRequiredFound: currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
								}
							}

							currentRelationInList2 = currentRelationInList2->next;
						}
					}
					else
					{
						passed2 = true;
					}
					if(passed2)
					{
						string tempString = currentRelationInList->relationArgument;
						int tempIndex = currentRelationInList->relationArgumentIndex;
						currentRelationInList->relationArgument = currentRelationInList->relationFunction;				
						currentRelationInList->relationFunction = tempString;
						currentRelationInList->relationArgumentIndex = currentRelationInList->relationFunctionIndex;				
						currentRelationInList->relationFunctionIndex = tempIndex;						
					}
				}
				currentRelationInList = currentRelationInList->next;
			}					
		}
				
		cout << "pass 2; identify entity types [define entities as objects, subjects, and being possessive of properties];" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			//possessive of properties [NB plural/singular indicates definite noun - eg a robin, the robin, the robins - which is therefore a property (entity instance):
								
			//possessive of properties:
			
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
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * ownerEntity = GIAEntityNodeArray[relationArgumentIndex];
				ownerEntity->hasPropertyTemp = true;
			}
			
			//possessive of properties:
			
			passed = false;
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
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				thingEntity->hasPropertyTemp = true;
			}
			
			//has subject:
				
			passed = false;
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}														
			//if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE))
			if(passed)
			{
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;	
				GIAEntityNode * subjectEntity = GIAEntityNodeArray[relationArgumentIndex];
				subjectEntity->isSubjectTemp = true;
			}
			
			//has object:
			
			passed = false;
			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}
			//if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
			if(passed)	
			{
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				GIAEntityNode * objectEntity = GIAEntityNodeArray[relationArgumentIndex];
				objectEntity->isObjectTemp = true; 
			}
						
			currentRelationInList = currentRelationInList->next;
		}
		
		cout << "pass 3; link references (eg his/her with joe/emily)" << endl;
		for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
		{	
			//cout << "w = " << w << endl;
				
			if(GIAEntityNodeArrayFilled[w])
			{
				GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
				//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;

				for(int i=0; i< REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES; i++)
				{
					//cout << "i = " << i << endl;

					if((currentGIAEntityNode->entityName == referenceTypePossessiveNameArray[i]) || (currentGIAEntityNode->entityName == referenceTypePersonNameArray[i]))
					{
						cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;
						//now go for a search in tree for given / this sentence + previous sentence until find candidate reference source

						GIAEntityNode * referenceSource = NULL;
						bool referenceSourceHasBeenFound = false;
						bool stillSentencesToSearch = true;
						Sentence * currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInList;
						int s2 = 0;
						while(!referenceSourceHasBeenFound && stillSentencesToSearch) 
						{
							cout << "s2 = " << s2 << endl;

							Relation * currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->firstRelationInList;
							int maxWordLimit = 999999;
							if(s2 == 0)
							{
								maxWordLimit = w;
							}
							int w2 = 0;
							while((currentRelationInWhichReferenceSourceIsBeingSearchedFor->next != NULL) && (w2 < maxWordLimit))
							{
								cout << "w2 = " << w2 << endl;

								long entityIndex = -1;
								bool entityAlreadyExistant = false;

								string entityName = currentRelationInWhichReferenceSourceIsBeingSearchedFor->relationArgument;
											
								//cout << "entityName = " << entityName << endl;
											
								if(entityName != "")
								{
								
									GIAEntityNode * currentEntityInWhichReferenceSourceIsBeingSearchedFor = findOrAddEntityNodeByName(indexOfEntityNodes, indexOfEntityNames, &entityName, &entityAlreadyExistant, &entityIndex, false);

									if(entityAlreadyExistant)
									{
										//cout << "candidateReferenceSourceEntityName = " << entityName << endl;
										
										bool entityPassesGrammaticalTestsForReference = true;
										

																	
										//if(referenceTypePersonCrossReferenceNumberArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceNumberArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp != referenceTypePersonCrossReferenceNumberArray[i])
											{
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferenceGenderArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceGenderArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp != referenceTypePersonCrossReferenceGenderArray[i])
											{
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferenceDefiniteArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferenceDefiniteArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp != referenceTypePersonCrossReferenceDefiniteArray[i])
											{
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										//if(referenceTypePersonCrossReferencePersonArray[i] != GRAMMATICAL_CATEGORY_UNDEFINED) 
										//if(!((referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED) && (referenceTypePersonCrossReferencePersonArray[i] == GRAMMATICAL_CATEGORY_UNDEFINED)))
										//{
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp != referenceTypePersonCrossReferencePersonArray[i])
											{
												entityPassesGrammaticalTestsForReference = false;
											}
										//}
										if(entityPassesGrammaticalTestsForReference)
										{	
										
											cout << "entityPassesGrammaticalTestsForReference" << endl;
											
											/*
											cout << "referenceTypePersonCrossReferenceNumberArray[i] = " << referenceTypePersonCrossReferenceNumberArray[i] << endl;
											cout << "referenceTypePersonCrossReferenceGenderArray[i] = " << referenceTypePersonCrossReferenceGenderArray[i] << endl;
											cout << "referenceTypePersonCrossReferenceDefiniteArray[i] = " << referenceTypePersonCrossReferenceDefiniteArray[i] << endl;
											cout << "referenceTypePersonCrossReferencePersonArray[i] = " << referenceTypePersonCrossReferencePersonArray[i] << endl;
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalNumberTemp << endl;
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalGenderTemp << endl;
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalDefiniteTemp << endl;
											cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->grammaticalPersonTemp << endl;
											*/
																																										
											if(currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp)
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isSubjectTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
											else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp) && (s2 > 0))
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->isObjectTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
											else if((currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp) && (s2 > 0))
											{
												//cout << "currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp = " << currentEntityInWhichReferenceSourceIsBeingSearchedFor->hasPropertyTemp << endl;
												referenceSourceHasBeenFound = true;
												referenceSource = currentEntityInWhichReferenceSourceIsBeingSearchedFor;
											}
										}

									}
									else
									{
										//cout << "error: !entityAlreadyExistant" << endl;	//will be non-existant in the case of intermediary words like "the"
										//exit(0);
									}
								}
								w2++;
								
								currentRelationInWhichReferenceSourceIsBeingSearchedFor = currentRelationInWhichReferenceSourceIsBeingSearchedFor->next;
							}

							if(currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous != NULL)
							{	
								currentSentenceInWhichReferenceSourceIsBeingSearchedFor = currentSentenceInWhichReferenceSourceIsBeingSearchedFor->previous;
							}
							else
							{
								stillSentencesToSearch = false;
							}
							s2++;

						}

						if(referenceSourceHasBeenFound)
						{//remap entity; eg He to John 
							cout << "referenceSourceHasBeenFound: assigning " << GIAEntityNodeArray[w]->entityName << " to " << referenceSource->entityName << "." << endl;
							//referenceSource->isReferenceEntityInThisSentence = true;
							GIAEntityNodeArray[w] =	referenceSource;
						}			
					}
				}
			}
		}
		
				
		cout << "pass B;" << endl;	
		cout << "0 pass; define properties (definite nouns); eg the house" << endl;
		if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS == 1)
		{
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{
				//if(GIAEntityNodeArrayFilled[i])
				//{ //condition not required; assumed true with GRAMMATICAL_DEFINITE
					if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES || (!GIAEntityNodeGrammaticalIsPersonArray[i] & !GIAEntityNodeIsDate[i]))
					{
						if(GIAEntityNodeGrammaticalIsDefiniteArray[i] == GRAMMATICAL_DEFINITE)
						{
							cout << "asd" << endl;
							cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;			
							addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
						}
					}
				//}
			}
		}
		cout << "0b pass; define properties (nouns with determinates); eg a house, the house, the houses [all nouns with singular/plural are assumed to have determintes, and are therefore properties]" << endl;
		if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES == 1)
		{
			for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
			{	
				if(GIAEntityNodeArrayFilled[i])
				{
					if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES || (!GIAEntityNodeGrammaticalIsPersonArray[i] && !GIAEntityNodeIsDate[i]))
					{
						bool passed = false;
						for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
						{
							if(GIAEntityNodeArray[i]->grammaticalNumberTemp == referenceTypeHasDeterminateCrossReferenceNumberArray[j])
							{
								passed = true;
							}
						}
						if(passed)
						{
							addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
						}
					}
				}
			}
		}
		
		cout << "0c pass; define properties (nouns with adjectives); _amod; eg locked door, _advmod; eg cheetahs run quickly [NOT and c) _predadj; eg giants are red / joe is late]" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{	
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveWhichImplyEntityInstanceNameArray[i])
				{
					passed = true;
				}
			}						
			if(passed)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				string thingName = currentRelationInList->relationFunction;
				string propertyName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				
				addPropertyToPropertyDefinition(thingEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}				
		
		cout << "0d pass; define properties (quantities [not quantity mods/multipiers] and measures);" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQuantityOrMeasureNameArray[i])
				{
					passed = true;
				}
			}						
			if(passed)
			{
				//create a new property for the entity and assign a property definition entity if not already created
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationFunctionIndex];
				
				addPropertyToPropertyDefinition(propertyEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}				
		
					
										
		cout << "1 pass; link properties (possessive relationships); eg joe's bike" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
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
				cout << "propertyName = " << propertyEntity->entityName << endl;
				cout << "ownerName = " << ownerEntity->entityName << endl;
													
				addOrConnectPropertyToEntity(ownerEntity, propertyEntity);
			}			
			currentRelationInList = currentRelationInList->next;
		}
	
		cout << "2 pass; link properties (descriptive relationships); eg joe is happy" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
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
				//create a new property for the entity and assign a property definition entity if not already created
				string thingName = currentRelationInList->relationFunction;
				string propertyName = currentRelationInList->relationArgument; 
				int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
				int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
				GIAEntityNode * thingEntity = GIAEntityNodeArray[relationFunctionIndex];
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationArgumentIndex];
				cout << "thingEntity = " << thingEntity->entityName << endl;
				cout << "propertyEntity = " << propertyEntity->entityName << endl;
												
				addOrConnectPropertyToEntity(thingEntity, propertyEntity);					
			}			
			currentRelationInList = currentRelationInList->next;
		}					

		cout << "2b pass; link entity definitions (appositive of nouns only)" << endl;
		currentRelationInList = currentSentenceInList->firstRelationInList;
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
				cout << "propertyName = " << propertyEntity->entityName << endl;
				cout << "entityName = " << definitionEntity->entityName << endl;
								
				addDefinitionToEntity(propertyEntity, definitionEntity);									
			}			
			currentRelationInList = currentRelationInList->next;
		}
					
		
 		cout <<"3 pass; define dependent subject-object definition/composition/action relationships and independent subject/object action relationships" << endl;
 		currentRelationInList = currentSentenceInList->firstRelationInList;
		bool subjectObjectRelationshipAlreadyAdded[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE;i++)
		{
			subjectObjectRelationshipAlreadyAdded[i] = false;
		}
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				
			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
			
			
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}												
			//if((currentRelationInList->relationType == RELATION_TYPE_SUBJECT) || ((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO) || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)))
			if(passed)
			{
				if(subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] != true)
				{
					subjectObjectRelationshipAlreadyAdded[relationFunctionIndex] = true;

					bool foundPartner = false;			
					string partnerTypeRequired;
					string subjectObjectName[2];
					GIAEntityNode * subjectObjectEntityArray[2];
					string partnerTypeRequiredArray[2];
					partnerTypeRequiredArray[SUBJECT_INDEX] = RELATION_TYPE_SUBJECT;
					partnerTypeRequiredArray[OBJECT_INDEX] = RELATION_TYPE_OBJECT;
					for(int i=0; i<2; i++)
					{
						subjectObjectEntityArray[i] = NULL;
					}
					int firstIndex;
					int secondIndex;
					int indexMapping[2];	//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
					
					bool passsubject = false;
					bool passobject = false;
					for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
						{
							passsubject = true;
						}
					}														
					for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
					{
						if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
						{
							passobject = true;
						}
					}			
					//if((currentRelationInList->relationType == RELATION_TYPE_SUBJECT) || (currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE))
					if(passsubject)
					{
						firstIndex = SUBJECT_INDEX;
						secondIndex = OBJECT_INDEX;			
					}
					//else if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
					else if(passobject)
					{
						firstIndex = OBJECT_INDEX;
						secondIndex = SUBJECT_INDEX;									

					}
					partnerTypeRequired = partnerTypeRequiredArray[secondIndex];
					subjectObjectName[firstIndex] = currentRelationInList->relationArgument;
					indexMapping[firstIndex] = relationArgumentIndex;	//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
					cout << partnerTypeRequiredArray[firstIndex] << " name = " << subjectObjectName[firstIndex] << endl;
					subjectObjectEntityArray[firstIndex] = GIAEntityNodeArray[relationArgumentIndex];					
					//cout << "subjectObjectEntityArray[firstIndex]->entityName = " << subjectObjectEntityArray[firstIndex]->entityName << endl;	


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
								
					//now find the associated object..
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{	
						//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

						bool partnerTypeRequiredFound = false;
						if(partnerTypeRequired == RELATION_TYPE_SUBJECT)
						{
							for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeSubjectNameArray[i])
								{
									partnerTypeRequiredFound = true;
								}
							}
						}
						else if(partnerTypeRequired == RELATION_TYPE_OBJECT)														
						{
							for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
							{
								if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
								{
									partnerTypeRequiredFound = true;
								}
							}
						}
						if(partnerTypeRequiredFound)
						{

							if(currentRelationInList2->relationFunctionIndex == currentRelationInList->relationFunctionIndex)
							{//found a matching object-subject relationship
									
								int relationFunctionIndex2 = currentRelationInList2->relationFunctionIndex;
								int relationArgumentIndex2 = currentRelationInList2->relationArgumentIndex;
												
								subjectObjectName[secondIndex] = currentRelationInList2->relationArgument;
								indexMapping[secondIndex] = relationArgumentIndex2;	//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
															
								cout << partnerTypeRequiredArray[secondIndex] << " name = " << subjectObjectName[secondIndex] << endl;
								subjectObjectEntityArray[secondIndex] = GIAEntityNodeArray[relationArgumentIndex2];
								//cout << "subjectObjectEntityArray[secondIndex]->entityName = " << subjectObjectEntityArray[secondIndex]->entityName << endl;	

								//cout << "subjectObjectEntityArray[SUBJECT_INDEX]->entityName = " << subjectObjectEntityArray[SUBJECT_INDEX]->entityName << endl;	
								//cout << "subjectObjectEntityArray[OBJECT_INDEX]->entityName = " << subjectObjectEntityArray[OBJECT_INDEX]->entityName << endl;

								//if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
								if(passdefinition)
								{//expected subject-object relationship is a definition "is"
									
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
										//NB definitions are only assigned to entities, not properties (instances of entities)
										
									addDefinitionToEntity(subjectObjectEntityArray[SUBJECT_INDEX], subjectObjectEntityArray[OBJECT_INDEX]);
								}
								//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
								else if(passcomposition)
								{//subject-object relationship is a composition [property]
									addOrConnectPropertyToEntity(subjectObjectEntityArray[SUBJECT_INDEX], subjectObjectEntityArray[OBJECT_INDEX]);
										//check can use properties for composition/comprises ; ie, does "tom is happy" = "tom comprises happiness" ?
								}
								else
								{//assume that the subject-object relationships is an action
									string actionName = currentRelationInList->relationFunction;
									cout << "actionName = " << actionName << endl;
									GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];
									
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
									GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];
									GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
																										
									addActionToEntity(subjectEntityTemp, objectEntityTemp, actionEntity);
								}
								foundPartner = true;	
								
												
							}
						}
						currentRelationInList2 = currentRelationInList2->next;
					}

					if(!foundPartner)
					{//add independent action if appropriate
						if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
						{
						}
						//else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
						else if(passcomposition)
						{
						}
						else
						{//assume that the subject-object relationships is an action
							string actionName = currentRelationInList->relationFunction;
							cout << "actionName = " << actionName << endl;
							GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];

							//addAction(actionEntity);	//WHY WAS THIS HERE????
							if(firstIndex == SUBJECT_INDEX)
							{//fired by joe..???? [is there a possible example of this?]
							
								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * subjectEntityTemp = subjectObjectEntityArray[SUBJECT_INDEX];

								addActionToSubject(subjectEntityTemp, actionEntity);						
							}
							else if(firstIndex == OBJECT_INDEX)
							{//eg the bow was fired
							
								//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
								GIAEntityNode * objectEntityTemp = subjectObjectEntityArray[OBJECT_INDEX];
															
								addActionToObject(objectEntityTemp, actionEntity);
							}	

						}
					}

					//now depending upon the function, do something different; create an action (if 'does' etc), create a definition (if 'is' etc)
				}
			}
		
			currentRelationInList = currentRelationInList->next;
		}
		
		
		 //3b pass; define indirect objects
		 currentRelationInList = currentSentenceInList->firstRelationInList;
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
				
		//4 pass; define action conditions
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

			int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
			int relationArgumentIndex = currentRelationInList->relationArgumentIndex;
				
			if((currentRelationInList->relationType == RELATION_TYPE_AT) || (currentRelationInList->relationType == RELATION_TYPE_ON))
			{
				GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationFunctionIndex];				
				GIAEntityNode * timeOrLocationConditionEntity = GIAEntityNodeArray[relationArgumentIndex];

				if(actionOrPropertyEntity->hasAssociatedActionTemp)
				{
					GIAActionNode * actionNode = actionOrPropertyEntity->AssociatedActionNodeList.back();
					
					if(timeOrLocationConditionEntity->hasAssociatedTime)
					{
						string timeConditionName = currentRelationInList->relationArgument; 
						
						cout << "HERE1" << endl;
						cout << "actionName = " << actionNode->actionName << endl;
						cout << "timeConditionName = " << timeConditionName << endl;

						addTimeConditionToAction(actionNode, timeOrLocationConditionEntity);				
					}
					else
					{//assume place/location

						string locationConditionName = currentRelationInList->relationArgument; 
						
						//cout << "HERE2" << endl;
						cout << "actionName = " << actionNode->actionName << endl;
						cout << "locationConditionName = " << locationConditionName << endl;

						if(timeOrLocationConditionEntity->hasAssociatedPropertyTemp)
						{
							timeOrLocationConditionEntity = timeOrLocationConditionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
						}
						
						addLocationConditionToAction(actionNode, timeOrLocationConditionEntity);
					}
				}
				else if(actionOrPropertyEntity->hasAssociatedPropertyTemp)
				//else if(actionOrPropertyEntity->hasAssociatedProperty)
				//else if(actionOrPropertyEntity->isProperty)		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)		
				{
					GIAEntityNode * propertyNode = actionOrPropertyEntity->AssociatedPropertyNodeList.back();
					//GIAEntityNode * propertyNode = actionOrPropertyEntity;		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)	
					
					if(timeOrLocationConditionEntity->hasAssociatedTime)
					{
						string timeConditionName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "timeConditionName = " << timeConditionName << endl;

						addTimeConditionToProperty(propertyNode, timeOrLocationConditionEntity);				
					}
					else
					{//assume place/location

						string locationConditionName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "locationConditionName = " << locationConditionName << endl;

						if(timeOrLocationConditionEntity->hasAssociatedPropertyTemp)
						{
							timeOrLocationConditionEntity = timeOrLocationConditionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
						}

						addLocationConditionToProperty(propertyNode, timeOrLocationConditionEntity);
					}				
				}
				else
				{
					cout << "error: actionOrPropertyEntity does not have associated Property or Action" << endl;
					cout << "actionOrPropertyEntity = " << actionOrPropertyEntity << endl;				
				}
				

			}		
			else if((currentRelationInList->relationType == RELATION_TYPE_WHEN) || (currentRelationInList->relationType == RELATION_TYPE_BECAUSE))
			{
				GIAEntityNode * actionOrPropertyEntity = GIAEntityNodeArray[relationFunctionIndex];				
				GIAEntityNode * actionOrPropertyConditionEntity = GIAEntityNodeArray[relationArgumentIndex];
								
				if(actionOrPropertyEntity->hasAssociatedActionTemp)
				{				
					GIAActionNode * actionNode = actionOrPropertyEntity->AssociatedActionNodeList.back();

					if(actionOrPropertyConditionEntity->hasAssociatedActionTemp)
					{//eg john rides his bike when joe plays with the table
						string actionConditionEntityName = currentRelationInList->relationArgument; 
						
						//cout << "l3" << endl;
						cout << "actionName = " << actionNode->actionName << endl;
						cout << "actionConditionEntityName = " << actionConditionEntityName << endl;

						GIAActionNode * actionConditionActionNode = actionOrPropertyConditionEntity->AssociatedActionNodeList.back();
						addActionConditionToAction(actionNode, actionConditionActionNode);
					}
					else if(actionOrPropertyConditionEntity->hasAssociatedPropertyTemp)
					//else if(actionOrPropertyConditionEntity->hasAssociatedProperty)
					//else if(actionOrPropertyConditionEntity->isProperty)	//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)
					{//eg john rides his bike when joe is happy
						string propertyConditionEntityName = currentRelationInList->relationArgument; 
						
						//cout << "l4" << endl;
						cout << "actionName = " << actionNode->actionName << endl;
						cout << "propertyConditionEntityName = " << propertyConditionEntityName << endl;
		
						GIAEntityNode * propertyNode = actionOrPropertyConditionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a

						addPropertyConditionToAction(actionNode, propertyNode);				
					}
					else
					{
						cout << "l1" << endl;
						cout << "error: actionOrPropertyConditionEntity does not have associated Property or Action" << endl;
						cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity << endl;
					}
				}
				else if(actionOrPropertyEntity->hasAssociatedPropertyTemp)
				//else if(actionOrPropertyEntity->hasAssociatedProperty)
				//else if(actionOrPropertyEntity->isProperty)	//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)	
				{
					GIAEntityNode * propertyNode = actionOrPropertyEntity->AssociatedPropertyNodeList.back();
					//GIAEntityNode * propertyNode = actionOrPropertyEntity;		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)	
					
					if(actionOrPropertyConditionEntity->hasAssociatedActionTemp)
					{//eg the stars are bright when john rides his bike
						string actionConditionEntityName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "actionConditionEntityName = " << actionConditionEntityName << endl;

						GIAActionNode * actionConditionActionNode = actionOrPropertyConditionEntity->AssociatedActionNodeList.back();
						addActionConditionToProperty(propertyNode, actionConditionActionNode);
					}
					else if(actionOrPropertyConditionEntity->hasAssociatedPropertyTemp)
					//else if(actionOrPropertyConditionEntity->hasAssociatedProperty)
					//else if(actionOrPropertyConditionEntity->isProperty)		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)
					{//eg the stars are bright when the sun is dim. 
						string propertyConditionEntityName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "propertyConditionEntityName = " << propertyConditionEntityName << endl;

						GIAEntityNode * propertyConditionNode = actionOrPropertyConditionEntity->AssociatedPropertyNodeList.back();	//added 4 May 11a
						
						addPropertyConditionToProperty(propertyNode, propertyConditionNode);				
					}
					else
					{
						cout << "error: actionOrPropertyConditionEntity does not have associated Property or Action" << endl;
						cout << "actionOrPropertyConditionEntity = " << actionOrPropertyConditionEntity << endl;
					}				
				}
				else
				{
					 
					cout << "error: actionOrPropertyEntity does not have associated Property or Action" << endl;
					cout << "actionOrPropertyEntity = " << actionOrPropertyEntity->entityName << endl;				
				}					
			}			
						
			currentRelationInList = currentRelationInList->next;
		
		}
	
		
		//4b pass; extract dates	[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{
				GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
				if(currentEntity->hasAssociatedTime)
				{			
					GIAEntityNode * timeConditionEntity = currentEntity;
					//cout << "as1" << endl;
					if(timeConditionEntity->ConditionNodeReverseList.back() != NULL)
					{
						//cout << "as2" << endl;

						GIAConditionNode * tempTimeCondition = timeConditionEntity->ConditionNodeReverseList.back();
						string monthString = timeConditionEntity->entityName;
						int monthInt = TIME_MONTH_UNDEFINED;
						for(int i=0; i<TIME_MONTH_NUMBER_OF_TYPES; i++)
						{
							if(monthString == timeMonthStringArray[i])
							{
								monthInt = i+1;
							}
						}
						tempTimeCondition->timeConditionNode->month = monthInt;
						
						//update/regenerate timeConditionName
						tempTimeCondition->conditionName = generateDateTimeConditionName(tempTimeCondition->timeConditionNode->dayOfMonth, tempTimeCondition->timeConditionNode->month, tempTimeCondition->timeConditionNode->year);
					}
					else
					{
						cout << "error: isolated date node found" << endl;
						exit(0);	//remove this later
					}
				}
			}
		}	
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if((currentRelationInList->relationType == RELATION_TYPE_DATE_DAY) || (currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR))
			{	
				//now locate and fill corresponding time condition node;	
				for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
				{
					if(GIAEntityNodeArrayFilled[i])
					{
						GIAEntityNode * currentEntity = GIAEntityNodeArray[i];
						if(currentEntity->hasAssociatedTime)
						{			
							GIAEntityNode * timeConditionEntity = currentEntity;

							if(timeConditionEntity->entityName == currentRelationInList->relationFunction)
							{	
								if(timeConditionEntity->ConditionNodeReverseList.back() != NULL)
								{
									GIAConditionNode * tempTimeCondition = timeConditionEntity->ConditionNodeReverseList.back();

									if(currentRelationInList->relationType == RELATION_TYPE_DATE_DAY)
									{
										//http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
											//The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.	[eg "3rd" -> 3]
										string dayOfMonthString = currentRelationInList->relationArgument;
										char * dayOfMonthStringcharstar = const_cast<char*>(dayOfMonthString.c_str());
										int dayOfMonthInt = atoi(dayOfMonthStringcharstar);
										tempTimeCondition->timeConditionNode->dayOfMonth = dayOfMonthInt;
										cout << "adding day of month: " << dayOfMonthInt << endl;
										
										//update/regenerate timeConditionName
										tempTimeCondition->conditionName = generateDateTimeConditionName(tempTimeCondition->timeConditionNode->dayOfMonth, tempTimeCondition->timeConditionNode->month, tempTimeCondition->timeConditionNode->year);
									}
									if(currentRelationInList->relationType == RELATION_TYPE_DATE_YEAR)
									{
										string yearString = currentRelationInList->relationArgument;
										char * yearStringcharstar = const_cast<char*>(yearString.c_str());
										int yearInt = atoi(yearStringcharstar);
										tempTimeCondition->timeConditionNode->year = yearInt;
										cout << "adding year: " << yearInt << endl;
										
										//update/regenerate timeConditionName
										tempTimeCondition->conditionName = generateDateTimeConditionName(tempTimeCondition->timeConditionNode->dayOfMonth, tempTimeCondition->timeConditionNode->month, tempTimeCondition->timeConditionNode->year);
									}
								}
							}
						}
					}
				}										
			}
			currentRelationInList = currentRelationInList->next;		
		}
		
		//4c pass; extract quantities	
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
														
			if(currentRelationInList->relationType == RELATION_TYPE_QUANTITY)
			{
				GIAEntityNode * quantityEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
				if(quantityEntity->AssociatedPropertyNodeList.back() != NULL)
				{
					GIAEntityNode * quantityProperty = quantityEntity->AssociatedPropertyNodeList.back();
					quantityProperty->hasQuantity = true;
					int quantityNumberInt = calculateQuantityNumberInt(currentRelationInList->relationArgument);
					quantityProperty->quantityNumber = quantityNumberInt;

					//now locate quantity modifiers and multiplicators
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{	
						//cout << "here1" << endl;
						//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MOD)
						{	
							if(currentRelationInList2->relationFunction == currentRelationInList->relationArgument)
							{
								/*
								int quantityModifierInt = calculateQuantityModifierInt(currentRelationInList2->relationArgument);
								quantityProperty->quantityModifier = quantityModifierInt;
								*/
								quantityProperty->quantityModifierString = currentRelationInList->relationArgument;
							}

						}	
						if(currentRelationInList2->relationType == RELATION_TYPE_QUANTITY_MULT)
						{
							if(currentRelationInList2->relationFunction == currentRelationInList->relationArgument)
							{
								int quantityMultiplierInt = calculateQuantityMultiplierInt(currentRelationInList2->relationArgument);
								quantityProperty->quantityNumber = quantityProperty->quantityNumber * quantityMultiplierInt;
							}						
						}						

						currentRelationInList2 = currentRelationInList2->next;
					}		
				}								
			}
			currentRelationInList = currentRelationInList->next;		
		}
		
		//4d pass; extract measures
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			bool pass = false;
			int measureTypeIndex = MEASURE_TYPE_UNDEFINED;
			for(int i=0; i<RELATION_TYPE_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeMeasureNameArray[i])
				{
					measureTypeIndex = i;
					pass = true;
				}
			}																		
			if(pass)
			{
				GIAEntityNode * measureEntity = GIAEntityNodeArray[currentRelationInList->relationFunctionIndex];
				if(measureEntity->AssociatedPropertyNodeList.back() != NULL)
				{
					GIAEntityNode * measureProperty = measureEntity->AssociatedPropertyNodeList.back();
					measureProperty->hasMeasure = true;
					measureProperty->measureType = measureTypeIndex;
					
					//string measurePropertyName = currentRelationInList->relationFunction; 
					//string quantityName = currentRelationInList->relationArgument; 
					int relationFunctionIndex = currentRelationInList->relationFunctionIndex;
					int relationArgumentIndex = currentRelationInList->relationArgumentIndex;				
					
					GIAEntityNode * measurePropertyEntity = GIAEntityNodeArray[relationFunctionIndex];
					GIAEntityNode * quantityPropertyEntity = GIAEntityNodeArray[relationArgumentIndex];
					cout << "measurePropertyName = " << measurePropertyEntity->entityName << endl;
					cout << "quantityPropertyName = " << quantityPropertyEntity->entityName << endl;

					addOrConnectPropertyToEntity(measurePropertyEntity, quantityPropertyEntity);	
				}								
			}
			currentRelationInList = currentRelationInList->next;		
		}
				
		/*
		//restore critical variables; temporary: used for GIA translator reference paser only - cleared every time a new sentence is parsed
		for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
		{	
			if(GIAEntityNodeArrayFilled[w])
			{
				GIAEntityNodeArray[w]->isReferenceEntityInThisSentence = false;
			}
		}
		*/
		


			
	
		currentSentenceInList = currentSentenceInList->next;
	}
	
	//restore critical variables; temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	long vectorSize = indexOfEntityNames->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{	
		GIAEntityNode * entityNode = indexOfEntityNodes->at(entityIndex);
		entityNode->entityAlreadyDeclaredInThisContext = false;
	}	
}

long maximumLong(long a, long b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//uses fast search algorithm
GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, string * entityNodeName, bool * found, long * index, bool addIfNonexistant)
{
	GIAEntityNode * entityNodeFound = NULL;
	
	long vectorSize = indexOfEntityNames->size();
	
	//vector<long>::iterator indexOfEntityNamesIterator;
	if(vectorSize == 0)
	{
		//cout << "vectorSize = "  << vectorSize << endl;
		//cout << "as" << endl;
		cout << "adding entity node; " << *entityNodeName << endl;

		entityNodeFound = new GIAEntityNode();
		entityNodesCompleteList.push_back(entityNodeFound);
		entityNodeFound->id = currentEntityNodeIDInCompleteList++;
		entityNodeFound->entityName = *entityNodeName;
			//configure property definition node
		indexOfEntityNodes->push_back(entityNodeFound);	
		indexOfEntityNames->push_back(*entityNodeName);	
					
	}
	else
	{
		long findIndex = vectorSize/2;
		long findRange = maximumLong(findIndex/2, 1);	//maximumLong(((findIndex+1)/2), 1);
		long previousFindRange = findRange;
		
		//indexOfEntityNamesIterator = indexOfEntityNames->begin() + startingPoint;

		//cout << "indexOfEntityNamesIterator = " << indexOfEntityNamesIterator << endl;

		int findRangeAtMinimaTimes = 0;

		bool searchOptionsAvailable = true;
		string previousTempName;
		string nameTemp;
		long previousFindIndex = findIndex;
		
		previousTempName = indexOfEntityNames->at(findIndex);	//requires start value
		
		bool first = true;
		
		while(searchOptionsAvailable)
		{
			nameTemp = indexOfEntityNames->at(findIndex);
			/*
			if(*entityNodeName == "ride")
			{
				cout << "vectorSize = "  << vectorSize << endl;
				cout << "findIndex = " << findIndex << endl;
				cout << "findRange = " << findRange << endl;		      
				cout << "nameTemp = " << nameTemp << endl;
				cout << "*entityNodeName = " << *entityNodeName << endl;		      
			}
			*/
			
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "findIndex = " << findIndex << endl;
			//cout << "findRange = " << findRange << endl;			
			//cout << "nameTemp = " << nameTemp << endl;
			//cout << "*entityNodeName = " << *entityNodeName << endl;
			
			if(nameTemp > *entityNodeName)
			{
				/*
				if(*entityNodeName == "ride")
				{
					cout << nameTemp << ">" << *entityNodeName << endl;
				}
				*/
				//cout << nameTemp << ">" << *entityNodeName << endl;
				//cout << "previousTempName = " << previousTempName << endl;
				if(((previousTempName < *entityNodeName) && (previousFindRange == 1)) || (vectorSize==1))	//&& (!first || (vectorSize==1))
				{//optimum position lies inbetween
					//cout << "as" <<endl;
					searchOptionsAvailable = false;
					*found = false;	
					//findIndex = findIndex [use current findIndex, nb vectors get inserted before the current index]			
				}
				else
				{
					previousFindIndex = findIndex;
					long temp = (findIndex - findRange);
					//cout << "temp = " << temp;
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					if(findIndex < 0)
					{
						//cout << "error: (findIndex < 0)" << endl;
							//cout << "entityNodeName " << *entityNodeName << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
						findIndex = 0; //findIndex++;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(nameTemp < *entityNodeName)
			{
				/*
				if(*entityNodeName == "ride")
				{
					cout << nameTemp << "<" << *entityNodeName << endl;
				}
				*/			
				//cout << nameTemp << "<" << *entityNodeName << endl;	
				
				if(((previousTempName > *entityNodeName) && (previousFindRange == 1)) || (vectorSize==1))		//& (!first || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;
					findIndex = previousFindIndex;
				}
				else
				{	
					previousFindIndex = findIndex;			
					long temp = (findIndex + findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					if(findIndex > (vectorSize-1))
					{

							//cout << "findIndex = " << findIndex << endl;
							//cout << "*entityNodeName = " << *entityNodeName << endl;
						//cout << "error: (findIndex > (vectorSize-1))" << endl;
							//cout << "entityNodeName " << *entityNodeName << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(nameTemp == *entityNodeName)
			{
				*index = findIndex;
				entityNodeFound = indexOfEntityNodes->at(findIndex);

				cout << "\tentity node found; " << *entityNodeName << endl;
				//cout << "findIndex = " << findIndex << endl;
				
				searchOptionsAvailable = false;
				*found = true;
			}
			
			if((searchOptionsAvailable == false) && (*found == false) && (addIfNonexistant))
			{
				cout << "\t\tadding entity node; " << *entityNodeName << endl;
				//cout << "previousFindIndex = " << previousFindIndex << endl;
				
				entityNodeFound = new GIAEntityNode();
				entityNodesCompleteList.push_back(entityNodeFound);
				entityNodeFound->id = currentEntityNodeIDInCompleteList++;
				entityNodeFound->entityName = *entityNodeName;
				vector<GIAEntityNode*>::iterator indexOfEntityNodesIterator = indexOfEntityNodes->begin();
				//indexOfEntityNodesIterator = indexOfEntityNodes->at(findIndex);
				advance(indexOfEntityNodesIterator,findIndex);
				indexOfEntityNodes->insert(indexOfEntityNodesIterator, entityNodeFound);

				vector<string>::iterator indexOfEntityNamesIterator = indexOfEntityNames->begin();
				//indexOfEntityNamesIterator = indexOfEntityNames->at(findIndex);
				advance(indexOfEntityNamesIterator,findIndex);
				indexOfEntityNames->insert(indexOfEntityNamesIterator, *entityNodeName);
			}
			
			first = false;
			
			previousTempName = nameTemp;
			

		}
	}

	/*
	cout << "debug entity" << endl;
	vector<GIAEntityNode*>::iterator entityIter;
	for (entityIter = indexOfEntityNodes->begin(); entityIter != indexOfEntityNodes->end(); entityIter++) 
	{
		cout << (*entityIter)->entityName << endl;
	}
	cout << "debug names" << endl;
	vector<string>::iterator stringIter;
	for (stringIter = indexOfEntityNames->begin(); stringIter != indexOfEntityNames->end(); stringIter++) 
	{
		cout << (*stringIter) << endl;
	}	
	*/			
				
	return entityNodeFound;
}


//CHECK THIS, it has been updated based upon above code as a template
GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant)
{
	GIATimeConditionNode * timeNodeFound = NULL;
	
	long vectorSize = indexOfTimeNumbers->size();
	
	//vector<long>::iterator indexOfEntityNamesIterator;
	if(vectorSize == 0)
	{
		//cout << "vectorSize = "  << vectorSize << endl;
		cout << "adding time node; " << *timeNodeNumber << endl;

		timeNodeFound = new GIATimeConditionNode();
		timeNodeFound->totalTimeInSeconds = *timeNodeNumber;
			//configure property definition node
		indexOfTimeNodes->push_back(timeNodeFound);	
		indexOfTimeNumbers->push_back(*timeNodeNumber);	
					
	}
	else
	{
		long findIndex = vectorSize/2;
		long findRange = maximumLong(findIndex/2, 1);	//maximumLong(((findIndex+1)/2), 1);
		long previousFindRange = findRange;
		
		int findRangeAtMinimaTimes = 0;

		bool searchOptionsAvailable = true;
		long previousTempTime;
		long timeTemp;
		long previousFindIndex = findIndex;
		
		previousTempTime = indexOfTimeNumbers->at(findIndex);	//requires start value
		
		while(searchOptionsAvailable)
		{
			//cout << "vectorSize = "  << vectorSize << endl;
			//cout << "findIndex = " << findIndex << endl;
			//cout << "findRange = " << findRange << endl;
			timeTemp = indexOfTimeNumbers->at(findIndex);
			//cout << "timeTemp = " << timeTemp << endl;

			

			if(timeTemp > *timeNodeNumber)
			{
				if(((previousTempTime < *timeNodeNumber) && (previousFindRange == 1)) || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;	
					//findIndex = findIndex [use current findIndex, nb vectors get inserted before the current index]			
				}
				else
				{
					previousFindIndex = findIndex;
					long temp = (findIndex - findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					//cout << timeTemp << ">" << *timeNodeNumber << endl;
					if(findIndex < 0)
					{
						//cout << "error: (findIndex < 0)" << endl;
						//cout << "timeNodeNumber " << *timeNodeNumber << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
						findIndex = 0; //findIndex++;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(timeTemp < *timeNodeNumber)
			{
				if(((previousTempTime > *timeNodeNumber) && (previousFindRange == 1)) || (vectorSize==1))
				{//optimum position lies inbetween
					searchOptionsAvailable = false;
					*found = false;
					findIndex = previousFindIndex;
				}
				else
				{	
					previousFindIndex = findIndex;			
					long temp = (findIndex + findRange);
					//findIndex = maximumLong(temp, 1);
					findIndex = temp;
					
					//cout << timeTemp << "<" << *timeNodeNumber << endl;		
					if(findIndex > (vectorSize-1))
					{

						//cout << "findIndex = " << findIndex << endl;
						//cout << "*timeNodeNumber = " << *timeNodeNumber << endl;
						//cout << "error: (findIndex > (vectorSize-1))" << endl;
						//cout << "timeNodeNumber " << *timeNodeNumber << " not found " << endl;
						searchOptionsAvailable = false;
						*found = false;
							//addIfNonexistant... see below
					}
				}

				previousFindRange = findRange;
				findRange = maximumLong(findRange/2, 1);	//findRange = (findRange+1)/2;

			}
			else if(timeTemp == *timeNodeNumber)
			{
				*index = findIndex;
				timeNodeFound = indexOfTimeNodes->at(findIndex);

				cout << "time node found; " << *timeNodeNumber << endl;
				//cout << "findIndex = " << findIndex << endl;
				
				searchOptionsAvailable = false;
				*found = true;
			}
			
			if((searchOptionsAvailable == false) && (*found == false) && (addIfNonexistant))
			{
				cout << "adding time node; " << *timeNodeNumber << endl;

				timeNodeFound = new GIATimeConditionNode();
				timeNodeFound->totalTimeInSeconds = *timeNodeNumber;
				vector<GIATimeConditionNode*>::iterator indexOfTimeNodesIterator = indexOfTimeNodes->begin();
				//indexOfTimeNodesIterator = indexOfTimeNodes->at(findIndex);
				advance(indexOfTimeNodesIterator,findIndex);
				indexOfTimeNodes->insert(indexOfTimeNodesIterator, timeNodeFound);

				vector<long>::iterator indexOfTimeNumbersIterator = indexOfTimeNumbers->begin();
				//indexOfTimeNumbersIterator = indexOfTimeNumbers->at(findIndex);
				advance(indexOfTimeNumbersIterator,findIndex);
				indexOfTimeNumbers->insert(indexOfTimeNumbersIterator, *timeNodeNumber);
			}
			
			previousTempTime = timeTemp;
			

		}
	}

	/*
	cout << "debug entity" << endl;
	vector<GIAEntityNode*>::iterator entityIter;
	for (entityIter = indexOfEntityNodes->begin(); entityIter != indexOfEntityNodes->end(); entityIter++) 
	{
		cout << (*entityIter)->entityName << endl;
	}
	cout << "debug names" << endl;
	vector<string>::iterator stringIter;
	for (stringIter = indexOfEntityNames->begin(); stringIter != indexOfEntityNames->end(); stringIter++) 
	{
		cout << (*stringIter) << endl;
	}	
	*/			
				
	return timeNodeFound;
}




