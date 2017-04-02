/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAmain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a5l 28-March-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN



#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#ifdef USE_CE
#include "CECodeextensionClass.h"
#endif
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
#include "GIApreprocessorMultiwordReduction.hpp"
#endif
#ifdef GIA_SEMANTIC_PARSER
#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
#include "GIAsemanticParser.hpp"
#endif
#include "GIAsemanticParserDatabase.hpp"
#endif

#define GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST (1000000)
//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE



#ifdef COMPILE_GIA
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
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
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
	#ifdef GIA_SEMANTIC_PARSER
	#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
	private: GIAsemanticParserClass GIAsemanticParser;
	#endif
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	#endif

public: bool executeGIA(

	GIAtranslatorVariablesClass* translatorVariables,
	GIAtranslatorVariablesClass* translatorVariablesQuery,

	string NLPexeFolderArray[],

	bool useInputTextPlainTXTFile,
	string inputTextPlainTXTfileName,

	#ifdef USE_CE
	bool useInputTextCodeextensionsTXTFileName,
	string inputTextCodeextensionsTXTFileName,
	#endif

	bool useInputTextNLPrelationXMLFile,
	string inputTextNLPrelationXMLfileName,
	bool useInputTextNLPfeatureXMLFile,
	string inputTextNLPfeatureXMLfileName,
	bool useOutputTextCFFFile,
	string outputTextCFFFileName,
	bool useInputTextXMLFile,
	string inputTextXMLFileName,
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
	string databaseFolderName,
	#endif

	#ifdef GIA_SEMANTIC_PARSER
	string semanticParserDatabaseFolderName,
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
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
