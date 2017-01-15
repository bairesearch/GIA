/*******************************************************************************
 *
 * File Name: GIAquery.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAdraw.h"
#include "XMLrulesClass.h"


void determineBasicPrintPositionsOfAllNodes(vector<GIAEntityNode*> *indexOfEntityNodes, int initialiseOrPrint, Reference * firstReferenceInPrintList, ofstream * writeFileObject)
{
	vector<GIAEntityNode*>::iterator entityIter;
	
	Reference * currentReferenceInPrintList = firstReferenceInPrintList;
	
	initiateMaxXAtAParticularY();
	int xInitial = DRAW_X_INITIAL_OFFSET;
	int yInitial = DRAW_Y_INITIAL_OFFSET;
	maxXAtAParticularY[yInitial] = xInitial;
	//first pass; determine maxXAtAParticularY	[and use these to centre each row {at a given y} respectively]
	for (entityIter = indexOfEntityNodes->begin(); entityIter != indexOfEntityNodes->end(); entityIter++) 
	{
		if(!((*entityIter)->initialisedForPrinting))
		{
			cout << "tracing..." << endl;
				
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



Reference * initialiseEntityNodeForPrinting(GIAEntityNode * entityNode, int y, int x, int initialiseOrPrint, Reference * currentReferenceInPrintList, ofstream * writeFileObject)
{
	
	//if(!(entityNode->initialisedForPrinting) || (entityNode->printY < y))
	if(!(entityNode->initialisedForPrinting))
	{
		
		if(entityNode->isProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}		
		else if(entityNode->hasAssociatedProperty)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedPropertyIsAction)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "entityNode = " << entityNode->entityName << endl;
		}

		
		/*
		cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
		cout << "\tentityNode->isProperty = " << entityNode->isProperty << endl;
		cout << "\tentityNode->hasAssociatedProperty = " << entityNode->hasAssociatedProperty << endl;
		cout << "\tentityNode->hasAssociatedPropertyIsAction = " << entityNode->hasAssociatedPropertyIsAction << endl;
		*/
		
		entityNode->initialisedForPrinting = true;
		
		maxXAtAParticularY[y] = maxXAtAParticularY[y] + DRAW_X_SPACE_BETWEEN_ENTITIES;	//only used, for indepdendent network visualisation (eg, when rendering next sentence)
		
		entityNode->printX = x;
		entityNode->printY = y;
		
		
		int q, r;
	
		vec pos1;
		vec pos2;
		vec pos3;
		vec pos4;
		vec pos5;
		
		pos1.x = entityNode->printX;
		pos1.y = entityNode->printY;	
		pos1.z = DRAW_CONNECTION_Z;
		
		//cout << "a1" << endl;
				
		//action connections;
		q = -DRAW_Y_SPACE_BETWEEN_ACTION_NODES;
		r = -DRAW_X_SPACE_BETWEEN_ACTION_NODES;		
		vector<GIAEntityNode*>::iterator actionIter;
		for(actionIter = entityNode->IncomingActionNodeList.begin(); actionIter != entityNode->IncomingActionNodeList.end(); actionIter++) 
		{
			//cout << "A" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*actionIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			//cout << "AA" << endl;
			q = q + DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE;
		}
		q = DRAW_Y_SPACE_BETWEEN_ACTION_NODES;
		r = DRAW_X_SPACE_BETWEEN_ACTION_NODES;				
		for(actionIter = entityNode->ActionNodeList.begin(); actionIter != entityNode->ActionNodeList.end(); actionIter++) 
		{
			//cout << "AAA" << endl;	
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*actionIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			//cout << "AAAA" << endl;
			q = q + DRAW_Y_SPACE_BETWEEN_ACTIONS_OF_SAME_NODE;
		}	




		q = -DRAW_Y_SPACE_BETWEEN_ACTION_NODES;
		r = -DRAW_X_SPACE_BETWEEN_ACTION_NODES;			
		if(entityNode->actionSubjectEntity != NULL)
		{		
			//cout << "b5" << endl;
			//cout << "entityNode->actionSubjectEntity->initialisedForPrinting = " << entityNode->actionSubjectEntity->initialisedForPrinting << endl;
			//cout << "entityNode->actionSubjectEntity->entityName = " << entityNode->actionSubjectEntity->entityName << endl;
			//cout << "entityNode->actionObjectEntity->entityName = " << entityNode->actionObjectEntity->entityName << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting(entityNode->actionSubjectEntity, y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			//cout << "b6" << endl;
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos3.x = entityNode->actionSubjectEntity->printX;
				pos3.y = entityNode->actionSubjectEntity->printY;	
				pos3.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos3, GIA_DRAW_ACTION_SUBJECT_CONNECTION_COLOUR, writeFileObject, "subject");
			}		
		}
		q = DRAW_Y_SPACE_BETWEEN_ACTION_NODES;
		r = DRAW_X_SPACE_BETWEEN_ACTION_NODES;				
		if(entityNode->actionObjectEntity != NULL)
		{		
			//cout << "b7" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting(entityNode->actionObjectEntity, y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);	
			//cout << "b8" << endl;
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...

				pos4.x = entityNode->actionObjectEntity->printX;
				pos4.y = entityNode->actionObjectEntity->printY;	
				pos4.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos4, GIA_DRAW_ACTION_OBJECT_CONNECTION_COLOUR, writeFileObject, "object");
			}		
		}
					
		
		//cout << "a2" << endl;
		
		//conditions connections
		q = DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
		r = DRAW_X_SPACE_BETWEEN_CONDITION_NODES;
		vector<GIAEntityNode*>::iterator ConditionIter;
		vector<string>::iterator ConditionNodeTypeListIterator = entityNode->ConditionNodeTypeList.begin();
		for(ConditionIter = entityNode->ConditionNodeList.begin(); ConditionIter != entityNode->ConditionNodeList.end(); ConditionIter++) 
		{	
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*ConditionIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			q = q+DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;
			
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos5.x = (*ConditionIter)->printX;
				pos5.y = (*ConditionIter)->printY;	
				pos5.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos5, GIA_DRAW_CONDITION_CONNECTION_COLOUR, writeFileObject, *ConditionNodeTypeListIterator);
			}
			ConditionNodeTypeListIterator++;			
		}				
		if(entityNode->timeConditionNode != NULL)
		{	
			if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				//cout << "b7" << endl;
				int timeConditionNodePrintX = x+r;
				int timeConditionNodePrintY = y+q;
				currentReferenceInPrintList = initialiseTimeConditionNodeForPrinting(entityNode->timeConditionNode, timeConditionNodePrintY, timeConditionNodePrintX, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
				
				q = q+DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;
				
				//cout << "b8" << endl;
				if(initialiseOrPrint == DRAW_PRINT)
				{	
					//may accidentially overwrite adjacent nodes that have already been printed here; be careful...

					pos5.x = timeConditionNodePrintX;
					pos5.y = timeConditionNodePrintY;	
					pos5.z = DRAW_CONNECTION_Z;
					currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos5, GIA_DRAW_CONDITION_CONNECTION_COLOUR, writeFileObject, "time");
				}
			}		
		}
				
		//go reverse also...
		q = DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
		r = DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
		ConditionNodeTypeListIterator = entityNode->ConditionNodeTypeReverseList.begin();
		for(ConditionIter = entityNode->ConditionNodeReverseList.begin(); ConditionIter != entityNode->ConditionNodeReverseList.end(); ConditionIter++) 
		{	
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*ConditionIter), y-q, x-r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			q = q+DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;
		
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos5.x = (*ConditionIter)->printX;
				pos5.y = (*ConditionIter)->printY;	
				pos5.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos5, GIA_DRAW_CONDITION_CONNECTION_COLOUR, writeFileObject, *ConditionNodeTypeListIterator);
			}	
			ConditionNodeTypeListIterator++;		
		}
				
		
		
		//cout << "a3" << endl;
		
		//property connections	
		vector<GIAEntityNode*>::iterator entityIter;
		q = DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES;
		r = DRAW_X_SPACE_BETWEEN_PROPERTY_NODES;		
		for(entityIter = entityNode->PropertyNodeList.begin(); entityIter != entityNode->PropertyNodeList.end(); entityIter++) 
		{//DRAW SHOULD NOT BE REQUIRED	
			//cout << "a31" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			q = q+DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE;
		}
		/*this has been removed 25 Sept - use entityNodeContainingThisProperty instead
		//go reverse also...
		q = -DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES;
		r = -DRAW_X_SPACE_BETWEEN_PROPERTY_NODES;			
		for(entityIter = entityNode->PropertyNodeReverseList.begin(); entityIter != entityNode->PropertyNodeReverseList.end(); entityIter++) 
		{//DRAW SHOULD NOT BE REQUIRED
			//cout << "a32" << endl;	
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			q = q+DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE;			//this was - not +
		}
		*/	
		//cout << "a3b" << endl;
		//go upwards also...
		q = -DRAW_Y_SPACE_BETWEEN_PROPERTY_DEFINITION_NODES;
		r = -DRAW_X_SPACE_BETWEEN_PROPERTY_DEFINITION_NODES;		
		if(entityNode->entityNodeDefiningThisPropertyOrAction != NULL)
		{
			//cout << "a33" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting(entityNode->entityNodeDefiningThisPropertyOrAction, y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos2.x = entityNode->entityNodeDefiningThisPropertyOrAction->printX;
				pos2.y = entityNode->entityNodeDefiningThisPropertyOrAction->printY;	
				pos2.z = DRAW_CONNECTION_Z;							
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos2, GIA_DRAW_PROPERTY_DEFINITION_CONNECTION_COLOUR, writeFileObject, "instance");
			}
				
		}
		//cout << "a3c" << endl;
		q = -DRAW_Y_SPACE_BETWEEN_PROPERTY_NODES;		//this used to be - not +
		r = -DRAW_X_SPACE_BETWEEN_PROPERTY_NODES;			
		if(entityNode->entityNodeContainingThisProperty != NULL)
		{
			//cout << "a34" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting(entityNode->entityNodeContainingThisProperty, y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos3.x = entityNode->entityNodeContainingThisProperty->printX;
				pos3.y = entityNode->entityNodeContainingThisProperty->printY;	
				pos3.z = DRAW_CONNECTION_Z;								
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos3, GIA_DRAW_PROPERTY_CONNECTION_COLOUR, writeFileObject, "property");
			}
				
		}
		//cout << "a4" << endl;
		

		
		//cout << "a5" << endl;
		
		q = -DRAW_Y_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;
		r = -DRAW_X_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;				
		for(entityIter = entityNode->EntityNodeDefinitionList.begin(); entityIter != entityNode->EntityNodeDefinitionList.end(); entityIter++) 
		{
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			if(initialiseOrPrint == DRAW_PRINT)
			{	
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				pos4.x = (*entityIter)->printX;
				pos4.y = (*entityIter)->printY;	
				pos4.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos4, GIA_DRAW_BASICENTITY_CONNECTION_COLOUR, writeFileObject, "is");
			}
			q = q+DRAW_Y_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;

		}
		//go reverse also...
		q = DRAW_Y_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;
		r = DRAW_X_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;			
		for(entityIter = entityNode->EntityNodeDefinitionReverseList.begin(); entityIter != entityNode->EntityNodeDefinitionReverseList.end(); entityIter++) 
		{//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			q = q+DRAW_Y_SPACE_BETWEEN_ENTITIES_OF_SAME_NODE;
		}
		//cout << "a6" << endl;
		
				
		//DRAW SHOULD NOT BE REQUIRED, as this should be performed when drilling down into them 
		//associated actions and properties [ie does this entity also define an action/verb or a property/adjective? [ie, it is not just a thing/noun]]
		q = DRAW_Y_SPACE_BETWEEN_PROPERTY_DEFINITION_NODES;
		r = DRAW_X_SPACE_BETWEEN_PROPERTY_DEFINITION_NODES;	//this used to be - not +		
		for(entityIter = entityNode->AssociatedPropertyNodeList.begin(); entityIter != entityNode->AssociatedPropertyNodeList.end(); entityIter++) 
		{
			//cout << "as0" << endl;
			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), y+q, x+r, initialiseOrPrint, currentReferenceInPrintList, writeFileObject);
			//cout << "as1" << endl;
			q = q+DRAW_Y_SPACE_BETWEEN_PROPERTIES_OF_SAME_NODE;
		}	
		
		//cout << "a7" << endl;
		
		if(initialiseOrPrint == DRAW_PRINT)
		{	
			//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
			
			int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
			
			int entityColour;
			if(entityNode->hasAssociatedPropertyIsAction)
			{
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_ACTION_DEFINITION_NODE_COLOUR;	//clearly identify the definition of the action
				}
			}			
			else if(entityNode->isProperty)
			{
				if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
				{
					boxThickness = GIA_DRAW_THICKNESS_THICK;
				}
				
				if(entityNode->hasMeasure)
				{
					entityColour = GIA_DRAW_PROPERTY_MEASURE_NODE_COLOUR;
				}
				else if(entityNode->hasQuantity)
				{
					entityColour = GIA_DRAW_PROPERTY_QUANTITY_NODE_COLOUR;
				}
				else
				{
					entityColour = GIA_DRAW_PROPERTY_NODE_COLOUR;
				}
			}
			else if(entityNode->isAction)
			{
				entityColour = GIA_DRAW_ACTION_NODE_COLOUR;	
			}			
			else if(entityNode->hasAssociatedTime)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
			}
			/*
			else if(entityNode->hasConditionTime)
			{
				entityColour = GIA_DRAW_CONDITION_DEFINITION_NODE_COLOUR;	//clear identify a time node
			}
			*/		
			else if(entityNode->hasAssociatedProperty)
			{//the original spec seemed to imply that entities that have associated properties (ie, that define properties) are special but they don't appear to be
				if(!(entityNode->isProperty))
				{		
					//added 2 May 11a (highlight entities which define property nodes)
					entityColour = GIA_DRAW_PROPERTY_DEFINITION_NODE_COLOUR;	//OLD: no colour modifier, just use basic entity colour; GIA_DRAW_BASICENTITY_NODE_COLOUR;
				}
			}					
			else
			{	
				entityColour = GIA_DRAW_BASICENTITY_NODE_COLOUR;
			}
										
			//first, print this action node.
			string nameOfBox = "";
			if(entityNode->hasQuantity)
			{
				char quantityNumberStringcharstar[20];
				if(entityNode->isQuery)
				{
					strcpy(quantityNumberStringcharstar, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_TEMP_FOR_DISPLAY_ONLY);
				}
				else
				{
					
					sprintf(quantityNumberStringcharstar, "%d", entityNode->quantityNumber);
					
				}
				nameOfBox = nameOfBox + quantityNumberStringcharstar + " " + entityNode->entityName;
				
			}
			else
			{
				nameOfBox = entityNode->entityName;
			}
			currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, writeFileObject, boxThickness);

		}
		
		//cout << "a8" << endl;	
		
		if(entityNode->isProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is property)" << endl;		
		}
		else if(entityNode->isAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;		
		}		
		else if(entityNode->hasAssociatedProperty)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property)" << endl;	
		}
		else if(entityNode->hasAssociatedPropertyIsAction)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated property is action)" << endl;
		} 
		else if(entityNode->hasAssociatedTime)
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated time)" << endl;	
		}
		else
		{
			cout << "Exiting: entityNode = " << entityNode->entityName << endl;
		}
							
	}
	//cout << "a0c" << endl;
	
	return currentReferenceInPrintList;	//does this need to be newCurrentReferenceInPrintList?
		
}
