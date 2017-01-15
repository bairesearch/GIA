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


#include "XMLParserClass.h"
#include "GIAEntityNodeClass.h"




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




/*entity node*/
#define NET_XML_ATTRIBUTE_grammaticalNumber ((string)"grammaticalNumber")
#define NET_XML_ATTRIBUTE_hasQuantity ((string)"hasQuantity")
#define NET_XML_ATTRIBUTE_quantityNumber ((string)"quantityNumber")
#define NET_XML_ATTRIBUTE_quantityModifier ((string)"quantityModifier")
#define NET_XML_ATTRIBUTE_quantityModifierString((string)"quantityModifierString")
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
#define NET_XML_ATTRIBUTE_entityName ((string)"entityName")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
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

#define NET_XML_TAG_EntityNodeDefinitionList ((string)"EntityNodeDefinitionList")
#define NET_XML_TAG_EntityNodeDefinitionReverseList ((string)"EntityNodeDefinitionReverseList")
#define NET_XML_TAG_AssociatedActionNodeList ((string)"AssociatedActionNodeList")
#define NET_XML_TAG_AssociatedPropertyNodeList ((string)"AssociatedPropertyNodeList")
//
#define NET_XML_TAG_ConditionNodeList ((string)"ConditionNodeList")
#define NET_XML_TAG_ConditionNodeReverseList ((string)"ConditionNodeReverseList")

#define NET_XML_TAG_actionNodeReference ((string)"actionNodeReference")
#define NET_XML_ATTRIBUTE_actionNodeID ((string)"actionNodeID")
#define NET_XML_TAG_entityNodeReference ((string)"entityNodeReference")
#define NET_XML_ATTRIBUTE_entityNodeID ((string)"entityNodeID")
#define NET_XML_TAG_conditionNodeReference ((string)"conditionNodeReference")
#define NET_XML_ATTRIBUTE_conditionNodeID ((string)"conditionNodeID")


/*action node*/
#define NET_XML_ATTRIBUTE_actionName ((string)"actionName")
/*
#define NET_XML_ATTRIBUTE_printX ((string)"printX")
#define NET_XML_ATTRIBUTE_printY ((string)"printY")
#define NET_XML_ATTRIBUTE_printXIndex ((string)"printXIndex")
#define NET_XML_ATTRIBUTE_printYIndex ((string)"printYIndex")
#define NET_XML_ATTRIBUTE_printTextX ((string)"printTextX")
#define NET_XML_ATTRIBUTE_printTextY ((string)"printTextY")
#define NET_XML_ATTRIBUTE_confidence ((string)"confidence")
*/
/*already defined
#define NET_XML_TAG_ConditionNodeList ((string)"ConditionNodeList")
#define NET_XML_TAG_ConditionNodeReverseList ((string)"ConditionNodeReverseList")
*/
#define NET_XML_ATTRIBUTE_entityNodeDefiningThisAction ((string)"entityNodeDefiningThisAction")
#define NET_XML_ATTRIBUTE_actionSubjectEntity ((string)"actionSubjectEntity")
#define NET_XML_ATTRIBUTE_actionObjectEntity ((string)"actionObjectEntity")





/*condition node*/
#define NET_XML_ATTRIBUTE_conditionName ((string)"conditionName")
#define NET_XML_ATTRIBUTE_conditionIsAction ((string)"conditionIsAction")
#define NET_XML_ATTRIBUTE_conditionAction ((string)"conditionAction")
#define NET_XML_ATTRIBUTE_conditionEntity ((string)"conditionEntity")
#define NET_XML_ATTRIBUTE_parentIsAction ((string)"parentIsAction")
#define NET_XML_ATTRIBUTE_parentAction ((string)"parentAction")
#define NET_XML_ATTRIBUTE_parentProperty ((string)"parentProperty")
#define NET_XML_ATTRIBUTE_conditionType ((string)"conditionType")	//time,location,action,property

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
bool testReadSemanticNetXMLFile2(GIAEntityNode * firstEntityInSemanticNetwork);
#endif


bool writeSemanticNetXMLFile(string xmlFileName, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork);
	bool generateXMLTagListBasedUponEntityContainerNode(XMLParserTag * firstTagInSemanticNet, GIAEntityNode * firstEntityNodeInNetwork);
		bool generateXMLTagListBasedUponEntityNode(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode);
	bool generateXMLTagListBasedUponActionContainerNode(XMLParserTag * firstTagInSemanticNet, GIAEntityNode * firstActionNodeInNetwork);
		bool generateXMLTagListBasedUponActionNode(XMLParserTag * firstTagInActionNode, GIAActionNode * actionNode);
	bool generateXMLTagListBasedUponConditionContainerNode(XMLParserTag * firstTagInSemanticNet, GIAConditionNode * firstConditionNodeInNetwork);
		bool generateXMLTagListBasedUponConditionNode(XMLParserTag * firstTagInSemanticNet, GIAConditionNode * firstConditionNodeInNetwork);	
			bool generateXMLTagListBasedUponTimeConditionNode(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode);
	
bool readSemanticNetXMLFile(string xmlFileName, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork);
	bool parseSemanticNetTag(XMLParserTag * firstTagInNetwork, GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork);
		//bool parseEntityContainerNodeTag(XMLParserTag * firstTagInEntityContainerNode, GIAEntityNode * firstEntityNodeInNetwork);
			bool parseEntityNodeTag(XMLParserTag * firstTagInEntityNode, GIAEntityNode * entityNode);
				bool parseActionNodeListTag(XMLParserTag * firstTagInActionNodeList, GIAActionNode * entityNode, GIAActionNode * firstActionNodeInNetwork);
				bool parseIncomingActionNodeListTag(XMLParserTag * firstTagInIncomingActionNodeList, GIAEntityNode * entityNode, GIAActionNode * firstActionNodeInNetwork);
				bool parsePropertyNodeListTag(XMLParserTag * firstTagInPropertyNodeList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork);
				bool parseEntityNodeDefinitionListTag(XMLParserTag * firstTagInEntityNodeDefinitionList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork);
				bool parseEntityNodeDefinitionReverseListTag(XMLParserTag * firstTagInEntityNodeDefinitionReverseList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork);
				bool parseAssociatedActionNodeListTag(XMLParserTag * firstTagInAssociatedActionNodeList, GIAEntityNode * entityNode, GIAActionNode * firstActionNodeInNetwork);
				bool parseAssociatedPropertyNodeListTag(XMLParserTag * firstTagInAssociatedPropertyNodeList, GIAEntityNode * entityNode, GIAEntityNode * firstEntityNodeInNetwork);
				bool parseConditionNodeListTag(XMLParserTag * firstTagInConditionNodeList, GIAEntityNode * entityNode, GIAConditionNode * firstConditionNodeInNetwork);
				bool parseConditionNodeReverseListTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAEntityNode * entityNode, GIAActionNode * firstConditionNodeInNetwork);
		//bool parseActionContainerNodeTag(XMLParserTag * firstTagInActionContainerNode, GIAActionNode * firstActionNodeInNetwork);	
			bool parseActionNodeTag(XMLParserTag * firstTagInActionNode, GIAActionNode * actionNode);
				bool parseConditionNodeListInActionTag(XMLParserTag * firstTagInConditionNodeList, GIAActionNode * actionNode, GIAConditionNode * firstConditionNodeInNetwork);	
				bool parseConditionNodeReverseListInActionTag(XMLParserTag * firstTagInConditionNodeReverseList, GIAActionNode * actionNode, GIAActionNode * firstConditionNodeInNetwork);				
		//bool parseConditionContainerNodeTag(XMLParserTag * firstTagInConditionContainerNode, GIAConditionNode * firstConditionNodeInNetwork);	
			bool parseConditionNodeTag(XMLParserTag * firstTagInConditionNode, GIAConditionNode * conditionNode);
				bool parseTimeConditionNodeTag(XMLParserTag * firstTagInTimeConditionNode, GIATimeConditionNode * timeConditionNode);

GIAEntityNode * findEntityNodeByID(long EntityNodeID, GIAEntityNode * firstEntityNodeInNetwork);
GIAActionNode * findActionNodeByID(long ActionNodeID, GIAActionNode * firstActionNodeInNetwork);
GIAConditionNode * findConditionNodeByID(long conditionNodeID, GIAConditionNode * firstConditionNodeInNetwork);
								
bool linkEntities(GIAEntityNode * firstEntityNodeInNetwork, GIAActionNode * firstActionNodeInNetwork, GIAConditionNode * firstConditionNodeInNetwork);

/*
	bool linkEntityNodeConnections(NeuronContainer * firstNeuronContainerInLayer, NeuronContainer * firstInputNeuronInNetwork, bool hasBackLayer, NeuronContainer * firstNeuronContainerInBackLayer);
		NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDToFind, bool * result);
NeuronContainer * recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons);
*/

#endif
