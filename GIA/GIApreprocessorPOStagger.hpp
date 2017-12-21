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
 * Project: General Intelligence Algorithm
 * Project Version: 3e8b 18-December-2017
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
#include "GIApreprocessorPOStaggerDatabase.hpp"


#ifdef GIA_PREPROCESSOR_POS_TAGGER

#ifdef GIA_PREPROCESSOR_WORD_LIST_INTERPRET_PRONOUN_POS_AS_NOUN_POS
static int GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE] = {GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_CONJUNCTION, GRAMMATICAL_WORD_TYPE_DETERMINER, GRAMMATICAL_WORD_TYPE_INTERJECTION};
#else
static int GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE] = {GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_CONJUNCTION, GRAMMATICAL_WORD_TYPE_DETERMINER, GRAMMATICAL_WORD_TYPE_PRONOUN};
#endif	

class GIApreprocessorPOStaggerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;

	private: bool findInstancePOStaggerMap(const string POSambiguityInfoPermutation, unsigned char centreWordPOSambiguityInfo, int* numberOfInstances, const bool incrementIfFound);
	private: void insertInstanceInPOStaggerMap(const string POSambiguityInfoPermutation, const unsigned char centreWordPOSambiguityInfo, const int numberOfInstances);
	private: multimap<string, pair<unsigned char, int>>* getPOStaggerMap();

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_WIKI_DUMP_TEXT
	public: bool createWordIndexListFromWikiDumpText(bool useLRP);
	#endif	
	//public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, bool useLRP);
	public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, const int wikiDumpFileBatchIndex, bool useLRP);
		private: string generateWikiDumpTextInputFileName(int wikiDumpFileBatchIndex);
		private: bool generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText, bool useLRP);
			private: bool createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences);
	#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
		public: bool generateANNexperienceFromPOSambiguityInfoPermutation(const string POSambiguityInfoPermutation, const int centreWordUnambiguousPOSvalue, ANNexperience* currentExperience);
			private: bool determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters(const unsigned char POSambiguityInfo, int* POSambiguityInfoNeuralNetworkInputNeuron);
		#endif
		public: int convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue);
		public: bool generatePOSambiguityInfoPermutation(vector<GIApreprocessorWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, string* POSambiguityInfoPermutationRecord);
		public: bool determinePOSambiguityInfoIsAmbiguous(const unsigned char POSambiguityInfo, int* unambiguousPOSvalue);
		public: bool determinePOSambiguityInfoForSpecialCharacters(const GIApreprocessorWord* word, unsigned char* contextWordPOSambiguityInfo);

		public: bool findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, unsigned char* POSambiguityInfoFound);

	private: string convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord);
};

#endif

#endif

