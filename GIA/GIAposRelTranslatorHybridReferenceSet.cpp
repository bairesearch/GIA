 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAposRelTranslatorHybridReferenceSet.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid Reference Set
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorHybridReferenceSet.hpp"
#include "GIAsynRelTranslatorDefs.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET

/*
grammar past participle
http://www.learnenglish.de/grammar/participlepast.html
http://www.grammar-monster.com/glossary/past_participles.htm

Past Participles in Participle Phrases
Past participles can often be found in participle phrases. A participle phrase acts like an adjective. In the examples below, the participle phrases are shaded and the past participles are in bold:
    The boy taken to hospital has recovered.
    (The participle phrase taken to hospital describes the boy.)
    I have a heart wracked with sorrow.
    (The participle phrase wracked with sorrow describes a heart.)
    Battered by the wind, John fell to his knees.
    (The participle phrase "Battered by the wind describes john.)
    Finally broken , Lee lowered his gloves.
    (The participle phrase Finally broken describes Lee.
    
NB there is no easy rcmod detection in these cases
*/

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED

bool GIAposRelTranslatorHybridReferenceSetClass::executeReferenceSetPreprocessor(vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList, GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, const bool expectToFindSubjectAuxObjectLogicReferenceVariable, int wordIndexSentence)
{
	bool result = true;

	//see getRelationshipNameAndType for algorithm for relationship type detection
	
	//Perform auxiliary detection by looking for the word "has/have/has had" / "is/was/will be" preceding the second word of the relation (ignoring intermediary determiners; a/the). Perform same reference set detection by checking for "that/which" before the auxiliary.
	//Reference set reduction preprocessor - rrp. 3. reference sets; look for "is" not following that/which or infinitive/present/continuous/past[/past participle?] tense verbs (eg move/moves/moving/moved) not following that/which [or intemediary auxiliaries eg that is running] or determiners (the/a) or possessive pronouns (his/her/its).
	//FUTURE:  Separate out 2. modifiers (eg adverb/adjectives),
	//FUTURE; the cat that has a dog, a bike, and an apple, is sad.	[filter out same reference sets from non-same reference sets]
	//FUTURE: Finally broken, Lee lowered his gloves.
	//OK: the cat that has a dog is sad.
	//FUTURE??: the cat that has a dog, is sad.
	
	//full referenceSet variables;	
	bool referenceSetDelimiterDetectedFull = false;
	int firstIndexOfReferenceSetDelimiterTextFull = INT_DEFAULT_VALUE;
	int lastIndexOfReferenceSetDelimiterTextFull = INT_DEFAULT_VALUE;
	
	//sub or full referenceSet variables;			
	bool referenceSetDelimiterDetected = false;
	#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
	int delimiterTypeFull = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
	int delimiterSpecialCaseFull = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;
	#endif
	bool referenceSetDelimiterIndicatesSameReferenceSet = false;
	int firstIndexOfReferenceSetDelimiterText = INT_DEFAULT_VALUE;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
	int lastIndexOfReferenceSetDelimiterText = 0;
	int lastIndexOfPreviousReferenceSet = INT_DEFAULT_VALUE;
	#endif	
	bool parsingReferenceSetDelimiter = false;
	
	bool referenceSetDelimiterIsLastWordInSentence = false;

	int currentDelimiterType = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
	int currentDelimiterSpecialCase = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;	
	
	bool previousWordWasVerb = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
	bool grammaticallyStrict = true;
	#else
	bool grammaticallyStrict = false;
	#endif
		
	//detect auxiliary or verb (present/past/future tense)
	int wordIndex = 0;
	for(int wordIndex = 0; wordIndex<logicReferenceVariableWordList->size(); wordIndex++)
	{
		LRPpreprocessorPlainTextWord* currentWordTag = (*logicReferenceVariableWordList)[wordIndex];
		string currentWord = currentWordTag->tagName;
		currentWord = SHAREDvars.convertStringToLowerCase(&currentWord);	//required for imperatives where the action is defined as the first word in the sentence 
		int wordIndexOfHypotheticalPreceedingThatWhich = wordIndex-1;
					
		bool currentWordIsReferenceSetDelimiter = false;
		//auxiliary detection
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "\t\tcurrentWord = " << currentWord << endl;
		cout << "\t\t\twordIndex = " << wordIndex << endl;
		#endif
		
		if(LRPpreprocessorWordIdentification.detectAuxiliary(currentWordTag, usePOSprelim))
		{	
			//cout << "detectAuxiliary: currentWord = " << currentWord << endl;
			currentWordIsReferenceSetDelimiter = true;
			currentDelimiterType = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY;
		}
		
		bool verbDetected = false;
		
		int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
		string verbBaseNameTemp = "";
		if(LRPpreprocessorWordIdentification.determineIsVerb(currentWordTag, usePOSprelim, grammaticallyStrict, &verbBaseNameTemp, &grammaticalBaseTenseForm))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
			if(!determineIsVerbAndAdjective(currentWordTag, usePOSprelim, grammaticallyStrict))
			{
			#endif
				//cout << "verbDetected: currentWord = " << currentWord << endl;	
				verbDetected = true;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
			}
			#endif
		}
		if(verbDetected)
		{						
			//infinitive/present/past tense verb detection (NB infinitive form must be accepted for a) future tense detection and b) imperatives where the action is defined as the first word in the sentence)
			/*
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
			#define LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
			*/
					
			bool referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = false;
			#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_STATE_SUCCEEDED_BY_NOUN
			if(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
			#endif
				#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
				if(wordIndex-1 >= 0)
				{
					//eg the [det] walk [verb]
					if(LRPpreprocessorWordIdentification.determineIsDeterminer(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
					{
						referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
					}
				}
				if(wordIndex-2 >= 0)
				{
					//eg the [det] wonderful [adj] walk [verb]
					if(LRPpreprocessorWordIdentification.determineIsDeterminer(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
					{
						if(LRPpreprocessorWordIdentification.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
						{
							referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
						}
					}
				}
				if(wordIndex-3 >= 0)
				{
					//eg the [det] very [adv] wonderful [adj] walk [verb]
					if(LRPpreprocessorWordIdentification.determineIsDeterminer(((*logicReferenceVariableWordList)[wordIndex-3]), usePOSprelim))
					{
						if(LRPpreprocessorWordIdentification.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
						{
							if(LRPpreprocessorWordIdentification.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
							{
								referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
							}
						}
					}
				}
				#endif
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_STATE_SUCCEEDED_BY_NOUN
				if(wordIndex-2 >= 0)
				{
					//eg the [det] newly [adv] controlled [verb]
					if(LRPpreprocessorWordIdentification.determineIsDeterminer(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
					{
						if(LRPpreprocessorWordIdentification.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
						{
							referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
						}
					}
				}
				#endif
			#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_STATE_SUCCEEDED_BY_NOUN
			}
			#endif
			if(!referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun)
			{
				currentWordIsReferenceSetDelimiter = true;
				currentDelimiterType = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB;
				//not required: verify that the verb is not preceeded by one or more auxiliaries (because it will have already been detected by (currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) when testing the previously parsed auxiliary)
				//cout << "verbDetected: currentWord = " << currentWord << endl;

				if(wordIndex+1 < logicReferenceVariableWordList->size())
				{
					#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
					if(!LRPpreprocessorWordIdentification.determineIsNoun(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))	//compensate for ambiguity in grammatical classification of words (e.g. "chicken" is classified as both a noun and an adjective by wordnet; eg A controlled chicken was moved to the car.
					{
					#endif
						if(LRPpreprocessorWordIdentification.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))
						{
							/*
							#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
							if(!determineIsVerbAndAdjective(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim, grammaticallyStrict))
							{
							#endif
							*/
								//eg Tom rides fast
								currentDelimiterSpecialCase = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
								#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
								wordIndex = wordIndex + 1;
								#endif
							/*
							#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
							}
							#endif
							*/
						}
						if(wordIndex+2 < logicReferenceVariableWordList->size())
						{
							if(LRPpreprocessorWordIdentification.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))
							{
								if(LRPpreprocessorWordIdentification.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex+2]), usePOSprelim))
								{
									/*
									#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
									if(!determineIsVerbAndAdjective(((*logicReferenceVariableWordList)[wordIndex+2]), usePOSprelim, grammaticallyStrict))
									{
									#endif
									*/
										//eg Tom rides very fast
										currentDelimiterSpecialCase = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
										#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
										wordIndex = wordIndex + 2;
										#endif
									/*
									#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
									}
									#endif
									*/
								}
							}
						}
					#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
					}
					#endif
				}
			}
		}
		
		if(LRPpreprocessorWordIdentification.determineIsPreposition(currentWordTag, usePOSprelim))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
			if(verifyIsPrepositionNotProgressiveVerbBasedOnContext(logicReferenceVariableWordList, wordIndex, usePOSprelim))
			{
			#endif
				currentWordIsReferenceSetDelimiter = true;
				currentDelimiterType = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
			}
			#endif
		}

		bool currentWordIsReferenceSetDelimiterPreceededByThatWhich = false;
		if(currentWordIsReferenceSetDelimiter)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "currentWordIsReferenceSetDelimiter: currentWord = " << currentWord << endl;
			cout << "currentWordIsReferenceSetDelimiter: currentDelimiterType = " << currentDelimiterType << endl;
			#endif
			
			bool previousWordIsAuxiliary = false;
			
			//verify that the auxiliary/verb is not preceeded by a modal auxiliary (e.g. for future cases; will be/have/ride), in which case must test the word prior to the modal auxiliary for that/which
						
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB)))
			#else
			if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB) && (grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)))
			#endif
			{
				if(wordIndex-1 >= 0)
				{
					if(LRPpreprocessorWordIdentification.detectModalAuxiliary(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
					{	
						previousWordIsAuxiliary = true;
						wordIndexOfHypotheticalPreceedingThatWhich--;
					}
				}
			}
			//verify that the auxiliary/verb is not preceeded by an auxiliary (e.g. for doing auxiliaries; is being/having/doing, or for verbs; is riding, or for prepositions; is near), in which case must test the word prior to the modal auxiliary for that/which
			if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || (currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB) || (currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
			{
				if(wordIndex-1 >= 0)
				{
					if(LRPpreprocessorWordIdentification.detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
					{	
						//eg that is riding
						previousWordIsAuxiliary = true;
						wordIndexOfHypotheticalPreceedingThatWhich--;
						
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "previousWordIsAuxiliary" << endl;
						cout << "currentWord = " << currentWord << endl;
						cout << "previousWord = " << ((*logicReferenceVariableWordList)[wordIndex-1])->tagName << endl;
						cout << "currentDelimiterType = " << currentDelimiterType << endl;
						cout << "wordIndexOfHypotheticalPreceedingThatWhich = " << wordIndexOfHypotheticalPreceedingThatWhich << endl;
						#endif
						
						if(wordIndex-2 >= 0)
						{
							if(LRPpreprocessorWordIdentification.detectModalAuxiliary(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
							{
								//eg (that) will be riding
								wordIndexOfHypotheticalPreceedingThatWhich--;
							}
							else if(LRPpreprocessorWordIdentification.detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
							{
								wordIndexOfHypotheticalPreceedingThatWhich--;
								//eg (that) have been riding
								if(wordIndex-3 >= 0)
								{
									if(LRPpreprocessorWordIdentification.detectModalAuxiliary(((*logicReferenceVariableWordList)[wordIndex-3]), usePOSprelim))
									{
										//eg (that) will have been riding
										wordIndexOfHypotheticalPreceedingThatWhich--;
									}
								}
								
							}
						}
					}
					else
					{
						if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
						{
							if(LRPpreprocessorWordIdentification.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
							{
								if(wordIndex-2 >= 0)
								{
									if(LRPpreprocessorWordIdentification.detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
									{
										//eg is very near / is not near
										previousWordIsAuxiliary = true;
										wordIndexOfHypotheticalPreceedingThatWhich = wordIndexOfHypotheticalPreceedingThatWhich-2;
									}
								}
							}
						}
					}
				}
			}
		
			if(wordIndexOfHypotheticalPreceedingThatWhich >= 0)
			{
				if(LRPpreprocessorWordIdentification.detectRcmodSameReferenceSetDelimiter(((*logicReferenceVariableWordList)[wordIndexOfHypotheticalPreceedingThatWhich]), usePOSprelim))
				{
					currentWordIsReferenceSetDelimiterPreceededByThatWhich = true;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "currentWordIsReferenceSetDelimiterPreceededByThatWhich" << endl;
					#endif
				}
			}
			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_STATE_SUCCEEDED_BY_NOUN
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_DETECT
			//added 3d5d;
			if(currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB) 
			{
				if(!currentWordIsReferenceSetDelimiterPreceededByThatWhich)
				{
					if(!previousWordIsAuxiliary)
					{
						//NB this doesn't yet work (not all past tense are necessarily same reference set); e.g. the dog ran_to the house /  the dog rode the bike.
						if(!((grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE) || (grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT)))	//the move/moves bike was sad
						/*
						if((grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) || 	//eg the moved bike was sad
						(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) || 	//eg the moved bike was sad (probably redundant)
						(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS) || 	//eg the moving bike was sad
						(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION) || 	//eg the movement bike was sad
						(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL) || 	//eg the moveable bike was sad
						(grammaticalBaseTenseForm == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE)	//eg the movive bike was sad
						)
						*/
						{
							currentDelimiterSpecialCase = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY;	//eg "controlled" in "a controlled chicken was moved to"
							#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_IGNORE
							currentWordIsReferenceSetDelimiter = false;
							#endif
							/*
							cout << "\t\tcurrentWord = " << currentWord << endl;
							cout << "GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY" << endl;
							cout << "grammaticalBaseTenseForm = " << grammaticalBaseTenseForm << endl;	
							*/
						}
					}
				}
			}
			#endif
			#endif
			
			//NB if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)): "that near" is not legal english (only "that is near"), but will be accepted here anyway
			//NB near to should have previously been compressed to near_to by LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
			if((currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION) && !previousWordIsAuxiliary && !currentWordIsReferenceSetDelimiterPreceededByThatWhich)
			{
				//eg the dog eats the apple near the bike.	[preposition near refers to verb eat]	[as compared to the dog eats the apple that is near the bike].
				currentDelimiterSpecialCase = GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB;
			}
		}
		
		bool currentWordIsReferenceSetDelimiterAndIsLastWordInSentence = (currentWordIsReferenceSetDelimiter && (wordIndex == logicReferenceVariableWordList->size()-1));
		int referenceSetDelimiterWordIndex = wordIndex-1;
		if(currentWordIsReferenceSetDelimiterAndIsLastWordInSentence)
		{
			referenceSetDelimiterWordIndex = wordIndex;
			referenceSetDelimiterIsLastWordInSentence = true;
			
			/*
			cout << "currentWordIsReferenceSetDelimiterAndIsLastWordInSentence" << endl;
			cout << "referenceSetDelimiterWordIndex = " << referenceSetDelimiterWordIndex << endl;
			cout << "referenceSetDelimiterDetected = " << referenceSetDelimiterDetected << endl;
			cout << "parsingReferenceSetDelimiter = " << parsingReferenceSetDelimiter << endl;
			cout << "currentWordIsReferenceSetDelimiterAndIsLastWordInSentence = " << currentWordIsReferenceSetDelimiterAndIsLastWordInSentence << endl;
			*/
			
			updateIndices(currentWordIsReferenceSetDelimiter, &referenceSetDelimiterDetected, &parsingReferenceSetDelimiter, currentDelimiterSpecialCase, wordIndexOfHypotheticalPreceedingThatWhich, currentWordIsReferenceSetDelimiterPreceededByThatWhich, &referenceSetDelimiterIndicatesSameReferenceSet, &firstIndexOfReferenceSetDelimiterText, &lastIndexOfPreviousReferenceSet);
		}
		bool currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb = false;
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUPPORT_CONSECUTIVE_VERBS
		if(currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB)
		{
			if(previousWordWasVerb)
			{
				currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb = true;
				//eg "ran" in "The chicken that was controlled ran to the car."
				/*
				cout << "currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb: currentWord = " << currentWord << endl;
				cout << "referenceSetDelimiterDetected = " << referenceSetDelimiterDetected << endl;
				cout << "parsingReferenceSetDelimiter = " << parsingReferenceSetDelimiter << endl;
				cout << "currentWordIsReferenceSetDelimiter = " << currentWordIsReferenceSetDelimiter << endl;
				*/
			}
		}
		#endif
					
		/*
		cout << "referenceSetDelimiterDetected = " << referenceSetDelimiterDetected << endl;
		cout << "parsingReferenceSetDelimiter = " << parsingReferenceSetDelimiter << endl;
		cout << "currentWordIsReferenceSetDelimiter = " << currentWordIsReferenceSetDelimiter << endl;
		cout << "currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb = " << currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb << endl;
		*/
		if(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && (!currentWordIsReferenceSetDelimiter || currentWordIsReferenceSetDelimiterAndIsLastWordInSentence || currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb))
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && (!currentWordIsReferenceSetDelimiter || currentWordIsReferenceSetDelimiterAndIsLastWordInSentence || currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb)): currentWord = " << currentWord << endl;
			#endif
				
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
			//add a subreference set to the subject/object
			if(!referenceSetDelimiterDetectedFull)
			{
				vector<LRPpreprocessorPlainTextWord*> subReferenceSetText;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "lastIndexOfReferenceSetDelimiterText = " << lastIndexOfReferenceSetDelimiterText << endl;
				cout << "lastIndexOfPreviousReferenceSet = " << lastIndexOfPreviousReferenceSet << endl;
				#endif
				if(lastIndexOfReferenceSetDelimiterText <= lastIndexOfPreviousReferenceSet)	//added 3d5d
				{
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t12 subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						LRPpreprocessorWordClassObject.printWordList(&subReferenceSetText);
						#endif

						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, &subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
					}
				}

				//add the sub referenceSetDelimiter to the subReferenceSet list:
				subReferenceSetText.clear();
				if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, firstIndexOfReferenceSetDelimiterText, referenceSetDelimiterWordIndex))
				{
					if(referenceSetDelimiterIndicatesSameReferenceSet)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t13 subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						LRPpreprocessorWordClassObject.printWordList(&subReferenceSetText);
						#endif
					
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
					else
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t14 subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						#endif
						addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetDelimiter, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
			}
			else
			{
				if(referenceSetDelimiterIndicatesSameReferenceSet)
				{
					vector<LRPpreprocessorPlainTextWord*> subReferenceSetText;
					if(lastIndexOfReferenceSetDelimiterText <= lastIndexOfPreviousReferenceSet)	//added 3d5d
					{
						if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
						{
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "\t15 subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
							#endif
							addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
						}
					}
					
					//add the sub referenceSetDelimiter to the subReferenceSet list:
					subReferenceSetText.clear();
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, firstIndexOfReferenceSetDelimiterText, referenceSetDelimiterWordIndex))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t16 subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						#endif
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
				else
				{
					cerr << "GIAposRelTranslatorHybridReferenceSetClass::executeReferenceSetPreprocessor{} error: (referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter) && !referenceSetDelimiterIndicatesSameReferenceSet - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
					LRPpreprocessorWordClassObject.printWordList(logicReferenceVariableWordList);
					exit(EXIT_ERROR);
				}
			}
			#endif

			if(!referenceSetDelimiterIndicatesSameReferenceSet)
			{
				if(referenceSetDelimiterDetectedFull)
				{
					cerr << "GIAposRelTranslatorHybridReferenceSetClass::executeReferenceSetPreprocessor{} error: (referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter) && (!referenceSetDelimiterIndicatesSameReferenceSet) && referenceSetDelimiterDetectedFull already - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
					cerr << "wordIndex = " << wordIndex << endl;
					exit(EXIT_ERROR);
				}
				else
				{
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "!referenceSetDelimiterIndicatesSameReferenceSet" << endl;
					cout << "currentWord = " << currentWord << endl;
					cout << "firstIndexOfReferenceSetDelimiterText = " << firstIndexOfReferenceSetDelimiterText << endl;
					#endif
					referenceSetDelimiterDetectedFull = true;
					#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
					delimiterTypeFull = currentDelimiterType;
					delimiterSpecialCaseFull = currentDelimiterSpecialCase;
					#endif
					firstIndexOfReferenceSetDelimiterTextFull = firstIndexOfReferenceSetDelimiterText;
					lastIndexOfReferenceSetDelimiterTextFull = wordIndex;
				}
			}
			
			parsingReferenceSetDelimiter = false;
			referenceSetDelimiterDetected = false;
			referenceSetDelimiterIndicatesSameReferenceSet = false;
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
			lastIndexOfReferenceSetDelimiterText = wordIndex;
			#endif
		}
	
		
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUPPORT_CONSECUTIVE_VERBS
		if(currentDelimiterType == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_TYPE_VERB) 
		{
			previousWordWasVerb = true;
		}
		else
		{
			previousWordWasVerb = false;
		}
		#endif
		
		if(!referenceSetDelimiterIsLastWordInSentence)
		{		
			//moved 3d5d
			updateIndices(currentWordIsReferenceSetDelimiter, &referenceSetDelimiterDetected, &parsingReferenceSetDelimiter, currentDelimiterSpecialCase, wordIndexOfHypotheticalPreceedingThatWhich, currentWordIsReferenceSetDelimiterPreceededByThatWhich, &referenceSetDelimiterIndicatesSameReferenceSet, &firstIndexOfReferenceSetDelimiterText, &lastIndexOfPreviousReferenceSet);
			/*
			cout << "\tparsingReferenceSetDelimiter = " << parsingReferenceSetDelimiter << endl;
			cout << "\treferenceSetDelimiterDetected = " << referenceSetDelimiterDetected << endl;
			*/
		}
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_SUB_REFERENCE_SETS
	//add final subReferenceSet to list;
	if(!referenceSetDelimiterIsLastWordInSentence)
	{
		if(referenceSetDelimiterDetectedFull)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "end referenceSetDelimiterDetectedFull" << endl;
			#endif
			vector<LRPpreprocessorPlainTextWord*> subReferenceSetText;
			if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, logicReferenceVariableWordList->size()-1))
			{
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "\t17 subReferenceSetText = " << subReferenceSetText << endl;
				#endif
				addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &subReferenceSetText, referenceSetDelimiterIndicatesSameReferenceSet, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);	//OLD before 3d5d: referenceSetDelimiter=false
			}	
		}
		else
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "end !referenceSetDelimiterDetectedFull" << endl;
			#endif
			vector<LRPpreprocessorPlainTextWord*> subReferenceSetText;
			if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, logicReferenceVariableWordList->size()-1))
			{
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "\t18 subReferenceSetText = " << subReferenceSetText << endl;
				#endif
				addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, &subReferenceSetText, referenceSetDelimiterIndicatesSameReferenceSet, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);	//OLD before 3d5d: referenceSetDelimiter=false
			}	
		}
	}
	#else
	if(referenceSetDelimiterDetectedFull)
	{	
		int wordIndex = 0;
		string referenceSetText = "";
		
		vector<LRPpreprocessorPlainTextWord*> referenceSetSubjectText;
		vector<LRPpreprocessorPlainTextWord*> referenceSetObjectText;
		vector<LRPpreprocessorPlainTextWord*> referenceSetDelimiterText;
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "firstIndexOfReferenceSetDelimiterTextFull = " << firstIndexOfReferenceSetDelimiterTextFull << endl;
		cout << "logicReferenceVariableWordList[firstIndexOfReferenceSetDelimiterTextFull] = " << (*logicReferenceVariableWordList)[firstIndexOfReferenceSetDelimiterTextFull]->tagName << endl;
		cout << "lastIndexOfReferenceSetDelimiterTextFull = " << lastIndexOfReferenceSetDelimiterTextFull << endl;
		cout << "logicReferenceVariableWordList[lastIndexOfReferenceSetDelimiterTextFull] = " << (*logicReferenceVariableWordList)[lastIndexOfReferenceSetDelimiterTextFull]->tagName << endl;
		#endif
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetSubjectText, 0, firstIndexOfReferenceSetDelimiterTextFull-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, &referenceSetSubjectText, false, 0, wordIndexSentence, BOOL_NA, BOOL_NA);
		}
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetDelimiterText, firstIndexOfReferenceSetDelimiterTextFull, lastIndexOfReferenceSetDelimiterTextFull-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetDelimiter, &referenceSetDelimiterText, true, firstIndexOfReferenceSetDelimiterTextFull, wordIndexSentence, delimiterTypeFull, delimiterSpecialCaseFull);
		}
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetObjectText, lastIndexOfReferenceSetDelimiterTextFull, logicReferenceVariableWordList->size()-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &referenceSetObjectText, false, lastIndexOfReferenceSetDelimiterTextFull, wordIndexSentence, BOOL_NA, BOOL_NA);
		}		
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "logicReferenceVariable->referenceSetSubject->subReferenceSetContents = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetSubject->subReferenceSetContents) << endl;
		cout << "logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents) << endl;
		cout << "logicReferenceVariable->referenceSetObject->subReferenceSetContents = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetObject->subReferenceSetContents) << endl;
		#endif
	}
	else
	{
		/*
		NB if no referenceSetDelimiterDetectedFull, then expect a future and/or to be defined (ie the GIAposRelTranslatorHybridLogicReferenceVariable is part of a conjunction list):
		1. once this is detected by executeLogicReferencePreprocessor: go back and reparse based on the first GIAposRelTranslatorHybridLogicReferenceVariable of type GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN
		2. extract the subject (referenceSetSubject) from this first GIAposRelTranslatorHybridLogicReferenceVariable in the conjunction list
		3. add the explicit subject to the phrase (see NLCpreprocessor code on how precisely to do this), reexecute executeReferenceSetPreprocessor on the GIAposRelTranslatorHybridLogicReferenceVariable
		see identifyConjunctionReferenceSets{} for details.
		*/
	}
	#endif
	
	if(expectToFindSubjectAuxObjectLogicReferenceVariable)
	{
		if(!referenceSetDelimiterDetectedFull)
		{
			cerr << "LRPpreprocessorClass::executeReferenceSetPreprocessor{} error: expectToFindSubjectAuxObjectLogicReferenceVariable && !referenceSetDelimiterDetectedFull" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return result;
	
}

#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_PREFERENCE_VERB_OR_NOUN_OVER_ADJECTIVE_POS_AMBIGUITY
bool GIAposRelTranslatorHybridReferenceSetClass::determineIsVerbAndAdjective(const LRPpreprocessorPlainTextWord* currentWordTag, const bool usePOSprelim, const bool grammaticallyStrict)
{
	bool verbAndAdjective = false;
	if(LRPpreprocessorWordIdentification.determineIsVerb(currentWordTag, usePOSprelim, grammaticallyStrict))
	{
		if(LRPpreprocessorWordIdentification.determineIsAdjective(currentWordTag, usePOSprelim))
		{	
			verbAndAdjective = true;
		}
	}
	return verbAndAdjective;
}	
#endif	
				
void GIAposRelTranslatorHybridReferenceSetClass::updateIndices(const bool currentWordIsReferenceSetDelimiter, bool* referenceSetDelimiterDetected, bool* parsingReferenceSetDelimiter, const int currentDelimiterSpecialCase, int wordIndexOfHypotheticalPreceedingThatWhich, const bool currentWordIsReferenceSetDelimiterPreceededByThatWhich, bool* referenceSetDelimiterIndicatesSameReferenceSet, int* firstIndexOfReferenceSetDelimiterText, int* lastIndexOfPreviousReferenceSet) 
{
	if(currentWordIsReferenceSetDelimiter)
	{
		if(*referenceSetDelimiterDetected)
		{
			if(!(*parsingReferenceSetDelimiter))
			{
				cerr << "GIAposRelTranslatorHybridReferenceSetClass::executeReferenceSetPreprocessor{} error: 2 valid referenceSetDelimiters are found in the current logicReference: currentWordIsReferenceSetDelimiter && !currentWordIsReferenceSetDelimiterPreceededByThatWhich && referenceSetDelimiterDetected [ie already/previously detected] && !parsingReferenceSetDelimiter" << endl;
				exit(EXIT_ERROR);
			}
		}
		else
		{	
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_DETECT
			#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_IGNORE
			if(currentDelimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY)
			{
				//case added 3d5d;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY" << endl;
				#endif
				*referenceSetDelimiterIndicatesSameReferenceSet = true;
				*firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
				*lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
			}	
			else 
			#endif
			#endif	
			if(currentDelimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)	//ie && !currentWordIsReferenceSetDelimiterPreceededByThatWhich
			{
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB" << endl;
				#endif
				*referenceSetDelimiterIndicatesSameReferenceSet = true;
				*firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
				*lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
			}
			/*
			else if(currentDelimiterSpecialCase == GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB)
			{
				SPECIFIC NOT REQUIRED;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB" << endl;
				#endif
			}
			*/
			else if(currentWordIsReferenceSetDelimiterPreceededByThatWhich)
			{
				*referenceSetDelimiterIndicatesSameReferenceSet = true;
				#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_DELIMITERS_DISCARD_THAT_WHICH
				*firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;	//NB the "that/which" will be removed from the text
				#else
				*firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich;
				#endif
				*lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich - 1;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "referenceSetDelimiterIndicatesSameReferenceSet" << endl;
				#endif
			}
			else
			{
				*firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
				*lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
			}
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "\tlastIndexOfPreviousReferenceSet = " << lastIndexOfPreviousReferenceSet << endl;
			#endif
			*referenceSetDelimiterDetected = true;
			*parsingReferenceSetDelimiter = true;
		}
	}
}

		
bool GIAposRelTranslatorHybridReferenceSetClass::formSubReferenceSetTextFromWordList(const vector<LRPpreprocessorPlainTextWord*>* logicReferenceVariableWordList, vector<LRPpreprocessorPlainTextWord*>* subReferenceSetText, const int firstIndexOfSubReferenceSet, const int lastIndexOfSubReferenceSet)
{
	bool result = true;
	
	int wordIndex = 0;
	for(vector<LRPpreprocessorPlainTextWord*>::const_iterator iter = logicReferenceVariableWordList->begin(); iter != logicReferenceVariableWordList->end(); iter++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (*iter);
		if((wordIndex >= firstIndexOfSubReferenceSet) && (wordIndex <= lastIndexOfSubReferenceSet))
		{
			subReferenceSetText->push_back(currentWord);
		}
		wordIndex++;
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIAposRelTranslatorHybridReferenceSetClass::formSubReferenceSetTextFromWordList{} debug: subReferenceSetText = " << LRPpreprocessorWordClassObject.generateTextFromVectorWordList(subReferenceSetText) << endl;
	#endif
	
	return result;
}

bool GIAposRelTranslatorHybridReferenceSetClass::addReferenceSetToReferenceSet(LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<LRPpreprocessorPlainTextWord*>* subReferenceSetText, bool referenceSetDelimiter, int wordIndexLogicReference, int wordIndexSentence, int delimiterType, int delimiterSpecialCase)
{
	return addSubReferenceSetToReferenceSet(firstSubReferenceSetInList, subReferenceSetText, referenceSetDelimiter, wordIndexLogicReference, wordIndexSentence, delimiterType, delimiterSpecialCase);
}

bool GIAposRelTranslatorHybridReferenceSetClass::addSubReferenceSetToReferenceSet(LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<LRPpreprocessorPlainTextWord*>* subReferenceSetText, bool referenceSetDelimiter, int wordIndexLogicReference, int wordIndexSentence, int delimiterType, int delimiterSpecialCase)
{
	bool result = true;
	
	bool isFirstSubreferenceSetInList = true;
	LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
		isFirstSubreferenceSetInList = false;
	}
	setReferenceSetText(currentSubReferenceSetInList, subReferenceSetText);	//currentSubReferenceSetInList->subReferenceSetContents = subReferenceSetText;
	currentSubReferenceSetInList->isReferenceSetDelimiter = referenceSetDelimiter;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	//cout << "* addSubReferenceSetToReferenceSet: " << endl;
	//cout << "* wordIndexSentence: " << wordIndexSentence << endl;
	//cout << "* wordIndexLogicReference: " << wordIndexLogicReference << endl;
	currentSubReferenceSetInList->firstIndexOfReferenceSetText = wordIndexLogicReference + wordIndexSentence;
	currentSubReferenceSetInList->lastIndexOfReferenceSetText = currentSubReferenceSetInList->firstIndexOfReferenceSetText + LRPpreprocessorWordClassObject.calculateLengthOfGeneratedVectorWordListText(subReferenceSetText);
	#endif
	if(referenceSetDelimiter)
	{
		currentSubReferenceSetInList->delimiterType = delimiterType;
		currentSubReferenceSetInList->delimiterSpecialCase = delimiterSpecialCase;
	}
	currentSubReferenceSetInList->isFirstSubreferenceSetInList = isFirstSubreferenceSetInList;
	
	currentSubReferenceSetInList->next = new LRPpreprocessorSubReferenceSet();
	
	return result;
}

#endif

bool GIAposRelTranslatorHybridReferenceSetClass::hasReferenceSet(LRPpreprocessorSubReferenceSet* referenceSet)
{
	bool referenceSetEnabled = false; 
	if(!(referenceSet->subReferenceSetContents).empty())
	{
		referenceSetEnabled = true;
	}
	return referenceSetEnabled;
}

void GIAposRelTranslatorHybridReferenceSetClass::setReferenceSetText(LRPpreprocessorSubReferenceSet* referenceSet, vector<LRPpreprocessorPlainTextWord*>* text)
{
	referenceSet->subReferenceSetContents = *text;
}

vector<LRPpreprocessorPlainTextWord*> GIAposRelTranslatorHybridReferenceSetClass::getReferenceSetText(const LRPpreprocessorSubReferenceSet* referenceSet)
{
	return referenceSet->subReferenceSetContents;
}

void GIAposRelTranslatorHybridReferenceSetClass::printSubReferenceSet(const LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList)
{
	for(int i=0; i<(currentSubReferenceSetInList->subReferenceSetContents).size(); i++)
	{
		cout << "(currentSubReferenceSetInList->subReferenceSetContents)[i] = " << (currentSubReferenceSetInList->subReferenceSetContents)[i]->tagName << endl; 
	}
}

#endif


#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT
bool GIAposRelTranslatorHybridReferenceSetClass::verifyIsPrepositionNotProgressiveVerbBasedOnContext(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsWordList, int prepositionWordIndex, bool usePOSprelim)
{
	bool result = true;

	#ifdef GIA_POS_REL_TRANSLATOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY
	bool grammaticallyStrict = true;
	#else
	bool grammaticallyStrict = false;
	#endif
		
	/*
	Determine pos type of ambiguous prepositions (eg "considering") as preposition rather than verb (VBG/progressive) based on context;
		method i) if the potential preposition a) is preceeded by a "," and b) not followed by an explicit conjunction "and", or;
			if followed by explicit and then it might be this verb case; "the cat was contemplating, considering, and examining the apple"
				what about scenarios where the and conjunction does not pertain to the current ", , , and" context?
				need to accomodate this preposition case; "the cat was happy, considering the house, and rode the bike"
		method ii) if the potential preposition is not preceeded by a being auxiliary (eg "was considering"/"is considering").
			doesn't support verb case conjunctions e.g.; "the cat was contemplating, considering, and examining the apple", so must integrate with method i)
	*/
		
	//LRPpreprocessorPlainTextWord* wordTagPreposition = sentenceContentsWordList->at(prepositionWordIndex);
	if(prepositionWordIndex > 0)
	{
		LRPpreprocessorPlainTextWord* wordTagPrepositionPrior = sentenceContentsWordList->at(prepositionWordIndex-1);
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT_METHOD1
		if(wordTagPrepositionPrior->tagName == SHAREDvars.convertCharToString(GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR))
		{
			bool foundConjunctionAndAfterPreposition = false;
			bool stillFindingValidVerbConjunctionSequence = true;
			bool foundConjunction = false;
			bool expectVerb = true; //else expect comma or and 
			for(int i=prepositionWordIndex; i<sentenceContentsWordList->size(); i++)
			{
				if(stillFindingValidVerbConjunctionSequence)
				{
					if(LRPpreprocessorWordIdentification.determineIsConjunction(sentenceContentsWordList->at(i), usePOSprelim))
					{
						expectVerb = true;
						foundConjunction = true;
					}
					else
					{
						if(expectVerb)
						{
							if(LRPpreprocessorWordIdentification.determineIsVerb(sentenceContentsWordList->at(i), usePOSprelim, grammaticallyStrict))
							{
								if(foundConjunction)
								{
									foundConjunctionAndAfterPreposition = true;
								}
								expectVerb = false;
							}
							else
							{
								stillFindingValidVerbConjunctionSequence = false;
							}
						}
						else
						{
							if(sentenceContentsWordList->at(i)->tagName == SHAREDvars.convertCharToString(GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR))
							{
								expectVerb = true;
							}
							else
							{
								stillFindingValidVerbConjunctionSequence = false;
							}
						}
					}
				}
			}
			if(foundConjunctionAndAfterPreposition)
			{
				result = false;
			}
		}
		else
		{
		#endif
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT_METHOD2
			if(LRPpreprocessorWordIdentification.determineIsAuxiliaryBeing(wordTagPrepositionPrior, usePOSprelim))
			{
				result = false;
			}
			#else
			result = false;
			#endif
		#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DETERMINE_AMBIGUOUS_PREPOSITION_POS_TYPES_BASED_ON_CONTEXT_METHOD1
		}
		#endif

	}
	else
	{
		result = true;	//first word in sentence
	}
	
	return result;
}
#endif




