/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 20-Apr-2012
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

#define GRAMMATICAL_DETERMINER_DEFINITE "the"
#define GRAMMATICAL_DETERMINER_INDEFINITE "a"
//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense POS information is replicated in both the auxillary and the verb)

#define GIA_STANFORD_CORE_NLP_INTERPRET_AS_PERSONS_FOR_THE_PURPOSES_OF_GIA





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
void locateAndAddAllConceptEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType);

void fillGrammaticalArrays(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int NLPfeatureParser, int NLPdependencyRelationsType);	//Stanford Compatible
	#ifdef GIA_USE_RELEX
	void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], int GIAEntityNodeWordTypeArray[]);
	#endif
	//uses dependency relations to derive grammar [uses POS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[], int NLPfeatureParser);
	#endif
		#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
		void extractPastTenseFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[]);
		void extractGrammaticalInformationFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);
		void extractGrammaticalInformationStanford(Feature * firstFeatureInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[], int NLPfeatureParser);
		void extractPastTense(int entityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int GIAEntityNodeGrammaticalTenseArray[], int NLPfeatureParser);
		#endif
		
void applyGrammaticalInfoToAllConceptEntities(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], string GIAEntityNodePOSArray[], int GIAEntityNodeWordTypeArray[]);

#ifdef GIA_USE_STANFORD_CORENLP
void disableRedundantNodesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);							//Stanford Specific
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);										//Stanford Specific
void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser);	//Stanford Specific
void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);				//Stanford Specific
void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);					//Stanford Specific
void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);				//Stanford Specific [makes up for deficency in Stanford Parser where it does not always generate quantity modifiers where expected]
void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
void redistributeStanfordRelationsPrtAndTmod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);							//Stanford Specific
#endif
								
void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[],  int NLPfeatureParser);				//Stanford Compatible / Relex Specific [$qvars not supported. What is the 'attr' stanford dependency relation?]
void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType);
void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType);						//Stanford Compatible	

void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_USE_STANFORD_CORENLP
void linkReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, StanfordCoreNLPCoreference * firstCoreferenceInList, bool GIAEntityNodeIsAReference[]);	//Stanford Compatible
#endif
			
#endif
