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
 * File Name: GIApreprocessorReferenceSet.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3e2c 10-December-2017
 * Requirements: requires plain text file
 * Description: Reference Set preprocessor
 *
 *******************************************************************************/


#include "GIApreprocessorReferenceSet.hpp"
#include "GIAtranslatorDefs.hpp"

#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET

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



bool GIApreprocessorReferenceSetClass::executeReferenceSetPreprocessor(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence)
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
	#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
	int delimiterTypeFull = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
	int delimiterSpecialCaseFull = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;
	#endif
	bool referenceSetDelimiterIndicatesSameReferenceSet = false;
	int firstIndexOfReferenceSetDelimiterText = INT_DEFAULT_VALUE;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
	int lastIndexOfReferenceSetDelimiterText = 0;
	int lastIndexOfPreviousReferenceSet = INT_DEFAULT_VALUE;
	#endif	
	bool parsingReferenceSetDelimiter = false;
	
	bool referenceSetDelimiterIsLastWordInSentence = false;

	int currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
	int currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;	
	
	bool previousWordWasVerb = false;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
	bool usePOSprelim = true;
	#else
	bool usePOSprelim = false;
	#endif
	
	//detect auxiliary or verb (present/past/future tense)
	int wordIndex = 0;
	for(int wordIndex = 0; wordIndex<logicReferenceVariableWordList->size(); wordIndex++)
	{
		GIApreprocessorWord* currentWordTag = (*logicReferenceVariableWordList)[wordIndex];
		string currentWord = currentWordTag->tagName;
		currentWord = SHAREDvars.convertStringToLowerCase(&currentWord);	//required for imperatives where the action is defined as the first word in the sentence 
		int wordIndexOfHypotheticalPreceedingThatWhich = wordIndex-1;
					
		bool currentWordIsReferenceSetDelimiter = false;
		//auxiliary detection
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "\t\tcurrentWord = " << currentWord << endl;
		#endif
		
		if(detectAuxiliary(currentWord))
		{	
			//cout << "detectAuxiliary: currentWord = " << currentWord << endl;
		
			currentWordIsReferenceSetDelimiter = true;
			currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY;
		}
		
		bool verbDetected = false;
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
		if(GIApreprocessorMultiwordReduction.determineIsVerb(currentWordTag, usePOSprelim))
		{
			verbDetected = true;
		}
		if(verbDetected)
		{
		#else
		int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
		string verbBaseNameTemp = "";
		if(GIApreprocessorMultiwordReduction.determineIsVerb(currentWordTag, usePOSprelim, &verbBaseNameTemp, &grammaticalBaseTenseForm))	//OLD: determineVerbCaseStandard
		{
			if(grammaticalBaseTenseForm != GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
				verbDetected = true;
			}
		}
		if(verbDetected)
		{
			//cout << "verbDetected: 	currentWord = " << currentWord << endl;
						
			//infinitive/present/past tense verb detection (NB infinitive detection is required for a) future tense detection and b) imperatives where the action is defined as the first word in the sentence)
			/*
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
			*/

					
			//why are these checks required if(grammaticalBaseTenseForm != GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)?
			bool referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = false;
			if(wordIndex-1 >= 0)
			{
				//eg the [det] walk [verb]
				if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-1])->tagName, relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
				SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-1])->tagName, entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
				{
					referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
				}
			}
			if(wordIndex-2 >= 0)
			{
				//eg the [det] wonderful [adj] walk [verb]
				if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-2])->tagName, relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
				SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-2])->tagName, entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
				{
					if(GIApreprocessorMultiwordReduction.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
					{
						referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
					}
				}
			}
			if(wordIndex-3 >= 0)
			{
				//eg the [det] very [adv] wonderful [adj] walk [verb]
				if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-3])->tagName, relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
				SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-3])->tagName, entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
				{
					if(GIApreprocessorMultiwordReduction.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex-2]), usePOSprelim))
					{
						if(GIApreprocessorMultiwordReduction.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
						{
							referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
						}
					}
				}
			}
			if(!referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun)
			{
		#endif
				currentWordIsReferenceSetDelimiter = true;
				currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB;
				//not required: verify that the verb is not preceeded by one or more auxiliaries (because it will have already been detected by (currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) when testing the previously parsed auxiliary)

				if(wordIndex+1 < logicReferenceVariableWordList->size())
				{
					#ifndef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
					if(!GIApreprocessorMultiwordReduction.determineIsNoun(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))	//compensate for ambiguity in grammatical classification of words (e.g. "chicken" is classified as both a noun and an adjective by wordnet; eg A controlled chicken was moved to the car.
					{
					#endif
						if(GIApreprocessorMultiwordReduction.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))
						{
							//eg Tom rides fast
							currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
							#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
							wordIndex = wordIndex + 1;
							#endif
						}
						if(wordIndex+2 < logicReferenceVariableWordList->size())
						{
							if(GIApreprocessorMultiwordReduction.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex+1]), usePOSprelim))
							{
								if(GIApreprocessorMultiwordReduction.determineIsAdjective(((*logicReferenceVariableWordList)[wordIndex+2]), usePOSprelim))
								{
									//eg Tom rides very fast
									currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
									#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
									wordIndex = wordIndex + 2;
									#endif
								}
							}
						}
					#ifndef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
					}
					#endif
				}
		#ifdef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
		}
		#else
			}
		}
		#endif
		
		if(GIApreprocessorMultiwordReduction.determineIsPreposition(currentWordTag, usePOSprelim))
		{
			currentWordIsReferenceSetDelimiter = true;
			currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION;
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
						
			#ifdef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB)))
			#else
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)))
			#endif
			{
				if(wordIndex-1 >= 0)
				{
					if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-1])->tagName, entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
					{	
						previousWordIsAuxiliary = true;
						wordIndexOfHypotheticalPreceedingThatWhich--;
					}
				}
			}
			//verify that the auxiliary/verb is not preceeded by an auxiliary (e.g. for doing auxiliaries; is being/having/doing, or for verbs; is riding, or for prepositions; is near), in which case must test the word prior to the modal auxiliary for that/which
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || (currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) || (currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
			{
				if(wordIndex-1 >= 0)
				{
					if(detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-1])->tagName))
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
							if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-2])->tagName, entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
							{
								//eg (that) will be riding
								wordIndexOfHypotheticalPreceedingThatWhich--;
							}
							else if(detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-2])->tagName))
							{
								wordIndexOfHypotheticalPreceedingThatWhich--;
								//eg (that) have been riding
								if(wordIndex-3 >= 0)
								{
									if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndex-3])->tagName, entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
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
						if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
						{
							if(GIApreprocessorMultiwordReduction.determineIsAdverb(((*logicReferenceVariableWordList)[wordIndex-1]), usePOSprelim))
							{
								if(wordIndex-2 >= 0)
								{
									if(detectAuxiliary(((*logicReferenceVariableWordList)[wordIndex-2])->tagName))
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
				if(SHAREDvars.textInTextArray(((*logicReferenceVariableWordList)[wordIndexOfHypotheticalPreceedingThatWhich])->tagName, preprocessorRcmodSameReferenceSetDelimiter, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET_DELIMITER_NUMBER_OF_TYPES))
				{
					currentWordIsReferenceSetDelimiterPreceededByThatWhich = true;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "currentWordIsReferenceSetDelimiterPreceededByThatWhich" << endl;
					#endif
				}
			}
			
			#ifndef GIA_PREPROCESSOR_SENTENCE_PREFERENCE_NLP_PRELIM_POS_TAGS_OVER_LRP_WORD_TYPE_LISTS
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_DETECT
			//added 3d5d;
			if(currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) 
			{
				if(!currentWordIsReferenceSetDelimiterPreceededByThatWhich)
				{
					if(!previousWordIsAuxiliary)
					{
						//NB this doesn't yet work (not all past tense are necessarily same reference set); e.g. the dog ran_to the house
						if((grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) || 	//eg the moved bike was sad
						(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS) || 	//eg the moving bike was sad
						(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION) || 	//eg the movement bike was sad
						(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL) || 	//eg the moveable bike was sad
						(grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE))	//eg the movive bike was sad
						{
							currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY;	//eg "controlled" in "a controlled chicken was moved to"
							#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_IGNORE
							currentWordIsReferenceSetDelimiter = false;
							#endif
							cout << "\t\tcurrentWord = " << currentWord << endl;
							cout << "GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY" << endl;
							cout << "grammaticalBaseTenseForm = " << grammaticalBaseTenseForm << endl;
						}
					}
				}
			}
			#endif
			#endif
			
			//NB if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)): "that near" is not legal english (only "that is near"), but will be accepted here anyway
			//NB near to should have previously been compressed to near_to by GIA_PREPROCESSOR_MULTIWORD_REDUCTION
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION) && !previousWordIsAuxiliary && !currentWordIsReferenceSetDelimiterPreceededByThatWhich)
			{
				//eg the dog eats the apple near the bike.	[preposition near refers to verb eat]	[as compared to the dog eats the apple that is near the bike].
				currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB;
			}
		}
		
		bool currentWordIsReferenceSetDelimiterAndIsLastWordInSentence = (currentWordIsReferenceSetDelimiter && (wordIndex == logicReferenceVariableWordList->size()-1));
		int referenceSetDelimiterWordIndex = wordIndex-1;
		if(currentWordIsReferenceSetDelimiterAndIsLastWordInSentence)
		{
			referenceSetDelimiterWordIndex = wordIndex;
			referenceSetDelimiterIsLastWordInSentence = true;
		}
		bool currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb = false;
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUPPORT_CONSECUTIVE_VERBS
		if(currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB)
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
					
		if(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && (!currentWordIsReferenceSetDelimiter || currentWordIsReferenceSetDelimiterAndIsLastWordInSentence || currentWordIsReferenceSetDelimiterAndPreviousWordWasVerb))
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter): currentWord = " << currentWord << endl;
			#endif
				
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
			//add a subreference set to the subject/object
			if(!referenceSetDelimiterDetectedFull)
			{
				vector<GIApreprocessorWord*> subReferenceSetText;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "lastIndexOfReferenceSetDelimiterText = " << lastIndexOfReferenceSetDelimiterText << endl;
				cout << "lastIndexOfPreviousReferenceSet = " << lastIndexOfPreviousReferenceSet << endl;
				#endif
				if(lastIndexOfReferenceSetDelimiterText <= lastIndexOfPreviousReferenceSet)	//added 3d5d
				{
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t12 subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						GIApreprocessorMultiwordReductionClassObject.printWordList(&subReferenceSetText);
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
						cout << "\t13 subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						GIApreprocessorMultiwordReductionClassObject.printWordList(&subReferenceSetText);
						#endif
					
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
					else
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t14 subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						#endif
						addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetDelimiter, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
			}
			else
			{
				if(referenceSetDelimiterIndicatesSameReferenceSet)
				{
					vector<GIApreprocessorWord*> subReferenceSetText;
					if(lastIndexOfReferenceSetDelimiterText <= lastIndexOfPreviousReferenceSet)	//added 3d5d
					{
						if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
						{
							#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
							cout << "\t15 subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
							#endif
							addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
						}
					}
					
					//add the sub referenceSetDelimiter to the subReferenceSet list:
					subReferenceSetText.clear();
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, firstIndexOfReferenceSetDelimiterText, referenceSetDelimiterWordIndex))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t16 subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&subReferenceSetText) << endl;
						#endif
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, &subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
				else
				{
					cerr << "GIApreprocessorReferenceSetClass::executeReferenceSetPreprocessor{} error: (referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter) && !referenceSetDelimiterIndicatesSameReferenceSet - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
					GIApreprocessorMultiwordReductionClassObject.printWordList(logicReferenceVariableWordList);
					exit(EXIT_ERROR);
				}
			}
			#endif

			if(!referenceSetDelimiterIndicatesSameReferenceSet)
			{
				if(referenceSetDelimiterDetectedFull)
				{
					cerr << "GIApreprocessorReferenceSetClass::executeReferenceSetPreprocessor{} error: (referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter) && (!referenceSetDelimiterIndicatesSameReferenceSet) && referenceSetDelimiterDetectedFull already - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
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
					#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
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
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
			lastIndexOfReferenceSetDelimiterText = wordIndex;
			#endif
		}
	
		//moved 3d5d
		if(currentWordIsReferenceSetDelimiter)
		{
			if(referenceSetDelimiterDetected)
			{
				if(!parsingReferenceSetDelimiter)
				{
					cerr << "GIApreprocessorReferenceSetClass::executeReferenceSetPreprocessor{} error: 2 valid referenceSetDelimiters are found in the current logicReference: currentWordIsReferenceSetDelimiter && !currentWordIsReferenceSetDelimiterPreceededByThatWhich && referenceSetDelimiterDetected [ie already/previously detected] && !parsingReferenceSetDelimiter" << endl;
					exit(EXIT_ERROR);
				}
			}
			else
			{	
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_DETECT
				#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY_IGNORE
				if(currentDelimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY)
				{
					//case added 3d5d;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY" << endl;
					#endif
					referenceSetDelimiterIndicatesSameReferenceSet = true;
					firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
					lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
				}	
				else 
				#endif
				#endif	
				if(currentDelimiterSpecialCase == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB)	//ie && !currentWordIsReferenceSetDelimiterPreceededByThatWhich
				{
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB" << endl;
					#endif
					referenceSetDelimiterIndicatesSameReferenceSet = true;
					firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
					lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
				}
				else if(currentWordIsReferenceSetDelimiterPreceededByThatWhich)
				{
					referenceSetDelimiterIndicatesSameReferenceSet = true;
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_DISCARD_THAT_WHICH
					firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;	//NB the "that/which" will be removed from the text
					#else
					firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich;
					#endif
					lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich - 1;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "referenceSetDelimiterIndicatesSameReferenceSet" << endl;
					#endif
				}
				else
				{
					firstIndexOfReferenceSetDelimiterText = wordIndexOfHypotheticalPreceedingThatWhich + 1;
					lastIndexOfPreviousReferenceSet = wordIndexOfHypotheticalPreceedingThatWhich;
				}
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "\tlastIndexOfPreviousReferenceSet = " << lastIndexOfPreviousReferenceSet << endl;
				#endif
				referenceSetDelimiterDetected = true;
				parsingReferenceSetDelimiter = true;
			}
		}
		
		#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUPPORT_CONSECUTIVE_VERBS
		if(currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) 
		{
			previousWordWasVerb = true;
		}
		else
		{
			previousWordWasVerb = false;
		}
		#endif
	}
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
	//add final subReferenceSet to list;
	if(!referenceSetDelimiterIsLastWordInSentence)
	{
		if(referenceSetDelimiterDetectedFull)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "end referenceSetDelimiterDetectedFull" << endl;
			#endif
			vector<GIApreprocessorWord*> subReferenceSetText;
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
			vector<GIApreprocessorWord*> subReferenceSetText;
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
		
		vector<GIApreprocessorWord*> referenceSetSubjectText;
		vector<GIApreprocessorWord*> referenceSetObjectText;
		vector<GIApreprocessorWord*> referenceSetDelimiterText;
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
		cout << "logicReferenceVariable->referenceSetSubject->subReferenceSetContents = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetSubject->subReferenceSetContents) << endl;
		cout << "logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents) << endl;
		cout << "logicReferenceVariable->referenceSetObject->subReferenceSetContents = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(&logicReferenceVariable->referenceSetObject->subReferenceSetContents) << endl;
		#endif
	}
	else
	{
		/*
		NB if no referenceSetDelimiterDetectedFull, then expect a future and/or to be defined (ie the GIApreprocessorLogicReferenceVariable is part of a conjunction list):
		1. once this is detected by executeLogicReferencePreprocessor: go back and reparse based on the first GIApreprocessorLogicReferenceVariable of type GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN
		2. extract the subject (referenceSetSubject) from this first GIApreprocessorLogicReferenceVariable in the conjunction list
		3. add the explicit subject to the phrase (see NLCpreprocessor code on how precisely to do this), reexecute executeReferenceSetPreprocessor on the GIApreprocessorLogicReferenceVariable
		see identifyConjunctionReferenceSets{} for details.
		*/
	}
	#endif
	
	if(expectToFindSubjectAuxObjectLogicReferenceVariable)
	{
		if(!referenceSetDelimiterDetectedFull)
		{
			cerr << "GIApreprocessorClass::executeReferenceSetPreprocessor{} error: expectToFindSubjectAuxObjectLogicReferenceVariable && !referenceSetDelimiterDetectedFull" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return result;
	
}

bool GIApreprocessorReferenceSetClass::formSubReferenceSetTextFromWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, vector<GIApreprocessorWord*>* subReferenceSetText, int firstIndexOfSubReferenceSet, int lastIndexOfSubReferenceSet)
{
	bool result = true;
	
	int wordIndex = 0;
	for(vector<GIApreprocessorWord*>::const_iterator iter = logicReferenceVariableWordList->begin(); iter != logicReferenceVariableWordList->end(); iter++)
	{
		GIApreprocessorWord* currentWord = (*iter);
		if((wordIndex >= firstIndexOfSubReferenceSet) && (wordIndex <= lastIndexOfSubReferenceSet))
		{
			subReferenceSetText->push_back(currentWord);
		}
		wordIndex++;
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIApreprocessorReferenceSetClass::formSubReferenceSetTextFromWordList{} debug: subReferenceSetText = " << GIApreprocessorMultiwordReductionClassObject.generateTextFromVectorWordList(subReferenceSetText) << endl;
	#endif
	
	return result;
}

bool GIApreprocessorReferenceSetClass::addReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<GIApreprocessorWord*>* subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase)
{
	return addSubReferenceSetToReferenceSet(firstSubReferenceSetInList, subReferenceSetText, referenceSetDelimiter, wordIndexLogicReference, wordIndexSentence, delimiterType, delimiterSpecialCase);
}

bool GIApreprocessorReferenceSetClass::addSubReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, vector<GIApreprocessorWord*>* subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase)
{
	bool result = true;
	
	bool firstSubreferenceSetInList = true;
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
		firstSubreferenceSetInList = false;
	}
	setReferenceSetText(currentSubReferenceSetInList, subReferenceSetText);	//currentSubReferenceSetInList->subReferenceSetContents = subReferenceSetText;
	currentSubReferenceSetInList->isReferenceSetDelimiter = referenceSetDelimiter;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	//cout << "* addSubReferenceSetToReferenceSet: " << endl;
	//cout << "* wordIndexSentence: " << wordIndexSentence << endl;
	//cout << "* wordIndexLogicReference: " << wordIndexLogicReference << endl;
	currentSubReferenceSetInList->firstIndexOfReferenceSetText = wordIndexLogicReference + wordIndexSentence;
	currentSubReferenceSetInList->lastIndexOfReferenceSetText = currentSubReferenceSetInList->firstIndexOfReferenceSetText + GIApreprocessorMultiwordReductionClassObject.calculateLengthOfGeneratedVectorWordListText(subReferenceSetText);
	#endif
	if(referenceSetDelimiter)
	{
		currentSubReferenceSetInList->delimiterType = delimiterType;
		currentSubReferenceSetInList->delimiterSpecialCase = delimiterSpecialCase;
	}
	currentSubReferenceSetInList->firstSubreferenceSetInList = firstSubreferenceSetInList;
	
	currentSubReferenceSetInList->next = new GIApreprocessorSubReferenceSet();
	
	return result;
}



bool GIApreprocessorReferenceSetClass::detectAuxiliary(const string currentWord)
{
	bool auxiliaryFound = false;
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryBeingArray, ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES))
	{
		auxiliaryFound = true;
	}
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryHavingArray, ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES))
	{
		auxiliaryFound = true;
	}
	if(SHAREDvars.textInTextArray(currentWord, entityAuxiliaryDoingArray, ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES))
	{
		auxiliaryFound = true;
	}
	return auxiliaryFound;
}

bool GIApreprocessorReferenceSetClass::hasReferenceSet(GIApreprocessorSubReferenceSet* referenceSet)
{
	bool referenceSetEnabled = false; 
	if(!(referenceSet->subReferenceSetContents).empty())
	{
		referenceSetEnabled = true;
	}
	return referenceSetEnabled;
}

void GIApreprocessorReferenceSetClass::setReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet, vector<GIApreprocessorWord*>* text)
{
	referenceSet->subReferenceSetContents = *text;
}

vector<GIApreprocessorWord*> GIApreprocessorReferenceSetClass::getReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet)
{
	return referenceSet->subReferenceSetContents;
}

void GIApreprocessorReferenceSetClass::printSubReferenceSet(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList)
{
	for(int i=0; i<(currentSubReferenceSetInList->subReferenceSetContents).size(); i++)
	{
		cout << "(currentSubReferenceSetInList->subReferenceSetContents)[i] = " << (currentSubReferenceSetInList->subReferenceSetContents)[i]->tagName << endl; 
	}
}

#endif





