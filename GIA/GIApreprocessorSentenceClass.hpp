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
 * File Name: GIApreprocessorSentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f10d 19-April-2018
 * Requirements: requires plain text file
 * Description: Preprocessor Sentence Class
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS
#define HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "GIAentityNodeClass.hpp"	//required for primaryEntityTemp, GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIAsentenceClass.hpp"	//required for GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIApreprocessorWordClass.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
#include "GIAtxtRelTranslatorHybridSentenceClass.hpp"
#endif
#ifdef GIA_TXT_REL_TRANSLATOR_RULES
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#endif


class GIApreprocessorSentence
{
public:
	GIApreprocessorSentence(void);
	~GIApreprocessorSentence(void);

	public: void copyPreprocessorSentence(GIApreprocessorSentence* sentenceToCopy, GIApreprocessorSentence* newSentence);

	int sentenceIndexOriginal;
	string sentenceContentsOriginalText;
	//#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	vector<GIApreprocessorPlainTextWord*> sentenceContentsOriginal;
	vector<GIApreprocessorPlainTextWord*> sentenceContentsLRP;
	//#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	bool hasLogicReference;	//if false, then firstLogicReferenceInList will only have a single GIAtxtRelTranslatorHybridLogicReference
	GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList;
	//int logicReferenceTotal;
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	int sentenceIndex;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	GIAsentence* sentenceReferencePrelim;		//temporary GIA sentence data (POS tags) required for execution of GIA preprocessor
	#endif	
	#else
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAsentence* sentenceReference;
	#endif	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup;
	#endif
	#ifdef GIA_PREPROCESSOR_WORD_EXTRACT_INDENTATION
	int indentation;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
	bool parseIsolatedSubreferenceSets;
	bool parseIsolatedSubreferenceSetsOnly;
	bool parseIsolatedSubreferenceSetsFirst;
	#endif

	GIApreprocessorSentence* next;
};


#endif
