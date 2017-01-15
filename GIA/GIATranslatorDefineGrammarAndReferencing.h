/*******************************************************************************
 *
 * File Name: GIATranslatorDefineGrammarAndReferencing.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i13b 15-Apr-2012
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

#define GIA_STANFORD_CORE_NLP_POS_TAG_VB "VB"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VBD "VBD"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VBG "VBG"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VBN "VBN"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VBP "VBP"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VBZ "VBZ"
#define GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PAST_NUMBER_OF_TYPES (2)
#define GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES (2)
static string posTagVerbPastArray[GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PAST_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_VBD, GIA_STANFORD_CORE_NLP_POS_TAG_VBN};
static string posTagVerbPresentArray[GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_VBP, GIA_STANFORD_CORE_NLP_POS_TAG_VBZ};
#define GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES (1)
static string posTagVerbProgressiveArray[GIA_STANFORD_CORE_NLP_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_VBG};

#define GIA_STANFORD_CORE_NLP_POS_TAG_NN "NN"
#define GIA_STANFORD_CORE_NLP_POS_TAG_NNS "NNS"
#define GIA_STANFORD_CORE_NLP_POS_TAG_NNP "NNP"
#define GIA_STANFORD_CORE_NLP_POS_TAG_NNPS "NNPS"
#define GIA_STANFORD_CORE_NLP_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES (2)
#define GIA_STANFORD_CORE_NLP_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES (2)
#define GIA_STANFORD_CORE_NLP_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES (2)
#define GIA_STANFORD_CORE_NLP_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES (2)
static string posTagProperNounArray[GIA_STANFORD_CORE_NLP_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_NNP, GIA_STANFORD_CORE_NLP_POS_TAG_NNPS};
static string posTagCommonNounArray[GIA_STANFORD_CORE_NLP_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_NN, GIA_STANFORD_CORE_NLP_POS_TAG_NNS};
static string posTagSingularNounArray[GIA_STANFORD_CORE_NLP_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_NN, GIA_STANFORD_CORE_NLP_POS_TAG_NNP};
static string posTagPluralNounArray[GIA_STANFORD_CORE_NLP_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES] = {GIA_STANFORD_CORE_NLP_POS_TAG_NNS, GIA_STANFORD_CORE_NLP_POS_TAG_NNPS};


#define RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES (1)
static string relationAuxillaryGovernerIndicatesFutureTenseArray[RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES] = {"will"};

/*
stanford parser tense
https://mailman.stanford.edu/pipermail/parser-user/2008-September/000073.html
Is there a way to identify the main verb of the sentence, and following
that, the tense of the sentence? So far I've been identifying the first
'VBP', 'VBD' or 'VBZ' tagged word, and assigning present or past tense
according to these tags.

stanford pos tagger specification
	http://bulba.sdsu.edu/jeanette/thesis/PennTags.html
	
http://nlp.stanford.edu/software/tagger.shtml	
	Part-of-speech name abbreviations: The English taggers use the Penn Treebank tag set.

J93-2004.pdf;
VB Verb, base form
VBD Verb, past tense	
VBG Verb, gerund/present participle
VBN Verb, past participle
VBP Verb, non-3rd ps. sing. present
VBZ Verb, 3rd ps. sing. present

NN Noun, singular or mass
NNS Noun, plural
NNP Proper noun, singular
NNPS Proper noun, plural

sing/VB be/VB do/VB have/VB			[N/A]
sings/VBZ is/VBZ does/VBZ has/VBZ		[present]
sang/VBD was/VBD did/VBD had/VBD		[past]
singing/VBG being/VBG doing/VBG having/VBG	[present participle / continuous]
sung/VBN been/VBN done/VBN had/VBN		[past participle]

proper noun versus noun
http://www.chompchomp.com/terms/propernoun.htm

http://www.cis.upenn.edu/~treebank/

http://stackoverflow.com/questions/1833252/java-stanford-nlp-part-of-speech-labels

stanford pos tagger "future tense"
www.lancs.ac.uk/staff/hardiea/cl03_urdu.pdf
for example, the verbal auxiliary element indicating future tense: see Schmidt 1999: 106, Bhatia and Koul 2000: 331-332

//NER; stanford NER person organization
Recognizes named (PERSON, LOCATION, ORGANIZATION, MISC) and numerical entities (DATE, TIME, MONEY, NUMBER)
	http://nlp.stanford.edu/software/corenlp.shtml
*/


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

void fillGrammaticalArrays(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], int NLPfeatureParser, int NLPdependencyRelationsType);	//Stanford Compatible
	#ifdef GIA_USE_RELEX
	void fillGrammaticalArraysRelex(Sentence * currentSentenceInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);
	#endif
	//uses dependency relations to derive grammar [uses POS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	void fillGrammaticalArraysStanford(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], int NLPfeatureParser);
	#endif
		#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
		void extractPastTenseFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[]);
		void extractGrammaticalInformationFromPOStag(string * POStag, int entityIndex, int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[]);
		void extractGrammaticalInformation(Feature * firstFeatureInList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[], int NLPfeatureParser);
		void extractPastTense(int entityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int GIAEntityNodeGrammaticalTenseArray[], int NLPfeatureParser);
		#endif
		
void applyGrammaticalInfoToAllConceptEntities(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], string GIAEntityNodeNERArray[], string GIAEntityNodeNormalizedNERArray[], string GIAEntityNodeTimexArray[]);

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);							//Stanford Specific
void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);	//Stanford Specific
void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);	//Stanford Specific
void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Specific
void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Specific
void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);		//Stanford Specific
void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);	//Stanford Specific [makes up for deficency in Stanford Parser where it does not always generate quantity modifiers where expected]
void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Specific
void redistributeStanfordRelationsPrtAndTmod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);				//Stanford Specific
#endif
										
void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);			//Stanford Incompatible	[$qvars not supported. What is the 'attr' stanford dependency relation?]
void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList, int NLPdependencyRelationsType);
void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], int NLPdependencyRelationsType);						//Stanford Compatible	

void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDateOrStanfordTime[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsRelexPersonOrStanfordProperNounArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);	//theoretically stanford Compatible - [would require a lot of parameters to be manually derived, where they are automatically derived by Relex, and it would take time to get this to work properly]. Dont use this, use references instead Stanford codependency references instead
#ifdef GIA_USE_STANFORD_CORENLP
void linkReferencesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, StanfordCoreNLPCoreference * firstCoreferenceInList, bool GIAEntityNodeIsAReference[]);	//Stanford Compatible
#endif
			
#endif
