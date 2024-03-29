 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorHybridLogicReference.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Logic Reference
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorHybridLogicReference.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
#include "GIAposRelTranslatorHybridReferenceSet.hpp"
#endif
#include "GIAsynRelTranslatorDefs.hpp"
#include "XMLrulesClass.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE

//Limitation: doesn't support conjunctions with embedded preposition logic references, eg; "The chicken is green, considering the teapot, red, considering the apple, or blue, considering the pie."
bool GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsWordList, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<XMLparserTag*>* SANIrulesTokenLayers)
{
	bool result = true;
	
	//OLD:  Separate out 1. ,/and/or (including doactionA and doactionB),

	int wordIndexSentence = 0;	//this gets reset at next sentence
	int logicReferenceVariableNameIndex = 0;
	vector<LRPpreprocessorPlainTextWord*> logicReferenceVariableWordList;	//this gets reset after each logic reference variable
	bool expectToFindSubjectAuxObjectLogicReferenceVariable = false;
	bool whiteSpace = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
	bool grammaticallyStrict = true;	//this is irrelevant and could be set to LRP_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT as determineIsWordType(usePOSprelim=true, ...) assumes grammaticallyStrict
	#else
	bool grammaticallyStrict = false;
	#endif
		
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = currentLRPpreprocessorSentenceInList->firstLogicReferenceInList;
	for(int w=0; w<sentenceContentsWordList->size(); w++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (*sentenceContentsWordList)[w];
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
		cout << "[currentWord = " << currentWord->tagName << "]" << endl;
		#endif
		bool foundClassType = false;
		string logicReferenceClassType = "";
		int logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED;
		if(SANIrules.isClassTag(currentWord->tagName, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction, &logicReferenceClassType, SANIrulesTokenLayers))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(LRPpreprocessorWordIdentification.determineIsConjunction(currentWord, usePOSprelim))
			{
			#endif
				logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				cout << "logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION " << endl;
				#endif
				foundClassType = true;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}
		else if(SANIrules.isClassTag(currentWord->tagName, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition, &logicReferenceClassType, SANIrulesTokenLayers))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(LRPpreprocessorWordIdentification.determineIsPreposition(currentWord, usePOSprelim))
			{
			#endif
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
				if(GIAposRelTranslatorHybridReferenceSet.verifyIsPrepositionNotProgressiveVerbBasedOnContext(sentenceContentsWordList, w, usePOSprelim))
				{
				#endif
					logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
					cout << "logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION " << endl;
					#endif
					foundClassType = true;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
				}
				#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}
		else if(SANIrules.isClassTag(currentWord->tagName, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb, &logicReferenceClassType, SANIrulesTokenLayers))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if(LRPpreprocessorWordIdentification.determineIsVerb(currentWord, usePOSprelim, grammaticallyStrict))
			{
			#endif
				logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				cout << "logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB " << endl;
				#endif
				foundClassType = true;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			}
			#endif
		}

		if(foundClassType)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			cout << "currentWord = " << currentWord->tagName << endl;
			cout << "logicReferenceClass = " << logicReferenceClass << endl;
			#endif

			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
			vector<LRPpreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContents(currentWord, logicReferenceClassType, logicReferenceClass, &logicReferenceVariableWordList);
			#else
			vector<LRPpreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContentsBase(currentWord, logicReferenceClassType);
			#endif

			if(logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
			{
				if(logicReferenceVariableWordList.size() == 0)
				{							
					if((currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
					{
						//x, and
						//eg [The house is blue], and the chicken is happy.
						//eg Tom said A, and proposed that the chicken is happy.
					}
					else
					{
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS
						//eg subsequent_to and before (FUTURE: and -> subsequent_to + before)
						if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
						{
							result = false;
						}
						#else
						cerr << "LRPpreprocessor::executeLogicReferencePreprocessor{} error: GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS not supported; eg subsequent_to and before" << endl;
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
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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

				currentLogicReferenceInList->logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT;	//this might be overwritten as a verb/preposition logic reference
				if(!identifyImplicitLogicReferenceSets(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType))
				{
					result = false;
				}
			}
			else if((logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) || (logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION))
			{	
				GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInListActive = currentLogicReferenceInList;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
				GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInListActiveOriginal = currentLogicReferenceInListActive;
				int currentLogicReferenceInListActiveOriginalClass = currentLogicReferenceInListActive->logicReferenceClass;
				#endif

				bool specialSwitch = false;
				if(logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
				{
					if((currentLogicReferenceInList->previous != NULL) && (currentLogicReferenceInList->previous->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						cout << "specialSwitch" << endl;
						#endif
	
						currentLogicReferenceInListActive = currentLogicReferenceInList->previous->lastLogicReferenceInUpperLevel;
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
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
					#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					else
					{
						if(logicReferenceVariableWordList.size() != 0)
						{								
							//eg The house is blue considering the apple.	(add 'The house is blue' to the currentLogicReferenceInList)
							
							specialSwitch = true;
							if(!initialiseNewSubLogicReference(currentLogicReferenceInListActive, logicReferenceContents, logicReferenceClass, logicReferenceClassType, wordIndexSentence))	//NB logicReferenceContents is not used here as logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB
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

					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
					if(currentLogicReferenceInList->previous != NULL)
					{		
						//The apple proposed A, the chicken proposed B...	/	The apple proposed A, said/proposed B...	/ The apple proposed A, B...
										
						GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList = currentLogicReferenceInListActive->previous;		//proposed
						firstLogicReferenceInList->next = NULL;	//delete the original logic reference as it will contain nothing
						if(firstLogicReferenceInList->previous != NULL)
						{
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->previous != NULL)" << endl;
							exit(EXIT_ERROR);
						}
						if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel == NULL)
						{
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->lastLogicReferenceInUpperLevel == NULL)" << endl;
							exit(EXIT_ERROR);
						}
						if(firstLogicReferenceInList->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)
						{
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (firstLogicReferenceInList->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
							exit(EXIT_ERROR);
						}

						GIAposRelTranslatorHybridLogicReference* conjunctionLogicReferenceOld = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;	//GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED
						if(conjunctionLogicReferenceOld->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
						{
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (conjunctionLogicReferenceOld->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)" << endl;
							exit(EXIT_ERROR);
						}

						GIAposRelTranslatorHybridLogicReference* verbLogicReference = conjunctionLogicReferenceOld->lastLogicReferenceInUpperLevel;	//GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION
						verbLogicReference->firstSubLogicReferenceInListDependent = conjunctionLogicReferenceOld->firstSubLogicReferenceInListArray;	//proposed	//delete the old conjunctionLogicReferenceOld	

						if(verbLogicReference->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
						{
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (verbLogicReference->logicReferenceClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)" << endl;
							exit(EXIT_ERROR);
						}

						bool copySubLogicReferences = true;
						if(!initialiseNewSubLogicReferenceConjunctionInferred(&verbLogicReference, wordIndexSentence, copySubLogicReferences))
						{
							result = false;
						}
						GIAposRelTranslatorHybridLogicReference* conjunctionLogicReferenceNew = verbLogicReference->lastLogicReferenceInUpperLevel;
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

				if(logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
				{
					if(logicReferenceVariableWordList.size() != 0)
					{
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						if((currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT))
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
								cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: expectToFindSubjectAuxObjectLogicReferenceVariable != false" << endl;
								exit(EXIT_ERROR);
							}
							*/
							expectToFindSubjectAuxObjectLogicReferenceVariable = false;
							if(!fillLogicReferenceVariable(currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
							{
								result = false;
							}
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						}
						else
						{
							//eg Subsequent_to proposed that the house is blue.
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (currentLogicReferenceInListActiveOriginalClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED||GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT)" << endl;
							cerr << "currentLogicReferenceInListActiveOriginal->logicReferenceClass = " << currentLogicReferenceInListActiveOriginal->logicReferenceClass << endl;
							exit(EXIT_ERROR);
						}
						#endif
					}
					else
					{
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
						currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor = NULL;
						#endif
						#else
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						if((currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT) || (currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED))
						{
						#endif
							/*
							//Tom said A and proposed [that] the house [is blue].
							//Tom said A, and proposed [that] the house [is blue].
							//Tom proposed A, suggested B, etc
							*/
						#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
						}
						else
						{	
							//eg The house is blue suggested the apple.
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB) && (logicReferenceVariableWordList->size() == 0) && !(currentLogicReferenceInListActiveOriginal->logicReferenceConjunctionClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT || GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
							exit(EXIT_ERROR);
						}
						#endif
						#endif
					}
					currentLogicReferenceInList = currentLogicReferenceInListActive->firstSubLogicReferenceInListDependent;
				}
				else if(logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
				{
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
					if((currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION))
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
							#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
							cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && (logicReferenceVariableWordList->size() != 0)" << endl;
							cerr << "currentWord->tagName = " << currentWord->tagName << endl;
							LRPpreprocessorWordClassObject.printWordList(&logicReferenceVariableWordList);
							exit(EXIT_ERROR);
							#endif
						}
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_PREPOSITIONS_MUST_SUCCEED_COMMA
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_ERROR_CHECKING
					}
					else
					{
						//eg the house is blue considering the pie.
						cerr << "GIAposRelTranslatorHybridLogicReferenceClass::executeLogicReferencePreprocessor{} error: (logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION) && !((currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED) || (currentLogicReferenceInListActiveOriginalClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION))" << endl;
						cerr << "currentLogicReferenceInListActiveOriginalClass = " << currentLogicReferenceInListActiveOriginalClass << endl;
						exit(EXIT_ERROR);
					}
					#endif
					#endif

					currentLogicReferenceInList = currentLogicReferenceInListActive->firstSubLogicReferenceInListGovernor;
				}

				if((w+1 < sentenceContentsWordList->size()) && ((*sentenceContentsWordList)[w+1]->tagName == GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME))
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
		else if(currentWord->tagName == SHAREDvars.convertCharToString(GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR))
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
				//eg The apple proposed A, B, and C	and(proposed(apple, A), B, C)	!GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR: proposed(apple, and(A, B, C))
				//eg Tom proposed that the apple is blue, green, and yellow.	and(proposed(Tom, the apple is blue), green, yellow)	!GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR: proposed(Tom, and(apple is blue, green, yellow))
				//eg Tom proposed that the apple is blue, said that the orange is green, and suggested that the pie is yellow.
				//FUTURE? Tom proposed that the apple is blue, said that the orange is green, and suggested that the pie is yellow, red, and purple.	and(proposed(Tom, the apple is blue), said(, orange is green), suggested(, and(pie is yellow, red, purple))	or?	and(proposed(Tom, the apple is blue), said(, orange is green), and(suggested(, pie is yellow, red, purple))
				//FUTURE? Considering that the house is red, blue, and green, the monkey is sad.
				//FUTURE? The monkey is sad, considering that the house is red, blue, and green.
				
				bool copySubLogicReferences = false;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
				currentLogicReferenceInList->logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED;
			}
		}
		else if(((currentWord->tagName).length() == 1) && SHAREDvars.charInCharArray((currentWord->tagName)[0], nlpPunctionMarkCharacterEndOfSentenceArray, LRP_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
		{
			if(!fillLogicReferenceVariable(currentLogicReferenceInList, &logicReferenceVariableWordList, &logicReferenceVariableNameIndex, &expectToFindSubjectAuxObjectLogicReferenceVariable, wordIndexSentence))
			{
				result = false;
			}
		}
		else if(((currentWord->tagName).length() == 1) && SHAREDvars.charInCharArray((currentWord->tagName)[0], nlpPunctionMarkCharacterArray, LRP_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
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

bool GIAposRelTranslatorHybridLogicReferenceClass::logicReferenceClassIsPreposition(const GIAposRelTranslatorHybridLogicReference* logicReference)
{
	bool result = false;
	//if(SHAREDvars.textInTextArray(logicReference->logicReferenceClassType, GIAposRelTranslatorHybridLogicReferenceClassPrepositionTypes, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM))
	if(logicReference->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
	{
		result = true;
	}
	return result;
}

bool GIAposRelTranslatorHybridLogicReferenceClass::initialiseNextLogicReferenceConjunction(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence)
{
	bool result = true;
	
	(*currentLogicReferenceInList)->next = new GIAposRelTranslatorHybridLogicReference();
	(*currentLogicReferenceInList)->next->previous = (*currentLogicReferenceInList);
	(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->next;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	(*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence = wordIndexSentence+1;	//CHECKTHIS
	#endif
	
	return result;
}

bool GIAposRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReferenceConjunctionInferred(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, const int wordIndexSentence, const bool copySubLogicReferences)
{
	vector<LRPpreprocessorPlainTextWord*> logicReferenceContentsEmpty;
	return initialiseNewSubLogicReferenceConjunction(currentLogicReferenceInList, logicReferenceContentsEmpty, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION, "", wordIndexSentence, copySubLogicReferences);
}

bool GIAposRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReferenceConjunction(GIAposRelTranslatorHybridLogicReference** currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType, const int wordIndexSentence, const bool copySubLogicReferences)
{
	GIAposRelTranslatorHybridLogicReferenceVariable* currentLogicReferenceVariableInListTemp = (*currentLogicReferenceInList)->logicReferenceVariable;
					
	(*currentLogicReferenceInList)->hasSubLogicReference = true;
	(*currentLogicReferenceInList)->hasSubLogicReferenceArray = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray = new GIAposRelTranslatorHybridLogicReference();
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->isSubLogicReferenceArray = true;
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->lastLogicReferenceInUpperLevel = (*currentLogicReferenceInList);  //so that can return to the current level upon the detection of a comma
	(*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	(*currentLogicReferenceInList)->logicReferenceVariable->wordIndexSentence = wordIndexSentence + LRP_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	#endif
	*/
	(*currentLogicReferenceInList)->logicReferenceVariable = (*currentLogicReferenceInList)->firstSubLogicReferenceInListArray->logicReferenceVariable;	//ie = new logicReferenceVariable
	(*currentLogicReferenceInList) = (*currentLogicReferenceInList)->firstSubLogicReferenceInListArray;
	(*currentLogicReferenceInList)->logicReferenceVariable = currentLogicReferenceVariableInListTemp;
}



bool GIAposRelTranslatorHybridLogicReferenceClass::initialiseNewSubLogicReference(GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType, const int wordIndexSentence)
{
	setLogicReferenceInfo(currentLogicReferenceInList, logicReferenceContents, logicReferenceClass, logicReferenceClassType);
	currentLogicReferenceInList->hasSubLogicReference = true;
	currentLogicReferenceInList->hasSubLogicReferenceArray = false;
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor = new GIAposRelTranslatorHybridLogicReference();
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent = new GIAposRelTranslatorHybridLogicReference();
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->isSubLogicReferenceGovernor = true;
	currentLogicReferenceInList->firstSubLogicReferenceInListGovernor->lastLogicReferenceInUpperLevel = currentLogicReferenceInList;  //so that can return to the current level upon the detection of a comma
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent->isSubLogicReferenceDependent = true;
	currentLogicReferenceInList->firstSubLogicReferenceInListDependent->lastLogicReferenceInUpperLevel = currentLogicReferenceInList; //so that can return to the current level upon the detection of a comma
	/*
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = wordIndexSentence + LRP_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	#endif
	*/
}

bool GIAposRelTranslatorHybridLogicReferenceClass::fillLogicReferenceVariable(GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool* expectToFindSubjectAuxObjectLogicReferenceVariable, int wordIndexSentence)
{
	//exit parsing of logical condition variable
	
	bool result = true;
	
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = wordIndexSentence - logicReferenceVariableWordList->size() + LRP_NLP_START_ENTITY_INDEX;	//CHECKTHIS
	//cout << "currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence = " << currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence << endl;
	#endif
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents = *logicReferenceVariableWordList;
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex;
	string logicReferenceVariableName = GIAposRelTranslatorHybridLogicReferenceVariableNames[*logicReferenceVariableNameIndex];
	currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName = logicReferenceVariableName;
	*logicReferenceVariableNameIndex = *logicReferenceVariableNameIndex + 1;
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET
	if(!GIAposRelTranslatorHybridReferenceSet.executeReferenceSetPreprocessor(logicReferenceVariableWordList, currentLogicReferenceInList->logicReferenceVariable, *expectToFindSubjectAuxObjectLogicReferenceVariable, currentLogicReferenceInList->logicReferenceVariable->wordIndexSentence))
	{
		result = false;	
	}
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
	if(islogicReferenceExplitOrImplicitConjunctionComponent(currentLogicReferenceInList))
	{
		if(GIAposRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject))
		{
			if(!GIAposRelTranslatorHybridReferenceSet.hasReferenceSet(currentLogicReferenceInList->logicReferenceVariable->referenceSetObject))
			{
				//eg Tom is happy and fast 
				//switch reference set subject/object
				LRPpreprocessorSubReferenceSet* tempSubReferenceSet = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
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
			cerr << "LRPpreprocessor::fillLogicReferenceVariable{} error: islogicReferenceConjunctionAndOr && isReferenceSetEmpty(referenceSetSubject)" << endl;
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
bool GIAposRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets(GIAposRelTranslatorHybridLogicReference* explicitConjunctionLogicReferenceInList, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType)
{
	bool result = false;
	
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = explicitConjunctionLogicReferenceInList;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES 
	while(currentLogicReferenceInList->previous != NULL)
	{
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
	}
	#else
	while(currentLogicReferenceInList->previous != NULL)
	{
		currentLogicReferenceInList = currentLogicReferenceInList->previous;
		
		if(currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)
		{
			currentLogicReferenceInList->logicReferenceClass = GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT_IMPLICIT_CONFIRMED;
			currentLogicReferenceInList->logicReferenceClassType = explicitConjunctionLogicReferenceInList->logicReferenceConjunctionClassType;
			result = true;
		}
		else
		{
			cerr << "GIAposRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets{} error; (currentLogicReferenceInList->logicReferenceConjunctionClass != GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInLayer = currentLogicReferenceInList;
	if(firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAposRelTranslatorHybridLogicReference* conjunctionLogicReference = firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel;
		setLogicReferenceInfo(conjunctionLogicReference, logicReferenceContents, logicReferenceClass, logicReferenceClassType);
	}
	else
	{
		cerr << "GIAposRelTranslatorHybridLogicReferenceClass::identifyImplicitLogicReferenceSets{} error; (firstLogicReferenceInLayer->lastLogicReferenceInUpperLevel === NULL)" << endl;
		exit(EXIT_ERROR);	
	}
	
	return result;	
}
	
void GIAposRelTranslatorHybridLogicReferenceClass::setLogicReferenceInfo(GIAposRelTranslatorHybridLogicReference* logicReference, vector<LRPpreprocessorPlainTextWord*> logicReferenceContents, int logicReferenceClass, string logicReferenceClassType)
{
	logicReference->logicReferenceContents = logicReferenceContents;
	logicReference->logicReferenceClass = logicReferenceClass;
	logicReference->logicReferenceClassType = logicReferenceClassType;	
}		

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
vector<LRPpreprocessorPlainTextWord*> GIAposRelTranslatorHybridLogicReferenceClass::generateLogicReferenceContents(LRPpreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType, const int logicReferenceClass, vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList)
{
	bool referenceSetDelimiterIndicatesSameReferenceSet = false;
	
	vector<LRPpreprocessorPlainTextWord*> logicReferenceContents = generateLogicReferenceContentsBase(logicReferenceWord, logicReferenceClassType);
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	
	if(logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
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
			if(LRPpreprocessorWordIdentification.detectModalAuxiliary((*logicReferenceVariableWordList)[wordIndex-1], usePOSprelim))
			{	
				previousWordIsModalAuxiliary = true;
				wordIndexOfHypotheticalPreceedingThatWhich--;
			}
		}
		//verify that the auxiliary/verb is not preceeded by an auxiliary (e.g. for doing auxiliaries; is being/having/doing, or for verbs; is riding, or for prepositions; is near), in which case must test the word prior to the modal auxiliary for that/which
		if(wordIndex-1 >= 0)
		{
			if(LRPpreprocessorWordIdentification.detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
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
					if(LRPpreprocessorWordIdentification.detectModalAuxiliary((*logicReferenceVariableWordList)[wordIndex-2], usePOSprelim))
					{
						//eg that will be riding
						wordIndexOfHypotheticalPreceedingThatWhich--;
					}
				}
			}
			/*
			else
			{
				if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
				{
					if(LRPpreprocessorWordIdentification.determineIsAdverb((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim)
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
			if(LRPpreprocessorWordIdentification.detectRcmodSameReferenceSetDelimiter((*logicReferenceVariableWordList)[wordIndexOfHypotheticalPreceedingThatWhich], usePOSprelim))
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
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS_DISCARD_THAT_WHICH
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
		cout << "BEFORE: logicReferenceContents = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&logicReferenceContents) << endl;
		cout << "BEFORE: logicReferenceVariableWordList = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(logicReferenceVariableWordList) << endl;
		#endif
		int logicReferenceDelimiterPrependTextSize = wordIndex-firstIndexOfReferenceSetDelimiterText;
		for(int i=0; i<logicReferenceDelimiterPrependTextSize; i++)
		{
			LRPpreprocessorPlainTextWord* prependWord = (*logicReferenceVariableWordList)[firstIndexOfReferenceSetDelimiterText+i];
			logicReferenceContents.insert(logicReferenceContents.begin(), prependWord);
			logicReferenceVariableWordList->pop_back();
		} 
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "AFTER: logicReferenceContents = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&logicReferenceContents) << endl;
		cout << "AFTER: logicReferenceVariableWordList = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(logicReferenceVariableWordList) << endl;
		#endif
	}
	
	return logicReferenceContents;
}
#endif
vector<LRPpreprocessorPlainTextWord*> GIAposRelTranslatorHybridLogicReferenceClass::generateLogicReferenceContentsBase(LRPpreprocessorPlainTextWord* logicReferenceWord, const string logicReferenceClassType)
{
	vector<LRPpreprocessorPlainTextWord*> logicReferenceContents;
	logicReferenceContents.push_back(logicReferenceWord);	//logicReferenceClassType;	//this needs to be updated
	return logicReferenceContents;
}

bool GIAposRelTranslatorHybridLogicReferenceClass::islogicReferenceExplitOrImplicitConjunctionComponent(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList)
{
	bool conjunction = false;
	if((currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED) || (currentLogicReferenceInList->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT))
	{
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		conjunction = true;
		//}
	}
	return conjunction;
}


bool GIAposRelTranslatorHybridLogicReferenceClass::islogicReferenceConjunctionComponentNotFirst(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, const GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList)
{
	bool result = false;
	
	bool parentIsConjunctionLogicReference = false;	
	if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		if(firstLogicReferenceInList->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
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
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		result = true;
		//}
	}
	return result;
}

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
bool GIAposRelTranslatorHybridLogicReferenceClass::islogicReferenceParentConjunctionComponentNotFirst(const GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList)
{
	bool result = false;
	
	bool conjunctionNotFirst = false;
	bool parentIsConjunctionLogicReferenceComponent = false;
	
	if(currentLogicReferenceInList->lastLogicReferenceInUpperLevel != NULL)
	{
		GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInListInUpper = firstLogicReferenceInList->lastLogicReferenceInUpperLevel;
		while(currentLogicReferenceInListInUpper->previous != NULL)
		{
			currentLogicReferenceInListInUpper = currentLogicReferenceInListInUpper->previous;
			conjunctionNotFirst = true;
		}
		
		GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInListInUpper = currentLogicReferenceInListInUpper;
		if(firstLogicReferenceInListInUpper->lastLogicReferenceInUpperLevel != NULL)
		{
			if(firstLogicReferenceInListInUpper->lastLogicReferenceInUpperLevel->logicReferenceClass == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
			{
				parentIsConjunctionLogicReferenceComponent = true;
			}
		}
	}
	
	if(parentIsConjunctionLogicReferenceComponent && conjunctionNotFirst)
	{
		//if((currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and) || (currentLogicReferenceInList->logicReferenceConjunctionClassType == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or))
		//{
		result = true;
		//}
	}
	
	return result;
}
#endif




#endif

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_PRINT_OUTPUT
bool GIAposRelTranslatorHybridLogicReferenceClass::printLogicReferenceLayer(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInLayer)
{
	bool result = true;
	GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList = firstLogicReferenceInLayer;
	bool stillParsingLogicReferenceLayer = true;
	while(stillParsingLogicReferenceLayer)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE	
		//cout << "\tcurrentLogicReferenceInList->logicReferenceVariable->sentenceIndex = " << currentLogicReferenceInList->logicReferenceVariable->sentenceIndex << ", logicReferenceVariableContents: " << currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableContents << ", logicReferenceVariableName: " << currentLogicReferenceInList->logicReferenceVariable->logicReferenceVariableName << endl;
		#endif
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
		if(currentLogicReferenceInList->hasSubLogicReference)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			//cout << "\ttcurrentLogicReferenceInList->logicReferenceClassType = " << currentLogicReferenceInList->logicReferenceClassType << endl;
			#endif
			if(currentLogicReferenceInList->hasSubLogicReferenceArray)
			{
				cout << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentLogicReferenceInList->logicReferenceContents) << "(";	//currentLogicReferenceInList->logicReferenceClassType 
				if(!printLogicReferenceLayer(currentLogicReferenceInList->firstSubLogicReferenceInListArray))
				{
					result = false;
				}
				cout << ")";
			}
			else
			{
				cout << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentLogicReferenceInList->logicReferenceContents) << "(";
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
				if(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor != NULL)
				{
				#endif
					if(!printLogicReferenceLayer(currentLogicReferenceInList->firstSubLogicReferenceInListGovernor))
					{
						result = false;
					}
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
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
			LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetSubject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				//cout << "\t\treferenceSetSubject: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;
			}
			cout << "]";
			cout << "[";
			//cout << "RSobj:";
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetObject;
			while(currentSubReferenceSetInList->next != NULL)
			{
				//cout << "\t\treferenceSetObject: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
				currentSubReferenceSetInList = currentSubReferenceSetInList->next;
			}
			cout << "]";
			cout << "[";
			//cout << "RSdel:";
			currentSubReferenceSetInList = currentLogicReferenceInList->logicReferenceVariable->referenceSetDelimiter;
			if(!(currentSubReferenceSetInList->subReferenceSetContents).empty())
			{
				//cout << "\t\treferenceSetDelimiter: currentSubReferenceSetInList->sentenceIndex = " << currentSubReferenceSetInList->sentenceIndex << ", subReferenceSetContents: " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents) << endl;
				cout << "|" << currentSubReferenceSetInList->sentenceIndex << ";" << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&currentSubReferenceSetInList->subReferenceSetContents);
			}
			cout << "]";
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE
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





