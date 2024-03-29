 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAnlp.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: NLP - external natural language processing
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_NLP
#define HEADER_GIA_NLP

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "XMLparserClass.hpp"
#include "XMLrulesClass.hpp"
#include "GIAnlpParser.hpp"
#include "GIAsynRelTranslatorDefs.hpp" //required for featureNERtypeArray only
#include "LRPpreprocessorWordIdentification.hpp"
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "LRPpreprocessorWordReduction.hpp" 
#endif
#include "GIAtranslatorOperations.hpp"
#include "SHAREDvars.hpp"	//file io

#define STANFORD_PARSER_SENTENCE_SKIP_SUPPORT
#define STANFORD_PARSER_SENTENCE_SKIPPED_TEXT "GIAsentence skipped"

#define Relex_CFF_XML_TAG_nlparse ((string)"nlparse")
	#define Relex_CFF_XML_ATTRIBUTE_xmlns ((string)"xmlns")
	#define Relex_CFF_XML_ATTRIBUTE_xmlns_DEFAULT_VALUE ((string)"http://opencog.org/RelEx/0.1.1")
	#define Relex_CFF_XML_TAG_parser ((string)"parser")
	#define Relex_CFF_XML_TAG_date ((string)"date")
	#define Relex_CFF_XML_TAG_source ((string)"source")
		#define Relex_CFF_XML_ATTRIBUTE_url ((string)"url")
	#define Relex_CFF_XML_TAG_paragraph ((string)"paragraph")
		#define Relex_CFF_XML_TAG_sentence ((string)"sentence")
		#define Relex_CFF_XML_ATTRIBUTE_index ((string)"index")
		#define Relex_CFF_XML_ATTRIBUTE_parses ((string)"parses")
		#define Relex_CFF_XML_TAG_parse ((string)"parse")
		#define Relex_CFF_XML_ATTRIBUTE_id ((string)"id")
		#define Relex_CFF_XML_TAG_relations ((string)"relations")
		#define Relex_CFF_XML_TAG_features ((string)"features")


#define StanfordCoreNLP_XML_TAG_root ((string)"root")
	#define StanfordCoreNLP_XML_TAG_document ((string)"document")
		#define StanfordCoreNLP_XML_TAG_sentences ((string)"sentences")
			#define StanfordCoreNLP_XML_TAG_sentence ((string)"sentence")
			#define StanfordCoreNLP_XML_ATTRIBUTE_id ((string)"id")
				#define StanfordCoreNLP_XML_TAG_tokens ((string)"tokens")
					#define StanfordCoreNLP_XML_TAG_token ((string)"token")
						#define StanfordCoreNLP_XML_TAG_word ((string)"word")
						#define StanfordCoreNLP_XML_TAG_lemma ((string)"lemma")
						#define StanfordCoreNLP_XML_TAG_CharacterOffsetBegin ((string)"CharacterOffsetBegin")
						#define StanfordCoreNLP_XML_TAG_CharacterOffsetEnd ((string)"CharacterOffsetEnd")
						#define StanfordCoreNLP_XML_TAG_POS ((string)"POS")
						#define StanfordCoreNLP_XML_TAG_NER ((string)"NER")
						#define StanfordCoreNLP_XML_TAG_NormalizedNER ((string)"NormalizedNER")
						#define StanfordCoreNLP_XML_TAG_Timex ((string)"Timex")
				#define StanfordCoreNLP_XML_TAG_parse ((string)"parse")
				#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
					#define StanfordCoreNLP_XML_TAG_dependencies ((string)"dependencies")
					#define StanfordCoreNLP_XML_TAG_basicdependencies (StanfordCoreNLP_XML_TAG_dependencies)
					#define StanfordCoreNLP_XML_TAG_collapseddependencies (StanfordCoreNLP_XML_TAG_dependencies)
					#define StanfordCoreNLP_XML_TAG_collapsedccprocesseddependencies (StanfordCoreNLP_XML_TAG_dependencies)
					#define StanfordCoreNLP_XML_ATTRIBUTE_basicdependencies ((string)"basic-dependencies")					//not used
					#define StanfordCoreNLP_XML_ATTRIBUTE_collapseddependencies ((string)"collapsed-dependencies")				//not used
					#define StanfordCoreNLP_XML_ATTRIBUTE_collapsedccprocesseddependencies ((string)"collapsed-ccprocessed-dependencies")	//not used
				#else
					#define StanfordCoreNLP_XML_TAG_basicdependencies ((string)"basic-dependencies")
					#define StanfordCoreNLP_XML_TAG_collapseddependencies ((string)"collapsed-dependencies")
					#define StanfordCoreNLP_XML_TAG_collapsedccprocesseddependencies ((string)"collapsed-ccprocessed-dependencies")
				#endif
					#define StanfordCoreNLP_XML_TAG_dep ((string)"dep")
					#define StanfordCoreNLP_XML_ATTRIBUTE_idx ((string)"idx")
						#define StanfordCoreNLP_XML_TAG_governor ((string)"governor")
						#define StanfordCoreNLP_XML_TAG_dependent ((string)"dependent")
		#define StanfordCoreNLP_XML_TAG_coreferences ((string)"coreference")			//NB this apppears to be a bug in the StanfordCoreNLP code (NB it should be called "coreferences"
			#define StanfordCoreNLP_XML_TAG_coreference ((string)"coreference")
				#define StanfordCoreNLP_XML_TAG_mention ((string)"mention")
				#define StanfordCoreNLP_XML_ATTRIBUTE_representative ((string)"representative")
					#define StanfordCoreNLP_XML_TAG_sentence ((string)"sentence")
					#define StanfordCoreNLP_XML_TAG_start ((string)"start")		//this may not be important
					#define StanfordCoreNLP_XML_TAG_end ((string)"end")		//this may not be important
					#define StanfordCoreNLP_XML_TAG_head ((string)"head")		//sentence entity ID of reference

#define StanfordCoreNLP_numberOfDependencySetsPerSentence (3)
#define StanfordCoreNLP_dependencySet_basicdependencies (0)
#define StanfordCoreNLP_dependencySet_collapseddependencies (1)
#define StanfordCoreNLP_dependencySet_collapsedccprocesseddependencies (2)
#define StanfordCoreNLP_dependencySet_usedByGIA (StanfordCoreNLP_dependencySet_collapsedccprocesseddependencies)

#define StanfordCoreNLPdefaultOutputFileExtensionAppend ".xml"


#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
//not used;
static string StanfordCoreNLP_relationSetAttributesNameArray[StanfordCoreNLP_numberOfDependencySetsPerSentence] = {StanfordCoreNLP_XML_ATTRIBUTE_basicdependencies, StanfordCoreNLP_XML_ATTRIBUTE_collapseddependencies, StanfordCoreNLP_XML_ATTRIBUTE_collapsedccprocesseddependencies};
#endif
static string StanfordCoreNLP_relationSetNameArray[StanfordCoreNLP_numberOfDependencySetsPerSentence] = {StanfordCoreNLP_XML_TAG_basicdependencies, StanfordCoreNLP_XML_TAG_collapseddependencies, StanfordCoreNLP_XML_TAG_collapsedccprocesseddependencies};

class GIAnlpClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIAnlpParserClass GIAnlpParser;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	#endif
	public: void executeNLPparser(const string inputTextPlainTXTfileName, const string inputTextNLPXMLfileName, const GIAtranslatorVariablesClass* translatorVariables, const bool parseRelationsOrFeatures);

	public: bool parseNLPparserFile(const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPfeatureParser, const int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode);
		public: bool parseNLPparserFeaturesFile(const string inputTextNLPfeatureXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPfeatureParser, bool* createNewSentences);
		public: bool parseNLPparserRelationsFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed);
			#ifdef GIA_RELEX
			private: bool parseRelexFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool parseRelations, const bool parseFeatures, bool NLPrelexCompatibilityMode, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed);
			#endif
			#ifdef GIA_STANFORD_CORENLP
			private: bool parseStanfordCoreNLPfile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool parseRelations, const bool parseFeatures, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed);
			#endif
			#ifdef GIA_STANFORD_PARSER
			public: bool parseStanfordParserFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed);
				public: bool parseStanfordParserFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool createNewSentences, bool parseGIA2file, const bool onlyParseIfCorpusLookupFailed);
			#endif
				private: int countSubstring(const std::string& str, const std::string& sub);

	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	public: void outputInternalRelationsInRelexFormat(string* nameOfRelexCompactFormatCFFfile, const string* originalInputFileName, const GIAsentence* firstSentenceInList, const int NLPdependencyRelationsParser, const int NLPfeatureParser, const string NLPexeFolderArray[]);
	#endif
};


#endif
