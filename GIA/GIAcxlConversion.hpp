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
 * File Name: GIAcxlConversion.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3c1d 01-June-2017
 * Description: Converts GIA network nodes into an XML, or converts an XML file into GIA network nodes
 *
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
			private: bool generateCXLentityNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, long* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList);
				private: XMLparserTag* addToCXLentityNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, bool networkIndexOrLinkingPhraseList, bool appearanceList, const int entityType);
					private: XMLparserTag* generateCXLentityNodeTag(XMLparserTag* currentTagL1, string entityName, long entityID, int printX, int printY, const bool networkIndexOrLinkingPhraseList, const bool appearanceList);
			private: bool generateCXLconnectionNodeTagList(XMLparserTag* firstTagInMap, vector<GIAentityNode*>* entityNodesActiveListComplete, const vector<GIAentityNode*>* entityNodesActiveListNetworkIndexes, long* currentCmapNodeIDinCmapNodeList, XMLparserTag* firstTagInLinkingPhraseList, XMLparserTag* firstTagInLinkingPhraseAppearanceList);
				private: XMLparserTag* addToCXLconnectionNodeTagList(XMLparserTag* currentTagL1, vector<GIAentityNode*>* entityNodesList, long* currentCmapNodeIDinCmapNodeList, const XMLparserTag* firstTagL1, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, XMLparserTag* firstTagInConnectionsList, const int entityType);
					private: XMLparserTag* generateCXLconnectionNodeTagAndLinkingPhraseTags(XMLparserTag* currentTagL1, const GIAentityNode* entity1, const GIAentityNode* entity2, string connectionTypeName, long* currentCmapNodeIDinCmapNodeList, XMLparserTag** currentTagInLinkingPhraseList, XMLparserTag** currentTagInLinkingPhraseAppearanceList, const XMLparserTag* firstTagInConnectionsList, const bool alreadyMadeLinkingPhrase);
						private: XMLparserTag* generateCXLconnectionNodeTag(XMLparserTag* currentTagL1, long fromID, long toID);

		#ifdef GIA_SEMANTIC_NET_CXL_REORDER_NETWORK_INDEX_IDS_UPON_CXL_WRITE_INSTEAD_OF_CXL_READ_OLD
		void resetIDsForNodeList2(vector<GIAentityNode*>* entityNodesList, long* currentEntityNodeIDinNetworkIndexEntityNodesList, int entityType)
		#endif

	#ifdef GIA_CMAP_CONVERSION_SANITISED_DO_NOT_ADD_REDUNDANT_NETWORK_INDEX_NODES
	private: bool checkIfPassedRedundantNetworkIndexNodeRemoval(GIAentityNode* currentEntity);
	#endif
};


#endif
