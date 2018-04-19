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
 * File Name: GIAtxtRelTranslatorHybridLogicReference.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f4b 14-April-2018
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Logic Reference
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorHybridLogicReference.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
#include "GIAtxtRelTranslatorHybridReferenceSet.hpp"
#endif
#include "GIAsynRelTranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE

//Limitation: doesn't support conjunctions with embedded preposition logic references, eg; "The chicken is green, considering the teapot, red, considering the apple, or blue, considering the pie."
bool GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor(const vector<GIApreprocessorPlainTextWord*>* sentenceContentsWordList, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers)
{
	bool result = true;
	
	//OLD:  Separate out 1. ,/and/or (including doactionA and doactionB),

	int wordIndexSentence = 0;	//this gets reset at next sentence
	int logicReferenceVariableNameIndex = 0;
	vector<GIApreprocessorPlainTextWord*> logicReferenceVariableWordList;	//this gets reset after each logic reference variable
	bool expectToFindSubjectAuxObjectLogicReferenceVariable = false;
	bool whiteSpace = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
	bool grammaticallyStrict = true;	//this is irrelevant and could be set to GIA_TXT_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT as determineIsWordType(usePOSprelim=true, ...) assumes grammaticallyStrict
	#else
	bool grammaticallyStrict = false;
	#endif
		
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = currentGIApreprocessorSentenceInList->firstLogicReferenceInList;
	for(int w=0; w<sentenceContentsWordList->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (*sentenceContentsWordList)[w];
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		cout << "[currentWord = " << currentWord->tagName << "]" << endl;
		#endif
		bool foundClassType = false;
		string logicReferenceClassType = "";
		int logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED;
		if(GIAtxtRelTranslatorRules.isClassTag(currentWord->tagName, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction, &logicReferenceClassType, GIAtxtRelTranslatorRulesTokenLayers))
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(GIApreprocessorMultiwordReduction.determineIsConjunction(currentWord, usePOSprelim))
			{
			#endif
				logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				cout << "logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION " << endl;
				#endif
				foundClassType = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}
		else if(GIAtxtRelTranslatorRules.isClassTag(currentWord->tagName, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition, &logicReferenceClassType, GIAtxtRelTranslatorRulesTokenLayers))
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(GIApreprocessorMultiwordReduction.determineIsPreposition(currentWord, usePOSprelim))
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
				if(GIAtxtRelTranslatorHybridReferenceSet.verifyIsPrepositionNotProgressiveVerbBasedOnContext(sentenceContentsWordList, w, usePOSprelim))
				{
				#endif
					logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
					cout << "logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION " << endl;
					#endif
					foundClassType = true;
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
				}
				#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}
		else if(GIAtxtRelTranslatorRules.isClassTag(currentWord->tagName, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb, &logicReferenceClassType, GIAtxtRelTranslatorRulesTokenLayers))
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(GIApreprocessorMultiwordReduction.determineIsVerb(currentWord, usePOSprelim, grammaticallyStrict))
			{
			#endif
				logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				cout << "logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB " << endl;
				#endif
				foundClassType = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}

		if(foundClassType)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			cout << "currentWord = " << currentWord->tagName << endl;
			cout << "logicReferenceClass = " << logicReferenceClass << endl;
			#endif

			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
			vector<GIApreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContents(currentWord, logicReferenceClassType, logicReferenceClass, &logicReferenceVariableWordList);
			#else
			vector<GIApreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContentsBase(currentWord, logicReferenceClassType);
			#endif

			if(logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
			{
				if(logicReferenceVariableWordList.size() == 0)
				{							
					if((currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
					{
						//x, and
						//eg [The house is blue], and the chicken is happy.
						//eg Tom said A, and proposed that the chicken is happy.
					}
					else
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS
						//eg subsequent_to and before (FUTURE: and -> subsequent_to + before)
						if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
						{
							result = false;
						}
						#else
						cerr << "GIApreprocessor::executeLogicReferencePreprocessor{} error: GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS not supported; eg subsequent_to and before" << endl;
						exit(EXIT_ERROR);
						#endif
					}
				}
				else
				{	
					//x and
					//eg [The house is blue] and the chicken is happy.
					//?eg [The house is blue, red] and green.
					if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
					{
						result = false;
					}

					bool copySubLogicReferences = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
					if(currentLogicReferenceInList->isSubLogicReferenceDependent && currentLogicReferenceInList->lastLogicReferenceInUpperLevel->previous != NULL)
					{
						currentLogicReferenceInList = currentLogicReferenceInList->lastLogicReferenceInUpperLevel;		//The apple proposed A and proposed B...	//The apple proposed A and the chicken proposed B...	//Tom proposed that the apple is blue, and suggested that the house is purple.
					}
					#endif

					if(currentLogicReferenceInList->previous == NULL)
					{
						initialiseNewSubLogicReferenceConjunction(&currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence, copySubLogicReferences);
					}
					if(!initialiseNextLogicReferenceConjunction(&currentLogicReferenceInList, wordIndexSentence))
					{
						result = false;
					}
				}

				currentLogicReferenceInList->logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT;	//this might be overwritten as a verb/preposition logic reference
				if(!identifyImplicitLogicReferenceSets(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType))
				{
					result = false;
				}
			}
			else if((logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) || (logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION))
			{	
				GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInListActive = currentLogicReferenceInList;
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
				GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInListActiveOriginal = currentLogicReferenceInListActive;
				int currentLogicReferenceInListActiveOriginalClass = currentLogicReferenceInListActive->logicReferenceClass;
				#endif

				bool specialSwitch = false;
				if(logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
				{
					if((currentLogicReferenceInList->previous != NULL) && (currentLogicReferenceInList->previous->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						cout << "specialSwitch" << endl;
						#endif
	
						currentLogicReferenceInListActive = currentLogicReferenceInList->previous->lastLogicReferenceInUpperLevel;
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						currentLogicReferenceInListActiveOriginal = currentLogicReferenceInListActive;
						currentLogicReferenceInListActiveOriginalClass = currentLogicReferenceInListActive->logicReferenceClass;
						//cout << "1 currentLogicReferenceInListActiveOriginalClass = " << currentLogicReferenceInListActiveOriginalClass << endl;
						#endif

						//eg The house is blue, considering the pie.	[make A (The house is blue) a sub logic reference dependent of logic reference]
						//eg The house is blue, considering that the pie is happy.	[make A (The house is blue) a sub logic reference dependent of logic reference]
						specialSwitch = true;
						if(!initialiseNewSubLogicReference(currentLogicReferenceInListActive, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
						{
							result = false;
						}

						currentLogicReferenceInListActive->firstSubLogicReferenceInListDependent->logicReferenceVariable = currentLogicReferenceInListActive->firstSubLogicReferenceInListArray->logicReferenceVariable;
						currentLogicReferenceInListActive->firstSubLogicReferenceInListArray = NULL;
					}
					#ifndef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					else
					{
						if(logicReferenceVariableWordList.size() != 0)
						{								
							//eg The house is blue considering the apple.	(add 'The house is blue' to the currentLogicReferenceInList)
							
							specialSwitch = true;
							if(!initialiseNewSubLogicReference(currentLogicReferenceInListActive, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))	//NB logicReferenceContents is not used here as logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB
							{
								result = false;
							}
						
							if(!fillLogicReferenceVariable(currentLogicReferenceInListActive->firstSubLogicReferenceInListDependent, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
							{
								result = false;
							}
						}
					}
					#endif
				}
				if(!specialSwitch)
				{	
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
					//cout << "!specialSwitch" << endl;
					#endif

					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
					if(currentLogicReferenceInList->previous != NULL)
					{		
						//The apple proposed A, the chicken proposed B...	/	The apple proposed A, said/proposed B...	/ The apple proposed A, B...
										
						GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList = currentLogicReferenceInListActive->previous;		//proposed
						firstLogicReferenceInList->next = NULL;	//delete the original logic reference as it will contain nothing
						if(firstLogicReferenceInList->previous != NULL)
						{
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->previous != NULL)" << endl;
							exit(EXIT_ERROR);
						}
						if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel == NULL)
						{
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->lastLogicReferenceInUpperLevel == NULL)" << endl;
							exit(EXIT_ERROR);
						}
						if(firstLogicReferenceInList->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)
						{
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
							exit(EXIT_ERROR);
						}

						GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReferenceOld = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;	//GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED
						if(conjunctionLogicReferenceOld->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
						{
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (conjunctionLogicReferenceOld->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)" << endl;
							exit(EXIT_ERROR);
						}

						GIAtxtRelTranslatorHybridLogicReference* verbLogicReference = conjunctionLogicReferenceOld->lastLogicReferenceInUpperLevel;	//GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION
						verbLogicReference->firstSubLogicReferenceInListDependent = conjunctionLogicReferenceOld->firstSubLogicReferenceInListArray;	//proposed	//delete the old conjunctionLogicReferenceOld	

						if(verbLogicReference->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
						{
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (verbLogicReference->logicReferenceClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)" << endl;
							exit(EXIT_ERROR);
						}

						bool copySubLogicReferences = true;
						if(!initialiseNewSubLogicReferenceConjunctionInferred(&verbLogicReference, wordIndexSentence, copySubLogicReferences))
						{
							result = false;
						}
						GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReferenceNew = verbLogicReference->lastLogicReferenceInUpperLevel;
						setLogicReferenceInfo(conjunctionLogicReferenceNew, conjunctionLogicReferenceOld->logicReferenceContents, conjunctionLogicReferenceOld->logicReferenceClass, conjunctionLogicReferenceOld->logicReferenceClassType);	  

						if(!initialiseNextLogicReferenceConjunction(&verbLogicReference, wordIndexSentence))
						{
							result = false;
						}

						currentLogicReferenceInListActive = verbLogicReference;
					}
					#endif

					if(!initialiseNewSubLogicReference(currentLogicReferenceInListActive, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))
					{
						result = false;
					}
				}

				if(logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
				{
					if(logicReferenceVariableWordList.size() != 0)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						if((currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT))
						{
						#endif
							/*
							//eg The apple proposed [that] the house [is blue].
							//FUTURE CONSIDER eg The house is blue, the apple proposed.
							//Tom said A and Jim proposed [that] the house [is blue].
							*/
							/*
							if(expectToFindSubjectAuxObjectLogicReferenceVariable != false)
							{
								cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: expectToFindSubjectAuxObjectLogicReferenceVariable != false" << endl;
								exit(EXIT_ERROR);
							}
							*/
							expectToFindSubjectAuxObjectLogicReferenceVariable = false;
							if(!fillLogicReferenceVariable(currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
							{
								result = false;
							}
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						}
						else
						{
							//eg Subsequent_to proposed that the house is blue.
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (currentLogicReferenceInListActiveOriginalClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED||GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT)" << endl;
							cerr << "currentLogicReferenceInListActiveOriginal->logicReferenceClass = " << currentLogicReferenceInListActiveOriginal->logicReferenceClass << endl;
							exit(EXIT_ERROR);
						}
						#endif
					}
					else
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
						currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor = NULL;
						#endif
						#else
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						if((currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT) || (currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
						{
						#endif
							/*
							//Tom said A and proposed [that] the house [is blue].
							//Tom said A, and proposed [that] the house [is blue].
							//Tom proposed A, suggested B, etc
							*/
						#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						}
						else
						{	
							//eg The house is blue suggested the apple.
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (logicReferenceVariableWordList->size() == 0) && !(currentLogicReferenceInListActiveOriginal->logicReferenceConjunctionClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT || GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
							exit(EXIT_ERROR);
						}
						#endif
						#endif
					}
					currentLogicReferenceInList = currentLogicReferenceInListActive->firstSubLogicReferenceInListDependent;
				}
				else if(logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
					if((currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION))
					{
					#endif
					#endif
						if(logicReferenceVariableWordList.size() == 0)
						{
							/*
							//eg The house is blue, considering the pie.
							//eg Considering the pie, the house is blue.
							//eg The house is blue, considering that the pie is happy.
							//eg Considering that the pie is happy, the house is blue.
							*/
							/*
							//eg Tom said A and considering [that] the house [is blue], the apple is red.
							//eg Tom said A, and considering [that] the house [is blue], the apple is red. 
							*/
						}
						else
						{
							//eg The house is blue considering the pie.
							#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
							cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && (logicReferenceVariableWordList->size() != 0)" << endl;
							cerr << "currentWord->tagName = " << currentWord->tagName << endl;
							GIApreprocessorMultiwordReductionClassObject.printWordList(&logicReferenceVariableWordList);
							exit(EXIT_ERROR);
							#endif
						}
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
					}
					else
					{
						//eg the house is blue considering the pie.
						cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && !((currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION))" << endl;
						cerr << "currentLogicReferenceInListActiveOriginalClass = " << currentLogicReferenceInListActiveOriginalClass << endl;
						exit(EXIT_ERROR);
					}
					#endif
					#endif

					currentLogicReferenceInList = currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor;
				}

				if((w+1 < sentenceContentsWordList->size()) && ((*sentenceContentsWordList)[w+1]->tagName == GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME))
				{
					w++;	//skip to end of that
					expectToFindSubjectAuxObjectLogicReferenceVariable = true;
				}
				else
				{
					expectToFindSubjectAuxObjectLogicReferenceVariable = false;	//redundant?
				}	
			}
		}
		else if(currentWord->tagName == SHAREDvars.convertCharToString(GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR))
		{	
			if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
			{
				result = false;
			}
				
			if((currentLogicReferenceInList->isSubLogicReferenceGovernor) && logicReferenceClassIsPreposition(currentLogicReferenceInList->lastLogicReferenceInUpperLevel))
			{
				//eg Considering the pie, the apple is blue.	considering(the pie, the apple is blue)
				currentLogicReferenceInList = currentLogicReferenceInList->lastLogicReferenceInUpperLevel->firstSubLogicReferenceInListDependent;
			}
			else
			{
				//eg The house is red, the apple is blue [, and the ferry is green]	the house is red [implicit conj], the apple is blue [implicit conj], the ferry is green [and] -> and(the house is red, the apple is blue, the ferry is green)
				//eg The apple proposed A, B, and C	and(proposed(apple, A), B, C)	!GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR: proposed(apple, and(A, B, C))
				//eg Tom proposed that the apple is blue, green, and yellow.	and(proposed(Tom, the apple is blue), green, yellow)	!GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR: proposed(Tom, and(apple is blue, green, yellow))
				//eg Tom proposed that the apple is blue, said that the orange is green, and suggested that the pie is yellow.
				//FUTURE? Tom proposed that the apple is blue, said that the orange is green, and suggested that the pie is yellow, red, and purple.	and(proposed(Tom, the apple is blue), said(, orange is green), suggested(, and(pie is yellow, red, purple))	or?	and(proposed(Tom, the apple is blue), said(, orange is green), and(suggested(, pie is yellow, red, purple))
				//FUTURE? Considering that the house is red, blue, and green, the monkey is sad.
				//FUTURE? The monkey is sad, considering that the house is red, blue, and green.
				
				bool copySubLogicReferences = false;
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
				if(currentLogicReferenceInList->isSubLogicReferenceDependent && currentLogicReferenceInList->lastLogicReferenceInUpperLevel->previous != NULL)
				{
					currentLogicReferenceInList = currentLogicReferenceInList->lastLogicReferenceInUpperLevel;		//The apple proposed A, the chicken proposed B...	/	The apple proposed A, said/proposed B...
				}
				#endif
				
				if(currentLogicReferenceInList->previous == NULL)
				{
					//insert a new conjunction logic reference
					initialiseNewSubLogicReferenceConjunctionInferred(&currentLogicReferenceInList, wordIndexSentence, copySubLogicReferences);
				}
				if(!initialiseNextLogicReferenceConjunction(&currentLogicReferenceInList, wordIndexSentence))
				{
					result = false;
				}
				currentLogicReferenceInList->logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED;
			}
		}
		else if(((currentWord->tagName).length() == 1) && SHAREDvars.charInCharArray((currentWord->tagName)[0], nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
		{
			if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
			{
				result = false;
			}
		}
		else if(((currentWord->tagName).length() == 1) && SHAREDvars.charInCharArray((currentWord->tagName)[0], nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
		{
			//CHAR_SEMICOLON / CHAR_COLON:
			logicReferenceVariableWordList.push_back(currentWord);	//CHECKTHIS: assume that logic reference variable will not occur before a punctuation mark (CHAR_SEMICOLON / CHAR_COLON)
		}
		else
		{
			logicReferenceVariableWordList.push_back(currentWord);
		}

		wordIndexSentence++;
	}
	
	return result;
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::logicReferenceClassIsPreposition(GIAtxtRelTranslatorHybridLogicReference* logicReference)
{
	bool result = false;
	//if(SHAREDvars.textInTextArray(logicReference->logicReferenceClassType, GIAtxtRelTranslatorHybridLogicReferenceClassPrepositionTypes, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM))
	if(logicReference->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
	{
		result = true;
	}
	return result;
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::initialiseNextLogicReferenceConjunction(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence)
{
	bool result = true;
	
	(*currentLogicReferenceInList)->next = new GIAtxtRelTranslatorHybridLogicReference();
	(*currentLogicReferenceInList)->next->previous = (*currentLogicReferenceInList);
	(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->next;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	(*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence = wordIndexSentence+1;	//CHECKTHIS
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReferenceConjunctionInferred(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence, const bool copySubLogicReferences)
{
	vector<GIApreprocessorPlainTextWord*> logicReferenceContentsEmpty;
	return initialiseNewSubLogicReferenceConjunction(currentLogicReferenceInList, logicReferenceContentsEmpty, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION, "", wordIndexSentence, copySubLogicReferences);
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReferenceConjunction(GIAtxtRelTranslatorHybridLogicReference** currentLogicReferenceInList, const vector<GIApreprocessorPlainTextWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence, const bool copySubLogicReferences)
{
	GIAtxtRelTranslatorHybridLogicReferenceVariable* currentLogicReferenceVariableInListTemp = (*currentLogicReferenceInList)->logicReferenceVariable;
					
	(*currentLogicReferenceInList)->hasSubLogicReference = true;
	(*currentLogicReferenceInList)->hasSubLogicReferenceArray = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray = new GIAtxtRelTranslatorHybridLogicReference();
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->isSubLogicReferenceArray = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->lastLogicReferenceInUpperLevel = (*currentLogicReferenceInList);  //so that can return to the current level upon the detection of a comma
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
	if(copySubLogicReferences)
	{
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->hasSubLogicReference = (*currentLogicReferenceInList)->hasSubLogicReference;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceClass = (*currentLogicReferenceInList)->logicReferenceClass;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceClassType = (*currentLogicReferenceInList)->logicReferenceClassType;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceContents = (*currentLogicReferenceInList)->logicReferenceContents;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->firstSubLogicReferenceInListGovernor = (*currentLogicReferenceInList)->firstSubLogicReferenceInListGovernor;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->firstSubLogicReferenceInListDependent = (*currentLogicReferenceInList)->firstSubLogicReferenceInListDependent;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListGovernor = NULL;
		(*currentLogicReferenceInList)->firstSubLogicReferenceInListDependent = NULL;
	}
	#endif
	setLogicReferenceInfo((*currentLogicReferenceInList), logicReferenceContents, logicReferenceClass, logicReferenceClassType);	  
	/*
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	(*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence = wordIndexSentence + GIA_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	#endif
	*/
	(*currentLogicReferenceInList)->logicReferenceVariable = (*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceVariable;	//ie = new logicReferenceVariable
	(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->firstSubLogicReferenceInListArray;
	(*currentLogicReferenceInList)->logicReferenceVariable = currentLogicReferenceVariableInListTemp;
}



bool GIAtxtRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReference(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, const vector<GIApreprocessorPlainTextWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence)
{
	setLogicReferenceInfo(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType);
	currentLogicReferenceInList->hasSubLogicReference = true;
	currentLogicReferenceInList->hasSubLogicReferenceArray = false;
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor = new GIAtxtRelTranslatorHybridLogicReference();
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent = new GIAtxtRelTranslatorHybridLogicReference();
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->isSubLogicReferenceGovernor = true;
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->lastLogicReferenceInUpperLevel = currentLogicReferenceInList;  //so that can return to the current level upon the detection of a comma
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent->isSubLogicReferenceDependent = true;
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent->lastLogicReferenceInUpperLevel = currentLogicReferenceInList; //so that can return to the current level upon the detection of a comma
	/*
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = wordIndexSentence + GIA_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	#endif
	*/
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::fillLogicReferenceVariable(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<GIApreprocessorPlainTextWord*>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool* expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence)
{
	//exit parsing of logical condition variable
	
	bool result = true;
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = wordIndexSentence - logicReferenceVariableWordList->size() + GIA_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	//cout << "currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = " << currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence << endl;
	#endif
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents = *logicReferenceVariableWordList;
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex;
	string logicReferenceVariableName = GIAtxtRelTranslatorHybridLogicReferenceVariableNames[*logicReferenceVariableNameIndex];
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName = logicReferenceVariableName;
	*logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex + 1;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	if(!GIAtxtRelTranslatorHybridReferenceSet.executeReferenceSetPreprocessor(logicReferenceVariableWordList, currentLogicReferenceInList->logicReferenceVariable, *expectToFindSubjectAuxObjectLogicReferenceVariable, currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence))
	{
		result = false;	
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
	if(islogicReferenceExplitOrImplicitConjunctionComponent(currentLogicReferenceInList))
	{
		if(GIAtxtRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
		{
			if(!GIAtxtRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				//eg Tom is happy and fast 
				//switch reference set subject/object
				GIApreprocessorSubReferenceSet* tempSubReferenceSet = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
				currentLogicReferenceInList->logicReferenceVariable->referenceSetObject = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
				currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject = tempSubReferenceSet;
			}
			else
			{
				//eg Tom is happy and Tom is fast / Tom is happy and is fast
			}
		}
		else
		{
			cerr << "GIApreprocessor::fillLogicReferenceVariable{} error: islogicReferenceConjunctionAndOr && isReferenceSetEmpty(referenceSetSubject)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	#endif
	
	*expectToFindSubjectAuxObjectLogicReferenceVariable = false;

	logicReferenceVariableWordList->clear();
	
	return result;
}



//note this function will not work if some logicReferences have class type preposition/conjunction
//assume that the explicitConjunctionLogicReferenceInList refers to the implicit conjunction list on the next (nearest) highest level
bool GIAtxtRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets(GIAtxtRelTranslatorHybridLogicReference* explicitConjunctionLogicReferenceInList, const vector<GIApreprocessorPlainTextWord*> logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType)
{
	bool result = false;
	
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = explicitConjunctionLogicReferenceInList;
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES 
	while(currentLogicReferenceInList->previous != NULL)
	{
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
	}
	#else
	while(currentLogicReferenceInList->previous != NULL)
	{
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
		
		if(currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)
		{
			currentLogicReferenceInList->logicReferenceClass = GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT_IMPLICIT_CONFIRMED;
			currentLogicReferenceInList->logicReferenceClassType = explicitConjunctionLogicReferenceInList->logicReferenceConjunctionClassType;
			result = true;
		}
		else
		{
			cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets{} error; (currentLogicReferenceInList->logicReferenceConjunctionClass != GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInLayer = currentLogicReferenceInList;
	if(firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAtxtRelTranslatorHybridLogicReference* conjunctionLogicReference = firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel;
		setLogicReferenceInfo(conjunctionLogicReference, logicReferenceContents, logicReferenceClass, logicReferenceClassType);
	}
	else
	{
		cerr << "GIAtxtRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets{} error; (firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel === NULL)" << endl;
		exit(EXIT_ERROR);	
	}
	
	return result;	
}
	
void GIAtxtRelTranslatorHybridLogicReferenceClass::setLogicReferenceInfo(GIAtxtRelTranslatorHybridLogicReference* logicReference, const vector<GIApreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType)
{
	logicReference->logicReferenceContents = logicReferenceContents;
	logicReference->logicReferenceClass = logicReferenceClass;
	logicReference->logicReferenceClassType = logicReferenceClassType;	
}		

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
vector<GIApreprocessorPlainTextWord*> GIAtxtRelTranslatorHybridLogicReferenceClass::generateLogicReferenceContents(GIApreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType, int logicReferenceClass, vector<GIApreprocessorPlainTextWord*>* logicReferenceVariableWordList)
{
	bool referenceSetDelimiterIndicatesSameReferenceSet = false;
	
	vector<GIApreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContentsBase(logicReferenceWord, logicReferenceClassType);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	
	if(logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
	{
		//eg had proposed / will have proposed, etc

		int wordIndex = logicReferenceVariableWordList->size();
		string currentWord = logicReferenceWord->tagName;


		//extract from executeReferenceSetPreprocessor;
		int wordIndexOfHypotheticalPreceedingThatWhich = wordIndex-1;

		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "currentWordIsReferenceSetDelimiter: currentWord = " << currentWord << endl;
		#endif

		bool previousWordIsModalAuxiliary = false;

		//verify that the auxiliary/verb is not preceeded by a modal auxiliary (e.g. for future cases; will be/have/ride), in which case must test the word prior to the modal auxiliary for that/which
		if(wordIndex-1 >= 0)
		{
			if(GIApreprocessorMultiwordReduction.detectModalAuxiliary((*logicReferenceVariableWordList)[wordIndex-1], usePOSprelim))
			{	
				previousWordIsModalAuxiliary = true;
				wordIndexOfHypotheticalPreceedingThatWhich--;
			}
		}
		//verify that the auxiliary/verb is not preceeded by an auxiliary (e.g. for doing auxiliaries; is being/having/doing, or for verbs; is riding, or for prepositions; is near), in which case must test the word prior to the modal auxiliary for that/which
		if(wordIndex-1 >= 0)
		{
			if(GIApreprocessorMultiwordReduction.detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
			{	
				//eg that is riding
				previousWordIsModalAuxiliary = true;
				wordIndexOfHypotheticalPreceedingThatWhich--;

				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "previousWordIsModalAuxiliary" << endl;
				cout << "currentWord = " << currentWord << endl;
				cout << "previousWord = " << (*logicReferenceVariableWordList)[wordIndex-1]->tagName << endl;
				cout << "currentDelimiterType = " << currentDelimiterType << endl;
				cout << "wordIndexOfHypotheticalPreceedingThatWhich = " << wordIndexOfHypotheticalPreceedingThatWhich << endl;
				#endif

				if(wordIndex-2 >= 0)
				{
					if(GIApreprocessorMultiwordReduction.detectModalAuxiliary((*logicReferenceVariableWordList)[wordIndex-2], usePOSprelim))
					{
						//eg that will be riding
						wordIndexOfHypotheticalPreceedingThatWhich--;
					}
				}
			}
			/*
			else
			{
				if((currentDelimiterType == GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
				{
					if(GIApreprocessorMultiwordReduction.determineIsAdverb((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim)
					{
						if(wordIndex-2 >= 0)
						{
							if(detectAuxiliary((*logicReferenceVariableWordList)[wordIndex-2]))
							{
								//eg is very near / is not near
								previousWordIsModalAuxiliary = true;
								wordIndexOfHypotheticalPreceedingThatWhich = wordIndexOfHypotheticalPreceedingThatWhich-2;
							}
						}
					}
				}
			}
			*/
		}

		bool currentWordIsReferenceSetDelimiterPreceededByThatWhich = false;
		if(wordIndexOfHypotheticalPreceedingThatWhich >= 0)
		{
			if(GIApreprocessorMultiwordReduction.detectRcmodSameReferenceSetDelimiter((*logicReferenceVariableWordList)[wordIndexOfHypotheticalPreceedingThatWhich], usePOSprelim))
			{
				currentWordIsReferenceSetDelimiterPreceededByThatWhich = true;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "currentWordIsReferenceSetDelimiterPreceededByThatWhich" << endl;
				#endif
			}
		}

		int firstIndexOfReferenceSetDelimiterText = 0;
		if(currentWordIsReferenceSetDelimiterPreceededByThatWhich)
		{
			referenceSetDelimiterIndicatesSameReferenceSet = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS_DISCARD_THAT_WHICH
			firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;	//NB the "that/which" will be removed from the text
			#else
			firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich;
			#endif
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "referenceSetDelimiterIndicatesSameReferenceSet" << endl;
			#endif
		}
		else
		{
			firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
		}
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "BEFORE: logicReferenceContents = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&logicReferenceContents) << endl;
		cout << "BEFORE: logicReferenceVariableWordList = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(logicReferenceVariableWordList) << endl;
		#endif
		int logicReferenceDelimiterPrependTextSize = wordIndex-firstIndexOfReferenceSetDelimiterText;
		for(int i=0; i<logicReferenceDelimiterPrependTextSize; i++)
		{
			GIApreprocessorPlainTextWord* prependWord = (*logicReferenceVariableWordList)[firstIndexOfReferenceSetDelimiterText+i];
			logicReferenceContents.insert(logicReferenceContents.begin(), prependWord);
			logicReferenceVariableWordList->pop_back();
		} 
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "AFTER: logicReferenceContents = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&logicReferenceContents) << endl;
		cout << "AFTER: logicReferenceVariableWordList = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(logicReferenceVariableWordList) << endl;
		#endif
	}
	
	return logicReferenceContents;
}
#endif
vector<GIApreprocessorPlainTextWord*> GIAtxtRelTranslatorHybridLogicReferenceClass::generateLogicReferenceContentsBase(GIApreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType)
{
	vector<GIApreprocessorPlainTextWord*> logicReferenceContents;
	logicReferenceContents.push_back(logicReferenceWord);	//logicReferenceClassType;	//this needs to be updated
	return logicReferenceContents;
}

bool GIAtxtRelTranslatorHybridLogicReferenceClass::islogicReferenceExplitOrImplicitConjunctionComponent(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList)
{
	bool conjunction = false;
	if((currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED) || (currentLogicReferenceInList->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT))
	{
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		conjunction = true;
		//}
	}
	return conjunction;
}


bool GIAtxtRelTranslatorHybridLogicReferenceClass::islogicReferenceConjunctionComponentNotFirst(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList)
{
	bool result = false;
	
	bool parentIsConjunctionLogicReference = false;	
	if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
		{
			parentIsConjunctionLogicReference = true;
		}
	}
	bool conjunctionNotFirst = false;
	if(currentLogicReferenceInList->previous != NULL)
	{
		conjunctionNotFirst = true;
	}
		
	if(parentIsConjunctionLogicReference && conjunctionNotFirst)
	{
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		result = true;
		//}
	}
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
bool GIAtxtRelTranslatorHybridLogicReferenceClass::islogicReferenceParentConjunctionComponentNotFirst(GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInList)
{
	bool result = false;
	
	bool conjunctionNotFirst = false;
	bool parentIsConjunctionLogicReferenceComponent = false;
	
	if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInListInUpper = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;
		while(currentLogicReferenceInListInUpper->previous != NULL)
		{
			currentLogicReferenceInListInUpper = currentLogicReferenceInListInUpper->previous;
			conjunctionNotFirst = true;
		}
		
		GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInListInUpper = currentLogicReferenceInListInUpper;
		if(firstLogicReferenceInListInUpper->lastLogicReferenceInUpperLevel != NULL)
		{
			if(firstLogicReferenceInListInUpper->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
			{
				parentIsConjunctionLogicReferenceComponent = true;
			}
		}
	}
	
	if(parentIsConjunctionLogicReferenceComponent && conjunctionNotFirst)
	{
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		result = true;
		//}
	}
	
	return result;
}
#endif




#endif

#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
bool GIAtxtRelTranslatorHybridLogicReferenceClass::printLogicReferenceLayer(GIAtxtRelTranslatorHybridLogicReference* firstLogicReferenceInLayer)
{
	bool result = true;
	GIAtxtRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInLayer;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE	
		//cout << "\tcurrentLogicReferenceInList->logicReferenceVariable->sentenceIndex = " << currentLogicReferenceInList->logicReferenceVariable->sentenceIndex << ", logicReferenceVariableContents: " << currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents << ", logicReferenceVariableName: " << currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName << endl;
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			//cout << "\ttcurrentLogicReferenceInList->logicReferenceClassType = " << currentLogicReferenceInList->logicReferenceClassType << endl;
			#endif
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				cout << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentLogicReferenceInList->logicReferenceContents) << "(";	//currentLogicReferenceInList->logicReferenceClassType 
				if(!printLogicReferenceLayer(currentLogicReferenceInList->firstSubLogicReferenceInListArray))
				{
					result = false;
				}
				cout << ")";
			}
			else
			{
				cout << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentLogicReferenceInList->logicReferenceContents) << "(";
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!printLogicReferenceLayer(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor))
					{
						result = false;
					}
				#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
				}
				#endif
				cout << ",";
				if(!printLogicReferenceLayer(currentLogicReferenceInList->firstSubLogicReferenceInListDependent))
				{
					result = false;
				}
				cout << ")";
			}
		}
		else
		{
		#endif
			//cout << "[RSsubj:";
			cout << "[";
			GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				//cout << "\t\treferenceSetSubject: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;
			}
			cout << "]";
			cout << "[";
			//cout << "RSobj:";
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				//cout << "\t\treferenceSetObject: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;
			}
			cout << "]";
			cout << "[";
			//cout << "RSdel:";
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter;
			if(!(currentSubReferenceSetInList->subReferenceSetContents).empty())
			{
				//cout << "\t\treferenceSetDelimiter: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
			}
			cout << "]";
		#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		}
		#endif
		
		if(currentLogicReferenceInList->next != NULL)
		{
			currentLogicReferenceInList = currentLogicReferenceInList->next;
			cout << " , ";
		}
		else
		{
			stillParsingLogicReferenceLayer = false;
		}
	}
	return result;
}
#endif

#endif





