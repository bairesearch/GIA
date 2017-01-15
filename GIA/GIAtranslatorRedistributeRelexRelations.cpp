/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIAtranslatorRedistributeRelexRelations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2c 19-July-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIAtimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslatorRedistributeRelexRelations.h"
#include "GIAtranslatorOperations.h"
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#include "GIAlrp.h"
#endif
#include "GIAtranslatorDefineGrammar.h"



#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS

void redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllContinuousVerbs(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
	//eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif

			int governorIndex = currentRelationInList->relationGovernorIndex;
			int dependentIndex = currentRelationInList->relationDependentIndex;
			GIAentityNode * governorEntity = GIAentityNodeArray[governorIndex];
			GIAentityNode * dependentEntity = GIAentityNodeArray[dependentIndex];		
			
			
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			//cout << "governorEntity->entityName = " << governorEntity->entityName << endl;
			//cout << "dependentEntity->entityName = " << dependentEntity->entityName << endl;
			if(correctContinuousVerbPOStagAndLemma(governorEntity, featureArrayTemp[governorIndex]))
			{
				currentRelationInList->relationGovernor = governorEntity->entityName;
			}
			if(correctContinuousVerbPOStagAndLemma(dependentEntity, featureArrayTemp[dependentIndex]))
			{		
				currentRelationInList->relationDependent = dependentEntity->entityName;
			}
				
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}	
}


//note this function tags all "continuous verbs" as VBG (even those which perhaps should be left as NNP because they appear at the beginning at the sentence eg "Swimming is good exercise.") 
//note this function can perhaps only be strictly used in circumstances where the continuous verb appears at the end of the sentence eg GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION (because "-ing" cannot be used in itself to detect continuous verbs - as there are some which perhaps should be left as NNP when they appear at the beginning at the sentence eg "Swimming is good exercise.")
bool correctContinuousVerbPOStagAndLemma(GIAentityNode * actionOrSubstanceEntity, Feature * currentFeature)
{
	bool foundContinuousVerb = false;
	
	//cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
	
	//if(actionOrSubstanceEntity->stanfordPOStemp == FEATURE_POS_TAG_VBG)		//Only Stanford Compatible
	if((actionOrSubstanceEntity->wordNetPOS == GRAMMATICAL_WORD_TYPE_VERB) && (actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true))	//Relex compatible
	{
		/*
		Wood is used for delivering milk.
		NB delivering is correctly tagged as VBG by stanford CoreNLP (NN by Stanford Parser; and this incorrect value is recorded assuming STANFORD_PARSER_USE_POS_TAGS is set)
		*/
		//cout << "foundVerb1" << endl;
		foundContinuousVerb = true;
	}
	else 
	{
		//cout << "NB: GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS requires GIA_USE_LRP to be defined and -lrpfolder to be set" << endl;					
		string baseNameFound = "";
		//cout << "actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
		#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL	
		if(determineIfWordIsContinuousCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound))		
		#elif defined GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
		if(determineIfWordIsIrregularVerbContinuousCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound))
		#endif 
		{
			//cout << "foundVerb2a" << endl;
			/*
			Wood is used for making milk.
			What is wood used in the making of?
			NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser					
			*/	
			string wordOrigLowerCase = convertStringToLowerCase(&(actionOrSubstanceEntity->wordOrig));
			if(wordOrigLowerCase == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)	//eg if wordOrig = Swimming, and entityName = swimming; then apply the lemma correction 
			{
				//change irregular verb name eg making to base irregular verb base name eg make
				//cout << "2 actionOrSubstanceEntity->entityName = " << actionOrSubstanceEntity->entityName << endl;
				//cout << "2 baseNameFound = " << baseNameFound << endl;
				actionOrSubstanceEntity->entityName = baseNameFound;
								
				currentFeature->stanfordPOS = FEATURE_POS_TAG_VBG;
				currentFeature->lemma = actionOrSubstanceEntity->entityName;

				extractPOSrelatedGrammaticalInformationStanford(currentFeature);			//regenerate grammatical information for feature
				applyPOSrelatedGrammaticalInfoToEntity(actionOrSubstanceEntity, currentFeature);	//regenerate grammatical information for entity			
			}
			
			foundContinuousVerb = true;
		}

		/*
		//STANFORD_PARSER_USE_POS_TAGS is no longer supported by GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS/redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs()...
		#ifdef STANFORD_PARSER_USE_POS_TAGS			
		if(determineIfWordIsVerbContinuousCase(&(actionOrSubstanceEntity->wordOrig)))	//OR &(currentRelationInList->relationGovernor)	//NB must use wordOrig as only wordOrig is guaranteed to still have "ing" attached - the word may be stripped by stanford corenlp in generation of the lemma 
		{
			//cout << "foundVerb2b" << endl;
			//What is wood used in the delivering of?
			//						
			//	Wood is used for making milk.
			//	What is wood used in the making of?
			//	NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser	
			//	still fails because "making" remains recorded as the lemma not "make"
			//	a solution involves GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE/GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL													
			foundContinuousVerb = true;
		}
		#endif
		*/

	}
	return foundContinuousVerb;
}


/*
bool determineIfWordIsVerbContinuousCase(string * word)
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
	
	if(word->length() > GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH)
	{
		continuousVerbFound = true;
		string ing = GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND;	
		int ingIndex = 0;
		for(int i = word->length()-GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH; i < word->length(); i++)
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
bool determineIfWordIsVerbContinuousCase(string * word)
{
	bool foundVerbContinuousCase = false;
	int wordStringLength = word->length();
	//cout << "word = " << *word << endl;
	//cout << "wordStringLength = " << wordStringLength << endl;
	if(wordStringLength > GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND)
	{
		int wordTenseFormContinuousAppendLength = string(GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND).length();
		string lastThreeLettersOfWord = word->substr(wordStringLength-wordTenseFormContinuousAppendLength, wordTenseFormContinuousAppendLength);
		//cout << "wordTenseFormContinuousAppendLength = " << wordTenseFormContinuousAppendLength << endl;
		//cout << "lastThreeLettersOfWord = " << lastThreeLettersOfWord << endl;
		if(lastThreeLettersOfWord == GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND)
		{
			foundVerbContinuousCase = true;
		}
	}
	return foundVerbContinuousCase;
}
*/



#endif






#ifdef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
//required for aliasing to work
void redistributeRelexRelationsCollapseSubjectAndObjectGenerateAppos(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[])
{
	/*
	She is the one.		_subj(be[2], she[1]) + _obj(be[2], one[4]) -> appos(She-1, one-4)
	Bikes are machines. 	_subj(be[2], bike[1]) + _obj(be[2], machine[3]) -> appos(bike-1, machine-3)		 
	That is Jim. 		_subj(be[2], that[1]) + _obj(be[2], Jim[3]) -> appos(that-1, Jim-3)		
	The time is 06:45.	_subj(be[3], time[2]) + _obj(be[3], 06:45[4])  -> appos(time-2, 06:45-4)	
	...	
	Kane is late.  No change required (Relex parses this correctly)
		_predadj(Kane[1], late[3])
	She is the one.
		_subj(be[2], she[1]) + _obj(be[2], one[4]) -> appos(She-1, one-4)		 
	The girl is tall. No change required (Relex parses this correctly)
		_predadj(girl[2], tall[4])		
	Bikes are machines. 
		_obj(be[2], machine[3]) + _subj(be[2], bike[1])	-> appos(bike-1, machine-3)			
	The wheels are green. No change required (Relex parses this correctly)
		_predadj(wheel[2], green[4])		
	That is Jim. 	
		_subj(be[2], that[1]) + _obj(be[2], Jim[3]) +  -> appos(that-1, Jim-3)		  
	The time is 06:45.
		_subj(be[3], time[2]) + _obj(be[3], 06:45[4]) +  -> appos(time-2, 06:45-4)	
	*/
	
	/*Also deal with special case queries (required to support aliasing / consistency with Stanford parser)
	What is the time? _subj(be[2], _$qVar[1]) + _obj(be[2], time[4]) -> appos(time[4], $qVar[1])
	Who is that?	_subj(be[2], _$qVar[1]) + _obj(be[2], that[3]) -> appos(that[3], _$qVar[1])	
	What time is it?	_subj(be[3], time[2]) + _obj(be[3], it[4]) -> appos(time[2], $qVar[4])
		note query comparison node used to be identified via identifyComparisonVariableAlternateMethod()
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationTest[REL2][REL_ENT1] = true; param.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;

	#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
	if(currentSentenceInList->isQuestion)
	{
	#endif
		#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
		/*
		What is the time? _subj(be[2], _$qVar[1]) + _obj(be[2], time[4]) -> appos(time[4], $qVar[1])
		Who is that?	_subj(be[2], _$qVar[1]) + _obj(be[2], that[3]) -> appos(that[3], _$qVar[1])	
		*/		
		GIAgenericDepRelInterpretationParameters paramA = param;
		paramA.useRelationTest[REL1][REL_ENT2] = true; paramA.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
		paramA.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramA.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		paramA.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL2; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;	
		paramA.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;	
		paramA.disableRelation[REL1] = true;
		paramA.disableEntity[REL1][REL_ENT1] = true;	
		genericDependecyRelationInterpretation(&paramA, REL1);	
		#endif

		/*
		What time is it?	_subj(be[3], time[2]) + _obj(be[3], it[4]) -> appos(time[2], $qVar[3])		//NB appos has to use Be/$qVar[3] - it cannot use a) it/$qVar[4] for grammatical reasons [as "it" is tagged as a noun/definite], and cannot use b) What/$qVar[1] since it is not referenced by any relex dependency relation 
			note query comparison node used to be identified via identifyComparisonVariableAlternateMethod())
		*/
						
		bool whichOrWhatQueryFound = false;
		//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX]	//"what" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled		
		if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
		{
			for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
			{
				//cout << "featureQueryWordAcceptedByAlternateMethodNameArray[i] = " << featureQueryWordAcceptedByAlternateMethodNameArray[i] << endl;
				//cout << "featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma = " << featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma << endl;	crashes
				if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
				{
					//cout << "whichOrWhatQueryFound" << endl;
					whichOrWhatQueryFound = true;
				}
			}
		}
		if(whichOrWhatQueryFound)
		{											
			GIAgenericDepRelInterpretationParameters paramB = param;
			paramB.useRelationTest[REL2][REL_ENT2] = true; paramB.relationTest[REL2][REL_ENT2] = RELATION_ENTITY_IT;
			paramB.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
			paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;	
			//NB appos has to use Be/$qVar[3] - it cannot use a) it/$qVar[4] for grammatical reasons [as "it" is definite, assigned as a noun, etc], and cannot use b) What/$qVar[1] since it is not referenced by any relex dependency relation 
			paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT1;	
			paramB.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; paramB.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "be" -> "$qvar"
			paramB.disableRelation[REL1] = true;
			paramB.disableEntity[REL2][REL_ENT2] = true; paramB.disableEntityUseOriginalValues[REL2][REL_ENT2] = true;	//oldRedundantItEntity
			paramB.disableEntityUseOriginalValues[REL2][REL_ENT2] = true;
			genericDependecyRelationInterpretation(&paramB, REL1);	
		}
	#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG	
	}
	#endif

	/*
	She is the one.		_subj(be[2], she[1]) + _obj(be[2], one[4]) -> appos(She-1, one-4)
	Bikes are machines. 	_subj(be[2], bike[1]) + _obj(be[2], machine[3]) -> appos(bike-1, machine-3)		 
	That is Jim. 		_subj(be[2], that[1]) + _obj(be[2], Jim[3]) -> appos(that-1, Jim-3)		
	The time is 06:45.	_subj(be[3], time[2]) + _obj(be[3], 06:45[4])  -> appos(time-2, 06:45-4)
	*/	
	GIAgenericDepRelInterpretationParameters paramC = param;
	paramC.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramC.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramC.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramC.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramC.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;	
	paramC.disableRelation[REL1] = true;
	paramC.disableEntity[REL1][REL_ENT1] = true;	//oldRedundantBeEntity
	genericDependecyRelationInterpretation(&paramC, REL1);
#else	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
						{
							if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
							{//found a matching preposition of object-subject relationship
								
								bool foundSpecialCaseQuery = false;
								
								#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG								
								if(currentSentenceInList->isQuestion)
								{
								#endif
									//deal with special case relex queries:
									#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
									if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
									{		
										/*
										What is the time? _subj(be[2], _$qVar[1]) + _obj(be[2], time[4]) -> appos(time[4], $qVar[1])
										Who is that?	_subj(be[2], _$qVar[1]) + _obj(be[2], that[3]) -> appos(that[3], _$qVar[1])	
										*/	

										currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList2->relationGovernor = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;

										currentRelationInList->disabled =  true;
										currentRelationInList->relationType = "dummyRelationCollapseSubjectAndObjectGenerateAppos";	//required to prevent use by GIA (when !GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD)

										foundSpecialCaseQuery = true;		
								
										GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
										disableEntity(oldRedundantBeEntity);										
									}
									else
									#endif
									if(currentRelationInList2->relationDependent == RELATION_ENTITY_IT)
									{	
										bool whichOrWhatQueryFound = false;
										if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
										{
											for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
											{
												if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
												{
													whichOrWhatQueryFound = true;
												}
											}
										}
										if(whichOrWhatQueryFound)
										{																													
											/*
											What time is it?	_subj(be[3], time[2]) + _obj(be[3], it[4]) -> appos(time[2], $qVar[3])		//NB appos has to use Be/$qVar[3] - it cannot use a) it/$qVar[4] for grammatical reasons [as "it" is tagged as a noun/definite], and cannot use b) What/$qVar[1] since it is not referenced by any relex dependency relation 
												note query comparison node used to be identified via identifyComparisonVariableAlternateMethod())
											*/
											
											GIAentityNode * oldRedundantItEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
											disableEntity(oldRedundantItEntity);
																							
											currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
											currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList2->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
											
											
											//NB appos has to use Be/$qVar[3] - it cannot use a) it/$qVar[4] for grammatical reasons [as "it" is definite, assigned as a noun, etc], and cannot use b) What/$qVar[1] since it is not referenced by any relex dependency relation 
											/*
											currentRelationInList2->relationDependentIndex = currentRelationInList2->relationDependentIndex;
											currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;      //convert "it" -> "$qvar" 
											featureArrayTemp[currentRelationInList2->relationDependentIndex]->grammaticalIsDefinite = false;
											featureArrayTemp[currentRelationInList2->relationDependentIndex]->grammaticalWordType = GRAMMATICAL_WORD_TYPE_UNDEFINED;	//"$qvar" cannot be a noun else it will be treated as a definite
											*/
											currentRelationInList2->relationDependentIndex = currentRelationInList->relationGovernorIndex;
											currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "be" -> "$qvar"										      
							
											GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
															
											currentRelationInList->disabled =  true;
											currentRelationInList->relationType = "dummyRelationCollapseSubjectAndObjectGenerateAppos";	//required to prevent use by GIA (when !GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD)

											featureArrayTemp[currentRelationInList2->relationGovernorIndex]->grammaticalIsDefinite = true;	//required such that "time" is treated the same way as when generated by "what is the time?" 
										
											foundSpecialCaseQuery = true;	
																				
										}							
									}
								#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG	
								}
								#endif
								
								if(!foundSpecialCaseQuery)
								{
									/*
									She is the one.		_subj(be[2], she[1]) + _obj(be[2], one[4]) -> appos(She-1, one-4)
									Bikes are machines. 	_subj(be[2], bike[1]) + _obj(be[2], machine[3]) -> appos(bike-1, machine-3)		 
									That is Jim. 		_subj(be[2], that[1]) + _obj(be[2], Jim[3]) -> appos(that-1, Jim-3)		
									The time is 06:45.	_subj(be[3], time[2]) + _obj(be[3], 06:45[4])  -> appos(time-2, 06:45-4)
									*/								
								
									currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
									currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
									currentRelationInList2->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;

									currentRelationInList->disabled =  true;
									currentRelationInList->relationType = "dummyRelationCollapseSubjectAndObjectGenerateAppos";	//required to prevent use by GIA (when !GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD)
								
									GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
									disableEntity(oldRedundantBeEntity);								
								}								
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif	
}
#endif
#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
//required for aliasing to work
void redistributeRelexRelationsDetectNameQueries(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
	bool firstWordOfSentenceIsWho = false;
	
	/*OLD:
	if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX])
	{
		if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
		{
			cout << "found who" << endl;
			firstWordOfSentenceIsWho = true;
		}
	}
	*/
	if(GIAentityNodeArrayFilled[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX])
	{
		if(featureArrayTemp[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
		{
			//cout << "found who" << endl;
			firstWordOfSentenceIsWho = true;
		}	
	}
	if(firstWordOfSentenceIsWho)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAentityNodeArrayFilled[i])
			{
				if(GIAentityNodeArray[i]->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{
					//cout << "found who query variable" << endl;			
					GIAentityNodeArray[i]->isNameQuery = true;
				}
			}
		}
	}
}
#endif

#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
//Added 28 October 2012b
void redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg1 Yarn is used in the making of cloth.	of(making[6], cloth[8]) + in(use[3], making[6])  -> _obj(making[6], _cloth[8])
	//eg2 What is yarn used in the making of?   interpret  of(making[7], of[8]) + _obj(of[8], _$qVar[1])  -> _obj(making[7], _$qVar[1])
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PREPOSITION_OF;
	param.relationTestSpecialCaseContinousVerb[REL1][REL_ENT1] = true;
		
	//eg Yarn is used in the making of cloth.	of(making[6], cloth[8]) + in(use[3], making[6]) -> _obj(making[6], _cloth[8])
	GIAgenericDepRelInterpretationParameters paramA = param;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_OF; paramA.relationTestIsNegative[REL2][REL_ENT3] = true;
	paramA.useRelationIndexTest[REL2][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	paramA.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramA.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	genericDependecyRelationInterpretation(&paramA, REL1);
		
	//eg What is wood used in the delivering of?   interpret  of(making[7], of[8]) + _obj(of[8], _$qVar[1])  -> _obj(making[7], _$qVar[1])
	GIAgenericDepRelInterpretationParameters paramB = param;
	paramB.useRelationTest[REL2][REL_ENT3] = true; paramB.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRelationTest[REL1][REL_ENT2] = true; paramB.relationTest[REL1][REL_ENT2] = RELATION_TYPE_PREPOSITION_OF;
	paramB.useRelationTest[REL2][REL_ENT1] = true; paramB.relationTest[REL2][REL_ENT1] = RELATION_TYPE_PREPOSITION_OF;
	paramB.useRelationTest[REL2][REL_ENT2] = true; paramB.relationTest[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
	paramB.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT1;	
	paramB.disableRelation[REL1] = true;
	paramB.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramB, REL1);
	
	//eg What is wood used in the delivering of?   interpret  _obj(making[6], _cloth[8]) + _obj(of[8], _$qVar[1])  -> _obj(making[7], _$qVar[1])
	GIAgenericDepRelInterpretationParameters paramC = paramB;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;	//assuming it has been overwritten with object via execution of paramA
	genericDependecyRelationInterpretation(&paramC, REL1);
		
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif
			bool prepositionFound = false;
			if(convertPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_RELEX, &prepositionFound) == RELATION_TYPE_PREPOSITION_OF)
			{				
				int continuousVerbIndex = currentRelationInList->relationGovernorIndex;
				GIAentityNode * continuousVerbEntity = GIAentityNodeArray[continuousVerbIndex];
				if((continuousVerbEntity->wordNetPOS == GRAMMATICAL_WORD_TYPE_VERB) && (continuousVerbEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true))
				{					
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
						{
						//#endif
							bool prepositionFound2 = false;
							if(convertPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_RELEX, &prepositionFound2) != RELATION_TYPE_PREPOSITION_OF)
							{
								if(currentRelationInList2->relationDependent == currentRelationInList->relationGovernor)
								{
									//eg Yarn is used in the making of cloth.	of(making[6], cloth[8]) + in(use[3], making[6]) -> _obj(making[6], _cloth[8])
									currentRelationInList->relationType = RELATION_TYPE_OBJECT;
									//cout << "1" << endl;
								}
							}
							
							//cout << "astg1" << endl;
							if(currentRelationInList2->relationType == RELATION_TYPE_OBJECT)
							{
								//cout << "astg2" << endl;
								//cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
								//cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
								
								if((currentRelationInList->relationDependent == RELATION_TYPE_PREPOSITION_OF) && (currentRelationInList2->relationGovernor == RELATION_TYPE_PREPOSITION_OF))
								{
									//cout << "astg3" << endl;
									if(currentRelationInList2->relationDependent == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
									{
										//eg What is wood used in the delivering of?   interpret  of(delivering[7], of[8]) + _obj(of[8], _$qVar[1])  -> _obj(making[7], _$qVar[1])
										//cout << "2" << endl;

										currentRelationInList2->relationType = RELATION_TYPE_OBJECT;				
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationGovernorIndex;
										currentRelationInList2->relationGovernor = currentRelationInList->relationGovernor;
										
										currentRelationInList->disabled =  true;
										currentRelationInList->relationType = "dummyRelationRedistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs";	//required to prevent use by GIA (when !GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD)
								
										GIAentityNode * oldRedundantOfEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
										disableEntity(oldRedundantOfEntity);								
									}
								}
							}

						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						//#endif

						currentRelationInList2 = currentRelationInList2->next;
					}
				}					
			}

		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif	
}
#endif

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
void redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]) 
{
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
	/*eg1;  Space is saved by having a chicken.	_subj(have[5], by[4]) + _obj(have[5], chicken[7]) + _advmod(save[3], by[4]) -> dobj(have[5], chicken[7]) + prep_by(save[3], have[5])	{required for interpretation by linkHavingPropertyConditionsAndBeingDefinitionConditions}
	_subj(have[5], by[4])
	_obj(have[5], chicken[7])
	_advmod(save[3], by[4])
	_obj(save[3], space[1]) 	[IRRELEVANT]
	*/
	/*eg2;  Space is saved by being a chicken.	_subj(be[5], by[4]) + _obj(be[5], chicken[7] + _advmod(save[3], by[4]) -> dobj(be[5], chicken[7]) + prep_by(save[3], be[5])	{required for interpretation by linkHavingPropertyConditionsAndBeingDefinitionConditions} 
	_subj(be[5], by[4])
	_obj(be[5], chicken[7])
	_advmod(save[3], by[4])
	_obj(save[3], space[1]) 	[IRRELEVANT]
	*/
	/*eg3;  Space is saved by moving a chicken.	_subj(move[5], by[4]) + _obj(move[5], chicken[7]) + _advmod(save[3], by[4]) -> -> dobj(move[5], chicken[7]) + prep_by(save[3], move[5])
	_subj(move[5], by[4])
	_obj(move[5], chicken[7])
	_advmod(save[3], by[4])
	_obj(save[3], space[1])		[IRRELEVANT]
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.useRelationIndexTest[REL1][REL_ENT2] = true; param.relationIndexTestRelationID[REL1][REL_ENT2] = REL3; param.relationIndexTestEntityID[REL1][REL_ENT2] = REL_ENT2;
	param.disableRelation[REL1] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT2] = REL_ENT1;	
	param.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT3] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT3] = REL_ENT2;	
	genericDependecyRelationInterpretation(&param, 1);
	#else
	cout << "!GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION" << endl;
	#endif
}
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
void redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationAllWithADefinitionFunctionAsPropertyLinks(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]) 
{
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO
	/*eg; The chicken is 3 minutes late.	_subj(be[3], chicken[2]) + _obj(be[3], minutes[5]) + _advmod(be[3], late[6]) -> _advmod(late[6], minutes[5]) + _advmod(chicken[2],  late[6])
	_subj(be[3], chicken[2])
	_obj(be[3], minutes[5])
	_advmod(be[3], late[6])
	_quantity(minutes[5], 3[4]) [IRRELEVANT]
	*/
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL3; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationTest[REL2][REL_ENT1] = true; param.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;	//redundant test
	param.useRelationTest[REL3][REL_ENT1] = true; param.relationTest[REL3][REL_ENT1] = RELATION_ENTITY_BE;	//redundant test
	param.disableRelation[REL1] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL3; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;	
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT1] = REL_ENT2;	
	genericDependecyRelationInterpretation(&param, 1);
	#else
	cout << "GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION" << endl;
	#endif
}
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
void redistributeRelexRelationsDisregardRedundantDefinitionRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])  
{
	/*
	Eg;	What are the patent claims on?	_subj(be[2], claim[5]) + _obj(on[6], _$qVar[1]) -> on(claim[5], _$qVar[1]) 
	_subj(be[2], claim[5])
	_obj(on[6], _$qVar[1])
	_advmod(be[2], on[6])	+ on(be[2], on[6]) [DUPLICATE INFO: IGNORE]
	*/

	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL3; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL3; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.disableRelation[REL1] = true;
	param.disableRelation[REL3] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;	
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL3; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2;	
	genericDependecyRelationInterpretation(&param, 1);
	
	#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
	cout << "!GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION" << endl;
	#endif

}
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
void redistributeRelexRelationsAdverbPlusSubjectRelationAsActionCondition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]) 
{

	/*eg 1 Space is saved by running fast.	_subj(run[5], by[4]) + _advmod(save[3], by[4]) -> by(save[3], run[5])
	_subj(run[5], by[4])
	_advmod(save[3], by[4])
	_obj(save[3], space[1]) [NOT MODIFIED]
	_advmod(run[5], fast[6]) [NOT MODIFIED + IRRELEVANT]
	*/

	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	paramA.numberOfRelations = 3;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	paramA.useRelationTest[REL3][REL_ENT3] = true; paramA.relationTest[REL3][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramA.useRelationIndexTest[REL1][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL1][REL_ENT2] = REL2; paramA.relationIndexTestEntityID[REL1][REL_ENT2] = REL_ENT2;
	paramA.useRelationIndexTest[REL2][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT1] = REL3; paramA.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramA.disableRelation[REL1] = true;
	paramA.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT1;	
	paramA.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL2; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2; paramA.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL2][REL_ENT3] = true;	
	genericDependecyRelationInterpretation(&paramA, 1);

	/*eg 2 What is the Co-cart designed for?	_obj(for[6], _$qVar[1]) + _advmod(design[5], for[6]) -> for(design[5], _$qVar[1])
	_obj(for[6], _$qVar[1])
	_advmod(design[5], for[6])
	_obj(design[5], Co[4]) [NOT MODIFIED]
	for(design[5], _$qVar[1]) [NOT MODIFIED + IRRELEVANT]
	*/

	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);	
	paramB.numberOfRelations = 3;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRelationTest[REL2][REL_ENT3] = true; paramB.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	paramB.useRelationTest[REL3][REL_ENT3] = true; paramB.relationTest[REL3][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRelationIndexTest[REL1][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; paramB.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT2;
	paramB.useRelationIndexTest[REL2][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL2][REL_ENT1] = REL3; paramB.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramB.disableRelation[REL1] = true;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;	
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL2; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2; paramB.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL2][REL_ENT3] = true;	
	genericDependecyRelationInterpretation(&paramB, 1);
	
}
#endif

#endif




