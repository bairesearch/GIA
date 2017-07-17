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
 * File Name: GIApreprocessorMultiwordReduction.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d3b 17-July-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION
#define HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReductionClass.hpp" 
#include "SHAREDvars.hpp"	//file io





class GIApreprocessorMultiwordReductionClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;

	public: bool initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP);
		private: bool loadWordListWrapper(const string lrpDataFolderName, bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
			private: bool loadWordList(const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
	public: bool getUseLRP();

	public: GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	public: void setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);

	public: bool parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName);
		private: bool loadIrregularVerbList(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
		private: bool loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
			private: bool generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
				private: void copyDefaultVerbTenseFormsToAlternateTenseForms(GIApreprocessorMultiwordReductionWord* baseTag, const bool irregularVerbFound);
		//public: bool loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText);
		private: bool searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo);
		private: bool loadMultiwordWordListAndSearchAndReplace(const string multiwordWordListFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
			private: bool loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList);
			private: bool searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
				private: void renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched);
		public: bool writeTagListToFile(const GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput);
			private: string generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	public: void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString);
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
	public: bool generateVerbCaseStandardList();
	private: bool generateVerbCaseAdditionalList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList);
		private: void addVerbCaseAdditional(GIApreprocessorMultiwordReductionWord* currentTagInVerbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, const string baseTenseFormStart, string baseTenseFormAppend, int grammaticalTenseModifierNew);
	#endif
	
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
	public: bool determineVerbCaseAdditionalWrapper(const string word, string* baseNameFound, int* grammaticalTenseModifier);
	#endif
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
	public: bool determineIfWordIsIrregularVerbContinuousCaseWrapper(const string word, string* baseNameFound);
		private: bool determineIfWordIsIrregularVerbContinuousCase(const string word, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, string* baseNameFound);
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
	private: bool loadPrepositionsInverseList(const string prepositionsInverseListFileName, unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* prepositionInverseList);
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	public: void detectIfInverseOrTwoWayConditionRequired(const string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
	public: bool identifyConditionType(GIAentityNode* conditionRelationshipEntity);
	#endif
	#endif
	
	public: bool determineIsVerb(GIApreprocessorWord* wordTag, int* grammaticalBaseTenseForm);
		public: bool determineVerbCaseStandardWithAdditional(const string word, int* grammaticalBaseTenseForm);
			public: bool determineVerbCaseStandard(const string word, int* grammaticalBaseTenseForm);
			public: bool determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalTenseModifier);
	public: bool determineIsPreposition(GIApreprocessorWord* wordTag);
		public: bool determineIsPreposition(const string word);
	public: bool determineIsAdverb(GIApreprocessorWord* wordTag);
		public: bool determineIsAdverb(const string word);
	public: bool determineIsAdjective(GIApreprocessorWord* wordTag);
		public: bool determineIsAdjective(const string word);
	public: bool determineIsNoun(GIApreprocessorWord* wordTag);
		public: bool determineIsNoun(const string word);
	
	public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word);
		public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word, GIApreprocessorMultiwordReductionWord** wordFound);
	public: bool findSentenceInSentenceListBasic(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionBasicSentence** sentenceFound);
	public: bool findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound);

			
};


#endif
