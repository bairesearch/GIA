/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAcxlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3p6a 07-August-2021
 * Description: CXL Conversion - Converts GIA network nodes into a CXL file
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 * /
 *******************************************************************************/


#include "GIAcxlConversion.hpp"

bool GIAcxlConversionClass::writeCmapToolsCXLFileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string,GIAentityNode*>* networkIndexEntityNodesListMap)
{
	vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes = new vector<GIAentityNode*>;
	for(unordered_map<string,GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = networkIndexEntityNodesListMap->begin(); networkIndexEntityNodesListMapIter != networkIndexEntityNodesListMap->end(); networkIndexEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
		entityNodesActiveListNetworkIndexes->push_back(entityNode);
	}

	bool result;
	result = writeCmapToolsCXLfile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes);

	delete entityNodesActiveListNetworkIndexes;

	return result;
}

bool GIAcxlConversionClass::writeCmapToolsCXLfile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

	XMLparserTag* currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_CXL_TAG_cmap;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserAttribute* currentAttribute = currentTagL0->firstAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlns;
		currentAttribute->value = NET_CXL_ATTRIBUTE_xmlns_DEFAULT_VALUE;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlnsdc;
		currentAttribute->value = NET_CXL_ATTRIBUTE_xmlnsdc_DEFAULT_VALUE;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = NET_CXL_TAG_map;
	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;
	XMLparserTag* newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	int64_t currentCmapNodeIDinCmapNodeList;

	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
	currentCmapNodeIDinCmapNodeList = 0;
	for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
	{
		resetIDsForNodeList2(entityNodesActiveListComplete, &currentCmapNodeIDinCmapNodeList, entityType);
	}
	#endif

	currentCmapNodeIDinCmapNodeList = 0;
	int64_t firstCmapNodeIDInNetworkIndexList = currentCmapNodeIDinCmapNodeList;
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, &currentCmapNodeIDinCmapNodeList, true, false))
	{
		result = false;
	}
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	int64_t firstCmapNodeIDInLinkingPhraseList = currentCmapNodeIDinCmapNodeList;	//NOT NECESSARY: redundant
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, &currentCmapNodeIDinCmapNodeList, false, false))
	{
		result = false;
	}
	XMLparserTag* firstTagInLinkingPhraseList = currentTagL2->firstLowerLevelTag;
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	currentCmapNodeIDinCmapNodeList = firstCmapNodeIDInNetworkIndexList;
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, &currentCmapNodeIDinCmapNodeList, true, true))
	{
		result = false;
	}
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	currentCmapNodeIDinCmapNodeList = firstCmapNodeIDInLinkingPhraseList;	//NOT NECESSARY: redundant

	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, &currentCmapNodeIDinCmapNodeList, false, true))
	{
		result = false;
	}
	XMLparserTag* firstTagInLinkingPhraseAppearanceList = currentTagL2->firstLowerLevelTag;
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;


	XMLparserTag* currentTagInLinkingPhraseList = firstTagInLinkingPhraseList;
	XMLparserTag* currentTagInLinkingPhraseAppearanceList = firstTagInLinkingPhraseAppearanceList;
	while(currentTagInLinkingPhraseList->nextTag != NULL)
	{
		currentTagInLinkingPhraseList = currentTagInLinkingPhraseList->nextTag;
	}
	while(currentTagInLinkingPhraseAppearanceList->nextTag != NULL)
	{
		currentTagInLinkingPhraseAppearanceList = currentTagInLinkingPhraseAppearanceList->nextTag;
	}
	XMLparserTag* lastTagInLinkingPhraseList = currentTagInLinkingPhraseList;
	XMLparserTag* lastTagInLinkingPhraseAppearanceList = currentTagInLinkingPhraseAppearanceList;

	if(!generateCXLconnectionNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, &currentCmapNodeIDinCmapNodeList, lastTagInLinkingPhraseList, lastTagInLinkingPhraseAppearanceList))
	{
		result = false;
	}


 	if(!XMLparserClass.writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;


	return result;
}

//generateCXLentityNodeTagList{}: generates and fills <networkIndex-list>, <networkIndex-appearance-list>, <linking-phrase-list> and <linking-phrase-appearance-list> tags.
bool GIAcxlConversionClass::generateCXLentityNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, int64_t* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

	XMLparserTag* currentTagL0 = firstTagInMap;
	if(networkIndexOrLinkingPhraseList)
	{
		if(!appearanceList)
		{
			currentTagL0->name = NET_CXL_TAG_networkIndexList;
		}
		else
		{
			currentTagL0->name = NET_CXL_TAG_networkIndexAppearanceList;
		}
	}
	else
	{
		if(!appearanceList)
		{
			currentTagL0->name = NET_CXL_TAG_linkingPhraseList;
		}
		else
		{
			currentTagL0->name = NET_CXL_TAG_linkingPhraseAppearanceList;
		}
	}
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	#ifdef GIA_CMAP_CONVERSION_SANITISED
	if(networkIndexOrLinkingPhraseList)
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_SUBSTANCE);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_CONCEPT);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_NETWORK_INDEX);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_QUALITY);
	}
	else
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_ACTION);	//required for timeCondition node printing
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, GIA_ENTITY_TYPE_CONDITION);	//required for timeCondition node printing
	}
	#else
	for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, networkIndexOrLinkingPhraseList, appearanceList, entityType);
	}
	#endif

	return result;
}


XMLparserTag* GIAcxlConversionClass::addToCXLentityNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, int64_t* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList, const int entityType)
{
	bool result = true;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;
		if(currentEntity->entityType == entityType)
		{
			#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{//this exception is required because timeCondition nodes have a separate idActiveList in Cmap maps / CXL
				currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave = (*currentCmapNodeIDinCmapNodeList);
				currentTagL1 = generateCXLentityNodeTag(currentTagL1, currentEntity->timeConditionNode->conditionName,* currentCmapNodeIDinCmapNodeList, currentEntity->timeConditionNode->printX, currentEntity->timeConditionNode->printY, networkIndexOrLinkingPhraseList, appearanceList);
				(*currentCmapNodeIDinCmapNodeList) = (*currentCmapNodeIDinCmapNodeList) + 1;
			}
			#endif

			if(!(currentEntity->disabled))
			{
				#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
				if(currentEntity->instanceNodeList->empty())
				{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES; then do not add a networkIndex entity if it has an associated instance (substance node)
				#endif

					#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
					currentEntity->idActiveListReorderdIDforXMLsave = *currentCmapNodeIDinCmapNodeList;
					int64_t currentEntityID = currentEntity->idActiveListReorderdIDforXMLsave;
					#elif defined GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
					//idActiveListReorderdIDforXMLsave already set
					int64_t currentEntityID = currentEntity->idActiveListReorderdIDforXMLsave;
					#else
					int64_t currentEntityID = currentEntity->idActiveList;
					#endif

					currentTagL1 = generateCXLentityNodeTag(currentTagL1, currentEntity->entityName, currentEntityID, currentEntity->printX, currentEntity->printY, networkIndexOrLinkingPhraseList, appearanceList);
					(*currentCmapNodeIDinCmapNodeList) = (*currentCmapNodeIDinCmapNodeList) + 1;

				#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
				}
				#endif
			}
		}
	}

	return currentTagL1;
}

XMLparserTag* GIAcxlConversionClass::generateCXLentityNodeTag(XMLparserTag* currentTagL1, string entityName, int64_t entityID, int printX, int printY, const bool networkIndexOrLinkingPhraseList, const bool appearanceList)
{
	bool result = true;

	//generate neuron connection tag
	if(networkIndexOrLinkingPhraseList)
	{
		if(!appearanceList)
		{
			currentTagL1->name = NET_CXL_TAG_networkIndex;
		}
		else
		{
			currentTagL1->name = NET_CXL_TAG_networkIndexAppearance;
		}
	}
	else
	{
		if(!appearanceList)
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhrase;
		}
		else
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhraseAppearance;
		}
	}

	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_ATTRIBUTE_id;
	currentAttribute->value = SHAREDvars.convertLongToString(entityID);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);


	if(!appearanceList)
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_label;
		currentAttribute->value = entityName;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	}
	else
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_x;
		currentAttribute->value = SHAREDvars.convertIntToString(printX*GIA_CXL_EXPAND_RATIO);;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_CXL_ATTRIBUTE_y;
		currentAttribute->value = SHAREDvars.convertIntToString(printY*GIA_CXL_EXPAND_RATIO);;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	}

	XMLparserTag* newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}

//generateCXLconnectionNodeTagList{}: generates <connection-list> tag, and fills <linking-phrase-list>, and  <linking-phrase-appearance-list> tags
bool GIAcxlConversionClass::generateCXLconnectionNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, int64_t* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagInLinkingPhraseList, XMLparserTag* firstTagInLinkingPhraseAppearanceList)
{
	bool result = true;

	XMLparserTag* currentTagInLinkingPhraseList = firstTagInLinkingPhraseList;
	XMLparserTag* currentTagInLinkingPhraseAppearanceList = firstTagInLinkingPhraseAppearanceList;

	XMLparserTag* currentTagL0 = firstTagInMap;
	currentTagL0->name = NET_CXL_TAG_connectionList;

	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
	{
		currentTagL1 = addToCXLconnectionNodeTagList(currentTagL1, entityNodesActiveListComplete, currentCmapNodeIDinCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1, entityType);
	}

	return result;
}

#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
bool GIAcxlConversionClass::checkIfPassedRedundantNetworkIndexNodeRemoval(GIAentityNode* currentEntity)
{
	bool result = false;

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES_OLD
	if(currentEntity->instanceNodeList->empty())
	{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES; then do not add a networkIndex entity if it has an associated instance (substance node)
	//if((currentEntity->entityType == GIA_ENTITY_TYPE_ACTION) || (currentEntity->entityType == GIA_ENTITY_TYPE_SUBSTANCE) || (currentEntity->entityType == GIA_ENTITY_TYPE_CONCEPT) || (currentEntity->entityType == GIA_ENTITY_TYPE_CONDITION))
	//{//do not add raw networkIndex nodes
		result = true;
	}
	#else

	if(currentEntity->instanceNodeList->empty())
	{
		//entity does not have an associated instance [entity is either a raw/isolated networkIndex node, or it is a substance]
		result = true;
	}
	else
	{
		//entity has an associated instance [entity is a networkIndex node]

		if(!(currentEntity->relationshipSubjectEntity->empty()))
		{
			result = true;
		}
		if(!(currentEntity->relationshipObjectEntity->empty()))
		{
			result = true;
		}

		if(!(currentEntity->instanceReverseNodeList->empty()))
		{//this shouldnt be defined, since this entity is a networkIndex node, not a substance node
			result = true;
		}


		if(!(currentEntity->actionNodeList->empty()))
		{
			result = true;
		}
		if(!(currentEntity->actionReverseNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->conditionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->conditionReverseNodeList->empty()))
		{
			result = true;
		}


		if(!(currentEntity->propertyNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->propertyReverseNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->definitionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->definitionReverseNodeList->empty()))
		{
			result = true;
		}

		if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
		{
			result = true;
		}
	}

	#endif

	return result;
}
#endif

XMLparserTag* GIAcxlConversionClass::addToCXLconnectionNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, int64_t* currentCmapNodeIDinCmapNodeList, const XMLparserTag* firstTagL1, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList, const int entityType)
{
	bool result = true;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;
		if(currentEntity->entityType == entityType)
		{
			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
			if(checkIfPassedRedundantNetworkIndexNodeRemoval(currentEntity))
			{
			#endif

				for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
				{
					if(entityVectorConnectionCXLwriteArray[i])
					{
						#ifdef GIA_CMAP_CONVERSION_SANITISED
						//directly link action/condition subject/objects together (like in a standard/CMapTools networkIndex map)
						if(((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT))
						{
							if(entityIsRelationship(currentEntity))
							{
								vector<GIAentityConnectionClass*>* entitySubjectVectorConnection = currentEntity->relationshipSubjectEntity;
								vector<GIAentityConnectionClass*>* entityObjectVectorConnection = currentEntity->relationshipObjectEntity;

								bool subjectIsEmpty = false;
								bool objectIsEmpty = false;
								if(entitySubjectVectorConnection->empty())
								{
									subjectIsEmpty = true;
								}
								if(entityObjectVectorConnection->empty())
								{
									objectIsEmpty = true;
								}
								GIAentityConnectionClass* entitySubjectConnection = entitySubjectVectorConnection->begin();	//this could equally be back() as there is only 1 subject/object connection
								GIAentityConnectionClass* entityObjectConnection = entityObjectVectorConnection->begin();	//this could equally be back() as there is only 1 subject/object connection

								string connectionTypeName = currentEntity->entityName;
								int64_t linkingPhraseID = currentEntity->idActiveListReorderdIDforXMLsave;
								if(subjectIsEmpty)
								{
									entitySubjectConnection->entity = new GIAentityNode();
									entitySubjectConnection->entity->CXLdummyNode = true;
								}
								if(objectIsEmpty)
								{
									entityObjectConnection->entity = new GIAentityNode();
									entityObjectConnection->entity->CXLdummyNode = true;
								}
								if(!(subjectIsEmpty && objectIsEmpty))
								{
									currentTagL1 = generateCXLconnectionNodeTagAndLinkingPhraseTags(currentTagL1, entitySubjectEntity, entityObjectEntity, connectionTypeName, &linkingPhraseID, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, true);
								}
								if(subjectIsEmpty)
								{
									delete entitySubjectConnection->entity;
									delete entitySubjectConnection;
								}
								if(objectIsEmpty)
								{
									delete entityObjectConnection->entity;
									delete entityObjectConnection;
								}
							}
						}
						else
						{
						#endif
							if(!(currentEntity->entityVectorConnectionsArray[i].empty()))
							{
								for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[i].begin(); connectionIter < currentEntity->entityVectorConnectionsArray[i].end(); connectionIter++)
								{
									bool pass = true;
									#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES_OLD
									if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
									{
										pass = false;
									}
									#endif
									if(pass)
									{
										GIAentityNode* entity = (*connectionIter)->entity;
										string connectionTypeName = entityVectorConnectionDrawConnectionNameArray[i];
										currentTagL1 = generateCXLconnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, entity, connectionTypeName, currentCmapNodeIDinCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
									}
								}
							}
						#ifdef GIA_CMAP_CONVERSION_SANITISED
						}
						#endif
					}
				}


				#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
				if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
				{
					GIAentityNode fakeTimeEntity;
					fakeTimeEntity.entityName = currentEntity->timeConditionNode->conditionName;
					fakeTimeEntity.printX = currentEntity->timeConditionNode->printX;
					fakeTimeEntity.printY = currentEntity->timeConditionNode->printY;
					#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
					fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave;
					#elif defined GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
					fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveList;
					#else
					fakeTimeEntity.idActiveList = currentEntity->timeConditionNode->idActiveList;
					#endif

					string connectionTypeName = "time";
					currentTagL1 = generateCXLconnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, &fakeTimeEntity, connectionTypeName, currentCmapNodeIDinCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
				}
				#endif

			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
			}
			#endif
		}
	}

	return currentTagL1;
}

XMLparserTag* GIAcxlConversionClass::generateCXLconnectionNodeTagAndLinkingPhraseTags(XMLparserTag* currentTagL1, const GIAentityNode* entity1, const GIAentityNode* entity2, string connectionTypeName, int64_t* currentCmapNodeIDinCmapNodeList, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, const XMLparserTag* firstTagInConnectionsList, const bool alreadyMadeLinkingPhrase)
{
	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;
	#elif defined GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;
	#else
	int entity1ID = entity1->idActiveList;
	int entity2ID = entity2->idActiveList;
	#endif


	int entity1X =  entity1->printX;
	int entity2X =  entity2->printX;
	int entity1Y =  entity1->printY;
	int entity2Y =  entity2->printY;
	int connectionTypeX = (entity1X + entity2X) / 2;
	int connectionTypeY = (entity1Y + entity2Y) / 2;
	int connectionID = (*currentCmapNodeIDinCmapNodeList);


	//verify connection is not already found;
	bool alreadyMadeConnection = false;

	/*OLD:
	XMLparserTag* currentTagInList = firstTagInConnectionsList;
	while(currentTagInList->nextTag != NULL)
	{
		if(currentTagInList->nextTag->nextTag != NULL)
		{
			int currentFromIDinList[2];
			int currentToIDinList[2];
			XMLparserAttribute* firstAttributeInList[2];
			firstAttributeInList[0] = currentTagInList->firstAttribute;
			firstAttributeInList[1] = currentTagInList->nextTag->firstAttribute;
			for(int i=0; i<2; i++)
			{
				XMLparserAttribute* currentAttributeInList = firstAttributeInList[i];
				while(currentAttributeInList->nextAttribute != NULL)
				{
					if(currentAttributeInList->name == NET_CXL_TAG_fromid)
					{
						currentFromIDinList[i] = convertStringToInt(currentAttributeInList->value);
					}
					if(currentAttributeInList->name == NET_CXL_TAG_toid)
					{
						currentToIDinList[i] = convertStringToInt(currentAttributeInList->value);
					}
					currentAttributeInList = currentAttributeInList->nextAttribute;
				}
			}

			if(((currentFromIDinList[0] == entity1ID) && (currentToIDinList[1] == entity2ID))
			|| ((currentFromIDinList[1] == entity1ID) && (currentToIDinList[0] == entity2ID)))
			{
				alreadyMadeConnection = true;
			}
		}
		currentTagInList = currentTagInList->nextTag;
	}
	*/

	if(!alreadyMadeConnection)
	{
		if(!(entity1->CXLdummyNode))
		{
			currentTagL1 = generateCXLconnectionNodeTag(currentTagL1, entity1ID, connectionID);
		}
		if(!(entity2->CXLdummyNode))
		{
			currentTagL1 = generateCXLconnectionNodeTag(currentTagL1, connectionID, entity2ID);
		}
		if(!alreadyMadeLinkingPhrase)
		{
			*currentTagInLinkingPhraseList = generateCXLentityNodeTag(*currentTagInLinkingPhraseList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, false);
			*currentTagInLinkingPhraseAppearanceList = generateCXLentityNodeTag(*currentTagInLinkingPhraseAppearanceList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, true);
			(*currentCmapNodeIDinCmapNodeList) = (*currentCmapNodeIDinCmapNodeList) + 1;
		}
	}

	return currentTagL1;
}


XMLparserTag* GIAcxlConversionClass::generateCXLconnectionNodeTag(XMLparserTag* currentTagL1, int64_t fromID, int64_t toID)
{
	bool result = true;


	//generate neuron connection tag
	currentTagL1->name = NET_CXL_TAG_connection;

	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_TAG_fromid;
	currentAttribute->value = SHAREDvars.convertLongToString(fromID);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_CXL_TAG_toid;
	currentAttribute->value = SHAREDvars.convertLongToString(toID);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	XMLparserTag* newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}



#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
//this method is insufficient due to time condition nodes
void resetIDsForNodeList2(vector<GIAentityNode*>* entityNodesList, int64_t* currentEntityNodeIDinNetworkIndexEntityNodesList, int entityType)
{
	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;
		if(currentEntity->entityType == entityType)
		{
			currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDinNetworkIndexEntityNodesList;
			(*currentEntityNodeIDinNetworkIndexEntityNodesList) = (*currentEntityNodeIDinNetworkIndexEntityNodesList) + 1;
		}
	}
}
#endif



