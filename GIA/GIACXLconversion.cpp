/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIACXLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q4b 14-October-2012
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 *
 *******************************************************************************/



#include "GIACXLconversion.h"

bool writeCMapToolsCXLFileOptimised(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
	vector<GIAEntityNode*> * entityNodesActiveListConcepts = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		entityNodesActiveListConcepts->push_back(entityNode);
	}

	bool result;
	result = writeCMapToolsCXLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);
	
	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif
	
	return result;
}

bool writeCMapToolsCXLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
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


	#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
	currentCmapNodeIDInCmapNodeList = 0;
	resetIDsForNodeList2(entityNodesActiveListConcepts, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListSubstances, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListActions, &currentCmapNodeIDInCmapNodeList);
	resetIDsForNodeList2(entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList);
	#endif


	currentCmapNodeIDInCmapNodeList = 0;
	long firstCmapNodeIDInConceptList = currentCmapNodeIDInCmapNodeList;
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList, true, false))
	{
		result = false;
	}


	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	long firstCmapNodeIDInLinkingPhraseList = currentCmapNodeIDInCmapNodeList;	//NOT NECESSARY: redundant
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList, false, false))
	{
		result = false;

	}
	XMLParserTag * firstTagInLinkingPhraseList = currentTagL2->firstLowerLevelTag;


	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	currentCmapNodeIDInCmapNodeList = firstCmapNodeIDInConceptList;
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList, true, true))
	{
		result = false;
	}

	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;


	currentCmapNodeIDInCmapNodeList = firstCmapNodeIDInLinkingPhraseList;	//NOT NECESSARY: redundant
	if(!generateCXLEntityNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList, false, true))
	{
		result = false;
	}
	XMLParserTag * firstTagInLinkingPhraseAppearanceList = currentTagL2->firstLowerLevelTag;


	currentTagL2 = currentTagL2->nextTag;
	newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;


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

	if(!generateCXLConnectionNodeTagList(currentTagL2, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, &currentCmapNodeIDInCmapNodeList, lastTagInLinkingPhraseList, lastTagInLinkingPhraseAppearanceList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "DEBUG generateXMLTagListBasedUponSubnet done " << endl;
	#endif
	
 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	#ifdef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif
	
	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "DEBUG writeXMLFile done " << endl;
	#endif
	
	return result;
}

//generateCXLEntityNodeTagList(): generates and fills <concept-list>, <concept-appearance-list>, <linking-phrase-list> and <linking-phrase-appearance-list> tags.
bool generateCXLEntityNodeTagList(XMLParserTag * firstTagInMap, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, long * currentCmapNodeIDInCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
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
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	*/

	#ifdef GIA_CMAP_CONVERSION_SANITISED
	if(conceptOrLinkingPhraseList)
	{
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	}
	else
	{
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing
		currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);	//required for timeCondition node printing
	}
	#else
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	currentTagL1 = addToCXLEntityNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDInCmapNodeList, conceptOrLinkingPhraseList, appearanceList);
	#endif


	return result;
}


XMLParserTag * addToCXLEntityNodeTagList(XMLParserTag * currentTagL1, vector<GIAEntityNode*> *entityNodesList, long * currentCmapNodeIDInCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAEntityNode * currentEntity = *entityNodesActiveCompleteListIterator;

		#ifndef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
		if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
		{//this exception is required because timeCondition nodes have a separate idActiveList in Cmap maps / CXL
			currentEntity->timeConditionNode->idActiveListReorderdIDforXMLsave = (*currentCmapNodeIDInCmapNodeList);
			currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->timeConditionNode->conditionName, *currentCmapNodeIDInCmapNodeList, currentEntity->timeConditionNode->printX, currentEntity->timeConditionNode->printY, conceptOrLinkingPhraseList, appearanceList);
			(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
		}
		#endif

		if(!(currentEntity->disabled))
		{
			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			if(currentEntity->AssociatedInstanceNodeList->empty())
			{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (substance node)
			#endif
				#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
				cout << "DEBUG: currentEntity->entityName = " << currentEntity->entityName << endl;
				#endif
				
				#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ
				currentEntity->idActiveListReorderdIDforXMLsave = *currentCmapNodeIDInCmapNodeList;
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

				currentTagL1 = generateCXLEntityNodeTag(currentTagL1, currentEntity->entityName, currentEntityID, currentEntity->printX, currentEntity->printY, conceptOrLinkingPhraseList, appearanceList);
				(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;

			#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
			}
			#endif
		}
	}

	return currentTagL1;
}

XMLParserTag * generateCXLEntityNodeTag(XMLParserTag * currentTagL1, string entityName, long entityID, int printX, int printY, bool conceptOrLinkingPhraseList, bool appearanceList)
{
	bool result = true;

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

	XMLParserTag * newTag = new XMLParserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}

//generateCXLConnectionNodeTagList(): generates <connection-list> tag, and fills <linking-phrase-list>, and  <linking-phrase-appearance-list> tags
bool generateCXLConnectionNodeTagList(XMLParserTag * firstTagInMap, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, long * currentCmapNodeIDInCmapNodeList, XMLParserTag * firstTagInLinkingPhraseList, XMLParserTag * firstTagInLinkingPhraseAppearanceList)
{
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


	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, entityNodesActiveListConcepts, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, entityNodesActiveListSubstances, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, entityNodesActiveListActions, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);
	currentTagL1 = addToCXLConnectionNodeTagList(currentTagL1, entityNodesActiveListConditions, currentCmapNodeIDInCmapNodeList, firstTagL1, &currentTagInLinkingPhraseList, &currentTagInLinkingPhraseAppearanceList, firstTagL1);

	return result;
}

#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
bool checkIfPassedRedundantConceptNodeRemoval(GIAEntityNode * currentEntity)
{
	bool result = false;

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
	if(currentEntity->AssociatedInstanceNodeList->empty())
	{//if GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES; then do not add a concept entity if it has an associated instance (substance node)
	//if((currentEntity->isAction) || (currentEntity->isSubstance) || (currentEntity->isCondition))
	//{//do not add raw concept nodes
		result = true;
	}
	#else

	if(currentEntity->AssociatedInstanceNodeList->empty())
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


		if(!(currentEntity->ActionNodeList->empty()))
		{
			result = true;
		}
		if(!(currentEntity->IncomingActionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->ConditionNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->IncomingConditionNodeList->empty()))
		{
			result = true;
		}


		if(!(currentEntity->PropertyNodeList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->PropertyNodeReverseList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->EntityNodeDefinitionList->empty()))
		{
			result = true;
		}

		if(!(currentEntity->EntityNodeDefinitionReverseList->empty()))
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
	bool result = true;

	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAEntityNode * currentEntity = *entityNodesActiveCompleteListIterator;


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
							vector<GIAEntityConnectionClass*> * entitySubjectVectorConnection;
							vector<GIAEntityConnectionClass*> * entityObjectVectorConnection;
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
							GIAEntityConnectionClass * entitySubjectConnection = entitySubjectVectorConnection->begin();
							GIAEntityConnectionClass * entityObjectConnection = entityObjectVectorConnection->begin();


							string connectionTypeName = currentEntity->entityName;
							#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
							cout << "connectionTypeName = " << connectionTypeName << endl;
							#endif
							long linkingPhraseID = currentEntity->idActiveListReorderdIDforXMLsave;
							if(subjectIsEmpty)
							{
								entitySubjectConnection->entity = new GIAEntityNode();
								entitySubjectConnection->entity->CXLdummyNode = true;
								#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
								cout << "entitySubjectConnection->entity == NULL" << endl;
								#endif
							}
							if(objectIsEmpty)
							{
								entityObjectConnection->entity = new GIAEntityNode();
								entityObjectConnection->entity->CXLdummyNode = true;
								#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
								cout << "entityObjectConnection->entity == NULL" << endl;
								#endif
							}
							if(!(subjectIsEmpty && objectIsEmpty))
							{
								currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, entitySubjectEntity, entityObjectEntity, connectionTypeName, &linkingPhraseID, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, true);
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
							for(vector<GIAEntityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[i].begin(); connectionIter < currentEntity->entityVectorConnectionsArray[i].end(); connectionIter++)
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
									GIAEntityNode * entity = (*connectionIter)->entity;
									string connectionTypeName = entityVectorConnectionDrawConnectionNameArray[i];
									currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, entity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
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
				GIAEntityNode fakeTimeEntity;
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
				currentTagL1 = generateCXLConnectionNodeTagAndLinkingPhraseTags(currentTagL1, currentEntity, &fakeTimeEntity, connectionTypeName, currentCmapNodeIDInCmapNodeList, currentTagInLinkingPhraseList, currentTagInLinkingPhraseAppearanceList, firstTagInConnectionsList, false);
			}
			#endif

		#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		}
		#endif
	}

	return currentTagL1;
}

XMLParserTag * generateCXLConnectionNodeTagAndLinkingPhraseTags(XMLParserTag * currentTagL1, GIAEntityNode * entity1, GIAEntityNode * entity2, string connectionTypeName, long * currentCmapNodeIDInCmapNodeList, XMLParserTag ** currentTagInLinkingPhraseList, XMLParserTag ** currentTagInLinkingPhraseAppearanceList, XMLParserTag * firstTagInConnectionsList, bool alreadyMadeLinkingPhrase)
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
	int connectionID = (*currentCmapNodeIDInCmapNodeList);


	//verify connection is not already found;
	bool alreadyMadeConnection = false;
	
	/*OLD:
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

	if(!alreadyMadeConnection)
	{
		#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
		cout << "currentTagL1->name = " << currentTagL1->name << endl;
		cout << "entity1ID = " << entity1ID << endl;
		cout << "connectionID = " << connectionID << endl;
		#endif
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
			*currentTagInLinkingPhraseList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, false);
			*currentTagInLinkingPhraseAppearanceList = generateCXLEntityNodeTag(*currentTagInLinkingPhraseAppearanceList, connectionTypeName, connectionID, connectionTypeX, connectionTypeY, false, true);
			(*currentCmapNodeIDInCmapNodeList) = (*currentCmapNodeIDInCmapNodeList) + 1;
		}
	}

	return currentTagL1;
}


XMLParserTag * generateCXLConnectionNodeTag(XMLParserTag * currentTagL1, long fromID, long toID)
{
	bool result = true;

	#ifdef GIA_SEMANTIC_NET_CXL_DEBUG
	cout << "fromID = " << fromID << endl;
	cout << "toID = " << toID << endl;
	#endif

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



#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
//this method is insufficient due to time condition nodes
void resetIDsForNodeList2(vector<GIAEntityNode*> *entityNodesList, long * currentEntityNodeIDInConceptEntityNodesList)
{
	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAEntityNode * currentEntity = *entityNodesActiveCompleteListIterator;
		currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDInConceptEntityNodesList;
		(*currentEntityNodeIDInConceptEntityNodesList) = (*currentEntityNodeIDInConceptEntityNodesList) + 1;
	}
}
#endif



