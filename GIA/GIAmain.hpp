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
 * File Name: GIAmain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j6a 10-January-2020
 * Requirements: 
 * Description: Main
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN



#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAtranslator.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdraw.hpp"
#include "GIAquery.hpp"
#include "GIAxmlConversion.hpp"
#include "GIAcxlConversion.hpp"
#include "GIAdatabase.hpp"
#include "GIAnlp.hpp"
#include "XMLrulesClass.hpp"
#include "LDsprite.hpp"
#include "LDopengl.hpp"
#ifdef USE_WORDNET
#include "GIAwordnet.hpp"
#endif
#ifdef GIA_NLG
#include "GIAnlg.hpp"
#endif
#ifdef GIA_PREPROCESSOR
#include "GIApreprocessor.hpp"
#include "GIApreprocessorWordIdentification.hpp"
#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "GIApreprocessorWordReduction.hpp" 
#endif
#endif
#ifdef GIA_SEM_REL_TRANSLATOR
#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
#include "GIAsemRelTranslator.hpp"
#endif
#include "GIAsemRelTranslatorDatabase.hpp"
#endif
#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
#include "GIApreprocessorPOStaggerDatabase.hpp"
#endif
#ifdef GIA_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#include "ANNdisplay.hpp"
#include "ANNdraw.hpp"
#include "ANNxmlConversion.hpp"
#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
#include "GIAneuralNetworkNonSemantic.hpp"
#endif
#include "GIAneuralNetworkOperations.hpp"
#endif
#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
#include "GIAtxtRelTranslator.hpp"
#endif

#define GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST (1000000)


#ifdef COMPILE_GIA
int main(const int argc, const char** argv);
#endif
#ifdef COMPILE_GIA_WITH_ANN_GENERATE_POS_TAGGER_DATABASE
int main(const int argc, const char** argv);
#endif

class GIAmainClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorClass GIAtranslator;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_PREPROCESSOR
	private: GIApreprocessorClass GIApreprocessor;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: GIApreprocessorWordReductionClass GIApreprocessorWordReduction;
	#endif
	#endif
	private: GIAnlpClass GIAnlp;
	private: XMLrulesClassClass XMLrulesClass;
	private: GIAqueryClass GIAquery;
	#ifdef USE_WORDNET
	private: GIAwordnetClass GIAwordnet;
	#endif
	private: GIAdrawClass GIAdraw;
	private: LDopenglClass LDopengl;
	private: LDspriteClass LDsprite;
	private: GIAxmlConversionClass GIAxmlConversion;
	private: GIAcxlConversionClass GIAcxlConversion;
	#ifdef GIA_NLG
	private: GIAnlgClass GIAnlg;
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR
	#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
	private: GIAsemRelTranslatorClass GIAsemRelTranslator;
	#endif
	private: GIAsemRelTranslatorDatabaseClass GIAsemRelTranslatorDatabase;
	#endif
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_PERSISTENT
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;	
	#endif
	#ifdef GIA_NEURAL_NETWORK
	private: ANNneuronClassClass ANNneuronClassClassObject;
	private: ANNdisplayClass ANNdisplay;
	private: ANNdrawClass ANNdraw;
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	private: GIAneuralNetworkNonSemanticClass GIAneuralNetworkNonSemantic;
	#endif
	private: GIAneuralNetworkOperationsClass GIAneuralNetworkOperations;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	private: GIAtxtRelTranslatorClass GIAtxtRelTranslator;
	#endif

	public: bool executeGIA(

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

		);

	#ifdef USE_CS_WORKAROUND
	public: bool executeGIA2();
	#endif
};




#endif
