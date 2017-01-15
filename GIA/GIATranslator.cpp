/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1a1a 15-Jan-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors indexOfEntityNodes/indexOfEntityNames with a map, and replace vectors GIATimeConditionNode/indexOfTimeNumbers with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslator.h"



void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	if(propertyEntity->hasAssociatedPropertyTemp)
	{
		propertyEntity = propertyEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a

		if(thingEntity->hasAssociatedPropertyTemp)
		{
			thingEntity = thingEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
		}

		//configure entity node containing this property
		thingEntity->firstPropertyNodeInList.push_back(propertyEntity);	
	}
	else
	{
		if(thingEntity->hasAssociatedPropertyTemp)
		{
			thingEntity = thingEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
		}
	
		//configure property node
		GIAEntityNode * newProperty = new GIAEntityNode();
		newProperty->entityName = propertyEntity->entityName;
		newProperty->isProperty = true;
		newProperty->entityNodeContainingThisProperty = thingEntity;
		newProperty->entityNodeDefiningThisProperty = propertyEntity;
		propertyEntity->hasAssociatedProperty = true;
		propertyEntity->hasAssociatedPropertyTemp = true;
		
		if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
		{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
			addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp);
		}

		//configure property definition node
		propertyEntity->firstAssociatedPropertyNodeInList.push_back(newProperty);
		
		//configure entity node containing this property
		thingEntity->firstPropertyNodeInList.push_back(newProperty);		
	}
}

void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity)
{
	//configure property node
	GIAEntityNode * newProperty = new GIAEntityNode();
	newProperty->entityName = propertyEntity->entityName;
	newProperty->isProperty = true;
	newProperty->entityNodeContainingThisProperty = NULL;
	newProperty->entityNodeDefiningThisProperty = propertyEntity;
	propertyEntity->hasAssociatedProperty = true;
	propertyEntity->hasAssociatedPropertyTemp = true;
	
	if(propertyEntity->grammaticalTenseTemp > GRAMMATICAL_TENSE_PRESENT)
	{//ie, tense = GRAMMATICAL_TENSE_FUTURE/GRAMMATICAL_TENSE_PAST
		addTenseOnlyTimeConditionToProperty(newProperty, propertyEntity->grammaticalTenseTemp);
	}
	
	//configure property definition node
	propertyEntity->firstAssociatedPropertyNodeInList.push_back(newProperty);		
}

/*not used
void connnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntityAssociatedProperty)
{
	//configure property node
	propertyEntityAssociatedProperty->entityNodeContainingThisProperty = thingEntity;
	
	//configure entity node containing this property
	thingEntity->firstPropertyNodeInList.push_back(propertyEntityAssociatedProperty);
		
}
*/



void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense)
{
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->sharedCondition->conditionName = tenseNameArray[tense];
	newTimeCondition->tense = tense;
	newTimeCondition->sharedCondition->conditionEntity = NULL;
	newTimeCondition->sharedCondition->parentProperty = propertyNode;
	newTimeCondition->sharedCondition->parentIsAction = false;
	newTimeCondition->sharedCondition->conditionIsAction = false;
	propertyNode->firstTimeConditionNodeInList.push_back(newTimeCondition);
}

void addTenseOnlyTimeConditionToAction(GIAActionNode * actionNode, int tense)
{
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->sharedCondition->conditionName = tenseNameArray[tense];
	newTimeCondition->tense = tense;
	newTimeCondition->sharedCondition->conditionEntity = NULL;
	newTimeCondition->sharedCondition->parentAction = actionNode;
	newTimeCondition->sharedCondition->parentIsAction = true;
	newTimeCondition->sharedCondition->conditionIsAction = false;
	actionNode->firstTimeConditionNodeInList.push_back(newTimeCondition);
}


/*unfinished - needed any more?;
void convertEntityToProperty(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity)
{
	//configure property node
	propertyEntity->isProperty = true;
	propertyEntity->entityNodeContainingThisProperty = thingEntity;
	newProperty->entityNodeDefiningThisProperty = propertyEntity;
	
	//configure property definition node
	propertyEntity->firstAssociatedPropertyNodeInList.push_back(newProperty);
	
	//configure entity node containing this property
	thingEntity->firstPropertyNodeInList.push_back(newProperty);
}
*/


void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity)
{
	//configure entity node
	thingEntity->firstEntityNodeInDefinitionList.push_back(definitionEntity);
	
	//configure entity definition node
	definitionEntity->firstEntityNodeInDefinitionReverseList.push_back(thingEntity);		
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
			newOrExistingAction = actionEntity->firstAssociatedActionNodeInList.back();
		}
	}

	addActionToSubject(subjectEntity, actionEntity);

	addActionToObject(objectEntity, actionEntity);

}

	//conditions required to be added [eg when, where, how, why]
GIAActionNode * addAction(GIAEntityNode * actionEntity)
{
	//configure action node
	GIAActionNode * newAction = new GIAActionNode();
	newAction->actionName = actionEntity->entityName;
	newAction->entityNodeDefiningThisAction = actionEntity;
	actionEntity->firstAssociatedActionNodeInList.push_back(newAction);
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
		newOrExistingAction = actionEntity->firstAssociatedActionNodeInList.back();
	}

	if(subjectEntity->hasAssociatedPropertyTemp)
	{
		subjectEntity = subjectEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
	}
		
	//configure subject entity node
	subjectEntity->firstActionNodeInList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionSubjectEntity = subjectEntity;
	//cout << "BUG SHOULD BE JOE; subjectEntity->entityName = " << subjectEntity->entityName << endl;
	
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
		newOrExistingAction = actionEntity->firstAssociatedActionNodeInList.back();
	}

	if(objectEntity->hasAssociatedPropertyTemp)
	{
		objectEntity = objectEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
	}
		
	//configure object entity node
	objectEntity->firstIncomingActionNodeInList.push_back(newOrExistingAction);
	
	newOrExistingAction->actionObjectEntity = objectEntity;
}

void addLocationConditionToAction(GIAActionNode * actionNode, GIAEntityNode * locationConditionEntity)
{
	GIALocationConditionNode * newLocationCondition = new GIALocationConditionNode();
	newLocationCondition->sharedCondition->conditionName = locationConditionEntity->entityName;
	newLocationCondition->sharedCondition->conditionEntity = locationConditionEntity;
	newLocationCondition->sharedCondition->parentAction = actionNode;
	newLocationCondition->sharedCondition->parentIsAction = true;
	newLocationCondition->sharedCondition->conditionIsAction = false;
	actionNode->firstLocationConditionNodeInList.push_back(newLocationCondition);
	locationConditionEntity->firstLocationConditionNodeInReverseList.push_back(newLocationCondition);
}

void addTimeConditionToAction(GIAActionNode * actionNode, GIAEntityNode * timeConditionEntity)
{
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->sharedCondition->conditionName = timeConditionEntity->entityName;
	//TO DO: parse time info here [into seconds since start of universe, totalTimeInSeconds]
	newTimeCondition->sharedCondition->conditionEntity = timeConditionEntity;
	newTimeCondition->sharedCondition->parentAction = actionNode;
	newTimeCondition->sharedCondition->parentIsAction = true;
	newTimeCondition->sharedCondition->conditionIsAction = false;
	actionNode->firstTimeConditionNodeInList.push_back(newTimeCondition);
	timeConditionEntity->firstTimeConditionNodeInReverseList.push_back(newTimeCondition);
}

void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity)
{
	GIALocationConditionNode * newLocationCondition = new GIALocationConditionNode();
	newLocationCondition->sharedCondition->conditionName = locationConditionEntity->entityName;
	newLocationCondition->sharedCondition->conditionEntity = locationConditionEntity;
	newLocationCondition->sharedCondition->parentProperty = propertyNode;
	newLocationCondition->sharedCondition->parentIsAction = false;
	newLocationCondition->sharedCondition->conditionIsAction = false;
	propertyNode->firstLocationConditionNodeInList.push_back(newLocationCondition);
	locationConditionEntity->firstLocationConditionNodeInReverseList.push_back(newLocationCondition);
}

void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity)
{
	GIATimeConditionNode * newTimeCondition = new GIATimeConditionNode();
	newTimeCondition->sharedCondition->conditionName = timeConditionEntity->entityName;
	//TO DO: parse time info here [into seconds since start of universe, totalTimeInSeconds]
	newTimeCondition->sharedCondition->conditionEntity = timeConditionEntity;
	newTimeCondition->sharedCondition->parentProperty = propertyNode;
	newTimeCondition->sharedCondition->parentIsAction = false;
	newTimeCondition->sharedCondition->conditionIsAction = false;
	propertyNode->firstTimeConditionNodeInList.push_back(newTimeCondition);
	timeConditionEntity->firstTimeConditionNodeInReverseList.push_back(newTimeCondition);
}




void addActionConditionToAction(GIAActionNode * actionNode, GIAActionNode * actionConditionActionNode)
{
	GIAActionConditionNode * newActionCondition = new GIAActionConditionNode();
	newActionCondition->sharedCondition->conditionName = actionConditionActionNode->actionName;
	newActionCondition->sharedCondition->conditionAction = actionConditionActionNode;
	newActionCondition->sharedCondition->parentAction = actionNode;
	newActionCondition->sharedCondition->parentIsAction = true;
	newActionCondition->sharedCondition->conditionIsAction = true;
	actionNode->firstActionConditionNodeInList.push_back(newActionCondition);
	
	actionConditionActionNode->firstActionConditionNodeInReverseList.push_back(newActionCondition);
}

void addPropertyConditionToAction(GIAActionNode * actionNode, GIAEntityNode * propertyConditionEntity)
{
	GIAPropertyConditionNode * newPropertyCondition = new GIAPropertyConditionNode();
	newPropertyCondition->sharedCondition->conditionName = propertyConditionEntity->entityName;
	newPropertyCondition->sharedCondition->conditionEntity = propertyConditionEntity;
	newPropertyCondition->sharedCondition->parentAction = actionNode;
	newPropertyCondition->sharedCondition->parentIsAction = true;
	newPropertyCondition->sharedCondition->conditionIsAction = false;
	actionNode->firstPropertyConditionNodeInList.push_back(newPropertyCondition);
	
	propertyConditionEntity->firstPropertyConditionNodeInReverseList.push_back(newPropertyCondition);
}

void addActionConditionToProperty(GIAEntityNode * propertyNode, GIAActionNode * actionConditionActionNode)
{
	GIAActionConditionNode * newActionCondition = new GIAActionConditionNode();
	newActionCondition->sharedCondition->conditionName = actionConditionActionNode->actionName;
	newActionCondition->sharedCondition->conditionAction = actionConditionActionNode;
	newActionCondition->sharedCondition->parentProperty = propertyNode;
	newActionCondition->sharedCondition->parentIsAction = false;
	newActionCondition->sharedCondition->conditionIsAction = true;
	propertyNode->firstActionConditionNodeInList.push_back(newActionCondition);
	
	actionConditionActionNode->firstActionConditionNodeInReverseList.push_back(newActionCondition);
}

void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity)
{
	GIAPropertyConditionNode * newPropertyCondition = new GIAPropertyConditionNode();
	newPropertyCondition->sharedCondition->conditionName = propertyConditionEntity->entityName;
	newPropertyCondition->sharedCondition->conditionEntity = propertyConditionEntity;
	newPropertyCondition->sharedCondition->parentProperty = propertyNode;
	newPropertyCondition->sharedCondition->parentIsAction = false;
	newPropertyCondition->sharedCondition->conditionIsAction = false;
	propertyNode->firstPropertyConditionNodeInList.push_back(newPropertyCondition);
	
	propertyConditionEntity->firstPropertyConditionNodeInReverseList.push_back(newPropertyCondition);
}






void convertSentenceRelationsIntoGIAnetworkNodes(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, Sentence * firstSentenceInList)
{
	
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
	firstTimeNodeInNetwork->sharedCondition->conditionName = firstTimeNameInNetwork;
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

			GIAEntityNodeArrayFilled[w] = false;

			//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
			//GIAEntityNodeArrayHasAssociatedProperty[w] = false;
			
			//GIAActionNodeArrayFilled[w] = false;
		}
		Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			//cout << "currentFeatureInList->tense = " << currentFeatureInList->tense << endl;
			
			//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
			if((currentFeatureInList->tense).find(FEATURE_GRAMMATICAL_TENSE_DATE) != -1)
			{
				GIAEntityNodeIsDate[currentFeatureInList->entityIndex] = true;
				cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}
			
			for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
			{
				//NB only the first characters of currentFeatureInList->tense contain the tense type name 
				if((currentFeatureInList->tense).find(tenseNameArray[grammaticalTenseIndex]) != -1) 
				//if((currentFeatureInList->tense).substr(0, tenseNameLengthsArray[grammaticalTenseIndex]) == tenseNameArray[grammaticalTenseIndex]) 
				{
					GIAEntityNodeGrammaticalTenseArray[currentFeatureInList->entityIndex] = grammaticalTenseIndex;
					cout << "currentFeatureInList->entityIndex grammaticalTenseIndex = " << tenseNameArray[grammaticalTenseIndex] << endl;
				}			
			}
			for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
			{
				//NB only the first characters of currentFeatureInList->tense contain the tense type name 
				if((currentFeatureInList->tense).find(tenseNameArray[grammaticalNumberIndex]) != -1) 				
				{
					GIAEntityNodeGrammaticalNumberArray[currentFeatureInList->entityIndex] = grammaticalNumberIndex;
					cout << "currentFeatureInList->entityIndex grammaticalNumberIndex = " << tenseNameArray[grammaticalNumberIndex] << endl;
				}			
			}
			if((currentFeatureInList->tense).find(GRAMMATICAL_DEFINITE_NAME) != -1)
			{
				GIAEntityNodeGrammaticalIsDefiniteArray[currentFeatureInList->entityIndex] = GRAMMATICAL_DEFINITE;
				cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				
			}			
									 
			currentFeatureInList = currentFeatureInList->next;
		}
		//exit(0);
		
		//pass A; locate/add all entities [and define objects and subjects];
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
				GIAEntityNodeArray[relationFunctionIndex]->definiteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationFunctionIndex];
			
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
				GIAEntityNodeArray[relationArgumentIndex]->definiteTemp = GIAEntityNodeGrammaticalIsDefiniteArray[relationArgumentIndex];
				
				GIAEntityNodeArray[relationArgumentIndex]->hasAssociatedPropertyTemp = false;	
				GIAEntityNodeArray[relationFunctionIndex]->hasAssociatedActionTemp = false;		
			}
			
			currentRelationInList = currentRelationInList->next;
		}
		//cout << "as2" <<endl;
		
		//pass B;	

		//0 pass; define properties (definite nouns); eg the house
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeGrammaticalIsDefiniteArray[i] == GRAMMATICAL_DEFINITE)
			{
				addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
			}
		}
							
		//1 pass; define properties (possessive relationships); eg joe's bike
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			if(currentRelationInList->relationType == RELATION_TYPE_POSSESSIVE)
			{
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
		
		//2 pass; define properties (descriptive relationships); eg joe is happy
		currentRelationInList = currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{			
			if((currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_2) || (currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_3))
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
				
 		//3 pass; define dependent subject-object definition/composition/action relationships and independent subject/object action relationships
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
												
			if((currentRelationInList->relationType == RELATION_TYPE_SUBJECT) || ((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO)))
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
					if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
					{
						firstIndex = SUBJECT_INDEX;
						secondIndex = OBJECT_INDEX;			
					}
					else if((currentRelationInList->relationType == RELATION_TYPE_OBJECT) || (currentRelationInList->relationType == RELATION_TYPE_OBJECT_TO))
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

					//now find the associated object..
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{	
						//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;

						if(currentRelationInList2->relationType == partnerTypeRequired)
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

								if(currentRelationInList->relationFunction == RELATION_FUNCTION_DEFINITION_1) 
								{//expected subject-object relationship is a definition "is"
									
									//added 1 May 11a (assign actions to instances (properties) of entities and not entities themselves where appropriate)
										//NB definitions are only assigned to entities, not properties (instances of entities)
										
									addDefinitionToEntity(subjectObjectEntityArray[SUBJECT_INDEX], subjectObjectEntityArray[OBJECT_INDEX]);
								}
								else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
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
						else if((currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_1) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_2) || (currentRelationInList->relationFunction == RELATION_FUNCTION_COMPOSITION_3))
						{
						}
						else
						{//assume that the subject-object relationships is an action
							string actionName = currentRelationInList->relationFunction;
							cout << "actionName = " << actionName << endl;
							GIAEntityNode * actionEntity = GIAEntityNodeArray[relationFunctionIndex];

							addAction(actionEntity);
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
					GIAActionNode * actionNode = actionOrPropertyEntity->firstAssociatedActionNodeInList.back();
					
					if(timeOrLocationConditionEntity->hasAssociatedTime)
					{
						string timeConditionName = currentRelationInList->relationArgument; 
						
						//cout << "HERE" << endl;
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
							timeOrLocationConditionEntity = timeOrLocationConditionEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
						}
						
						addLocationConditionToAction(actionNode, timeOrLocationConditionEntity);
					}
				}
				else if(actionOrPropertyEntity->hasAssociatedPropertyTemp)
				//else if(actionOrPropertyEntity->hasAssociatedProperty)
				//else if(actionOrPropertyEntity->isProperty)		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)		
				{
					GIAEntityNode * propertyNode = actionOrPropertyEntity->firstAssociatedPropertyNodeInList.back();
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
							timeOrLocationConditionEntity = timeOrLocationConditionEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
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
					GIAActionNode * actionNode = actionOrPropertyEntity->firstAssociatedActionNodeInList.back();

					if(actionOrPropertyConditionEntity->hasAssociatedActionTemp)
					{//eg john rides his bike when joe plays with the table
						string actionConditionEntityName = currentRelationInList->relationArgument; 
						
						//cout << "l3" << endl;
						cout << "actionName = " << actionNode->actionName << endl;
						cout << "actionConditionEntityName = " << actionConditionEntityName << endl;

						GIAActionNode * actionConditionActionNode = actionOrPropertyConditionEntity->firstAssociatedActionNodeInList.back();
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
		
						GIAEntityNode * propertyNode = actionOrPropertyConditionEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a

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
					GIAEntityNode * propertyNode = actionOrPropertyEntity->firstAssociatedPropertyNodeInList.back();
					//GIAEntityNode * propertyNode = actionOrPropertyEntity;		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)	
					
					if(actionOrPropertyConditionEntity->hasAssociatedActionTemp)
					{//eg the stars are bright when john rides his bike
						string actionConditionEntityName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "actionConditionEntityName = " << actionConditionEntityName << endl;

						GIAActionNode * actionConditionActionNode = actionOrPropertyConditionEntity->firstAssociatedActionNodeInList.back();
						addActionConditionToProperty(propertyNode, actionConditionActionNode);
					}
					else if(actionOrPropertyConditionEntity->hasAssociatedPropertyTemp)
					//else if(actionOrPropertyConditionEntity->hasAssociatedProperty)
					//else if(actionOrPropertyConditionEntity->isProperty)		//?added 1 May 11a (assign action conditions to instances (properties) of entities and not entities themselves where appropriate)
					{//eg the stars are bright when the sun is dim. 
						string propertyConditionEntityName = currentRelationInList->relationArgument; 
						
						cout << "propertyName = " << propertyNode->entityName << endl;
						cout << "propertyConditionEntityName = " << propertyConditionEntityName << endl;

						GIAEntityNode * propertyConditionNode = actionOrPropertyConditionEntity->firstAssociatedPropertyNodeInList.back();	//added 4 May 11a
						
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
		currentSentenceInList = currentSentenceInList->next;
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
		
		while(searchOptionsAvailable == true)
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
		
		while(searchOptionsAvailable == true)
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




