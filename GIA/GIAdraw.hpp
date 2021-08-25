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
 * File Name: GIAdraw.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3q1a 25-August-2021
 * Requirements: 
 * Description: Draw - Draws GIA nodes in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_DRAW
#define HEADER_GIA_DRAW

#include "LDreferenceClass.hpp"
#include "XMLparserClass.hpp"
#include "GIAglobalDefs.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"	//required for generateConnectionType/generateConnectionTypeReverse
#include "XMLrulesClass.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "XMLrulesClass.hpp"
#include "LDparser.hpp"
#include "LDsprite.hpp"
#include "LDreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"


#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_COLLAPSE_ALIASES_ONLY
	#define GIA_DRAW_PRINT_ALIASES
#endif

#define GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_DEFINITION_NODES	
#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#define GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_CONNECTIONS
#endif

#define GIA_SVG_ELLIPTICAL_BOXES (true)

#define MAX_GIA_TREE_DEPTH (100000)
#define DRAW_X_INITIAL_OFFSET (150)
#define DRAW_Y_INITIAL_OFFSET (150)

#define GIA_DRAW_THICKNESS_NORMAL (1)
#define GIA_DRAW_THICKNESS_THICK (2)

#define GIA_DRAW_USE_CONNECTION_TYPE_NAME_TEXT (true)

#define GIA_DRAW_CONNECTION_DEFINING_INSTANCE_COLOUR (LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX_COLOUR)

/*
#define GIA_DRAW_CONDITION_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)				//this was red in the original spec
#define GIA_DRAW_CONDITION_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)			//this was red in the original spec
#define GIA_DRAW_CONDITION_CONNECTION_DEFINITION_COLOUR (SHARED_COLOUR_ORANGE)	//colour not defined in original GIA spec
//#define GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR (SHARED_COLOUR_ORANGE)		//Not used	//this was red in the original GIA spec		[uses whatever colour the definition is already, eg cyan for entity substance]
#define GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR (SHARED_COLOUR_ORANGE)		//this was red in the original spec
*/


#define GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR (SHARED_COLOUR_BLACK)	//SHARED_COLOUR_AQUA		//this was black SHARED_COLOUR_BLACK in the original spec
#define GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR (SHARED_COLOUR_BLACK)	//SHARED_COLOUR_PURPLE	//this was black SHARED_COLOUR_BLACK in the original spec

#define GIA_DRAW_NETWORK_INDEX_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX_COLOUR)	//this was blue SHARED_COLOUR_BLUE in the original spec, white SHARED_COLOUR_WHITE temporarily

#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	#define GIA_DRAW_DEFINITION_MARK_AS_ALIAS_CONNECTION_COLOUR (SHARED_COLOUR_DARKBLUE)
#endif

#define GIA_DRAW_ACTION_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_ACTION_COLOUR)
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_CONNECTIONS
	#define GIA_DRAW_ACTION_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_ACTION_COLOUR)
	#define GIA_DRAW_ACTION_REVERSE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_ACTION_COLOUR)
#else
	#define GIA_DRAW_ACTION_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR)
	#define GIA_DRAW_ACTION_REVERSE_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR)		
#endif
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_DEFINITION_NODES
	#define GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR (SHARED_COLOUR_BROWN)					//this was dark green SHARED_COLOUR_DARKGREEN in original GIA specA
#else
	#define GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR (GIA_DRAW_NETWORK_INDEX_NODE_COLOUR)
#endif

#define GIA_DRAW_CONDITION_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_CONNECTIONS
	#define GIA_DRAW_CONDITION_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)
	#define GIA_DRAW_CONDITION_REVERSE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)
#else
	#define GIA_DRAW_CONDITION_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR)
	#define GIA_DRAW_CONDITION_REVERSE_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR)		
#endif
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_DEFINITION_NODES
	#define GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR (SHARED_COLOUR_ORANGE)
#else
	#define GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR (GIA_DRAW_NETWORK_INDEX_NODE_COLOUR)
#endif

#define GIA_DRAW_PROPERTY_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_PROPERTY_COLOUR)							//not used in original spec
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_CONNECTIONS
	#define GIA_DRAW_PROPERTY_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_PROPERTY_COLOUR)
	#define GIA_DRAW_PROPERTY_REVERSE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_PROPERTY_COLOUR)		
#else
	#define GIA_DRAW_PROPERTY_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR)
	#define GIA_DRAW_PROPERTY_REVERSE_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR)		
#endif

#define GIA_DRAW_DEFINITION_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_DEFINITION_COLOUR)							//not used in original spec
#ifdef GIA_DRAW_USE_UNIQUE_COLOURS_FOR_ENTITY_CONNECTIONS
	#define GIA_DRAW_DEFINITION_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_DEFINITION_COLOUR)
	#define GIA_DRAW_DEFINITION_REVERSE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_DEFINITION_COLOUR)
#else
	#define GIA_DRAW_DEFINITION_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR)
	#define GIA_DRAW_DEFINITION_REVERSE_CONNECTION_COLOUR (GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR)		
#endif

#define GIA_DRAW_INSTANCE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX_COLOUR)
#define GIA_DRAW_INSTANCE_REVERSE_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX_COLOUR)


#define GIA_DRAW_CONDITION_TIME_CONNECTION_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)			//colour not defined in original GIA spec
#define GIA_DRAW_CONDITION_TIME_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR)			//this was red in the original spec
//#define GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR (SHARED_COLOUR_ORANGE)	//colour not defined in original GIA spec

//#define GIA_DRAW_CONDITION_TIME_STATE_NODE_COLOUR (SHARED_COLOUR_DARKRED)
#define GIA_DRAW_SUBSTANCE_QUALITY_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_QUALITY_COLOUR)



#define GIA_DRAW_SUBSTANCE_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_SUBSTANCE_COLOUR)	//this was cyan(?) in the original spec

#define GIA_DRAW_CONCEPT_NODE_COLOUR (LRP_SHARED_ENTITY_TYPE_CONCEPT_COLOUR)
#define GIA_DRAW_ACTION_CONCEPT_NODE_COLOUR (GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR)

#define GIA_DRAW_SUBSTANCE_QUANTITY_NODE_COLOUR (SHARED_COLOUR_PURPLE)
#define GIA_DRAW_SUBSTANCE_MEASURE_NODE_COLOUR (SHARED_COLOUR_LIGHTGREY)

#define GIA_DRAW_QUERY_QUESTION_NODE_COLOUR (SHARED_COLOUR_YELLOW)
#define GIA_DRAW_QUERY_ANSWER_NODE_COLOUR (SHARED_COLOUR_YELLOW)
#define GIA_DRAW_QUERY_ANSWER_CONTEXT_NODE_COLOUR (SHARED_COLOUR_DARKGREY)

#define GIA_DRAW_ENTITY_REFERENCE_NODE_COLOUR (LRP_SHARED_ENTITY_CONNECTION_TYPE_REFERENCE_COLOUR)





#define GIA_DRAW_CONDITION_NODE_HEIGHT (5)
#define GIA_DRAW_CONDITION_NODE_WIDTH (20)
#define GIA_DRAW_ACTION_NODE_HEIGHT (5)
#define GIA_DRAW_ACTION_NODE_WIDTH (20)
#define GIA_DRAW_BASICENTITY_NODE_HEIGHT (5)
#define GIA_DRAW_BASICENTITY_NODE_WIDTH (20)
#define GIA_DRAW_SUBSTANCE_NODE_HEIGHT (5)
#define GIA_DRAW_SUBSTANCE_NODE_WIDTH (20)


//these are not used (they are dynamically replaced with the opposite of their relationship connection values eg -GIA_DRAW_Y_SPACE_BETWEEN_ACTION_NODES / -GIA_DRAW_X_SPACE_BETWEEN_DEFINITION_NODES):
#define GIA_DRAW_Y_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT (0)
#define GIA_DRAW_X_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT (0)
#define GIA_DRAW_Y_SPACE_BETWEEN_BASICCONNECTION_OF_SAME_NODE_IRRELEVANT (0)

#define GIA_DRAW_Y_SPACE_BETWEEN_ENTITIES (0)				//OLD: should really be 1 for initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_ENTITIES (300)

#define GIA_DRAW_Y_SPACE_BETWEEN_DEFINITION_NODES (-50)			//OLD: should really be 1 for initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_DEFINITION_NODES (0)
#define GIA_DRAW_Y_SPACE_BETWEEN_DEFINITIONS_OF_SAME_NODE (100)	

#define GIA_DRAW_Y_SPACE_BETWEEN_ACTION_NODES (0)
#define GIA_DRAW_X_SPACE_BETWEEN_ACTION_NODES (100)		//should not be used during initialiseForPrint
#define GIA_DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE (100)
#define GIA_DRAW_X_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE (0)		//should not be used during initialiseForPrint
//#define GIA_DRAW_Y_SPACE_BETWEEN_ACTION_DEFINITION_NODES (30)	//should not be used during initialiseForPrint
//#define GIA_DRAW_X_SPACE_BETWEEN_ACTION_DEFINITION_NODES (-50)	//should not be used during initialiseForPrint

#define GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES (20)		//should not be used during initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES (30)		//should not be used during initialiseForPrint
#define GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE (15)	//should not be used during initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE (0)	//should not be used during initialiseForPrint
//#define GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_DEFINITION_NODES (30)		//should not be used during initialiseForPrint
//#define GIA_DRAW_X_SPACE_BETWEEN_CONDITION_DEFINITION_NODES (-50)	//should not be used during initialiseForPrint


#define GIA_DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES (50)			//should not be used during initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_PROPERTY_NODES (0)
#define GIA_DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE (15)	//should not be used during initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_SUBSTANCES_OF_SAME_NODE (0)	//should not be used during initialiseForPrint
//#define GIA_DRAW_Y_SPACE_BETWEEN_SUBSTANCE_DEFINITION_NODES (30)	//should not be used during initialiseForPrint
//#define GIA_DRAW_X_SPACE_BETWEEN_SUBSTANCE_DEFINITION_NODES (-50)	//should not be used during initialiseForPrint

#define GIA_DRAW_Y_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES (30)	//should not be used during initialiseForPrint
#define GIA_DRAW_X_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES (-50)	//should not be used during initialiseForPrint

#define GIA_SVG_SCALE_FACTOR (1)
#define GIA_SVG_TEXT_SCALE_FACTOR (5)

#define GIA_DRAW_SVG_VIEWBOX_MIN_X_OFFSET (100)
#define GIA_DRAW_SVG_VIEWBOX_MIN_Y_OFFSET (100)
#define GIA_DRAW_SVG_VIEWBOX_MIN_X_OFFSET_SINGLE_SENTENCE (100)
#define GIA_DRAW_SVG_VIEWBOX_MIN_Y_OFFSET_SINGLE_SENTENCE (0)
#define GIA_DRAW_SVG_VIEWBOX_MIN_X (-GIA_DRAW_SVG_VIEWBOX_MIN_X_OFFSET)
#define GIA_DRAW_SVG_VIEWBOX_MAX_X (1920)
#define GIA_DRAW_SVG_VIEWBOX_MIN_Y (-GIA_DRAW_SVG_VIEWBOX_MIN_Y_OFFSET)
#define GIA_DRAW_SVG_VIEWBOX_MAX_Y (1400)


#define GIA_DRAW_CONNECTION_Z (0.0)
#define GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG (1.0)

#define GIA_DRAW_INITIALISE_FOR_PRINT (0)
#define GIA_DRAW_PRINT_TO_SVG_AND_CREATE_LDR_REFERENCES (1)
#define GIA_DRAW_INITIALISE (GIA_DRAW_INITIALISE_FOR_PRINT)
#define GIA_DRAW_PRINT (GIA_DRAW_PRINT_TO_SVG_AND_CREATE_LDR_REFERENCES)
#define GIA_DRAW_CREATE_LDR_REFERENCES (0)
#define GIA_DRAW_CREATE_SVG_REFERENCES (1)
#define GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES (2)

#define GIA_OUTPUT_Z_POSITION_CONNECTIONS (0.6)
#define GIA_OUTPUT_Z_POSITION_NODES (0.4)
#define GIA_OUTPUT_Z_POSITION_TEXT (0.2)

#define GIA_DRAW_UNDEFINED_CONNECTION_COLOUR (INT_DEFAULT_VALUE)

#ifdef GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE
	#define GIA_DRAW_LOGIC_REFERENCE_ENTITY_COLOUR (LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE_COLOUR)
#endif

#define GIA_DRAW_SENTENCE_INDEX_PRINT_ALL_SENTENCES (0)


static int entityNodeTypeColourArray[GIA_ENTITY_NUMBER_OF_TYPES] = {GIA_DRAW_NETWORK_INDEX_NODE_COLOUR, GIA_DRAW_SUBSTANCE_NODE_COLOUR, GIA_DRAW_CONCEPT_NODE_COLOUR, GIA_DRAW_ACTION_NODE_COLOUR, GIA_DRAW_CONDITION_NODE_COLOUR, GIA_DRAW_PROPERTY_NODE_COLOUR, GIA_DRAW_DEFINITION_NODE_COLOUR, GIA_DRAW_SUBSTANCE_QUALITY_NODE_COLOUR};


static int entityVectorConnectionDrawPosYinitialArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_DRAW_Y_SPACE_BETWEEN_ACTION_NODES, -GIA_DRAW_Y_SPACE_BETWEEN_ACTION_NODES, GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES, -GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES, -GIA_DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES, GIA_DRAW_Y_SPACE_BETWEEN_DEFINITION_NODES, -GIA_DRAW_Y_SPACE_BETWEEN_DEFINITION_NODES, GIA_DRAW_Y_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT, -GIA_DRAW_Y_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT, GIA_DRAW_Y_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES, -GIA_DRAW_Y_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES};
static int entityVectorConnectionDrawPosXinitialArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_DRAW_X_SPACE_BETWEEN_ACTION_NODES, -GIA_DRAW_X_SPACE_BETWEEN_ACTION_NODES, GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES, -GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES, GIA_DRAW_X_SPACE_BETWEEN_PROPERTY_NODES, -GIA_DRAW_X_SPACE_BETWEEN_PROPERTY_NODES, GIA_DRAW_X_SPACE_BETWEEN_DEFINITION_NODES, -GIA_DRAW_X_SPACE_BETWEEN_DEFINITION_NODES, GIA_DRAW_X_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT, -GIA_DRAW_X_SPACE_BETWEEN_SUBJECT_OBJECT_NODES_IRRELEVANT, GIA_DRAW_X_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES, -GIA_DRAW_X_SPACE_BETWEEN_INSTANCE_DEFINITION_NODES};
static int entityVectorConnectionDrawPosYspacingArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_DEFINITIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_DEFINITIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_BASICCONNECTION_OF_SAME_NODE_IRRELEVANT, GIA_DRAW_Y_SPACE_BETWEEN_BASICCONNECTION_OF_SAME_NODE_IRRELEVANT, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_BASICCONNECTION_OF_SAME_NODE_IRRELEVANT};
#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
static int entityVectorConnectionDrawColourNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_DRAW_ACTION_CONNECTION_COLOUR, GIA_DRAW_ACTION_REVERSE_CONNECTION_COLOUR, GIA_DRAW_CONDITION_CONNECTION_COLOUR, GIA_DRAW_CONDITION_REVERSE_CONNECTION_COLOUR, GIA_DRAW_PROPERTY_CONNECTION_COLOUR, GIA_DRAW_PROPERTY_REVERSE_CONNECTION_COLOUR, GIA_DRAW_DEFINITION_CONNECTION_COLOUR, GIA_DRAW_DEFINITION_REVERSE_CONNECTION_COLOUR, GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR, GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR, GIA_DRAW_INSTANCE_CONNECTION_COLOUR, GIA_DRAW_INSTANCE_REVERSE_CONNECTION_COLOUR};	//instance colour is dynamicallyDetermined
static bool entityVectorConnectionDrawConnectionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, true, true, true, true, true, true, true, true, true, true, true};
#ifdef GIA_DRAW_ONLY_PRINT_CONNECTIONS_IN_ONE_DIRECTION
static bool entityVectorConnectionDrawPrintConnectionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, true, true, true, true, true, true, true, false, false, true, false};
#endif
#else
static int entityVectorConnectionDrawColourNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_PROPERTY_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_DEFINITION_CONNECTION_COLOUR, GIA_DRAW_RELATIONSHIP_SUBJECT_CONNECTION_COLOUR, GIA_DRAW_RELATIONSHIP_OBJECT_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR, GIA_DRAW_UNDEFINED_CONNECTION_COLOUR};	//instance colour is dynamicallyDetermined
static bool entityVectorConnectionDrawConnectionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, true, false, true, true, true, false, true};
#endif

static int relationshipEntityVectorConnectionDrawPosYinitialArray[GIA_RELATIONSHIP_ENTITY_NUMBER_OF_TYPES] = {GIA_DRAW_Y_SPACE_BETWEEN_ACTION_NODES, GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES, GIA_DRAW_Y_SPACE_BETWEEN_DEFINITION_NODES};
static int relationshipEntityVectorConnectionDrawPosXinitialArray[GIA_RELATIONSHIP_ENTITY_NUMBER_OF_TYPES] = {GIA_DRAW_X_SPACE_BETWEEN_ACTION_NODES, GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES, GIA_DRAW_X_SPACE_BETWEEN_PROPERTY_NODES, GIA_DRAW_X_SPACE_BETWEEN_DEFINITION_NODES};
static int relationshipEntityVectorConnectionDrawPosYspacingArray[GIA_RELATIONSHIP_ENTITY_NUMBER_OF_TYPES] = {GIA_DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE, GIA_DRAW_Y_SPACE_BETWEEN_DEFINITIONS_OF_SAME_NODE};

class GIAdrawVariables
{
	public:
		GIAdrawVariables(void);
		~GIAdrawVariables(void);

		bool svgTinySpec;
		bool printType[3];
		int maxNumberSentences;
		int sentenceToPrint;
		GIAentityNode* entityReference;
};

class GIAdrawClass
{
	private: LDsvgClass LDsvg;
	private: LDopenglClass LDopengl;
	private: LDparserClass LDparser;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: RTpixelMapsClass RTpixelMaps;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: LDspriteClass LDsprite;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	
	public: bool printGIAnetworkNodesToSVGstring(GIAtranslatorVariablesClass* translatorVariables, const int width, const int height, const int sentenceIndex, string* writeFileStringSVG, constEffective GIAentityNode* entityReference);
	public: bool printGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, int width, int height, string outputFileNameLDR, string outputFileNameSVG, string outputFileNamePPM, const bool display, const bool useOutputLDRfile, const bool useOutputSVGfile, const bool useOutputPPMfile);

	public: bool determineBasicPrintPositionsOfAllNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, GIAdrawVariables* drawVariables, LDreference* firstReferenceInPrintList, XMLparserTag* firstTagInSVGFile);

	private: void initiateMaxXAtParticularY();
	private: bool initialiseEntityConnectionForPrinting(vec* pos1, GIAentityConnection* entityConnection, GIAdrawVariables* drawVariables, string connectionName, int entityConnectionColour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag);
		private: bool initialiseEntityNodeForPrinting(GIAentityNode* entityNode, int y, int x, GIAdrawVariables* drawVariables, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const int sentenceIndex, bool thisIsDefinitionAndPreviousNodeWasInstance);
		private: bool initialiseTimeConditionNodeForPrinting(GIAtimeConditionNode* timeConditionNode, const int y, const int x, GIAdrawVariables* drawVariables, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag);

	private: bool createReferenceConnectionWithText(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, string connectionTypeName, GIAdrawVariables* drawVariables);
		private: bool createReferenceConnection(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const GIAdrawVariables* drawVariables);
	private: bool createBox(const vec* vect, double width, double height, int colour, string* text, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, int thickness, GIAdrawVariables* drawVariables);

};








#endif
