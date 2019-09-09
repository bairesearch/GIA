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
 * File Name: GIAcxlConversion.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j4a 09-September-2019
 * Description: CXL Conversion - Converts GIA network nodes into a CXL file
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_CXL_CONVERSION
#define HEADER_GIA_CXL_CONVERSION


#include "XMLparserClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"

#ifdef GIA_CMAP_CONVERSION_SANITISED
	#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
	/*
	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
		//#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES_OLD
	#endif
	*/
	#define GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_TIME_CONDITION_NODES
#endif
#define GIA_CXL_EXPAND_RATIO (3)

#define GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ			//required to support time condition nodes
//#define GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD		//this method is insufficient due to time condition nodes

/*CXL*/

#define MAX_ATTRIBUTE_VALUE_SIZE 200

#define NET_CXL_TAG_cmap ((string)"cmap")
#define NET_CXL_ATTRIBUTE_xmlns ((string)"xmlns")
#define NET_CXL_ATTRIBUTE_xmlns_DEFAULT_VALUE ((string)"http://cmap.ihmc.us/xml/cmap/")
#define NET_CXL_ATTRIBUTE_xmlnsdc ((string)"xmlns:dc")
#define NET_CXL_ATTRIBUTE_xmlnsdc_DEFAULT_VALUE ((string)"http://purl.org/dc/elements/1.1/")
#define NET_CXL_TAG_map ((string)"map")

#define NET_CXL_TAG_networkIndexList ((string)"networkIndex-list")
#define NET_CXL_TAG_linkingPhraseList ((string)"linking-phrase-list")
#define NET_CXL_TAG_networkIndex ((string)"networkIndex")
#define NET_CXL_TAG_linkingPhrase ((string)"linking-phrase")
#define NET_CXL_ATTRIBUTE_id ((string)"id")
#define NET_CXL_ATTRIBUTE_label ((string)"label")

#define NET_CXL_TAG_connectionList ((string)"connection-list")
#define NET_CXL_TAG_connection ((string)"connection")
#define NET_CXL_TAG_fromid ((string)"from-id")
#define NET_CXL_TAG_toid ((string)"to-id")

#define NET_CXL_TAG_networkIndexAppearanceList ((string)"networkIndex-appearance-list")
#define NET_CXL_TAG_linkingPhraseAppearanceList ((string)"linking-phrase-appearance-list")
#define NET_CXL_TAG_networkIndexAppearance ((string)"networkIndex-appearance")
#define NET_CXL_TAG_linkingPhraseAppearance ((string)"linking-phrase-appearance")
#define NET_CXL_ATTRIBUTE_x ((string)"x")
#define NET_CXL_ATTRIBUTE_y ((string)"y")

static bool entityVectorConnectionCXLwriteArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, false, true, false, true, false, true, false, true, true, true, false};	//CHECKTHIS

class GIAcxlConversionClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	public: bool writeCmapToolsCXLFileOptimised(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* networkIndexEntityNodesListMap);
		private: bool writeCmapToolsCXLfile(const string xmlFileName, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes);
			private: bool generateCXLentityNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, int64_t* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList);
				private: XMLparserTag* addToCXLentityNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, int64_t* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList, const int entityType);
					private: XMLparserTag* generateCXLentityNodeTag(XMLparserTag* currentTagL1, string entityName, int64_t entityID, int printX, int printY, const bool networkIndexOrLinkingPhraseList, const bool appearanceList);
			private: bool generateCXLconnectionNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, int64_t* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagInLinkingPhraseList, XMLparserTag* firstTagInLinkingPhraseAppearanceList);
				private: XMLparserTag* addToCXLconnectionNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, int64_t* currentCmapNodeIDinCmapNodeList, const XMLparserTag* firstTagL1, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList, const int entityType);
					private: XMLparserTag* generateCXLconnectionNodeTagAndLinkingPhraseTags(XMLparserTag* currentTagL1, const GIAentityNode* entity1, const GIAentityNode* entity2, string connectionTypeName, int64_t* currentCmapNodeIDinCmapNodeList, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, const XMLparserTag* firstTagInConnectionsList, const bool alreadyMadeLinkingPhrase);
						private: XMLparserTag* generateCXLconnectionNodeTag(XMLparserTag* currentTagL1, int64_t fromID, int64_t toID);

		#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
		void resetIDsForNodeList2(vector<GIAentityNode*>* entityNodesList, int64_t* currentEntityNodeIDinNetworkIndexEntityNodesList, int entityType)
		#endif

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
	private: bool checkIfPassedRedundantNetworkIndexNodeRemoval(GIAentityNode* currentEntity);
	#endif
};


#endif
