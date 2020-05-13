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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k11b 09-May-2020
 * Requirements: requires plain text file
 * Description: Preprocessor POS tagger
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_POS_TAGGER
#define HEADER_GIA_PREPROCESSOR_POS_TAGGER

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorWordIdentification.hpp"
#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "GIApreprocessorWordReduction.hpp" 
#endif
#include "GIAtranslatorGrammar.hpp"
#include "GIApreprocessorPOStaggerDatabase.hpp"
#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
#include "GIAposRelTranslatorSANIPropagateCompactGenerate.hpp"
#include "GIAposRelTranslatorRules.hpp"
#endif
#ifdef USE_ANN
#include "ANNexperienceClass.hpp"
#endif	
		


//static int GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE] = {GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_CONJUNCTION, GRAMMATICAL_WORD_TYPE_DETERMINER, GRAMMATICAL_WORD_TYPE_PRONOUN};

class GIApreprocessorPOStaggerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: GIApreprocessorWordReductionClass GIApreprocessorWordReduction;
	#endif
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_NEURAL_NETWORK_SEQUENCE_GRAMMAR
	private: GIAposRelTranslatorSANIPropagateCompactGenerateClass GIAposRelTranslatorSANIPropagateCompactGenerate;
	private: GIAposRelTranslatorRulesClass GIAposRelTranslatorRules;
	#endif
		
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	public: bool executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables);
		private: bool predictPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, GIApreprocessorPlainTextWord* centreWord, bool* foundMatchingCentreWordPOSambiguityInfo, unsigned char* centreWordPOSindexPrediction, bool* centreWordPOSisAmbiguous, double* experienceBackPropagationPassError, int* maximumNumberOfInstances);
	#endif
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_INITIALISE_WORD_INDEX_LIST_FROM_WIKI_DUMP_TEXT
	public: bool createWordIndexListFromWikiDumpText(bool useLRP);
	#endif	
	#ifndef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_TRAIN_SINGLE_BATCH_ONLY
	public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, bool useLRP);
	#endif
	public: bool generatePOStaggerDatabaseFromWikiDumpText(const string wikiDumpFolderName, const int wikiDumpFileBatchIndex, bool useLRP);
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE_WITHOUT_TARGET_CLASSES
		private: bool addPOStaggerDatabaseEntry(vector<uint64_t>* POSambiguityInfoPermutation, ofstream* XtrainBatchFileObject);
		#else
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_EXTERNAL_MEMORY_FREE_WRITE_EXPERIENCES_DIRECTLY_TO_FILE
		private: bool addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ofstream* XtrainBatchFileObject, ofstream* YtrainBatchFileObject);
		#else
		private: bool addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience* currentExperienceInList);
		#endif
		#else
		private: bool addPOStaggerDatabaseEntry(const uint64_t centreWordPOSambiguityInfo, vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, int64_t* numberOfNeuralNetworkFeeds);
		#endif
		#endif
		private: string generateWikiDumpTextInputFileName(int wikiDumpFileBatchIndex);
		private: bool generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText, bool useLRP);
			private: bool createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences);
	#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
		public: bool generateANNexperienceFromPOSambiguityInfoPermutation(vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience* currentExperience);
		#endif
		public: int convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue);
		public: bool generatePOSambiguityInfoPermutation(vector<GIApreprocessorPlainTextWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, bool* identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, vector<uint64_t>* POSambiguityInfoPermutation);

	private: string convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord);

	#endif
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: void generatePOSambiguityInfoUnambiguousPermutationArray(vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, vector<uint64_t>* POSambiguityInfoPermutation, vector<uint64_t>* POSambiguityInfoUnambiguousPermutationLocal, int wordIndex);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	public: bool determinePOSambiguityInfoWrapper(vector<GIApreprocessorPlainTextWord*>* sentenceContents, vector<uint64_t>* POSambiguityInfoPermutation, vector<string>* explicitWordList);
	#endif	
		public: bool determinePOSambiguityInfo(GIApreprocessorPlainTextWord* contextWord, uint64_t* contextWordPOSambiguityInfo, bool* contextWordPOSisAmbiguous, unsigned char* contextWordUnambiguousPOSindex, bool* identifiedEveryWordInDatabasePOSpermutation);
			public: bool findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, uint64_t* POSambiguityInfoFound);

	public: bool printPOSambiguityInfoPermutation(vector<uint64_t>* POSambiguityInfoPermutation);
		public: bool printPOSambiguityInfo(uint64_t POSambiguityInfo);
	public: bool printPOSambiguityInfoPermutationAssumeUnambiguous(vector<uint64_t>* POSambiguityInfoPermutation);
	#endif

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
	public: bool generateANNexperienceFromPOSambiguityInfoPermutationStandalone(vector<uint64_t>* POSambiguityInfoPermutation, const unsigned char outputNeuronExperienceValue, ANNexperience* currentExperience);
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	public: bool setSentenceContentsWordsUnambiguousPOSindex(vector<GIApreprocessorPlainTextWord*>* sentenceContents, vector<uint64_t>* POSambiguityInfoPermutationTemp);
	#endif
};

#endif

