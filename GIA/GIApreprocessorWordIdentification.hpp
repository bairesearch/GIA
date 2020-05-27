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
 * File Name: GIApreprocessorWordIdentification.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k17a 26-May-2020
 * Requirements: requires plain text file
 * Description: Preprocessor Word Identification
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_WORD_IDENTIFICATION
#define HEADER_GIA_PREPROCESSOR_WORD_IDENTIFICATION

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp" 
#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "GIApreprocessorWordReduction.hpp" 
#endif
#include "SHAREDvars.hpp"	//file io





class GIApreprocessorWordIdentificationClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: GIApreprocessorWordReductionClass GIApreprocessorWordReduction;
	#endif

	private: unordered_map<string, GIApreprocessorMultiwordReductionWord*>* getWordList(const int GIAposType);
		private: unordered_map<string, GIApreprocessorMultiwordReductionWord*>* getWordListIfExistent(const int GIAposType, bool* result);
	private: bool wordListExistent(int GIAposType);
	private: bool transferWordList(int GIAposType1, int GIAposType2, unordered_map<string, GIApreprocessorMultiwordReductionWord*>** wordList1record);
	
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, uint64_t>>* getWordListAllTypesWithPOSambiguityInfo();
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY_ASSIGN_WEIGHTS_TO_TRAINED_POS_TYPES
	public: double getGIApreprocessorPOStypeWeight(int GIAposType);
	#endif
	//private: bool findInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, unsigned char>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, unsigned char* POSambiguityInfo);
	private: void insertInstanceInMapWordListAllTypesWithPOSambiguityInfo(unordered_map<string, pair<GIApreprocessorMultiwordReductionWord*, uint64_t>>* mapWordListAllTypesWithPOSambiguityInfo, const string wordIndex, GIApreprocessorMultiwordReductionWord* word, const uint64_t POSambiguityInfo);
	#endif
	
	public: bool initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP);
	public: bool getUseLRP();
		private: bool loadWordListWrapper(bool* wordListLoaded, const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
			private: bool loadWordList(const string wordListFileName, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList);
		private: bool loadStructuredDataListIrregularVerb(const string irregularVerbListFileName, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);

	#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
	private: bool generateNounPluralVariantsList(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* irregularNounList);
		private: bool generateNounPluralVariants(GIApreprocessorMultiwordReductionWord* wordTag, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* nounPluralVariantsList, unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* irregularNounList);
	#endif
	
	#ifdef GIA_PREPROCESSOR_WORD_LOAD_WORD_LISTS
	public: bool generateVerbCaseStandardAndAdditionalList(bool grammaticallyStrict);
	#endif	
		#ifdef GIA_PREPROCESSOR_WORD_LOAD_WORD_LISTS
		private: bool generateAdditionalTenseVariantsOfVerbBase(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* verbCaseAdditionalList, GIApreprocessorMultiwordReductionWord* baseTag, bool irregularVerbFound, bool grammaticallyStrict);
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

	#ifdef GIA_PREPROCESSOR_WORD_LOAD_INVERSE_PREPOSITIONS_LIST
	private: bool loadStructuredDataList(const string prepositionsInverseListFileName, unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* prepositionInverseList);
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	public: void detectIfInverseOrTwoWayConditionRequired(const string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName);
	#ifdef GIA_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	public: bool identifyConditionType(GIAentityNode* conditionRelationshipEntity);
	#endif
	#endif

	public: bool checkGrammaticalWordTypeFeaturePrelim(GIApreprocessorPlainTextWord* wordTag, const int grammaticalWordType);

	public: bool determineIsWordType(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int grammaticalWordType);
		public: bool determineIsWordType(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, const int grammaticalWordType, string* baseNameFound, int* grammaticalBaseTenseForm);
			public: bool determineIsWordType(const string word, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordTypeList);
	public: bool determineIsVerb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict);	
		public: bool determineIsVerb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, const bool grammaticallyStrict, string* baseNameFound, int* grammaticalBaseTenseForm);
		public: bool determineIsVerbString(const string word);
			public: bool determineVerbCaseStandardWithAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseStandard(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				public: bool determineVerbCaseAdditional(const string word, string* baseNameFound, int* grammaticalBaseTenseForm);
				//public: bool convertVerbCaseGrammaticalTenseFormToTenseModifier(const int grammaticalTenseForm, int* grammaticalTenseModifier);
				public: bool verbCaseDetectGrammaticallyStrictVariant(const int grammaticalTenseForm);
	public: bool determineIsPreposition(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		public: bool determineIsPrepositionString(const string word);
	public: bool determineIsAdverb(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		public: bool determineIsAdverbString(const string word);
	public: bool determineIsAdjective(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		public: bool determineIsAdjectiveString(const string word);
	public: bool determineIsNoun(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		public: bool determineIsNoun(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim, string* baseNameFound, int* grammaticalBaseForm);
			public: bool determineIsNounString(const string word);
			public: bool determineNounPluralVariant(const string word, GIApreprocessorMultiwordReductionWord** nounBaseFound, int* grammaticalBaseTenseForm);
	public: bool determineIsConjunction(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsConjunctionString(const string word);
	public: bool determineIsDeterminer(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
		//public: bool determineIsDeterminerString(const string word);
	public: bool determineIsAuxiliaryBeing(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	public: bool determineIsAuxiliaryHaving(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	public: bool determineIsAuxiliaryDoing(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	public: bool detectAuxiliary(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	public: bool detectModalAuxiliary(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	public: bool detectRcmodSameReferenceSetDelimiter(GIApreprocessorPlainTextWord* wordTag, const bool usePOSprelim);
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OLD
	public: bool determineIsWordTypeStringBasic(const string word, const int GIAposType);
	#endif
	
	public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word);
		public: bool findWordInWordList(unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, const string word, GIApreprocessorMultiwordReductionWord** wordFound);
	public: bool findSentenceInSentenceListBasic(unordered_map<string, GIApreprocessorMultiwordReductionBasicSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionBasicSentence** sentenceFound);
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: bool createWordIndexListFromLRPfiles();
	#endif
			
};


#endif
