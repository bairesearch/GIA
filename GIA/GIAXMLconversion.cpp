/*******************************************************************************
 *
 * File Name: GIAXMLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
 *
 *******************************************************************************/



#include "GIAXMLconversion.h"

//this function works and no longer needs to be tested
#ifdef GIA_XML_DEBUG
bool testReadSemanticNetXMLFile1()
{
	bool result = true;

	GIAEntityNode * firstEntityNodeInNetwork = new GIAEntityNode();	//the firstEntityNodeInNetwork object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,

	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, firstEntityNodeInNetwork))
	{
		result = false;
	}

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, firstEntityNodeInNetwork))
	{
		result = false;
	}
	return result;
}

bool testReadSemanticNetXMLFile2(GIAEntityNode * firstEntityNodeInNetwork)
{
	bool result = true;

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, firstEntityNodeInNetwork))
	{
		result = false;
	}

	GIAEntityNode * tempfirstEntityNodeInNetwork = new GIAEntityNode();
	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, tempfirstEntityNodeInNetwork))
	{
		result = false;
	}

	//cout << "here3" << endl;
	if(!writeNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, tempfirstEntityNodeInNetwork))
	{
		result = false;
	}
	return result;
}
#endif

bool readSemanticNetXMLFile(string xmlFileName, GIAEntityNode * firstEntityNodeInNetwork)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

		//cout << "a" << endl;

 	if(!readXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}
		//cout << "b" << endl;

	if(!parseSemanticNetTag(firstTagInXMLFile, firstEntityNodeInNetwork))
	{
		result = false;
	}

		//cout << "c" << endl;

	if(!linkEntities(firstEntityNodeInNetwork))
	{
		result = false;
	}
	return result;
}

//Top Level
bool parseSemanticNetTag(XMLParserTag * firstTagInNetwork, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork, bool linkConnections)
{
	bool result = true;

	XMLParserTag * currentTagUpdatedL1 = firstTagInNetwork;

	if(currentTagUpdatedL1->name == NET_XML_TAG_semanticNetwork)
	{
	}
	else
	{
		cout << "parseSemanticNetTag error: semanticNetwork tag not detected";
		result = false;
	}

	if(result)
	{
		XMLParserTag * currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
		if(currentTagUpdatedL2->name == NET_XML_TAG_entityNodeContainer)
		{
			GIAEntityNode * currentEntity = firstEntityNodeInNetwork;
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					cout << "Entity Node ID: " << currentTagUpdatedL3->value << endl;
					if(linkConnections)
					{
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity))
						{
							result = false;
						}
					}
					GIAEntityNode * newEntity = new GIAEntityNode();
					currentEntity->next = newEntity;
					currentEntity = currentEntity->next; 
				}
				else
				{
					cout << "parseSemanticNetTag error: entity node tag not detected";
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			cout << "parseSemanticNetTag error: no entity container node tag detected";
		}
		currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		if(currentTagUpdatedL2->name == NET_XML_TAG_actionNodeContainer)
		{
			GIAActionNode * currentAction = firstActionNodeInNetwork;
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_actionNode)
				{
					cout << "Action Node ID: " << currentTagUpdatedL3->value << endl;
					if(linkConnections)
					{
						if(!parseActionNodeTag(currentTagUpdatedL3, currentAction))
						{
							result = false;
						}
					}
					GIAActionNode * newAction = new GIAActionNode();
					currentAction->next = newAction;
					currentAction = currentAction->next; 					
				}
				else
				{
					cout << "parseSemanticNetTag error: action node tag not detected";
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			//cout << "parseSemanticNetTag error: no action container node tag detected";
		}
		
		if(currentTagUpdatedL2->name == NET_XML_TAG_conditionNodeContainer)
		{
			GIAConditionNode * currentCondition = firstConditionNodeInNetwork;
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_conditionNode)
				{
					cout << "Condition Node ID: " << currentTagUpdatedL3->value << endl;
					if(linkConnections)
					{
						if(!parseConditionNodeTag(currentTagUpdatedL3, currentCondition))
						{
							result = false;
						}
					}
					GIAConditionNode * newCondition = new GIAConditionNode();
					currentCondition->next = newCondition;
					currentCondition = currentCondition->next; 					
				}
				else
				{
					cout << "parseSemanticNetTag error: condition node tag not detected";
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			//cout << "parseSemanticNetTag error: no condition node container tag detected";
		}
	}

	return result;
}


bool parseActionNodeTag(XMLParserTag * firstTagInActionNode, GIAActionNode * actionNode)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool printXFound = false;
	bool printYFound = false;
	bool printXIndexFound = false;
	bool printYIndexFound = false;
	bool printTextXFound = false;
	bool printTextYFound = false;

	bool actionNameFound = false;
	bool ConditionNodeListFound = false;
	bool ConditionNodeReverseListFound = false;
	bool entityNodeDefiningThisActionFound = false;
	bool actionSubjectEntityFound = false;
	bool actionObjectEntityFound = false;
	
	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_actionName)
		{
			string attributeValue = currentAttribute->value.c_str();
			actionNode->actionName = attributeValue;
			actionNameFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityNodeDefiningThisAction)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->entityNodeDefiningThisAction = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			entityNodeDefiningThisActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_actionSubjectEntity)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->actionSubjectEntity = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			actionSubjectEntityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_actionObjectEntity)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->actionObjectEntity = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			actionObjectEntityFound = true;
		}

		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printX = attributeValue;
			printXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printY = attributeValue;
			printYFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printXIndex = attributeValue;
			printXIndexFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printYIndex = attributeValue;
			printYIndexFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printTextX = attributeValue;
			printTextXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->printTextY = attributeValue;
			printTextYFound = true;
		}
	
		currentAttribute = currentAttribute->nextAttribute;
	}

	XMLParserTag * currentTagUpdatedL3 = firstTagInEntityNode->firstLowerLevelTag;
	while(currentTagUpdatedL3->nextTag != NULL)
	{
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeList)
		{
			cout << "ConditionNodeList: " << endl;
			if(!parseConditionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, currentActionNode, firstConditionNodeInNetwork))
			{
				result = false;
			}	
			ConditionNodeListFound = true;				
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeReverseList)
		{
			cout << "ConditionNodeReverseList: " << endl;
			if(!parseConditionNodeReverseListTag(currentTagUpdatedL3->firstLowerLevelTag, currentActionNode, firstConditionNodeInNetwork))
			{
				result = false;
			}
			ConditionNodeReverseListFound = true;					
		}																				
		
		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}
			
				
	if(!entityNodeDefiningThisAction)
	{	
		cout << "parseActionNodeTag error: !entityNodeDefiningThisAction" << endl;
		result = false;				
	}
	if(!actionSubjectEntityFound || !actionObjectEntityFound)
	{	
		cout << "parseActionNodeTag error: !actionSubjectEntityFound || !actionObjectEntityFound" << endl;
		result = false;				
	}


	return result;	
}

bool parseConditionNodeListInActionTag(XMLParserTag * firstTagInConditionNodeList, GIAActionNode * actionNode, GIAConditionNode * firstConditionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_conditionNodeReference)
		{
			cout << "conditionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->ConditionNodeList.push_back(findConditionNodeByID(attributeValue, firstConditionNodeInNetwork));
		}
		else
		{
			cout << "parseConditionNodeListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseConditionNodeReverseListInActionTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAActionNode * actionNode, GIAActionNode * firstConditionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeReverseList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_conditionNodeReference)
		{
			cout << "conditionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			actionNode->ConditionNodeReverseList.push_back(findConditionNodeByID(attributeValue, firstConditionNodeInNetwork));
		}
		else
		{
			cout << "parseConditionNodeReverseListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}


bool parseConditionNodeTag(XMLParserTag * firstTagInConditionNode, GIAConditionNode * conditionNode)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool conditionNameFound = false;
	bool conditionIsActionFound = false;
	bool conditionActionFound = false;
	bool conditionEntityFound = false;
	bool parentIsActionFound = false;
	bool parentActionFound = false;
	bool parentPropertyFound = false;
	bool conditionTypeFound = false;
	bool timeConditionNodeFound = false;
	
	bool printXFound = false;
	bool printYFound = false;
	bool printXIndexFound = false;
	bool printYIndexFound = false;
	bool printTextXFound = false;
	bool printTextYFound = false;
	
	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionName)
		{
			string attributeValue = currentAttribute->value.c_str();
			conditionNode->conditionName = attributeValue;
			conditionNameFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionIsAction)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->conditionIsAction = attributeValue;
			conditionIsActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionAction)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->conditionAction = findActionNodeByID(attributeValue, firstActionNodeInNetwork);
			conditionActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionEntity)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->conditionEntity = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			conditionEntityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_parentIsAction)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->parentIsAction = attributeValue;
			parentIsActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_parentAction)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->parentAction = findActionNodeByID(attributeValue, firstActionNodeInNetwork);
			parentActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_parentProperty)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->parentProperty = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			parentPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->conditionType = attributeValue;
			conditionTypeFound = true;
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printX = attributeValue;
			printXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printY = attributeValue;
			printYFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printXIndex = attributeValue;
			printXIndexFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printYIndex = attributeValue;
			printYIndexFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printTextX = attributeValue;
			printTextXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			conditionNode->printTextY = attributeValue;
			printTextYFound = true;
		}
	
		currentAttribute = currentAttribute->nextAttribute;
	}
		
	if(conditionTypeFound && (conditionType == CONDITION_NODE_TYPE_TIME))
	{	
		XMLParserTag * currentTagUpdatedL3 = firstTagInConditionNode->firstLowerLevelTag;
		while(currentTagUpdatedL3->nextTag != NULL)
		{
			if(currentTagUpdatedL3->name == NET_XML_TAG_timeConditionNode)
			{
				cout << "timeConditionNode: " << endl;
				if(!parseTimeConditionNodeListTag(currentTagUpdatedL3, conditionNode->timeConditionNode))
				{
					result = false;
				}	
				timeConditionNodeFound = true;				
			}																				

			currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
		}
	}
			
				
	if(parentIsActionFound)
	{	
		if(conditionNode->parentIsAction)
		{
			if(!parentActionFound)
			{
				cout << "parseConditionNodeTag error: parentIsActionFound && parentIsAction && !parentActionFound" << endl;
			}
			if(parentPropertyFound)
			{
				cout << "parseConditionNodeTag error: parentIsActionFound && parentIsAction && parentProperty" << endl;
			}
		}
		else
		{
			if(parentActionFound)
			{
				cout << "parseConditionNodeTag error: parentIsActionFound && !parentIsAction && parentActionFound" << endl;
			}
			if(!parentPropertyFound)
			{
				cout << "parseConditionNodeTag error: parentIsActionFound && !parentIsAction && !parentPropertyFound" << endl;
			}		
		}		
		result = false;				
	}
	else
	{
		cout << "parseConditionNodeTag error: !parentIsActionFound" << endl;
	}
	
	if(conditionIsActionFound)
	{	
		if(conditionNode->conditionIsAction)
		{
			if(!conditionActionFound)
			{
				cout << "parseConditionNodeTag error: conditionIsActionFound && conditionIsAction && !conditionActionFound" << endl;
			}
			if(conditionEntityFound)
			{
				cout << "parseConditionNodeTag error: conditionIsActionFound && conditionIsAction && conditionEntityFound" << endl;
			}
		}
		else
		{
			if(conditionActionFound)
			{
				cout << "parseConditionNodeTag error: conditionIsActionFound && !conditionIsAction && conditionActionFound" << endl;
			}
			if(!conditionEntityFound)
			{
				cout << "parseConditionNodeTag error: conditionIsActionFound && !conditionIsAction && !conditionEntityFound" << endl;
			}		
		}		
		result = false;				
	}
	else
	{
		cout << "parseConditionNodeTag error: !conditionIsActionFound" << endl;
	}

	return result;		
		
}

bool parseTimeConditionNodeTag(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool tenseFound = false;
	bool secondFound = false;
	bool hourFound = false;
	bool dayOfWeekFound = false;
	bool monthFound = false;
	bool dayOfMonthFound = false;
	bool yearFound = false;
	bool periodFound = false;
	bool totalTimeInSecondsFound = false;

		
}




bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = firstTagInEntityNode->firstAttribute;

	bool grammaticalNumberFound = false;
	bool hasQuantityFound = false;
	bool quantityNumberFound = false;
	bool quantityModifierFound = false;
	bool quantityModifierStringFound = false;
	bool hasMeasureFound = false;
	bool measureTypeFound = false;

	bool printXFound = false;
	bool printYFound = false;
	bool printXIndexFound = false;
	bool printYIndexFound = false;
	bool printTextXFound = false;
	bool printTextYFound = false;
	
	bool entityNameFound = false;
	bool confidenceFound = false;
	bool isPropertyFound = false;
	bool hasAssociatedPropertyFound = false;
	bool hasAssociatedActionFound = false;
	bool hasAssociatedTimeFound = false;
	
	bool ActionNodeListFound = false;
	bool IncomingActionNodeListFound = false;
	
	bool PropertyNodeListFound = false;
	bool entityNodeContainingThisPropertyFound = false;
	bool entityNodeDefiningThisPropertyFound = false;
	
	bool EntityNodeDefinitionListFound = false;
	bool EntityNodeDefinitionReverseListFound = false;
	bool AssociatedActionNodeListFound = false;
	bool AssociatedPropertyNodeListFound = false;
	
	bool ConditionNodeListFound = false;
	bool ConditionNodeReverseListFound = false;
	
	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	//parseEntityNodeTag->... = 0;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalNumber)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->grammaticalNumber = attributeValue;
			grammaticalNumberFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasQuantity)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasQuantity = attributeValue;
			hasQuantityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityNumber)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->quantityNumber = attributeValue;
			quantityNumberFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityModifier)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->quantityModifier = attributeValue;
			quantityModifierFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityModifierString)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->quantityModifierString = attributeValue;
			quantityModifierStringFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasMeasure)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasMeasure = attributeValue;
			hasMeasureFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_measureType)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->measureType = attributeValue;
			measureTypeFound = true;
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printX = attributeValue;
			printXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printY = attributeValue;
			printYFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printXIndex = attributeValue;
			printXIndexFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printYIndex = attributeValue;
			printYIndexFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printTextX = attributeValue;
			printTextXFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printTextY = attributeValue;
			printTextYFound = true;
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityName)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->entityName = attributeValue;
			entityNameFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_confidence)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			entityNode->confidence = attributeValue;
			confidenceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isProperty)
		{
			int attributeValue = atof(currentAttribute->value.c_str());
			entityNode->isProperty = attributeValue;
			isPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedProperty)
		{
			bool attributeValue = atof(currentAttribute->value.c_str());
			entityNode->hasAssociatedProperty = attributeValue;
			hasAssociatedPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedAction)
		{
			bool attributeValue = atof(currentAttribute->value.c_str());
			entityNode->hasAssociatedAction = attributeValue;
			hasAssociatedActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedTime)
		{
			bool attributeValue = atof(currentAttribute->value.c_str());
			entityNode->hasAssociatedTime = attributeValue;
			hasAssociatedTimeFound = true;
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityNodeContainingThisProperty)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->entityNodeContainingThisProperty = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			entityNodeContainingThisPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityNodeDefiningThisProperty)
		{
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->entityNodeDefiningThisProperty = findEntityNodeByID(attributeValue, firstEntityNodeInNetwork);
			entityNodeDefiningThisPropertyFound = true;
		}		
		
		currentAttribute = currentAttribute->nextAttribute;
	}

	XMLParserTag * currentTagUpdatedL3 = firstTagInEntityNode->firstLowerLevelTag;
	while(currentTagUpdatedL3->nextTag != NULL)
	{
		if(currentTagUpdatedL3->name == NET_XML_TAG_ActionNodeList)
		{
			cout << "ActionNodeList: " << endl;
			if(!parseActionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstActionNodeInNetwork))
			{
				result = false;
			}
			ActionNodeListFound = true;					
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_IncomingActionNodeList)
		{
			cout << "IncomingActionNodeList: " << endl;
			if(!parseIncomingActionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstActionNodeInNetwork))
			{
				result = false;
			}	
			IncomingActionNodeListFound = true;				
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_PropertyNodeList)
		{
			cout << "PropertyNodeList: " << endl;
			if(!parsePropertyNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstEntityNodeInNetwork))
			{
				result = false;
			}				
			PropertyNodeListFound = true;	
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_EntityNodeDefinitionList)
		{
			cout << "EntityNodeDefinitionList: " << endl;
			if(!parseEntityNodeDefinitionListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstEntityNodeInNetwork))
			{
				result = false;
			}				
			EntityNodeDefinitionListFound = true;	
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_EntityNodeDefinitionReverseList)
		{
			cout << "EntityNodeDefinitionReverseList: " << endl;
			if(!parseEntityNodeDefinitionReverseListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstEntityNodeInNetwork))
			{
				result = false;
			}				
			EntityNodeDefinitionReverseListFound = true;	
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_AssociatedActionNodeList)
		{
			cout << "AssociatedActionNodeList: " << endl;
			if(!parseAssociatedActionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstActionNodeInNetwork))
			{
				result = false;
			}				
			AssociatedActionNodeListFound = true;	
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_AssociatedPropertyNodeList)
		{
			cout << "AssociatedPropertyNodeList: " << endl;
			if(!parseAssociatedPropertyNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstEntityNodeInNetwork))
			{
				result = false;
			}
			AssociatedPropertyNodeListFound = true;						
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeList)
		{
			cout << "ConditionNodeList: " << endl;
			if(!parseConditionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstConditionNodeInNetwork))
			{
				result = false;
			}	
			ConditionNodeListFound = true;				
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeReverseList)
		{
			cout << "ConditionNodeReverseList: " << endl;
			if(!parseConditionNodeReverseListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, firstConditionNodeInNetwork))
			{
				result = false;
			}
			ConditionNodeReverseListFound = true;					
		}																				
		
		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}
			
				
	if(isPropertyFound && entityNode->isProperty)
	{
		/*
		if(!entityNodeContainingThisPropertyFound)
		{
			result = false;
		}
		*/
		if(!entityNodeDefiningThisPropertyFound)
		{
			cout << "parseEntityNodeTag error: isPropertyFound && entityNode->isProperty && !entityNodeDefiningThisPropertyFound" << endl;
			result = false;
		}				
	}
	else
	{
	
	}


	return result;
}



bool parseActionNodeListTag(XMLParserTag * firstTagInActionNodeList, GIAActionNode * entityNode, GIAActionNode * firstActionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInActionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_actionNodeReference)
		{
			cout << "actionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->ActionNodeList.push_back(findActionNodeByID(attributeValue, firstActionNodeInNetwork));
		}
		else
		{
			cout << "parseActionNodeListTag error: actionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}	
}

bool parseIncomingActionNodeListTag(XMLParserTag * firstTagInIncomingActionNodeList, GIAEntityNode * entityNode, GIAActionNode * firstActionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInIncomingActionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_actionNodeReference)
		{
			cout << "actionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->IncomingActionNodeList.push_back(findActionNodeByID(attributeValue, firstActionNodeInNetwork));
		}
		else
		{
			cout << "parseIncomingActionNodeListTag error: actionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parsePropertyNodeListTag(XMLParserTag * firstTagInPropertyNodeList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInPropertyNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->PropertyNodeList.push_back(findEntityNodeByID(attributeValue, firstEntityNodeInNetwork));
		}
		else
		{
			cout << "parsePropertyNodeListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseEntityNodeDefinitionListTag(XMLParserTag * firstTagInEntityNodeDefinitionList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInEntityNodeDefinitionList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->EntityNodeDefinitionList.push_back(findEntityNodeByID(attributeValue, firstEntityNodeInNetwork));
		}
		else
		{
			cout << "parseEntityNodeDefinitionListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseEntityNodeDefinitionReverseListTag(XMLParserTag * firstTagInEntityNodeDefinitionReverseList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInEntityNodeDefinitionReverseList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->EntityNodeDefinitionReverseList.push_back(findEntityNodeByID(attributeValue, firstEntityNodeInNetwork));
		}
		else
		{
			cout << "parseEntityNodeDefinitionReverseListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseAssociatedActionNodeListTag(XMLParserTag * firstTagInAssociatedActionNodeList, GIAEntityNode * entityNode, GIAActionNode * firstActionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInAssociatedActionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_actionNodeReference)
		{
			cout << "actionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->AssociatedActionNodeList.push_back(findActionNodeByID(attributeValue, firstActionNodeInNetwork));
		}
		else
		{
			cout << "parseAssociatedActionNodeListTag error: actionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseAssociatedPropertyNodeListTag(XMLParserTag * firstTagInAssociatedPropertyNodeList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInAssociatedPropertyNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->AssociatedPropertyNodeList.push_back(findEntityNodeByID(attributeValue, firstEntityNodeInNetwork));
		}
		else
		{
			cout << "parseAssociatedPropertyNodeListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseConditionNodeListTag(XMLParserTag * firstTagInConditionNodeList, GIAEntityNode * entityNode, GIAConditionNode * firstConditionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_conditionNodeReference)
		{
			cout << "conditionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->ConditionNodeList.push_back(findConditionNodeByID(attributeValue, firstConditionNodeInNetwork));
		}
		else
		{
			cout << "parseConditionNodeListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseConditionNodeReverseListTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAEntityNode * entityNode, GIAActionNode * firstConditionNodeInNetwork)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeReverseList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_conditionNodeReference)
		{
			cout << "conditionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->ConditionNodeReverseList.push_back(findConditionNodeByID(attributeValue, firstConditionNodeInNetwork));
		}
		else
		{
			cout << "parseConditionNodeReverseListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}


GIAEntityNode * findEntityNodeByID(long EntityNodeID, GIAEntityNode * firstEntityNodeInNetwork)
{
	GIAEntityNode * foundEntityNode = NULL;
	GIAEntityNode * currentEntityNode = firstEntityNodeInNetwork;
	while(currentEntityNode->next != NULL)
	{
		if(currentEntityNode->id == EntityNodeID)
		{
			foundEntityNode = currentEntityNode;
		}
		currentEntityNode = currentEntityNode->next;
	}
	return foundEntityNode;
}

GIAActionNode * findActionNodeByID(long ActionNodeID, GIAActionNode * firstActionNodeInNetwork)
{
	GIAActionNode * foundActionNode = NULL;
	GIAActionNode * currentActionNode = firstActionNodeInNetwork;
	while(currentActionNode->next != NULL)
	{
		if(currentActionNode->id == ActionNodeID)
		{
			foundActionNode = currentActionNode;
		}
		currentActionNode = currentActionNode->next;
	}
	return foundActionNode;
}

GIAConditionNode * findConditionNodeByID(long conditionNodeID, GIAConditionNode * firstConditionNodeInNetwork)
{
	GIAConditionNode * foundConditionNode = NULL;
	GIAConditionNode * currentConditionNode = firstConditionNodeInNetwork;
	while(currentConditionNode->next != NULL)
	{
		if(currentConditionNode->id == ConditionNodeID)
		{
			foundConditionNode = currentConditionNode;
		}
		currentConditionNode = currentConditionNode->next;
	}
	return foundConditionNode;
}




bool writeSemanticNetXMLFile(string xmlFileName, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

	XMLParserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_semanticNetwork;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	if(!parseEntityContainerNodeTag(currentTagL1, firstEntityNodeInNetwork))
	{
		result = false;
	}
	
	currentTagL1 = currentTagL1->nextTag
	XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag2;
	
	if(!parseEntityContainerNodeTag(currentTagL1, firstActionNodeInNetwork))
	{
		result = false;
	}

	currentTagL1 = currentTagL1->nextTag
	XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag3;
	
	if(!parseEntityContainerNodeTag(currentTagL1, firstConditionNodeInNetwork))
	{
		result = false;
	}
	

	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;


	return result;
}


bool generateXMLTagListBasedUponSemanticNetEntityList(XMLParserTag * firstTagInSemanticNet, GIAEntityNode * firstEntityNodeInNetwork)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagL0 = firstTagInSemanticNet;
	NeuronContainer * currentEntity = firstEntityNodeInNetwork;

	bool stillMoreLayers = true;
	while(stillMoreLayers)
	{
		bool currentLayerHasFrontLayer = false;

		NeuronContainer * firstNeuronInLayer = currentNeuron;

		currentTagL0->name = NET_XML_TAG_layer;
		XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
		currentTagL0->nextTag = newTag0;

		XMLParserTag * firstTagL1 = new XMLParserTag();
		currentTagL0->firstLowerLevelTag = firstTagL1;
		XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
		XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
		currentTagL1->nextTag = newTag1;

		//cout << "h4" << endl;

		while(currentNeuron->nextNeuronContainer != NULL)
		{
			//cout << "h5" << endl;

			char tempString[MAX_ATTRIBUTE_VALUE_SIZE];

			//generate neuron connection tag
			currentTagL1->name = NET_XML_TAG_neuronContainer;
			XMLParserTag * firstTagL2 = new XMLParserTag();
			currentTagL1->firstLowerLevelTag = firstTagL2;
			XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

			//generate neuron tag
			currentTagL2->name = NET_XML_TAG_neuron;

			XMLParserAttribute * currentAttribute = currentTagL2->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%d", (currentNeuron->neuron->id));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute1;
			currentAttribute = currentAttribute->nextAttribute;

			//cout << "h5a" << endl;

		#ifndef DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS

			currentAttribute->name = NET_XML_ATTRIBUTE_layerID;
			sprintf(tempString, "%d", (currentNeuron->neuron->layerID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute2;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_orderID;
			sprintf(tempString, "%d", (currentNeuron->neuron->orderID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute3 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute3;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_subnetID;
			sprintf(tempString, "%d", (currentNeuron->neuron->subnetID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute4 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute4;
			currentAttribute = currentAttribute->nextAttribute;
		#endif
			//cout << "h5b" << endl;

		#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

			currentAttribute->name = NET_XML_ATTRIBUTE_bias;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->bias));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute5 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute5;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_output;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->output));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute6 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute6;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_classTarget;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->classTarget));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute7 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute7;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_error;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->error));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute8 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute8;
			currentAttribute = currentAttribute->nextAttribute;
		#endif

			//cout << "h5cB1" << endl;

		#ifndef DO_NOT_STORE_NET_XML_NEURON_SPATIAL_COORD_PARAMETERS

			if(currentNeuron->neuron->spatialCoordinatesSet)
			{

				currentAttribute->name = NET_XML_ATTRIBUTE_xPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->xPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute9 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute9;
				currentAttribute = currentAttribute->nextAttribute;

				currentAttribute->name = NET_XML_ATTRIBUTE_yPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->yPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute10 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute10;
				currentAttribute = currentAttribute->nextAttribute;

				currentAttribute->name = NET_XML_ATTRIBUTE_zPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->zPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute11 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute11;
				//currentAttribute = currentAttribute->nextAttribute;
			}
		#endif
			//cout << "h5d" << endl;


			XMLParserTag * newTag = new XMLParserTag();
			currentTagL2->nextTag = newTag;
			currentTagL2 = currentTagL2->nextTag;

			//cout << "h5e" << endl;

			//generate NET_XML_TAG_forwardNeuronConnectionsList tag
			if(currentNeuron -> firstFrontNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
			{
				//cout << "h6" << endl;
				currentTagL2->name = NET_XML_TAG_forwardNeuronConnectionsList;
				XMLParserTag * firstTagL3 = new XMLParserTag();
				currentTagL2->firstLowerLevelTag = firstTagL3;
				XMLParserTag * currentTagL3 = currentTagL2->firstLowerLevelTag;
				NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;
				while(currentNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
				{
					//cout << "h7" << endl;

					//generate NET_XML_TAG_forwardNeuronConnection tag and attributes
					currentTagL3->name = NET_XML_TAG_forwardNeuronConnection;

					XMLParserAttribute * currentAttribute = currentTagL3->firstAttribute;

					currentAttribute->name = "neuronID";
					sprintf(tempString, "%d", (currentNeuronConnectionContainer->neuron->id));
					currentAttribute->value = tempString;

					XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
					currentAttribute->nextAttribute = newAttribute1;
					currentAttribute = currentAttribute->nextAttribute;

				#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
					currentAttribute->name = "weight";
					sprintf(tempString, "%0.6f", (currentNeuronConnectionContainer->neuronConnection->weight));
					currentAttribute->value = tempString;

					XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
					currentAttribute->nextAttribute = newAttribute2;
					//currentAttribute = currentAttribute->nextAttribute;
				#endif

					XMLParserTag * newTag = new XMLParserTag();
					currentTagL3->nextTag = newTag;
					currentTagL3 = currentTagL3->nextTag;

					currentNeuronConnectionContainer = currentNeuronConnectionContainer -> nextNeuronConnectionContainer;

					//cout << "h7b" << endl;

				}
				XMLParserTag * newTag = new XMLParserTag();
				currentTagL2->nextTag = newTag;

				//cout << "h6b" << endl;


			}

		#ifdef ANN
			currentTagL2 = currentTagL2->nextTag;

			//generate subtag tag
			if(currentNeuron -> isSubnet)
			{
				currentTagL2->name = NET_XML_TAG_subnet;

				XMLParserTag * newTag1 = new XMLParserTag();
				currentTagL2->firstLowerLevelTag = newTag1;
				if(!generateXMLTagListBasedUponSubnet(currentTagL2->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet))
				{
					result = false;
				}

				XMLParserTag * newTag = new XMLParserTag();
				currentTagL2->nextTag = newTag;
				//currentTagL2 = currentTagL2->nextTag;
			}
		#endif

			XMLParserTag * newTag2 = new XMLParserTag();
			currentTagL1->nextTag = newTag2;
			currentTagL1 = currentTagL1->nextTag;

			currentNeuron = currentNeuron->nextNeuronContainer;
		}

		//cout << "h8" << endl;

		if(firstNeuronInLayer->hasFrontLayer)
		{
			currentNeuron=firstNeuronInLayer->firstNeuronInFrontLayer;
		}
		else
		{
			stillMoreLayers = false;
		}

		XMLParserTag * newTag = new XMLParserTag();
		currentTagL0->nextTag = newTag;
		currentTagL0 = currentTagL0->nextTag;
	}

	return result;
}


bool linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(NeuronContainer * firstNeuronContainerInLayer, NeuronContainer * firstInputNeuronInNetwork, bool hasBackLayer, NeuronContainer * firstNeuronContainerInBackLayer)
{
	bool result = true;

	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;
		while(currentNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
		{
			bool tempResult = false;

			NeuronContainer * neuronToConnect = findNeuronContainer(firstInputNeuronInNetwork, currentNeuronConnectionContainer->neuronID, &tempResult);
			if(tempResult)
			{
				//link neurons
				currentNeuronConnectionContainer->neuron = neuronToConnect->neuron;
				neuronToConnect->currentBackNeuronConnectionContainer->neuron = currentNeuron->neuron;		//OLD: 				neuronToConnect->currentBackNeuronConnectionContainer->neuron = currentNeuronConnectionContainer->neuron;
				neuronToConnect->currentBackNeuronConnectionContainer->neuronConnection = currentNeuronConnectionContainer->neuronConnection;

				NeuronConnectionContainer * newNeuronConnectionContainer = new NeuronConnectionContainer();
				neuronToConnect->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainer;
				neuronToConnect->currentBackNeuronConnectionContainer = neuronToConnect->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;
			}
			else
			{
				cout << "linkNeuronsBasedUponConnectionIDs error: neuron in neuron connection list not found. neuron id being searched for = " << currentNeuronConnectionContainer->neuronID <<  endl;
				result = false;
			}

			currentNeuronConnectionContainer = currentNeuronConnectionContainer -> nextNeuronConnectionContainer;
		}

		if(firstNeuronContainerInLayer->firstNeuronInFrontLayer != NULL)
		{
			currentNeuron->hasFrontLayer = true;
			currentNeuron->firstNeuronInFrontLayer = firstNeuronContainerInLayer->firstNeuronInFrontLayer;
		}
		if(hasBackLayer)
		{
			currentNeuron->hasBackLayer = true;
			currentNeuron->firstNeuronInBackLayer = firstNeuronContainerInBackLayer;
		}

	#ifdef ANN
		if(currentNeuron->firstNeuronContainerInBackLayerOfSubnet != NULL)
		{
			currentNeuron->isSubnet = true;
			if(currentNeuron->hasFrontLayer != true)
			{
				currentNeuron->isInputSubnet = true;
			}
			if(currentNeuron->hasFrontLayer != true)
			{
				currentNeuron->isOutputSubnet = true;
			}

			if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, firstInputNeuronInNetwork, false, NULL))
			{
				result = false;
			}

			NeuronContainer * firstOutputNeuronInSubnet;
			long temp;
			firstOutputNeuronInSubnet = recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, &temp, &temp);
			currentNeuron->firstNeuronContainerInFrontLayerOfSubnet	= firstOutputNeuronInSubnet;

		}
	#endif

		currentNeuron = currentNeuron -> nextNeuronContainer;
	}
	if(firstNeuronContainerInLayer->hasFrontLayer)
	{
		if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(firstNeuronContainerInLayer->firstNeuronInFrontLayer, firstInputNeuronInNetwork, true, firstNeuronContainerInLayer))
		{
			result = false;
		}
	}

	return result;
}


NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDToFind, bool * result)
{
	NeuronContainer * foundNeuronContainer = NULL;


	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		if(currentNeuron->neuron->id == neuronIDToFind)
		{
			//cout << "currentNeuron->neuron->id  = " << currentNeuron->neuron->id << endl;
			*result = true;
			foundNeuronContainer = currentNeuron;
		}
	#ifdef ANN
		if(currentNeuron->firstNeuronContainerInBackLayerOfSubnet != NULL)
		{
			bool tempResult = false;
			NeuronContainer * tempFoundNeuronContainer;
			tempFoundNeuronContainer = findNeuronContainer(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, neuronIDToFind, &tempResult);
			if(tempResult)
			{
				*result = true;
				foundNeuronContainer = tempFoundNeuronContainer;
			}
		}
	#endif
		currentNeuron = currentNeuron -> nextNeuronContainer;
	}

	//cout << "firstNeuronContainerInLayer ID = " << firstNeuronContainerInLayer->neuron->id << endl;
	if(firstNeuronContainerInLayer->firstNeuronInFrontLayer != NULL)
	{
		//cout << "trace1" << endl;
		bool tempResult = false;
		NeuronContainer * tempFoundNeuronContainer;
		tempFoundNeuronContainer = findNeuronContainer(firstNeuronContainerInLayer->firstNeuronInFrontLayer, neuronIDToFind, &tempResult);
		if(tempResult)
		{
			*result = true;
			foundNeuronContainer = tempFoundNeuronContainer;
		}
	}

	return foundNeuronContainer;
}





bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode)
{
	bool result = true;

	XMLParserTag * currentTag = firstTagInNeuronContainer;

	if(currentTag->name == NET_XML_TAG_neuron)
	{
		if(!parseNeuronTag(currentTag, currentNeuron, layerIDCounter, orderIDCounter, wrongAndNotUsedIDCounter, subnetIDCounter))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		cout << "parseNeuronContainerTag error: no neuron tag detected";
	}

	if(currentTag->name == NET_XML_TAG_forwardNeuronConnectionsList)
	{
		if(!parseForwardNeuronConnectionsListTag(currentTag->firstLowerLevelTag, currentNeuron))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		//NB there may not be a forwardNeuronConnectionsList
		//cout << "parseNeuronContainerTag error: no forwardNeuronConnectionsList tag detected";
	}

#ifdef ANN
	if(currentTag->name == NET_XML_TAG_subnet)
	{
		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->firstNeuronContainerInBackLayerOfSubnet = newNeuron;
		if(!parseSubnetTag(currentTag->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet, 1, wrongAndNotUsedIDCounter, (subnetIDCounter+1)))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
#endif

	if(currentTag->nextTag != NULL)
	{
		cout << "parseNeuronContainerTag error: end of content not detected";
	}

	return result;
}



bool parseForwardNeuronConnectionsListTag(XMLParserTag * firstTagInForwardNeuronConnectionsList, NeuronContainer * currentNeuron)
{
	bool result = true;

	XMLParserTag * currentTag = firstTagInForwardNeuronConnectionsList;
	NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;

	while(currentTag->nextTag != NULL)
	{
		//create a new neuron connection for currentNeuronConnectionContainer
		NeuronConnection * newNeuronConnection = new NeuronConnection();
		currentNeuronConnectionContainer->neuronConnection = newNeuronConnection;

		if(!parseForwardNeuronConnectionTag(currentTag, currentNeuronConnectionContainer))
		{
			result = false;
		}

		//create a new currentNeuronConnectionContainer
		NeuronConnectionContainer * newNeuronConnectionContainer = new NeuronConnectionContainer();
		currentNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->nextNeuronConnectionContainer;

		currentTag = currentTag->nextTag;
	}

	return result;

}


bool parseForwardNeuronConnectionTag(XMLParserTag * currentTag, NeuronConnectionContainer * currentNeuronConnectionContainer)
{
	bool result = true;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool neuronIDFound = false;
	bool weightFound = false;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_neuronID)
		{
			currentNeuronConnectionContainer->neuronID = atof(currentAttribute->value.c_str());		//temporary variable used to link neuron connections at a later stage
			neuronIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_weight)
		{
			currentNeuronConnectionContainer->neuronConnection->weight = atof(currentAttribute->value.c_str());
			weightFound = true;
		}

		currentAttribute = currentAttribute->nextAttribute;
	}

	if(!neuronIDFound)
	{
		cout << "parseForwardNeuronConnectionsListTag error: neuronID attribute not detected";
		result = false;
	}
#ifdef ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
	if(!weightFound)
	{
		cout << "parseForwardNeuronConnectionsListTag error: weight attribute not detected";
		result = false;
	}
#endif

	return result;

}







/*
XMLParserTag * parseTagDownALevel(XMLParserTag * currentTag, string sectionTagName, bool * result)
{
	*result = true;

	XMLParserTag * updatedCurrentTag;

	if(currentTag->name != sectionTagName)
	{
		*result = false;
		cout << "parse error; lower level sectionTagName expected = " <<  sectionTagName << ". section tag name found currentTag->name = " << currentTag->name << endl;
		updatedCurrentTag = currentTag;
	}
	else
	{
		updatedCurrentTag = currentTag->firstLowerLevelTag;
	}

	return updatedCurrentTag;

}
*/

