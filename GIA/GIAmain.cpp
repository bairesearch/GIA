/*******************************************************************************
 *
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1d4b 01-Nov-2011
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Yet to Do: all Nodes should be indexed in an indexed database to allow for fast referencing
 *
 *******************************************************************************/
 
/* Additional Dependencies: Relex
install relex as per relex installation instructions	"relex installation procedure EL6.txt"
su root
cp -rf relex-1.3.0 /home/baiappserver/bin
cd /home/baiappserver/bin
chown -R baiappserver:baiappserver /home/baiappserver/bin/relex-1.3.0

NB execute-relex.sh contains the following text;

	export LANG=en_US.UTF-8

	VM_OPTS="-Xmx1024m"

	RELEX_OPTS="\
		-Djava.library.path=/usr/lib:/usr/local/lib \
		-Drelex.algpath=data/relex-semantic-algs.txt \
		-Dwordnet.configfile=data/wordnet/file_properties.xml \
		"

	CLASSPATH="-classpath \
	bin:\
	/usr/local/share/java/opennlp-tools-1.4.3.jar:\
	/usr/local/share/java/opennlp-tools-1.3.0.jar:\
	/usr/local/share/java/maxent-2.5.2.jar:\
	/usr/local/share/java/maxent-2.4.0.jar:\
	/usr/local/share/java/trove.jar:\
	/usr/local/share/java/jwnl.jar:\
	/usr/local/share/java/jwnl-1.3.3.jar:\
	/usr/share/java/commons-logging.jar:\
	/usr/share/java/gnu-getopt.jar:\
	/usr/local/share/java/linkgrammar.jar:\
	/usr/share/java/linkgrammar.jar:\
	/usr/share/java/xercesImpl.jar:\
	/usr/share/java/xml-apis.jar:\
	/opt/GATE-4.0/lib/PDFBox-0.7.2.jar:\
	#/opt/GATE-4.0/bin/gate.jar:\
	/opt/GATE-4.0/lib/jdom.jar:\
	/opt/GATE-4.0/lib/jasper-compiler-jdt.jar:\
	/opt/GATE-4.0/lib/nekohtml-0.9.5.jar:\
	/opt/GATE-4.0/lib/ontotext.jar:\
	"

	cat $3/$1 | java $VM_OPTS $RELEX_OPTS $CLASSPATH relex.WebFormat -g --url "$3/$1" > $3/$2

Make sure to set the exefolder to the folder where relex-1.3.0 presides, eg;

./GIA.exe -itxt text.txt -oxml semanticNet.xml -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -exefolder "/home/rich/soft/BAISource/relex/relex-1.3.0"
OR
./GIA.exe -itxt text.txt -oxml semanticNet.xml -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -exefolder "/home/rich/soft/BAISource/relex/relex-1.3.0"  -tempfolder '/tmp/baiappserver/BAIGIA' -workingfolder '/var/www/html/user/104' 

*/	
	


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
#include "GIAdraw.h"
#include "GIAquery.h"
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
"\n\n\t-itxt [string]  : plain text .txt input filename to be parsed by Relex (def: text.txt)"
"\n\n\t-irelex [string] : RelEx compact output .xml input filename (def: relexCompactOutput.xml)"
"\n\n\t-ixml [string]   : semantic network definition .xml input filename (def: semanticNet.xml)"
"\n\n\t-qtxt [string]  : plain text .txt query filename to be parsed by Relex (def: textQuery.txt)"
"\n\n\t-qrelex [string] : RelEx compact output .xml query filename (def: relexCompactOutputQuery.xml)"
"\n\n\t-qxml [string]   : semantic network definition .xml query filename (def: semanticNetQuery.xml)"
"\n\n\t-oxml [string]   : semantic network definition .xml output filename (def: semanticNet.xml)"
"\n\t-osvg [string]     : semantic network display .svg 2D vector graphics output filename (def: semanticNet.svg)"
"\n\t-oldr [string]     : semantic network display .ldr 3D vector graphics output filename (def: semanticNet.ldr)"
"\n\t-oppm [string]     : semantic network display .ppm raster graphics output filename (def: semanticNet.ppm)"
"\n\t-oall [string]     : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
"\n\t-oanswer [string]  : plain text .txt file containing the answer to the query (def: answer.txt)"
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
	//print execution time
	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%i:%i:%i %i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, current->tm_mon, (current->tm_year + GIA_TM_STRUCT_YEAR_OFFSET));
	cout << "GIA execution time: " << timeAndDateString << " (start)" << endl;
		 
	bool result = true;

	bool useInputPlainTXTFile = false;
	string inputPlainTXTFileName = "text.txt";
		
	bool useInputRelexXMLFile = false;
	string inputRelexXMLFileName = "relexCompactOutput.xml";
	
	bool useInputXMLFile = false;
	string inputXMLFileName = "semanticNet.xml";

	bool useQueryPlainTXTFile = false;
	string queryPlainTXTFileName = "textQuery.txt";
		
	bool useQueryRelexXMLFile = false;
	string queryRelexXMLFileName = "relexCompactOutputQuery.xml";
	
	bool useQueryXMLFile = false;
	string queryXMLFileName = "semanticNetQuery.xml";
		
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
	
	bool useOutputAnswerPlainTXTFile = false;
	string outputAnswerPlainTXTFileName = "answer.txt";
			
	bool printOutput = false;
	bool displayInOpenGLAndOutputScreenshot = true;

	int rasterImageWidth = 640;
	int rasterImageHeight = 480; 
	
	bool useQuery = false;
	
	//bool train = false;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml input file is specified, just form network - do not train network

	if(exists_argument(argc,argv,"-itxt") || exists_argument(argc,argv,"-irelex") || exists_argument(argc,argv,"-ixml"))
	{
		if(exists_argument(argc,argv,"-itxt"))
		{
			inputPlainTXTFileName=get_char_argument(argc,argv,"-itxt");
			useInputPlainTXTFile = true;
		}
	
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

		if(exists_argument(argc,argv,"-qtxt"))
		{
			queryPlainTXTFileName=get_char_argument(argc,argv,"-qtxt");
			useQueryPlainTXTFile = true;
			useQuery = true;
		}

		if(exists_argument(argc,argv,"-qrelex"))
		{
			queryRelexXMLFileName=get_char_argument(argc,argv,"-qrelex");
			useQueryRelexXMLFile = true;
			useQuery = true;
		}

		if(exists_argument(argc,argv,"-qxml"))
		{
			queryXMLFileName=get_char_argument(argc,argv,"-qxml");
			useQueryXMLFile = true;
			useQuery = true;
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
		
		if(exists_argument(argc,argv,"-oanswer"))
		{
			outputAnswerPlainTXTFileName=get_char_argument(argc,argv,"-oanswer");
			useOutputAnswerPlainTXTFile = true;
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
		cout << "error: GIA requires either a plain text input file (.txt), a relex input file (.xml) or GIA semantic network (.xml) to be defined" << endl;
		printf(errmessage);
		exit(1);
	}
	
	/*	
	cout << "workingFolderCharStar = " << workingFolderCharStar << endl;
	cout << "tempFolderCharStar = " << tempFolderCharStar << endl;
	cout << "exeFolderCharStar = " << exeFolderCharStar << endl;
	*/
	
	if(!parseGIARulesXMLFile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInLDSpriteExternVariables();
	///GIA specific rules.xml file is not used at the moment	[once right variables have been decided upon they will be fed to xml]
	//fillInGIARulesExternVariables();

	
		
	vector<GIAEntityNode*> * entityNodesCompleteList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * conceptEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * propertyEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * actionEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * conditionEntityNodesList = new vector<GIAEntityNode*>;
	vector<string> * conceptEntityNamesList = new vector<string>;
	vector<GIATimeConditionNode*> * timeConditionNodesList = new vector<GIATimeConditionNode*>;
	vector<long> * timeConditionNumbersList = new vector<long>;	
		
	vector<GIAEntityNode*> * entityNodesCompleteListQuery = new vector<GIAEntityNode*>;	
	vector<GIAEntityNode*> * conceptEntityNodesListQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * propertyEntityNodesListQuery = new  vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * actionEntityNodesListQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * conditionEntityNodesListQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry	
	vector<string> * conceptEntityNamesListQuery = new vector<string>;					//not required - declared for symmetry
	vector<GIATimeConditionNode*> * timeConditionNodesListQuery = new vector<GIATimeConditionNode*>;
	vector<long> * timeConditionNumbersListQuery = new vector<long>;	
			
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

					
	if(useInputPlainTXTFile)
	{
		if(useInputRelexXMLFile)
		{
			cout << "error: useInputPlainTXTFile && useInputRelexXMLFile" << endl;
			exit(0);
		}
		else if(useInputXMLFile)
		{
			cout << "error: useInputPlainTXTFile && useInputXMLFile" << endl;
			exit(0);
		}
		else
		{	
			executeRelex(inputPlainTXTFileName, inputRelexXMLFileName);			
			useInputRelexXMLFile = true;	//now will parse the relex file
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
			//cout << "as" << endl;
			if(!parseRelexFile(inputRelexXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, conceptEntityNamesList, timeConditionNodesList, timeConditionNumbersList))
			{
				result = false;
			}
			//cout << "as2" << endl;
		}
	}
	
	if(useInputXMLFile)
	{
		if(useInputPlainTXTFile)
		{
			cout << "error: useInputXMLFile && useInputPlainTXTFile" << endl;
			exit(0);		
		}
		if(useInputRelexXMLFile)
		{
			cout << "error: useInputXMLFile && useInputRelexXMLFile" << endl;
			exit(0);
		}
		else
		{		
			if(!readSemanticNetXMLFile(inputXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, conceptEntityNamesList))
			{
				result = false;
			}
			
		}
	}
			
	if(useQueryPlainTXTFile)
	{
		if(useQueryRelexXMLFile)
		{
			cout << "error: useQueryPlainTXTFile && useQueryRelexXMLFile" << endl;
			exit(0);
		}
		else if(useQueryXMLFile)
		{
			cout << "error: useQueryPlainTXTFile && useQueryXMLFile" << endl;
			exit(0);
		}
		else
		{	
			executeRelex(queryPlainTXTFileName, queryRelexXMLFileName);			
			useQueryRelexXMLFile = true;	//now will parse the relex file
		}
	}
			
	if(useQueryRelexXMLFile)
	{
		if(useQueryXMLFile)
		{
			cout << "error: useQueryXMLFile && useQueryRelexXMLFile" << endl;
			exit(0);
		}
		else
		{
			if(!parseRelexFile(queryRelexXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery, conceptEntityNamesListQuery, timeConditionNodesListQuery, timeConditionNumbersListQuery))
			{
				result = false;
			}
		}
	}
		
	if(useQueryXMLFile)
	{
		if(useQueryPlainTXTFile)
		{
			cout << "error: useQueryXMLFile && useQueryPlainTXTFile" << endl;
			exit(0);		
		}
		if(useQueryRelexXMLFile)
		{
			cout << "error: useQueryXMLFile && useQueryRelexXMLFile" << endl;
			exit(0);
		}
		else
		{		
			entityNodesCompleteListQuery = new vector<GIAEntityNode*>;
			if(!readSemanticNetXMLFile(queryXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery, conceptEntityNamesListQuery))
			{
				result = false;
			}
			
		}
	}
		

	if(useQuery)
	{
		/*
		implement comparison of question semantic net to semanic net - locate the question semantic net as a subset of the semantic net, and;
			1. highlight it 
			2. return missing variables 
			3. NB for which/what questions, make the software just locate the identical structure, and if necessary return the parent of the primary property (eg the parent of the "object" of the question)
		*/

		bool foundComparisonVariable = getFoundComparisonVariable();
		GIAEntityNode* comparisonVariableNode = getComparisonVariableNode();
		bool foundAnswer = false;
		double confidence = 0.0;
		
		GIAEntityNode* queryAnswerNode;
		GIAEntityNode* queryAnswerPreviousNode;
		string queryAnswerContext = "";
		queryAnswerNode = answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(conceptEntityNodesList, conceptEntityNamesList, conceptEntityNodesListQuery, foundComparisonVariable, comparisonVariableNode, &foundAnswer, queryAnswerNode, &confidence, &queryAnswerPreviousNode, &queryAnswerContext);
		
		double maxConfidence = determineMaxConfidenceOfQuerySemanticNetwork(conceptEntityNodesListQuery);		//OLD [simple]: entityNodesCompleteListQuery->size();
	
		string answerString = "";
				 
		if(foundAnswer)
		{	
		
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			cout << "Answer Found." << endl;					
			#endif
			answerString = answerString + "\nAnswer found.";
			if(foundComparisonVariable)
			{
				#ifndef GIA_DO_NOT_PRINT_RESULTS
				cout << "Exact Found Answer: " << queryAnswerNode->entityName << endl;
				#endif
				answerString = answerString + "\nExact Answer found: " + queryAnswerNode->entityName;	
				if(comparisonVariableNode->hasQuantity)
				{
					#ifndef GIA_DO_NOT_PRINT_RESULTS
					cout << "Quantity number: " << queryAnswerNode->quantityNumber << endl;
					#endif
					char tempQuantityNumberStringCharStar[100]; 
					sprintf(tempQuantityNumberStringCharStar, "%d", queryAnswerNode->quantityNumber);					
					answerString = answerString + "\nQuantity number: " + tempQuantityNumberStringCharStar;	
				}
				/*
				if(queryAnswerPreviousNode->isCondition)
				{
					#ifndef GIA_DO_NOT_PRINT_RESULTS
					cout << "Answer is a Condition of type/preposition:" << queryAnswerPreviousNode->entityName << endl;
					#endif
					answerString = answerString + "\nAnswer is a Condition of type/preposition: " + queryAnswerPreviousNode->entityName;	
				}
				*/
			}
			//cout << "ahsd2" << endl;
		}
		else
		{
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			cout << "Answer Not Found." << endl;
			#endif
			answerString = answerString + "\nAnswer Not Found.";
		}
		
		if(foundAnswer && !foundComparisonVariable)
		{
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			cout << "Best Inexact Answer Found: " << queryAnswerNode->entityName << endl;
			#endif
			answerString = answerString + "\nBest Inexact Answer Found: " + queryAnswerNode->entityName;
			//answerString = answerString + printEntityNode(queryAnswerNode);	
		}
		
		if(foundAnswer)
		{
			//print AnswerPreviousNode relationship with answerNode
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			cout << "Answer Context: " << queryAnswerContext << endl;
			answerString = answerString + "\nAnswer Context: " + queryAnswerContext;
			#endif
		}
		
					
		//add confidence to answer
		char tempConfidenceStringCharStar[100]; 
		sprintf(tempConfidenceStringCharStar, "%0.6f", confidence*10);
		char tempMaxConfidenceStringCharStar[100]; 
		sprintf(tempMaxConfidenceStringCharStar, "%0.6f", maxConfidence*10);		
		answerString = answerString + "\nconfidence = " + tempConfidenceStringCharStar;			
		answerString = answerString + "\nmax confidence = " + tempMaxConfidenceStringCharStar;	
		#ifndef GIA_DO_NOT_PRINT_RESULTS
		cout << "confidence: " << tempConfidenceStringCharStar << endl;
		cout << "max confidence: " << tempMaxConfidenceStringCharStar << endl;
		#endif
										
		char * fileByteArray = const_cast<char*>(answerString.c_str());
		char * outputAnswerPlainTXTFileNameCharStar = const_cast<char*>(outputAnswerPlainTXTFileName.c_str());	
		writeByteArrayToFile(outputAnswerPlainTXTFileNameCharStar, fileByteArray, answerString.length());		
	}
	else if(useOutputAnswerPlainTXTFile)
	{
		cout << "error: output answer require a query to be set" << endl;
	}
	
	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif	
			
	if(printOutput)
	{	
		printGIAnetworkNodes(entityNodesCompleteList, rasterImageWidth, rasterImageHeight, outputLDRFileName, outputSVGFileName, outputPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputLDRFile, useOutputPPMFile);
	}			
	
	#ifdef GIA_XML_DEBUG_TEST_WRITE_READ_WRITE
	if(!testReadSemanticNetXMLFile2(entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, conceptEntityNamesList))
	{
		result = false;
	}
	#else				
	if(useOutputXMLFile)
	{			
		if(!writeSemanticNetXMLFile(outputXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, conceptEntityNamesList))
		{
			result = false;
		}
	}
	#endif
	
	//print execution time (end)
	time(&now);
	current = localtime(&now);
	sprintf(timeAndDateString, "%i:%i:%i %i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, current->tm_mon, (current->tm_year + GIA_TM_STRUCT_YEAR_OFFSET));
	cout << "GIA execution time: " << timeAndDateString << " (finish)" << endl;
	
}

void executeRelex(string inputPlainTXTFileName, string inputRelexXMLFileName)
{
	//execute Relex on plain text
	string executeRelexCommand = "";	
	executeRelexCommand = executeRelexCommand + exeFolderCharStar + "/" + GIA_RELEX_EXECUTABLE_NAME + " " + inputPlainTXTFileName + " " + inputRelexXMLFileName + " " + workingFolderCharStar + " " + tempFolderCharStar;

	#ifdef LINUX
	chdir(exeFolderCharStar);						
	#else
	::SetCurrentDirectory(exeFolderCharStar);
	#endif	

	#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	cout << "system(" << executeRelexCommand << ");" << endl;
	#endif
	system(executeRelexCommand.c_str());

	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif	
			
}


	
bool parseRelexFile(string inputRelexXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, vector<GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<string> * conceptEntityNamesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList)
{
	bool result = true;
	
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
				Feature * newFeature = new Feature();
				newSentence->previous = currentSentence;				
				newSentence->firstRelationInList = newRelation;
				newSentence->firstFeatureInList = newFeature;					
				currentSentence->next = newSentence;
				currentSentence = currentSentence->next;

			}
			currentTag = currentTag->nextTag;
		}

	}
	setTranslatorEntityNodesCompleteList(entityNodesCompleteList);
	setTranslatorPropertyEntityNodesList(propertyEntityNodesList);
	setTranslatorActionEntityNodesList(actionEntityNodesList);
	setTranslatorConditionEntityNodesList(conditionEntityNodesList);
	convertSentenceRelationsIntoGIAnetworkNodes(conceptEntityNodesList, conceptEntityNamesList, timeConditionNodesList, timeConditionNumbersList, firstSentenceInList);
	
	return result;
}
