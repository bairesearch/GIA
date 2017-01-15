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
 * File Name: GIAnlp.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2n7b 03-October-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLP
#define HEADER_GIA_NLP

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"

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

void executeNLPparser(string inputTextPlainTXTfileName, string inputTextNLPXMLfileName, int NLPParser, string NLPexeFolderArray[], bool parseRelationsOrFeatures, bool NLPrelexCompatibilityMode);

bool parseNLPparserFile(string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode);
	bool parseNLPparserFeaturesFile(string inputTextNLPfeatureXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, int NLPfeatureParser, bool* createNewSentences);
	bool parseNLPparserRelationsFile(string inputTextNLPrelationXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool createNewSentences, bool onlyParseIfCorpusLookupFailed);
		#ifdef GIA_USE_RELEX
		bool parseRelexFile(string inputTextNLPrelationXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, bool parseRelations, bool parseFeatures, bool NLPrelexCompatibilityMode, bool createNewSentences, bool onlyParseIfCorpusLookupFailed);
		#endif
		#ifdef GIA_USE_STANFORD_CORENLP
		bool parseStanfordCoreNLPfile(string inputTextNLPrelationXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, bool parseRelations, bool parseFeatures, bool createNewSentences, bool onlyParseIfCorpusLookupFailed);
		#endif
		#ifdef GIA_USE_STANFORD_PARSER
		bool parseStanfordParserFile(string inputTextNLPrelationXMLfileName, bool isQuery, GIAparagraph* firstParagraphInList, bool createNewSentences, bool onlyParseIfCorpusLookupFailed);
			bool parseStanfordParserFile(string inputTextNLPrelationXMLfileName, bool isQuery, GIAsentence* firstSentenceInList, bool createNewSentences, bool parseGIA2file, bool onlyParseIfCorpusLookupFailed);
		#endif
			int countSubstring(const std::string& str, const std::string& sub);

#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
void outputInternalRelationsInRelexFormat(string* nameOfRelexCompactFormatCFFfile, string* originalInputFileName, GIAparagraph* firstParagraphInList, int NLPdependencyRelationsParser, int NLPfeatureParser, string NLPexeFolderArray[]);
#endif

#endif
