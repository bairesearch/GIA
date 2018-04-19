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
 * Project Version: 3f3e 10-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor POS tagger
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_POS_TAGGER
#define HEADER_GIA_PREPROCESSOR_POS_TAGGER

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReductionClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"
#include "GIAtranslatorGrammar.hpp"
#include "GIApreprocessorPOStaggerDatabase.hpp"
		
		


//static int GIAPOStaggerValueCrossReferenceGrammaticalWordTypeArray[GIA_PREPROCESSOR_WORD_LIST_ARRAY_SIZE] = {GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_CONJUNCTION, GRAMMATICAL_WORD_TYPE_DETERMINER, GRAMMATICAL_WORD_TYPE_PRONOUN};

class GIApreprocessorPOStaggerClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	public: bool executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables);
		private: bool predictPOStaggerDatabaseEntry(const unsigned long centreWordPOSambiguityInfo, vector<unsigned long>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, GIApreprocessorPlainTextWord* centreWord, bool* foundMatchingCentreWordPOSambiguityInfo, unsigned char* centreWordPOSindexPrediction, bool* centreWordPOSisAmbiguous, double* experienceBackPropagationPassError, int* maximumNumberOfInstances);
	#endif
	
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
		private: bool addPOStaggerDatabaseEntry(const unsigned long centreWordPOSambiguityInfo, vector<unsigned long>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ofstream* XtrainBatchFileObject, ofstream* YtrainBatchFileObject);
		#else
		private: bool addPOStaggerDatabaseEntry(const unsigned long centreWordPOSambiguityInfo, vector<unsigned long>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience* currentExperienceInList);
		#endif
		#else
		private: bool addPOStaggerDatabaseEntry(const unsigned long centreWordPOSambiguityInfo, vector<unsigned long>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, long* numberOfNeuralNetworkFeeds);
		#endif
		private: string generateWikiDumpTextInputFileName(int wikiDumpFileBatchIndex);
		private: bool generatePreprocessorSentenceObjectsFromText(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInText, bool useLRP);
			private: bool createPreprocessSentencesBasic(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences);
	#endif
		#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK
		public: bool generateANNexperienceFromPOSambiguityInfoPermutation(vector<unsigned long>* POSambiguityInfoPermutation, const unsigned char centreWordUnambiguousPOSindex, ANNexperience* currentExperience);
		#endif
		public: int convertGIAPOStaggerValueToGrammaticalWordType(const int POSvalue);
		public: bool generatePOSambiguityInfoPermutation(vector<GIApreprocessorPlainTextWord*>* sentenceContentsLRP, int wCentre, bool* identifiedEveryWordInDatabasePOSpermutation, bool* identifiedEveryWordInDatabasePOSpermutationIsUnambiguous, vector<unsigned long>* POSambiguityInfoPermutation);

	private: string convertBoolVectorToString(vector<bool>* inputNeuronExperienceValuesContextWord);

	#endif
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	public: void generatePOSambiguityInfoUnambiguousPermutationArray(vector<vector<unsigned long>*>* POSambiguityInfoUnambiguousPermutationArray, vector<unsigned long>* POSambiguityInfoPermutation, vector<unsigned long>* POSambiguityInfoUnambiguousPermutationLocal, int wordIndex);
	public: bool determinePOSambiguityInfo(GIApreprocessorPlainTextWord* contextWord, unsigned long* contextWordPOSambiguityInfo, bool* contextWordPOSisAmbiguous, unsigned char* contextWordUnambiguousPOSindex, bool* identifiedEveryWordInDatabasePOSpermutation);
		public: bool findWordInWordListAllTypesWithPOSambiguityInfo(const string word, GIApreprocessorMultiwordReductionWord** wordFound, unsigned long* POSambiguityInfoFound);

	public: bool printPOSambiguityInfoPermutation(vector<unsigned long>* POSambiguityInfoPermutation);
	#endif

};

#endif

