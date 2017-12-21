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
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3e2c 10-December-2017
 * Requirements: requires plain text file
 * Description: preprocessor POS tagger
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_POS_TAGGER
#define HEADER_GIA_PREPROCESSOR_POS_TAGGER

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReductionClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"
#include "GIAtranslatorDefineGrammar.hpp"


class GIApreprocessorPOStaggerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;

	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	private: bool findInstanceInMapPOStaggerDatabase(multimap<string, pair<unsigned char, int>>* mapPOStaggerDatabase, const string POSpermutationIndex, unsigned char* POSambiguityInfo, int* numberOfInstances, const bool incrementIfFound);
	private: void insertInstanceInMapPOStaggerDatabase(multimap<string, pair<unsigned char, int>>* mapPOStaggerDatabase, const string POSpermutationIndex, const unsigned char POSambiguityInfo, const int numberOfInstances);

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	public: bool generatePOStaggerDatabaseFromWikiDumpText();
		private: bool determinePOSambiguityInfoForSpecialCharacters(const GIApreprocessorWord* word, int* contextWordPOSambiguityInfo);
	#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_WIKI_DUMP_TEXT
		private: bool createWordIndexListFromWikiDumpText();
		#endif
			private: string generateWikiDumpTextInputFileName(int wikiDumpFileIndex);
			private: bool generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText);
		private: bool findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, int* POSambiguityInfoFound);

	private: bool createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences);
	#endif
};

#endif

