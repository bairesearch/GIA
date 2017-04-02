/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAxmlConversion.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a5g 28-March-2017
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 *
 *******************************************************************************/


#ifndef HEADER_GIA_XML_CONVERSION
#define HEADER_GIA_XML_CONVERSION

#include "XMLparserClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAdatabase.hpp"

#define GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_TAGS
#define GIA_SEMANTIC_NET_DO_NOT_ADD_EMPTY_ATTRIBUTES

#define GIA_SEMANTIC_NET_XML_FILE_NAME "semanticNet.xml"
#define GIA_SEMANTIC_NET_XML_FILE_NAME1 "semanticNet1.xml"

#define GIA_SEMANTIC_NET_XML_NULL_NODE_ID "-1"



//#define GIA_ENABLE_XML_READ_DEBUG

#define MAX_ATTRIBUTE_VALUE_SIZE 200



/*top level*/
#define NET_XML_TAG_semanticNetwork ((string)"semanticNetwork")
	#define NET_XML_TAG_networkIndexEntityNodeContainer ((string)"networkIndexEntityNodeContainer")
	#define NET_XML_TAG_substanceEntityNodeContainer ((string)"substanceEntityNodeContainer")
	#define NET_XML_TAG_conceptEntityNodeContainer ((string)"conceptEntityNodeContainer")
	#define NET_XML_TAG_actionEntityNodeContainer ((string)"actionEntityNodeContainer")
	#define NET_XML_TAG_conditionEntityNodeContainer ((string)"conditionEntityNodeContainer")
	#define NET_XML_TAG_propertyEntityNodeContainer ((string)"propertyEntityNodeContainer")
	#define NET_XML_TAG_definitionEntityNodeContainer ((string)"definitionEntityNodeContainer")
	#define NET_XML_TAG_qualityEntityNodeContainer ((string)"qualityEntityNodeContainer")
		#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")
		#define NET_XML_TAG_entityNode ((string)"entityNode")
static string entityTypeNodeContainerXMLtags[GIA_ENTITY_NUMBER_OF_TYPES] = {NET_XML_TAG_networkIndexEntityNodeContainer, NET_XML_TAG_substanceEntityNodeContainer, NET_XML_TAG_conceptEntityNodeContainer, NET_XML_TAG_actionEntityNodeContainer, NET_XML_TAG_conditionEntityNodeContainer, NET_XML_TAG_propertyEntityNodeContainer, NET_XML_TAG_definitionEntityNodeContainer, NET_XML_TAG_qualityEntityNodeContainer};


#define NET_XML_TAG_actionNodeReference ((string)"actionNodeReference")
#define NET_XML_TAG_conditionNodeReference ((string)"conditionNodeReference")
#define NET_XML_TAG_propertyNodeReference ((string)"propertyNodeReference")
#define NET_XML_TAG_definitionNodeReference ((string)"definitionNodeReference")
#define NET_XML_TAG_entityNodeReference ((string)"entityNodeReference")
#define NET_XML_ATTRIBUTE_entityNodeID ((string)"entityNodeID")

/*CXL*/

#define NET_CXL_TAG_cmap ((string)"cmap")
#define NET_CXL_ATTRIBUTE_xmlns ((string)"xmlns")
#define NET_CXL_ATTRIBUTE_xmlnsdc ((string)"xmlns:dc")
#define NET_CXL_TAG_map ((string)"map")

#define NET_CXL_TAG_networkIndexList ((string)"networkIndex-list")
#define NET_CXL_TAG_linkingPhraseList ((string)"linking-phrase-list")
#define NET_CXL_TAG_networkIndex ((string)"networkIndex")
#define NET_CXL_TAG_linkingPhrase ((string)"linking-phrase")
#define NET_CXL_ATTRIBUTE_id ((string)"idActiveList")
#define NET_CXL_ATTRIBUTE_label ((string)"label")

#define NET_CXL_TAG_connectionList ((string)"connectionList")
#define NET_CXL_TAG_connection ((string)"connection")
#define NET_CXL_TAG_fromid ((string)"from-idActiveList")
#define NET_CXL_TAG_toid ((string)"to-idActiveList")

#define NET_CXL_TAG_networkIndexAppearanceList ((string)"networkIndex-appearance-list")
#define NET_CXL_TAG_linkingPhraseAppearanceList ((string)"linking-phrase-appearance-list")
#define NET_CXL_TAG_networkIndexAppearance ((string)"networkIndex-appearance")
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
#define NET_XML_ATTRIBUTE_entityType ((string)"entityType")
#define NET_XML_ATTRIBUTE_hasAssociatedInstance ((string)"hasAssociatedInstance")
#define NET_XML_ATTRIBUTE_hasAssociatedTime ((string)"hasAssociatedTime")
#define NET_XML_ATTRIBUTE_isActionConcept ((string)"isActionConcept")
#define NET_XML_ATTRIBUTE_negative ((string)"negative")
#define NET_XML_ATTRIBUTE_disabled ((string)"disabled")
#define NET_XML_ATTRIBUTE_isExpletive ((string)"isExpletive")

//
#define NET_XML_TAG_actionNodeList ((string)"actionNodeList")
#define NET_XML_TAG_actionReverseNodeList ((string)"actionReverseNodeList")
//
#define NET_XML_TAG_conditionNodeList ((string)"conditionNodeList")
#define NET_XML_TAG_conditionReverseNodeList ((string)"conditionReverseNodeList")
//
#define NET_XML_TAG_propertyNodeList ((string)"propertyNodeList")
#define NET_XML_TAG_propertyReverseNodeList ((string)"propertyReverseNodeList")	//this was removed 25 Sept 2011 - assume using entityNodeContainingThisSubstance instead, and was restored on 8 Dec 2011
//#define NET_XML_ATTRIBUTE_entityNodeContainingThisSubstance ((string)"entityNodeContainingThisSubstance")	//removed 8 Dec 2011
//
#define NET_XML_TAG_definitionNodeList ((string)"definitionNodeList")
#define NET_XML_TAG_definitionReverseNodeList ((string)"definitionReverseNodeList")
//
#define NET_XML_TAG_relationshipSubjectEntity ((string)"relationshipSubjectEntity")
#define NET_XML_TAG_relationshipObjectEntity ((string)"relationshipObjectEntity")
//
#define NET_XML_TAG_instanceNodeList ((string)"instanceNodeList")
#define NET_XML_TAG_instanceReverseNodeList ((string)"instanceReverseNodeList")
//
#define NET_XML_ATTRIBUTE_conditionType ((string)"conditionType")	//time,location,action,substance
#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")
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
#ifdef GIA_PREDETERMINERS
#define NET_XML_ATTRIBUTE_grammaticalPredeterminerTemp ((string)"grammaticalPredeterminerTemp")	//this is only used for NLC	//NB GIAxmlConversion does not properly support GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS (GIA_PREDETERMINERS:!GIA_DISABLE_CROSS_SENTENCE_REFERENCING) at present as it will not store grammaticalPredeterminerTempSentenceArray
#endif
#define NET_XML_ATTRIBUTE_entityIndexTemp ((string)"entityIndexTemp")	//this is only used for NLC
#define NET_XML_ATTRIBUTE_isQuery ((string)"isQuery")	//this is only used for NLC
#ifdef GIA_ADVANCED_REFERENCING
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
#define NET_XML_ATTRIBUTE_grammaticalTenseTemp ((string)"grammaticalTenseTemp")	//this is only used for NLC
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
#define NET_XML_ATTRIBUTE_conditionType2 ((string)"conditionType2")
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
#define NET_XML_ATTRIBUTE_conditionTwoWay ((string)"conditionTwoWay")
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
#define NET_XML_ATTRIBUTE_inverseConditionTwoWay ((string)"inverseConditionTwoWay")
#endif
#endif
#endif


static string entityVectorConnectionXMLtagNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {NET_XML_TAG_actionNodeList, NET_XML_TAG_actionReverseNodeList, NET_XML_TAG_conditionNodeList, NET_XML_TAG_conditionReverseNodeList, NET_XML_TAG_propertyNodeList, NET_XML_TAG_propertyReverseNodeList, NET_XML_TAG_definitionNodeList, NET_XML_TAG_definitionReverseNodeList, NET_XML_TAG_relationshipSubjectEntity, NET_XML_TAG_relationshipObjectEntity, NET_XML_TAG_instanceNodeList, NET_XML_TAG_instanceReverseNodeList};
static string entityVectorConnectionXMLtagNameCrossReferenceNodeTypeArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {NET_XML_TAG_actionNodeReference, NET_XML_TAG_actionNodeReference, NET_XML_TAG_conditionNodeReference, NET_XML_TAG_conditionNodeReference, NET_XML_TAG_propertyNodeReference, NET_XML_TAG_propertyNodeReference, NET_XML_TAG_definitionNodeReference, NET_XML_TAG_definitionNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference, NET_XML_TAG_entityNodeReference};



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

#define GIA_SEMANTIC_NET_XML_FILE_NAME1 "tempsemanticNet1.xml"
class GIAxmlConversionClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAdatabaseClass GIAdatabase;
	
	#ifdef GIA_XML_DEBUG
	private: bool testReadSemanticNetXMLFile1();
	public: bool testReadSemanticNetXMLFile2(vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes);
	#endif

	public: bool writeSemanticNetXMLFileOptimised(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
		private: bool writeSemanticNetXMLFile(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes);
			private: void resetIDsForNodeList(vector<GIAentityNode*>* entityNodesActiveListComplete, long* currentEntityNodeIDinEntityNodesActiveCompleteList, const int entityType);
			private: bool generateXMLentityNodeTagList(XMLparserTag* firstTagInSemanticNet, vector<GIAentityNode*>* entityNodesList, string entityContainerTagName, long* currentEntityNodeIDinEntityNodesActiveCompleteList, const int entityType);
				private: XMLparserTag* generateXMLentityNodeTag(XMLparserTag* currentTagL1, GIAentityNode* currentEntity, long currentEntityNodeIDinEntityNodesActiveCompleteList);
					private: bool generateXMLconditionTimeNodeTagList(XMLparserTag* firstTagInConditionTimeNode, GIAtimeConditionNode* conditionTimeNode);

	public: bool readSemanticNetXMLfileOptimised(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences);
		private: bool readSemanticNetXMLfile(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes);
			private: bool parseSemanticNetTag(XMLparserTag* firstTagInNetwork, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, const bool linkConnections);
				private: bool parseEntityNodeTag(XMLparserTag* firstTagInEntityNode, GIAentityNode* entityNode, const vector<GIAentityNode*>* entityNodesActiveListComplete, const bool linkConnections);
					private: bool parseSemanticEntityTypeNodeContainerTag(XMLparserTag* currentTagUpdatedL2, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListType, const bool linkConnections, long* currentEntityNodeIDinCompleteList);
						private: bool parseEntityVectorConnectionNodeListTag(const XMLparserTag* firstTagInEntityVectorConnectionNodeList, GIAentityNode* entityNode, const vector<GIAentityNode*>* entityNodesActiveListComplete, int entityVectorConnectionIndex);
						private: bool parseTimeConditionNodeTag(XMLparserTag* firstTagInTimeConditionNode, GIAtimeConditionNode* timeConditionNode);

	private: string convertBooleanArrayToString(const bool booleanArray[], const int booleanArraySize);
	private: void convertStringToBooleanArray(const string str, bool booleanArray[], const int booleanArraySize);
};

#endif
