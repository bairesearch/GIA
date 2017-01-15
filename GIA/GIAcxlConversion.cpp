/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAcxlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2l6b 29-December-2016
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 *
 *******************************************************************************/


#include "GIAcxlConversion.h"

bool writeCmapToolsCXLFileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
{
	vector<GIAentityNode*>* entityNodesActiveListConcepts = new vector<GIAentityNode*>;
	for(unordered_map<string, GIAentityNode*>::iterator conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = conceptEntityNodesListMapIter->second;
		entityNodesActiveListConcepts->push_back(entityNode);
	}

	bool result;
	result = writeCmapToolsCXLfile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);

	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif

	return result;
}

bool writeCmapToolsCXLfile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
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
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_CXL_ATTRIBUTE_xmlnsdc;
		currentAttribute->value = NET_CXL_ATTRIBUTE_xmlnsdc_DEFAULT_VALUE;
		currentAttribute = createNewAttribute(currentAttribute);

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

	long currentCmapNodeIDinCmapNodeList;


	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
	currentCmapNodeIDinCmapNodeList = 0;
	resetIDsForNodeList2(entityNodesActiveListConcepts, &currentCmapNodeIDinCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListSubstances, &currentCmapNodeIDinCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListActions, &currentCmapNodeIDinCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList);
	#endif


	currentCmapNodeIDinCmapNodeList = 0;
	long firstCmapNodeIDInConceptList = currentCmapNodeIDinCmapNodeList;
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList, true, false))
	{
		result = false;
	}


	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	long firstCmapNodeIDInLinkingPhraseList = currentCmapNodeIDinCmapNodeList;	//NOT NECESSARY: redundant
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList, false, false))
	{
		result = false;

	}
	XMLparserTag* firstTagInLinkingPhraseList = currentTagL2->firstLowerLevelTag;


	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	currentCmapNodeIDinCmapNodeList = firstCmapNodeIDInConceptList;
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList, true, true))
	{
		result = false;
	}

	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;


	currentCmapNodeIDinCmapNodeList = firstCmapNodeIDInLinkingPhraseList;	//NOT NECESSARY: redundant
	if(!generateCXLentityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList, false, true))
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

	if(!generateCXLconnectionNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDinCmapNodeList, lastTagInLinkingPhraseList, lastTagInLinkingPhraseAppearanceList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "DEBUG generateXMLTagListBasedUponSubnet done " << endl;
	#endif

 	if(!writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	#ifdef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "DEBUG writeXMLfile done " << endl;
	#endif

	return result;
}

//generateCXLentityNodeTagList{}: generates and fills <concept-list>, <concept-appearance-list>, <linking-phrase-list> and <linking-phrase-appearance-list> tags.
bool generateCXLentityNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, long* currentCmapNodeIDinCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

	XMLparserTag* currentTagL0 = firstTagInMap;
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
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	/*OLD version <= 1h4d
	if(conceptOrLinkingPhraseList)
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	*/

	#ifdef GIA_CMAP_CONVERSION_SANITISED
	if(conceptOrLinkingPhraseList)
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	else
	{
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing
		currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing
	}
	#else
	currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDinCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	#endif


	return result;
}


XMLparserTag* addToCXLentityNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;

		#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
		if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
		{//this exception is required because timeCondition nodes have a separate idActiveList in Cmap maps / CXL
			currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave = (*currentCmapNodeIDinCmapNodeList);
			currentTagL1 = generateCXLentityNodeTag(currentTagL1, currentEntity->timeConditionNode->conditionName,* currentCmapNodeIDinCmapNodeList, currentEntity->timeConditionNode->printX, currentEntity->timeConditionNode->printY, conceptOrLinkingPhraseList, appearanceList);
			(*currentCmapNodeIDinCmapNodeList) = (*currentCmapNodeIDinCmapNodeList) + 1;
		}
		#endif

		if(!(currentEntity->disabled))
		{
			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			if(currentEntity->associatedInstanceNodeList->empty())
			{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (substance node)
			#endif
				#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
				cout << "DEBUG: currentEntity->entityName = " << currentEntity->entityName << endl;
				#endif

				#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
				currentEntity->idActiveListReorderdIDforXMLsave = *currentCmapNodeIDinCmapNodeList;
				long currentEntityID = currentEntity->idActiveListReorderdIDforXMLsave;
				#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
				cout << "DEBUG: currentEntity->idActiveListReorderdIDforXMLsave = " << currentEntity->idActiveListReorderdIDforXMLsave << endl;
				#endif
				#elif defined GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
				//idActiveListReorderdIDforXMLsave already set
				long currentEntityID = currentEntity->idActiveListReorderdIDforXMLsave;
				#else
				long currentEntityID = currentEntity->idActiveList;
				#endif

				currentTagL1 = generateCXLentityNodeTag(currentTagL1, currentEntity->entityName, currentEntityID, currentEntity->printX, currentEntity->printY, conceptOrLinkingPhraseList, appearanceList);
				(*currentCmapNodeIDinCmapNodeList) = (*currentCmapNodeIDinCmapNodeList) + 1;

			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			}
			#endif
		}
	}

	return currentTagL1;
}

XMLparserTag* generateCXLentityNodeTag(XMLparserTag* currentTagL1, string entityName, long entityID, int printX, int printY, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

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

	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_ATTRIBUTE_id;
	currentAttribute->value = convertLongToString(entityID);
	currentAttribute = createNewAttribute(currentAttribute);


	if(!appearanceList)
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_label;
		currentAttribute->value = entityName;
		currentAttribute = createNewAttribute(currentAttribute);
	}
	else
	{
		currentAttribute->name = NET_CXL_ATTRIBUTE_x;
		currentAttribute->value = convertIntToString(printX*GIA_CXL_EXPAND_RATIO);;
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_CXL_ATTRIBUTE_y;
		currentAttribute->value = convertIntToString(printY*GIA_CXL_EXPAND_RATIO);;
		currentAttribute = createNewAttribute(currentAttribute);
	}

	XMLparserTag* newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}

//generateCXLconnectionNodeTagList{}: generates <connection-list> tag, and fills <linking-phrase-list>, and  <linking-phrase-appearance-list> tags
bool generateCXLconnectionNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, long* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagInLinkingPhraseList, XMLparserTag* firstTagInLinkingPhraseAppearanceList)
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


	currentTagL1 = addToCXLconnectionNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDinCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLconnectionNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDinCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLconnectionNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDinCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLconnectionNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDinCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);

	return result;
}

#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
bool checkIfPassedRedundantConceptNodeRemoval(GIAentityNode* currentEntity)
{
	bool result = false;

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
	if(currentEntity->associatedInstanceNodeList->empty())
	{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (substance node)
	//if((currentEntity->isAction) || (currentEntity->isSubstance) || (currentEntity->isCondition))
	//{//do not add raw concept nodes
		result = true;
	}
	#else

	if(currentEntity->associatedInstanceNodeList->empty())
	{
		//entity does not have an associated instance [entity is either a raw/isolated concept node, or it is a substance]
		result = true;
	}
	else
	{
		//entity has an associated instance [entity is a concept node]

		if(!(currentEntity->actionSubjectEntity->empty()))
		{
			result = true;
		}
		if(!(currentEntity->actionObjectEntity->empty()))
		{
			result = true;
		}

		if(!(currentEntity->conditionSubjectEntity->empty()))
		{
			result = true;
		}
		if(!(currentEntity->conditionObjectEntity->empty()))
		{
			result = true;
		}

		if(!(currentEntity->entityNodeDefiningThisInstance->empty()))
		{//this shouldnt be defined, since this entity is a concept node, not a substance node
			result = true;
		}


		if(!(currentEntity->actionNodeList->empty()))
		{
			result = true;
		}
		if(!(currentEntity->incomingActionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->conditionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->incomingConditionNodeList->empty()))
		{
			result = true;
		}


		if(!(currentEntity->propertyNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->propertyNodeReverseList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->entityNodeDefinitionList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->entityNodeDefinitionReverseList->empty()))
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

XMLparserTag* addToCXLconnectionNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagL1, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList)
{
	bool result = true;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;


		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		if(checkIfPassedRedundantConceptNodeRemoval(currentEntity))
		{
		#endif

			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				if(entityVectorConnectionCXLwriteArray[i])
				{
					#ifdef GIA_CMAP_CONVERSION_SANITISED
					//directly link action/condition subject/objects together (like in a standard/CMapTools concept map)
					if(((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT)) || ((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT)))
					{
						if((currentEntity->isAction) || (currentEntity->isCondition))
						{
							vector<GIAentityConnectionClass*>* entitySubjectVectorConnection;
							vector<GIAentityConnectionClass*>* entityObjectVectorConnection;
							if(currentEntity->isAction)
							{
								entitySubjectVectorConnection = currentEntity->actionSubjectEntity;
								entityObjectVectorConnection = currentEntity->actionObjectEntity;
							}
							else if(currentEntity->isCondition)
							{
								entitySubjectVectorConnection = currentEntity->conditionSubjectEntity;
								entityObjectVectorConnection = currentEntity->conditionObjectEntity;
							}

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
							GIAentityConnectionClass* entitySubjectConnection = entitySubjectVectorConnection->begin();
							GIAentityConnectionClass* entityObjectConnection = entityObjectVectorConnection->begin();


							string connectionTypeName = currentEntity->entityName;
							#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
							cout << "connectionTypeName = " << connectionTypeName << endl;
							#endif
							long linkingPhraseID = currentEntity->idActiveListReorderdIDforXMLsave;
							if(subjectIsEmpty)
							{
								entitySubjectConnection->entity = new GIAentityNode();
								entitySubjectConnection->entity->CXLdummyNode = true;
								#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
								cout << "entitySubjectConnection->entity == NULL" << endl;
								#endif
							}
							if(objectIsEmpty)
							{
								entityObjectConnection->entity = new GIAentityNode();
								entityObjectConnection->entity->CXLdummyNode = true;
								#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
								cout << "entityObjectConnection->entity == NULL" << endl;
								#endif
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
								#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
								if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
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
				#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
				fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave;
				#elif defined GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
				fakeTimeEntity.idActiveListReorderdIDforXMLsave = currentEntity->timeConditionNode->idActiveList;
				#else
				fakeTimeEntity.idActiveList = currentEntity->timeConditionNode->idActiveList;
				#endif

				string connectionTypeName = "time";
				currentTagL1 = generateCXLconnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, &fakeTimeEntity, connectionTypeName, currentCmapNodeIDinCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
			}
			#endif

		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		}
		#endif
	}

	return currentTagL1;
}

XMLparserTag* generateCXLconnectionNodeTagAndLinkingPhraseTags(XMLparserTag* currentTagL1, GIAentityNode* entity1, GIAentityNode* entity2, string connectionTypeName, long* currentCmapNodeIDinCmapNodeList, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList, bool alreadyMadeLinkingPhrase)
{
	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;
	#elif defined GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "entity1->idActiveListReorderdIDforXMLsave = " << entity1->idActiveListReorderdIDforXMLsave << endl;
	cout << "entity2->idActiveListReorderdIDforXMLsave = " << entity2->idActiveListReorderdIDforXMLsave << endl;
	#endif
	int entity1ID = entity1->idActiveListReorderdIDforXMLsave;
	int entity2ID = entity2->idActiveListReorderdIDforXMLsave;
	#else
	int entity1ID = entity1->idActiveList;
	int entity2ID = entity2->idActiveList;
	#endif

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "entity1ID = " << entity1ID << endl;
	cout << "entity2ID = " << entity2ID << endl;
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
		#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
		cout << "currentTagL1->name = " << currentTagL1->name << endl;
		cout << "entity1ID = " << entity1ID << endl;
		cout << "connectionID = " << connectionID << endl;
		#endif
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


XMLparserTag* generateCXLconnectionNodeTag(XMLparserTag* currentTagL1, long fromID, long toID)
{
	bool result = true;

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "fromID = " << fromID << endl;
	cout << "toID = " << toID << endl;
	#endif

	//generate neuron connection tag
	currentTagL1->name = NET_CXL_TAG_connection;

	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_CXL_TAG_fromid;
	currentAttribute->value = convertLongToString(fromID);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_CXL_TAG_toid;
	currentAttribute->value = convertLongToString(toID);
	currentAttribute = createNewAttribute(currentAttribute);

	XMLparserTag* newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}



#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
//this method is insufficient due to time condition nodes
void resetIDsForNodeList2(vector<GIAentityNode*>* entityNodesList, long* currentEntityNodeIDinConceptEntityNodesList)
{
	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;
		currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDinConceptEntityNodesList;
		(*currentEntityNodeIDinConceptEntityNodesList) = (*currentEntityNodeIDinConceptEntityNodesList) + 1;
	}
}
#endif



