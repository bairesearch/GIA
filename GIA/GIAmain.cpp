/*******************************************************************************
 *
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Yet to Do: all Nodes should be indexed in an indexed database to allow for fast referencing
 *
 *******************************************************************************/
 


#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


#include "GIAmain.h"
#include "GIAParser.h"
#include "GIATranslator.h"
#include "GIAEntityNodeClass.h"
#include "GIAActionNodeClass.h"
#include "GIAdraw.h"
#include "GIAXMLconversion.h"
#include "XMLParserClass.h"
#include "XMLrulesClass.h"
#include "LDsprite.h"

#define CFF_XML_TAG_nlparse ((string)"nlparse")
#define CFF_XML_TAG_sentence ((string)"sentence")
#define CFF_XML_TAG_parse ((string)"parse")
#define CFF_XML_TAG_relations ((string)"relations")
#define CFF_XML_TAG_features ((string)"features")

static char errmessage[] = "Usage:  GIA.exe [options]\n\n\twhere options are any of the following\n"
"\n\n\t-irelex [string] : RelEx compact output .xml input filename (def: relexCompactOuput.xml)"
"\n\n\t-ixml [string]   : semantic network definition .xml input filename (def: semanticNet.xml)"
"\n\n\t-oxml [string]   : semantic network definition .xml output filename (def: semanticNet.xml)"
"\n\t-osvg [string]     : semantic network display .svg 2D vector graphics output filename (def: semanticNet.svg)"
"\n\t-oldr [string]     : semantic network display .ldr 3D vector graphics output filename (def: semanticNet.ldr)"
"\n\t-oppm [string]     : semantic network display .ppm raster graphics output filename (def: semanticNet.ppm)"
"\n\t-oall [string]     : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
"\n\t-notshow           : do not display output in opengl"
"\n\t-width [int]       : raster graphics width in pixels (def: 640)"
"\n\t-height [int]      : raster graphics height in pixels (def: 480)"
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables GIA.exe and (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\n\t-version         : print version"
"\n\n\tThis program performs GIA (General Intelligence Algorithm) operations - creates semantic network based upon RelEx dependencies file (.xml) or GIA semantic network file (.xml); outputs semantic network to GIA semantic network file (.xml); displays semantic network (using opengl); prints semantic network to raster image (.ppm), 3D vector graphics (.ldr), or 2D vector graphics (.svg).\n\n";

//Dependency Relationship Extractor

int main(int argc,char **argv)
{
	bool result = true;
	
	bool useInputRelexXMLFile = false;
	string inputRelexXMLFileName = "relexCompactOuput.xml";
	
	bool useInputXMLFile = false;
	string inputXMLFileName = "semanticNet.xml";
	
	bool useOutputXMLFile = false;
	string outputXMLFileName = "semanticNet.xml";
	
	bool useOutputLDRFile = false;
	string outputLDRFileName = "semanticNet.ldr";
	
	bool useOutputPPMFile = false;
	string outputPPMFileName = "semanticNet.ppm";
	
	bool useOutputSVGFile = false;
	string outputSVGFileName = "semanticNet.svg";
	
	bool useOutputAllFile = false;
	string outputAllFileName = "semanticNet";
	
	bool printOutput = false;
	bool displayInOpenGLAndOutputScreenshot = true;

	int rasterImageWidth = 640;
	int rasterImageHeight = 480; 
	
	

	//bool train = false;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml input file is specified, just form network - do not train network

	if(exists_argument(argc,argv,"-irelex") || exists_argument(argc,argv,"-ixml"))
	{
		if(exists_argument(argc,argv,"-irelex"))
		{
			inputRelexXMLFileName=get_char_argument(argc,argv,"-irelex");
			useInputRelexXMLFile = true;
		}

		if(exists_argument(argc,argv,"-ixml"))
		{
			inputXMLFileName=get_char_argument(argc,argv,"-ixml");
			//train = true;
			useInputXMLFile = true;
		}

		if(exists_argument(argc,argv,"-oxml"))
		{
			outputXMLFileName=get_char_argument(argc,argv,"-oxml");
			useOutputXMLFile = true;
		}

		if(exists_argument(argc,argv,"-oldr"))
		{
			outputLDRFileName=get_char_argument(argc,argv,"-oldr");
			useOutputLDRFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oppm"))
		{
			outputPPMFileName=get_char_argument(argc,argv,"-oppm");
			useOutputPPMFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-osvg"))
		{
			outputSVGFileName=get_char_argument(argc,argv,"-osvg");
			useOutputSVGFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oall"))
		{
			outputAllFileName=get_char_argument(argc,argv,"-oall");
			useOutputAllFile = true;
			printOutput = true;
		}
		
		/*
		if(exists_argument(argc,argv,"-train"))
		{
			int trainInt
			trainInt=get_float_argument(argc,argv,"-train");
			train = (bool)trainInt;
		}
		*/

		if (exists_argument(argc,argv,"-notshow"))
		{
			displayInOpenGLAndOutputScreenshot = false;
		}

		if (exists_argument(argc,argv,"-width"))
		rasterImageWidth=get_float_argument(argc,argv,"-width");

		if (exists_argument(argc,argv,"-height"))
		rasterImageHeight=get_float_argument(argc,argv,"-height");
		
		char currentFolder[EXE_FOLDER_PATH_MAX_LENGTH];	
		#ifdef LINUX
		getcwd(currentFolder, EXE_FOLDER_PATH_MAX_LENGTH);					
		#else
		::GetCurrentDirectory(EXE_FOLDER_PATH_MAX_LENGTH, currentFolder);
		#endif
						
		if (exists_argument(argc,argv,"-workingfolder"))
		{
			workingFolderCharStar=get_char_argument(argc,argv,"-workingfolder");
		}
		else
		{
			workingFolderCharStar = currentFolder;		
		}
		if (exists_argument(argc,argv,"-exefolder"))
		{
			exeFolderCharStar=get_char_argument(argc,argv,"-exefolder");
		}
		else
		{
			exeFolderCharStar = currentFolder;
		}
		if (exists_argument(argc,argv,"-tempfolder"))
		{
			tempFolderCharStar=get_char_argument(argc,argv,"-tempfolder");
		}
		else
		{
			tempFolderCharStar = currentFolder;
		}

		#ifdef LINUX
		chdir(workingFolderCharStar);						
		#else
		::SetCurrentDirectory(workingFolderCharStar);
		#endif		
								
		if (exists_argument(argc,argv,"-version"))
		{
			cout << "GIA.exe version: 1b10a" << endl;
			exit(1);
		}

		
	}
	else
	{
		cout << "error: GIA requires either a relex input file (.xml) or GIA semantic network (.xml) to be defined" << endl;
		printf(errmessage);
		exit(1);
	}
		
	vector<GIAEntityNode*> * entityNodesCompleteList;
	vector<GIAActionNode*> * actionNodesCompleteList;
	vector<GIAConditionNode*> * conditionNodesCompleteList;

	if(printOutput)
	{
		if(!useOutputXMLFile)
		{	
			if(useOutputAllFile)
			{	
				useOutputXMLFile = true;		
				outputXMLFileName = outputAllFileName + ".xml";
			}
		}		
		if(!useOutputLDRFile)
		{		
			if(useOutputAllFile || displayInOpenGLAndOutputScreenshot)		//LDR output is always required when displaying semantic network in OpenGL and outputing screenshot
			{
				useOutputLDRFile = true;			
				outputLDRFileName = outputAllFileName + ".ldr";
			}
		}
		if(!useOutputSVGFile)
		{	
			useOutputSVGFile = true;	//SVG output is always required when printing/drawing semantic network
			outputSVGFileName = outputAllFileName + ".svg";
		}
		if(!useOutputPPMFile)
		{
			if(useOutputAllFile)
			{
				useOutputPPMFile = true;		
				outputPPMFileName = outputAllFileName + ".ppm";
			}
		}		
	}
	
					
	if(useInputXMLFile)
	{
		if(useInputRelexXMLFile)
		{
			cout << "error: useInputXMLFile && useInputRelexXMLFile" << endl;
			exit(0);
		}
		else
		{		
			entityNodesCompleteList = new vector<GIAEntityNode*>;
			actionNodesCompleteList = new vector<GIAActionNode*>;
			conditionNodesCompleteList = new vector<GIAConditionNode*>;
			if(!readSemanticNetXMLFile(inputXMLFileName, entityNodesCompleteList, actionNodesCompleteList, conditionNodesCompleteList))
			{
				result = false;
			}
			
		}
	}
	
	if(useInputRelexXMLFile)
	{
		if(useInputXMLFile)
		{
			cout << "error: useInputXMLFile && useInputRelexXMLFile" << endl;
			exit(0);
		}
		else
		{
			Sentence * firstSentenceInList = new Sentence();
			Relation * newRelation = new Relation();
			Feature * newFeature = new Feature();
			firstSentenceInList->firstRelationInList = newRelation;
			firstSentenceInList->firstFeatureInList = newFeature;
			Sentence * currentSentence = firstSentenceInList;

			XMLParserTag * firstTagInXMLFile = new XMLParserTag();
			readXMLFile(inputRelexXMLFileName, firstTagInXMLFile);

			XMLParserTag * currentTag = firstTagInXMLFile;
			currentTag = parseTagDownALevel(currentTag, CFF_XML_TAG_nlparse, &result);
			if(result)
			{

				//now for every sentence;
				while(currentTag->nextTag != NULL)
				{
					if(currentTag->name == CFF_XML_TAG_sentence)
					{
						XMLParserTag * firstTagInSentence;
						firstTagInSentence = parseTagDownALevel(currentTag, CFF_XML_TAG_sentence, &result);	
						XMLParserTag * firstTagInFirstParse;
						firstTagInFirstParse = parseTagDownALevel(firstTagInSentence, CFF_XML_TAG_parse, &result);

						if(result)
						{
							XMLParserTag * currentTagInParse = firstTagInFirstParse;
							while(currentTagInParse->nextTag != NULL)
							{

								if(currentTagInParse->name == CFF_XML_TAG_relations)
								{
									//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
									int maxNumberOfWordsInSentence = 0;
									GIATHparseRelationsText(&(currentTagInParse->value), currentSentence->firstRelationInList, &maxNumberOfWordsInSentence);
									currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
								}
								else if(currentTagInParse->name == CFF_XML_TAG_features)
								{
									//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
									GIATHparseFeaturesText(&(currentTagInParse->value), currentSentence->firstFeatureInList);
								}

								currentTagInParse = currentTagInParse->nextTag;
							}

						}
						Sentence * newSentence = new Sentence();
						Relation * newRelation = new Relation();
						Feature * newFeaturesultre = new Feature();
						newSentence->previous = currentSentence;				
						newSentence->firstRelationInList = newRelation;
						newSentence->firstFeatureInList = newFeature;					
						currentSentence->next = newSentence;
						currentSentence = currentSentence->next;

					}
					currentTag = currentTag->nextTag;
				}

			}

			vector<GIAEntityNode*> indexOfEntityNodes;
			vector<string> indexOfEntityNames;
			vector<GIATimeConditionNode*> indexOfTimeNodes;
			vector<long> indexOfTimeNumbers;

			convertSentenceRelationsIntoGIAnetworkNodes(&indexOfEntityNodes, &indexOfEntityNames, &indexOfTimeNodes, &indexOfTimeNumbers, firstSentenceInList);
			
			entityNodesCompleteList = getTranslatorEntityNodesCompleteList();
			actionNodesCompleteList = getTranslatorActionNodesCompleteList();
			conditionNodesCompleteList = getTranslatorConditionNodesCompleteList();
		}
	}
	
	
	if(!parseGIARulesXMLFile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInLDSpriteExternVariables();
	///GIA specific rules.xml file is not used at the moment	[once right variables have been decided upon they will be fed to xml]
	//fillInGIARulesExternVariables();
	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif
	
				
	if(printOutput)
	{	
		printGIAnetworkNodes(entityNodesCompleteList, rasterImageWidth, rasterImageHeight, outputLDRFileName, outputSVGFileName, outputPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputLDRFile, useOutputPPMFile);
	}			
		
	if(useOutputXMLFile)
	{			
		if(!writeSemanticNetXMLFile(outputXMLFileName, entityNodesCompleteList, actionNodesCompleteList, conditionNodesCompleteList))
		{
			result = false;
		}
	}
		
	#ifdef GIA_XML_DEBUG_TEST_WRITE_READ_WRITE
	if(!testReadSemanticNetXMLFile2(entityNodesCompleteList, actionNodesCompleteList, conditionNodesCompleteList))
	{
		result = false;
	}
	#endif
	
}
