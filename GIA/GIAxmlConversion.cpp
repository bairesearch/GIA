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
 * File Name: GIAxmlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t7d 09-August-2013
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 * NB this function creates entity idActiveList values upon read (it could create idActiveListReorderdIDforXMLsave values instead - however currently it is assumed that when an XML file is loaded, this will populate the idActiveList in its entirety)
 *
 *******************************************************************************/



#include "GIAxmlConversion.h"
#include "GIAdatabase.h"

//this function works and no longer needs to be tested
#ifdef GIA_XML_DEBUG
bool testReadSemanticNetXMLFile1()
{
	bool result = true;

	vector<GIAentityNode*> *entityNodesActiveListComplete;	//the entityNodesActiveListComplete object must be initialised here (in GIAxmlConversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,
	vector<GIAentityNode*> *entityNodesActiveListConcepts;
	vector<GIAentityNode*> *entityNodesActiveListSubstances;
	vector<GIAentityNode*> *entityNodesActiveListActions;
	vector<GIAentityNode*> *entityNodesActiveListConditions;

	if(!readSemanticNetXMLfile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	return result;
}

bool testReadSemanticNetXMLFile2(vector<GIAentityNode*> *entityNodesActiveListComplete, vector<GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions)
{
	bool result = true;

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}

	vector<GIAentityNode*> tempentityNodesActiveListComplete;
	vector<GIAentityNode*> tempentityNodesActiveListConcepts;
	vector<GIAentityNode*> tempentityNodesActiveListSubstances;
	vector<GIAentityNode*> tempentityNodesActiveListActions;
	vector<GIAentityNode*> tempentityNodesActiveListConditions;

	if(!readSemanticNetXMLfile(GIA_SEMANTIC_NET_XML_FILE_NAME, &tempentityNodesActiveListComplete, &tempentityNodesActiveListConcepts, &tempentityNodesActiveListSubstances, &tempentityNodesActiveListActions, &tempentityNodesActiveListConditions))
	{
		result = false;
	}

	if(!writeSemanticNetXMLFile(GIA_SEMANTIC_NET_XML_FILE_NAME1, &tempentityNodesActiveListComplete, &tempentityNodesActiveListConcepts, &tempentityNodesActiveListSubstances, &tempentityNodesActiveListActions, &tempentityNodesActiveListConditions))
	{
		result = false;
	}
	return result;
}
#endif

bool readSemanticNetXMLfileOptimised(string xmlFileName, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions)
{
	bool result = false;

	vector<GIAentityNode*> *entityNodesActiveListConcepts = new vector<GIAentityNode*>;
	readSemanticNetXMLfile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);

	//now convert entityNodesActiveListComplete to entityNodesCompleteListMap;
	long vectorSize = entityNodesActiveListConcepts->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{
		GIAentityNode * entityNode = entityNodesActiveListConcepts->at(entityIndex);
		//entityNodesActiveListConcepts[entityNodeName] = entityNodeFound;
		conceptEntityNodesListMap->insert(pair<string, GIAentityNode*>(entityNode->entityName, entityNode));
	}
	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif
	
	return result;
}


bool readSemanticNetXMLfile(string xmlFileName, vector<GIAentityNode*> *entityNodesActiveListComplete, vector<GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions)
{
	bool result = true;

 	XMLparserTag * firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in GIAxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

 	if(!readXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}
	
	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, false))
	{
		result = false;
	}

	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, true))
	{
		result = false;
	}
	
	#ifdef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif

	return result;
}

//Top Level
bool parseSemanticNetTag(XMLparserTag * firstTagInNetwork, vector<GIAentityNode*> *entityNodesActiveListComplete, vector<GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, bool linkConnections)
{
	bool result = true;

	XMLparserTag * currentTagUpdatedL1 = firstTagInNetwork;

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
		long currentEntityNodeIDinCompleteList = 0;

		XMLparserTag * currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
		if(currentTagUpdatedL2->name == NET_XML_TAG_conceptEntityNodeContainer)
		{
			long currentEntityNodeIDinEntityNodesActiveCompleteList = 0;

			XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
					#endif

					if(linkConnections)
					{
						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						//cout << "currentEntityNodeIDinCompleteList = " << currentEntityNodeIDinCompleteList << endl;
						#endif
						
						GIAentityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode * newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;

						bool foundTemp = false;
						long indexTemp = 0;

						/*
						string conceptEntityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value;
						GIAentityNode * tempNode = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, &conceptEntityName, &foundTemp, &indexTemp, true, &currentEntityNodeIDinCompleteList, &currentEntityNodeIDinEntityNodesActiveCompleteList);
						*/


						entityNodesActiveListConcepts->push_back(newEntity);
						currentEntityNodeIDinEntityNodesActiveCompleteList++;

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
						GIAentityNode * newEntity = new GIAentityNode();
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

			XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
					#endif

					if(linkConnections)
					{
						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						//cout << "currentEntityNodeIDinCompleteList = " << currentEntityNodeIDinCompleteList << endl;
						#endif
						GIAentityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode * newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;
						entityNodesActiveListSubstances->push_back(newEntity);
						currentEntityNodeIDInSubstanceEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Substance Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif
						/*
						GIAentityNode * newEntity = new GIAentityNode();
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

			XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
					#endif

					if(linkConnections)
					{
						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						//cout << "currentEntityNodeIDinCompleteList = " << currentEntityNodeIDinCompleteList << endl;
						#endif
						
						GIAentityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode * newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;
						entityNodesActiveListActions->push_back(newEntity);
						currentEntityNodeIDInActionEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Action Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						/*
						GIAentityNode * newEntity = new GIAentityNode();
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

			XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
			while(currentTagUpdatedL3->nextTag != NULL)
			{
				if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
				{
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					if(currentTagUpdatedL3->firstAttribute->name == NET_XML_ATTRIBUTE_id)
					{
						//cout << "Entity Node ID: " << currentTagUpdatedL3->firstAttribute->value << endl;
					}
					#endif

					if(linkConnections)
					{
						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						//cout << "currentEntityNodeIDinCompleteList = " << currentEntityNodeIDinCompleteList << endl;
						#endif
						GIAentityNode * currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode * newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = atoi(currentTagUpdatedL3->firstAttribute->value.c_str());
						newEntity->idActiveList = attributeValue;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;
						entityNodesActiveListConditions->push_back(newEntity);
						currentEntityNodeIDInConditionEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Condition Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						/*
						GIAentityNode * newEntity = new GIAentityNode();
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



bool parseEntityNodeTag(XMLparserTag * firstTagInEntityNode, GIAentityNode * entityNode, vector<GIAentityNode*> *entityNodesActiveListComplete)
{
	bool result = true;

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
	bool isSubstanceQualityFound = false;
	bool isSubstanceConceptFound = false;
	bool isActionConceptFound = false;
	bool negativeFound = false;
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
		if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
		{
			long attributeValue = atol(currentAttribute->value.c_str());
			entityNode->idActiveList = attributeValue;
			idFound = true;
			#ifdef GIA_SEMANTIC_NET_XML_DEBUG
			//cout << "entityNode->idActiveList = " << entityNode->idActiveList << endl;
			#endif
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
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstanceQuality)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isSubstanceQuality = attributeValue;
			isSubstanceQualityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstanceConcept)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isSubstanceConcept = attributeValue;
			isSubstanceConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isActionConcept)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->isActionConcept = attributeValue;
			isActionConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_negative)
		{
			bool attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->negative = attributeValue;
			negativeFound = true;
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
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printY = attributeValue;
			printYFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printXIndex = attributeValue;
			printXIndexFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printYIndex)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printYIndex = attributeValue;
			printYIndexFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printTextX = attributeValue;
			printTextXFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = atoi(currentAttribute->value.c_str());
			entityNode->printTextY = attributeValue;
			printTextYFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}

		currentAttribute = currentAttribute->nextAttribute;
	}

	XMLparserTag * currentTagUpdatedL3 = firstTagInEntityNode->firstLowerLevelTag;
	while(currentTagUpdatedL3->nextTag != NULL)
	{
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			if(currentTagUpdatedL3->name == entityVectorConnectionXMLtagNameArray[i])
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
			#ifdef GIA_SEMANTIC_NET_XML_DEBUG
			//cout << "timeConditionNode: " << endl;
			#endif
			entityNode->timeConditionNode = new GIAtimeConditionNode();
			if(!parseTimeConditionNodeTag(currentTagUpdatedL3, entityNode->timeConditionNode))
			{
				result = false;
			}
			timeConditionNodeFound = true;
		}

		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}

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


bool parseEntityVectorConnectionNodeListTag(XMLparserTag * firstTagInEntityVectorConnectionNodeList, GIAentityNode * entityNode, vector<GIAentityNode*> *entityNodesActiveListComplete, int entityVectorConnectionIndex)
{
	bool result = true;
	XMLparserTag * currentTagUpdatedL1 = firstTagInEntityVectorConnectionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex])
		{
			#ifdef GIA_SEMANTIC_NET_XML_DEBUG
			//cout <<  entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " nodeReference: " << endl;
			#endif
			XMLParserAttribute * currentAttribute = currentTagUpdatedL1->firstAttribute;
			long attributeValue = atol(currentAttribute->value.c_str());
			long idActiveList = attributeValue;
			GIAentityNode * entity = findActiveEntityNodeByID(idActiveList, entityNodesActiveListComplete);
			GIAentityConnection * newConnection = new GIAentityConnection();
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
			#ifdef GIA_SEMANTIC_NET_XML_DEBUG
			//cout << "attributeValue = " << attributeValue << endl;
			//cout << "refID = " << findActiveEntityNodeByID(attributeValue, entityNodesActiveListComplete)->idActiveList << endl;
			#endif
		}
		else
		{
			cout << "parse " << entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " error: tag not found" << endl;
		}
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
	return result;
}

bool parseTimeConditionNodeTag(XMLparserTag * firstTagInTimeConditionNode, GIAtimeConditionNode * timeConditionNode)
{
	bool result = true;

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
















bool writeSemanticNetXMLFileOptimised(string xmlFileName, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *conceptEntityNodesListMap, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions)
{
	vector<GIAentityNode*> * entityNodesActiveListConcepts = new vector<GIAentityNode*>;
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListMapIter;
	for(conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListMapIter->second;
		entityNodesActiveListConcepts->push_back(entityNode);
	}

	bool result;
	result = writeSemanticNetXMLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);
	
	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif
	
	return result;
}


bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAentityNode*> *entityNodesActiveListComplete, vector<GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions)
{
	bool result = true;

 	XMLparserTag * firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

	XMLparserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_semanticNetwork;
	XMLparserTag * newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag * firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag * newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;


	long currentEntityNodeIDinEntityNodesActiveCompleteList;

	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
	currentEntityNodeIDinEntityNodesActiveCompleteList = 0;
	resetIDsForNodeList(entityNodesActiveListConcepts, &currentEntityNodeIDinEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListSubstances, &currentEntityNodeIDinEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListActions, &currentEntityNodeIDinEntityNodesActiveCompleteList);
	resetIDsForNodeList(entityNodesActiveListConditions, &currentEntityNodeIDinEntityNodesActiveCompleteList);
	#endif

	currentEntityNodeIDinEntityNodesActiveCompleteList = 0;

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListConcepts, NET_XML_TAG_conceptEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif
	
	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListConcepts, NET_XML_TAG_conceptEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, NET_XML_TAG_substanceEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif
	
	currentTagL1 = currentTagL1->nextTag;
	XMLparserTag * newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag2;

	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, NET_XML_TAG_substanceEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListActions, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif
	
	currentTagL1 = currentTagL1->nextTag;
	XMLparserTag * newTag3 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag3;

	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListActions, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, NET_XML_TAG_conditionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif
	
	currentTagL1 = currentTagL1->nextTag;
	XMLparserTag * newTag4 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag4;

	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, NET_XML_TAG_conditionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

 	if(!writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	#ifdef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif
	
	return result;
}







#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
void resetIDsForNodeList(vector<GIAentityNode*> *entityNodesList, long * currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode * currentEntity = (*entityNodesActiveCompleteListIterator);
		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		if(!(currentEntity->disabled))
		{
		#endif
			currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDinEntityNodesActiveCompleteList;
			(*currentEntityNodeIDinEntityNodesActiveCompleteList) = (*currentEntityNodeIDinEntityNodesActiveCompleteList) + 1;
		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}
}
#endif


bool generateXMLentityNodeTagList(XMLparserTag * firstTagInSemanticNet, vector<GIAentityNode*> *entityNodesList, string entityContainerTagName, long * currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	bool result = true;

	XMLparserTag * currentTagL0 = firstTagInSemanticNet;
	currentTagL0->name = entityContainerTagName;
	XMLparserTag * newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag * firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag * newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode * currentEntity = *entityNodesActiveCompleteListIterator;

		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		//tested 11 Feb 2012; failed
		if(!(currentEntity->disabled))
		{
		#endif
			currentTagL1 = generateXMLentityNodeTag(currentTagL1, currentEntity, *currentEntityNodeIDinEntityNodesActiveCompleteList);
			(*currentEntityNodeIDinEntityNodesActiveCompleteList) = (*currentEntityNodeIDinEntityNodesActiveCompleteList) + 1;
		
		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}

	return result;
}



XMLparserTag * generateXMLentityNodeTag(XMLparserTag * currentTagL1, GIAentityNode * currentEntity, long currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	bool result = true;

	char tempString[MAX_ATTRIBUTE_VALUE_SIZE];
	//generate neuron connection tag
	currentTagL1->name = NET_XML_TAG_entityNode;
	XMLparserTag * firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLParserAttribute * currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_id;
	sprintf(tempString, "%ld", currentEntityNodeIDinEntityNodesActiveCompleteList);
	/*
	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
		//do not write current idActiveList value, write new idActiveList value - this enables fast linking of parsed xml file
		sprintf(tempString, "%ld", currentEntityNodeIDinEntityNodesActiveCompleteList);
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

	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstanceQuality;
	sprintf(tempString, "%d", int(currentEntity->isSubstanceQuality));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;
	
	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstanceConcept;
	sprintf(tempString, "%d", int(currentEntity->isSubstanceConcept));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;	

	currentAttribute->name = NET_XML_ATTRIBUTE_isActionConcept;
	sprintf(tempString, "%d", int(currentEntity->isActionConcept));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;	

	currentAttribute->name = NET_XML_ATTRIBUTE_negative;
	sprintf(tempString, "%d", int(currentEntity->negative));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;	
		
	currentAttribute->name = NET_XML_ATTRIBUTE_disabled;
	sprintf(tempString, "%d", int(currentEntity->disabled));
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType;
	sprintf(tempString, "%d", currentEntity->conditionType);
	currentAttribute->value = tempString;

	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentAttribute = currentAttribute->nextAttribute;

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


	XMLparserTag * firstTagL3;
	XMLparserTag * currentTagL3;
	XMLparserTag * newTag2;

	#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES
	if(!(currentEntity->disabled))
	{
	#endif
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
			if(currentEntity->entityVectorConnectionsArray[i].begin() != currentEntity->entityVectorConnectionsArray[i].end())
			{
			#endif			
				currentTagL2->name = entityVectorConnectionXMLtagNameArray[i];
				firstTagL3 = new XMLparserTag();
				currentTagL2->firstLowerLevelTag = firstTagL3;
				currentTagL3 = currentTagL2->firstLowerLevelTag;

				for(vector<GIAentityConnection*>::iterator connectionIter = currentEntity->entityVectorConnectionsArray[i].begin(); connectionIter < currentEntity->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					GIAentityNode * connectionEntityNode = ((*connectionIter)->entity);
					#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
					if(!(connectionEntityNode->disabled))
					{
					#endif
						currentTagL3->name = entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[i];

						currentAttribute = currentTagL3->firstAttribute;

						currentAttribute->name = NET_XML_ATTRIBUTE_id;
						sprintf(tempString, "%ld", connectionEntityNode->idActiveListReorderdIDforXMLsave);
						currentAttribute->value = tempString;

						XMLParserAttribute * newAttribute = new XMLParserAttribute();
						currentAttribute->nextAttribute = newAttribute;
						currentAttribute = currentAttribute->nextAttribute;

						XMLparserTag * newTag3 = new XMLparserTag();	//had to add a null tag
						currentTagL3->nextTag = newTag3;
						currentTagL3 = currentTagL3->nextTag;
					#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
					}
					#endif					
				}
				newTag2 = new XMLparserTag();	//had to add a null tag
				currentTagL2->nextTag = newTag2;
				currentTagL2 = currentTagL2->nextTag;
			#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
			}
			#endif
		}
	#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES
	}
	#endif		

	if(currentEntity->conditionType == CONDITION_NODE_TYPE_TIME)
	{
		currentTagL2->name = NET_XML_TAG_timeConditionNode;

		/*
		XMLparserTag * firstTagL3 = new XMLparserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		XMLparserTag * currentTagL3 = currentTagL2->firstLowerLevelTag;
		*/

		if(!generateXMLconditionTimeNodeTagList(currentTagL2, currentEntity->timeConditionNode))
		{
			result = false;
		}

		XMLparserTag * newTag2 = new XMLparserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	}

	XMLparserTag * newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}








bool generateXMLconditionTimeNodeTagList(XMLparserTag * firstTagInConditionTimeNode, GIAtimeConditionNode * conditionTimeNode)
{
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



