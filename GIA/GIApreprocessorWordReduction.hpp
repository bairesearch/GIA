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
 * File Name: GIApreprocessorWordReduction.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f10e 19-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Word Reduction
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_WORD_REDUCTION
#define HEADER_GIA_PREPROCESSOR_WORD_REDUCTION

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp" 
#include "SHAREDvars.hpp"	//file io




class GIApreprocessorWordReductionClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;

	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION

	public: bool initialiseLRPreduction(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
		private: bool loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList);
	
	/*
	public: multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* getPhrasalVerbListGlobal();
	public: unordered_map<string, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>*>* getMultiwordListsGlobal();
	*/
	
	public: GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	public: void setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	
	public: bool parseTextFileAndReduceLanguage(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName);
		//public: bool loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductionSentence* firstTagInPlainText);
		private: bool searchAndReplacePhrasalVerbs(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionPhrasalVerbSentence*>* phrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo);
		private: bool loadMultiwordWordList(const string multiwordWordListFileName, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList);
		private: bool searchAndReplaceMultiwordWordList(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, multimap<string, GIApreprocessorMultiwordReductionBasicSentence*>* multiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
		#ifdef GIA_PREPROCESSOR_WORD_DYNAMIC
		private: bool searchAndReplaceMultiwordWordListDynamic(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, unordered_map<string, GIApreprocessorMultiwordReductionWord*>* wordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
		#endif		
			private: void createNewCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo** currentCorrespondenceInfo, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorMultiwordReductionPlainTextWord* firstTagInPlainTextSentence, GIApreprocessorMultiwordReductionPlainTextWord** currentTagInPlainTextSentence, int entityIndex, int numberWordsInMultiwordMatched);
			private: void renumberEntityIndiciesInCorrespondenceInfo(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, int sentenceIndex, int entityIndex, int numberWordsInMultiwordMatched);
		public: bool writeTagListToFile(const GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPoutput, const bool performLRPforNLPoutput);
			private: string generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductionPlainTextWord* currentTagInPlainTextSentence);
	#ifdef GIA_PREPROCESSOR_WORD_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	public: void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString);
	#endif
	#endif
	
	public: bool generateIrregularTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* irregularVerbList, bool grammaticallyStrict);	
	public: bool generateStandardTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductionWord* baseTag, bool irregularVerbFound, bool grammaticallyStrict);
	public: bool findSentenceInSentenceListIrregularVerb(unordered_map<string, GIApreprocessorMultiwordReductionIrregularVerbSentence*>* sentenceList, const string word, GIApreprocessorMultiwordReductionIrregularVerbSentence** sentenceFound);

	#ifdef GIA_PREPROCESSOR_WORD_COLLAPSE_NUMERICAL_WORDS_TO_NUMBERS
	private: bool replaceAllNumericalWordsWithNumbers(GIApreprocessorSentence* firstGIApreprocessorSentenceInList);
		private: long convertWordToNumber(vector<GIApreprocessorPlainTextWord*>* numericalWordList);
			private: long parseNumerals(vector<GIApreprocessorPlainTextWord*>* numericalWordListSubset);
				private: long getValueOf(const string wordText);
	#endif
			
};


#endif
