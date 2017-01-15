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
 * File Name: GIAxmlConversion.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2m6a 09-September-2016
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 *
 *******************************************************************************/


#ifndef HEADER_GIA_XML_CONVERSION
#define HEADER_GIA_XML_CONVERSION

#include "XMLparserClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"

#define GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ


#define GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
#define GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES

#define GIA_SEMANTIC_NET_XML_FILE_NAME "semanticNet.xml"
#define GIA_SEMANTIC_NET_XML_FILE_NAME1 "semanticNet1.xml"

#define GIA_SEMANTIC_NET_XML_NULL_NODE_ID "-1"



//#define GIA_ENABLE_XML_READ_DEBUG

#define MAX_ATTRIBUTE_VALUE_SIZE 200



/*top level*/
#define NET_XML_TAG_semanticNetwork ((string)"semanticNetwork")
	#define NET_XML_TAG_conceptEntityNodeContainer ((string)"conceptEntityNodeContainer")
	#define NET_XML_TAG_substanceEntityNodeContainer ((string)"substanceEntityNodeContainer")
	#define NET_XML_TAG_actionEntityNodeContainer ((string)"actionEntityNodeContainer")
	#define NET_XML_TAG_conditionEntityNodeContainer ((string)"conditionEntityNodeContainer")
		#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")
		#define NET_XML_TAG_entityNode ((string)"entityNode")
	/*
	#define NET_XML_TAG_entityNodeContainer ((string)"entityNodeContainer")
		#define NET_XML_TAG_entityNode ((string)"entityNode")
	*/
	/*
	#define NET_XML_TAG_entityNodeContainer ((string)"entityNodeContainer")
		#define NET_XML_TAG_entityNode ((string)"entityNode")
	#define NET_XML_TAG_actionNodeContainer ((string)"actionNodeContainer")
		#define NET_XML_TAG_actionNode ((string)"actionNode")
	#define NET_XML_TAG_conditionNodeContainer ((string)"conditionNodeContainer")
		#define NET_XML_TAG_conditionNode ((string)"conditionNode")
	*/


#define NET_XML_TAG_actionNodeReference ((string)"actionNodeReference")
#define NET_XML_ATTRIBUTE_actionNodeID ((string)"actionNodeID")
#define NET_XML_TAG_entityNodeReference ((string)"entityNodeReference")
#define NET_XML_ATTRIBUTE_entityNodeID ((string)"entityNodeID")
#define NET_XML_TAG_conditionNodeReference ((string)"conditionNodeReference")
#define NET_XML_ATTRIBUTE_conditionNodeID ((string)"conditionNodeID")

/*CXL*/

#define NET_CXL_TAG_cmap ((string)"cmap")
#define NET_CXL_ATTRIBUTE_xmlns ((string)"xmlns")
#define NET_CXL_ATTRIBUTE_xmlnsdc ((string)"xmlns:dc")
#define NET_CXL_TAG_map ((string)"map")

#define NET_CXL_TAG_conceptList ((string)"concept-list")
#define NET_CXL_TAG_linkingPhraseList ((string)"linking-phrase-list")
#define NET_CXL_TAG_concept ((string)"concept")
#define NET_CXL_TAG_linkingPhrase ((string)"linking-phrase")
#define NET_CXL_ATTRIBUTE_id ((string)"idActiveList")
#define NET_CXL_ATTRIBUTE_label ((string)"label")

#define NET_CXL_TAG_connectionList ((string)"connectionList")
#define NET_CXL_TAG_connection ((string)"connection")
#define NET_CXL_TAG_fromid ((string)"from-idActiveList")
#define NET_CXL_TAG_toid ((string)"to-idActiveList")

#define NET_CXL_TAG_conceptAppearanceList ((string)"concept-appearance-list")
#define NET_CXL_TAG_linkingPhraseAppearanceList ((string)"linking-phrase-appearance-list")
#define NET_CXL_TAG_conceptAppearance ((string)"concept-appearance")
#define NET_CXL_TAG_linkingPhraseAppearance ((string)"linking-phrase-appearance")
#define NET_CXL_ATTRIBUTE_x ((string)"x")
#define NET_CXL_ATTRIBUTE_y ((string)"y"

/*entity node*/

#define NET_XML_ATTRIBUTE_id ((string)"idActiveList")
#define NET_XML_ATTRIBUTE_entityName ((string)"entityName")
#define NET_XML_ATTRIBUTE_wordOrig ((string)"wordOrig")
#define NET_XML_ATTRIBUTE_aliases ((string)"aliases")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
//
#define NET_XML_ATTRIBUTE_isConcept ((string)"isConcept")
#define NET_XML_ATTRIBUTE_isSubstance ((string)"isSubstance")
#define NET_XML_ATTRIBUTE_isAction ((string)"isAction")
#define NET_XML_ATTRIBUTE_isCondition ((string)"isCondition")
#define NET_XML_ATTRIBUTE_hasAssociatedInstance ((string)"hasAssociatedInstance")
#define NET_XML_ATTRIBUTE_hasAssociatedInstanceIsAction ((string)"hasAssociatedInstanceIsAction")
#define NET_XML_ATTRIBUTE_hasAssociatedInstanceIsCondition ((string)"hasAssociatedInstanceIsCondition")
#define NET_XML_ATTRIBUTE_hasAssociatedTime ((string)"hasAssociatedTime")
#define NET_XML_ATTRIBUTE_isSubstanceQuality ((string)"isSubstanceQuality")
#define NET_XML_ATTRIBUTE_isSubstanceConcept ((string)"isSubstanceConcept")
#define NET_XML_ATTRIBUTE_isActionConcept ((string)"isActionConcept")
#define NET_XML_ATTRIBUTE_negative ((string)"negative")
#define NET_XML_ATTRIBUTE_disabled ((string)"disabled")

//
#define NET_XML_TAG_ActionNodeList ((string)"actionNodeList")
#define NET_XML_TAG_IncomingActionNodeList ((string)"incomingActionNodeList")
#define NET_XML_TAG_actionSubjectEntity ((string)"actionSubjectEntity")
#define NET_XML_TAG_actionObjectEntity ((string)"actionObjectEntity")
//
#define NET_XML_ATTRIBUTE_conditionType ((string)"conditionType")	//time,location,action,substance
#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")
//
#define NET_XML_TAG_SubstanceNodeList ((string)"propertyNodeList")
#define NET_XML_TAG_SubstanceNodeReverseList ((string)"propertyNodeReverseList")	//this was removed 25 Sept 2011 - assume using entityNodeContainingThisSubstance instead, and was restored on 8 Dec 2011
//#define NET_XML_ATTRIBUTE_entityNodeContainingThisSubstance ((string)"entityNodeContainingThisSubstance")	//removed 8 Dec 2011
#define NET_XML_TAG_entityNodeDefiningThisSubstance ((string)"entityNodeDefiningThisInstance")
//
#define NET_XML_TAG_EntityNodeDefinitionList ((string)"entityNodeDefinitionList")
#define NET_XML_TAG_EntityNodeDefinitionReverseList ((string)"entityNodeDefinitionReverseList")
#define NET_XML_TAG_AssociatedSubstanceNodeList ((string)"associatedInstanceNodeList")
//
#define NET_XML_TAG_ConditionNodeList ((string)"conditionNodeList")
#define NET_XML_TAG_IncomingConditionNodeList ((string)"incomingConditionNodeList")
#define NET_XML_TAG_conditionSubjectEntity ((string)"conditionSubjectEntity")
#define NET_XML_TAG_conditionObjectEntity ((string)"conditionObjectEntity")
//
#define NET_XML_ATTRIBUTE_grammaticalNumber ((string)"grammaticalNumber")
#define NET_XML_ATTRIBUTE_hasQuantity ((string)"hasQuantity")
#define NET_XML_ATTRIBUTE_quantityNumber ((string)"quantityNumber")
#define NET_XML_ATTRIBUTE_quantityNumberString ((string)"quantityNumberString")
#define NET_XML_ATTRIBUTE_quantityModifier ((string)"quantityModifier")
#define NET_XML_ATTRIBUTE_quantityModifierString ((string)"quantityModifierString")
#define NET_XML_ATTRIBUTE_hasQuantityMultiplier ((string)"hasQuantityMultiplier")
#define NET_XML_ATTRIBUTE_hasMeasure ((string)"hasMeasure")
#define NET_XML_ATTRIBUTE_measureType ((string)"measureType")
//
#define NET_XML_ATTRIBUTE_printX ((string)"printX")
#define NET_XML_ATTRIBUTE_printY ((string)"printY")
#define NET_XML_ATTRIBUTE_printXIndex ((string)"printXIndex")
#define NET_XML_ATTRIBUTE_printYIndex ((string)"printYIndex")
#define NET_XML_ATTRIBUTE_printTextX ((string)"printTextX")
#define NET_XML_ATTRIBUTE_printTextY ((string)"printTextY")
//
#ifdef GIA_XML_RECORD_ADDITIONAL_VARIABLES
#define NET_XML_ATTRIBUTE_sentenceIndexTemp ((string)"sentenceIndexTemp")	//this is only used for NLC
#define NET_XML_ATTRIBUTE_grammaticalDefiniteTemp ((string)"grammaticalDefiniteTemp")	//this is only used for NLC
#define NET_XML_ATTRIBUTE_grammaticalIndefinitePluralTemp ((string)"grammaticalIndefinitePluralTemp")	//this is only used for NLC
#define NET_XML_ATTRIBUTE_grammaticalProperNounTemp ((string)"grammaticalProperNounTemp")	//this is only used for NLC
#ifdef GIA_SUPPORT_PREDETERMINERS
#define NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp ((string)"grammaticalPredeterminerTemp")	//this is only used for NLC	//NB GIAxmlConversion does not properly support GIA_SUPPORT_PREDETERMINERS:!GIA_DISABLE_CROSS_SENTENCE_REFERENCING at present as it will not store grammaticalPredeterminerTempSentenceArray
#endif
#define NET_XML_ATTRIBUTE_entityIndexTemp ((string)"entityIndexTemp")	//this is only used for NLC
#define NET_XML_ATTRIBUTE_isQuery ((string)"isQuery")	//this is only used for NLC
#ifdef GIA_USE_ADVANCED_REFERENCING
#define NET_XML_ATTRIBUTE_wasReference ((string)"wasReference")	//this is only used for NLC
#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
#define NET_XML_ATTRIBUTE_isReference ((string)"isReference")	//this is only used for NLC
#endif
#endif
#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
#define NET_XML_ATTRIBUTE_sameReferenceSet ((string)"sameReferenceSet")	//this is only used for NLC
#endif
#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
#define NET_XML_ATTRIBUTE_isAlias ((string)"isAlias")	//this is only used for NLC
#endif
#define NET_XML_ATTRIBUTE_grammaticalTenseModifierArrayTemp ((string)"grammaticalTenseModifierArrayTemp")	//this is only used for NLC
#endif

#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
#define NET_XML_ATTRIBUTE_conditionType2 ((string)"conditionType2")
#endif
#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
#define NET_XML_ATTRIBUTE_conditionTwoWay ((string)"conditionTwoWay")
#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
#define NET_XML_ATTRIBUTE_inverseConditionTwoWay ((string)"inverseConditionTwoWay")
#endif
#endif
#endif

static string entityVectorConnectionXMLtagNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {NET_XML_TAG_ActionNodeList, NET_XML_TAG_IncomingActionNodeList, NET_XML_TAG_ConditionNodeList, NET_XML_TAG_IncomingConditionNodeList, NET_XML_TAG_SubstanceNodeList, NET_XML_TAG_SubstanceNodeReverseList, NET_XML_TAG_EntityNodeDefinitionList, NET_XML_TAG_EntityNodeDefinitionReverseList, NET_XML_TAG_AssociatedSubstanceNodeList, NET_XML_TAG_actionSubjectEntity, NET_XML_TAG_actionObjectEntity, NET_XML_TAG_conditionSubjectEntity, NET_XML_TAG_conditionObjectEntity, NET_XML_TAG_entityNodeDefiningThisSubstance};
static string entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {NET_XML_TAG_actionNodeReference, NET_XML_TAG_actionNodeReference, NET_XML_TAG_conditionNodeReference, NET_XML_TAG_conditionNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference};



/*time condition node*/
#define NET_XML_ATTRIBUTE_conditionName ((string)"conditionName")
#define NET_XML_ATTRIBUTE_tense ((string)"tense")
#define NET_XML_ATTRIBUTE_second ((string)"second")
#define NET_XML_ATTRIBUTE_hour ((string)"hour")
#define NET_XML_ATTRIBUTE_dayOfWeek ((string)"dayOfWeek")
#define NET_XML_ATTRIBUTE_month ((string)"month")
#define NET_XML_ATTRIBUTE_dayOfMonth ((string)"dayOfMonth")
#define NET_XML_ATTRIBUTE_year ((string)"year")
#define NET_XML_ATTRIBUTE_period ((string)"period")
#define NET_XML_ATTRIBUTE_totalTimeInSeconds ((string)"totalTimeInSeconds")


#define GIA_XML_DEBUG
#ifdef GIA_XML_DEBUG
#define GIA_SEMANTIC_NET_XML_FILE_NAME1 "tempsemanticNet1.xml"
bool testReadSemanticNetXMLFile1();
bool testReadSemanticNetXMLFile2(vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
#endif

bool writeSemanticNetXMLFileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
	bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
		bool generateXMLentityNodeTagList(XMLparserTag* firstTagInSemanticNet, vector<GIAentityNode*>* entityNodesList, string entityContainerTagName, long* currentEntityNodeIDinEntityNodesActiveCompleteList);
			XMLparserTag* generateXMLentityNodeTag(XMLparserTag* currentTagL1, GIAentityNode* currentEntity, long currentEntityNodeIDinEntityNodesActiveCompleteList);
				bool generateXMLconditionTimeNodeTagList(XMLparserTag* firstTagInConditionTimeNode, GIAtimeConditionNode* conditionTimeNode);
		#ifdef GIA_SEMANTIC_NET_XML_REORDER_CONCEPT_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ
		void resetIDsForNodeList(vector<GIAentityNode*>* entityNodesList, long* currentEntityNodeIDinEntityNodesActiveCompleteList);
		//void resetIDsForNodeMap(unordered_map<string, GIAentityNode*>* entityNodesMap, long* currentEntityNodeIDinEntityNodesActiveCompleteList);
		#endif

bool readSemanticNetXMLfileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences);
	bool readSemanticNetXMLfile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
		bool parseSemanticNetTag(XMLparserTag* firstTagInNetwork, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, bool linkConnections);
			bool parseEntityNodeTag(XMLparserTag* firstTagInEntityNode, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseEntityVectorConnectionNodeListTag(XMLparserTag* firstTagInEntityVectorConnectionNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete, int entityVectorConnectionIndex);
				bool parseTimeConditionNodeTag(XMLparserTag* firstTagInTimeConditionNode, GIAtimeConditionNode* timeConditionNode);


				/*
				bool parseActionNodeListTag(XMLparserTag* firstTagInActionNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseIncomingActionNodeListTag(XMLparserTag* firstTagInIncomingActionNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseSubstanceNodeListTag(XMLparserTag* firstTagInSubstanceNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseSubstanceNodeReverseListTag(XMLparserTag* firstTagInSubstanceNodeReverseList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseEntityNodeDefinitionListTag(XMLparserTag* firstTagInEntityNodeDefinitionList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseEntityNodeDefinitionReverseListTag(XMLparserTag* firstTagInEntityNodeDefinitionReverseList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseAssociatedSubstanceNodeListTag(XMLparserTag* firstTagInAssociatedSubstanceNodeList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseConditionNodeListTag(XMLparserTag* firstTagInEntityNodeDefinitionList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				bool parseIncomingConditionNodeListTag(XMLparserTag* firstTagInEntityNodeDefinitionReverseList, GIAentityNode* entityNode, vector<GIAentityNode*>* entityNodesActiveListComplete);
				*/

string convertBooleanArrayToString(bool booleanArray[], int booleanArraySize);
void convertStringToBooleanArray(string str, bool booleanArray[], int booleanArraySize);

#endif
