/*******************************************************************************
 *
 * File Name: GIATranslatorDefineProperties.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1f 26-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors conceptEntityNodesList/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslatorDefineProperties.h"
#include "GIATranslatorOperations.h"
#include "GIAdatabase.h"






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
				disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList->relationDependentIndex]);
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
			//cout << "here1" << endl;
			//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

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
							//cout << "here1" << endl;
							//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;

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
									disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationGovernorIndex]);
									disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[currentRelationInList2->relationDependentIndex]);

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

void definePropertiesObjectsAndSubjectsWithProperties(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{	
		if(GIAEntityNodeArrayFilled[i])
		{
			if(((GIAEntityNodeArray[i]->isObjectTemp) && (GIAEntityNodeArray[i]->hasPropertyTemp)) || ((GIAEntityNodeArray[i]->isSubjectTemp) && (GIAEntityNodeArray[i]->hasPropertyTemp)))
			{
				addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);	
			}
		}
	}
}	

void definePropertiesDefiniteNouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeIsDateOrStanfordTime[], bool GIAEntityNodeIsAReference[])
{
	if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS == 1)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{ //condition required as GIAEntityNodeArrayFilled is not always true. With GRAMMATICAL_DEFINITE, eg "Mr" of "Mr Smith" will still be interpreted as a definite
				if(!GIAEntityNodeIsAReference[i])
				{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
					if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS || !GIAEntityNodeGrammaticalIsProperNounArray[i])	//&& !GIAEntityNodeIsDateOrStanfordTime[i]
					{
						if(GIAEntityNodeGrammaticalIsDefiniteArray[i] == GRAMMATICAL_DEFINITE)
						{
							//cout << "as0" << endl;
							//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;			
							addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
						}
					}
				}
			}
		}
	}
}

void definePropertiesNounsWithDeterminates(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeIsDateOrStanfordTime[], bool GIAEntityNodeIsAReference[])
{	
	if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES == 1)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{	
			if(GIAEntityNodeArrayFilled[i])
			{
				if(!GIAEntityNodeIsAReference[i])
				{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)
					if(!GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS || !GIAEntityNodeGrammaticalIsProperNounArray[i])	//&& !GIAEntityNodeIsDateOrStanfordTime[i]
					{
						bool passed = false;
						for(int j=0; j<GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES; j++)
						{
							if(GIAEntityNodeArray[i]->grammaticalNumberTemp == referenceTypeHasDeterminateCrossReferenceNumberArray[j])
							{
								passed = true;
							}
						}
						if(passed)
						{
							//cout << "as1" << endl;
							addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);
						}
					}
				}
			}
		}
	}
}		

void definePropertiesNounsWithAdjectives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType)
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
					//create a new property for the entity and assign a property definition entity if not already created
					string thingName = currentRelationInList->relationGovernor;
					string propertyName = currentRelationInList->relationDependent; 
					int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
					int relationDependentIndex = currentRelationInList->relationDependentIndex;


					GIAEntityNode * thingEntity = GIAEntityNodeArray[relationGovernorIndex];
					GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];

					//cout << "as2" << endl;

					addPropertyToPropertyDefinition(thingEntity);			
				}
			}			
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif
		currentRelationInList = currentRelationInList->next;
	}				
}	

void definePropertiesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
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
				//create a new property for the entity and assign a property definition entity if not already created
				int relationGovernorIndex = currentRelationInList->relationGovernorIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationGovernorIndex];

				//cout << "as3" << endl;

				addPropertyToPropertyDefinition(propertyEntity);					
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	

		currentRelationInList = currentRelationInList->next;
	}
}	

void definePropertiesQuantityModifiers(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
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
				//create a new property for the entity and assign a property definition entity if not already created
				int relationDependentIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];

				//cout << "as3" << endl;
				addPropertyToPropertyDefinition(propertyEntity);					
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		
		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesExpletives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
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
				//create property definition
				int relationDependentIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];

				#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
				//cout << "as4" << endl;
				addPropertyToPropertyDefinition(propertyEntity);	
				#else
				disableEntityBasedUponFirstSentenceToAppearInNetwork(propertyEntity);
				#endif				
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif				
		currentRelationInList = currentRelationInList->next;
	}
}

void definePropertiesPronouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[])
{		
	if(GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_PRONOUNS == 1)
	{
		for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
		{
			if(GIAEntityNodeArrayFilled[i])
			{
				if(!GIAEntityNodeIsAReference[i])
				{//do not define properties based upon references (as the grammatical information is no longer correct, and it has already been done previously if necessary to the referenced entity)

					if(GIAEntityNodeGrammaticalIsPronounArray[i] == GRAMMATICAL_PRONOUN)
					{
						//cout << "as5" << endl;
						//cout << "asd" << endl;
						//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;			
						addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);			
					}
				}
			}
		}
	}
}

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
void definePropertiesNonExplicitPronouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])
{		
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAEntityNodeArrayFilled[i])
		{
			bool passed = false;
			for(int j=0; j<RELATION_TYPE_TREAT_AS_PRONOUN_IE_PROPERTY_NUMBER_OF_TYPES; j++)
			{
				//cout << "GIAEntityNodeArray[i]->entityName = " << GIAEntityNodeArray[i]->entityName << endl;
				//cout << "relationTypeTreatAsPronounIeProperty[j] = " << relationTypeTreatAsPronounIeProperty[j] << endl;
				
				if(GIAEntityNodeArray[i]->entityName == relationTypeTreatAsPronounIeProperty[j])
				{
					passed = true;
				}
			}
			
			if(passed)
			{
				addPropertyToPropertyDefinition(GIAEntityNodeArray[i]);	
			}
		}
	}
}
#endif


void definePropertiesToBe(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
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
		
				//create a new property for the entity and assign a property definition entity if not already created
				int relationDependentIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * propertyEntity = GIAEntityNodeArray[relationDependentIndex];

				addPropertyToPropertyDefinition(propertyEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	

		//cout << "as3" << endl;

		currentRelationInList = currentRelationInList->next;
	}	
}

void definePropertiesToDo(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[])
{
	//cout << "0i pass; define properties (to_do);" << endl;
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{			
		#endif							
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
			{
		
				//create a new property for the entity and assign a property definition entity if not already created
				int relationDependentIndex = currentRelationInList->relationDependentIndex;
				GIAEntityNode * actionEntity = GIAEntityNodeArray[relationDependentIndex];

				addActionToActionDefinition(actionEntity);
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}			
		#endif	
		//cout << "as3" << endl;

		currentRelationInList = currentRelationInList->next;
	}			
}	
							
void definePropertiesHasTime(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[])	
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{	
		//cout << "w = " << w << endl;

		if(GIAEntityNodeArrayFilled[w])
		{
			if(GIAEntityNodeArray[w]->hasAssociatedTime)
			{
				GIAEntityNode * currentGIAEntityNode = GIAEntityNodeArray[w];
				//cout << "currentGIAEntityNode->entityName = " << currentGIAEntityNode->entityName << endl;

				addPropertyToPropertyDefinition(currentGIAEntityNode);			
			}
		}
	}
}			
