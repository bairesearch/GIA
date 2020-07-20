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
 * File Name: GIApreprocessor.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l8c 15-July-2020
 * Requirements: requires plain text file
 * Description: Preprocessor
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR
#define HEADER_GIA_PREPROCESSOR

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorWordIdentification.hpp"
#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "GIApreprocessorWordReduction.hpp" 
#endif

#include "GIAtranslatorOperations.hpp"
#include "XMLparserClass.hpp"
#ifdef GIA_PREPROCESSOR_POS_TAGGER
#include "GIApreprocessorPOStagger.hpp"
#include "GIApreprocessorPOStaggerDatabase.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "GIAposRelTranslator.hpp"
#include "GIAposRelTranslatorHybrid.hpp"
#endif



class GIApreprocessorClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: GIApreprocessorWordReductionClass GIApreprocessorWordReduction;
	#endif
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	#endif
	#ifdef GIA_PREPROCESSOR
	public: bool preprocessTextForGIAwrapper(const bool useLRP, string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, bool* useInputTextPlainTXTFile, const string inputTextNLPfeatureXMLfileName);
		public: bool preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPfeatureXMLfileName);
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
			bool preprocessSentencesForGIAwrapper(GIAtranslatorVariablesClass* translatorVariables, const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, const string outputFileName, const string outputFileNameLRPforNLP, const bool isQuery);
			#endif
		public: bool regenerateFileFromPreprocessedTextWithoutLRP(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, GIAtranslatorVariablesClass* translatorVariables);
			public: bool createPreprocessSentencesForGIA(const string inputFileName, GIAtranslatorVariablesClass* translatorVariables);
				public: bool createPreprocessSentences(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences, bool splitMultisentenceLines);
					#ifdef GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION
					private: bool extractIndentationFromCurrentLine(const string* lineContents, int* indexOfCurrentToken, int* currentIndentation, string* indentationContents);
					public: string generateIndentationContents(int currentIndentation);
					#endif
				private: void copySentenceContentsPreprocessor(GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents1FirstWord, GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents2firstWord);
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	public: bool addSentenceToPreprocessorSentence(GIAtranslatorVariablesClass* translatorVariables);
		#ifndef GIA_POS_REL_TRANSLATOR_HYBRID
		public: bool getPreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSentence** sentenceFound);
		#endif
		public: bool addSentenceFeatureOutputToPreprocessorSentenceWordList(vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables, bool originalNLPfeatures);
		public: bool addSentenceEntityOutputToPreprocessorSentenceWordList(vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables);
	#endif	
	#endif
		
};

#endif

