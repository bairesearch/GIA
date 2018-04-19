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
 * File Name: GIAdraw.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1k 22-February-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Draw - Draws GIA nodes in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAdraw.hpp"


#include <GL/freeglut.h>	//need to use freeglut as it contains extensions functions which glut does not have; glutMainLoopEvent()

int maxXAtAParticularY[MAX_GIA_TREE_DEPTH];

GIAdrawVariables::GIAdrawVariables(void)
{
	svgTinySpec = false;
	//printType = 
	maxNumberSentences = 0;
	sentenceToPrint = GIA_DRAW_SENTENCE_INDEX_PRINT_ALL_SENTENCES;
	entityReference = NULL;
}
GIAdrawVariables::~GIAdrawVariables(void)
{
}

bool GIAdrawClass::printGIAnetworkNodesToSVGstring(GIAtranslatorVariablesClass* translatorVariables, const int width, const int height, int sentenceIndex, string* writeFileStringSVG, GIAentityNode* entityReference)
{//most of this is copied from CSexecFlow.cpp
	bool result = true;

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();
	
	GIAdrawVariables drawVariables;
	drawVariables.svgTinySpec = true;
	drawVariables.sentenceToPrint = sentenceIndex;
	drawVariables.maxNumberSentences = translatorVariables->maxNumberSentences;
	drawVariables.printType[GIA_DRAW_CREATE_LDR_REFERENCES] = false;
	drawVariables.printType[GIA_DRAW_CREATE_SVG_REFERENCES] = true;
	drawVariables.printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	if(entityReference != NULL)
	{
		drawVariables.entityReference = entityReference;
	}
	
	LDreference* firstReferenceInPrintList = NULL;
	determineBasicPrintPositionsOfAllNodes(translatorVariables->entityNodesActiveListComplete, &drawVariables, firstReferenceInPrintList, firstTagInSVGFile);
	//cout << "firstTagInSVGFile = " << firstTagInSVGFile->name << endl;
	
	const int viewBoxMinX = -GIA_DRAW_SVG_VIEWBOX_MIN_X_OFFSET_SINGLE_SENTENCE;
	const int viewBoxMaxX = width-GIA_DRAW_SVG_VIEWBOX_MIN_X_OFFSET_SINGLE_SENTENCE;
	const int viewBoxMinY = -GIA_DRAW_SVG_VIEWBOX_MIN_Y_OFFSET_SINGLE_SENTENCE;
	const int viewBoxMaxY = height-GIA_DRAW_SVG_VIEWBOX_MIN_Y_OFFSET_SINGLE_SENTENCE;
	if(!LDsvg.writeSVGfile(writeFileStringSVG, firstTagInSVGFile, viewBoxMinX, viewBoxMaxX, viewBoxMinY, viewBoxMaxY))
	{
		result = false;
	}
	
	delete firstTagInSVGFile;
	
	return result;
}

bool GIAdrawClass::printGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const int width, const int height, const string outputFileNameLDR, const string outputFileNameSVG, const string outputFileNamePPM, const bool display, const bool useOutputLDRfile, const bool useOutputPPMfile, const bool useOutputSVGfile)
{//most of this is copied from CSexecFlow.cpp
	bool result = true;

	XMLparserTag* firstTagInSVGFile = new XMLparserTag();

	GIAdrawVariables drawVariables;
	drawVariables.sentenceToPrint = GIA_DRAW_SENTENCE_INDEX_PRINT_ALL_SENTENCES;
	drawVariables.maxNumberSentences = translatorVariables->maxNumberSentences;
	drawVariables.printType[GIA_DRAW_CREATE_LDR_REFERENCES] = false;
	drawVariables.printType[GIA_DRAW_CREATE_SVG_REFERENCES] = false;
	drawVariables.printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = false;
	if(useOutputLDRfile || display)
	{// || useOutputPPMfile - implied
		drawVariables.printType[GIA_DRAW_CREATE_LDR_REFERENCES] = true;
		drawVariables.printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}
	if(useOutputSVGfile)
	{
		drawVariables.printType[GIA_DRAW_CREATE_SVG_REFERENCES] = true;
		drawVariables.printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] = true;
	}

	LDreference* firstReferenceInPrintList = new LDreference();
	determineBasicPrintPositionsOfAllNodes(translatorVariables->entityNodesActiveListComplete, &drawVariables, firstReferenceInPrintList, firstTagInSVGFile);
	
	if(useOutputSVGfile)
	{
		const int viewBoxMinX = GIA_DRAW_SVG_VIEWBOX_MIN_X;
		const int viewBoxMaxX = GIA_DRAW_SVG_VIEWBOX_MAX_X;
		const int viewBoxMinY = GIA_DRAW_SVG_VIEWBOX_MIN_Y;
		const int viewBoxMaxY = GIA_DRAW_SVG_VIEWBOX_MAX_Y;
		if(!LDsvg.writeSVGfile(outputFileNameSVG, firstTagInSVGFile, viewBoxMinX, viewBoxMaxX, viewBoxMinY, viewBoxMaxY))
		{
			result = false;
		}
		delete firstTagInSVGFile;
	}

	if(drawVariables.printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{
		LDreferenceManipulation.writeReferencesToFile(outputFileNameLDR, firstReferenceInPrintList);
	}

	if(display)
	{

		//re-parse, then re-write to create a collapsed referencelist file...
		//method1:
		string topLevelSceneFileName = outputFileNameLDR;
		string topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
		LDreference* initialReferenceInSceneFile = new LDreference();
		LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
		{//file does not exist
			cerr << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
			exit(EXIT_ERROR);
		}
		LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
		/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
		char* topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
		LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
		*/

		delete initialReferenceInSceneFile;
		delete topLevelReferenceInSceneFile;


		unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

		LDopengl.setViewPort3Dortho(-100.0, width-100, height-100, -100.0, 1.0, -1.0);	//-100 is used to display the most left semantic network nodes

		//now reparse file
		LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
		LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
		{//file does not exist
			cerr << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
			exit(EXIT_ERROR);
		}

		LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
		LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			//due to opengl code bug, need to execute this function twice.

		//delete initialReferenceInCollapsedSceneFile;	//this cannot be deleted because it is still used by glutDisplayFunc
		delete topLevelReferenceInCollapsedSceneFile;

		if(useOutputPPMfile)
		{
			RTpixelMaps.generatePixmapFromRGBmap(outputFileNamePPM, width, height, rgbMap);
		}

		delete rgbMap;

	}
	else
	{
		//must use an external program to view the .ldr file (Eg LDView)
	}

	delete firstReferenceInPrintList;
	
	return result;
}

void GIAdrawClass::initiateMaxXAtParticularY()
{
	//now print based upon above lists;
	for(int i=0; i<MAX_GIA_TREE_DEPTH; i++)
	{
		maxXAtAParticularY[i] = 0;
	}
}


bool GIAdrawClass::determineBasicPrintPositionsOfAllNodes(vector<GIAentityNode*>* entityNodesActiveListComplete, GIAdrawVariables* drawVariables, LDreference* firstReferenceInPrintList, XMLparserTag* firstTagInSVGFile)
{
	bool result = true;
	
	initiateMaxXAtParticularY();
	int xInitial = DRAW_X_INITIAL_OFFSET;
	int yInitial = DRAW_Y_INITIAL_OFFSET;
	maxXAtAParticularY[yInitial] = xInitial;
	//first pass; determine maxXAtAParticularY	[and use these to centre each row {at a given y} respectively]

	#ifndef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	drawVariables->maxNumberSentences = 1;
	#endif
	bool printFromXMLcoordinatesAlreadyDefined = false;
	if(entityNodesActiveListComplete->size() > 0)
	{
		if((*(entityNodesActiveListComplete->begin()))->printCoordsAlreadyDefined)
		{
			printFromXMLcoordinatesAlreadyDefined = true;
			drawVariables->maxNumberSentences = 1;
		}
	}
	
	if(drawVariables->sentenceToPrint == GIA_DRAW_SENTENCE_INDEX_PRINT_ALL_SENTENCES)
	{
		for(int sentenceIndex=GIA_NLP_START_SENTENCE_INDEX; sentenceIndex <= drawVariables->maxNumberSentences; sentenceIndex++)
		{
			for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
			{
				GIAentityNode* entityNode = *entityIter;

				//initiateMaxXAtParticularY();
				xInitial = maxXAtAParticularY[yInitial];
				//xInitial = sentenceIndex*100;	//compact

				bool thisIsDefinitionAndPreviousNodeWasInstance = false;

				initialiseEntityNodeForPrinting(entityNode, yInitial, xInitial, drawVariables, &firstReferenceInPrintList, &firstTagInSVGFile, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);
			}
		}
	}
	else
	{
		for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
		{
			GIAentityNode* entityNode = *entityIter;

			#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
			#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
			if(!(entityNode->wasReference))
			{
			#endif
			#endif
				//initiateMaxXAtParticularY();
				xInitial = maxXAtAParticularY[yInitial];
				//xInitial = sentenceIndex*100;	//compact
				
				bool thisIsDefinitionAndPreviousNodeWasInstance = false;
	
				initialiseEntityNodeForPrinting(entityNode, yInitial, xInitial, drawVariables, &firstReferenceInPrintList, &firstTagInSVGFile, drawVariables->sentenceToPrint, thisIsDefinitionAndPreviousNodeWasInstance);
			
			#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
			#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
			}
			#endif
			#endif
		}		
	}

	//reset initialisedForPrinting
	for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
	{
		GIAentityNode* entityNode = (*entityIter);
		entityNode->initialisedForPrinting = false;
	}
		
	return result;
}

bool GIAdrawClass::initialiseEntityConnectionForPrinting(vec* pos1, GIAentityConnection* entityConnection, GIAdrawVariables* drawVariables, string connectionName, int entityConnectionColour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag)
{
	bool result = true;
	GIAentityNode* entityNodeToConnect = entityConnection->entity;


	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	#ifdef GIA_DRAW_IGNORE_INITIALISED_ENTITY_CONNECTION_FOR_PRINTING_BOOL
	if((entityNodeToConnect->initialisedForPrinting) && !(entityNodeToConnect->disabled))
	#else
	if((entityNodeToConnect->initialisedForPrinting) && !(entityConnection->initialisedForPrinting) && !(entityNodeToConnect->disabled))	
	#endif	
	//added condition 31 August 2013: only print connection if node has been printed
	#else
	if(!(entityNodeToConnect->disabled))
	#endif
	{
		#ifndef GIA_DRAW_IGNORE_INITIALISED_ENTITY_CONNECTION_FOR_PRINTING_BOOL
		#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
		entityConnection->initialisedForPrinting = true;
		#endif
		#endif
		
		//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
		vec pos2;
		pos2.x = entityNodeToConnect->printX;
		pos2.y = entityNodeToConnect->printY;
		pos2.z = GIA_DRAW_CONNECTION_Z;

		//connectionName = connectionName + convertIntToString(entityConnection->sentenceIndexTemp);

		createReferenceConnectionWithText(pos1, &pos2, entityConnectionColour, currentReferenceInPrintList, currentTag, connectionName, drawVariables);
	}

	return result;
}


bool GIAdrawClass::initialiseEntityNodeForPrinting(GIAentityNode* entityNode, int y, int x, GIAdrawVariables* drawVariables, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, int sentenceIndex, bool thisIsDefinitionAndPreviousNodeWasInstance)
{
	bool result = true;
	
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
			entityNode->initialisedForPrinting = true;

			maxXAtAParticularY[y] = maxXAtAParticularY[y] + GIA_DRAW_X_SPACE_BETWEEN_ENTITIES;	//only used, for indepdendent network visualisation (eg, when rendering next sentence)

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
			pos1.z = GIA_DRAW_CONNECTION_Z;


			int entityDefinitionConnectionColour = GIA_DRAW_CONNECTION_DEFINING_INSTANCE_COLOUR;

			for(int connectionType=0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
			{
				int q = entityVectorConnectionDrawPosYinitialArray[connectionType];
				int r = entityVectorConnectionDrawPosXinitialArray[connectionType];
				int qSpacing = entityVectorConnectionDrawPosYspacingArray[connectionType];
				
				if(GIAentityNodeClass.entityIsRelationship(entityNode))
				{
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT)
					{
						q = -relationshipEntityVectorConnectionDrawPosYinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						r = -relationshipEntityVectorConnectionDrawPosXinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						qSpacing = -relationshipEntityVectorConnectionDrawPosYspacingArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
					}
					else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT)
					{
						q = relationshipEntityVectorConnectionDrawPosYinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						r = relationshipEntityVectorConnectionDrawPosXinitialArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
						qSpacing = relationshipEntityVectorConnectionDrawPosYspacingArray[GIAentityNodeClass.getRelationshipEntityRelativeTypeIndex(entityNode)];
					}
				}

				for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
				{
					GIAentityConnection* connection = *connectionIter;
					
					bool thisIsDefinitionAndPreviousNodeWasInstance = false;
					#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE)
					{
						thisIsDefinitionAndPreviousNodeWasInstance = true;
					}
					#endif
					initialiseEntityNodeForPrinting(connection->entity, y+q, x+r, drawVariables, currentReferenceInPrintList, currentTag, sentenceIndex, thisIsDefinitionAndPreviousNodeWasInstance);

					bool pass = true;
					int entityConnectionColour = entityVectorConnectionDrawColourNameArray[connectionType];

					/*
					//this shouldn't be necessary:
					#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
					if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE)
					{
						if(GIAentityNodeClass.entityIsRelationship(connection->entity))
						{
							pass = false;
						}
					}	
					#endif				
					*/
					
					string connectionName = entityVectorConnectionDrawConnectionNameArray[connectionType];
					#ifdef GIA_DRAW_ONLY_PRINT_CONNECTIONS_IN_ONE_DIRECTION
					if(!entityVectorConnectionDrawPrintConnectionArray[connectionType])
					{
						//pass = false;	//not compatible with GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
						
						int connectionTypeGenerated;
						if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT)
						{
							connectionTypeGenerated = GIAtranslatorOperations.generateConnectionType(entityNode->entityType);
						}
						else if(connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT)
						{
							connectionTypeGenerated = GIAtranslatorOperations.generateConnectionTypeReverse(entityNode->entityType);
						}
						else
						{
							connectionTypeGenerated = inverseVectorConnectionsArray[connectionType];	
						}
						
						connectionName = entityVectorConnectionDrawConnectionNameArray[connectionTypeGenerated];
						
					}
					#endif
					#ifdef GIA_DRAW_PRINT_CONNECTION_SENTENCE_INDEX
					connectionName = string("s") + SHAREDvars.convertIntToString(connection->sentenceIndexTemp) + connectionName;
					#endif
					
					#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
					if(GIAtranslatorOperations.connectionIsAlias(connection))
					{
						entityConnectionColour = GIA_DRAW_DEFINITION_MARK_AS_ALIAS_CONNECTION_COLOUR;
					}
					#endif
					if(pass)
					{
						if(entityVectorConnectionDrawConnectionArray[connectionType])
						{
							#ifdef GIA_DRAW_VERBOSE_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
							if(connection->sameReferenceSet)
							{
								entityConnectionColour = DAT_FILE_COLOUR_GREEN;
							}
							else
							{
								entityConnectionColour = DAT_FILE_COLOUR_RED;
							}
							#endif
							
							initialiseEntityConnectionForPrinting(&pos1, connection, drawVariables, connectionName, entityConnectionColour, currentReferenceInPrintList, currentTag);
						}
					}

					q = q + qSpacing;
				}
			}

			q = GIA_DRAW_Y_SPACE_BETWEEN_CONDITION_NODES;
			r = GIA_DRAW_X_SPACE_BETWEEN_CONDITION_NODES;
			if(entityNode->conditionType == CONDITION_NODE_TYPE_TIME)
			{
				int timeConditionNodePrintX = x+r;
				int timeConditionNodePrintY = y+q;
				initialiseTimeConditionNodeForPrinting(entityNode->timeConditionNode, timeConditionNodePrintY, timeConditionNodePrintX, drawVariables, currentReferenceInPrintList, currentTag);

				q = q+GIA_DRAW_Y_SPACE_BETWEEN_CONDITIONS_OF_SAME_NODE;

				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...
				vec pos2;
				pos2.x = timeConditionNodePrintX;
				pos2.y = timeConditionNodePrintY;
				pos2.z = GIA_DRAW_CONNECTION_Z;
				createReferenceConnectionWithText(&pos1, &pos2, GIA_DRAW_CONDITION_TIME_CONNECTION_COLOUR, currentReferenceInPrintList, currentTag, "time", drawVariables);

			}


			if(drawVariables->printType[GIA_DRAW_CREATE_LDR_OR_SVG_REFERENCES] == true)
			{
				//may accidentially overwrite adjacent nodes that have already been printed here; be careful...

				int boxThickness = GIA_DRAW_THICKNESS_NORMAL;

				int entityColour;
				entityColour = entityNodeTypeColourArray[entityNode->entityType];
				
				//special entity cases;
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
				else if(entityNode->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
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
				}
				else if(entityNode->entityType == GIA_ENTITY_TYPE_ACTION)
				{
					if(entityNode->isActionConcept)
					{
						entityColour = GIA_DRAW_ACTION_CONCEPT_NODE_COLOUR;
					}
				}
				/*
				else if(entityNode->hasAssociatedTime)
				{
					entityColour = GIA_DRAW_CONDITION_DEFINITION_TIME_NODE_COLOUR;	//clear identify a time node
				}
				*/

				#ifdef GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE
				if(entityNode->isLogicReferenceEntity)
				{
					entityColour = GIA_DRAW_LOGIC_REFERENCE_ENTITY_COLOUR;
				}
				#endif
					
				if((drawVariables->sentenceToPrint != GIA_DRAW_SENTENCE_INDEX_PRINT_ALL_SENTENCES) && (drawVariables->entityReference != NULL))
				{
					if(drawVariables->entityReference == entityNode)
					{
						entityColour = GIA_DRAW_ENTITY_REFERENCE_NODE_COLOUR;
					}
				}


				//first, print this action node.
				string nameOfBox = "";
				if(entityNode->hasQuantity)
				{
					string quantityNumberStringTemp;
					if(entityNode->isQuery)
					{
						quantityNumberStringTemp = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
					}
					else
					{
						quantityNumberStringTemp = GIAentityNodeClass.printQuantityNumberString(entityNode);
					}
					nameOfBox = nameOfBox + quantityNumberStringTemp + " " + entityNode->entityName;

				}
				else if(entityNode->negative)
				{
					nameOfBox = nameOfBox + "!" + entityNode->entityName;
				}
				#ifdef GIA_DRAW_PRINT_ALIASES
				else if(entityNode->aliasList.size() > 0)
				{
					nameOfBox = entityNode->aliasList[0];
				}
				#endif
				else
				{
					nameOfBox = entityNode->entityName;
				}
				
				#ifdef GIA_DRAW_PRINT_ENTITY_INSTANCE_ID
				//nameOfBox = SHAREDvars.convertIntToString(entityNode->referenceSetID);
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->idInstance);
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_ACTIVELIST_ID
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->idActiveList);
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_SENTENCE_INDEX
				nameOfBox = string("s") + SHAREDvars.convertIntToString(entityNode->sentenceIndexTemp) + nameOfBox;
				#endif
				#ifdef GIA_DRAW_PRINT_ENTITY_ENTITY_INDEX
				nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->entityIndexTemp);
				#endif

				//nameOfBox = nameOfBox + SHAREDvars.convertIntToString(entityNode->grammaticalDefiniteTemp);

				createBox(&pos1, GIA_DRAW_ACTION_NODE_WIDTH, GIA_DRAW_ACTION_NODE_HEIGHT, entityColour, &nameOfBox, currentReferenceInPrintList, currentTag, boxThickness, drawVariables);
			}



		}
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	}
	#endif

	return result;

}




bool GIAdrawClass::initialiseTimeConditionNodeForPrinting(GIAtimeConditionNode* timeConditionNode, const int y, const int x, GIAdrawVariables* drawVariables, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag)
{
	bool result = true;
	
	int timeConditionNodePrintX = x;
	int timeConditionNodePrintY = y;

	timeConditionNode->printX = timeConditionNodePrintX;
	timeConditionNode->printY = timeConditionNodePrintY;

	vec pos1;
	vec pos2;
	vec pos3;

	pos1.x = timeConditionNodePrintX;
	pos1.y = timeConditionNodePrintY;
	pos1.z = GIA_DRAW_CONNECTION_Z;

	//may accidentially overwrite adjacent nodes/connections that have already been printed here; be careful...


	int boxThickness = GIA_DRAW_THICKNESS_NORMAL;
	if(timeConditionNode->isProgressive)
	{
		boxThickness = GIA_DRAW_THICKNESS_THICK;
	}
	createBox(&pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, GIA_DRAW_CONDITION_TIME_NODE_COLOUR, &(timeConditionNode->conditionName), currentReferenceInPrintList, currentTag, boxThickness, drawVariables);

	/*
	int timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_NODE_COLOUR;
	if(timeConditionNode->isProgressive)
	{
		timeConditionNodeColour = GIA_DRAW_CONDITION_TIME_STATE_NODE_COLOUR;
	}
	createBox(currentReferenceInPrintList, &pos1, GIA_DRAW_CONDITION_NODE_WIDTH, GIA_DRAW_CONDITION_NODE_HEIGHT, timeConditionNodeColour, &(timeConditionNode->conditionName), currentReferenceInPrintList, currentTag, GIA_DRAW_THICKNESS_NORMAL, drawVariables);
	*/


	return result;
}


bool GIAdrawClass::createReferenceConnectionWithText(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, string connectionTypeName, GIAdrawVariables* drawVariables)
{
	bool result = true;
	
	createReferenceConnection(pos1, pos2, colour, currentReferenceInPrintList, currentTag, drawVariables);

	if(GIA_DRAW_USE_CONNECTION_TYPE_NAME_TEXT)
	{
		vec vect;
		vect.x = (pos1->x + pos2->x)/2;
		vect.y = (pos1->y + pos2->y)/2;
		vect.z = (pos1->z + pos2->z)/2;

		if(drawVariables->printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
		{
			int numSpritesAdded;	//not used
			vec positionLDR;
			positionLDR.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
			positionLDR.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionLDR.z = vect.z - GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			*currentReferenceInPrintList = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(connectionTypeName, *currentReferenceInPrintList, &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
		}
		if(drawVariables->printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
		{
			vec positionSVG;
			positionSVG.x = vect.x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
			positionSVG.y = vect.y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
			positionSVG.z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
			LDsvg.writeSVGtext(currentTag, connectionTypeName, &positionSVG, GIA_SVG_SCALE_FACTOR*GIA_SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK, drawVariables->svgTinySpec);
		}
	}

	return result;
}

bool GIAdrawClass::createReferenceConnection(vec* pos1, vec* pos2, int colour, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const GIAdrawVariables* drawVariables)
{
	bool result = true;
	
	if(drawVariables->printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = colour;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = pos1->x;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = pos1->y;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = pos1->z;


		(*currentReferenceInPrintList)->vertex2relativePosition.x = pos2->x;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = pos2->y;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = pos2->z;


		LDreference* newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;
	}

	if(drawVariables->printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
	{
		pos1->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		pos2->z = GIA_OUTPUT_Z_POSITION_CONNECTIONS;
		LDsvg.writeSVGline(currentTag, pos1, pos2, colour);
	}

	return result;
}



//consider using elipse instead; <ellipse cx="240" cy="100" rx="220" ry="30">

bool GIAdrawClass::createBox(vec* vect, const double width, const double height, int colour, string* text, LDreference** currentReferenceInPrintList, XMLparserTag** currentTag, const int thickness, const GIAdrawVariables* drawVariables)
{
	bool result = true;
	
	if(drawVariables->printType[GIA_DRAW_CREATE_LDR_REFERENCES] == true)
	{

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_QUAD;
		(*currentReferenceInPrintList)->colour = colour;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex3relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex3relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex3relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex4relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex4relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex4relativePosition.z = vect->z;


		LDreference* newDispayReference;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x + width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;

		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		(*currentReferenceInPrintList)->type = REFERENCE_TYPE_LINE;
		(*currentReferenceInPrintList)->colour = DAT_FILE_COLOUR_BLACK;

		(*currentReferenceInPrintList)->vertex1relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.y = vect->y + height/2.0;
		(*currentReferenceInPrintList)->vertex1relativePosition.z = vect->z;

		(*currentReferenceInPrintList)->vertex2relativePosition.x = vect->x - width/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.y = vect->y - height/2.0;
		(*currentReferenceInPrintList)->vertex2relativePosition.z = vect->z;


		newDispayReference = new LDreference();
		(*currentReferenceInPrintList)->next = newDispayReference;
		(*currentReferenceInPrintList) = (*currentReferenceInPrintList)->next;

		int numSpritesAdded;	//not used
		vec positionLDR;
		positionLDR.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/4;
		positionLDR.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionLDR.z = vect->z - GIA_OUTPUT_Z_POSITION_NODES;
		(*currentReferenceInPrintList) = LDsprite.LDaddBasicTextualSpriteStringToReferenceList(*text, (*currentReferenceInPrintList), &positionLDR, &numSpritesAdded, false, DAT_FILE_COLOUR_BLACK, 0.3);	//add sprite text within box
	}

	if(drawVariables->printType[GIA_DRAW_CREATE_SVG_REFERENCES] == true)
	{
		vec positionSVG;
		positionSVG.x = vect->x + GIA_DRAW_BASICENTITY_NODE_WIDTH/2;
		positionSVG.y = vect->y;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_NODES;
		LDsvg.writeSVGbox(currentTag, &positionSVG, width, height, colour, GIA_FILE_TEXT_BOX_OUTLINE_WIDTH_SVG*thickness, GIA_SVG_ELLIPTICAL_BOXES);
		positionSVG.x = vect->x - GIA_DRAW_BASICENTITY_NODE_WIDTH/3;
		positionSVG.y = vect->y - GIA_DRAW_BASICENTITY_NODE_HEIGHT/4;
		positionSVG.z = GIA_OUTPUT_Z_POSITION_TEXT;
		
		LDsvg.writeSVGtext(currentTag,* text, &positionSVG, GIA_SVG_SCALE_FACTOR*GIA_SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK, drawVariables->svgTinySpec);
	}

	return result;
}








