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
 * File Name: GIApreprocessorMultiwordReduction.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1k 22-February-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 * /
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

	private: unordered_map<string, GIApreprocessorMultiwordReductionWord*>* getWordList(const int GIAposType);
		private: unordered_map<string, GIApreprocessorMultiwordReductionWord*>* getWordListIfExistent(const int GIAposType, bool* result);
	private: bool wordListExistent(int GIAposType);
	private: bool transferWordList(int GIAposType1, int GIAposType2, unordered_map<string, GIApreprocessorMultiwordReductionWord*>** wordList1record);
	
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>* getWordListAllTypesWithPOSambiguityInfo();
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES
	public: double getGIApreprocessorPOStypeWeight(int GIAposType);
	#endif
	//private: bool findInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, unsigned char* POSambiguityInfo);
	private: void insertInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned long>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, const unsigned long POSambiguityInfo);
	#endif
	
	public: bool initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP);
	public: bool getUseLRP();
		private: bool loadWordListWrapper(bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
			private: bool loadWordList(const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
		private: bool loadIrregularVerbList(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
		private: bool loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
		
	private: bool generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, bool grammaticallyStrict);
			private: void copyDefaultVerbTenseFormsToAlternateTenseForms(GIApreprocessorMultiwordReductionWord* baseTag, const bool irregularVerbFound);

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	public: GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	public: void setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	
	public: bool parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName);
		//public: bool loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText);
		private: bool searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo);
		private: bool loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList);
		private: bool searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DYNAMIC
		private: bool searchAndReplaceMultiwordWordListDynamic(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
		#endif		
			private: void createNewCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo** currentCorrespondenceInfo, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence, GIApreprocessorMultiwordReductionPlainTextWord** currentTagInPlainTextSentence, int entityIndex, int numberWordsInMultiwordMatched);
			private: void renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched);
		public: bool writeTagListToFile(const GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput);
			private: string generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	public: void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString);
	#endif
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

	public: bool checkGrammaticalWordTypeFeaturePrelim(GIApreprocessorWord* wordTag, const int grammaticalWordType);

	public: bool determineIsWordType(GIApreprocessorWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int grammaticalWordType);
		public: bool determineIsWordType(GIApreprocessorWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int grammaticalWordType, string* baseNameFound, int* grammaticalBaseTenseForm);
			public: bool determineIsWordType(const string word, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordTypeList);
	public: bool determineIsVerb(GIApreprocessorWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict);	
		public: bool determineIsVerb(GIApreprocessorWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, string* baseNameFound, int* grammaticalBaseTenseForm);
			public: bool determineVerbCaseStandardWithAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseStandard(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				//public: bool convertVerbCaseGrammaticalTenseFormToTenseModifier(const int grammaticalTenseForm, int* grammaticalTenseModifier);
				public: bool verbCaseDetectGrammaticallyStrictVariant(const int grammaticalTenseForm);
	public: bool determineIsPreposition(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsPreposition(const string word);
	public: bool determineIsAdverb(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsAdverb(const string word);
	public: bool determineIsAdjective(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsAdjective(const string word);
	public: bool determineIsNoun(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		public: bool determineIsNoun(GIApreprocessorWord* wordTag, const bool usePOSprelim, string* baseNameFound, int* grammaticalBaseForm);
			//public: bool determineIsNoun(const string word);
			public: bool determineNounPluralVariant(const string word, GIApreprocessorMultiwordReductionWord** nounBaseFormFound);
	public: bool determineIsConjunction(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsConjunction(const string word);
	public: bool determineIsDeterminer(GIApreprocessorWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsDeterminer(const string word);
	public: bool determineIsAuxiliaryBeing(GIApreprocessorWord* wordTag, const bool usePOSprelim);
	public: bool determineIsAuxiliaryHaving(GIApreprocessorWord* wordTag, const bool usePOSprelim);
	public: bool determineIsAuxiliaryDoing(GIApreprocessorWord* wordTag, const bool usePOSprelim);
	public: bool detectAuxiliary(GIApreprocessorWord* wordTag, const bool usePOSprelim);
	public: bool detectModalAuxiliary(GIApreprocessorWord* wordTag, const bool usePOSprelim);
	public: bool detectRcmodSameReferenceSetDelimiter(GIApreprocessorWord* wordTag, const bool usePOSprelim);

	public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word);
		public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word, GIApreprocessorMultiwordReductionWord** wordFound);
	public: bool findSentenceInSentenceListBasic(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionBasicSentence** sentenceFound);
	public: bool findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound);
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: bool createWordIndexListFromLRPfiles();
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS
	private: bool replaceAllNumericalWordsWithNumbers(GIApreprocessorSentence* firstGIApreprocessorSentenceInList);
		private: long convertWordToNumber(vector<GIApreprocessorWord*>* numericalWordList);
			private: long parseNumerals(vector<GIApreprocessorWord*>* numericalWordListSubset);
				private: long getValueOf(const string wordText);
	#endif
			
};


#endif
