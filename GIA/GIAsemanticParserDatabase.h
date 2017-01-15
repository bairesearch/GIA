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
 * File Name: GIAsemanticParserDatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k5e 21-July-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_DATABASE
#define HEADER_GIA_SEMANTIC_PARSER_DATABASE

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
#define GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase/")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".txt")

#define GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (25)
static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Pe", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "Pa", "In", "Ve", "Wh", "Pd", "Pq", "Ab", "Ah", "Ad", "Di", "Sr"};


void initialiseSemanticParserDatabase(string newSemanticParserDatabaseFolderName);
void prepareSemanticParserDatabaseForWriting();
string semanticParserDBgenerateFolderName(GIAfeature* firstFeatureInList);
#ifndef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
GIArelation* getFirstRelationInSemanticParserSentenceList();
GIArelation* getCurrentRelationInSemanticParserSentenceList();
void prepareSemanticParserSemanticRelationListForWriting();
#endif

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
void prepareSemanticParserCorpusDatabaseFileTextForWriting();
void saveTextLineToCorpusFileString(string sentenceText);
void saveTextToCorpusFileString(string sentenceText);
/*
void removeTextFromCorpusFileString(string sentenceText);
void removeTextLineFromCorpusFileString(string sentenceText);
*/
#else
string generateSemanticParserCorpusSemanticRelationsText(GIArelation* firstSemanticRelationInList);
#endif
void writeSemanticParserCorpusFile(GIAfeature* firstFeatureInSentence);
bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, GIAfeature* firstFeatureInListorSubset);
string semanticParserCorpusDBgenerateFileName(GIAfeature* firstFeatureInList);
#endif

#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
#define SEMANTIC_RELATION_TYPE_NON_EXISTENT (INT_DEFAULT_VALUE)
bool loadSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[]);
void writeSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int firstWordInTupleIndex, int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet);
void initialiseIntArray(int intArray[], int size, int value);
int calculateSemanticRelationTypeOptimisedDatabase(int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet);
bool calculateSemanticRelationTypeDirection(int semanticRelationTypeOptimisedDatabase);
bool calculateSemanticRelationTypeSameReferenceSet(int semanticRelationTypeOptimisedDatabase);
int calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(int semanticRelationTypeOptimisedDatabase);
string semanticParserOptimisedDBgenerateFileName(GIAfeature* firstFeatureInList, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);
void addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, int semanticRelationTypeOptimisedDatabase, int firstWordInTupleIndex, int secondWordInTupleIndex);
void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);
#endif

#endif

#endif
