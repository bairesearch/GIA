 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemRelTranslatorDatabase.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SEM_REL_TRANSLATOR_DATABASE
#define HEADER_GIA_SEM_REL_TRANSLATOR_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAsemRelTranslatorDefs.hpp"	//required for GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.hpp"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.hpp"
#include "SHAREDvars.hpp"	//required for writeStringToFileObject


#ifdef GIA_SEM_REL_TRANSLATOR

#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
#define GIA_SEM_REL_TRANSLATOR_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase")
#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".gsr")

static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Pe", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "Pa", "In", "Ve", "Wh", "Pq", "Pd", "Po", "Ab", "Ah", "Ad", "Di", "Sr"};

#endif

class GIAsemRelTranslatorDatabaseClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAnlpClass GIAnlp;
	
	#ifdef GIA_SEM_REL_TRANSLATOR
	
	public: void initialiseSemanticParserDatabase(const string newSemanticParserDatabaseFolderName);
	public: void prepareSemanticParserDatabaseForWriting();
	private: string semanticParserDBgenerateFolderName(const GIAfeature* firstFeatureInList);

	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
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

	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
	#define GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT (INT_DEFAULT_VALUE)
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
	
	#endif
	
	#ifdef GIA_SEM_REL_TRANSLATOR_COMMON
	public: void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);
	#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: GIArelation* getFirstRelationInSemanticParserSentenceList();
	public: GIArelation* getCurrentRelationInSemanticParserSentenceList();
	private: void prepareSemanticParserSemanticRelationListForWriting();
	#endif
	#endif
	
};

#endif
