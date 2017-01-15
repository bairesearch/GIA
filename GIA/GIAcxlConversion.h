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
 * File Name: GIAcxlConversion.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2j3a 05-June-2015
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CXL_CONVERSION
#define HEADER_GIA_CXL_CONVERSION


#include "XMLparserClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"

#ifdef GIA_CMAP_CONVERSION_SANITISED
	#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
	/*
	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
		//#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES_OLD
	#endif
	*/
	#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
#endif
#define GIA_CXL_EXPAND_RATIO (3)

#define GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ			//required to support time condition nodes
//#define GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD		//this method is insufficient due to time condition nodes

/*CXL*/

#define MAX_ATTRIBUTE_VALUE_SIZE 200

#define NET_CXL_TAG_cmap ((string)"cmap")
#define NET_CXL_ATTRIBUTE_xmlns ((string)"xmlns")
#define NET_CXL_ATTRIBUTE_xmlns_DEFAULT_VALUE ((string)"http://cmap.ihmc.us/xml/cmap/")
#define NET_CXL_ATTRIBUTE_xmlnsdc ((string)"xmlns:dc")
#define NET_CXL_ATTRIBUTE_xmlnsdc_DEFAULT_VALUE ((string)"http://purl.org/dc/elements/1.1/")
#define NET_CXL_TAG_map ((string)"map")

#define NET_CXL_TAG_conceptList ((string)"concept-list")
#define NET_CXL_TAG_linkingPhraseList ((string)"linking-phrase-list")
#define NET_CXL_TAG_concept ((string)"concept")
#define NET_CXL_TAG_linkingPhrase ((string)"linking-phrase")
#define NET_CXL_ATTRIBUTE_id ((string)"id")
#define NET_CXL_ATTRIBUTE_label ((string)"label")

#define NET_CXL_TAG_connectionList ((string)"connection-list")
#define NET_CXL_TAG_connection ((string)"connection")
#define NET_CXL_TAG_fromid ((string)"from-id")
#define NET_CXL_TAG_toid ((string)"to-id")

#define NET_CXL_TAG_conceptAppearanceList ((string)"concept-appearance-list")
#define NET_CXL_TAG_linkingPhraseAppearanceList ((string)"linking-phrase-appearance-list")
#define NET_CXL_TAG_conceptAppearance ((string)"concept-appearance")
#define NET_CXL_TAG_linkingPhraseAppearance ((string)"linking-phrase-appearance")
#define NET_CXL_ATTRIBUTE_x ((string)"x")
#define NET_CXL_ATTRIBUTE_y ((string)"y")

static bool entityVectorConnectionCXLwriteArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false,false,false,false,false,true,true,false,false,true,true,true,true,true};

bool writeCmapToolsCXLFileOptimised(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* conceptEntityNodesListMap, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
	bool writeCmapToolsCXLfile(string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions);
		bool generateCXLentityNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, long* currentCmapNodeIDinCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList);
			XMLparserTag* addToCXLentityNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, bool conceptOrLinkingPhraseList, bool appearanceList);
				XMLparserTag* generateCXLentityNodeTag(XMLparserTag* currentTagL1, string entityName, long entityID, int printX, int printY, bool conceptOrLinkingPhraseList, bool appearanceList);
		bool generateCXLconnectionNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, vector<GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, long* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagInLinkingPhraseList, XMLparserTag* firstTagInLinkingPhraseAppearanceList);
			XMLparserTag* addToCXLconnectionNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagL1, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList);
				XMLparserTag* generateCXLconnectionNodeTagAndLinkingPhraseTags(XMLparserTag* currentTagL1, GIAentityNode* entity1, GIAentityNode* entity2, string connectionTypeName, long* currentCmapNodeIDinCmapNodeList, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList, bool alreadyMadeLinkingPhrase);
					XMLparserTag* generateCXLconnectionNodeTag(XMLparserTag* currentTagL1, long fromID, long toID);

		#ifdef GIA_SEMANTIC_NET_CXL_REORDER_CONCEPT_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
		void resetIDsForNodeList2(vector<GIAentityNode*>* entityNodesList, long* currentEntityNodeIDinConceptEntityNodesList);
		//void resetIDsForNodeMap(unordered_map<string, GIAentityNode*>* entityNodesMap, long* currentEntityNodeIDinConceptEntityNodesList);
		#endif

#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_CONCEPT_NODES
bool checkIfPassedRedundantConceptNodeRemoval(GIAentityNode* currentEntity);
#endif

#endif
