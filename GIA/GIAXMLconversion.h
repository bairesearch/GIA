/*******************************************************************************
 *
 * File Name: GIAXMLconversion.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b8b 22-Sept-11
 *
 *******************************************************************************/
 

#ifndef HEADER_GIA_XML_CONVERSION
#define HEADER_GIA_XML_CONVERSION



#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "XMLParserClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAActionNodeClass.h"
#include "GIAConditionNodeClass.h"

#define GIA_SEMANTIC_NET_XML_NULL_NODE_ID "-1"

#define GIA_SEMANTIC_NET_XML_FILE_NAME "semanticNet.xml"
#define GIA_SEMANTIC_NET_XML_FILE_NAME1 "semanticNet1.xml"



//#define ENFORCE_EXPLICIT_NET_XML_NEURON_ID_PARAMETERS
//#define ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

//#define DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS
//#define DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

#define MAX_ATTRIBUTE_VALUE_SIZE 200



/*top level*/
#define NET_XML_TAG_semanticNetwork ((string)"semanticNetwork")
	#define NET_XML_TAG_entityNodeContainer ((string)"entityNodeContainer")
		#define NET_XML_TAG_entityNode ((string)"entityNode")
	#define NET_XML_TAG_actionNodeContainer ((string)"actionNodeContainer")
		#define NET_XML_TAG_actionNode ((string)"actionNode")
	#define NET_XML_TAG_conditionNodeContainer ((string)"conditionNodeContainer")
		#define NET_XML_TAG_conditionNode ((string)"conditionNode")
			#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")

#define NET_XML_TAG_actionNodeReference ((string)"actionNodeReference")
#define NET_XML_ATTRIBUTE_actionNodeID ((string)"actionNodeID")
#define NET_XML_TAG_entityNodeReference ((string)"entityNodeReference")
#define NET_XML_ATTRIBUTE_entityNodeID ((string)"entityNodeID")
#define NET_XML_TAG_conditionNodeReference ((string)"conditionNodeReference")
#define NET_XML_ATTRIBUTE_conditionNodeID ((string)"conditionNodeID")



/*entity node*/

#define NET_XML_ATTRIBUTE_id ((string)"id")
#define NET_XML_ATTRIBUTE_entityName ((string)"entityName")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
//
#define NET_XML_ATTRIBUTE_isProperty ((string)"isProperty")
#define NET_XML_ATTRIBUTE_hasAssociatedProperty ((string)"hasAssociatedProperty")
#define NET_XML_ATTRIBUTE_hasAssociatedAction ((string)"hasAssociatedAction")
#define NET_XML_ATTRIBUTE_hasAssociatedTime ((string)"hasAssociatedTime")
//
#define NET_XML_TAG_ActionNodeList ((string)"ActionNodeList")
#define NET_XML_TAG_IncomingActionNodeList ((string)"IncomingActionNodeList")
//
#define NET_XML_TAG_PropertyNodeList ((string)"PropertyNodeList")
/*#define NET_XML_TAG_PropertyNodeReverseList ((string)"PropertyNodeReverseList")*/	//this has been removed 25 Sept - use entityNodeContainingThisProperty instead
#define NET_XML_ATTRIBUTE_entityNodeContainingThisProperty ((string)"entityNodeContainingThisProperty")
#define NET_XML_ATTRIBUTE_entityNodeDefiningThisProperty ((string)"entityNodeDefiningThisProperty")
//
#define NET_XML_TAG_EntityNodeDefinitionList ((string)"EntityNodeDefinitionList")
#define NET_XML_TAG_EntityNodeDefinitionReverseList ((string)"EntityNodeDefinitionReverseList")
#define NET_XML_TAG_AssociatedActionNodeList ((string)"AssociatedActionNodeList")
#define NET_XML_TAG_AssociatedPropertyNodeList ((string)"AssociatedPropertyNodeList")
//
#define NET_XML_TAG_ConditionNodeList ((string)"ConditionNodeList")
#define NET_XML_TAG_ConditionNodeReverseList ((string)"ConditionNodeReverseList")
//
#define NET_XML_ATTRIBUTE_grammaticalNumber ((string)"grammaticalNumber")
#define NET_XML_ATTRIBUTE_hasQuantity ((string)"hasQuantity")
#define NET_XML_ATTRIBUTE_quantityNumber ((string)"quantityNumber")
#define NET_XML_ATTRIBUTE_quantityModifier ((string)"quantityModifier")
#define NET_XML_ATTRIBUTE_quantityModifierString ((string)"quantityModifierString")
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



/*action node*/
/*
#define NET_XML_ATTRIBUTE_id ((string)"id")
*/
#define NET_XML_ATTRIBUTE_actionName ((string)"actionName")
//
#define NET_XML_ATTRIBUTE_entityNodeDefiningThisAction ((string)"entityNodeDefiningThisAction")
#define NET_XML_ATTRIBUTE_actionSubjectEntity ((string)"actionSubjectEntity")
#define NET_XML_ATTRIBUTE_actionObjectEntity ((string)"actionObjectEntity")
/*already defined
#define NET_XML_TAG_ConditionNodeList ((string)"ConditionNodeList")
#define NET_XML_TAG_ConditionNodeReverseList ((string)"ConditionNodeReverseList")
*/
/*
#define NET_XML_ATTRIBUTE_printX ((string)"printX")
#define NET_XML_ATTRIBUTE_printY ((string)"printY")
#define NET_XML_ATTRIBUTE_printXIndex ((string)"printXIndex")
#define NET_XML_ATTRIBUTE_printYIndex ((string)"printYIndex")
#define NET_XML_ATTRIBUTE_printTextX ((string)"printTextX")
#define NET_XML_ATTRIBUTE_printTextY ((string)"printTextY")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
*/




/*condition node*/
/*
#define NET_XML_ATTRIBUTE_id ((string)"id")
*/
#define NET_XML_ATTRIBUTE_conditionName ((string)"conditionName")
//
#define NET_XML_ATTRIBUTE_conditionIsAction ((string)"conditionIsAction")
#define NET_XML_ATTRIBUTE_conditionAction ((string)"conditionAction")
#define NET_XML_ATTRIBUTE_conditionEntity ((string)"conditionEntity")
#define NET_XML_ATTRIBUTE_parentIsAction ((string)"parentIsAction")
#define NET_XML_ATTRIBUTE_parentAction ((string)"parentAction")
#define NET_XML_ATTRIBUTE_parentProperty ((string)"parentProperty")
#define NET_XML_ATTRIBUTE_conditionType ((string)"conditionType")	//time,location,action,property
//
#define NET_XML_TAG_timeConditionNode ((string)"timeConditionNode")
/*
#define NET_XML_ATTRIBUTE_printX ((string)"printX")
#define NET_XML_ATTRIBUTE_printY ((string)"printY")
#define NET_XML_ATTRIBUTE_printXIndex ((string)"printXIndex")
#define NET_XML_ATTRIBUTE_printYIndex ((string)"printYIndex")
#define NET_XML_ATTRIBUTE_printTextX ((string)"printTextX")
#define NET_XML_ATTRIBUTE_printTextY ((string)"printTextY")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
*/

/*time condition node*/
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
bool testReadSemanticNetXMLFile2(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
#endif


bool writeSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
	bool generateXMLEntityNodeTagList(XMLParserTag * firstTagInSemanticNet, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
	bool generateXMLActionNodeTagList(XMLParserTag * firstTagInSemanticNet, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
	bool generateXMLConditionNodeTagList(XMLParserTag * firstTagInSemanticNet, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
		bool generateXMLConditionTimeNodeTagList(XMLParserTag * firstTagInConditionTimeNode, GIATimeConditionNode * conditionTimeNode);
	
bool readSemanticNetXMLFile(string xmlFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
	bool parseSemanticNetTag(XMLParserTag * firstTagInNetwork, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList, bool linkConnections);
		bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
			bool parseActionNodeListTag(XMLParserTag * firstTagInActionNodeList, GIAEntityNode * entityNode, vector<GIAActionNode*> *actionNodesCompleteList);
			bool parseIncomingActionNodeListTag(XMLParserTag * firstTagInIncomingActionNodeList, GIAEntityNode * entityNode, vector<GIAActionNode*> *actionNodesCompleteList);
			bool parsePropertyNodeListTag(XMLParserTag * firstTagInPropertyNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList);
			bool parseEntityNodeDefinitionListTag(XMLParserTag * firstTagInEntityNodeDefinitionList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList);
			bool parseEntityNodeDefinitionReverseListTag(XMLParserTag * firstTagInEntityNodeDefinitionReverseList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList);
			bool parseAssociatedActionNodeListTag(XMLParserTag * firstTagInAssociatedActionNodeList, GIAEntityNode * entityNode, vector<GIAActionNode*> *actionNodesCompleteList);
			bool parseAssociatedPropertyNodeListTag(XMLParserTag * firstTagInAssociatedPropertyNodeList, GIAEntityNode * entityNode, vector<GIAEntityNode*> *entityNodesCompleteList);
			bool parseConditionNodeListTag(XMLParserTag * firstTagInConditionNodeList, GIAEntityNode * entityNode, vector<GIAConditionNode*> *conditionNodesCompleteList);
			bool parseConditionNodeReverseListTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAEntityNode * entityNode, vector<GIAConditionNode*> *conditionNodesCompleteList);
		bool parseActionNodeTag(XMLParserTag * firstTagInActionNode, GIAActionNode * actionNode, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
			bool parseConditionNodeListInActionTag(XMLParserTag * firstTagInConditionNodeList, GIAActionNode * actionNode, vector<GIAConditionNode*> *conditionNodesCompleteList);	
			bool parseConditionNodeReverseListInActionTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAActionNode * actionNode, vector<GIAConditionNode*> *conditionNodesCompleteList);				
		bool parseConditionNodeTag(XMLParserTag * firstTagInConditionNode, GIAConditionNode * conditionNode, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);
			bool parseTimeConditionNodeTag(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode);

GIAEntityNode * findEntityNodeByID(long EntityNodeID, vector<GIAEntityNode*> *entityNodesCompleteList);
GIAActionNode * findActionNodeByID(long ActionNodeID, vector<GIAActionNode*> *actionNodesCompleteList);
GIAConditionNode * findConditionNodeByID(long conditionNodeID, vector<GIAConditionNode*> *conditionNodesCompleteList);
								
bool linkEntities(vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAActionNode*> *actionNodesCompleteList, vector<GIAConditionNode*> *conditionNodesCompleteList);

/*
	bool linkEntityNodeConnections(NeuronContainer * firstNeuronContainerInLayer, NeuronContainer * firstInputNeuronInNetwork, bool hasBackLayer, NeuronContainer * firstNeuronContainerInBackLayer);
		NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDToFind, bool * result);
NeuronContainer * recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons);
*/

#endif
