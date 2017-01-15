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
 * File Name: GIAtranslatorDefineSubstances.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2c4a 15-January-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslatorDefineSubstances.h"
#include "GIAdatabase.h"




#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void defineSubstances(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[], int NLPdependencyRelationsType)
{
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0a0 pass; define substances (objects/subjects with substances; eg 'Truffles which are picked are tasty.' - Truffle must be an instance/substance in this case); _obj(pick[4], truffle[1]), _predadj(truffle[1], tasty[6])" << endl;
	#endif
	defineSubstancesObjectsAndSubjectsWithSubstances(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, GIAentityNodeArray);	//why did this used to be GIAfeatureTempEntityNodeArray?

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0a1 pass; define substances (expletives eg 'there' in 'there is a place');" << endl;
	#endif
	defineSubstancesExpletives(currentSentenceInList, GIAentityNodeArray);
	
	#ifdef GIA_CREATE_SUBSTANCE_CONCEPTS_FOR_ALL_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0a2 pass; define substances all nodes" << endl;
	#endif
	defineSubstancesAllNodes(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
		
	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_DEFINITIVE_NOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0b pass; define substances (definite nouns); eg the house" << endl;
	#endif
	defineSubstancesDefiniteNouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#endif

	/*
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0kSHIFTED pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif
	*/

	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0c pass; define substances based on determinates of definition entities" << endl;
	#endif
	defineSubstancesBasedOnDeterminatesOfDefinitionEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	/*
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			cout << "1: GIAentityNodeArray[" << w << "] = " << GIAentityNodeArray[w]->entityName << ", isConcept = " << GIAentityNodeArray[w]->isConcept << endl;
		}
	}
	*/

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_NOUNS_WITH_DETERMINATES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0d pass; define substances (nouns with determinates); eg a house, the house, the houses [all nouns with singular/plural are assumed to have determintes, and are therefore substances]" << endl;
	#endif
	defineSubstancesNounsWithDeterminates(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0e pass; define substances (nouns with adjectives); _amod; eg locked door, _advmod; eg cheetahs run quickly [NOT and c) _predadj; eg giants are red / joe is late]" << endl;
	#endif
	defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(currentSentenceInList, GIAentityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0f pass; define substances (quantities [not quantity mods/multipiers, not measure pers] and measures);" << endl;
	#endif
	defineSubstancesQuantitiesAndMeasures(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0g pass; define substances (quantity mods);" << endl;
	#endif
	defineSubstancesQuantityModifiers(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_PRONOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0i pass; define substances (pronouns eg 'we'/'I');" << endl;
	#endif
	defineSubstancesPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0j pass; define substances (to_be);" << endl;
	#endif
	defineSubstancesToBe(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0k pass; define substances (to_do);" << endl;
	#endif
	defineActionsToDo(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0l pass; define substances (has time);" << endl;
	#endif
	defineSubstancesHasTime(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0m pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_4A_RELATIONS_DEFINE_SUBSTANCES_BASED_UPON_INDIRECT_OBJECTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0n pass; define substances indirect objects;" << endl;
	#endif
	defineSubstancesIndirectObjects(currentSentenceInList, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0o pass; define substances (possessive prepositions, eg 'all' in 'all of the mice');" << endl;
	#endif
	defineSubstancesOfPossessivePrepositions(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0p pass; define substance concepts (ie specific substance concepts)" << endl;
	#endif
	defineSubstanceConcepts(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0q pass; define substances (actions), eg 'run' in 'Tom runs' [exceptions 'having a chicken'/being a chicken': dobj(having-5, chicken-7) / dobj(be-5, chicken-7)]);" << endl;
	#endif
	defineSubstancesActions(currentSentenceInList, GIAentityNodeArray);
	#endif

	#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0r pass; define substances action concepts, eg 'swim' in 'To swim to the beach requires strength.'" << endl;
	#endif
	defineSubstancesActionConcepts(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#endif
}

void defineSubstancesObjectsAndSubjectsWithSubstances(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("isObjectTemp", "true");
	EntityCharacteristic entityCharacteristicsTest2("isSubjectTemp", "true");
	EntityCharacteristic entityCharacteristicsTest3("hasSubstanceTemp", "true");
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest3);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			if(((GIAfeatureTempEntityNodeArray[i]->isObjectTemp) && (GIAfeatureTempEntityNodeArray[i]->hasSubstanceTemp)) || ((GIAfeatureTempEntityNodeArray[i]->isSubjectTemp) && (GIAfeatureTempEntityNodeArray[i]->hasSubstanceTemp)))
			{
				GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAentityNodeArray[i]);
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesObjectsAndSubjectsWithSubstances: " << GIAentityNodeArray[i]->entityName << endl;
				#endif
			}
		}
	}
#endif
}

void defineSubstancesExpletives(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
	//eg There is 	_expl(be[2], there[1]) [Relex]	/ expl(is-2, There-1) [stanford]
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_EXPLETIVE;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
	#else
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_EXPLETIVE;
	param.disableEntity[REL1][REL_ENT2] = true;
	genericDependecyRelationInterpretation(&param, REL1);
	#endif
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
				//create substance definition
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
				GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesExpletives: " << substanceEntity->entityName << endl;
				#endif
				#else
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(substanceEntity);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void defineSubstancesDefiniteNouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("isPronounReference", "false");
	#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
	EntityCharacteristic entityCharacteristicsTest2("grammaticalProperNounTemp", "false");
	#endif
	EntityCharacteristic entityCharacteristicsTest3("grammaticalDefiniteTemp", "true");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest2);
	#endif
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest3);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{ //condition required as GIAentityNodeArrayFilled is not always true. With grammaticalIsDefinite, eg "Mr" of "Mr Smith" will still be interpreted as a definite
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define substances based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				if(!(featureArrayTemp[i]->grammaticalIsProperNoun))	//&& !GIAEntityNodeIsDateOrTime[i]
				{
				#endif
					if(featureArrayTemp[i]->grammaticalIsDefinite)
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						//cout << "addSubstanceToSubstanceDefinition: GIAentityNodeArray[i]->entityName = " << GIAentityNodeArray[i]->entityName << endl;
						#endif
						#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
						cout << "defineSubstancesDefiniteNouns: " << GIAentityNodeArray[i]->entityName << endl;
						cout << "i: " << i << endl;
						#endif
						GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAentityNodeArray[i]);
					}
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				}
				#endif
			}
		}
	}
#endif
}


#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
//Added 09 August 2012 [INCOMPLETE]
void defineSubstancesBasedOnDeterminatesOfDefinitionEntities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
	/* Added 09 August 2012
	The bat is an animal - (definition connection, bat = substance, animal = concept)	GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE [assumes defineSubstancesDefiniteNouns() executed]
		eg2; The red bat is a zenomorph. The blue bat is a non-zenomorph.
		NO: definite implies animal is an adjective/quality [check NLP]
	Bats are animals.  (definition connection, bat = concept, animal = concept)		GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL
	A book is an animal (definition connection, bat = concept, animal = concept)		GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE
	Bat is an animal. (definition connection, bat = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS] with isName==true, animal = concept)	GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE
	The bat is Max. (double definition connection, bat = substance, animal = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS] with isName==true)		GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME	[assumes defineSubstancesDefiniteNouns() executed]
	Bat is the animal. (double definition connection, bat = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS] with isName==true, animal = substance)	GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_6_GOVERNOR_NAME_DEPENDENT_DEFINITE
	That is Jim. (double definition connection, that = substance, Jim = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS])	GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_7_GOVERNOR_INDEFINITE_DEPENDENT_NAME
	Max is Jim. (double definition connection, Max = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS], Jim = concept or substance [if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS])	GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_8_GOVERNOR_NAME_DEPENDENT_NAME	//case added 14 October 2012
	*/

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
	cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD not migrated for GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES" << endl;
	#else
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	EntityCharacteristic entityCharacteristicsSet("alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", "true");
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&entityCharacteristicsSet);
	param.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&entityCharacteristicsSet);
	genericDependecyRelationInterpretation(&param, REL1);

	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramA.numberOfRelations = 1;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	EntityCharacteristic entityCharacteristicsTest1("grammaticalNumber", GRAMMATICAL_NUMBER_SINGULAR_STRING);
	EntityCharacteristic entityCharacteristicsTest2("grammaticalDefiniteTemp", "false");
	EntityCharacteristic entityCharacteristicsTest3("grammaticalProperNounTemp", "true");
	paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1);
	paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest2);
	paramA.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest3);
	EntityCharacteristic entityCharacteristicsSetAB("alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", "false");
	paramA.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&entityCharacteristicsSetAB);
	genericDependecyRelationInterpretation(&paramA, REL1);

	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramB.numberOfRelations = 1;
	paramB.useRelationTest[REL1][REL_ENT3] = true; paramB.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1);
	paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest2);
	paramB.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest3);
	paramB.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&entityCharacteristicsSetAB);
	genericDependecyRelationInterpretation(&paramB, REL1);

	entityCharacteristicsTest3.value = "false";
	GIAgenericDepRelInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramC.numberOfRelations = 1;
	paramC.useRelationTest[REL1][REL_ENT3] = true; paramC.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramC.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest1);
	paramC.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest2);
	paramC.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&entityCharacteristicsTest3);
	EntityCharacteristic entityCharacteristicsSetCD("mustSetIsSubstanceConceptBasedOnApposRelation", "true");
	paramC.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT1].push_back(&entityCharacteristicsSetCD);
	if(genericDependecyRelationInterpretation(&paramC, REL1))
	{
		//cout << "at1" << endl;
	}

	GIAgenericDepRelInterpretationParameters paramD(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	paramD.numberOfRelations = 1;
	paramD.useRelationTest[REL1][REL_ENT3] = true; paramD.relationTest[REL1][REL_ENT3] = RELATION_TYPE_APPOSITIVE_OF_NOUN;
	paramD.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest1);
	paramD.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest2);
	paramD.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT2].push_back(&entityCharacteristicsTest3);
	paramD.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&entityCharacteristicsSetCD);
	if(genericDependecyRelationInterpretation(&paramD, REL1))
	{
		//cout << "at2" << endl;
	}
	#endif
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD		//disabled nodes must be ignored, else disabled name definitions (eg name is Tom) will conflict (eg "The red dog's name is Tom.")
		if(!(currentRelationInList->disabled))
		{
		//#endif
			if(currentRelationInList->relationType == RELATION_TYPE_APPOSITIVE_OF_NOUN)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "defineSubstancesBasedOnDeterminatesOfDefinitionEntities(): RELATION_TYPE_APPOSITIVE_OF_NOUN" << endl;
				#endif

				int thingIndex = currentRelationInList->relationGovernorIndex;
				int definitionIndex = currentRelationInList->relationDependentIndex;

				bool thingFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[thingIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAconceptNodeArray to featureArrayTemp 14 July 2012b
					{
						thingFeatureHasDeterminate = true;
					}
				}
				bool definitionFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[definitionIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAconceptNodeArray to featureArrayTemp 14 July 2012b
					{
						definitionFeatureHasDeterminate = true;
					}
				}

				bool thingFeatureIsProperNoun = featureArrayTemp[thingIndex]->grammaticalIsProperNoun;
				bool definitionFeatureIsProperNoun = featureArrayTemp[definitionIndex]->grammaticalIsProperNoun;

				bool thingIsDefinite =  featureArrayTemp[thingIndex]->grammaticalIsDefinite;
				bool definitionIsDefinite = featureArrayTemp[definitionIndex]->grammaticalIsDefinite;

				GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];
				GIAentityNode * definitionEntity = GIAentityNodeArray[definitionIndex];
				featureArrayTemp[thingIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = true;
				featureArrayTemp[definitionIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = true;

				#ifdef GIA_ALIASES_DEBUG
				cout << "thingName = " << thingEntity->entityName << endl;
				cout << "\tthingFeatureHasDeterminate = " << thingFeatureHasDeterminate << endl;
				cout << "\tthingIsDefinite = " << thingIsDefinite << endl;
				cout << "\tthingFeatureIsProperNoun = " << thingFeatureIsProperNoun << endl;
				cout << "definitionName = " << definitionEntity->entityName << endl;
				cout << "\tdefinitionFeatureHasDeterminate = " << definitionFeatureHasDeterminate << endl;
				cout << "\tdefinitionIsDefinite = " << definitionIsDefinite << endl;
				cout << "\tdefinitionFeatureIsProperNoun = " << definitionFeatureIsProperNoun << endl;
				#endif

			#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE
				if((thingIsDefinite) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE" << endl;
					#endif
					//substance already assigned to thing  [assumes defineSubstancesDefiniteNouns() executed]
					//substance will not be assigned to definition
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL
				else if((!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (!definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL" << endl;
					#endif
					//substance will not be assigned to thing
					//substance will not be assigned to definition
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE
				else if((thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE" << endl;
					#endif
					//substance will not be assigned to thing
					//substance will not be assigned to definition
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE
				else if((thingFeatureHasDeterminate && !thingIsDefinite && thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))		//NB thingFeatureHasDeterminate will be true for proper nouns [as haveDeterminate is regenerated based upon GRAMMATICAL_NUMBER_SINGULAR], even though proper nouns do not actually have a determinate (eg a/the) assigned
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE" << endl;
					#endif
					//substance will be assigned to thing [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[thingIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
					//substance will not be assigned to definition
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME
				else if((thingFeatureHasDeterminate && thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && definitionFeatureIsProperNoun))
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME" << endl;
					#endif
					//substance already assigned to thing  [assumes defineSubstancesDefiniteNouns() executed]
					//substance will be assigned to definition [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[definitionIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_6_GOVERNOR_NAME_DEPENDENT_DEFINITE
				else if((thingFeatureHasDeterminate && !thingIsDefinite && thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_6_GOVERNOR_NAME_DEPENDENT_DEFINITE" << endl;
					#endif
					//substance will be assigned to thing [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[thingIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
					//substance already assigned to definition [assumes defineSubstancesDefiniteNouns() executed]
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_7_GOVERNOR_INDEFINITE_DEPENDENT_NAME
				else if((thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && definitionFeatureIsProperNoun))	//Added 14 August 2012 - 1o4aTEMP11a
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_7_GOVERNOR_INDEFINITE_DEPENDENT_NAME" << endl;
					#endif
					//substance will be assigned to definition [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[definitionIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
					//substance already assigned to thing [assumes defineSubstancesDefiniteNouns()/defineSubstancesNonExplicitPronouns() executed]
				}
				#endif

				//now prepare for the future setting of isSubstanceConcept by setting isSubstanceConceptTemp for the current concept node
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE
				if((thingIsDefinite) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				#endif
				/*The following is not required here as these will be set later on in defineSubstanceConcepts();
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL
				else if((!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (!definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				#endif
				*/
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE
				else if((thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))
				{
					featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				#endif
				#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE
				else if((thingFeatureHasDeterminate && !thingIsDefinite && thingFeatureIsProperNoun) && (definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun))		//NB thingFeatureHasDeterminate will be true for proper nouns [as haveDeterminate is regenerated based upon GRAMMATICAL_NUMBER_SINGULAR], even though proper nouns do not actually have a determinate (eg a/the) assigned
				{
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				#endif
			#else
				if(thingFeatureHasDeterminate && !thingIsDefinite && thingFeatureIsProperNoun)	 //NB thingFeatureHasDeterminate will be true for proper nouns [as haveDeterminate is regenerated based upon GRAMMATICAL_NUMBER_SINGULAR], even though proper nouns do not actually have a determinate (eg a/the) assigned
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES: substance will be assigned to thing [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]" << endl;
					#endif
					//substance will be assigned to thing [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[thingIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
				}
				else if(definitionFeatureHasDeterminate && !definitionIsDefinite && definitionFeatureIsProperNoun)	//changed 'else if' to 'if' 14 October 2012 to take into account case GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_8_GOVERNOR_NAME_DEPENDENT_NAME
				{
					#ifdef GIA_ALIASES_DEBUG
					cout << "GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES: substance will be assigned to definition [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]" << endl;
					#endif
					//substance will be assigned to definition [only if GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS]
					featureArrayTemp[definitionIndex]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;
				}

				//now prepare for the future setting of isSubstanceConcept by setting isSubstanceConceptTemp for the current concept node
				if(thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)
				{
					featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
					//cout << "mustSetIsSubstanceConceptBasedOnApposRelation: " << thingIndex << ", " << featureArrayTemp[thingIndex]->lemma << endl;
				}
				if(definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun)
				{
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
					//cout << "mustSetIsSubstanceConceptBasedOnApposRelation: " << definitionIndex << ", " << featureArrayTemp[thingIndex]->lemma << endl;

				}
				/*The following is not required here as these will be set later on in defineSubstanceConcepts();
				if(!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)
				{
					featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				if(!definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun)
				{
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				*/
			#endif

			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
	#ifdef GIA_ALIASES_DEBUG
	cout << "exit defineSubstancesBasedOnDeterminatesOfDefinitionEntities" << endl;
	#endif
#endif
}
#endif

void defineSubstancesNounsWithDeterminates(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("isPronounReference", "false");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
	EntityCharacteristic entityCharacteristicsTest2("grammaticalProperNounTemp", "false");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest2);
	#endif
	EntityCharacteristic entityCharacteristicsTest3("grammaticalNumber", GRAMMATICAL_NUMBER_SINGULAR_STRING);
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest3);
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	EntityCharacteristic entityCharacteristicsTest4("alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp", "false");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest4);
	#endif
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define substances based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				if(!(featureArrayTemp[i]->grammaticalIsProperNoun))	//&& !GIAEntityNodeIsDateOrTime[i]
				{
				#endif
					bool passed = false;
					for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
					{
						if(featureArrayTemp[i]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAconceptNodeArray to featureArrayTemp 14 July 2012b
						{
							passed = true;
						}
					}
					if(passed)
					{
						#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
						if(!(featureArrayTemp[i]->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp))
						{
						#endif
							#ifdef GIA_TRANSLATOR_DEBUG
							//cout << "\t\taddSubstanceToSubstanceDefinition: GIAentityNodeArray[i]->entityName = " << GIAentityNodeArray[i]->entityName << endl;
							#endif
							#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
							cout << "defineSubstancesNounsWithDeterminates: " << GIAentityNodeArray[i]->entityName << endl;
							#endif
							GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAentityNodeArray[i]);
						#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
						}
						#endif
					}
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				}
				#endif
			}
		}
	}
#endif
}

void defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType)
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramA.numberOfRelations = 1;
	paramA.useRelationArrayTest[REL1][REL_ENT3] = true; paramA.relationArrayTest[REL1][REL_ENT3] = relationTypeAdjectiveWhichImplyEntityInstanceNameArray; paramA.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES;
	paramA.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	paramA.functionEntityRelationID[FUNC_ENT1] = REL1; paramA.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	genericDependecyRelationInterpretation(&paramA, REL1);

	#ifdef GIA_TRANSLATOR_DEFINE_NOUNS_WITH_PRENOMINAL_MODIFIERS_AS_SUBSTANCES
	GIAgenericDepRelInterpretationParameters paramB(currentSentenceInList, NULL, GIAentityNodeArray, true);
	paramB.numberOfRelations = 1;
	paramB.useRelationArrayTest[REL1][REL_ENT3] = true; paramB.relationArrayTest[REL1][REL_ENT3] = relationTypePrenominalModifierNameArray; paramB.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES;
	paramB.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR
	paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	#else
	paramB.functionEntityRelationID[FUNC_ENT1] = REL1; paramB.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	#endif
	genericDependecyRelationInterpretation(&paramB, REL1);
	#endif
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeAdjectiveWhichImplyEntityInstanceNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				bool passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAentityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);

				if(passed3)
				{
				#endif
					//create a new substance for the entity and assign a substance definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string substanceName = currentRelationInList->relationDependent;
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;

					GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];
					GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

					GIAentityNodeArray[thingIndex] = addSubstanceToSubstanceDefinition(thingEntity);
					#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
					cout << "defineSubstancesNounsWithAdjectivesOrPrenominalModifiers: " << thingEntity->entityName << endl;
					#endif
				#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				}
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}

	#ifdef GIA_TRANSLATOR_DEFINE_NOUNS_WITH_PRENOMINAL_MODIFIERS_AS_SUBSTANCES
	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypePrenominalModifierNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR
				//create a new substance for the entity and assign a substance definition entity if not already created
				string thingName = currentRelationInList->relationGovernor;
				string substanceName = currentRelationInList->relationDependent;
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int substanceIndex = currentRelationInList->relationDependentIndex;

				GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				GIAentityNodeArray[thingIndex] = addSubstanceToSubstanceDefinition(thingEntity);
				#else
				//create a new substance for the entity and assign a substance definition entity if not already created
				string thingName = currentRelationInList->relationDependent;
				string substanceName = currentRelationInList->relationGovernor;
				int thingIndex = currentRelationInList->relationDependentIndex;
				int substanceIndex = currentRelationInList->relationGovernorIndex;

				GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				GIAentityNodeArray[thingIndex] = addSubstanceToSubstanceDefinition(thingEntity);
				#endif
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesNounsWithAdjectivesOrPrenominalModifiers: " << thingEntity->entityName << endl;
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
	#endif
#endif
}

void defineSubstancesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeQuantityOrMeasureNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQuantityOrMeasureNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				//create a new substance for the entity and assign a substance definition entity if not already created
				int substanceIndex = currentRelationInList->relationGovernorIndex;
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesQuantitiesAndMeasures: " << substanceEntity->entityName << endl;
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void defineSubstancesQuantityModifiers(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeQuantityOrMeasureSwitchedNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeQuantityOrMeasureSwitchedNameArray[i])
				{
					passed = true;
				}
			}
			if(passed)
			{
				//create a new substance for the entity and assign a substance definition entity if not already created
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesQuantityModifiers: " << substanceEntity->entityName << endl;
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void defineSubstancesPronouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("isPronounReference", "false");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	EntityCharacteristic entityCharacteristicsTest2("grammaticalPronounTemp", "true");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest2);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define substances based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)

				if(featureArrayTemp[i]->grammaticalIsPronoun == GRAMMATICAL_PRONOUN)
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "addSubstanceToSubstanceDefinition: GIAentityNodeArray[i]->entityName = " << GIAentityNodeArray[i]->entityName << endl;
					#endif
					#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
					cout << "defineSubstancesPronouns: " << GIAentityNodeArray[i]->entityName << endl;
					#endif
					GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAentityNodeArray[i]);
				}
			}
		}
	}
#endif
}


void defineSubstancesToBe(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{

				//create a new substance for the entity and assign a substance definition entity if not already created
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];

				GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);

				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesToBe: " << substanceEntity->entityName << endl;
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void defineActionsToDo(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_DO;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addActionToActionDefinitionDefineSubstances;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{

				//create a new substance for the entity and assign a substance definition entity if not already created
				int actionIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode * actionEntity = GIAentityNodeArray[actionIndex];

				GIAentityNodeArray[actionIndex] = addActionToActionDefinitionDefineSubstances(actionEntity);

				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineActionsToDo: " << actionEntity->entityName << endl;
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
#endif
}

void defineSubstancesHasTime(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("hasAssociatedTime", "true");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(featureArrayTemp[w]->grammaticalIsDateOrTime)
			{
				GIAentityNode * currentGIAEntityNode = GIAentityNodeArray[w];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "grammaticalIsDateOrTime; currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;
				#endif
				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesHasTime: " << currentGIAEntityNode->entityName << endl;
				#endif
				GIAentityNodeArray[w] = addSubstanceToSubstanceDefinition(currentGIAEntityNode);
			}
		}
	}
#endif
}

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
void defineSubstancesNonExplicitPronouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.useEntityArrayTest = true; param.entityArrayTest = featureTypeTreatAsPronounIeSubstance; param.entityArrayTestSize = RELATION_TYPE_TREAT_AS_PRONOUN_IE_SUBSTANCE_NUMBER_OF_TYPES;
	#ifdef GIA_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE_ASSIGN_DETERMINATE_SINGULAR
	EntityCharacteristic entityCharacteristicsSet("grammaticalNumber", GRAMMATICAL_NUMBER_SINGULAR_STRING);	//added 14 August 2012	(select any value from referenceTypeHasDeterminateCrossReferenceNumberArray[])
	param.specialCaseCharacteristicsAssignmentVector.push_back(&entityCharacteristicsSet);
	#endif
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			bool passed = false;
			for(int j=0; j<RELATION_TYPE_TREAT_AS_PRONOUN_IE_SUBSTANCE_NUMBER_OF_TYPES; j++)
			{
				if(GIAentityNodeArray[i]->entityName == featureTypeTreatAsPronounIeSubstance[j])
				{
					passed = true;
				}
			}

			if(passed)
			{
				#ifdef GIA_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE_ASSIGN_DETERMINATE_SINGULAR
				GIAentityNodeArray[i]->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;	//added 14 August 2012	(select any value from referenceTypeHasDeterminateCrossReferenceNumberArray[])
				#endif
				GIAentityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAentityNodeArray[i]);

				#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
				cout << "defineSubstancesNonExplicitPronouns: " << GIAentityNodeArray[i]->entityName << endl;
				#endif
			}
		}
	}
#endif
}
#endif

void defineSubstancesIndirectObjects(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 2;
	param.useRelationTest[REL1][REL_ENT3] = true; param.relationTest[REL1][REL_ENT3] = RELATION_TYPE_INDIRECT_OBJECT;
	param.useRelationArrayTest[REL2][REL_ENT3] = true; param.relationArrayTest[REL2][REL_ENT3] = relationTypeObjectNameArray; param.relationArrayTestSize[REL2][REL_ENT3] = RELATION_TYPE_OBJECT_NUMBER_OF_TYPES;
	param.useRelationIndexTest[REL2][REL_ENT1] = true; param.relationIndexTestRelationID[REL2][REL_ENT1] = REL1; param.relationIndexTestEntityID[REL2][REL_ENT1] = REL_ENT1;
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL2; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT2;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool partnerTypeRequiredFound = false;
						for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
						{
							if(currentRelationInList2->relationType == relationTypeObjectNameArray[i])
							{
								partnerTypeRequiredFound = true;
							}
						}
						if(partnerTypeRequiredFound)
						{

							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching object-indirectobject relationship

								int substanceIndex = currentRelationInList2->relationDependentIndex;
								int thingIndex = currentRelationInList->relationDependentIndex;

								GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];
								GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];

								GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);

								#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
								cout << "defineSubstancesIndirectObjects: " << substanceEntity->entityName << endl;
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

#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
void defineSubstanceConcepts(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION

	GIAgenericEntityInterpretationParameters paramA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	EntityCharacteristic entityCharacteristicsTestA1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_NOUN_STRING);
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA1);
	#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
	EntityCharacteristic entityCharacteristicsTestA2("wasReference", "false");
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA2);
	#endif
	#ifndef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
	EntityCharacteristic entityCharacteristicsTestA3("grammaticalPronounTemp", "false");
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA3);
	#endif
	EntityCharacteristic entityCharacteristicsTestA4("grammaticalNumber", GRAMMATICAL_NUMBER_SINGULAR_STRING, true);
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA4);
	EntityCharacteristic entityCharacteristicsTestA5("grammaticalDefiniteTemp", "false");
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA5);
	EntityCharacteristic entityCharacteristicsTestA6("grammaticalProperNounTemp", "false");
	paramA.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestA6);
	EntityCharacteristic entityCharacteristicsSetA("isSubstanceConcept", "true");
	paramA.specialCaseCharacteristicsAssignmentVector.push_back(&entityCharacteristicsSetA);
	genericEntityInterpretation(&paramA);

	GIAgenericEntityInterpretationParameters paramB(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	EntityCharacteristic entityCharacteristicsTestB1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_NOUN_STRING);
	paramB.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestB1);
	#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
	EntityCharacteristic entityCharacteristicsTestB2("wasReference", "false");
	paramB.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestB2);
	#endif
	#ifndef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
	EntityCharacteristic entityCharacteristicsTestB3("grammaticalPronounTemp", "false");
	paramB.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestB3);
	#endif
	EntityCharacteristic entityCharacteristicsTestB4("mustSetIsSubstanceConceptBasedOnApposRelation", "true");
	paramB.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestB4);
	EntityCharacteristic entityCharacteristicsSetB("isSubstanceConcept", "true");
	paramB.specialCaseCharacteristicsAssignmentVector.push_back(&entityCharacteristicsSetB);
	genericEntityInterpretation(&paramB);

	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
	GIAgenericEntityInterpretationParameters paramC(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	EntityCharacteristic entityCharacteristicsTestC1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_NOUN_STRING);
	paramC.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestC1);
	#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
	EntityCharacteristic entityCharacteristicsTestC2("wasReference", "false");
	paramC.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestC2);
	#endif
	EntityCharacteristic entityCharacteristicsTestC3("grammaticalPronounTemp", "true");
	paramC.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestC3);
	EntityCharacteristic entityCharacteristicsSetC("isSubstanceConcept", "true");
	paramC.specialCaseCharacteristicsAssignmentVector.push_back(&entityCharacteristicsSetC);
	genericEntityInterpretation(&paramC);
	#endif

	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PROPERNOUNS
	GIAgenericEntityInterpretationParameters paramD(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, false);
	EntityCharacteristic entityCharacteristicsTestD1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_NOUN_STRING);
	paramD.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestD1);
	#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
	EntityCharacteristic entityCharacteristicsTestD2("wasReference", "false");
	paramD.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestD2);
	#endif
	EntityCharacteristic entityCharacteristicsTestD3("grammaticalProperNounTemp", "true");
	paramD.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTestD3);
	EntityCharacteristic entityCharacteristicsSetD("isSubstanceConcept", "true");
	paramD.specialCaseCharacteristicsAssignmentVector.push_back(&entityCharacteristicsSetD);
	genericEntityInterpretation(&paramD);
	#endif
#else
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			int thingIndex = i;
			if(featureArrayTemp[thingIndex]->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
			{
				GIAentityNode * thingEntity = GIAentityNodeArray[thingIndex];

				#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
				if(!(thingEntity->wasReference))
				{
				#endif
					//cout << "thingEntity->entityName = " << thingEntity->entityName << endl;
					//cout << "thingIndex = " << thingIndex << endl;

					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "defineSubstancesBasedOnDeterminatesOfDefinitionEntities(): RELATION_TYPE_APPOSITIVE_OF_NOUN" << endl;
					#endif

					bool thingFeatureHasDeterminate = false;
					for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
					{
						if(featureArrayTemp[thingIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAconceptNodeArray to featureArrayTemp 14 July 2012b
						{
							thingFeatureHasDeterminate = true;
						}
					}

					bool thingFeatureIsProperNoun = featureArrayTemp[thingIndex]->grammaticalIsProperNoun;
					bool thingIsDefinite =  featureArrayTemp[thingIndex]->grammaticalIsDefinite;

					bool thingFeatureIsPronoun = false;
					if(featureArrayTemp[thingIndex]->grammaticalIsPronoun == GRAMMATICAL_PRONOUN)
					{
						thingFeatureIsPronoun = true;
					}
					#ifndef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
					if(!thingFeatureIsPronoun)
					{
					#endif
						if(!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)
						{
							/*
							cout << "thingEntity->entityName = " << i << ", " << thingEntity->entityName << endl;
							cout << "\t(!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)" << endl;
							*/
							thingEntity->isSubstanceConcept = true;
						}
						if(featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation)
						{
							/*
							cout << "thingEntity->entityName = " << i << ", " << thingEntity->entityName << endl;
							cout << "\t(featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation)" << endl;
							*/
							thingEntity->isSubstanceConcept = true;
						}
					#ifndef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
					}
					#endif
					#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS
					if(thingFeatureIsPronoun)
					{
						/*
						cout << "thingEntity->entityName = " << i << ", " << thingEntity->entityName << endl;
						cout << "\tthingFeatureIsPronoun" << endl;
						*/
						thingEntity->isSubstanceConcept = true;
					}
					#endif
					#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PROPERNOUNS
					if(thingFeatureIsProperNoun)
					{
						/*
						cout << "thingEntity->entityName = " << i << ", " << thingEntity->entityName << endl;
						cout << "\tthingFeatureIsProperNoun" << endl;
						*/
						thingEntity->isSubstanceConcept = true;
					}
					#endif
				#ifdef GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
				}
				#endif
			}
		}
	}
#endif
}
#endif


void defineSubstancesOfPossessivePrepositions(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	/*
	eg _to-be(grow[2], tired[3])
	*/
	//test this generalisation 2 August 2013
	GIAgenericDepRelInterpretationParameters paramA(currentSentenceInList, NULL, GIAentityNodeArray, false);
	paramA.numberOfRelations = 1;
	paramA.useRelationTest[REL1][REL_ENT3] = true; paramA.relationTest[REL1][REL_ENT3] = RELATION_TYPE_COMPLIMENT_TO_BE;
	//paramA.useRedistributeSpecialCaseIsToBeComplimentOfActionTempAssignment[REL1][REL_ENT2] = true;
	EntityCharacteristic useRedistributeSpecialCaseIsToBeComplimentOfActionTempAssignment("isToBeComplimentOfActionTemp", "true");
	paramA.specialCaseCharacteristicsAssignmentVector[REL1][REL_ENT2].push_back(&useRedistributeSpecialCaseIsToBeComplimentOfActionTempAssignment);
	genericDependecyRelationInterpretation(&paramA, REL1);
	/*OLD:
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{
				//_to-be(grow[2], tired[3])
				GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isToBeComplimentOfActionTemp = true;;
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
	*/

	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypePossessivePrepositionsNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES;
	//param.relationTestSpecialCaseIsNotAction[REL1][REL_ENT1] = true;	//Stanford Condition
	//param.relationTestSpecialCaseIsNotToBeComplimentOfAction[REL1][REL_ENT1] = true;	//Relex Condition
	EntityCharacteristic relationTestSpecialCaseIsNotAction("isAction", "false");								//Stanford Condition
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseIsNotAction);			//Stanford Condition
	EntityCharacteristic relationTestSpecialCaseIsNotToBeComplimentOfAction("isToBeComplimentOfActionTemp", "false");			//Relex Condition
	param.specialCaseCharacteristicsTestAndVector[REL1][REL_ENT1].push_back(&relationTestSpecialCaseIsNotToBeComplimentOfAction);	//Relex Condition
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	genericDependecyRelationInterpretation(&param, REL1);
#else
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{
				//_to-be(grow[2], tired[3])
				GIAentityNodeArray[currentRelationInList->relationDependentIndex]->isToBeComplimentOfActionTemp = true;;
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}

	currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool foundPossessivePreposition = false;
			for(int i=0; i<RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES; i++)
			{
				bool prepositionFound = false;
				if(convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound) == relationTypePossessivePrepositionsNameArray[i])
				{
					foundPossessivePreposition = true;
				}
			}

			if(foundPossessivePreposition)
			{//make sure the parent is not an action

				bool foundAction = false;
				int substanceIndex = currentRelationInList->relationGovernorIndex;		//eg 'grow' in prep_of(grew-2, pie-6)
				GIAentityNode * substanceEntity = GIAentityNodeArray[substanceIndex];
				if((substanceEntity->isActionTemp) || (substanceEntity->isToBeComplimentOfActionTemp))
				{
					/*Relex:
					NB not in this case "She grew tired of the pie."
					of(tired[3], pie[6])
					_to-be(grow[2], tired[3])
					_subj(grow[2], she[1])
					*/
					/*Stanford:
					prep_of(grew-2, pie-6)
					acomp(grew-2, tired-3)
					nsubj(grew-2, She-1)
					*/

					foundAction = true;
				}

				if(!foundAction)
				{
					/*
					NB this case "The house of Kriton is blue." should create 2 substance connections (not just 1)
					of(house[2], Kriton[4])
					_predadj(house[2], blue[6])
					eg2 'all' in 'all of the mice'
					*/

					//eg 'all' in prep_of(all-x, mouse-y)

					GIAentityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);

					#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
					cout << "defineSubstancesOfPossessivePrepositions: " << substanceEntity->entityName << endl;
					#endif
				}
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
#endif
}

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
//this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION and was designed for it
void defineSubstancesActions(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[])
{
	/*
	eg 'run' in 'Tom runs' _subj(run[2], Tom[1])
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	exceptions required for linkHavingPropertyConditionsAndBeingDefinitionConditions():
		'having a chicken'/being a chicken': dobj(having-5, chicken-7) / dobj(be-5, chicken-7)]
	#endif
	no exceptions required for linkSubjectObjectRelationships(); this will cause no problems:
		 eg The rabbit is 20 meters away.	_subj(away[6], rabbit[2]) + _measure_distance(away[6], meter[5])	[away is a substance of rabbit, not a condition of rabbit]
	*/

//#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
	GIAgenericDepRelInterpretationParameters param(currentSentenceInList, NULL, GIAentityNodeArray, true);
	param.numberOfRelations = 1;
	param.useRelationArrayTest[REL1][REL_ENT3] = true; param.relationArrayTest[REL1][REL_ENT3] = relationTypeSubjectObjectNameArray; param.relationArrayTestSize[REL1][REL_ENT3] = RELATION_TYPE_SUBJECT_OBJECT_NUMBER_OF_TYPES;
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	param.useRelationArrayTest[REL1][REL_ENT1] = true; param.relationArrayTest[REL1][REL_ENT1] = linkHavingPropertyConditionsAndBeingDefinitionConditionsSpecialEntityNameArray; param.relationArrayTestSize[REL1][REL_ENT1] = RELATION_TYPE_HAVING_AND_BEING_SPECIAL_ENTITY_NUMBER_OF_TYPES; param.relationArrayTestIsNegative[REL1][REL_ENT1] = true;	//exceptions required for linkHavingPropertyConditionsAndBeingDefinitionConditions()
	#endif
	
	param.functionToExecuteUponFind = GIA_GENERIC_DEP_REL_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	param.functionEntityRelationID[FUNC_ENT1] = REL1; param.functionEntityRelationEntityID[FUNC_ENT1] = REL_ENT1;
	genericDependecyRelationInterpretation(&param, REL1);
//#else
	//not coded as this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
//#endif
}
#endif

#endif

#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
void defineSubstancesActionConcepts(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[])
{
	/*
	eg 'swim' in 'To swim to the beach requires strength.'
	*/
#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	EntityCharacteristic entityCharacteristicsTest1("grammaticalWordTypeTemp", GRAMMATICAL_WORD_TYPE_VERB_STRING);
	EntityCharacteristic entityCharacteristicsTest2("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE);
	EntityCharacteristic entityCharacteristicsTest3("grammaticalTenseModifierArrayTemp", "true", GRAMMATICAL_TENSE_MODIFIER_INFINITIVE);
	EntityCharacteristic entityCharacteristicsTest4("foundPossibleInfinitiveVerbTemp", "true");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest2);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest3);
	param.specialCaseCharacteristicsTestOrVector.push_back(&entityCharacteristicsTest4);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
#else
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAentityNodeArray[w]->disabled))
			{
				if((featureArrayTemp[w]->grammaticalWordType == GRAMMATICAL_WORD_TYPE_VERB) && ((featureArrayTemp[w]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true) || (featureArrayTemp[w]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true) || (featureArrayTemp[w]->foundPossibleInfinitiveVerb)))
				{
					GIAentityNode * currentGIAEntityNode = GIAentityNodeArray[w];
					#ifdef GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
					cout << "defineSubstancesActionConcepts: " << currentGIAEntityNode->entityName << endl;
					#endif
					GIAentityNodeArray[w] = addSubstanceToSubstanceDefinition(currentGIAEntityNode);
				}
			}
		}
	}
#endif
}
#endif

void defineSubstancesAllNodes(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	#ifdef GIA_USE_GENERIC_ENTITY_INTERPRETATION
	GIAgenericEntityInterpretationParameters param(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, true);
	param.useEntityTest = true; param.entityTest = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE; param.entityTestIsNegative = true;
	EntityCharacteristic entityCharacteristicsTest1("isConcept", "true");
	param.specialCaseCharacteristicsTestAndVector.push_back(&entityCharacteristicsTest1);
	param.functionToExecuteUponFind = GIA_GENERIC_ENTITY_INTERP_EXECUTE_FUNCTION_addSubstanceToSubstanceDefinition;
	genericEntityInterpretation(&param);
	#else
	cout << "defineSubstancesAllNodes() error: not coded as this function was developed after GIA_USE_GENERIC_ENTITY_INTERPRETATION" << endl;
	#endif	
}
