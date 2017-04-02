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
 * Project Version: 3a5a 28-March-2017
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

//preconditions: must conform to wordIndex identification in GIApreprocessorMultiwordReductionClass::loadPlainTextFile (which is the same as that used in NLP). See also GIApreprocessorLogicReferenceClass::executeLogicReferencePreprocessor
bool GIApreprocessorReferenceSetClass::generateSentenceWordList(const string* sentenceContents, vector<string>* logicReferenceVariableWordList)
{
	bool result = true;
	
	int wordIndexSentence = 0;
	string currentWord = "";
	string currentContents = "";
	bool whiteSpace = false;
		
	for(int i=0; i<sentenceContents->length(); i++)
	{
		char currentChar = (*sentenceContents)[i];
		currentContents = currentContents + currentChar;
		cout << currentChar;
		
		bool whiteSpaceFound = false;
		bool punctuationMarkFound = false;
		if(!whiteSpace)
		{
			if(SHAREDvars.charInCharArray(currentChar, nlpWhitespaceCharacterArray, GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS))
			{
				whiteSpaceFound = true;
				
				logicReferenceVariableWordList->push_back(currentWord);
				
				currentWord = "";
				wordIndexSentence++;
			}
			else if(SHAREDvars.charInCharArray(currentChar, nlpPunctionMarkCharacterEndOfSentenceArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE))
			{
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
				if(GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(i, sentenceContents))
				{
					currentWord = currentWord + currentChar;
				}
				else
				{
				#endif	
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "GIApreprocessorReferenceSetClass::generateSentenceWordList{}: currentWord = " << currentWord << endl;
					#endif
					if(currentContents == "")
					{
						cout << "GIApreprocessor::generateSentenceWordList{} error: GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(currentChar, &sentenceContents) && (currentContents == "")" << endl;
						exit(EXIT_ERROR);	
					}
					else
					{
						logicReferenceVariableWordList->push_back(currentWord);
					}

					/* 
					//not required because finished parsing sentence;
					punctuationMarkFound = true;
					currentWord = "";
					wordIndexSentence++;
					*/
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
				}
				#endif
			}
			else if(SHAREDvars.charInCharArray(currentChar, nlpPunctionMarkCharacterArray, GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS))
			{
				//CHAR_SEMICOLON / CHAR_COLON:

				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
				if(GIApreprocessorMultiwordReduction.isIntrawordPunctuationMark(i, sentenceContents))
				{
					currentWord = currentWord + currentChar;
				}
				else
				{
				#endif
					currentWord = "";
					wordIndexSentence++;
					punctuationMarkFound = true;
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
				}
				#endif	
			}
			else
			{
				currentWord = currentWord + currentChar;
			}
		}
		else
		{
			//skip (do not parse) multiple white space/punctuation characters (eg ". "/".."/"  "/" .")
			#ifndef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_REMOVE_DOUBLE_WHITE_SPACE
			//removal of all double white space is required to simplify updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo:
			currentWord = currentWord + currentChar;
			#endif
		}
		
		if(whiteSpaceFound || punctuationMarkFound)
		{
			whiteSpace = true;
		}
		else
		{
			whiteSpace = false;
		}
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIApreprocessorReferenceSetClass::generateSentenceWordList{}: " << endl;
	for(int i=0; i<logicReferenceVariableWordList->size(); i++)
	{
		cout << (*logicReferenceVariableWordList)[i] << STRING_SPACE;
	}
	cout << endl;
	#endif
	
	return result;
}



bool GIApreprocessorReferenceSetClass::executeReferenceSetPreprocessor(const string* logicReferenceVariableContents, vector<string>* logicReferenceVariableWordList, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool expectToFindSubjectAuxObjectLogicReferenceVariable, const int wordIndexSentence)
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

	
		
	//detect auxiliary or verb (present/past/future tense)
	int wordIndex = 0;
	for(int wordIndex = 0; wordIndex<logicReferenceVariableWordList->size(); wordIndex++)
	{
		string currentWord = (*logicReferenceVariableWordList)[wordIndex];
		currentWord = SHAREDvars.convertStringToLowerCase(&currentWord);	//required for imperatives where the action is defined as the first word in the sentence 
		int wordIndexOfHypotheticalPreceedingThatWhich = wordIndex-1;
					
		bool currentWordIsReferenceSetDelimiter = false;
		//auxiliary detection
		int currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED;
		int currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED;
		int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "currentWord = " << currentWord << endl;
		#endif
		
		if(detectAuxiliary(currentWord))
		{			
			currentWordIsReferenceSetDelimiter = true;
			currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY;
		}
		if(GIApreprocessorMultiwordReduction.determineVerbCaseStandardWithAdditional(currentWord, &grammaticalBaseTenseForm))	//OLD: determineVerbCaseStandard
		{
			if(grammaticalBaseTenseForm != GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)
			{
				//infinitive/present/past tense verb detection (NB infinitive detection is required for a) future tense detection and b) imperatives where the action is defined as the first word in the sentence)
				/*
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
				*/

				bool referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = false;
				if(wordIndex-1 >= 0)
				{
					//eg the [det] walk [verb]
					if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-1], relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
					SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-1], entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
					{
						referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
					}
				}
				if(wordIndex-2 >= 0)
				{
					//eg the [det] wonderful [adj] walk [verb]
					if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-2], relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
					SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-2], entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
					{
						if(GIApreprocessorMultiwordReduction.determineIsAdjective((*logicReferenceVariableWordList)[wordIndex-1]))
						{
							referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
						}
					}
				}
				if(wordIndex-3 >= 0)
				{
					//eg the [det] very [adv] wonderful [adj] walk [verb]
					if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-3], relationDeterminerArray, GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES) || 
					SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-3], entityPronounPossessiveArray, ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES))
					{
						if(GIApreprocessorMultiwordReduction.determineIsAdverb((*logicReferenceVariableWordList)[wordIndex-2]))
						{
							if(GIApreprocessorMultiwordReduction.determineIsAdjective((*logicReferenceVariableWordList)[wordIndex-1]))
							{
								referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun = true;
							}
						}
					}
				}
				if(!referenceSetDelimiterVerbPreceededByDeterminerOrPossessivePronoun)
				{
					currentWordIsReferenceSetDelimiter = true;
					currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB;
					//not required: verify that the verb is not preceeded by one or more auxiliaries (because it will have already been detected by (currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) when testing the previously parsed auxiliary)


					if(wordIndex+1 < logicReferenceVariableWordList->size())
					{
						if(GIApreprocessorMultiwordReduction.determineIsAdjective((*logicReferenceVariableWordList)[wordIndex+1]))
						{
							//eg Tom rides fast
							currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
							#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
							wordIndex = wordIndex + 1;
							#endif
						}
					}
					if(wordIndex+2 < logicReferenceVariableWordList->size())
					{
						if(GIApreprocessorMultiwordReduction.determineIsAdverb((*logicReferenceVariableWordList)[wordIndex+1]))
						{
							if(GIApreprocessorMultiwordReduction.determineIsAdjective((*logicReferenceVariableWordList)[wordIndex+2]))
							{
								//eg Tom rides very fast
								currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB;
								#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES
								wordIndex = wordIndex + 2;
								#endif
							}
						}
					}
				}
			}
		}
		if(GIApreprocessorMultiwordReduction.determineIsPreposition(currentWord))
		{
			currentWordIsReferenceSetDelimiter = true;
			currentDelimiterType = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION;
		}

		if(currentWordIsReferenceSetDelimiter)
		{
		
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "currentWordIsReferenceSetDelimiter: currentWord = " << currentWord << endl;
			cout << "\tcurrentDelimiterType = " << currentDelimiterType << endl;
			#endif
			
			bool previousWordIsModalAuxiliary = false;
			
			//verify that the auxiliary/verb is not preceeded by a modal auxiliary (e.g. for future cases; will be/have/ride), in which case must test the word prior to the modal auxiliary for that/which
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)))
			{
				if(wordIndex-1 >= 0)
				{
					if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-1], entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
					{	
						previousWordIsModalAuxiliary = true;
						wordIndexOfHypotheticalPreceedingThatWhich--;
					}
				}
			}
			//verify that the auxiliary/verb is not preceeded by an auxiliary (e.g. for doing auxiliaries; is being/having/doing, or for verbs; is riding, or for prepositions; is near), in which case must test the word prior to the modal auxiliary for that/which
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY) || ((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB) && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE)) || (currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
			{
				if(wordIndex-1 >= 0)
				{
					if(detectAuxiliary((*logicReferenceVariableWordList)[wordIndex-1]))
					{	
						//eg that is riding
						previousWordIsModalAuxiliary = true;
						wordIndexOfHypotheticalPreceedingThatWhich--;
						
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "previousWordIsModalAuxiliary" << endl;
						cout << "currentWord = " << currentWord << endl;
						cout << "previousWord = " << (*logicReferenceVariableWordList)[wordIndex-1] << endl;
						cout << "currentDelimiterType = " << currentDelimiterType << endl;
						cout << "wordIndexOfHypotheticalPreceedingThatWhich = " << wordIndexOfHypotheticalPreceedingThatWhich << endl;
						#endif
						
						if(wordIndex-2 >= 0)
						{
							if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndex-2], entityModalAuxiliaryArray, ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES))
							{
								//eg that will be riding
								wordIndexOfHypotheticalPreceedingThatWhich--;
							}
						}
					}
					else
					{
						if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION))
						{
							if(GIApreprocessorMultiwordReduction.determineIsAdverb((*logicReferenceVariableWordList)[wordIndex-1]))
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
				}
			}
		
			bool currentWordIsReferenceSetDelimiterPreceededByThatWhich = false;
			if(wordIndexOfHypotheticalPreceedingThatWhich >= 0)
			{
				if(SHAREDvars.textInTextArray((*logicReferenceVariableWordList)[wordIndexOfHypotheticalPreceedingThatWhich], preprocessorRcmodSameReferenceSetDelimiter, GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET_DELIMITER_NUMBER_OF_TYPES))
				{
					currentWordIsReferenceSetDelimiterPreceededByThatWhich = true;
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "currentWordIsReferenceSetDelimiterPreceededByThatWhich" << endl;
					#endif
				}
			}


			//NB if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION)): "that near" is not legal english (only "that is near"), but will be accepted here anyway
			//NB near to should have previously been compressed to near_to by GIA_PREPROCESSOR_MULTIWORD_REDUCTION
			if((currentDelimiterType == GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION) && !previousWordIsModalAuxiliary && !currentWordIsReferenceSetDelimiterPreceededByThatWhich)
			{
				//eg the dog eats the apple near the bike.	[preposition near refers to verb eat]	[as compared to the dog eats the apple that is near the bike].
				currentDelimiterSpecialCase = GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB;
			}
			
			if(referenceSetDelimiterDetected)
			{
				if(!parsingReferenceSetDelimiter)
				{
					cout << "GIApreprocessor::executeReferenceSetPreprocessor{} error: 2 valid referenceSetDelimiters are found in the current logicReference: currentWordIsReferenceSetDelimiter && !currentWordIsReferenceSetDelimiterPreceededByThatWhich && referenceSetDelimiterDetected [ie already/previously detected] && !parsingReferenceSetDelimiter" << endl;
					cout << "logicReferenceVariableContents = " << *logicReferenceVariableContents << endl;
					exit(EXIT_ERROR);
				}
			}
			else
			{				
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
		
		if(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter)
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "(referenceSetDelimiterDetected && parsingReferenceSetDelimiter && !currentWordIsReferenceSetDelimiter): currentWord = " << currentWord << endl;
			#endif
				
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
			//add a subreference set to the subject/object
			if(!referenceSetDelimiterDetectedFull)
			{
				string subReferenceSetText = "";
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
				cout << "lastIndexOfReferenceSetDelimiterText = " << lastIndexOfReferenceSetDelimiterText << endl;
				cout << "lastIndexOfPreviousReferenceSet = " << lastIndexOfPreviousReferenceSet << endl;
				#endif
				if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
				{
					#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
					cout << "\t12 subReferenceSetText = " << subReferenceSetText << endl;
					#endif
					addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
				}

				//add the sub referenceSetDelimiter to the subReferenceSet list:
				subReferenceSetText = "";
				if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, firstIndexOfReferenceSetDelimiterText, wordIndex-1))
				{
					if(referenceSetDelimiterIndicatesSameReferenceSet)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t13 subReferenceSetText = " << subReferenceSetText << endl;
						#endif
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
					else
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t14 subReferenceSetText = " << subReferenceSetText << endl;
						#endif
						addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetDelimiter, subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
			}
			else
			{
				if(referenceSetDelimiterIndicatesSameReferenceSet)
				{
					string subReferenceSetText = "";
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, lastIndexOfPreviousReferenceSet))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t15 subReferenceSetText = " << subReferenceSetText << endl;
						#endif
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
					}
					
					//add the sub referenceSetDelimiter to the subReferenceSet list:
					subReferenceSetText = "";
					if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, firstIndexOfReferenceSetDelimiterText, wordIndex-1))
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
						cout << "\t16 subReferenceSetText = " << subReferenceSetText << endl;
						#endif
						addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, subReferenceSetText, true, firstIndexOfReferenceSetDelimiterText, wordIndexSentence, currentDelimiterType, currentDelimiterSpecialCase);
					}
				}
				else
				{
					cout << "GIApreprocessor::executeReferenceSetPreprocessor{} error: referenceSetDelimiterDetectedFull && referenceSetDelimiterDetectedFull already - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
					exit(EXIT_ERROR);
				}
			}
			#endif

			if(!referenceSetDelimiterIndicatesSameReferenceSet)
			{
				if(referenceSetDelimiterDetectedFull)
				{
					cout << "GIApreprocessor::executeReferenceSetPreprocessor{} error: referenceSetDelimiterDetectedFull && referenceSetDelimiterDetectedFull already - a sentence can only contain 1 referenceSetDelimiterFull: currentWord = " << currentWord << endl;
					cout << "wordIndex = " << wordIndex << endl;
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
	}
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
	//add final subReferenceSet to list;
	if(referenceSetDelimiterDetectedFull)
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "end referenceSetDelimiterDetectedFull" << endl;
		#endif
		string subReferenceSetText = "";
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, logicReferenceVariableWordList->size()-1))
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "\t17 subReferenceSetText = " << subReferenceSetText << endl;
			#endif
			addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
		}	
	}
	else
	{
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "end !referenceSetDelimiterDetectedFull" << endl;
		#endif
		string subReferenceSetText = "";
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &subReferenceSetText, lastIndexOfReferenceSetDelimiterText, logicReferenceVariableWordList->size()-1))
		{
			#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			cout << "\t18 subReferenceSetText = " << subReferenceSetText << endl;
			#endif
			addSubReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, subReferenceSetText, false, lastIndexOfReferenceSetDelimiterText, wordIndexSentence, BOOL_NA, BOOL_NA);
		}	
	}
	#else
	if(referenceSetDelimiterDetectedFull)
	{	
		int wordIndex = 0;
		string referenceSetText = "";
		
		string referenceSetSubjectText = "";
		string referenceSetObjectText = "";
		string referenceSetDelimiterText = "";
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "firstIndexOfReferenceSetDelimiterTextFull = " << firstIndexOfReferenceSetDelimiterTextFull << endl;
		cout << "logicReferenceVariableWordList[firstIndexOfReferenceSetDelimiterTextFull] = " << (*logicReferenceVariableWordList)[firstIndexOfReferenceSetDelimiterTextFull] << endl;
		cout << "lastIndexOfReferenceSetDelimiterTextFull = " << lastIndexOfReferenceSetDelimiterTextFull << endl;
		cout << "logicReferenceVariableWordList[lastIndexOfReferenceSetDelimiterTextFull] = " << (*logicReferenceVariableWordList)[lastIndexOfReferenceSetDelimiterTextFull] << endl;
		#endif
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetSubjectText, 0, firstIndexOfReferenceSetDelimiterTextFull-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetSubject, referenceSetSubjectText, false, 0, wordIndexSentence, BOOL_NA, BOOL_NA);
		}
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetDelimiterText, firstIndexOfReferenceSetDelimiterTextFull, lastIndexOfReferenceSetDelimiterTextFull-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetDelimiter, referenceSetDelimiterText, true, firstIndexOfReferenceSetDelimiterTextFull, wordIndexSentence, delimiterTypeFull, delimiterSpecialCaseFull);
		}
		if(formSubReferenceSetTextFromWordList(logicReferenceVariableWordList, &referenceSetObjectText, lastIndexOfReferenceSetDelimiterTextFull, logicReferenceVariableWordList->size()-1))
		{
			addReferenceSetToReferenceSet(logicReferenceVariable->referenceSetObject, referenceSetObjectText, false, lastIndexOfReferenceSetDelimiterTextFull, wordIndexSentence, BOOL_NA, BOOL_NA);
		}		
		
		#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
		cout << "logicReferenceVariable->referenceSetSubject->subReferenceSetContents = " << logicReferenceVariable->referenceSetSubject->subReferenceSetContents << endl;
		cout << "logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents = " << logicReferenceVariable->referenceSetDelimiter->subReferenceSetContents << endl;
		cout << "logicReferenceVariable->referenceSetObject->subReferenceSetContents = " << logicReferenceVariable->referenceSetObject->subReferenceSetContents << endl;
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
			cout << "GIApreprocessorClass::executeReferenceSetPreprocessor{} error: expectToFindSubjectAuxObjectLogicReferenceVariable && !referenceSetDelimiterDetectedFull" << endl;
		}
	}
	
	return result;
	
}

bool GIApreprocessorReferenceSetClass::formSubReferenceSetTextFromWordList(vector<string>* logicReferenceVariableWordList, string* subReferenceSetText, int firstIndexOfSubReferenceSet, int lastIndexOfSubReferenceSet)
{
	bool result = true;
	
	int wordIndex = 0;
	for(vector<string>::iterator iter = logicReferenceVariableWordList->begin(); iter < logicReferenceVariableWordList->end(); iter++)
	{
		string currentWord = *iter;
		if((wordIndex >= firstIndexOfSubReferenceSet) && (wordIndex <= lastIndexOfSubReferenceSet))
		{
			*subReferenceSetText = *subReferenceSetText + currentWord;
		}
		if((wordIndex >= firstIndexOfSubReferenceSet) && (wordIndex < lastIndexOfSubReferenceSet))
		{
			*subReferenceSetText = *subReferenceSetText + STRING_SPACE;			
		}
		wordIndex++;
	}
	
	#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
	cout << "GIApreprocessorReferenceSetClass::formSubReferenceSetTextFromWordList{} debug: subReferenceSetText = " << *subReferenceSetText << endl;
	#endif
	
	return result;
}

bool GIApreprocessorReferenceSetClass::addSubReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, string subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase)
{
	bool result = true;
	
	GIApreprocessorSubReferenceSet* currentSubReferenceSetInList = firstSubReferenceSetInList;
	while(currentSubReferenceSetInList->next != NULL)
	{
		currentSubReferenceSetInList = currentSubReferenceSetInList->next;	
	}
	setReferenceSetText(currentSubReferenceSetInList, subReferenceSetText);	//currentSubReferenceSetInList->subReferenceSetContents = subReferenceSetText;
	currentSubReferenceSetInList->isReferenceSetDelimiter = referenceSetDelimiter;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	currentSubReferenceSetInList->firstIndexOfReferenceSetText = wordIndexLogicReference + wordIndexSentence;
	currentSubReferenceSetInList->lastIndexOfReferenceSetText = currentSubReferenceSetInList->firstIndexOfReferenceSetText + subReferenceSetText.length();
	#endif
	if(referenceSetDelimiter)
	{
		currentSubReferenceSetInList->delimiterType = delimiterType;
		currentSubReferenceSetInList->delimiterSpecialCase = delimiterSpecialCase;
	}
	currentSubReferenceSetInList->next = new GIApreprocessorSubReferenceSet();
	
	return result;
}

bool GIApreprocessorReferenceSetClass::addReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, string subReferenceSetText, const bool referenceSetDelimiter, const int wordIndexLogicReference, const int wordIndexSentence, const int delimiterType, const int delimiterSpecialCase)
{
	return addSubReferenceSetToReferenceSet(firstSubReferenceSetInList, subReferenceSetText, referenceSetDelimiter, wordIndexLogicReference, wordIndexSentence, delimiterType, delimiterSpecialCase);
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
	if(referenceSet->subReferenceSetContents != "")
	{
		referenceSetEnabled = true;
	}
	return referenceSetEnabled;
}

void GIApreprocessorReferenceSetClass::setReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet, string text)
{
	referenceSet->subReferenceSetContents = text;
}

string GIApreprocessorReferenceSetClass::getReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet)
{
	return referenceSet->subReferenceSetContents;
}


#endif






