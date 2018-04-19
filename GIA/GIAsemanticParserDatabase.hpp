/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserDatabase.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e12b 12-February-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_DATABASE
#define HEADER_GIA_SEMANTIC_PARSER_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAtranslatorDefs.hpp"	//required for GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.hpp"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.hpp"
#include "SHAREDvars.hpp"	//required for writeStringToFileObject

#ifdef GIA_SEMANTIC_PARSER

#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
#define GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".gsr")

#define GIA_SEMANTIC_PARSER_POS_TYPE_ARRAY_NUMBER_OF_TYPES (25)
static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_SEMANTIC_PARSER_POS_TYPE_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Pe", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "Pa", "In", "Ve", "Wh", "Pd", "Pq", "Ab", "Ah", "Ad", "Di", "Sr"};


class GIAsemanticParserDatabaseClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAnlpClass GIAnlp;
	public: void initialiseSemanticParserDatabase(const string newSemanticParserDatabaseFolderName);
	public: void prepareSemanticParserDatabaseForWriting();
	private: string semanticParserDBgenerateFolderName(const GIAfeature* firstFeatureInList);
	#ifndef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: GIArelation* getFirstRelationInSemanticParserSentenceList();
	public: GIArelation* getCurrentRelationInSemanticParserSentenceList();
	private: void prepareSemanticParserSemanticRelationListForWriting();
	#endif

	#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
	public: void prepareSemanticParserCorpusDatabaseFileTextForWriting();
	public: void saveTextLineToCorpusFileString(const string sentenceText);
	public: void saveTextToCorpusFileString(const string sentenceText);
	/*
	void removeTextFromCorpusFileString(string sentenceText);
	void removeTextLineFromCorpusFileString(string sentenceText);
	*/
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence);
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence, const string* sentenceSemanticRelationsText);
	public: bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, const GIAfeature* firstFeatureInListorSubset);
	public: string semanticParserCorpusDBgenerateFileName(const GIAfeature* firstFeatureInList);
	private: string generateCorpusFileHeaderText(const GIAfeature* firstFeatureInSentence, const bool addPOSinfo);
	#endif

	#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
	#define SEMANTIC_RELATION_TYPE_NON_EXISTENT (INT_DEFAULT_VALUE)
	public: bool loadSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[]);
	public: void writeSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int firstWordInTupleIndex, const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet);
	public: void initialiseIntArray(int intArray[], const int size, int value);
	private: int calculateSemanticRelationTypeOptimisedDatabase(const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet);
	private: bool calculateSemanticRelationTypeDirection(const int semanticRelationTypeOptimisedDatabase);
	private: bool calculateSemanticRelationTypeSameReferenceSet(const int semanticRelationTypeOptimisedDatabase);
	private: int calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(const int semanticRelationTypeOptimisedDatabase);
	private: string semanticParserOptimisedDBgenerateFileName(const GIAfeature* firstFeatureInList, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);
	public: void addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, const int semanticRelationTypeOptimisedDatabase, const int firstWordInTupleIndex, const int secondWordInTupleIndex);
	#endif
	public: void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);

};

#endif

#endif
