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
 * File Name: GIATranslatorDefineSubstances.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q10a 12-November-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineSubstances.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"





/*
void collapseRedundantRelationAndMakeNegative(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif
}
*/

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void collapseRedundantRelationAndMakeNegativeStanford(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_NEGATIVE)
			{
				//eg The chicken has not eaten a pie.: neg(eaten-5, not-4)

				currentRelationInList->disabled = true;
				GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->negative = true;
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);
			}

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

#ifdef GIA_USE_RELEX
void collapseRedundantRelationAndMakeNegativeRelex(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//_subj(not[5], by[4]), _subj(have[6], not[5])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{

				bool passed = false;
				for(int j=0; j<RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES; j++)
				{
					if(GIAEntityNodeArray[currentRelationInList->relationGovernorIndex]->entityName == relationContextNegativeNameArray[j])
					{
						passed = true;
					}
				}
				if(passed)
				{
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						if(!(currentRelationInList2->disabled))
						{
						#endif
							#ifdef GIA_TRANSLATOR_DEBUG
							//cout << "currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
							#endif
							if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
							{

								bool passed2 = false;
								for(int j=0; j<RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES; j++)
								{
									if(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]->entityName == relationContextNegativeNameArray[j])
									{
										passed2 = true;
									}
								}
								if(passed2)
								{
									disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]);
									disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]);

									GIAEntityNodeArray[currentRelationInList2->relationDependentIndex] = GIAEntityNodeArray[currentRelationInList->relationDependentIndex];
									GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]->negative = true;
									currentRelationInList->disabled = true;
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

void defineSubstancesObjectsAndSubjectsWithSubstances(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], GIAEntityNode * GIAFeatureTempEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			if(((GIAFeatureTempEntityNodeArray[i]->isObjectTemp) && (GIAFeatureTempEntityNodeArray[i]->hasSubstanceTemp)) || ((GIAFeatureTempEntityNodeArray[i]->isSubjectTemp) && (GIAFeatureTempEntityNodeArray[i]->hasSubstanceTemp)))
			{
				GIAEntityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAEntityNodeArray[i]);
			}
		}
	}
}

void defineSubstancesDefiniteNouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{ //condition required as GIAEntityNodeArrayFilled is not always true. With grammaticalIsDefinite, eg "Mr" of "Mr Smith" will still be interpreted as a definite
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define substances based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				if(!(featureArrayTemp[i]->grammaticalIsProperNoun))	//&& !GIAEntityNodeIsDateOrTime[i]
				{
				#endif
					if(featureArrayTemp[i]->grammaticalIsDefinite)
					{
						#ifdef GIA_TRANSLATOR_DEBUG
						//cout << "addSubstanceToSubstanceDefinition: GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
						#endif
						GIAEntityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAEntityNodeArray[i]);
					}
				#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS
				}
				#endif
			}
		}
	}
}


#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
//Added 09 August 2012 [INCOMPLETE]
void defineSubstancesBasedOnDeterminatesOfDefinitionEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
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

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS		//disabled nodes must be ignored, else disabled name definitions (eg name is Tom) will conflict (eg "The red dog's name is Tom.")
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
					if(featureArrayTemp[thingIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
					{
						thingFeatureHasDeterminate = true;
					}
				}
				bool definitionFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[definitionIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
					{
						definitionFeatureHasDeterminate = true;
					}
				}

				bool thingFeatureIsProperNoun = featureArrayTemp[thingIndex]->grammaticalIsProperNoun;
				bool definitionFeatureIsProperNoun = featureArrayTemp[definitionIndex]->grammaticalIsProperNoun;

				bool thingIsDefinite =  featureArrayTemp[thingIndex]->grammaticalIsDefinite;
				bool definitionIsDefinite = featureArrayTemp[definitionIndex]->grammaticalIsDefinite;

				GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
				GIAEntityNode * definitionEntity = GIAEntityNodeArray[definitionIndex];
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
			#endif
			
				//now prepare for the future setting of isSubstanceConcept by setting isSubstanceConceptTemp for the current concept node		
			#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
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
				if(thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun) 
				{
					featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
				}
				if(definitionFeatureHasDeterminate && !definitionIsDefinite && !definitionFeatureIsProperNoun) 
				{
					featureArrayTemp[definitionIndex]->mustSetIsSubstanceConceptBasedOnApposRelation = true;
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
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
	#ifdef GIA_ALIASES_DEBUG
	cout << "exit defineSubstancesBasedOnDeterminatesOfDefinitionEntities" << endl;
	#endif
}
#endif

void defineSubstancesNounsWithDeterminates(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
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
						if(featureArrayTemp[i]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
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
							//cout << "\t\taddSubstanceToSubstanceDefinition: GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
							#endif
							GIAEntityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAEntityNodeArray[i]);
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
}

void defineSubstancesNounsWithAdjectives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
				bool passed3 = isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(currentRelationInList, GIAEntityNodeArray, currentRelationInList->relationGovernorIndex, NLPdependencyRelationsType);

				if(passed3)
				{
					//create a new substance for the entity and assign a substance definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string substanceName = currentRelationInList->relationDependent;
					int thingIndex = currentRelationInList->relationGovernorIndex;
					int substanceIndex = currentRelationInList->relationDependentIndex;


					GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
					GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

					GIAEntityNodeArray[thingIndex] = addSubstanceToSubstanceDefinition(thingEntity);
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void defineSubstancesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

				GIAEntityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void defineSubstancesQuantityModifiers(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

				GIAEntityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void defineSubstancesExpletives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//eg There is 	_expl(be[2], there[1]) [Relex]	/ expl(is-2, There-1) [stanford]
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
				//create substance definition
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

				#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
				GIAEntityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
				#else
				disableInstanceAndConceptEntityBasedUponFirstSentenceToAppearInNetwork(substanceEntity);
				#endif
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void defineSubstancesPronouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			if(!(featureArrayTemp[i]->isPronounReference))
			{//do not define substances based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)

				if(featureArrayTemp[i]->grammaticalIsPronoun == GRAMMATICAL_PRONOUN)
				{
					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "addSubstanceToSubstanceDefinition: GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
					#endif
					GIAEntityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAEntityNodeArray[i]);
				}
			}
		}
	}
}


void defineSubstancesToBe(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_BE)
			{

				//create a new substance for the entity and assign a substance definition entity if not already created
				int substanceIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];

				GIAEntityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void defineActionsToDo(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{

				//create a new substance for the entity and assign a substance definition entity if not already created
				int actionIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * actionEntity = GIAEntityNodeArray[actionIndex];

				GIAEntityNodeArray[actionIndex] = addActionToActionDefinitionDefineSubstances(actionEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void defineSubstancesHasTime(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], Feature * featureArrayTemp[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			if(featureArrayTemp[w]->grammaticalIsDateOrTime)
			{
				GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "grammaticalIsDateOrTime; currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;
				#endif
				GIAEntityNodeArray[w] = addSubstanceToSubstanceDefinition(currentGIAEntityNode);
			}
		}
	}
}

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
void defineSubstancesNonExplicitPronouns(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			bool passed = false;
			for(int j=0; j<RELATION_TYPE_TREAT_AS_PRONOUN_IE_SUBSTANCE_NUMBER_OF_TYPES; j++)
			{
				if(GIAEntityNodeArray[i]->entityName == relationTypeTreatAsPronounIeSubstance[j])
				{
					passed = true;
				}
			}

			if(passed)
			{
				#ifdef GIA_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE_ASSIGN_DETERMINATE_SINGULAR
				GIAEntityNodeArray[i]->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;	//added 14 August 2012	(select any value from referenceTypeHasDeterminateCrossReferenceNumberArray[])
				#endif
				GIAEntityNodeArray[i] = addSubstanceToSubstanceDefinition(GIAEntityNodeArray[i]);
			}
		}
	}
}
#endif

void defineSubstancesIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_INDIRECT_OBJECT)
			{
				//now find the associated object..
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

								GIAEntityNode * substanceEntity = GIAEntityNodeArray[substanceIndex];
								GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];

								GIAEntityNodeArray[substanceIndex] = addSubstanceToSubstanceDefinition(substanceEntity);
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					}
					#endif

					currentRelationInList2 = currentRelationInList2->next;
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
void defineSubstanceConcepts(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			int thingIndex = i;		
			if(featureArrayTemp[thingIndex]->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
			{
				GIAEntityNode * thingEntity = GIAEntityNodeArray[thingIndex];
			
				#ifdef GIA_TRANSLATOR_DEBUG				
				//cout << "defineSubstancesBasedOnDeterminatesOfDefinitionEntities(): RELATION_TYPE_APPOSITIVE_OF_NOUN" << endl;
				#endif

				bool thingFeatureHasDeterminate = false;
				for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
				{
					if(featureArrayTemp[thingIndex]->grammaticalNumber == referenceTypeHasDeterminateCrossReferenceNumberArray[j])	//changed from GIAConceptNodeArray to featureArrayTemp 14 July 2012b
					{
						thingFeatureHasDeterminate = true;
					}
				}			

				bool thingFeatureIsProperNoun = featureArrayTemp[thingIndex]->grammaticalIsProperNoun;
				bool thingIsDefinite =  featureArrayTemp[thingIndex]->grammaticalIsDefinite;

				if(!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)
				{
					//cout << "thingEntity->entityName = " << thingEntity->entityName << endl;			
					//cout << "(!thingFeatureHasDeterminate && !thingIsDefinite && !thingFeatureIsProperNoun)" << endl;
					thingEntity->isSubstanceConcept = true;
				}
				if(featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation)
				{
					//cout << "thingEntity->entityName = " << thingEntity->entityName << endl;
					//cout << "(featureArrayTemp[thingIndex]->mustSetIsSubstanceConceptBasedOnApposRelation)" << endl;			
					thingEntity->isSubstanceConcept = true;
				}
				#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS_AND_PROPERNOUNS
				bool thingFeatureIsPronoun = false;
				if(featureArrayTemp[thingIndex]->grammaticalIsPronoun == GRAMMATICAL_PRONOUN)
				{
					thingFeatureIsPronoun = true;	
				}			
				if(thingFeatureIsPronoun)
				{
					//cout << "thingEntity->entityName = " << thingEntity->entityName << endl;
					//cout << "thingFeatureIsPronoun" << endl;
					thingEntity->isSubstanceConcept = true;
				}
				if(thingFeatureIsProperNoun)
				{
					//cout << "thingEntity->entityName = " << thingEntity->entityName << endl;
					//cout << "thingFeatureIsPronoun" << endl;
					thingEntity->isSubstanceConcept = true;
				}	
				#endif
			}		
		}
	}
}
#endif




