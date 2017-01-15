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
 * File Name: GIAtranslatorRedistributeRelationsStanford.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k2a 10-July-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorRedistributeRelationsStanford.h"
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#include "GIAlrp.h"
#endif
#include "GIAtranslatorDefineGrammar.h"


#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void redistributeStanfordRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser, GIAfeature* featureArrayTemp[])
{
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout <<"pass 1c2; disable redundant nodes Stanford Core NLP/Parser" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
		#endif
		disableRedundantNodesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	}
	#endif
	disableRedundantNodesStanfordParser(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c3; redistribute Stanford Relations Create Query Vars Adjust for Action Preposition Action" << endl;
	#endif
	//redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction() needs to be called with LRP also - separated out from redistributeStanfordRelationsMultiwordPreposition() 24 July 2013 to achieve this
	redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);	//added 28 October 2012

	#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
	if(!(getUseLRP()))
	{
	#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c4; redistribute Stanford Relations Multiword Preposition" << endl;
		#endif
		redistributeStanfordRelationsMultiwordPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
	}
	#endif

	//added 8 August 2012
	#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c5a; redistribute Relations - prep_of (eg The ball of the red dog is green..   nsubj(green-8, ball-2) / prep_of(ball-2, dog-6) ->  nsubj(green-7, ball-5) / poss(ball-5, dog-3)" << endl;
	#endif
	redistributeStanfordRelationsInterpretOfAsPossessive(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
	#ifndef GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT
	if(currentSentenceInList->isQuestion)
	{
	#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c5b; redistribute Relations - what is the name/number of? 	nsubj(is-2, name-4) / attr(is-2, What-1) {/ det(name-4, the-3)} / poss/prep_of(name-4, dog-8) -> appos(That-1, _$qVar[1])" << endl;
		#endif
		redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#ifndef GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT
	}
	#endif
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c5c; redistribute Relations - intepret name of as definition (eg interpret 'The red dog's name is Max.' / 'The name of the red dog is Max.'	nsubj(Max-7, name-5) / poss/prep_of(name-5, dog-3) -> appos(dog-3, Max-7)" << endl;
	#endif
	redistributeStanfordRelationsInterpretNameOfAsDefinition(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c6; redistribute Stanford Relations - Collapse Advmod GIArelation Function Be (eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6)   +    Kane is late.	nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3)          +    She is the one     nsubj(one-4, She-1) /cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4)" << endl;
	//[OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - >nsubj(away-6, rabbit-2)] )" << endl;
	#endif
	redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c7; redistribute Stanford Relations - Generate Adjectives And Appos" << endl;
	cout << "eg1 Kane is late	      nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3)			      [NB non-determinate of governer and dependent of subject relation; take as indicator of substance]" << endl;
	cout << "eg2 She is the one	      nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4) 	      [NB determinate of dependent of subject relation; take as an indicator of definition]" << endl;
	cout << "eg3 The girl is tall	      nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1) -> _predadj(girl-2, tall-6)      [NB non-plural and determinate of governer of subject relation; take as indicator of substance]" << endl;
	cout << "eg4 bikes are machines        nsubj(machines-3, bikes-1) / cop(machines-3, are-2) -> appos(bikes-1, machines-3) 	      [NB plural and non-determinate of governer of subject relation; take as an indicator of definition]" << endl;
	cout << "eg5 the wheels are green      nsubj(green-6, wheels-4) / cop(green-6, are-5) -> _predadj(wheels-4, green-6)		      [NB plural and determinate of governer of subject relation; take as indicator of substance]" << endl;
	cout << "eg6 That is Jim.	      nsubj(Jim-3, That-1) / cop(Jim-3, is-2) -> appos(That-1, Jim-3)" << endl;
	cout << "eg7 The time is 06:45	      nsubj(06:45-4, time-2) / cop(06:45-4, is-3) / det(time-2, The-1) -> appos(time-2, 06:45-4)" << endl;
	#endif
	redistributeStanfordRelationsCollapseSubjectAndCopGenerateAdjectivesAndAppos(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c8; redistribute Stanford Relations - Adverbal Clause Modifier And Complement (eg The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as))" << endl;
	#endif
	redistributeStanfordRelationsAdverbalClauseModifierAndComplement(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifndef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE1
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c9; redistribute Stanford Relations - Clausal Subject (eg What she said makes sense. 	csubj (make, say)/dobj ( said-3 , What-1 ))" << endl;
	#endif
	redistributeStanfordRelationsClausalSubject(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	/*OLD:
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c10; redistribute Stanford Relations - Phrasal Verb Particle (eg They shut down the station.    prt(shut, down))" << endl;
	#endif
	redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	*/

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c10; redistribute Stanford Relations - Conjunction And Coordinate (eg I eat a pie or tom rows the boat. 	cc(pie-4, or-5) / conj(pie-4, tom-6))" << endl;
	#endif
	redistributeStanfordRelationsConjunctionAndCoordinate(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c11; redistribute Stanford Relations - Generate Unparsed Quantity Modifers (eg The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5)) / num($-5, 1000-6)" << endl;
	#endif
	redistributeStanfordRelationsGenerateUnparsedQuantityModifers(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c12; redistribute Stanford Relations - Generate Measures (eg years old - npadvmod(old, years) -> _measure_time(old[7], years[6]))" << endl;
	#endif
	redistributeStanfordRelationsGenerateMeasures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c13; redistribute Stanford Relations - Prt And Tmods (eg The disaster happened over night.   prt(happened-3, over-4) / tmod(happened-3, night-5) -> over(happened-3, night-5) )" << endl;
	#endif
	redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifndef GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT
	if(currentSentenceInList->isQuestion)
	{
	#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c14; redistribute Stanford Relations - Create Query Vars (eg interpret 'who is that' / 'what is the time.'  attr(is-2, Who-1) / attr(is-2, What-1) | interpret 'how much'/'how many' | interpret 'which' det(house-2, Which-1) | interpret how/when/where/why advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) / advmod(fall-5, Why-1)	 )" << endl;
		#endif
		redistributeStanfordRelationsCreateQueryVars(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#ifndef GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c15; redistribute Stanford Relations - partmod (eg Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle) )" << endl;
	#endif
	redistributeStanfordRelationsPartmod(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
	//Added 28 October 2012b
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c16; redistribute Stanford Relations - Interpret Of As Object For ContinuousVerb (eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )" << endl;
	#endif
	redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
	//Added 13 November 2012
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c17; redistribute Stanford Relations - expletives (eg 'There is a place that we go'   _expl(be[2], there[1]) + _subj(be[2], place[4]) + _subj(go[7], we[6]) + _obj(be[2], go[7]) -> _subj(go[7], we[6]) + _obj(go[7], place[4])  )" << endl;
	#endif
	redistributeStanfordRelationsExpletives(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP
	//Added 13 November 2012
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c18; redistribute Stanford Relations - dependency prepositions (eg 'Given the nature of the bank, write the letter.'  prep(write-8, Given-1) / dep(Given-1, nature-3) -> prep_given(write-8, nature-3) )" << endl;
	#endif
	redistributeStanfordRelationsDependencyPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "1c19 pass; redistributeStanfordRelationsAuxHave (eg Red dogs have flies.	aux(flies-4, have-3) + nsubj(flies-4, dogs-2) -> obj(have-3, flies-4) + nsubj(have-3, dogs-2)" << endl;	//updated 2f12a 13-July-2014
	#endif
	redistributeStanfordRelationsAuxHave(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif

	#ifdef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
	//added 12 July 2013
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c20; redistribute Stanford Relations - disable Aux And Cop Relations" << endl;
	#endif
	redistributeStanfordRelationsDisableAuxAndCop(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//this has been shifted before linkHavingPropertyConditionsAndBeingDefinitionConditions - 1t2l
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "1c21 pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
	#endif
	collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);
	#endif
	#endif

	//NLC Only (see GIArules.xml)
	/*
	#ifdef GIA_REMOVE_REDUNDANT_LOGICAL_CONDITION_ENTITIES
	redistributeStanfordRelationsDisableRedundantLogicalConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
	#endif
	*/

}


#ifdef GIA_USE_STANFORD_CORENLP
void disableRedundantNodesStanfordCoreNLP(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eliminate all redundant date relations eg num(December-4, 3rd-5)/num(December-4, 1990-7)/nn(3rd-5, December-4)/appos(3rd-5, 1990-7), where both the governer and the dependent have NER tag set to DATE
	/*Also: Ms. Savata's hand slipped.
          <dep type="nn">
            <governor idx="2">Savata</governor>
            <dependent idx="1">Ms.</dependent>
          </dep>
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeConjugationNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; param.relationArrayTestIsNegative[REL1][REL_ENT3] = true;

	GIAgenericDepRelInterpretationParameters paramA1 = param;
	GIAentityCharacteristic entityCharacteristicsTestA("NERTemp", FEATURE_NER_UNDEFINED_STRING, true);
	paramA1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTestA);
	paramA1.useSpecialCaseCharacteristicsRelationIndexTest[REL1][REL_ENT1] = true; paramA1.specialCaseCharacteristicsRelationIndexTestRelationID[REL1][REL_ENT1] = REL1; paramA1.specialCaseCharacteristicsRelationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT2; paramA1.specialCaseCharacteristicsRelationIndexTest[REL1][REL_ENT1].name = "NERTemp";
	GIAentityCharacteristic entityCharacteristicsTest1i1("NERTemp", FEATURE_NER_PERSON_STRING);
	GIAentityCharacteristic entityCharacteristicsTest1i2("NERTemp", FEATURE_NER_LOCATION_STRING);
	GIAentityCharacteristic entityCharacteristicsTest1i3("NERTemp", FEATURE_NER_ORGANIZATION_STRING);
	GIAentityCharacteristic entityCharacteristicsTest1i4("NERTemp", FEATURE_NER_MISC_STRING);
	paramA1.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i1);
	paramA1.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i2);
	paramA1.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i3);
	paramA1.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i4);
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP
	GIAentityCharacteristic entityCharacteristicsTest1ii1("stanfordPOStemp", FEATURE_POS_TAG_NOUN_NNP);
	paramA1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1ii1);
	paramA1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1ii1);
	#endif
	paramA1.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT1] = true;
		paramA1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][0] = REL1; paramA1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][0] = REL_ENT2;
		paramA1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][1] = REL1; paramA1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][1] = REL_ENT1;
	paramA1.disableRelation[REL1] = true;
	paramA1.disableEntity[REL1][REL_ENT2] = true;
	if(genericDependecyRelationInterpretation(&paramA1, REL1))
	{
		//cout << "A1" << endl;
	}

	#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
	GIAgenericDepRelInterpretationParameters paramB = param;


	GIAgenericDepRelInterpretationParameters paramB1 = param;
	GIAentityCharacteristic entityCharacteristicsTestB("NERTemp", FEATURE_NER_PERSON_STRING);
	paramB1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTestB);
	paramB1.useRelationTest[REL1][REL_ENT3] = true; paramB1.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PRENOMIAL_MODIFIER;
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP
	paramB1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1ii1);
	paramB1.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1ii1);
	#endif
	paramB1.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT1] = true;
		paramB1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][0] = REL1; paramB1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][0] = REL_ENT2;
		paramB1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][1] = REL1; paramB1.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][1] = REL_ENT1;
	paramB1.disableRelation[REL1] = true;
	paramB1.disableEntity[REL1][REL_ENT2] = true;
	if(genericDependecyRelationInterpretation(&paramB1, REL1))
	{
		//cout << "B1" << endl;
	}
	#endif

	GIAgenericDepRelInterpretationParameters paramA2 = param;
	paramA2.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i1);
	paramA2.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i2);
	paramA2.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i3);
	paramA2.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1i4);
	paramA2.useSpecialCaseCharacteristicsRelationIndexTest[REL1][REL_ENT1] = true; paramA2.specialCaseCharacteristicsRelationIndexTestRelationID[REL1][REL_ENT1] = REL1; paramA2.specialCaseCharacteristicsRelationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT2; paramA2.specialCaseCharacteristicsRelationIndexTest[REL1][REL_ENT1].name = "NERTemp";
	paramA2.disableEntity[REL1][REL_ENT2] = true;
	if(genericDependecyRelationInterpretation(&paramA2, REL1))
	{
		//cout << "A2" << endl;
	}

	#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
	GIAgenericDepRelInterpretationParameters paramB2 = param;
	paramB2.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTestB);
	paramB2.useRelationTest[REL1][REL_ENT3] = true; paramB2.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PRENOMIAL_MODIFIER;
	paramB2.disableEntity[REL1][REL_ENT2] = true;
	if(genericDependecyRelationInterpretation(&paramB2, REL1))
	{
		//cout << "B2" << endl;
	}
	#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool relationTypeConjuction = false;
			for(int i=0; i<RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeConjugationNameArray[i])
				{
					relationTypeConjuction = true;
				}
			}
			if(!relationTypeConjuction)
			{//added 25 July 2013

				GIAentityNode* governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

				bool governerAndDependentBothHaveSameNERvalue = false;
				for(int i=0; i<FEATURE_NER_EXPLICIT_NUMBER_OF_TYPES; i++)
				{
					if((governerEntity->NERTemp == featureNERexplicitTypeArray[i]) && (dependentEntity->NERTemp == featureNERexplicitTypeArray[i]))
					{
						governerAndDependentBothHaveSameNERvalue = true;
					}
				}

				bool governerIsPersonAndRelationTypeIsPrenominalModifier = false;
				#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
				//the assignment of NER values implementation appears to have changed in Stanford Core NLP
				if((governerEntity->NERTemp == FEATURE_NER_PERSON) && (currentRelationInList->relationType == RELATION_TYPE_PRENOMIAL_MODIFIER))
				{
					governerIsPersonAndRelationTypeIsPrenominalModifier = true;
				}
				#endif

				/*
				cout << "governerEntity->entityName = " << governerEntity->entityName << endl;
				cout << "dependentEntity->entityName = " << dependentEntity->entityName << endl;
				cout << "governerEntity->NERTemp = " << governerEntity->NERTemp << endl;
				cout << "dependentEntity->NERTemp = " << dependentEntity->NERTemp << endl;
				*/

				//if(((governerEntity->NERTemp == FEATURE_NER_DATE) && (dependentEntity->NERTemp == FEATURE_NER_DATE)) || ((governerEntity->NERTemp == FEATURE_NER_MONEY) && (dependentEntity->NERTemp == FEATURE_NER_MONEY)) || ((governerEntity->NERTemp == FEATURE_NER_NUMBER) && (dependentEntity->NERTemp == FEATURE_NER_NUMBER)) || ((governerEntity->NERTemp == FEATURE_NER_TIME) && (dependentEntity->NERTemp == FEATURE_NER_TIME)))
				if(governerAndDependentBothHaveSameNERvalue || governerIsPersonAndRelationTypeIsPrenominalModifier)
				{
					//cout << "governerEntity->entityName = " << governerEntity->entityName << endl;
					//cout << "dependentEntity->entityName = " << dependentEntity->entityName << endl;

					bool featureNERindicatesNameConcatenationRequired = false;
					for(int i=0; i<FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_OF_TYPES; i++)
					{
						if(governerEntity->NERTemp == featureNERindicatesNameConcatenationRequiredTypeArray[i])
						{
							featureNERindicatesNameConcatenationRequired = true;
						}
					}

					//if((governerEntity->NERTemp == FEATURE_NER_PERSON) || (governerEntity->NERTemp == FEATURE_NER_LOCATION) || (governerEntity->NERTemp == FEATURE_NER_ORGANIZATION) || (governerEntity->NERTemp == FEATURE_NER_MISC))
					if(featureNERindicatesNameConcatenationRequired)
					{
						bool featureNERindicatesNameConcatenationRequiredAllowedByPOS = false;
						#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP
						if((dependentEntity->stanfordPOStemp == FEATURE_POS_TAG_NOUN_NNP) && (governerEntity->stanfordPOStemp == FEATURE_POS_TAG_NOUN_NNP))
						{
							featureNERindicatesNameConcatenationRequiredAllowedByPOS = true;
						}
						#else
						featureNERindicatesNameConcatenationRequiredAllowedByPOS = true;
						#endif

						if(featureNERindicatesNameConcatenationRequiredAllowedByPOS)
						{
							governerEntity->entityName = dependentEntity->entityName + FEATURE_NER_NAME_CONCATENATION_TOKEN + governerEntity->entityName;	//join names together
							/*//OLD: before moving disableRedundantNodesStanfordCoreNLP() forward in execution heirachy (GIATranslatorDefineGrammarAndReferencing.cpp)
							if(governerEntity->hasAssociatedInstanceTemp)
							{//disable its substance also
								(governerEntity->associatedInstanceNodeList.back())->entityName = governerEntity->entityName;	//join names together
							}
							*/

							#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
							//cout << "governerEntity->NERTemp = " << governerEntity->NERTemp << endl;
							//cout << "dependentEntity->NERTemp = " << dependentEntity->NERTemp << endl;
							#endif

							currentRelationInList->disabled = true;

							disableEntity(dependentEntity);
						}
					}
					else
					{
						#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
						//cout << "governerEntity->NERTemp = " << governerEntity->NERTemp << endl;
						//cout << "dependentEntity->NERTemp = " << dependentEntity->NERTemp << endl;
						#endif

						//currentRelationInList->disabled = true;	//removed 14 July 2013 for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK (leave quantity dependency relations active) [although unncessary since quantity link generalisation remains unimplemented]

						disableEntity(dependentEntity);

					}

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

void disableRedundantNodesStanfordParser(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	ONLY USED FOR OLD VERSION OF STANFORD PARSER/CORENLP? note the following example no longer generates a complm dependency relation...
	eliminate redundant complementizer 'that' nodes eg 'The authors believe that their method can identify the time at which a subject becomes aware of her own movement.' / _complm(identify[8], that[4])
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLEMENTIZER;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_DEPENDENT_THAT;
	param.disableRelation[REL1] = true;
	param.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLEMENTIZER)
			{
				if(currentRelationInList->relationDependent == RELATION_DEPENDENT_THAT)
				{
					GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

					currentRelationInList->disabled = true;
					disableEntity(dependentEntity);

				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

		//general parameters
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.useRelationIndexTest[REL1][REL_ENT3] = true; param.relationIndexTestRelationID[REL1][REL_ENT3] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT3] = REL_ENT3; param.relationIndexTestIsNegative[REL1][REL_ENT3] = true;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.useRelationArrayTest[REL2][REL_ENT2] = true; param.relationArrayTest[REL2][REL_ENT2] = featureQueryActionPrepositionActionEquivalentQueryVariableNameArray; param.relationArrayTestSize[REL2][REL_ENT2] = FEATURE_QUERY_ACTION_PREPOSITION_ACTION_EQUIVALENT_QUERY_VARIABLE_NUMBER_OF_TYPES;

		//opt1
	GIAgenericDepRelInterpretationParameters paramOpt1 = param;

		//opt1a
	GIAgenericDepRelInterpretationParameters paramOpt1a = paramOpt1;
	paramOpt1a.expectToFindPrepositionTest[REL1] = true;	//added 24 July 2013
	/*
	What is yarn used in the making of?
	dobj(used-4, What-1)
	auxpass(used-4, is-2)
	nsubjpass(used-4, yarn-3)
	root(ROOT-0, used-4)
	det(making-7, the-6)
	prep_in(used-4, making-7)
	prep(making-7, of-8)
	/
	_obj(making[7], what[1])
	_obj(use[4], yarn[3])
	prep_in(use[4], making[7])
	_prep(making[7], of[8])

	prep_in(use[4], making[7]) + _obj(used[4], what[1]) +  _prep(making[7], of[8])	-> prep_of(making[7], what[1])
		*/
	paramOpt1a.numberOfRelations = 3;
	paramOpt1a.useRelationTest[REL3][REL_ENT3] = true; paramOpt1a.relationTest[REL3][REL_ENT3] = RELATION_TYPE_PREPOSITION_MODIFIER;
	paramOpt1a.useRelationIndexTest[REL3][REL_ENT3] = true; paramOpt1a.relationIndexTestRelationID[REL3][REL_ENT3] = REL1; paramOpt1a.relationIndexTestEntityID[REL3][REL_ENT3] = REL_ENT3; paramOpt1a.relationIndexTestIsNegative[REL3][REL_ENT3] = true;
	paramOpt1a.useRelationIndexTest[REL3][REL_ENT1] = true; paramOpt1a.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramOpt1a.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
	paramOpt1a.disableRelation[REL2] = true;
	paramOpt1a.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT3] = true; paramOpt1a.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT3] = REL3; paramOpt1a.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT3] = REL_ENT2; paramOpt1a.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL3][REL_ENT3] = true;
	paramOpt1a.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT2] = true; paramOpt1a.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT2] = REL2; paramOpt1a.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT2] = REL_ENT2;
	if(genericDependecyRelationInterpretation(&paramOpt1a, REL1))
	{
		//cout << "opt1a" << endl;
	}

		//opt1b
	/*
	What is the cart designed to integrate with?
	dep(designed-5, What-1)
	auxpass(designed-5, is-2)
	det(cart-4, the-3)
	nsubjpass(designed-5, cart-4)
	xsubj(integrate-7, cart-4)
	root(ROOT-0, designed-5)
	aux(integrate-7, to-6)
	xcomp(designed-5, integrate-7)
	prep(integrate-7, with-8)
	xcomp(designed-5, integrate-7) + dep(designed, what) + prep(integrate-7, with-8)	-> prep_with(integrate[7], what[1])
	*/
	GIAgenericDepRelInterpretationParameters paramOpt1b = paramOpt1a;
	paramOpt1b.expectToFindPrepositionTest[REL1] = false;
	paramOpt1b.useRelationTest[REL1][REL_ENT3] = true; paramOpt1b.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	if(genericDependecyRelationInterpretation(&paramOpt1b, REL1))
	{
		//cout << "opt1b" << endl;
	}

		//opt1c
	/*
	x(y, _$qVar[1]) + prep_q(y, a) -> x(a, _$qVar[1])
	eg1;
		Apples is used for making juice.
		What is apples used for making?
		dobj(used-4, What-1)
		auxpass(used-4, is-2)
		nsubjpass(used-4, apples-3)
		root(ROOT-0, used-4)
		prepc_for(used-4, making-6)
		/
		_obj(use[4], _$qVar[1])
		_obj(use[4], apple[3])
		prepc_for(use[4], make[6])
		NO [this solution is not universal]: dobj(used-4, What-1) + prepc_for(used-4, making-6) -> dobj(making-6, What-1)
		_obj(use[4], _$qVar[1]) + prepc_for(use[4], make[6]) -> _obj(make[6], _$qVar[1])
	eg 2;
		Apples are used for eating in the forest.
		Where are apples used for eating?
		advmod(used-4, Where-1)
		auxpass(used-4, are-2)
		nsubjpass(used-4, apples-3)
		root(ROOT-0, used-4)
		prepc_for(used-4, eating-6)
		/
		_%atLocation(use[4], _$qVar[1])
		_obj(use[4], apple[3])
		prepc_for(use[4], eat[6])
		_%atLocation(use[4], _$qVar[1]) + prepc_for(use[4], eat[6]) -> _%atLocation(eat[6], _$qVar[1])
	*/
	GIAgenericDepRelInterpretationParameters paramOpt1c = paramOpt1;
	paramOpt1c.expectToFindPrepositionTest[REL1] = true;
	paramOpt1c.numberOfRelations = 2;
	paramOpt1c.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramOpt1c.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramOpt1c.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	if(genericDependecyRelationInterpretation(&paramOpt1c, REL1))
	{
		//cout << "opt1c" << endl;
	}

	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP
		//opt2
	GIAgenericDepRelInterpretationParameters paramOpt2 = param;
	paramOpt2.numberOfRelations = 2;
	paramOpt2.useRelationTest[REL1][REL_ENT3] = true; paramOpt2.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;

		//opt2a
	/*
	What is the Co-cart designed to do?
	interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> _to-do(design[5], do[7]) + _dep(design[5], do[7]) [see 2c] -> _to-do(design[5], what[1]) [2a]
	*/
	GIAgenericDepRelInterpretationParameters paramOpt2a = paramOpt2;
	paramOpt2a.useRelationTest[REL1][REL_ENT2] = true; paramOpt2a.relationTest[REL1][REL_ENT2] = RELATION_DEPENDENT_DO;
	paramOpt2a.disableEntity[REL1][REL_ENT2] = true; paramOpt2a.disableEntityUseOriginalValues[REL1][REL_ENT2] = true;
	paramOpt2a.disableRelation[REL2] = true;
	paramOpt2a.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramOpt2a.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramOpt2a.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramOpt2a.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramOpt2a.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramOpt2a.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT2;
	if(genericDependecyRelationInterpretation(&paramOpt2a, REL1))
	{
		//cout << "opt2a" << endl;
	}

		//opt2b
	/*
	eg
	What is yarn used to make?
	dep(used-4, What-1)
	auxpass(used-4, is-2)
	nsubjpass(used-4, yarn-3)
	xsubj(make-6, yarn-3)
	root(ROOT-0, used-4)
	aux(make-6, to-5)
	xcomp(used-4, make-6)
	interpret; _to-do(used-4, make-6) + dep(used-4, What-1) -> _to-do(used-4, make-6) + dep(make-6, What-1) [see 2c] -> _to-do(used-4, make-6) + obj(make-6, What-1) [2b]
	*/
	GIAgenericDepRelInterpretationParameters paramOpt2b = paramOpt2;
	paramOpt2b.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramOpt2b.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramOpt2b.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramOpt2b.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramOpt2b.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	if(genericDependecyRelationInterpretation(&paramOpt2b, REL1))
	{
		//cout << "opt2b" << endl;
	}

		//opt2c
	//interpret; ...
	GIAgenericDepRelInterpretationParameters paramOpt2c = paramOpt2;
	paramOpt2c.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramOpt2c.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramOpt2c.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	if(genericDependecyRelationInterpretation(&paramOpt2c, REL1))
	{
		//cout << "opt2c" << endl;
	}

	#endif

#else
	//cout << "\n" << endl;

	//added 28 October 2012
 	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	bool foundFirstPreposition = false;
	while(currentRelationInList->next != NULL)
	{
		//cout << "GIArelation: " << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool prepositionFound = false;
			string relexPreposition = convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);

			#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP
			if(prepositionFound || (currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO))
			#else
			if(prepositionFound)
			#endif
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					//cout << "Relation2: " << currentRelationInList2->relationType << "(" << currentRelationInList2->relationGovernor << ", " << currentRelationInList2->relationDependent << ")" << endl;

					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList->relationType != currentRelationInList2->relationType)
						{
							if(currentRelationInList->relationGovernorIndex == currentRelationInList2->relationGovernorIndex)
							{
								bool queryEquivalentQueryVariableFound = false;
								for(int i=0; i<FEATURE_QUERY_ACTION_PREPOSITION_ACTION_EQUIVALENT_QUERY_VARIABLE_NUMBER_OF_TYPES; i++)
								{
									if(currentRelationInList2->relationDependent == featureQueryActionPrepositionActionEquivalentQueryVariableNameArray[i])
									{
										queryEquivalentQueryVariableFound = true;
									}
								}
								if(queryEquivalentQueryVariableFound)
								{
									if(!foundFirstPreposition)
									{
										bool foundSecondPreposition = false;
 										GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											if(!(currentRelationInList3->disabled))
											{
											//#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_PREPOSITION_MODIFIER)
												{
													if(currentRelationInList3->relationType != currentRelationInList->relationType)
													{//ensure unique preposition found
														if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)
														{//opt1a
															//cout << "opt1a" << endl;

															/*
															What is yarn used in the making of?
															dobj(used-4, What-1)
															auxpass(used-4, is-2)
															nsubjpass(used-4, yarn-3)
															root(ROOT-0, used-4)
															det(making-7, the-6)
															prep_in(used-4, making-7)
															prep(making-7, of-8)
															/
															_obj(making[7], _$qVar[1])
															_obj(use[4], yarn[3])
															prep_in(use[4], making[7])
															_prep(making[7], of[8])
															prep_in(use[4], making[7]) + _obj(used[4], _$qVar[1]) + _prep(making[7], of[8])	-> prep_of(making[7], _$qVar[1])
															*/

															string newPrepositionName = "";
															string relexPrepositionNew = GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->entityName; 	//currentRelationInList3->relationDependent
															newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + relexPrepositionNew;
															currentRelationInList2->disabled = true;

															currentRelationInList3->relationType = newPrepositionName;
															#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
															currentRelationInList3->relationTypeIndex = currentRelationInList3->relationDependentIndex;
															#endif
															currentRelationInList3->relationDependent = currentRelationInList2->relationDependent;
															currentRelationInList3->relationDependentIndex = currentRelationInList2->relationDependentIndex;

															foundSecondPreposition = true;
														}
													}
												}
											//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											}
											//#endif
											currentRelationInList3 = currentRelationInList3->next;
										}

										if(!foundSecondPreposition)
										{//opt1c/2c
											//cout << "opt1c/2c" << endl;
											//cout << "(!foundSecondPreposition)" << endl;

											/*
											x(y, _$qVar[1]) + prep_q(y, a) -> x(a, _$qVar[1])
											eg1;
												Apples is used for making juice.
												What is apples used for making?
												dobj(used-4, What-1)
												auxpass(used-4, is-2)
												nsubjpass(used-4, apples-3)
												root(ROOT-0, used-4)
												prepc_for(used-4, making-6)
												/
												_obj(use[4], _$qVar[1])
												_obj(use[4], apple[3])
												prepc_for(use[4], make[6])
												NO [this solution is not universal]: dobj(used-4, What-1) + prepc_for(used-4, making-6) -> dobj(making-6, What-1)
												_obj(use[4], _$qVar[1]) + prepc_for(use[4], make[6]) -> _obj(make[6], _$qVar[1])
											eg 2;
												Apples are used for eating in the forest.
												Where are apples used for eating?
												advmod(used-4, Where-1)
												auxpass(used-4, are-2)
												nsubjpass(used-4, apples-3)
												root(ROOT-0, used-4)
												prepc_for(used-4, eating-6)
												/
												_%atLocation(use[4], _$qVar[1])
												_obj(use[4], apple[3])
												prepc_for(use[4], eat[6])
												_%atLocation(use[4], _$qVar[1]) + prepc_for(use[4], eat[6]) -> _%atLocation(eat[6], _$qVar[1])
											*/

											currentRelationInList2->relationGovernor = currentRelationInList->relationDependent;
											currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP
											if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
											{//opt2a+b
												if(currentRelationInList->relationDependent == RELATION_DEPENDENT_DO)
												{//opt2a
													//cout << "opt2a" << endl;
													/*
													What is the Co-cart designed to do?
													interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> {IRRELEVANT: _to-do(design[5], do[7]) + _dep(design[5], do[7]) [2a] } -> _to-do(design[5], what[1]) [2b]
													*/

													currentRelationInList2->disabled = true;
													disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

													currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
													currentRelationInList->relationDependent = currentRelationInList2->relationDependent;
												}
												else
												{//opt2b
													//cout << "opt2b" << endl;
													/*
													eg
													What is yarn used to make?
													dep(used-4, What-1)
													auxpass(used-4, is-2)
													nsubjpass(used-4, yarn-3)
													xsubj(make-6, yarn-3)
													root(ROOT-0, used-4)
													aux(make-6, to-5)
													xcomp(used-4, make-6) [_to-do]
													interpret; _to-do(used-4, make-6) + dep(used-4, What-1) -> _to-do(used-4, make-6) + dep(make-6, What-1) [2a] -> _to-do(used-4, make-6) + obj(make-6, What-1) [2c]
													*/
													currentRelationInList2->relationType = RELATION_TYPE_OBJECT;
												}
											}
											#endif
										}

										foundFirstPreposition = true;
									}
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

void redistributeStanfordRelationsMultiwordPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	GIArelation* currentRelationInList;

#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_ACTION_PREPOSITION_ACTION
	//for queries only (1j6h)
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

		/*
		What is the Co-cart designed to do?
		dep(designed-5, What-1)
		auxpass(designed-5, is-2)
		nsubjpass(designed-5, Co-cart-4)
		xsubj(do-7, Co-cart-4)
		aux(do-7, to-6)
		xcomp(designed-5, do-7)
		interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> _to-do(design[5], what[1])

		What is the cart designed to integrate with?
		dep(designed-5, What-1)
		auxpass(designed-5, is-2)
		det(cart-4, the-3)
		nsubjpass(designed-5, cart-4)
		xsubj(integrate-7, cart-4)
		root(ROOT-0, designed-5)
		aux(integrate-7, to-6)
		xcomp(designed-5, integrate-7)
		prep(integrate-7, with-8)
		interpret;
			dep(designed-5, What-1)
			_to-do(designed-5, integrate-7)
			prep(integrate-7, with-8)
			=>
			_to-do(designed-5, integrate-7)
			prep_with(integrate-7, What-1)
		*/

		GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);

		paramA.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
		paramA.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_DEPENDENT;
		paramA.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;

		//opt1
		//interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> _to-do(design[5], what[1])
		GIAgenericDepRelInterpretationParameters paramOptA1 = paramA;
		paramOptA1.numberOfRelations = 2;
		paramOptA1.useRelationTest[REL1][REL_ENT2] = true; paramOptA1.relationTest[REL1][REL_ENT2] = RELATION_DEPENDENT_DO;	//this is check required? (perhaps the same case needs to be accounted for when (currentRelationInList->relationDependent != RELATION_DEPENDENT_DO)
		paramOptA1.disableRelation[REL2] = true;
		paramOptA1.disableEntity[REL1][REL_ENT2] = true; paramOptA1.disableEntityUseOriginalValues[REL1][REL_ENT2] = true;
		paramOptA1.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramOptA1.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramOptA1.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT2;
		genericDependecyRelationInterpretation(&paramOptA1, REL1);

		//opt2
		/*
		interpret;
		_to-do(designed-5, integrate-7)
		dep(designed-5, What-1)
		prep(integrate-7, with-8)
		=>
		_to-do(designed-5, integrate-7)
		prep_with(integrate-7, What-1)
		*/
		GIAgenericDepRelInterpretationParameters paramOptA2 = paramA;
		paramOptA2.numberOfRelations = 3;
		paramOptA2.useRelationTest[REL3][REL_ENT3] = true; paramOptA2.relationTest[REL3][REL_ENT3] = RELATION_TYPE_PREPOSITION_MODIFIER;
		paramOptA2.useRelationIndexTest[REL3][REL_ENT1] = true; paramOptA2.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramOptA2.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
		paramOptA2.disableRelation[REL2] = true;
		paramOptA2.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT3] = true; paramOptA2.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT3] = REL3; paramOptA2.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT3] = REL_ENT2; paramOptA2.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL3][REL_ENT3] = true;
		paramOptA2.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT2] = true; paramOptA2.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT2] = REL2; paramOptA2.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT2] = REL_ENT2;
		genericDependecyRelationInterpretation(&paramOptA2, REL1);

	#else

		GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
			if(!(currentRelationInList->disabled))
			{
			#endif
				if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
				{
 					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
						//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford

							if(currentRelationInList2->relationType == RELATION_TYPE_DEPENDENT)
							{
								if(currentRelationInList->relationGovernor == currentRelationInList2->relationGovernor)
								{
									if(currentRelationInList->relationDependent == RELATION_DEPENDENT_DO)		//this is check required? (perhaps the same case needs to be accounted for when (currentRelationInList->relationDependent != RELATION_DEPENDENT_DO)
									{
										//interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> _to-do(design[5], what[1])

										currentRelationInList2->disabled = true;
										disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

										currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationDependent = currentRelationInList2->relationDependent;

									}
									else
									{
										//see if preposition exists;

 										GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											if(!(currentRelationInList3->disabled))
											{
											#endif

												if(currentRelationInList3->relationType == RELATION_TYPE_PREPOSITION_MODIFIER)
												{

													if(currentRelationInList3->relationGovernor == currentRelationInList->relationDependent)
													{
														/*
														interpret;
														_to-do(designed-5, integrate-7)
														dep(designed-5, What-1)
														prep(integrate-7, with-8)
														=>
														_to-do(designed-5, integrate-7)
														prep_with(integrate-7, What-1)
														*/

														currentRelationInList2->disabled = true;

														string newPrepositionName = "";
														string relexPreposition = GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->entityName; 	//currentRelationInList3->relationDependent
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + relexPreposition;

														currentRelationInList3->relationType = newPrepositionName;
														#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
														currentRelationInList3->relationTypeIndex = currentRelationInList3->relationDependentIndex;
														#endif
														currentRelationInList3->relationDependentIndex = currentRelationInList2->relationDependentIndex;
														currentRelationInList3->relationDependent = currentRelationInList2->relationDependent;

													}
												}
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											}
											#endif

											currentRelationInList3 = currentRelationInList3->next;
										}
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
	#endif

#endif


	/*
	What are the patent claims on?
	match dep with prep
		dep(are-2, What-1) + nsubj(are-2, claims-5) + prep(are-2, on-6):
			dep(are-2, What-1) + prep(are-2, on-6) => prep_on(are-2, What-1)
				[then rely upon redistributeStanfordRelationsMultiwordPreposition() for  nsubj(are-4, claims-3) / prep_on(are-4, frame-8) => prep_on(claims-3, frame-8)]
	*/
	//for queries only (1j6g)

//Supports Stanford Parser/CoreNLP version 2+ e.g. 2013-04-04
//FUTURE (get from XML): Supports Stanford Parser/CoreNLP version 3+ e.g. 2014-01-04
#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_WITH_BE
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramB.numberOfRelations = 2;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DEPENDENT;
	paramB.useRelationTest[REL2][REL_ENT3] = true; paramB.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_MODIFIER;
	paramB.useRelationIndexTest[REL2][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramB.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramB.useRelationTest[REL1][REL_ENT1] = true; paramB.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	paramB.useRelationTest[REL2][REL_ENT1] = true; paramB.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	paramB.disableRelation[REL1] = true;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL2; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2; paramB.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL2][REL_ENT3] = true;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;
	paramB.disableEntity[REL1][REL_ENT1] = true;
	genericDependecyRelationInterpretation(&paramB, REL1);
#else

	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		//#endif
			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					//#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_MODIFIER)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching relationship

								if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
								{
									currentRelationInList->disabled = true;
									string newPrepositionName = "";
									string relexPreposition = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName; 	//currentRelationInList2->relationDependent
									newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + relexPreposition;

									currentRelationInList2->relationType = newPrepositionName;
									#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
									currentRelationInList2->relationTypeIndex = currentRelationInList2->relationDependentIndex;
									#endif									
									currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
									currentRelationInList2->relationDependent = currentRelationInList->relationDependent;

									disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
								}
							}
						}
					//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					//#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
#endif



	/*
	need to consider this case for the following example text;
	The patent claims are on the frame.
	The patent claims are on the cart frame, the wheels and the golf bag connection mechanism.
	det(claims-3, The-1)
	nn(claims-3, patent-2)
	nsubj(are-4, claims-3)
	root(ROOT-0, are-4)
	det(frame-8, the-6)
	nn(frame-8, cart-7)
	prep_on(are-4, frame-8)
	det(wheels-11, the-10)
	prep_on(are-4, wheels-11)
	conj_and(frame-8, wheels-11)
	det(mechanism-17, the-13)
	nn(mechanism-17, golf-14)
	nn(mechanism-17, bag-15)
	nn(mechanism-17, connection-16)
	prep_on(are-4, mechanism-17)
	conj_and(frame-8, mechanism-17)
	*/
#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	/*eg [case added GIA 2c1c]
	The chicken is not near the house.
	nsubj(is-3, chicken-2) + prep_near(is-3, house-7) + neg(is-3, not-4) -> neg(near-5, not-4)
	The chicken is sometimes near the house.
	nsubj(is-3, chicken-2) + prep_near(is-3, house-7) + advmod(is-3, sometimes-4) -> advmod(near-5, sometimes-4)
	*/
	GIAgenericDepRelInterpretationParameters paramC2(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramC2.numberOfRelations = 3;
	paramC2.useRelationTest[REL1][REL_ENT3] = true; paramC2.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramC2.expectToFindPrepositionTest[REL2] = true;
	paramC2.useRelationArrayTest[REL3][REL_ENT3] = true; paramC2.relationArrayTest[REL3][REL_ENT3] = redistributionStanfordRelationsNegAndAdvRelations; paramC2.relationArrayTestSize[REL3][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_NEG_AND_ADV_RELATIONS;
	paramC2.useRelationIndexTest[REL2][REL_ENT1] = true; paramC2.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramC2.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramC2.useRelationIndexTest[REL3][REL_ENT1] = true; paramC2.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramC2.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramC2.useRelationTest[REL1][REL_ENT1] = true; paramC2.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	paramC2.useRelationTest[REL2][REL_ENT1] = true; paramC2.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	paramC2.useRelationTest[REL3][REL_ENT1] = true; paramC2.relationTest[REL3][REL_ENT1] = RELATION_ENTITY_BE;
	paramC2.useRedistributeRelationEntityIndexReassignment[REL3][REL_ENT1] = true; paramC2.redistributeRelationEntityIndexReassignmentRelationID[REL3][REL_ENT1] = REL2; paramC2.redistributeRelationEntityIndexReassignmentRelationEntityID[REL3][REL_ENT1] = REL_ENT3;
	genericDependecyRelationInterpretation(&paramC2, REL1);
	#endif

	//eg1 look for nsubj/prep combination, eg nsubj(are-4, claims-3) + prep_on(are-4, frame-8) => prep_on(claims-3, frame-8)
	//eg2 case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION; The claims that are on the frame are blue. , nsubj(are-4, claims-2) + prep_on(are-4, frame-7) + rcmod(claims-2, are-4)
		//OLD: look for nsubj/prep combination, eg nsubj(next-4, garage-2) + prep_to(next-4, house-7)	=> prep_subj(next_to, house) + prep_subj(next_to, garage)
		//general parameters
	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramC.numberOfRelations = 2;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramC.expectToFindPrepositionTest[REL2] = true;
	paramC.useRelationIndexTest[REL2][REL_ENT1] = true; paramC.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramC.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION_OLD
	paramC.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramC.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
	paramC.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramC.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
	paramC.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL2][REL_ENT1] = true;
		paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL2][REL_ENT1][0] = REL2; paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL2][REL_ENT1][0] = REL_ENT1;
		paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL2][REL_ENT1][1] = REL2; paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL2][REL_ENT1][1] = REL_ENT3;
	#else
	paramC.useRelationTest[REL1][REL_ENT1] = true; paramC.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	paramC.useRelationTest[REL2][REL_ENT1] = true; paramC.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	paramC.disableRelation[REL1] = true;
	paramC.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramC.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramC.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramC.disableEntity[REL1][REL_ENT1] = true;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	/*eg;
	[case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION]
	The claims that are on the frame are blue.
	nsubj(are-4, claims-2)
	prep_on(are-4, frame-7)
	rcmod(claims-2, are-4)
	*/
	paramC.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL2] = true;
	paramC.useRelationTest[REL3][REL_ENT3] = true; paramC.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramC.useRelationIndexTest[REL3][REL_ENT2] = true; paramC.relationIndexTestRelationID[REL3][REL_ENT2] = REL1; paramC.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
	paramC.useRelationIndexTest[REL3][REL_ENT1] = true; paramC.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramC.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
	paramC.useRelationTest[REL3][REL_ENT2] = true; paramC.relationTest[REL3][REL_ENT2] = RELATION_ENTITY_BE;
	#endif
	genericDependecyRelationInterpretation(&paramC, REL1);
	#endif
#else

	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool prepositionFound = false;
						string relexPreposition = convertPrepositionToRelex(&(currentRelationInList2->relationType), &prepositionFound);

						if(prepositionFound)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching relationship

								#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION_OLD
								if(!(currentRelationInList2->prepositionCombinationAlreadyCreatedTemp))
								{
									string newPrepositionName = "";
									newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

									#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
									//cout << "newPrepositionName = " << newPrepositionName << endl;
									#endif

									GIArelation* subjectOfPrepositionRelation = currentRelationInList;
									GIArelation* objectOfPrepositionRelation = currentRelationInList2;
									subjectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
									objectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
									GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName = newPrepositionName;
									currentRelationInList2->prepositionCombinationAlreadyCreatedTemp = true;
								}
								#else
									if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
									{
										#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
										/*eg;
										[case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION]
										The claims that are on the frame are blue.
										nsubj(are-4, claims-2)
										prep_on(are-4, frame-7)
										rcmod(claims-2, are-4)
										*/
											//NB prepositions imply same reference set by default: "The claims that are on the frame are blue." = "The claims on the frame are blue."
										bool auxiliaryIndicatesDifferentReferenceSet = true;
										GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
											{
												if((currentRelationInList3->relationDependentIndex == currentRelationInList->relationGovernorIndex) && (currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex) && (currentRelationInList3->relationDependent == RELATION_ENTITY_BE))
												{
													auxiliaryIndicatesDifferentReferenceSet = false;
												}
											}
											currentRelationInList3 = currentRelationInList3->next;
										}
										currentRelationInList2->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;
										#ifdef GIA_ADVANCED_REFERENCING_DEBUG
										//cout << "\t\t\t0currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
										//cout << "\t\t\t0auxiliaryIndicatesDifferentReferenceSet = " << auxiliaryIndicatesDifferentReferenceSet << endl;
										#endif
										#endif

										currentRelationInList->disabled = true;
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList->relationDependent;

										disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
									}
								#endif
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
#endif


	/*
	//stanford parser prepositition reduction check (based upon http://en.wikipedia.org/wiki/List_of_English_prepositions);
	Y The woman is white according to John.
	Y The job was complete apart from the axel.
	Y As for Tim, he knew no other way.
	Y As per physics, the ball fell.
	N As regards the apple.
	Y Aside from that, all was good.
	N They went back to the mall.
	Y It grew tall because of the water.
	N He is close to the house.
	N The computer broke due to the fire.
	Y All is fine except for the carrot.
	Y The house is far from the beach.
	Y The man fell into the boat.
	Y The kitten is inside of the box.
	Y The kettle is noisy instead of quite.
	N The house is left of the bank.
	N The carriage is near to the horse.
	N The farmer is next to the plank.
	N The chicken goes on to the plank.
	N The chicken when out of the box.
	N The man is outside of the house.
	N Owing to the weather, he bought the paint.
	Y Prior to the fall, he ate a pie.

	Y Regardless of the time, it will be done.
	N Right of the house, is the beach.
	N Subsequent to the holidays, it will be done.
	N Thanks to his results, he will watch tv.
	N is that of Tom's doing?
	N He reached up to the sky.
	N He eats chocolate, where as he is skinny.	[Only possible for stanford core nlp - not for stanford parser]

	N As far as they are concerned, nothing could be better. [Only possible for stanford core nlp - not for stanford parser]
	Y He bought a pie as well as the cake.

	collapse these prepositions;

	[CaseD]
	nsubj(broke-3, computer-2)
	acomp(broke-3, due-4)
	prep_to(due-4, fire-7)

		[case added 1 June 2012]
		He used a computer that broke due to the fire.
		nsubj(broke-6, computer-4)
		rcmod(computer-4, broke-6)
		acomp(broke-6, due-7)
		prep_to(due-7, fire-10)

	[CaseA]
	nsubj(close-3, He-1)
	cop(close-3, is-2)
	prep_to(close-3, house-6)	[assert "to"/"of" prepositions 5 July 2013]

	[CaseA]
	nsubjpass(left-4, house-2)
	auxpass(left-4, is-3)
	prep_of(left-4, bank-7)

		[DONE required to be removed based upon the following; In addition to fast assembly, time is saved by not having to connect and disconnect the golf cart and bag at the beginning and end of a game.]
		nsubjpass(saved-9, time-7)
		auxpass(saved-9, is-8)
		prep_in_addition_to(saved-9, assembly-5)

	[CaseA]
	nsubj(near-4, carriage-2)
	cop(near-4, is-3)
	prep_to(near-4, horse-7)

		[case added 15 May 2012]
		He rode the carriage that is near to the horse.
		nsubj(near-7, carriage-4)
		cop(near-7, is-6)
		rcmod(carriage-4, near-7)
		prep_to(near-7, horse-10)

	[CaseA]
	nsubj(next-4, farmer-2)
	cop(next-4, is-3)
	prep_to(next-4, plank-7

	[CaseB]
	nsubj(goes-3, chicken-2)
	prt(goes-3, on-4)
	prep_to(goes-3, plank-7)

	[CaseA]
	nsubj(outside-4, man-2)
	cop(outside-4, is-3)
	prep_of(outside-4, house-7)

		-------------------
		All removed cases:
		-------------------

		[Removed 3 August 2012]
		[CaseB]
		nsubj(went-2, They-1)
		advmod(went-2, back-3)
		prep_to(went-2, mall-6)

		[Removed because (currentRelationInList3->relationGovernorIndex != currentRelationInList->relationGovernorIndex)]
		dobj(bought-7, paint-9)
		partmod(bought-7, Owing-1)	//NB currently interpreted as obj
		prep_to(Owing-1, weather-4)

		[CaseE]
		nsubj(beach-8, Right-1)
		cop(beach-8, is-6)
		prep_of(Right-1, house-4)

		[CaseD]
		nsubjpass(done-9, it-6)
		partmod(done-9, Subsequent-1)
		prep_to(Subsequent-1, holidays-4)

		[CaseD]
		nsubj(watch-8, he-6)
		partmod(watch-8, Thanks-1)
		prep_to(Thanks-1, results-4)

		[CaseF]
		nsubj(doing-6, that-2)
		aux(doing-6, is-1)
		prep_of(that-2, Tom-4)

		[Not required to be removed based upon the following; Space is saved by not having a bulky cart to store at home and in the car. , because "to-10" is not "be-10"]
		nsubj(store-11, cart-9)
		aux(store-11, to-10)
		prep_at(store-11, home-13)
		prep_in(store-11', car-17)

	[CaseB]
	nsubj(reached-2, He-1)
	prt(reached-2, up-3)
	prep_to(reached-2, sky-6)

		[case added 1 June 2012]
		It was he that reached up to the sky.
		nsubj(reached-5, he-3)
		rcmod(he-3, reached-5)
		prt(reached-5, up-6)
		prep_to(reached-5, sky-9)


	summary of multiword preposition contraction process;
	3. obj/subj (a, b)
	2. aux, auxpass, cop, acomp, partmod, prt ) (a, z)
	1. prep_x (a, c)
	->
	3. obj/subj
	1. prep_z_c(a, c)

	....
	additional tests added 15 May 2012;

	Y she hits the ball, close to the house.
	N she hits the ball, left of the house.
	Y she hits the ball, near to the house.
	Y she hits the ball, outside of the house.
	N she hits the ball, right of the house.

	nsubj(hits-2, she-1)
	root(ROOT-0, hits-2)
	det(ball-4, the-3)
	dobj(hits-2, ball-4)
	appos(ball-4, right-6)
	det(house-9, the-8)
	prep_of(right-6, house-9)

		-------------------
		All original cases:
		-------------------

		[CaseA]
		nsubj(close-3, He-1)
		cop(close-3, is-2)
		prep_to(close-3, house-6)

		[CaseA] {NOT SUPPORTED}
		nsubjpass(left-4, house-2)
		auxpass(left-4, is-3)
		prep_of(left-4, bank-7)

		[CaseA]
		nsubj(near-4, carriage-2)
		cop(near-4, is-3)
		prep_to(near-4, horse-7)

		[CaseA]
		nsubj(next-4, farmer-2)
		cop(next-4, is-3)
		prep_to(next-4, plank-7

		[CaseA]
		nsubj(outside-4, man-2)
		cop(outside-4, is-3)
		prep_of(outside-4, house-7)

		[CaseB]
		nsubj(went-2, They-1)
		advmod(went-2, back-3)
		prep_to(went-2, mall-6)

		[CaseB]
		nsubj(goes-3, chicken-2)
		prt(goes-3, on-4)
		prep_to(goes-3, plank-7)

		[CaseB]
		nsubj(reached-2, He-1)
		prt(reached-2, up-3)
		prep_to(reached-2, sky-6)


		[CaseD]
		nsubj(broke-3, computer-2)
		acomp(broke-3, due-4)
		prep_to(due-4, fire-7)

		[CaseD]
		nsubjpass(done-9, it-6)
		partmod(done-9, Subsequent-1)
		prep_to(Subsequent-1, holidays-4)

		[CaseD]
		nsubj(watch-8, he-6)
		partmod(watch-8, Thanks-1)
		prep_to(Thanks-1, results-4)

		[CaseE]
		nsubj(beach-8, Right-1)
		cop(beach-8, is-6)
		prep_of(Right-1, house-4)

		[CaseF]
		nsubj(doing-6, that-2)
		aux(doing-6, is-1)
		prep_of(that-2, Tom-4)


	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

		//general parameters
	GIAgenericDepRelInterpretationParameters paramOptMult(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramOptMult.expectToFindPrepositionTest[REL1] = true;
	//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford [must disable these later, at the end of redistribution]
	paramOptMult.useRelationArrayTest[REL3][REL_ENT3] = true; paramOptMult.relationArrayTest[REL3][REL_ENT3] = redistributionStanfordRelationsMultiwordPrepositionSubjObjRelations; paramOptMult.relationArrayTestSize[REL3][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS;
	//note the following test should not be required as disabled relations eg cop are not being parsed: "if(!(currentRelationInList->prepositionCombinationAlreadyCreatedTemp))"

	//optMultA
	/*
	[CaseA]
	He is close to the house.
	prep_to(close-3, house-6)
	cop(close-3, is-2)
	nsubj(close-3, He-1)
	det(house-6, the-5)
	*/
	GIAgenericDepRelInterpretationParameters paramOptMultA = paramOptMult;
	paramOptMultA.numberOfRelations = 3;
	paramOptMultA.useRelationArrayTest[REL1][REL_ENT3] = true; paramOptMultA.relationArrayTest[REL1][REL_ENT3] = redistributionStanfordRelationsMultiwordPrepositionSupportedPrepositionsTypeA; paramOptMultA.relationArrayTestSize[REL1][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUPPORTED_PREPOSITIONS_TYPEA;
	paramOptMultA.useRelationArrayTest[REL2][REL_ENT3] = true; paramOptMultA.relationArrayTest[REL2][REL_ENT3] = redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeA; paramOptMultA.relationArrayTestSize[REL2][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA;
	paramOptMultA.useRelationTest[REL2][REL_ENT2] = true; paramOptMultA.relationTest[REL2][REL_ENT2] = RELATION_ENTITY_BE;
	paramOptMultA.useRelationIndexTest[REL2][REL_ENT1] = true; paramOptMultA.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramOptMultA.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramOptMultA.useRelationIndexTest[REL3][REL_ENT1] = true; paramOptMultA.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramOptMultA.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT1;
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_IGNORE_NSUBJ_AND_PREPOSITION_AND_COP_AND_DET
	/*
	prevents multiword prepositional collapse of "name of" in; "Max is the name of the red dog" [/ "3 is the number of the red dogs."], based on 'det(name-4, the-3)'
	nsubj(name-4, Max-1)
	cop(name-4, is-2)	//rel2
	det(name-4, the-3)
	root(ROOT-0, name-4)
	det(dog-8, the-6)	//rel4
	amod(dog-8, red-7)
	prep_of(name-4, dog-8)
	*/
	//paramOptMultA.relationTestSpecialCaseNotDefinite[REL3][REL_ENT1] = true;
	GIAentityCharacteristic relationTestSpecialCaseNotDefinite("grammaticalDefiniteTemp", "false");
	paramOptMultA.specialCaseCharacteristicsTestAndVector[REL3][REL_ENT1].push_back(&relationTestSpecialCaseNotDefinite);
	#endif
	paramOptMultA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramOptMultA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL3; paramOptMultA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
	paramOptMultA.disableRelation[REL2] = true;
	paramOptMultA.disableRelation[REL3] = true;
	paramOptMultA.disableEntity[REL2][REL_ENT1] = true;
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//[case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION]
	//He rode the carriage that is near to the horse.
	//nsubj(near-7, carriage-4)
	//cop(near-7, is-6)
	//rcmod(carriage-4, near-7)
	//prep_to(near-7, horse-10)
	paramOptMultA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
	paramOptMultA.useRelationTest[REL4][REL_ENT3] = true; paramOptMultA.relationTest[REL4][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
	paramOptMultA.useRelationIndexTest[REL4][REL_ENT2] = true; paramOptMultA.relationIndexTestRelationID[REL4][REL_ENT2] = REL3; paramOptMultA.relationIndexTestEntityID[REL4][REL_ENT2] = REL_ENT1;
	paramOptMultA.useRelationIndexTest[REL4][REL_ENT1] = true; paramOptMultA.relationIndexTestRelationID[REL4][REL_ENT1] = REL3; paramOptMultA.relationIndexTestEntityID[REL4][REL_ENT1] = REL_ENT2;
	#endif
	paramOptMultA.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT3] = true;
		paramOptMultA.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][0] = REL2; paramOptMultA.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][0] = REL_ENT1;
		paramOptMultA.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][1] = REL1; paramOptMultA.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][1] = REL_ENT3;
	genericDependecyRelationInterpretation(&paramOptMultA, REL1);

	//optMultB
	/*
	[CaseB]
	The chicken goes on to the plank.
	nsubj(goes-3, chicken-2)
	prt(goes-3, on-4)
	prep_to(goes-3, plank-7)
	*/
	GIAgenericDepRelInterpretationParameters paramOptMultB = paramOptMult;
	paramOptMultB.numberOfRelations = 3;
	paramOptMultB.useRelationArrayTest[REL2][REL_ENT3] = true; paramOptMultB.relationArrayTest[REL2][REL_ENT3] = redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeB; paramOptMultB.relationArrayTestSize[REL2][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB;
	paramOptMultB.useRelationIndexTest[REL2][REL_ENT1] = true; paramOptMultB.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramOptMultB.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramOptMultB.useRelationIndexTest[REL3][REL_ENT1] = true; paramOptMultB.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramOptMultB.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT1;
	paramOptMultB.disableRelation[REL2] = true;
	paramOptMultB.disableEntity[REL2][REL_ENT2] = true;
	paramOptMultB.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT3] = true;
		paramOptMultB.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][0] = REL2; paramOptMultB.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][0] = REL_ENT2;
		paramOptMultB.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][1] = REL1; paramOptMultB.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][1] = REL_ENT3;
	genericDependecyRelationInterpretation(&paramOptMultB, REL1);

	//optMultD
	/*
	[CaseD]
	Subsequent to the holidays, it will be done.
	nsubjpass(done-9, it-6)
	partmod(done-9, Subsequent-1)
	prep_to(Subsequent-1, holidays-4)
	*/
	GIAgenericDepRelInterpretationParameters paramOptMultD = paramOptMult;
	paramOptMultD.numberOfRelations = 3;
	paramOptMultD.useRelationArrayTest[REL2][REL_ENT3] = true; paramOptMultD.relationArrayTest[REL2][REL_ENT3] = redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeD; paramOptMultD.relationArrayTestSize[REL2][REL_ENT3] = GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPED;
	paramOptMultD.useRelationIndexTest[REL2][REL_ENT2] = true; paramOptMultD.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; paramOptMultD.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	paramOptMultD.useRelationIndexTest[REL3][REL_ENT1] = true; paramOptMultD.relationIndexTestRelationID[REL3][REL_ENT1] = REL2; paramOptMultD.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT1;
	//paramOptMultD.disableRelation[REL2] = true;	//removed 15 July 2013 to allow double use of partmod eg "The Co-cart is a high quality, ultra compact, lightweight cart, also known as a trolley or buggie.: partmod(quality-6, known-15) + prep_as(known-15, trolley-18) + prep_as(known-15, buggie-20) -> prep_know_as(quality[6], trolley[18]) + prep_know_as(quality[6], buggie[20])
	paramOptMultD.disableEntity[REL2][REL_ENT2] = true;
	paramOptMultD.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramOptMultD.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL3; paramOptMultD.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT1;
	paramOptMultD.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT3] = true;
		paramOptMultD.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][0] = REL2; paramOptMultD.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][0] = REL_ENT2;
		paramOptMultD.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT3][1] = REL1; paramOptMultD.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT3][1] = REL_ENT3;
	if(genericDependecyRelationInterpretation(&paramOptMultD, REL1))
	{
		paramOptMultD.relationFinalResult[REL2]->disabled =  true;	//see above
	}

#else
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool prepositionFound = false;
			string relexPreposition = convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);

			if(prepositionFound)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
					//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford

						bool multiwordPrepositionIntermediaryRelationTypeAFound = false;
						bool multiwordPrepositionSupportedPrepositionTypeAFound = false;
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUPPORTED_PREPOSITIONS_TYPEA; i++)
						{
							if(relexPreposition == redistributionStanfordRelationsMultiwordPrepositionSupportedPrepositionsTypeA[i])
							{
								multiwordPrepositionSupportedPrepositionTypeAFound = true;
							}
						}
						if(multiwordPrepositionSupportedPrepositionTypeAFound)
						{//[assert "to"/"of" prepositions 5 July 2013]

							for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA; i++)
							{
								if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeA[i])
								{
									if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
									{
										multiwordPrepositionIntermediaryRelationTypeAFound = true;
									}
								}
							}
						}

						bool multiwordPrepositionIntermediaryRelationTypeBFound = false;
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB; i++)
						{
							if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeB[i])
							{
								multiwordPrepositionIntermediaryRelationTypeBFound = true;
							}
						}

						bool multiwordPrepositionIntermediaryRelationTypeDFound = false;
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPED; i++)
						{
							if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeD[i])
							{
								multiwordPrepositionIntermediaryRelationTypeDFound = true;
							}
						}


						bool multiwordPrepositionIntermediaryRelationTypeEFound = false;
						/*
						for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEE; i++)
						{
							if(currentRelationInList2->relationType == redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeE[i])
							{
								if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
								{
									multiwordPrepositionIntermediaryRelationTypeEFound = true;
								}
							}
						}
						*/


						if(multiwordPrepositionIntermediaryRelationTypeAFound || multiwordPrepositionIntermediaryRelationTypeBFound || multiwordPrepositionIntermediaryRelationTypeDFound || multiwordPrepositionIntermediaryRelationTypeEFound)
						{
 							GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								if(!(currentRelationInList3->disabled))
								{
								#endif
									bool multiwordPrepositionSubjectOrObjectRelationFound = false;
									for(int i=0; i<GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS; i++)
									{
										if(currentRelationInList3->relationType == redistributionStanfordRelationsMultiwordPrepositionSubjObjRelations[i])
										{
											multiwordPrepositionSubjectOrObjectRelationFound = true;
										}
									}

									if(multiwordPrepositionSubjectOrObjectRelationFound)
									{
										if(!(currentRelationInList->prepositionCombinationAlreadyCreatedTemp))
										{
											if(multiwordPrepositionIntermediaryRelationTypeAFound)
											{
												/*
												[CaseA]
												He is close to the house.
												nsubj(close-3, He-1)
												cop(close-3, is-2)
												prep_to(close-3, house-6)
												*/
												if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
												{//found a matching relationship

													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
													{//found a matching relationship

														/*
														#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
														bool multiwordPrepositionSubjectOrObjectDependentOrGovernorNameOfNumberOfFound = false;
														for(int i=0; i<GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER_OF_TYPES; i++)
														{
															if(currentRelationInList3->relationGovernor == redistributionRelationsSupportNameOfSubjectDependentOrGovernorNameArray[i])
															{
																multiwordPrepositionSubjectOrObjectDependentOrGovernorNameOfNumberOfFound = true;
																//prevents multiword prepositional collapse of "name of" in; "Max is the name of the red dog" / "3 is the number of the red dogs."
																//nsubj(name-4, Max-1)
																//cop(name-4, is-2)
																//det(name-4, the-3)
																//root(ROOT-0, name-4)
																//det(dog-8, the-6)
																//amod(dog-8, red-7)
																//prep_of(name-4, dog-8)
															}
														}
														if(!multiwordPrepositionSubjectOrObjectDependentOrGovernorNameOfNumberOfFound)
														{
														#endif
														*/
														#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_IGNORE_NSUBJ_AND_PREPOSITION_AND_COP_AND_DET
														if(!(GIAentityNodeArray[currentRelationInList3->relationGovernorIndex]->grammaticalDefiniteTemp))
														{
														/*
														prevents multiword prepositional collapse of "name of" in; "Max is the name of the red dog" [/ "3 is the number of the red dogs."], based on 'det(name-4, the-3)'
														nsubj(name-4, Max-1)
														cop(name-4, is-2)
														det(name-4, the-3)
														root(ROOT-0, name-4)
														det(dog-8, the-6)
														amod(dog-8, red-7)
														prep_of(name-4, dog-8)
														*/
														#endif

															#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
															//[case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION]
															//He rode the carriage that is near to the horse.
															//nsubj(near-7, carriage-4)
															//cop(near-7, is-6)
															//rcmod(carriage-4, near-7)
															//prep_to(near-7, horse-10)
															bool auxiliaryIndicatesDifferentReferenceSet = true;
															GIArelation* currentRelationInList4 = currentSentenceInList->firstRelationInList;
															while(currentRelationInList4->next != NULL)
															{
																if(currentRelationInList4->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
																{
																	if((currentRelationInList4->relationDependentIndex == currentRelationInList3->relationGovernorIndex) && (currentRelationInList4->relationGovernorIndex == currentRelationInList3->relationDependentIndex))		//OLD: before 1 June 2012 code review: if((currentRelationInList4->relationDependentIndex == currentRelationInList->relationGovernorIndex) && (currentRelationInList4->relationGovernorIndex == currentRelationInList->relationDependentIndex))
																	{
																		auxiliaryIndicatesDifferentReferenceSet = false;
																	}
																}
																currentRelationInList4 = currentRelationInList4->next;
															}
															#ifdef GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILIARY_AND_SET_DIFFERENT_REFERENCE_SET
															//need to reset same reference set values on the preposition

															currentRelationInList->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;

															#ifdef GIA_ADVANCED_REFERENCING_DEBUG
															//currentRelationInList3->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;
															//cout << "\t\t\t1currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
															//cout << "\t\t\t1currentRelationInList->auxiliaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxiliaryIndicatesDifferentReferenceSet << endl;
															#endif

															#endif
															#endif


															GIAentityNode* entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex];

															string newPrepositionName = "";
															newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

															currentRelationInList->relationType = newPrepositionName;
															/*
															#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
															currentRelationInList->relationTypeIndex = entityContainingFirstWordOfMultiwordPreposition->entityIndex;
															#endif
															*/
															currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

															currentRelationInList->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
															currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->entityName;

															#ifdef GIA_ADVANCED_REFERENCING_DEBUG
															//cout << "redistributeStanfordRelationsMultiwordPreposition{}: newPrepositionName = " << newPrepositionName << endl;
															//cout << "\t\t\t2currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
															//cout << "\t\t\t2auxiliaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxiliaryIndicatesDifferentReferenceSet << endl;
															#endif

															currentRelationInList2->disabled = true;
															currentRelationInList3->disabled = true;	//added 3 June 2012

															disableEntity(entityContainingFirstWordOfMultiwordPreposition);
														#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_IGNORE_NSUBJ_AND_PREPOSITION_AND_COP_AND_DET
														}
														#endif
													}
												}
											}
											if(multiwordPrepositionIntermediaryRelationTypeBFound)
											{
												/*
												[CaseB]
												The chicken goes on to the plank.
												nsubj(goes-3, chicken-2)
												prt(goes-3, on-4)
												prep_to(goes-3, plank-7)
												*/

												if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
												{//found a matching relationship

													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
													{//found a matching relationship

														GIAentityNode* entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
														/*
														#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
														currentRelationInList->relationTypeIndex = entityContainingFirstWordOfMultiwordPreposition->entityIndex;
														#endif
														*/
														currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

														currentRelationInList2->disabled = true;

														disableEntity(entityContainingFirstWordOfMultiwordPreposition);
													}
												}
											}
											if(multiwordPrepositionIntermediaryRelationTypeDFound)
											{
												/*
												[CaseD]
												Subsequent to the holidays, it will be done.
												nsubjpass(done-9, it-6)
												partmod(done-9, Subsequent-1)
												prep_to(Subsequent-1, holidays-4)
												*/

												if(currentRelationInList2->relationDependentIndex == currentRelationInList->relationGovernorIndex)
												{//found a matching relationship

													if(currentRelationInList3->relationGovernorIndex == currentRelationInList2->relationGovernorIndex)
													{//found a matching relationship

														//cout << "sdf" << endl;

														GIAentityNode* entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
														/*
														#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
														currentRelationInList->relationTypeIndex = entityContainingFirstWordOfMultiwordPreposition->entityIndex;
														#endif
														*/
														currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

														currentRelationInList->relationGovernorIndex = currentRelationInList3->relationGovernorIndex;
														currentRelationInList->relationGovernor =  GIAentityNodeArray[currentRelationInList3->relationGovernorIndex]->entityName;

														currentRelationInList2->disabled = true;

														disableEntity(entityContainingFirstWordOfMultiwordPreposition);
													}
												}
											}
											if(multiwordPrepositionIntermediaryRelationTypeEFound)
											{
												/*
												[CaseE]
												Right of the house, is the beach.
												nsubj(beach-8, Right-1)
												cop(beach-8, is-6)
												prep_of(Right-1, house-4)
												*/

												if(currentRelationInList2->relationGovernorIndex == currentRelationInList3->relationGovernorIndex)
												{//found a matching relationship
													if(currentRelationInList3->relationDependentIndex == currentRelationInList->relationGovernorIndex)
													{//found a matching relationship

														GIAentityNode* entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList3->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
														/*
														#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW_ASSIGN_RELATIONTYPEINDEX_FOR_CONCATONATIONS
														currentRelationInList->relationTypeIndex = entityContainingFirstWordOfMultiwordPreposition->entityIndex;
														#endif
														*/
														currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

														currentRelationInList->relationGovernorIndex = currentRelationInList3->relationGovernorIndex;
														currentRelationInList->relationGovernor =  GIAentityNodeArray[currentRelationInList3->relationGovernorIndex]->entityName;

														currentRelationInList2->disabled = true;

														disableEntity(entityContainingFirstWordOfMultiwordPreposition);
													}
												}
											}
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								}
								#endif

								currentRelationInList3 = currentRelationInList3->next;
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



#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
void redistributeStanfordRelationsInterpretOfAsPossessive(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg The ball of the red dog is green.   prep_of(ball-2, dog-6) ->  poss(ball-5, dog-3)

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.expectToFindPrepositionTest[REL1] = true;	//redundant
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PREPOSITION_OF;
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_POSSESSIVE;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		//#endif

			bool prepositionFound = false;
			string relexPreposition = convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);

			if(prepositionFound)
			{
				if(relexPreposition == RELATION_TYPE_PREPOSITION_OF)
				{
					#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
					cout << "DEBUG: redistributeStanfordRelationsInterpretOfAsPossessive{}: RELATION_TYPE_PREPOSITION_OF" << endl;
					#endif
					currentRelationInList->relationType = RELATION_TYPE_POSSESSIVE;
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


#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
void redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*interpret;
		[given 'The name of the dog near the farm is Max.']
		'What is the name of the red dog near the farm?' [return entity names]
			nsubj(is-2, name-4) + attr(is-2, What-1) {+ det(name-4, the-3)} + poss/prep_of(name-4, dog-8) -> appos(dog[8], _$qVar[1])	{_name(dog[8], _$qVar)??}
		'What is the dog's name?'
			nsubj(is-2, name-6) + attr(is-2, What-1) + poss(name-6, dog-4)

		'What is the number of the red dogs near the farm?' [return entity number/quantity]
			nsubj(is-2, number-4) + attr(is-2, What-1) {/ det(number-4, the-3)} + poss/prep_of(number-4, dogs-8) -> _quantity(dog[8], _$qVar[1])

	*/

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ATTRIBUTIVE;
	param.useRelationArrayTest[REL2][REL_ENT2] = true; param.relationArrayTest[REL2][REL_ENT2] = featureQueryWhatIsTheNameNumberOfNameArray; param.relationArrayTestSize[REL2][REL_ENT2] = FEATURE_QUERY_WHAT_IS_THE_NAME_NUMBER_OF_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.useRelationTest[REL2][REL_ENT1] = true; param.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationArrayTest[REL3][REL_ENT3] = true; param.relationArrayTest[REL3][REL_ENT3] = relationTypeOfOrPossessivePrepositionsNameArray; param.relationArrayTestSize[REL3][REL_ENT3] = RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
	param.useRelationTest[REL2][REL_ENT2] = true; param.relationTest[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHAT;

	/*interpret
	What is the name of the red dog near the farm? [return entity names]
		nsubj(is-2, name-4) + attr(is-2, What-1) {+ det(name-4, the-3)} + poss/prep_of(name-4, dog-8) -> appos(dog-1, _$qVar[1])	{_name(That-1, _$qVar)??}
	'What is the dog's name?'
		nsubj(is-2, name-6) + attr(is-2, What-1) + poss(name-6, dog-4)
	*/
	GIAgenericDepRelInterpretationParameters paramNameQuery = param;
	paramNameQuery.useRelationTest[REL1][REL_ENT2] = true; paramNameQuery.relationTest[REL1][REL_ENT2] = GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME;
	paramNameQuery.disableRelation[REL1] = true;
	paramNameQuery.disableRelation[REL3] = true;
	paramNameQuery.disableEntity[REL1][REL_ENT1] = true;
	paramNameQuery.disableEntity[REL3][REL_ENT1] = true;
	paramNameQuery.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramNameQuery.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramNameQuery.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramNameQuery.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL3; paramNameQuery.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramNameQuery.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; paramNameQuery.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
	//paramNameQuery.useRedistributeSpecialCaseIsNameQueryAssignment[REL2][REL_ENT2] = true;
	GIAentityCharacteristic useRedistributeSpecialCaseIsNameQueryAssignment("isNameQuery", "true");
	paramNameQuery.specialCaseCharacteristicsAssignmentVector[REL2][REL_ENT2].push_back(&useRedistributeSpecialCaseIsNameQueryAssignment);
	genericDependecyRelationInterpretation(&paramNameQuery, REL1);

	/*interpret
	What is the number of the red dogs near the farm? [return entity number/quantity]
	nsubj(is-2, number-4) + attr(is-2, What-1) {+ det(number-4, the-3)} + poss/prep_of(number-4, dogs-8) -> _quantity(dog[8], _$qVar[1])
	*/
	GIAgenericDepRelInterpretationParameters paramNumberQuery = param;
	paramNumberQuery.useRelationTest[REL1][REL_ENT2] = true; paramNumberQuery.relationTest[REL1][REL_ENT2] = GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER;
	paramNumberQuery.disableRelation[REL1] = true;
	paramNumberQuery.disableRelation[REL3] = true;
	paramNumberQuery.disableEntity[REL1][REL_ENT1] = true;
	paramNumberQuery.disableEntity[REL3][REL_ENT1] = true;
	paramNumberQuery.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramNumberQuery.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_QUANTITY;
	paramNumberQuery.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramNumberQuery.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL3; paramNumberQuery.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramNumberQuery.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; paramNumberQuery.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
	genericDependecyRelationInterpretation(&paramNumberQuery, REL1);

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				/*
				bool queryWhoWhatRelationDependentFound = false;
				for(int i=0; i<FEATURE_QUERY_WHAT_IS_THE_NAME_NUMBER_OF_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList->relationDependent == featureQueryWhatIsTheNameNumberOfNameArray[i])
					{
						queryWhoWhatRelationDependentFound = true;
					}
				}
				if(!queryWhoWhatRelationDependentFound)
				{
				*/
					bool attributiveRelationFound = false;

					//now find the associated relation..
 					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_ATTRIBUTIVE)
							{
								bool queryWhoWhatRelationDependentFound2 = false;
								for(int i=0; i<FEATURE_QUERY_WHAT_IS_THE_NAME_NUMBER_OF_NUMBER_OF_TYPES; i++)
								{
									if(currentRelationInList2->relationDependent == featureQueryWhatIsTheNameNumberOfNameArray[i])
									{
										queryWhoWhatRelationDependentFound2 = true;
									}
								}
								if(queryWhoWhatRelationDependentFound2)
								{
									if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
									{
										if((currentRelationInList2->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE))
										{//found a matching relationship

											bool queryWhatNameOrNumberRelationFound = false;
 											GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
											while(currentRelationInList3->next != NULL)
											{
												#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
												if(!(currentRelationInList2->disabled))
												{
												#endif

													#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
													if(currentRelationInList3->relationType == RELATION_TYPE_POSSESSIVE)
													#else
													bool prepositionFound = false;
													string relexPreposition = convertPrepositionToRelex(&(currentRelationInList3->relationType), &prepositionFound);
													if((currentRelationInList3->relationType == RELATION_TYPE_POSSESSIVE) || (prepositionFound && (relexPreposition == RELATION_TYPE_PREPOSITION_OF)))
													#endif
													{
														if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)
														{
															if(currentRelationInList2->relationDependent == REFERENCE_TYPE_QUESTION_QUERY_WHAT)
															{

																if(currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)
																{
																	/*interpret
																	What is the name of the red dog near the farm? [return entity names]
																		nsubj(is-2, name-4) / attr(is-2, What-1) {/ det(name-4, the-3)} / poss/prep_of(name-4, dog-8) -> appos(dog-1, _$qVar[1])	{_name(That-1, _$qVar)??}
																	*/

																	queryWhatNameOrNumberRelationFound = true;
																	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
																	cout << "DEBUG: redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(); GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME" << endl;
																	#endif
																	currentRelationInList->disabled =  true;
																	currentRelationInList3->disabled =  true;

																	GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
																	disableEntity(oldRedundantBeEntity);
																	GIAentityNode* oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
																	disableEntity(oldRedundantNameEntity);

																	currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
																	currentRelationInList2->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
																	currentRelationInList2->relationGovernor = currentRelationInList3->relationDependent;

																	GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
																	GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->isNameQuery = true;
																	currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;

																}
																else if(currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER)
																{
																	/*interpret
																	What is the number of the red dogs near the farm? [return entity number/quantity]
																		nsubj(is-2, number-4) / attr(is-2, What-1) {/ det(number-4, the-3)} / poss/prep_of(number-4, dogs-8) -> _quantity(dog[8], _$qVar[1])
																	*/

																	queryWhatNameOrNumberRelationFound = true;
																	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
																	cout << "DEBUG: redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(); GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER" << endl;
																	#endif
																	currentRelationInList->disabled =  true;
																	currentRelationInList3->disabled =  true;

																	GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
																	disableEntity(oldRedundantBeEntity);
																	GIAentityNode* oldRedundantNumberEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
																	disableEntity(oldRedundantNumberEntity);

																	currentRelationInList2->relationType = RELATION_TYPE_QUANTITY;
																	currentRelationInList2->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
																	currentRelationInList2->relationGovernor = currentRelationInList3->relationDependent;

																	GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
																	currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
																}
															}
														}
													}
												#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
												}
												#endif
												currentRelationInList3 = currentRelationInList3->next;
											}
										}
									}
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				/*
				}
				*/
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}
#endif

#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
void redistributeStanfordRelationsInterpretNameOfAsDefinition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	eg interpret 'The red dog's name is Max.'		nsubj(Max-7, name-5) + poss(name-5, dog-3) -> appos(dog-3, Max-7)
	eg interpret 'The name of the red dog is Max.'	nsubj(Max-7, name-5) + prep_of(name-5, dog-3) -> appos(dog-3, Max-7)
	eg interpret 'Max is the name of the red dog.' 	nsubj(name-4, Max-1) + prep_of(name-4, dog-8) -> appos(dog-3, Max-7)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
		//general parameters
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationArrayTest[REL2][REL_ENT3] = true; param.relationArrayTest[REL2][REL_ENT3] = relationTypeOfOrPossessivePrepositionsNameArray; param.relationArrayTestSize[REL2][REL_ENT3] = RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES;

	GIAgenericDepRelInterpretationParameters paramType1 = param;
	paramType1.useRelationTest[REL1][REL_ENT2] = true; paramType1.relationTest[REL1][REL_ENT2] = GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME;
	paramType1.useRelationIndexTest[REL1][REL_ENT2] = true; paramType1.relationIndexTestRelationID[REL1][REL_ENT2] = REL2; paramType1.relationIndexTestEntityID[REL1][REL_ENT2] = REL_ENT1;	//added 18 April 2014
	paramType1.disableRelation[REL1] = true;
	paramType1.disableEntity[REL1][REL_ENT2] = true;
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	paramType1.disableEntity[REL2][REL_ENT3] = true;	//added 18 April 2014
	#endif
	paramType1.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramType1.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramType1.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramType1.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL2; paramType1.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramType1.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramType1.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramType1.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT1;
	//paramType1.useRedistributeSpecialCaseIsNameAssignment[REL2][REL_ENT2] = true;
	GIAentityCharacteristic useRedistributeSpecialCaseIsNameAssignment("isName", "true");
	paramType1.specialCaseCharacteristicsAssignmentVector[REL2][REL_ENT2].push_back(&useRedistributeSpecialCaseIsNameAssignment);
	genericDependecyRelationInterpretation(&paramType1, REL1);

	GIAgenericDepRelInterpretationParameters paramType2 = param;
	paramType2.useRelationTest[REL1][REL_ENT1] = true; paramType2.relationTest[REL1][REL_ENT1] = GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME;
	paramType2.useRelationIndexTest[REL1][REL_ENT1] = true; paramType2.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; paramType2.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;	//added 18 April 2014
	paramType2.disableRelation[REL1] = true;
	paramType2.disableEntity[REL1][REL_ENT1] = true;
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	paramType2.disableEntity[REL2][REL_ENT3] = true;	//added 18 April 2014
	#endif
	paramType2.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramType2.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramType2.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramType2.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL2; paramType2.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramType2.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramType2.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; paramType2.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;
	//paramType2.useRedistributeSpecialCaseIsNameAssignment[REL2][REL_ENT2] = true;
	paramType2.specialCaseCharacteristicsAssignmentVector[REL2][REL_ENT2].push_back(&useRedistributeSpecialCaseIsNameAssignment);
	genericDependecyRelationInterpretation(&paramType2, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				if((currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME) || (currentRelationInList->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME))
				{
					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						//#endif
							#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
							if(currentRelationInList2->relationType == RELATION_TYPE_POSSESSIVE)
							#else
							bool prepositionFound = false;
							string relexPreposition = convertPrepositionToRelex(&(currentRelationInList2->relationType), &prepositionFound);
							if((currentRelationInList2->relationType == RELATION_TYPE_POSSESSIVE) || (prepositionFound && (relexPreposition == RELATION_TYPE_PREPOSITION_OF)))
							#endif
							{
								if(currentRelationInList2->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)	//added 11 August 2012
								{
									if(currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)
									{
										#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
										cout << "DEBUG: redistributeStanfordRelationsInterpretNameOfAsDefinition{}: relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME" << endl;
										#endif

										currentRelationInList->disabled =  true;

										GIAentityNode* oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
										disableEntity(oldRedundantNameEntity);

										currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationGovernorIndex;
										currentRelationInList2->relationDependent = currentRelationInList->relationGovernor;

										GIAentityNode* definitionEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
										definitionEntity->isName = true;
									}
									else if(currentRelationInList->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)
									{
										#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
										cout << "DEBUG: redistributeStanfordRelationsInterpretNameOfAsDefinition{}: relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME" << endl;
										#endif

										currentRelationInList->disabled =  true;

										GIAentityNode* oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
										disableEntity(oldRedundantNameEntity);

										currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationDependent = currentRelationInList->relationDependent;

										GIAentityNode* definitionEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
										definitionEntity->isName = true;
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
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}
#endif



void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser)
{
	/*
	eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) + advmod(is-3, away-6) - > _predadj(rabbit-2, away-6)
		//OLD: nsubj(is-3, rabbit-2) + advmod(is-3, away-6) - > nsubj(away-6, rabbit-2) )
	case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION; nsubj(is-4, rabbit-2) + advmod(is-4, away-7) + rcmod(rabbit-2, is-4) -> _predadj(rabbit-2, away-7)
	*/

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationTest[REL2][REL_ENT1] = true; param.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.disableEntity[REL1][REL_ENT1] = true; param.disableEntityUseOriginalValues[REL1][REL_ENT1] = true;
	#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
		#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
		param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_ADJECTIVE_PREDADJ;
		param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
		param.disableRelation[REL1] = true;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		/*//case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION;
		The rabbit that is 20 meters away is fat.
		nsubj(is-4, rabbit-2)
		advmod(is-4, away-7)
		rcmod(rabbit-2, is-4)
		*/
		param.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL2] = true;
		param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
		param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
		param.useRelationTest[REL3][REL_ENT2] = true; param.relationTest[REL3][REL_ENT2] = RELATION_ENTITY_BE;
		#endif
		genericDependecyRelationInterpretation(&param, REL1);
		#else
		//Case currently enabled
		param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_PREDADJ;
		param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT2;
		param.disableRelation[REL2] = true;
		#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
		/*//case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION;
		The rabbit that is 20 meters away is fat.
		nsubj(is-4, rabbit-2)
		advmod(is-4, away-7)
		rcmod(rabbit-2, is-4)
		*/
		param.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
		param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
		param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
		param.useRelationIndexTest[REL3][REL_ENT2] = true; param.relationIndexTestRelationID[REL3][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
		#endif
		genericDependecyRelationInterpretation(&param, REL1);
		#endif
	#else
		param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		param.disableRelation[REL2] = true;
		genericDependecyRelationInterpretation(&param, REL1);
	#endif

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
						{
							if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
								{//found a matching object-subject relationship

									GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

									disableEntity(oldRedundantBeEntity);

									#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
										#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
										//CASE CURRENTLY DISABLED

										#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
										//case added 15 May 2012 for GIA_RECORD_SAME_REFERENCE_SET_INFORMATION;
										The rabbit that is 20 meters away is fat.
										nsubj(is-4, rabbit-2)
										advmod(is-4, away-7)
										rcmod(rabbit-2, is-4)
										*/
										bool auxiliaryIndicatesDifferentReferenceSet = true;
										currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
											{
												if((currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex) && (currentRelationInList3->relationDependent == RELATION_ENTITY_BE))
												{
													auxiliaryIndicatesDifferentReferenceSet = false;
												}
											}
											currentRelationInList3 = currentRelationInList3->next;
										}
										currentRelationInList2->auxiliaryIndicatesDifferentReferenceSet = auxiliaryIndicatesDifferentReferenceSet;	//was currentRelationInList before 1 June 2012 code check
										#ifdef GIA_ADVANCED_REFERENCING_DEBUG
										//cout << "\t\t\t3currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
										//cout << "\t\t\t3currentRelationInList2->auxiliaryIndicatesDifferentReferenceSet = " << currentRelationInList2->auxiliaryIndicatesDifferentReferenceSet << endl;
										#endif
										#endif

										currentRelationInList2->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;

										currentRelationInList->disabled =  true;
										#else
										//CASE CURRENTLY ENABLED
										currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
										currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled =  true;
										#endif
									#else
										//CASE CURRENTLY DISABLED
										currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled = true;
									#endif
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

void redistributeStanfordRelationsCollapseSubjectAndCopGenerateAdjectivesAndAppos(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser)
{
	/*
	eg1 Kane is late. 		nsubj(late-3, Kane-1) + cop(late-3, is-2) -> _predadj(kane-1, late-3) 				[NB non-determinate of governer and dependent of subject relation; take as indicator of substance]
	eg1b Riding in the park is good. csubj(good-6, Riding-1) + cop(good-6, is-5) -> _predadj(Riding-1, good-6) 	//added 27 October 2013
	or
	eg2 She is the one.	      nsubj(one-4, She-1) + cop(one-4, is-2) + det(one-4, the-3) -> appos(She-1, one-4) 	      [NB determinate of dependent of subject relation; take as an indicator of definition]
	or
	eg3 The girl is tall.	      nsubj(tall-6, girl-2) + cop(tall-6, is-3) + det(girl-2, The-1) -> _predadj(girl-2, tall-6)      [NB non-plural and determinate of governer of subject relation; take as indicator of substance]
	or
	eg4 bikes are machines.       nsubj(machines-3, bikes-1) + cop(machines-3, are-2) -> appos(bikes-1, machines-3) 	      [NB plural and non-determinate of governer of subject relation; take as an indicator of definition]
	or
	eg5 the wheels are green.     nsubj(green-6, wheels-4) + cop(green-6, are-5) -> _predadj(wheels-4, green-6)		      [NB plural and determinate of governer of subject relation; take as indicator of substance]
	or
	eg6 That is Jim.	      nsubj(Jim-3, That-1) + cop(Jim-3, is-2) -> appos(That-1, Jim-3)
	or
	eg7 The time is 06:45.        nsubj(06:45-4, time-2) + cop(06:45-4, is-3) + det(time-2, The-1) -> appos(time-2, 06:45-4)
	*/

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COPULA;
	param.useRelationTest[REL2][REL_ENT2] = true; param.relationTest[REL2][REL_ENT2] = RELATION_ENTITY_BE;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;	//redundant test
	#ifndef GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION
		#ifndef GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD
		//currently in use
		if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			param.numberOfRelations = 2;

			GIAgenericDepRelInterpretationParameters paramA = param;
			#ifdef GIA_TRANSLATOR_SUPPORT_CSUB_AND_COP_GENERATE_ADJECTIVES
			paramA.useRelationTest[REL1][REL_ENT3] = false;
			paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypeSubjectCsubjNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_CSUBJ_NUMBER_OF_TYPES;
			#endif

			//paramA.useRelationArrayTest[REL1][REL_ENT1] = true; paramA.relationArrayTest[REL1][REL_ENT1] = featurePOSindicatesAdjectiveOrAdverbTypeArray; paramA.relationArrayTestSize[REL1][REL_ENT1] = FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_OF_TYPES; paramA.relationArrayTestSpecialCasePOStemp[REL1][REL_ENT1] = true;
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1A("stanfordPOStemp", FEATURE_POS_TAG_ADJECTIVE_JJ);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1B("stanfordPOStemp", FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1C("stanfordPOStemp", FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1D("stanfordPOStemp", FEATURE_POS_TAG_ADVERB);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1E("stanfordPOStemp", FEATURE_POS_TAG_ADVERB_COMPARATIVE);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp1F("stanfordPOStemp", FEATURE_POS_TAG_ADVERB_SUPERLATIVE);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1A);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1B);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1C);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1D);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1E);
			paramA.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp1F);
			paramA.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramA.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_PREDADJ;
			paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
			paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
			paramA.disableRelation[REL2] = true;
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			paramA.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
			paramA.useRelationTest[REL3][REL_ENT3] = true; paramA.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
			paramA.useRelationIndexTest[REL3][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramA.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
			paramA.useRelationIndexTest[REL3][REL_ENT2] = true; paramA.relationIndexTestRelationID[REL3][REL_ENT2] = REL1; paramA.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
			#endif
			if(genericDependecyRelationInterpretation(&paramA, REL1))
			{
				//cout << "passA" << endl;
			}

			GIAgenericDepRelInterpretationParameters paramB = param;
			//paramB.useRelationArrayTest[REL1][REL_ENT1] = true; paramB.relationArrayTest[REL1][REL_ENT1] = featurePOSindicatesNounTypeArray; paramB.relationArrayTestSize[REL1][REL_ENT1] = FEATURE_POS_TAG_INDICATES_NOUN_NUMBER_OF_TYPES; paramB.relationArrayTestSpecialCasePOStemp[REL1][REL_ENT1] = true;
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp2A("stanfordPOStemp", FEATURE_POS_TAG_CARDINALNUMBER_CD);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp2B("stanfordPOStemp", FEATURE_POS_TAG_NOUN_NN);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp2C("stanfordPOStemp", FEATURE_POS_TAG_NOUN_NNS);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp2D("stanfordPOStemp", FEATURE_POS_TAG_NOUN_NNP);
			GIAentityCharacteristic relationArrayTestSpecialCasePOStemp2E("stanfordPOStemp", FEATURE_POS_TAG_NOUN_NNPS);
			paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp2A);
			paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp2B);
			paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp2C);
			paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp2D);
			paramB.specialCaseCharacteristicsTestOrVector[REL1][REL_ENT1].push_back(&relationArrayTestSpecialCasePOStemp2E);
			paramB.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
			paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
			paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
			paramB.disableRelation[REL2] = true;
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			paramB.useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL1] = true;
			paramB.useRelationTest[REL3][REL_ENT3] = true; paramB.relationTest[REL3][REL_ENT3] = RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER;
			paramB.useRelationIndexTest[REL3][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramB.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
			paramB.useRelationIndexTest[REL3][REL_ENT2] = true; paramB.relationIndexTestRelationID[REL3][REL_ENT2] = REL1; paramB.relationIndexTestEntityID[REL3][REL_ENT2] = REL_ENT1;
			#endif
			if(genericDependecyRelationInterpretation(&paramB, REL1))
			{
				//cout << "passB" << endl;
			}

			//must switch subject to object;
			/*eg;
			The boat that was rowed ate a pie.
			The boat that is rowed ate a pie.
			The boat that will be rowed ate a pie.
			The boat that has been rowed ate a pie.
			The boat that was being rowed ate a pie.
			The boat that is being rowed ate a pie.
			nsubj(rowed-5, boat-2) -> dobj(rowed-5, boat-2)
			nsubj(rowed-5, boat-2) -> dobj(rowed-5, boat-2)
			nsubj(rowed-6, boat-2) -> dobj(rowed-5, boat-2)
			nsubj(rowed-6, boat-2) -> dobj(rowed-5, boat-2)
			nsubjpass(rowed-6, boat-2)	[already interpreted as obj in GIA]
			nsubjpass(rowed-6, boat-2)	[already interpreted as obj in GIA]
			*/
			GIAgenericDepRelInterpretationParameters paramC = param;
			//paramC.useRelationTest[REL1][REL_ENT1] = true; paramC.relationTest[REL1][REL_ENT1] = FEATURE_POS_TAG_VERB_VBN; paramC.relationTestSpecialCasePOStemp[REL1][REL_ENT1] = true;
			GIAentityCharacteristic relationTestSpecialCasePOStemp("stanfordPOStemp", FEATURE_POS_TAG_VERB_VBN);
			param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCasePOStemp);
			paramC.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramC.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
			if(genericDependecyRelationInterpretation(&paramC, REL1))
			{
				//cout << "passC" << endl;
			}
		}
		else
		{
			cout << "warning: redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe() executed with (NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP) - performance substantially reduced" << endl;
		}
		#else
		//not used
		//eg the girl is tall	nsubj(tall-6, girl-2) + cop(tall-6, is-3) + det(girl-2, The-1)
		//eg the wheel is green		nsubj(green-6, wheels-4) + cop(green-6, are-5)
		GIAgenericDepRelInterpretationParameters paramA = param;
		paramA.numberOfRelations = 3;
		paramA.useRelationTest[REL3][REL_ENT3] = true; paramA.relationTest[REL3][REL_ENT3] = RELATION_TYPE_DETERMINER;
		paramA.useRelationIndexTest[REL3][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramA.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
		paramA.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramA.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_PREDADJ;
		paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
		paramA.disableRelation[REL2] = true;
		genericDependecyRelationInterpretation(&paramA, REL1);

		//eg she is the one	nsubj(one-4, She-1) + cop(one-4, is-2) + det(one-4, the-3)
		GIAgenericDepRelInterpretationParameters paramB = param;
		paramB.numberOfRelations = 3;
		paramB.useRelationTest[REL3][REL_ENT3] = true; paramB.relationTest[REL3][REL_ENT3] = RELATION_TYPE_DETERMINER;
		paramB.useRelationIndexTest[REL3][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; paramA.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT1;
		paramB.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
		paramB.disableRelation[REL2] = true;
		genericDependecyRelationInterpretation(&paramB, REL1);

		GIAgenericDepRelInterpretationParameters paramC = param;
		paramB.numberOfRelations = 2;
		paramB.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
		paramB.disableRelation[REL2] = true;
		genericDependecyRelationInterpretation(&paramC, REL1);
		#endif
	#else
		//not used
		param.numberOfRelations = 2;
		param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
		param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
		param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1;
		param.disableRelation[REL2] = true;
		genericDependecyRelationInterpretation(&param, REL1);
	#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{

 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
					//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford

						if(currentRelationInList2->relationType == RELATION_TYPE_COPULA)
						{
							if(currentRelationInList2->relationDependent == RELATION_ENTITY_BE)
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
								{//found a matching object-subject relationship

									#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
									currentRelationInList->auxiliaryIndicatesDifferentReferenceSet = true;
									#ifdef GIA_ADVANCED_REFERENCING_DEBUG
									//cout << "\t\t\t4currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
									//cout << "\t\t\t4currentRelationInList->auxiliaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxiliaryIndicatesDifferentReferenceSet << endl;
									#endif
									#endif

									#ifndef GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION
										#ifndef GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD
										if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
										{
											GIAentityNode* subjectGovernorEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

											#ifdef GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_WORKAROUND_STANFORD_BUG_SINGLE_DIGIT_TIMES_MARKED_AS_ADJECTIVE
											if(subjectGovernorEntity->NERTemp == FEATURE_NER_TIME)
											{
												subjectGovernorEntity->stanfordPOStemp = FEATURE_POS_TAG_CARDINALNUMBER_CD;
											}
											#endif

											bool subjectGovernorAdjectiveOrAdvebFound = false;
											for(int i=0; i<FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_OF_TYPES; i++)
											{
												if(subjectGovernorEntity->stanfordPOStemp == featurePOSindicatesAdjectiveOrAdverbTypeArray[i])
												{
													subjectGovernorAdjectiveOrAdvebFound = true;
												}
											}
											bool subjectGovernorNounFound = false;
											for(int i=0; i<FEATURE_POS_TAG_INDICATES_NOUN_NUMBER_OF_TYPES; i++)
											{
												if(subjectGovernorEntity->stanfordPOStemp == featurePOSindicatesNounTypeArray[i])
												{
													subjectGovernorNounFound = true;
												}
											}

											if(subjectGovernorAdjectiveOrAdvebFound)
											{
												currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
												currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
												currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
												currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;

												currentRelationInList2->disabled =  true;
											}
											else if(subjectGovernorNounFound)
											{
												currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
												currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
												currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
												currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;

												currentRelationInList2->disabled =  true;
											}
											else
											{
												if(subjectGovernorEntity->stanfordPOStemp == FEATURE_POS_TAG_VBN)
												{
													//must switch subject to object;
													/*eg;
													The boat that was rowed ate a pie.
													The boat that is rowed ate a pie.
													The boat that will be rowed ate a pie.
													The boat that has been rowed ate a pie.
													The boat that was being rowed ate a pie.
													The boat that is being rowed ate a pie.
													nsubj(rowed-5, boat-2) -> dobj(rowed-5, boat-2)
													nsubj(rowed-5, boat-2) -> dobj(rowed-5, boat-2)
													nsubj(rowed-6, boat-2) -> dobj(rowed-5, boat-2)
													nsubj(rowed-6, boat-2) -> dobj(rowed-5, boat-2)
													nsubjpass(rowed-6, boat-2)	[already interpreted as obj in GIA]
													nsubjpass(rowed-6, boat-2)	[already interpreted as obj in GIA]
													*/
													currentRelationInList->relationType = RELATION_TYPE_OBJECT;
												}
											}
										}
										else
										{
											cout << "warning: redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe() executed with (NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP) - performance substantially reduced" << endl;
										}
										#else
										bool detectedDeterminateOfSubjectGoverner = false;
										bool detectedDeterminateOfSubjectDependent = false;
										bool detectedDeterminate = false;
 										GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{

											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											if(!(currentRelationInList3->disabled))
											{
											#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_DETERMINER)
												{
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
													{//found a matching object-subject relationship
														//eg she is the one	nsubj(one-4, She-1) + cop(one-4, is-2) + det(one-4, the-3)
														detectedDeterminateOfSubjectGoverner = true;
														detectedDeterminate = true;
													}
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)	//redundant test
													{//found a matching object-subject relationship
														//eg the girl is tall	nsubj(tall-6, girl-2) + cop(tall-6, is-3) + det(girl-2, The-1)
														detectedDeterminateOfSubjectDependent = true;
														detectedDeterminate = true;

														//eg the wheel is green		nsubj(green-6, wheels-4) + cop(green-6, are-5)
													}
												}
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
											}
											#endif
											currentRelationInList3 = currentRelationInList3->next;
										}


										if(detectedDeterminateOfSubjectGoverner || !detectedDeterminate)
										{

											currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
											currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
											currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
											currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;

											currentRelationInList2->disabled =  true;
										}
										if(detectedDeterminateOfSubjectDependent)
										{
											currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
											currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
											currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
											currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;

											currentRelationInList2->disabled =  true;
											/*//Not necessary; already disabled in fillGrammaticalArraysStanford;
											GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
											disableEntity(oldRedundantBeEntity);
											*/
										}
										#endif

									#else
										currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
										currentRelationInList->relationDependentIndex = currentRelationInList2->relationGovernorIndex;
										currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName;

										currentRelationInList2->disabled =  true;
									#endif
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


void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//eg	If the basket is near the house, the dog is happy. advcl(happy-12, is-4) + mark(is-4, If-1) + nsubj(is-4, basket-3) -> prep_if(happy-12, basket-3)
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.parseDisabledRelation[REL3] = true;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_BE;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL1][REL_ENT2] = REL_ENT2;
	param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL1][REL_ENT2] = REL_ENT2;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT3] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT3] = REL_ENT2;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL3; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT2;
	paramOr.disableRelation[REL2] = true;
	paramOr.disableRelation[REL3] = true;
	genericDependecyRelationInterpretation(&param, REL1);

	//eg	The accident happened as the night was falling. 	advcl(happen, fall) + mark(fall, as) -> prep_as (happen, fall)
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT3] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT3] = REL_ENT2;
	paramOr.disableRelation[REL2] = true;
	genericDependecyRelationInterpretation(&param, REL1);

	//NEW
	//eg If the cow is blue fight the turtle, else fight the pie. : csubj(fight-6, blue-5) + mark(blue-5, If-1) -> prep_if(blue-5, fight-6)
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CLAUSAL_SUBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT3] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT3] = REL_ENT2;
	paramOr.disableRelation[REL2] = true;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER)
			{

 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool partnerTypeRequiredFound = false;
						if(currentRelationInList2->relationType == RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER)
						{
							partnerTypeRequiredFound = true;
						}

						if(partnerTypeRequiredFound)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching object-subject relationship

								GIAentityNode* oldPrepositionEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

								//currentRelationInList2->disabled = true;
								//disableEntity(oldPrepositionEntity);

								string newRelationType = "";
								newRelationType = newRelationType + STANFORD_PARSER_PREPOSITION_PREPEND + oldPrepositionEntity->entityName;
								#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
								cout << "DEBUG: redistributeStanfordRelationsAdverbalClauseModifierAndComplement();" << endl;
								cout << "newRelationType = " << newRelationType << endl;
								#endif
								currentRelationInList->relationType = newRelationType;
								#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
								currentRelationInList->relationTypeIndex = oldPrepositionEntity->entityIndex;
								#endif
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


void redistributeStanfordRelationsClausalSubject(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg	What she said makes sense. 	csubj (make, say) + dobj ( said-3 , What-1 )
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_CLAUSAL_SUBJECT;
	param.useRelationIndexTest[REL2][REL_ENT2] = true; param.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_OBJECT)
			{

 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_CLAUSAL_SUBJECT)
						{
							if(currentRelationInList2->relationDependentIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-subject relationship

								currentRelationInList2->relationType = RELATION_TYPE_SUBJECT;
								currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
								currentRelationInList2->relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
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

void redistributeStanfordRelationsConjunctionAndCoordinate(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg	I eat a pie or tom rows the boat. 	cc(pie-4, or-5)  + conj(pie-4, tom-6)
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_CONJUNCT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_COORDINATION;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;

	GIAgenericDepRelInterpretationParameters paramAnd = param;
	paramAnd.useRelationTest[REL2][REL_ENT2] = true; paramAnd.relationTest[REL2][REL_ENT2] = RELATION_COORDINATION_DEPENDENT_AND;
	paramAnd.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramAnd.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_CONJUGATION_AND;
	paramAnd.disableRelation[REL2] = true;
	paramAnd.disableEntity[REL2][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramAnd, REL1);

	GIAgenericDepRelInterpretationParameters paramOr = param;
	paramOr.useRelationTest[REL2][REL_ENT2] = true; paramOr.relationTest[REL2][REL_ENT2] = RELATION_COORDINATION_DEPENDENT_OR;
	paramOr.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramOr.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_CONJUGATION_OR;
	paramOr.disableRelation[REL2] = true;
	paramOr.disableEntity[REL2][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramOr, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_CONJUNCT)
			{

 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool partnerTypeRequiredFound = false;
						if(currentRelationInList2->relationType == RELATION_TYPE_COORDINATION)
						{
							partnerTypeRequiredFound = true;
						}

						if(partnerTypeRequiredFound)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-subject relationship

								string newRelationType = "";
								GIAentityNode* coordinationDependentEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
								string coordinationDependent = coordinationDependentEntity->entityName;
								if(coordinationDependent == RELATION_COORDINATION_DEPENDENT_AND)
								{
									newRelationType = RELATION_TYPE_CONJUGATION_AND;
								}
								else if(coordinationDependent == RELATION_COORDINATION_DEPENDENT_OR)
								{
									newRelationType = RELATION_TYPE_CONJUGATION_OR;
								}
								else
								{
									cout << "error redistributeStanfordRelationsConjunctionAndCoordinate{}: unknown coordination dependent - " << coordinationDependent << endl;
								}
								currentRelationInList2->relationType = newRelationType;

								currentRelationInList->disabled = true;
								disableEntity(coordinationDependentEntity);
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

void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg	 The punter won almost $1000. 	advmod(won-3, almost-4) + pobj(almost-4, $-5) + num($-5, 1000-6)	[Relex: _obj(win[3], $[5]) + _quantity_mod($[5], almost[4])] -> _obj(win[3], $[5]) +  _quantity_mod($[5], almost[4])
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_ADVMOD;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_QUANTITY;
	param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL3][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT2;
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT2;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_QUANTITY_MOD;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL2; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2; param.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL2][REL_ENT2] = true;	//required to swap variables via redistributeRelationEntityIndexReassignmentUseOriginalValues;
	genericDependecyRelationInterpretation(&param, REL1);

#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching object-subject relationship

 								GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
								while(currentRelationInList3->next != NULL)
								{
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
									if(!(currentRelationInList3->disabled))
									{
									#endif
										if(currentRelationInList3->relationType == RELATION_TYPE_QUANTITY)
										{
											if(currentRelationInList3->relationGovernorIndex == currentRelationInList2->relationDependentIndex)
											{//found a matching object-subject relationship


												int indexOfQuantityModifier = currentRelationInList->relationDependentIndex;
												string quantityModifier = currentRelationInList->relationDependent;

												currentRelationInList->relationType = RELATION_TYPE_OBJECT;
												currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
												currentRelationInList->relationDependent = currentRelationInList2->relationDependent;

												currentRelationInList2->relationType = RELATION_TYPE_QUANTITY_MOD;
												currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
												currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;
												currentRelationInList2->relationDependentIndex = indexOfQuantityModifier;
												currentRelationInList2->relationDependent = quantityModifier;
											}
										}
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
									}
									#endif
									currentRelationInList3 = currentRelationInList3->next;
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


void redistributeStanfordRelationsGenerateMeasures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//note GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES is not supported by GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//eg1	The undergraduate student is 18 years old. 	years old - npadvmod(old, years) -> _measure_time(old[7], years[6])		   {IRRELEVANT years: <NER>NUMBER</NER>} + old: <NER>DURATION</NER>
	//eg2	The rabbit is 20 meters away.		meters away - npadvmod(away-6, meters-5) -> _measure_distance(away[6], meter[5])
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER;
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_UNKNOWN;
	genericDependecyRelationInterpretation(&param, REL1);

	//eg3 Robert ate 4 times a day.	dep(times-4, day-6) -> measure_dependency(times-4, day-6)			{Relex: _measure_per(times[4], day[6])}
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DEPENDENT;
	GIAentityCharacteristic useRedistributeSpecialCaseNERTempCheck("NERTemp", FEATURE_NER_DURATION_STRING); param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&useRedistributeSpecialCaseNERTempCheck);
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;
	genericDependecyRelationInterpretation(&param, REL1);

	/*
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
				GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
				if(dependentEntity->NERTemp == FEATURE_NER_DURATION)
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
	*/
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			//eg1	The undergraduate student is 18 years old.	years old - npadvmod(old, years) -> _measure_time(old[7], years[6])		   {IRRELEVANT years: <NER>NUMBER</NER>} + old: <NER>DURATION</NER>
			//eg2	The rabbit is 20 meters away.	meters away - npadvmod(away-6, meters-5) -> _measure_distance(away[6], meter[5])

			if(currentRelationInList->relationType == RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER)
			{
				GIAentityNode* governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

				if(governerEntity->NERTemp == FEATURE_NER_DURATION)
				{
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_UNKNOWN;
					#else
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_TIME;
					#endif
				}
				else
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_UNKNOWN;
				}
			}

			//eg3 Robert ate 4 times a day.	dep(times-4, day-6) -> measure_dependency(times-4, day-6)			{Relex: _measure_per(times[4], day[6])}

			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
				GIAentityNode* governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

				if(dependentEntity->NERTemp == FEATURE_NER_DURATION)
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;
				}
				else
				{
					//do nothing
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void redistributeStanfordRelationsPhrasalVerbParticle(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

	#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_TEMPORAL_MODIFIER
	//The disaster happened over night.   prt(happened-3, over-4) + tmod(happened-3, night-5) -> over(happened-3, night-5)
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramA.numberOfRelations = 2;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PHRASAL_VERB_PARTICLE;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_TEMPORAL_MODIFIER;
	paramA.useRelationIndexTest[REL2][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramA.disableRelation[REL1] = true;
	paramA.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2;
	genericDependecyRelationInterpretation(&paramA, REL1);
	#endif

	#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_OBJECT_OF_PREPOSITION
	//What does the red laser work on?	prt(work-6, on-7) + pobj(on-7, What-1) -> on(work-6, What-1)	//case added 7 August 2012b
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramB.numberOfRelations = 2;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PHRASAL_VERB_PARTICLE;
	paramB.useRelationTest[REL2][REL_ENT3] = true; paramB.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
	paramB.useRelationIndexTest[REL2][REL_ENT1] = true; paramB.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramB.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramB.disableRelation[REL1] = true;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT2;
	paramB.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT1;
	genericDependecyRelationInterpretation(&paramB, REL1);
	#endif

	#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE
	//eg They shut down the station. 	prt(shut, down)
	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramC.numberOfRelations = 1;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PHRASAL_VERB_PARTICLE;
	paramC.disableEntity[REL1][REL_ENT2] = true;
	paramC.disableRelation[REL1] = true;	//added 11 July 2013 - check this
	paramC.useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL1][REL_ENT1] = true;
		paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][0] = REL1; paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][0] = REL_ENT1;
		paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL1][REL_ENT1][1] = REL1; paramC.redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL1][REL_ENT1][1] = REL_ENT2;
	genericDependecyRelationInterpretation(&paramC, REL1);
	#endif
#else

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PHRASAL_VERB_PARTICLE)
			{
				bool foundTemporalModifierOrObjectOfPreposition = false;

				#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_TEMPORAL_MODIFIER
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_TEMPORAL_MODIFIER)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching relationship

								//The disaster happened over night.   prt(happened-3, over-4) + tmod(happened-3, night-5) -> over(happened-3, night-5)

								currentRelationInList->disabled = true;
								GIAentityNode* oldPreposition = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
								string newPrepositionName = "";
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + currentRelationInList->relationDependent;	//oldPreposition->entityName
								currentRelationInList2->relationType = newPrepositionName;
								#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
								currentRelationInList2->relationTypeIndex = currentRelationInList->relationDependentIndex;
								#endif
								foundTemporalModifierOrObjectOfPreposition = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
				#endif

				#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_OBJECT_OF_PREPOSITION
				//case added 7 August 2012b
 				currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching relationship

								//What does the red laser work on?	prt(work-6, on-7) + pobj(on-7, What-1) -> on(work-6, What-1)	//case added 7 August 2012b

								currentRelationInList->disabled = true;
								GIAentityNode* oldPreposition = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
								string newPrepositionName = "";
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + currentRelationInList->relationDependent;	//oldPreposition->entityName
								currentRelationInList2->relationType = newPrepositionName;
								#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
								currentRelationInList2->relationTypeIndex = currentRelationInList->relationDependentIndex;
								#endif
								currentRelationInList2->relationGovernor =  currentRelationInList->relationGovernor;
								currentRelationInList2->relationGovernorIndex = currentRelationInList->relationGovernorIndex;

								foundTemporalModifierOrObjectOfPreposition = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
				#endif


				#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE
				if(!foundTemporalModifierOrObjectOfPreposition)
				{
					if(!(currentRelationInList->disabled))	//added 3 June 2012
					{//do not parse it again, in the case it has already been parsed in redistributeStanfordRelationsMultiwordPreposition()

						//eg They shut down the station. 	prt(shut, down)

						GIAentityNode* governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAentityNode* dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
						governerEntity->entityName = governerEntity->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + dependentEntity->entityName;

						disableEntity(dependentEntity);

						//currentRelationInList->disabled = true;
						//currentRelationInList->relationGovernor = governerEntity->entityName;
					}
				}
				#endif

			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}


void redistributeStanfordRelationsCreateQueryVars(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[])
{
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10a; redistribute Stanford Relations - Create Query Vars (eg interpret 'who is that' / 'what is the time.'  attr(is-2, Who-1) / attr(is-2, What-1)" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsWhoWhat(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10b; redistribute Stanford Relations - Create Query Vars (eg interpret 'how much'/'how many' advmod(much-2, How-1) amod(milk-3, much-2))" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10c; redistribute Stanford Relations - Create Query Vars (eg interpret 'which' det(house-2, Which-1) )" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsWhich(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10d; redistribute Stanford Relations - Create Query Vars (eg interpret how/when/where/why advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) / advmod(fall-5, Why-1) )" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10e; redistribute Stanford Relations - Create Query Vars (eg interpret 'what is...'  _obj(enable[5], what[1])" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsWhat(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	/*
	//this is now done at the beginning of GIAtranslatorRedistributeRelationsStanford.cpp
	#ifdef GIA_TRANSLATOR_DEBUG
	//added 28 October 2012
	cout << "pass 1z10f; redistribute Stanford Relations - query variables connected to action-preposition-action (eg interpret 'Where are apples used for eating?'  _%atLocation(use[4], _$qVar[1]) + prepc_for(use[4], eat[6]) -> _%atLocation(eat[6], _$qVar[1]) )" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	*/
}

void redistributeStanfordRelationsCreateQueryVarsWhoWhat(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[])
{
	//interpret; 'Who is that?' / 'What is the time?'  attr(is-2, What-1) + nsubj(is-2, time-4) -> appos(time-4, _$qVar-1)   /   attr(is-2, Who-1) + nsubj(is-2, that-3) -> appos(That-3, _$qVar-1)	[NB _$qVar can be switched in both cases with respect to GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY]
	//interpret; 'Who rode the bike?' / 'What broke the glass?' -> nsubj(rode-2, Who-1) -> nsubj(rode-2, _$qVar-1)  /  nsubj(broke-2, What-1) -> nsubj(broke-2, _$qVar-1) [added 7 August 2012]
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//interpret; 'Who rode the bike?' / 'What broke the glass?' -> nsubj(rode-2, Who-1) -> nsubj(rode-2, _$qVar) / nsubj(broke-2, What-1) -> nsubj(broke-2, _$qVar) [added 7 August 2012]
	GIAgenericDepRelInterpretationParameters paramNsubj(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramNsubj.numberOfRelations = 1;
	paramNsubj.useRelationTest[REL1][REL_ENT3] = true; paramNsubj.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramNsubj.useRedistributeRelationEntityReassignment[REL1][REL_ENT2] = true; paramNsubj.redistributeRelationEntityReassignment[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
	GIAgenericDepRelInterpretationParameters paramNsubjWho = paramNsubj;
	paramNsubjWho.useRelationTest[REL1][REL_ENT2] = true; paramNsubjWho.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHO;
	#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
	//paramNsubjWho.useRedistributeSpecialCaseIsNameQueryAssignment[REL1][REL_ENT2] = true;
	GIAentityCharacteristic useRedistributeSpecialCaseIsNameQueryAssignment("isNameQuery", "true");
	paramNsubjWho.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&useRedistributeSpecialCaseIsNameQueryAssignment);
	#endif
	genericDependecyRelationInterpretation(&paramNsubjWho, REL1);

	GIAgenericDepRelInterpretationParameters paramNsubjWhat = paramNsubj;
	paramNsubjWhat.useRelationTest[REL1][REL_ENT2] = true; paramNsubjWhat.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHAT;
	genericDependecyRelationInterpretation(&paramNsubjWhat, REL1);

	//interpret; 'who is that' / 'what is the time.'  attr(is-2, What-1) + nsubj(is-2, time-4) -> appos(time-4, _$qVar-1)   /   attr(is-2, Who-1) + nsubj(is-2, that-3) -> appos(That-3, _$qVar-1)	[NB _$qVar can be switched in both cases with respect to GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY]
	GIAgenericDepRelInterpretationParameters paramNsubjAndAttr(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramNsubjAndAttr.numberOfRelations = 2;
	paramNsubjAndAttr.useRelationTest[REL1][REL_ENT3] = true; paramNsubjAndAttr.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramNsubjAndAttr.useRelationTest[REL2][REL_ENT3] = true; paramNsubjAndAttr.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ATTRIBUTIVE;
	paramNsubjAndAttr.useRelationIndexTest[REL2][REL_ENT1] = true; paramNsubjAndAttr.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramNsubjAndAttr.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	paramNsubjAndAttr.useRelationTest[REL1][REL_ENT1] = true; paramNsubjAndAttr.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	paramNsubjAndAttr.useRelationTest[REL2][REL_ENT1] = true; paramNsubjAndAttr.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
	paramNsubjAndAttr.disableRelation[REL1] = true;
	paramNsubjAndAttr.disableEntity[REL1][REL_ENT1] = true;
	paramNsubjAndAttr.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramNsubjAndAttr.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramNsubjAndAttr.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramNsubjAndAttr.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; paramNsubjAndAttr.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramNsubjAndAttr.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; paramNsubjAndAttr.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
	#else
	paramNsubjAndAttr.disableRelation[REL2] = true;
	paramNsubjAndAttr.disableEntity[REL1][REL_ENT1] = true;	paramNsubjAndAttr.disableEntityUseOriginalValues[REL1][REL_ENT1] = true;
	paramNsubjAndAttr.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramNsubjAndAttr.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramNsubjAndAttr.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramNsubjAndAttr.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL2; paramNsubjAndAttr.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
	paramNsubjAndAttr.useRedistributeRelationEntityReassignment[REL1][REL_ENT1] = true; paramNsubjAndAttr.redistributeRelationEntityReassignment[REL1][REL_ENT1] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
	#endif
	GIAgenericDepRelInterpretationParameters paramNsubjAndAttrWho = paramNsubjAndAttr;
	paramNsubjAndAttrWho.useRelationTest[REL2][REL_ENT2] = true; paramNsubjAndAttrWho.relationTest[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHO;
	#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
	#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
	//paramNsubjAndAttrWho.useRedistributeSpecialCaseIsNameQueryAssignment[REL2][REL_ENT2] = true;
	paramNsubjAndAttrWho.specialCaseCharacteristicsAssignmentVector[REL2][REL_ENT2].push_back(&useRedistributeSpecialCaseIsNameQueryAssignment);
	#else
	//paramNsubjAndAttrWho.useRedistributeSpecialCaseIsNameQueryAssignment[REL2][REL_ENT1] = true;
	paramNsubjAndAttrWho.specialCaseCharacteristicsAssignmentVector[REL2][REL_ENT1].push_back(&useRedistributeSpecialCaseIsNameQueryAssignment);
	#endif
	#endif
	genericDependecyRelationInterpretation(&paramNsubjAndAttrWho, REL1);

	GIAgenericDepRelInterpretationParameters paramNsubjAndAttrWhat = paramNsubjAndAttr;
	paramNsubjAndAttrWhat.useRelationTest[REL2][REL_ENT2] = true; paramNsubjAndAttrWhat.relationTest[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHAT;
	genericDependecyRelationInterpretation(&paramNsubjAndAttrWhat, REL1);
#else

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				bool queryWhoWhatRelationDependentFound = false;
				for(int i=0; i<FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES; i++)
				{
					if(currentRelationInList->relationDependent == featureQueryWordWhoWhatNameArray[i])
					{
						queryWhoWhatRelationDependentFound = true;
					}
				}
				if(queryWhoWhatRelationDependentFound)
				{
					//interpret; 'Who rode the bike?' / 'What broke the glass?' -> nsubj(rode-2, Who-1) -> nsubj(rode-2, _$qVar) / nsubj(broke-2, What-1) -> nsubj(broke-2, _$qVar) [added 7 August 2012]
					GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
					#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
					if(currentRelationInList->relationDependent == REFERENCE_TYPE_QUESTION_QUERY_WHO)
					{
						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isNameQuery = true;
					}
					#endif
					currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
				}
				else
				{
					bool attributiveRelationFound = false;

					//now find the associated relation..
 					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_ATTRIBUTIVE)
							{
								bool queryWhoWhatRelationDependentFound2 = false;
								for(int i=0; i<FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES; i++)
								{
									if(currentRelationInList2->relationDependent == featureQueryWordWhoWhatNameArray[i])
									{
										queryWhoWhatRelationDependentFound2 = true;
									}
								}
								if(queryWhoWhatRelationDependentFound2)
								{
									if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
									{
										if((currentRelationInList2->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE))
										{//found a matching relationship

											//interpret 'what is the time.' nsubj(is-2, time-4) + attr(is-2, What-1)  -> appos(time-4, _$qVar-1),  'who is that'  nsubj(is-2, that-3) attr(is-2, Who-1)   -> appos(That-3, _$qVar-1)	[NB _$qVar can be switched in both cases with respect to GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY]

											#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
											cout << "redistributeStanfordRelationsCreateQueryVarsWhoWhat" << endl;
											#endif

										#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY

											currentRelationInList->disabled =  true;
											GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
											disableEntity(oldRedundantBeEntity);

											currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
											currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
											currentRelationInList2->relationGovernor = currentRelationInList->relationDependent;

											GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
											#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
											if(currentRelationInList2->relationDependent == REFERENCE_TYPE_QUESTION_QUERY_WHO)
											{
												GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->isNameQuery = true;
											}
											#endif
											currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;

										#else

											currentRelationInList2->disabled =  true;
											GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
											disableEntity(oldRedundantBeEntity);

											currentRelationInList->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
											currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;	//"what"/"who"
											currentRelationInList->relationGovernor = currentRelationInList2->relationDependent;

											GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What"/"Who" to _$qVar
											#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
											if(currentRelationInList2->relationDependent == REFERENCE_TYPE_QUESTION_QUERY_WHO)
											{
												GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->isNameQuery = true;
											}
											#endif
											currentRelationInList->relationGovernor = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;

										#endif

										}
									}
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}

				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif

	//added 3 July 2013
	//interpret; 'What time is it?'  attr(is-3, time-2) + nsubj(is-3, it-4) + det(time-2, What-1) -> appos(time-4, _$qVar-1)
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters paramNsubjAndAttrAndDet(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramNsubjAndAttrAndDet.numberOfRelations = 3;
	paramNsubjAndAttrAndDet.useRelationTest[REL1][REL_ENT3] = true; paramNsubjAndAttrAndDet.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramNsubjAndAttrAndDet.useRelationTest[REL2][REL_ENT3] = true; paramNsubjAndAttrAndDet.relationTest[REL2][REL_ENT3] = RELATION_TYPE_ATTRIBUTIVE;
	paramNsubjAndAttrAndDet.useRelationTest[REL3][REL_ENT3] = true; paramNsubjAndAttrAndDet.relationTest[REL3][REL_ENT3] = RELATION_TYPE_DETERMINER;
	paramNsubjAndAttrAndDet.useRelationArrayTest[REL3][REL_ENT2] = true; paramNsubjAndAttrAndDet.relationArrayTest[REL3][REL_ENT2] = featureQueryWordWhatNameArray; paramNsubjAndAttrAndDet.relationArrayTestSize[REL3][REL_ENT2] = FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES;
	paramNsubjAndAttrAndDet.useRelationTest[REL1][REL_ENT1] = true; paramNsubjAndAttrAndDet.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	paramNsubjAndAttrAndDet.useRelationTest[REL2][REL_ENT1] = true; paramNsubjAndAttrAndDet.relationTest[REL2][REL_ENT1] = RELATION_ENTITY_BE;
	paramNsubjAndAttrAndDet.disableRelation[REL1] = true;
	paramNsubjAndAttrAndDet.disableEntity[REL1][REL_ENT1] = true;	//oldRedundantBeEntity
	paramNsubjAndAttrAndDet.disableEntity[REL1][REL_ENT2] = true;	//oldRedundantItEntity
	paramNsubjAndAttrAndDet.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; paramNsubjAndAttrAndDet.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramNsubjAndAttrAndDet.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; paramNsubjAndAttrAndDet.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL2; paramNsubjAndAttrAndDet.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramNsubjAndAttrAndDet.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; paramNsubjAndAttrAndDet.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL3; paramNsubjAndAttrAndDet.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;
	paramNsubjAndAttrAndDet.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; paramNsubjAndAttrAndDet.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//CHECK THIS overwrite
	if(genericDependecyRelationInterpretation(&paramNsubjAndAttrAndDet, REL1))
	{
		featureArrayTemp[paramNsubjAndAttrAndDet.relationEntityIndexFinalResult[REL2][REL_ENT1]]->grammaticalIsDefinite = true;	//required such that "time" is treated the same way as when generated by "what is the time?"
	}
#else
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				//now find the associated relation..
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_ATTRIBUTIVE)
						{
							//now find the associated relation..
 							GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;

							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								if(!(currentRelationInList3->disabled))
								{
								#endif
									if(currentRelationInList3->relationType == RELATION_TYPE_DETERMINER)
									{

										bool queryWhatRelationDependentFound = false;
										for(int i=0; i<FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES; i++)
										{
											if(currentRelationInList3->relationDependent == featureQueryWordWhatNameArray[i])
											{
												queryWhatRelationDependentFound = true;
											}
										}
										if(queryWhatRelationDependentFound)
										{
											if((currentRelationInList2->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE))
											{
												//interpret; 'what time is it.'  attr(is-3, time-2) + nsubj(is-3, it-4) + det(time-2, What-1) -> appos(time-4, _$qVar-1)

												currentRelationInList->disabled =  true;
												GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
												disableEntity(oldRedundantBeEntity);
												GIAentityNode* oldRedundantItEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
												disableEntity(oldRedundantItEntity);

												currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
												currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
												currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;

												currentRelationInList2->relationDependentIndex =  currentRelationInList3->relationDependentIndex;
												currentRelationInList2->relationDependent =  currentRelationInList3->relationDependent;

												GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
												currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;

												featureArrayTemp[currentRelationInList2->relationGovernorIndex]->grammaticalIsDefinite = true;	//required such that "time" is treated the same way as when generated by "what is the time?"
											}
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								}
								#endif
								currentRelationInList3 = currentRelationInList3->next;
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



void redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//Create Query Vars (eg interpret 'how much'/'how many' advmod(much-2, How-1) + amod(milk-3, much-2) -> _quantity(milk[3], _$qVar[2]) )
	//eg1 How much milk should he buy?
	//eg2 How many apples should he buy?
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = featureQueryHowMuchFirstRelationNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES;
	param.useRelationArrayTest[REL1][REL_ENT1] = true; param.relationArrayTest[REL1][REL_ENT1] = featureQueryWordMuchManyNameArray; param.relationArrayTestSize[REL1][REL_ENT1] = FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES;
	param.useRelationArrayTest[REL1][REL_ENT2] = true; param.relationArrayTest[REL1][REL_ENT2] = featureQueryWordHowNameArray; param.relationArrayTestSize[REL1][REL_ENT2] = FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES;
	param.useRelationArrayTest[REL2][REL_ENT3] = true; param.relationArrayTest[REL2][REL_ENT3] = featureQueryHowMuchSecondRelationNameArray; param.relationArrayTestSize[REL2][REL_ENT3] = FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL2][REL_ENT2] = true; param.relationIndexTestRelationID[REL2][REL_ENT2] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT2] = REL_ENT1;
	param.disableRelation[REL1] = true;
	param.disableEntity[REL1][REL_ENT2] = true;	//disable how
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_QUANTITY;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT2] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "much" to _$qVar
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool queryHowMuchFirstRelationFound = false;
			bool queryHowMuchFirstRelationGovernorFound = false;
			bool queryHowMuchFirstRelationDependentFound = false;
			for(int i=0; i<FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == featureQueryHowMuchFirstRelationNameArray[i])
				{
					#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
					//cout << "queryHowMuchFirstRelationFound = " << endl;
					#endif
					queryHowMuchFirstRelationFound = true;
				}
			}
			for(int i=0; i<FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationGovernor == featureQueryWordMuchManyNameArray[i])
				{
					#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
					//cout << "queryHowMuchFirstRelationGovernorFound " << endl;
					#endif
					queryHowMuchFirstRelationGovernorFound = true;
				}
			}
			for(int i=0; i<FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationDependent == featureQueryWordHowNameArray[i])
				{
					#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
					//cout << "queryHowMuchFirstRelationDependentFound = " << endl;
					#endif
					queryHowMuchFirstRelationDependentFound = true;
				}
			}

			if(queryHowMuchFirstRelationFound && queryHowMuchFirstRelationGovernorFound && queryHowMuchFirstRelationDependentFound)
			{
				#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
				//cout << "(queryHowMuchFirstRelationFound && queryHowMuchFirstRelationGovernorFound && queryHowMuchFirstRelationDependentFound) = " << queryHowMuchFirstRelationDependentFound << endl;
				#endif

				//now find the associated relation..
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif

						bool queryHowMuchSecondRelationFound = false;
						for(int i=0; i<FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == featureQueryHowMuchSecondRelationNameArray[i])
							{
								queryHowMuchSecondRelationFound = true;
							}
						}
						if(queryHowMuchSecondRelationFound)
						{
							if(currentRelationInList2->relationDependentIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching relationship
								#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
								cout << "redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany" << endl;
								#endif

								currentRelationInList->disabled =  true;
								GIAentityNode* oldRedundantHowEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];	//disable how
								disableEntity(oldRedundantHowEntity);

								currentRelationInList2->relationType = RELATION_TYPE_QUANTITY;
								currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
								GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "much" to _$qVar
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

void redistributeStanfordRelationsCreateQueryVarsWhich(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//nothing is required here: see identifyComparisonVariableAlternateMethod()
}

void redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//interpret  'How did the disaster happen?' / 'When should they leave?' / 'Where is the ball?' / 'Why does the star fall?' --> advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) [+ nsubj(is-2, ball-4)] / advmod(fall-5, Why-1) -> how(happen[5], _$qVar[1]) / _%atTime(leave[4], _$qVar[1]) / _%atLocation(ball[4], _$qVar[1]) / _%because(fall[5], _$qVar[1])
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//interpret  'How did the disaster happen?' / 'When should they leave?' / 'Where is the ball?' / 'Why does the star fall?'	advmod(happen-5, How-1) / [advmod(leave-4, When-1)?] / advmod(is-2, Where-1) / advmod(fall-5, Why-1) -> how(happen[5], _$qVar[1]) / _%atTime(leave[4], _$qVar[1]) / [_%atLocation(ball[4], _$qVar[1])?] / _%because(fall[5], _$qVar[1])
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = featureQueryHowWhenWhereWhyRelationNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES;
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "how"/"when"/"why" to _$qVar

	GIAgenericDepRelInterpretationParameters paramHow = param;
	paramHow.useRelationTest[REL1][REL_ENT2] = true; paramHow.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_HOW;
	paramHow.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramHow.redistributeRelationEntityReassignment[REL1][REL_ENT3] = REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW;
	genericDependecyRelationInterpretation(&paramHow, REL1);

	GIAgenericDepRelInterpretationParameters paramWhen = param;
	paramWhen.useRelationTest[REL1][REL_ENT2] = true; paramWhen.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHEN;
	paramWhen.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramWhen.redistributeRelationEntityReassignment[REL1][REL_ENT3] = REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN;
	genericDependecyRelationInterpretation(&paramWhen, REL1);

	GIAgenericDepRelInterpretationParameters paramWhere = param;
	paramWhere.useRelationTest[REL1][REL_ENT2] = true; paramWhere.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHERE;
	paramWhere.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramWhere.redistributeRelationEntityReassignment[REL1][REL_ENT3] = REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE;
	genericDependecyRelationInterpretation(&paramWhere, REL1);

	GIAgenericDepRelInterpretationParameters paramWhy = param;
	paramWhy.useRelationTest[REL1][REL_ENT2] = true; paramWhy.relationTest[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_QUERY_WHY;
	paramWhy.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramWhy.redistributeRelationEntityReassignment[REL1][REL_ENT3] = REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY;
	genericDependecyRelationInterpretation(&paramWhy, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		//#endif
			//bool queryHowWhenWhereWhyRelationFound = true;

			bool queryHowWhenWhereWhyRelationFound = false;
			for(int i=0; i<FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == featureQueryHowWhenWhereWhyRelationNameArray[i])
				{
					queryHowWhenWhereWhyRelationFound = true;
				}
			}
			bool queryHowWhenWhereWhyRelationDependentFound = false;
			int queryWordHowWhenWhereWhyFoundIndex;
			for(int queryWordHowWhenWhereWhyIndex=0; queryWordHowWhenWhereWhyIndex<FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES; queryWordHowWhenWhereWhyIndex++)
			{
				if(currentRelationInList->relationDependent == featureQueryWordHowWhenWhereWhyNameArray[queryWordHowWhenWhereWhyIndex])
				{
					queryWordHowWhenWhereWhyFoundIndex = queryWordHowWhenWhereWhyIndex;
					queryHowWhenWhereWhyRelationDependentFound = true;
				}
			}
			if(queryHowWhenWhereWhyRelationFound && queryHowWhenWhereWhyRelationDependentFound)
			{
				/*
				bool queryHowWhenWhereWhySpecialCaseRelationDependentFound = false;
				int queryWordHowWhenWhereWhySpecialCaseFoundIndex=0;
				for(int queryWordHowWhenWhereWhySpecialCaseIndex=0; queryWordHowWhenWhereWhySpecialCaseIndex<FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES; queryWordHowWhenWhereWhySpecialCaseIndex++)
				{
					if(currentRelationInList->relationDependent == featureQueryWordHowWhenWhereWhySpecialCaseNameArray[queryWordHowWhenWhereWhySpecialCaseIndex])
					{
						queryHowWhenWhereWhySpecialCaseRelationDependentFound = true;	//where
						queryWordHowWhenWhereWhySpecialCaseFoundIndex = queryWordHowWhenWhereWhySpecialCaseIndex;
					}
				}

				bool foundMatchForSpecialCase = false;
				if(queryHowWhenWhereWhySpecialCaseRelationDependentFound)
				{//perform special case REFERENCE_TYPE_QUESTION_QUERY_WHERE:		//This should not be required becuase advmod(is-2, Where-1), nsubj(is-2, ball-4)  should already be collapsed into
					//where

					//eg advmod(is-2, Where-1), nsubj(is-2, ball-4) -> _%atLocation(ball[4], _$qVar[1])

					//now find the associated relation..
 					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						if(!(currentRelationInList2->disabled))
						{
						#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
								{
									if((currentRelationInList2->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE))
									{//found a matching relationship

										foundMatchForSpecialCase = true;

										currentRelationInList2->disabled =  true;
										GIAentityNode* oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];	//disable is
										disableEntity(oldRedundantBeEntity);

										currentRelationInList->relationType = featureQueryWordHowWhenWhereWhySpecialCaseCrossReferenceQueryVariableNameArray[queryWordHowWhenWhereWhySpecialCaseFoundIndex];		//_%atLocation
										currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationGovernor = currentRelationInList2->relationDependent;

										GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "where" to _$qVar
										currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
									}
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}


				if(!queryHowWhenWhereWhySpecialCaseRelationDependentFound || !foundMatchForSpecialCase)
				{//how, when, why
				*/
				//the above commented out code is not required, because _predadj should be generated in this case in accordance with; redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe{}: advmod(is-3, where-6) / nsubj(is-3, rabbit-2) - > _predadj(rabbit-2, where-6)

					/* no longer required as GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD has been used here
					if(currentRelationInList->relationGovernor != RELATION_ENTITY_BE)
					{//check is used to prevent a situation in the case a disabled relation has been used [NB should really utilise GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD here...]
					*/
					 	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
						cout << "redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy: " << currentRelationInList->relationDependent << endl;
						#endif

						//interpret  'How did the disaster happen?' / 'When should they leave?' / 'Where is the ball?' / 'Why does the star fall?' advmod(happen-5, How-1) / [advmod(leave-4, When-1)?] / advmod(is-2, Where-1) / advmod(fall-5, Why-1) -> how(happen[5], _$qVar[1]) / _%atTime(leave[4], _$qVar[1]) / [_%atLocation(ball[4], _$qVar[1])?] / _%because(fall[5], _$qVar[1])
						//????  interpret 'Who is that?' / 'What is the time?' ?????

						currentRelationInList->relationType = featureQueryWordHowWhenWhereWhyCrossReferenceQueryVariableNameArray[queryWordHowWhenWhereWhyFoundIndex];		// prep_how / _%atTime / _%because

						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "how"/"when"/"why" to _$qVar
						currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
					/*
					}
					else
					{

						//cout << "redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy{} error: (!queryHowWhenWhereWhySpecialCaseRelationDependentFound || !foundMatchForSpecialCase) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE)" << endl;
						//cout << "relationType = " << currentRelationInList->relationType << endl;
						//cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
						//cout << "relationDependent = " <<currentRelationInList->relationDependent << endl;

					}
					*/
				/*
				}
				*/

			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}




void redistributeStanfordRelationsCreateQueryVarsWhat(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//interpret 'what is...'  _obj(enable[5], what[1]) ->  _obj(enable[5], _$qVar[1])
	/* e.g.
	What is the Co-cart designed to do?
	What are the patent claims on?
	What is the name of the red dog near the farm?
	What is the dog's name?
	What is the number of the red dogs near the farm?
	What she said makes sense.
	What does the red laser work on?
	What is the time?
	What broke the glass?
	What is wood used in the delivering of?
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DETERMINER; param.relationTestIsNegative[REL1][REL_ENT3] = true;	//OR if(GIAentityNodeArray[currentRelationInList->relationDependentIndex]->stanfordPOStemp != FEATURE_POS_TAG_WH_DETERMINER_WDT)	//to distinguish standard what queries from equivalent which queries eg "what time is it?"
	param.useRelationArrayTest[REL1][REL_ENT2] = true; param.relationArrayTest[REL1][REL_ENT2] = featureQueryWordWhatNameArray; param.relationArrayTestSize[REL1][REL_ENT2] = FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES;
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT2] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
	genericDependecyRelationInterpretation(&param, REL1);
#else

	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		/*
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
		*/
		//if(currentRelationInList->relationType != RELATION_TYPE_DETERMINER)
		if(GIAentityNodeArray[currentRelationInList->relationDependentIndex]->stanfordPOStemp != FEATURE_POS_TAG_WH_DETERMINER_WDT)	//to distinguish standard what queries from equivalent which queries eg "what time is it?"
		{
			bool queryWhatRelationDependentFound = false;
			for(int i=0; i<FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationDependent == featureQueryWordWhatNameArray[i])
				{
					queryWhatRelationDependentFound = true;
				}
			}
			if(queryWhatRelationDependentFound)
			{
				currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
				GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "What" to _$qVar
			}
		}
		/*
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		*/
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void redistributeStanfordRelationsPartmod(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle)
	/*
		Truffles picked during the spring are tasty.
		prep_during(pick, spring)
		nsubj(tasty, truffle)
		partmod(truffle, pick)
		cop(tasty, be)
		->
		during(pick, spring)
		_predadj(truffle, tasty)
		_obj(pick, truffle)

		An elevator governor rectifying a chicken.
		det(governor-3, An-1)
		nn(governor-3, elevator-2)
		root(ROOT-0, governor-3)
		partmod(governor-3, rectifying-4)
		det(chicken-6, a-5)
		dobj(rectifying-4, chicken-6)
		->
		_subj(governor-3, rectifying-4)

	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_PARTMOD_DEAL_WITH_PROGRESSIVE_ANOMALY
	//An elevator governor rectifying a chicken. partmod(governor-3, rectifying-4) + dobj(rectifying-4, chicken-6) -> _subj(governor-3, rectifying-4) + dobj(rectifying-4, chicken-6)
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramA.numberOfRelations = 2;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PARTICIPIAL_MODIFIER;
	paramA.useRelationTest[REL2][REL_ENT3] = true; paramA.relationTest[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramA.useRelationIndexTest[REL2][REL_ENT1] = true; paramA.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; paramA.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT2;
	paramA.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramA.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT;
	paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
	paramA.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramA.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL1; paramA.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1; paramA.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramA, REL1);
	#endif
	//Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle)
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramB.numberOfRelations = 1;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PARTICIPIAL_MODIFIER;
	paramB.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; paramB.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2;
	paramB.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; paramB.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL1; paramB.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1; paramB.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramB, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PARTICIPIAL_MODIFIER)
			{
				#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_PARTMOD_DEAL_WITH_PROGRESSIVE_ANOMALY
				if(GIAentityNodeArray[currentRelationInList->relationDependentIndex]->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE])
				{
					//Eg An elevator governor rectifying/comprising a chicken.
					currentRelationInList->relationType = RELATION_TYPE_SUBJECT;
				}
				else
				{
					currentRelationInList->relationType = RELATION_TYPE_OBJECT;
				}
				#else
				currentRelationInList->relationType = RELATION_TYPE_OBJECT;
				#endif

				//now switch governor and dependent;
				int tempIndex = currentRelationInList->relationGovernorIndex;
				string tempName = currentRelationInList->relationGovernor;
				currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
				currentRelationInList->relationGovernor = currentRelationInList->relationDependent;
				currentRelationInList->relationDependentIndex = tempIndex;
				currentRelationInList->relationDependent = tempName;
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}



#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
//Added 28 October 2012b
void redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser, GIAfeature* featureArrayTemp[])
{
	//eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PREPOSITION_OF;
	GIAentityCharacteristic relationTestSpecialCaseContinousVerbA("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_VERB_STRING);
	GIAentityCharacteristic relationTestSpecialCaseContinousVerbB("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseContinousVerbA);
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseContinousVerbB);
	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
	param.disableEntity[REL1][REL_ENT3] = true;	//added 2i4b
	#endif
	genericDependecyRelationInterpretation(&param, REL1);

	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
	cout << "GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION" << endl;
	#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif

			int actionOrSubstanceIndex = currentRelationInList->relationGovernorIndex;
			int actionOrSubstanceConditionIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAentityNode* actionOrSubstanceEntity = GIAentityNodeArray[actionOrSubstanceIndex];
			GIAentityNode* actionOrSubstanceConditionEntity = GIAentityNodeArray[actionOrSubstanceConditionIndex];

			bool prepositionFound = false;
			if(convertPrepositionToRelex(&relationType, &prepositionFound) == RELATION_TYPE_PREPOSITION_OF)
			{
				//cout << "actionOrSubstanceEntity->stanfordPOStemp = " << actionOrSubstanceEntity->stanfordPOStemp << endl;
				//cout << "actionOrSubstanceEntity->grammaticalWordTypeTemp = " << actionOrSubstanceEntity->grammaticalWordTypeTemp << endl;
				#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
				/*
				if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
				{//to work effectively/best, this function requires a) GIA_NLP_PARSER_STANFORD_CORENLP as NLPfeatureParser
					#ifndef STANFORD_PARSER_USE_POS_TAGS
					//to work effectively/best, this function requires b) Stanford CoreNLP as pos tagger (ie it is incompatible with STANFORD_PARSER_USE_POS_TAGS, as stanford Parser tags a lot more -ing words as NN [rather than VBG] as compared to Stanford CoreNLP)
				*/
				if(correctVerbPOStagAndLemma(actionOrSubstanceEntity, featureArrayTemp[actionOrSubstanceIndex]))
				{
					currentRelationInList->relationGovernor = actionOrSubstanceEntity->entityName;
				}
				/*
					#endif
				}
				*/
				#endif
				//if(actionOrSubstanceEntity->stanfordPOStemp == FEATURE_POS_TAG_VBG)
				if((actionOrSubstanceEntity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && (actionOrSubstanceEntity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true))
				{
					currentRelationInList->relationType = RELATION_TYPE_OBJECT;
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

#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
void redistributeStanfordRelationsExpletives(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	//eg 'There is a place that we go.' _expl(be[2], there[1]) + _subj(be[2], place[4]) + _subj(go[7], we[6]) [IRRELEVANT] + _obj(be[2], go[7]) -> _subj(go[7], we[6]) + _obj(go[7], place[4])
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 3;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_EXPLETIVE;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationTest[REL1][REL_ENT1] = true; param.relationTest[REL1][REL_ENT1] = RELATION_ENTITY_BE;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL3; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL3; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.disableRelation[REL1] = true;
	param.disableRelation[REL3] = true;
	param.disableEntity[REL1][REL_ENT1] = true;
	param.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
						{
 							GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								if(!(currentRelationInList3->disabled))
								{
								#endif
									if(currentRelationInList3->relationType == RELATION_TYPE_OBJECT)
									{
										if(currentRelationInList->relationGovernor == RELATION_ENTITY_BE)
										{
											if(currentRelationInList->relationGovernorIndex == currentRelationInList2->relationGovernorIndex)
											{
												if(currentRelationInList->relationGovernorIndex == currentRelationInList3->relationGovernorIndex)
												{
													currentRelationInList2->relationType = RELATION_TYPE_OBJECT;
													currentRelationInList2->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
													currentRelationInList2->relationGovernor = currentRelationInList3->relationDependent;
													currentRelationInList->disabled = true;
													currentRelationInList3->disabled = true;
													disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
													disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

													/*WRONG;
													currentRelationInList3->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
													currentRelationInList3->relationGovernor = currentRelationInList2->relationDependent;
													currentRelationInList->disabled = true;
													currentRelationInList2->disabled = true;
													disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
													disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);
													*/
												}
											}
										}
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
								}
								#endif
								currentRelationInList3 = currentRelationInList3->next;
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


void redistributeStanfordRelationsDependencyPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	redistribute the following stanford relations, eg
		Given the nature of the bank, write the letter.
			translate prep+dep into prep
			prep(write-8, Given-1)
			dep(Given-1, nature-3)
			->prep_given(write-8, nature-3)
				prep(write-8, Given-1)
				det(nature-3, the-2)
				dep(Given-1, nature-3)
				det(bank-6, the-5)
				prep_of(nature-3, bank-6)
				root(ROOT-0, write-8)
				det(letter-10, the-9)
				dobj(write-8, letter-10)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DEPENDENT;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_PREPOSITION_MODIFIER;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT2;
	param.disableRelation[REL1] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT3] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT3] = REL_ENT1;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT2] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		//#endif
			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
 				GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					//#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_MODIFIER)
						{
							if(currentRelationInList->relationGovernorIndex == currentRelationInList2->relationDependentIndex)
							{//found a matching relationship

								currentRelationInList->disabled = true;
								string newPrepositionName = "";
								string relexPreposition = GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->entityName; 	//currentRelationInList2->relationDependent
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + relexPreposition;

								currentRelationInList2->relationType = newPrepositionName;
								#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW
								currentRelationInList2->relationTypeIndex = currentRelationInList->relationGovernorIndex;
								#endif
								currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
								currentRelationInList2->relationDependent = currentRelationInList->relationDependent;

								//disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);	//this will be used
							}
						}
					//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					}
					//#endif
					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

// updated and reactivated 2f12a 13-July-2014
// redistributeStanfordRelationsAuxHave() was designed for faulty case "flies" is incorrectly tagged by stanford parser in "Red dogs have flies." (a fault which occurs with STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS, when using Stanford Parser in conjunction with Stanford CoreNLP. redistributeStanfordRelationsAuxHave() was temporarily disabled @GIA 2c2a [because it was incompatible with "The chicken has not eaten a pie."])
#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
void redistributeStanfordRelationsAuxHave(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	/*
	eg Red dogs have flies.	aux(flies-4, have-3) + nsubj(flies-4, dogs-2) + !obj(flies-4, x) -> obj(have-3, flies-4) + nsubj(have-3, dogs-2)	//updated 2f12a 13-July-2014
	nn(dogs-2, Red-1)
	nsubj(flies-4, dogs-2)
	aux(flies-4, have-3)
	root(ROOT-0, flies-4)
	Joe is happy.	_predadj(Joe[1], happy[3])
	*/
//#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_MODAL_AUX;
	param.useRelationTest[REL1][REL_ENT2] = true; param.relationTest[REL1][REL_ENT2] = RELATION_ENTITY_HAVE;
	param.useRelationTest[REL2][REL_ENT3] = true; param.relationTest[REL2][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRelationIndexTest[REL1][REL_ENT1] = true; param.relationIndexTestRelationID[REL1][REL_ENT1] = REL2; param.relationIndexTestEntityID[REL1][REL_ENT1] = REL_ENT1;

	param.useRedistributeSpecialCaseNonExistantRelationCheck[REL2] = true;
	param.useRelationTest[REL3][REL_ENT3] = true; param.relationTest[REL3][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRelationIndexTest[REL3][REL_ENT1] = true; param.relationIndexTestRelationID[REL3][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL3][REL_ENT1] = REL_ENT1;
	param.disableEntity[REL1][REL_ENT2] = true;

	param.useRedistributeRelationEntityReassignment[REL1][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL1][REL_ENT3] = RELATION_TYPE_OBJECT;
	param.useRedistributeRelationEntityReassignment[REL2][REL_ENT3] = true; param.redistributeRelationEntityReassignment[REL2][REL_ENT3] = RELATION_TYPE_SUBJECT;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT1] = REL_ENT2; param.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL1][REL_ENT1] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL1][REL_ENT2] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL1][REL_ENT2] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL1][REL_ENT2] = REL_ENT1; param.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL1][REL_ENT2] = true;
	param.useRedistributeRelationEntityIndexReassignment[REL2][REL_ENT1] = true; param.redistributeRelationEntityIndexReassignmentRelationID[REL2][REL_ENT1] = REL1; param.redistributeRelationEntityIndexReassignmentRelationEntityID[REL2][REL_ENT1] = REL_ENT2; param.redistributeRelationEntityIndexReassignmentUseOriginalValues[REL2][REL_ENT1] = true;
	genericDependecyRelationInterpretation(&param, REL1);
//#else
	//not coded as this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
//#endif
}
#endif
#endif



#ifdef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
void redistributeStanfordRelationsDisableAuxAndCop(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramA.numberOfRelations = 1;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_MODAL_AUX;
	paramA.disableRelation[REL1] = true;
	genericDependecyRelationInterpretation(&paramA, REL1);

	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramB.numberOfRelations = 1;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_PASSIVE_AUX;
	paramB.disableRelation[REL1] = true;
	genericDependecyRelationInterpretation(&paramB, REL1);

	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramC.numberOfRelations = 1;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COPULA;
	paramC.disableRelation[REL1] = true;
	genericDependecyRelationInterpretation(&paramC, REL1);

	GIAgenericDepRelInterpretationParameters paramD(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramD.numberOfRelations = 1;
	paramD.useRelationTest[REL1][REL_ENT3] = true; paramD.relationTest[REL1][REL_ENT3] = RELATION_TYPE_DETERMINER;
	paramD.disableRelation[REL1] = true;
	genericDependecyRelationInterpretation(&paramD, REL1);

	#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	//added GIA 1c1a
	GIAgenericDepRelInterpretationParameters paramE(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramE.numberOfRelations = 1;
	paramE.useRelationTest[REL1][REL_ENT3] = true; paramE.relationTest[REL1][REL_ENT3] = RELATION_TYPE_NEGATIVE;
	paramE.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&paramE, REL1);
	#endif
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)
			{
				currentRelationInList->disabled = true;
			}
			if(currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				currentRelationInList->disabled = true;
			}
			if(currentRelationInList->relationType == RELATION_TYPE_COPULA)
			{
				currentRelationInList->disabled = true;
			}
			if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)
			{
				currentRelationInList->disabled = true;
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}
#endif

void collapseRedundantRelationAndMakeNegativeStanford(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[])
{
	/*
	eg The chicken has not eaten a pie.: neg(eaten-5, not-4)
	*/
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_NEGATIVE;
	param.disableRelation[REL1] = true;
	param.disableEntity[REL1][REL_ENT2] = true; 	//disable "not" entity
	//param.useRedistributeSpecialCaseDisableInstanceAndConcept[REL1][REL_ENT2]= true;	//no longer required because collapseRedundantRelationAndMakeNegativeRelex() is executed during redistribution
	//param.useRedistributeSpecialCaseNegativeAssignment[REL1][REL_ENT1] = true;
	GIAentityCharacteristic useRedistributeSpecialCaseNegativeAssignment("negative", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&useRedistributeSpecialCaseNegativeAssignment);
	genericDependecyRelationInterpretation(&param, REL1);
#else
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_NEGATIVE)
			{
				//eg The chicken has not eaten a pie.: neg(eaten-5, not-4)

				currentRelationInList->disabled = true;
				GIAentityNodeArray[currentRelationInList->relationGovernorIndex]->negative = true;
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

#endif
#endif


/*
#ifdef GIA_REMOVE_REDUNDANT_LOGICAL_CONDITION_ENTITIES
void redistributeStanfordRelationsDisableRedundantLogicalConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION

#else
	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		bool prepositionFound = false;
		string prepositionName = convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);
		if(prepositionFound)
		{
			//added GIA 2f13a - to account for scenarios in which 2 prepositions with identical type (eg for) and subject (eg eat) are being created; eg prep_for(eat[13], apple[8]) and prep_for(eat[13], basket[3]) in "For all baskets in the house and apples in the tree, eat the pie."
			//disable the relation without a conjunction conditionConnection
			//bool foundLogicalConditionOperationBasic = textInTextArray(prepositionName, logicalConditionOperationsArray, NLC_LOGICAL_CONDITION_OPERATIONS_NUMBER_OF_TYPES, &logicalOperation);
			if(prepositionName == RELATION_TYPE_PREPOSITION_FOR)
			{//eg for/while/if

				//cout << "prepositionName == RELATION_TYPE_PREPOSITION_FOR" << endl;
				bool primaryLogicalConditionRelation = false;
				GIArelation* currentRelationInList3 = currentSentenceInList->firstRelationInList;
 				while(currentRelationInList3->next != NULL)
				{
					bool conjunctionConditionFound = textInTextArray(currentRelationInList3->relationType, relationTypeConjugationNameArray, RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES);
					if(conjunctionConditionFound)
					{
						if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)
						{
							//preposition has an outgoing conjunction connection; take this as the primary relation and disable all others
							primaryLogicalConditionRelation = true;
							//cout << "primaryLogicalConditionRelation" << endl;
						}
					}
					currentRelationInList3 = currentRelationInList3->next;
				}
				if(primaryLogicalConditionRelation)
				{
					GIArelation* currentRelationInList2 = currentSentenceInList->firstRelationInList;
 					while(currentRelationInList2->next != NULL)
					{
						if(currentRelationInList2->relationType == currentRelationInList->relationType)
						{
							//found matching preposition
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{
								//found matching preposition subject
								if(currentRelationInList2 != currentRelationInList)
								{
									//redundant logical condition preposition relation found: disable it
									currentRelationInList2->disabled = true;
									#ifdef GIA_TRANSLATOR_DEBUG
									cout << "redundant logical condition preposition relation found: disable it" << endl;
									#endif
								}
							}
						}
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}

		}
		currentRelationInList = currentRelationInList->next;
	}
#endif
}
#endif
*/
