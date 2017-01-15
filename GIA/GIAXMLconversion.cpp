/*******************************************************************************
 *
 * File Name: GIAXMLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function overwrites entity id values upon read/write to speed up linking process
 *
 *******************************************************************************/



#include "GIAXMLconversion.h"
#include "GIAdatabase.h"

//this function works and no longer needs to be tested
#ifdef GIA_XML_DEBUG
bool testReadSemanticNetXMLFile1()
{
	bool result = true;

	vector<GIAEntityNode*> *entityNodesCompleteList;	//the entityNodesCompleteList object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,
	vector<GIAEntityNode*> *conceptEntityNodesList;
	vector<GIAEntityNode*> *propertyEntityNodesList;
	vector<GIAEntityNode*> *actionEntityNodesList;
	vector<string> *conceptEntityNamesList;
	
	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conceptEntityNamesList))
	{
		result = false;
	}

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conceptEntityNamesList))
	{
		result = false;
	}
	
	return result;
}

bool testReadSemanticNetXMLFile2(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<string> * conceptEntityNamesList)
{
	bool result = true;

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conceptEntityNamesList))
	{
		result = false;
	}

	vector<GIAEntityNode*> tempentityNodesCompleteList;
	vector<GIAEntityNode*> tempconceptEntityNodesList;
	vector<GIAEntityNode*> temppropertyEntityNodesList;
	vector<GIAEntityNode*> tempactionEntityNodesList;
	vector<string> tempconceptEntityNamesList;
	
	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, &tempentityNodesCompleteList, &tempconceptEntityNodesList, &temppropertyEntityNodesList, &tempactionEntityNodesList, &tempconceptEntityNamesList))
	{
		result = false;
	}

	//cout << "here3" << endl;
	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, &tempentityNodesCompleteList, &tempconceptEntityNodesList, &temppropertyEntityNodesList, &tempactionEntityNodesList, &tempconceptEntityNamesList))
	{
		result = false;
	}
	return result;
}
#endif

bool readSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<string> * conceptEntityNamesList)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

		//cout << "a" << endl;

 	if(!readXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}
		//cout << "b" << endl;

	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conceptEntityNamesList, false))
	{
		result = false;
	}
		//cout << "b2" << endl;
	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conceptEntityNamesList, true))
	{
		result = false;
	}
	
		//cout << "c" << endl;

	
	return result;
}

//Top Level
bool parseSemanticNetTag(XMLParserTag * firstTagInNetwork, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<string> * conceptEntityNamesList, bool linkConnections)
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
		long currentEntityNodeIDInCompleteList = 0;
		
		XMLParserTag * currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
		if(currentTagUpdatedL2->name == NET_XML_TAG_conceptEntityNodeContainer)
		{
			long currentEntityNodeIDInConceptEntityNodesList = 0;
				
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
									
					if(linkConnections)
					{
						//cout << "currentEntityNodeIDInCompleteList = " << currentEntityNodeIDInCompleteList << endl;
						
						GIAEntityNode * currentEntity = entityNodesCompleteList->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesCompleteList))
						{
							result = false;
						}
						//cout << "as2" << endl;
					}
					else
					{						
						GIAEntityNode * newEntity = new GIAEntityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->id = attributeValue;
						#endif
						
						entityNodesCompleteList->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						
						bool foundTemp = false;
						long indexTemp = 0;
						
						/*
						string conceptEntityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value;
						GIAEntityNode * tempNode = findOrAddEntityNodeByName(entityNodesCompleteList, conceptEntityNodesList, conceptEntityNamesList, &conceptEntityName, &foundTemp, &indexTemp, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInConceptEntityNodesList);
						*/
						
						
						conceptEntityNodesList->push_back(newEntity);
						currentEntityNodeIDInConceptEntityNodesList++;
						
						#ifdef GIA_ENABLE_XML_READ_DEBUG
						if(currentTagUpdatedL3->firstAttribute != NULL)
						{
							if(currentTagUpdatedL3->firstAttribute->nextAttribute->name != NET_XML_ATTRIBUTE_id)
							{
								if(currentTagUpdatedL3->firstAttribute->nextAttribute != NULL)
								{
									if(currentTagUpdatedL3->firstAttribute->nextAttribute->name != NET_XML_ATTRIBUTE_name)
									{

									}
									else
									{
										cout << "error: concept entityTag does not have name as second attribute" << endl;
										exit(0);								
									}
								}
								else
								{
									cout << "error: concept entityTag does not have name" << endl;
									exit(0);
								}								
							}
							else
							{
								cout << "error: concept entityTag does not have id as first attribute" << endl;
								exit(0);								
							}	
						}
						else
						{
							cout << "error: concept entityTag does not have ID" << endl;
							exit(0);
						}
						#endif
						cout << "Concept Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;	
						conceptEntityNamesList->push_back(currentTagUpdatedL3->firstAttribute->nextAttribute->value);		
										
										
						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next; 
						*/
					}
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
			cout << "parseSemanticNetTag error: no concept entity container node tag detected";
		}
		
		if(currentTagUpdatedL2->name == NET_XML_TAG_propertyEntityNodeContainer)
		{
			long currentEntityNodeIDInPropertyEntityNodesList = 0;
			
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
									
					if(linkConnections)
					{
						//cout << "currentEntityNodeIDInCompleteList = " << currentEntityNodeIDInCompleteList << endl;
						
						GIAEntityNode * currentEntity = entityNodesCompleteList->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesCompleteList))
						{
							result = false;
						}
						//cout << "as2" << endl;
					}
					else
					{						
						GIAEntityNode * newEntity = new GIAEntityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->id = attributeValue;
						#endif
						
						entityNodesCompleteList->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						propertyEntityNodesList->push_back(newEntity);
						currentEntityNodeIDInPropertyEntityNodesList++;	
												
						cout << "Property Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;	
											
						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next; 
						*/
					}
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
			cout << "parseSemanticNetTag error: no property entity container node tag detected";
		}
		
		if(currentTagUpdatedL2->name == NET_XML_TAG_actionEntityNodeContainer)
		{
			long currentEntityNodeIDInActionEntityNodesList = 0;	
			
			XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
									
					if(linkConnections)
					{
						//cout << "currentEntityNodeIDInCompleteList = " << currentEntityNodeIDInCompleteList << endl;
						
						GIAEntityNode * currentEntity = entityNodesCompleteList->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesCompleteList))
						{
							result = false;
						}
						//cout << "as2" << endl;
					}
					else
					{						
						GIAEntityNode * newEntity = new GIAEntityNode();
						
						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->id = attributeValue;
						#endif
						
						entityNodesCompleteList->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						actionEntityNodesList->push_back(newEntity);
						currentEntityNodeIDInActionEntityNodesList++;	
						
						cout << "Action Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;	
											
						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next; 
						*/
					}
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
			cout << "parseSemanticNetTag error: no action entity container node tag detected";
		}				
	}
	

	return result;
}



bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = firstTagInEntityNode->firstAttribute;

	bool idFound = false;
	bool entityNameFound = false;
	bool confidenceFound = false;
	bool isPropertyFound = false;
	bool isActionFound = false;
	
	bool actionSubjectEntityFound = false;
	bool actionObjectEntityFound = false;
	bool conditionTypeFound = false;
	
	bool hasAssociatedPropertyFound = false;
	bool hasAssociatedPropertyIsActionFound = false;
	bool hasAssociatedTimeFound = false;
	
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
	
		
	bool ActionNodeListFound = false;
	bool IncomingActionNodeListFound = false;
	
	bool PropertyNodeListFound = false;
	bool entityNodeContainingThisPropertyFound = false;
	bool entityNodeDefiningThisPropertyFound = false;
	
	bool EntityNodeDefinitionListFound = false;
	bool EntityNodeDefinitionReverseListFound = false;
	bool AssociatedPropertyNodeListFound = false;
	
	bool ConditionNodeListFound = false;
	bool ConditionNodeReverseListFound = false;
	
	bool timeConditionNodeFound = false;
	
	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	//parseEntityNodeTag->... = 0;

	while(currentAttribute->nextAttribute != NULL)
	{
		//cout << "df1" << endl;
		
		if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
		{			
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->id = attributeValue;
			idFound = true;
			//cout << "entityNode->id = " << entityNode->id << endl;
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
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isProperty = attributeValue;
			isPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isAction)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isAction = attributeValue;
			isActionFound = true;
		}		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedProperty)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedProperty = attributeValue;
			hasAssociatedPropertyFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedPropertyIsAction)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedPropertyIsAction = attributeValue;
			hasAssociatedPropertyIsActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedTime)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedTime = attributeValue;
			hasAssociatedTimeFound = true;
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_actionSubjectEntity)
		{
			if(currentAttribute->value != GIA_SEMANTIC_NET_XML_NULL_NODE_ID)
			{
				long attributeValue = atol(currentAttribute->value.c_str());
				entityNode->actionSubjectEntity = findEntityNodeByID(attributeValue, entityNodesCompleteList);
				actionSubjectEntityFound = true;
			}
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_actionObjectEntity)
		{
			if(currentAttribute->value != GIA_SEMANTIC_NET_XML_NULL_NODE_ID)
			{
				long attributeValue = atol(currentAttribute->value.c_str());
				entityNode->actionObjectEntity = findEntityNodeByID(attributeValue, entityNodesCompleteList);
				actionObjectEntityFound = true;
			}
		}
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->conditionType = attributeValue;
			conditionTypeFound = true;
		}
				
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityNodeContainingThisProperty)
		{
			//cout << "df1a" << endl;
			if(currentAttribute->value != GIA_SEMANTIC_NET_XML_NULL_NODE_ID)
			{
				long attributeValue = atol(currentAttribute->value.c_str());
				//cout << "df1a2 attributeValue = " << attributeValue << endl;
			
				entityNode->entityNodeContainingThisProperty = findEntityNodeByID(attributeValue, entityNodesCompleteList);
				entityNodeContainingThisPropertyFound = true;
			}
			//cout << "df1b" << endl;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityNodeDefiningThisProperty)
		{
			
			if(currentAttribute->value != GIA_SEMANTIC_NET_XML_NULL_NODE_ID)
			{		
				long attributeValue = atol(currentAttribute->value.c_str());	
				entityNode->entityNodeDefiningThisPropertyOrAction = findEntityNodeByID(attributeValue, entityNodesCompleteList);
				entityNodeDefiningThisPropertyFound = true;
			}
		}	
		
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalNumber)
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
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printYIndex)
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
		
		//cout << "df3" << endl;
		
		currentAttribute = currentAttribute->nextAttribute;
	}
	
	//cout << "df4" << endl;

	XMLParserTag * currentTagUpdatedL3 = firstTagInEntityNode->firstLowerLevelTag;
	while(currentTagUpdatedL3->nextTag != NULL)
	{
		if(currentTagUpdatedL3->name == NET_XML_TAG_ActionNodeList)
		{
			//cout << "ActionNodeList: " << endl;
			if(!parseActionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}
			ActionNodeListFound = true;					
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_IncomingActionNodeList)
		{
			//cout << "IncomingActionNodeList: " << endl;
			if(!parseIncomingActionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}	
			IncomingActionNodeListFound = true;				
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_PropertyNodeList)
		{
			//cout << "PropertyNodeList: " << endl;
			if(!parsePropertyNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}				
			PropertyNodeListFound = true;	
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_EntityNodeDefinitionList)
		{
			//cout << "EntityNodeDefinitionList: " << endl;
			if(!parseEntityNodeDefinitionListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}				
			EntityNodeDefinitionListFound = true;	
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_EntityNodeDefinitionReverseList)
		{
			//cout << "EntityNodeDefinitionReverseList: " << endl;
			if(!parseEntityNodeDefinitionReverseListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}				
			EntityNodeDefinitionReverseListFound = true;	
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_AssociatedPropertyNodeList)
		{
			//cout << "AssociatedPropertyNodeList: " << endl;
			if(!parseAssociatedPropertyNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}
			AssociatedPropertyNodeListFound = true;						
		}
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeList)
		{
			//cout << "ConditionNodeList: " << endl;
			if(!parseConditionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}	
			ConditionNodeListFound = true;				
		}
		if(currentTagUpdatedL3->name == NET_XML_TAG_ConditionNodeReverseList)
		{
			//cout << "ConditionNodeReverseList: " << endl;
			if(!parseConditionNodeReverseListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesCompleteList))
			{
				result = false;
			}
			ConditionNodeReverseListFound = true;					
		}																				
		
		if(currentTagUpdatedL3->name == NET_XML_TAG_timeConditionNode)
		{
			cout << "timeConditionNode: " << endl;
			entityNode->timeConditionNode = new GIATimeConditionNode();
			if(!parseTimeConditionNodeTag(currentTagUpdatedL3, entityNode->timeConditionNode))
			{
				result = false;
			}	
			cout << "END timeConditionNode: " << endl;
			timeConditionNodeFound = true;				
		}
					
		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}
	
	//cout << "df5" << endl;			
				
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

	/*
	if(printXFound && printYFound && printXIndexFound && printYIndexFound && printTextXFound && printTextYFound)
	{
		entityNode->initialisedForPrinting = true; 
	}
	*/

	return result;
}



bool parseActionNodeListTag(XMLParserTag * firstTagInActionNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInActionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_actionNodeReference)
		{
			//cout << "actionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->ActionNodeList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
			//cout << "attributeValue = " << attributeValue << endl;
			//cout << "refID = " << findEntityNodeByID(attributeValue, entityNodesCompleteList)->id << endl;
		}
		else
		{
			cout << "parseActionNodeListTag error: actionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}	
}

bool parseIncomingActionNodeListTag(XMLParserTag * firstTagInIncomingActionNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInIncomingActionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_actionNodeReference)
		{
			//cout << "actionNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->IncomingActionNodeList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
		}
		else
		{
			cout << "parseIncomingActionNodeListTag error: actionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parsePropertyNodeListTag(XMLParserTag * firstTagInPropertyNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInPropertyNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			//cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->PropertyNodeList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
		}
		else
		{
			cout << "parsePropertyNodeListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseEntityNodeDefinitionListTag(XMLParserTag * firstTagInEntityNodeDefinitionList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInEntityNodeDefinitionList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			//cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->EntityNodeDefinitionList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
		}
		else
		{
			cout << "parseEntityNodeDefinitionListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseEntityNodeDefinitionReverseListTag(XMLParserTag * firstTagInEntityNodeDefinitionReverseList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInEntityNodeDefinitionReverseList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			//cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->EntityNodeDefinitionReverseList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
		}
		else
		{
			cout << "parseEntityNodeDefinitionReverseListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseAssociatedPropertyNodeListTag(XMLParserTag * firstTagInAssociatedPropertyNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInAssociatedPropertyNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			//cout << "entityNodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->AssociatedPropertyNodeList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
		}
		else
		{
			cout << "parseAssociatedPropertyNodeListTag error: entityNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseConditionNodeListTag(XMLParserTag * firstTagInConditionNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			bool idFound = false;
			bool conditionTypeStringFound = false;

			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			while(currentAttribute->nextAttribute != NULL)
			{
				//cout << "df1" << endl;
				if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
				{
					long attributeValue = atol(currentAttribute->value.c_str());
					entityNode->id = attributeValue;
					idFound = true;
					entityNode->ConditionNodeList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
				}			
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionTypeString)
				{
					string attributeValue = currentAttribute->value.c_str();
					entityNode->entityName = attributeValue;
					conditionTypeStringFound = true;
					entityNode->ConditionNodeTypeList.push_back(attributeValue);
				}
				currentAttribute = currentAttribute->nextAttribute;
			}
			
			if(!idFound || !conditionTypeStringFound)
			{
				cout << "error: parseConditionNodeListTag(); (!idFound || !conditionTypeStringFound)" << endl;
			}

		}
		else
		{
			cout << "parseConditionNodeListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}

bool parseConditionNodeReverseListTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList)
{
	XMLParserTag * currentTagUpdatedL1 = firstTagInConditionNodeReverseList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_entityNodeReference)
		{
			bool idFound = false;
			bool conditionTypeStringFound = false;

			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			while(currentAttribute->nextAttribute != NULL)
			{
				//cout << "df1" << endl;
				if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
				{
					long attributeValue = atol(currentAttribute->value.c_str());
					entityNode->id = attributeValue;
					idFound = true;
					entityNode->ConditionNodeReverseList.push_back(findEntityNodeByID(attributeValue, entityNodesCompleteList));
				}			
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionTypeString)
				{
					string attributeValue = currentAttribute->value.c_str();
					entityNode->entityName = attributeValue;
					conditionTypeStringFound = true;
					entityNode->ConditionNodeTypeReverseList.push_back(attributeValue);
				}
				currentAttribute = currentAttribute->nextAttribute;
			}
			
			if(!idFound || !conditionTypeStringFound)
			{
				cout << "error: parseConditionNodeReverseListTag(); (!idFound || !conditionTypeStringFound)" << endl;
			}
		}
		else
		{
			cout << "parseConditionNodeReverseListTag error: conditionNodeReference tag not found" << endl;
		}	
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
}


bool parseTimeConditionNodeTag(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = firstTagInTimeConditionNode->firstAttribute;

	bool tenseFound = false;
	bool secondFound = false;
	bool hourFound = false;
	bool dayOfWeekFound = false;
	bool monthFound = false;
	bool dayOfMonthFound = false;
	bool yearFound = false;
	bool periodFound = false;
	bool totalTimeInSecondsFound = false;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_tense)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->tense = attributeValue;
			tenseFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_second)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			timeConditionNode->second = attributeValue;
			secondFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hour)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->hour = attributeValue;
			hourFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfWeek)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->dayOfWeek = attributeValue;
			dayOfWeekFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_month)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->month = attributeValue;
			monthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfMonth)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->dayOfMonth = attributeValue;
			dayOfMonthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_year)
		{
			long attributeValue = atol(currentAttribute->value.c_str());
			timeConditionNode->year = attributeValue;
			yearFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_period)
		{
			double attributeValue = atoi(currentAttribute->value.c_str());
			timeConditionNode->period = attributeValue;
			periodFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_totalTimeInSeconds)
		{
			long attributeValue = atol(currentAttribute->value.c_str());
			timeConditionNode->totalTimeInSeconds = attributeValue;
			totalTimeInSecondsFound = true;
		}
	
		currentAttribute = currentAttribute->nextAttribute;
	}		
}




bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<string> * conceptEntityNamesList)
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

	long currentEntityNodeIDInConceptEntityNodesList = 0;
	
	if(!generateXMLEntityNodeTagList(currentTagL1, conceptEntityNodesList, NET_XML_TAG_conceptEntityNodeContainer, &currentEntityNodeIDInConceptEntityNodesList))
	{
		result = false;
	}
	
	//cout << "a" << endl;
	
	currentTagL1 = currentTagL1->nextTag;
	XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag2;
	
	if(!generateXMLEntityNodeTagList(currentTagL1, propertyEntityNodesList, NET_XML_TAG_propertyEntityNodeContainer, &currentEntityNodeIDInConceptEntityNodesList))
	{
		result = false;
	}

	//cout << "b" << endl;
	
	currentTagL1 = currentTagL1->nextTag;
	XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag3;
	
	if(!generateXMLEntityNodeTagList(currentTagL1, actionEntityNodesList, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDInConceptEntityNodesList))
	{
		result = false;
	}
	
	//cout << "c" << endl;
	

	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;


	return result;
}




bool generateXMLEntityNodeTagList(XMLParserTag * firstTagInSemanticNet, vector<GIAEntityNode*> *entityNodesList, string entityContainerTagName, long * currentEntityNodeIDInConceptEntityNodesList)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagL0 = firstTagInSemanticNet;
	currentTagL0->name = entityContainerTagName;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;
		
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;

		currentTagL1 = generateXMLEntityNodeTag(currentTagL1, currentEntity, *currentEntityNodeIDInConceptEntityNodesList);
		
		(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
	
	return result;
}



XMLParserTag * generateXMLEntityNodeTag(XMLParserTag * currentTagL1, GIAEntityNode * currentEntity, long currentEntityNodeIDInConceptEntityNodesList)
{	
	bool result = true;
	
	//cout << "h5" << endl;

	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	//generate neuron connection tag
	currentTagL1->name = NET_XML_TAG_entityNode;
	XMLParserTag * firstTagL2 = new XMLParserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLParserAttribute * currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_id;

	//do not write current id value, write new id value - this enables fast linking of parsed xml file			
	//sprintf(tempString, "%ld", (currentEntity->id));
	sprintf(tempString, "%ld", currentEntityNodeIDInConceptEntityNodesList);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute1;
	currentAttribute = currentAttribute->nextAttribute;


	currentAttribute->name = NET_XML_ATTRIBUTE_entityName;
	currentAttribute->value = currentEntity->entityName;

	XMLParserAttribute * newAttribute15 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute15;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_confidence;
	sprintf(tempString, "%0.6f", (currentEntity->confidence));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute16 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute16;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_isProperty;
	sprintf(tempString, "%d", int(currentEntity->isProperty));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute17 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute17;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_isAction;
	sprintf(tempString, "%d", int(currentEntity->isAction));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute17b = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute17b;
	currentAttribute = currentAttribute->nextAttribute;		

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedProperty;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedProperty));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute18 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute18;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedPropertyIsAction;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedPropertyIsAction));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute19 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute19;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedTime;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedTime));
	currentAttribute->value = tempString;

	//cout << "h5c2" << endl;

	XMLParserAttribute * newAttribute20 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute20;
	currentAttribute = currentAttribute->nextAttribute;

	if(currentEntity->actionSubjectEntity != NULL)
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_actionSubjectEntity;
		sprintf(tempString, "%ld", (currentEntity->actionSubjectEntity->id));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute22 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute22;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#ifndef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	else
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_actionSubjectEntity;
		currentAttribute->value = GIA_SEMANTIC_NET_XML_NULL_NODE_ID;

		XMLParserAttribute * newAttribute22 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute22;
		currentAttribute = currentAttribute->nextAttribute;	
	}
	#endif		

	if(currentEntity->actionObjectEntity != NULL)
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_actionObjectEntity;
		sprintf(tempString, "%ld", (currentEntity->actionObjectEntity->id));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute23 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute23;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#ifndef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	else
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_actionObjectEntity;
		currentAttribute->value = GIA_SEMANTIC_NET_XML_NULL_NODE_ID;

		XMLParserAttribute * newAttribute23 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute23;
		currentAttribute = currentAttribute->nextAttribute;				
	}
	#endif		


	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType;
	sprintf(tempString, "%d", currentEntity->conditionType);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute22 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute22;
	currentAttribute = currentAttribute->nextAttribute;


	if(currentEntity->entityNodeContainingThisProperty != NULL)
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_entityNodeContainingThisProperty;
		sprintf(tempString, "%ld", (currentEntity->entityNodeContainingThisProperty->id));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute21 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute21;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#ifndef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	else
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_entityNodeContainingThisProperty;
		currentAttribute->value = GIA_SEMANTIC_NET_XML_NULL_NODE_ID;

		XMLParserAttribute * newAttribute21 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute21;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#endif


	if(currentEntity->entityNodeDefiningThisPropertyOrAction != NULL)
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_entityNodeDefiningThisProperty;
		sprintf(tempString, "%ld", (currentEntity->entityNodeDefiningThisPropertyOrAction->id));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute22 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute22;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#ifndef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	else
	{
		currentAttribute->name = NET_XML_ATTRIBUTE_entityNodeDefiningThisProperty;
		currentAttribute->value = GIA_SEMANTIC_NET_XML_NULL_NODE_ID;

		XMLParserAttribute * newAttribute22 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute22;
		currentAttribute = currentAttribute->nextAttribute;			
	}
	#endif

	//cout << "h5a" << endl;

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalNumber;
	sprintf(tempString, "%d", (currentEntity->grammaticalNumber));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute2;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantity;
	sprintf(tempString, "%d", int(currentEntity->hasQuantity));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute3 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute3;
	currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasQuantity)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumber;
		sprintf(tempString, "%d", (currentEntity->quantityNumber));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute4 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute4;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifier;
		sprintf(tempString, "%d", (currentEntity->quantityModifier));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute5 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute5;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifierString;
		currentAttribute->value = currentEntity->quantityModifierString;

		XMLParserAttribute * newAttribute6 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute6;
		currentAttribute = currentAttribute->nextAttribute;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_hasMeasure;
	sprintf(tempString, "%d", int(currentEntity->hasMeasure));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute7 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute7;
	currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasMeasure)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_measureType;
		sprintf(tempString, "%d", (currentEntity->measureType));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute8 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute8;
		currentAttribute = currentAttribute->nextAttribute;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES	
	}
	#endif
	//cout << "h5b" << endl;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->initialisedForPrinting)
	{
	#endif

		currentAttribute->name = NET_XML_ATTRIBUTE_printX;
		sprintf(tempString, "%d", (currentEntity->printX));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute9 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute9;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printY;
		sprintf(tempString, "%d", (currentEntity->printY));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute10 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute10;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printXIndex;
		sprintf(tempString, "%d", (currentEntity->printXIndex));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute11 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute11;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printYIndex;
		sprintf(tempString, "%d", (currentEntity->printYIndex));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute12 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute12;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextX;
		sprintf(tempString, "%d", (currentEntity->printTextX));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute13 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute13;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextY;
		sprintf(tempString, "%d", (currentEntity->printTextY));
		currentAttribute->value = tempString;

		XMLParserAttribute * newAttribute14 = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute14;
		currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	//cout << "h5c" << endl;




	//cout << "h5d" << endl;

	XMLParserTag * firstTagL3;
	XMLParserTag * currentTagL3;
	XMLParserTag * newTag2;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->ActionNodeList.begin() != currentEntity->ActionNodeList.end())
	{
	#endif
		currentTagL2->name = NET_XML_TAG_ActionNodeList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;

		for(currentEntity->ActionNodeListIterator = currentEntity->ActionNodeList.begin(); currentEntity->ActionNodeListIterator < currentEntity->ActionNodeList.end(); currentEntity->ActionNodeListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_actionNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->ActionNodeListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;		
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->IncomingActionNodeList.begin() != currentEntity->IncomingActionNodeList.end())
	{
	#endif
		currentTagL2->name = NET_XML_TAG_IncomingActionNodeList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		for(currentEntity->IncomingActionNodeListIterator = currentEntity->IncomingActionNodeList.begin(); currentEntity->IncomingActionNodeListIterator < currentEntity->IncomingActionNodeList.end(); currentEntity->IncomingActionNodeListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_actionNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->IncomingActionNodeListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;	
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif		

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->PropertyNodeList.begin() != currentEntity->PropertyNodeList.end())
	{
	#endif		
		currentTagL2->name = NET_XML_TAG_PropertyNodeList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		for(currentEntity->PropertyNodeListIterator = currentEntity->PropertyNodeList.begin(); currentEntity->PropertyNodeListIterator < currentEntity->PropertyNodeList.end(); currentEntity->PropertyNodeListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_entityNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->PropertyNodeListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;		
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif		

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->EntityNodeDefinitionList.begin() != currentEntity->EntityNodeDefinitionList.end())
	{
	#endif			
		currentTagL2->name = NET_XML_TAG_EntityNodeDefinitionList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		for(currentEntity->EntityNodeDefinitionListIterator = currentEntity->EntityNodeDefinitionList.begin(); currentEntity->EntityNodeDefinitionListIterator < currentEntity->EntityNodeDefinitionList.end(); currentEntity->EntityNodeDefinitionListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_entityNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->EntityNodeDefinitionListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif			

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->EntityNodeDefinitionReverseList.begin() != currentEntity->EntityNodeDefinitionReverseList.end())
	{
	#endif		
		currentTagL2->name = NET_XML_TAG_EntityNodeDefinitionReverseList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		for(currentEntity->EntityNodeDefinitionReverseListIterator = currentEntity->EntityNodeDefinitionReverseList.begin(); currentEntity->EntityNodeDefinitionReverseListIterator < currentEntity->EntityNodeDefinitionReverseList.end(); currentEntity->EntityNodeDefinitionReverseListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_entityNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->EntityNodeDefinitionReverseListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif											

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->AssociatedPropertyNodeList.begin() != currentEntity->AssociatedPropertyNodeList.end())
	{
	#endif
		currentTagL2->name = NET_XML_TAG_AssociatedPropertyNodeList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		for(currentEntity->AssociatedPropertyNodeListIterator = currentEntity->AssociatedPropertyNodeList.begin(); currentEntity->AssociatedPropertyNodeListIterator < currentEntity->AssociatedPropertyNodeList.end(); currentEntity->AssociatedPropertyNodeListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_entityNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->AssociatedPropertyNodeListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif		

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->ConditionNodeList.begin() != currentEntity->ConditionNodeList.end())
	{
	#endif
		currentTagL2->name = NET_XML_TAG_ConditionNodeList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		currentEntity->ConditionNodeTypeListIterator = currentEntity->ConditionNodeTypeList.begin();
		for(currentEntity->ConditionNodeListIterator = currentEntity->ConditionNodeList.begin(); currentEntity->ConditionNodeListIterator < currentEntity->ConditionNodeList.end(); currentEntity->ConditionNodeListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_conditionNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->ConditionNodeListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_conditionTypeString;
			currentAttribute->value = *(currentEntity->ConditionNodeTypeListIterator);

			XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute2;
			currentAttribute = currentAttribute->nextAttribute;				

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

			currentEntity->ConditionNodeTypeListIterator++;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	}
	#endif		

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
	if(currentEntity->ConditionNodeReverseList.begin() != currentEntity->ConditionNodeReverseList.end())
	{
	#endif
		currentTagL2->name = NET_XML_TAG_ConditionNodeReverseList;
		firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentTagL3 = currentTagL2->firstLowerLevelTag;
		currentEntity->ConditionNodeTypeReverseListIterator = currentEntity->ConditionNodeTypeReverseList.begin();			
		for(currentEntity->ConditionNodeReverseListIterator = currentEntity->ConditionNodeReverseList.begin(); currentEntity->ConditionNodeReverseListIterator < currentEntity->ConditionNodeReverseList.end(); currentEntity->ConditionNodeReverseListIterator++)
		{	
			currentTagL3->name = NET_XML_TAG_conditionNodeReference;

			currentAttribute = currentTagL3->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%ld", (*(currentEntity->ConditionNodeReverseListIterator))->id);
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_conditionTypeString;
			currentAttribute->value = *(currentEntity->ConditionNodeTypeReverseListIterator);

			XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute2;
			currentAttribute = currentAttribute->nextAttribute;					

			XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
			currentTagL3->nextTag = newTag3;			
			currentTagL3 = currentTagL3->nextTag;

			currentEntity->ConditionNodeTypeReverseListIterator++;

		} 
		newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	}				
	//cout << "h5e" << endl;

	if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		currentTagL2->name = NET_XML_TAG_timeConditionNode;

		/*
		XMLParserTag * firstTagL3 = new XMLParserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		XMLParserTag * currentTagL3 = currentTagL2->firstLowerLevelTag;
		*/

		if(!generateXMLConditionTimeNodeTagList(currentTagL2, currentEntity->timeConditionNode))
		{
			result = false;
		}

		XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;		
	}

	XMLParserTag * newTag = new XMLParserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}








bool generateXMLConditionTimeNodeTagList(XMLParserTag * firstTagInConditionTimeNode, GIATimeConditionNode * conditionTimeNode)
{
	//cout << "h3" << endl;

	bool result = true;
	
	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	
	XMLParserAttribute * currentAttribute = firstTagInConditionTimeNode->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_tense;
	sprintf(tempString, "%d", (conditionTimeNode->tense));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute1;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_second;
	sprintf(tempString, "%0.6f", (conditionTimeNode->tense));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute2;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hour;
	sprintf(tempString, "%d", conditionTimeNode->hour);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute3 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute3;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfWeek;
	sprintf(tempString, "%d", conditionTimeNode->dayOfWeek);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute4 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute4;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_month;
	sprintf(tempString, "%d", conditionTimeNode->month);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute5 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute5;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfMonth;
	sprintf(tempString, "%d", conditionTimeNode->dayOfMonth);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute6 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute6;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_year;
	sprintf(tempString, "%ld", conditionTimeNode->year);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute7 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute7;
	currentAttribute = currentAttribute->nextAttribute;			

	currentAttribute->name = NET_XML_ATTRIBUTE_period;
	sprintf(tempString, "%0.6f", conditionTimeNode->period);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute8 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute8;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_totalTimeInSeconds;
	sprintf(tempString, "%ld", conditionTimeNode->totalTimeInSeconds);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute9 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute9;
	currentAttribute = currentAttribute->nextAttribute;
		
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

