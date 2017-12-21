/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIApreprocessorPOStagger.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3e3a 13-December-2017
 * Requirements: requires plain text file
 * Description: preprocessor POS tagger
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_POS_TAGGER_DATABASE
#define HEADER_GIA_PREPROCESSOR_POS_TAGGER_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAdatabase.hpp"	//required for checkIfFolderExistsAndIfNotMakeAndSetAsCurrent


#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAPOStaggerDatabase")
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "/home/systemusername/source/"	 //this could be "/mnt/serverNameX/" once configuring appropriate NFS Linux File Sharing in /etc/fstab
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (5) 	//eg hex (eg ffff/ffff/ffff/ffff/ffff/ffffffffffffffffffff.txt
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_SUBDIRECTORY_INDEX_NUMBER_OF_WORDS_PER_LEVEL (2) 	//hex (eg ffff)
	
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_HEX_START_POS (0)
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_BYTE_HEX_LENGTH (2)
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_START_POS (3)
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_CENTRE_WORD_POS_AMBIGUITY_NUMBER_OF_INSTANCES_INT_LENGTH (10)
	
	#define ASCII_TABLE_INDEX_OF_a (97)
	#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
	#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_FILE_NAME_PARTA "POSpermutation"
	#define GIA_PREPROCESSOR_POS_TAGGER_DATABASE_POS_PERMUTATION_ENTRY_FILE_NAME_EXTENSION ".pos"
#endif


class GIApreprocessorPOStaggerDatabaseClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAdatabaseClass GIAdatabase;

	#ifdef GIA_PREPROCESSOR_POS_TAGGER_DATABASE
	public: void initialisePOStaggerDatabase(const string newGIAposTaggerDatabaseFolderName);

	private: string DBgenerateFileName(const string POSambiguityInfoPermutation);
		private: string DBgenerateSubFolderName(const string POSambiguityInfoPermutation, const int level, const int numberOfWordsPerLevel);
			private: string DBconvertByteToHex(const unsigned char byte);
			private: unsigned char DBconvertHexToByte(string hexString);
	
	public: bool DBreadPOSpermutationEstimates(const string POSambiguityInfoPermutation, vector<string>* centreWordPOSambiguityInfoList);
	public: bool DBwritePOSpermutationEstimate(const string POSambiguityInfoPermutation, const unsigned char centreWordPOSambiguityInfo);
	#endif
	
	public: string generateIntFormatString(int numberOfCharacters);
};

#endif

