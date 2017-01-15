/*******************************************************************************
 *
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i9f 11-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Yet to Do: all Nodes should be indexed in an indexed database to allow for fast referencing
 *
 *******************************************************************************/
 
/* Additional Dependencies: NLP (Relex/Standord Core NLP/Stanford Parser)
install relex as per relex installation instructions	"relex installation procedure EL6.txt"
su root
cp -rf relex-1.3.0 /home/baiappserver/bin
cd /home/baiappserver/bin
chown -R baiappserver:baiappserver /home/baiappserver/bin/APPLICATION

where APPLICATION = relex-1.3.0 / relex-1.4.0 / stanford-parser-2012-03-09 / stanford-corenlp-2012-04-03

*/


/* Additional Dependencies: Relex

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

Make sure to set the nlprelexefolder to the folder where relex-1.x.0 presides, eg;

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelexefolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlprelation 0 -notshowOR

*/	

/* Additional Dependencies: Stanford NLP Core

NB execute-stanfordCoreNLP.sh contains the following text;

	java -cp stanford-corenlp-2012-04-03.jar:stanford-corenlp-2012-03-09-models.jar:xom.jar:joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelexefolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlprelation 1 -notshow

*/

/* Additional Dependencies: Stanford Parser

NB execute-stanfordParser.sh contains the following text;

	#!/usr/bin/env bash
	scriptdir=`dirname $0`
	java -mx150m -cp "$scriptdir/*:" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz $3/$1 > $4/$2

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelexefolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -nlprelation 2 -notshow

Additional example where relations + features are parsed from different NLP files;
./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlprelexefolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -nlpfeature 1 -nlptagexefolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -notshow

Additional example where relations + features parsed from different NLP file, queries parsed using different NLP file;
./GIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlprelexefolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -nlpfeature 1 -nlptagexefolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlprelationq 0 -nlprelqexefolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpfeatureq 0 -nlptagqexefolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -notshow

Additional example where relations + features parsed from different NLP file, queries parsed using different NLP file;
./GIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlprelexefolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -nlpfeature 1 -nlptagexefolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlprelationq 0 -nlprelqexefolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpfeatureq 0 -nlptagqexefolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -notshow

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
#include "GIATranslator.h"
#include "GIATranslatorOperations.h"
#include "GIAEntityNodeClass.h"
#include "GIAdraw.h"
#include "GIAquery.h"
#include "GIAXMLconversion.h"
#include "GIACXLconversion.h"
#include "GIAdatabase.h"
#include "GIAnlp.h"
#include "XMLrulesClass.h"
#include "LDsprite.h"
#include "LDopengl.h"
#ifdef USE_CE
	#include "CEcreateLayout.h"
	#include "CEvars.h"
#endif

static char errmessage[] = "Usage:  GIA.exe [options]\n\n\twhere options are any of the following\n"
#ifdef USE_CE
"\n\t-iclaims [string]  : enumerated claims layout .txt filename to be parsed/generated by CE.exe (def: claimsEnumerated.txt)"
#endif
"\n\t-itxt [string]     : plain text .txt input filename to be parsed by the NLP parser (def: inputText.txt)"
"\n\t-ionlprel [string] : NLP dependency relation parser .xml intermediary input/output filename (def: inputNLPrelation.xml)"
"\n\t-ionlptag [string] : NLP feature tag parser .xml intermediary input/output filename (def: inputNLPfeature.xml)"
"\n\t-ixml [string]     : semantic network definition .xml input filename (def: semanticNet.xml)"
"\n\t-itxtq [string]    : query plain text .txt input filename to be parsed by the NLP processor (def: inputTextQuery.txt)"
"\n\t-ionlprelq [string]: query NLP dependency relation parser .xml intermediary input/output filename (def: inputNLPrelationQuery.xml)"
"\n\t-ionlptagq [string]: query NLP feature tag parser .xml intermediary input/output filename (def: inputNLPfeatureQuery.xml)"
"\n\t-ixmlq [string]    : query semantic network definition .xml input filename (def: semanticNetQuery.xml)"
"\n\t-oxml [string]     : semantic network definition .xml output filename (def: semanticNet.xml)"
"\n\t-ocxl [string]     : semantic network display .cxl vector graphics output filename (def: semanticNet.cxl)"
"\n\t-osvg [string]     : semantic network display .svg 2D vector graphics output filename (def: semanticNet.svg)"
"\n\t-oldr [string]     : semantic network display .ldr 3D vector graphics output filename (def: semanticNet.ldr)"
"\n\t-oppm [string]     : semantic network display .ppm raster graphics output filename (def: semanticNet.ppm)"
"\n\t-oxmlq [string]    : query semantic network definition .xml output filename (def: semanticNetQuery.xml)"
"\n\t-ocxlq [string]    : query semantic network display .cxl vector graphics output filename (def: semanticNetQuery.cxl)"
"\n\t-osvgq [string]    : query semantic network display .svg 2D vector graphics output filename (def: semanticNetQuery.svg)"
"\n\t-oldrq [string]    : query semantic network display .ldr 3D vector graphics output filename (def: semanticNetQuery.ldr)"
"\n\t-oppmq [string]    : query semantic network display .ppm raster graphics output filename (def: semanticNetQuery.ppm)"
"\n\t-oall [string]     : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
"\n\t-oanswer [string]  : plain text .txt file containing the answer to the query (def: answer.txt)"
"\n\t-notshow           : do not display outputText in opengl"
"\n\t-width [int]       : raster graphics width in pixels (def: 640)"
"\n\t-height [int]      : raster graphics height in pixels (def: 480)"
"\n\t-nlprelation [int] : NLP dependency relation parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP, 2 - Stanford Parser [def])"
"\n\t-nlpfeature [int]  : NLP feature parser to be executed by GIA (def: same developer as nlprelation) (0 - Relex, 1 - Stanford Core NLP [def], 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmode       : sets Relex into Stanford compatibilty mode (Relex dependency relation parser creates Stanford relations type) [UNTESTED]"
"\n\t-nlprelationq [int]: query NLP dependency relation parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP, 2 - Stanford Parser [def])"
"\n\t-nlpfeatureq [int] : query NLP feature parser to be executed by GIA (def: same developer as nlprelation) (0 - Relex, 1 - Stanford Core NLP [def], 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmodeq      : query sets Relex into Stanford compatibilty mode (Relex dependency relation parser creates Stanford relations type) [UNTESTED]"
"\n"
"\n\t-workingfolder [string]   : working directory name for input files (def: same as exe)"
"\n\t-nlprelexefolder [string] : exe directory name for NLP dependency relation parser executable (def: same as exe)"
"\n\t-nlptagexefolder [string] : exe directory name for NLP feature tag parser executable (def: same as nlprelexefolder)"
"\n\t-nlprelqexefolder [string] : query exe directory name for NLP dependency relation parser executable (def: same as nlprelexefolder)"
"\n\t-nlptagqexefolder [string] : query exe directory name for NLP feature tag parser executable (def: same as nlprelqexefolder)"
"\n\t-tempfolder [string]      : temp directory name for temporary and outputText files (def: same as exe)"
"\n"
"\n\n\t-version         : print version"
"\n\n\tThis program performs GIA (General Intelligence Algorithm) operations - creates semantic network based upon NPL dependencies file (.xml) or GIA semantic network file (.xml); outputTexts semantic network to GIA semantic network file (.xml); displays semantic network (using opengl); prints semantic network to raster image (.ppm), 3D vector graphics (.ldr), or 2D vector graphics (.svg).\n\n";

//Dependency Relationship Extractor

static int dependencyRelationsTypes[GIA_NLP_PARSER_NUMBER_OF_TYPES] = {GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE}; 

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

	int NLPfeatureParser = GIA_NLP_FEATURE_PARSER_DEFAULT;
	int NLPdependencyRelationsParser = GIA_NLP_DEPENDENCY_RELATIONS_PARSER_DEFAULT;
	bool NLPrelexCompatibilityMode = false;		//sets Relex dependency Relations to Stanford type

	int queryNLPfeatureParser = GIA_NLP_FEATURE_PARSER_DEFAULT;
	int queryNLPdependencyRelationsParser = GIA_NLP_DEPENDENCY_RELATIONS_PARSER_DEFAULT;
	bool queryNLPrelexCompatibilityMode = false;		//sets Relex dependency Relations to Stanford type
		
	bool useInputTextPlainTXTFile = false;
	string inputTextPlainTXTFileName = "inputText.txt";

#ifdef USE_CE
	bool useInputTextClaimsTXTFileName = false;
	string inputTextClaimsTXTFileName = "claimsEnumerated.txt";
#endif

	bool useInputTextNLPrelationXMLFile = false;
	string inputTextNLPrelationXMLFileName = "inputNLPrelation.xml";
	
	bool useInputTextNLPfeatureXMLFile = false;
	string inputTextNLPfeatureXMLFileName = "inputNLPfeature.xml";	
	
	bool useInputTextXMLFile = false;
	string inputTextXMLFileName = "semanticNet.xml";

	bool useOutputTextXMLFile = false;
	string outputTextXMLFileName = "semanticNet.xml";

	bool useOutputTextCXLFile = false;
	string outputTextCXLFileName = "semanticNet.cxl";
		
	bool useOutputTextLDRFile = false;
	string outputTextLDRFileName = "semanticNet.ldr";
	
	bool useOutputTextPPMFile = false;
	string outputTextPPMFileName = "semanticNet.ppm";
	
	bool useOutputTextSVGFile = false;
	string outputTextSVGFileName = "semanticNet.svg";
		
	bool useInputQueryPlainTXTFile = false;
	string inputQueryPlainTXTFileName = "inputTextQuery.txt";
		
	bool useInputQueryNLPrelationXMLFile = false;
	string inputQueryNLPrelationXMLFileName = "inputNLPrelationQuery.xml";
	
	bool useInputQueryNLPfeatureXMLFile = false;
	string inputQueryNLPfeatureXMLFileName = "inputNLPfeatureQuery.xml";	
	
	bool useInputQueryXMLFile = false;
	string inputQueryXMLFileName = "semanticNetQuery.xml";	
	
	bool useOutputQueryXMLFile = false;
	string outputQueryXMLFileName = "semanticNetQuery.xml";

	bool useOutputQueryCXLFile = false;
	string outputQueryCXLFileName = "semanticNetQuery.cxl";
		
	bool useOutputQueryLDRFile = false;
	string outputQueryLDRFileName = "semanticNetQuery.ldr";
	
	bool useOutputQueryPPMFile = false;
	string outputQueryPPMFileName = "semanticNetQuery.ppm";
	
	bool useOutputQuerySVGFile = false;
	string outputQuerySVGFileName = "semanticNetQuery.svg";
		
	bool useOutputTextAllFile = false;
	string outputTextAllFileName = "semanticNet";
			
	bool useOutputTextAnswerPlainTXTFile = false;
	string outputTextAnswerPlainTXTFileName = "answer.txt";
			
	bool printOutput = false;
	bool printOutputQuery = false;
	bool displayInOpenGLAndOutputScreenshot = true;

	int rasterImageWidth = 640;
	int rasterImageHeight = 480; 
	
	bool useInputQuery = false;
	
 
	//bool train = false;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml inputText file is specified, just form network - do not train network

	#ifdef USE_CE
	if(exists_argument(argc,argv,"-iclaims"))
	#else
	if(exists_argument(argc,argv,"-itxt") || exists_argument(argc,argv,"-ionlprel") || exists_argument(argc,argv,"-ixml"))	
	#endif
	{
		if(exists_argument(argc,argv,"-itxt"))
		{
			inputTextPlainTXTFileName=get_char_argument(argc,argv,"-itxt");
			useInputTextPlainTXTFile = true;
		}
	
	#ifdef USE_CE
		if(exists_argument(argc,argv,"-iclaims"))
		{
			inputTextClaimsTXTFileName=get_char_argument(argc,argv,"-iclaims");
			useInputTextClaimsTXTFileName = true;
		}
	#endif
	
		if(exists_argument(argc,argv,"-ionlprel"))
		{
			inputTextNLPrelationXMLFileName=get_char_argument(argc,argv,"-ionlprel");
			useInputTextNLPrelationXMLFile = true;
		}
		if(exists_argument(argc,argv,"-ionlptag"))
		{
			inputTextNLPfeatureXMLFileName=get_char_argument(argc,argv,"-ionlptag");
			useInputTextNLPfeatureXMLFile = true;
		}		

		if(exists_argument(argc,argv,"-ixml"))
		{
			inputTextXMLFileName=get_char_argument(argc,argv,"-ixml");
			//train = true;
			useInputTextXMLFile = true;
		}

		if(exists_argument(argc,argv,"-itxtq"))
		{
			inputQueryPlainTXTFileName=get_char_argument(argc,argv,"-itxtq");
			useInputQueryPlainTXTFile = true;
			useInputQuery = true;
		}

		if(exists_argument(argc,argv,"-ionlprelq"))
		{
			inputQueryNLPrelationXMLFileName=get_char_argument(argc,argv,"-ionlprelq");
			useInputQueryNLPrelationXMLFile = true;
			useInputQuery = true;
		}
		if(exists_argument(argc,argv,"-ionlptagq"))
		{
			inputQueryNLPfeatureXMLFileName=get_char_argument(argc,argv,"-ionlptagq");
			useInputQueryNLPfeatureXMLFile = true;
			useInputQuery = true;
		}
		

		if(exists_argument(argc,argv,"-ixmlq"))
		{
			inputQueryXMLFileName=get_char_argument(argc,argv,"-ixmlq");
			useInputQueryXMLFile = true;
			useInputQuery = true;
		}
		
		if(exists_argument(argc,argv,"-oxml"))
		{
			outputTextXMLFileName=get_char_argument(argc,argv,"-oxml");
			useOutputTextXMLFile = true;
		}

		if(exists_argument(argc,argv,"-ocxl"))
		{
			outputTextCXLFileName=get_char_argument(argc,argv,"-ocxl");
			useOutputTextCXLFile = true;
		}

		if(exists_argument(argc,argv,"-oldr"))
		{
			outputTextLDRFileName=get_char_argument(argc,argv,"-oldr");
			useOutputTextLDRFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oppm"))
		{
			outputTextPPMFileName=get_char_argument(argc,argv,"-oppm");
			useOutputTextPPMFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-osvg"))
		{
			outputTextSVGFileName=get_char_argument(argc,argv,"-osvg");
			useOutputTextSVGFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oxmlq"))
		{
			outputQueryXMLFileName=get_char_argument(argc,argv,"-oxmlq");
			useOutputQueryXMLFile = true;
		}

		if(exists_argument(argc,argv,"-ocxlq"))
		{
			outputQueryCXLFileName=get_char_argument(argc,argv,"-ocxlq");
			useOutputQueryCXLFile = true;
		}

		if(exists_argument(argc,argv,"-oldrq"))
		{
			outputQueryLDRFileName=get_char_argument(argc,argv,"-oldrq");
			useOutputQueryLDRFile = true;
			printOutputQuery = true;
		}

		if(exists_argument(argc,argv,"-oppmq"))
		{
			outputQueryPPMFileName=get_char_argument(argc,argv,"-oppmq");
			useOutputQueryPPMFile = true;
			printOutputQuery = true;
		}

		if(exists_argument(argc,argv,"-osvgq"))
		{
			outputQuerySVGFileName=get_char_argument(argc,argv,"-osvgq");
			useOutputQuerySVGFile = true;
			printOutputQuery = true;
		}
		if(exists_argument(argc,argv,"-oall"))
		{
			outputTextAllFileName=get_char_argument(argc,argv,"-oall");
			useOutputTextAllFile = true;
			printOutput = true;
		}
		
		if(exists_argument(argc,argv,"-oanswer"))
		{
			outputTextAnswerPlainTXTFileName=get_char_argument(argc,argv,"-oanswer");
			useOutputTextAnswerPlainTXTFile = true;
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


		if(exists_argument(argc,argv,"-nlprelation"))
		{
			NLPdependencyRelationsParser = int(get_float_argument(argc,argv,"-nlprelation"));
		}
		
		if(exists_argument(argc,argv,"-nlpfeature"))
		{
			NLPfeatureParser = int(get_float_argument(argc,argv,"-nlpfeature"));
		}
		else
		{
			NLPfeatureParser = NLPdependencyRelationsParser;
		}
		if(exists_argument(argc,argv,"-nlpcompmode"))
		{
			if(NLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
			{
				NLPrelexCompatibilityMode = true;
			}
		}
		
		if(exists_argument(argc,argv,"-nlprelationq"))
		{
			queryNLPdependencyRelationsParser = int(get_float_argument(argc,argv,"-nlprelationq"));
		}
		
		if(exists_argument(argc,argv,"-nlpfeatureq"))
		{
			queryNLPfeatureParser = int(get_float_argument(argc,argv,"-nlpfeatureq"));
		}
		else
		{
			queryNLPfeatureParser = queryNLPdependencyRelationsParser;
		}
		if(exists_argument(argc,argv,"-nlpcompmodeq"))
		{
			if(queryNLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
			{
				queryNLPrelexCompatibilityMode = true;
			}
		}
						
								
		if (exists_argument(argc,argv,"-workingfolder"))
		{
			workingFolderCharStar=get_char_argument(argc,argv,"-workingfolder");
		}
		else
		{
			workingFolderCharStar = currentFolder;		
		}
		if (exists_argument(argc,argv,"-nlprelexefolder"))
		{
			NPLrelationExeFolderCharStar=get_char_argument(argc,argv,"-nlprelexefolder");
		}
		else
		{
			NPLrelationExeFolderCharStar = currentFolder;
		}
		if (exists_argument(argc,argv,"-nlptagexefolder"))
		{
			NPLfeatureExeFolderCharStar=get_char_argument(argc,argv,"-nlptagexefolder");
		}
		else
		{
			NPLfeatureExeFolderCharStar = NPLrelationExeFolderCharStar;
		}
		if (exists_argument(argc,argv,"-nlprelqexefolder"))
		{
			queryNPLrelationExeFolderCharStar=get_char_argument(argc,argv,"-nlprelqexefolder");
		}
		else
		{
			queryNPLrelationExeFolderCharStar = NPLrelationExeFolderCharStar;
		}
		if (exists_argument(argc,argv,"-nlptagqexefolder"))
		{
			queryNPLfeatureExeFolderCharStar=get_char_argument(argc,argv,"-nlptagqexefolder");
		}
		else
		{
			queryNPLfeatureExeFolderCharStar = queryNPLrelationExeFolderCharStar;
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
			cout << "GIA.exe version: 1i8a" << endl;
			exit(1);
		}

		
	}
	else
	{
		cout << "error: GIA requires either a plain text inputText file (.txt), an NPL parsed inputText file (.xml) or GIA semantic network (.xml) to be defined" << endl;
		printf(errmessage);
		exit(1);
	}
	
	/*	
	cout << "workingFolderCharStar = " << workingFolderCharStar << endl;
	cout << "tempFolderCharStar = " << tempFolderCharStar << endl;
	cout << "NPLrelationExeFolderCharStar = " << NPLrelationExeFolderCharStar << endl;
	cout << "NPLfeatureExeFolderCharStar = " << NPLfeatureExeFolderCharStar << endl;
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
	unordered_map<string, GIAEntityNode*> * conceptEntityNodesList = new unordered_map<string, GIAEntityNode*>;
	vector<GIAEntityNode*> * propertyEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * actionEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * conditionEntityNodesList = new vector<GIAEntityNode*>;
	vector<GIATimeConditionNode*> * timeConditionNodesList = new vector<GIATimeConditionNode*>;
	vector<long> * timeConditionNumbersList = new vector<long>;	
		
	vector<GIAEntityNode*> * entityNodesCompleteListQuery = new vector<GIAEntityNode*>;	
	unordered_map<string, GIAEntityNode*> * conceptEntityNodesListQuery = new unordered_map<string, GIAEntityNode*>;
	vector<GIAEntityNode*> * propertyEntityNodesListQuery = new  vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * actionEntityNodesListQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * conditionEntityNodesListQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry	
	vector<GIATimeConditionNode*> * timeConditionNodesListQuery = new vector<GIATimeConditionNode*>;
	vector<long> * timeConditionNumbersListQuery = new vector<long>;	
			
	if(printOutput)
	{
		if(!useOutputTextXMLFile)
		{	
			if(useOutputTextAllFile)
			{	
				useOutputTextXMLFile = true;		
				outputTextXMLFileName = outputTextAllFileName + ".xml";
			}
		}	
		if(!useOutputTextCXLFile)
		{	
			if(useOutputTextAllFile)
			{	
				useOutputTextCXLFile = true;		
				outputTextCXLFileName = outputTextAllFileName + ".cxl";
			}
		}			
		if(!useOutputTextLDRFile)
		{		
			if(useOutputTextAllFile || displayInOpenGLAndOutputScreenshot)		//LDR outputText is always required when displaying semantic network in OpenGL and outputTexting screenshot
			{
				useOutputTextLDRFile = true;			
				outputTextLDRFileName = outputTextAllFileName + ".ldr";
			}
		}
		if(!useOutputTextSVGFile)
		{
			if(useOutputTextAllFile)
			{				
				useOutputTextSVGFile = true;
				outputTextSVGFileName = outputTextAllFileName + ".svg";
			}
		}
		if(!useOutputTextPPMFile)
		{
			if(useOutputTextAllFile)
			{
				useOutputTextPPMFile = true;		
				outputTextPPMFileName = outputTextAllFileName + ".ppm";
			}
		}
		
		if(displayInOpenGLAndOutputScreenshot)
		{
			initiateOpenGL(rasterImageWidth, rasterImageHeight);
		}	
		
		if(useInputQuery)
		{		
			if(useOutputTextAllFile)
			{
				printOutputQuery = true;
			}
			
			if(!useOutputQueryXMLFile)
			{	
				if(useOutputTextAllFile)
				{	
					useOutputQueryXMLFile = true;		
					outputQueryXMLFileName = outputTextAllFileName + "Query.xml";
				}
			}	
			if(!useOutputQueryCXLFile)
			{	
				if(useOutputTextAllFile)
				{	
					useOutputQueryCXLFile = true;		
					outputQueryCXLFileName = outputTextAllFileName + "Query.cxl";
				}
			}			
			if(!useOutputQueryLDRFile)
			{		
				if(useOutputTextAllFile || displayInOpenGLAndOutputScreenshot)		//LDR outputQuery is always required when displaying semantic network in OpenGL and outputQuerying screenshot
				{
					useOutputQueryLDRFile = true;			
					outputQueryLDRFileName = outputTextAllFileName + "Query.ldr";
				}
			}
			if(!useOutputQuerySVGFile)
			{
				if(useOutputTextAllFile)
				{				
					useOutputQuerySVGFile = true;
					outputQuerySVGFileName = outputTextAllFileName + "Query.svg";
				}
			}
			if(!useOutputQueryPPMFile)
			{
				if(useOutputTextAllFile)
				{				
					useOutputQueryPPMFile = true;		
					outputQueryPPMFileName = outputTextAllFileName + "Query.ppm";
				}
			}				
		}	
	}
	
	/*
	cout << errmessage << endl;
	cout << "Parameters to be used:" << endl; 	
	cout << "inputTextPlainTXTFileName = " << inputTextPlainTXTFileName << endl;
	cout << "inputTextNLPrelationXMLFileName = " << inputTextNLPrelationXMLFileName << endl;
	cout << "inputTextNLPfeatureXMLFileName = " << inputTextNLPfeatureXMLFileName << endl;	
	cout << "inputTextXMLFileName = " << inputTextXMLFileName << endl;
	cout << "outputTextXMLFileName = " << outputTextXMLFileName << endl;
	cout << "outputTextCXLFileName = " << outputTextCXLFileName << endl;
	cout << "outputTextLDRFileName = " << outputTextLDRFileName << endl;
	cout << "outputTextPPMFileName = " << outputTextPPMFileName << endl;
	cout << "outputTextSVGFileName = " << outputTextSVGFileName << endl;
	cout << "inputQueryPlainTXTFileName = " << inputQueryPlainTXTFileName << endl;
	cout << "inputQueryNLPrelationXMLFileName = " << inputQueryNLPrelationXMLFileName << endl;
	cout << "inputQueryNLPfeatureXMLFileName = " << inputQueryNLPfeatureXMLFileName << endl;	
	cout << "inputQueryXMLFileName = " << inputQueryXMLFileName << endl;
	cout << "outputQueryXMLFileName = " << outputQueryXMLFileName << endl;
	cout << "outputQueryCXLFileName = " << outputQueryCXLFileName << endl;
	cout << "outputQueryLDRFileName = " << outputQueryLDRFileName << endl;
	cout << "outputQueryPPMFileName = " << outputQueryPPMFileName << endl;
	cout << "outputQuerySVGFileName = " << outputQuerySVGFileName << endl;
	cout << "useOutputTextAnswerPlainTXTFile = " << useOutputTextAnswerPlainTXTFile << endl;
	cout << "displayInOpenGLAndOutputScreenshot = " << displayInOpenGLAndOutputScreenshot << endl;
	cout << "rasterImageWidth = " << rasterImageWidth << endl;
	cout << "rasterImageHeight = " << rasterImageHeight << endl;
	*/
	
#ifdef USE_CE
	//generate claims heirachy
	//NB claimLayoutFileName = inputTextPlainTXTFileName;
	//NB claimEnumeratedFileName = inputTextClaimsTXTFileName;	
	string subclaimPrependPartA = CE_SUB_CLAIM_PREPEND_PART_A_DEFAULT;
	string subclaimPrependPartC = CE_SUB_CLAIM_PREPEND_PART_C_DEFAULT;
	#ifdef GIA_WITH_CE_DERIVE_SUBCLAIM_PREPEND
	bool deriveSubclaimPrepend = true;
	#else
	bool deriveSubclaimPrepend = false;		
	#endif

	bool generateClaimClassHeirachy = true;		//NB this is required for external applications needing to extract claims layout information using CE
	CEClaim * firstClaimInHeirachy = new CEClaim();		
	vector<CEClaim*> * claimsList = new vector<CEClaim*>;

	if(!deriveSubclaimPrependAndCreateClaimsLayout(inputTextClaimsTXTFileName, inputTextPlainTXTFileName, subclaimPrependPartA, subclaimPrependPartC, deriveSubclaimPrepend, generateClaimClassHeirachy, firstClaimInHeirachy, claimsList))
	{
		result = false;
	}
	
	if(!useInputTextNLPrelationXMLFile)
	{//if NLP parsed claims not available, then execute NLP parser on generated claims summary (inputTextPlainTXTFileName)
		useInputTextPlainTXTFile = true;
	}
#endif	
					
	if(useInputTextPlainTXTFile)
	{
		/*
		if(useInputTextNLPrelationXMLFile)
		{
			cout << "error: useInputTextPlainTXTFile && useInputTextNLPrelationXMLFile" << endl;
			exit(0);
		}
		else 
		*/
		if(useInputTextXMLFile)
		{
			cout << "error: useInputTextPlainTXTFile && useInputTextXMLFile" << endl;
			exit(0);
		}
		else
		{	
			executeNLPparser(inputTextPlainTXTFileName, inputTextNLPrelationXMLFileName, NLPdependencyRelationsParser, GIA_NLP_PARSER_TYPE_RELATIONS, false);	
			if(inputTextNLPfeatureXMLFileName != inputTextNLPrelationXMLFileName)
			{
				executeNLPparser(inputTextPlainTXTFileName, inputTextNLPfeatureXMLFileName, NLPfeatureParser, GIA_NLP_PARSER_TYPE_FEATURES, false);	
			}		
			useInputTextNLPrelationXMLFile = true;	//now will parse the NLP Parsed file
		}
	}		
	
	if(useInputTextNLPrelationXMLFile)
	{
		if(useInputTextXMLFile)
		{
			cout << "error: useInputTextXMLFile && useInputTextNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{
			//cout << "as" << endl;
			#ifdef USE_CE	
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, timeConditionNodesList, timeConditionNumbersList, false, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, firstClaimInHeirachy, claimsList))
			#else
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, timeConditionNodesList, timeConditionNumbersList, false, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode))
			#endif
			{
				result = false;
			}
			
			//cout << "as2" << endl;
		}
	}
	
	if(useInputTextXMLFile)
	{
		if(useInputTextPlainTXTFile)
		{
			cout << "error: useInputTextXMLFile && useInputTextPlainTXTFile" << endl;
			exit(0);		
		}
		if(useInputTextNLPrelationXMLFile)
		{
			cout << "error: useInputTextXMLFile && useInputTextNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{		
			if(!readSemanticNetXMLFileOptimised(inputTextXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList))
			{
				result = false;
			}
			
		}
	}
			
	if(useInputQueryPlainTXTFile)
	{
		/*
		if(useInputQueryNLPrelationXMLFile)
		{
			cout << "error: useInputQueryPlainTXTFile && useInputQueryNLPrelationXMLFile" << endl;
			exit(0);
		}
		else 
		*/
		if(useInputQueryXMLFile)
		{
			cout << "error: useInputQueryPlainTXTFile && useInputQueryXMLFile" << endl;
			exit(0);
		}
		else
		{	
			executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPrelationXMLFileName, queryNLPdependencyRelationsParser, GIA_NLP_PARSER_TYPE_RELATIONS, true);	
			if(inputQueryNLPfeatureXMLFileName != inputQueryNLPrelationXMLFileName)
			{
				executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPfeatureXMLFileName, queryNLPfeatureParser, GIA_NLP_PARSER_TYPE_FEATURES, true);	
			}					
			useInputQueryNLPrelationXMLFile = true;	//now will parse the NLP Parsed file
		}
	}
			
	if(useInputQueryNLPrelationXMLFile)
	{
		if(useInputQueryXMLFile)
		{
			cout << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{
			#ifdef USE_CE	
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery, timeConditionNodesListQuery, timeConditionNumbersListQuery, true, queryNLPfeatureParser, queryNLPdependencyRelationsParser, queryNLPrelexCompatibilityMode, firstClaimInHeirachy, claimsList))
			#else
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery, timeConditionNodesListQuery, timeConditionNumbersListQuery, true, queryNLPfeatureParser, queryNLPdependencyRelationsParser, queryNLPrelexCompatibilityMode))			
			#endif
			{
				result = false;
			}					
		}

		if(useOutputQueryXMLFile)
		{			
			if(!writeSemanticNetXMLFileOptimised(outputQueryXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery))
			{
				result = false;
			}
		}

						
	}
		
	if(useInputQueryXMLFile)
	{
		if(useInputQueryPlainTXTFile)
		{
			cout << "error: useInputQueryXMLFile && useInputQueryPlainTXTFile" << endl;
			exit(0);		
		}
		if(useInputQueryNLPrelationXMLFile)
		{
			cout << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{		
			entityNodesCompleteListQuery = new vector<GIAEntityNode*>;
			if(!readSemanticNetXMLFileOptimised(inputQueryXMLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery))
			{
				result = false;
			}
			
		}
	}

	if(useInputQuery)
	{
		if(printOutputQuery)	//moved here in version 1i8a
		{	
			printGIAnetworkNodes(entityNodesCompleteListQuery, rasterImageWidth, rasterImageHeight, outputQueryLDRFileName, outputQuerySVGFileName, outputQueryPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputQueryLDRFile, useOutputQueryPPMFile, useOutputQuerySVGFile);
		}	
		if(useOutputQueryCXLFile)	//moved here in version 1i8a
		{
			if(!writeCMapToolsCXLFileOptimised(outputQueryCXLFileName, entityNodesCompleteListQuery, conceptEntityNodesListQuery, propertyEntityNodesListQuery, actionEntityNodesListQuery, conditionEntityNodesListQuery))
			{
				result = false;
			}	
		}
			
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
		//cout << "a" << endl;
		queryAnswerNode = answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(conceptEntityNodesList, conceptEntityNodesListQuery, foundComparisonVariable, comparisonVariableNode, &foundAnswer, queryAnswerNode, &confidence, &queryAnswerPreviousNode, &queryAnswerContext);
		//cout << "b" << endl;
		
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
			if(foundComparisonVariable)
			{		
				#ifndef GIA_DO_NOT_PRINT_RESULTS
				cout << "Answer Not Found." << endl;
				#endif
				answerString = answerString + "\nAnswer Not Found.";
			}
			else
			{
				if(confidence >= (maxConfidence-0.0001))
				{
					#ifndef GIA_DO_NOT_PRINT_RESULTS
					cout << "Answer: Yes." << endl;
					#endif
					answerString = answerString + "\nAnswer: Yes.";				
				}
				else
				{ 
					#ifndef GIA_DO_NOT_PRINT_RESULTS
					cout << "Answer: No." << endl;
					#endif
					answerString = answerString + "\nAnswer: No.";
				}				
			}
		}
			
		if(foundAnswer && !foundComparisonVariable)
		{
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			cout << "Best Inexact Answer Found: " << queryAnswerNode->entityName << endl;
			#endif
			answerString = answerString + "\nBest Inexact Answer Found: " + queryAnswerNode->entityName;
		}

		if(foundAnswer)
		{
			#ifdef GIA_COMPILE_PRINT_INEXACT_ANSWER_AMBIENT_CONTEXT
			int irrelevant;
			string printEntityNodeString = "";
			traceEntityNode(queryAnswerNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT, &irrelevant, &printEntityNodeString);	
			answerString = answerString + printEntityNodeString;	
			#else
			string printEntityNodeQualitiesString = "";
			printEntityNodeQualitiesOnly(queryAnswerNode, &printEntityNodeQualitiesString);	
			cout << printEntityNodeQualitiesString;
			answerString = answerString + printEntityNodeQualitiesString;
			
			string printEntityNodeTimeConditionString = "";
			printEntityTimeConditionNodeOnly(queryAnswerNode, &printEntityNodeTimeConditionString);	
			cout << printEntityNodeTimeConditionString;
			answerString = answerString + printEntityNodeTimeConditionString;								
			#endif
					
			//print AnswerPreviousNode relationship with answerNode
			#ifndef GIA_DO_NOT_PRINT_RESULTS
			answerString = answerString + "\nAnswer Context: " + queryAnswerContext;
			cout << "Answer Context: " << queryAnswerContext << endl;
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
		char * outputTextAnswerPlainTXTFileNameCharStar = const_cast<char*>(outputTextAnswerPlainTXTFileName.c_str());	
		writeByteArrayToFile(outputTextAnswerPlainTXTFileNameCharStar, fileByteArray, answerString.length());		
	}
	else if(useOutputTextAnswerPlainTXTFile)
	{
		cout << "error: outputText answer require a query to be set" << endl;
	}
	
	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif	
			
	if(printOutput)
	{	
		printGIAnetworkNodes(entityNodesCompleteList, rasterImageWidth, rasterImageHeight, outputTextLDRFileName, outputTextSVGFileName, outputTextPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputTextLDRFile, useOutputTextPPMFile, useOutputTextSVGFile);
	}			
	
	#ifdef GIA_XML_DEBUG_TEST_WRITE_READ_WRITE
	if(!testReadSemanticNetXMLFile2(entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList))
	{
		result = false;
	}
	#else				
	if(useOutputTextXMLFile)
	{			
		if(!writeSemanticNetXMLFileOptimised(outputTextXMLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList))
		{
			result = false;
		}
	}
	if(useOutputTextCXLFile)
	{
		if(!writeCMapToolsCXLFileOptimised(outputTextCXLFileName, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList))
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


#ifdef USE_CE	
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList)
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode)
#endif
{
	bool result = true;
	
	Paragraph * firstParagraphInList = new Paragraph();
	
	if(!parseNLPParserFile(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, isQuery, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode))	
	{
		result = false;
	}

	#ifdef USE_CE	
	if(!createSemanticNetworkBasedUponDependencyGrammarParsedSentences(firstParagraphInList, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, timeConditionNodesList, timeConditionNumbersList, isQuery, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, firstClaimInHeirachy, claimsList))
	#else
	if(!createSemanticNetworkBasedUponDependencyGrammarParsedSentences(firstParagraphInList, entityNodesCompleteList, conceptEntityNodesList, propertyEntityNodesList, actionEntityNodesList, conditionEntityNodesList, timeConditionNodesList, timeConditionNumbersList, isQuery, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode))
	#endif
	{
		result = false;
	}

	return result;
}

#ifdef USE_CE	
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList)
#else
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode)
#endif
{
	bool result = true;
	
	int NLPdependencyRelationsType = dependencyRelationsTypes[NLPdependencyRelationsParser];		
	if(NLPrelexCompatibilityMode)
	{
		NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
	}
			
	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	

	setTranslatorEntityNodesCompleteList(entityNodesCompleteList);
	setTranslatorPropertyEntityNodesList(propertyEntityNodesList);
	setTranslatorActionEntityNodesList(actionEntityNodesList);
	setTranslatorConditionEntityNodesList(conditionEntityNodesList);
	
	initialiseGIATranslatorForTexualContext();
	
	#ifdef USE_CE	
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(firstParagraphInList->next->next != NULL)
		{
			cout << "convertParagraphSentenceRelationsIntoGIAnetworkNodesBasedUponClaimHeirachy(): error - CE only supports a single paragraph of text, one claim per line" << endl;
			exit(0);
		}
		Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;
		#endif
		convertSentenceListRelationsIntoGIAnetworkNodesBasedUponClaimHeirachy(conceptEntityNodesList, timeConditionNodesList, timeConditionNumbersList, firstSentenceInList, firstClaimInHeirachy, claimsList, NLPfeatureParser, NLPdependencyRelationsType);				
	#else	
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			convertParagraphSentenceRelationsIntoGIAnetworkNodes(conceptEntityNodesList, timeConditionNodesList, timeConditionNumbersList, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsType);
		}
		else
		{
			convertSentenceListRelationsIntoGIAnetworkNodes(conceptEntityNodesList, timeConditionNodesList, timeConditionNumbersList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType);
		}
		#else
		convertSentenceListRelationsIntoGIAnetworkNodes(conceptEntityNodesList, timeConditionNodesList, timeConditionNumbersList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType);	
		#endif
	#endif
	
	return result;
}


