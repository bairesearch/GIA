/*******************************************************************************
 *
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n5b 26-July-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
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

Make sure to set the nlprelexfolder to the folder where relex-1.x.0 presides, eg;

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlprelation 0 -notshow
./GIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 0 -nlpfeature 0 -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -notshow

*/

/* Additional Dependencies: Stanford NLP Core

NB execute-stanfordCoreNLP.sh contains the following text;

	java -cp stanford-corenlp-2012-04-03.jar:stanford-corenlp-2012-03-09-models.jar:xom.jar:joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5

NB execute-stanfordCoreNLP.bat contains the following text;

	"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-2012-04-03.jar;stanford-corenlp-2012-03-09-models.jar;xom.jar;joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlpstanfordcorenlpfolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlprelation 1 -notshow

*/

/* Additional Dependencies: Stanford Parser

NB execute-stanfordParser.sh contains the following text;

	#!/usr/bin/env bash
	scriptdir=`dirname $0`
	java -mx2000m -cp "$scriptdir/*:" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz $3/$1 > $4/$2

NB execute-stanfordParser.bat contains the following text;

	@echo off
	:: Runs the English PCFG parser on one or more files, printing trees only
	:: usage: lexparser fileToparse
	"C:\Program Files\Java\jre7\bin\java.exe" -d64 -mx150m -cp "*;" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz %3/%1 > %4/%2

./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlpstanfordparserfolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -nlprelation 2 -notshow

*/

/*
Additional example where relations + features are parsed from different NLP files;
./GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlpfeature 1 -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
./GIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
GIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlpfeature 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
GIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow

Additional example where relations + features parsed from different NLP file, queries parsed using different NLP file;
./GIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 0 -nlpfeatureq 0 -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
[Relex not supported in Windows]

Additional example where relations + features parsed from different NLP file, queries parsed using same/corresponding NLP files;
./GIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "/home/rich/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/rich/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/rich/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
GIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09"

*/

/*
GIA Database is a (Linux) filesystem structure of the format:
	//server/database/w/o/r/word1/0/0/1/referencesOfInstance1.txt
	//server/database/w/o/r/word1/0/0/2/referencesOfInstance2.txt
	/server/database/w/o/r/word2/0/0/1/referencesOfInstance1.txt etc
*/


#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include "GIAmain.h"
#include "GIATranslator.h"
#include "GIATranslatorOperations.h"
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
#ifdef USE_WORDNET
#include "GIAwordnet.h"
#endif
#ifdef GIA_USE_NLG
#include "GIAnlg.h"
#endif

#ifndef LINUX
	#include <windows.h>
#endif


#ifdef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
static char errmessage[] = "GIA Usage Requirement Failure. Please re-specify parameters.\n";
#else
static char errmessage[] = "Usage:  GIA.exe [options]\n\n\twhere options are any of the following\n"
#ifdef USE_CE
"\n\t-iclaims [string]  : enumerated claims layout .txt filename to be parsed/generated by CE.exe (def: claimsEnumerated.txt) [must still specify -itxt [eg same] OR -ionlp]"
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
"\n\t-ocff [string]     : NLP parser generated .cff output filename (def: outputNLP.cff)"
"\n\t-oxmlq [string]    : query semantic network definition .xml output filename (def: semanticNetQuery.xml)"
"\n\t-ocxlq [string]    : query semantic network display .cxl vector graphics output filename (def: semanticNetQuery.cxl)"
"\n\t-osvgq [string]    : query semantic network display .svg 2D vector graphics output filename (def: semanticNetQuery.svg)"
"\n\t-oldrq [string]    : query semantic network display .ldr 3D vector graphics output filename (def: semanticNetQuery.ldr)"
"\n\t-oppmq [string]    : query semantic network display .ppm raster graphics output filename (def: semanticNetQuery.ppm)"
"\n\t-ocffq [string]    : query NLP parser generated .cff output filename (def: outputNLPQuery.cff)"
"\n\t-oall [string]     : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
"\n\t-oanswer [string]  : plain text .txt file containing the answer to the query (def: answer.txt)"
"\n\t-notshow           : do not display outputText in opengl"
"\n\t-width [int]       : raster graphics width in pixels (def: 640)"
"\n\t-height [int]      : raster graphics height in pixels (def: 480)"
"\n\t-nlprelation [int] : NLP dependency relation parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP, 2 - Stanford Parser [def])"
"\n\t-nlpfeature [int]  : NLP feature parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP [def], 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmode [int] : sets Relex into Stanford compatibility mode (0 - off [def], 1 - (assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode, 2 - assumes Relex dependency relations were generated by GIA in cff) [UNTESTED]"
"\n\t-nlprelationq [int]: query NLP dependency relation parser to be executed by GIA (0 - Relex [def], 1 - Stanford Core NLP, 2 - Stanford Parser)"
"\n\t-nlpfeatureq [int] : query NLP feature parser to be executed by GIA (0 - Relex [def], 1 - Stanford Core NLP, 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmodeq [int]: query sets Relex into Stanford compatibility mode (0 - off [def], 1 - (assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode, 2 - assumes Relex dependency relations were generated by GIA in cff) [UNTESTED]"
#ifdef GIA_USE_DATABASE
"\n\t-dbread            : read from database (GIA knowledge base) [improves referencing capacity]"
"\n\t-dbwrite           : write to database (GIA knowledge base) [saves knowledge]"
#endif
"\n"
"\n\t-workingfolder [string]            : working directory name for input files (def: same as exe)"
"\n\t-nlprelexfolder [string]           : directory name for Relex (def: same as exe)"
"\n\t-nlpstanfordcorenlpfolder [string] : directory name for Stanford Parser (def: same as nlprelexefolder)"
"\n\t-nlpstanfordparserfolder [string]  : directory name for Stanford CoreNLP (def: same as nlprelexefolder)"
"\n\t-tempfolder [string]               : temp directory name for temporary and outputText files (def: same as exe)"
"\n"
"\n\n\t-version         : print version"
"\n\n\tThis program performs GIA (General Intelligence Algorithm) operations - creates semantic network based upon NPL dependencies file (.xml) or GIA semantic network file (.xml); outputTexts semantic network to GIA semantic network file (.xml); displays semantic network (using opengl); prints semantic network to raster image (.ppm), 3D vector graphics (.ldr), or 2D vector graphics (.svg).\n\n";
#endif

//Dependency Relationship Extractor

static int dependencyRelationsTypes[GIA_NLP_PARSER_NUMBER_OF_TYPES] = {GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE};

int main(int argc,char **argv)
{
	#ifdef GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP
	initialiseWordNet();
	string wordExample = "like";
	bool wordIsFound = false;
	string listOfSynonyms[WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS];
	int wordNetPOS = VERB;	//NOUN	VERB
	checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(wordExample, &wordIsFound, wordNetPOS);
	//findSynonymsOLD(wordExample, &wordIsFound, listOfSynonyms, wordNetPOS);
	exit(0);
	#endif

	#ifdef USE_WORDNET
	initialiseWordNet();
	#endif

	//print execution time
	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%i:%i:%i %.2i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + GIA_TM_STRUCT_YEAR_OFFSET));
	cout << "GIA execution time: " << timeAndDateString << " (start)" << endl;

	bool result = true;

	int NLPfeatureParser = GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT;
	int NLPdependencyRelationsParser = GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT;
	bool NLPrelexCompatibilityMode = false;			//(assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode
	bool NLPassumePreCollapsedStanfordRelations = false;	//(assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode / assumes Relex dependency relations were generated by GIA in cff

	int queryNLPfeatureParser = GIA_NLP_FEATURE_PARSER_FOR_INPUT_QUERY_DEFAULT;
	int queryNLPdependencyRelationsParser = GIA_NLP_RELATIONS_PARSER_FOR_INPUT_QUERY_DEFAULT;
	bool queryNLPrelexCompatibilityMode = false;			//(assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode
	bool queryNLPassumePreCollapsedStanfordRelations = false;	//(assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode / assumes Relex dependency relations were generated by GIA in cff

	string NLPexeFolderArray[GIA_NLP_PARSER_NUMBER_OF_TYPES];

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

	bool useOutputTextCFFFile = false;
	string outputTextCFFFileName = "outputNLP.cff";

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

	bool useOutputQueryCFFFile = false;
	string outputQueryCFFFileName = "outputNLPQuery.cff";

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

	bool readFromDatabase = false;
	bool writeToDatabase = false;
	bool useDatabase = false;

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
			//cout << "DEBUGZ" << endl;
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
			//cout << "DEBUG ionlprel = " << inputTextNLPrelationXMLFileName << endl;
		}
		if(exists_argument(argc,argv,"-ionlptag"))
		{
			inputTextNLPfeatureXMLFileName=get_char_argument(argc,argv,"-ionlptag");
			useInputTextNLPfeatureXMLFile = true;
			//cout << "DEBUG ionlptag = " << inputTextNLPfeatureXMLFileName << endl;
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
			//cout << "DEBUG ionlprelq = " << inputQueryNLPrelationXMLFileName << endl;
		}
		if(exists_argument(argc,argv,"-ionlptagq"))
		{
			inputQueryNLPfeatureXMLFileName=get_char_argument(argc,argv,"-ionlptagq");
			useInputQueryNLPfeatureXMLFile = true;
			useInputQuery = true;
			//cout << "DEBUG ionlptagq = " << inputQueryNLPfeatureXMLFileName << endl;
		}

		if(exists_argument(argc,argv,"-ixmlq"))
		{
			inputQueryXMLFileName=get_char_argument(argc,argv,"-ixmlq");
			useInputQueryXMLFile = true;
			useInputQuery = true;
		}

		if(exists_argument(argc,argv,"-ocff"))
		{
			outputTextCFFFileName=get_char_argument(argc,argv,"-ocff");
			useOutputTextCFFFile = true;
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

		if(exists_argument(argc,argv,"-ocffq"))
		{
			outputQueryCFFFileName=get_char_argument(argc,argv,"-ocffq");
			useOutputQueryCFFFile = true;
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
			if(queryNLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
			{
				int nlpcompmode = int(get_float_argument(argc,argv,"-nlpcompmode"));
				if(nlpcompmode == 1)
				{
					NLPrelexCompatibilityMode = true;
					NLPassumePreCollapsedStanfordRelations = true;
				}
				else if(nlpcompmode == 2)
				{
					NLPassumePreCollapsedStanfordRelations = true;
				}
			}
			else
			{
				cout << "error: nlpcompmode set but (NLPdependencyRelationsParser != GIA_NLP_PARSER_RELEX)" << endl;
				exit(0);
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
				int nlpcompmodeq = int(get_float_argument(argc,argv,"-nlpcompmodeq"));
				if(nlpcompmodeq == 1)
				{
					queryNLPrelexCompatibilityMode = true;
					queryNLPassumePreCollapsedStanfordRelations = true;
				}
				else if(nlpcompmodeq == 2)
				{
					queryNLPassumePreCollapsedStanfordRelations = true;
				}
			}
			else
			{
				cout << "error: nlpcompmodeq set but (queryNLPdependencyRelationsParser != GIA_NLP_PARSER_RELEX)" << endl;
				exit(0);
			}
		}

		if(exists_argument(argc,argv,"-dbread"))
		{
			readFromDatabase = true;
			useDatabase = true;
		}
		if(exists_argument(argc,argv,"-dbwrite"))
		{
			writeToDatabase = true;
			useDatabase = true;
		}


		if (exists_argument(argc,argv,"-workingfolder"))
		{
			workingFolderCharStar =get_char_argument(argc,argv,"-workingfolder");
		}
		else
		{
			workingFolderCharStar = currentFolder;
		}

		if (exists_argument(argc,argv,"-nlprelexfolder"))
		{
			NLPexeFolderArray[GIA_NLP_PARSER_RELEX] =get_char_argument(argc,argv,"-nlprelexfolder");
		}
		else
		{
			NLPexeFolderArray[GIA_NLP_PARSER_RELEX] = currentFolder;
		}
		if (exists_argument(argc,argv,"-nlpstanfordcorenlpfolder"))
		{
			NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP] =get_char_argument(argc,argv,"-nlpstanfordcorenlpfolder");
		}
		else
		{
			NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP] = currentFolder;
		}
		if (exists_argument(argc,argv,"-nlpstanfordparserfolder"))
		{
			NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER] =get_char_argument(argc,argv,"-nlpstanfordparserfolder");
		}
		else
		{
			NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER] = currentFolder;
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
			cout << "GIA.exe - Project Version: 1n5b 26-July-2012" << endl;
			exit(1);
		}


	}
	else
	{
		#ifdef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
		cout << "error: GIA requires either a plain text inputText file (.txt), an NPL parsed inputText file (.xml) or GIA database (.xml) to be defined" << endl;
		#else
		cout << "error: GIA requires either a plain text inputText file (.txt), an NPL parsed inputText file (.xml) or GIA semantic network (.xml) to be defined" << endl;
		#endif
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

	#ifdef GIA_USE_DATABASE
	if(useDatabase)
	{
		openDatabase(readFromDatabase);
	}
	#endif

	vector<GIAEntityNode*> * entityNodesActiveListComplete = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> * entityNodesActiveListConcepts = new unordered_map<string, GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListProperties = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListActions = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListConditions = new vector<GIAEntityNode*>;
	unordered_map<long, GIATimeConditionNode*> * timeConditionNodesActiveList = new unordered_map<long, GIATimeConditionNode*>;

	vector<GIAEntityNode*> * entityNodesActiveListCompleteQuery = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> * entityNodesActiveListConceptsQuery = new unordered_map<string, GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListPropertiesQuery = new  vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * entityNodesActiveListActionsQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry
	vector<GIAEntityNode*> * entityNodesActiveListConditionsQuery = new vector<GIAEntityNode*>;			//not required - declared for symmetry
	unordered_map<long, GIATimeConditionNode*> * timeConditionNodesActiveListQuery = new unordered_map<long, GIATimeConditionNode*>;

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
		if(!useOutputTextCFFFile)
		{
			if(useOutputTextAllFile)
			{
				useOutputTextCFFFile = true;
				outputTextCFFFileName = outputTextAllFileName + ".cff";
			}
		}

		if(displayInOpenGLAndOutputScreenshot)
		{
			initiateOpenGL(rasterImageWidth, rasterImageHeight, 0, 0, false);
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
			if(!useOutputQueryCFFFile)
			{
				if(useOutputTextAllFile)
				{
					useOutputQueryCFFFile = true;
					outputQueryCFFFileName = outputTextAllFileName + "Query.cff";
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
	cout << "outputTextCFFFileName = " << outputTextCFFFileName << endl;
	cout << "inputQueryPlainTXTFileName = " << inputQueryPlainTXTFileName << endl;
	cout << "inputQueryNLPrelationXMLFileName = " << inputQueryNLPrelationXMLFileName << endl;
	cout << "inputQueryNLPfeatureXMLFileName = " << inputQueryNLPfeatureXMLFileName << endl;
	cout << "inputQueryXMLFileName = " << inputQueryXMLFileName << endl;
	cout << "outputQueryXMLFileName = " << outputQueryXMLFileName << endl;
	cout << "outputQueryCXLFileName = " << outputQueryCXLFileName << endl;
	cout << "outputQueryLDRFileName = " << outputQueryLDRFileName << endl;
	cout << "outputQueryPPMFileName = " << outputQueryPPMFileName << endl;
	cout << "outputQuerySVGFileName = " << outputQuerySVGFileName << endl;
	cout << "outputQueryCFFFileName = " << outputQueryCFFFileName << endl;
	cout << "useOutputTextAnswerPlainTXTFile = " << useOutputTextAnswerPlainTXTFile << endl;
	cout << "displayInOpenGLAndOutputScreenshot = " << displayInOpenGLAndOutputScreenshot << endl;
	cout << "rasterImageWidth = " << rasterImageWidth << endl;
	cout << "rasterImageHeight = " << rasterImageHeight << endl;
	*/

#ifdef USE_CE
	bool useClaimsHeirachy = false;
	CEClaim * firstClaimInHeirachy = NULL;
	vector<CEClaim*> * claimsList;
	if(useInputTextClaimsTXTFileName)
	{
		useClaimsHeirachy = true;
		
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
		firstClaimInHeirachy = new CEClaim();
		claimsList = new vector<CEClaim*>;

		if(!deriveSubclaimPrependAndCreateClaimsLayout(inputTextClaimsTXTFileName, inputTextPlainTXTFileName, subclaimPrependPartA, subclaimPrependPartC, deriveSubclaimPrepend, generateClaimClassHeirachy, firstClaimInHeirachy, claimsList))
		{
			result = false;
		}

		/*//need to explicitly state whether to use a plain text file, else will use NLP xml file
		if(!useInputTextNLPrelationXMLFile)
		{//if NLP parsed claims not available, then execute NLP parser on generated claims summary (inputTextPlainTXTFileName)
			useInputTextPlainTXTFile = true;
		}
		*/
			
		if(useInputTextNLPrelationXMLFile && !(fileExists(&inputTextNLPrelationXMLFileName)))
		{
			useInputTextPlainTXTFile = true;
		}
	}
#endif

	if(useInputTextPlainTXTFile)
	{
		//cout << "DEBUGA" << endl;
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
			executeNLPparser(inputTextPlainTXTFileName, inputTextNLPrelationXMLFileName, NLPdependencyRelationsParser, NLPexeFolderArray);
			if(inputTextNLPfeatureXMLFileName != inputTextNLPrelationXMLFileName)
			{
				executeNLPparser(inputTextPlainTXTFileName, inputTextNLPfeatureXMLFileName, NLPfeatureParser, NLPexeFolderArray);
			}
			useInputTextNLPrelationXMLFile = true;	//now will parse the NLP Parsed file
		}
	}

	if(useInputTextNLPrelationXMLFile)
	{
		//cout << "DEBUGB" << endl;
		//cout << "inputTextNLPrelationXMLFileName = " << inputTextNLPrelationXMLFileName << endl;
		//cout << "inputTextNLPfeatureXMLFileName = " << inputTextNLPfeatureXMLFileName << endl;
		//cout << "outputTextCFFFileName = " << outputTextCFFFileName << endl;

		if(useInputTextXMLFile)
		{
			cout << "error: useInputTextXMLFile && useInputTextNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{
			//cout << "as" << endl;
			#ifdef USE_CE
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, outputTextCFFFileName, NLPexeFolderArray, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, false, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, NLPassumePreCollapsedStanfordRelations, firstClaimInHeirachy, claimsList, useClaimsHeirachy))
			#else
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, outputTextCFFFileName, NLPexeFolderArray, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, false, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, NLPassumePreCollapsedStanfordRelations))
			#endif
			{
				result = false;
			}

			//cout << "as2" << endl;
		}
	}

	if(useInputTextXMLFile)
	{
		//cout << "DEBUGC" << endl;
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
			if(!readSemanticNetXMLFileOptimised(inputTextXMLFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions))
			{
				result = false;
			}
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "record concept nodes as permanent if they are disabled (prepare for use in GIA)" << endl;
			#endif
			recordConceptNodesAsNonPermanentIfTheyAreDisabled(entityNodesActiveListConcepts);	//prepare for use in GIA

		}
	}

	if(useInputQueryPlainTXTFile)
	{
		//cout << "DEBUG2A" << endl;
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
			executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPrelationXMLFileName, queryNLPdependencyRelationsParser, NLPexeFolderArray);
			if(inputQueryNLPfeatureXMLFileName != inputQueryNLPrelationXMLFileName)
			{
				executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPfeatureXMLFileName, queryNLPfeatureParser, NLPexeFolderArray);
			}
			useInputQueryNLPrelationXMLFile = true;	//now will parse the NLP Parsed file
		}
	}

	if(useInputQueryNLPrelationXMLFile)
	{
		//cout << "DEBUG2B" << endl;

		if(useInputQueryXMLFile)
		{
			cout << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
			exit(0);
		}
		else
		{
			#ifdef USE_CE
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, outputQueryCFFFileName, NLPexeFolderArray, entityNodesActiveListCompleteQuery, entityNodesActiveListConceptsQuery, entityNodesActiveListPropertiesQuery, entityNodesActiveListActionsQuery, entityNodesActiveListConditionsQuery, timeConditionNodesActiveListQuery, true, queryNLPfeatureParser, queryNLPdependencyRelationsParser, queryNLPrelexCompatibilityMode, NLPassumePreCollapsedStanfordRelations, firstClaimInHeirachy, claimsList, false))
			#else
			if(!parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, outputQueryCFFFileName, NLPexeFolderArray, entityNodesActiveListCompleteQuery, entityNodesActiveListConceptsQuery, entityNodesActiveListPropertiesQuery, entityNodesActiveListActionsQuery, entityNodesActiveListConditionsQuery, timeConditionNodesActiveListQuery, true, queryNLPfeatureParser, queryNLPdependencyRelationsParser, queryNLPrelexCompatibilityMode, NLPassumePreCollapsedStanfordRelations))
			#endif
			{
				result = false;
			}
		}

		if(useOutputQueryXMLFile)
		{
			if(!writeSemanticNetXMLFileOptimised(outputQueryXMLFileName, entityNodesActiveListCompleteQuery, entityNodesActiveListConceptsQuery, entityNodesActiveListPropertiesQuery, entityNodesActiveListActionsQuery, entityNodesActiveListConditionsQuery))
			{
				result = false;
			}
		}


	}

	if(useInputQueryXMLFile)
	{
		//cout << "DEBUG2C" << endl;

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
			entityNodesActiveListCompleteQuery = new vector<GIAEntityNode*>;
			if(!readSemanticNetXMLFileOptimised(inputQueryXMLFileName, entityNodesActiveListCompleteQuery, entityNodesActiveListConceptsQuery, entityNodesActiveListPropertiesQuery, entityNodesActiveListActionsQuery, entityNodesActiveListConditionsQuery))
			{
				result = false;
			}
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "record concept nodes as permanent if they are disabled (prepare for use in GIA)" << endl;
			#endif
			recordConceptNodesAsNonPermanentIfTheyAreDisabled(entityNodesActiveListConceptsQuery);	//prepare for use in GIA

		}
	}

	if(useInputQuery)
	{
		if(printOutputQuery)	//moved here in version 1i8a
		{
			printGIAnetworkNodes(entityNodesActiveListCompleteQuery, rasterImageWidth, rasterImageHeight, outputQueryLDRFileName, outputQuerySVGFileName, outputQueryPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputQueryLDRFile, useOutputQueryPPMFile, useOutputQuerySVGFile);
		}
		if(useOutputQueryCXLFile)	//moved here in version 1i8a
		{
			if(!writeCMapToolsCXLFileOptimised(outputQueryCXLFileName, entityNodesActiveListCompleteQuery, entityNodesActiveListConceptsQuery, entityNodesActiveListPropertiesQuery, entityNodesActiveListActionsQuery, entityNodesActiveListConditionsQuery))
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
		string queryAnswerContext = "";
		//cout << "a" << endl;
		queryAnswerNode = answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(entityNodesActiveListConcepts, entityNodesActiveListConceptsQuery, foundComparisonVariable, comparisonVariableNode, &foundAnswer, queryAnswerNode, &confidence, &queryAnswerContext);
		//cout << "b" << endl;

		double maxConfidence = determineMaxConfidenceOfQuerySemanticNetwork(entityNodesActiveListConceptsQuery);		//OLD [simple]: entityNodesActiveListCompleteQuery->size();

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
				#ifdef GIA_QUERY_RELAX_CONFIDENCE_REQUIREMENTS_FOR_YES
				if(((maxConfidence <= (3.0+GIA_QUERY_DOUBLE_ERROR)) && (confidence >= (maxConfidence-(0.0+GIA_QUERY_DOUBLE_ERROR)))) || ((maxConfidence >= (4.0-GIA_QUERY_DOUBLE_ERROR)) && (confidence >= (maxConfidence-(1.0+GIA_QUERY_DOUBLE_ERROR)))))
				#else
				if(confidence >= (maxConfidence-GIA_QUERY_DOUBLE_ERROR))
				#endif
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
			#ifdef GIA_QUERY_TRACE_INSTANTIATIONS
			bool traceInstantiations = true;
			#else
			bool traceInstantiations = false;
			#endif
			traceEntityNode(queryAnswerNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_PRINT, &irrelevant, &printEntityNodeString, NULL, traceInstantiations);
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
			
			#ifdef GIA_USE_NLG
			NLGSentence * firstNLGsentence = new NLGSentence();
			//look for action links
			NLGSentence * currentNLGsentence = generateLanguageFromEntityNode(queryAnswerNode, firstNLGsentence, true, 1);
			int irrelevant;
			string printEntityNodeString = "";
			bool traceInstantiations = false;
			traceEntityNode(queryAnswerNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);			
			if(firstNLGsentence->NLGInputViewText == "")
			{
				//look for condition links
				NLGSentence * currentNLGsentence = generateLanguageFromEntityNode(queryAnswerNode, firstNLGsentence, true, 2);
				traceEntityNode(queryAnswerNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);			
				
			}
			if(firstNLGsentence->NLGInputViewText == "")
			{
				//look for property/definition links
				NLGSentence * currentNLGsentence = generateLanguageFromEntityNode(queryAnswerNode, firstNLGsentence, true, 3);
				traceEntityNode(queryAnswerNode, GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_RESET_PARSEDFORLANGUAGEGENERATION, &irrelevant, &printEntityNodeString, false, NULL, traceInstantiations);		
			}
						
			currentNLGsentence = firstNLGsentence;
			while(currentNLGsentence->next != NULL)
			{
				cout << "Answer Context (NLG): " << currentNLGsentence->NLGInputViewText << endl;
				currentNLGsentence = currentNLGsentence->next;
			}			
			#endif
		}


		//add confidence to answer
		char tempConfidenceStringCharStar[100];
		sprintf(tempConfidenceStringCharStar, "%0.6f", confidence*GIA_QUERY_CONFIDENCE_MULTIPLIER);
		char tempMaxConfidenceStringCharStar[100];
		sprintf(tempMaxConfidenceStringCharStar, "%0.6f", maxConfidence*GIA_QUERY_CONFIDENCE_MULTIPLIER);
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
		printGIAnetworkNodes(entityNodesActiveListComplete, rasterImageWidth, rasterImageHeight, outputTextLDRFileName, outputTextSVGFileName, outputTextPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputTextLDRFile, useOutputTextPPMFile, useOutputTextSVGFile);
	}

	#ifdef GIA_XML_DEBUG_TEST_WRITE_READ_WRITE
	if(!testReadSemanticNetXMLFile2(entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions))
	{
		result = false;
	}
	#else
	if(useOutputTextXMLFile)
	{
		if(!writeSemanticNetXMLFileOptimised(outputTextXMLFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions))
		{
			result = false;
		}
	}
	if(useOutputTextCXLFile)
	{
		if(!writeCMapToolsCXLFileOptimised(outputTextCXLFileName, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions))
		{
			result = false;
		}
	}
	#ifdef GIA_USE_NLG
	NLGSentence * firstNLGsentence = new NLGSentence();
	NLGSentence * currentNLGsentence = firstNLGsentence;
	string generatedText = "";
	for(unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListMapIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListMapIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListMapIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListMapIter->second;
		currentNLGsentence = generateLanguageFromEntityNode(entityNode, currentNLGsentence, false, 0); 
	}
	#ifdef GIA_USE_NLG_OUTPUT_TO_COMMAND_LINE
	cout << "DEBUG: NLG generated text = " << endl;	
	#ifdef GIA_USE_NLG2
	cout << "(input text into NLG2)" << endl;		
	#endif
	currentNLGsentence = firstNLGsentence;
	while(currentNLGsentence->next != NULL)
	{
		cout << currentNLGsentence->NLGInputViewText << endl;
		//execute NLG2 on this text
		currentNLGsentence = currentNLGsentence->next;
	}
	#endif	
	#endif
	
	#ifdef GIA_USE_DATABASE
	if(useDatabase)
	{
		if(writeToDatabase)
		{
			//NB currently uses entityNodesActiveListComplete to record which nodes might possibly require an update on the server
			writeAndCloseDatabase(entityNodesActiveListComplete);
		}
		else
		{
			closeDatabase();
		}
	}
	#endif
	#endif



	//print execution time (end)
	time(&now);
	current = localtime(&now);
	sprintf(timeAndDateString, "%i:%i:%i %.2i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + GIA_TM_STRUCT_YEAR_OFFSET));
	cout << "GIA execution time: " << timeAndDateString << " (finish)" << endl;

}


#ifdef USE_CE
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList, bool useClaimsHeirachy)
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations)
#endif
{

	bool result = true;

	Paragraph * firstParagraphInList = new Paragraph();

	if(!parseNLPParserFile(inputTextNLPrelationXMLFileName, inputTextNLPfeatureXMLFileName, isQuery, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode))
	{
		result = false;
	}

	#ifdef USE_CE
	if(!createSemanticNetworkBasedUponDependencyGrammarParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsParser, NLPassumePreCollapsedStanfordRelations, firstClaimInHeirachy, claimsList, useClaimsHeirachy))
	#else
	if(!createSemanticNetworkBasedUponDependencyGrammarParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListProperties, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsParser, NLPassumePreCollapsedStanfordRelations))
	#endif
	{
		result = false;
	}

	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	string originalInputFileName = "";
	originalInputFileName = originalInputFileName + inputTextNLPrelationXMLFileName + " " + inputTextNLPfeatureXMLFileName;
	outputInternalRelationsInRelexFormat(&outputCFFFileName, &originalInputFileName, firstParagraphInList, NLPdependencyRelationsParser, NLPfeatureParser, NLPexeFolderArray);
	#endif

	return result;
}

#ifdef USE_CE
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList, bool useClaimsHeirachy)
#else
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations)
#endif
{
	bool result = true;

	int NLPdependencyRelationsType = dependencyRelationsTypes[NLPdependencyRelationsParser];

	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;

	setTranslatorEntityNodesCompleteList(entityNodesActiveListComplete);
	setTranslatorPropertyEntityNodesList(entityNodesActiveListProperties);
	setTranslatorActionEntityNodesList(entityNodesActiveListActions);
	setTranslatorConditionEntityNodesList(entityNodesActiveListConditions);

	initialiseGIATranslatorForTexualContext();

	#ifdef USE_CE
	if(useClaimsHeirachy)
	{
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(firstParagraphInList->next->next != NULL)
		{
			cout << "convertParagraphSentenceRelationsIntoGIAnetworkNodesBasedUponClaimHeirachy(): error - CE only supports a single paragraph of text, one claim per line" << endl;
			exit(0);
		}
		#endif
		convertSentenceListRelationsIntoGIAnetworkNodesBasedUponClaimHeirachy(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, firstClaimInHeirachy, claimsList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
	}
	else
	{
	#endif
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			convertParagraphSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		}
		else
		{
			convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		}
		#else
		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef USE_CE
	}
	#endif
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record concept nodes as disabled if they are not permanent (used for printing/xml write purposes)" << endl;
	#endif
	recordConceptNodesAsDisabledIfTheyAreNotPermanent(entityNodesActiveListConcepts);

	return result;
}

bool fileExists(string * fileName)
{
	bool result = false;
 	FILE * pFile = NULL;
	const char * fileNameCharStar = fileName->c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile != NULL)
	{
		result = true;
	}
	return result;
}


