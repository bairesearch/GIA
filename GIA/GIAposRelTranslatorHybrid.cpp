 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorHybrid.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorHybrid.hpp"
#include "GIAposRelTranslatorHybridSentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"

#ifdef GIA_POS_REL_TRANSLATOR

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID


#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
bool GIAposRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables)
{
	#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!LRPpreprocessorWordIdentification.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#endif
	
	#ifdef LRP_PREPROCESSOR_POS_TAGGER
	if(!LRPpreprocessorPOStagger.executePrelimFeatureProcessingOnSentences(outputLRPTextPlainTXTFileName, inputTextNLPfeatureXMLfileName, translatorVariables))
	{
		result = false;
	}
	#else
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();

	string outputLRPTextPlainTXTFileNamePrelim = outputLRPTextPlainTXTFileName + LRP_PREPROCESSOR_INTERMEDIARY_PRELIM_FILE_EXTENSION;
	string inputTextNLPfeatureXMLfileNamePrelim = string("prelim") + inputTextNLPfeatureXMLfileName;
	//cout << "outputLRPTextPlainTXTFileNamePrelim = " << outputLRPTextPlainTXTFileNamePrelim << endl;
	//cout << "inputTextNLPfeatureXMLfileNamePrelim = " << inputTextNLPfeatureXMLfileNamePrelim << endl;
	
	//print sentences to temporary file
	SHAREDvars.setCurrentDirectory(outputFolder);
	if(!LRPpreprocessorWordIdentification.writeTagListToFile(translatorVariables->firstLRPpreprocessorSentenceInList, outputLRPTextPlainTXTFileNamePrelim, "", true, false))
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
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = translatorVariables->firstLRPpreprocessorSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		currentLRPpreprocessorSentenceInList->sentenceReferencePrelim = currentSentenceInList;	//this doesnt appear to be used
		
		if(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)->size() != currentSentenceInList->numberOfWordsInSentence)
		{
			cerr << "GIAposRelTranslatorHybridClass::executePrelimFeatureProcessingOnSentences{} error: LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)->size() != currentSentenceInList->numberOfWordsInSentence)" << endl;
			exit(EXIT_ERROR);
		}
				
		GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;		
		for(int w = 0; w < currentSentenceInList->numberOfWordsInSentence; w++)
		{
			bool grammaticallyStrict = true;	//grammaticallyStrict required for preprocessor POS detection
			GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeatureInList, grammaticallyStrict);	//this is required to derive grammaticalWordType from NLP POS tags for GIA_POS_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			(*LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList))[w]->featureReferencePrelim = currentFeatureInList;
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
		cout << "currentLRPpreprocessorSentenceInList->sentenceContentsOriginal[0]->tagName = " << currentLRPpreprocessorSentenceInList->sentenceContentsOriginal[0]->tagName << endl;
		*/
		
		currentSentenceInList = currentSentenceInList->next;
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	
	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);	
	#endif
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
					
bool GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, string outputFileName, string outputFileNameLRPforNLP)
{
	bool result = true;
	
	XMLparserTag* firstLogicReferenceClassTag = NULL;

	vector<XMLparserTag*> SANIrulesTokenLayers;
	if(!SANIrules.extractSANIrulesTokens(&SANIrulesTokenLayers))
	{
		result = true;
	}
	
	int charCount = 0;
	char currentToken;
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = firstLRPpreprocessorSentenceInList;
	int sentenceIndex = LRP_NLP_START_SENTENCE_INDEX;
	string outputTextContents = "";
	string outputTextContentsForNLP = "";
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		//string sentenceContentsOriginalText = removePrependingWhiteSpace(sentenceContents);	//only for NLC?
		if(!executeTxtRelTranslatorDepreciatedSentence(currentLRPpreprocessorSentenceInList, SANIrulesTokenLayers))
		{
			result = false;
		}

		string outputTextContentsSentence = "";
		string outputTextContentsForNLPsentence = "";
		#ifdef LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
		currentLRPpreprocessorSentenceInList->sentenceIndex = sentenceIndex;
		outputTextContents = outputTextContents + LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_SENTENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
		outputTextContentsForNLP = outputTextContentsForNLP + LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_SENTENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
		sentenceIndex++;
		#endif
		if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentLRPpreprocessorSentenceInList->firstLogicReferenceInList, &sentenceIndex, &outputTextContentsSentence, &outputTextContentsForNLPsentence))
		{
			result = false;
		}

		outputTextContents = outputTextContents + outputTextContentsSentence;
		outputTextContentsForNLP = outputTextContentsForNLP + outputTextContentsForNLPsentence;
		
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}

	SHAREDvars.setCurrentDirectory(outputFolder);	//save output files to output folder
	SHAREDvars.writeStringToFile(outputFileName, &outputTextContents);
	SHAREDvars.writeStringToFile(outputFileNameLRPforNLP, &outputTextContentsForNLP);
	SHAREDvars.setCurrentDirectory(inputFolder);	//set current directory back to the original inputFolder (this is required for both NLC_RECONCILE_CLASS_DEFINITION_LIST_FUNCTION_DECLARATION_ARGUMENTS and GIA, even if the GIA's inputFolder is changed to outputFolder as it should be in the case of NLC preprocessed input)


	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
	cout << "GIA_POS_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT: " << endl;
	cout << "create preprocessed file, outputFileName = " << outputFileName << endl;
	cout << "outputTextContents = \n" << outputTextContents << endl;
	cout << "outputTextContentsForNLP = \n" << outputTextContentsForNLP << endl;
	cout << endl;
	currentLRPpreprocessorSentenceInList = firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		cout << "currentLRPpreprocessorSentenceInList->sentenceIndex = " << currentLRPpreprocessorSentenceInList->sentenceIndex << ", sentenceIndexOriginal: " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << endl;
		GIAposRelTranslatorHybridLogicReferenceObject.printLogicReferenceLayer(currentLRPpreprocessorSentenceInList->firstLogicReferenceInList);
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	cout << endl;
	#endif
	
	return result;
}


bool GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciatedSentence(LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<XMLparserTag*>* SANIrulesTokenLayers)
{
	bool result = true;
	
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	vector<LRPpreprocessorPlainTextWord*>* sentenceContentsWordList = &(currentLRPpreprocessorSentenceInList->sentenceContentsLRP);
	#else
	vector<LRPpreprocessorPlainTextWord*>* sentenceContentsWordList = &(currentLRPpreprocessorSentenceInList->sentenceContentsOriginal);
	#endif
	int sentenceIndexOriginal = currentLRPpreprocessorSentenceInList->sentenceIndexOriginal;

	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
	if(!GIAposRelTranslatorHybridLogicReferenceObject.executeLogicReferencePreprocessor(sentenceContentsWordList, currentLRPpreprocessorSentenceInList, SANIrulesTokenLayers))
	{
		result = false;
	}
	#else
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList = currentLRPpreprocessorSentenceInList->firstLogicReferenceInList;
	if(!GIAposRelTranslatorHybridReferenceSet.executeReferenceSetPreprocessor(&sentenceContentsWordList, firstLogicReferenceInList->logicReferenceVariable, true, LRP_NLP_START_ENTITY_INDEX))
	{
		result = false;
	}
	firstLogicReferenceInList->next = new GIAposRelTranslatorHybridLogicReference();	//required for generateTxtRelTranslatorHybridSentenceNLPparsablePhrases
	#endif
	#endif
	
	return result;
}


string GIAposRelTranslatorHybridClass::removePrependingWhiteSpace(string sentenceContents)
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
bool GIAposRelTranslatorHybridClass::generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP)
{
	bool result = true;
	
	/*OLD generated format:
	eg "Tom [A] said that Mary [B] said that I like custard tarts that are yellow"
	=>
	"
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
	B said that C
	A said that B
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
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
			
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
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
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, outputTextContentsSentence, outputTextContentsSentenceForNLP))
					{
						result = false;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
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
	
			#ifdef LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
			currentLogicReferenceInList->logicReferenceVariable->sentenceIndex = *sentenceIndex;
			sentenceContentsPreprocessedLogicReferenceVariables = sentenceContentsPreprocessedLogicReferenceVariables + LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_LOGIC_REFERENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
			sentenceContentsPreprocessedLogicReferenceVariablesForNLP = sentenceContentsPreprocessedLogicReferenceVariablesForNLP + LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_LOGIC_REFERENCE_CONTENTS + CHAR_FULLSTOP + CHAR_NEWLINE;
			*sentenceIndex = *sentenceIndex + 1;
			#endif

			//cout << "Q sentenceIndex = " << *sentenceIndex << endl;
			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			/*//redundant (equivalent code)
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
			*/
			LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT);				
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			while(currentSubReferenceSetInList->next != NULL)
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentSubReferenceSetInList, sentenceIndex, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
			}
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "addSentenceToSentenceContentsPreprocessedLogicReferenceVariables GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, sentenceIndex = " << *sentenceIndex << endl;
			#endif
			if(!(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents).empty())
			{
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(&sentenceContentsPreprocessedLogicReferenceVariables, &sentenceContentsPreprocessedLogicReferenceVariablesForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER);
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
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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


#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
void GIAposRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, string* sentenceContentsPreprocessedLogicReferenceForNLP, GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, int* sentenceIndex)
{
	if(currentLogicReferenceInList->hasSubLogicReference)
	{
		string logicReferenceSetContentsWithVariableNames = "";
		if(currentLogicReferenceInList->hasSubLogicReferenceArray)
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			generateLogicReferenceSetContentsWithVariableNamesConjunction(&logicReferenceSetContentsWithVariableNames, currentLogicReferenceInList, sentenceIndex);
			#endif
		}
		else
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			if(currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
			{			
			#endif
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->isReferenceSetDelimiter = true;
				currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = currentLogicReferenceInList->logicReferenceContents;	//eg "had proposed"
				addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(sentenceContentsPreprocessedLogicReference, sentenceContentsPreprocessedLogicReferenceForNLP, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, sentenceIndex, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER);
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
			}
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
			LRPpreprocessorPlainTextWord* firstSubLogicReferenceInListGovernorLogicReferenceVariableName = new LRPpreprocessorPlainTextWord(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->logicReferenceVariable->logicReferenceVariableName);
			LRPpreprocessorPlainTextWord* firstSubLogicReferenceInListDependentLogicReferenceVariableName = new LRPpreprocessorPlainTextWord(currentLogicReferenceInList->firstSubLogicReferenceInListDependent->logicReferenceVariable->logicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject->subReferenceSetContents).push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject->subReferenceSetContents).push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);
			//for debug only:
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListGovernorLogicReferenceVariableName);
			logicReferenceSetContentsWithVariableNames.push_back(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents);
			logicReferenceSetContentsWithVariableNames.push_back(firstSubLogicReferenceInListDependentLogicReferenceVariableName);	
			#endif
		}
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
		*sentenceContentsPreprocessedLogicReference = *sentenceContentsPreprocessedLogicReference + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceForNLP = *sentenceContentsPreprocessedLogicReferenceForNLP + logicReferenceSetContentsWithVariableNames + CHAR_FULLSTOP + CHAR_NEWLINE;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNames = logicReferenceSetContentsWithVariableNames;
		currentLogicReferenceInList->logicReferenceSetContentsWithVariableNamesSentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
		#endif
	}
	else
	{
		cerr << "GIAposRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference{} error: !(currentLogicReferenceInList->hasSubLogicReference)" << endl;
		exit(EXIT_ERROR);
	}
}

void GIAposRelTranslatorHybridClass::generateLogicReferenceSetContentsWithVariableNamesConjunction(string* logicReferenceSetContentsWithVariableNames, GIAposRelTranslatorHybridLogicReference* conjunctionLogicReference, const int* sentenceIndex)
{
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReference->firstSubLogicReferenceInListArray;
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


void GIAposRelTranslatorHybridClass::addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, LRPpreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, const int referenceSetType)
{
	#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(!(referenceSet->isReferenceSetDelimiter))
	{
	#endif
		//cout << "add = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) << endl;
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
		(referenceSet->subReferenceSetContentsOutputForNLP).clear();	//probably not required
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		if(referenceSet->isReferenceSetDelimiter)
		{
			LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
			LRPpreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
			#endif
			LRPpreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
			#endif
			referenceSet->dummyNLPtestOffset = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE;
		} 
		else
		{
		#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT_FIRST_SUBREFERENCE_ONLY
			if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT && referenceSet->isFirstSubreferenceSetInList)
			#else
			if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT)	
			#endif
			{
				LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE);
				LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				LRPpreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				LRPpreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE - GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE;
			} 
			else if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT)
			{
			#endif
				LRPpreprocessorWordClassObject.addWordListToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), &(referenceSet->subReferenceSetContents));
				LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE);
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
				LRPpreprocessorWordClassObject.addStringArrayToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE);
				#endif
				LRPpreprocessorWordClassObject.addStringToWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), SHAREDvars.convertCharToString(CHAR_FULLSTOP));
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSet->subReferenceSetContentsOutputForNLP = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) << endl;
				#endif
				referenceSet->dummyNLPtestOffset = 0;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
			}
			#endif

		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
		}
		#endif
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContentsOutputForNLP), true) + CHAR_NEWLINE;
		#else
		*sentenceContentsPreprocessedLogicReferenceVariables = *sentenceContentsPreprocessedLogicReferenceVariables + LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents)) + CHAR_NEWLINE;
		*sentenceContentsPreprocessedLogicReferenceVariablesForNLP = *sentenceContentsPreprocessedLogicReferenceVariablesForNLP + LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&(referenceSet->subReferenceSetContents), true) + CHAR_NEWLINE;
		#endif
		
		referenceSet->sentenceIndex = *sentenceIndex;
		*sentenceIndex = *sentenceIndex + 1;
	#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
}





//preconditions: all multiwords in GIArules.xml <preprocessor><logicReference><class name="preposition"> must be in LRPdata WikipediaEnglishClubMultiwordPrepositions.txt
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
bool GIAposRelTranslatorHybridClass::updateLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(constEffective LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, bool isQuery)
{
	bool result = true;

	LRPpreprocessorWordReduction.setActiveLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(isQuery);	
	LRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstLRPpreprocessorMultiwordReductiontagCorrespondenceInfo = LRPpreprocessorWordReduction.getActiveLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	LRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = firstLRPpreprocessorMultiwordReductiontagCorrespondenceInfo;
	while(currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next != NULL)
	{
		int sentenceIndexOriginal = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex;
		int entityIndexOriginal = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex;
		
		LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = NULL;
		if(getLRPpreprocessorSentence(firstLRPpreprocessorSentenceInList, currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex, &currentLRPpreprocessorSentenceInList))
		{
			//calling this multiple times is somewhat inefficient
			LRPpreprocessorSubReferenceSet* LRPpreprocessorSubReferenceSetFound = NULL;
			if(getGIAposRelTranslatorHybridReferenceSet(currentLRPpreprocessorSentenceInList->firstLogicReferenceInList, entityIndexOriginal, &LRPpreprocessorSubReferenceSetFound))
			{
				currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = LRPpreprocessorSubReferenceSetFound->sentenceIndex;
				currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = entityIndexOriginal - (LRPpreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText - LRP_NLP_START_ENTITY_INDEX) + LRPpreprocessorSubReferenceSetFound->dummyNLPtestOffset;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "entityIndexOriginal = " << entityIndexOriginal << endl;
				cout << "sentenceIndexOriginal = " << sentenceIndexOriginal << endl;
				cout << "currentLRPpreprocessorSentenceInList->sentenceContentsOriginalText = " << currentLRPpreprocessorSentenceInList->sentenceContentsOriginalText << endl;
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->sentenceIndex << endl;
				cout << "currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex = " << currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->entityIndex << endl;
				cout << "LRPpreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText = " << LRPpreprocessorSubReferenceSetFound->firstIndexOfReferenceSetText << endl;
				#endif
			}
			else
			{
				cerr << "LRPpreprocessor::updateLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo{} error: !getGIAposRelTranslatorHybridReferenceSet" << endl;
				exit(EXIT_ERROR);	
			}
		}
		else
		{
			cerr << "LRPpreprocessor::updateLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo{} error: !getLRPpreprocessorSentenceInList" << endl;
			exit(EXIT_ERROR);	
		}
		
		currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo = currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo->next;
	}
	
	return result;
}	


		
	
bool GIAposRelTranslatorHybridClass::getLRPpreprocessorSentence(constEffective LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const int sentenceIndexOriginal, constEffective LRPpreprocessorSentence** LRPpreprocessorSentenceFound)
{
	bool result = false;
	constEffective LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		if(currentLRPpreprocessorSentenceInList->sentenceIndexOriginal == sentenceIndexOriginal)
		{
			*LRPpreprocessorSentenceFound = currentLRPpreprocessorSentenceInList;
			result = true;
		}
		
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	return result;
}

bool GIAposRelTranslatorHybridClass::getGIAposRelTranslatorHybridReferenceSet(constEffective GIAposRelTranslatorHybridLogicReference* firstGIAposRelTranslatorHybridLogicReferenceInList, const int entityIndexOriginal, constEffective LRPpreprocessorSubReferenceSet** LRPpreprocessorSubReferenceSetFound)
{
	//cout << "1 getGIAposRelTranslatorHybridReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;

	bool result = false;
	constEffective GIAposRelTranslatorHybridLogicReference* currentGIAposRelTranslatorHybridLogicReferenceInList = firstGIAposRelTranslatorHybridLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentGIAposRelTranslatorHybridLogicReferenceInList->hasSubLogicReference)
		{
			if(currentGIAposRelTranslatorHybridLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListArray, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
			else
			{
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentGIAposRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListGovernor, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->firstSubLogicReferenceInListDependent, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
				{
					result = true;
				}
			}
		}
		else
		{
		#endif
			if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetSubject, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
			{
				result = true;
			}
 			if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetObject, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
			{
				result = true;
			}		
			if(getGIAposRelTranslatorHybridReferenceSet(currentGIAposRelTranslatorHybridLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, entityIndexOriginal, LRPpreprocessorSubReferenceSetFound))
			{
				result = true;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE			
		}
		#endif
		
		if(currentGIAposRelTranslatorHybridLogicReferenceInList->next != NULL)
		{
			currentGIAposRelTranslatorHybridLogicReferenceInList = currentGIAposRelTranslatorHybridLogicReferenceInList->next;
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	return result;
}

bool GIAposRelTranslatorHybridClass::getGIAposRelTranslatorHybridReferenceSet(constEffective LRPpreprocessorSubReferenceSet* firstLRPpreprocessorSubReferenceInList, const int entityIndexOriginal, constEffective LRPpreprocessorSubReferenceSet** LRPpreprocessorSubReferenceSetFound)
{
	bool result = false;
	//cout << "2 getGIAposRelTranslatorHybridReferenceSet: entityIndexOriginal = " << entityIndexOriginal << endl;
	constEffective LRPpreprocessorSubReferenceSet* currentLRPpreprocessorSubReferenceInList = firstLRPpreprocessorSubReferenceInList;
	while(currentLRPpreprocessorSubReferenceInList->next != NULL)
	{
		//cout << "\t getGIAposRelTranslatorHybridReferenceSet: currentLRPpreprocessorSubReferenceInList->firstIndexOfReferenceSetText = " << currentLRPpreprocessorSubReferenceInList->firstIndexOfReferenceSetText << endl;
		//cout << "\t getGIAposRelTranslatorHybridReferenceSet: currentLRPpreprocessorSubReferenceInList->lastIndexOfReferenceSetText = " << currentLRPpreprocessorSubReferenceInList->lastIndexOfReferenceSetText << endl;

		if((entityIndexOriginal >= currentLRPpreprocessorSubReferenceInList->firstIndexOfReferenceSetText) && (entityIndexOriginal < currentLRPpreprocessorSubReferenceInList->lastIndexOfReferenceSetText))
		{
			result = true;
			*LRPpreprocessorSubReferenceSetFound = currentLRPpreprocessorSubReferenceInList;
		}
		
		currentLRPpreprocessorSubReferenceInList = currentLRPpreprocessorSubReferenceInList->next;
	}
	return result;
}

#endif

#endif	




bool GIAposRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentLRPpreprocessorSentenceInList, currentLRPpreprocessorSentenceInList->firstLogicReferenceInList, translatorVariables, NULL);
		
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	return result;
}

bool GIAposRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity)
{	
	bool result = true;

	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInListInUpper = NULL;
	if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInListInUpper = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;
		while(currentLogicReferenceInListInUpper->previous != NULL)
		{
			currentLogicReferenceInListInUpper = currentLogicReferenceInListInUpper->previous;
		}
		firstLogicReferenceInListInUpper = currentLogicReferenceInListInUpper;
	}
	#endif
	
	
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{			
		#ifdef LRP_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
		translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->sentenceIndex; //this sentenceIndex will be replaced in the future
		#else
		#ifdef LRP_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
		translatorVariables->sentenceIndex = currentLRPpreprocessorSentenceInList->sentenceIndex;
		#else
		//no new entities or connections will be created
		#endif
		#endif
		
	
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		bool sameReferenceSetLogicReference = true;	//CHECKTHIS
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			GIAentityNode* logicReferenceEntity = NULL;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				logicReferenceEntity = createNewRelationshipEntity(&(currentLogicReferenceInList->logicReferenceContents), GIA_ENTITY_TYPE_LOGIC_REFERENCE, translatorVariables);	//generate artificial conjunction entity
			}
			else
			{
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
				if(currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
				{			
				#endif
					int sentenceIndexRecord = translatorVariables->sentenceIndex;
					translatorVariables->sentenceIndex = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex;	
					if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &logicReferenceEntity, NULL))
					{
						cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
						exit(EXIT_ERROR);				
					}
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE			
					changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex, currentLRPpreprocessorSentenceInList->sentenceIndex, translatorVariables);	
					#endif
					translatorVariables->sentenceIndex = sentenceIndexRecord;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY
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
					cerr << "GIAposRelTranslatorHybridClass::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: currentLogicReferenceInList->hasSubLogicReference && (currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL) && !(isSubLogicReferenceGovernor || isSubLogicReferenceDependent)" << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(currentLogicReferenceInList->previous != NULL)
			{
				GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, logicReferenceEntity, sameReferenceSetLogicReference, translatorVariables);	//eg Tom said A and B 	[where currentLogicReferenceInList is B]	
			}


			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentLRPpreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListArray, translatorVariables, logicReferenceEntity));
				{
					result = false;
				}
			}
			else
			{
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentLRPpreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, translatorVariables, logicReferenceEntity));
					{
						result = false;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				}
				#endif
				if(!connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(currentLRPpreprocessorSentenceInList, currentLogicReferenceInList->firstSubLogicReferenceInListDependent, translatorVariables, logicReferenceEntity));
				{
					result = false;
				}
			}
		}
		else
		{
		#endif
			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			bool subjectDefined = false;
			if(GIAposRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
			{
				subjectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "subjectDefined" << endl;
				#endif
			}
			bool objectDefined = false;
			if(GIAposRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				objectDefined = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "objectDefined" << endl;
				#endif
			}
			bool delimiterDefined = false;
			if(GIAposRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter))
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
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&subjectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, NULL, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT))
				{
					result = false;
				}
			}
			if(delimiterDefined)
			{
	
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntity, NULL))
				{
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
					exit(EXIT_ERROR);				
				}
				if(!updateRelationshipType(delimiterEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, translatorVariables))
				{
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !updateRelationshipType" << endl;
					exit(EXIT_ERROR);	
				}
				#else
				delimiterEntityTempTenseInformation = NULL;
				if(!findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER, &delimiterEntityTempTenseInformation, NULL))
				{
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections" << endl;
					exit(EXIT_ERROR);				
				}
				delimiterEntity = createNewRelationship(currentLogicReferenceInList->logicReferenceVariable, translatorVariables);	//OLD: this will create an intermediary action/possession("have") node if necessary
				if(!updateRelationshipTenseInformation(delimiterEntity, delimiterEntityTempTenseInformation, translatorVariables))
				{
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !updateRelationshipTenseInformation" << endl;
					exit(EXIT_ERROR);	
				}
				#endif
				#else
				delimiterEntity = createNewRelationship(currentLogicReferenceInList->logicReferenceVariable, translatorVariables);	//OLD: this will create an intermediary action/possession("have") node if necessary
				#endif
			}
			if(objectDefined)
			{
				if(!findPrimaryEntityAndReconcileSubReferenceSets(&objectEntity, currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, delimiterEntity, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT))
				{
					result = false;
				}
			}

			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			bool conjunctionWithoutSubjectOrObjectFound = false;
			bool conjunctionFound = false;
			if(GIAposRelTranslatorHybridLogicReferenceObject.islogicReferenceConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
			{
				conjunctionFound = true;
			}
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
			bool conjunctionFoundEmbedded = false;
			if(GIAposRelTranslatorHybridLogicReferenceObject.islogicReferenceParentConjunctionComponentNotFirst(currentLogicReferenceInList, firstLogicReferenceInList))
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
									
				GIAposRelTranslatorHybridLogicReference* firstImplicitConjunction = NULL;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
					GIAposRelTranslatorHybridLogicReferenceVariable* firstImplicitConjunctionLogicReferenceVariable = NULL;
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
					if(conjunctionFoundEmbedded)
					{
						if(firstLogicReferenceInListInUpper->hasSubLogicReference)
						{
							/*
							if(!delimiterDefined)
							{
								cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (firstLogicReferenceInListInUpper->hasSubLogicReference) && (!delimiterDefined)" << endl;
								exit(EXIT_ERROR);
							}
							*/
							firstImplicitConjunctionSubject = firstImplicitConjunction->firstSubLogicReferenceInListGovernor->logicReferenceVariable->referenceSetSubject->primaryEntityTemp;
						}
						else
						{
							cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !(firstLogicReferenceInListInUpper->hasSubLogicReference)" << endl;
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

									#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
									//create a delimiter
									delimiterEntity = createNewRelationshipAndConnectToSource(firstImplicitConjunctionSubject, firstImplicitConjunctionLogicReferenceVariable, sameReferenceSetLogicReferenceConjunction, translatorVariables);	//this will create an intermediary action/possession("have") node if necessary
									//dogs have cars and [have] chickens -> connect dogs to newly created artificial [have] node
									#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
									cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: islogicReferenceConjunctionAndOr(currentLogicReferenceInList) && (!subjectDefined && objectDefined) && !delimiterDefined" << endl;
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
							#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
							else if(subjectDefined)
							{
								if(delimiterDefined)
								{
									cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: islogicReferenceConjunctionAndOr(currentLogicReferenceInList) && (subjectDefined && !objectDefined) && delimiterDefined" << endl;
									exit(EXIT_ERROR);
								}
								else
								{
									//create a delimiter
									delimiterEntity = createNewRelationshipAndConnectToSource(firstImplicitConjunctionSubject, firstImplicitConjunctionLogicReferenceVariable, sameReferenceSetLogicReferenceConjunction, translatorVariables);	//this will create an intermediary action/possession("have") node if necessary
									//dogs have cars and [have] chickens -> connect dogs to newly created artificial [have] node
									#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
						cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: conjunctionFound && firstImplicitConjunctionSubject == NULL" << endl;
						exit(EXIT_ERROR);	
					}
				}
				else
				{
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: conjunctionFound && !findFirstImplicitConjunctionLogicReferenceSetBefore" << endl;
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
							cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: subjectDefined && !delimiterDefined && objectDefined" << endl;
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
							cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !subjectDefined && delimiterDefined && !objectDefined" << endl;
							exit(EXIT_ERROR);
						}
					}
					else
					{
						/*//may be a conjunction layer; this will be processed later
						//eg regarding (logicReference) the pie (object)
						cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: !subjectDefined && !delimiterDefined && objectDefined" << endl;
						exit(EXIT_ERROR);
						*/
					}	
				}
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			}
			#endif

			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			//connect the logic reference to the delimiter or subject
			#ifdef LRP_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES_IGNORE_CONNECTIONS_TO_SENTENCE_LOGIC_REFERENCE_SET
			translatorVariables->sentenceIndex = currentLRPpreprocessorSentenceInList->sentenceIndex;
			#endif
			if(currentLogicReferenceInList->isSubLogicReferenceGovernor)
			{
				if(delimiterDefined)
				{
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
					{
						GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(parentLogicReferenceEntity, delimiterEntity, sameReferenceSetLogicReference, translatorVariables);	//, considering (action) that the pie is (connection) blue.	//eg Considering (action) that the pie is (connection) blue.
					}
					else
					{
						//eg Tom is (connection) green proposed (action)
						cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentLRPpreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentLRPpreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (delimiterDefined)" << endl;
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
					if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
					{
						GIAtranslatorOperations.connectLogicReferenceRelationshipToTarget(parentLogicReferenceEntity, subjectEntity, sameReferenceSetLogicReference, translatorVariables);		//eg ...proposed (action) the plan (object)
					}
					else
					{
						//eg The apple (object), considering (action). / Considering (action)...,  the apple (object)
						cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentLRPpreprocessorSentenceInList->isSubLogicReferenceGovernor) && (currentLRPpreprocessorSentenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && (delimiterDefined)" << endl;
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
					cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: (currentLRPpreprocessorSentenceInList->isSubLogicReferenceArray) && (!delimiterDefined) && (!subjectDefined)." << endl;
					exit(EXIT_ERROR);
				}
			}
			#endif

			#ifdef LRP_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			#ifdef LRP_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, translatorVariables);
			#else
			//cout << "currentLRPpreprocessorSentenceInList->sentenceIndex = " << currentLRPpreprocessorSentenceInList->sentenceIndex << endl;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject, currentLRPpreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject, currentLRPpreprocessorSentenceInList->sentenceIndex, translatorVariables);
			changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter, currentLRPpreprocessorSentenceInList->sentenceIndex, translatorVariables);
			#else
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
			changeSentenceIndexOfEntityNodesAndConnections(currentLogicReferenceInList->logicReferenceVariable->sentenceIndex, currentLRPpreprocessorSentenceInList->sentenceIndex, translatorVariables);		
			#endif
			#endif
			#endif
			#endif

			#else
			//implementation unknown
			cerr << "LRPpreprocessor::connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities{} error: implementation currently requires GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET" << endl;
			exit(EXIT_ERROR);
			#endif
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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

#ifdef LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
void GIAposRelTranslatorHybridClass::changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
{
	LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		changeSentenceIndexOfEntityNodesAndConnections(currentSubReferenceSetInList->sentenceIndex, sentenceIndexNew, translatorVariables);		
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;
	}
}

void GIAposRelTranslatorHybridClass::changeSentenceIndexOfEntityNodesAndConnections(int sentenceIndexOriginal, int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables)
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





#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
bool GIAposRelTranslatorHybridClass::updateRelationshipType(GIAentityNode* relationshipEntity, LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, const GIAtranslatorVariablesClass* translatorVariables)
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
bool GIAposRelTranslatorHybridClass::updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, LRPpreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, const GIAtranslatorVariablesClass* translatorVariables)
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
	
	
	

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES
bool GIAposRelTranslatorHybridClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIAposRelTranslatorHybridLogicReference* conjunctionLogicReferenceInList, constEffective GIAposRelTranslatorHybridLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT_COMPONENT_CONFIRMED logical conditions
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList;
	while(currentLogicReferenceInList->previous != NULL)
	{
		foundFirstImplicitConjunction = true;
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
		*firstImplicitConjunction = currentLogicReferenceInList;
	}
	return foundFirstImplicitConjunction;
}
#else
bool GIAposRelTranslatorHybridClass::findFirstImplicitConjunctionLogicReferenceSetBefore(GIAposRelTranslatorHybridLogicReference* conjunctionLogicReferenceInList, constEffective GIAposRelTranslatorHybridLogicReference** firstImplicitConjunction)
{
	bool foundFirstImplicitConjunction = false;
	
	//go back and reprocess the referenceSet content of all previous GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED logical conditions
	if(conjunctionLogicReferenceInList->previous != NULL)
	{
		GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = conjunctionLogicReferenceInList->previous;
		bool stillParsing = true;
		while((currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED) && stillParsing)
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






bool GIAposRelTranslatorHybridClass::findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntity, LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAentityNode* primaryDelimiterEntity, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType)
{
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "\n\nGIAposRelTranslatorHybridClass::findPrimaryEntityAndReconcileSubReferenceSets" << endl;
	#endif
	
	GIAentityNode* lastDelimiterEntity = NULL;
	
	LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
		cout << "currentSubReferenceSetInList->subReferenceSetContents = " << generateTextFromVectorWordList(currentSubReferenceSetInList->subReferenceSetContents) << endl;
		#endif
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
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
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
				if(currentSubReferenceSetInList == firstSubReferenceSetInList)
				{
				#endif
					*primaryEntity = intermediaryPrimaryEntity;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "*primaryEntity = intermediaryPrimaryEntity = " << intermediaryPrimaryEntity->entityName << endl;
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
					firstSubReferenceSetInList->primaryEntityTemp = *primaryEntity;
					#endif
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
				}
				else
				{
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
					if(currentSubReferenceSetInList->isReferenceSetDelimiter)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "(currentSubReferenceSetInList->isReferenceSetDelimiter)" << endl;
						#endif
						
						bool sameReferenceSet = true;	//"that"/"which" imply sameReferenceSet
						if(currentSubReferenceSetInList->delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)
						{
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "currentSubReferenceSetInList->delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB" << endl;
							#endif
							if(primaryDelimiterEntity != NULL)
							{
								GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(intermediaryPrimaryEntity, primaryDelimiterEntity, sameReferenceSet, translatorVariables);	//eg tom rides the bike near the ball -> rides near
							}
							else
							{
								cerr << "LRPpreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: (currentSubReferenceSetInList->delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB) && (lastDelimiterEntity == NULL)" << endl;
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
							cerr << "LRPpreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: !(currentSubReferenceSetInList->isReferenceSetDelimiter) && (lastDelimiterEntity == NULL)" << endl;
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
				cerr << "LRPpreprocessor::findPrimaryEntityAndReconcileSubReferenceSets{} error: !findPrimaryEntityBasedOnSentenceIndexAndType" << endl;
				exit(EXIT_ERROR);
			}
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
		#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
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


void GIAposRelTranslatorHybridClass::deleteExternalConnectionsToConnection(GIAentityConnection* connection)
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




bool GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType, GIAentityNode** primaryEntity, GIAentityNode* primaryDelimiterEntity)
{
	bool result = true;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	if(currentSubReferenceSetInList->isReferenceSetDelimiter)		//REDUNDANT: || (referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_DELIMITER)
	{
		//dummySubject -> relationship <- dummyObject
		
		GIAentityNode* subjectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
		{
			result = false;
			cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			
			cerr << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
			cerr << "subjectParentEntity = " << subjectParentEntity->entityName << endl;
			GIAposRelTranslatorHybridReferenceSet.printSubReferenceSet(currentSubReferenceSetInList);
			
			cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
			exit(EXIT_ERROR);
		}
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
		GIAentityNode* objectParentEntity = NULL;
		if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
		{
			result = false;
			cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
		if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
		{
			result = false;
			cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} isReferenceSetDelimiter: !findSameSentenceRelationshipEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
			exit(EXIT_ERROR);
		}
		#endif
		
		*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;	//OR	*primaryEntity = parentEntityToPrimaryEntityConnection2->entity;

		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(subjectParentEntity);	//disable if not already disabled
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
		deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);	
		GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
		#endif
	}
	else
	{
	#endif
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT_FIRST_SUBREFERENCE_ONLY
		if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT && currentSubReferenceSetInList->isFirstSubreferenceSetInList)
		#else
		if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT)	
		#endif
		{
			//dummySubject -> dummyRelationship -> object
			
			GIAentityNode* subjectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT, &subjectParentEntity))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(subjectParentEntity, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(subjectParentEntity, true, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT" << endl;
				exit(EXIT_ERROR);
			}
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_OBJECT: !findSameSentenceEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
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
		else if(referenceSetType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT)
		{
		#endif
			//cout << "aq1" << endl;
			//subject <- dummyRelationship <- dummyObject
			
			GIAentityNode* relationshipParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &relationshipParentEntity))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection1 = NULL;
			if(!findSameSentenceSubjectObjectEntityNodePointerInVector(relationshipParentEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, &parentEntityToPrimaryEntityConnection1, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				
				cerr << "currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << endl;
				cerr << "relationshipParentEntity = " << relationshipParentEntity->entityName << endl;
				GIAposRelTranslatorHybridReferenceSet.printSubReferenceSet(currentSubReferenceSetInList);

				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA" << endl;
				exit(EXIT_ERROR);
			}
			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			GIAentityNode* objectParentEntity = NULL;
			if(!findParentEntityBasedOnSentenceIndex(currentSubReferenceSetInList, translatorVariables, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT, &objectParentEntity))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findParentEntityBasedOnSentenceIndex GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			GIAentityConnection* parentEntityToPrimaryEntityConnection2 = NULL;
			if(!findSameSentenceEntityNodePointerInVector(objectParentEntity, GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			//OR: if(!findSameSentenceRelationshipEntityNodePointerInVector(objectParentEntity, false, &parentEntityToPrimaryEntityConnection2, currentSubReferenceSetInList->sentenceIndex))
			{
				result = false;
				cerr << "GIAposRelTranslatorHybridClass::findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections{} GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_TYPE_SUBJECT: !findSameSentenceEntityNodePointerInVector GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT" << endl;
				exit(EXIT_ERROR);
			}
			#endif
			
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection1);
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(relationshipParentEntity);	//disable if not already disabled
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_SUBJECT_AND_DUMMY_DELIMITER_AND_DUMMY_OBJECT
			deleteExternalConnectionsToConnection(parentEntityToPrimaryEntityConnection2);
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntity(objectParentEntity);	//disable if not already disabled
			#endif
			
			*primaryEntity = parentEntityToPrimaryEntityConnection1->entity;
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_SUBJECT_AND_DUMMY_DELIMITER_AND_OBJECT
		}
		#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
	}
	#endif
	
	return result;
}

bool GIAposRelTranslatorHybridClass::findParentEntityBasedOnSentenceIndex(LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity)
{
	bool result = false;
	GIAentityNode* entityFound = NULL;

	bool assumeAdvancedReferencing = false;
	#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING
	if(parentEntityName != GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA)
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


bool GIAposRelTranslatorHybridClass::findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex)
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

bool GIAposRelTranslatorHybridClass::transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(GIAentityNode* dummyDelimiterEntity, GIAentityNode* primaryDelimiterEntity)
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


bool GIAposRelTranslatorHybridClass::findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex)
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

bool GIAposRelTranslatorHybridClass::findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, const string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex)
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
bool GIAposRelTranslatorHybridClass::getRelationshipNameAndType(LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, int* relationshipEntityType, string* relationshipName)
{
	bool result = true; 
	
	/*
	algorithm for relationship type detection;
		if verb then action
			ignore: the move, the fast move, the very fast move
			eg tom moves the ball
			eg tom does move the ball [may be preceeded by an auxiliary]
			eg tom rides fast	[may be preceeded by an adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
			eg tom rides very fast	[may be preceeded by an adverb and adjective. object must refer to previous verb delimiter: delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB]
		else if preposition then condition
			eg tom is near the ball 	[may be preceeded by an auxiliary]
			eg tom is certainly near the ball [may be preceeded by an auxiliary and adverb]
			eg tom rides the bike near the ball 	[may not be preceeded by an auxiliary, in which case the condition refers to {ie is subsequently linked by GIA to} the preceeding referenceSetDelimiter (eg verb): delimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB]
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
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;	//CHECKTHIS - can still use POSprelim during post processing (perhaps better to use POS)?
	#else
	bool usePOSprelim = false;
	#endif
	
	LRPpreprocessorPlainTextWord* relationshipWord = (relationshipReference->subReferenceSetContents).back();
	*relationshipName = relationshipWord->tagName;
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "relationshipName = " << *relationshipName << endl;
	cout << "relationshipReference->delimiterType = " << relationshipReference->delimiterType << endl;
	#endif
	
	if(relationshipReference->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)	//GIAposRelTranslatorHybridReferenceSet.detectAuxiliary(*relationshipName)
	{
	}
	else if(relationshipReference->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
	}
	else if(relationshipReference->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)
	{
		*relationshipEntityType = GIA_ENTITY_TYPE_CONDITION;		
	}
	
	if(relationshipReference->delimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY)
	{	
		LRPpreprocessorPlainTextWord* firstWordAfterAuxiliary = NULL;
		LRPpreprocessorPlainTextWord* secondWordAfterAuxiliary = NULL;
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
			cerr << "GIAposRelTranslatorHybridClass::getRelationshipNameAndType{} error: (relationshipObject == NULL)" << endl;
			exit(EXIT_ERROR);
		}
		
		if(hasFirstWordAfterAuxiliary)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			//cout << "firstWordAfterAuxiliary = " << firstWordAfterAuxiliary << endl;
			#endif
			if(LRPpreprocessorWordIdentification.determineIsAuxiliaryBeing(relationshipWord, usePOSprelim))
			{
				if(LRPpreprocessorWordIdentification.determineIsAdjective(firstWordAfterAuxiliary, usePOSprelim) || (hasSecondWordAfterAuxiliary && LRPpreprocessorWordIdentification.determineIsAdverb(firstWordAfterAuxiliary, usePOSprelim) && LRPpreprocessorWordIdentification.determineIsAdjective(secondWordAfterAuxiliary, usePOSprelim)))
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;
					//FUTURE GIA: if GIA_ENTITY_TYPE_PROPERTY detected via determineIsAdjective(firstWordAfterAuxiliary)||determineIsAdjective(secondWordAfterAuxiliary), then need to set the relationshipEntityObject to GIA_ENTITY_TYPE_QUALITY also
				}
				else
				{
					*relationshipEntityType = GIA_ENTITY_TYPE_DEFINITION;	
				}
			}
			if(LRPpreprocessorWordIdentification.determineIsAuxiliaryHaving(relationshipWord, usePOSprelim))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_PROPERTY;		
			}
			if(LRPpreprocessorWordIdentification.determineIsAuxiliaryDoing(relationshipWord, usePOSprelim))
			{
				*relationshipEntityType = GIA_ENTITY_TYPE_ACTION;
			}
		}
		else
		{
			cerr << "GIAposRelTranslatorHybridClass::getRelationshipNameAndType{} error : !hasFirstWordAfterAuxiliary" << endl;
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



	
bool GIAposRelTranslatorHybridClass::getPreprocessorSentenceSubReferenceSet(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const int sentenceIndex, constEffective LRPpreprocessorSubReferenceSet** subReferenceSetFound)
{
	bool result = false;
	
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		if(getPreprocessorSentenceSubReferenceSet(currentLRPpreprocessorSentenceInList->firstLogicReferenceInList, sentenceIndex, subReferenceSetFound))
		{
			result = true;
		}

		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	
	return result;
}
bool GIAposRelTranslatorHybridClass::getPreprocessorSentenceSubReferenceSet(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, const int sentenceIndex, constEffective LRPpreprocessorSubReferenceSet** subReferenceSetFound)
{		
	bool result = false;
	
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInList;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(getPreprocessorSentenceSubReferenceSet(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor, sentenceIndex, subReferenceSetFound))
					{
						result = true;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
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

			LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
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
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
			if(currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter->sentenceIndex == sentenceIndex)
			{
				*subReferenceSetFound = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter;
				result = true;
			}
			#endif

		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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

GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	
{
	return createNewRelationshipAndConnectToSource(sourceEntity, logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, sameReferenceSet, translatorVariables);
}

GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables)	//OLD: this will create an intermediary action/possession("have") node if necessary
{
	GIAentityNode* relationshipEntity = createNewRelationship(relationshipReference, relationshipObject, translatorVariables);
	GIAtranslatorOperations.connectLogicReferenceRelationshipToSource(relationshipEntity, sourceEntity, sameReferenceSet, translatorVariables);
	return relationshipEntity;
}


GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationship(GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables)
{
	return createNewRelationship(logicReferenceVariable->referenceSetDelimiter, logicReferenceVariable->referenceSetObject, translatorVariables);
}
GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationship(LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables)
{
	//FUTURE: upgrade createNewRelationship:getRelationshipNameAndType to detect auxiliary tense information, and any associated adverb/adjectives within relationshipReference [else use GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS: this is currently required to process delimiter (eg being/having/verb/preposition) tense, adverbs (eg very near), and adjectives (eg rides fast) using NLP]

	int relationshipEntityType = GIA_ENTITY_TYPE_UNDEFINED;
	string relationshipName = "";
	getRelationshipNameAndType(relationshipReference, relationshipObject, &relationshipEntityType, &relationshipName);
	GIAentityNode* relationshipEntity = createNewRelationshipEntity(relationshipName, relationshipEntityType, translatorVariables);
	return relationshipEntity;
}

#endif



GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationshipEntity(vector<LRPpreprocessorPlainTextWord*>* logicReferenceContents, int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	GIAentityNode* relationshipEntity = NULL;
	if(logicReferenceContents->size() == 1)
	{
		relationshipEntity = createNewRelationshipEntity(LRPpreprocessorWordClassObject.generateTextFromVectorWordList(logicReferenceContents), relationshipEntityType, translatorVariables);
	}
	else
	{
		cerr << "GIAposRelTranslatorHybridClass::createNewRelationshipEntity{} error: (logicReferenceContents->size() != 1)" << endl;
		exit(EXIT_ERROR);
	}

	return relationshipEntity;
}

GIAentityNode* GIAposRelTranslatorHybridClass::createNewRelationshipEntity(string relationshipEntityName, int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables)
{
	/*
	//NB see findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificial{} / GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation{}:findOrAddNetworkIndexEntityByNameSimpleWrapperRelationship{};
	//a new entity index is not required to be created because connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities is currently executed after GIAtranslator (or at least after the sentence has been parsed by GIAtranslator)
	functionEntityIndex3 = currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	currentSentenceInList->relationshipEntityArtificialIndexCurrent = param->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;
	*/
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial(relationshipEntityType, relationshipEntityName, translatorVariables);	
	#else
	GIAentityNode* relationshipEntity = GIAtranslatorOperations.addEntityNodeByNameSimpleWrapperRelationshipArtificial2(relationshipEntityType, relationshipEntityName, translatorVariables);
	#endif
	
	relationshipEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	
	return relationshipEntity;
}


#endif





