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
 * File Name: GIAxmlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2l3b 15-October-2015
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

	vector<GIAentityNode*>* entityNodesActiveListComplete;	//the entityNodesActiveListComplete object must be initialised here (in GIAxmlConversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,
	vector<GIAentityNode*>* entityNodesActiveListConcepts;
	vector<GIAentityNode*>* entityNodesActiveListSubstances;
	vector<GIAentityNode*>* entityNodesActiveListActions;
	vector<GIAentityNode*>* entityNodesActiveListConditions;

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

bool testReadSemanticNetXMLFile2(vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
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

bool readSemanticNetXMLfileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	bool result = false;

	vector<GIAentityNode*>* entityNodesActiveListConcepts = new vector<GIAentityNode*>;
	readSemanticNetXMLfile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);

	//now convert entityNodesActiveListComplete to entityNodesCompleteListMap;
	long vectorSize = entityNodesActiveListConcepts->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{
		GIAentityNode* entityNode = entityNodesActiveListConcepts->at(entityIndex);
		//entityNodesActiveListConcepts[entityNodeName] = entityNodeFound;
		conceptEntityNodesListMap->insert(pair<string, GIAentityNode*>(entityNode->entityName, entityNode));
	}
	
	//create entityNodesActiveListSentences based on entityNodesActiveListComplete (just fill every sentence with the same vector entityNodesActiveListComplete, since there is no way a sentence's cross-sentence entity references can be derived from the xml file as they will have different sentenceIndexes. FUTURE: Consider storing a per sentence entity reference list in the GIA xml file);
	int maxSentenceIndex = 0;
	for(vector<GIAentityNode*>::iterator entityNodeListIterator = entityNodesActiveListComplete->begin(); entityNodeListIterator < entityNodesActiveListComplete->end(); entityNodeListIterator++)
	{
		GIAentityNode* entity = *entityNodeListIterator;
		if(entity->sentenceIndexTemp > maxSentenceIndex)
		{
			maxSentenceIndex = entity->sentenceIndexTemp;
		}
	}	
	for(int sentenceIndex=GIA_NLP_START_SENTENCE_INDEX; sentenceIndex<maxSentenceIndex; sentenceIndex++)
	{
		entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(sentenceIndex, entityNodesActiveListComplete));
	}
	
	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif

	return result;
}


bool readSemanticNetXMLfile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in GIAxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

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
bool parseSemanticNetTag(XMLparserTag* firstTagInNetwork, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, bool linkConnections)
{
	bool result = true;

	XMLparserTag* currentTagUpdatedL1 = firstTagInNetwork;

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

		XMLparserTag* currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
		if(currentTagUpdatedL2->name == NET_XML_TAG_conceptEntityNodeContainer)
		{
			long currentEntityNodeIDinEntityNodesActiveCompleteList = 0;

			XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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

						GIAentityNode* currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode* newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = convertStringToInt(currentTagUpdatedL3->firstAttribute->value);
						newEntity->idActiveList = attributeValue;
						newEntity->entityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value.c_str();
						//cout << "newEntity->entityName = " << newEntity->entityName << endl;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;

						bool foundTemp = false;
						long indexTemp = 0;

						/*
						string conceptEntityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value;
						GIAentityNode* tempNode = findOrAddConceptEntityNodeByName(entityNodesActiveListComplete, entityNodesActiveListConcepts, &conceptEntityName, &foundTemp, &indexTemp, true, &currentEntityNodeIDinCompleteList, &currentEntityNodeIDinEntityNodesActiveCompleteList);
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
						GIAentityNode* newEntity = new GIAentityNode();
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

			XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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
						GIAentityNode* currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode* newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = convertStringToInt(currentTagUpdatedL3->firstAttribute->value);
						newEntity->idActiveList = attributeValue;
						newEntity->entityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value.c_str();
						//cout << "newEntity->entityName = " << newEntity->entityName << endl;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;
						entityNodesActiveListSubstances->push_back(newEntity);
						currentEntityNodeIDInSubstanceEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Substance Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif
						/*
						GIAentityNode* newEntity = new GIAentityNode();
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

			XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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

						GIAentityNode* currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode* newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = convertStringToInt(currentTagUpdatedL3->firstAttribute->value);
						newEntity->idActiveList = attributeValue;
						newEntity->entityName = currentTagUpdatedL3->firstAttribute->nextAttribute->value.c_str();
						//cout << "newEntity->entityName = " << newEntity->entityName << endl;
						#endif

						entityNodesActiveListComplete->push_back(newEntity);
						currentEntityNodeIDinCompleteList++;
						entityNodesActiveListActions->push_back(newEntity);
						currentEntityNodeIDInActionEntityNodesList++;

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						cout << "Action Entity Node name: " << currentTagUpdatedL3->firstAttribute->nextAttribute->value << endl;
						#endif

						/*
						GIAentityNode* newEntity = new GIAentityNode();
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

			XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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
						GIAentityNode* currentEntity = entityNodesActiveListComplete->at(currentEntityNodeIDinCompleteList);
						currentEntityNodeIDinCompleteList++;
						if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete))
						{
							result = false;
						}
					}
					else
					{
						GIAentityNode* newEntity = new GIAentityNode();

						#ifdef GIA_SEMANTIC_NET_XML_DEBUG
						int attributeValue = convertStringToInt(currentTagUpdatedL3->firstAttribute->value);
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
						GIAentityNode* newEntity = new GIAentityNode();
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



bool parseEntityNodeTag(XMLparserTag* firstTagInEntityNode, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = firstTagInEntityNode->firstAttribute;

	bool idFound = false;
	bool entityNameFound = false;
	#ifdef GIA_USE_WORD_ORIG
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

	#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
	bool sentenceIndexTempFound = false;
	bool grammaticalDefiniteTempFound = false;
	bool grammaticalIndefinitePluralTempFound = false;
	bool grammaticalProperNounTempFound = false;
	#ifdef GIA_SUPPORT_PREDETERMINERS
	bool grammaticalPredeterminerTempFound = false;
	#endif
	bool entityIndexFound = false;
	bool wasReferenceFound = false;
	bool isQueryFound = false;
	bool grammaticalTenseModifierArrayTempFound = false;
	#endif
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
	bool conditionType2Found = false;
	#endif
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
	bool conditionTwoWayFound = false;
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseConditionTwoWayFound = false;
	#endif
	#endif
	#endif	

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
			long attributeValue = convertStringToLong(currentAttribute->value);
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
		#ifdef GIA_USE_WORD_ORIG
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
			double attributeValue = convertStringToDouble(currentAttribute->value);
			entityNode->confidence = attributeValue;
			confidenceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isConcept)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isConcept = attributeValue;
			isConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstance)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isSubstance = attributeValue;
			isSubstanceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isAction)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isAction = attributeValue;
			isActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isCondition)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isCondition = attributeValue;
			isConditionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedInstance)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasAssociatedInstance = attributeValue;
			hasAssociatedSubstanceFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedInstanceIsAction)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasAssociatedInstanceIsAction = attributeValue;
			hasAssociatedSubstanceIsActionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedInstanceIsCondition)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasAssociatedInstanceIsCondition = attributeValue;
			hasAssociatedSubstanceIsConditionFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedTime)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasAssociatedTime = attributeValue;
			hasAssociatedTimeFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstanceQuality)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isSubstanceQuality = attributeValue;
			isSubstanceQualityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isSubstanceConcept)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isSubstanceConcept = attributeValue;
			isSubstanceConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isActionConcept)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isActionConcept = attributeValue;
			isActionConceptFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_negative)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->negative = attributeValue;
			negativeFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_disabled)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->disabled = attributeValue;
			disabledFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->conditionType = attributeValue;
			conditionTypeFound = true;
		}


		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalNumber)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->grammaticalNumber = attributeValue;
			grammaticalNumberFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasQuantity)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasQuantity = attributeValue;
			hasQuantityFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityNumber)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
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
			int attributeValue = convertStringToInt(currentAttribute->value);
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
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasQuantityMultiplier = attributeValue;
			hasQuantityMultiplierFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasMeasure)
		{
			bool attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->hasMeasure = attributeValue;
			hasMeasureFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_measureType)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->measureType = attributeValue;
			measureTypeFound = true;
		}


		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printX)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printX = attributeValue;
			printXFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printY = attributeValue;
			printYFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printXIndex = attributeValue;
			printXIndexFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printYIndex)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printYIndex = attributeValue;
			printYIndexFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printTextX = attributeValue;
			printTextXFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->printTextY = attributeValue;
			printTextYFound = true;
			entityNode->printCoordsAlreadyDefined = true;
		}

		#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_sentenceIndexTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->sentenceIndexTemp = attributeValue;
			sentenceIndexTempFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalDefiniteTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->grammaticalDefiniteTemp = attributeValue;
			grammaticalDefiniteTempFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalIndefinitePluralTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->grammaticalIndefinitePluralTemp = attributeValue;
			grammaticalIndefinitePluralTempFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalProperNounTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->grammaticalProperNounTemp = attributeValue;
			grammaticalProperNounTempFound = true;
		}
		#ifdef GIA_SUPPORT_PREDETERMINERS
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->grammaticalPredeterminerTemp = attributeValue;
			grammaticalPredeterminerTempFound = true;
		}
		#endif
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityIndexTemp)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->entityIndexTemp = attributeValue;
			entityIndexFound = true;
		}
		#ifdef GIA_USE_ADVANCED_REFERENCING
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_wasReference)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->wasReference = attributeValue;
			wasReferenceFound = true;
		}
		#endif
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_isQuery)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->isQuery = attributeValue;
			isQueryFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp)
		{
			string attributeValue = currentAttribute->value;
			convertStringToBooleanArray(attributeValue, entityNode->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
			grammaticalTenseModifierArrayTempFound = true;
		}
		#endif
		
		#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
		#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType2)
		{
			string attributeValue = currentAttribute->value.c_str();
			entityNode->conditionType2 = attributeValue;
			conditionType2Found = true;
		}
		#endif
		#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionTwoWay)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->conditionTwoWay = attributeValue;
			conditionTwoWayFound = true;
		}
		#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_inverseConditionTwoWay)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			entityNode->inverseConditionTwoWay = attributeValue;
			inverseConditionTwoWayFound = true;
		}
		#endif
		#endif
		#endif		

		currentAttribute = currentAttribute->nextAttribute;
	}

	XMLparserTag* currentTagUpdatedL3 = firstTagInEntityNode->firstLowerLevelTag;
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


bool parseEntityVectorConnectionNodeListTag(XMLparserTag* firstTagInEntityVectorConnectionNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete, int entityVectorConnectionIndex)
{
	bool result = true;
	XMLparserTag* currentTagUpdatedL1 = firstTagInEntityVectorConnectionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex])
		{
			#ifdef GIA_SEMANTIC_NET_XML_DEBUG
			//cout <<  entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " nodeReference: " << endl;
			#endif
			XMLparserAttribute* currentAttribute = currentTagUpdatedL1->firstAttribute;

			long idActiveList = INT_DEFAULT_VALUE;
			GIAentityConnection* newConnection = new GIAentityConnection();

			bool idFound = false;
			#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
			#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
			bool sentenceIndexTempFound = false;
			#endif
			#ifdef GIA_USE_ADVANCED_REFERENCING
			#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
			bool isReferenceFound = false;
			#endif
			#endif
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			bool sameReferenceSetFound = false;
			#ifdef GIA_RECORD_RCMOD_SET_INFORMATION
			bool rcmodIndicatesSameReferenceSetFound = false;
			#endif
			#endif
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			bool isAliasFound = false;
			#endif
			#endif
			
			while(currentAttribute->nextAttribute != NULL)
			{
				if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
				{
					long attributeValue = convertStringToLong(currentAttribute->value);
					idActiveList = attributeValue;
					idFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
				#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_sentenceIndexTemp)
				{
					int attributeValue = convertStringToInt(currentAttribute->value);
					newConnection->sentenceIndexTemp = attributeValue;
					sentenceIndexTempFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#endif
				#ifdef GIA_USE_ADVANCED_REFERENCING
				#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_isReference)
				{
					bool attributeValue = convertStringToInt(currentAttribute->value);
					newConnection->isReference = attributeValue;
					isReferenceFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#endif
				#endif
				#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_sameReferenceSet)
				{
					bool attributeValue = convertStringToInt(currentAttribute->value);
					newConnection->sameReferenceSet = attributeValue;
					sameReferenceSetFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#ifdef GIA_RECORD_RCMOD_SET_INFORMATION
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_rcmodIndicatesSameReferenceSet)
				{
					bool attributeValue = convertStringToInt(currentAttribute->value);
					newConnection->rcmodIndicatesSameReferenceSet = attributeValue;
					rcmodIndicatesSameReferenceSetFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#endif
				#endif
				#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_isAlias)
				{
					bool attributeValue = convertStringToInt(currentAttribute->value);
					newConnection->isAlias = attributeValue;
					isAliasFound = true;
					#ifdef GIA_SEMANTIC_NET_XML_DEBUG
					//cout << "connection idActiveList = " << idActiveList << endl;
					#endif
				}
				#endif
				#endif

				currentAttribute = currentAttribute->nextAttribute;
			}
			if(idFound)
			{
				GIAentityNode* entity = findActiveEntityNodeByID(idActiveList, entityNodesActiveListComplete);
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
				cout << "parseEntityVectorConnectionNodeListTag{} error: !idFound" << endl;
			}
		}
		else
		{
			cout << "parse " << entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex] << " error: tag not found" << endl;
		}
		currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
	}
	return result;
}

bool parseTimeConditionNodeTag(XMLparserTag* firstTagInTimeConditionNode, GIAtimeConditionNode* timeConditionNode)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = firstTagInTimeConditionNode->firstAttribute;

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
			int attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->tense = attributeValue;
			tenseFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_second)
		{
			double attributeValue = convertStringToDouble(currentAttribute->value);
			timeConditionNode->second = attributeValue;
			secondFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hour)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->hour = attributeValue;
			hourFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfWeek)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->dayOfWeek = attributeValue;
			dayOfWeekFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_month)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->month = attributeValue;
			monthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfMonth)
		{
			int attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->dayOfMonth = attributeValue;
			dayOfMonthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_year)
		{
			long attributeValue = convertStringToLong(currentAttribute->value);
			timeConditionNode->year = attributeValue;
			yearFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_period)
		{
			double attributeValue = convertStringToInt(currentAttribute->value);
			timeConditionNode->period = attributeValue;
			periodFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_totalTimeInSeconds)
		{
			long attributeValue = convertStringToLong(currentAttribute->value);
			timeConditionNode->totalTimeInSeconds = attributeValue;
			totalTimeInSecondsFound = true;
		}

		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}
















bool writeSemanticNetXMLFileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
{
	vector<GIAentityNode*>* entityNodesActiveListConcepts = new vector<GIAentityNode*>;
	for(unordered_map<string, GIAentityNode*>::iterator conceptEntityNodesListMapIter = conceptEntityNodesListMap->begin(); conceptEntityNodesListMapIter != conceptEntityNodesListMap->end(); conceptEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = conceptEntityNodesListMapIter->second;
		entityNodesActiveListConcepts->push_back(entityNode);
	}

	bool result;
	result = writeSemanticNetXMLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions);

	#ifdef GIA_FREE_MEMORY1
	delete entityNodesActiveListConcepts;
	#endif

	return result;
}


bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

	XMLparserTag* currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_semanticNetwork;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
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
	XMLparserTag* newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag2;

	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListSubstances, NET_XML_TAG_substanceEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListActions, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif

	currentTagL1 = currentTagL1->nextTag;
	XMLparserTag* newTag3 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag3;

	if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListActions, NET_XML_TAG_actionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))
	{
		result = false;
	}

	#ifdef GIA_SEMANTIC_NET_XML_DEBUG
	//cout << "(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListConditions, NET_XML_TAG_conditionEntityNodeContainer, &currentEntityNodeIDinEntityNodesActiveCompleteList))" << endl;
	#endif

	currentTagL1 = currentTagL1->nextTag;
	XMLparserTag* newTag4 = new XMLparserTag();	//had to add a null tag
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
void resetIDsForNodeList(vector<GIAentityNode*>* entityNodesList, long* currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = (*entityNodesActiveCompleteListIterator);
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


bool generateXMLentityNodeTagList(XMLparserTag* firstTagInSemanticNet, vector<GIAentityNode*>* entityNodesList, string entityContainerTagName, long* currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	bool result = true;

	XMLparserTag* currentTagL0 = firstTagInSemanticNet;
	currentTagL0->name = entityContainerTagName;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesList->begin(); entityNodesActiveCompleteListIterator < entityNodesList->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = *entityNodesActiveCompleteListIterator;

		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		//tested 11 Feb 2012; failed
		if(!(currentEntity->disabled))
		{
		#endif
			currentTagL1 = generateXMLentityNodeTag(currentTagL1, currentEntity,* currentEntityNodeIDinEntityNodesActiveCompleteList);
			(*currentEntityNodeIDinEntityNodesActiveCompleteList) = (*currentEntityNodeIDinEntityNodesActiveCompleteList) + 1;

		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}

	return result;
}



XMLparserTag* generateXMLentityNodeTag(XMLparserTag* currentTagL1, GIAentityNode* currentEntity, long currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	bool result = true;

	//generate neuron connection tag
	currentTagL1->name = NET_XML_TAG_entityNode;
	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_id;
	/*
	#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
		//do not write current idActiveList value, write new idActiveList value - this enables fast linking of parsed xml file
		tempString = convertLongToString(currentEntityNodeIDinEntityNodesActiveCompleteList);
	#else
		tempString = convertLongToString((currentEntity->idActiveList));
	#endif
	*/
	currentAttribute->value = convertLongToString(currentEntityNodeIDinEntityNodesActiveCompleteList);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_entityName;
	currentAttribute->value = currentEntity->entityName;
	currentAttribute = createNewAttribute(currentAttribute);

	#ifdef GIA_USE_WORD_ORIG
	currentAttribute->name = NET_XML_ATTRIBUTE_wordOrig;
	currentAttribute->value = currentEntity->wordOrig;
	currentAttribute = createNewAttribute(currentAttribute);
	#endif

	#ifdef GIA_SUPPORT_ALIASES
	currentAttribute->name = NET_XML_ATTRIBUTE_aliases;
	convertAliasesToAliasesString(currentEntity, &(currentAttribute->value));
	currentAttribute = createNewAttribute(currentAttribute);
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_confidence;
	currentAttribute->value = convertDoubleToString((currentEntity->confidence), "%0.6f");
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isConcept;
	currentAttribute->value = convertIntToString(int(currentEntity->isConcept));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstance;
	currentAttribute->value = convertIntToString(int(currentEntity->isSubstance));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isAction;
	currentAttribute->value = convertIntToString(int(currentEntity->isAction));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isCondition;
	currentAttribute->value = convertIntToString(int(currentEntity->isCondition));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedInstance;
	currentAttribute->value = convertIntToString(int(currentEntity->hasAssociatedInstance));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedInstanceIsAction;
	currentAttribute->value = convertIntToString(int(currentEntity->hasAssociatedInstanceIsAction));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedInstanceIsCondition;
	currentAttribute->value = convertIntToString(int(currentEntity->hasAssociatedInstanceIsCondition));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedTime;
	currentAttribute->value = convertIntToString(int(currentEntity->hasAssociatedTime));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstanceQuality;
	currentAttribute->value = convertIntToString(int(currentEntity->isSubstanceQuality));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isSubstanceConcept;
	currentAttribute->value = convertIntToString(int(currentEntity->isSubstanceConcept));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isActionConcept;
	currentAttribute->value = convertIntToString(int(currentEntity->isActionConcept));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_negative;
	currentAttribute->value = convertIntToString(int(currentEntity->negative));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_disabled;
	currentAttribute->value = convertIntToString(int(currentEntity->disabled));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType;
	currentAttribute->value = convertIntToString(currentEntity->conditionType);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalNumber;
	currentAttribute->value = convertIntToString((currentEntity->grammaticalNumber));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantity;
	currentAttribute->value = convertIntToString(int(currentEntity->hasQuantity));
	currentAttribute = createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasQuantity)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumber;
		currentAttribute->value = convertIntToString((currentEntity->quantityNumber));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumberString;
		currentAttribute->value = currentEntity->quantityNumberString;
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifier;
		currentAttribute->value = convertIntToString((currentEntity->quantityModifier));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifierString;
		currentAttribute->value = currentEntity->quantityModifierString;
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantityMultiplier;
		currentAttribute->value = convertIntToString(int(currentEntity->hasQuantityMultiplier));
		currentAttribute = createNewAttribute(currentAttribute);
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_hasMeasure;
	currentAttribute->value = convertIntToString(int(currentEntity->hasMeasure));
	currentAttribute = createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasMeasure)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_measureType;
		currentAttribute->value = convertIntToString((currentEntity->measureType));
		currentAttribute = createNewAttribute(currentAttribute);
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->initialisedForPrinting)
	{
	#endif

		currentAttribute->name = NET_XML_ATTRIBUTE_printX;
		currentAttribute->value = convertIntToString((currentEntity->printX));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printY;
		currentAttribute->value = convertIntToString((currentEntity->printY));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printXIndex;
		currentAttribute->value = convertIntToString((currentEntity->printXIndex));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printYIndex;
		currentAttribute->value = convertIntToString((currentEntity->printYIndex));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextX;
		currentAttribute->value = convertIntToString((currentEntity->printTextX));
		currentAttribute = createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextY;
		currentAttribute->value = convertIntToString((currentEntity->printTextY));
		currentAttribute = createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
	currentAttribute->name = NET_XML_ATTRIBUTE_sentenceIndexTemp;
	currentAttribute->value = convertIntToString((currentEntity->sentenceIndexTemp));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalDefiniteTemp;
	currentAttribute->value = convertIntToString(int(currentEntity->grammaticalDefiniteTemp));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalIndefinitePluralTemp;
	currentAttribute->value = convertIntToString(int(currentEntity->grammaticalIndefinitePluralTemp));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalProperNounTemp;
	currentAttribute->value = convertIntToString(int(currentEntity->grammaticalProperNounTemp));
	currentAttribute = createNewAttribute(currentAttribute);
	
	#ifdef GIA_SUPPORT_PREDETERMINERS
	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp;
	currentAttribute->value = convertIntToString(int(currentEntity->grammaticalPredeterminerTemp));
	currentAttribute = createNewAttribute(currentAttribute);
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_entityIndexTemp;
	currentAttribute->value = convertIntToString((currentEntity->entityIndexTemp));
	currentAttribute = createNewAttribute(currentAttribute);

	#ifdef GIA_USE_ADVANCED_REFERENCING
	currentAttribute->name = NET_XML_ATTRIBUTE_wasReference;
	currentAttribute->value = convertIntToString((currentEntity->wasReference));
	currentAttribute = createNewAttribute(currentAttribute);
	#endif
	
	currentAttribute->name = NET_XML_ATTRIBUTE_isQuery;
	currentAttribute->value = convertIntToString(int(currentEntity->isQuery));
	currentAttribute = createNewAttribute(currentAttribute);
	
	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp;
	currentAttribute->value = convertBooleanArrayToString(currentEntity->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
	currentAttribute = createNewAttribute(currentAttribute);	
	#endif
	
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType2;
	currentAttribute->value = currentEntity->conditionType2;
	currentAttribute = createNewAttribute(currentAttribute);
	#endif
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
	currentAttribute->name = NET_XML_ATTRIBUTE_conditionTwoWay;
	currentAttribute->value = convertIntToString(int(currentEntity->conditionTwoWay));
	currentAttribute = createNewAttribute(currentAttribute);
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	currentAttribute->name = NET_XML_ATTRIBUTE_inverseConditionTwoWay;
	currentAttribute->value = convertIntToString(int(currentEntity->inverseConditionTwoWay));
	currentAttribute = createNewAttribute(currentAttribute);
	#endif
	#endif
	#endif	
		

	XMLparserTag* firstTagL3;
	XMLparserTag* currentTagL3;
	XMLparserTag* newTag2;

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
					GIAentityConnection* connection= (*connectionIter);
					GIAentityNode* connectionEntityNode = (connection->entity);
					#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES
					if(!(connectionEntityNode->disabled))
					{
					#endif
						currentTagL3->name = entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[i];

						currentAttribute = currentTagL3->firstAttribute;

						currentAttribute->name = NET_XML_ATTRIBUTE_id;
						currentAttribute->value = convertLongToString(connectionEntityNode->idActiveListReorderdIDforXMLsave);
						currentAttribute = createNewAttribute(currentAttribute);

						#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
						#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
						currentAttribute->name = NET_XML_ATTRIBUTE_sentenceIndexTemp;
						currentAttribute->value = convertIntToString(connection->sentenceIndexTemp);
						currentAttribute = createNewAttribute(currentAttribute);
						#endif
						
						#ifdef GIA_USE_ADVANCED_REFERENCING
						#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
						currentAttribute->name = NET_XML_ATTRIBUTE_isReference;
						currentAttribute->value = convertIntToString(int(connection->isReference));
						currentAttribute = createNewAttribute(currentAttribute);
						#endif
						#endif
						
						#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
						currentAttribute->name = NET_XML_ATTRIBUTE_sameReferenceSet;
						currentAttribute->value = convertIntToString(int(connection->sameReferenceSet));
						currentAttribute = createNewAttribute(currentAttribute);
						
						#ifdef GIA_RECORD_RCMOD_SET_INFORMATION
						currentAttribute->name = NET_XML_ATTRIBUTE_rcmodIndicatesSameReferenceSet;
						currentAttribute->value = convertIntToString(int(connection->rcmodIndicatesSameReferenceSet));
						currentAttribute = createNewAttribute(currentAttribute);
						#endif
						#endif
						
						#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
						currentAttribute->name = NET_XML_ATTRIBUTE_isAlias;
						currentAttribute->value = convertIntToString(int(connection->isAlias));
						currentAttribute = createNewAttribute(currentAttribute);
						#endif
						
						#endif

						XMLparserTag* newTag3 = new XMLparserTag();	//had to add a null tag
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
		XMLparserTag* firstTagL3 = new XMLparserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		XMLparserTag* currentTagL3 = currentTagL2->firstLowerLevelTag;
		*/

		if(!generateXMLconditionTimeNodeTagList(currentTagL2, currentEntity->timeConditionNode))
		{
			result = false;
		}

		XMLparserTag* newTag2 = new XMLparserTag();	//had to add a null tag
		currentTagL2->nextTag = newTag2;
		currentTagL2 = currentTagL2->nextTag;
	}

	XMLparserTag* newTag = new XMLparserTag();
	currentTagL1->nextTag = newTag;
	currentTagL1 = currentTagL1->nextTag;

	return currentTagL1;
}








bool generateXMLconditionTimeNodeTagList(XMLparserTag* firstTagInConditionTimeNode, GIAtimeConditionNode* conditionTimeNode)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = firstTagInConditionTimeNode->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionName;
	currentAttribute->value = conditionTimeNode->conditionName;
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_tense;
	currentAttribute->value = convertIntToString((conditionTimeNode->tense));
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_second;
	currentAttribute->value = convertDoubleToString((conditionTimeNode->second), "%0.6f");
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hour;
	currentAttribute->value = convertIntToString(conditionTimeNode->hour);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfWeek;
	currentAttribute->value = convertIntToString(conditionTimeNode->dayOfWeek);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_month;
	currentAttribute->value = convertIntToString(conditionTimeNode->month);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfMonth;
	currentAttribute->value = convertIntToString(conditionTimeNode->dayOfMonth);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_year;
	currentAttribute->value = convertLongToString(conditionTimeNode->year);
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_period;
	currentAttribute->value = convertDoubleToString(conditionTimeNode->period, "%0.6f");
	currentAttribute = createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_totalTimeInSeconds;
	currentAttribute->value = convertLongToString(conditionTimeNode->totalTimeInSeconds);
	currentAttribute = createNewAttribute(currentAttribute);

	return result;
}



string convertBooleanArrayToString(bool booleanArray[], int booleanArraySize)
{
	string str = "";
	for(int i=0; i<booleanArraySize; i++)
	{
		string tempStr = convertIntToString(int(booleanArray[i]));
		str = str + tempStr;
	}
	//cout << "convertBooleanArrayToString = " << str << endl;
	return str;

}

void convertStringToBooleanArray(string str, bool booleanArray[], int booleanArraySize)
{
	for(int i=0; i<booleanArraySize; i++)
	{
		string tempStr = "";
		tempStr = tempStr + str[i];
		booleanArray[i] = convertStringToInt(tempStr);
		//cout << "convertStringToBooleanArray booleanArray[i]  = " << convertStringToInt(tempStr) << endl;
	}
}
