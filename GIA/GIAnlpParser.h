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
 * File Name: GIAnlpParser.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3a 14-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of RelEx CFF/Stanford Parser File
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLP_PARSER
#define HEADER_GIA_NLP_PARSER

#include "GIAsentenceClass.h"
#include "GIAglobalDefs.h"

#define RELEX_DEPENDENCY_RELATION_PREPENDITION "_"

void GIATHparseStanfordParseWordsAndPOStagsText(const string* POStagsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool createFeaturesGIA2only);
	bool findReplicateRelation(const GIAsentence* currentSentenceInList, const GIArelation* relation);
		bool compareRelations(const GIArelation* relation1, const GIArelation* relation2);
	#ifdef GIA2_SUPPORT_QUERIES
	bool findString(string entityName, string stringToFind);
	#endif
	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
	string createSameReferenceSetRecord2(const bool sameReferenceSet);
	#endif
void GIATHparseStanfordParserRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool featuresNotPreviouslyFilled, const bool parseGIA2file, const bool NLPrelexCompatibilityMode);
	void convertStanfordRelationToRelex(GIArelation* currentRelationInList, const GIAsentence* currentSentenceInList);
		string convertPrepositionToRelex2(const string* preposition, bool* prepositionFound);
		#ifdef GIA_LRP
		void convertStanfordRelationToRelexLRPreversion(GIArelation* currentRelationInList, const GIAsentence* currentSentenceInList, const bool stanfordPrepositionFound, const string tempRelexPrepositionString, string* relationTypeRelexStandard);
		#endif
	/*
	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
	void convertGIAsemanticRelation(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList);
		#ifdef GIA_LRP
		void convertGIAsemanticRelationLRPreversion(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList);
		#endif
	#endif
	*/

void GIATHparseRelexFeaturesText(const string* featuresText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence);
void GIATHparseRelexRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool NLPrelexCompatibilityMode);

#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
string generateRelexCFFfeatureTagContent(const GIAfeature* firstFeatureInList);
string generateRelexCFFrelationTagContent(const GIArelation* firstRelationInList);
#endif


#endif
