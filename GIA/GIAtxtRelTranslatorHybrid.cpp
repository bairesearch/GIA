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
 * File Name: GIAtxtRelTranslatorHybrid.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g8b 19-December-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorHybrid.hpp"
#include "GIAtxtRelTranslatorHybridSentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID


#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
bool GIAtxtRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!GIApreprocessorWordIdentification.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#endif
	
	#ifdef GIA_PREPROCESSOR_POS_TAGGER
	if(!GIApreprocessorPOStagger.executePrelimFeatureProcessingOnSentences(outputLRPTextPlainTXTFileName, inputTextNLPfeatureXMLfileName, translatorVariables))
	{
		result = false;
	}
	#else
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();

	string outputLRPTextPlainTXTFileNamePrelim = outputLRPTextPlainTXTFileName + GIA_PREPROCESSOR_INTERMEDIARY_PRELIM_FILE_EXTENSION;
	string inputTextNLPfeatureXMLfileNamePrelim = string("prelim") + inputTextNLPfeatureXMLfileName;
	//cout << "outputLRPTextPlainTXTFileNamePrelim = " << outputLRPTextPlainTXTFileNamePrelim << endl;
	//cout << "inputTextNLPfeatureXMLfileNamePrelim = " << inputTextNLPfeatureXMLfileNamePrelim << endl;
	
	//print sentences to temporary file
	SHAREDvars.setCurrentDirectory(outputFolder);
	if(!GIApreprocessorWordIdentification.writeTagListToFile(translatorVariables->firstGIApreprocessorSentenceInList, outputLRPTextPlainTXTFileNamePrelim, "", true, false))
	{
		result = false;
	}	

	//execute prelim NLP parser (to extract POS features)
	GIAnlp.executeNLPparser(outputLRPTextPlainTXTFileNamePrelim, inputTextNLPfeatureXMLfileNamePrelim, translatorVariables, false);
	
	//now parse the NLP extracted sentence (ie feature POS) data
	GIAparagraph firstSentenceInList;
	bool createNewSentences = true;
	if(!GIAnlp.parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileNamePrelim, translatorVariables->isQuery, &firstSentenceInList, translatorVariables->NLPfeatureParser, &createNewSentences))
	{	
		result = false;
	}	
	
	//copy NLP extracted sentence (ie feature POS) data to the GIA preprocessor structure (as a temporary preliminary sentence record)
	GIAsentence* currentSentenceInList = &firstSentenceInList;
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		currentGIApreprocessorSentenceInList->sentenceReferencePrelim = currentSentenceInList;	//this doesnt appear to be used
		
		if(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)->size() != currentSentenceInList->numberOfWordsInSentence)
		{
			cerr << "GIAtxtRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences{} error: GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)->size() != currentSentenceInList->numberOfWordsInSentence)" << endl;
			exit(EXIT_ERROR);
		}
				
		GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;		
		for(int w = 0; w < currentSentenceInList->numberOfWordsInSentence; w++)
		{
			bool grammaticallyStrict = true;	//grammaticallyStrict required for preprocessor POS detection
			GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeatureInList, grammaticallyStrict);	//this is required to derive grammaticalWordType from NLP POS tags for GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			(*GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList))[w]->featureReferencePrelim = currentFeatureInList;
			currentFeatureInList = currentFeatureInList->next;
		}
		
		//debug:
		/*
		GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;
			currentFeatureInList = currentFeatureInList->next;
		}
		*/
		/*
		cout << "currentSentenceInList->firstFeatureInList->word = " << currentSentenceInList->firstFeatureInList->word << endl;	
		cout << "currentGIApreprocessorSentenceInList->sentenceContentsOriginal[0]->tagName = " << currentGIApreprocessorSentenceInList->sentenceContentsOriginal[0]->tagName << endl;
		*/
		
		currentSentenceInList = currentSentenceInList->next;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);	
	#endif
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_DEPRECIATED
					
bool GIAtxtRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string outputFileName, const string outputFileNameLRPforNLP)
{
	bool result = true;
	
	XMLparserTag* firstLogicReferenceClassTag = NULL;

	vector<XMLparserTag*> GIAtxtRelTranslatorRulesTokenLayers;
	if(!GIAtxtRelTranslatorRules.extractGIAtxtRelTranslatorRulesTokens(&GIAtxtRelTranslatorRulesTokenLayers))
	{
		result = true;
	}
	
	int charCount = 0;
	char currentToken;
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	int sentenceIndex = GIA_NLP_START_SENTENCE_INDEX;
	string outputTextContents = "";
	string outputTextContentsForNLP = "";
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		//string sentenceContentsOriginalText = removePrependingWhiteSpace(sentenceContents);	//only for NLC?
		if(!executeTxtRelTranslatorDepreciatedSentence(currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesTokenLayers))
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
		if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentGIApreprocessorSentenceInList->firstLogicReferenceInList, &sentenceIndex, &outputTextContentsSentence, &outputTextContentsForNLPsentence))
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


	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
	cout << "GIA_TXT_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT: " << endl;
	cout << "create preprocessed file, outputFileName = " << outputFileName << endl;
	cout << "outputTextContents = \n" << outputTextContents << endl;
	cout << "outputTextContentsForNLP = \n" << outputTextContentsForNLP << endl;
	cout << endl;
	currentGIApreprocessorSentenceInList = firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		cout << "currentGIApreprocessorSentenceInList->sentenceIndex = " << currentGIApreprocessorSentenceInList->sentenceIndex << ", sentenceIndexOriginal: " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << endl;
		GIAtxtRelTranslatorHybridLogicReferenceObject.printLogicReferenceLayer(currentGIApreprocessorSentenceInList->firstLogicReferenceInList);
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	cout << endl;
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorHybridClass::executeTxtRelTranslatorDepreciatedSentence(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers)
{
	bool result = true;
	
	#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	vector<GIApreprocessorPlainTextWord*>* sentenceContentsWordList = &(currentGIApreprocessorSentenceInList->sentenceContentsLRP);
	#else
	vector<GIApreprocessorPlainTextWord*>* sentenceContentsWordList = &(currentGIApreprocessorSentenceInList->sentenceContentsOriginal);
	#endif
	int sentenceIndexOriginal = currentGIApreprocessorSentenceInList->sentenceIndexOriginal;

	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	if(!GIAtxtRelTranslatorHybridLogicReferenceObject.executeLogicReferencePreprocessor(sentenceContentsWordList, currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesTokenLayers))
	{
		result = false;
	}
	#else
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList = currentGIApreprocessorSentenceInList->firstLogicReferenceInList;
	if(!GIAtxtRelTranslatorHybridReferenceSet.executeReferenceSetPreprocessor(sentenceContentsFirstWord, &sentenceContentsWordList, firstLogicReferenceInList->logicReferenceVariable, true, GIA_NLP_START_ENTITY_INDEX))
	{
		result = false;
	}
	firstLogicReferenceInList->next = new GIAtxtRelTranslatorHybridLogicReference();	//required for generateTxtRelTranslatorHybridSentenceNLPparsablePhrases
	#endif
	#endif
	
	return result;
}


string GIAtxtRelTranslatorHybridClass::removePrependingWhiteSpace(string sentenceContents)
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
bool GIAtxtRelTranslatorHybridClass::generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP)
{
	bool result = true;
	
	/*OLD generated format:
	eg "Tom [A] said that Mary [B] said that I like custard tarts that are yellow"
	=>
	"
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
	B said that C
	A said that B
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
	I 
	[like]
	custard tarts
	[that are]
	yellow	
	#else
	I 
	[like]
	custard tarts that are yellow
	#endif
	#else
	I like custard tarts that are yellow
	#endif
	#endif
	"
	*/
			
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			string sentenceContentsPreprocessedLogicReference = "";
			string sentenceContentsPreprocessedLogicReferenceForNLP = "";
			generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(&sentenceContentsPreprocessedLogicReference, &sentenceContentsPreprocessedLogicReferenceForNLP, currentLogicReferenceInList, sentenceIndex);
			*outputTextContentsSentence = *outputTextContentsSentence + sentenceContentsPreprocessedLogicReference;
			*outputTextContentsSentenceForNLP = *outputTextContentsSentenceForNLP + sentenceContentsPreprocessedLogicReferenceForNLP;
			#endif
	
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListArray, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
				{
					result = false;
				}
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
					{
						result = false;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListDependent, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
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
			
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			/*//redundant (equivalent code)
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
			*/
			GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT);				
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			if(!(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents).empty())
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER);
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
		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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


#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
void GIAtxtRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, string* sentenceContentsPreprocessedLogicReferenceForNLP, GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, int* sentenceIndex)
{
	if(currentLogicReferenceInList->hasSubLogicReference)
	{
		string logicReferenceSetContentsWithVariableNames = "";
		if(currentLogicReferenceInList->hasSubLogicReferenceArray)
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			generateLogicReferenceSetContentsWithVariableNamesConjunction(&logicReferenceSetContentsWithVariableNames, currentLogicReferenceInList, sentenceIndex);
			#endif
		}
		else
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			if(currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
			{			
			#endif
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->isReferenceSetDelimiter = true;
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = currentLogicReferenceInList->logicReferenceContents;	//eg "had proposed"
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(sentenceContentsPreprocessedLogicReference, sentenceContentsPreprocessedLogicReferenceForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER);
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			GIApreprocessorPlainTextWord* firstSubLogicReferenceInListGovernorLogicReferenceVariableName = new GIApreprocessorPlainTextWord(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->logicReferenceVariable->logicReferenceVariableName);
			GIApreprocessorPlainTextWord* firstSubLogicReferenceInListDependentLogicReferenceVariableName = new GIApreprocessorPlainTextWord(currentLogicReferenceInList->firstSubLogicReferenceInListDependent->logicReferenceVariable->logicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject->subReferenceSetContents).push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject->subReferenceSetContents).push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);
			//for debug only:
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			logicReferenceSetContentsWithVariableNames.push_back(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents);
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);	
			#endif
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
		*sentenceContentsPreprocessedLogicReference = *sentenceContentsPreprocessedLogicReference + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceForNLP = *sentenceContentsPreprocessedLogicReferenceForNLP + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNames = logicReferenceSetContentsWithVariableNames;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNamesSentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
		#endif
	}
	else
	{
		cerr << "GIAtxtRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference{} error: !(currentLogicReferenceInList->hasSubLogicReference)" << endl;
		exit(EXIT_ERROR);
	}
}

void GIAtxtRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesConjunction(string* logicReferenceSetContentsWithVariableNames, GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReference, int* sentenceIndex)
{
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReference->firstSubLogicReferenceInListArray;
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


void GIAtxtRelTranslatorHybridClass::addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, int referenceSetType)
{
	#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(!(referenceSet->isReferenceSetDelimiter))
	{
	#endif
		//cout << "add = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) << endl;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
		(referenceSet->subReferenceSetContentsOutputForNLP).clear();	//probably not required
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		if(referenceSet->isReferenceSetDelimiter)
		{
			GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
			GIApreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
			#endif
			GIApreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
			#endif
			referenceSet->dummyNLPtestOffset = GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE;
		} 
		else
		{
		#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT_FIRST_SUBREFERENCE_ONLY
			if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT && referenceSet->isFirstSubreferenceSetInList)
			#else
			if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT)	
			#endif
			{
				GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
				GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				GIApreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				GIApreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE - GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE;
			} 
			else if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT)
			{
			#endif
				GIApreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
				GIApreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
				#endif
				GIApreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = 0;
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
			}
			#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		}
		#endif
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) + CHAR_NEWLINE;
		#else
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + GIApreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents), true) + CHAR_NEWLINE;
		#endif
		
		referenceSet->sentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
	#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
}





//preconditions: all multiwords in GIArules.xml <preprocessor><logicReference><class name="preposition"> must be in LRPdata WikipediaEnglishClubMultiwordPrepositions.txt
#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
bool GIAtxtRelTranslatorHybridClass::updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const bool isQuery)
{
	bool result = true;

	GIApreprocessorWordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	
	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo = GIApreprocessorWordReduction.getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
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
			if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIApreprocessorSentenceInList->firstLogicReferenceInList, entityIndexOriginal, &GIApreprocessorSubReferenceSetFound))
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
				cerr << "GIApreprocessor::updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo{} error: !getGIAtxtRelTranslatorHybridReferenceSet" << endl;
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


		
	
bool GIAtxtRelTranslatorHybridClass::getGIApreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const int sentenceIndexOriginal, GIApreprocessorSentence** GIApreprocessorSentenceFound)
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

bool GIAtxtRelTranslatorHybridClass::getGIAtxtRelTranslatorHybridReferenceSet(GIAtxtRelTranslatorHybridLogicReference* firstGIAtxtRelTranslatorHybridLogicReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound)
{
	//cout << "1 getGIAtxtRelTranslatorHybridReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;

	bool result = false;
	GIAtxtRelTranslatorHybridLogicReference* currentGIAtxtRelTranslatorHybridLogicReferenceInList = firstGIAtxtRelTranslatorHybridLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentGIAtxtRelTranslatorHybridLogicReferenceInList->hasSubLogicReference)
		{
			if(currentGIAtxtRelTranslatorHybridLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListArray, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentGIAtxtRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListGovernor, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListDependent, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
		}
		else
		{
		#endif
			if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetSubject, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}
 			if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetObject, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}		
			if(getGIAtxtRelTranslatorHybridReferenceSet(currentGIAtxtRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, entityIndexOriginal, GIApreprocessorSubReferenceSetFound))
			{
				result = true;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE			
		}
		#endif
		
		if(currentGIAtxtRelTranslatorHybridLogicReferenceInList->next != NULL)
		{
			currentGIAtxtRelTranslatorHybridLogicReferenceInList = currentGIAtxtRelTranslatorHybridLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	return result;
}

bool GIAtxtRelTranslatorHybridClass::getGIAtxtRelTranslatorHybridReferenceSet(GIApreprocessorSubReferenceSet* firstGIApreprocessorSubReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound)
{
	bool result = false;
	//cout << "2 getGIAtxtRelTranslatorHybridReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;
	GIApreprocessorSubReferenceSet* currentGIApreprocessorSubReferenceInList = firstGIApreprocessorSubReferenceInList;
	while(currentGIApreprocessorSubReferenceInList->next != NULL)
	{
		//cout << "\t getGIAtxtRelTranslatorHybridReferenceSet: currentGIApreprocessorSubReferenceInList->firstIndexOfReferenceSetText = " << currentGIApreprocessorSubReferenceInList->firstIndexOfReferenceSetText << endl;
		//cout << "\t getGIAtxtRelTranslatorHybridReferenceSet: currentGIApreprocessorSubReferenceInList->lastIndexOfReferenceSetText = " << currentGIApreprocessorSubReferenceInList->lastIndexOfReferenceSetText << endl;

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




bool GIAtxtRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables)
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

bool GIAtxtRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity)
{	
	bool result = true;

	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInListInUpper = NULL;
	if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInListInUpper = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;
		while(currentLogicReferenceInListInUpper->previous != NULL)
		{
			currentLogicReferenceInListInUpper = currentLogicReferenceInListInUpper->previous;
		}
		firstLogicReferenceInListInUpper = currentLogicReferenceInListInUpper;
	}
	#endif
	
	
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
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
		
	
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		bool sameReferenceSetLogicReference = true;	//CHECKTHIS
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			GIAentityNode* logicReferenceEntity = NULL;
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				logicReferenceEntity = createNewRelationshipEntity(&(currentLogicReferenceInList->logicReferenceContents), GIA_ENTITY_TYPE_LOGIC_REFERENCE, translatorVariables);	//generate artificial conjunction entity
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
				if(currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
				{			
				#endif
					int sentenceIndexRecord = translatorVariables->sentenceIndex;
					translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex;	
					if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &logicReferenceEntity, NULL))
					{
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
						exit(EXIT_ERROR);				
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE			
					changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);	
					#endif
					translatorVariables->sentenceIndex = sentenceIndexRecord;
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
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
					GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said that A said B	[where currentLogicReferenceInList is A]
				}
				else if(currentLogicReferenceInList->isSubLogicReferenceDependent)
				{
					GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said that B said A	[where currentLogicReferenceInList is A]
				}
				else if(currentLogicReferenceInList->isSubLogicReferenceArray)
				{
					GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said A and B 	[where currentLogicReferenceInList is A]
					//CHECKTHIS; need to ensure that action relationship can have multiple objects connected
				}
				else
				{
					cerr << "GIAtxtRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: currentLogicReferenceInList->hasSubLogicReference && (currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL) && !(isSubLogicReferenceGovernor || isSubLogicReferenceDependent)" << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(currentLogicReferenceInList->previous != NULL)
			{
				GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said A and B 	[where currentLogicReferenceInList is B]	
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
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentGIApreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, translatorVariables, logicReferenceEntity));
					{
						result = false;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
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
			
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			bool subjectDefined = false;
			if(GIAtxtRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
			{
				subjectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "subjectDefined" << endl;
				#endif
			}
			bool objectDefined = false;
			if(GIAtxtRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				objectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "objectDefined" << endl;
				#endif
			}
			bool delimiterDefined = false;
			if(GIAtxtRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
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
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&subjectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, NULL, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT))
				{
					result = false;
				}
			}
			if(delimiterDefined)
			{
	
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntity, NULL))
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
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntityTempTenseInformation, NULL))
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
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&objectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, delimiterEntity, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT))
				{
					result = false;
				}
			}

			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			bool conjunctionWithoutSubjectOrObjectFound = false;
			bool conjunctionFound = false;
			if(GIAtxtRelTranslatorHybridLogicReferenceObject.islogicReferenceConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
			{
				conjunctionFound = true;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
			bool conjunctionFoundEmbedded = false;
			if(GIAtxtRelTranslatorHybridLogicReferenceObject.islogicReferenceParentConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
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
									
				GIAtxtRelTranslatorHybridLogicReference* firstImplicitConjunction = NULL;
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
					GIAtxtRelTranslatorHybridLogicReferenceVariable* firstImplicitConjunctionLogicReferenceVariable = NULL;
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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

									#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
									//create a delimiter
									delimiterEntity = createNewRelationshipAndConnectToSource(firstImplicitConjunctionSubject, firstImplicitConjunctionLogicReferenceVariable, sameReferenceSetLogicReferenceConjunction, translatorVariables);	//this will create an intermediary action/possession("have") node if necessary
									//dogs have cars and [have] chickens -> connect dogs to newly created artificial [have] node
									#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
									if(conjunctionFoundEmbedded)
									{
										delimiterEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
										delimiterEntity->isLogicReferenceEntity = true;
									}
									#endif
									delimiterDefined = true;

									//connect the delimiter to object 
									GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReference, translatorVariables);
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
									GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReference, translatorVariables);

									//translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject->sentenceIndex;
									GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(delimiterEntity, firstImplicitConjunctionSubject, sameReferenceSetLogicReferenceConjunction, translatorVariables);
								}
							}
							#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
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
									#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
									if(conjunctionFoundEmbedded)
									{
										delimiterEntity->entityType = GIA_ENTITY_TYPE_LOGIC_REFERENCE;
										delimiterEntity->isLogicReferenceEntity = true;
									}
									#endif
									delimiterDefined = true;

									GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceConjunction, translatorVariables);
								}
							}
							#endif
						}
						else
						{
							//conjunction logic reference already connected (connected to a unique subject)
							if(conjunctionFoundEmbedded)
							{
								GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(parentLogicReferenceEntity, firstImplicitConjunctionSubject, sameReferenceSetLogicReference, translatorVariables);
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
							GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
							GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
						}	
						else
						{
							//eg regarding (logicReference) Tom's (subject) moving (delimiter)
							GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(delimiterEntity, subjectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
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
							GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(delimiterEntity, objectEntity, sameReferenceSetLogicReferenceVariable, translatorVariables);
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
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			}
			#endif

			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			//connect the logic reference to the delimiter or subject
			#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES_IGNORE_CONNECTIONS_TO_SENTENCE_LOGIC_REFERENCE_SET
			translatorVariables->sentenceIndex = currentGIApreprocessorSentenceInList->sentenceIndex;
			#endif
			if(currentLogicReferenceInList->isSubLogicReferenceGovernor)
			{
				if(delimiterDefined)
				{
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
					{
						GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);	//, considering (action) that the pie is (connection) blue.	//eg Considering (action) that the pie is (connection) blue.
					}
					else
					{
						//eg Tom is (connection) green proposed (action)
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentGIApreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentGIApreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (delimiterDefined)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);		//eg , considering (action) the pie (subject).	//eg Considering (action) the pie (subject).	/ , considering (action) the pie (subject).	/	Tom (subject) proposed (action)...
				}
			}
			else if(currentLogicReferenceInList->isSubLogicReferenceDependent)
			{
				if(delimiterDefined)
				{	
					GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);		//eg The apple is (connection) large, considering (action). / Considering (action)...,  the apple is (connction) large	/ ...proposed (action) that the berries are (connection) tasty 
				}
				else
				{
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
					{
						GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);		//eg ...proposed (action) the plan (object)
					}
					else
					{
						//eg The apple (object), considering (action). / Considering (action)...,  the apple (object)
						cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentGIApreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentGIApreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && (delimiterDefined)" << endl;
						exit(EXIT_ERROR);
					}

				}
			}
			else if(firstLogicReferenceInList->isSubLogicReferenceArray)
			{
				if(delimiterDefined)
				{
					GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);
				}
				else if(subjectDefined)
				{
					GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);
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
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);
			#else
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, currentGIApreprocessorSentenceInList->sentenceIndex, translatorVariables);		
			#endif
			#endif
			#endif
			#endif

			#else
			//implementation unknown
			cerr << "GIApreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: implementation currently requires GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET" << endl;
			exit(EXIT_ERROR);
			#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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
void GIAtxtRelTranslatorHybridClass::changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
{
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		changeSentenceIndexOfEntityNodesAndConnections(currentSubReferenceSetInList->sentenceIndex, sentenceIndexNew, translatorVariables);		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
}

void GIAtxtRelTranslatorHybridClass::changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
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





#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
bool GIAtxtRelTranslatorHybridClass::updateRelationshipType(GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables)
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
bool GIAtxtRelTranslatorHybridClass::updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, GIAtranslatorVariablesClass* translatorVariables)
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
	
	
	

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES
bool GIAtxtRelTranslatorHybridClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT_COMPONENT_CONFIRMED logical conditions
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList;
	while(currentLogicReferenceInList->previous != NULL)
	{
		foundFirstImplicitConjunction = true;
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
		*firstImplicitConjunction = currentLogicReferenceInList;
	}
	return foundFirstImplicitConjunction;
}
#else
bool GIAtxtRelTranslatorHybridClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED logical conditions
	if(conjunctionLogicReferenceInList->previous != NULL)
	{
		GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList->previous;
		bool stillParsing = true;
		while((currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED) && stillParsing)
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






bool GIAtxtRelTranslatorHybridClass::findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntity, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAentityNode* primaryDelimiterEntity, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType)
{
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "\n\nGIAtxtRelTranslatorHybridClass::findPrimaryEntityAndReconcileSubReferenceSets" << endl;
	#endif
	
	GIAentityNode* lastDelimiterEntity = NULL;
	
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
		cout << "currentSubReferenceSetInList->subReferenceSetContents = " << generateTextFromVectorWordList(currentSubReferenceSetInList->subReferenceSetContents) << endl;
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
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
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
				if(currentSubReferenceSetInList == firstSubReferenceSetInList)
				{
				#endif
					*primaryEntity = intermediaryPrimaryEntity;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "*primaryEntity = intermediaryPrimaryEntity = " << intermediaryPrimaryEntity->entityName << endl;
					#endif
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
					firstSubReferenceSetInList->primaryEntityTemp = *primaryEntity;
					#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
				}
				else
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
					if(currentSubReferenceSetInList->isReferenceSetDelimiter)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "(currentSubReferenceSetInList->isReferenceSetDelimiter)" << endl;
						#endif
						
						bool sameReferenceSet = true;	//"that"/"which" imply sameReferenceSet
						if(currentSubReferenceSetInList->delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)
						{
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "currentSubReferenceSetInList->delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB" << endl;
							#endif
							if(primaryDelimiterEntity != NULL)
							{
								GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(intermediaryPrimaryEntity, primaryDelimiterEntity, sameReferenceSet, translatorVariables);	//eg tom rides the bike near the ball -> rides near
							}
							else
							{
								cerr << "GIApreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: (currentSubReferenceSetInList->delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB) && (lastDelimiterEntity == NULL)" << endl;
								exit(EXIT_ERROR);
							}
						}
						else
						{
							GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(intermediaryPrimaryEntity, *primaryEntity, sameReferenceSet, translatorVariables);
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "connectLogicReferenceRelationshipToSource" << endl;
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
							GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(lastDelimiterEntity, intermediaryPrimaryEntity, sameReferenceSet, translatorVariables);
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
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
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


void GIAtxtRelTranslatorHybridClass::deleteExternalConnectionsToConnection(GIAentityConnection* connection)
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




bool GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType, GIAentityNode** primaryEntity, GIAentityNode* primaryDelimiterEntity)
{
	bool result = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(currentSubReferenceSetInList->isReferenceSetDelimiter)		//REDUNDANT: || (referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER)
	{
		//dummySubject -> relationship <- dummyObject
		
		GIAentityNode* subjectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
		{
			result = false;
			cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			
			cerr << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
			cerr << "subjectParentEntity = " << subjectParentEntity->entityName << endl;
			GIAtxtRelTranslatorHybridReferenceSet.printSubReferenceSet(currentSubReferenceSetInList);
			
			cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
		GIAentityNode* objectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
		{
			result = false;
			cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		#endif
		
		*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;	//OR	*primaryEntity = parentEntityToPrimaryEntityConnection2->entity;

		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(subjectParentEntity);	//disable if not already disabled
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);	
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
		#endif
	}
	else
	{
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT_FIRST_SUBREFERENCE_ONLY
		if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT && currentSubReferenceSetInList->isFirstSubreferenceSetInList)
		#else
		if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT)	
		#endif
		{
			//dummySubject -> dummyRelationship -> object
			
			GIAentityNode* subjectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(subjectParentEntity, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
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
		else if(referenceSetType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT)
		{
		#endif
			//cout << "aq1" << endl;
			//subject <- dummyRelationship <- dummyObject
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				
				cerr << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
				cerr << "relationshipParentEntity = " << relationshipParentEntity->entityName << endl;
				GIAtxtRelTranslatorHybridReferenceSet.printSubReferenceSet(currentSubReferenceSetInList);

				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			GIAentityNode* objectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(objectParentEntity, GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAtxtRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			#endif
			
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(relationshipParentEntity);	//disable if not already disabled
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
			#endif
			
			*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
		}
		#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorHybridClass::findParentEntityBasedOnSentenceIndex(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity)
{
	bool result = false;
	GIAentityNode* entityFound = NULL;

	bool assumeAdvancedReferencing = false;
	#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING
	if(parentEntityName != GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA)
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


bool GIAtxtRelTranslatorHybridClass::findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex)
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

bool GIAtxtRelTranslatorHybridClass::transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(GIAentityNode* dummyDelimiterEntity, GIAentityNode* primaryDelimiterEntity)
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


bool GIAtxtRelTranslatorHybridClass::findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex)
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

bool GIAtxtRelTranslatorHybridClass::findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex)
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




		





//FUTURE GIA - upgrade getRelationshipNameAndType to use context to determine relationship type; property, condition, action, or definition? else how to distinguish between is adjective (eg tom is red) and is definition (a tom is red)?: need to look for the determiner of the subject/object (a x is a y/xs are ys/x is a y). actions (verb detection) and properties (have auxiliary detection) are ok.
bool GIAtxtRelTranslatorHybridClass::getRelationshipNameAndType(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, int* relationshipEntityType, string* relationshipName)
{
	bool result = true; 
	
	/*
	algorithm for relationship type detection;
		if verb then action
			ignore: the move, the fast move, the very fast move
			eg tom moves the ball
			eg tom does move the ball [may be preceeded by an auxiliary]
			eg tom rides fast	[may be preceeded by an adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
			eg tom rides very fast	[may be preceeded by an adverb and adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
		else if preposition then condition
			eg tom is near the ball 	[may be preceeded by an auxiliary]
			eg tom is certainly near the ball [may be preceeded by an auxiliary and adverb]
			eg tom rides the bike near the ball 	[may not be preceeded by an auxiliary, in which case the condition refers to {ie is subsequently linked by GIA to} the preceeding referenceSetDelimiter (eg verb): delimiterSpecialCase == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB]
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
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;	//CHECKTHIS - can still use POSprelim during post processing (perhaps better to use POS)?
	#else
	bool usePOSprelim = false;
	#endif
	
	GIApreprocessorPlainTextWord* relationshipWord = (relationshipReference->subReferenceSetContents).back();
	*relationshipName = relationshipWord->tagName;
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "relationshipName = " << *relationshipName << endl;
	cout << "relationshipReference->delimiterType = " << relationshipReference->delimiterType << endl;
	#endif
	
	if(relationshipReference->delimiterType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)	//GIAtxtRelTranslatorHybridReferenceSet.detectAuxiliary(*relationshipName)
	{
	}
	else if(relationshipReference->delimiterType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
	}
	else if(relationshipReference->delimiterType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_CONDITION;		
	}
	
	if(relationshipReference->delimiterType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)
	{	
		GIApreprocessorPlainTextWord* firstWordAfterAuxiliary = NULL;
		GIApreprocessorPlainTextWord* secondWordAfterAuxiliary = NULL;
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
			cerr << "GIAtxtRelTranslatorHybridClass::getRelationshipNameAndType{} error: (relationshipObject == NULL)" << endl;
			exit(EXIT_ERROR);
		}
		
		if(hasFirstWordAfterAuxiliary)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			//cout << "firstWordAfterAuxiliary = " << firstWordAfterAuxiliary << endl;
			#endif
			if(GIApreprocessorWordIdentification.determineIsAuxiliaryBeing(relationshipWord, usePOSprelim))
			{
				if(GIApreprocessorWordIdentification.determineIsAdjective(firstWordAfterAuxiliary, usePOSprelim) || (hasSecondWordAfterAuxiliary && GIApreprocessorWordIdentification.determineIsAdverb(firstWordAfterAuxiliary, usePOSprelim) && GIApreprocessorWordIdentification.determineIsAdjective(secondWordAfterAuxiliary, usePOSprelim)))
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;
					//FUTURE GIA: if GIA_ENTITY_TYPE_PROPERTY detected via determineIsAdjective(firstWordAfterAuxiliary)||determineIsAdjective(secondWordAfterAuxiliary), then need to set the relationshipEntityObject to GIA_ENTITY_TYPE_QUALITY also
				}
				else
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_DEFINITION;	
				}
			}
			if(GIApreprocessorWordIdentification.determineIsAuxiliaryHaving(relationshipWord, usePOSprelim))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;		
			}
			if(GIApreprocessorWordIdentification.determineIsAuxiliaryDoing(relationshipWord, usePOSprelim))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
			}
		}
		else
		{
			cerr << "GIAtxtRelTranslatorHybridClass::getRelationshipNameAndType{} error : !hasFirstWordAfterAuxiliary" << endl;
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



	
bool GIAtxtRelTranslatorHybridClass::getPreprocessorSentenceSubReferenceSet(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound)
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
bool GIAtxtRelTranslatorHybridClass::getPreprocessorSentenceSubReferenceSet(GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound)
{		
	bool result = false;
	
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getPreprocessorSentenceSubReferenceSet(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, subReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
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
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			if(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex == sentenceIndex)
			{
				*subReferenceSetFound = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter;
				result = true;
			}
			#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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

GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIAtxtRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	
{
	return createNewRelationshipAndConnectToSource(sourceEntity, logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, sameReferenceSet, translatorVariables);
}

GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	//OLD: this will create an intermediary action/possession("have") node if necessary
{
	GIAentityNode* relationshipEntity = createNewRelationship(relationshipReference, relationshipObject, translatorVariables);
	GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(relationshipEntity, sourceEntity, sameReferenceSet, translatorVariables);
	return relationshipEntity;
}


GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationship(GIAtxtRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables)
{
	return createNewRelationship(logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, translatorVariables);
}
GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationship(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables)
{
	//FUTURE: upgrade createNewRelationship:getRelationshipNameAndType to detect auxiliary tense information, and any associated adverb/adjectives within relationshipReference [else use GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS: this is currently required to process delimiter (eg being/having/verb/preposition) tense, adverbs (eg very near), and adjectives (eg rides fast) using NLP]

	int relationshipEntityType = GIA_ENTITY_TYPE_UNDEFINED;
	string relationshipName = "";
	getRelationshipNameAndType(relationshipReference, relationshipObject, &relationshipEntityType, &relationshipName);
	GIAentityNode* relationshipEntity = createNewRelationshipEntity(relationshipName, relationshipEntityType, translatorVariables);
	return relationshipEntity;
}

#endif



GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationshipEntity(vector<GIApreprocessorPlainTextWord*>* logicReferenceContents, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = NULL;
	if(logicReferenceContents->size() == 1)
	{
		relationshipEntity = createNewRelationshipEntity(GIApreprocessorWordClassObject.generateTextFromVectorWordList(logicReferenceContents), relationshipEntityType, translatorVariables);
	}
	else
	{
		cerr << "GIAtxtRelTranslatorHybridClass::createNewRelationshipEntity{} error: (logicReferenceContents->size() != 1)" << endl;
		exit(EXIT_ERROR);
	}

	return relationshipEntity;
}

GIAentityNode* GIAtxtRelTranslatorHybridClass::createNewRelationshipEntity(string relationshipEntityName, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	//NB see findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial{} / GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation{}:findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship{};
	//a new entity index is not required to be created because connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities is currently executed after GIAtranslator (or at least after the sentence has been parsed by GIAtranslator)
	functionEntityIndex3 = currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	currentSentenceInList->relationshipEntityArtificialIndexCurrent = param->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
	*/
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);	
	#else
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial2(relationshipEntityType, relationshipEntityName, translatorVariables);
	#endif
	
	relationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	
	return relationshipEntity;
}


#endif





