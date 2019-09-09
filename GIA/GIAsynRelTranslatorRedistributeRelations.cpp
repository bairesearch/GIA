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
 * File Name: GIAsynRelTranslatorRedistributeRelations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2l 10-August-2019
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAsynRelTranslatorRedistributeRelations.hpp"
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
#endif



#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS

//NB Translator:fillGrammaticalArraysStanford{}:extractGrammaticalInformationStanford{}:extractPOSrelatedGrammaticalInformationStanford{}:extractGrammaticalInformationFromStanfordPOStag{} performs initial infinitive/imperative determination based on NLP tags and previous word "to" (and sets previousWordInSentenceIsTo for redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs{}:extractPOSrelatedGrammaticalInformationStanford{}:extractGrammaticalInformationFromStanfordPOStag{} to reperform infinitive/imperative determination in case Stanford parser/CoreNLP failed to tag the word correctly ie as VB);
void GIAsynRelTranslatorRedistributeRelationsClass::redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(GIAtranslatorVariablesClass* translatorVariables)
{
	//eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif

			int governorIndex = currentRelationInList->relationGovernorIndex;
			int dependentIndex = currentRelationInList->relationDependentIndex;
			GIAentityNode* governorEntity = (*translatorVariables->GIAentityNodeArray)[governorIndex];
			GIAentityNode* dependentEntity = (*translatorVariables->GIAentityNodeArray)[dependentIndex];

			#ifdef GIA_SEM_REL_TRANSLATOR_CORRECT_POSTAGS_FIX2
			if((*translatorVariables->featureArrayTemp)[governorIndex] != NULL)
			{
			#endif
				if(correctVerbPOStagAndLemma(governorEntity, (*translatorVariables->featureArrayTemp)[governorIndex]))
				{
					currentRelationInList->relationGovernor = governorEntity->entityName;
				}
			#ifdef GIA_SEM_REL_TRANSLATOR_CORRECT_POSTAGS_FIX2
			}
			if((*translatorVariables->featureArrayTemp)[dependentIndex] != NULL)
			{
			#endif
				if(correctVerbPOStagAndLemma(dependentEntity, (*translatorVariables->featureArrayTemp)[dependentIndex]))
				{
					currentRelationInList->relationDependent = dependentEntity->entityName;
				}
			#ifdef GIA_SEM_REL_TRANSLATOR_CORRECT_POSTAGS_FIX2
			}
			#endif

		//#ifdef GIA_SYN_REL_TRANSLATOR_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
}


//note this function tags all "continuous verbs" as VBG (even those which perhaps should be left as NNP because they appear at the beginning at the sentence eg "Swimming is good exercise.")
//note this function can perhaps only be strictly used in circumstances where the continuous verb appears at the end of the sentence eg GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION (because "-ing" cannot be used in itself to detect continuous verbs - as there are some which perhaps should be left as NNP when they appear at the beginning at the sentence eg "Swimming is good exercise.")
bool GIAsynRelTranslatorRedistributeRelationsClass::correctVerbPOStagAndLemma(GIAentityNode* actionOrSubstanceEntity, GIAfeature* currentFeature)
{
	bool updatedLemma = false;
	#ifdef GIA_SEM_REL_TRANSLATOR_CORRECT_POSTAGS_FIX1
	if(actionOrSubstanceEntity->wordOrig != "")		//required to ignore dynamically generated entities, e.g. "have"/"$qvar"/etc
	{
	#endif
		string baseNameFound = "";
		
		int grammaticalBaseTenseForm = INT_DEFAULT_VALUE;
		bool foundContinuousOrInfinitiveOrImperativeOrPotentialVerb = GIApreprocessorWordIdentification.determineVerbCaseAdditionalWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound, &grammaticalBaseTenseForm);
		
		//This section of code cannot be used as originally intended as some verb infinitives are also nouns (eg "yarn") - therefore must formally rely on correct infinitive tagging of verbs...
		if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true) || (actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE] == true)))
		{
			//infinitive/imperative verb already detected by NLP/extractGrammaticalInformationFromStanfordPOStag{}
		}
		else
		{
			if(currentFeature->stanfordPOS == FEATURE_POS_TAG_VERB_VB)
			{
				cout << "correctVerbPOStagAndLemma{}: expectation error; FEATURE_POS_TAG_VERB_VB already defined" << endl;
			}
			if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == INT_DEFAULT_VALUE))
			{
				//Should the NLP settings be overruled here - is a verb base form always an imperative or infinitive? Answer = NO; Therefore only perform upgrade for special infinitive cases
				if(currentFeature->previousWordInSentenceIsTo)
				{//only perform upgrade here for special infinitive cases... (eg "to verbbaseform"), as some verbbaseforms are also nouns (eg "tabled" and "table"). Also don't wish to overwrite VBP cases eg "I eat the..."
					string stanfordPOS = FEATURE_POS_TAG_VERB_VB;	//FUTURE GIA - consider using new non-standard pos tage FEATURE_POS_TAG_VERB_VBDESCRIPTION instead of reusing FEATURE_POS_TAG_VERB_VBs

					currentFeature->stanfordPOS = stanfordPOS;
					GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature - it should identify the verb as an infinitive/imperative based on previousWordInSentenceIsTo
					GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
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
			#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
			if(GIApreprocessorWordIdentification.determineIfWordIsIrregularVerbContinuousCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound))
			#elif defined GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
			#ifdef GIA_TRANSLATOR_BACKUP_OF_OLD_GRAMMAR_EXECUTION_WITHOUT_BUG_CORRECTION
			if(false)	//corrected @GIA3f1a - used to be (grammaticalTenseModifier == GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP) (meaning it was never true due to a bug in GIApreprocessorWordClass::convertVerbCaseGrammaticalTenseFormToTenseModifier that set GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE instead of GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP)
			#else
			if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS))
			#endif
			#endif
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBG;
				/*
				Wood is used for making milk.
				What is wood used in the making of?
				NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser (this example appears correct @stanford-parser-2013-04-05/stanford-corenlp-2013-04-04 + @stanford-parser-full-2014-01-04/stanford-corenlp-full-2014-01-04)
				*/
				string wordOrigLowerCase = SHAREDvars.convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = Swimming, and entityName = swimming; then apply the lemma correction
				{

					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;	//change irregular verb name eg making to base irregular verb base name eg make
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP_PROGRESSIVE_CASE
					currentFeature->stanfordPOS = stanfordPOS;
					GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
					GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP_PROGRESSIVE_CASE
				currentFeature->stanfordPOS = stanfordPOS;
				GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
				GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}

			/*
			//STANFORD_PARSER_USE_POS_TAGS is no longer supported by GIA_SYN_REL_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS/redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs{}...
			#ifdef STANFORD_PARSER_USE_POS_TAGS
			if(determineVerbCaseAdditional(&(actionOrSubstanceEntity->wordOrig)))	//OR &(currentRelationInList->relationGovernor)	//NB must use wordOrig as only wordOrig is guaranteed to still have "ing" attached - the word may be stripped by stanford corenlp in generation of the lemma
			{
				#ifdef GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_DEBUG
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
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL))
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ)	//NB "able" words will be marked as JJ/adjective or NN/noun by Stanford/Relex POS tagger (but ignore nouns)
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIAL;

				string wordOrigLowerCase = SHAREDvars.convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
					GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
				GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE))
		{
			if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ))	//NB "ive" words will be marked as JJ/adjective or NN/noun by Stanford/Relex POS tagger (but ignore nouns)
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE;

				string wordOrigLowerCase = SHAREDvars.convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
					GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
				GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#endif
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))	//removed 2h2h: || (grammaticalBaseTenseForm == INT_DEFAULT_VALUE)
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ)	//NB "is ..." and "is ..ed" (not Stanford CoreNLP/Relex) verbs may be marked as JJ/adjective by Stanford/Relex POS tagger eg "It is open"/"He is tired."
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBSTATE;

				currentFeature->stanfordPOS = stanfordPOS;
				GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
				GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
			}
		}
		#endif
		#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
		if(foundContinuousOrInfinitiveOrImperativeOrPotentialVerb && (grammaticalBaseTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION))
		{
			if(actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)	//NB "ion"/"ment" words will be marked as NN/noun by Stanford/Relex POS tagger
			{
				string stanfordPOS = FEATURE_POS_TAG_VERB_VBDESCRIPTION;

				string wordOrigLowerCase = SHAREDvars.convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
				if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = runnable, and entityName (NLP identified lemma) = runnable; then apply the lemma correction
				{
					updatedLemma = true;
					actionOrSubstanceEntity->entityName = baseNameFound;
					currentFeature->lemma = actionOrSubstanceEntity->entityName;

					#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
					currentFeature->stanfordPOS = stanfordPOS;
					GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
					GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
					#endif
				}
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP
				currentFeature->stanfordPOS = stanfordPOS;
				GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeature, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature
				GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity
				#endif
			}
		}
		#endif
	#ifdef GIA_SEM_REL_TRANSLATOR_CORRECT_POSTAGS_FIX1
	}
	#endif

	return updatedLemma;
}


#endif


