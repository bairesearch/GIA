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
 * File Name: GIApreprocessor.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3c5c 21-June-2017
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#include "GIApreprocessor.hpp"
#include "GIAtranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

#ifdef GIA_PREPROCESSOR
bool GIApreprocessorClass::preprocessTextForGIAwrapper(const bool useLRP, string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, bool* useInputTextPlainTXTFile)
{
	bool result = true;
	
	if(translatorVariables->firstGIApreprocessorSentenceInList != NULL)
	{
		*useInputTextPlainTXTFile = true;
	}
	
	if(useLRP)
	{
		if(!preprocessTextForGIA(inputTextPlainTXTfileName, outputLRPTextPlainTXTFileName, isQuery, translatorVariables))
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

bool GIApreprocessorClass::preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	
	string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FILE_EXTENSION;	
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	#else
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = inputTextPlainTXTfileName;
	#endif
	string outputLRPTextPlainTXTFileNameIntermediarySentence = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_INTERMEDIARY_SENTENCE_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_SENTENCE_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence;
	#else
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	string outputLRPTextPlainTXTFileNameIntermediaryMultiword = outputLRPTextPlainTXTFileName;	
	string outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword = outputLRPTextForNLPonlyPlainTXTFileNameBase + GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION;
	string outputLRPTextForNLPonlyPlainTXTFileName = outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword;
	#else
	string outputLRPTextForNLPonlyPlainTXTFileName = string outputLRPTextForNLPonlyPlainTXTFileNameBase;
	#endif
	#endif

	if(!createPreprocessSentencesForGIA(*inputTextPlainTXTfileName, translatorVariables))
	{
		result = false;
	}
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	GIApreprocessorMultiwordReduction.initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);
	GIApreprocessorMultiwordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	//required for local variable access
	if(!GIApreprocessorMultiwordReduction.parseTextFileAndReduceLanguage(translatorVariables->firstGIApreprocessorSentenceInList, outputLRPTextPlainTXTFileNameIntermediaryMultiword, outputLRPTextForNLPonlyPlainTXTFileNameIntermediaryMultiword))
	{
		result = false;
	}
	#endif
	
	#ifdef GIA_PREPROCESSOR_DEBUG
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		cout << "currentGIApreprocessorSentenceInList->sentenceContentsLRPfirstWord = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP), false) << endl;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	if(!preprocessSentencesForGIA(translatorVariables->firstGIApreprocessorSentenceInList, outputLRPTextPlainTXTFileNameIntermediarySentence, outputLRPTextForNLPonlyPlainTXTFileNameIntermediarySentence))
	{
		result = false;
	}
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	if(!updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(translatorVariables->firstGIApreprocessorSentenceInList, isQuery))
	{
		result = false;
	}			
	#endif	
	#endif
	
	*inputTextPlainTXTfileName = outputLRPTextForNLPonlyPlainTXTFileName;	//now perform NLP using NLP specific (dummy) version of LRP output	
	SHAREDvars.setCurrentDirectory(outputFolder);	//this is required for NLP to parse preprocessed text from outputfolder	//3c2b

	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_PREMATURE_QUIT
	cerr << "Premature quit for debug" << endl;
	exit(EXIT_ERROR);
	#endif
	
	
	return result;
}

			
bool GIApreprocessorClass::regenerateFileFromPreprocessedTextWithoutLRP(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//write text file for nlp input based on firstGIApreprocessorSentenceInList data

	string outputLRPTextForNLPonlyPlainTXTFileContents = "";
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		outputLRPTextForNLPonlyPlainTXTFileContents = outputLRPTextForNLPonlyPlainTXTFileContents + GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsOriginal)) + CHAR_NEWLINE;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	//sync generated filename with preprocessTextForGIA;
	string outputLRPTextForNLPonlyPlainTXTFileNameBase = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_OUTPUT_FILE_EXTENSION;
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
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
			if(!GIApreprocessorMultiwordReductionClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
			{
			#endif
				punctuationMarkFound = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
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
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
		if(GIApreprocessorMultiwordReductionClassObject.isApostrophePossessionOrOmission(charCount, &fileContents))
		{
			apostrophePossessionOrOmissionFound = true;
			punctuationMarkFound = false;
		}
		#endif
		
		bool dollarFound = false;
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_DOLLARS
		if(currentToken == CHAR_DOLLAR)
		{
			dollarFound = true;
		}
		#endif
		
		bool quotationMarkFound = false;
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
		if(!apostrophePossessionOrOmissionFound)
		{
		#endif
			if(SHAREDvars.charInCharArray(currentToken, nlpQuotationMarkCharacterArray, GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS))
			{
				quotationMarkFound = true;
			}
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION	
		}
		#endif
		
		bool mathFound = false;
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
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
		
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
		if(quotationMarkFound)
		{//NB imbedded/recursive quotation marks not currently supported eg "'hello'"
			if(readingQuotation)
			{
				readingQuotation = false;
				currentWord = currentWord + currentToken;	//add quotation mark to word
				preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, charCount);
			}
			else
			{
				readingQuotation = true;
				if(currentWord != "")
				{
					cerr << "GIApreprocessorClass::createPreprocessSentences{}: GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS error: quotationMarkFound && !readingQuotation && (currentWord != "")" << endl;
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
					preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
					currentWord = GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_ANOMALY_INTERPRET_CANNOT_AS_CAN_NOT_NEW_WORD_2;
				}
				#endif

				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
				if(mathFound && previousCharacterIsMathGrouped)
				{
					currentWord = currentWord + currentToken;
					lastCharacterIndexOfLastWordBeingFilled = charCount;
				}
				#endif
				 
				if(currentWord != "")
				{//do not add empty tag after closing quotation marks	//e.g. GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS or (newlineFound && interpretNewLinesAsNewSentences && previousCharacter==whitespace)
					lastWordBlank = false;
					preprocessorFillCurrentWord(&currentWordInSentence, &currentWord, &entityIndex, lastCharacterIndexOfLastWordBeingFilled);
				}

				if(quotationMarkFound)
				{
					string quotationMark = ""; 
					quotationMark = quotationMark + currentToken;
					preprocessorFillCurrentWord(&currentWordInSentence, &quotationMark, &entityIndex, charCount);
				}
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
				else if(mathFound && !previousCharacterIsMathGrouped)
				{
					string mathMark = ""; 
					mathMark = mathMark + currentToken;
					preprocessorFillCurrentWord(&currentWordInSentence, &mathMark, &entityIndex, charCount);
				}
				#endif
				else if(punctuationMarkFound)
				{
					string punctuationMark = ""; 
					punctuationMark = punctuationMark + currentToken;
					preprocessorFillCurrentWord(&currentWordInSentence, &punctuationMark, &entityIndex, charCount);

					if(SHAREDvars.charInCharArray(currentToken, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
					{
						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
						if(!GIApreprocessorMultiwordReductionClassObject.isIntrawordPunctuationMark(charCount, &fileContents))
						{
						#endif
							endOfSentencePunctuationMarkFound = true;
						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
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
						GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
						currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
						currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
						currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
						currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
						firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
						currentWordInSentence = firstWordInSentence;
						sentenceContentsOriginalText = "";
						entityIndex = 0;
						sentenceIndex++;
					}
				}
				else if(newlineFound && interpretNewLinesAsNewSentences)
				{	
					if(!(splitMultisentenceLines && endOfSentencePunctuationMarkFoundPreviously))
					{				
						GIApreprocessorMultiwordReductionClassObject.generateSentenceWordList(firstWordInSentence, &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal));
						currentGIApreprocessorSentenceInList->sentenceIndexOriginal = sentenceIndex;
						currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = sentenceContentsOriginalText;
						currentGIApreprocessorSentenceInList->next = new GIApreprocessorSentence();
						currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
						firstWordInSentence = new GIApreprocessorMultiwordReductionPlainTextWord();
						currentWordInSentence = firstWordInSentence;
						sentenceContentsOriginalText = "";
						entityIndex = 0;
						sentenceIndex++;
					}

					#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
					string indentationContents = "";
					int currentIndentation = 0;
					extractIndentationFromCurrentLine(&fileContents, &charCount, &(currentGIApreprocessorSentenceInList->indentation), &indentationContents);
					//cout << "GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION: indentationContents = " << indentationContents << endl;
					sentenceContentsOriginalText = sentenceContentsOriginalText + indentationContents;
					#endif
				}
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_DOLLARS
				else if(dollarFound)
				{
					if(lastWordBlank)
					{
						string dollarMark = ""; 
						dollarMark = dollarMark + currentToken;
						preprocessorFillCurrentWord(&currentWordInSentence, &dollarMark, &entityIndex, charCount);
					}
					else
					{
						cerr << "GIApreprocessorClass::createPreprocessSentences{}: error: dollarFound && !lastWordBlank" << endl;	//text cannot be followed by $ sign; e.g. 10$
						exit(EXIT_ERROR);
					}
				}
				#endif
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
				else if(apostrophePossessionOrOmissionFound)
				{
					currentWord = currentWord + currentToken;
				}
				#endif
				
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
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

		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
		}
		#endif
		charCount++;
	}
		
	return result;
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
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


void GIApreprocessorClass::preprocessorFillCurrentWord(GIApreprocessorMultiwordReductionPlainTextWord** currentWordInSentence, string* currentWord, int* entityIndex, int lastCharacterIndexOfWordInSentence)
{
	(*currentWordInSentence)->tagName = *currentWord;
	(*currentWordInSentence)->entityIndex = *entityIndex;
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	(*currentWordInSentence)->characterIndexInSentenceContentsOriginalText = lastCharacterIndexOfWordInSentence - currentWord->size() - 1;
	#endif
	(*currentWordInSentence)->nextTag = new GIApreprocessorMultiwordReductionPlainTextWord();
	(*currentWordInSentence) = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>((*currentWordInSentence)->nextTag);
	(*entityIndex) = (*entityIndex) + 1;
	*currentWord = "";
}
			
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



#ifdef GIA_PREPROCESSOR_SENTENCE
					
bool GIApreprocessorClass::preprocessSentencesForGIA(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string outputFileName, const string outputFileNameLRPforNLP)
{
	bool result = true;
	
	XMLparserTag* firstLogicReferenceClassTag = NULL;
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	vector<string> logicReferenceInstanceList;
	if(!GIApreprocessorLogicReferenceObject.extractGIApreprocessorLogicReferenceClasses(&firstLogicReferenceClassTag, &logicReferenceInstanceList))
	{
		result = true;
	}
	#endif
			
	int charCount = 0;
	char currentToken;
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;
	string outputTextContents = "";
	string outputTextContentsForNLP = "";
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		//string sentenceContentsOriginalText = this->removePrependingWhiteSpace(sentenceContents);	//only for NLC?
		if(!generateGIApreprocessorSentence(currentGIApreprocessorSentenceInList, firstLogicReferenceClassTag))
		{
			result = false;
		}

		string outputTextContentsSentence = "";
		string outputTextContentsForNLPsentence = "";
		#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
		currentGIApreprocessorSentenceInList->sentenceIndex = sentenceIndex;
		outputTextContents = outputTextContents + GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_SENTENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
		outputTextContentsForNLP = outputTextContentsForNLP + GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_SENTENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
		sentenceIndex++;
		#endif
		if(!generatePreprocessorSentenceNLPparsablePhrases(currentGIApreprocessorSentenceInList->firstLogicReferenceInList, &sentenceIndex, &outputTextContentsSentence, &outputTextContentsForNLPsentence))
		{
			result = false;
		}

		outputTextContents = outputTextContents + outputTextContentsSentence;
		outputTextContentsForNLP = outputTextContentsForNLP + outputTextContentsForNLPsentence;
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}

	SHAREDvars.setCurrentDirectory(outputFolder);	//save output files to output folder
	SHAREDvars.writeStringToFile(outputFileName, &outputTextContents);
	SHAREDvars.writeStringToFile(outputFileNameLRPforNLP, &outputTextContentsForNLP);
	SHAREDvars.setCurrentDirectory(inputFolder);	//set current directory back to the original inputFolder (this is required for both NLC_RECONCILE_CLASS_DEFINITION_LIST_FUNCTION_DECLARATION_ARGUMENTS and GIA, even if the GIA's inputFolder is changed to outputFolder as it should be in the case of NLC preprocessed input)


	#ifdef GIA_PREPROCESSOR_SENTENCE_PRINT_OUTPUT
	cout << "GIA_PREPROCESSOR_SENTENCE_PRINT_OUTPUT: " << endl;
	cout << "create preprocessed file, outputFileName = " << outputFileName << endl;
	cout << "outputTextContents = \n" << outputTextContents << endl;
	cout << "outputTextContentsForNLP = \n" << outputTextContentsForNLP << endl;
	cout << endl;
	currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		cout << "currentGIApreprocessorSentenceInList->sentenceIndex = " << currentGIApreprocessorSentenceInList->sentenceIndex << ", sentenceIndexOriginal: " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << endl;
		GIApreprocessorLogicReferenceObject.printLogicReferenceLayer(currentGIApreprocessorSentenceInList->firstLogicReferenceInList);
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	cout << endl;
	#endif
	
	return result;
}


bool GIApreprocessorClass::generateGIApreprocessorSentence(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, XMLparserTag* firstLogicReferenceClassTag)
{
	bool result = true;
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	vector<GIApreprocessorWord*>* sentenceContentsWordList = &(currentGIApreprocessorSentenceInList->sentenceContentsLRP);
	#else
	vector<GIApreprocessorWord*>* sentenceContentsWordList = &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal);
	#endif
	int sentenceIndexOriginal = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;

	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	if(!GIApreprocessorLogicReferenceObject.executeLogicReferencePreprocessor(sentenceContentsWordList, currentGIApreprocessorSentenceInList, firstLogicReferenceClassTag))
	{
		result = false;
	}
	#else
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	GIApreprocessorLogicReference* firstLogicReferenceInList = currentGIApreprocessorSentenceInList->firstLogicReferenceInList;
	if(!GIApreprocessorReferenceSet.executeReferenceSetPreprocessor(sentenceContentsFirstWord, &sentenceContentsWordList, firstLogicReferenceInList->logicReferenceVariable, true, GIA_NLP_START_ENTITY_INDEX))
	{
		result = false;
	}
	firstLogicReferenceInList->next = new GIApreprocessorLogicReference();	//required for generatePreprocessorSentenceNLPparsablePhrases
	#endif
	#endif
	
	return result;
}


string GIApreprocessorClass::removePrependingWhiteSpace(string sentenceContents)
{
	//this function is used in case the current sentence occurs after a previous sentence on the same line (ie after ". ")
	if(sentenceContents.length() > 0)
	{
		if(sentenceContents[0] == CHAR_SPACE)
		{
			sentenceContents = sentenceContents.substr(1, sentenceContents.length()-1);
		}
	}

	return sentenceContents;
}

	
//NB sentenceContentsWithLogicReferenceVariableNames, eg "if A then B"
bool GIApreprocessorClass::generatePreprocessorSentenceNLPparsablePhrases(GIApreprocessorLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP)
{
	bool result = true;
	
	/*OLD generated format:
	eg "Tom [A] said that Mary [B] said that I like custard tarts that are yellow"
	=>
	"
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
	B said that C
	A said that B
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
	I 
	custard tarts
	yellow	
	#else
	I 
	custard tarts that are yellow
	#endif
	#else
	I like custard tarts that are yellow
	#endif
	#endif
	"
	*/
			
	GIApreprocessorLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			string sentenceContentsPreprocessedLogicReference = "";
			string sentenceContentsPreprocessedLogicReferenceForNLP = "";
			generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(&sentenceContentsPreprocessedLogicReference, &sentenceContentsPreprocessedLogicReferenceForNLP, currentLogicReferenceInList, sentenceIndex);
			*outputTextContentsSentence = *outputTextContentsSentence + sentenceContentsPreprocessedLogicReference;
			*outputTextContentsSentenceForNLP = *outputTextContentsSentenceForNLP + sentenceContentsPreprocessedLogicReferenceForNLP;
			#endif
	
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(!generatePreprocessorSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListArray, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
				{
					result = false;
				}
			}
			else
			{
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!generatePreprocessorSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
					{
						result = false;
					}
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(!generatePreprocessorSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListDependent, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
				{
					result = false;
				}
			}
		}
		else
		{
		#endif	
			string sentenceContentsPreprocessedLogicReferenceVariables = "";
			string sentenceContentsPreprocessedLogicReferenceVariablesForNLP = "";
	
			#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
			currentLogicReferenceInList->logicReferenceVariable->sentenceIndex = *sentenceIndex;
			sentenceContentsPreprocessedLogicReferenceVariables = sentenceContentsPreprocessedLogicReferenceVariables + GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_LOGIC_REFERENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
			sentenceContentsPreprocessedLogicReferenceVariablesForNLP = sentenceContentsPreprocessedLogicReferenceVariablesForNLP + GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_LOGIC_REFERENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
			*sentenceIndex = *sentenceIndex + 1;
			#endif

			//cout << "Q sentenceIndex = " << *sentenceIndex << endl;
			
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
			/*//redundant (equivalent code)
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
			*/
			GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT);				
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			if(!(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents).empty())
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER);
			}
			#endif
			#else
			sentenceContentsPreprocessedLogicReferenceVariables = sentenceContentsPreprocessedLogicReferenceVariables + currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents + CHAR_NEWLINE;
			sentenceContentsPreprocessedLogicReferenceVariablesForNLP = sentenceContentsPreprocessedLogicReferenceVariablesForNLP + currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents + CHAR_NEWLINE;
			#endif
			
			//cout << "Q sentenceContentsPreprocessedLogicReferenceVariables = " << sentenceContentsPreprocessedLogicReferenceVariables << endl;
			//cout << "Q sentenceContentsPreprocessedLogicReferenceVariablesForNLP = " << sentenceContentsPreprocessedLogicReferenceVariablesForNLP << endl;
			
			*outputTextContentsSentence = *outputTextContentsSentence + sentenceContentsPreprocessedLogicReferenceVariables;
			*outputTextContentsSentenceForNLP = *outputTextContentsSentenceForNLP + sentenceContentsPreprocessedLogicReferenceVariablesForNLP;
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		}
		#endif
		
		if(currentLogicReferenceInList->next != NULL)
		{
			currentLogicReferenceInList = currentLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	
	return result;
}


#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
void GIApreprocessorClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, string* sentenceContentsPreprocessedLogicReferenceForNLP, GIApreprocessorLogicReference* currentLogicReferenceInList, int* sentenceIndex)
{
	if(currentLogicReferenceInList->hasSubLogicReference)
	{
		string logicReferenceSetContentsWithVariableNames = "";
		if(currentLogicReferenceInList->hasSubLogicReferenceArray)
		{
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			generateLogicReferenceSetContentsWithVariableNamesConjunction(&logicReferenceSetContentsWithVariableNames, currentLogicReferenceInList, sentenceIndex);
			#endif
		}
		else
		{
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB)
			{			
			#endif
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->isReferenceSetDelimiter = true;
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = currentLogicReferenceInList->logicReferenceContents;	//eg "had proposed"
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(sentenceContentsPreprocessedLogicReference, sentenceContentsPreprocessedLogicReferenceForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER);
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			}
			#endif
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			GIApreprocessorWord* firstSubLogicReferenceInListGovernorLogicReferenceVariableName = new GIApreprocessorWord(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->logicReferenceVariable->logicReferenceVariableName);
			GIApreprocessorWord* firstSubLogicReferenceInListDependentLogicReferenceVariableName = new GIApreprocessorWord(currentLogicReferenceInList->firstSubLogicReferenceInListDependent->logicReferenceVariable->logicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject->subReferenceSetContents).push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject->subReferenceSetContents).push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);
			//for debug only:
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			logicReferenceSetContentsWithVariableNames.push_back(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents);
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);	
			#endif
		}
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
		*sentenceContentsPreprocessedLogicReference = *sentenceContentsPreprocessedLogicReference + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceForNLP = *sentenceContentsPreprocessedLogicReferenceForNLP + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNames = logicReferenceSetContentsWithVariableNames;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNamesSentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
		#endif
	}
	else
	{
		cerr << "GIApreprocessorClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference{} error: !(currentLogicReferenceInList->hasSubLogicReference)" << endl;
		exit(EXIT_ERROR);
	}
}

void GIApreprocessorClass::generateLogicReferenceSetContentsWithVariableNamesConjunction(string* logicReferenceSetContentsWithVariableNames, GIApreprocessorLogicReference* conjunctionLogicReference, int* sentenceIndex)
{
	GIApreprocessorLogicReference* currentLogicReferenceInList = conjunctionLogicReference->firstSubLogicReferenceInListArray;
	bool stillParsingLogicReferenceLayer = true;
	while(currentLogicReferenceInList->next != NULL)
	{
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			*logicReferenceSetContentsWithVariableNames = *logicReferenceSetContentsWithVariableNames + currentLogicReferenceInList->logicReferenceClassType;	//CHECKTHIS	//eg proposed		
		}
		else
		{
			*logicReferenceSetContentsWithVariableNames = *logicReferenceSetContentsWithVariableNames + currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName;	//eg A
		}
		if(currentLogicReferenceInList->next != NULL)
		{
			*logicReferenceSetContentsWithVariableNames = *logicReferenceSetContentsWithVariableNames + conjunctionLogicReference->logicReferenceClassType;	//eg and
		}
		currentLogicReferenceInList = currentLogicReferenceInList->next;
	}
}
#endif


void GIApreprocessorClass::addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, int referenceSetType)
{
	#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(!(referenceSet->isReferenceSetDelimiter))
	{
	#endif
		//cout << "add = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) << endl;
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
		(referenceSet->subReferenceSetContentsOutputForNLP).clear();	//probably not required
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		if(referenceSet->isReferenceSetDelimiter)
		{
			GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
			GIApreprocessorMultiwordReductionClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
			GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
			GIApreprocessorMultiwordReductionClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
			#endif
			referenceSet->dummyNLPtestOffset = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE;
			referenceSet->dummyNLPtestOffset = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE;
		} 
		else
		{
		#endif
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER
			if(referenceSetType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT)
			{
			#endif
				GIApreprocessorMultiwordReductionClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
				GIApreprocessorMultiwordReductionClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = 0;
				referenceSet->dummyNLPtestOffset = 0;
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER
			}
			else if(referenceSetType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT)
			{
				GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
				GIApreprocessorMultiwordReductionClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				GIApreprocessorMultiwordReductionClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				GIApreprocessorMultiwordReductionClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE - GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE;
				referenceSet->dummyNLPtestOffset = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE - GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE;
			} 
			#endif
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		}
		#endif
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) + CHAR_NEWLINE;
		#else
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents), true) + CHAR_NEWLINE;
		#endif
		
		referenceSet->sentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
	#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
}





//preconditions: all multiwords in GIArules.xml <preprocessor><logicReference><class name="preposition"> must be in LRPdata WikipediaEnglishClubMultiwordPrepositions.txt
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
bool GIApreprocessorClass::updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const bool isQuery)
{
	bool result = true;

	GIApreprocessorMultiwordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = GIApreprocessorMultiwordReduction.getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next != NULL)
	{
		int sentenceIndexOriginal = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex;
		int entityIndexOriginal = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex;
		
		GIApreprocessorSentence* currentGIApreprocessorSentenceInList = NULL;
		if(getGIApreprocessorSentence(firstGIApreprocessorSentenceInList, currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex, &currentGIApreprocessorSentenceInList))
		{
			//calling this multiple times is somewhat inefficient
			GIApreprocessorSubReferenceSet* GIApreprocessorSubReferenceSetFound = NULL;
			if(getGIApreprocessorReferenceSet(currentGIApreprocessorSentenceInList->firstLogicReferenceInList, entityIndexOriginal, &GIApreprocessorSubReferenceSetFound))
			{
				currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = GIApreprocessorSubReferenceSetFound->sentenceIndex;
				currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = entityIndexOriginal - (GIApreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText - GIA_NLP_START_ENTITY_INDEX) + GIApreprocessorSubReferenceSetFound->dummyNLPtestOffset;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "entityIndexOriginal = " << entityIndexOriginal << endl;
				cout << "sentenceIndexOriginal = " << sentenceIndexOriginal << endl;
				cout << "currentGIApreprocessorSentenceInList->sentenceContentsOriginalText = " << currentGIApreprocessorSentenceInList->sentenceContentsOriginalText << endl;
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex << endl;
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex << endl;
				cout << "GIApreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText = " << GIApreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText << endl;
				#endif
			}
			else
			{
				cerr << "GIApreprocessor::updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo{} error: !getGIApreprocessorReferenceSet" << endl;
				exit(EXIT_ERROR);	
			}
		}
		else
		{
			cerr << "GIApreprocessor::updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo{} error: !getGIApreprocessorSentenceInList" << endl;
			exit(EXIT_ERROR);	
		}
		
		currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next;
	}
	
	return result;
}	


		
	
bool GIApreprocessorClass::getGIApreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const int sentenceIndexOriginal, GIApreprocessorSentence** GIApreprocessorSentenceFound)
{
	bool result = false;
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		if(currentGIApreprocessorSentenceInList->sentenceIndexOriginal == sentenceIndexOriginal)
		{
			*GIApreprocessorSentenceFound = currentGIApreprocessorSentenceInList;
			result = true;
		}
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	return result;
}

bool GIApreprocessorClass::getGIApreprocessorReferenceSet(GIApreprocessorLogicReference* firstGIApreprocessorLogicReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound)
{
	//cout << "1 getGIApreprocessorReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;

	bool result = false;
	GIApreprocessorLogicReference* currentGIApreprocessorLogicReferenceInList = firstGIApreprocessorLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		if(currentGIApreprocessorLogicReferenceInList->hasSubLogicReference)
		{
			if(currentGIApreprocessorLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->firstSubLogicReferenceInListArray, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
			else
			{
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentGIApreprocessorLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->firstSubLogicReferenceInListGovernor, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->firstSubLogicReferenceInListDependent, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
		}
		else
		{
		#endif
			if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->logicReferenceVariable->referenceSetSubject, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}
 			if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->logicReferenceVariable->referenceSetObject, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}		
			if(getGIApreprocessorReferenceSet(currentGIApreprocessorLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE			
		}
		#endif
		
		if(currentGIApreprocessorLogicReferenceInList->next != NULL)
		{
			currentGIApreprocessorLogicReferenceInList = currentGIApreprocessorLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	return result;
}

bool GIApreprocessorClass::getGIApreprocessorReferenceSet(GIApreprocessorSubReferenceSet* firstGIApreprocessorSubReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound)
{
	bool result = false;
	//cout << "2 getGIApreprocessorReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;
	GIApreprocessorSubReferenceSet* currentGIApreprocessorSubReferenceInList = firstGIApreprocessorSubReferenceInList;
	while(currentGIApreprocessorSubReferenceInList->next != NULL)
	{
		//cout << "\t getGIApreprocessorReferenceSet: currentGIApreprocessorSubReferenceInList->firstIndexOfReferenceSetText = " << currentGIApreprocessorSubReferenceInList->firstIndexOfReferenceSetText << endl;
		//cout << "\t getGIApreprocessorReferenceSet: currentGIApreprocessorSubReferenceInList->lastIndexOfReferenceSetText = " << currentGIApreprocessorSubReferenceInList->lastIndexOfReferenceSetText << endl;

		if((entityIndexOriginal >= currentGIApreprocessorSubReferenceInList->firstIndexOfReferenceSetText) && (entityIndexOriginal < currentGIApreprocessorSubReferenceInList->lastIndexOfReferenceSetText))
		{
			result = true;
			*GIApreprocessorSubReferenceSetFound = currentGIApreprocessorSubReferenceInList;
		}
		
		currentGIApreprocessorSubReferenceInList = currentGIApreprocessorSubReferenceInList->next;
	}
	return result;
}

#endif

#endif	




#ifdef GIA_PREPROCESSOR_SENTENCE

bool GIApreprocessorClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentGIApreprocessorSentenceInList, currentGIApreprocessorSentenceInList->firstLogicReferenceInList, translatorVariables, NULL);
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	return result;
}

bool GIApreprocessorClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity)
{	
	bool result = true;

	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	GIApreprocessorLogicReference* firstLogicReferenceInListInUpper = NULL;
	if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		GIApreprocessorLogicReference* currentLogicReferenceInListInUpper = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;
		while(currentLogicReferenceInListInUpper->previous != NULL)
		{
			currentLogicReferenceInListInUpper = currentLogicReferenceInListInUpper->previous;
		}
		firstLogicReferenceInListInUpper = currentLogicReferenceInListInUpper;
	}
	#endif
	
	
	GIApreprocessorLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{			
		#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
		translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->sentenceIndex; //this sentenceIndex will be replaced in the future
		#else
		#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
		translatorVariables->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndex;
		#else
		//no new entities or connections will be created
		#endif
		#endif
		
	
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		bool sameReferenceSetLogicReference = true;	//CHECKTHIS
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			GIAentityNode* logicReferenceEntity = NULL;
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				logicReferenceEntity = createNewRelationshipEntity(&(currentLogicReferenceInList->logicReferenceContents), GIA_ENTITY_TYPE_LOGIC_REFERENCE, translatorVariables);	//generate artificial conjunction entity
			}
			else
			{
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
				if(currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB)
				{			
				#endif
					int sentenceIndexRecord = translatorVariables->sentenceIndex;
					translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex;	
					if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER, &logicReferenceEntity, NULL))
					{
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
						exit(EXIT_ERROR);				
					}
					#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE			
					changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);	
					#endif
					translatorVariables->sentenceIndex = sentenceIndexRecord;
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
				}
				else
				{
					logicReferenceEntity = createNewRelationshipEntity(&(currentLogicReferenceInList->logicReferenceContents), GIA_ENTITY_TYPE_LOGIC_REFERENCE, translatorVariables);
				}			
				#endif
			}
			#else
			logicReferenceEntity = createNewRelationshipEntity(&(currentLogicReferenceInList->logicReferenceContents), GIA_ENTITY_TYPE_LOGIC_REFERENCE, translatorVariables);	//OLD: currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			/*
			cout << "logicReferenceEntity = " << logicReferenceEntity->entityName << endl;
			cout << "currentLogicReferenceInList->logicReferenceContents = " << generateTextFromVectorWordList(currentLogicReferenceInList->logicReferenceContents) << endl;
			cout << "logicReferenceEntity->sentenceIndexTemp = " << logicReferenceEntity->sentenceIndexTemp << endl;
			*/
			#endif
			#endif
			logicReferenceEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
			logicReferenceEntity->isLogicReferenceEntity = true;
			logicReferenceEntity->logicReferenceClass = currentLogicReferenceInList->logicReferenceClass;
			logicReferenceEntity->logicReferenceClassType = currentLogicReferenceInList->logicReferenceClassType; 
			if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
			{
				if(currentLogicReferenceInList->isSubLogicReferenceGovernor)
				{
					connectRelationshipToSource(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said that A said B	[where currentLogicReferenceInList is A]
				}
				else if(currentLogicReferenceInList->isSubLogicReferenceDependent)
				{
					connectRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said that B said A	[where currentLogicReferenceInList is A]
				}
				else if(currentLogicReferenceInList->isSubLogicReferenceArray)
				{
					connectRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said A and B 	[where currentLogicReferenceInList is A]
					//CHECKTHIS; need to ensure that action relationship can have multiple objects connected
				}
				else
				{
					cerr << "GIApreprocessorClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: currentLogicReferenceInList->hasSubLogicReference && (currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL) && !(isSubLogicReferenceGovernor || isSubLogicReferenceDependent)" << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(currentLogicReferenceInList->previous != NULL)
			{
				connectRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said A and B 	[where currentLogicReferenceInList is B]	
			}


			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentGIApreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListArray, translatorVariables, logicReferenceEntity));
				{
					result = false;
				}
			}
			else
			{
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentGIApreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, translatorVariables, logicReferenceEntity));
					{
						result = false;
					}
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentGIApreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListDependent, translatorVariables, logicReferenceEntity));
				{
					result = false;
				}
			}
		}
		else
		{
		#endif
			
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
			bool subjectDefined = false;
			if(GIApreprocessorReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
			{
				subjectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "subjectDefined" << endl;
				#endif
			}
			bool objectDefined = false;
			if(GIApreprocessorReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				objectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "objectDefined" << endl;
				#endif
			}
			bool delimiterDefined = false;
			if(GIApreprocessorReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
			{
				delimiterDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "delimiterDefined" << endl;
				#endif
			}


			GIAentityNode* subjectEntity = NULL;
			GIAentityNode* objectEntity = NULL;
			GIAentityNode* delimiterEntity = NULL;
			GIAentityNode* referenceSetActionEntity = NULL;
			if(subjectDefined)
			{
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&subjectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, NULL, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT))
				{
					result = false;
				}
			}
			if(delimiterDefined)
			{
	
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntity, NULL))
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
					exit(EXIT_ERROR);				
				}
				if(!updateRelationshipType(delimiterEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, translatorVariables))
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !updateRelationshipType" << endl;
					exit(EXIT_ERROR);	
				}
				#else
				delimiterEntityTempTenseInformation = NULL;
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntityTempTenseInformation, NULL))
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
					exit(EXIT_ERROR);				
				}
				delimiterEntity = createNewRelationship(currentLogicReferenceInList->logicReferenceVariable, translatorVariables);	//OLD: this will create an intermediary action/possession("have") node if necessary
				if(!updateRelationshipTenseInformation(delimiterEntity, delimiterEntityTempTenseInformation, translatorVariables))
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !updateRelationshipTenseInformation" << endl;
					exit(EXIT_ERROR);	
				}
				#endif
				#else
				delimiterEntity = createNewRelationship(currentLogicReferenceInList->logicReferenceVariable, translatorVariables);	//OLD: this will create an intermediary action/possession("have") node if necessary
				#endif
			}
			if(objectDefined)
			{
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&objectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, delimiterEntity, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT))
				{
					result = false;
				}
			}

			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			bool conjunctionWithoutSubjectOrObjectFound = false;
			bool conjunctionFound = false;
			if(GIApreprocessorLogicReferenceObject.islogicReferenceConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
			{
				conjunctionFound = true;
			}
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
			bool conjunctionFoundEmbedded = false;
			if(GIApreprocessorLogicReferenceObject.islogicReferenceParentConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
			{
				conjunctionFoundEmbedded = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				cout << "conjunctionFoundEmbedded" << endl;
				#endif
			}
			if(conjunctionFound || conjunctionFoundEmbedded)
			#else
			if(conjunctionFound)
			#endif
			{
				bool sameReferenceSetLogicReferenceConjunction = false;	//CHECKTHIS

				//connect the object (if conjunction) to the first subject in the list
									
				GIApreprocessorLogicReference* firstImplicitConjunction = NULL;
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
				if((conjunctionFound && findFirstImplicitConjunctionLogicReferenceSetBefore(currentLogicReferenceInList, &firstImplicitConjunction)) || (conjunctionFoundEmbedded && findFirstImplicitConjunctionLogicReferenceSetBefore(currentLogicReferenceInList->lastLogicReferenceInUpperLevel, &firstImplicitConjunction)))
				#else
				if(findFirstImplicitConjunctionLogicReferenceSetBefore(currentLogicReferenceInList, &firstImplicitConjunction))
				#endif
				{
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
					cout << "findFirstImplicitConjunctionLogicReferenceSetBefore" << endl;
					//cout << "firstImplicitConjunction->logicReferenceVariable->referenceSetDelimiter->delimiterType = " << firstImplicitConjunction->logicReferenceVariable->referenceSetDelimiter->delimiterType << endl;
					#endif
					
					GIAentityNode* firstImplicitConjunctionSubject = NULL;
					GIApreprocessorLogicReferenceVariable* firstImplicitConjunctionLogicReferenceVariable = NULL;
					#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
					if(conjunctionFoundEmbedded)
					{
						if(firstLogicReferenceInListInUpper->hasSubLogicReference)
						{
							/*
							if(!delimiterDefined)
							{
								cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (firstLogicReferenceInListInUpper->hasSubLogicReference) && (!delimiterDefined)" << endl;
								exit(EXIT_ERROR);
							}
							*/
							firstImplicitConjunctionSubject = firstImplicitConjunction->firstSubLogicReferenceInListGovernor->logicReferenceVariable->referenceSetSubject->primaryEntityTemp;
						}
						else
						{
							cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !(firstLogicReferenceInListInUpper->hasSubLogicReference)" << endl;
							exit(EXIT_ERROR);
						}
					}
					else
					{
						firstImplicitConjunctionSubject = firstImplicitConjunction->logicReferenceVariable->referenceSetSubject->primaryEntityTemp;
						firstImplicitConjunctionLogicReferenceVariable = firstImplicitConjunction->logicReferenceVariable;
					}
					#else
					firstImplicitConjunctionSubject = firstImplicitConjunction->logicReferenceVariable->referenceSetSubject->primaryEntityTemp;
					firstImplicitConjunctionLogicReferenceVariable = firstImplicitConjunction->logicReferenceVariable;
					#endif
					if(firstImplicitConjunctionSubject != NULL)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						cout << "(firstImplicitConjunctionSubject != NULL)" << endl;
						#endif

						if(!subjectDefined || !objectDefined)
						{
							conjunctionWithoutSubjectOrObjectFound = true;				

							GIAentityNode* targetEntity = objectEntity;
							if(objectDefined)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								cout << "objectDefined" << endl;
								#endif

								if(!delimiterDefined)
								{
									#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
									cout << "!delimiterDefined" << endl;
									#endif

									#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
									//create a delimiter
									delimiterEntity = createNewRelationshipAndConnectToSource(firstImplicitConjunctionSubject, firstImplicitConjunctionLogicReferenceVariable, sameReferenceSetLogicReferenceConjunction, translatorVariables);	//this will create an intermediary action/possession("have") node if necessary
									//dogs have cars and [have] chickens -> connect dogs to newly created artificial [have] node
									#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
									if(conjunctionFoundEmbedded)
									{
										delimiterEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
										delimiterEntity->isLogicReferenceEntity = true;
									}
									#endif
									delimiterDefined = true;

									//connect the delimiter to object 
									connectRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReference, translatorVariables);
									#else
									cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: islogicReferenceConjunctionAndOr(currentLogicReferenceInList) && (!subjectDefined && objectDefined) && !delimiterDefined" << endl;
									exit(EXIT_ERROR);
									#endif
								}
								else
								{
									#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
									cout << "delimiterDefined" << endl;
									#endif

									//connect the delimiter to object 
									connectRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReference, translatorVariables);

									//translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject->sentenceIndex;
									connectRelationshipToSource(delimiterEntity, firstImplicitConjunctionSubject, sameReferenceSetLogicReferenceConjunction, translatorVariables);
								}
							}
							#ifndef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
							else if(subjectDefined)
							{
								if(delimiterDefined)
								{
									cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: islogicReferenceConjunctionAndOr(currentLogicReferenceInList) && (subjectDefined && !objectDefined) && delimiterDefined" << endl;
									exit(EXIT_ERROR);
								}
								else
								{
									//create a delimiter
									delimiterEntity = createNewRelationshipAndConnectToSource(firstImplicitConjunctionSubject, firstImplicitConjunctionLogicReferenceVariable, sameReferenceSetLogicReferenceConjunction, translatorVariables);	//this will create an intermediary action/possession("have") node if necessary
									//dogs have cars and [have] chickens -> connect dogs to newly created artificial [have] node
									#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
									if(conjunctionFoundEmbedded)
									{
										delimiterEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
										delimiterEntity->isLogicReferenceEntity = true;
									}
									#endif
									delimiterDefined = true;

									connectRelationshipToTarget(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceConjunction, translatorVariables);
								}
							}
							#endif
						}
						else
						{
							//conjunction logic reference already connected (connected to a unique subject)
							if(conjunctionFoundEmbedded)
							{
								connectRelationshipToSource(parentLogicReferenceEntity, firstImplicitConjunctionSubject, sameReferenceSetLogicReference, translatorVariables);
							}
						}
					}
					else
					{
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: conjunctionFound && firstImplicitConjunctionSubject == NULL" << endl;
						exit(EXIT_ERROR);	
					}
				}
				else
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: conjunctionFound && !findFirstImplicitConjunctionLogicReferenceSetBefore" << endl;
					exit(EXIT_ERROR);	
				}
			}
			if(!conjunctionWithoutSubjectOrObjectFound)
			{
			#endif
				bool sameReferenceSetLogicReferenceVariable = false;	//CHECKTHIS
				//connect the subject to the delimiter to the object
				if(subjectDefined)
				{
					if(delimiterDefined)
					{
						//translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex;
						if(objectDefined)
						{
							//eg regarding (logicReference) Tom's (subject) moving (delimiter) the pie (object)
							connectRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
							connectRelationshipToSource(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
						}	
						else
						{
							//eg regarding (logicReference) Tom's (subject) moving (delimiter)
							connectRelationshipToSource(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
						}
					}
					else
					{
						if(objectDefined)
						{
							cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: subjectDefined && !delimiterDefined && objectDefined" << endl;
							exit(EXIT_ERROR);
						}	
						else
						{
							//eg regarding (logicReference) the pie (subject)
						}	
					}
				}
				else
				{
					if(delimiterDefined)
					{
						if(objectDefined)
						{
							//eg regarding (logicReference) moving (delimiter) the pie (object)
							connectRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
						}	
						else
						{
							//eg regarding (logicReference) moving (delimiter)
							cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !subjectDefined && delimiterDefined && !objectDefined" << endl;
							exit(EXIT_ERROR);
						}
					}
					else
					{
						/*//may be a conjunction layer; this will be processed later
						//eg regarding (logicReference) the pie (object)
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !subjectDefined && !delimiterDefined && objectDefined" << endl;
						exit(EXIT_ERROR);
						*/
					}	
				}
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			}
			#endif

			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			//connect the logic reference to the delimiter or subject
			#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES_IGNORE_CONNECTIONS_TO_SENTENCE_LOGIC_REFERENCE_SET
			translatorVariables->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndex;
			#endif
			if(currentLogicReferenceInList->isSubLogicReferenceGovernor)
			{
				if(delimiterDefined)
				{
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION)
					{
						connectRelationshipToSource(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);	//, considering (action) that the pie is (connection) blue.	//eg Considering (action) that the pie is (connection) blue.
					}
					else
					{
						//eg Tom is (connection) green proposed (action)
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentGIApreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentGIApreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB) && (delimiterDefined)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					connectRelationshipToSource(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);		//eg , considering (action) the pie (subject).	//eg Considering (action) the pie (subject).	/ , considering (action) the pie (subject).	/	Tom (subject) proposed (action)...
				}
			}
			else if(currentLogicReferenceInList->isSubLogicReferenceDependent)
			{
				if(delimiterDefined)
				{	
					connectRelationshipToTarget(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);		//eg The apple is (connection) large, considering (action). / Considering (action)...,  the apple is (connction) large	/ ...proposed (action) that the berries are (connection) tasty 
				}
				else
				{
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB)
					{
						connectRelationshipToTarget(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);		//eg ...proposed (action) the plan (object)
					}
					else
					{
						//eg The apple (object), considering (action). / Considering (action)...,  the apple (object)
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentGIApreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentGIApreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION) && (delimiterDefined)" << endl;
						exit(EXIT_ERROR);
					}

				}
			}
			else if(firstLogicReferenceInList->isSubLogicReferenceArray)
			{
				if(delimiterDefined)
				{
					connectRelationshipToTarget(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);
				}
				else if(subjectDefined)
				{
					connectRelationshipToTarget(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);
				}
				else
				{
					cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentGIApreprocessorSentenceInList->isSubLogicReferenceArray) && (!delimiterDefined) && (!subjectDefined)." << endl;
					exit(EXIT_ERROR);
				}
			}
			#endif

			#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			#else
			//cout << "currentGIApreprocessorSentenceInList->sentenceIndex = " << currentGIApreprocessorSentenceInList->sentenceIndex << endl;
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			#else
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);		
			#endif
			#endif
			#endif
			#endif

			#else
			//implementation unknown
			cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: implementation currently requires GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET" << endl;
			exit(EXIT_ERROR);
			#endif
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		}
		#endif
		
		if(currentLogicReferenceInList->next != NULL)
		{
			currentLogicReferenceInList = currentLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}	
	}
	
	return result;
}

#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
void GIApreprocessorClass::changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
{
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		changeSentenceIndexOfEntityNodesAndConnections(currentSubReferenceSetInList->sentenceIndex, sentenceIndexNew, translatorVariables);		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
}

void GIApreprocessorClass::changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
{
	//FUTURE GIA: need to update changeSentenceIndexOfEntityNodesAndConnections to update translatorVariables->entityNodesActiveListSentences; position entities with right sentence index
	GIAentityNode* entityFound = NULL;
	map<int, vector<GIAentityNode*>*>::iterator entityNodesActiveListSentenceIter = translatorVariables->entityNodesActiveListSentences->find(sentenceIndexOriginal);	
	if(entityNodesActiveListSentenceIter != translatorVariables->entityNodesActiveListSentences->end())
	{
		vector<GIAentityNode*>* entityNodesActiveListSentence = entityNodesActiveListSentenceIter->second;
		for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListSentence->begin(); entityIter != entityNodesActiveListSentence->end(); entityIter++)
		{		
			GIAentityNode* entity = *entityIter;
			if(entity->sentenceIndexTemp == sentenceIndexOriginal)
			{
				entity->sentenceIndexTemp = sentenceIndexNew;
			}
			
			for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
			{	
				if(entityVectorConnectionChangeSentenceIndexOfEntityNodesAndConnectionsArray[connectionType])
				{
					for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
					{
						if((*connectionIter)->sentenceIndexTemp == sentenceIndexOriginal)
						{
							(*connectionIter)->sentenceIndexTemp = sentenceIndexNew;
							//cout << "changing connection; " << sentenceIndexNew << endl; 
						}
					}
				}
			}
		}
	}
}
#endif



void GIApreprocessorClass::connectRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	int connectionTypeTargetToRelationship = generateConnectionTypeTargetToRelationship(relationship);
	GIAtranslatorOperations.connectEntities(relationship, targetEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, connectionTypeTargetToRelationship, sameReferenceSet, translatorVariables);
}
void GIApreprocessorClass::connectRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)
{
	int connectionTypeSourceToRelationship = generateConnectionTypeSourceToRelationship(relationship);
	GIAtranslatorOperations.connectEntities(relationship, sourceEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, connectionTypeSourceToRelationship, sameReferenceSet, translatorVariables);
}

GIAentityNode* GIApreprocessorClass::createNewRelationshipEntity(vector<GIApreprocessorWord*>* logicReferenceContents, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = NULL;
	if(logicReferenceContents->size() == 1)
	{
		relationshipEntity = createNewRelationshipEntity(GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(logicReferenceContents), relationshipEntityType, translatorVariables);
	}
	else
	{
		cerr << "GIApreprocessorClass::createNewRelationshipEntity{} error: (logicReferenceContents->size() != 1)" << endl;
		exit(EXIT_ERROR);
	}

	return relationshipEntity;
}

GIAentityNode* GIApreprocessorClass::createNewRelationshipEntity(string relationshipEntityName, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	//NB see findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial{} / GIAtranslatorGeneric.cpp:genericDependecyRelationInterpretation{}:findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship{};
	//a new entity index is not required to be created because connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities is currently executed after GIAtranslator (or at least after the sentence has been parsed by GIAtranslator)
	functionEntityIndex3 = currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	currentSentenceInList->relationshipEntityArtificialIndexCurrent = param->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
	*/
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);	
	#else
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial2(relationshipEntityType, relationshipEntityName, translatorVariables);
	#endif
	
	relationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	
	return relationshipEntity;
}


GIAentityNode* GIApreprocessorClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	
{
	return createNewRelationshipAndConnectToSource(sourceEntity, logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, sameReferenceSet, translatorVariables);
}

GIAentityNode* GIApreprocessorClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	//OLD: this will create an intermediary action/possession("have") node if necessary
{
	GIAentityNode* relationshipEntity = createNewRelationship(relationshipReference, relationshipObject, translatorVariables);
	connectRelationshipToSource(relationshipEntity, sourceEntity, sameReferenceSet, translatorVariables);
	return relationshipEntity;
}


GIAentityNode* GIApreprocessorClass::createNewRelationship(GIApreprocessorLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables)
{
	return createNewRelationship(logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, translatorVariables);
}
GIAentityNode* GIApreprocessorClass::createNewRelationship(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables)
{
	//FUTURE: upgrade createNewRelationship:getRelationshipNameAndType to detect auxiliary tense information, and any associated adverb/adjectives within relationshipReference [else use GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS: this is currently required to process delimiter (eg being/having/verb/preposition) tense, adverbs (eg very near), and adjectives (eg rides fast) using NLP]

	int relationshipEntityType = GIA_ENTITY_TYPE_UNDEFINED;
	string relationshipName = "";
	getRelationshipNameAndType(relationshipReference, relationshipObject, &relationshipEntityType, &relationshipName);
	GIAentityNode* relationshipEntity = createNewRelationshipEntity(relationshipName, relationshipEntityType, translatorVariables);
	return relationshipEntity;
}

#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
bool GIApreprocessorClass::updateRelationshipType(GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	int relationshipEntityType = GIA_ENTITY_TYPE_UNDEFINED;
	string relationshipName = "";
	getRelationshipNameAndType(relationshipReference, relationshipObject, &relationshipEntityType, &relationshipName);
	relationshipEntity->entityType = relationshipEntityType;
	relationshipEntity->entityName = relationshipName;
	return result;
}
#else
bool GIApreprocessorClass::updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	relationshipEntity->grammaticalTenseTemp = delimiterEntityTempTenseInformation->grammaticalTenseTemp;
	for(int i=0; i<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; i++)
	{
		relationshipEntity->grammaticalTenseModifierArrayTemp[i] = delimiterEntityTempTenseInformation->grammaticalTenseModifierArrayTemp[i];
	}
	return result;
}
#endif
	
	
	

#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES
bool GIApreprocessorClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, GIApreprocessorLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT_COMPONENT_CONFIRMED logical conditions
	GIApreprocessorLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList;
	while(currentLogicReferenceInList->previous != NULL)
	{
		foundFirstImplicitConjunction = true;
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
		*firstImplicitConjunction = currentLogicReferenceInList;
	}
	return foundFirstImplicitConjunction;
}
#else
bool GIApreprocessorClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, GIApreprocessorLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED logical conditions
	if(conjunctionLogicReferenceInList->previous != NULL)
	{
		GIApreprocessorLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList->previous;
		bool stillParsing = true;
		while((currentLogicReferenceInList->logicReferenceClass == GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED) && stillParsing)
		{
			foundFirstImplicitConjunction = true;
			*firstImplicitConjunction = currentLogicReferenceInList;
			
			if(conjunctionLogicReferenceInList->previous != NULL)
			{
				currentLogicReferenceInList = currentLogicReferenceInList->previous;				
			}
			else
			{
				stillParsing = false;
			}
		}
	}
	return foundFirstImplicitConjunction;
}
#endif






bool GIApreprocessorClass::findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntity, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAentityNode* primaryDelimiterEntity, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType)
{
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "\n\nGIApreprocessorClass::findPrimaryEntityAndReconcileSubReferenceSets" << endl;
	#endif
	
	GIAentityNode* lastDelimiterEntity = NULL;
	
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
		cout << "currentSubReferenceSetInList->subReferenceSetContents = " << generateTextFromVectorWordList(currentSubReferenceSetInList->subReferenceSetContents) << endl;
		#endif
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		if(!(currentSubReferenceSetInList->referenceSetDelimiter))
		{
		#endif
		#endif
			GIAentityNode* intermediaryPrimaryEntity = NULL;
			if(findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentSubReferenceSetInList, translatorVariables, referenceSetType, &intermediaryPrimaryEntity, primaryDelimiterEntity))
			{
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "intermediaryPrimaryEntity = " << intermediaryPrimaryEntity->entityName << endl;
				#endif
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
				if(currentSubReferenceSetInList == firstSubReferenceSetInList)
				{
				#endif
					*primaryEntity = intermediaryPrimaryEntity;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "*primaryEntity = intermediaryPrimaryEntity = " << intermediaryPrimaryEntity->entityName << endl;
					#endif
					#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
					firstSubReferenceSetInList->primaryEntityTemp = *primaryEntity;
					#endif
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
				}
				else
				{
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
					if(currentSubReferenceSetInList->isReferenceSetDelimiter)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "(currentSubReferenceSetInList->isReferenceSetDelimiter)" << endl;
						#endif
						
						bool sameReferenceSet = true;	//"that"/"which" imply sameReferenceSet
						if(currentSubReferenceSetInList->delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)
						{
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "currentSubReferenceSetInList->delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB" << endl;
							#endif
							if(primaryDelimiterEntity != NULL)
							{
								connectRelationshipToSource(intermediaryPrimaryEntity, primaryDelimiterEntity, sameReferenceSet, translatorVariables);	//eg tom rides the bike near the ball -> rides near
							}
							else
							{
								cerr << "GIApreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: (currentSubReferenceSetInList->delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB) && (lastDelimiterEntity == NULL)" << endl;
								exit(EXIT_ERROR);
							}
						}
						else
						{
							connectRelationshipToSource(intermediaryPrimaryEntity, *primaryEntity, sameReferenceSet, translatorVariables);
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "connectRelationshipToSource" << endl;
							#endif
						}
						lastDelimiterEntity = intermediaryPrimaryEntity;
					}
					else
					{
						//connect the last delimiter connection (set its target) to the current subreference set entity
						if(lastDelimiterEntity != NULL)
						{
							bool sameReferenceSet = true;
							connectRelationshipToTarget(lastDelimiterEntity, intermediaryPrimaryEntity, sameReferenceSet, translatorVariables);
							lastDelimiterEntity = NULL;
						}
						else
						{
							cerr << "GIApreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: !(currentSubReferenceSetInList->isReferenceSetDelimiter) && (lastDelimiterEntity == NULL)" << endl;
							exit(EXIT_ERROR);
						}
					}					
					#else
					//connect the last delimiter connection (its target) to the current subreference set entity
					lastDelimiterEntity = intermediaryPrimaryEntity;
					#endif
				}
				#endif
			}
			else
			{
				cerr << "GIApreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: !findPrimaryEntityBasedOnSentenceIndexAndType" << endl;
				exit(EXIT_ERROR);
			}
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		}
		else
		{
			lastDelimiterEntity = createNewRelationshipAndConnectToSource(*primaryEntity, currentSubReferenceSetInList, currentSubReferenceSetInList->next, true);	//OLD: this will create an intermediary action/possession("have") node if necessary
		}
		#endif
		#endif
		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
}			


void GIApreprocessorClass::deleteExternalConnectionsToConnection(GIAentityConnection* connection)
{
	GIAentityNode* entityOrigin = connection->entityOrigin;
	entityOrigin->sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED;	//disable the artificial entityOrigin	//FUTURE; have to remove these from the GIAentityNodeActiveList?
	GIAentityNode* entityTarget = connection->entity;
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{	
		for(vector<GIAentityConnection*>::iterator connectionIter = entityOrigin->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityOrigin->entityVectorConnectionsArray[connectionType].end(); )
		{
			if((*connectionIter) == connection)
			{
				connectionIter = entityOrigin->entityVectorConnectionsArray[connectionType].erase(connectionIter);
			}
			else
			{
				connectionIter++;
			}
		}
		
		for(vector<GIAentityConnection*>::iterator connectionIter = entityTarget->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityTarget->entityVectorConnectionsArray[connectionType].end(); )
		{
			if((*connectionIter) == connection)
			{
				connectionIter = entityTarget->entityVectorConnectionsArray[connectionType].erase(connectionIter);
			}
			else
			{
				connectionIter++;
			}
		}
	}		
}




bool GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType, GIAentityNode** primaryEntity, GIAentityNode* primaryDelimiterEntity)
{
	bool result = true;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(currentSubReferenceSetInList->isReferenceSetDelimiter)		//REDUNDANT: || (referenceSetType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER)
	{
		//dummySubject -> relationship <- dummyObject
		
		GIAentityNode* subjectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
		{
			result = false;
			cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}

		GIAentityNode* objectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
		{
			result = false;
			cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		
		*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;	//OR	*primaryEntity = parentEntityToPrimaryEntityConnection2->entity;

		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);	
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(subjectParentEntity);	//disable if not already disabled
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
	}
	else
	{
	#endif
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER
		if(referenceSetType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT)
		{
		#endif
			//cout << "aq1" << endl;
			//subject <- dummyRelationship <- dummyObject
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			
			GIAentityNode* objectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(objectParentEntity, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(relationshipParentEntity);	//disable if not already disabled
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
			
			*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER
		}
		else if(referenceSetType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT)
		{
			//dummySubject -> dummyRelationship -> object
			
			GIAentityNode* subjectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(subjectParentEntity, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIApreprocessorClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			
			if(primaryDelimiterEntity != NULL)
			{
				if(!transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(relationshipParentEntity, primaryDelimiterEntity))
				{
					result = false;
				}
			}
			
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);	
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(subjectParentEntity);		//disable if not already disabled
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(relationshipParentEntity);	//disable if not already disabled
				
			*primaryEntity = parentEntityToPrimaryEntityConnection2->entity;
		}
		#endif
		
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
	
	return result;
}

bool GIApreprocessorClass::findParentEntityBasedOnSentenceIndex(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity)
{
	bool result = false;
	GIAentityNode* entityFound = NULL;

	bool assumeAdvancedReferencing = false;
	#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING
	if(parentEntityName != GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA)
	{
		assumeAdvancedReferencing = true;
	}
	#endif
	
	if(assumeAdvancedReferencing)
	{
		GIAentityNode* parentEntityNetworkIndex = GIAdatabase.findEntityInActiveNetworkIndexList(parentEntityName, translatorVariables->entityNodesActiveListNetworkIndexes);
		if(parentEntityNetworkIndex != NULL)
		{
			if(!(parentEntityNetworkIndex->instanceNodeList->empty()))
			{
				*parentEntity = (parentEntityNetworkIndex->instanceNodeList->back())->entity;	//assume advanced referencing has collapsed all instances of parentEntityName into one
				result = true;
			}
		}	
	}
	else
	{
		map<int, vector<GIAentityNode*>*>::iterator entityNodesActiveListSentenceIter = translatorVariables->entityNodesActiveListSentences->find(currentSubReferenceSetInList->sentenceIndex);	
		if(entityNodesActiveListSentenceIter != translatorVariables->entityNodesActiveListSentences->end())
		{		
			vector<GIAentityNode*>* entityNodesActiveListSentence = entityNodesActiveListSentenceIter->second;
			for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListSentence->begin(); entityIter != entityNodesActiveListSentence->end(); entityIter++)
			{
				GIAentityNode* entity = *entityIter;
				if(entity->entityName == parentEntityName)
				{
					*parentEntity = entity;
					result = true;
				}
			}
		}
	}

	return result;
}


bool GIApreprocessorClass::findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex)
{
	bool foundNode = false;
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES ; connectionType++)
	{
		bool passedConnectionType = false;
		if(direction)
		{
			if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION))
			{
				passedConnectionType = true;
			}
		}
		else
		{
			if((connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE) || (connectionType == GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE))
			{
				passedConnectionType = true;
			}		
		}
		if(passedConnectionType)
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
			{
				if((*connectionIter)->sentenceIndexTemp == sentenceIndex)
				{
					foundNode = true;
					*connectionFound = *connectionIter;
				}
			}
		}
	}
	return foundNode;
}

bool GIApreprocessorClass::transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(GIAentityNode* dummyDelimiterEntity, GIAentityNode* primaryDelimiterEntity)
{
	bool result = true;
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		if(entityVectorConnectionTransferFromDummyRelationshipToRealRelationshipArray[connectionType])
		{
			for(vector<GIAentityConnection*>::iterator connectionIter = dummyDelimiterEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != dummyDelimiterEntity->entityVectorConnectionsArray[connectionType].end(); )
			{
				GIAentityConnection* connection = (*connectionIter);
				GIAentityNode* relationshipEntity = connection->entity;
				connection->entityOrigin = primaryDelimiterEntity;
				if((relationshipEntity->relationshipSubjectEntity)->back() == NULL)
				{
					cerr << "transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter error{}: ((relationshipEntity->relationshipSubjectEntity)->back() == NULL)" << endl;
					exit(EXIT_ERROR);
				}
				relationshipEntity->relationshipSubjectEntity->back()->entity = primaryDelimiterEntity;
				primaryDelimiterEntity->entityVectorConnectionsArray[connectionType].push_back(connection);
				connectionIter = dummyDelimiterEntity->entityVectorConnectionsArray[connectionType].erase(connectionIter);
			}
		}
	}
	return result;
}


bool GIApreprocessorClass::findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex)
{	
	bool foundNode = false;
	for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
	{
		if((*connectionIter)->sentenceIndexTemp == sentenceIndex)
		{
			foundNode = true;
			*connectionFound = *connectionIter;
		}
	}
	return foundNode;
}

bool GIApreprocessorClass::findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex)
{	
	bool foundNode = false;
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES ; connectionType++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entityNode->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entityNode->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
		{
			if((*connectionIter)->sentenceIndexTemp == sentenceIndex)
			{
				if((*connectionIter)->entity->entityName == entityNameToFind)
				{
					foundNode = true;
					*connectionFound = *connectionIter;
				}
			}
		}
	}
	return foundNode;
}




		




int GIApreprocessorClass::generateConnectionTypeTargetToRelationship(GIAentityNode* relationship)
{
	int connectionTypeTargetToRelationship = GIAtranslatorOperations.generateConnectionTypeReverse(relationship->entityType);
	return connectionTypeTargetToRelationship;
}

int GIApreprocessorClass::generateConnectionTypeSourceToRelationship(GIAentityNode* relationship)
{
	int connectionTypeTargetToRelationship = GIAtranslatorOperations.generateConnectionType(relationship->entityType);
	return connectionTypeTargetToRelationship;
}

//FUTURE GIA - upgrade getRelationshipNameAndType to use context to determine relationship type; property, condition, action, or definition? else how to distinguish between is adjective (eg tom is red) and is definition (a tom is red)?: need to look for the determiner of the subject/object (a x is a y/xs are ys/x is a y). actions (verb detection) and properties (have auxiliary detection) are ok.
bool GIApreprocessorClass::getRelationshipNameAndType(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, int* relationshipEntityType, string* relationshipName)
{
	bool result = true; 
	
	/*
	algorithm for relationship type detection;
		if verb then action
			ignore: the move, the fast move, the very fast move
			eg tom moves the ball
			eg tom does move the ball [may be preceeded by an auxiliary]
			eg tom rides fast	[may be preceeded by an adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
			eg tom rides very fast	[may be preceeded by an adverb and adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
		else if preposition then condition
			eg tom is near the ball 	[may be preceeded by an auxiliary]
			eg tom is certainly near the ball [may be preceeded by an auxiliary and adverb]
			eg tom rides the bike near the ball 	[may not be preceeded by an auxiliary, in which case the condition refers to {ie is subsequently linked by GIA to} the preceeding referenceSetDelimiter (eg verb): delimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB]
		else if having aux then property
			eg tom has the ball
			eg tom is having the ball	[may be preceeded by a being auxiliary]
		else if doing aux then action
			eg tom is doing {FUTURE: the move}	[upgrade executeReferenceSetPreprocessor; may be preceeded by a being auxiliary {FUTURE: may be succeeded by a delimiter and verb}]
		else if being aux (is, was etc) then:
			if object is adjective then property [quality] eg Tom is red..
				if object is adverb + adjective then property [quality] eg Tom is very red
				NO: if object is quality then property [quality]
			Else is definition eg Tom is a
				Note can't just detect determiner (a) as there won't necessarily be one, eg Toms are humans / Toms are red
		NB very/certainly/not are adverbs...
		NB cold/blue/red are adjectives
	*/
	

	*relationshipName = ((relationshipReference->subReferenceSetContents).back())->tagName;
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "relationshipName = " << *relationshipName << endl;
	cout << "relationshipReference->delimiterType = " << relationshipReference->delimiterType << endl;
	#endif
	
	if(relationshipReference->delimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)	//GIApreprocessorReferenceSet.detectAuxiliary(*relationshipName)
	{
	}
	else if(relationshipReference->delimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
	}
	else if(relationshipReference->delimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_CONDITION;		
	}
	
	if(relationshipReference->delimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)
	{	
		GIApreprocessorWord* firstWordAfterAuxiliary = NULL;
		GIApreprocessorWord* secondWordAfterAuxiliary = NULL;
		bool hasFirstWordAfterAuxiliary = false;
		bool hasSecondWordAfterAuxiliary = false;
		if(relationshipObject != NULL)
		{
			if(!(relationshipObject->subReferenceSetContents).empty())
			{
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				//cout << "relationshipObject->subReferenceSetContents = " << generateTextFromVectorWordList(relationshipObject->subReferenceSetContents) << endl;
				#endif
				
				hasFirstWordAfterAuxiliary = true;
				firstWordAfterAuxiliary = ((relationshipObject->subReferenceSetContents)[0]);
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "firstWordAfterAuxiliary->tagName = " << firstWordAfterAuxiliary->tagName << endl;
				#endif					
				if((relationshipObject->subReferenceSetContents).size() >= 2)
				{
					hasSecondWordAfterAuxiliary = true;
					secondWordAfterAuxiliary = ((relationshipObject->subReferenceSetContents)[1]);
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "secondWordAfterAuxiliary->tagName = " << secondWordAfterAuxiliary->tagName << endl;
					#endif
				}
			}
		}
		else
		{
			cerr << "GIApreprocessorClass::getRelationshipNameAndType{} error: (relationshipObject == NULL)" << endl;
			exit(EXIT_ERROR);
		}
		
		if(hasFirstWordAfterAuxiliary)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			//cout << "firstWordAfterAuxiliary = " << firstWordAfterAuxiliary << endl;
			#endif
			if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryBeingArray, ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES))
			{
				if(GIApreprocessorMultiwordReduction.determineIsAdjective(firstWordAfterAuxiliary) || (hasSecondWordAfterAuxiliary && GIApreprocessorMultiwordReduction.determineIsAdverb(firstWordAfterAuxiliary) && GIApreprocessorMultiwordReduction.determineIsAdjective(secondWordAfterAuxiliary)))
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;
					//FUTURE GIA: if GIA_ENTITY_TYPE_PROPERTY detected via determineIsAdjective(firstWordAfterAuxiliary)||determineIsAdjective(secondWordAfterAuxiliary), then need to set the relationshipEntityObject to GIA_ENTITY_TYPE_QUALITY also
				}
				else
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_DEFINITION;	
				}
			}
			if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryHavingArray, ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;		
			}
			if(SHAREDvars.textInTextArray(*relationshipName, entityAuxiliaryDoingArray, ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
			}
		}
		else
		{
			cerr << "GIApreprocessorClass::getRelationshipNameAndType{} error : !hasFirstWordAfterAuxiliary" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "relationshipEntityType = " << *relationshipEntityType << endl;
	#endif
	
	if((relationshipReference->subReferenceSetContents).empty())
	{
		result = false;
	}
	
	return result;
}

#endif

#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES

bool GIApreprocessorClass::addSentenceToPreprocessorSentence(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	GIApreprocessorSubReferenceSet* subReferenceSetFound = NULL;
	if(getPreprocessorSentenceSubReferenceSet(translatorVariables->firstGIApreprocessorSentenceInList, currentSentenceInList->sentenceIndex, &subReferenceSetFound))
	{
		subReferenceSetFound->sentenceReference = currentSentenceInList;
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
		vector<GIApreprocessorWord*> preprocessorSentenceWordList = subReferenceSetFound->subReferenceSetContentsOutputForNLP;		
		#else
		vector<GIApreprocessorWord*> preprocessorSentenceWordList = subReferenceSetFound->subReferenceSetContents;		
		#endif
	#else
	GIApreprocessorSentence* preprocessorSentenceFound = NULL;
	if(getPreprocessorSentence(translatorVariables->firstGIApreprocessorSentenceInList, currentSentenceInList->sentenceIndex, &preprocessorSentenceFound))
	{
		preprocessorSentenceFound->sentenceReference = currentSentenceInList;
		vector<GIApreprocessorWord*> preprocessorSentenceWordList = preprocessorSentenceFound->sentenceContentsLRP;
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
		result = false;
	}
	
	return result;
}
	
#ifdef GIA_PREPROCESSOR_SENTENCE			
bool GIApreprocessorClass::getPreprocessorSentenceSubReferenceSet(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound)
{
	bool result = false;
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		if(getPreprocessorSentenceSubReferenceSet(currentGIApreprocessorSentenceInList->firstLogicReferenceInList, sentenceIndex, subReferenceSetFound))
		{
			result = true;
		}

		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	return result;
}
bool GIApreprocessorClass::getPreprocessorSentenceSubReferenceSet(GIApreprocessorLogicReference* firstLogicReferenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound)
{		
	bool result = false;
	
	GIApreprocessorLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(getPreprocessorSentenceSubReferenceSet(currentLogicReferenceInList->firstSubLogicReferenceInListArray, sentenceIndex, subReferenceSetFound))
				{
					result = true;
				}
			}
			else
			{
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getPreprocessorSentenceSubReferenceSet(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, subReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(getPreprocessorSentenceSubReferenceSet(currentLogicReferenceInList->firstSubLogicReferenceInListDependent, sentenceIndex, subReferenceSetFound))
				{
					result = true;
				}
			}
		}
		else
		{
		#endif	

			GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				if(currentSubReferenceSetInList->sentenceIndex == sentenceIndex)
				{
					*subReferenceSetFound = currentSubReferenceSetInList;
					result = true;
				}
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				if(currentSubReferenceSetInList->sentenceIndex == sentenceIndex)
				{
					*subReferenceSetFound = currentSubReferenceSetInList;
					result = true;
				}
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			if(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex == sentenceIndex)
			{
				*subReferenceSetFound = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter;
				result = true;
			}
			#endif

		#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		}
		#endif
		
		if(currentLogicReferenceInList->next != NULL)
		{
			currentLogicReferenceInList = currentLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	
	return result;
}
#else
bool GIApreprocessorClass::getPreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSentence** sentenceFound)
{
	bool result = false;	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
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
	
bool GIApreprocessorClass::addSentenceFeatureOutputToPreprocessorSentenceWordList(vector<GIApreprocessorWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables, bool originalNLPfeatures)
{	
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	//vector<GIApreprocessorWord*>* preprocessorSentenceWordList = translatorVariables->preprocessorSentenceWordListTemp;
	
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
	vector<GIApreprocessorWord*>::iterator preprocessorSentenceWordListIter = preprocessorSentenceWordList->begin();
	while(currentFeatureInList->next != NULL)
	{
		GIApreprocessorWord* preprocessorSentenceWord = *preprocessorSentenceWordListIter;
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
				if(NLPparsedWordOriginal == GIA_STANFORD_PARSER_AND_CORENLP_FEATURE_PARSER_INVERTED_COMMAS_WORD)
				{
					NLPparsedWordOriginal = SHAREDvars.convertCharToString(CHAR_INVERTED_COMMAS);
				}
			}
			#endif
			if(NLPparsedWordOriginal != preprocessorSentenceWord->tagName)
			{
				cerr << "GIApreprocessorClass::addSentenceFeatureOutputToPreprocessorSentenceWordList{} error: (currentFeatureInList->word != preprocessorSentenceWord->tagName)" << endl;
				cerr << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				cerr << "preprocessorSentenceWord->tagName = " << preprocessorSentenceWord->tagName << endl;
				cerr << (currentFeatureInList->word).length() << endl;
				cerr << (preprocessorSentenceWord->tagName).length() << endl;
				cerr << "preprocessorSentenceWordList = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(preprocessorSentenceWordList) << endl;
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

bool GIApreprocessorClass::addSentenceEntityOutputToPreprocessorSentenceWordList(vector<GIApreprocessorWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables)
{	
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->currentSentenceInList;
	//vector<GIApreprocessorWord*>* preprocessorSentenceWordList = translatorVariables->preprocessorSentenceWordListTemp;
	
	vector<GIApreprocessorWord*>::iterator preprocessorSentenceWordListIter = preprocessorSentenceWordList->begin();
	for(int i=0; i<preprocessorSentenceWordList->size()-1; i++)	//ignore artificial entities with entityIndex > preprocessorSentenceWordList.size() [feature list size]	//-1 to ignore the fullstop
	{
		GIApreprocessorWord* preprocessorSentenceWord = *preprocessorSentenceWordListIter;

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





