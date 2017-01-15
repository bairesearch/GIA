/*******************************************************************************
 *
 * File Name: GIAdraw.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l1d 22-May-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Draws GIA nodes in GIA network/tree
 *
 *******************************************************************************/


#include "GIAdraw.h"
#include "XMLrulesClass.h"

#include "LDsvg.h"
#include "LDopengl.h"
#include "XMLrulesClass.h"
#include "LDparser.h"
#include "LDsprite.h"
#include "LDreferenceManipulation.h"
#include "RTpixelMaps.h"



int maxXAtAParticularY[MAX_GIA_TREE_DEPTH];

void initiateMaxXAtAParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_GIA_TREE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}


void determineBasicPrintPositionsOfAllNodes(vector<GIAEntityNode*> *entityNodesCompleteList, bool initialiseOrPrint[], Reference * firstReferenceInPrintList, ofstream * writeFileObject)
{
	vector<GIAEntityNode*>::iterator entityIter;
	
	Reference * currentReferenceInPrintList = firstReferenceInPrintList;
		
	initiateMaxXAtAParticularY();
	int xInitial = DRAW_X_INITIAL_OFFSET;
	int yInitial = DRAW_Y_INITIAL_OFFSET;
	maxXAtAParticularY[yInitial] = xInitial;
	//first pass; determine maxXAtAParticularY	[and use these to centre each row {at a given y} respectively]
		
	for (entityIter = entityNodesCompleteList->begin(); entityIter != entityNodesCompleteList->end(); entityIter++) 
	{		
		if(!((*entityIter)->initialisedForPrinting))
		{
			#ifdef GIA_DRAW_DEBUG
			cout << "\ttracing..." << (*entityIter)->entityName << endl;
			#endif
			
			//initiateMaxXAtAParticularY();
			xInitial = maxXAtAParticularY[yInitial];
								
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), yInitial, xInitial, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			//cout << "h2" << endl;
		}	
        	else
		{//NB if already initalised for printing, disregard
	
		}
	}
}

Reference * initialiseEntityConnectionForPrinting(vec * pos1, GIAEntityNode * entityNodeToConnect, Reference * currentReferenceInPrintList, bool initialiseOrPrint[], string connectionName, int entityDefinitionConnectionColour, ofstream * writeFileObject)
{
	if(!(entityNodeToConnect->disabled))
	{
		//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
		vec pos2;
		pos2.x = entityNodeToConnect->printX;
		pos2.y = entityNodeToConnect->printY;	
		pos2.z = DRAW_CONNECTION_Z;
		currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, pos1, &pos2, entityDefinitionConnectionColour, writeFileObject, connectionName, initialiseOrPrint);
	}
	
	return currentReferenceInPrintList;
}


Reference * initialiseEntityNodeForPrinting(GIAEntityNode * entityNode, int y, int x, bool initialiseOrPrint[], Reference * currentReferenceInPrintList, ofstream * writeFileObject)
{
	
	//if(!(entityNode->initialisedForPrinting) || (entityNode->printY < y))
	if(!(entityNode->initialisedForPrinting) && !(entityNode->disabled))
	{		
		#ifdef GIA_DRAW_DEBUG
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}
		else if(entityNode->isCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;		
		}				
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;	
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		}
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		}		 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}
		#endif

		
		
		//cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		//cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		//cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
		//cout << "\tentityNode->hasAssociatedInstanceIsAction = " << entityNode->hasAssociatedInstanceIsAction << endl;
		
		
		entityNode->initialisedForPrinting = true;
		
		maxXAtAParticularY[y] = maxXAtAParticularY[y] + DRAW_X_SPACE_BETWEEN_ENTITIES;	//only used, for indepdendent network visualisation (eg, when rendering next sentence)
		
		entityNode->printX = x;
		entityNode->printY = y;
		
		
		int q, r;
	
		vec pos1;

		pos1.x = entityNode->printX;
		pos1.y = entityNode->printY;	
		pos1.z = DRAW_CONNECTION_Z;
		
		//cout << "a1" << endl;

		int entityDefinitionConnectionColour;
		if(entityNode->isProperty)
		{
			entityDefinitionConnectionColour = GIA_DRAW_PROPERTY_DEFINITION_CONNECTION_COLOUR;
		}
		else if(entityNode->isAction)
		{
			entityDefinitionConnectionColour = GIA_DRAW_ACTION_DEFINITION_CONNECTION_COLOUR;
		}
		else if(entityNode->isCondition)
		{
			//cout << "conditionFound" << endl;
			entityDefinitionConnectionColour = GIA_DRAW_CONDITION_DEFINITION_CONNECTION_COLOUR;
		}
		else
		{
			entityDefinitionConnectionColour = GIA_DRAW_PROPERTY_DEFINITION_CONNECTION_COLOUR;
		}
		
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
		{
			int q = entityVectorConnectionDrawPosYinitialArray[i];
			int r = entityVectorConnectionDrawPosXinitialArray[i];			
			for(vector<GIAEntityNode*>::iterator entityIter = entityNode->entityVectorConnectionsArray[i].begin(); entityIter != entityNode->entityVectorConnectionsArray[i].end(); entityIter++) 
			{	
				currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
				
				if(entityVectorConnectionDrawConnectionArray[i])
				{
					currentReferenceInPrintList = initialiseEntityConnectionForPrinting(&pos1, (*entityIter), currentReferenceInPrintList, initialiseOrPrint, entityVectorConnectionDrawConnectionNameArray[i], entityVectorConnectionDrawColourNameArray[i], writeFileObject);				
				}
				q = q + entityVectorConnectionDrawPosYspacingArray[i];			
			}
		}
		for(int i=0; i<GIA_ENTITY_NUMBER_OF_BASIC_CONNECTION_TYPES; i++)
		{
			int q = entityBasicConnectionDrawPosYinitialArray[i];
			int r = entityBasicConnectionDrawPosXinitialArray[i];							
			if(entityNode->entityBasicConnectionsArray[i] != NULL)
			{	
				currentReferenceInPrintList = initialiseEntityNodeForPrinting(entityNode->actionSubjectEntity, y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
				
				bool pass = true;
				int entityConnectionColour = entityBasicConnectionDrawColourNameArray[i];
				if(i == GIA_ENTITY_BASIC_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
				{
					if(initialiseOrPrint[DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
					{
						entityConnectionColour = entityDefinitionConnectionColour;
					}
					else
					{
						pass = false;
					}
				}
				
				if(pass)
				{
					if(entityBasicConnectionDrawConnectionArray[i])
					{
						currentReferenceInPrintList = initialiseEntityConnectionForPrinting(&pos1, entityNode->entityBasicConnectionsArray[i], currentReferenceInPrintList, initialiseOrPrint, entityBasicConnectionDrawConnectionNameArray[i], entityConnectionColour, writeFileObject);						
					}
				}
			}									
		}
				
						
		if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
		{
			//cout << "b7" << endl;
			int timeConditionNodePrintX = x+r;
			int timeConditionNodePrintY = y+q;
			currentReferenceInPrintList = initialiseTimeConditionNodeForPrinting(entityNode->timeConditionNode, timeConditionNodePrintY, timeConditionNodePrintX, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);

			q = q+DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;

			//cout << "b8" << endl;
			
			//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
			vec pos2;
			pos2.x = timeConditionNodePrintX;
			pos2.y = timeConditionNodePrintY;	
			pos2.z = DRAW_CONNECTION_Z;
			currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos2, GIA_DRAW_CONDITION_TIME_CONNECTION_COLOUR, writeFileObject, "time", initialiseOrPrint);
			
		}
		
		
		
				
		//cout << "a7" << endl;
		
		if(initialiseOrPrint[DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
		{	
			//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
			
			int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
			
			int entityColour;
			if(entityNode->isQuery)
			{
				entityColour = GIA_DRAW_QUERY_QUESTION_NODE_COLOUR;
			}			
			else if(entityNode->isAnswerToQuery)
			{
				entityColour = GIA_DRAW_QUERY_ANSWER_NODE_COLOUR;
			}
			else if(entityNode->testedForQueryComparison)
			{
				entityColour = GIA_DRAW_QUERY_ANSWER_CONTEXT_NODE_COLOUR;
			}
			else if(entityNode->hasAssociatedInstanceIsAction)
			{
				/*
				if(entityNode->isAction)
				{
					cout << "asd" << endl;
				}
				*/
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR;	//clearly identify the definition of the action
				}
			}
			else if(entityNode->hasAssociatedInstanceIsCondition)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR;	//clearly identify the definition of the action
			}						
			else if(entityNode->isProperty)
			{
				if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
				{
					boxThickness = GIA_DRAW_THICKNESS_THICK;
				}
				
				if(entityNode->hasQuality)
				{
					entityColour = GIA_DRAW_PROPERTY_QUALITY_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_PROPERTY_NODE_COLOUR;
				}				
								
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else if(entityNode->hasQuantity)
				{
					entityColour = GIA_DRAW_PROPERTY_QUANTITY_NODE_COLOUR;
				}

			}
			else if(entityNode->isAction)
			{
				entityColour = GIA_DRAW_ACTION_NODE_COLOUR;	
			}
			/*			
			else if(entityNode->hasAssociatedTime)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
			}
			*/
			else if(entityNode->isCondition)
			{
				entityColour = GIA_DRAW_CONDITION_NODE_COLOUR;	//clear identify a time node				
			}		
			else if(entityNode->hasAssociatedInstance)
			{//the original spec seemed to imply that entities that have associated properties (ie, that define properties) are special but they don't appear to be
				if(!(entityNode->isProperty))
				{		
					//added 2 May 11a (highlight entities which define property nodes)
					entityColour = GIA_DRAW_PROPERTY_DEFINITION_NODE_COLOUR;	//OLD: no colour modifier, just use basic entity colour; GIA_DRAW_CONCEPT_NODE_COLOUR;
				}
			}					
			else
			{	
				entityColour = GIA_DRAW_CONCEPT_NODE_COLOUR;
			}
										
			//first, print this action node.
			string nameOfBox = "";
			if(entityNode->hasQuantity)
			{
				string quantityNumberStringTemp;
				if(entityNode->isQuery)
				{
					quantityNumberStringTemp = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
				}
				else
				{
					quantityNumberStringTemp = printQuantityNumberString(entityNode);
				}
				nameOfBox = nameOfBox + quantityNumberStringTemp + " " + entityNode->entityName;
				
			}
			else if(entityNode->negative)
			{
				nameOfBox = nameOfBox + "!" + entityNode->entityName;
			}
			
			else
			{
				nameOfBox = entityNode->entityName;
			}
			currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, writeFileObject, boxThickness, initialiseOrPrint);

		}
		
		//cout << "a8" << endl;	
		
		#ifdef GIA_DRAW_DEBUG
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}
		else if(entityNode->isCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;		
		}					
		else if(entityNode->hasAssociatedInstance)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedInstanceIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedInstanceIsCondition)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is condition)" << endl;
		}		
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
		#endif
							
	}
	//cout << "a0c" << endl;
	
	return currentReferenceInPrintList;	//does this need to be newCurrentReferenceInPrintList?
		
}



Reference * initialiseTimeConditionNodeForPrinting(GIATimeConditionNode * timeConditionNode, int y, int x, bool initialiseOrPrint[], Reference * currentReferenceInPrintList, ofstream * writeFileObject)
{

	int timeConditionNodePrintX = x;
	int timeConditionNodePrintY = y;
	timeConditionNode->printX = timeConditionNodePrintX;
	timeConditionNode->printY = timeConditionNodePrintY;
	
	vec pos1;
	vec pos2;
	vec pos3;

	pos1.x = timeConditionNodePrintX;
	pos1.y = timeConditionNodePrintY;	
	pos1.z = DRAW_CONNECTION_Z;
	
	//may accidentially overwrite adjacent nodes/connections that have already been printed here; be careful...

	
	int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
	if(timeConditionNode->isProgressive)
	{
		//cout << "box thickness is high" << endl;
		boxThickness = GIA_DRAW_THICKNESS_THICK;
	}				
	currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, GIA_DRAW_CONDITION_TIME_NODE_COLOUR, &(timeConditionNode->conditionName), writeFileObject, boxThickness, initialiseOrPrint);

	/*
	int timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_NODE_COLOUR;
	if(timeConditionNode->isProgressive)
	{
		timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_STATE_NODE_COLOUR;
	}
	currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, timeConditionNodeColour, &(timeConditionNode->conditionName), writeFileObject, GIA_DRAW_THICKNESS_NORMAL, initialiseOrPrint);
	*/
	
	#ifdef GIA_DRAW_DEBUG
	cout << "Exiting: timeConditionNode = " << timeConditionNode->conditionName << endl;
	#endif
	
	return currentReferenceInPrintList;	//does this need to be newCurrentReferenceInPrintList?
}


Reference * createReferenceConnectionWithText(Reference * currentReferenceInPrintList, vec * pos1, vec * pos2, int colour, ofstream * writeFileObject, string connectionTypeName, bool initialiseOrPrint[])
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	newCurrentReferenceInPrintList = createReferenceConnection(newCurrentReferenceInPrintList, pos1, pos2, colour, writeFileObject, initialiseOrPrint);

	if(GIA_DRAW_USE_CONNECTION_TYPE_NAME_TEXT)
	{
		vec vect;
		vect.x = (pos1->x + pos2->x)/2;
		vect.y = (pos1->y + pos2->y)/2;
		vect.z = (pos1->z + pos2->z)/2;

		if(initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] == true)
		{
			int numSpritesAdded;	//not used
			vec positionLDR;
			positionLDR.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
			positionLDR.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionLDR.z = vect.z - GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(&connectionTypeName, newCurrentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
		}
		if(initialiseOrPrint[DRAW_CREATE_SVG_REFERENCES] == true)
		{		
			vec positionSVG;
			positionSVG.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
			positionSVG.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionSVG.z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			writeSVGText(writeFileObject, connectionTypeName, &positionSVG, SVG_SCALE_FACTOR*SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
		}
	}
	
	return newCurrentReferenceInPrintList;
}

Reference * createReferenceConnection(Reference * currentReferenceInPrintList, vec * pos1, vec * pos2, int colour, ofstream * writeFileObject, bool initialiseOrPrint[])
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	if(initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] == true)
	{

		//cout << "drawing connection" << endl;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = colour;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = pos1->x;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = pos1->y;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = pos1->z;


		newCurrentReferenceInPrintList->vertex2relativePosition.x = pos2->x;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = pos2->y;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = pos2->z;

		/*
		cout << "createFileOrFunctionReferenceConnection():" << endl;
		cout << "currentReferenceInAboveList->name = " << currentReferenceInAboveList->name << endl;
		cout << "reference->name = " << reference->name << endl;
		cout << "newCurrentReferenceInPrintList->type = " << newCurrentReferenceInPrintList->type << endl;
		cout << "newCurrentReferenceInPrintList->colour = " << newCurrentReferenceInPrintList->colour << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.x = " << newCurrentReferenceInPrintList->vertex1relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.y = " << newCurrentReferenceInPrintList->vertex1relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.z = " << newCurrentReferenceInPrintList->vertex1relativePosition.z << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.x = " << newCurrentReferenceInPrintList->vertex2relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.y = " << newCurrentReferenceInPrintList->vertex2relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.z = " << newCurrentReferenceInPrintList->vertex2relativePosition.z << endl;
		*/

		Reference * newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;
	}
	
	if(initialiseOrPrint[DRAW_CREATE_SVG_REFERENCES] == true)
	{	
		pos1->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		pos2->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		writeSVGLine(writeFileObject, pos1, pos2, colour);
	}
	
	return newCurrentReferenceInPrintList;
}



//consider using elipse instead; <ellipse cx="240" cy="100" rx="220" ry="30">

Reference * createBox(Reference * currentReferenceInPrintList, vec * vect, double width, double height, int colour, string * text, ofstream * writeFileObject, int thickness, bool initialiseOrPrint[])
{
	Reference * newCurrentReferenceInPrintList = currentReferenceInPrintList;

	if(initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] == true)
	{
		
		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_QUAD;
		newCurrentReferenceInPrintList->colour = colour;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex3relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex3relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex3relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex4relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex4relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex4relativePosition.z = vect->z;

		/*
		cout << "createFileOrFunctionReferenceBox():" << endl;
		cout << "reference->name = " << reference->name << endl;
		cout << "newCurrentReferenceInPrintList->type = " << newCurrentReferenceInPrintList->type << endl;
		cout << "newCurrentReferenceInPrintList->colour = " << newCurrentReferenceInPrintList->colour << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.x = " << newCurrentReferenceInPrintList->vertex1relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.y = " << newCurrentReferenceInPrintList->vertex1relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex1relativePosition.z = " << newCurrentReferenceInPrintList->vertex1relativePosition.z << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.x = " << newCurrentReferenceInPrintList->vertex2relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.y = " << newCurrentReferenceInPrintList->vertex2relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex2relativePosition.z = " << newCurrentReferenceInPrintList->vertex2relativePosition.z << endl;
		cout << "newCurrentReferenceInPrintList->vertex3relativePosition.x = " << newCurrentReferenceInPrintList->vertex3relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex3relativePosition.y = " << newCurrentReferenceInPrintList->vertex3relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex3relativePosition.z = " << newCurrentReferenceInPrintList->vertex3relativePosition.z << endl;
		cout << "newCurrentReferenceInPrintList->vertex4relativePosition.x = " << newCurrentReferenceInPrintList->vertex4relativePosition.x << endl;
		cout << "newCurrentReferenceInPrintList->vertex4relativePosition.y = " << newCurrentReferenceInPrintList->vertex4relativePosition.y << endl;
		cout << "newCurrentReferenceInPrintList->vertex4relativePosition.z = " << newCurrentReferenceInPrintList->vertex4relativePosition.z << endl;
		*/

		Reference * newDispayReference;

		newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = DAT_FILE_COLOUR_BLACK;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;

		newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = DAT_FILE_COLOUR_BLACK;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;

		newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = DAT_FILE_COLOUR_BLACK;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x + width/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;

		newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = DAT_FILE_COLOUR_BLACK;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = vect->y + height/2.0;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = vect->z;

		newCurrentReferenceInPrintList->vertex2relativePosition.x = vect->x - width/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = vect->y - height/2.0;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = vect->z;


		newDispayReference = new Reference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		int numSpritesAdded;	//not used
		vec positionLDR;
		positionLDR.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
		positionLDR.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionLDR.z = vect->z  -GIA_OUTPUT_Z_POSITION_NODES;
		newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(text, newCurrentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
	}
	
	if(initialiseOrPrint[DRAW_CREATE_SVG_REFERENCES] == true)
	{	
		vec positionSVG;
		positionSVG.x = vect->x + GIA_DRAW_BASICENTITY_NODE_WIDTH/2;
		positionSVG.y = vect->y;	
		positionSVG.z = GIA_OUTPUT_Z_POSITION_NODES;
		writeSVGBox(writeFileObject, &positionSVG, width, height, colour, GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG*thickness, GIA_USE_SVG_ELLIPTICAL_BOXES);
		positionSVG.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
		positionSVG.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_TEXT;
		writeSVGText(writeFileObject, *text, &positionSVG, SVG_SCALE_FACTOR*SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
	}
	
	return newCurrentReferenceInPrintList;
}





void printGIAnetworkNodes(vector<GIAEntityNode*> *entityNodesCompleteList, int width, int height, string outputFileNameLDR, string outputFileNameSVG, string outputFileNamePPM, bool display, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile)
{//most of this is copied from CSexecFlow.cpp
	
	char * outputFileNameLDRcharstar = const_cast<char*>(outputFileNameLDR.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputFileNamePPM.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputFileNameSVG.c_str());

	ofstream * writeFileObject;
	if(useOutputSVGFile)
	{
		writeFileObject = new ofstream(outputFileNameSVGcharstar);
		writeSVGHeader(writeFileObject);
	}
	
	bool initialiseOrPrint[3];
	initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] = false;
	initialiseOrPrint[DRAW_CREATE_SVG_REFERENCES] = false;
	initialiseOrPrint[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = false;
	if(useOutputLDRFile || display)
	{// || useOutputPPMFile - implied
		initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] = true;
		initialiseOrPrint[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}
	if(useOutputSVGFile)
	{
		initialiseOrPrint[DRAW_CREATE_SVG_REFERENCES] = true;
		initialiseOrPrint[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}
	
	Reference * firstReferenceInPrintList = new Reference();
	determineBasicPrintPositionsOfAllNodes(entityNodesCompleteList, initialiseOrPrint, firstReferenceInPrintList, writeFileObject);
	/*
	//cout << "h1" << endl;
	initialiseOrPrint = DRAW_PRINT;
	determineBasicPrintPositionsOfAllNodes(entityNodesCompleteList, initialiseOrPrint, firstReferenceInPrintList, writeFileObject);
	*/
		


	//cout << "h2" << endl;

	if(useOutputSVGFile)
	{
		writeSVGFooter(writeFileObject);
		writeFileObject->close();
		delete writeFileObject;
	}
	
	//cout << "h3" <<endl;
	

	if(initialiseOrPrint[DRAW_CREATE_LDR_REFERENCES] == true)
	{
		writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
	}
	
	if(display)
	{
	
		//re-parse, then re-write to create a collapsed referencelist file...
		//method1:
		char * topLevelSceneFileName = outputFileNameLDRcharstar;
		char * topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
		Reference * initialReferenceInSceneFile = new Reference();
		Reference * topLevelReferenceInSceneFile = new Reference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
			exit(0);
		}
		write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
		/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
		char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
		*/

		//cout << "has" << endl;


		unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

		//setViewPort2DOrtho(-100.0, 2000.0, -100.0, 2000.0);
		setViewPort3DOrtho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

		//now reparse file
		Reference * initialReferenceInCollapsedSceneFile = new Reference();
		Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
			exit(0);
		}

		drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
		drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			//due to opengl code bug, need to execute this function twice.

		if(useOutputPPMFile)
		{
			generatePixmapFromRGBMap(displayFileNamePPMcharstar, width, height, rgbMap);
		}
		
		delete rgbMap;

	}
	else
	{
		//must use an external program to view the .ldr file (Eg LDView)
	}



}



/*
static double GIA_OUTPUT_Z_POSITION_FILE_CONNECTIONS;
static double GIA_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX;
static double GIA_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS;
static double GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_BOX;
static double GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT;

static double GIA_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X;
static double GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR;
static double GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG;
static double GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B;
static double GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG;
static double GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG;

static double GIA_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR;
static double GIA_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR;
static double GIA_FILE_MAX_TEXT_LENGTH;
static double GIA_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH;
static double GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION;
static double GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION;
static double GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X;
static double GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B;
static double GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B;
static double GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C;
static double GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG;

static double GIA_FUNCTION_VECTOROBJECTS_SCALE_FACTOR;
static double GIA_FUNCTION_MAX_TEXT_LENGTH;
static double GIA_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH;
static double GIA_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION;
static double GIA_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG;

static int GIA_LAYER_0_COLOUR;
static int GIA_LAYER_1_COLOUR;
static int GIA_LAYER_2_COLOUR;
static int GIA_LAYER_3_COLOUR;
static int GIA_LAYER_4_COLOUR;
static int GIA_LAYER_5_COLOUR;
static int GIA_LAYER_6_COLOUR;
static int GIA_LAYER_7_COLOUR;
static int GIA_LAYER_8_COLOUR;
static int GIA_LAYER_9_COLOUR;
static int GIA_LAYER_10_COLOUR;
static int GIA_LAYER_11_COLOUR;
static int GIA_LAYER_12_COLOUR;

static int GIA_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR;
static int GIA_FUNCTION_BOX_HIGHLIGHT_COLOUR;



void fillInGIARulesExternVariables()
{
	//extract common sprite variables from either xml file (LRRC or ANN)

	RulesClass * currentReferenceRulesClass = CSrulesDraw;

	while(currentReferenceRulesClass->next != NULL)
	{

		if(currentReferenceRulesClass->name == GIA_OUTPUT_Z_POSITION_FILE_CONNECTIONS_NAME)
		{
			GIA_OUTPUT_Z_POSITION_FILE_CONNECTIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX_NAME)
		{
			GIA_OUTPUT_Z_POSITION_FILE_CONTAINER_BIG_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS_NAME)
		{
			GIA_OUTPUT_Z_POSITION_FUNCTION_CONNECTIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_BOX_NAME)
		{
			GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_BOX = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT_NAME)
		{
			GIA_OUTPUT_Z_POSITION_FILE_AND_FUNCTION_TEXT = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_SCALE_FACTOR_X = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_LDR  = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_TEXT_SCALE_FACTOR_Y_SVG_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_X_SPACING_FRACTION_SVG = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG_NAME)
		{
			GIA_FILE_OR_FUNCTION_TEXT_BOX_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_SVG = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			GIA_FILE_FUNCTIONS_DISABLED_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_MAX_TEXT_LENGTH_NAME)
		{
			GIA_FILE_MAX_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME)
		{
			GIA_FILE_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME)
		{
			GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			GIA_FILE_FUNCTIONS_DISABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_X_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_X_SPACING_FRACTION_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_B = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C_NAME)
		{
			GIA_FILE_FUNCTIONS_ENABLED_LARGE_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_C = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME)
		{
			GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}



		else if(currentReferenceRulesClass->name == GIA_FUNCTION_VECTOROBJECTS_SCALE_FACTOR_NAME)
		{
			GIA_FUNCTION_VECTOROBJECTS_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FUNCTION_MAX_TEXT_LENGTH_NAME)
		{
			GIA_FUNCTION_MAX_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH_NAME)
		{
			GIA_FUNCTION_TEXT_BOX_PADDING_FRACTION_OF_TEXT_LENGTH = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION_NAME)
		{
			GIA_FUNCTION_TEXT_BOX_SCALE_FACTOR_Y_SPACING_FRACTION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG_NAME)
		{
			GIA_FUNCTION_TEXT_BOX_OUTLINE_WIDTH_SVG = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == GIA_LAYER_0_COLOUR_NAME)
		{
			GIA_LAYER_0_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_1_COLOUR_NAME)
		{
			GIA_LAYER_1_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_2_COLOUR_NAME)
		{
			GIA_LAYER_2_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_3_COLOUR_NAME)
		{
			GIA_LAYER_3_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_4_COLOUR_NAME)
		{
			GIA_LAYER_4_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_5_COLOUR_NAME)
		{
			GIA_LAYER_5_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_6_COLOUR_NAME)
		{
			GIA_LAYER_6_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_7_COLOUR_NAME)
		{
			GIA_LAYER_7_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_8_COLOUR_NAME)
		{
			GIA_LAYER_8_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_9_COLOUR_NAME)
		{
			GIA_LAYER_9_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_10_COLOUR_NAME)
		{
			GIA_LAYER_10_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_11_COLOUR_NAME)
		{
			GIA_LAYER_11_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_LAYER_12_COLOUR_NAME)
		{
			GIA_LAYER_12_COLOUR = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == GIA_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR_NAME)
		{
			GIA_FUNCTION_CONNECTION_HIGHLIGHT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == GIA_FUNCTION_BOX_HIGHLIGHT_COLOUR_NAME)
		{
			GIA_FUNCTION_BOX_HIGHLIGHT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

}
*/


