 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAnlpParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: NLP Parser - Parses tabular subsections (Eg <relations>) of RelEx CFF/Stanford Parser File
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_NLP_PARSER
#define HEADER_GIA_NLP_PARSER

#include "GIAsentenceClass.hpp"
#include "GIAglobalDefs.hpp"
#include "GIAsynRelTranslatorDefs.hpp"	//required for convertStanfordRelationToRelex
#include "GIAsemRelTranslatorDefs.hpp"	//required for GIA_SEM_REL_TRANSLATOR_SUBSETS:GIA_SEM_REL_TRANSLATOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES
#include "LRPpreprocessorWordIdentification.hpp"
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "LRPpreprocessorWordReduction.hpp" 
#endif

#define RELEX_DEPENDENCY_RELATION_PREPENDITION "_"

class GIAnlpParserClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	#endif
	public: void GIATHparseStanfordParseWordsAndPOStagsText(const string* POStagsText, GIAsentence* currentSentenceInList, int* numberOfWordsInSentence, const bool createFeaturesGIA2only);
		private: bool findReplicateRelation(const GIAsentence* currentSentenceInList, const GIArelation* relation);
			private: bool compareRelations(const GIArelation* relation1, const GIArelation* relation2);
		#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
		private: bool findString(string entityName, string stringToFind);
		#endif
		#ifdef GIA_SEM_REL_TRANSLATOR
		private: string createSameReferenceSetRecord2(const bool sameReferenceSet);
		#endif
	public: void GIATHparseStanfordParserRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* numberOfWordsInSentence, const bool featuresNotPreviouslyFilled, const bool parseGIA2file, const bool NLPrelexCompatibilityMode);
		public: void convertStanfordRelationToRelex(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList);
			private: string convertPrepositionToRelex2(const string* preposition, bool* prepositionFound);
			#ifdef LRP_PREPROCESSOR_WORD_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
			private: void convertStanfordRelationToRelexLRPreversion(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, const bool stanfordPrepositionFound, const string tempRelexPrepositionString, string* relationTypeRelexStandard);
			#endif
	/*
	#ifdef GIA_SEM_REL_TRANSLATOR
	void convertGIAsemanticRelation(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList);
		#ifdef LRP_PREPROCESSOR_WORD_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
		void convertGIAsemanticRelationLRPreversion(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList);
		#endif
	#endif
	*/

	public: void GIATHparseRelexFeaturesText(const string* featuresText, GIAsentence* currentSentenceInList, int* numberOfWordsInSentence);
	public: void GIATHparseRelexRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* numberOfWordsInSentence, const bool NLPrelexCompatibilityMode);

	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	public: string generateRelexCFFfeatureTagContent(const GIAfeature* firstFeatureInList);
	public: string generateRelexCFFrelationTagContent(const GIArelation* firstRelationInList);
	#endif
};



#endif
