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
 * File Name: GIAXMLconversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1p2b 13-September-2012
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 * NB this function creates entity idActiveList values upon read (it could create idActiveListReorderdIDforXMLsave values instead - however currently it is assumed that when an XML file is loaded, this will populate the idActiveList in its entirety)
 *
 *******************************************************************************/



#include "GIAXMLconversion.h"
#include "GIAdatabase.h"

//this function works and no longer needs to be tested
#ifdef GIA_XML_DEBUG
bool testReadSemanticNetXMLFile1()
{
	bool result = true;

	vector<GIAEntityNode*> *entityNodesActiveListComplete;	//the entityNodesActiveListComplete object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,
	vector<GIAEntityNode*> *entityNodesActiveListConcepts;
	vector<GIAEntityNode*> *entityNodesActiveListSubstances;
	vector<GIAEntityNode*> *entityNodesActiveListActions;
	vector<GIAEntityNode*> *entityNodesActiveListConditions;

	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	return result;
}

bool testReadSemanticNetXMLFile2(vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
	bool result = true;

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	vector<GIAEntityNode*> tempentityNodesActiveListComplete;
	vector<GIAEntityNode*> tempentityNodesActiveListConcepts;
	vector<GIAEntityNode*> tempentityNodesActiveListSubstances;
	vector<GIAEntityNode*> tempentityNodesActiveListActions;
	vector<GIAEntityNode*> tempentityNodesActiveListConditions;

	if(!readSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, &tempentityNodesActiveListComplete, &tempentityNodesActiveListConcepts, &tempentityNodesActiveListSubstances, &tempentityNodesActiveListActions, &tempentityNodesActiveListConditions))
	{
		result = false;
	}

	//cout << "here3" << endl;
	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, &tempentityNodesActiveListComplete, &tempentityNodesActiveListConcepts, &tempentityNodesActiveListSubstances, &tempentityNodesActiveListActions, &tempentityNodesActiveListConditions))
	{
		result = false;
	}
	return result;
}
#endif

bool readSemanticNetXMLFileOptimised(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
	bool result = false;

	vector<GIAEntityNode*> *entityNodesActiveListConcepts = new vector<GIAEntityNode*>;
	readSemanticNetXMLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);

	//now convert entityNodesActiveListComplete to entityNodesCompleteListMap;
	long vectorSize = entityNodesActiveListConcepts->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{
		GIAEntityNode * entityNode = entityNodesActiveListConcepts->at(entityIndex);
		//entityNodesActiveListConcepts[entityNodeName] = entityNodeFound;
		conceptEntityNodesListMap->insert(pair<string, GIAEntityNode*>(entityNode->entityName, entityNode));
	}
		//cout << "c" << endl;

	return result;
}


bool readSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in GIAXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

		//cout << "a" << endl;

 	if(!readXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}
		//cout << "b" << endl;
	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, false))
	{
		result = false;
	}
		//cout << "b2" << endl;
	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, true))
	{
		result = false;
	}
		//cout << "c" << endl;

	return result;
}

//Top Level
bool parseSemanticNetTag(XMLParserTag * firstTagInNetwork, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, bool linkConnections)
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
			long currentEntityNodeIDInEntityNodesActiveCompleteList = 0;

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

						GIAEntityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
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
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;

						bool foundTemp = false;
						long indexTemp = 0;

						/*
						string conceptEntityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value;
						GIAEntityNode * tempNode = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, &conceptEntityName, &foundTemp, &indexTemp, true, &currentEntityNodeIDInCompleteList, &currentEntityNodeIDInEntityNodesActiveCompleteList);
						*/


						entityNodesActiveListConcepts->push_back(newEntity);
						currentEntityNodeIDInEntityNodesActiveCompleteList++;

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
								cout << "error: concept entityTag does not have idActiveList as first attribute" << endl;
								exit(0);
							}
						}
						else
						{
							cout << "error: concept entityTag does not have ID" << endl;
							exit(0);
						}
						#endif

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Concept Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						//conceptEntityNamesList->push_back(currentTagUpdatedL3->firstAttribute->nextAttribute->value);


						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next;
						*/
					}
				}
				else
				{
					if(currentTagUpdatedL3->name != "")
					{//NB concept entities nodes should always exist, so this condition is not required (at the moment)
						cout << "parseSemanticNetTag error 1: entity node tag not detected" << endl;
						//cout << "tag found: " << currentTagUpdatedL3->name << endl;
						//cout << "tag expected: " << NET_XML_TAG_entityNode << endl;
					}
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			cout << "parseSemanticNetTag error: no concept entity container node tag detected" << endl;
		}

		if(currentTagUpdatedL2->name == NET_XML_TAG_substanceEntityNodeContainer)
		{
			long currentEntityNodeIDInSubstanceEntityNodesList = 0;

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

						GIAEntityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
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
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						entityNodesActiveListSubstances->push_back(newEntity);
						currentEntityNodeIDInSubstanceEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Substance Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif
						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next;
						*/
					}
				}
				else
				{
					if(currentTagUpdatedL2->name != "")
					{
						cout << "parseSemanticNetTag error 2: entity node tag not detected" << endl;
						//cout << "tag found: " << currentTagUpdatedL3->name << endl;
						//cout << "tag expected: " << NET_XML_TAG_entityNode << endl;
					}
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			cout << "parseSemanticNetTag error: no substance entity container node tag detected" << endl;
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

						GIAEntityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
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
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						entityNodesActiveListActions->push_back(newEntity);
						currentEntityNodeIDInActionEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Action Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next;
						*/
					}
				}
				else
				{
					if(currentTagUpdatedL3->name != "")
					{
						cout << "parseSemanticNetTag error 3: entity node tag not detected" << endl;
						//cout << "tag found: " << currentTagUpdatedL3->name << endl;
						//cout << "tag expected: " << NET_XML_TAG_entityNode << endl;
					}
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			cout << "parseSemanticNetTag error: no action entity container node tag detected" << endl;
		}

		if(currentTagUpdatedL2->name == NET_XML_TAG_conditionEntityNodeContainer)
		{
			long currentEntityNodeIDInConditionEntityNodesList = 0;

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

						GIAEntityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDInCompleteList);
						currentEntityNodeIDInCompleteList++;
						//cout << "as1" << endl;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
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
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDInCompleteList++;
						entityNodesActiveListConditions->push_back(newEntity);
						currentEntityNodeIDInConditionEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Condition Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						/*
						GIAEntityNode * newEntity = new GIAEntityNode();
						currentEntity->next = newEntity;
						currentEntity = currentEntity->next;
						*/
					}
				}
				else
				{
					if(currentTagUpdatedL3->name != "")
					{
						cout << "parseSemanticNetTag error 4: entity node tag not detected" << endl;
						//cout << "tag found: " << currentTagUpdatedL3->name << endl;
						//cout << "tag expected: " << NET_XML_TAG_entityNode << endl;
					}
				}
				currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
		}
		else
		{
			cout << "parseSemanticNetTag error: no action entity container node tag detected" << endl;
		}
	}

	return result;
}



bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesActiveListComplete)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = firstTagInEntityNode->firstAttribute;

	bool idFound = false;
	bool entityNameFound = false;
	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	bool wordOrigFound = false;
	#endif
	#ifdef GIA_SUPPORT_ALIASES
	bool aliasesFound = false;
	#endif
	bool confidenceFound = false;
	bool isConceptFound = false;
	bool isSubstanceFound = false;
	bool isActionFound = false;
	bool isConditionFound = false;

	bool conditionTypeFound = false;

	bool hasAssociatedSubstanceFound = false;
	bool hasAssociatedSubstanceIsActionFound = false;
	bool hasAssociatedSubstanceIsConditionFound = false;
	bool hasAssociatedTimeFound = false;
	bool hasQualityFound = false;
	bool disabledFound = false;

	bool grammaticalNumberFound = false;
	bool hasQuantityFound = false;
	bool quantityNumberFound = false;
	bool quantityNumberStringFound = false;
	bool quantityModifierFound = false;
	bool quantityModifierStringFound = false;
	bool hasQuantityMultiplierFound = false;
	bool hasMeasureFound = false;
	bool measureTypeFound = false;

	bool printXFound = false;
	bool printYFound = false;
	bool printXIndexFound = false;
	bool printYIndexFound = false;
	bool printTextXFound = false;
	bool printTextYFound = false;

	bool entityVectorConnectionNodeFoundArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];
	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		entityVectorConnectionNodeFoundArray[i] = false;
	}

	bool timeConditionNodeFound = false;

	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	//parseEntityNodeTag->... = 0;

	while(currentAttribute->nextAttribute != NULL)
	{
		//cout << "df1" << endl;

		if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
		{
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->idActiveList = attributeValue;
			idFound = true;
			//cout << "entityNode->idActiveList = " << entityNode->idActiveList << endl;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityName)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->entityName = attributeValue;
			entityNameFound = true;
		}
		#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_wordOrig)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->wordOrig = attributeValue;
			wordOrigFound = true;
		}
		#endif
		#ifdef GIA_SUPPORT_ALIASES
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_aliases)
		{
			string attributeValue = currentAttribute->value.c_str();
			convertAliasesStringToAliases(entityNode, attributeValue);
			aliasesFound = true;
		}
		#endif
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_confidence)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			entityNode->confidence = attributeValue;
			confidenceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isConcept)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isConcept = attributeValue;
			isConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstance)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isSubstance = attributeValue;
			isSubstanceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isAction)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isAction = attributeValue;
			isActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isCondition)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isCondition = attributeValue;
			isConditionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedSubstance)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedInstance = attributeValue;
			hasAssociatedSubstanceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedSubstanceIsAction)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedInstanceIsAction = attributeValue;
			hasAssociatedSubstanceIsActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedSubstanceIsCondition)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedInstanceIsCondition = attributeValue;
			hasAssociatedSubstanceIsConditionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedTime)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasAssociatedTime = attributeValue;
			hasAssociatedTimeFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasQuality)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isSubstanceQuality = attributeValue;
			hasQualityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_disabled)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->disabled = attributeValue;
			disabledFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->conditionType = attributeValue;
			conditionTypeFound = true;
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
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityNumberString)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->quantityNumberString = attributeValue;
			quantityNumberStringFound = true;
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
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasQuantityMultiplier)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->hasQuantityMultiplier = attributeValue;
			hasQuantityMultiplierFound = true;
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
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			if(currentTagUpdatedL3->name == entityVectorConnectionXMLTagNameArray[i])
			{
				if(!parseEntityVectorConnectionNodeListTag(currentTagUpdatedL3->firstLowerLevelTag, entityNode, entityNodesActiveListComplete, i))
				{
					result = false;
				}
				entityVectorConnectionNodeFoundArray[i] = true;
			}
		}

		if(currentTagUpdatedL3->name == NET_XML_TAG_timeConditionNode)
		{
			//cout << "timeConditionNode: " << endl;
			entityNode->timeConditionNode = new GIATimeConditionNode();
			if(!parseTimeConditionNodeTag(currentTagUpdatedL3, entityNode->timeConditionNode))
			{
				result = false;
			}
			//cout << "END timeConditionNode: " << endl;
			timeConditionNodeFound = true;
		}

		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}

	//cout << "df5" << endl;

	if(isSubstanceFound && entityNode->isSubstance)
	{
		/*
		if(!entityNodeContainingThisSubstanceFound)
		{
			result = false;
		}
		*/
		#ifndef GIA_TEMPORARILY_DISABLE_GIA_XML_READ_CHECKS
		if(!entityVectorConnectionNodeFoundArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE])
		{
			cout << "parseEntityNodeTag error: isSubstanceFound && entityNode->isSubstance && !entityNodeDefiningThisSubstanceFound" << endl;
			result = false;
		}
		#endif
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


bool parseEntityVectorConnectionNodeListTag(XMLParserTag * firstTagInEntityVectorConnectionNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesActiveListComplete, int entityVectorConnectionIndex)
{
	bool result = true;
	XMLParserTag * currentTagUpdatedL1 = firstTagInEntityVectorConnectionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == entityVectorConnectionXMLTagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex])
		{
			//cout <<  entityVectorConnectionXMLTagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " nodeReference: " << endl;
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			long idActiveList = attributeValue;
			GIAEntityNode * entity = findActiveEntityNodeByID(idActiveList, entityNodesActiveListComplete);
			GIAEntityConnection * newConnection = new GIAEntityConnection();
			newConnection->entity = entity;
			#ifdef GIA_USE_DATABASE
			newConnection->referenceLoaded = true;
			newConnection->entityName = entity->entityName;
			newConnection->idInstance = entity->idInstance;
			newConnection->loaded = true;
			newConnection->modified = false;
			newConnection->added = true;
			#endif
			entityNode->entityVectorConnectionsArray[entityVectorConnectionIndex].push_back(newConnection);
			//cout << "attributeValue = " << attributeValue << endl;
			//cout << "refID = " << findActiveEntityNodeByID(attributeValue, entityNodesActiveListComplete)->idActiveList << endl;
		}
		else
		{
			cout << "parse " << entityVectorConnectionXMLTagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " error: tag not found" << endl;
		}
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
	return result;
}

bool parseTimeConditionNodeTag(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

	XMLParserAttribute * currentAttribute = firstTagInTimeConditionNode->firstAttribute;

	bool conditionName = false;
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
		if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionName)
		{
			timeConditionNode->conditionName = currentAttribute->value;
			conditionName = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_tense)
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
	return result;
}
















bool writeSemanticNetXMLFileOptimised(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
{
	vector<GIAEntityNode*> * entityNodesActiveListConcepts = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		entityNodesActiveListConcepts->push_back(entityNode);
	}

	bool result;
	result = writeSemanticNetXMLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);
	return result;
}


bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesActiveListComplete, vector<GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions)
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


	long currentEntityNodeIDInEntityNodesActiveCompleteList;

	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
	currentEntityNodeIDInEntityNodesActiveCompleteList = 0;
	resetIDsForNodeList(entityNodesActiveListConcepts, &currentEntityNodeIDInEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListSubstances, &currentEntityNodeIDInEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListActions, &currentEntityNodeIDInEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListConditions, &currentEntityNodeIDInEntityNodesActiveCompleteList);
	#endif

	currentEntityNodeIDInEntityNodesActiveCompleteList = 0;

	if(!generateXMLEntityNodeTagList(currentTagL1, entityNodesActiveListConcepts, NET_XML_TAG_conceptEntityNodeContainer, &currentEntityNodeIDInEntityNodesActiveCompleteList))
	{
		result = false;
	}

	//cout << "a" << endl;

	currentTagL1 = currentTagL1->nextTag;
	XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag2;

	if(!generateXMLEntityNodeTagList(currentTagL1, entityNodesActiveListSubstances, NET_XML_TAG_substanceEntityNodeContainer, &currentEntityNodeIDInEntityNodesActiveCompleteList))
	{
		result = false;
	}

	//cout << "b" << endl;

	currentTagL1 = currentTagL1->nextTag;
	XMLParserTag * newTag3 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag3;

	if(!generateXMLEntityNodeTagList(currentTagL1, entityNodesActiveListActions, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDInEntityNodesActiveCompleteList))
	{
		result = false;
	}

	//cout << "c" << endl;

	currentTagL1 = currentTagL1->nextTag;
	XMLParserTag * newTag4 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag4;

	if(!generateXMLEntityNodeTagList(currentTagL1, entityNodesActiveListConditions, NET_XML_TAG_conditionEntityNodeContainer, &currentEntityNodeIDInEntityNodesActiveCompleteList))
	{
		result = false;
	}

	//cout << "d" << endl;


	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;


	return result;
}







#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
void resetIDsForNodeList(vector<GIAEntityNode*> *entityNodesList, long * currentEntityNodeIDInEntityNodesActiveCompleteList)
{
	//cout << "h3" << endl;

	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAEntityNode * currentEntity = (*entityNodesActiveCompleteListIterator);
		currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDInEntityNodesActiveCompleteList;
		(*currentEntityNodeIDInEntityNodesActiveCompleteList) = (*currentEntityNodeIDInEntityNodesActiveCompleteList) + 1;

		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;
}
#endif


bool generateXMLEntityNodeTagList(XMLParserTag * firstTagInSemanticNet, vector<GIAEntityNode*> *entityNodesList, string entityContainerTagName, long * currentEntityNodeIDInEntityNodesActiveCompleteList)
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

	for(vector<GIAEntityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAEntityNode * currentEntity = *entityNodesActiveCompleteListIterator;

		//if(!(currentEntity->disabled))
		//{//tested 11 Feb 2012; failed
		currentTagL1 = generateXMLEntityNodeTag(currentTagL1, currentEntity, *currentEntityNodeIDInEntityNodesActiveCompleteList);

		(*currentEntityNodeIDInEntityNodesActiveCompleteList) = (*currentEntityNodeIDInEntityNodesActiveCompleteList) + 1;
		//}

		//cout << "h5f" << endl;
	}

	//cout << "h6" << endl;

	return result;
}



XMLParserTag * generateXMLEntityNodeTag(XMLParserTag * currentTagL1, GIAEntityNode * currentEntity, long currentEntityNodeIDInEntityNodesActiveCompleteList)
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
	sprintf(tempString, "%ld", currentEntityNodeIDInEntityNodesActiveCompleteList);
	/*
	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
		//do not write current idActiveList value, write new idActiveList value - this enables fast linking of parsed xml file
		sprintf(tempString, "%ld", currentEntityNodeIDInEntityNodesActiveCompleteList);
	#else
		sprintf(tempString, "%ld", (currentEntity->idActiveList));
	#endif
	*/
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_entityName;
	currentAttribute->value = currentEntity->entityName;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
	currentAttribute->name = NET_XML_ATTRIBUTE_wordOrig;
	currentAttribute->value = currentEntity->wordOrig;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;
	#endif

	#ifdef GIA_SUPPORT_ALIASES
	currentAttribute->name = NET_XML_ATTRIBUTE_aliases;
	convertAliasesToAliasesString(currentEntity, &(currentAttribute->value));

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_confidence;
	sprintf(tempString, "%0.6f", (currentEntity->confidence));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_isConcept;
	sprintf(tempString, "%d", int(currentEntity->isConcept));
	currentAttribute->value = tempString;

	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstance;
	sprintf(tempString, "%d", int(currentEntity->isSubstance));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_isAction;
	sprintf(tempString, "%d", int(currentEntity->isAction));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_isCondition;
	sprintf(tempString, "%d", int(currentEntity->isCondition));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedSubstance;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedInstance));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedSubstanceIsAction;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedInstanceIsAction));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedSubstanceIsCondition;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedInstanceIsCondition));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedTime;
	sprintf(tempString, "%d", int(currentEntity->hasAssociatedTime));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasQuality;
	sprintf(tempString, "%d", int(currentEntity->isSubstanceQuality));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_disabled;
	sprintf(tempString, "%d", int(currentEntity->disabled));
	currentAttribute->value = tempString;

	//cout << "h5c2" << endl;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType;
	sprintf(tempString, "%d", currentEntity->conditionType);
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;



	//cout << "h5a" << endl;

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalNumber;
	sprintf(tempString, "%d", (currentEntity->grammaticalNumber));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantity;
	sprintf(tempString, "%d", int(currentEntity->hasQuantity));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasQuantity)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumber;
		sprintf(tempString, "%d", (currentEntity->quantityNumber));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumberString;
		currentAttribute->value = currentEntity->quantityNumberString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifier;
		sprintf(tempString, "%d", (currentEntity->quantityModifier));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifierString;
		currentAttribute->value = currentEntity->quantityModifierString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantityMultiplier;
		sprintf(tempString, "%d", int(currentEntity->hasQuantityMultiplier));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_hasMeasure;
	sprintf(tempString, "%d", int(currentEntity->hasMeasure));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasMeasure)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_measureType;
		sprintf(tempString, "%d", (currentEntity->measureType));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
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

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printY;
		sprintf(tempString, "%d", (currentEntity->printY));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printXIndex;
		sprintf(tempString, "%d", (currentEntity->printXIndex));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

		currentAttribute->name = NET_XML_ATTRIBUTE_printYIndex;
		sprintf(tempString, "%d", (currentEntity->printYIndex));
		currentAttribute->value = tempString;

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
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

		newAttribute = new XMLParserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	//cout << "h5c" << endl;




	//cout << "h5d" << endl;

	XMLParserTag * firstTagL3;
	XMLParserTag * currentTagL3;
	XMLParserTag * newTag2;

	for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
	{
		#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
		if(currentEntity->entityVectorConnectionsArray[i].begin() != currentEntity->entityVectorConnectionsArray[i].end())
		{
		#endif
			currentTagL2->name = entityVectorConnectionXMLTagNameArray[i];
			firstTagL3 = new XMLParserTag();
			currentTagL2->firstLowerLevelTag = firstTagL3;
			currentTagL3 = currentTagL2->firstLowerLevelTag;

			for(vector<GIAEntityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[i].begin(); connectionIter < currentEntity->entityVectorConnectionsArray[i].end(); connectionIter++)
			{
				currentTagL3->name = entityVectorConnectionXMLTagNameCrossReferenceNodeTypeArray[i];

				currentAttribute = currentTagL3->firstAttribute;

				currentAttribute->name = NET_XML_ATTRIBUTE_id;
				sprintf(tempString, "%ld", ((*connectionIter)->entity)->idActiveListReorderdIDforXMLsave);
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

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionName;
	currentAttribute->value = conditionTimeNode->conditionName;

	XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute1;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_tense;
	sprintf(tempString, "%d", (conditionTimeNode->tense));
	currentAttribute->value = tempString;

	XMLParserAttribute * newAttribute1b = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute1b;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_second;
	sprintf(tempString, "%0.6f", (conditionTimeNode->second));
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



