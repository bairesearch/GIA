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
 * File Name: GIAsemanticParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b3a 25-May-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER
#define HEADER_GIA_SEMANTIC_PARSER

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAnlp.hpp"
#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"	//file io

#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
class GIAsemanticParserClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAnlpClass GIAnlp;
	private: GIAtranslatorClass GIAtranslator;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	public: bool performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputPlainTXTFileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName, const string NLPexeFolderArray[]);
		private: bool lookupSemanticParserFiles(GIAparagraph* firstParagraphInList, const int NLPfeatureParser);
			private: bool lookupSemanticParserFiles(GIAsentence* firstSentenceInList, const int NLPfeatureParser);
};
#endif

#endif
