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
 * File Name: GIAtranslatorRedistributeStanfordRelations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1r10b 28-November-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIAtimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslatorRedistributeStanfordRelations.h"
#include "GIAtranslatorOperations.h"
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#include "GIAlrp.h"
#endif


#ifdef GIA_USE_STANFORD_CORENLP
void disableRedundantNodesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eliminate all redundant date relations eg num(December-4, 3rd-5)/num(December-4, 1990-7)/nn(3rd-5, December-4)/appos(3rd-5, 1990-7), where both the governer and the dependent have NER tag set to DATE

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			GIAentityNode * governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode * dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

			bool governerAndDependentBothHaveSameNERvalue = false;
			for(int i=0; i<FEATURE_NER_EXPLICIT_NUMBER_TYPES; i++)
			{
				if((governerEntity->NERTemp == featureNERexplicitTypeArray[i]) && (dependentEntity->NERTemp == featureNERexplicitTypeArray[i]))
				{
					governerAndDependentBothHaveSameNERvalue = true;
				}
			}

			//if(((governerEntity->NERTemp == FEATURE_NER_DATE) && (dependentEntity->NERTemp == FEATURE_NER_DATE)) || ((governerEntity->NERTemp == FEATURE_NER_MONEY) && (dependentEntity->NERTemp == FEATURE_NER_MONEY)) || ((governerEntity->NERTemp == FEATURE_NER_NUMBER) && (dependentEntity->NERTemp == FEATURE_NER_NUMBER)) || ((governerEntity->NERTemp == FEATURE_NER_TIME) && (dependentEntity->NERTemp == FEATURE_NER_TIME)))
			if(governerAndDependentBothHaveSameNERvalue)
			{

				bool featureNERindicatesNameConcatenationRequired = false;
				for(int i=0; i<FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_TYPES; i++)
				{
					if(governerEntity->NERTemp == featureNERindicatesNameConcatenationRequiredTypeArray[i])
					{
						featureNERindicatesNameConcatenationRequired = true;
					}
				}

				//if((governerEntity->NETTemp == FEATURE_NER_PERSON) || (governerEntity->NETTemp == FEATURE_NER_LOCATION) || (governerEntity->NETTemp == FEATURE_NER_ORGANIZATION) || (governerEntity->NETTemp == FEATURE_NER_MISC))
				if(featureNERindicatesNameConcatenationRequired)
				{
					bool featureNERindicatesNameConcatenationRequiredAllowedByPOS = false;
					#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP
					if((dependentEntity->stanfordPOStemp == FEATURE_POS_TAG_NNP) && (governerEntity->stanfordPOStemp == FEATURE_POS_TAG_NNP))
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
					
					currentRelationInList->disabled = true;

					disableEntity(dependentEntity);

				}

			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}
#endif




#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS

void disableRedundantNodesStanfordParser(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eliminate redundant complementizer 'that' nodes eg 'The authors believe that their method...' / _complm(identify[8], that[4])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_COMPLEMENTIZER)
			{
				if(currentRelationInList->relationDependent == RELATION_DEPENDENT_THAT)
				{
					GIAentityNode * dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

					currentRelationInList->disabled = true;
					disableEntity(dependentEntity);

				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{

	Relation * currentRelationInList;
	
#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_ACTION_PREPOSITION_ACTION
	//added 28 October 2012
	redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
#else	
	//for queries only (1j6h)
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP

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

		Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
			if(!(currentRelationInList->disabled))
			{
			#endif
				if(currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO)
				{
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

 										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
														currentRelationInList3->relationDependentIndex = currentRelationInList2->relationDependentIndex;
														currentRelationInList3->relationDependent = currentRelationInList2->relationDependent;

													}
												}
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											}
											#endif

											currentRelationInList3 = currentRelationInList3->next;
										}
									}
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

	#endif

#endif
		/*
		What are the patent claims on?
		match dep with prep
			dep(are-2, What-1)/ nsubj(are-2, claims-5) / prep(are-2, on-6):
				dep(are-2, What-1) / prep(are-2, on-6) => prep_on(are-2, What-1)
					[then rely upon redistributeStanfordRelationsMultiwordPreposition() for  nsubj(are-4, claims-3) / prep_on(are-4, frame-8) => prep_on(claims-3, frame-8)]
		*/

	//for queries only (1j6g)
	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP
		currentRelationInList = currentSentenceInList->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
			if(!(currentRelationInList->disabled))
			{
			//#endif
				if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
				{
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationDependent = currentRelationInList->relationDependent;

										disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
									}
								}
							}
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						//#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}
			//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
			}
			//#endif
			currentRelationInList = currentRelationInList->next;
		}
	#endif


#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION

	/*
	need to consider this case for the following example text;
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

	//look for nsubj/prep combination, eg nsubj(are-4, claims-3) / prep_on(are-4, frame-8) => prep_on(claims-3, frame-8)
	//case added 15 May 2012 for GIA_USE_ADVANCED_REFERENCING; The claims that are on the frame are blue. , nsubj(are-4, claims-2) / prep_on(are-4, frame-7) / rcmod(claims-2, are-4)

	//OLD: look for nsubj/prep combination, eg nsubj(next-4, garage-2) / prep_to(next-4, house-7)	=> prep_subj(next_to, house) / prep_subj(next_to, garage)

	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						bool stanfordPrepositionFound = false;
						string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

						if(stanfordPrepositionFound)
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
									
									Relation * subjectOfPrepositionRelation = currentRelationInList;
									Relation * objectOfPrepositionRelation = currentRelationInList2;
									subjectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION;
									objectOfPrepositionRelation->relationType = RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION;
									GIAentityNodeArray[currentRelationInList2->relationGovernorIndex]->entityName = newPrepositionName;
									currentRelationInList2->prepositionCombinationAlreadyCreatedTemp = true;
								}
								#else
									if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
									{
										#ifdef GIA_USE_ADVANCED_REFERENCING
										/*eg;
										[case added 15 May 2012 for GIA_USE_ADVANCED_REFERENCING]
										The claims that are on the frame are blue.
										nsubj(are-4, claims-2)
										prep_on(are-4, frame-7)
										rcmod(claims-2, are-4)
										*/
											//NB prepositions imply same reference set by default: "The claims that are on the frame are blue." = "The claims on the frame are blue."
										bool auxillaryIndicatesDifferentReferenceSet = true;
										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
											{
												if((currentRelationInList3->relationDependentIndex == currentRelationInList->relationGovernorIndex) && (currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex) && (currentRelationInList3->relationDependent == RELATION_ENTITY_BE))
												{
													auxillaryIndicatesDifferentReferenceSet = false;
												}
											}
											currentRelationInList3 = currentRelationInList3->next;
										}
										currentRelationInList2->auxillaryIndicatesDifferentReferenceSet = auxillaryIndicatesDifferentReferenceSet;
										#ifdef GIA_ADVANCED_REFERENCING_DEBUG
										//cout << "\t\t\t0currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
										//cout << "\t\t\t0auxillaryIndicatesDifferentReferenceSet = " << auxillaryIndicatesDifferentReferenceSet << endl;
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
	prep_to(close-3, house-6)

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


	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool stanfordPrepositionFound = false;
			string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

			if(stanfordPrepositionFound)
			{
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
					//NB this assumes "cop/aux" etc relations cannot be disabled in fillGrammaticalArraysStanford

						bool multiwordPrepositionIntermediaryRelationTypeAFound = false;
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
 							Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

															#ifdef GIA_USE_ADVANCED_REFERENCING
															//[case added 15 May 2012 for GIA_USE_ADVANCED_REFERENCING]
															//He rode the carriage that is near to the horse.
															//nsubj(near-7, carriage-4)
															//cop(near-7, is-6)
															//rcmod(carriage-4, near-7)
															//prep_to(near-7, horse-10)
															bool auxillaryIndicatesDifferentReferenceSet = true;
															Relation * currentRelationInList4 = currentSentenceInList->firstRelationInList;
															while(currentRelationInList4->next != NULL)
															{
																if(currentRelationInList4->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
																{
																	if((currentRelationInList4->relationDependentIndex == currentRelationInList3->relationGovernorIndex) && (currentRelationInList4->relationGovernorIndex == currentRelationInList3->relationDependentIndex))		//OLD: before 1 June 2012 code review: if((currentRelationInList4->relationDependentIndex == currentRelationInList->relationGovernorIndex) && (currentRelationInList4->relationGovernorIndex == currentRelationInList->relationDependentIndex))
																	{
																		auxillaryIndicatesDifferentReferenceSet = false;
																	}
																}
																currentRelationInList4 = currentRelationInList4->next;
															}
															#ifdef GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILLARY_AND_SET_DIFFERENT_REFERENCE_SET
															//need to reset same reference set values on the preposition

															currentRelationInList->auxillaryIndicatesDifferentReferenceSet = auxillaryIndicatesDifferentReferenceSet;
															
															#ifdef GIA_ADVANCED_REFERENCING_DEBUG
															//currentRelationInList3->auxillaryIndicatesDifferentReferenceSet = auxillaryIndicatesDifferentReferenceSet;
															//cout << "\t\t\t1currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
															//cout << "\t\t\t1currentRelationInList->auxillaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxillaryIndicatesDifferentReferenceSet << endl;
															#endif

															#endif
															#endif


															GIAentityNode * entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationGovernorIndex];

															string newPrepositionName = "";
															newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

															currentRelationInList->relationType = newPrepositionName;
															currentRelationInList->prepositionCombinationAlreadyCreatedTemp = true;

															currentRelationInList->relationGovernorIndex = currentRelationInList3->relationDependentIndex;
															currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->entityName;

															#ifdef GIA_ADVANCED_REFERENCING_DEBUG
															//cout << "redistributeStanfordRelationsMultiwordPreposition(): newPrepositionName = " << newPrepositionName << endl;															
															//cout << "\t\t\t2currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
															//cout << "\t\t\t2auxillaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxillaryIndicatesDifferentReferenceSet << endl;
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

														GIAentityNode * entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
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

														GIAentityNode * entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
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

														GIAentityNode * entityContainingFirstWordOfMultiwordPreposition = GIAentityNodeArray[currentRelationInList3->relationDependentIndex];

														string newPrepositionName = "";
														newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + entityContainingFirstWordOfMultiwordPreposition->entityName + STANFORD_PARSER_PREPOSITION_DELIMITER + relexPreposition;

														currentRelationInList->relationType = newPrepositionName;
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
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
								}
								#endif

								currentRelationInList3 = currentRelationInList3->next;
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



#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
void redistributeStanfordRelationsInterpretOfAsPossessive(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg The ball of the red dog is green..   prep_of(ball-2, dog-6) ->  poss(ball-5, dog-3)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		//#endif

			bool stanfordPrepositionFound = false;
			string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);

			if(stanfordPrepositionFound)
			{
				if(relexPreposition == GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_OF)
				{
					#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
					cout << "DEBUG: redistributeStanfordRelationsInterpretOfAsPossessive(): GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_OF" << endl;
					#endif
					currentRelationInList->relationType = RELATION_TYPE_POSSESSIVE;
				}
			}

		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
}
#endif


#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
void redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	/*interpret;
		[given 'The name of the dog near the farm is Max.']
		'What is the name of the red dog near the farm?' [return entity names]
			nsubj(is-2, name-4) / attr(is-2, What-1) {/ det(name-4, the-3)} / poss/prep_of(name-4, dog-8) -> appos(dog[8], _$qVar[1])	{_name(dog[8], _$qVar)??}
		'What is the number of the red dogs near the farm?' [return entity number/quantity]
			nsubj(is-2, number-4) / attr(is-2, What-1) {/ det(number-4, the-3)} / poss/prep_of(number-4, dogs-8) -> _quantity(dog[8], _$qVar[1])
	*/

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
 											Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
											while(currentRelationInList3->next != NULL)
											{
												#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
												if(!(currentRelationInList2->disabled))
												{
												#endif

													#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
													if(currentRelationInList3->relationType == RELATION_TYPE_POSSESSIVE)
													#else
													bool stanfordPrepositionFound = false;
													string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList3->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);
													if((currentRelationInList3->relationType == RELATION_TYPE_POSSESSIVE) || (stanfordPrepositionFound && (relexPreposition == GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_OF)))
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

																	GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
																	disableEntity(oldRedundantBeEntity);
																	GIAentityNode * oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
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

																	GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
																	disableEntity(oldRedundantBeEntity);
																	GIAentityNode * oldRedundantNumberEntity = GIAentityNodeArray[currentRelationInList3->relationGovernorIndex];
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
												#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
												}
												#endif
												currentRelationInList3 = currentRelationInList3->next;
											}
										}
									}
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				/*
				}
				*/
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}
#endif

#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
void redistributeStanfordRelationsInterpretNameOfAsDefinition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg interpret 'The red dog's name is Max.'		nsubj(Max-7, name-5)  poss(name-5, dog-3) -> appos(dog-3, Max-7)
	//eg interpret 'The name of the red dog is Max.'	nsubj(Max-7, name-5)  prep_of(name-5, dog-3) -> appos(dog-3, Max-7)
	//eg interpret 'Max is the name of the red dog.' 	nsubj(name-4, Max-1)  prep_of(name-4, dog-8) -> appos(dog-3, Max-7)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{
				if((currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME) || (currentRelationInList->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME))
				{
					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
					while(currentRelationInList2->next != NULL)
					{
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						if(!(currentRelationInList2->disabled))
						{
						//#endif
							#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
							if(currentRelationInList2->relationType == RELATION_TYPE_POSSESSIVE)
							#else
							bool stanfordPrepositionFound = false;
							string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList2->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);
							if((currentRelationInList2->relationType == RELATION_TYPE_POSSESSIVE) || (stanfordPrepositionFound && (relexPreposition == GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_OF)))
							#endif
							{
								if(currentRelationInList2->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)	//added 11 August 2012
								{
									if(currentRelationInList->relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)
									{
										#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
										cout << "DEBUG: redistributeStanfordRelationsInterpretNameOfAsDefinition(): relationDependent == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME" << endl;
										#endif

										currentRelationInList->disabled =  true;

										GIAentityNode * oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
										disableEntity(oldRedundantNameEntity);

										currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationGovernorIndex;
										currentRelationInList2->relationDependent = currentRelationInList->relationGovernor;
									}
									else if(currentRelationInList->relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME)
									{
										#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
										cout << "DEBUG: redistributeStanfordRelationsInterpretNameOfAsDefinition(): relationGovernor == GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME" << endl;
										#endif

										currentRelationInList->disabled =  true;

										GIAentityNode * oldRedundantNameEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
										disableEntity(oldRedundantNameEntity);

										currentRelationInList2->relationType = RELATION_TYPE_APPOSITIVE_OF_NOUN;
										currentRelationInList2->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList2->relationGovernor = currentRelationInList2->relationDependent;
										currentRelationInList2->relationDependentIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationDependent = currentRelationInList->relationDependent;
									}
								}
							}
						//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						//#endif
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



void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser)
{
	//eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6)
	//OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > nsubj(away-6, rabbit-2) )
		//case added 15 May 2012 for GIA_USE_ADVANCED_REFERENCING; nsubj(is-4, rabbit-2) / advmod(is-4, away-7) / rcmod(rabbit-2, is-4) -> _predadj(rabbit-2, away-7)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
						{
							if((currentRelationInList->relationGovernor == RELATION_ENTITY_BE) && (currentRelationInList2->relationGovernor == RELATION_ENTITY_BE))
							{
								if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
								{//found a matching object-subject relationship

									GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

									disableEntity(oldRedundantBeEntity);

									#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
										#ifdef GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
										//CASE CURRENTLY ENABLED

										#ifdef GIA_USE_ADVANCED_REFERENCING
										//case added 15 May 2012 for GIA_USE_ADVANCED_REFERENCING;
										The rabbit that is 20 meters away is fat.
										nsubj(is-4, rabbit-2)
										advmod(is-4, away-7)
										rcmod(rabbit-2, is-4)
										*/
										bool auxillaryIndicatesDifferentReferenceSet = true;
										currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											if(currentRelationInList3->relationType == RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER)
											{
												if((currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex) && (currentRelationInList3->relationDependent == RELATION_ENTITY_BE))
												{
													auxillaryIndicatesDifferentReferenceSet = false;
												}
											}
											currentRelationInList3 = currentRelationInList3->next;
										}
										currentRelationInList2->auxillaryIndicatesDifferentReferenceSet = auxillaryIndicatesDifferentReferenceSet;	//was currentRelationInList before 1 June 2012 code check
										#ifdef GIA_ADVANCED_REFERENCING_DEBUG
										//cout << "\t\t\t3currentRelationInList2->relationType = " << currentRelationInList2->relationType << endl;
										//cout << "\t\t\t3currentRelationInList2->auxillaryIndicatesDifferentReferenceSet = " << currentRelationInList2->auxillaryIndicatesDifferentReferenceSet << endl;
										#endif
										#endif

										currentRelationInList2->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList2->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList2->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;

										currentRelationInList->disabled =  true;
										#else
										//CASE CURRENTLY NOT ENABLED
										currentRelationInList->relationType = RELATION_TYPE_ADJECTIVE_PREDADJ;
										currentRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName;
										currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationDependent = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled =  true;
										#endif
									#else
										//CASE CURRENTLY NOT DISABLED
										currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationGovernor = GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName;

										currentRelationInList2->disabled = true;
									#endif
								}
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


	//eg1 Kane is late 		nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3) 				[NB non-determinate of governer and dependent of subject relation; take as indicator of substance]
	//or
	//eg2 She is the one 		nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4)		[NB determinate of dependent of subject relation; take as an indicator of definition]
	//or
	//eg3 The girl is tall 		nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1) -> _predadj(girl-2, tall-6) 	[NB non-plural and determinate of governer of subject relation; take as indicator of substance]
	//or
	//eg4 bikes are machines  	nsubj(machines-3, bikes-1) / cop(machines-3, are-2) -> appos(bikes-1, machines-3)		[NB plural and non-determinate of governer of subject relation; take as an indicator of definition]
	//or
	//eg5 the wheels are green  	nsubj(green-6, wheels-4) / cop(green-6, are-5) -> _predadj(wheels-4, green-6)			[NB plural and determinate of governer of subject relation; take as indicator of substance]
	//or
	//eg6 That is Jim.   		nsubj(Jim-3, That-1) / cop(Jim-3, is-2) -> appos(That-1, Jim-3)
	//or
	//eg7 The time is 06:45		nsubj(06:45-4, time-2) / cop(06:45-4, is-3) / det(time-2, The-1) -> appos(time-2, 06:45-4)
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT)
			{

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

									#ifdef GIA_USE_ADVANCED_REFERENCING
									currentRelationInList->auxillaryIndicatesDifferentReferenceSet = true;
									#ifdef GIA_ADVANCED_REFERENCING_DEBUG
									//cout << "\t\t\t4currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
									//cout << "\t\t\t4currentRelationInList->auxillaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxillaryIndicatesDifferentReferenceSet << endl;
									#endif
									#endif

									#ifndef GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION
										#ifndef GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD
										if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
										{
											GIAentityNode * subjectGovernorEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];

											bool subjectGovernorAdjectiveOrAdvebFound = false;
											for(int i=0; i<FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_TYPES; i++)
											{
												if(subjectGovernorEntity->stanfordPOStemp == featurePOSindicatesAdjectiveOrAdverbTypeArray[i])
												{
													subjectGovernorAdjectiveOrAdvebFound = true;
												}
											}
											bool subjectGovernorNounFound = false;
											for(int i=0; i<FEATURE_POS_TAG_INDICATES_NOUN_NUMBER_TYPES; i++)
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
 										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{

											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											if(!(currentRelationInList3->disabled))
											{
											#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_DETERMINER)
												{
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationGovernorIndex)	//redundant test
													{//found a matching object-subject relationship
														//eg she is the one	nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3)
														detectedDeterminateOfSubjectGoverner = true;
														detectedDeterminate = true;
													}
													if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)	//redundant test
													{//found a matching object-subject relationship
														//eg the girl is tall	nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1)
														detectedDeterminateOfSubjectDependent = true;
														detectedDeterminate = true;

														//eg the wheel is green		nsubj(green-6, wheels-4) / cop(green-6, are-5)
													}
												}
											#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
											GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
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


void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg	The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as) -> prep_as (happen, fall)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER)
			{

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

								GIAentityNode * oldPrepositionEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];

								//currentRelationInList2->disabled = true;
								//disableEntity(oldPrepositionEntity);

								string newRelationType = "";
								newRelationType = newRelationType + STANFORD_PARSER_PREPOSITION_PREPEND + oldPrepositionEntity->entityName;
								#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
								cout << "DEBUG: redistributeStanfordRelationsAdverbalClauseModifierAndComplement();" << endl;
								cout << "newRelationType = " << newRelationType << endl;
								#endif
								currentRelationInList->relationType = newRelationType;
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


void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg	What she said makes sense. 	csubj (make, say) / dobj ( said-3 , What-1 )

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_OBJECT)
			{

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg	I eat a pie or tom rows the boat. 	cc(pie-4, or-5)  / conj(pie-4, tom-6)

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_CONJUNCT)
			{

 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
								GIAentityNode * coordinationDependentEntity = GIAentityNodeArray[currentRelationInList2->relationDependentIndex];
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
									cout << "error redistributeStanfordRelationsConjunctionAndCoordinate(): unknown coordination dependent - " << coordinationDependent << endl;
								}
								currentRelationInList2->relationType = newRelationType;

								currentRelationInList->disabled = true;
								disableEntity(coordinationDependentEntity);
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

void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg	 The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5) / num($-5, 1000-6)	[Relex: _obj(win[3], $[5])   / _quantity_mod($[5], almost[4])] -> _obj(win[3], $[5]) /  _quantity_mod($[5], almost[4])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_ADJECTIVE_ADVMOD)
			{
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching object-subject relationship

 								Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
								while(currentRelationInList3->next != NULL)
								{
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
									#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
									}
									#endif
									currentRelationInList3 = currentRelationInList3->next;
								}
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


void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			//eg1	years old - npadvmod(old, years) -> _measure_time(old[7], years[6])		   {IRRELEVANT years: <NER>NUMBER</NER>} + old: <NER>DURATION</NER>
			//eg2	meters away - npadvmod(away-6, meters-5) -> _measure_distance(away[6], meter[5])

			if(currentRelationInList->relationType == RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER)
			{
				GIAentityNode * governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAentityNode * dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

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

			//eg3 dep(times-4, day-6) -> measure_dependency(times-4, day-6)			{Relex: _measure_per(times[4], day[6])}

			if(currentRelationInList->relationType == RELATION_TYPE_DEPENDENT)
			{
				GIAentityNode * governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
				GIAentityNode * dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

				if(dependentEntity->NERTemp == FEATURE_NER_DURATION)
				{
					currentRelationInList->relationType = RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN;
				}
				else
				{
					//do nothing
				}
			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}

void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_PHRASAL_VERB_PARTICLE)
			{
				bool foundTemporalModifierOrObjectOfPreposition = false;

				#ifdef GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_TEMPORAL_MODIFIER
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_TEMPORAL_MODIFIER)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationGovernorIndex)
							{//found a matching relationship

								//The disaster happened over night.   prt(happened-3, over-4) / tmod(happened-3, night-5) -> over(happened-3, night-5)

								currentRelationInList->disabled = true;
								GIAentityNode * oldPreposition = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
								string newPrepositionName = "";
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + currentRelationInList->relationDependent;	//oldPreposition->entityName
								currentRelationInList2->relationType = newPrepositionName;
								foundTemporalModifierOrObjectOfPreposition = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION)
						{
							if(currentRelationInList2->relationGovernorIndex == currentRelationInList->relationDependentIndex)
							{//found a matching relationship

								//What does the red laser work on?	prt(work-6, on-7) / pobj(on-7, What-1) -> on(work-6, What-1)	//case added 7 August 2012b

								currentRelationInList->disabled = true;
								GIAentityNode * oldPreposition = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
								string newPrepositionName = "";
								newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + currentRelationInList->relationDependent;	//oldPreposition->entityName
								currentRelationInList2->relationType = newPrepositionName;
								currentRelationInList2->relationGovernor =  currentRelationInList->relationGovernor;
								currentRelationInList2->relationGovernorIndex = currentRelationInList->relationGovernorIndex;

								foundTemporalModifierOrObjectOfPreposition = true;
							}
						}
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

						GIAentityNode * governerEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
						GIAentityNode * dependentEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];
						governerEntity->entityName = governerEntity->entityName + "_" + dependentEntity->entityName;

						disableEntity(dependentEntity);

						//currentRelationInList->disabled = true;
						//currentRelationInList->relationGovernor = governerEntity->entityName;
					}
				}
				#endif

			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void redistributeStanfordRelationsCreateQueryVars(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1z10a; redistribute Stanford Relations - Create Query Vars (eg interpret 'who is that' / 'what is the time.'  attr(is-2, Who-1) / attr(is-2, What-1)" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsWhoWhat(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

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
	//this is now done at the beginning of GIAtranslatorRedistributeStanfordRelations.cpp
	#ifdef GIA_TRANSLATOR_DEBUG
	//added 28 October 2012
	cout << "pass 1z10f; redistribute Stanford Relations - query variables connected to action-preposition-action (eg interpret 'Where are apples used for eating?'  _%atLocation(use[4], _$qVar[1]) + prepc_for(use[4], eat[6]) -> _%atLocation(eat[6], _$qVar[1]) )" << endl;
	#endif
	redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	*/
}

void redistributeStanfordRelationsCreateQueryVarsWhoWhat(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//interpret; 'who is that' / 'what is the time.'  attr(is-2, What-1) nsubj(is-2, time-4) -> appos(time-4, _$qVar-1)   /   attr(is-2, Who-1)  nsubj(is-2, that-3) -> appos(That-3, _$qVar-1)	[NB _$qVar can be switched in both cases with respect to GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY]

	//interpret; 'Who rode the bike?' / 'What broke the glass?' -> nsubj(rode-2, Who-1) -> nsubj(rode-2, _$qVar-1) / nsubj(broke-2, What-1) -> nsubj(broke-2, _$qVar-1) [added 7 August 2012]

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

											//interpret 'who is that'  nsubj(is-2, time-4) / attr(is-2, What-1)  -> appos(time-4, _$qVar-1), 'what is the time.' nsubj(is-2, that-3) attr(is-2, Who-1)   -> appos(That-3, _$qVar-1)	[NB _$qVar can be switched in both cases with respect to GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY]

											#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
											cout << "redistributeStanfordRelationsCreateQueryVarsWhoWhat" << endl;
											#endif

										#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY

											currentRelationInList->disabled =  true;
											GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
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
											GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
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



void redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//Create Query Vars (eg interpret 'how much'/'how many' advmod(much-2, How-1) amod(milk-3, much-2) -> _quantity(milk[3], _$qVar[2]) )

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
								GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationDependentIndex];	//disable how
								disableEntity(oldRedundantBeEntity);

								currentRelationInList2->relationType = RELATION_TYPE_QUANTITY;
								currentRelationInList2->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
								GIAentityNodeArray[currentRelationInList2->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "much" to _$qVar
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

void redistributeStanfordRelationsCreateQueryVarsWhich(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//nothing is required here: see identifyComparisonVariableAlternateMethod()
}

void redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//interpret  'How did the disaster happen?' / 'When should they leave?' / 'Where is the ball?' / 'Why does the star fall?' --> advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) [nsubj(is-2, ball-4)] / advmod(fall-5, Why-1) -> how(happen[5], _$qVar[1]) / _%atTime(leave[4], _$qVar[1]) / _%atLocation(ball[4], _$qVar[1]) / _%because(fall[5], _$qVar[1])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
 					Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;

					while(currentRelationInList2->next != NULL)
					{
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
										GIAentityNode * oldRedundantBeEntity = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];	//disable is
										disableEntity(oldRedundantBeEntity);

										currentRelationInList->relationType = featureQueryWordHowWhenWhereWhySpecialCaseCrossReferenceQueryVariableNameArray[queryWordHowWhenWhereWhySpecialCaseFoundIndex];		//_%atLocation
										currentRelationInList->relationGovernorIndex = currentRelationInList2->relationDependentIndex;
										currentRelationInList->relationGovernor = currentRelationInList2->relationDependent;

										GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "where" to _$qVar
										currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
									}
								}
							}
						#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
						}
						#endif
						currentRelationInList2 = currentRelationInList2->next;
					}
				}


				if(!queryHowWhenWhereWhySpecialCaseRelationDependentFound || !foundMatchForSpecialCase)
				{//how, when, why
				*/
				//the above commented out code is not required, because _predadj should be generated in this case in accordance with; redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(): advmod(is-3, where-6) / nsubj(is-3, rabbit-2) - > _predadj(rabbit-2, where-6)

					/* no longer required as GIA_DO_NOT_PARSE_DISABLED_RELATIONS has been used here
					if(currentRelationInList->relationGovernor != RELATION_ENTITY_BE)
					{//check is used to prevent a situation in the case a disabled relation has been used [NB should really utilise GIA_DO_NOT_PARSE_DISABLED_RELATIONS here...]
					*/
					 	#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
						cout << "redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy: " << currentRelationInList->relationDependent << endl;
						#endif

						//interpret 'who is that' / 'what is the time.' advmod(happen-5, How-1) / [advmod(leave-4, When-1)?] / advmod(is-2, Where-1) / advmod(fall-5, Why-1) -> how(happen[5], _$qVar[1]) / _%atTime(leave[4], _$qVar[1]) / [_%atLocation(ball[4], _$qVar[1])?] / _%because(fall[5], _$qVar[1])

						currentRelationInList->relationType = featureQueryWordHowWhenWhereWhyCrossReferenceQueryVariableNameArray[queryWordHowWhenWhereWhyFoundIndex];		// prep_how / _%atTime / _%because

						GIAentityNodeArray[currentRelationInList->relationDependentIndex]->entityName = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;	//convert "how"/"when"/"why" to _$qVar
						currentRelationInList->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
					/*
					}
					else
					{

						//cout << "redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy() error: (!queryHowWhenWhereWhySpecialCaseRelationDependentFound || !foundMatchForSpecialCase) && (currentRelationInList->relationGovernor == RELATION_ENTITY_BE)" << endl;
						//cout << "relationType = " << currentRelationInList->relationType << endl;
						//cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
						//cout << "relationDependent = " <<currentRelationInList->relationDependent << endl;

					}
					*/
				/*
				}
				*/

			}
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		//#endif
		currentRelationInList = currentRelationInList->next;
	}
}




void redistributeStanfordRelationsCreateQueryVarsWhat(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//interpret 'what is...'  _obj(enable[5], what[1]) ->  _obj(enable[5], _$qVar[1])

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
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

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}

void redistributeStanfordRelationsPartmod(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{
	//eg Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle)
	/*
		prep_during(pick, spring)
		nsubj(tasty, truffle)
		partmod(truffle, pick)
		cop(tasty, be)
		->
		during(pick, spring)
		_predadj(truffle, tasty)
		_obj(pick, truffle)
	*/

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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

		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		#endif
		currentRelationInList = currentRelationInList->next;
	}
}


void redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
{	
	//cout << "\n" << endl;
	
	//added 28 October 2012
 	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	bool foundPreposition = false;
	while(currentRelationInList->next != NULL)
	{
		//cout << "Relation: " << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
	
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool stanfordPrepositionFound = false;
			string relexPreposition = convertStanfordPrepositionToRelex(&(currentRelationInList->relationType), GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD, &stanfordPrepositionFound);
			
			#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP
			if(stanfordPrepositionFound || (currentRelationInList->relationType == RELATION_TYPE_COMPLIMENT_TO_DO))
			#else
			if(stanfordPrepositionFound)
			#endif
			{				
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{	
					//cout << "Relation2: " << currentRelationInList2->relationType << "(" << currentRelationInList2->relationGovernor << ", " << currentRelationInList2->relationDependent << ")" << endl;
							
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
									if(!foundPreposition)
									{
										bool foundSecondPreposition = false;
 										Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
										while(currentRelationInList3->next != NULL)
										{
											//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											if(!(currentRelationInList3->disabled))
											{
											//#endif
												if(currentRelationInList3->relationType == RELATION_TYPE_PREPOSITION_MODIFIER)
												{	
													if(currentRelationInList3->relationType != currentRelationInList->relationType)
													{//ensure unique preposition found
														if(currentRelationInList3->relationGovernorIndex == currentRelationInList->relationDependentIndex)
														{
															//cout << "(foundSecondPreposition)" << endl;

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
															_obj(used[4], _$qVar[1]) + prep_in(use[4], making[7]) + _prep(making[7], of[8])	-> prep_of(making[7], _$qVar[1])
															*/

															string newPrepositionName = "";
															string relexPrepositionNew = GIAentityNodeArray[currentRelationInList3->relationDependentIndex]->entityName; 	//currentRelationInList3->relationDependent
															newPrepositionName = newPrepositionName + STANFORD_PARSER_PREPOSITION_PREPEND + relexPrepositionNew;
															currentRelationInList2->disabled = true;

															currentRelationInList3->relationType = newPrepositionName;
															currentRelationInList3->relationDependent = currentRelationInList2->relationDependent;
															currentRelationInList3->relationDependentIndex = currentRelationInList2->relationDependentIndex;																											

															foundSecondPreposition = true;
														}
													}
												}
											//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
											}
											//#endif
											currentRelationInList3 = currentRelationInList3->next;
										}

										if(!foundSecondPreposition)
										{
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
											{
												if(currentRelationInList->relationDependent == RELATION_DEPENDENT_DO)
												{	
													/*
													What is the Co-cart designed to do?
													interpret; _to-do(design[5], do[7]) + _dep(design[5], what[1]) -> _to-do(design[5], what[1])
													*/
													
													currentRelationInList2->disabled = true;
													disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);

													currentRelationInList->relationDependentIndex = currentRelationInList2->relationDependentIndex;
													currentRelationInList->relationDependent = currentRelationInList2->relationDependent;												
												}
												else
												{
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
													*/
													currentRelationInList2->relationType = RELATION_TYPE_OBJECT;												
												}
											}
											#endif
										}

										foundPreposition = true;
									}
								}
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


#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
//Added 28 October 2012b
void redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType)
{
	//eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))	//required to prevent re-interpretation of prepositions in main preposition interpretation function createConditionBasedUponPreposition
		{
		//#endif
		
			int actionOrSubstanceIndex = currentRelationInList->relationGovernorIndex;
			int actionOrSubstanceConditionIndex = currentRelationInList->relationDependentIndex;
			string relationType = currentRelationInList->relationType;
			GIAentityNode * actionOrSubstanceEntity = GIAentityNodeArray[actionOrSubstanceIndex];
			GIAentityNode * actionOrSubstanceConditionEntity = GIAentityNodeArray[actionOrSubstanceConditionIndex];		

			bool stanfordPrepositionFound = false;
			if(convertStanfordPrepositionToRelex(&relationType, NLPdependencyRelationsType, &stanfordPrepositionFound) == RELATION_TYPE_OF)
			{
				#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
				//cout << "actionOrSubstanceEntity->stanfordPOStemp = " << actionOrSubstanceEntity->stanfordPOStemp << endl;
				//cout << "actionOrSubstanceEntity->wordNetPOS = " << actionOrSubstanceEntity->wordNetPOS << endl;				
				bool foundContinuousVerb = false;
				if(actionOrSubstanceEntity->stanfordPOStemp == FEATURE_POS_TAG_VBG)
				//if(actionOrSubstanceEntity->wordNetPOS == GRAMMATICAL_WORD_TYPE_VERB)
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
					#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
					//cout << "NB: GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS requires GIA_USE_LRP to be defined and -lrpfolder to be set" << endl;					
					string baseNameFound = "";
					if(determineIfWordIsIrregularVerbContinuousCaseWrapper(actionOrSubstanceEntity->wordOrig, &baseNameFound))
					{
						//cout << "foundVerb2a" << endl;
						/*
						Wood is used for making milk.
						What is wood used in the making of?
						NB making is an irregular verb and will be tagged incorrectly by both Stanford coreNLP and Stanford Parser					
						*/	
						if(actionOrSubstanceEntity->wordOrig == actionOrSubstanceEntity->entityName)	//OR if(actionOrSubstanceEntity->entityName != baseNameFound)
						{	
							//change irregular verb name eg making to base irregular verb base name eg make
							actionOrSubstanceEntity->entityName = baseNameFound;
							currentRelationInList->relationGovernor = baseNameFound;
						}
						
						foundContinuousVerb = true;
					}
					#endif	
					
					//STANFORD_PARSER_USE_POS_TAGS is no longer supported by GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS/redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs()...
					/*
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
						//	a solution involves GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS														
						foundContinuousVerb = true;
					}
					#endif
					*/
					
				}
				
				if(foundContinuousVerb)
				{
					currentRelationInList->relationType = RELATION_TYPE_OBJECT;				
				}
				#endif
			}

		//#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		}
		//#endif

		currentRelationInList = currentRelationInList->next;
	}
}

/*
NB determineIfWordIsIrregularVerbContinuousCaseWrapper() could be used instead of determineIfWordIsVerbContinuousCase(), as Stanford CoreNLP only has a problem identifying verbs (pos tag "VBG") when they are irregular verbs. NB Stanford Parser appears to have a problem with all irregular continous verbs.
*/
bool determineIfWordIsVerbContinuousCase(string * word)
{
	bool foundVerbContinuousCase = false;
	int wordStringLength = word->length();
	//cout << "word = " << *word << endl;
	//cout << "wordStringLength = " << wordStringLength << endl;
	int wordTenseFormContinuousAppendLength = string(GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND).length();
	string lastThreeLettersOfWord = word->substr(wordStringLength-wordTenseFormContinuousAppendLength, wordTenseFormContinuousAppendLength);
	//cout << "wordTenseFormContinuousAppendLength = " << wordTenseFormContinuousAppendLength << endl;
	//cout << "lastThreeLettersOfWord = " << lastThreeLettersOfWord << endl;
	if(lastThreeLettersOfWord == GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND)
	{
		foundVerbContinuousCase = true;
	}
	return foundVerbContinuousCase;
}
#endif

#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
void redistributeStanfordRelationsExpletives(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType)
{
	//eg 'There is a place that we go' _expl(be[2], there[1]) / _subj(be[2], place[4]) / _subj(go[7], we[6]) [IRRELEVANT] / _obj(be[2], go[7]) -> _subj(go[7], we[6]) / _obj(place[4], go[7]) 
	
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
		if(!(currentRelationInList->disabled))
		{
		#endif
			if(currentRelationInList->relationType == RELATION_TYPE_SUBJECT_EXPLETIVE)
			{
 				Relation * currentRelationInList2 = currentSentenceInList->firstRelationInList;
				while(currentRelationInList2->next != NULL)
				{
					#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
					if(!(currentRelationInList2->disabled))
					{
					#endif
						if(currentRelationInList2->relationType == RELATION_TYPE_SUBJECT)
						{					
 							Relation * currentRelationInList3 = currentSentenceInList->firstRelationInList;
							while(currentRelationInList3->next != NULL)
							{
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
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
													currentRelationInList3->relationGovernorIndex = currentRelationInList2->relationDependentIndex; 
													currentRelationInList3->relationGovernor = currentRelationInList2->relationDependent; 
													currentRelationInList->disabled = true; 
													currentRelationInList2->disabled = true; 
													disableEntity(GIAentityNodeArray[currentRelationInList->relationGovernorIndex]);
													disableEntity(GIAentityNodeArray[currentRelationInList->relationDependentIndex]);
												}	
											}
										}				
									}
								#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS
								}
								#endif
								currentRelationInList3 = currentRelationInList3->next;
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
#endif


#endif


