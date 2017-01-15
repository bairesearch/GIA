/*******************************************************************************
 *
 * File Name: GIATranslatorDefineReferencing.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l4c 02-June-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING
#define HEADER_GIA_TRANSLATOR_DEFINE_REFERENCING

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"
#include "GIAConditionNodeClass.h"

void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser);				//Stanford Compatible / Relex Specific [$qvars not supported. What is the 'attr' stanford dependency relation?]
void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType);
void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType);									//Stanford Compatible	

void linkPronounReferencesRelex(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAConceptNodeArray[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_USE_STANFORD_CORENLP
void linkPronounAndTextualContextReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAConceptNodeArray[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, StanfordCoreNLPCoreference * firstCoreferenceInList, Feature * featureArrayTemp[]);	//Stanford Compatible
#endif

#ifdef GIA_USE_ADVANCED_REFERENCING
void fillExplicitReferenceSameSetTags(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
int identifyReferenceSets(unordered_map<string, GIAEntityNode*> *sentenceConceptEntityNodesList);
int identifyDefiniteReferenceSets(unordered_map<string, GIAEntityNode*> *sentenceConceptEntityNodesList);
	void identityReferenceSet(GIAEntityNode * entityNode, int referenceSetID);
	void identityReferenceSetDetermineNextCourseOfAction(GIAEntityNode * entityNode, int referenceSetID);
void createGIACoreferenceInListBasedUponIdentifiedReferenceSets(unordered_map<string, GIAEntityNode*> *sentenceConceptEntityNodesList, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, GIACoreference * firstGIACoreferenceInList, int numberReferenceSets);	//bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]
	GIACoreference * generateCoreferenceListBasedUponPreviouslyMatchedEntityNode(GIAEntityNode * entityNode, GIACoreference * currentGIACoreferenceInList);
void linkAdvancedReferencesGIA(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAConceptNodeArray[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, GIACoreference * firstCoreferenceInList, Feature * featureArrayTemp[]);

#endif
			
#endif
