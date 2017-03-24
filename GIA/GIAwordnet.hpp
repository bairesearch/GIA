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
 * File Name: GIAwordnet.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1p 26-February-2017
 * Requirements: requires wordnet libraries to be installed
 * Description: searches wordnet database and parses wordnet output
 *
 *******************************************************************************/


#ifndef HEADER_GIA_WORDNET
#define HEADER_GIA_WORDNET

#include "GIAglobalDefs.hpp"
#include "wn.h"
#include "SHAREDvars.hpp"

#define SYNONYMN_DETECTION_STATUS_OFF 0
#define SYNONYMN_DETECTION_STATUS_QUERIES_ONLY 1
#define SYNONYMN_DETECTION_STATUS_QUERIES_AND_ADVANCED_REFERENCING 2

#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT (10000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_LINE (1000)
#define MAX_CHARACTERS_OF_WORDNET_FINDTHEINFO_OUTPUT_NUMBER_OF_SENSES (10)	//supports e^10 senses
#define WORDNET_FINDTHEINFO_OUTPUT_MAX_NUMBER_SYNONYMS (100)

#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES (1)
static int wordnetDataEntryPointersIndicatingSimilarSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_SIMILAR_SYNSETS_NUMBER_OF_TYPES] = {SIMPTR};

//#define CURRENTSYNSET (99)

#ifdef WORDNET_SEARCH_RELATED_SYNSETS
	#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES (2)
	static int wordnetDataEntryPointersIndicatingRelatedSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES] = {SIMPTR, HYPOPTR};
#else
	#define WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES (0)
	static int wordnetDataEntryPointersIndicatingRelatedSynsetsArray[WORDNET_DATA_ENTRY_POINTERS_INDICATING_RELATED_SYNSETS_NUMBER_OF_TYPES] = {};
#endif
#define CURRENTSYNSETPOINTERINDEX (-1)

class GIAwordnetClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void initialiseWordNet(const int newSynonymnDetectionStatus);
	public: int getSynonymnDetectionStatus();

	public: bool checkIfWordIsContainedWithinOtherWordsSynsetsOrViceVersa(string* word, string* otherWord, const int wordNetPOS);
		private: bool checkIfWordIsContainedWithinAnotherWordsSynsets(const string* word, const string* otherWord, const int wordNetPOS);
	private: SynsetPtr findMostPopularSynsets(const string* word, bool* wordIsFound, const int wordNetPOS);
			private: SynsetPtr findSynsets(const string* word, bool* wordIsFound, const int wordNetPOS, const int similarityType);
			private: SynsetPtr checkIfSynsetListContainsSynonymousEntityNamesAndRecordMostPopularSynset(SynsetPtr firstSenseInList, const int wordNetPOS, int* maximumNumberOfTags, bool* entityNamesAreSynonymous, const string* word, const string* otherWord, const bool compareEntityNames, bool* senseOutputWithHighestTagsPassedNewSynsetMustFree);



	private: void findSynonymsOLD(const string word, bool* wordIsFound, string listOfSynonyms[], const int wordNetPOS);
	private: bool recordUntilCharacterOrEscapeCharacterOLD(int charIndex, const char* output, int* newCharIndex, string* lineString, const char characterToRecordUntil, const char escapeCharacter);
};

#endif
