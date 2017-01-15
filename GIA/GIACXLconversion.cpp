/*******************************************************************************
 *
 * File Name: GIACXLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1d 22-May-2012
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
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
	//cout << "zz" << endl;
	
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

	XMLParserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_CXL_TAG_cmap;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserAttribute * currentAttribute = currentTagL0->firstAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlns;
		currentAttribute->value = NET_CXL_ATTRIBUTE_xmlns_DEFAULT_VALUE;

		XMLParserAttribute * newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlnsdc;
		currentAttribute->value = NET_CXL_ATTRIBUTE_xmlnsdc_DEFAULT_VALUE;

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
	
	/*
	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_THIS_SHOULDNT_EVER_BE_USED
	currentCmapNodeIDInCmapNodeList = 0;
	resetIDsForNodeList2(conceptEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(propertyEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(actionEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList);
	#endif
	*/
	
	//cout << "z" << endl;
	
	currentCmapNodeIDInCmapNodeList = 0;
	long firstCmapNodeIDInConceptList = currentCmapNodeIDInCmapNodeList;	
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, true, false))
	{
		result = false;
	}
	
	//cout << "a" << endl;
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	long firstCmapNodeIDInLinkingPhraseList = currentCmapNodeIDInCmapNodeList;	//NOT NECESSARY: redundant
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, false, false))
	{
		result = false;

	}
	XMLParserTag * firstTagInLinkingPhraseList = currentTagL2->firstLowerLevelTag;
		
	//cout << "b" << endl;
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	currentCmapNodeIDInCmapNodeList = firstCmapNodeIDInConceptList;
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, true, true))
	{
		result = false;
	}
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	//cout << "c" << endl;
	
	currentCmapNodeIDInCmapNodeList = firstCmapNodeIDInLinkingPhraseList;	//NOT NECESSARY: redundant
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, false, true))
	{
		result = false;
	}
	XMLParserTag * firstTagInLinkingPhraseAppearanceList = currentTagL2->firstLowerLevelTag;
	
	
	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;
	
	//cout << "d" << endl;
	
	XMLParserTag * currentTagInLinkingPhraseList = firstTagInLinkingPhraseList;
	XMLParserTag * currentTagInLinkingPhraseAppearanceList = firstTagInLinkingPhraseAppearanceList;
	while(currentTagInLinkingPhraseList->nextTag != NULL)
	{
		currentTagInLinkingPhraseList = currentTagInLinkingPhraseList->nextTag;
	}
	while(currentTagInLinkingPhraseAppearanceList->nextTag != NULL)
	{
		currentTagInLinkingPhraseAppearanceList = currentTagInLinkingPhraseAppearanceList->nextTag;
	}
	XMLParserTag * lastTagInLinkingPhraseList = currentTagInLinkingPhraseList;
	XMLParserTag * lastTagInLinkingPhraseAppearanceList = currentTagInLinkingPhraseAppearanceList;
			
	if(!generateCXLConnectionNodeTagList(currentTagL2, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, &currentCmapNodeIDInCmapNodeList, lastTagInLinkingPhraseList, lastTagInLinkingPhraseAppearanceList))
	{
		result = false;
	}	
	
	//cout << "e" << endl;	

	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;

	return result;
}

//generateCXLEntityNodeTagList(): generates and fills <concept-list>, <concept-appearance-list>, <linking-phrase-list> and <linking-phrase-appearance-list> tags.
bool generateCXLEntityNodeTagList(XMLParserTag * firstTagInMap, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, long * currentCmapNodeIDInCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagL0 = firstTagInMap;
	if(conceptOrLinkingPhraseList)
	{
		if(!appearanceList)
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
		if(!appearanceList)
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

	/*OLD version <= 1h4d
	if(conceptOrLinkingPhraseList)
	{	
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	*/
			
	#ifdef GIA_CMAP_CONVERSION_SANITISED
	//cout << "z2" << endl;
	if(conceptOrLinkingPhraseList)
	{
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	else
	{
		//cout << "sad" << endl;
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing		
		//cout << "sad2" << endl;
	}
	#else
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	#endif
		
	

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

		//cout << "h4" << endl;
		
		#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
		if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
		{//this exception is required because timeCondition nodes have a separate idActiveList in Cmap maps / CXL
			//cout << "asd" << endl;
			currentEntity->timeConditionNode->idActiveList = (*currentCmapNodeIDInCmapNodeList);			
			currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->timeConditionNode->conditionName, *currentCmapNodeIDInCmapNodeList, currentEntity->timeConditionNode->printX, currentEntity->timeConditionNode->printY, conceptOrLinkingPhraseList, appearanceList);		
			(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
		}
		#endif
		
		if(!(currentEntity->disabled))
		{
			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			if(currentEntity->AssociatedInstanceNodeList.begin() == currentEntity->AssociatedInstanceNodeList.end())
			{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (property node)
			#endif
				//cout << "DEBUG: currentEntity->entityName = " << currentEntity->entityName << endl;

				currentEntity->idActiveListReorderdIDforXMLsave = *currentCmapNodeIDInCmapNodeList;
				/*
				#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
				currentEntity->idActiveListReorderdIDforXMLsave = *currentCmapNodeIDInCmapNodeList;
				//cout << "currentEntity->idActiveListReorderdIDforXMLsave = " << currentEntity->idActiveListReorderdIDforXMLsave << endl;
				long currentEntityID = *currentCmapNodeIDInCmapNodeList;
				#else	
				long currentEntityID = currentEntity->idActiveList;
				#endif
				*/
				
				currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->entityName, currentEntityID, currentEntity->printX, currentEntity->printY, conceptOrLinkingPhraseList, appearanceList);				
				(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;

				//cout << "h5f" << endl;
			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			}
			#endif	
		}		
	}

	//cout << "h6" << endl;
	
	return currentTagL1;
}

XMLParserTag * generateCXLEntityNodeTag(XMLParserTag * currentTagL1, string entityName, long entityID, int printX, int printY, bool conceptOrLinkingPhraseList, bool appearanceList)
{	
	bool result = true;
	
	//cout << "h5" << endl;

	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	//generate neuron connection tag
	if(conceptOrLinkingPhraseList)
	{
		if(!appearanceList)
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
		if(!appearanceList)
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhrase;
		}
		else
		{
			currentTagL1->name = NET_CXL_TAG_linkingPhraseAppearance;
		}		
	}
	//cout << "h5" << endl;
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

	//cout << "h5ii" << endl;
	
	if(!appearanceList)
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
		sprintf(tempString, "%d", printX*GIA_CXL_EXPAND_RATIO);
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_CXL_ATTRIBUTE_y;
		sprintf(tempString, "%d", printY*GIA_CXL_EXPAND_RATIO);
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;	
	}
	//cout << "h5iii" << endl;
	XMLParserTag * newTag = new XMLParserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;	
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
	
	//cout << "h3b" << endl;

	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conceptEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, propertyEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, actionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, conditionEntityNodesList, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	
	//cout << "h6" << endl;
	
	return result;
}	

#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES	
bool checkIfPassedRedundantConceptNodeRemoval(GIAEntityNode * currentEntity)
{
	bool result = false;

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
	if(currentEntity->AssociatedInstanceNodeList.begin() == currentEntity->AssociatedInstanceNodeList.end())
	{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (property node) 
	//if((currentEntity->isAction) || (currentEntity->isProperty) || (currentEntity->isCondition))
	//{//do not add raw concept nodes
		result = true;
	}
	#else
			
	if((currentEntity->AssociatedInstanceNodeList.begin() == currentEntity->AssociatedInstanceNodeList.end()))
	{
		//entity does not have an associated instance [entity is either a raw/isolated concept node, or it is a property]
		result = true;
	}
	else
	{
		//entity has an associated instance [entity is a concept node]

		if(currentEntity->actionSubjectEntity != NULL)
		{
			result = true;				
		}
		if(currentEntity->actionObjectEntity != NULL)
		{
			result = true;
		}

		if(currentEntity->conditionSubjectEntity != NULL)
		{
			result = true;				
		}
		if(currentEntity->conditionObjectEntity != NULL)
		{
			result = true;
		}	
		
		if(currentEntity->entityNodeDefiningThisInstance != NULL)
		{//this shouldnt be defined, since this entity is a concept node, not a property node 
			result = true;		
		}

		//cout << "w2" << endl;

		
		if(currentEntity->ActionNodeList.begin() != currentEntity->ActionNodeList.end())
		{
			result = true;
		}
		if(currentEntity->IncomingActionNodeList.begin() != currentEntity->IncomingActionNodeList.end())
		{
			result = true;
		}
		
		if(currentEntity->ConditionNodeList.begin() != currentEntity->ConditionNodeList.end())
		{
			result = true;
		}

		if(currentEntity->IncomingConditionNodeList.begin() != currentEntity->IncomingConditionNodeList.end())
		{
			result = true;
		}
					

		if(currentEntity->PropertyNodeList.begin() != currentEntity->PropertyNodeList.end())
		{
			result = true;
		}

		if(currentEntity->PropertyNodeReverseList.begin() != currentEntity->PropertyNodeReverseList.end())
		{
			result = true;
		}

		if(currentEntity->EntityNodeDefinitionList.begin() != currentEntity->EntityNodeDefinitionList.end())
		{
			result = true;
		}

		if(currentEntity->EntityNodeDefinitionReverseList.begin() != currentEntity->EntityNodeDefinitionReverseList.end())
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
	
XMLParserTag * addToCXLConnectionNodeTagList(XMLParserTag * currentTagL1, vector<GIAEntityNode*> *entityNodesList, long * currentCmapNodeIDInCmapNodeList, XMLParserTag * firstTagL1, XMLParserTag ** currentTagInLinkingPhraseList, XMLParserTag ** currentTagInLinkingPhraseAppearanceList, XMLParserTag * firstTagInConnectionsList)
{
	//cout << "h3ii" << endl;

	bool result = true;
		
	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;

		
		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		if(checkIfPassedRedundantConceptNodeRemoval(currentEntity))
		{
		#endif
			//cout << "cxl1" << endl;
			//cout << "w1" << endl;
			
			#ifdef GIA_CMAP_CONVERSION_SANITISED
			if(currentEntity->isAction)
			{
				//cout << "w1b" << endl;
				string connectionTypeName = currentEntity->entityName;
				//cout << "connectionTypeName = " << connectionTypeName << endl;
				GIAEntityNode * actionSubjectEntity = currentEntity->actionSubjectEntity;
				GIAEntityNode * actionObjectEntity = currentEntity->actionObjectEntity;
				long linkingPhraseID = currentEntity->idActiveListReorderdIDforXMLsave;
				bool subjectIsEmpty = false;
				bool objectIsEmpty = false;
				if(currentEntity->actionSubjectEntity == NULL)
				{
					subjectIsEmpty = true;				
					actionSubjectEntity = new GIAEntityNode();
					actionSubjectEntity->CXLdummyNode = true;
					//cout << "currentEntity->actionSubjectEntity == NULL" << endl;					
				
				}
				if(currentEntity->actionObjectEntity == NULL)
				{
					objectIsEmpty = true;
					actionObjectEntity = new GIAEntityNode();
					actionObjectEntity->CXLdummyNode = true;
					//cout << "currentEntity->actionObjectEntity == NULL" << endl;	
				
				}
				if(!(subjectIsEmpty && objectIsEmpty))
				{	
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, actionSubjectEntity, actionObjectEntity, connectionTypeName, &linkingPhraseID, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, true);		
				}
				if(subjectIsEmpty)
				{
					delete actionSubjectEntity;
				}
				if(objectIsEmpty)
				{
					delete actionObjectEntity;
				}
							
			}
			
			if(currentEntity->isCondition)
			{
				//cout << "w1c" << endl;
				string connectionTypeName = currentEntity->entityName;
				
				GIAEntityNode * conditionSubjectEntity = currentEntity->conditionSubjectEntity;
				GIAEntityNode * conditionObjectEntity = currentEntity->conditionObjectEntity;
				long linkingPhraseID = currentEntity->idActiveListReorderdIDforXMLsave;
				bool subjectIsEmpty = false;
				bool objectIsEmpty = false;
				if(currentEntity->conditionSubjectEntity == NULL)
				{
					subjectIsEmpty = true;
					conditionSubjectEntity = new GIAEntityNode();
					conditionSubjectEntity->CXLdummyNode = true;	
					//cout << "currentEntity->conditionSubjectEntity == NULL" << endl;
				}
				if(currentEntity->conditionObjectEntity == NULL)
				{
					objectIsEmpty = true;		
					conditionObjectEntity = new GIAEntityNode();
					conditionObjectEntity->CXLdummyNode = true;
					//cout << "currentEntity->conditionObjectEntity == NULL" << endl;
						
				}
				if(!(subjectIsEmpty && objectIsEmpty))
				{							
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, conditionSubjectEntity, conditionObjectEntity, connectionTypeName, &linkingPhraseID, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, true);				
				}
				
				if(subjectIsEmpty)
				{
					delete conditionSubjectEntity;
				}
				if(objectIsEmpty)
				{
					delete conditionObjectEntity;
				}			
			}
							
			#else
			if(currentEntity->actionSubjectEntity != NULL)
			{
				string connectionTypeName = "subject";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->actionSubjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
			}	

			if(currentEntity->actionObjectEntity != NULL)
			{
				string connectionTypeName = "object";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->actionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);			
			}

			if(currentEntity->conditionSubjectEntity != NULL)
			{	
				string connectionTypeName = "subject";	
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->conditionSubjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);			
			}	

			if(currentEntity->conditionObjectEntity != NULL)
			{
				string connectionTypeName = "object";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->conditionObjectEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);			
			}		
			#endif
			
			//cout << "cxl2" << endl;
			

			//cout << "w1d" << endl;

			#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
			if(currentEntity->entityNodeDefiningThisInstance != NULL)
			{
				string connectionTypeName = "instance";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, currentEntity->entityNodeDefiningThisInstance, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);						
			}
			#endif

			//cout << "w2" << endl;
			
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

			//cout << "cxl3" << endl;
			if(currentEntity->PropertyNodeReverseList.begin() != currentEntity->PropertyNodeReverseList.end())
			{
				for(vector<GIAEntityNode*>::iterator entityIter = currentEntity->PropertyNodeReverseList.begin(); entityIter < currentEntity->PropertyNodeReverseList.end(); entityIter++)
				{	
					string connectionTypeName = "property";
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, *(entityIter), connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);							
				} 
			}

			if(currentEntity->EntityNodeDefinitionList.begin() != currentEntity->EntityNodeDefinitionList.end())
			{
				for(vector<GIAEntityNode*>::iterator entityIter = currentEntity->EntityNodeDefinitionList.begin(); entityIter < currentEntity->EntityNodeDefinitionList.end(); entityIter++)
				{
					string connectionTypeName = "definition";
					currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, *(entityIter), connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
				} 
			}
			//cout << "cxl4" << endl;

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

			#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
			if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				GIAEntityNode fakeTimeEntity;
				fakeTimeEntity.entityName = currentEntity->timeConditionNode->conditionName;
				fakeTimeEntity.printX = currentEntity->timeConditionNode->printX;
				fakeTimeEntity.printY = currentEntity->timeConditionNode->printY;
				fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave;
				/*
				#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
				fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveList;
				#else
				fakeTimeEntity.idActiveList = currentEntity->timeConditionNode->idActiveList;				
				#endif
				*/
				string connectionTypeName = "time";
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, &fakeTimeEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
			}
			#endif
			
			//cout << "cxl5" << endl;
			//cout << "w3" << endl;
		
		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		}		
		#endif
		//cout << "h5f" << endl;
	}

	//cout << "w6" << endl;
	
	return currentTagL1;
}

XMLParserTag * generateCXLConnectionNodeTagAndLinkingPhraseTags(XMLParserTag * currentTagL1, GIAEntityNode * entity1, GIAEntityNode * entity2, string connectionTypeName, long * currentCmapNodeIDInCmapNodeList, XMLParserTag ** currentTagInLinkingPhraseList, XMLParserTag ** currentTagInLinkingPhraseAppearanceList, XMLParserTag * firstTagInConnectionsList, bool alreadyMadeLinkingPhrase)
{
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;	
	/*		
	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
	//cout << "entity1->idActiveListReorderdIDforXMLsave = " << entity1->idActiveListReorderdIDforXMLsave << endl;
	//cout << "entity2->idActiveListReorderdIDforXMLsave = " << entity2->idActiveListReorderdIDforXMLsave << endl;
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;	
	#else
	int entity1ID = entity1->idActiveList;
	int entity2ID = entity2->idActiveList;	
	#endif
	*/
	//cout << "entity1ID = " << entity1ID << endl;
	//cout << "entity2ID = " << entity2ID << endl;
		
	int entity1X =  entity1->printX;
	int entity2X =  entity2->printX;
	int entity1Y =  entity1->printY;
	int entity2Y =  entity2->printY;	
	int connectionTypeX = (entity1X + entity2X) / 2;
	int connectionTypeY = (entity1Y + entity2Y) / 2;
	int connectionID = (*currentCmapNodeIDInCmapNodeList);
					
	//cout << "q1" << endl;
	
	//verify connection is not already found;
	bool alreadyMadeConnection = false;
	/*
	XMLParserTag * currentTagInList = firstTagInConnectionsList;
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
	*/
	
	//cout << "q2" << endl;

	if(!alreadyMadeConnection)
	{	
		//cout << "q3" << endl;
		//cout << "currentTagL1->name = " << currentTagL1->name << endl;
		//cout << "entity1ID = " << entity1ID << endl;
		//cout << "connectionID = " << connectionID << endl;		
		//cout << "q3" << endl;
		if(!(entity1->CXLdummyNode))
		{
			currentTagL1 = generateCXLConnectionNodeTag(currentTagL1, entity1ID, connectionID);
		}
		if(!(entity2->CXLdummyNode))
		{		
			currentTagL1 = generateCXLConnectionNodeTag(currentTagL1, connectionID, entity2ID);
		}
		if(!alreadyMadeLinkingPhrase)
		{
			//cout << "a" << endl;
			*currentTagInLinkingPhraseList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, false);
			*currentTagInLinkingPhraseAppearanceList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseAppearanceList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, true);		
			(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
			//cout << "b" << endl;
		}
		//cout << "q4" << endl;
	}

	//cout << "q5" << endl;
	return currentTagL1;
}
	

XMLParserTag * generateCXLConnectionNodeTag(XMLParserTag * currentTagL1, long fromID, long toID)
{	
	bool result = true;
	
	/*
	cout << "fromID = " << fromID << endl;
	cout << "toID = " << toID << endl;
	*/
	
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

	XMLParserTag * newTag = new XMLParserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;
	
	return currentTagL1;
}


/*
#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_THIS_SHOULDNT_EVER_BE_USED
void resetIDsForNodeList2(vector<GIAEntityNode*> *entityNodesList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	//cout << "h3" << endl;

	for(vector<GIAEntityNode*>::iterator entityNodesCompleteListIterator = entityNodesList->begin(); entityNodesCompleteListIterator < entityNodesList->end(); entityNodesCompleteListIterator++)	
	{
		GIAEntityNode * currentEntity = *entityNodesCompleteListIterator;
		currentEntity->idActiveList = *currentEntityNodeIDInConceptEntityNodesList;
		(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;

		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
}
#endif
*/

