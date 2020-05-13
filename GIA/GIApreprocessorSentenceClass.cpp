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
 * File Name: GIApreprocessorSentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k13a 11-May-2020
 * Requirements: requires plain text file
 * Description: Preprocessor Sentence Class
 * /
 *******************************************************************************/


#include "GIApreprocessorSentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"

GIApreprocessorSentence::GIApreprocessorSentence(void)
{
	sentenceIndexOriginal = 0;	
	sentenceContentsOriginalText = "";
	//#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	//sentenceContentsOriginal = "";
	//sentenceContentsLRP = "";
	//#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	hasLogicReference = false;
	firstLogicReferenceInList = new GIAposRelTranslatorHybridLogicReference();
	//logicReferenceTotal = 0;
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	sentenceIndex = INT_DEFAULT_VALUE;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	sentenceReferencePrelim = NULL;
	#endif	
	#else
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	sentenceReference = NULL;
	#endif	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES
	firstParseTreeGroup = NULL;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION
	indentation = 0;
	#endif		

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
	parseIsolatedSubreferenceSets = false;
	parseIsolatedSubreferenceSetsOnly = false;
	parseIsolatedSubreferenceSetsFirst = false;
	#endif	
	
	next = NULL;
}
GIApreprocessorSentence::~GIApreprocessorSentence(void)
{
	/*
	//doesn't work because word->next pointers in words will have changed based on lrp
	if(sentenceContentsOriginal.size() > 0)
	{
		delete sentenceContentsOriginal[0];
	}
	*/
	for(vector<GIApreprocessorPlainTextWord*>::iterator it = sentenceContentsOriginal.begin(); it != sentenceContentsOriginal.end(); it++)
	{
		GIApreprocessorPlainTextWord* currentWord = *it;
		GIApreprocessorMultiwordReductionPlainTextWord* plaintextWord = static_cast<GIApreprocessorMultiwordReductionPlainTextWord*>(currentWord);	//require to cast back to plaintextword to ensure that all memory is deleted	//CHECKTHIS
		currentWord->nextTag = NULL;	//prevents future words from being deleted
		delete plaintextWord;
	}
	sentenceContentsOriginal.clear();
	sentenceContentsLRP.clear();
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	delete firstLogicReferenceInList;
	#endif
	
	if(next != NULL)
	{
		delete next;
	}
}

void GIApreprocessorSentenceClass::copyPreprocessorSentence(GIApreprocessorSentence* sentenceToCopy, GIApreprocessorSentence* newSentence)
{
	*newSentence = *sentenceToCopy;
}

vector<GIApreprocessorPlainTextWord*>* GIApreprocessorSentenceClass::getSentenceContents(GIApreprocessorSentence* currentGIApreprocessorSentenceInList)
{
	return  &(currentGIApreprocessorSentenceInList->sentenceContentsLRP);
}

#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
bool GIApreprocessorSentenceClass::calculateParseIsolatedSubreferenceSets1(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, bool* parseIsolatedSubreferenceSets1)
{
	bool result = true;
	if((currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSets) && ((currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSetsFirst) || (currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSetsOnly)))
	{
		*parseIsolatedSubreferenceSets1 = true;
	}
	return result;
}
bool GIApreprocessorSentenceClass::calculateParseIsolatedSubreferenceSets2(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, bool* parseIsolatedSubreferenceSets2)
{
	bool result = false;
	if(currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSets)
	{
		if(!(currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSetsOnly))
		{
			result = true;
			if(currentGIApreprocessorSentenceInList->parseIsolatedSubreferenceSetsFirst)
			{
				parseIsolatedSubreferenceSets2 = false;
			}
		}
	}			
	return result;
}
#endif	

void GIApreprocessorSentenceClass::printSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool error)
{
	if(error)
	{
		cerr << "sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(sentenceContents) << endl;
	}
	else
	{
		cout << "sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(sentenceContents) << endl;
	}
	
	/*
	for(int w=0; w<sentenceContents->size(); w++)
	{
		cout << sentenceContents->at(w)->tagName << " ";
	}
	cout << endl;
	*/
}








