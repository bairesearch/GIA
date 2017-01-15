/*******************************************************************************
 *
 * File Name: GIACXLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1g6c 12-Feb-2012
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function overwrites entity id values upon read/write to speed up linking process
 *
 *******************************************************************************/



#include "GIACXLconversion.h"

bool writeCMapToolsCXLFileOptimised(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList)
{	
	vector<GIAEntityNode*> * conceptEntityNodesList = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++) 
	{	
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		conceptEntityNodesList->push_back(entityNode);
	}	
	
	bool result;
	result = writeCMapToolsCXLFile(xmlFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList);
	return result;
}

bool writeCMapToolsCXLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

	XMLParserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_CXL_TAG_cmap;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserAttribute * currentAttribute = currentTagL0->firstAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlns;
		currentAttribute->value = "http://cmap.ihmc.us/xml/cmap/";

		XMLParserAttribute * newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlnsdc;
		currentAttribute->value = "http://purl.org/dc/elements/1.1/";

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;
				
	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = NET_CXL_TAG_map;
	XMLParserTag * firstTagL2 = new XMLParserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;
	XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	long currentCmapNodeIDInCmapNodeList;
	
	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
	currentCmapNodeIDInCmapNodeList = 0;
	resetIDsForNodeList2(conceptEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(propertyEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(actionEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	#endif

	currentCmapNodeIDInCmapNodeList = 0;
	
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, true, false))
	{
		result = false;
	}
	
	cout << "a" << endl;
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	currentCmapNodeIDInCmapNodeList = 0;
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, false, false))
	{
		result = false;

	}
	XMLParserTag * firstTagInLinkingPhraseList = currentTagL2->firstLowerLevelTag;
		
	//cout << "b" << endl;
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, true, true))
	{
		result = false;
	}
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	//cout << "c" << endl;
	
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, false, true))
	{
		result = false;
	}
	XMLParserTag * firstTagInLinkingPhraseAppearanceList = currentTagL2->firstLowerLevelTag;
	
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	cout << "d" << endl;
		
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	if(!generateCXLConnectionNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, firstTagInLinkingPhraseList, firstTagInLinkingPhraseAppearanceList))
	{
		result = false;
	}	
	
	cout << "e" << endl;	

	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;

	return result;
}

//generateCXLEntityNodeTagList(): generates and fills <concept-list> and <concept-appearance-list> tags. generates <linking-phrase-list> and <linking-phrase-appearance-list> tags.
bool generateCXLEntityNodeTagList(XMLParserTag * firstTagInMap, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, long * currentCmapNodeIDInCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagL0 = firstTagInMap;
	if(conceptOrLinkingPhraseList)
	{
		if(appearanceList)
		{
			currentTagL0->name = NET_CXL_TAG_conceptList;
		}
		else
		{
			currentTagL0->name = NET_CXL_TAG_conceptAppearanceList;
		}
	}
	else
	{
		if(appearanceList)
		{
			currentTagL0->name = NET_CXL_TAG_linkingPhraseList;
		}
		else
		{
			currentTagL0->name = NET_CXL_TAG_linkingPhraseAppearanceList;
		}		
	}
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	if(conceptOrLinkingPhraseList)
	{	
		#ifdef GIA_CMAP_CONVERSION_SANITISED
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		#else
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		#endif
	}

	//cout << "h6" << endl;
	
	return result;
}


XMLParserTag * addToCXLEntityNodeTagList(XMLParserTag * currentTagL1, vector<GIAEntityNode*> *entityNodesList, long * currentCmapNodeIDInCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	//cout << "h3" << endl;

	bool result = true;
		
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;

		if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
		{//this exception is required because timeCondition nodes have a separate id in Cmap maps / CXL
			currentEntity->timeConditionNode->id = (*currentCmapNodeIDInCmapNodeList);
			(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
		}
			
		#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
		currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->entityName, *currentCmapNodeIDInCmapNodeList, currentEntity->printXIndex, currentEntity->printYIndex, conceptOrLinkingPhraseList, appearanceList);
		#else		
		currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->entityName, currentEntity->id, currentEntity->printXIndex, currentEntity->printYIndex, conceptOrLinkingPhraseList, appearanceList);
		#endif
		(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
		
		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
	
	return currentTagL1;
}

XMLParserTag * generateCXLEntityNodeTag(XMLParserTag * currentTagL1, string entityName, long entityID, int printXIndex, int printYIndex, bool conceptOrLinkingPhraseList, bool appearanceList)
{	
	bool result = true;
	
	//cout << "h5" << endl;

	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	//generate neuron connection tag
	if(conceptOrLinkingPhraseList)
	{
		if(appearanceList)
		{
			currentTagL1->name = NET_CXL_TAG_concept;
		}
		else
		{
			currentTagL1->name = NET_CXL_TAG_conceptAppearance;
		}
	}
	else
	{
		if(appearanceList)
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhrase;
		}
		else
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhraseAppearance;
		}		
	}
	XMLParserTag * firstTagL2 = new XMLParserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLParserAttribute * currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_ATTRIBUTE_id;
	sprintf(tempString, "%ld", entityID);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	if(appearanceList)
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_label;
		currentAttribute->value = entityName;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;
	}
	else
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_x;
		sprintf(tempString, "%d", printXIndex);
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_y;
		sprintf(tempString, "%d", printYIndex);
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;	
	}
}

//generateCXLConnectionNodeTagList(): generates <connection-list> tag, and fills <linking-phrase-list>, and  <linking-phrase-appearance-list> tags
bool generateCXLConnectionNodeTagList(XMLParserTag * firstTagInMap, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, long * currentCmapNodeIDInCmapNodeList, XMLParserTag * firstTagInLinkingPhraseList, XMLParserTag * firstTagInLinkingPhraseAppearanceList)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagInLinkingPhraseList = firstTagInLinkingPhraseList;
	XMLParserTag * currentTagInLinkingPhraseAppearanceList = firstTagInLinkingPhraseAppearanceList;

	XMLParserTag * currentTagL0 = firstTagInMap;
	currentTagL0->name = NET_CXL_TAG_connectionList;

	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;
	
	#ifdef GIA_CMAP_CONVERSION_SANITISED
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	#else
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList);
	#endif

	//cout << "h6" << endl;
	
	return result;
}

XMLParserTag * generateCXLConnectionNodeTagAndLinkingPhraseTags(XMLParserTag * currentTagL1, GIAEntityNode * entity1, GIAEntityNode * entity2, string connectionTypeName, long * currentCmapNodeIDInCmapNodeList, XMLParserTag ** currentTagInLinkingPhraseList, XMLParserTag ** currentTagInLinkingPhraseAppearanceList)
{
	int entity1ID = entity1->id;
	int entity2ID = entity2->id;
	int entity1X =  entity1->printXIndex;
	int entity2X =  entity2->printXIndex;
	int entity1Y =  entity1->printYIndex;
	int entity2Y =  entity2->printYIndex;	
	int connectionTypeX = (entity1X + entity2X) / 2;
	int connectionTypeY = (entity1Y + entity2Y) / 2;
	int connectionID = (*currentCmapNodeIDInCmapNodeList);
	
	//verify connection is not already found;
	bool alreadyMadeConnection = false;
	XMLParserTag * currentTagInList = currentTagL1;
	while(currentTagInList->nextTag != NULL)
	{
		if(currentTagInList->nextTag->nextTag != NULL)
		{
			int currentFromIDinList[2];
			int currentToIDinList[2];
			XMLParserAttribute * firstAttributeInList[2];
			firstAttributeInList[0] = currentTagInList->firstAttribute;
			firstAttributeInList[1] = currentTagInList->nextTag->firstAttribute;
			for(int i=0; i<2; i++)
			{
				XMLParserAttribute * currentAttributeInList = firstAttributeInList[i]; 
				while(currentAttributeInList->nextAttribute != NULL)
				{
					if(currentAttributeInList->name == NET_CXL_TAG_fromid)
					{
						currentFromIDinList[i] = atoi(currentAttributeInList->value.c_str());				
					}
					if(currentAttributeInList->name == NET_CXL_TAG_toid)
					{
						currentToIDinList[i] = atoi(currentAttributeInList->value.c_str());				
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

	if(!alreadyMadeConnection)
	{	
	
		currentTagL1 = generateCXLConnectionNodeTag(currentTagL1, entity1ID, connectionID);
		currentTagL1 = generateCXLConnectionNodeTag(currentTagL1, connectionID, entity2ID);
		*currentTagInLinkingPhraseList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, false);
		*currentTagInLinkingPhraseAppearanceList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseAppearanceList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, true);
		(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
	}
}
		
		
XMLParserTag * addToCXLConnectionNodeTagList(XMLParserTag * currentTagL1, vector<GIAEntityNode*> *entityNodesList, long * currentCmapNodeIDInCmapNodeList, XMLParserTag * firstTagL1, XMLParserTag ** currentTagInLinkingPhraseList, XMLParserTag ** currentTagInLinkingPhraseAppearanceList)
{
	//cout << "h3" << endl;

	bool result = true;
		
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;

		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		if(currentEntity->AssociatedInstanceNodeList.begin() == currentEntity->AssociatedInstanceNodeList.end())
		{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (property node) 
		#endif
		
			#ifdef GIA_CMAP_CONVERSION_SANITISED
			if(currentEntity->isAction)
			{
				string connectionTypeName = currentEntity->entityName;
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity->actionSubjectEntity, currentEntity->actionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);		
			}
			if(currentEntity->isCondition)
			{
				string connectionTypeName = currentEntity->entityName;
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity->conditionSubjectEntity, currentEntity->conditionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);				
			}				
			#else
			if(currentEntity->actionSubjectEntity != NULL)
			{
				string connectionTypeName = "subject";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->actionSubjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);
			}	

			if(currentEntity->actionObjectEntity != NULL)
			{
				string connectionTypeName = "object";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->actionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);			
			}

			if(currentEntity->conditionSubjectEntity != NULL)
			{	
				string connectionTypeName = "subject";	
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->conditionSubjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);			
			}	

			if(currentEntity->conditionObjectEntity != NULL)
			{
				string connectionTypeName = "object";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->conditionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);			
			}		
			#endif


			#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			if(currentEntity->entityNodeDefiningThisInstance != NULL)
			{
				string connectionTypeName = "instance";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->entityNodeDefiningThisInstance, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);						
			}
			#endif

			/*
			if(currentEntity->ActionNodeList.begin() != currentEntity->ActionNodeList.end())
			{
				for(currentEntity->ActionNodeListIterator = currentEntity->ActionNodeList.begin(); currentEntity->ActionNodeListIterator < currentEntity->ActionNodeList.end(); currentEntity->ActionNodeListIterator++)
				{	
					string connectionTypeName = "instance";
					*(currentEntity->ActionNodeListIterator)

				} 
			}
			if(currentEntity->IncomingActionNodeList.begin() != currentEntity->IncomingActionNodeList.end())
			{
				for(currentEntity->IncomingActionNodeListIterator = currentEntity->IncomingActionNodeList.begin(); currentEntity->IncomingActionNodeListIterator < currentEntity->IncomingActionNodeList.end(); currentEntity->IncomingActionNodeListIterator++)
				{	
					*(currentEntity->IncomingActionNodeListIterator)
				} 
			}
			*/

			/*
			if(currentEntity->ConditionNodeList.begin() != currentEntity->ConditionNodeList.end())
			{
				for(currentEntity->ConditionNodeListIterator = currentEntity->ConditionNodeList.begin(); currentEntity->ConditionNodeListIterator < currentEntity->ConditionNodeList.end(); currentEntity->ConditionNodeListIterator++)
				{	
					*(currentEntity->ConditionNodeListIterator)
				} 
			}

			if(currentEntity->IncomingConditionNodeList.begin() != currentEntity->IncomingConditionNodeList.end())
			{
				for(currentEntity->IncomingConditionNodeListIterator = currentEntity->IncomingConditionNodeList.begin(); currentEntity->IncomingConditionNodeListIterator < currentEntity->IncomingConditionNodeList.end(); currentEntity->IncomingConditionNodeListIterator++)
				{	
					*(currentEntity->IncomingConditionNodeListIterator)
				} 
			}
			*/			

			/*
			if(currentEntity->PropertyNodeList.begin() != currentEntity->PropertyNodeList.end())
			{
				for(currentEntity->PropertyNodeListIterator = currentEntity->PropertyNodeList.begin(); currentEntity->PropertyNodeListIterator < currentEntity->PropertyNodeList.end(); currentEntity->PropertyNodeListIterator++)
				{	
					string connectionTypeName = "property";
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, *(currentEntity->PropertyNodeListIterator), connectionTypeName);							
				} 
			}
			*/

			if(currentEntity->PropertyNodeReverseList.begin() != currentEntity->PropertyNodeReverseList.end())
			{
				for(currentEntity->PropertyNodeReverseListIterator = currentEntity->PropertyNodeReverseList.begin(); currentEntity->PropertyNodeReverseListIterator < currentEntity->PropertyNodeReverseList.end(); currentEntity->PropertyNodeReverseListIterator++)
				{	
					string connectionTypeName = "property";
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, *(currentEntity->PropertyNodeReverseListIterator), connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);							
				} 
			}

			if(currentEntity->EntityNodeDefinitionList.begin() != currentEntity->EntityNodeDefinitionList.end())
			{
				for(currentEntity->EntityNodeDefinitionListIterator = currentEntity->EntityNodeDefinitionList.begin(); currentEntity->EntityNodeDefinitionListIterator < currentEntity->EntityNodeDefinitionList.end(); currentEntity->EntityNodeDefinitionListIterator++)
				{
					string connectionTypeName = "definition";
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, *(currentEntity->EntityNodeDefinitionListIterator), connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);
				} 
			}

			/*
			if(currentEntity->EntityNodeDefinitionReverseList.begin() != currentEntity->EntityNodeDefinitionReverseList.end())
			{
				for(currentEntity->EntityNodeDefinitionReverseListIterator = currentEntity->EntityNodeDefinitionReverseList.begin(); currentEntity->EntityNodeDefinitionReverseListIterator < currentEntity->EntityNodeDefinitionReverseList.end(); currentEntity->EntityNodeDefinitionReverseListIterator++)
				{	
					*(currentEntity->EntityNodeDefinitionReverseListIterator)
				} 
			}
			*/

			/*
			if(currentEntity->AssociatedInstanceNodeList.begin() != currentEntity->AssociatedInstanceNodeList.end())
			{
				for(currentEntity->AssociatedInstanceNodeListIterator = currentEntity->AssociatedInstanceNodeList.begin(); currentEntity->AssociatedInstanceNodeListIterator < currentEntity->AssociatedInstanceNodeList.end(); currentEntity->AssociatedInstanceNodeListIterator++)
				{	
					*(currentEntity->AssociatedInstanceNodeListIterator)
				} 
			}
			*/

			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				GIAEntityNode fakeTimeEntity;
				fakeTimeEntity.entityName = currentEntity->timeConditionNode->conditionName;
				fakeTimeEntity.printX = currentEntity->timeConditionNode->printX;
				fakeTimeEntity.printY = currentEntity->timeConditionNode->printY;
				fakeTimeEntity.id = currentEntity->timeConditionNode->id;
				string connectionTypeName = "time";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, &fakeTimeEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList);
			}
		
		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		}		
		#endif
		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
	
	return currentTagL1;
}


XMLParserTag * generateCXLConnectionNodeTag(XMLParserTag * currentTagL1, long fromID, long toID)
{	
	bool result = true;
	
	//cout << "h5" << endl;

	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	//generate neuron connection tag
	currentTagL1->name = NET_CXL_TAG_connection;
	
	XMLParserTag * firstTagL2 = new XMLParserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLParserAttribute * currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_TAG_fromid;			
	sprintf(tempString, "%ld", fromID);
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_CXL_TAG_toid;			
	sprintf(tempString, "%ld", toID);
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;
}



#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
void resetIDsForNodeList2(vector<GIAEntityNode*> *entityNodesList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	//cout << "h3" << endl;

	
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;
		currentEntity->id = *currentEntityNodeIDInConceptEntityNodesList;
		(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
}
#endif


