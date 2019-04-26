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
 * File Name: GIAxmlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3h3b 24-April-2019
 * Description: XML Conversion - Converts GIA network nodes into an XML file, or converts an XML file into GIA network nodes
 * NB this function creates entity idActiveListReorderdIDforXMLsave values upon write to speed up linking process (does not use original idActiveList values)
 * NB this function creates entity idActiveList values upon read (it could create idActiveListReorderdIDforXMLsave values instead - however currently it is assumed that when an XML file is loaded, this will populate the idActiveList in its entirety)
 * /
 *******************************************************************************/


#include "GIAxmlConversion.hpp"

//this function works and no longer needs to be tested

bool GIAxmlConversionClass::readSemanticNetXMLfileOptimised(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences)
{
	bool result = false;

	vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes = new vector<GIAentityNode*>;
	readSemanticNetXMLfile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes);

	//now convert entityNodesActiveListComplete to entityNodesCompleteListMap;
	int64_t vectorSize = entityNodesActiveListNetworkIndexes->size();
	for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
	{
		GIAentityNode* entityNode = entityNodesActiveListNetworkIndexes->at(entityIndex);
		//entityNodesActiveListNetworkIndexes[entityNodeName] = entityNodeFound;
		networkIndexEntityNodesListMap->insert(pair<string, GIAentityNode*>(entityNode->entityName, entityNode));
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

	delete entityNodesActiveListNetworkIndexes;

	return result;
}

bool GIAxmlConversionClass::readSemanticNetXMLfile(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in GIAxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

 	if(!XMLparserClass.readXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, false))
	{
		result = false;
	}

	if(!parseSemanticNetTag(firstTagInXMLFile, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, true))
	{
		result = false;
	}

	delete firstTagInXMLFile;

	return result;
}

bool GIAxmlConversionClass::parseSemanticNetTag(XMLparserTag* firstTagInNetwork, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, const bool linkConnections)
{
	bool result = true;

	XMLparserTag* currentTagUpdatedL1 = firstTagInNetwork;

	//not used;
	vector<GIAentityNode*> entityNodesActiveListSubstances;
	vector<GIAentityNode*> entityNodesActiveListConcepts;
	vector<GIAentityNode*> entityNodesActiveListActions;
	vector<GIAentityNode*> entityNodesActiveListConditions;
	vector<GIAentityNode*> entityNodesActiveListProperties;
	vector<GIAentityNode*> entityNodesActiveListDefinitions;
	vector<GIAentityNode*> entityNodesActiveListQualities;
	vector<GIAentityNode*>* entityNodesActiveListArray[GIA_ENTITY_NUMBER_OF_TYPES];
	entityNodesActiveListArray[GIA_ENTITY_TYPE_NETWORK_INDEX] = entityNodesActiveListNetworkIndexes;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_SUBSTANCE] = &entityNodesActiveListSubstances;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_CONCEPT] = &entityNodesActiveListConcepts;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_ACTION] = &entityNodesActiveListActions;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_CONDITION] = &entityNodesActiveListConditions;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_PROPERTY] = &entityNodesActiveListProperties;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_DEFINITION] = &entityNodesActiveListDefinitions;
	entityNodesActiveListArray[GIA_ENTITY_TYPE_QUALITY] = &entityNodesActiveListQualities;
	
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
		int64_t currentEntityNodeIDinCompleteList = 0;
		XMLparserTag* currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;

		for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
		{
			if(currentTagUpdatedL2->name == entityTypeNodeContainerXMLtags[entityType])
			{

				if(!parseSemanticEntityTypeNodeContainerTag(currentTagUpdatedL2, entityNodesActiveListComplete, entityNodesActiveListArray[entityType], linkConnections, &currentEntityNodeIDinCompleteList))
				{
					result = false;
				}
				currentTagUpdatedL2 = currentTagUpdatedL2->nextTag;
			}
			else
			{
				cout << "parseSemanticNetTag error: no networkIndex entity container node tag detected" << endl;
			}
		}
	}

	return result;
}


bool GIAxmlConversionClass::parseSemanticEntityTypeNodeContainerTag(XMLparserTag* currentTagUpdatedL2, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListType, const bool linkConnections, int64_t* currentEntityNodeIDinCompleteList)
{
	bool result = true;
	int64_t currentEntityNodeIDinEntityTypeNodeList = 0;

	XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
	while(currentTagUpdatedL3->nextTag != NULL)
	{
		if(currentTagUpdatedL3->name == NET_XML_TAG_entityNode)
		{

			GIAentityNode* currentEntity = NULL;

			if(linkConnections)
			{
				currentEntity = entityNodesActiveListComplete->at(*currentEntityNodeIDinCompleteList);

				(*currentEntityNodeIDinCompleteList) = (*currentEntityNodeIDinCompleteList) + 1;
			}
			else
			{
				currentEntity = new GIAentityNode();

				entityNodesActiveListComplete->push_back(currentEntity);
				(*currentEntityNodeIDinCompleteList) = (*currentEntityNodeIDinCompleteList) + 1;
				entityNodesActiveListType->push_back(currentEntity);
				currentEntityNodeIDinEntityTypeNodeList++;
			}

			if(!parseEntityNodeTag(currentTagUpdatedL3, currentEntity, entityNodesActiveListComplete, linkConnections))
			{
				result = false;
			}

		}
		else
		{
			if(currentTagUpdatedL3->name != "")
			{//NB networkIndex entities nodes should always exist, so this condition is not required (at the moment)
				cout << "parseSemanticEntityTypeNodeContainerTag error 1: entity node tag not detected" << endl;
			}
		}
		currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
	}
	return result;
}




bool GIAxmlConversionClass::parseEntityNodeTag(XMLparserTag* firstTagInEntityNode, GIAentityNode* entityNode, const vector<GIAentityNode*>* entityNodesActiveListComplete, const bool linkConnections)
{
	bool result = true;

	bool idFound = false;
	bool entityNameFound = false;
	#ifdef GIA_WORD_ORIG
	bool wordOrigFound = false;
	#endif
	#ifdef GIA_ALIASES
	bool aliasesFound = false;
	#endif
	bool confidenceFound = false;

	bool entityTypeFound = false;
	bool isActionConceptFound = false;
	bool hasAssociatedInstanceFound = false;
	bool hasAssociatedTimeFound = false;
	bool negativeFound = false;

	bool conditionTypeFound = false;

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

	bool grammaticalNumberFound = false;
	#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
	bool grammaticalTenseModifierArrayTempFound = false;
	bool grammaticalTenseTempFound = false;
	bool grammaticalDefiniteTempFound = false;
	bool grammaticalIndefinitePluralTempFound = false;
	bool grammaticalProperNounTempFound = false;
	#ifdef GIA_PREDETERMINERS
	bool grammaticalPredeterminerTempFound = false;
	#endif

	bool entityIndexFound = false;
	bool wasReferenceFound = false;
	bool isQueryFound = false;

	bool sentenceIndexTempFound = false;
	#endif

	bool disabledFound = false;
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	bool isExpletiveFound = false;
	#endif
	
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	bool conditionType2Found = false;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	bool conditionTwoWayFound = false;
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseConditionTwoWayFound = false;
	#endif
	#endif
	#endif

	bool timeConditionNodeFound = false;

	if(!linkConnections)
	{
		//incase network is never trained, but output for visualisation purposes, set these values to dummy values
		//parseEntityNodeTag->... = 0;
		XMLparserAttribute* currentAttribute = firstTagInEntityNode->firstAttribute;

		while(currentAttribute->nextAttribute != NULL)
		{
			if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
			{
				int64_t attributeValue = SHAREDvars.convertStringToLong(currentAttribute->value);
				entityNode->idActiveList = attributeValue;
				idFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityName)
			{
				string attributeValue = currentAttribute->value.c_str();
				entityNode->entityName = attributeValue;
				entityNameFound = true;
			}
			#ifdef GIA_WORD_ORIG
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_wordOrig)
			{
				string attributeValue = currentAttribute->value.c_str();
				entityNode->wordOrig = attributeValue;
				wordOrigFound = true;
			}
			#endif
			#ifdef GIA_ALIASES
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_aliases)
			{
				string attributeValue = currentAttribute->value.c_str();
				GIAentityNodeClass.convertAliasesStringToAliases(entityNode, attributeValue);
				aliasesFound = true;
			}
			#endif
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_confidence)
			{
				double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
				entityNode->confidence = attributeValue;
				confidenceFound = true;
			}

			else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityType)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->entityType = attributeValue;
				entityTypeFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_isActionConcept)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->isActionConcept = attributeValue;
				isActionConceptFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedInstance)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->hasAssociatedInstance = attributeValue;
				hasAssociatedInstanceFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasAssociatedTime)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->hasAssociatedTime = attributeValue;
				hasAssociatedTimeFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_negative)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->negative = attributeValue;
				negativeFound = true;
			}


			else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->conditionType = attributeValue;
				conditionTypeFound = true;
			}


			else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasQuantity)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->hasQuantity = attributeValue;
				hasQuantityFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_quantityNumber)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
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
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
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
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->hasQuantityMultiplier = attributeValue;
				hasQuantityMultiplierFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_hasMeasure)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->hasMeasure = attributeValue;
				hasMeasureFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_measureType)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->measureType = attributeValue;
				measureTypeFound = true;
			}


			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printX)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printX = attributeValue;
				printXFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printY)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printY = attributeValue;
				printYFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printXIndex)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printXIndex = attributeValue;
				printXIndexFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printYIndex)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printYIndex = attributeValue;
				printYIndexFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextX)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printTextX = attributeValue;
				printTextXFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_printTextY)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->printTextY = attributeValue;
				printTextYFound = true;
				entityNode->printCoordsAlreadyDefined = true;
			}

			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalNumber)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->grammaticalNumber = attributeValue;
				grammaticalNumberFound = true;
			}
			#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp)
			{
				string attributeValue = currentAttribute->value;
				convertStringToBooleanArray(attributeValue, entityNode->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
				grammaticalTenseModifierArrayTempFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalTenseTemp)
			{
				string attributeValue = currentAttribute->value;
				entityNode->grammaticalTenseTemp = SHAREDvars.convertStringToInt(currentAttribute->value);
				grammaticalTenseTempFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalDefiniteTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->grammaticalDefiniteTemp = attributeValue;
				grammaticalDefiniteTempFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalIndefinitePluralTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->grammaticalIndefinitePluralTemp = attributeValue;
				grammaticalIndefinitePluralTempFound = true;
			}
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalProperNounTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->grammaticalProperNounTemp = attributeValue;
				grammaticalProperNounTempFound = true;
			}
			#ifdef GIA_PREDETERMINERS
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->grammaticalPredeterminerTemp = attributeValue;
				grammaticalPredeterminerTempFound = true;
			}
			#endif

			else if(currentAttribute->name == NET_XML_ATTRIBUTE_entityIndexTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->entityIndexTemp = attributeValue;
				entityIndexFound = true;
			}
			#ifdef GIA_ADVANCED_REFERENCING
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_wasReference)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->wasReference = attributeValue;
				wasReferenceFound = true;
			}
			#endif
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_isQuery)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->isQuery = attributeValue;
				isQueryFound = true;
			}

			else if(currentAttribute->name == NET_XML_ATTRIBUTE_sentenceIndexTemp)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->sentenceIndexTemp = attributeValue;
				sentenceIndexTempFound = true;
			}
			#endif

			else if(currentAttribute->name == NET_XML_ATTRIBUTE_disabled)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->disabled = attributeValue;
				disabledFound = true;
			}
			#ifdef GIA_TRANSLATOR_EXPLETIVES
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_isExpletive)
			{
				bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->isExpletive = attributeValue;
				isExpletiveFound = true;
			}
			#endif

			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
			#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionType2)
			{
				string attributeValue = currentAttribute->value.c_str();
				entityNode->conditionType2 = attributeValue;
				conditionType2Found = true;
			}
			#endif
			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_conditionTwoWay)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->conditionTwoWay = attributeValue;
				conditionTwoWayFound = true;
			}
			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
			else if(currentAttribute->name == NET_XML_ATTRIBUTE_inverseConditionTwoWay)
			{
				int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
				entityNode->inverseConditionTwoWay = attributeValue;
				inverseConditionTwoWayFound = true;
			}
			#endif
			#endif
			#endif

			currentAttribute = currentAttribute->nextAttribute;
		}
	}
	else
	{

		bool entityVectorConnectionNodeFoundArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			entityVectorConnectionNodeFoundArray[i] = false;
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
				entityNode->timeConditionNode = new GIAtimeConditionNode();
				if(!parseTimeConditionNodeTag(currentTagUpdatedL3, entityNode->timeConditionNode))
				{
					result = false;
				}
				timeConditionNodeFound = true;
			}

			currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
		}

		if(entityNode->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
		{
			/*
			if(!entityNodeContainingThisSubstanceFound)
			{
				result = false;
			}
			*/
			#ifndef GIA_TEMPORARILY_DISABLE_GIA_XML_READ_CHECKS
			if(!entityVectorConnectionNodeFoundArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE])
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
	}

	return result;
}


bool GIAxmlConversionClass::parseEntityVectorConnectionNodeListTag(const XMLparserTag* firstTagInEntityVectorConnectionNodeList, GIAentityNode* entityNode, const vector<GIAentityNode*>* entityNodesActiveListComplete, int entityVectorConnectionIndex)
{
	bool result = true;
	const XMLparserTag* currentTagUpdatedL1 = firstTagInEntityVectorConnectionNodeList;
	while(currentTagUpdatedL1->nextTag != NULL)
	{
		if(currentTagUpdatedL1->name == entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[entityVectorConnectionIndex])
		{
			const XMLparserAttribute* currentAttribute = currentTagUpdatedL1->firstAttribute;

			int64_t idActiveList = INT_DEFAULT_VALUE;
			GIAentityConnection* newConnection = new GIAentityConnection();

			bool idFound = false;
			#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
			bool sentenceIndexTempFound = false;
			#ifdef GIA_ADVANCED_REFERENCING
			#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
			bool isReferenceFound = false;
			#endif
			#endif
			bool sameReferenceSetFound = false;
			
			#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			bool isAliasFound = false;
			#endif
			#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
			bool negativeFound = false;
			bool grammaticalTenseModifierArrayTempFound = false;
			bool grammaticalTenseTempFound = false;
			#endif
			#endif
			
			#endif

			while(currentAttribute->nextAttribute != NULL)
			{
				if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
				{
					int64_t attributeValue = SHAREDvars.convertStringToLong(currentAttribute->value);
					idActiveList = attributeValue;
					idFound = true;
				}
				#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_sentenceIndexTemp)
				{
					int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
					newConnection->sentenceIndexTemp = attributeValue;
					sentenceIndexTempFound = true;
				}
				#ifdef GIA_ADVANCED_REFERENCING
				#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_isReference)
				{
					bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
					newConnection->isReference = attributeValue;
					isReferenceFound = true;
				}
				#endif
				#endif
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_sameReferenceSet)
				{
					bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
					newConnection->sameReferenceSet = attributeValue;
					sameReferenceSetFound = true;
				}
				
				#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_isAlias)
				{
					bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
					newConnection->isAlias = attributeValue;
					isAliasFound = true;
				}
				#endif
				#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_negative)
				{
					bool attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
					newConnection->negative = attributeValue;
					negativeFound = true;
				}
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp)
				{
					string attributeValue = currentAttribute->value;
					convertStringToBooleanArray(attributeValue, newConnection->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
					grammaticalTenseModifierArrayTempFound = true;
				}
				else if(currentAttribute->name == NET_XML_ATTRIBUTE_grammaticalTenseTemp)
				{
					string attributeValue = currentAttribute->value;
					newConnection->grammaticalTenseTemp = SHAREDvars.convertStringToInt(currentAttribute->value);
					grammaticalTenseTempFound = true;
				}
				#endif
				#endif
				
				#endif

				currentAttribute = currentAttribute->nextAttribute;
			}
			if(idFound)
			{
				GIAentityNode* targetEntity = GIAdatabase.findActiveEntityNodeByID(idActiveList, entityNodesActiveListComplete);
				newConnection->entity = targetEntity;
				#ifdef GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
				newConnection->entityOrigin = entityNode;
				#ifdef GIA_ENTITY_CONNECTION_RECORD_RELATIONSHIP_TYPE
				newConnection->connectionType = entityVectorConnectionIndex;
				#endif
				#endif
				#ifdef GIA_DATABASE
				newConnection->referenceLoaded = true;
				newConnection->entityName = targetEntity->entityName;
				newConnection->idInstance = targetEntity->idInstance;
				newConnection->loaded = true;
				newConnection->modified = false;
				newConnection->added = true;
				#endif
				entityNode->entityVectorConnectionsArray[entityVectorConnectionIndex].push_back(newConnection);
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

bool GIAxmlConversionClass::parseTimeConditionNodeTag(XMLparserTag* firstTagInTimeConditionNode, GIAtimeConditionNode* timeConditionNode)
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
			int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->tense = attributeValue;
			tenseFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_second)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			timeConditionNode->second = attributeValue;
			secondFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_hour)
		{
			int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->hour = attributeValue;
			hourFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfWeek)
		{
			int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->dayOfWeek = attributeValue;
			dayOfWeekFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_month)
		{
			int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->month = attributeValue;
			monthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_dayOfMonth)
		{
			int attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->dayOfMonth = attributeValue;
			dayOfMonthFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_year)
		{
			int64_t attributeValue = SHAREDvars.convertStringToLong(currentAttribute->value);
			timeConditionNode->year = attributeValue;
			yearFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_period)
		{
			double attributeValue = SHAREDvars.convertStringToInt(currentAttribute->value);
			timeConditionNode->period = attributeValue;
			periodFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_totalTimeInSeconds)
		{
			int64_t attributeValue = SHAREDvars.convertStringToLong(currentAttribute->value);
			timeConditionNode->totalTimeInSeconds = attributeValue;
			totalTimeInSecondsFound = true;
		}

		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}
















bool GIAxmlConversionClass::writeSemanticNetXMLFileOptimised(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap)
{
	vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes = new vector<GIAentityNode*>;
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = networkIndexEntityNodesListMap->begin(); networkIndexEntityNodesListMapIter != networkIndexEntityNodesListMap->end(); networkIndexEntityNodesListMapIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
		entityNodesActiveListNetworkIndexes->push_back(entityNode);
	}

	bool result;
	result = writeSemanticNetXMLFile(xmlFileName, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes);

	delete entityNodesActiveListNetworkIndexes;

	return result;
}


bool GIAxmlConversionClass::writeSemanticNetXMLFile(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes)
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


	int64_t currentEntityNodeIDinEntityNodesActiveCompleteList;
	#ifdef GIA_SEMANTIC_NET_XML_REORDER_NETWORK_INDEX_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
	currentEntityNodeIDinEntityNodesActiveCompleteList = 0;
	for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
	{
		resetIDsForNodeList(entityNodesActiveListComplete, &currentEntityNodeIDinEntityNodesActiveCompleteList, entityType);
	}
	#endif

	currentEntityNodeIDinEntityNodesActiveCompleteList = 0;
	for(int entityType=0; entityType<GIA_ENTITY_NUMBER_OF_TYPES; entityType++)
	{

		if(!generateXMLentityNodeTagList(currentTagL1, entityNodesActiveListComplete, entityTypeNodeContainerXMLtags[entityType], &currentEntityNodeIDinEntityNodesActiveCompleteList, entityType))
		{
			result = false;
		}

		currentTagL1 = currentTagL1->nextTag;
		XMLparserTag* newTag2 = new XMLparserTag();	//had to add a null tag
		currentTagL1->nextTag = newTag2;
	}

 	if(!XMLparserClass.writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;

	return result;
}







#ifdef GIA_SEMANTIC_NET_XML_REORDER_NETWORK_INDEX_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
void GIAxmlConversionClass::resetIDsForNodeList(vector<GIAentityNode*>* entityNodesActiveListComplete, int64_t* currentEntityNodeIDinEntityNodesActiveCompleteList, const int entityType)
{
	for(vector<GIAentityNode*>::iterator entityNodesActiveCompleteListIterator = entityNodesActiveListComplete->begin(); entityNodesActiveCompleteListIterator < entityNodesActiveListComplete->end(); entityNodesActiveCompleteListIterator++)
	{
		GIAentityNode* currentEntity = (*entityNodesActiveCompleteListIterator);
		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		if(!(currentEntity->disabled))
		{
		#endif
			if(currentEntity->entityType == entityType)
			{
				currentEntity->idActiveListReorderdIDforXMLsave = *currentEntityNodeIDinEntityNodesActiveCompleteList;
				(*currentEntityNodeIDinEntityNodesActiveCompleteList) = (*currentEntityNodeIDinEntityNodesActiveCompleteList) + 1;
			}
		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}
}
#endif


bool GIAxmlConversionClass::generateXMLentityNodeTagList(XMLparserTag* firstTagInSemanticNet, vector<GIAentityNode*>* entityNodesList, string entityContainerTagName, int64_t* currentEntityNodeIDinEntityNodesActiveCompleteList, const int entityType)
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
			if(currentEntity->entityType == entityType)
			{
				#ifdef GIA_SEMANTIC_NET_XML_REORDER_NETWORK_INDEX_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
				currentTagL1 = generateXMLentityNodeTag(currentTagL1, currentEntity, *currentEntityNodeIDinEntityNodesActiveCompleteList);
				#else
				currentTagL1 = generateXMLentityNodeTag(currentTagL1, currentEntity, currentEntity->idActiveList);
				#endif
				(*currentEntityNodeIDinEntityNodesActiveCompleteList) = (*currentEntityNodeIDinEntityNodesActiveCompleteList) + 1;
			}

		#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		}
		#endif
	}

	return result;
}



XMLparserTag* GIAxmlConversionClass::generateXMLentityNodeTag(XMLparserTag* currentTagL1, GIAentityNode* currentEntity, int64_t currentEntityNodeIDinEntityNodesActiveCompleteList)
{
	bool result = true;

	//generate neuron connection tag
	currentTagL1->name = NET_XML_TAG_entityNode;
	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

	XMLparserAttribute* currentAttribute = currentTagL1->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_id;

	currentAttribute->value = SHAREDvars.convertLongToString(currentEntityNodeIDinEntityNodesActiveCompleteList);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_entityName;
	currentAttribute->value = currentEntity->entityName;
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_WORD_ORIG
	currentAttribute->name = NET_XML_ATTRIBUTE_wordOrig;
	currentAttribute->value = currentEntity->wordOrig;
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif

	#ifdef GIA_ALIASES
	currentAttribute->name = NET_XML_ATTRIBUTE_aliases;
	GIAentityNodeClass.convertAliasesToAliasesString(currentEntity, &(currentAttribute->value));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_confidence;
	currentAttribute->value = SHAREDvars.convertDoubleToString((currentEntity->confidence), "%0.6f");
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);


	currentAttribute->name = NET_XML_ATTRIBUTE_entityType;
	currentAttribute->value = SHAREDvars.convertIntToString(currentEntity->entityType);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_isActionConcept;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->isActionConcept));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedInstance;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->hasAssociatedInstance));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hasAssociatedTime;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->hasAssociatedTime));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_negative;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->negative));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);


	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType;
	currentAttribute->value = SHAREDvars.convertIntToString(currentEntity->conditionType);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);


	currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantity;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->hasQuantity));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasQuantity)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumber;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->quantityNumber));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityNumberString;
		currentAttribute->value = currentEntity->quantityNumberString;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifier;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->quantityModifier));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_quantityModifierString;
		currentAttribute->value = currentEntity->quantityModifierString;
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_hasQuantityMultiplier;
		currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->hasQuantityMultiplier));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_hasMeasure;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->hasMeasure));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->hasMeasure)
	{
	#endif
		currentAttribute->name = NET_XML_ATTRIBUTE_measureType;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->measureType));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	if(currentEntity->initialisedForPrinting)
	{
	#endif

		currentAttribute->name = NET_XML_ATTRIBUTE_printX;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printX));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printY;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printY));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printXIndex;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printXIndex));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printYIndex;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printYIndex));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextX;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printTextX));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

		currentAttribute->name = NET_XML_ATTRIBUTE_printTextY;
		currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->printTextY));
		currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES
	}
	#endif


	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalNumber;
	currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->grammaticalNumber));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp;
	currentAttribute->value = convertBooleanArrayToString(currentEntity->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalTenseTemp;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalTenseTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalDefiniteTemp;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalDefiniteTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalIndefinitePluralTemp;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalIndefinitePluralTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalProperNounTemp;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalProperNounTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_PREDETERMINERS
	currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalPredeterminerTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif


	currentAttribute->name = NET_XML_ATTRIBUTE_entityIndexTemp;
	currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->entityIndexTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_ADVANCED_REFERENCING
	currentAttribute->name = NET_XML_ATTRIBUTE_wasReference;
	currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->wasReference));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif

	currentAttribute->name = NET_XML_ATTRIBUTE_isQuery;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->isQuery));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);


	currentAttribute->name = NET_XML_ATTRIBUTE_sentenceIndexTemp;
	currentAttribute->value = SHAREDvars.convertIntToString((currentEntity->sentenceIndexTemp));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif


	currentAttribute->name = NET_XML_ATTRIBUTE_disabled;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->disabled));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	#ifdef GIA_TRANSLATOR_EXPLETIVES
	currentAttribute->name = NET_XML_ATTRIBUTE_isExpletive;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->isExpletive));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif
			

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	currentAttribute->name = NET_XML_ATTRIBUTE_conditionType2;
	currentAttribute->value = currentEntity->conditionType2;
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	currentAttribute->name = NET_XML_ATTRIBUTE_conditionTwoWay;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->conditionTwoWay));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	currentAttribute->name = NET_XML_ATTRIBUTE_inverseConditionTwoWay;
	currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->inverseConditionTwoWay));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
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
						#ifdef GIA_SEMANTIC_NET_XML_REORDER_NETWORK_INDEX_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
						currentAttribute->value = SHAREDvars.convertLongToString(connectionEntityNode->idActiveListReorderdIDforXMLsave);
						#else
						currentAttribute->value = SHAREDvars.convertLongToString(connectionEntityNode->idActiveList);
						#endif
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

						#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
						currentAttribute->name = NET_XML_ATTRIBUTE_sentenceIndexTemp;
						currentAttribute->value = SHAREDvars.convertIntToString(connection->sentenceIndexTemp);
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

						#ifdef GIA_ADVANCED_REFERENCING
						#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
						currentAttribute->name = NET_XML_ATTRIBUTE_isReference;
						currentAttribute->value = SHAREDvars.convertIntToString(int(connection->isReference));
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
						#endif
						#endif

						currentAttribute->name = NET_XML_ATTRIBUTE_sameReferenceSet;
						currentAttribute->value = SHAREDvars.convertIntToString(int(connection->sameReferenceSet));
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

						#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
						#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
						currentAttribute->name = NET_XML_ATTRIBUTE_isAlias;
						currentAttribute->value = SHAREDvars.convertIntToString(int(connection->isAlias));
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
						#endif

						#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
						currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp;
						currentAttribute->value = convertBooleanArrayToString(currentEntity->grammaticalTenseModifierArrayTemp, GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES);
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
					
						currentAttribute->name = NET_XML_ATTRIBUTE_grammaticalTenseTemp;
						currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->grammaticalTenseTemp));
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
						
						currentAttribute->name = NET_XML_ATTRIBUTE_negative;
						currentAttribute->value = SHAREDvars.convertIntToString(int(currentEntity->negative));
						currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
						#endif
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








bool GIAxmlConversionClass::generateXMLconditionTimeNodeTagList(XMLparserTag* firstTagInConditionTimeNode, GIAtimeConditionNode* conditionTimeNode)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = firstTagInConditionTimeNode->firstAttribute;

	currentAttribute->name = NET_XML_ATTRIBUTE_conditionName;
	currentAttribute->value = conditionTimeNode->conditionName;
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_tense;
	currentAttribute->value = SHAREDvars.convertIntToString((conditionTimeNode->tense));
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_second;
	currentAttribute->value = SHAREDvars.convertDoubleToString((conditionTimeNode->second), "%0.6f");
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_hour;
	currentAttribute->value = SHAREDvars.convertIntToString(conditionTimeNode->hour);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfWeek;
	currentAttribute->value = SHAREDvars.convertIntToString(conditionTimeNode->dayOfWeek);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_month;
	currentAttribute->value = SHAREDvars.convertIntToString(conditionTimeNode->month);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_dayOfMonth;
	currentAttribute->value = SHAREDvars.convertIntToString(conditionTimeNode->dayOfMonth);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_year;
	currentAttribute->value = SHAREDvars.convertLongToString(conditionTimeNode->year);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_period;
	currentAttribute->value = SHAREDvars.convertDoubleToString(conditionTimeNode->period, "%0.6f");
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	currentAttribute->name = NET_XML_ATTRIBUTE_totalTimeInSeconds;
	currentAttribute->value = SHAREDvars.convertLongToString(conditionTimeNode->totalTimeInSeconds);
	currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

	return result;
}



string GIAxmlConversionClass::convertBooleanArrayToString(const bool booleanArray[], const int booleanArraySize)
{
	string str = "";
	for(int i=0; i<booleanArraySize; i++)
	{
		string tempStr = SHAREDvars.convertIntToString(int(booleanArray[i]));
		str = str + tempStr;
	}
	return str;

}

void GIAxmlConversionClass::convertStringToBooleanArray(const string str, bool booleanArray[], const int booleanArraySize)
{
	for(int i=0; i<booleanArraySize; i++)
	{
		string tempStr = "";
		tempStr = tempStr + str[i];
		booleanArray[i] = SHAREDvars.convertStringToInt(tempStr);
	}
}
