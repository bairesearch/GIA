/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR_AND_REFERENCING
#define HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR_AND_REFERENCING

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
#include "GIAConditionNodeClass.h"

//Pass A
void fillGrammaticalArrays(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int NLPparserType, int NLPdependencyRelationsType);	//Stanford Compatible
	#ifdef GIA_USE_RELEX
	void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);
	#endif
	//uses dependency relations to derive grammar
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);				//Stanford Specific [CHECK THIS: NOT YET CODED]
	#endif
		//uses POS/NER information to derive grammar
		#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
		void fillGrammaticalArraysStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);		//Stanford Specific [CHECK THIS: NOT YET CODED]
		#endif				
void locateAndAddAllConceptEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType);
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);	//Stanford Specific
#endif	
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Specific
#endif
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Specific
#endif	
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsNSubjAndPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);							//Stanford Specific
#endif											
void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Incompatible	[$qvars not supported. What is the 'attr' stanford dependency relation?]
void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType);
void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType);						//Stanford Compatible	

void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_USE_STANFORD_CORENLP
void linkReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, StanfordCoreNLPCoreference * firstCoreferenceInList, bool GIAEntityNodeIsAReference[]);	//Stanford Compatible [CHECK THIS: NOT YET CODED]
#endif
			
#endif
