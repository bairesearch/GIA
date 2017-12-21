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
 * Project Version: 3e7a 16-December-2017
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

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* getWordListAllTypesWithPOSambiguityInfo();
	//private: bool findInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, unsigned char* POSambiguityInfo);
	private: void insertInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, const unsigned char POSambiguityInfo);
	#endif
	
	public: bool initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP);
		private: bool loadWordListWrapper(bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
			private: bool loadWordList(const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
	public: bool getUseLRP();

	public: GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	public: void setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);

	public: bool parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName);
		private: bool loadIrregularVerbList(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
		private: bool loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
			private: bool generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, bool grammaticallyStrict);
				private: void copyDefaultVerbTenseFormsToAlternateTenseForms(GIApreprocessorMultiwordReductionWord* baseTag, const bool irregularVerbFound);
		//public: bool loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText);
		private: bool searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo);
		private: bool searchAndReplaceMultiwordWordList(multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
			private: bool loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList);
			private: bool searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
				private: void renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched);
		public: bool writeTagListToFile(const GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput);
			private: string generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	public: void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString);
	#endif

	#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
	private: bool generateNounPluralVariantsList();
		private: bool generateNounPluralVariants(GIApreprocessorMultiwordReductionWord* wordTag, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounPluralVariantsList);
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
	public: bool generateVerbCaseStandardList(bool grammaticallyStrict);
	private: bool generateVerbCaseAdditionalList(bool grammaticallyStrict);
		private: void addVerbCaseAdditional(GIApreprocessorMultiwordReductionWord* currentTagInVerbList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, const string baseTenseFormStart, string baseTenseFormAppend, int grammaticalTenseFormNew);
	#endif
		
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
	public: bool determineVerbCaseAdditionalWrapper(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
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

	public: bool checkGrammaticalWordTypeFeaturePrelim(GIApreprocessorWord* wordTag, int grammaticalWordType);

	public: bool determineIsWordType(GIApreprocessorWord* wordTag, bool usePOSprelim, int grammaticalWordType);
		public: bool determineIsWordType(GIApreprocessorWord* wordTag, bool usePOSprelim, int grammaticalWordType, string* baseNameFound, int* grammaticalBaseTenseForm);
			public: bool determineIsWordType(const string word, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordTypeList);
	public: bool determineIsVerb(GIApreprocessorWord* wordTag, bool usePOSprelim);	
		public: bool determineIsVerb(GIApreprocessorWord* wordTag, bool usePOSprelim, string* baseNameFound, int* grammaticalBaseTenseForm);
			public: bool determineVerbCaseStandardWithAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseStandard(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool convertVerbCaseGrammaticalTenseFormToTenseModifier(const int grammaticalTenseForm, int* grammaticalTenseModifier);
				private: bool verbCaseDetectGrammaticallyStrictVariant(GIApreprocessorMultiwordReductionWord* word);
	public: bool determineIsPreposition(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsPreposition(const string word);
	public: bool determineIsAdverb(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsAdverb(const string word);
	public: bool determineIsAdjective(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsAdjective(const string word);
	public: bool determineIsNoun(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsNoun(GIApreprocessorWord* wordTag, bool usePOSprelim, string* baseNameFound, int* grammaticalBaseForm);
			public: bool determineIsNoun(const string word);
			public: bool determineNounPluralVariant(const string word, GIApreprocessorMultiwordReductionWord** nounBaseFormFound);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS_ADDITIONAL
	public: bool determineIsConjunction(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsConjunction(const string word);
	public: bool determineIsDeterminer(GIApreprocessorWord* wordTag, bool usePOSprelim);
		public: bool determineIsDeterminer(const string word);
	#endif
	
	public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word);
		public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word, GIApreprocessorMultiwordReductionWord** wordFound);
	public: bool findSentenceInSentenceListBasic(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionBasicSentence** sentenceFound);
	public: bool findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound);
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: bool createWordIndexListFromLRPfiles();
	#endif
			
};


#endif
