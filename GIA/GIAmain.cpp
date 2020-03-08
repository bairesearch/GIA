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
 * File Name: GIAmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1a 05-March-2020
 * Requirements: 
 * Description: Main
 * /
 *******************************************************************************/


#include "GIAmain.hpp"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_NEURAL_NETWORK
#include "ANNdraw.hpp"
#endif

	

	

#ifdef COMPILE_GIA

static char errmessage[] = "Usage:  GIA.exe [options]\n\n\twhere options are any of the following\n"
"\n\t-itxt [string]       : plain text .txt input filename to be parsed by the NLP parser (def: inputText.txt)"
"\n\t-itxtq [string]      : query plain text .txt input filename to be parsed by the NLP processor (def: inputTextQuery.txt)"
#ifdef GIA_NEURAL_NETWORK_ACTIVE
"\n\t-iannxml [string]    : neural network definition .xml input filename (def: neuralNet.xml)"
#endif
#ifndef GIA_DISABLE_SEMANTIC_TRANSLATOR
#ifndef GIA_DISABLE_SYNTACTIC_TRANSLATOR
"\n\t-ionlprel [string]   : NLP dependency relation parser .xml intermediary input/output filename (def: inputNLPrelation.xml)"
"\n\t-ionlptag [string]   : NLP feature tag parser .xml intermediary input/output filename (def: inputNLPfeature.xml)"
"\n\t-ionlprelq [string]  : query NLP dependency relation parser .xml intermediary input/output filename (def: inputNLPrelationQuery.xml)"
"\n\t-ionlptagq [string]  : query NLP feature tag parser .xml intermediary input/output filename (def: inputNLPfeatureQuery.xml)"
#endif
"\n\t-ixml [string]       : semantic network definition .xml input filename (def: semanticNet.xml)"
"\n\t-ixmlq [string]      : query semantic network definition .xml input filename (def: semanticNetQuery.xml)"
#ifdef GIA_INPUT_FILE_LISTS
"\n\t-ilist               : all input files will be treated as file lists (new line delimited)"
#endif
"\n\t-ocff [string]       : semantic network NLP parser generated .cff output filename (def: semanticNet.cff)"
"\n\t-oxml [string]       : semantic network definition .xml output filename (def: semanticNet.xml)"
"\n\t-ocxl [string]       : semantic network display .cxl vector graphics output filename (def: semanticNet.cxl)"
"\n\t-oldr [string]       : semantic network display .ldr 3D vector graphics output filename (def: semanticNet.ldr)"
"\n\t-oppm [string]       : semantic network display .ppm raster graphics output filename (def: semanticNet.ppm)"
"\n\t-osvg [string]       : semantic network display .svg 2D vector graphics output filename (def: semanticNet.svg)"
"\n\t-ocffq [string]      : query semantic network NLP parser generated .cff output filename (def: semanticNetQuery.cff)"
"\n\t-oxmlq [string]      : query semantic network definition .xml output filename (def: semanticNetQuery.xml)"
"\n\t-ocxlq [string]      : query semantic network display .cxl vector graphics output filename (def: semanticNetQuery.cxl)"
"\n\t-oldrq [string]      : query semantic network display .ldr 3D vector graphics output filename (def: semanticNetQuery.ldr)"
"\n\t-oppmq [string]      : query semantic network display .ppm raster graphics output filename (def: semanticNetQuery.ppm)"
"\n\t-osvgq [string]      : query semantic network display .svg 2D vector graphics output filename (def: semanticNetQuery.svg)"
"\n\t-oall [string]       : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
"\n\t-oanswer [string]    : plain text .txt file containing the answer to the query (def: answer.txt)"
#endif
"\n\t-nlprelation [int]   : NLP dependency relation parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP, 2 - Stanford Parser [def])"
"\n\t-nlpfeature [int]    : NLP feature parser to be executed by GIA (0 - Relex, 1 - Stanford Core NLP [def], 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmode [int]   : sets Relex into Stanford compatibility mode (0 - off [def], 1 - (assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode, 2 - assumes Relex dependency relations were generated by GIA in cff) [UNTESTED]"
"\n\t-nlprelationq [int]  : query NLP dependency relation parser to be executed by GIA (0 - Relex [def], 1 - Stanford Core NLP, 2 - Stanford Parser)"
"\n\t-nlpfeatureq [int]   : query NLP feature parser to be executed by GIA (0 - Relex [def], 1 - Stanford Core NLP, 2 - Stanford Parser (ie, none))"
"\n\t-nlpcompmodeq [int]  : query sets Relex into Stanford compatibility mode (0 - off [def], 1 - (assumes) Relex dependency relations are(/were) generated by Relex parser in Stanford compatibility mode, 2 - assumes Relex dependency relations were generated by GIA in cff) [UNTESTED]"
#ifdef GIA_DATABASE
"\n\t-dbread              : read from KB database (GIA knowledge base) [improves referencing capacity]"
"\n\t-dbwrite             : write to KB database (GIA knowledge base) [saves knowledge]"
"\n\t-dbfolder [string]   : KB database base folder path (def: /home/systemusername/source/GIAKBdatabase)"
#endif
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
"\n\t-dbpostaggerfolder [string]   : pos tagger database base folder path (def: /home/systemusername/source/GIAPOStaggerDatabase)"
#endif
#ifdef GIA_SEM_REL_TRANSLATOR
"\n\t-dbsemanticparserfolder [string]   : direct semantic parser (corpus or optimised) database base folder path (def: /home/systemusername/source/GIAsemanticparserdatabase)"
#endif
"\n\t-nlprelexfolder [string]           : directory name for Relex (def: same as exe)"
"\n\t-nlpstanfordcorenlpfolder [string] : directory name for Stanford Parser (def: same as exe)"
"\n\t-nlpstanfordparserfolder [string]  : directory name for Stanford CoreNLP (def: same as exe)"
#ifdef GIA_NLP_CLIENT_SERVER
"\n\t-nlpclient                         : execute NLP as client (requires server to be already started)"
#endif
#endif
#ifdef GIA_NEURAL_NETWORK
"\n\t-oannxml [string]    : neural network definition .xml output filename (def: neuralNet.xml)"
"\n\t-oannldr [string]    : neural network display .ldr output filename (def: neuralNet.ldr)"
"\n\t-oannsvg [string]    : neural network display .svg output filename (def: neuralNet.svg)"
"\n\t-oannppm [string]    : neural network display .ppm opengl output filename (def: neuralNet.ppm)"
"\n\t-oannppm2 [string]   : neural network display .ppm raytraced output filename (def: neuralNetRaytraced.ppm)"
"\n\t-oannall [string]    : neural network display .xml/.svg/.ldr/.ppm default generic output filename (def: neuralNet)"
#endif
#ifdef GIA_PREPROCESSOR
"\n\t-lrp                               : language reduction preprocessor"
"\n\t-olrptxt [string]                  : plain text .txt output filename with GIA language reduction preprocessor applied (def: inputTextWithLRP.txt)"
"\n\t-olrptxtq [string]                 : query plain text .txt output filename with GIA language reduction preprocessor applied (def: inputTextWithLRPQuery.txt)"
"\n\t-lrpfolder [string]                : folder of LRP data files (list of multiword verbs, multiword prepositions etc) (def: same as exe)"
#endif
#ifdef USE_WORDNET
"\n\t-syndet                            : wordnet synonymn detection (0 - off, 1 - during queries only, 2 - during referencing and queries [def])"
#endif
"\n\t-show                : display output in opengl"
"\n\t-width [int]         : raster graphics width in pixels (def: 640)"
"\n\t-height [int]        : raster graphics height in pixels (def: 480)"
"\n\t-inputfolder [string]              : input directory name for input files (def: same as exe)"
"\n\t-outputfolder [string]             : output directory name for temporary and output files (def: same as exe)"
"\n"
"\n\n\t-version         : print version"
"\n\n\tThis program performs GIA (General Intelligence Algorithm) operations - creates semantic network based upon NLP dependencies file (.xml) or GIA semantic network file (.xml); outputs semantic network to GIA semantic network file (.xml); displays semantic network (using opengl); prints semantic network to raster image (.ppm), 3D vector graphics (.ldr), or 2D vector graphics (.svg).\n\n";

int main(const int argc, const char** argv)
{
	bool result = true;

	#ifdef GIA_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("GIA execution time: ", " (start)");
	#endif

	string currentFolder = SHAREDvarsClass().getCurrentDirectory();
	string inputFolderLocal = currentFolder;
	string outputFolderLocal = currentFolder;

	#ifdef GIA_NLP_CLIENT_SERVER
	bool NLPclient = false;
	#endif
	
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
	string inputTextPlainTXTfileName = string(GIA_TEXT_BASE_FILE_NAME) + GIA_TEXT_FILE_EXTENSION;			//"inputText.txt";

	bool useInputTextNLPrelationXMLFile = false;
	string inputTextNLPrelationXMLfileName = string(GIA_NLP_RELATION_BASE_FILE_NAME) + GIA_NLP_XML_FILE_EXTENSION;	//"inputNLPrelation.xml";
	bool useInputTextNLPfeatureXMLFile = false;
	string inputTextNLPfeatureXMLfileName = string(GIA_NLP_FEATURE_BASE_FILE_NAME) + GIA_NLP_XML_FILE_EXTENSION;	//"inputNLPfeature.xml";
	
	bool useOutputTextCFFFile = false;
	string outputTextCFFFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_CFF_FILE_EXTENSION;
	bool useInputTextXMLFile = false;
	string inputTextXMLFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_XML_FILE_EXTENSION;
	bool useOutputTextXMLFile = false;
	string outputTextXMLFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_XML_FILE_EXTENSION;
	bool useOutputTextCXLFile = false;
	string outputTextCXLFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_CXL_FILE_EXTENSION;
	bool useOutputTextLDRFile = false;
	string outputTextLDRFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_LDR_FILE_EXTENSION;
	bool useOutputTextPPMFile = false;
	string outputTextPPMFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_PPM_FILE_EXTENSION;
	bool useOutputTextSVGFile = false;
	string outputTextSVGFileName = string(GIA_SEMANTIC_NET_BASE_FILE_NAME) + GIA_SEMANTIC_NET_SVG_FILE_EXTENSION;
	bool useInputQueryPlainTXTFile = false;
	string inputQueryPlainTXTFileName = string(GIA_TEXT_BASE_FILE_NAME) + GIA_QUERY_FILE_PREPEND + GIA_TEXT_FILE_EXTENSION;				//"inputTextQuery.txt";
	
	bool useInputQueryNLPrelationXMLFile = false;
	string inputQueryNLPrelationXMLFileName = string(GIA_NLP_RELATION_BASE_FILE_NAME) + GIA_QUERY_FILE_PREPEND + GIA_NLP_XML_FILE_EXTENSION;	//"inputNLPrelationQuery.xml";
	bool useInputQueryNLPfeatureXMLFile = false;
	string inputQueryNLPfeatureXMLFileName = string(GIA_NLP_FEATURE_BASE_FILE_NAME) + GIA_QUERY_FILE_PREPEND + GIA_NLP_XML_FILE_EXTENSION;		//"inputNLPfeatureQuery.xml";
	
	bool useOutputQueryCFFFile = false;
	string outputQueryCFFFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_CFF_FILE_EXTENSION;
	bool useInputQueryXMLFile = false;
	string inputQueryXMLFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_XML_FILE_EXTENSION;
	bool useOutputQueryXMLFile = false;
	string outputQueryXMLFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_XML_FILE_EXTENSION;
	bool useOutputQueryCXLFile = false;
	string outputQueryCXLFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_CXL_FILE_EXTENSION;
	bool useOutputQueryLDRFile = false;
	string outputQueryLDRFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_LDR_FILE_EXTENSION;
	bool useOutputQueryPPMFile = false;
	string outputQueryPPMFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_PPM_FILE_EXTENSION;
	bool useOutputQuerySVGFile = false;
	string outputQuerySVGFileName = string(GIA_SEMANTIC_NET_BASE_QUERY_FILE_NAME) + GIA_SEMANTIC_NET_SVG_FILE_EXTENSION;
	bool useOutputTextAllFile = false;
	string outputTextAllFileName = GIA_SEMANTIC_NET_BASE_FILE_NAME;

	#ifdef GIA_NEURAL_NETWORK
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = false;
	string ANNoutputSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseOutputAllFile = false;
	string ANNoutputAllFileName = NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME;
	bool ANNdrawOutput = false;
	bool ANNuseSprites = true;
	#endif

	#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
	bool useOutputTextAnswerPlainTXTFile = false;
	string outputTextAnswerPlainTXTFileName = GIA_ANSWER_FILE_NAME;
	#endif	
	
	#ifdef GIA_INPUT_FILE_LISTS
	bool inputFileList = false;
	#endif
	bool printOutput = false;
	bool printOutputQuery = false;
	bool displayInOpenGLAndOutputScreenshot = false;

	int rasterImageWidth = 640;
	int rasterImageHeight = 480;

	bool useInputQuery = false;

	#ifdef GIA_DATABASE
	bool readFromDatabase = false;
	bool writeToDatabase = false;
	bool useDatabase = false;
	string KBdatabaseFolderName = GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_BASE + GIA_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR
	string semanticParserDatabaseFolderName = GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_BASE + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	string POStaggerDatabaseFolderName = GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_BASE + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	#endif
	
	#ifdef GIA_PREPROCESSOR
	bool useLRP = false;
	bool useOutputLRPTextPlainTXTFile = false;
	string outputLRPTextPlainTXTFileName = string(GIA_TEXT_BASE_FILE_NAME) + GIA_TEXT_FILE_WITH_LRP_PREPEND + GIA_TEXT_FILE_EXTENSION;
	bool useOutputQueryLRPTextPlainTXTFile = false;
	string outputQueryLRPTextPlainTXTFileName = string(GIA_TEXT_BASE_FILE_NAME) + GIA_TEXT_FILE_WITH_LRP_PREPEND + GIA_QUERY_FILE_PREPEND + GIA_TEXT_FILE_EXTENSION;
	string lrpDataFolderName = "";
	#endif

	#ifdef USE_WORDNET
	#ifdef GIA_SYNONYMN_DETECTION_DEFAULT_QUERIES_AND_ADVANCED_REFERENCING
	int synonymnDetectionStatus = SYNONYMN_DETECTION_STATUS_QUERIES_AND_ADVANCED_REFERENCING;
	#else
	int synonymnDetectionStatus = SYNONYMN_DETECTION_STATUS_QUERIES_ONLY;
	#endif
	#endif

	//bool train = false;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml inputText file is specified, just form network - do not train network

	#ifndef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	#ifdef GIA_DISABLE_SEMANTIC_TRANSLATOR
	if(!SHAREDvarsClass().argumentExists(argc, argv, "-itxt"))
	#else
	if(!(SHAREDvarsClass().argumentExists(argc, argv, "-itxt") || SHAREDvarsClass().argumentExists(argc, argv, "-ionlprel") || SHAREDvarsClass().argumentExists(argc, argv, "-ixml")))
	#endif
	{
		cerr << "error: GIA requires either a plain text inputText file (.txt), an NPL parsed inputText file (.xml) or GIA semantic network (.xml) to be defined" << endl;
		printf(errmessage);
		exit(EXIT_ERROR);
	}
	#endif
	
	if(SHAREDvarsClass().argumentExists(argc, argv, "-itxt"))
	{
		inputTextPlainTXTfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-itxt");
		useInputTextPlainTXTFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-itxtq"))
	{
		inputQueryPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-itxtq");
		useInputQueryPlainTXTFile = true;
		useInputQuery = true;
	}
	
#ifdef GIA_NEURAL_NETWORK_ACTIVE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-iannxml"))
	{
		ANNinputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-iannxml");
		//train = true;
		ANNuseInputXMLFile = true;
	}
#endif
#ifndef GIA_DISABLE_SEMANTIC_TRANSLATOR
#ifndef GIA_DISABLE_SYNTACTIC_TRANSLATOR
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ionlprel"))
	{
		inputTextNLPrelationXMLfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ionlprel");
		useInputTextNLPrelationXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ionlptag"))
	{
		inputTextNLPfeatureXMLfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ionlptag");
		useInputTextNLPfeatureXMLFile = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-ionlprelq"))
	{
		inputQueryNLPrelationXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ionlprelq");
		useInputQueryNLPrelationXMLFile = true;
		useInputQuery = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ionlptagq"))
	{
		inputQueryNLPfeatureXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ionlptagq");
		useInputQueryNLPfeatureXMLFile = true;
		useInputQuery = true;
	}
#endif
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ixml"))
	{
		inputTextXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ixml");
		//train = true;
		useInputTextXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ixmlq"))
	{
		inputQueryXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ixmlq");
		useInputQueryXMLFile = true;
		useInputQuery = true;
	}

	#ifdef GIA_INPUT_FILE_LISTS
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ilist"))
	{
		inputFileList = true;
	}
	#endif

	if(SHAREDvarsClass().argumentExists(argc, argv, "-ocff"))
	{
		outputTextCFFFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ocff");
		useOutputTextCFFFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oxml"))
	{
		outputTextXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oxml");
		useOutputTextXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ocxl"))
	{
		outputTextCXLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ocxl");
		useOutputTextCXLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oldr"))
	{
		outputTextLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oldr");
		useOutputTextLDRFile = true;
		printOutput = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oppm"))
	{
		outputTextPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oppm");
		useOutputTextPPMFile = true;
		printOutput = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-osvg"))
	{
		outputTextSVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-osvg");
		useOutputTextSVGFile = true;
		printOutput = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ocffq"))
	{
		outputQueryCFFFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ocffq");
		useOutputQueryCFFFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oxmlq"))
	{
		outputQueryXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oxmlq");
		useOutputQueryXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ocxlq"))
	{
		outputQueryCXLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ocxlq");
		useOutputQueryCXLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oldrq"))
	{
		outputQueryLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oldrq");
		useOutputQueryLDRFile = true;
		printOutputQuery = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oppmq"))
	{
		outputQueryPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oppmq");
		useOutputQueryPPMFile = true;
		printOutputQuery = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-osvgq"))
	{
		outputQuerySVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-osvgq");
		useOutputQuerySVGFile = true;
		printOutputQuery = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oall"))
	{
		outputTextAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oall");
		useOutputTextAllFile = true;
		printOutput = true;
	}

	#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oanswer"))
	{
		outputTextAnswerPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oanswer");
		useOutputTextAnswerPlainTXTFile = true;
	}
	#endif

	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlprelation"))
	{
		NLPdependencyRelationsParser = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlprelation"));
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpfeature"))
	{
		NLPfeatureParser = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlpfeature"));
	}
	else
	{
		NLPfeatureParser = NLPdependencyRelationsParser;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpcompmode"))
	{
		if(queryNLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
		{
			int nlpcompmode = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlpcompmode"));
			if(nlpcompmode == 1)
			{
				NLPrelexCompatibilityMode = true;
			}
			else if(nlpcompmode == 2)
			{
				NLPassumePreCollapsedStanfordRelations = true;
			}
		}
		else
		{
			cerr << "error: nlpcompmode set but (NLPdependencyRelationsParser != GIA_NLP_PARSER_RELEX)" << endl;
			exit(EXIT_ERROR);
		}
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlprelationq"))
	{
		queryNLPdependencyRelationsParser = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlprelationq"));
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpfeatureq"))
	{
		queryNLPfeatureParser = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlpfeatureq"));
	}
	else
	{
		queryNLPfeatureParser = queryNLPdependencyRelationsParser;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpcompmodeq"))
	{
		if(queryNLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
		{
			int nlpcompmodeq = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-nlpcompmodeq"));
			if(nlpcompmodeq == 1)
			{
				queryNLPrelexCompatibilityMode = true;
			}
			else if(nlpcompmodeq == 2)
			{
				queryNLPassumePreCollapsedStanfordRelations = true;
			}
		}
		else
		{
			cerr << "error: nlpcompmodeq set but (queryNLPdependencyRelationsParser != GIA_NLP_PARSER_RELEX)" << endl;
			exit(EXIT_ERROR);
		}
	}

	#ifdef GIA_DATABASE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbread"))
	{
		readFromDatabase = true;
		useDatabase = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbwrite"))
	{
		writeToDatabase = true;
		useDatabase = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbfolder"))
	{
		KBdatabaseFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-dbfolder");
	}
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbsemanticparserfolder"))
	{
		semanticParserDatabaseFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-dbsemanticparserfolder");
	}
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbpostaggerfolder"))
	{
		POStaggerDatabaseFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-dbpostaggerfolder");
	}
	#endif

	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlprelexfolder"))
	{
		NLPexeFolderArray[GIA_NLP_PARSER_RELEX] = SHAREDvarsClass().getStringArgument(argc, argv, "-nlprelexfolder");
	}
	else
	{
		NLPexeFolderArray[GIA_NLP_PARSER_RELEX] = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpstanfordcorenlpfolder"))
	{
		NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP] = SHAREDvarsClass().getStringArgument(argc, argv, "-nlpstanfordcorenlpfolder");
	}
	else
	{
		NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP] = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpstanfordparserfolder"))
	{
		NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER] = SHAREDvarsClass().getStringArgument(argc, argv, "-nlpstanfordparserfolder");
	}
	else
	{
		NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER] = currentFolder;
	}
	#ifdef GIA_NLP_CLIENT_SERVER
	if(SHAREDvarsClass().argumentExists(argc, argv, "-nlpclient"))
	{
		NLPclient = true;
	}	
	#endif	
	
#endif


	#ifdef GIA_NEURAL_NETWORK
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannxml"))
	{
		ANNoutputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannxml");
		ANNuseOutputXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannldr"))
	{
		ANNoutputLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannldr");
		ANNuseOutputLDRFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannsvg"))
	{
		ANNoutputSVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannsvg");
		ANNuseOutputSVGFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm"))
	{
		ANNoutputPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm");
		ANNuseOutputPPMFile = true;
		ANNuseOutputLDRFile = true;	//required for OpenGL image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm2"))
	{
		ANNoutputPPMFileNameRaytraced = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm2");
		ANNuseOutputPPMFileRaytraced = true;
		ANNuseOutputLDRFile = true;	//required for raytrace image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannall"))
	{
		ANNoutputAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannall");
		ANNuseOutputAllFile = true;
	}
	#ifndef ANN_DISPLAY_DISABLE_SPRITES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-annsprites"))
	{
		ANNuseSprites = true;
	}
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrp"))
	{
		useLRP = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-olrptxt"))
	{
		outputLRPTextPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-olrptxt");
		useOutputLRPTextPlainTXTFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-olrptxtq"))
	{
		outputQueryLRPTextPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-olrptxtq");
		useOutputQueryLRPTextPlainTXTFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrpfolder"))
	{
		lrpDataFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-lrpfolder");
	}
	else
	{
		lrpDataFolderName = currentFolder;
	}
	#endif
	
	#ifdef USE_WORDNET
	if(SHAREDvarsClass().argumentExists(argc, argv, "-syndet"))
	{
		synonymnDetectionStatus = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-syndet"));
	}
	#endif
	
	if(SHAREDvarsClass().argumentExists(argc, argv, "-show"))
	{
		displayInOpenGLAndOutputScreenshot = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-width"))
	{
		rasterImageWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-width");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-height"))
	{
		rasterImageHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-height");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-inputfolder"))
	{
		inputFolderLocal = SHAREDvarsClass().getStringArgument(argc, argv, "-inputfolder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-outputfolder"))
	{
		outputFolderLocal = SHAREDvarsClass().getStringArgument(argc, argv, "-outputfolder");
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
	{
		cout << "GIA.exe - Project Version: 3k1a 05-March-2020" << endl;
		exit(EXIT_OK);
	}


	GIAtranslatorVariablesClass* translatorVariables = new GIAtranslatorVariablesClass();
	translatorVariables->isQuery = false;
	translatorVariables->entityNodesActiveListComplete = new vector<GIAentityNode*>;
	translatorVariables->entityNodesActiveListNetworkIndexes = new unordered_map<string, GIAentityNode*>;
	translatorVariables->timeConditionNodesActiveList = new unordered_map<int64_t, GIAtimeConditionNode*>;
	translatorVariables->entityNodesActiveListSentences = new map<int, vector<GIAentityNode*>*>;
	translatorVariables->NLPfeatureParser = NLPfeatureParser;
	translatorVariables->NLPdependencyRelationsParser = NLPdependencyRelationsParser;
	translatorVariables->NLPrelexCompatibilityMode = NLPrelexCompatibilityMode;
	translatorVariables->NLPassumePreCollapsedStanfordRelations = NLPassumePreCollapsedStanfordRelations;
	#ifdef GIA_NEURAL_NETWORK
	translatorVariables->firstInputNeuronInNetwork = new ANNneuron();
	#endif
	#ifdef GIA_NLP_CLIENT_SERVER
	translatorVariables->NLPclient = NLPclient;
	#endif
	SHAREDvarsClass().copyStringArray(NLPexeFolderArray, translatorVariables->NLPexeFolderArray, GIA_NLP_PARSER_NUMBER_OF_TYPES);
	

	GIAtranslatorVariablesClass* translatorVariablesQuery = new GIAtranslatorVariablesClass();
	translatorVariablesQuery->isQuery = true;
	translatorVariablesQuery->entityNodesActiveListComplete = new vector<GIAentityNode*>;
	translatorVariablesQuery->entityNodesActiveListNetworkIndexes = new unordered_map<string, GIAentityNode*>;
	translatorVariablesQuery->timeConditionNodesActiveList = new unordered_map<int64_t, GIAtimeConditionNode*>;
	translatorVariablesQuery->entityNodesActiveListSentences = new map<int, vector<GIAentityNode*>*>;
	translatorVariablesQuery->NLPfeatureParser = queryNLPfeatureParser;
	translatorVariablesQuery->NLPdependencyRelationsParser = queryNLPdependencyRelationsParser;
	translatorVariablesQuery->NLPrelexCompatibilityMode = queryNLPrelexCompatibilityMode;
	translatorVariablesQuery->NLPassumePreCollapsedStanfordRelations = queryNLPassumePreCollapsedStanfordRelations;
	#ifdef GIA_NLP_CLIENT_SERVER
	translatorVariablesQuery->NLPclient = NLPclient;
	#endif
	SHAREDvarsClass().copyStringArray(NLPexeFolderArray, translatorVariablesQuery->NLPexeFolderArray, GIA_NLP_PARSER_NUMBER_OF_TYPES);

	
	GIAmainClass().executeGIA(

		translatorVariables,
		translatorVariablesQuery,

		inputFolderLocal,
		outputFolderLocal,

		useInputTextPlainTXTFile,
		inputTextPlainTXTfileName,
		useInputTextNLPrelationXMLFile,
		inputTextNLPrelationXMLfileName,
		useInputTextNLPfeatureXMLFile,
		inputTextNLPfeatureXMLfileName,
		useInputTextXMLFile,
		inputTextXMLFileName,
				
		useOutputTextCFFFile,
		outputTextCFFFileName,
		useOutputTextXMLFile,
		outputTextXMLFileName,
		useOutputTextCXLFile,
		outputTextCXLFileName,
		useOutputTextLDRFile,
		outputTextLDRFileName,
		useOutputTextPPMFile,
		outputTextPPMFileName,
		useOutputTextSVGFile,
		outputTextSVGFileName,
		useInputQueryPlainTXTFile,
		inputQueryPlainTXTFileName,
		useInputQueryNLPrelationXMLFile,
		inputQueryNLPrelationXMLFileName,
		useInputQueryNLPfeatureXMLFile,
		inputQueryNLPfeatureXMLFileName,
		useOutputQueryCFFFile,
		outputQueryCFFFileName,
		useInputQueryXMLFile,
		inputQueryXMLFileName,
		useOutputQueryXMLFile,
		outputQueryXMLFileName,
		useOutputQueryCXLFile,
		outputQueryCXLFileName,
		useOutputQueryLDRFile,
		outputQueryLDRFileName,
		useOutputQueryPPMFile,
		outputQueryPPMFileName,
		useOutputQuerySVGFile,
		outputQuerySVGFileName,
		useOutputTextAllFile,
		outputTextAllFileName,

		#ifdef GIA_NEURAL_NETWORK
		ANNuseInputXMLFile,
		ANNinputXMLFileName,	
		ANNuseOutputXMLFile,
		ANNoutputXMLFileName,
		ANNuseOutputLDRFile,
		ANNoutputLDRFileName,
		ANNuseOutputSVGFile,
		ANNoutputSVGFileName,
		ANNuseOutputPPMFile,
		ANNoutputPPMFileName,
		ANNuseOutputPPMFileRaytraced,
		ANNoutputPPMFileNameRaytraced,			
		ANNuseOutputAllFile,
		ANNoutputAllFileName,
		ANNuseSprites,
		#endif

		#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
		useOutputTextAnswerPlainTXTFile,
		outputTextAnswerPlainTXTFileName,
		#endif

		#ifdef GIA_INPUT_FILE_LISTS
		inputFileList,
		#endif
		printOutput,
		printOutputQuery,
		displayInOpenGLAndOutputScreenshot,

		rasterImageWidth,
		rasterImageHeight,

		useInputQuery,

		#ifdef GIA_DATABASE
		readFromDatabase,
		writeToDatabase,
		useDatabase,
		KBdatabaseFolderName,
		#endif
		#ifdef GIA_SEM_REL_TRANSLATOR
		semanticParserDatabaseFolderName,
		#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
		POStaggerDatabaseFolderName,
		#endif

		#ifdef GIA_PREPROCESSOR
		useLRP,
		useOutputLRPTextPlainTXTFile,
		outputLRPTextPlainTXTFileName,
		useOutputQueryLRPTextPlainTXTFile,
		outputQueryLRPTextPlainTXTFileName,
		lrpDataFolderName,
		#endif

		#ifdef USE_WORDNET
		synonymnDetectionStatus
		#endif
	);

	#ifdef GIA_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("GIA execution time: ", " (finish)");
	#endif
}
#endif


#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
static char errmessage[] = "Usage:  GIAposTaggerGenerateDatabase.exe [options]\n\n\twhere options are any of the following\n"
"\n\t-dbpostaggerfolder [string]   : pos tagger database base folder path (def: /home/systemusername/source/GIAPOStaggerDatabase)"
"\n\t-lrp                          : language reduction preprocessor"
"\n\t-lrpfolder [string]           : folder of LRP data files (list of multiword verbs, multiword prepositions etc) (def: same as exe)"
"\n\t-wikiDumpFolder               : wiki dump folder (def: /home/systemusername/soft/wiki/output)"
"\n\t-wikiDumpFileBatchIndex       : batch index"
#ifdef GIA_NEURAL_NETWORK
"\n\t-oannxml [string]    : neural network definition .xml output filename (def: neuralNet.xml)"
"\n\t-oannldr [string]    : neural network display .ldr output filename (def: neuralNet.ldr)"
"\n\t-oannsvg [string]    : neural network display .svg output filename (def: neuralNet.svg)"
"\n\t-oannppm [string]    : neural network display .ppm opengl output filename (def: neuralNet.ppm)"
"\n\t-oannppm2 [string]   : neural network display .ppm raytraced output filename (def: neuralNetRaytraced.ppm)"
"\n\t-oannall [string]    : neural network display .xml/.svg/.ldr/.ppm default generic output filename (def: neuralNet)"
#endif
"\n\n\tThis program generates the POS tagger database for a wiki dump batch.\n\n";

int main(const int argc, const char** argv)
{
	bool result = true;
	
	string currentFolder = SHAREDvarsClass().getCurrentDirectory();
	string inputFolderLocal = currentFolder;
	string outputFolderLocal = currentFolder;

	bool useLRP = false;
	string lrpDataFolderName = "";
	string POStaggerDatabaseFolderName = GIA_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME_BASE + GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	string wikiDumpFolderName = GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_DOC_XML_OUTPUT_FOLDER;
	int wikiDumpFileBatchIndex = 0;
	
	#ifdef GIA_NEURAL_NETWORK
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = false;
	string ANNoutputSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseOutputAllFile = false;
	string ANNoutputAllFileName = NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME;
	bool ANNuseSprites = true;
	#endif
	
	int rasterImageWidth = 640;
	int rasterImageHeight = 480;
			
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrpfolder"))
	{
		lrpDataFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-lrpfolder");
	}
	else
	{
		lrpDataFolderName = currentFolder;
	}	
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrp"))
	{
		useLRP = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbpostaggerfolder"))
	{
		POStaggerDatabaseFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-dbpostaggerfolder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-wikiDumpFolder"))
	{
		wikiDumpFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-wikiDumpFolder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-wikiDumpFileBatchIndex"))
	{
		wikiDumpFileBatchIndex = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-wikiDumpFileBatchIndex"));
	}
	
	#ifdef GIA_NEURAL_NETWORK
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannxml"))
	{
		ANNoutputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannxml");
		ANNuseOutputXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannldr"))
	{
		ANNoutputLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannldr");
		ANNuseOutputLDRFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannsvg"))
	{
		ANNoutputSVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannsvg");
		ANNuseOutputSVGFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm"))
	{
		ANNoutputPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm");
		ANNuseOutputPPMFile = true;
		ANNuseOutputLDRFile = true;	//required for OpenGL image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm2"))
	{
		ANNoutputPPMFileNameRaytraced = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm2");
		ANNuseOutputPPMFileRaytraced = true;
		ANNuseOutputLDRFile = true;	//required for raytrace image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannall"))
	{
		ANNoutputAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannall");
		ANNuseOutputAllFile = true;
	}
	#ifndef ANN_DISPLAY_DISABLE_SPRITES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-annsprites"))
	{
		ANNuseSprites = true;
	}
	#endif
	#endif
	

	#ifdef GIA_NEURAL_NETWORK
	bool ANNdisplayInOpenGL = false;
	bool ANNdrawOutput = false;
	if(ANNuseOutputPPMFile)
	{
		ANNdisplayInOpenGL = true;
	}
	if(ANNuseOutputLDRFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputSVGFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFileRaytraced)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputAllFile)
	{
		ANNdrawOutput = true;
	}	
	if(ANNdrawOutput)
	{
		if(ANNuseOutputAllFile)
		{
			if(!ANNuseOutputLDRFile)
			{
				ANNuseOutputLDRFile = true;
				ANNoutputLDRFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
			}

			if(!ANNuseOutputXMLFile)
			{
				ANNuseOutputXMLFile = true;
				ANNoutputXMLFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
			}

			if(!ANNuseOutputSVGFile)
			{
				ANNuseOutputSVGFile = true;	//SVG output is not always required when printing/drawing neural network
				ANNoutputSVGFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
			}
			if(!ANNuseOutputPPMFile)
			{
				ANNuseOutputPPMFile = true;
				ANNoutputPPMFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
			}
			/* disable raytrace output by default
			if(!ANNuseOutputPPMFileRaytraced)
			{
				ANNuseOutputPPMFileRaytraced = true;
				ANNoutputPPMFileNameRaytraced = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
			}
			*/
		}
	}
	#endif
		
	POStaggerDatabaseFolderName = POStaggerDatabaseFolderName + CHAR_FOLDER_DELIMITER;
	lrpDataFolderName = lrpDataFolderName + CHAR_FOLDER_DELIMITER;	
	wikiDumpFolderName = wikiDumpFolderName + CHAR_FOLDER_DELIMITER;	
	
	inputFolder = inputFolderLocal;
	outputFolder = outputFolderLocal;
	SHAREDvarsClass().setCurrentDirectory(inputFolder);

	cout << "executing GIAposTaggerGenerateDatabase.exe with the following parameters;" << endl;
	cout << "lrpDataFolderName = " << lrpDataFolderName << endl;
	cout << "useLRP = " << useLRP << endl;
	cout << "POStaggerDatabaseFolderName = " << POStaggerDatabaseFolderName << endl;
	cout << "wikiDumpFolderName = " << wikiDumpFolderName << endl;
	cout << "wikiDumpFileBatchIndex = " << wikiDumpFileBatchIndex << endl;

	GIAtranslatorVariablesClass* translatorVariables = new GIAtranslatorVariablesClass();

	GIApreprocessorPOStaggerDatabaseClass().initialisePOStaggerDatabase(POStaggerDatabaseFolderName);
	
	#ifdef GIA_PREPROCESSOR
	if(!GIApreprocessorWordIdentificationClass().initialiseLRP(lrpDataFolderName, useLRP))
	{
		result = false;
	}
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR		
	vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes = new vector<GIAtxtRelTranslatorRulesGroupType*>;
	vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers = new vector<XMLparserTag*>;
	if(!GIAtxtRelTranslatorRulesClass().extractGIAtxtRelTranslatorRules(GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesTokenLayers))
	{
		result = false;
	}
	if(!GIAtxtRelTranslatorSANIFormationClass().createGIAtxtRelTranslatorSANI(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	#endif	
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY
	if(!GIApreprocessorPOStaggerClass().generatePOStaggerDatabaseFromWikiDumpText(wikiDumpFolderName, wikiDumpFileBatchIndex, useLRP))
	#else
	if(!GIApreprocessorPOStaggerClass().generatePOStaggerDatabaseFromWikiDumpText(wikiDumpFolderName, useLRP))	
	#endif
	{
		result = false;
	}	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	GIAtxtRelTranslatorSANIFormationClass().createANNconnectivity(GIAtxtRelTranslatorRulesGroupTypes);
	#endif
	#endif

	#ifdef GIA_NEURAL_NETWORK
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ANN
	GIAneuralNetworkOperationsClass().generateNeuralNetFromGIAtxtRelTranslatorNet(translatorVariables);	//generate GIA NLP neural network
	#endif
	#ifdef GIA_NEURAL_NETWORK_PASSIVE
	GIAneuralNetworkOperationsClass().generateNeuralNetFromSemanticNet(translatorVariables);	//generate GIA KB neural network
	#endif
	
	if(ANNdrawOutput)
	{
		string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
		ANNdisplayClass().outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight);
	}
	if(ANNuseOutputXMLFile)
	{
		if(!GIAneuralNetworkOperationsClass().writeNeuralNetXMLfile(ANNoutputXMLFileName, translatorVariables->firstInputNeuronInNetwork))
		{
			result = false;
		}
	}		
	#endif
}
#endif


bool GIAmainClass::executeGIA(

	GIAtranslatorVariablesClass* translatorVariables,
	GIAtranslatorVariablesClass* translatorVariablesQuery,

	string inputFolderLocal, 
	string outputFolderLocal,

	bool useInputTextPlainTXTFile,
	string inputTextPlainTXTfileName,
	bool useInputTextNLPrelationXMLFile,
	string inputTextNLPrelationXMLfileName,
	bool useInputTextNLPfeatureXMLFile,
	string inputTextNLPfeatureXMLfileName,
	bool useInputTextXMLFile,
	string inputTextXMLFileName,	
	
	bool useOutputTextCFFFile,
	string outputTextCFFFileName,
	bool useOutputTextXMLFile,
	string outputTextXMLFileName,
	bool useOutputTextCXLFile,
	string outputTextCXLFileName,
	bool useOutputTextLDRFile,
	string outputTextLDRFileName,
	bool useOutputTextPPMFile,
	string outputTextPPMFileName,
	bool useOutputTextSVGFile,
	string outputTextSVGFileName,
	bool useInputQueryPlainTXTFile,
	string inputQueryPlainTXTFileName,
	bool useInputQueryNLPrelationXMLFile,
	string inputQueryNLPrelationXMLFileName,
	bool useInputQueryNLPfeatureXMLFile,
	string inputQueryNLPfeatureXMLFileName,
	bool useOutputQueryCFFFile,
	string outputQueryCFFFileName,
	bool useInputQueryXMLFile,
	string inputQueryXMLFileName,
	bool useOutputQueryXMLFile,
	string outputQueryXMLFileName,
	bool useOutputQueryCXLFile,
	string outputQueryCXLFileName,
	bool useOutputQueryLDRFile,
	string outputQueryLDRFileName,
	bool useOutputQueryPPMFile,
	string outputQueryPPMFileName,
	bool useOutputQuerySVGFile,
	string outputQuerySVGFileName,
	bool useOutputTextAllFile,
	string outputTextAllFileName,
	
	#ifdef GIA_NEURAL_NETWORK
	bool ANNuseInputXMLFile,
	string ANNinputXMLFileName,
	bool ANNuseOutputXMLFile,
	string ANNoutputXMLFileName,
	bool ANNuseOutputLDRFile,
	string ANNoutputLDRFileName,
	bool ANNuseOutputSVGFile,
	string ANNoutputSVGFileName,
	bool ANNuseOutputPPMFile,
	string ANNoutputPPMFileName,
	bool ANNuseOutputPPMFileRaytraced,
	string ANNoutputPPMFileNameRaytraced,
	bool ANNuseOutputAllFile,
	string ANNoutputAllFileName,
	bool ANNuseSprites,
	#endif
		
	#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
	bool useOutputTextAnswerPlainTXTFile,
	string outputTextAnswerPlainTXTFileName,
	#endif

	#ifdef GIA_INPUT_FILE_LISTS
	bool inputFileList,
	#endif
	bool printOutput,
	bool printOutputQuery,
	bool displayInOpenGLAndOutputScreenshot,

	int rasterImageWidth,
	int rasterImageHeight,

	bool useInputQuery,

	#ifdef GIA_DATABASE
	bool readFromDatabase,
	bool writeToDatabase,
	bool useDatabase,
	string KBdatabaseFolderName,
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR
	string semanticParserDatabaseFolderName,
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	string POStaggerDatabaseFolderName,
	#endif
		
	#ifdef GIA_PREPROCESSOR
	bool useLRP,
	bool useOutputLRPTextPlainTXTFile,
	string outputLRPTextPlainTXTFileName,
	bool useOutputQueryLRPTextPlainTXTFile,
	string outputQueryLRPTextPlainTXTFileName,
	string lrpDataFolderName,
	#endif

	#ifdef USE_WORDNET
	int synonymnDetectionStatus
	#endif
	)
{
#ifdef USE_CS_WORKAROUND
}
bool GIAmainClass::executeGIA2()
{
#endif
	bool result = true;

	bool displayInOpenGL = false;
	if(displayInOpenGLAndOutputScreenshot)
	{
		displayInOpenGL = true;
	}
	#ifdef GIA_NEURAL_NETWORK
	bool ANNdisplayInOpenGL = false;
	bool ANNdrawOutput = false;
	if(ANNuseOutputPPMFile)
	{
		ANNdisplayInOpenGL = true;
		displayInOpenGL = true;
	}
	if(ANNuseOutputLDRFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputSVGFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFileRaytraced)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputAllFile)
	{
		ANNdrawOutput = true;
	}	
	if(ANNdrawOutput)
	{
		if(ANNuseOutputAllFile)
		{
			if(!ANNuseOutputLDRFile)
			{
				ANNuseOutputLDRFile = true;
				ANNoutputLDRFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
			}

			if(!ANNuseOutputXMLFile)
			{
				ANNuseOutputXMLFile = true;
				ANNoutputXMLFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
			}

			if(!ANNuseOutputSVGFile)
			{
				ANNuseOutputSVGFile = true;	//SVG output is not always required when printing/drawing neural network
				ANNoutputSVGFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
			}
			if(!ANNuseOutputPPMFile)
			{
				ANNuseOutputPPMFile = true;
				ANNoutputPPMFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
			}
			/* disable raytrace output by default
			if(!ANNuseOutputPPMFileRaytraced)
			{
				ANNuseOutputPPMFileRaytraced = true;
				ANNoutputPPMFileNameRaytraced = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
			}
			*/
		}
	}
	#endif
	
	#ifdef GIA_DATABASE
	KBdatabaseFolderName = KBdatabaseFolderName + CHAR_FOLDER_DELIMITER;
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR
	semanticParserDatabaseFolderName = semanticParserDatabaseFolderName + CHAR_FOLDER_DELIMITER;
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	POStaggerDatabaseFolderName = POStaggerDatabaseFolderName + CHAR_FOLDER_DELIMITER;
	#endif	
	#ifdef GIA_PREPROCESSOR
	lrpDataFolderName = lrpDataFolderName + CHAR_FOLDER_DELIMITER;	
	#endif
	
	inputFolder = inputFolderLocal;
	outputFolder = outputFolderLocal;
	SHAREDvarsClass().setCurrentDirectory(inputFolder);
	
	if(!XMLrulesClass.parseGIArulesXMLfile())
	{
		cerr << "error: GIArules.xml file not detected" << endl;
		cout << "currentDirectory = " << SHAREDvarsClass().getCurrentDirectory() << endl;
		exit(EXIT_ERROR);
	}
	LDsprite.fillInLDspriteExternVariables();
	///GIA specific rules.xml file is not used at the moment	[once right variables have been decided upon they will be fed to xml]
	//fillInGIARulesExternVariables();
	
	#ifdef GIA_NEURAL_NETWORK
	if(!XMLrulesClassClass().parseANNrulesXMLfile())
	{
		result = false;
	}
	ANNdraw.fillInANNSpriteExternVariables();
	#endif

	#ifdef GIA_DATABASE
	GIAdatabase.initialiseDatabase(readFromDatabase, KBdatabaseFolderName, useDatabase, translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes);
	SHAREDvars.setCurrentDirectory(inputFolder);
	#endif

	#ifdef GIA_SEM_REL_TRANSLATOR
	GIAsemRelTranslatorDatabase.initialiseSemanticParserDatabase(semanticParserDatabaseFolderName);
	#endif
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	GIApreprocessorPOStaggerDatabase.initialisePOStaggerDatabase(POStaggerDatabaseFolderName);
	#endif
	
	#ifdef USE_WORDNET
	GIAwordnet.initialiseWordNet(synonymnDetectionStatus);
	#endif

	#ifdef GIA_PREPROCESSOR
	if(!GIApreprocessorWordIdentification.initialiseLRP(lrpDataFolderName, useLRP))
	{
		result = false;
	}
	#endif
	
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


		if(displayInOpenGL)
		{
			LDopengl.initiateOpenGL(rasterImageWidth, rasterImageHeight, 0, 0, false);
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
	if(useLRP)
	{
		if(!useOutputLRPTextPlainTXTFile)
		{
			useOutputLRPTextPlainTXTFile = true;
			outputLRPTextPlainTXTFileName = outputTextAllFileName + "afterLRP.txt";
		}
		if(useInputQuery)
		{
			if(!useOutputQueryLRPTextPlainTXTFile)
			{
				useOutputQueryLRPTextPlainTXTFile = true;
				outputQueryLRPTextPlainTXTFileName = outputTextAllFileName + "afterLRPQuery.txt";
			}
		}
	}





	#ifdef GIA_INPUT_FILE_LISTS
	int numberOfInputFilesInList = 1;
	string* inputTextPlainTXTFileNameArray = new string[GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST];
	string* inputTextNLPrelationXMLFileNameArray = new string[GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST];
	string* inputTextNLPfeatureXMLFileNameArray = new string[GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST];
	string* inputTextXMLFileNameArray = new string[GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST];
	string outputTextCFFFileNameBase = outputTextCFFFileName;
	string outputLRPTextPlainTXTFileNameBase = outputLRPTextPlainTXTFileName;
	if(inputFileList)
	{
		if(useInputTextPlainTXTFile)
		{
			if(!SHAREDvars.getLinesFromFile(inputTextPlainTXTfileName, inputTextPlainTXTFileNameArray, &numberOfInputFilesInList))
			{
				cout << "main{} error: !getLinesFromFile: " << inputTextPlainTXTfileName << endl;
			}
		}
		if(useInputTextNLPrelationXMLFile)
		{
			if(!SHAREDvars.getLinesFromFile(inputTextNLPrelationXMLfileName, inputTextNLPrelationXMLFileNameArray, &numberOfInputFilesInList))
			{
				cout << "main{} error: !getLinesFromFile: " << inputTextNLPrelationXMLfileName << endl;
			}
		}
		if(useInputTextNLPfeatureXMLFile)
		{
			if(!SHAREDvars.getLinesFromFile(inputTextNLPfeatureXMLfileName, inputTextNLPfeatureXMLFileNameArray, &numberOfInputFilesInList))
			{
				cout << "main{} error: !getLinesFromFile: " << inputTextNLPfeatureXMLfileName << endl;
			}
		}

		if(useInputTextXMLFile)
		{
			if(!SHAREDvars.getLinesFromFile(inputTextXMLFileName, inputTextXMLFileNameArray, &numberOfInputFilesInList))
			{
				cout << "main{} error: !getLinesFromFile: " << inputTextXMLFileName << endl;
			}
		}
	}

	for(int inputFileNameIndex=0; inputFileNameIndex<numberOfInputFilesInList; inputFileNameIndex++)	//CHECKTHIS: change back to 0
	{
		SHAREDvars.setCurrentDirectory(inputFolder);

		if(inputFileList)
		{
			if(useInputTextPlainTXTFile)
			{
				inputTextPlainTXTfileName = inputTextPlainTXTFileNameArray[inputFileNameIndex];
			}
			if(useInputTextNLPrelationXMLFile)
			{
				inputTextNLPrelationXMLfileName = inputTextNLPrelationXMLFileNameArray[inputFileNameIndex];
			}
			if(useInputTextNLPfeatureXMLFile)
			{
				inputTextNLPfeatureXMLfileName = inputTextNLPfeatureXMLFileNameArray[inputFileNameIndex];
			}

			if(useInputTextXMLFile)
			{
				inputTextXMLFileName = inputTextXMLFileNameArray[inputFileNameIndex];
			}

			string inputFileNameIndexString = SHAREDvars.convertIntToString(inputFileNameIndex);
			outputTextCFFFileName = outputTextCFFFileNameBase + "." + inputFileNameIndexString;
			outputLRPTextPlainTXTFileName = outputLRPTextPlainTXTFileNameBase + "." + inputFileNameIndexString;
		}
	#endif
		
		#ifdef GIA_PREPROCESSOR
		if(!(!useInputTextPlainTXTFile && useInputTextXMLFile))
		{
			if(!GIApreprocessor.preprocessTextForGIAwrapper(useLRP, &inputTextPlainTXTfileName, outputLRPTextPlainTXTFileName, false, translatorVariables, &useInputTextPlainTXTFile, inputTextNLPfeatureXMLfileName))
			{
				result = false;
			}
		}
		#endif

	#ifdef GIA_NEURAL_NETWORK_ACTIVE
		if(ANNuseInputXMLFile)
		{
			if(!GIAneuralNetworkNonSemanticOperations.readNeuralNetXMLfile(ANNinputXMLFileName, translatorVariables->firstInputNeuronInNetwork))
			{
				result = false;
			}
		}
	#endif
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE
		if(useInputTextPlainTXTFile)
		{
			//add text to network
			if(!GIAneuralNetworkNonSemantic.addTextToNetwork(translatorVariables))
			{
				result = false;
			}
		}
		else
		{
			cout << "GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE: GIAmain error{}: !useInputTextPlainTXTFile" << endl;
			exit(EXIT_ERROR);
		}
	#endif
	#ifndef GIA_DISABLE_SEMANTIC_TRANSLATOR
	#ifdef GIA_DISABLE_SYNTACTIC_TRANSLATOR
		if(useInputTextPlainTXTFile)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
			#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
			translatorVariables->firstSentenceInList = new GIAsentence();
			if(!GIAtxtRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariables, inputTextPlainTXTfileName, "", "", outputTextCFFFileName))
			{
				result = false;
			}
			delete translatorVariables->firstSentenceInList;
			#endif	
			#endif
		}
	#else
		
		if(inputFileList)
		{
			cout << "inputTextPlainTXTfileName = " << inputTextPlainTXTfileName << endl;
			cout << "inputTextNLPrelationXMLfileName = " << inputTextNLPrelationXMLfileName << endl;
			cout << "inputTextNLPfeatureXMLfileName = " << inputTextNLPfeatureXMLfileName << endl;
		}
		
		if(useInputTextPlainTXTFile)
		{
			/*
			if(useInputTextNLPrelationXMLFile)
			{
				cerr << "error: useInputTextPlainTXTFile && useInputTextNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			*/
			if(useInputTextXMLFile)
			{
				cerr << "error: useInputTextPlainTXTFile && useInputTextXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				#ifndef GIA_SEM_REL_TRANSLATOR_DO_NOT_PARSE_DEPENDENCY_RELATION_FILE
				GIAnlp.executeNLPparser(inputTextPlainTXTfileName, inputTextNLPrelationXMLfileName, translatorVariables, true);
				if(inputTextNLPfeatureXMLfileName != inputTextNLPrelationXMLfileName)
				{
				#endif
					GIAnlp.executeNLPparser(inputTextPlainTXTfileName, inputTextNLPfeatureXMLfileName, translatorVariables, false);
				#ifndef GIA_SEM_REL_TRANSLATOR_DO_NOT_PARSE_DEPENDENCY_RELATION_FILE
				}
				#endif
				useInputTextNLPrelationXMLFile = true;	//now will parse the NLP Parsed file

				SHAREDvars.setCurrentDirectory(outputFolder);
			}
		}
		
		if(useInputTextNLPrelationXMLFile)
		{
			if(useInputTextXMLFile)
			{
				cerr << "error: useInputTextNLPrelationXMLFile && useInputTextXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				translatorVariables->firstSentenceInList = new GIAsentence();
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
				if(!GIAtxtRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariables, inputTextPlainTXTfileName, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputTextCFFFileName))
				{
					result = false;
				}
				#else
				#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
				if(!GIAsemRelTranslator.performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariables, inputTextPlainTXTfileName, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputTextCFFFileName))
				{
					result = false;
				}
				#else
				if(!GIAtranslator.parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(translatorVariables, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputTextCFFFileName))
				{
					result = false;
				}
				#endif
				#endif
				delete translatorVariables->firstSentenceInList;
			}
		}
	#endif

		if(useInputTextXMLFile)
		{
			if(useInputTextPlainTXTFile)
			{
				cerr << "error: useInputTextXMLFile && useInputTextPlainTXTFile" << endl;
				exit(EXIT_ERROR);
			}
			if(useInputTextNLPrelationXMLFile)
			{
				cerr << "error: useInputTextXMLFile && useInputTextNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				if(!GIAxmlConversion.readSemanticNetXMLfileOptimised(inputTextXMLFileName, translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes, translatorVariables->entityNodesActiveListSentences))
				{
					result = false;
				}
				GIAtranslatorOperations.recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(translatorVariables->entityNodesActiveListNetworkIndexes);	//prepare for use in GIA

			}
		}	
	#endif

		#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
		if(useLRP)
		{
			GIApreprocessorWordReduction.deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(false);	//required for local variable access
		}
		#endif
#ifdef GIA_INPUT_FILE_LISTS
	}
#endif

	SHAREDvars.setCurrentDirectory(inputFolder);

	if(useInputQuery)
	{
		#ifdef GIA_PREPROCESSOR
		if(!(!useInputQueryPlainTXTFile && useInputQueryXMLFile))
		{
			if(!GIApreprocessor.preprocessTextForGIAwrapper(useLRP, &inputQueryPlainTXTFileName, outputQueryLRPTextPlainTXTFileName, true, translatorVariablesQuery, &useInputQueryPlainTXTFile, inputQueryNLPfeatureXMLFileName))
			{
				result = false;
			}
		}
		#endif

	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE
		if(useInputQueryPlainTXTFile)
		{
			//perform query
			if(!GIAneuralNetworkNonSemantic.performQuery(translatorVariables, translatorVariablesQuery))
			{
				result = false;
			}
		}
		else
		{
			cout << "GIA_NEURAL_NETWORK_NON_SEMANTIC_ACTIVE: GIAmain error{}: !useInputQueryPlainTXTFile" << endl;
			exit(EXIT_ERROR);
		}
	#endif

	#ifndef GIA_DISABLE_SEMANTIC_TRANSLATOR
	#ifdef GIA_DISABLE_SYNTACTIC_TRANSLATOR
		if(useInputQueryPlainTXTFile)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
			#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
			translatorVariablesQuery->firstSentenceInList = new GIAsentence();
			if(!GIAtxtRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariablesQuery, inputQueryPlainTXTFileName, "", "", outputQueryCFFFileName))
			{
				result = false;
			}
			delete translatorVariablesQuery->firstSentenceInList;
			#endif	
			#endif
		}
	#else
		if(useInputQueryPlainTXTFile)
		{
			/*
			if(useInputQueryNLPrelationXMLFile)
			{
				cerr << "error: useInputQueryPlainTXTFile && useInputQueryNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			*/
			if(useInputQueryXMLFile)
			{
				cerr << "error: useInputQueryPlainTXTFile && useInputQueryXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				#ifndef GIA_SEM_REL_TRANSLATOR_DO_NOT_PARSE_DEPENDENCY_RELATION_FILE
				GIAnlp.executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPrelationXMLFileName, translatorVariablesQuery, true);
				if(inputQueryNLPfeatureXMLFileName != inputQueryNLPrelationXMLFileName)
				{
				#endif
					GIAnlp.executeNLPparser(inputQueryPlainTXTFileName, inputQueryNLPfeatureXMLFileName, translatorVariablesQuery, false);
				#ifndef GIA_SEM_REL_TRANSLATOR_DO_NOT_PARSE_DEPENDENCY_RELATION_FILE
				}
				#endif
				useInputQueryNLPrelationXMLFile = true;	//now will parse the NLP Parsed file

				SHAREDvars.setCurrentDirectory(outputFolder);
			}
		}

		if(useInputQueryNLPrelationXMLFile)
		{
			if(useInputQueryXMLFile)
			{
				cerr << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				translatorVariablesQuery->firstSentenceInList = new GIAsentence();
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
				if(!GIAtxtRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariablesQuery, inputQueryPlainTXTFileName, inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, outputQueryCFFFileName))
				{
					result = false;
				}
				#else
				#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
				if(!GIAsemRelTranslator.performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariablesQuery, inputQueryPlainTXTFileName, inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, outputQueryCFFFileName))
				{
					result = false;
				}
				#else
				if(!GIAtranslator.parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(translatorVariablesQuery, inputQueryNLPrelationXMLFileName, inputQueryNLPfeatureXMLFileName, outputQueryCFFFileName))
				{
					result = false;
				}
				#endif
				#endif
				delete translatorVariablesQuery->firstSentenceInList;
			}
		}
	#endif

		if(useInputQueryXMLFile)
		{
			if(useInputQueryPlainTXTFile)
			{
				cerr << "error: useInputQueryXMLFile && useInputQueryPlainTXTFile" << endl;
				exit(EXIT_ERROR);
			}
			if(useInputQueryNLPrelationXMLFile)
			{
				cerr << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				translatorVariablesQuery->entityNodesActiveListComplete = new vector<GIAentityNode*>;
				if(!GIAxmlConversion.readSemanticNetXMLfileOptimised(inputQueryXMLFileName, translatorVariablesQuery->entityNodesActiveListComplete, translatorVariablesQuery->entityNodesActiveListNetworkIndexes, translatorVariablesQuery->entityNodesActiveListSentences))
				{
					result = false;
				}
				GIAtranslatorOperations.recordNetworkIndexNodesAsNonPermanentIfTheyAreDisabled(translatorVariablesQuery->entityNodesActiveListNetworkIndexes);	//prepare for use in GIA

			}
		}
		else
		{
			if(useOutputQueryXMLFile)
			{
				if(!GIAxmlConversion.writeSemanticNetXMLFileOptimised(outputQueryXMLFileName, translatorVariablesQuery->entityNodesActiveListComplete, translatorVariablesQuery->entityNodesActiveListNetworkIndexes))
				{
					result = false;
				}
			}		
		}

		if(printOutputQuery)	//moved here in version 1i8a
		{
			GIAdraw.printGIAnetworkNodes(translatorVariablesQuery, rasterImageWidth, rasterImageHeight, outputQueryLDRFileName, outputQuerySVGFileName, outputQueryPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputQueryLDRFile, useOutputQueryPPMFile, useOutputQuerySVGFile);
		}
		if(useOutputQueryCXLFile)	//moved here in version 1i8a
		{
			if(!GIAcxlConversion.writeCmapToolsCXLFileOptimised(outputQueryCXLFileName, translatorVariablesQuery->entityNodesActiveListComplete, translatorVariablesQuery->entityNodesActiveListNetworkIndexes))
			{
				result = false;
			}
		}

		#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_ACTIVE
		//TODO
		#else
		/*
		implement comparison of question semantic net to semanic net - locate the question semantic net as a subset of the semantic net, and;
			1. highlight it
			2. return missing variables
			3. NB for which/what questions, make the software just locate the identical structure, and if necessary return the parent of the primary substance (eg the parent of the "object" of the question)
		*/

		bool foundComparisonVariable = GIAtranslatorOperations.getFoundComparisonVariable();
		GIAentityNode* comparisonVariableNode = GIAtranslatorOperations.getComparisonVariableNode();

		bool foundAnswer = false;
		double confidence = 0.0;

		GIAentityNode* queryAnswerNode = NULL;
		string queryAnswerContext = "";
		queryAnswerNode = GIAquery.answerQueryOrFindAndTagForHighlightingMatchingStructureInSemanticNetwork(translatorVariables, translatorVariablesQuery, foundComparisonVariable, comparisonVariableNode, &foundAnswer, queryAnswerNode, &confidence, &queryAnswerContext);

		double maxConfidence = GIAquery.determineMaxConfidenceOfQuerySemanticNetwork(translatorVariablesQuery->entityNodesActiveListNetworkIndexes);		//OLD [simple]: entityNodesActiveListCompleteQuery->size();

		string answerString = "";
		if(foundAnswer)
		{
			answerString = answerString + "\nAnswer found.";
			if(foundComparisonVariable)
			{
				#ifdef GIA_ALIASES
				if(comparisonVariableNode->isNameQuery)
				{
					for(vector<string>::iterator aliasIter = queryAnswerNode->aliasList.begin(); aliasIter != queryAnswerNode->aliasList.end(); aliasIter++)
					{
						answerString = answerString + "\nExact Found Answer (alias): " + *aliasIter;	//CHECKTHIS; this is not working yet?
					}
				}
				#endif
				answerString = answerString + "\nExact Answer found: " + queryAnswerNode->entityName;
				if(comparisonVariableNode->hasQuantity)
				{
					string tempQuantityNumberString = SHAREDvars.convertIntToString(queryAnswerNode->quantityNumber);
					answerString = answerString + "\nQuantity number: " + tempQuantityNumberString;
				}
			}
		}
		else
		{
			if(foundComparisonVariable)
			{
				answerString = answerString + "\nAnswer Not Found.";
			}
			else
			{
				if(confidence >= (maxConfidence-1-GIA_QUERY_DOUBLE_ERROR))
				{
					answerString = answerString + "\nAnswer: Yes.";
				}
				else
				{
					answerString = answerString + "\nAnswer: No.";
				}
			}
		}

		if(foundAnswer && !foundComparisonVariable)
		{
			answerString = answerString + "\nBest Inexact Answer Found: " + queryAnswerNode->entityName;
		}

		if(foundAnswer)
		{
			string printEntityNodeQualitiesString = "";
			GIAquery.printEntityNodeQualitiesOnly(queryAnswerNode, &printEntityNodeQualitiesString);
			answerString = answerString + printEntityNodeQualitiesString;

			string printEntityNodeTimeConditionString = "";
			GIAquery.printEntityTimeConditionNodeOnly(queryAnswerNode, &printEntityNodeTimeConditionString);
			cout << printEntityNodeTimeConditionString;
			answerString = answerString + printEntityNodeTimeConditionString;


			//print AnswerPreviousNode relationship with answerNode
			
			answerString = answerString + "\nAnswer Context: " + queryAnswerContext;

			#ifdef GIA_NLG
			answerString = answerString + "\nAnswer Context (NLG): ";
			answerString = answerString + GIAnlg.generateLanguageFromQuery(comparisonVariableNode, queryAnswerNode);				
			/*
			if(comparisonVariableNode->isWhichOrEquivalentWhatQuery)
			{
				answerString = answerString + GIAnlg.generateLanguageFromWhichQuery(comparisonVariableNode, queryAnswerNode);
			}
			else
			{
				answerString = answerString + GIAnlg.generateLanguageFromQuery(comparisonVariableNode, queryAnswerNode);				
			}
			*/
			#endif
		}

		//add confidence to answer
		double confidencePrint = confidence*GIA_QUERY_CONFIDENCE_MULTIPLIER;
		double maxConfidencePrint = maxConfidence*GIA_QUERY_CONFIDENCE_MULTIPLIER;
		string tempConfidenceString = SHAREDvars.convertDoubleToString(confidencePrint, "%0.6f");
		string tempMaxConfidenceString = SHAREDvars.convertDoubleToString(maxConfidencePrint, "%0.6f");
		answerString = answerString + "\nconfidence = " + tempConfidenceString;
		answerString = answerString + "\nmax confidence = " + tempMaxConfidenceString;

		SHAREDvars.setCurrentDirectory(outputFolder);

		#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
		writeStringToFile(outputTextAnswerPlainTXTFileName, &answerString);
		#else		
		cout << answerString << endl;
		#endif
		
		#ifdef USE_GIAI
		translatorVariables->giaQueryAnswer = answerString;
		#endif
		
		#endif
	#endif
	
		#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
		if(useLRP)
		{
			GIApreprocessorWordReduction.deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(true);	//required for local variable access
		}
		#endif
			
	}
	#ifdef GIA_QUERY_WRITE_ANSWER_TO_FILE
	else if(useOutputTextAnswerPlainTXTFile)
	{
		cout << "error: outputText answer require a query to be set" << endl;
	}
	#endif

	SHAREDvars.setCurrentDirectory(outputFolder);

	#ifdef GIA_NEURAL_NETWORK
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_ANN
	GIAneuralNetworkOperations.generateNeuralNetFromGIAtxtRelTranslatorNet(translatorVariables);	//generate GIA NLP neural network
	#endif
	#ifdef GIA_NEURAL_NETWORK_PASSIVE
	GIAneuralNetworkOperations.generateNeuralNetFromSemanticNet(translatorVariables);	//generate GIA KB neural network
	#endif
	
	if(ANNdrawOutput)
	{
		string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
		ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight);
	}
	if(ANNuseOutputXMLFile)
	{
		if(!GIAneuralNetworkOperations.writeNeuralNetXMLfile(ANNoutputXMLFileName, translatorVariables->firstInputNeuronInNetwork))
		{
			result = false;
		}
	}		
	#endif	
	
#ifndef GIA_DISABLE_SEMANTIC_TRANSLATOR

	if(printOutput)
	{
		GIAdraw.printGIAnetworkNodes(translatorVariables, rasterImageWidth, rasterImageHeight, outputTextLDRFileName, outputTextSVGFileName, outputTextPPMFileName, displayInOpenGLAndOutputScreenshot, useOutputTextLDRFile, useOutputTextPPMFile, useOutputTextSVGFile);
	}

	if(useOutputTextXMLFile)
	{
		if(!GIAxmlConversion.writeSemanticNetXMLFileOptimised(outputTextXMLFileName, translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes))
		{
			result = false;
		}
	}
	if(useOutputTextCXLFile)
	{
		if(!GIAcxlConversion.writeCmapToolsCXLFileOptimised(outputTextCXLFileName, translatorVariables->entityNodesActiveListComplete, translatorVariables->entityNodesActiveListNetworkIndexes))
		{
			result = false;
		}
	}
		
	#ifdef GIA_DATABASE
	if(useDatabase)
	{
		#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		//set networkIndexEntityLoaded disabled values (used by DBwriteNetworkIndexEntityNodesLoadedList() to prevent the writing of disabled networkIndex nodes...)
		for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListMapIter = translatorVariables->entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListMapIter != translatorVariables->entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListMapIter++)
		{
			GIAentityNode* entityNode = networkIndexEntityNodesListMapIter->second;
			if(entityNode->disabled)
			{
				(entityNode->networkIndexEntityLoaded)->disabled = true;
			}
		}
		#endif

		if(writeToDatabase)
		{
			//NB currently uses entityNodesActiveListComplete to record which nodes might possibly require an update on the server
			GIAdatabase.writeAndCloseDatabase(translatorVariables->entityNodesActiveListComplete);
		}
		else
		{
			GIAdatabase.closeDatabase();
		}
	}
	#endif
#endif

	if(displayInOpenGLAndOutputScreenshot)
	{
		LDopengl.exitOpenGL();
	}

	return result;
}

		

