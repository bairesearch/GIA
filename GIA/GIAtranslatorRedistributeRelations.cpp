/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAtranslatorRedistributeRelations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o2b 12-October-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorRedistributeRelations.h"
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#include "GIAlrp.h"
#endif
#include "GIAtranslatorDefineGrammar.h"
#include "SHAREDvars.h"	//required for convertStringToLowerCase



#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS

//NB Translator:fillGrammaticalArraysStanford{}:extractGrammaticalInformationStanford{}:extractPOSrelatedGrammaticalInformationStanford{}:extractGrammaticalInformationFromPOStag{} performs initial infinitive/imperative determination based on NLP tags and previous word "to" (and sets previousWordInSentenceIsTo for redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs{}:extractPOSrelatedGrammaticalInformationStanford{}:extractGrammaticalInformationFromPOStag{} to reperform infinitive/imperative determination in case Stanford parser/CoreNLP failed to tag the word correctly ie as VB);
void redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[])
{
	//eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif

			int governorIndex = currentRelationInList->relationGovernorIndex;
			int dependentIndex = currentRelationInList->relationDependentIndex;
			GIAentityNode* governorEntity = GIAentityNodeArray[governorIndex];
			GIAentityNode* dependentEntity = GIAentityNodeArray[dependentIndex];

			#ifdef GIA_DEBUG
			cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			cout << "governorEntity->entityName = " << governorEntity->entityName << endl;
			cout << "dependentEntity->entityName = " << dependentEntity->entityName << endl;
			cout << "governorIndex = " << governorIndex << endl;
			cout << "dependentIndex = " << dependentIndex << endl;
			#endif
			#ifdef GIA2_CORRECT_POSTAGS_FIX2
			if(featureArrayTemp[governorIndex] != NULL)
			{
			#endif
				if(correctVerbPOStagAndLemma(governorEntity, featureArrayTemp[governorIndex]))
				{
					currentRelationInList->relationGovernor = governorEntity->entityName;
				}
			#ifdef GIA2_CORRECT_POSTAGS_FIX2
			}
			if(featureArrayTemp[dependentIndex] != NULL)
			{
			#endif
				if(correctVerbPOStagAndLemma(dependentEntity, featureArrayTemp[dependentIndex]))
				{
					currentRelationInList->relationDependent = dependentEntity->entityName;
				}
			#ifdef GIA2_CORRECT_POSTAGS_FIX2
			}
			#endif

		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
}


//note this function tags all "continuous verbs" as VBG (even those which perhaps should be left as NNP because they appear at the beginning at the sentence eg "Swimming is good exercise.")
//note this function can perhaps only be strictly used in circumstances where the continuous verb appears at the end of the sentence eg GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION (because "-ing" cannot be used in itself to detect continuous verbs - as there are some which perhaps should be left as NNP when they appear at the beginning at the sentence eg "Swimming is good exercise.")
bool correctVerbPOStagAndLemma(GIAentityNode* actionOrSubstanceEntity, GIAfeature* currentFeature)
{
	bool updatedLemma = false;
	#ifdef GIA2_CORRECT_POSTAGS_FIX1
	if(actionOrSubstanceEntity->wordOrig != "")		//required to ignore dynamically generated entities, e.g. "have"/"$qvar"/etc
	{
	#endif
		#ifdef GIA_DEBUG
		//cout << "\tcorrectVerbPOStagAndLemma{}:" << endl;
		//cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
		#endif

		string baseNameFound = "";
		int grammaticalTenseModifier = INT_DEFAULT_VALUE;

		bool foundContinuousOrInfinitiveOrImperativeOrPotentialVerb = determineVerbCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound, &grammaticalTenseModifier);
		#ifdef GIA_DEBUG
		//cout << "foundContinuousOrInfinitiveOrImperativeOrPotentialVerb = " << foundContinuousOrInfinitiveOrImperativeOrPotentialVerb << endl;
		//cout << "actionOrSubstanceEntity->wordOrig = " << actionOrSubstanceEntity->wordOrig << endl;
		//cout << "baseNameFound = " << baseNameFound << endl;
		#endif

		//This section of code cannot be used as originally intended as some verb infinitives are also nouns (eg "yarn") - therefore must formally rely on correct infinitive tagging of verbs...
		if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true) || (actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE] == true)))
		{
			//infinitive/imperative verb already detected by NLP/extractGrammaticalInformationFromPOStag{}
		}
		else
		{
			if(currentFeature->stanfordPOS == FEATURE_POS_TAG_VERB_VB)
			{
				cout << "correctVerbPOStagAndLemma{}: expectation error; FEATURE_POS_TAG_VERB_VB already defined" << endl;
			}
			if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP))
			{
				//Should the NLP settings be overruled here - is a verb base form always an imperative or infinitive? Answer = NO; Therefore only perform upgrade for special infinitive cases
				if(currentFeature->previousWordInSentenceIsTo)
				{//only perform upgrade here for special infinitive cases... (eg "to verbbaseform"), as some verbbaseforms are also nouns (eg "tabled" and "table"). Also don't wish to overwrite VBP cases eg "I eat the..."
					string stanfordPOS = FEATURE_POS_TAG_VERB_VB;	//FUTURE GIA - consider using new non-standard pos tage FEATURE_POS_TAG_VERB_VBDESCRIPTION instead of reusing FEATURE_POS_TAG_VERB_VBs

					currentFeature->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature - it should identify the verb as an infinitive/imperative based on previousWordInSentenceIsTo
					applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				}
			}
		}

		//if(actionOrSubstanceEntity->stanfordPOStemp == FEATURE_POS_TAG_VERB_VBG)		//Only Stanford Compatible
		if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && (actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true))	//Relex compatible
		{
			//continuous verb already detected by NLP
			/*
			Wood is used for delivering milk.
			NB delivering is correctly tagged as VBG by stanford CoreNLP (NN by Stanford Parser; and this incorrect value is recorded assuming STANFORD_PARSER_USE_POS_TAGS is set)
			*/
		}
		else
		{
			//FUTURE GIA - consider updating correctVerbPOStagAndLemma{}; currently detecting all instances of "ing"/VBG. This is required such that appropriate instances can be marked as action networkIndexes eg "swimming involves/requires...". Alternatively consider marking these words directly here as GRAMMATICAL_TENSE_MODIFIER_INFINITIVE (ie GRAMMATICAL_TENSE_MODIFIER_ACTIONNETWORK_INDEX) such that they can be assigned action networkIndex by defineActionConcepts2{}
			#ifdef GIA_DEBUG
			//cout << "NB: GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS requires GIA_USE_LRP to be defined and -lrpfolder to be set" << endl;
			//cout << "1 actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
			#endif
			#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
			if(determineIfWordIsIrregularVerbContinuousCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound))
			#elif defined GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
			if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP))
			#endif
			{
				#ifdef GIA_DEBUG
				//cout << "GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS: 2 actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
				#endif

				string stanfordPOS = FEATURE_POS_TAG_VERB_VBG;
				/*
				Wood is used for making milk.
				What is wood used in the making of?
				NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser (this example appears correct @stanford-parser-2013-04-05/stanford-corenlp-2013-04-04 + @stanford-parser-full-2014-01-04/stanford-corenlp-full-2014-01-04)
				*/
				string wordOrigLowerCase = convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = Swimming, and entityName = swimming; then apply the lemma correction
				{
					#ifdef GIA_DEBUG
					//cout << "3 actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
					#endif

					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;	//change irregular verb name eg making to base irregular verb base name eg make
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP_PROGRESSIVE_CASE
					currentFeature->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
					applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP_PROGRESSIVE_CASE
				currentFeature->stanfordPOS = stanfordPOS;
				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}

			/*
			//STANFORD_PARSER_USE_POS_TAGS is no longer supported by GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS/redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs{}...
			#ifdef STANFORD_PARSER_USE_POS_TAGS
			if(determineVerbCase(&(actionOrSubstanceEntity->wordOrig)))	//OR &(currentRelationInList->relationGovernor)	//NB must use wordOrig as only wordOrig is guaranteed to still have "ing" attached - the word may be stripped by stanford corenlp in generation of the lemma
			{
				#ifdef GIA_DEBUG
				//cout << "foundVerb" << endl;
				#endif
				//What is wood used in the delivering of?
				//
				//	Wood is used for making milk.
				//	What is wood used in the making of?
				//	NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser
				//	still fails because "making" remains recorded as the lemma not "make"
				//	a solution involves GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE/GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
				updatedLemma = true;
			}
			#endif
			*/
		}

		#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP))
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ)	//NB "able" words will be marked as JJ/adjective or NN/noun by Stanford/Relex POS tagger (but ignore nouns)
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIAL;
				#ifdef GIA_DEBUG
				//cout << "foundVerb FEATURE_POS_TAG_VERB_VBPOTENTIAL" << endl;
				#endif

				string wordOrigLowerCase = convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
					applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP))
		{
			if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ))	//NB "ive" words will be marked as JJ/adjective or NN/noun by Stanford/Relex POS tagger (but ignore nouns)
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE;
				#ifdef GIA_DEBUG
				//cout << "foundVerb FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE" << endl;
				#endif

				string wordOrigLowerCase = convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
					applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#endif
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP))	//removed 2h2h: || (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP)
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ)	//NB "is ..." and "is ..ed" (not Stanford CoreNLP/Relex) verbs may be marked as JJ/adjective by Stanford/Relex POS tagger eg "It is open"/"He is tired."
			{
				#ifdef GIA_DEBUG
				//cout << "foundVerb FEATURE_POS_TAG_VERB_VBSTATE" << endl;
				#endif
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBSTATE;

				currentFeature->stanfordPOS = stanfordPOS;
				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
			}
		}
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP))
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)	//NB "ion"/"ment" words will be marked as NN/noun by Stanford/Relex POS tagger
			{
				#ifdef GIA_DEBUG
				//cout << "foundVerb FEATURE_POS_TAG_VERB_VBDESCRIPTION" << endl;
				#endif
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBDESCRIPTION;

				string wordOrigLowerCase = convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
					applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#endif
	#ifdef GIA2_CORRECT_POSTAGS_FIX1
	}
	#endif

	return updatedLemma;
}


/*
bool determineVerbCase(string* word)
{

	//detectContinuousVerbBasic Algorithm:
	//Note a simple "ing" appendition check is not possible as some nouns end in ing also eg "thing"]
	//Case 1. thinking
	//Case 2. changing - "chang" [change e] + "ing"
	//Case 3. running - "run" + "n" [run n] + "ing"

	//ongoing?
	//outstanding?
	//being?
	//becoming?


	bool foundVerbContinuousCase = false;

	if(word->length() > GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH)
	{
		continuousVerbFound = true;
		string ing = GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;
		int ingIndex = 0;
		for(int i = word->length()-GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH; i < word->length(); i++)
		{
			if((*word)[i] != ing[ingIndex])
			{
				foundVerbContinuousCase = true;
			}
		}
	}

	cout << "continuousVerbFound" << endl;

	return foundVerbContinuousCase;

}
bool determineVerbCase(string* word)
{
	bool foundVerbContinuousCase = false;
	int wordStringLength = word->length();
	#ifdef GIA_DEBUG
	//cout << "word = " <<* word << endl;
	//cout << "wordStringLength = " << wordStringLength << endl;
	#endif
	if(wordStringLength > GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND)
	{
		int wordTenseFormContinuousAppendLength = string(GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND).length();
		string lastThreeLettersOfWord = word->substr(wordStringLength-wordTenseFormContinuousAppendLength, wordTenseFormContinuousAppendLength);
		#ifdef GIA_DEBUG
		//cout << "wordTenseFormContinuousAppendLength = " << wordTenseFormContinuousAppendLength << endl;
		//cout << "lastThreeLettersOfWord = " << lastThreeLettersOfWord << endl;
		#endif
		if(lastThreeLettersOfWord == GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND)
		{
			foundVerbContinuousCase = true;
		}
	}
	return foundVerbContinuousCase;
}
*/



#endif


