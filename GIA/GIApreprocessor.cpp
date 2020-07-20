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
 * File Name: GIApreprocessor.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l8c 15-July-2020
 * Requirements: requires plain text file
 * Description: Preprocessor
 * /
 *******************************************************************************/


#include "GIApreprocessor.hpp"
#include "GIAsynRelTranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

#ifdef GIA_PREPROCESSOR

	
	
bool GIApreprocessorClass::preprocessTextForGIAwrapper(const bool useLRP, string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, bool* useInputTextPlainTXTFile, const string inputTextNLPfeatureXMLfileName)
{
	bool result = true;
	
	if(translatorVariables->firstGIApreprocessorSentenceInList != NULL)
	{
		*useInputTextPlainTXTFile = true;
	}
	
	if(useLRP)
	{
		if(!preprocessTextForGIA(inputTextPlainTXTfileName, outputLRPTextPlainTXTFileName, isQuery, translatorVariables, inputTextNLPfeatureXMLfileName))
		{
			result = false;
		}
	}
	else
	{
		if(translatorVariables->firstGIApreprocessorSentenceInList != NULL)
		{
			if(!regenerateFileFromPreprocessedTextWithoutLRP(inputTextPlainTXTfileName, outputLRPTextPlainTXTFileName, translatorVariables))
			{
				result = false;
			}
		}

		#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
		if(!createPreprocessSentencesForGIA(*inputTextPlainTXTfileName, translatorVariables))
		{
			result = false;
		}
		#endif	
	}
	
	return result;
}

bool GIApreprocessorClass::preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPfeatureXMLfileName)
{
	bool result = true;
	
	
	string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FOR_NLP_ONLY_FILE_EXTENSION;	
	
	#ifdef GIA_POS_REL_TRANSLATOR
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	#else
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = *inputTextPlainTXTfileName;
	#endif
	string outputLRPTextPlainTXTFileNameIntermediarySentence = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_INTERMEDIARY_SENTENCE_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_SENTENCE_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence;
	#else
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = outputLRPTextPlainTXTFileName;	
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword;
	#else
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameBase;
	#endif
	#endif

	if(!createPreprocessSentencesForGIA(*inputTextPlainTXTfileName, translatorVariables))
	{
		result = false;
	}
	
	#ifdef GIA_PREPROCESSOR_DEBUG
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		cout << "currentGIApreprocessorSentenceInList->sentenceContentsLRPfirstWord = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentPreprocessorSentenceInList), false) << endl;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif
	
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	GIApreprocessorWordReduction.initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);
	GIApreprocessorWordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required for local variable access
	if(!GIApreprocessorWordReduction.parseTextFileAndReduceLanguage(translatorVariables->firstGIApreprocessorSentenceInList, outputLRPTextPlainTXTFileNameIntermediaryMultiword, outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword))
	{
		result = false;
	}
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	if(!preprocessSentencesForGIAwrapper(translatorVariables, outputLRPTextPlainTXTFileName, inputTextNLPfeatureXMLfileName, outputLRPTextPlainTXTFileNameIntermediarySentence, outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence, isQuery))
	{
		result = false;
	}
	#endif
	
	*inputTextPlainTXTfileName = outputLRPTextForNLPonlyPlainTXTFileName;	//now perform NLP using NLP specific (dummy) version of LRP output	
	SHAREDvars.setCurrentDirectory(outputFolder);	//this is required for NLP to parse preprocessed text from outputfolder	//3c2b

	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_PREMATURE_QUIT
	cerr << "Premature quit for debug" << endl;
	exit(EXIT_ERROR);
	#endif
	
	
	return result;
}

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
bool GIApreprocessorClass::preprocessSentencesForGIAwrapper(GIAtranslatorVariablesClass* translatorVariables, const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, const string outputFileName, const string outputFileNameLRPforNLP, const bool isQuery)
{
	bool result = true;
	
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	if(!GIAposRelTranslatorHybrid.executePrelimFeatureProcessingOnSentences(outputLRPTextPlainTXTFileName, inputTextNLPfeatureXMLfileName, translatorVariables))
	{
		result = true;
	}	
	#endif
	if(!GIAposRelTranslatorHybrid.executeTxtRelTranslatorDepreciated(translatorVariables->firstGIApreprocessorSentenceInList, outputFileName, outputFileNameLRPforNLP))
	{
		result = false;
	}
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	if(!GIAposRelTranslatorHybrid.updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(translatorVariables->firstGIApreprocessorSentenceInList, isQuery))
	{
		result = false;
	}			
	#endif
#else
	GIAposRelTranslatorRulesGroup* firstTxtRelTranslatorRulesGroupInSentence = new GIAposRelTranslatorRulesGroup();
	if(!GIAposRelTranslator.executeTxtRelTranslator(translatorVariables, &firstTxtRelTranslatorRulesGroupInSentence))
	{
		result = false;
	}
		
	//NOT YET CODED:
	if(!GIAposRelTranslatorHybrid.convertTxtRelationsToLogicReferencesAndReferenceSets(translatorVariables, firstTxtRelTranslatorRulesGroupInSentence))
	{
		result = false;
	}	
	
#endif
	
	return result;
}
#endif
	
			
bool GIApreprocessorClass::regenerateFileFromPreprocessedTextWithoutLRP(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//write text file for nlp input based on firstGIApreprocessorSentenceInList data

	string outputLRPTextForNLPonlyPlainTXTFileContents = "";
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		outputLRPTextForNLPonlyPlainTXTFileContents = outputLRPTextForNLPonlyPlainTXTFileContents + GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsOriginal)) + CHAR_NEWLINE;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	//sync generated filename with preprocessTextForGIA;
	string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FOR_NLP_ONLY_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameBase;
	
	SHAREDvars.setCurrentDirectory(outputFolder);
	SHAREDvars.writeStringToFile(outputLRPTextForNLPonlyPlainTXTFileName, &outputLRPTextForNLPonlyPlainTXTFileContents);
	SHAREDvars.setCurrentDirectory(inputFolder);
	
	*inputTextPlainTXTfileName = outputLRPTextForNLPonlyPlainTXTFileName;	
	
	return result;
}

bool GIApreprocessorClass::createPreprocessSentencesForGIA(const string inputFileName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;

	if(translatorVariables->firstGIApreprocessorSentenceInList == NULL)
	{
		translatorVariables->firstGIApreprocessorSentenceInList = new GIApreprocessorSentence();	//the GIA translator semantic parser needs to use this object to reconstruct the semantic relations from the segregated NLP parsed reference sets generated by the GIA preprocessor and subsequently processed by NLP(/GIA direct semantic relation parser)	
		string fileContents = SHAREDvars.getFileContents(inputFileName);
		bool interpretNewLinesAsNewSentences = false;
		bool splitMultisentenceLines = true;
		if(!createPreprocessSentences(fileContents, translatorVariables->firstGIApreprocessorSentenceInList, interpretNewLinesAsNewSentences, splitMultisentenceLines))
		{
			result = false;
		}
	}

	//copy sentenceContentsOriginalCurrentWord to sentenceContentsLRPCurrentWord (required by multiword preprocessor)
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		currentGIApreprocessorSentenceInList->sentenceContentsLRP = currentGIApreprocessorSentenceInList->sentenceContentsOriginal;
		
		#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
		//added 3b4b
		for(int w=0; w<currentGIApreprocessorSentenceInList->sentenceContentsLRP.size(); w++)
		{		
			GIApreprocessorMultiwordReductionPlainTextWord* sentenceContentsLRPcurrentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((currentGIApreprocessorSentenceInList->sentenceContentsLRP)[w]);
			GIApreprocessorMultiwordReductionPlainTextWord* sentenceContentsOriginalcurrentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((currentGIApreprocessorSentenceInList->sentenceContentsLRP)[w]);
			sentenceContentsLRPcurrentWord->preprocessorUpperLevelWordReference = sentenceContentsOriginalcurrentWord;
			sentenceContentsLRPcurrentWord->preprocessorUpperLevelWordReferenceSize = 1;
		}
		#endif
	
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
		
	return result;
}



bool GIApreprocessorClass::createPreprocessSentences(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences, bool splitMultisentenceLines)
{
	bool result = true;
	
	int charCount = 0;
	char currentToken;
	bool readingQuotation = false;
	string currentWord = "";
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	GIApreprocessorMultiwordReductionPlainTextWord* firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
	GIApreprocessorMultiwordReductionPlainTextWord* currentWordInSentence = firstWordInSentence;
	string sentenceContentsOriginalText = "";
	int entityIndex = GIA_NLP_START_ENTITY_INDEX;	//only assigned after collapse?
	int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;
	bool endOfSentencePunctuationMarkFoundPreviously = false;	//NB if interpretNewLinesAsNewSentences && splitMultisentenceLines, GIA preprocessor expects a new line character immediately after a punctuation mark character for it to be interpreted as the delimiter of a single sentence. i.e ". \n" will be interpreted as 2 sentences.
	bool previousCharacterIsMathGrouped = false;
	while(charCount < fileContents.length())
	{		
		currentToken = fileContents[charCount];
		sentenceContentsOriginalText = sentenceContentsOriginalText + currentToken;
		
		bool punctuationMarkFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			if(!GIApreprocessorWordClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
			{
			#endif
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			}
			#endif
		}
		
		bool whiteSpaceFound = false;
		if(SHAREDvars.charInCharArray(currentToken, nlpWhitespaceCharacterArray, GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS))	//NB this includes CHAR_NEWLINE
		{
			whiteSpaceFound = true;
		}
		
		bool newlineFound = false;
		if(currentToken == CHAR_NEWLINE)
		{
			newlineFound = true;
		}
		
		bool apostrophePossessionOrOmissionFound = false;
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
		if(GIApreprocessorWordClassObject.isApostrophePossessionOrOmission(charCount, &fileContents))
		{
			apostrophePossessionOrOmissionFound = true;
			punctuationMarkFound = false;
		}
		#endif
		
		bool dollarFound = false;
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_DOLLARS
		if(currentToken == CHAR_DOLLAR)
		{
			dollarFound = true;
		}
		#endif
		
		bool quotationMarkFound = false;
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
		if(!apostrophePossessionOrOmissionFound)
		{
		#endif
			if(SHAREDvars.charInCharArray(currentToken, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
			{
				quotationMarkFound = true;
			}
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION	
		}
		#endif
		
		bool mathFound = false;
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_MATH
		#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
		if(SHAREDvars.charInCharArray(currentToken, nlpMathCharacterGroupedArray, GIA_NLP_NUMBER_OF_MATH_CHARACTERS_GROUPED))
		{
			//group consecutive math characters into single word; e.g. && / || / == / ~~ / <= / >= etc
			if(charCount+1 < fileContents.length())
			{
				if(!SHAREDvars.charInCharArray(fileContents[charCount+1], nlpMathCharacterGroupedArray, GIA_NLP_NUMBER_OF_MATH_CHARACTERS_GROUPED))
				{
					mathFound = true;
				}
				else
				{
					previousCharacterIsMathGrouped = true;
				}
			}
			else
			{
				mathFound = true;
			}
		}
		if(SHAREDvars.charInCharArray(currentToken, nlpMathCharacterUngroupedArray, GIA_NLP_NUMBER_OF_MATH_CHARACTERS_UNGROUPED))
		{
			mathFound = true;
		}
		#else
		if(SHAREDvars.charInCharArray(currentToken, nlpMathCharacterArray, GIA_NLP_NUMBER_OF_MATH_CHARACTERS))
		{
			mathFound = true;
		}
		#endif
		#endif
		
		#ifdef GIA_PREPROCESSOR_WORD_REDUCE_QUOTES_TO_SINGLE_WORDS
		if(quotationMarkFound)
		{//NB imbedded/recursive quotation marks not currently supported eg "'hello'"
			if(readingQuotation)
			{
				readingQuotation = false;
				currentWord = currentWord + currentToken;	//add quotation mark to word
				GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, charCount);
			}
			else
			{
				readingQuotation = true;
				if(currentWord != "")
				{
					cerr << "GIApreprocessorClass::createPreprocessSentences{}: GIA_PREPROCESSOR_WORD_REDUCE_QUOTES_TO_SINGLE_WORDS error: quotationMarkFound && !readingQuotation && (currentWord != "")" << endl;
					exit(EXIT_ERROR);
				}
				currentWord = currentWord + currentToken;	//add quotation mark to word
			}
		}
		else
		{
		#endif
			bool endOfSentencePunctuationMarkFound = false;
			
			if((whiteSpaceFound || newlineFound || punctuationMarkFound || apostrophePossessionOrOmissionFound || dollarFound || quotationMarkFound || mathFound) && !readingQuotation)
			{
				bool lastWordBlank = true;
				
				int lastCharacterIndexOfLastWordBeingFilled = charCount-1; 
				
				#ifdef GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT
				if(currentWord == GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT_ORIGINAL_WORD)
				{
					//note this will break characterIndexInSentenceContentsOriginalText (required implementation: need to preprepreprocess words like "cannot" that require splitting by NLP - ie preprocess the text before executing the GIA prepreprocessor createPreprocessSentences)
					int lastCharacterIndexOfWord1BeingFilled = lastCharacterIndexOfLastWordBeingFilled - (string(GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT_NEW_WORD_2)).length();
					currentWord = GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT_NEW_WORD_1;
					GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
					currentWord = GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT_NEW_WORD_2;
				}
				#endif

				#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
				if(mathFound && previousCharacterIsMathGrouped)
				{
					currentWord = currentWord + currentToken;
					lastCharacterIndexOfLastWordBeingFilled = charCount;
				}
				#endif
				 
				if(currentWord != "")
				{//do not add empty tag after closing quotation marks	//e.g. GIA_PREPROCESSOR_WORD_REDUCE_QUOTES_TO_SINGLE_WORDS or (newlineFound && interpretNewLinesAsNewSentences && previousCharacter==whitespace)
					lastWordBlank = false;
					GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
				}

				if(quotationMarkFound)
				{
					string quotationMark = ""; 
					quotationMark = quotationMark + currentToken;
					GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &quotationMark, &entityIndex, charCount);
				}
				#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
				else if(mathFound && !previousCharacterIsMathGrouped)
				{
					string mathMark = ""; 
					mathMark = mathMark + currentToken;
					GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &mathMark, &entityIndex, charCount);
				}
				#endif
				else if(punctuationMarkFound)
				{
					string punctuationMark = ""; 
					punctuationMark = punctuationMark + currentToken;
					GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &punctuationMark, &entityIndex, charCount);

					if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
					{
						#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
						if(!GIApreprocessorWordClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
						{
						#endif
							endOfSentencePunctuationMarkFound = true;
						#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
						}
						#endif
					}
					
					bool lastCharacterInFile = false;
					#ifndef GIA_EXPECT_NEWLINE_AT_END_OF_INPUT_TEXT_FILE
					if(charCount == fileContents.length()-1)
					{
						lastCharacterInFile = true;
					}
					#endif
					if(endOfSentencePunctuationMarkFound && (!interpretNewLinesAsNewSentences || lastCharacterInFile || splitMultisentenceLines))
					{
						GIApreprocessorWordClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
						currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
						currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
						currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
						currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
						firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
						currentWordInSentence = firstWordInSentence;
						sentenceContentsOriginalText = "";
						entityIndex = GIA_NLP_START_ENTITY_INDEX;
						sentenceIndex++;
					}
				}
				else if(newlineFound && interpretNewLinesAsNewSentences)
				{	
					if(!(splitMultisentenceLines && endOfSentencePunctuationMarkFoundPreviously))
					{				
						GIApreprocessorWordClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
						currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
						currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
						currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
						currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
						firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
						currentWordInSentence = firstWordInSentence;
						sentenceContentsOriginalText = "";
						entityIndex = GIA_NLP_START_ENTITY_INDEX;
						sentenceIndex++;
					}

					#ifdef GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION
					string indentationContents = "";
					int currentIndentation = 0;
					extractIndentationFromCurrentLine(&fileContents, &charCount, &(currentGIApreprocessorSentenceInList->indentation), &indentationContents);
					//cout << "GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION: indentationContents = " << indentationContents << endl;
					sentenceContentsOriginalText = sentenceContentsOriginalText + indentationContents;
					#endif
				}
				#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_DOLLARS
				else if(dollarFound)
				{
					if(lastWordBlank)
					{
						string dollarMark = ""; 
						dollarMark = dollarMark + currentToken;
						GIApreprocessorWordClassObject.preprocessorFillCurrentWord(&currentWordInSentence, &dollarMark, &entityIndex, charCount);
					}
					else
					{
						cerr << "GIApreprocessorClass::createPreprocessSentences{}: error: dollarFound && !lastWordBlank" << endl;	//text cannot be followed by $ sign; e.g. 10$
						exit(EXIT_ERROR);
					}
				}
				#endif
				#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
				else if(apostrophePossessionOrOmissionFound)
				{
					currentWord = currentWord + currentToken;
				}
				#endif
				
				#ifdef GIA_PREPROCESSOR_WORD_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
				if(mathFound && previousCharacterIsMathGrouped)
				{
					previousCharacterIsMathGrouped = false;
				}
				#endif
				
			}
			else
			{
				currentWord = currentWord + currentToken;
			}
			
			endOfSentencePunctuationMarkFoundPreviously = endOfSentencePunctuationMarkFound;

		#ifdef GIA_PREPROCESSOR_WORD_REDUCE_QUOTES_TO_SINGLE_WORDS
		}
		#endif
		charCount++;
	}
		
	return result;
}

#ifdef GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION
bool GIApreprocessorClass::extractIndentationFromCurrentLine(const string* lineContents, int* indexOfCurrentToken, int* currentIndentation, string* indentationContents)
{
	bool result = false;
	if(*indexOfCurrentToken+1 < lineContents->length())
	{
		int i = *indexOfCurrentToken+1;
		while((i < lineContents->length()) && ((*lineContents)[i] == GIA_PREPROCESSOR_INDENTATION_CHAR))
		{
			*currentIndentation = *currentIndentation + 1;
			*indentationContents = *indentationContents + GIA_PREPROCESSOR_INDENTATION_CHAR;
			i++;
		}
		if(i > *indexOfCurrentToken+1)
		{
			*indexOfCurrentToken = *indexOfCurrentToken + *currentIndentation;
			result = true;
		}
	}
	return result;
}

string GIApreprocessorClass::generateIndentationContents(int currentIndentation)
{
	string indentationContents = "";
	for(int i=0; i<currentIndentation; i++)
	{
		indentationContents = indentationContents + GIA_PREPROCESSOR_INDENTATION_CHAR;
	}
	return indentationContents;
}
#endif



			
void GIApreprocessorClass::copySentenceContentsPreprocessor(GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents1FirstWord, GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents2firstWord)
{
	GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents1currentWord = sentenceContents1FirstWord;
	GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents2currentWord = sentenceContents2firstWord;
		
	while(sentenceContents1currentWord->nextTag != NULL)
	{		
		sentenceContents2currentWord->tagName = sentenceContents1currentWord->tagName;
		sentenceContents2currentWord->preprocessorUpperLevelWordReference = sentenceContents1currentWord->preprocessorUpperLevelWordReference;
		sentenceContents2currentWord->preprocessorUpperLevelWordReferenceSize = sentenceContents1currentWord->preprocessorUpperLevelWordReferenceSize;
		sentenceContents2currentWord->entityIndex = sentenceContents1currentWord->entityIndex;
		sentenceContents2currentWord->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();
		sentenceContents2currentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(sentenceContents2currentWord->nextTag);

		sentenceContents1currentWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(sentenceContents1currentWord->nextTag);
	}
}




#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES

bool GIApreprocessorClass::addSentenceToPreprocessorSentence(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	
	int sentenceIndex = GIAtranslatorOperations.getCurrentSentenceIndex(translatorVariables);
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	GIApreprocessorSubReferenceSet* subReferenceSetFound = NULL;
	if(GIAposRelTranslatorHybrid.getPreprocessorSentenceSubReferenceSet(translatorVariables->firstGIApreprocessorSentenceInList, sentenceIndex, &subReferenceSetFound))
	{
		subReferenceSetFound->sentenceReference = currentSentenceInList;
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
		vector<GIApreprocessorPlainTextWord*> preprocessorSentenceWordList = subReferenceSetFound->subReferenceSetContentsOutputForNLP;		
		#else
		vector<GIApreprocessorPlainTextWord*> preprocessorSentenceWordList = subReferenceSetFound->subReferenceSetContents;		
		#endif
	#else
	GIApreprocessorSentence* preprocessorSentenceFound = NULL;
	if(getPreprocessorSentence(translatorVariables->firstGIApreprocessorSentenceInList, sentenceIndex, &preprocessorSentenceFound))
	{
		preprocessorSentenceFound->sentenceReference = currentSentenceInList;
		vector<GIApreprocessorPlainTextWord*> preprocessorSentenceWordList = *GIApreprocessorSentenceClassObject.getSentenceContents(preprocessorSentenceFound);
	#endif
		//translatorVariables->preprocessorSentenceWordListTemp = preprocessorSentenceWordList;
		
		if(!addSentenceFeatureOutputToPreprocessorSentenceWordList(&preprocessorSentenceWordList, translatorVariables, true))
		{
			result = false;
		}
		if(!addSentenceFeatureOutputToPreprocessorSentenceWordList(&preprocessorSentenceWordList, translatorVariables, false))
		{
			result = false;
		}
		if(!addSentenceEntityOutputToPreprocessorSentenceWordList(&preprocessorSentenceWordList, translatorVariables))
		{
			result = false;
		}
	}
	else
	{
		cerr << "GIApreprocessorClass::addSentenceToPreprocessorSentence{} error: !getPreprocessorSentence" << endl;
		result = false;
	}
	
	return result;
}
	
#ifndef GIA_POS_REL_TRANSLATOR_HYBRID			
bool GIApreprocessorClass::getPreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSentence** sentenceFound)
{
	bool result = false;	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		//cout << "sentenceIndex = " << sentenceIndex << endl;
		//cout << "currentGIApreprocessorSentenceInList->sentenceIndexOriginal = " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << endl;
		if(currentGIApreprocessorSentenceInList->sentenceIndexOriginal == sentenceIndex)
		{
			*sentenceFound = currentGIApreprocessorSentenceInList;
			result = true;
		}		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	return result;
}
#endif
	
bool GIApreprocessorClass::addSentenceFeatureOutputToPreprocessorSentenceWordList(vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables, bool originalNLPfeatures)
{	
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	//vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList = translatorVariables->preprocessorSentenceWordListTemp;
	
	int entityIndex = GIA_NLP_START_ENTITY_INDEX;
	int w = 0;
	GIAfeature* currentFeatureInList = NULL;
	if(originalNLPfeatures)
	{
		currentFeatureInList = currentSentenceInList->firstFeatureInListOriginal;
	}
	else
	{
		currentFeatureInList = currentSentenceInList->firstFeatureInList;
	}
	vector<GIApreprocessorPlainTextWord*>::iterator preprocessorSentenceWordListIter = preprocessorSentenceWordList->begin();
	while(currentFeatureInList->next != NULL)
	{
		GIApreprocessorPlainTextWord* preprocessorSentenceWord = *preprocessorSentenceWordListIter;
		if(originalNLPfeatures)
		{
			string NLPparsedWordOriginal = currentFeatureInList->word;
			#ifdef GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_WORD_CHANGES
			if((translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP) || (translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER))
			{
				if(NLPparsedWordOriginal == GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_LEFT_BRACKET_WORD)
				{
					NLPparsedWordOriginal = SHAREDvars.convertCharToString(CHAR_OPEN_BRACKET);
				}
				if(NLPparsedWordOriginal == GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_RIGHT_BRACKET_WORD)
				{
					NLPparsedWordOriginal = SHAREDvars.convertCharToString(CHAR_CLOSE_BRACKET);
				}
				if(SHAREDvars.textInTextArray(NLPparsedWordOriginal, stanfordParserAndCoreNLPinvertedCommasWordArray, GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_INVERTED_COMMAS_WORD_ARRAY_SIZE))
				{
					NLPparsedWordOriginal = SHAREDvars.convertCharToString(CHAR_INVERTED_COMMAS);
				}
			}
			#endif
			if(NLPparsedWordOriginal != preprocessorSentenceWord->tagName)
			{
				cerr << "GIAposRelTranslatorHybridClass::addSentenceFeatureOutputToPreprocessorSentenceWordList{} error: (currentFeatureInList->word != preprocessorSentenceWord->tagName)" << endl;
				cerr << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				cerr << "preprocessorSentenceWord->tagName = " << preprocessorSentenceWord->tagName << endl;
				cerr << (currentFeatureInList->word).length() << endl;
				cerr << (preprocessorSentenceWord->tagName).length() << endl;
				cerr << "preprocessorSentenceWordList = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(preprocessorSentenceWordList) << endl;
				cerr << "originalNLPfeatures = " << originalNLPfeatures << endl;
				exit(EXIT_ERROR);
			}
			preprocessorSentenceWord->featureReferenceOriginal = currentFeatureInList;
		}
		else
		{
			if((*translatorVariables->GIAentityNodeArrayFilled)[w])
			{
				preprocessorSentenceWord->featureReference = currentFeatureInList;
			}
		}
		
		currentFeatureInList = currentFeatureInList->next;
		preprocessorSentenceWordListIter++;
		entityIndex++;
		w++;
	}
	
	return result;
}

bool GIApreprocessorClass::addSentenceEntityOutputToPreprocessorSentenceWordList(vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables)
{	
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	//vector<GIApreprocessorPlainTextWord*>* preprocessorSentenceWordList = translatorVariables->preprocessorSentenceWordListTemp;
	
	vector<GIApreprocessorPlainTextWord*>::iterator preprocessorSentenceWordListIter = preprocessorSentenceWordList->begin();
	for(int i=0; i<preprocessorSentenceWordList->size()-1; i++)	//ignore artificial entities with entityIndex > preprocessorSentenceWordList.size() [feature list size]	//-1 to ignore the fullstop
	{
		GIApreprocessorPlainTextWord* preprocessorSentenceWord = *preprocessorSentenceWordListIter;

		//cout << "preprocessorSentenceWord = " << preprocessorSentenceWord->tagName << endl;
		int w = i + GIA_NLP_START_ENTITY_INDEX;
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			//cout << "preprocessorSentenceWord->entityReference = " << (*translatorVariables->GIAentityNodeArray)[w]->entityName << endl;
			preprocessorSentenceWord->entityReference = (*translatorVariables->GIAentityNodeArray)[w];
		}
		
		preprocessorSentenceWordListIter++;
	}
		
	return result;
}

#endif

#endif





