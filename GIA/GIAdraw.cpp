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
 * File Name: GIAdraw.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p2a 12-December-2016
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

#include <GL/freeglut.h>	//need to use freeglut as it contains extensions functions which glut does not have; glutMainLoopEvent()

int maxXAtAParticularY[MAX_GIA_TREE_DEPTH];

void printGIAnetworkNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, int width, int height, string outputFileNameLDR, string outputFileNameSVG, string outputFileNamePPM, bool display, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputSVGfile, int maxNumberSentences)
{//most of this is copied from CSexecFlow.cpp
	bool result = true;

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();
	XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;

	bool printType[3];
	printType[DRAW_CREATE_LDR_REFERENCES] = false;
	printType[DRAW_CREATE_SVG_REFERENCES] = false;
	printType[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = false;
	if(useOutputLDRfile || display)
	{// || useOutputPPMfile - implied
		printType[DRAW_CREATE_LDR_REFERENCES] = true;
		printType[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}
	if(useOutputSVGfile)
	{
		printType[DRAW_CREATE_SVG_REFERENCES] = true;
		printType[DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}

	LDreference* firstReferenceInPrintList = new LDreference();
	determineBasicPrintPositionsOfAllNodes(entityNodesActiveListComplete, printType, firstReferenceInPrintList, &currentTagInSVGFile, maxNumberSentences);

	if(useOutputSVGfile)
	{
		if(!writeSVGfile(outputFileNameSVG, firstTagInSVGFile))
		{
			result = false;
		}
		delete firstTagInSVGFile;
	}

	if(printType[DRAW_CREATE_LDR_REFERENCES] == true)
	{
		writeReferencesToFile(outputFileNameLDR, firstReferenceInPrintList);
	}

	if(display)
	{

		//re-parse, then re-write to create a collapsed referencelist file...
		//method1:
		string topLevelSceneFileName = outputFileNameLDR;
		string topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
		LDreference* initialReferenceInSceneFile = new LDreference();
		LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
			exit(0);
		}
		write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
		/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
		char* topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
		*/

		#ifdef GIA_FREE_MEMORY1
		delete initialReferenceInSceneFile;
		delete topLevelReferenceInSceneFile;
		#endif


		unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

		setViewPort3Dortho(-100.0, width-100, height-100, -100.0, 1.0, -1.0);	//-100 is used to display the most left semantic network nodes

		//now reparse file
		LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
		LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
		{//file does not exist
			cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
			exit(0);
		}

		drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
		drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			//due to opengl code bug, need to execute this function twice.

		#ifdef GIA_FREE_MEMORY1
		//delete initialReferenceInCollapsedSceneFile;	//this cannot be deleted because it is still used by glutDisplayFunc
		delete topLevelReferenceInCollapsedSceneFile;
		#endif

		if(useOutputPPMfile)
		{
			generatePixmapFromRGBmap(outputFileNamePPM, width, height, rgbMap);
		}

		delete rgbMap;

	}
	else
	{
		//must use an external program to view the .ldr file (Eg LDView)
	}


	#ifdef GIA_FREE_MEMORY1
	delete firstReferenceInPrintList;
	#endif
}

void initiateMaxXAtParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_GIA_TREE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}


void determineBasicPrintPositionsOfAllNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, bool printType[], LDreference* firstReferenceInPrintList, XMLparserTag** currentTag, int maxNumberSentences)
{
	LDreference* currentReferenceInPrintList = firstReferenceInPrintList;

	initiateMaxXAtParticularY();
	int xInitial = DRAW_X_INITIAL_OFFSET;
	int yInitial = DRAW_Y_INITIAL_OFFSET;
	maxXAtAParticularY[yInitial] = xInitial;
	//first pass; determine maxXAtAParticularY	[and use these to centre each row {at a given y} respectively]

	#ifndef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	maxNumberSentences = 1;
	#endif
	bool printFromXMLcoordinatesAlreadyDefined = false;
	if((*(entityNodesActiveListComplete->begin()))->printCoordsAlreadyDefined)
	{
		printFromXMLcoordinatesAlreadyDefined = true;
		maxNumberSentences = 1;
	}
	#ifdef GIA_DEBUG
	//cout << "maxNumberSentences = " << maxNumberSentences << endl;
	#endif
	for(int sentenceIndex=GIA_NLP_START_SENTENCE_INDEX; sentenceIndex <= maxNumberSentences; sentenceIndex++)
	{
		#ifdef GIA_DRAW_DEBUG
		cout << "*** sentenceIndex = " << sentenceIndex << endl;
		#endif
		for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
		{
			#ifdef GIA_DRAW_DEBUG
			cout << "\ttracing..." << (*entityIter)->entityName << endl;
			#endif

			//initiateMaxXAtParticularY();
			xInitial = maxXAtAParticularY[yInitial];
			//xInitial = sentenceIndex*100;	//compact

			bool thisIsDefinitionAndPreviousNodeWasInstance = false;

			currentReferenceInPrintList = initialiseEntityNodeForPrinting((*entityIter), yInitial, xInitial, printType, currentReferenceInPrintList, currentTag, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);
		}
	}
}

LDreference* initialiseEntityConnectionForPrinting(vec* pos1, GIAentityConnection* entityConnection, LDreference* currentReferenceInPrintList, bool printType[], string connectionName, int entityConnectionColour, XMLparserTag** currentTag)
{
	GIAentityNode* entityNodeToConnect = entityConnection->entity;

	#ifdef GIA_DEBUG
	//cout << "initialiseEntityConnectionForPrinting: entityNodeToConnect->entityName = " << entityNodeToConnect->entityName << endl;
	#endif

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	if((entityNodeToConnect->initialisedForPrinting) && !(entityConnection->initialisedForPrinting) && !(entityNodeToConnect->disabled))
	//added condition 31 August 2013: only print connection if node has been printed
	#else
	if(!(entityNodeToConnect->disabled))
	#endif
	{
		#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
		entityConnection->initialisedForPrinting = true;
		#endif

		//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
		vec pos2;
		pos2.x = entityNodeToConnect->printX;
		pos2.y = entityNodeToConnect->printY;
		pos2.z = DRAW_CONNECTION_Z;

		//connectionName = connectionName + convertIntToString(entityConnection->sentenceIndexTemp);

		currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, pos1, &pos2, entityConnectionColour, currentTag, connectionName, printType);
	}

	return currentReferenceInPrintList;
}


LDreference* initialiseEntityNodeForPrinting(GIAentityNode* entityNode, int y, int x, bool printType[], LDreference* currentReferenceInPrintList, XMLparserTag** currentTag, int sentenceIndex, bool thisIsDefinitionAndPreviousNodeWasInstance)
{
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	if((entityNode->sentenceIndexTemp == sentenceIndex) || (entityNode->wasReference) || thisIsDefinitionAndPreviousNodeWasInstance || entityNode->printCoordsAlreadyDefined)	//condition (entityNode->wasReference) added 12 October 2012 1q3b
	#else
	if((entityNode->sentenceIndexTemp == sentenceIndex) || thisIsDefinitionAndPreviousNodeWasInstance || entityNode->printCoordsAlreadyDefined)
	#endif
	{
	#endif
		//if(!(entityNode->initialisedForPrinting) || (entityNode->printY < y))
		if(!(entityNode->initialisedForPrinting) && !(entityNode->disabled))
		{
			#ifdef GIA_DEBUG
			//cout << "initialiseEntityNodeForPrinting: entityNode->entityName = " << entityNode->entityName << endl;
			/*
			cout << "\nentityNode->sentenceIndexTemp = " << entityNode->sentenceIndexTemp << endl;
			cout << "entityNode->entityName = " << entityNode->entityName << endl;
			cout << "entityNode->wasReference = " << entityNode->wasReference << endl;
			if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
			{
				cout << "entityNode = " << entityNode->entityName << " (is networkIndex)" << endl;
			}
			if(entityNode->wasReference)
			{
				cout << "entityNode = " << entityNode->entityName << " (wasReference)" << endl;
			}
			*/
			#endif

			#ifdef GIA_DRAW_DEBUG
			if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
			{
				cout << "entityNode = " << entityNode->entityName << " (is substance)" << endl;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
			{
				cout << "entityNode = " << entityNode->entityName << " (is action)" << endl;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
			{
				cout << "entityNode = " << entityNode->entityName << " (is condition)" << endl;
			}
			else if(entityNode->hasAssociatedInstance)
			{
				cout << "entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
			}
			else if(entityNode->hasAssociatedTime)
			{
				cout << "entityNode = " << entityNode->entityName << " (has associated time)" << endl;
			}
			else
			{
				cout << "entityNode = " << entityNode->entityName << endl;
			}
			//cout << "\tentityNode->isAction = " << entityNode->isAction << endl;
			//cout << "\tentityNode->isSubstance = " << entityNode->isSubstance << endl;
			//cout << "\tentityNode->hasAssociatedInstance = " << entityNode->hasAssociatedInstance << endl;
			#endif


			entityNode->initialisedForPrinting = true;

			maxXAtAParticularY[y] = maxXAtAParticularY[y] + DRAW_X_SPACE_BETWEEN_ENTITIES;	//only used, for indepdendent network visualisation (eg, when rendering next sentence)

			if(entityNode->printCoordsAlreadyDefined)
			{
				x = entityNode->printX;
				y = entityNode->printY;
			}
			else
			{
				entityNode->printX = x;
				entityNode->printY = y;
			}

			int q, r;

			vec pos1;

			pos1.x = entityNode->printX;
			pos1.y = entityNode->printY;
			pos1.z = DRAW_CONNECTION_Z;


			int entityDefinitionConnectionColour;
			if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
			{
				entityDefinitionConnectionColour = GIA_DRAW_SUBSTANCE_DEFINITION_CONNECTION_COLOUR;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
			{
				entityDefinitionConnectionColour = GIA_DRAW_ACTION_DEFINITION_CONNECTION_COLOUR;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
			{
				entityDefinitionConnectionColour = GIA_DRAW_CONDITION_DEFINITION_CONNECTION_COLOUR;
			}
			else
			{
				entityDefinitionConnectionColour = GIA_DRAW_SUBSTANCE_DEFINITION_CONNECTION_COLOUR;
			}

			for(int i=0; i<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; i++)
			{
				int q = entityVectorConnectionDrawPosYinitialArray[i];
				int r = entityVectorConnectionDrawPosXinitialArray[i];

				for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[i].begin(); connectionIter != entityNode->entityVectorConnectionsArray[i].end(); connectionIter++)
				{
					int qTemp = q;
					int rTemp = r;
					#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
					if(isActionSpecialPossessive(entityNode) || isActionSpecialPossessive((*connectionIter)->entity))
					{
						if((i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT) || (i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT))
						{
							qTemp = r/4;
							rTemp = q/4;
						}
					}
					#endif

					#ifdef GIA_DEBUG
					//cout << "\ti = " << i << ", initialiseEntityNodeForPrinting; " << (*connectionIter)->entity->entityName << endl;
					#endif
					bool thisIsDefinitionAndPreviousNodeWasInstance = false;
					#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
					{
						thisIsDefinitionAndPreviousNodeWasInstance = true;
					}
					#endif
					currentReferenceInPrintList = initialiseEntityNodeForPrinting((*connectionIter)->entity, y+qTemp, x+rTemp, printType, currentReferenceInPrintList, currentTag, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);

					bool pass = true;
					int entityConnectionColour = entityVectorConnectionDrawColourNameArray[i];
					if(i == GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE)
					{
						if(printType[DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
						{
							entityConnectionColour = entityDefinitionConnectionColour;
						}
						else
						{
							pass = false;
						}
					}
					#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
					if((*connectionIter)->isAlias)
					{
						entityConnectionColour = GIA_DRAW_DEFINITION_MARK_AS_ALIAS_CONNECTION_COLOUR;
					}
					#endif
					if(pass)
					{
						if(entityVectorConnectionDrawConnectionArray[i])
						{
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
							if((*connectionIter)->sameReferenceSet)
							{
								entityConnectionColour = DAT_FILE_COLOUR_GREEN;
							}
							else
							{
								entityConnectionColour = DAT_FILE_COLOUR_RED;
							}
							#endif

							#ifdef GIA_DRAW_PRINT_CONNECTION_SENTENCE_INDICES
							//string connectionName = convertIntToString((*connectionIter)->sentenceIndexTemp);
							string connectionName = string("s") + convertIntToString((*connectionIter)->sentenceIndexTemp) + entityVectorConnectionNameArray[i];
							currentReferenceInPrintList = initialiseEntityConnectionForPrinting(&pos1,* connectionIter, currentReferenceInPrintList, printType, connectionName, entityConnectionColour, currentTag);
							#else
							currentReferenceInPrintList = initialiseEntityConnectionForPrinting(&pos1,* connectionIter, currentReferenceInPrintList, printType, entityVectorConnectionDrawConnectionNameArray[i], entityConnectionColour, currentTag);
							#endif
						}
					}

					q = q + entityVectorConnectionDrawPosYspacingArray[i];
				}
			}

			q = DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
			r = DRAW_X_SPACE_BETWEEN_CONDITION_NODES;
			if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				int timeConditionNodePrintX = x+r;
				int timeConditionNodePrintY = y+q;
				currentReferenceInPrintList = initialiseTimeConditionNodeForPrinting(entityNode->timeConditionNode, timeConditionNodePrintY, timeConditionNodePrintX, printType, currentReferenceInPrintList, currentTag);

				q = q+DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;

				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				vec pos2;
				pos2.x = timeConditionNodePrintX;
				pos2.y = timeConditionNodePrintY;
				pos2.z = DRAW_CONNECTION_Z;
				currentReferenceInPrintList = createReferenceConnectionWithText(currentReferenceInPrintList, &pos1, &pos2, GIA_DRAW_CONDITION_TIME_CONNECTION_COLOUR, currentTag, "time", printType);

			}


			if(printType[DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
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
				#ifdef GIA_DRAW_DISPLAY_ANSWER_CONTEXTS
				else if(entityNode->queryAnswerContext)
				{
					entityColour = GIA_DRAW_QUERY_ANSWER_CONTEXT_NODE_COLOUR;
				}
				#endif
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
				{
					if(entityNode->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						boxThickness = GIA_DRAW_THICKNESS_THICK;
					}
					if(entityNode->hasMeasure)
					{
						entityColour = GIA_DRAW_SUBSTANCE_MEASURE_NODE_COLOUR;
					}
					else if(entityNode->hasQuantity)
					{
						entityColour = GIA_DRAW_SUBSTANCE_QUANTITY_NODE_COLOUR;
					}
					else
					{
						entityColour = GIA_DRAW_SUBSTANCE_NODE_COLOUR;
					}

				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
				{
					entityColour = GIA_DRAW_CONCEPT_NODE_COLOUR;
				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_QUALITY)
				{
					entityColour = GIA_DRAW_SUBSTANCE_QUALITY_NODE_COLOUR;
				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
				{
					if(entityNode->isActionConcept)
					{
						entityColour = GIA_DRAW_ACTION_CONCEPT_NODE_COLOUR;
					}
					else
					{
						entityColour = GIA_DRAW_ACTION_NODE_COLOUR;
					}
				}
				/*
				else if(entityNode->hasAssociatedTime)
				{
					entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
				}
				*/
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
				{
					entityColour = GIA_DRAW_CONDITION_NODE_COLOUR;	//clear identify a time node
				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)
				{
					/*
					if(entityNode->hasAssociatedInstanceIsAction)
					{
						#ifdef GIA_DRAW_DEBUG
						//if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
						//{
						//	cout << "(entityNode->hasAssociatedInstanceIsAction) && (entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)" << endl;
						//}
						#endif
						if(entityNode->hasMeasure)
						{
							entityColour = GIA_DRAW_SUBSTANCE_MEASURE_NODE_COLOUR;
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
					*/
					if(entityNode->hasAssociatedInstance)
					{//the original spec seemed to imply that entities that have associated substances (ie, that define substances) are special but they don't appear to be
						//added 2 May 11a (highlight entities which define substance nodes)
						entityColour = GIA_DRAW_SUBSTANCE_DEFINITION_NODE_COLOUR;	//OLD: no colour modifier, just use basic entity colour; GIA_DRAW_NETWORK_INDEX_NODE_COLOUR;
					}
					else
					{
						entityColour = GIA_DRAW_NETWORK_INDEX_NODE_COLOUR;
					}
				}
				else
				{
					cout << "initialiseEntityNodeForPrinting{} error: illegal entityNode->entityType: " << entityNode->entityType << endl;
					exit(0);
				}

				#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
				if(entityNode->referenceSetID == GIA_REFERENCE_SET_ID_UNDEFINED)
				{
					entityColour = DAT_FILE_COLOUR_RED;
				}
				else
				{
					entityColour = DAT_FILE_COLOUR_GREEN;
				}
				#endif

				#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
				if(isActionSpecialPossessive(entityNode))
				{
					entityColour = GIA_DRAW_ACTION_SPECIAL_POSSESSIVE_NODE_COLOUR;
				}
				#endif

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
				#ifdef GIA_DRAW_PRINT_INSTANCE_ID
				//nameOfBox = convertIntToString(entityNode->referenceSetID);
				nameOfBox = nameOfBox + convertIntToString(entityNode->idInstance);
				#endif
				#ifdef GIA_DRAW_PRINT_ACTIVELIST_ID
				nameOfBox = nameOfBox + convertIntToString(entityNode->idActiveList);
				#endif

				//nameOfBox = nameOfBox + convertIntToString(entityNode->grammaticalDefiniteTemp);

				currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, currentTag, boxThickness, printType);
			}


			#ifdef GIA_DRAW_DEBUG
			if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)
			{
				cout << "Exiting: entityNode = " << entityNode->entityName << " (is substance)" << endl;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_ACTION)
			{
				cout << "Exiting: entityNode = " << entityNode->entityName << " (is action)" << endl;
			}
			else if(entityNode->entityType == GIA_ENTITY_TYPE_TYPE_CONDITION)
			{
				cout << "Exiting: entityNode = " << entityNode->entityName << " (is condition)" << endl;
			}
			else if(entityNode->hasAssociatedInstance)
			{
				cout << "Exiting: entityNode = " << entityNode->entityName << " (has associated instance)" << endl;
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
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	}
	#endif

	return currentReferenceInPrintList;	//does this need to be newCurrentReferenceInPrintList?

}




LDreference* initialiseTimeConditionNodeForPrinting(GIAtimeConditionNode* timeConditionNode, int y, int x, bool printType[], LDreference* currentReferenceInPrintList, XMLparserTag** currentTag)
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
		boxThickness = GIA_DRAW_THICKNESS_THICK;
	}
	currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, GIA_DRAW_CONDITION_TIME_NODE_COLOUR, &(timeConditionNode->conditionName), currentTag, boxThickness, printType);

	/*
	int timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_NODE_COLOUR;
	if(timeConditionNode->isProgressive)
	{
		timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_STATE_NODE_COLOUR;
	}
	currentReferenceInPrintList = createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, timeConditionNodeColour, &(timeConditionNode->conditionName), currentTag, GIA_DRAW_THICKNESS_NORMAL, printType);
	*/

	#ifdef GIA_DRAW_DEBUG
	cout << "Exiting: timeConditionNode = " << timeConditionNode->conditionName << endl;
	#endif

	return currentReferenceInPrintList;	//does this need to be newCurrentReferenceInPrintList?
}


LDreference* createReferenceConnectionWithText(LDreference* currentReferenceInPrintList, vec* pos1, vec* pos2, int colour, XMLparserTag** currentTag, string connectionTypeName, bool printType[])
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

	newCurrentReferenceInPrintList = createReferenceConnection(newCurrentReferenceInPrintList, pos1, pos2, colour, currentTag, printType);

	if(GIA_DRAW_USE_CONNECTION_TYPE_NAME_TEXT)
	{
		vec vect;
		vect.x = (pos1->x + pos2->x)/2;
		vect.y = (pos1->y + pos2->y)/2;
		vect.z = (pos1->z + pos2->z)/2;

		if(printType[DRAW_CREATE_LDR_REFERENCES] == true)
		{
			int numSpritesAdded;	//not used
			vec positionLDR;
			positionLDR.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
			positionLDR.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionLDR.z = vect.z - GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(connectionTypeName, newCurrentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
		}
		if(printType[DRAW_CREATE_SVG_REFERENCES] == true)
		{
			vec positionSVG;
			positionSVG.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
			positionSVG.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionSVG.z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			writeSVGtext(currentTag, connectionTypeName, &positionSVG, SVG_SCALE_FACTOR*SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
		}
	}

	return newCurrentReferenceInPrintList;
}

LDreference* createReferenceConnection(LDreference* currentReferenceInPrintList, vec* pos1, vec* pos2, int colour, XMLparserTag** currentTag, bool printType[])
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

	if(printType[DRAW_CREATE_LDR_REFERENCES] == true)
	{
		#ifdef GIA_DRAW_DEBUG
		//cout << "drawing connection" << endl;
		#endif

		newCurrentReferenceInPrintList->type = REFERENCE_TYPE_LINE;
		newCurrentReferenceInPrintList->colour = colour;

		newCurrentReferenceInPrintList->vertex1relativePosition.x = pos1->x;
		newCurrentReferenceInPrintList->vertex1relativePosition.y = pos1->y;
		newCurrentReferenceInPrintList->vertex1relativePosition.z = pos1->z;


		newCurrentReferenceInPrintList->vertex2relativePosition.x = pos2->x;
		newCurrentReferenceInPrintList->vertex2relativePosition.y = pos2->y;
		newCurrentReferenceInPrintList->vertex2relativePosition.z = pos2->z;

		#ifdef GIA_DRAW_DEBUG
		/*
		cout << "createFileOrFunctionReferenceConnection{}:" << endl;
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
		#endif

		LDreference* newDispayReference = new LDreference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;
	}

	if(printType[DRAW_CREATE_SVG_REFERENCES] == true)
	{
		pos1->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		pos2->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		writeSVGline(currentTag, pos1, pos2, colour);
	}

	return newCurrentReferenceInPrintList;
}



//consider using elipse instead; <ellipse cx="240" cy="100" rx="220" ry="30">

LDreference* createBox(LDreference* currentReferenceInPrintList, vec* vect, double width, double height, int colour, string* text, XMLparserTag** currentTag, int thickness, bool printType[])
{
	LDreference* newCurrentReferenceInPrintList = currentReferenceInPrintList;

	if(printType[DRAW_CREATE_LDR_REFERENCES] == true)
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

		#ifdef GIA_DRAW_DEBUG
		/*
		cout << "createFileOrFunctionReferenceBox{}:" << endl;
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
		#endif

		LDreference* newDispayReference;

		newDispayReference = new LDreference();
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

		newDispayReference = new LDreference();
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

		newDispayReference = new LDreference();
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

		newDispayReference = new LDreference();
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


		newDispayReference = new LDreference();
		newCurrentReferenceInPrintList->next = newDispayReference;
		newCurrentReferenceInPrintList = newCurrentReferenceInPrintList->next;

		int numSpritesAdded;	//not used
		vec positionLDR;
		positionLDR.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
		positionLDR.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionLDR.z = vect->z  -GIA_OUTPUT_Z_POSITION_NODES;
		newCurrentReferenceInPrintList = LDaddBasicTextualSpriteStringToReferenceList(*text, newCurrentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
	}

	if(printType[DRAW_CREATE_SVG_REFERENCES] == true)
	{
		vec positionSVG;
		positionSVG.x = vect->x + GIA_DRAW_BASICENTITY_NODE_WIDTH/2;
		positionSVG.y = vect->y;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_NODES;
		writeSVGbox(currentTag, &positionSVG, width, height, colour, GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG*thickness, GIA_SVG_ELLIPTICAL_BOXES);
		positionSVG.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
		positionSVG.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_TEXT;
		writeSVGtext(currentTag,* text, &positionSVG, SVG_SCALE_FACTOR*SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);
	}

	return newCurrentReferenceInPrintList;
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

	XMLrulesClass* currentReferenceRulesClass = CSrulesDraw;

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


