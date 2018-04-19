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
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e10a 15-January-2018
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
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY
	public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, bool useLRP);
	#endif
	public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, const int wikiDumpFileBatchIndex, bool useLRP);
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
		private: bool addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, ofstream* XtrainBatchFileObject, ofstream* YtrainBatchFileObject);
		#else
		private: bool addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, ANNexperience* currentExperienceInList);
		#endif
		#else
		private: bool addPOStaggerDatabaseEntry(const unsigned int centreWordPOSambiguityInfo, vector<unsigned int>* POSambiguityInfoPermutation, const unsigned int centreWordUnambiguousPOSvalue, long* numberOfNeuralNetworkFeeds);
		#endif
		private: string generateWikiDumpTextInputFileName(int wikiDumpFileBatchIndex);
		private: bool generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText, bool useLRP);
			private: bool createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences);
	#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FEED_ALL_PERMUTATIONS_INDIVIDUALLY
		public: void generatePOSambiguityInfoUnambiguousPermutationArray(vector<vector<unsigned int>*>* POSambiguityInfoUnambiguousPermutationArray, vector<unsigned int>* POSambiguityInfoPermutation, vector<unsigned int>* POSambiguityInfoUnambiguousPermutationLocal, int wordIndex);
		#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
		public: bool generateANNexperienceFromPOSambiguityInfoPermutation(vector<unsigned int>* POSambiguityInfoPermutation, const int centreWordUnambiguousPOSvalue, ANNexperience* currentExperience);
			#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_RESTRICT_POS_TYPES_TO_8BIT	
			private: bool determinePOSambiguityInfoNeuralNetworkInputNeuronForSpecialCharacters(const unsigned int POSambiguityInfo, int* POSambiguityInfoNeuralNetworkInputNeuron);
			#endif
		#endif
		public: int convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue);
		public: bool generatePOSambiguityInfoPermutation(vector<GIApreprocessorWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, bool* identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, vector<unsigned int>* POSambiguityInfoPermutation);
		public: bool determinePOSambiguityInfoIsAmbiguous(const unsigned int POSambiguityInfo, unsigned int* unambiguousPOSvalue, const bool treatWordAsAmbiguousIfNullPOSvalue);
		public: bool determinePOSambiguityInfoForSpecialCharacters(const GIApreprocessorWord* word, unsigned int* contextWordPOSambiguityInfo);

		public: bool findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, unsigned int* POSambiguityInfoFound);

	private: string convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord);
	public: string convertPOSambiguityInfoPermutationToString(vector<unsigned int>* unsignedIntVector);
		public: unsigned char convertPOSambiguityInfoToChar(unsigned int POSambiguityInfo);


};

#endif

#endif

