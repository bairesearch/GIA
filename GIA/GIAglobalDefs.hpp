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
 * File Name: GIAglobalsDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3b4a 28-May-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: GIA specific global definitions
 *
 *
 * 		Dependencies Introduction
 *		=================================
 *
 *		The source for GIA is written in ISO C++, some of which may require ISO/IEC 14882:2011 (C++11).
 *
 *		GIA requires the following software to be pre-installed;
 *
 *		GIA Execution Dependencies Installation
 *		=================================
 *
 *		Install External Package 1 - Freeglut
 *		--------------------------------------
 *
 *			(Linux Ubuntu 14.04 Only)
 *			sudo apt-get install freeglut3 OR;
 *			sudo dpkg -i freeglut3_2.8.1-1_amd64.deb
 *
 *			(Linux Ubuntu 16.04 Only)
 *			sudo apt-get install freeglut3 OR;
 *			sudo dpkg -i freeglut3_2.8.1-2_amd64.deb
 *
 *			(Linux EL5 Only)
 *			yum install freeglut OR;
 *			rpm -i freeglut-2.4.0-7.1.el5.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			yum install freeglut OR;
 *			rpm -i freeglut-2.6.0-1.el6.x86_64.rpm
 *
 *			(Linux EL7 Only)
 *			yum install freeglut OR;
 *			rpm -i freeglut-2.8.1-3.el7.x86_64.rpm
 *
 *			(Windows XP Only)
 *			unzip freeglut 2.4.0 (compiled).zip
 *			copy freeglut.dll from freeglut 2.4.0 (compiled).zip to c:\Windows\System32
 *
 *			(Windows 7/8 x86_64 Only)
 *			unzip freeglut 2.6.0 (compiled).zip
 *			copy freeglut.dll from freeglut 2.6.0 (compiled) - 32bit.zip to: C:\WINDOWS\SysWOW64
 *			[or System32]
 *
 *			(Windows 10 x86_64 Only)
 *			unzip freeglut 2.8.1 (compiled).zip
 * 			copy freeglut.dll from freeglut 2.8.1 (compiled) - 32bit.zip to: C:\WINDOWS\SysWOW64 [or System32]
 *
 *		Freeglut may be downloaded via;
 *
 *			(Linux) distribution repository
 *			(Windows)
 *				Install Visual C++ 2010 (download VS2010Express1.iso from Microsoft.com)
 *				download freeglut-2.8.1.tar.gz from https://sourceforge.net/projects/freeglut/files/freeglut/2.8.1/
 *				open freeglut-2.8.1/VisualStudio/2010/freeglut.vcxproj
 *				change to 'Release'
 *				Build Solution
 *				extract files from freeglut-2.8.1\VisualStudio\lib\x86 folder;
 *					copy freeglut.dll to freeglut 2.8.1 (compiled) - 32bit\System32
 *					copy freeglut.lib to freeglut 2.8.1 (compiled) - 32bit\lib
 *				extract files from freeglut-2.6.0/Include/GL
 *					Copy 4 files to freeglut 2.8.1 (compiled) - 32bit\include\GL
 *
 *		Install External Package 2 - Wordnet libraries
 *		--------------------------------------
 *
 *			(Linux Ubuntu 16.04 Only)
 *			sudo apt-get install wordnet [wordnet-sense-index] OR;
 *			sudo dpkg -i wordnet_1%3a3.0-33_amd64.deb wordnet-base_1%3a3.0-33_all.deb wordnet-dev_1%3a3.0-33_amd64.deb  wordnet-gui_1%3a3.0-33_amd64.deb [wordnet-sense-index_1%3a3.0-32_all.deb]
 *
 *			(Linux Ubuntu 14.04 Only)
 *			sudo apt-get install wordnet wordnet-sense-index OR;
 *			sudo dpkg -i wordnet_1%3a3.0-32_amd64.deb wordnet-base_1%3a3.0-32_all.deb wordnet-dev_1%3a3.0-32_amd64.deb wordnet-gui_1%3a3.0-32_amd64.deb wordnet-sense-index_1%3a3.0-32_all.deb
 *
 *			(Linux EL5 Only)
 *			yum install wordnet OR;
 *			rpm -i wordnet-3.0-13.el6.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			yum install wordnet OR;
 *			rpm -i wordnet-3.0-13.el5.x86_64.rpm
 *
 *			(Linux EL7 Only)
 *			yum install wordnet OR;
 *			rpm -i wordnet-3.0-13.el7.x86_64.rpm?
 *
 *			(Windows XP Only {UNTESTED})
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *
 *			(Windows 7/8/10 x86_64 Only)
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *
 *		WordNet may be downloaded via;
 *
 *		* (Linux) distribution repository
 *		* (Windows) Download WordNet 3.0 for Windows from http://code.google.com/p/wordnet3p0forwindows
 *
 *
 *		Install External Package 3a - Relex [OPTIONAL] {Linux Only}
 *		--------------------------------------
 *
 *		Create/Copy execute-relex.sh and place it in the Relex binary folder.
 *
 *		NB execute-relex.sh contains the following text;
 *
 *			export LANG=en_US.UTF-8
 *
 *			VM_OPTS="-Xmx1024m"
 *
 *			RELEX_OPTS="\
 *				-Djava.library.path=/usr/lib:/usr/local/lib \		[Ubuntu: -Djava.library.path=/usr/lib:/usr/lib/jni:/usr/local/lib:/usr/local/lib/jni \ ] {extract from autogenerated batch-process.sh}
 *				-Drelex.algpath=data/relex-semantic-algs.txt \
 *				-Dwordnet.configfile=data/wordnet/file_properties.xml \
 *				"
 *
 *			CLASSPATH="-classpath \
 *			bin:\
 *			/usr/local/share/java/opennlp-tools-1.4.3.jar:\
 *			/usr/local/share/java/opennlp-tools-1.3.0.jar:\
 *			/usr/local/share/java/maxent-2.5.2.jar:\
 *			/usr/local/share/java/maxent-2.4.0.jar:\
 *			/usr/local/share/java/trove.jar:\
 *			/usr/local/share/java/jwnl.jar:\
 *			/usr/local/share/java/jwnl-1.3.3.jar:\
 *			/usr/share/java/commons-logging.jar:\
 *			/usr/share/java/gnu-getopt.jar:\
 *			/usr/local/share/java/linkgrammar.jar:\
 *			/usr/share/java/linkgrammar.jar:\
 *			/usr/share/java/xercesImpl.jar:\
 *			/usr/share/java/xml-apis.jar:\
 *			/opt/GATE-4.0/lib/PDFBox-0.7.2.jar:\
 *			#/opt/GATE-4.0/bin/gate.jar:\
 *			/opt/GATE-4.0/lib/jdom.jar:\
 *			/opt/GATE-4.0/lib/jasper-compiler-jdt.jar:\
 *			/opt/GATE-4.0/lib/nekohtml-0.9.5.jar:\
 *			/opt/GATE-4.0/lib/ontotext.jar:\
 *			"
 *
 *		1. 	Install Java Development Tools (eg openjdk and openjdk-devel)
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				sudo apt-get install openjdk-7-jdk OR;
 *				sudo dpkg -i openjdk-7-jdk_7u51-2.4.4-0ubuntu0.13.10.1_amd64.deb (and dependencies)
 *
 *				Linux EL5/EL6 example;
 *
 *				yum install java-1.6.0-openjdk-devel
 *				yum install java-1.6.0-openjdk
 *					OR (EL5);
 *				rpm -i java-1.6.0-openjdk-devel.el5.x86_64.rpm
 *				rpm -i java-1.6.0-openjdk.el5.x86_64.rpm
 *					OR (EL6);
 *				rpm -i java-1.6.0-openjdk-devel-1.6.0.0-1.36.b17.el6_0.x86_64.rpm / java-1.6.0-openjdk-devel-1.6.0.0-1.39.b17.el6_0.x86_64.rpm
 *				rpm -i java-1.6.0-openjdk-1.6.0.0-1.36.b17.el6_0.x86_64.rpm / java-1.6.0-openjdk-1.6.0.0-1.39.b17.el6_0.x86_64.rpm
 *
 *		2. 	Install Link Parser
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				sudo apt-get install link-grammar liblink-grammar4-dev liblink-grammar4-java OR;
 *				sudo dpkg -i liblink-grammar4_4.7.4-2_amd64.deb liblink-grammar4-dev_4.7.4-2_amd64.deb liblink-grammar4-java_4.7.4-2_amd64.deb (and dependencies)
 *
 *				Linux EL5/EL6 example;
 *				(download: http://www.link.cs.cmu.edu/link/
 *				alternate: http://www.abisource.com/projects/link-grammar/#download)
 *
 *				tar -xvf link-grammar-4.7.2.tar.gz
 *				cd link-grammar-4.7.2
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				./configure
 *				make
 *				su root
 *				make install
 *
 *		3. 	Install WordNet dependencies (eg tcl and tk)
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				sudo apt-get install tcl8.5 tcl8.5-lib tk8.5 tk8.5-lib OR;
 *				sudo dpkg -i tcl8.5_8.5.13-1ubuntu4_amd64.deb tcl8.5-lib_8.5.13-1ubuntu4_amd64.deb tk8.5_8.5.11-2ubuntu4_amd64.deb tk8.5-lib_8.5.11-2ubuntu4_amd64.deb (and dependencies)
 *
 *				Linux EL5/EL6 example;
 *
 *				yum install tcl
 *				yum install tcl-devel
 *				yum install tk
 *				yum install tk-devel
 *					OR (EL5);
 *				rpm -i tcl-8.4.13-4.el5.x86_64.rpm
 *				rpm -i tcl-devel-8.4.13-4.el5.x86_64.rpm
 *				rpm -i tk-8.4.13-5.el5_1.1.x86_64.rpm
 *				rpm -i tk-devel-8.4.13-5.el5_1.1.x86_64.rpm
 *					OR (EL6);
 *				rpm -i tcl-8.5.7-6.el6.x86_64.rpm
 *				rpm -i tcl-devel-8.5.7-6.el6.x86_64.rpm
 *				rpm -i tk-8.5.7-5.el6.x86_64.rpm
 *				rpm -i tk-devel-8.5.7-5.el6.x86_64.rpm
 *
 *		4.	Install WordNet
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				sudo apt-get install wordnet wordnet-dev wordnet-sense-index OR;
 *				sudo dpkg -i wordnet_1%3a3.0-31_amd64.deb wordnet-base_1%3a3.0-31_all.deb wordnet-dev_1%3a3.0-31_amd64.deb wordnet-gui_1%3a3.0-31_amd64.deb wordnet-sense-index_1%3a3.0-31_all.deb
 *
 *				Linux EL5/EL6 example;
 *
 *				rpm -i epel-release...
 *				yum install wordnet wordnet-devel
 *					OR (EL5);
 *				rpm -i wordnet-3.0-13.el5.x86_64.rpm wordnet-devel-3.0-13.el5.x86_64.rpm
 *					OR (EL6);
 *				rpm -i wordnet-3.0-13.el6.x86_64.rpm wordnet-devel-3.0-13.el6.x86_64.rpm
 *
 *		5.	Install JWNL Java wordnet library
 *			(download: http://sourceforge.net/projects/jwordnet/)
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				unzip jwnl14-rc2.zip
 *				cd jwnl14-rc2
 *				sudo cp jwnl.jar /usr/share/java/		[see relex-1.4.0/build.xml for location]
 *				sudo mkdir /usr/local/share/java/
 *				sudo cp jwnl.jar /usr/local/share/java/		[see relex-1.4.0/batch-process.sh for location]
 *				sudo chmod 777 /usr/share/java/jwnl.jar
 *				sudo chmod 777 /usr/local/share/java/jwnl.jar
 *
 *				Linux/EL5 example;
 *
 *				unzip jwnl14-rc2.zip
 *				cd jwnl14-rc2
 *				su root
 *				cp jwnl.jar /usr/share/java/		[see relex-1.4.0/build.xml for location]
 *				mkdir /usr/local/share/java/
 *				cp jwnl.jar /usr/local/share/java/	[see relex-1.4.0/batch-process.sh for location]
 *				chmod 777 /usr/share/java/jwnl.jar
 *				chmod 777 /usr/local/share/java/jwnl.jar
 *
 *		6. 	Install Relex Dependencies (ant)
 *
 *				Linux Ubuntu 14.04 example;
 *
 *				sudo apt-get install ant
 *				sudo apt-get install libcommons-logging-java
 *					OR;
 *				sudo dpkg -i ant_1.9.2-1_all.deb ant-optional_1.9.2-1_all.deb libxerces2-java_2.11.0-7_all.deb libxml-commons-external-java_1.4.01-2build1_all.deb libxml-commons-resolver1.1-java_1.2-7build1_all.deb
 *				sudo dpkg -i libapache-pom-java_10-2build1_all.deb libcommons-logging-java_1.1.3-1_all.deb libcommons-parent-java_22-2build1_all.deb liblink-grammar4-dev_4.7.4-2_amd64.deb liblink-grammar4-java_4.7.4-2_amd64.deb
 *
 *				Linux/EL5 example;
 *
 *				yum install ant
 *					OR [EL5]
 *				rpm -i ant-1.6.5-2jpp.2.x86_64.rpm
 *					OR [EL6]
 *				rpm -i ant-1.7.1-13.el6.x86_64.rpm
 *				rpm -i xml-commons-apis-1.3.04-3.6.el6.x86_64.rpm
 *		  		rpm -i xml-commons-resolver-1.1-4.18.el6.x86_64.rpm
 *		  		rpm -i xerces-j2-2.7.1-12.5.el6.x86_64.rpm
 *				rpm -i ant-nodeps-1.7.1-13.el6.x86_64.rpm
 *				rpm -i jakarta-commons-logging-1.0.4-10.el6.noarch.rpm?
 *				rpm -i ant-commons-logging-1.7.1-13.el6.x86_64.rpm
 *
 *		7. 	Install Gate 4.0 (optional)
 *			(download: http://gate.ac.uk/download/)
 *
 *				Linux EL5/EL6 example;
 *				[user needs to have logged in as root at least one time in the past before able to run this from root]
 *
 *				su root
 *				java -jar gate-4.0-build2752-installer-other.jar
 *					run through GUI;
 *					set installation path to /opt/GATE-4.0
 *					tick sources and development documentation
 *					install
 *
 *		8. 	Install GNU getopt (optional) (not required for Ubuntu)
 *			(download: http://www.java2s.com/Code/Jar/GHI/Downloadgetoptjar.htm)
 *
 *				Linux EL5/EL6 example;
 *
 *				unzip getopt.jar.zip
 *				cd getopt.jar.zip_FILES
 *				su root
 *				cp getopt.jar /usr/share/java/gnu-getopt.jar
 *
 *			[not required for Ubuntu: install the "libgetopt-java" package].
 *
 *		9.	Install opennlp (optional)
 *			(download: http://sourceforge.net/projects/opennlp/)
 *
 *				Linux EL5/EL6 example;
 *
 *				tar -xvf opennlp-tools-1.4.3.tgz
 *				cd opennlp-tools-1.4.3
 *				su root
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				./build.sh
 *				cp output/opennlp-tools-1.4.3.jar /usr/local/share/java/
 *				cp lib/maxent-2.5.2.jar /usr/local/share/java/
 *				cp lib/jwnl-1.3.3.jar /usr/local/share/java/
 *				cp lib/jwnl-1.3.3.jar /usr/local/share/java/jwnl.jar
 *				cp lib/trove.jar /usr/local/share/java/
 *
 *		10. 	Install Relex
 *			(download: http://sourceforge.net/projects/relex/
 *			alternate: https://launchpad.net/relex/+download)
 *
 *				Ubuntu 14.04 example;
 *
 *				tar -xvf relex-1.4.0.tar.gz
 *				cd relex-1.4.0
 *				ant
 *				./batch-process.sh
 *
 *				Linux/EL5 example;
 *
 *				tar -xvf relex-1.4.0.tar.gz
 *				cd relex-1.4.0
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				ant
 *				./batch-process.sh
 *
 *
 *		11.	Configure relex xml files for wordnet
 *
 *				Ubuntu 14.04 example;
 *
 *				cd relex-1.4.0/data/wordnet
 *				gedit file_properties.xml
 *				change dictionary_path to <param name="dictionary_path" value="/usr/share/wordnet"/>
 *
 *				Linux EL5/EL6 example;
 *
 *				cd relex-1.4.0/data/wordnet
 *				gedit file_properties.xml
 *				change dictionary_path to <param name="dictionary_path" value="/usr/share/wordnet-3.0/dict"/>
 *
 *
 *		12.	Configure relex batch-process.sh/***.sh class paths;
 *
 *				Linux EL5/EL6 and Ubuntu example;
 *
 *				cd relex-1.4.0
 *				gedit batch-process.sh [append/modify the following]
 *					/usr/local/share/java/jwnl-1.3.3.jar:\
 *					/usr/share/java/xml-apis.jar:\
 *					/opt/GATE-4.0/lib/PDFBox-0.7.2.jar:\
 *
 *
 *
 *
 *		Install External Package 3b - Stanford Parser [OPTIONAL] {Linux/Windows}
 *		--------------------------------------
 *
 *		Download OpenGIANLPDependencies-25September2012.zip / OpenGIANLPDependencies-12October2013.zip / OpenGIANLPDependencies-19April2014 / OpenGIANLPDependencies-07July2015.zip / OpenGIANLPDependencies-19May2017.zip
 *		Copy execute-stanfordParser.sh (Linux) / execute-stanfordParser.bat (Windows) and place it in the Stanford Parser binary folder.
 *
 *		NB execute-stanfordParser.sh (#!/usr/bin/env bash) contains the following text;
 *
 *			#!/usr/bin/env bash
 *			scriptdir=`dirname $0`
 *			java -mx2g -cp "$scriptdir/*:" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "wordsAndTags,penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz $3/$1 > $4/$2
 *
 *		NB execute-stanfordParser.bat contains the following text;
 *
 *			@echo off
 *			:: Runs the English PCFG parser on one or more files, printing trees only
 *			:: usage: lexparser fileToparse
 *			"C:\Program Files\Java\jre8\bin\java.exe" -d64 -mx2g -cp "*;" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "wordsAndTags,penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz %3/%1 > %4/%2
 *
 *		Stanford Parser may be downloaded via;
 *		(Linux and Windows) eg http://nlp.stanford.edu/software/stanford-parser-2012-03-09.tgz / http://nlp.stanford.edu/software/stanford-parser-2013-04-05.zip / http://nlp.stanford.edu/software/stanford-parser-full-2014-01-04.zip / / http://nlp.stanford.edu/software/stanford-parser-full-2015-04-20.zip / http://nlp.stanford.edu/software/stanford-parser-full-2016-10-31.zip
 *
 *		Note Stanford Parser/CoreNLP requires Java Runtime Environment to be installed;
 *
 *			(Linux Ubuntu 16.04 only);
 *			Java 8 (1.8) installation:
 *				sudo apt-get install openjdk-8-jdk
 *
 *			(Linux Ubuntu 14.04 only);
 *			Java 7 (1.7) installation:
 *				sudo apt-get install openjdk-7-jdk
 *			Java 8 (1.8) installation - required for Stanford Parser/CoreNLP 2015-04-20+:
 *				(http://ubuntuhandbook.org/index.php/2015/01/install-openjdk-8-ubuntu-14-04-12-04-lts)
 *				sudo add-apt-repository ppa:openjdk-r/ppa
 *				sudo apt-get update
 *				sudo apt-get install openjdk-8-jdk
 *				sudo update-alternatives --config java	[to set active/default version of Java]
 *				sudo update-alternatives --config javac [to set active/default Java Compiler]
 *				java -version [check version of java running]
 *
 *			(Linux EL5/EL6 only);
 *			Java 7 (1.7) installation:
 *				yum install java-1.7.0-openjdk-devel
 *				yum install java-1.7.0-openjdk
 *			Java 8 (1.8) installation - required for Stanford Parser/CoreNLP 2015-04-20+:
 *				yum install java-1.8.0-openjdk.x86_64
 *				sudo update-alternatives --config java	[to set active/default version of Java]
 *				NOTNEEDED: sudo update-alternatives --config javac [to set active/default Java Compiler]
 *				java -version [check version of java running]
 *
 *			(Linux EL7 only);
 *			Java 7 (1.7) installation
 *				yum search java | grep openjdk
 *				yum install java-1.7.0-openjdk-headless.x86_64
 *			Java 8 (1.8) installation - required for Stanford Parser/CoreNLP 2015-04-20+:
 *				yum search java | grep openjdk
 *				yum install java-1.8.0-openjdk-headless.x86_64
 *
 *			(Windows 7/8/10 x86_64 Only)
 *			Java 7 (1.7) installation:
 *				download and install jre-7uXX-windows-x64.exe
 *				Ensure to install Java 7 to "C:\Program Files\Java\jre7"
 *
 *			Java 8 (1.8) installation - required for Stanford Parser/CoreNLP 2015-04-20+:
 *				download and install jre-8uXX-windows-x64.exe
 *				(http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html)
 *				Ensure to install Java 8 to "C:\Program Files\Java\jre8" (change destination folder when installing and create "jre8" folder)
 *
 *
 *		Install External Package 3c - Stanford CoreNLP [OPTIONAL] {Linux/Windows}
 *		--------------------------------------
 *
 *		Download OpenGIANLPDependencies-25September2012.zip / OpenGIANLPDependencies-12October2013.zip / OpenGIANLPDependencies-19April2014 / OpenGIANLPDependencies-07July2015.zip / OpenGIANLPDependencies-19May2017.zip
 *		Copy execute-stanfordCoreNLP.sh and execute-stanfordCoreNLPwithoutPT.sh (Linux) / execute-stanfordCoreNLP.bat and execute-stanfordCoreNLPwithoutPT.bat (Windows) and place them in the Stanford CoreNLP binary folder.
 *
 *		NB execute-stanfordCoreNLP.sh contains the following text (depending on the version of Stanford CoreNLP);
 *
 *			eg;
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-2012-04-03.jar:$scriptdir/stanford-corenlp-2012-03-09-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-1.3.5.jar:$scriptdir/stanford-corenlp-1.3.5-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.3.1.jar:$scriptdir/stanford-corenlp-3.3.1-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.5.2.jar:$scriptdir/stanford-corenlp-3.5.2-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.7.0.jar:$scriptdir/stanford-corenlp-3.7.0-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar:$scriptdir/protobuf.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *
 *		NB execute-stanfordCoreNLP.bat contains the following text (depending on the version of Stanford CoreNLP);
 *
 *			eg;
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-2012-04-03.jar;stanford-corenlp-2012-03-09-models.jar;xom.jar;joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-1.3.5.jar;stanford-corenlp-1.3.5-models.jar;xom.jar;joda-time.jar;jollyday.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-3.3.1.jar;stanford-corenlp-3.3.1-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre8\bin\java.exe" -d64 -cp stanford-corenlp-3.5.2.jar;stanford-corenlp-3.5.2-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre8\bin\java.exe" -d64 -cp stanford-corenlp-3.7.0.jar;stanford-corenlp-3.7.0-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar;protobuf.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *
 *		NB execute-stanfordCoreNLPwithoutPT.sh contains the following text (depending on the version of Stanford CoreNLP);
 *
 *			eg;
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-2012-04-03.jar:$scriptdir/stanford-corenlp-2012-03-09-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-1.3.5.jar:$scriptdir/stanford-corenlp-1.3.5-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file $3/$1 -outputDirectory $4 -outputExtension $5
 *				/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.3.1.jar:$scriptdir/stanford-corenlp-3.3.1-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file $3/$1 -outputDirectory $4 -outputExtension $5
 *			/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.5.2.jar:$scriptdir/stanford-corenlp-3.5.2-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file $3/$1 -outputDirectory $4 -outputExtension $5
 *			/
 *			scriptdir=`dirname $0`
 *			java -cp $scriptdir/stanford-corenlp-3.7.0.jar:$scriptdir/stanford-corenlp-3.7.0-models.jar:$scriptdir/xom.jar:$scriptdir/joda-time.jar:$scriptdir/jollyday.jar:$scriptdir/ejml-0.23.jar:$scriptdir/protobuf.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file $3/$1 -outputDirectory $4 -outputExtension $5
 *
 *		NB execute-stanfordCoreNLPwithoutPT.bat contains the following text (depending on the version of Stanford CoreNLP);
 *
 *			eg;
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-2012-04-03.jar;stanford-corenlp-2012-03-09-models.jar;xom.jar;joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-1.3.5.jar;stanford-corenlp-1.3.5-models.jar;xom.jar;joda-time.jar;jollyday.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-3.3.1.jar;stanford-corenlp-3.3.1-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre8\bin\java.exe" -d64 -cp stanford-corenlp-3.5.2.jar;stanford-corenlp-3.5.2-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file %3/%1 -outputDirectory %4 -outputExtension %5
 *				/
 *			"C:\Program Files\Java\jre8\bin\java.exe" -d64 -cp stanford-corenlp-3.7.0.jar;stanford-corenlp-3.7.0-models.jar;xom.jar;joda-time.jar;jollyday.jar;ejml-0.23.jar;protobuf.jar -Xmx2g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,parse -file %3/%1 -outputDirectory %4 -outputExtension %5
 *
 *		Stanford CoreNLP may be downloaded via;
 *		(Linux and Windows) eg http://nlp.stanford.edu/software/stanford-corenlp-2012-04-03.tgz / http://nlp.stanford.edu/software/stanford-corenlp-full-2013-04-04.zip / http://nlp.stanford.edu/software/stanford-corenlp-full-2014-01-04.zip / http://nlp.stanford.edu/software/stanford-corenlp-full-2015-04-20.zip / http://nlp.stanford.edu/software/stanford-corenlp-full-2016-10-31.zip
 *
 *		Note Stanford Parser/CoreNLP requires Java Runtime Environment to be installed (see above for instructions).
 *
 *		Install dos2unix (required for Stanford CoreNLP compatibility: this is executed by GIA):
 *
 *			(Linux Ubuntu 16.04 Only)
 *			sudo apt-get install dos2unix
 *			sudo dpkg -i dos2unix_6.0.4-1_amd64.deb
 *
 *			(Linux Ubuntu 14.04 Only)
 *			sudo apt-get install dos2unix
 *			sudo dpkg -i dos2unix_6.0.3-1_amd64.deb
 *
 *			(Linux EL5/6/7 Only)
 *			yum install dos2unix
 *			rpm -i dos2unixXXX.rpm
 *
 *
 *		GIA Development Dependencies Installation
 *		=================================
 *
 *		Install External Package 1 - Freeglut development libraries
 *		--------------------------------------
 *
 *			(Linux Ubuntu 14.04 Only)
 *			sudo apt-get install freeglut3-dev OR;
 *			sudo dpkg -i freeglut3-dev_2.8.1-1_amd64.deb
 *
 *			(Linux Ubuntu 16.04 Only)
 *			sudo apt-get install freeglut3-dev OR;
 *			sudo dpkg -i freeglut3-dev_2.8.1-2_amd64.deb
 *
 *			(Linux EL5 Only)
 *			yum install freeglut-devel OR;
 *			rpm - i freeglut-devel-2.4.0-7.1.el5.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			yum install freeglut-devel OR;
 *			rpm -i freeglut-devel-2.6.0-1.el6.x86_64.rpm
 *
 *			(Windows XP Only);
 *			unpack source code freeglut 2.4.0 (compiled).zip
 *			copy freeglut.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			(note: you'll have to create the GL folder)
 *			copy freeglut_ext.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			copy freeglut_std.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			copy freeglut.lib to: C:\Program Files\Microsoft Visual Studio 9.0\VC\lib\
 *			copy freeglut.dll to: C:\WINDOWS\system32\
 *
 *			(Windows 7 x86_64 Only);
 *			unpack source code freeglut 2.6.0 (compiled) - 32bit.zip
 *			copy freeglut.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			(note: you'll have to create the GL folder)
 *			copy freeglut_ext.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			copy freeglut_std.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			copy freeglut.lib to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\
 *			copy freeglut.dll to: C:\WINDOWS\SysWOW64 [or System32]
 *
 *			(Windows 8 x86_64 Only);
 *			unpack source code freeglut 2.6.0 (compiled) - 32bit.zip
 *			copy freeglut.h to: C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\GL\
 *			(note: you'll have to create the GL folder)
 *			copy freeglut_ext.h to: C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\GL\
 *			copy freeglut_std.h to: C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\GL\
 *			copy freeglut.lib to: C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\
 *			copy freeglut.dll to: C:\WINDOWS\SysWOW64 [or System32]
 *
 *			(Windows 10 x86_64 Only);
 *			unpack source code freeglut 2.8.1 (compiled) - 32bit.zip
 *			copy freeglut.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\ (note: you'll have to create the GL folder)
 * 			copy freeglut_ext.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 * 			copy freeglut_std.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 * 			copy freeglut.lib to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\
 * 			copy freeglut.dll to: C:\WINDOWS\SysWOW64 [or System32]
 *
 *		Freeglut may be downloaded via;
 *
 *			(Linux) distribution repository
 *			(Windows)
 *				Install Visual C++ 2010 (download VS2010Express1.iso from Microsoft.com)
 *				download freeglut-2.8.1.tar.gz from https://sourceforge.net/projects/freeglut/files/freeglut/2.8.1/
 *				open freeglut-2.8.1/VisualStudio/2010/freeglut.vcxproj
 *				change to 'Release'
 *				Build Solution
 *				extract files from freeglut-2.8.1\VisualStudio\lib\x86 folder;
 *					copy freeglut.dll to freeglut 2.8.1 (compiled) - 32bit\System32
 *					copy freeglut.lib to freeglut 2.8.1 (compiled) - 32bit\lib
 *				extract files from freeglut-2.6.0/Include/GL
 *					Copy 4 files to freeglut 2.8.1 (compiled) - 32bit\include\GL
 *
 *		Install External Package 2 - Wordnet development libraries
 *		--------------------------------------
 *
 *			(Linux Ubuntu 16.04 Only)
 *			sudo apt-get install wordnet [wordnet-sense-index] OR;
 *			sudo dpkg -i wordnet_1%3a3.0-33_amd64.deb wordnet-base_1%3a3.0-33_all.deb wordnet-dev_1%3a3.0-33_amd64.deb  wordnet-gui_1%3a3.0-33_amd64.deb [wordnet-sense-index_1%3a3.0-32_all.deb]

 *			(Linux Ubuntu 14.04 Only)
 *			sudo apt-get install wordnet wordnet-dev wordnet-sense-index OR;
 *			sudo dpkg -i wordnet_1%3a3.0-31_amd64.deb wordnet-base_1%3a3.0-31_all.deb wordnet-dev_1%3a3.0-31_amd64.deb wordnet-gui_1%3a3.0-31_amd64.deb wordnet-sense-index_1%3a3.0-31_all.deb
 *
 *			(Linux EL5 Only)
 *			yum install wordnet-devel OR;
 *			rpm -i wordnet-devel-3.0-13.el6.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			yum install wordnet-devel OR;
 *			rpm -i wordnet-devel-3.0-13.el5.x86_64.rpm
 *
 *			(Linux EL7 Only)
 *			yum install wordnet OR;
 *			rpm -i wordnet-3.0-13.el7.x86_64.rpm?
 *
 *			(Windows XP Only {UNTESTED})
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *			copy wn.lib from wn source project\Release to C:\Program Files\Microsoft Visual Studio 9.0\VC\lib\
 *			copy wn.h from wn source project to C:\Program Files\Microsoft Visual Studio 9.0\VC\Include\
 *			ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *			(Windows 7/8/10 x86_64 Only)
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *			copy wn.lib from wn source project\Release to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib
 *			copy wn.h from wn source project to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\Include\
 *			ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *			To Compile Wordnet 3.0 in Visual Studio 2010 (to create wn.lib);
 *			Install WordNet_3.0_win32-setup.exe (from Google Code])
 *			extract C:\Program Files (x86)\WordNet\3.0\src\wn source project.zip
 *			Open wn.sln (convert to Visual C++ 2010 project)
 *			change to 'Release'
 *			wn project - properties - General - Common Language Runtime Support - No Common Language Runtime Support
 *			wn project - properties - General - Configuration Type - Static Library (.lib)
 *			Project - Build Solution
 *
 *		WordNet may be downloaded via;
 *
 *		* (Linux) yum install wordnet-devel
 *		* (Windows) Download WordNet 3.0 for Windows from http://code.google.com/p/wordnet3p0forwindows
 *
 *
 *
 *		GIA Compilation
 *		=================================
 *
 *		Compile GIA.exe
 *
 *			(Linux Only)
 *			open SHAREDglobalDefs.h,
 *				ensure #define LINUX is added
 *				ensure only #define COMPILE_GIA is uncommented
 *			copy makefile.GIA makefile
 *			./clear.bat (rm* .o)
 *			make
 *
 *			(Windows Only)
 *			open SHAREDglobalDefs.h
 *				ensure #define LINUX is commented out
 *				ensure only #define COMPILE_GIA is uncommented
 *			Open GIA.sln
 *			Build All
 *
 *		*Important preprocessor definitions to consider before compilation;*
 *
 *			#define LINUX
 *
 *
 *		Usage Examples;
 *		---------------------------------
 *
 *		Linux EL5/EL6/UB14:
 *
 *		Examples using NLP input from Relex;
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -oall semanticNet -nlprelation 0 -nlpfeature 0 -nlprelationq 0 -nlpfeatureq 0 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 0 -nlpfeature 0 -nlprelationq 0 -nlpfeatureq 0 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		Example using NLP input from Stanford Core NLP;
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -oall semanticNet -nlprelation 1 -nlpfeature 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 1 -nlpfeature 1 -nlprelationq 1 -nlpfeatureq 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		Example using NLP input from Stanford Parser + Stanford Core NLP;
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow > answer.txt
 *		./GIA.exe -lrpfolder "/home/systemusername/source/LRPdata" -syndet 1 -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *
 *		Windows XP/7/8:
 *		Example using NLP input from Stanford Core NLP;
 *		GIA.exe -lrpfolder "C:/Files/source/LRPdata" -itxt inputText.txt -oall semanticNet -nlprelation 1 -nlpfeature 1 -nlprelationq 1 -nlpfeatureq 1 -nlprelexfolder "C:/Files/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "C:/Files/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "C:/Files/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		GIA.exe -lrpfolder "C:/Files/source/LRPdata" -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 1 -nlpfeature 1 -nlprelationq 1 -nlpfeatureq 1 -nlprelexfolder "C:/Files/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "C:/Files/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "C:/Files/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		Example using NLP input from Stanford Parser + Stanford Core NLP;
 *		GIA.exe -lrpfolder "C:/Files/source/LRPdata" -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "C:/Files/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "C:/Files/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "C:/Files/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *		GIA.exe -lrpfolder "C:/Files/source/LRPdata" -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "C:/Files/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "C:/Files/stanford/coreNLP/stanford-corenlp-full-2015-04-20" -nlpstanfordparserfolder "C:/Files/stanford/parser/stanford-parser-full-2015-04-20" -notshow
 *
 *		Filesystem Database Configuration;
 *		---------------------------------
 *
 *		The GIA Database (KB) is a (Linux) filesystem structure of the format:
 *			//server/database/networkIndexEntityNodesLoadedList.txt
 *			//server/database/w/o/r/word1/0/0/0/entity.ent [instance0 is the networkIndex associated with word1]
 *			//server/database/w/o/r/word1/0/0/0/[referencesOfInstance0].ref [instance0 is the networkIndex associated with word1]
 *			//server/database/w/o/r/word1/0/0/1/entity.ent
 *			//server/database/w/o/r/word1/0/0/1/[referencesOfInstance1].ref
 *			//server/database/w/o/r/word1/0/0/2/entity.ent
 *			//server/database/w/o/r/word1/0/0/2/[referencesOfInstance2].ref
 *			//server/database/w/o/r/word2/0/0/0/entity.ent [instance0 is the networkIndex associated with word2]
 *			//server/database/w/o/r/word2/0/0/0/[referencesOfInstance0].ref [instance0 is the networkIndex associated with word2]
 *			//server/database/w/o/r/word2/0/0/1/entity.ent
 *			//server/database/w/o/r/word2/0/0/1/[referencesOfInstance1].ref etc
 *
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.hpp"

#ifdef USE_NLC
	#define GIA_DISABLE_CROSS_SENTENCE_REFERENCING	//added 2g5a/05-September-2014 - required for NLC 1j2b+, optional for NLC 1k11a+
#endif
//#define GIA_SEMANTIC_PARSER

//#define GIA_DEBUG_DISABLE_3b_CODE
#ifndef GIA_DEBUG_DISABLE_3b_CODE
	#define GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2016_10_31_OR_GREATER
	#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2016_10_31_OR_GREATER
		#define GIA_NLP_CLIENT_SERVER	//3b1a
	#endif
	#define GIA_PREPROCESSOR_RECORD_REFERENCES	//3b2a	//required by GIAI
	//#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION	//has not yet been defined
		//#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK	//has not yet been defined
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS	//3b2e
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS_TITLE_PREFIXES_NUMBER_OF_TYPES (26)
				static string preprocessorSupportAbbreviationsTitlePrefixesArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_ABBREVIATIONS_TITLE_PREFIXES_NUMBER_OF_TYPES] = {"1Lt.", "1stLt.", "2Lt.", "2ndLt.", "Amb.", "BGen.", "BrigGen.", "Capt.", "Col.", "Dr.", "Drs.", "Gen.", "Gov.", "Hon.", "LtCol.", "LtGen.", "MSgt.", "Maj.", "MajGen.", "Mdme.", "Mr.", "Mrs.", "Ms.", "Msgr.", "Prof.", "Rev."};
			#endif
		//#endif
		#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION	//3b2e
		#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_DOLLARS //3b2e
		//#ifdef USE_NLC	//precondition temporarily disabled for testing
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION	//3b3a upgraded for NLC
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
				#define GIA_PREPROCESSOR_INDENTATION_CHAR CHAR_TAB
			#endif
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS	//GIA 2b4a	//disabled 2i36a	//3b3a restored and upgraded for NLC
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH	//3b3a	//required for NLC
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING	//3b3a - required for current NLC implementation; e.g. see how preprocessorMathOperatorsEquivalentConjunctions are defined [need to detect individual opening/closing brackets]
			#endif
		//#endif
	//#endif
#endif

//#define GIA_DEBUG_DISABLE_3a_CODE
#ifndef GIA_DEBUG_DISABLE_3a_CODE
	
	//this provides a major architectural change:
	#define GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS	//creates an artificial have/be entity node for every property/definition connection [required to store time information consistently and reference the connection]	//this is required for GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		//#define GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES	//this is designed for GIA3 but is not operative (not used for advanced referencing; only used for post processing reference set matching GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING; ie specific concepts)
		#ifdef GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES
			#define GIA_QUERY_SKIP_OVER_PROPERTY_AND_DEFINITION_RELATIONSHIP_ENTITIES_QUERIES	//this is optional
		#endif
	#else
		//NB linkHavingPropertyConditionsAndBeingDefinitionConditions is no longer supported
	#endif
	#define GIA_PREPROCESSOR_FIND_EXISTING_RELATIONSHIP_IN_SENTENCE_ENFORCE_SAME_SENTENCE_CHECKS	//3a6d
	
	//#define GIA_NLG_MORPHOLOGY_GENERATOR
	#define GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE	//3a1f	//aka GIA_REFERENCING_UPDATE_ENTITY_INDEXES_CONTINUOUSLY (this is an extension of GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY)
	#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
		//#define GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE_NETWORK_INDICES
	#endif
	#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DISABLE_SUBCLASSES
	#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REINSERT_NEWLINE_CHARACTERS_AFTER_EVERY_SENTENCE	//added 3a1a
	#define GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
	#ifdef GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
		#define GIA_ENTITY_CONNECTION_RECORD_RELATIONSHIP_TYPE	//this is required for !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS generalised code
	#endif
	#define GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	//#define GIA_NLG		//NLG codebase has been upgraded so is now defined in GIA_DEBUG_DISABLE_3a_CODE
	#ifdef GIA_NLG	//not yet defined
		#define GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_QUALITIES	//eg "the dog that is blue" (vs "the blue dog")
		#define GIA_NLG_ADD_AUXILIARIES_TO_SAME_REFERENCE_SET_CONDITIONS	//eg "the dog that is near" (vs "the dog near the house")
	#endif

	//#define GIA_ADD_INSTANCES_TO_ACTIVE_LIST_AFTER_PARSE_SENTENCE 	//?
	#define GIA_PREPROCESSOR
	#ifdef GIA_PREPROCESSOR
		//#define GIA_PREPROCESSOR_SUPPORT_PUNCTUATION_MARKS_WITH_PRECEEDING_WHITE_SPACE	//not yet coded: do not currently support punctuation marks with preceeding white space. Currently skip (do not parse) multiple white space/punctuation characters (eg ". "/".."/"  "/" .")	
		#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_ALL_WORD_TYPES	//don't just reduce multiword prepositions (reduce multiword verbs, nouns, adjectives and adverbs also)
		#ifndef USE_NLC
			//#define GIA_PREPROCESSOR_SENTENCE	//3a1a
		#endif
		#ifdef GIA_PREPROCESSOR_SENTENCE
					
			//#define GIA_PREPROCESSOR_SENTENCE_TEMPORARY_SENTENCE_INDEX (-2)
			#define GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
				#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
				#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
					#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_SENTENCE_CONTENTS "giadummysentence"
				#endif
			#endif
				
			#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				//#assert defined GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS

				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_ERROR_CHECKING
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_CONJUNCTIONS_ALLOW_INTERMEDIARY_PREPOSITION_AND_VERB_LOGIC_REFERENCES
					#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR	//3a5i
					#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR
						#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTION_LR_WITH_EMBEDDED_PREPOSITION_VERB_LR_DELETE_IMPLICIT_GOVERNOR
					#endif
				#endif
				
				//#define GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES	//after parsing semantic relations for individual reference sets, GIA will assign every entity in the logic reference structure the same sentenceIndex, and each logic reference variable (with all its reference sets) the same sentence index
				#ifdef GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
					//after parsing semantic relations for individual reference sets, GIA will assign every entity in the original pre-preprocessed sentence to the same sentenceIndex
					#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
					#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
						#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES_DUMMY_LOGIC_REFERENCE_CONTENTS "giadummylogicreference"
					#endif
					//#define GIA_PREPROCESSOR_REASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES_IGNORE_CONNECTIONS_TO_SENTENCE_LOGIC_REFERENCE_SET	//use this if the connection between the logicReference entity and the logicReferenceVariable should have the sentenceIndex of the currentGIApreprocessorSentenceInList rather than that of the logicReferenceVariable 
				#endif
				
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_REMOVE_DOUBLE_WHITE_SPACE
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE	//3a7a	//OLD: this is not required as the information is contained within GIApreprocessorLogicReference structure and it can be manually derived (without NLP)
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
					#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES	//mandatory
					#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_SUPPORT_VERB_TENSE
					//#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
					#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERB_DELIMITERS_ONLY	//this is required because NLP doesn't parse "A giadummyrelationshipsubject [logic reference preposition eg considering] a giadummyrelationshipobject."
				#endif
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION	//mandatory 3a5e //eg It is proposed that [X is Q] but [all [chickens are blue] except for [Tom]], Tom said that [Mary said [the car goes to the fair]], I think that [Tom thinks [I am happy]]
				//#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_ABSTRACT_CONJUNCTIONS	//eg eg subsequent_to and before (FUTURE: and -> subsequent_to + before)
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT	//eg Tom said that... ['that' will not be recorded in the logical condition variable]
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT
					#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SKIP_APPENDED_THAT_NAME "that"
					#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SUPPORT_PREPOSITIONS_WITH_THAT
				#endif
				
				//#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CONJUNCTIONS_ADD_EXPLICIT_SUBJECT_TEXT_FOR_CONJUNCTIONS	//CHECKTHIS (based on splitMathDetectedLineIntoNLPparsablePhrasesLogicalConditionAddExplicitSubjectTextForConjunctions)
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_ASSUME_PARSING_REFERENCE_VARIABLE_AT_START_OF_SENTENCE	//required for some logic references (eg propositions; "Tom said that B"), but not other logic references (eg regarding; "Regards Q...")
				#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_NEW_LOGIC_REFERENCE_CHAR (CHAR_COMMA)
			#endif
			#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
			#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
				//#assert defined GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				
				
				#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS	//this is currently required to process delimiter (eg being/having/verb/preposition) tense, adverbs (eg very near), and adjectives (eg rides fast) using NLP 	
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
				#endif
				#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS	//this improves NLP/GIA translator semantic parser (by reducing the size of the text chunks being processed)
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS_DELIMITERS	//mandatory	//eg The car had a bike. -> GIAdummyactionsubject + "had " + GIAdummyactionobject
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_SUB_REFERENCE_SETS_DELIMITERS
						#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_DISCARD_THAT_WHICH
						#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS_CONTAIN_VERB_ADJECTIVES	//this is currently required to process verb delimiter adjectives (eg rides fast) using NLP 
					#endif
				#endif
				#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT	//this is required a) for third party NLP (as opposed to GIA direct semantic relation parser) and b) to relink logic reference variable entities without a subject/delimiter back to the high level logic reference GIA network structure 
				#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
					#ifdef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
						#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING	//will be disabled by default
					#else
						#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_NO_ADVANCED_REFERENCING
					#endif
					
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP "compartmentalises"		//giadummyrelationship	//verify no conflict with NLC_PREPROCESSOR_INTERPRET_SINGLE_WORD_SENTENCES_AS_ACTIONS_DUMMY_TEXT_ACTION	//"do"?
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT "giadummyrelationshipsubject"	//verify no conflict with NLC_PREPROCESSOR_INTERPRET_SINGLE_WORD_SENTENCES_AS_ACTIONS_DUMMY_TEXT_ACTION_SUBJECT	//"they"?
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT "giadummyrelationshipobject"	//verify no conflict with NLC_PREPROCESSOR_INTERPRET_SINGLE_WORD_SENTENCES_AS_ACTIONS_DUMMY_TEXT_ACTION_OBJECT	//"this"?
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE (1)
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE (2)
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE (2)
					static string giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipFullArray[GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_FULL_SIZE] = {GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP};
					static string giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipSubjectFullArray[GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT_FULL_SIZE] = {"A", GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_SUBJECT};
					static string giaPreprocessorSentenceReferenceSetAddDummyNLPtextRelationshipObjectFullArray[GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT_FULL_SIZE] = {"a", GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_OBJECT};
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT_RELATIONSHIP_LEMMA "compartmentalise"
					#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER	//set reference set as subject/object before semantic relation parse	//this is required to parse object conditions that are parsed by GIA as properties/conditions (instead of actions) of the relationship eg Tom rides to the park ('to' is connected to 'ride') 
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_SUBJECT_OR_OBJECT_AND_DELIMITER
						#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_SET_CONJUNCTION_AND_OR_TEXT_TO_REFERENCE_SET_OBJECT_BEFORE_SEMANTIC_RELATION_PARSE
					#endif
				#endif	
				#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
				#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET_DELIMITER_NUMBER_OF_TYPES (3)
				static string preprocessorRcmodSameReferenceSetDelimiter[GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RCMOD_SAME_REFERENCE_SET_DELIMITER_NUMBER_OF_TYPES] = {"that", "which", "that's"};		//see preprocessorMathRcmodSameReferenceSetDelimiter
			#else
				#define GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES	//mandatory
			#endif
			//#define GIA_PREPROCESSOR_SENTENCE_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE	//this is unsupported as each GIApreprocessorSentence will contain multiple GIAsentences
			//#define GIA_PREPROCESSOR_RECORD_PARSABLE_PHRASE_POSITION_APPROXIMATE	//unimplemented
			//#define GIA_PREPROCESSOR_SUPPORT_MULTILINE_SENTENCES	//unimplemented
			//#define GIA_DEBUG_PREPROCESSOR_SENTENCE_REFERENCE_SET
			//#define GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
			//#define GIA_DEBUG_PREPROCESSOR_SENTENCE_PREMATURE_QUIT	
			#define GIA_PREPROCESSOR_SENTENCE_PRINT_OUTPUT	//temp debug
		#endif
		#define GIA_PREPROCESSOR_INTERMEDIARY_MULTIWORD_FILE_EXTENSION ".intermediary.multiword"
		#define GIA_PREPROCESSOR_INTERMEDIARY_SENTENCE_FILE_EXTENSION ".intermediary.sentence"
	#endif
#endif

//#define GIA_DEBUG_DISABLE_2g_CODE
//#define GIA_DEBUG_DISABLE_2h_CODE
//#define GIA_DEBUG_DISABLE_2i_CODE
//#define GIA_DEBUG_DISABLE_2j_CODE
//#define GIA_DEBUG_DISABLE_2k_CODE
//#define GIA_DEBUG_DISABLE_2l_CODE
//#define GIA_DEBUG_DISABLE_2m_CODE
//#define GIA_DEBUG_DISABLE_2n_CODE
//#define GIA_DEBUG_DISABLE_2o_CODE
#ifndef GIA_DEBUG_DISABLE_2o_CODE
	#ifdef USE_NLC
		#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
			#define GIA_ADVANCED_REFERENCING_PREVENT_REFERENCING_OF_PLURAL_ENTITIES	//2o7b (assumes that the application built on GIA, eg NLC, can handle their referencing independently)	
			//#ifdef GIA_PREDETERMINERS	//has not yet been defined
				#define GIA_ADVANCED_REFERENCING_PREVENT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS	//2o7b (assumes that the application built on GIA, eg NLC, can handle their referencing independently)
			//#endif
		#endif
	#endif
	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC	//disabled 2c1a, reenabled 2o6a, removed 3a1a	//required for !GIA_DISABLE_CROSS_SENTENCE_REFERENCING	//NB if modified, need to update GIArules.xml accordingly
	#endif
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC			
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO	//2o6a
	#endif
	#define GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES	//2o2a
	#ifdef GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES
		//#define GIA_ADVANCED_REFERENCING_ENSURE_QUANTITY_MATCHES_SUPPORT_QUERY_PLURAL_TO_QUANTITY_MATCH	//added 2o5a, removed 2o7a
	#endif
	#ifdef USE_NLC
		#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
			#define GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ACCEPT_PROPERNOUNS_ISOLATE	//2o5b, disabled 2o5c, reenabled 2o9b (assumes that the application built on GIA, eg NLC, can handle their referencing independently)
		#endif
	#endif
#endif
#ifndef GIA_DEBUG_DISABLE_2n_CODE
	#define GIA_EXPLETIVES	//2n7b
	#ifdef GIA_EXPLETIVES
		#define GIA_EXPLETIVES_WORKAROUND_STANFORD_ANOMALY_HAS_ASSIGNMENTS		//2n7b
		#ifndef GIA_EXPLETIVES_WORKAROUND_STANFORD_ANOMALY_HAS_ASSIGNMENTS
			#define GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS_IN_SAME_SENTENCE	//2n7b	//this is only necessary due to faulty NLP relation assignments
		#endif
	#endif
#endif
#ifndef GIA_DEBUG_DISABLE_2m_CODE
	#define GIA_SYNONYMN_DETECTION_DISABLE_DURING_SPECIFIC_CONCEPT_ACTION_LINKING	//2m3a
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES	//2m2a
	#define GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES_PLURAL_GOVERNOR	//2m1b
	//#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_QUALITIES	//added 2m1a, disabled 2m1c, removed 3a1a
	#define GIA_TRANSLATOR_INTERPRET_OF_AS_DEFINITION_FOR_SUBSTANCES_PROPERNOUN_DEPENDENT //2m1c
	#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DISABLE_PROPERTIES_OR_DEFINITIONS	//2m1a/b
	#ifndef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DISABLE_SUBCLASSES
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES	//disabled 3a1a	//2m1c - create subclasses based on prenominal modifiers (eg toyshop:shop), or add alias (eg "the planet Earth")
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING	//2m4a	//eg The fruit is an apple. The apple fruit is happy.
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX (-1)	//2m5a
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DISABLE_REFERENCING_OF_SUBCLASS_ARTIFICIAL_ENTITIES	//2m5a
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES	//2m6a
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DISABLE_PROPERTIES_OR_DEFINITIONS
		#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT	//CHECKTHIS
	#endif
#endif

#ifndef GIA_DEBUG_DISABLE_2l_CODE
	#define GIA_REFERENCING_WILD_CARDS	//2l3a	//interpret something/anything entities as GIA wild card - affects both GIA advanced referencing and queries
	#ifdef GIA_REFERENCING_WILD_CARDS
		#define GIA_REFERENCING_WILD_CARDS_SUBSTANCES_NUMBER_OF_TYPES (2)
		#define GIA_REFERENCING_WILD_CARDS_ACTIONS_NUMBER_OF_TYPES (1)
		static string giaReferencingWildCardSubstanceArray[GIA_REFERENCING_WILD_CARDS_SUBSTANCES_NUMBER_OF_TYPES] = {"something", "anything"};
		static string giaReferencingWildCardActionArray[GIA_REFERENCING_WILD_CARDS_ACTIONS_NUMBER_OF_TYPES] = {"do"};
	#endif
#endif

#ifndef GIA_DEBUG_DISABLE_2k_CODE
	//#ifdef GIA_SEMANTIC_PARSER_SUBSETS	//has not yet been defined
		#ifndef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE	//logic reference preprocessor already splits/processes conjunctions
			//#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS	//2k1a [UNTESTED]		//NB training a connectionist network with this optimisation requires the NLP to directly connect the primary entity (object) in each subphrase to the subject, with the conjunction relations being auxiliary to these connections (not supported by Stanford Parser/CoreNLP at present; to implement a workaround see NLC removeRedundantConditionConjunctions/addConjunctionsConnectedToConditionConjunctionObject for related code)
		#endif
		//#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE	//2k3a	//optionally disabled 3a4a for user friendly output format
		#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
			//#define GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS	//for offline reimplementions of GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE only
			#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_EXTENSIVE	//added 2k3d; record semantic relation tuple files for all non-existant relations
			#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_EXTENSIVE
				//#ifdef GIA_SEMANTIC_PARSER_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH	//has not yet been defined
					#define GIA_SEMANTIC_PARSER_SUBSETS_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS	//added 2k3d
				//#endif
			#endif
		#else
			#define GIA_SEMANTIC_PARSER_SUBSETS_UNOPTIMISED_TEXT_CORPUS	//original GIA2 implementation: semantic relation designation based on text corpus lookup (indexed by POS permutation)
			#ifdef GIA_SEMANTIC_PARSER_SUBSETS_UNOPTIMISED_TEXT_CORPUS
				#define GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
				//#ifdef GIA_SEMANTIC_PARSER_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH	//has not yet been defined
					#define GIA_SEMANTIC_PARSER_SUBSETS_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS	//added 2k3d
				//#endif
			#endif
		#endif
		#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
			//#define GIA_SEMANTIC_PARSER_CONNECTIONIST_NETWORK_USE_TEXT_CORPUS_FOR_TEMP_RELATION_DATA_STORAGE	//original GIA2 implementation	//disabled 2k3a
			//#define GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD	//disabled 2k3d
		#endif
	//#endif
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
		#define GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION	//added 2k3a	//supported by GIA2 including GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE (unlike GIA_RECORD_RCMOD_SET_INFORMATION)  //only required to identify subject (primary entity holding math value) in nlp parsable phrase?
	#endif
#endif

#ifndef GIA_DEBUG_DISABLE_2j_CODE
	#define GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER	//takes into account changes in dependency relations generated by Stanford
	#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
		#define GIA_STANFORD_PARSER_VERSION_2015_04_20_BUG_FAILURE_TO_PARSE_SHORT_POSSESSIVE_CASES	//eg "The dog's value."
	#endif
	#define GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2014_04_04_OR_GREATER
	#define GIA_DYNAMICALLY_LINK_FROM_CONDITIONS	//added 2j4a
	#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS
		//#define GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_GENERALISE_ACTION_TYPES	//disabled 2j13b (to only support move function; this supports use of from conditions in other contexts)
		#define GIA_STANFORD_PARSER_CONNECT_TO_CONDITION_TO_FROM_CONDITION_BUG_GIA_WORKAROUND
		#ifdef GIA_STANFORD_PARSER_CONNECT_TO_CONDITION_TO_FROM_CONDITION_BUG_GIA_WORKAROUND
			//#define GIA_STANFORD_PARSER_CONNECT_TO_CONDITION_TO_FROM_CONDITION_BUG_GIA_WORKAROUND_GENERALISE_ACTION_TYPES		//2j7a, disabled 2j15b for special cases, eg "Given the nature of the bank, write the letter."
			#define GIA_STANFORD_PARSER_CONNECT_TO_CONDITION_TO_FROM_CONDITION_BUG_GIA_WORKAROUND_GENERALISE_CONDITION_TYPES	//2j7a
		#endif
		//#define GIA_STANFORD_PARSER_CONNECT_PREPOSITIONS_TO_ACTION_RATHER_THAN_ACTION_OBJECT_GENERALISE_ACTION_TYPES	//added 2j7a, disable 2j10a for backwards compatibility in GIA output (NB does not improve grammatical correctness of translation, as it just parses that which is ambiguous differently)
		//#define GIA_LINK_ACTION_CONDITIONS_CORRECT_SAME_REFERENCE_SET		//2j7a, disabled 2j14a for NLC compatibility
		//#define GIA_DYNAMICALLY_LINK_FROM_CONDITIONS_ONLY_ACCEPT_AT_CONDITIONS	//not coded
		#define GIA_STANFORD_PARSER_CONNECT_PREPOSITIONS_TO_ACTION_OBJECT_RATHER_THAN_ACTION	//2j7a	//eg "Move the bike near the car." - note this case is ambigious - note stanford connects the "near" preposition to the car object (instead of the move action)
	#endif
	#define GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEMANTIC_PARSER_SEMANTIC_RELATION_GENERATION	//2j5b
	//#define GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES	//considered for 2j5a but rejected	//this method is broken as of 3b4a and requires reimplementation
	#ifndef GIA_SEMANTIC_PARSER_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
		#define GIA_SEMANTIC_PARSER_CORRECT_POSTAGS_FIX1	//2j5a
		#define GIA_SEMANTIC_PARSER_CORRECT_POSTAGS_FIX2	//2j5d
	#endif
	#ifndef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION
		//#define GIA_RECORD_RCMOD_SET_INFORMATION	//added 2j8a, disabled 2k3a (not supported by GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE), no longer supported 2l5b 	//was only required to identify subject (primary entity holding math value) in nlp parsable phrase, but possessionAuxiliaryHave is now used for this purpose	//originally implemented 2i16a but rejected
	#endif
#endif

#ifndef GIA_DEBUG_DISABLE_2g_CODE
	#define GIA_NUMBER_OF	//added 2g9a/24-September-2014
	#ifdef USE_NLC
		#define GIA_NLC_INTEGRATION	//GIA uses NLCpreprocessorSentence code
		#ifdef GIA_NLC_INTEGRATION
			#define GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS 	//added 2i24a
		#endif
	#endif
	#ifdef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
		#define GIA_DISABLE_ALIAS_ENTITY_MERGING	//added 2g11a/21-October-2014 - required for NLC 1k14a+	//IMPORTANT: when activating/deactivating from compilation, ensure GIArules.xml is updated accordingly (search for "GIA_DISABLE_ALIAS_ENTITY_MERGING")
		//#define GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES	//untested and unused
		#ifdef GIA_NLC_INTEGRATION
			//#define GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS	//added 2i31a, disabled 2k7a
		#endif
	#else
		#ifdef GIA_NLC_INTEGRATION
			#define GIA_NLC_INTEGRATION_DEFINE_REFERENCE_CONTEXT_BY_TEXT_INDENTATION	//added 2g10a/17-October-2014	//requires NLC preprocessor to be executed to extract text indentation, NLCpreprocessorSentenceClass.h, and setNLCsentence() to be called before executing GIA
		#endif
	#endif
	#define GIA_STORE_CONNECTION_SENTENCE_INDEX	//mandatory 3a1a	//added 2f15d/16-July-2014 [required for NLC 1g15a+]
	#define GIA_REMOVE_REDUNDANT_LOGICAL_CONDITION_ENTITIES	//added 2f13a/14-July-2014
	#define GIA_XML_RECORD_ADDITIONAL_VARIABLES
#endif

#define GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES		//mandatory as of 3a1a	//2a7a
#ifndef GIA_DEBUG_DISABLE_2i_CODE
	//#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES	//defined out of scope
		#define GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES_PLURAL_DEPENDENT	//added 2i7e
	//endif
	#define GIA_PREDETERMINERS	//added 2i34a
	#define GIA_PREVENT_CONCEPTS_FROM_BEEN_ADDED_AS_CHILDREN_OF_NON_CONCEPTS	//2i20a
	#ifdef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
		#define GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY	//2i19c	//NB this requires !GIA_QUERY_TRACE_NETWORK_INDEX_NODES_DEFINING_INSTANTIATIONS	//designed for GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
		#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
			#define GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING	//2i19e
		#endif
	#else
		#define GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING	//2i19e
	#endif
	#ifndef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING
		#define GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS_ADVANCED //2i19a, disabled 2i27a
	#endif
	#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
		#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES	//2i18a
	#endif
	#define GIA_TRANSLATOR_CONVERT_AMOD_WITH_NUMBERS_TO_QUANTITY_RELATION	//2i12a
	#define GIA_TRANSLATOR_DO_NOT_CREATE_CONCEPT_PROPERTIES_FOR_NON_CONCEPT_PARENTS	//2i10a
	#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
		#ifndef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DISABLE_PROPERTIES_OR_DEFINITIONS
			#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS	//disabled 2m1a	//2i9a	//requires GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES (this is required such that only the instance list of a networkIndex need be parsed when tracing both its definitions and properties/conditions)
		#endif
		#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
			#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_SWITCH_DEFINITION_LINKS_IF_NON_MATCHING_CONCEPTS	//2i10b - designed to enable compatibility with NLC
			#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_DEFINITE	//2i28a
			#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS_ENSURE_PROPERTY_PARENT_IS_CONCEPT_IF_NECESSARY	//2i30a
		#endif
	#endif
	#define GIA_REFERENCING_QUERY_SUPPORT_SAME_REFERENCE_SET_TESTS		//2i6a
	#define GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER	//2i5a
	#define GIA_EXTRA_DETERMINERS	//added 2i2a
#endif
#ifndef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING
	#define GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT	//GIA 2a10a	//disabled 2i19b
#endif

#ifndef GIA_DEBUG_DISABLE_2h_CODE
	#define GIA_SPATIOTEMPORAL_NETWORK		//yet to implement ~2h2a/17-November-2014+
	#ifdef GIA_SPATIOTEMPORAL_NETWORK
		#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS	//added 2h1a/14-November-2014 - required for NLC 1m1a+
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
				#ifdef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
					#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_DISABLED	//only create twoway condition links in derivatives (eg NLC)
				#else
					#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED	//required for GIA advanced referencing
				#endif
			#endif
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE		//added 2h1a/14-November-2014 - required for NLC 1m1a+
		#endif
		#define GIA_ACTIONS_OF_ACTIONS
		#ifdef GIA_ACTIONS_OF_ACTIONS
			#define GIA_NONSTANDARD_INTERMEDIARY_VERB_TYPES
			#ifdef GIA_NONSTANDARD_INTERMEDIARY_VERB_TYPES
				#define GIA_FEATURE_POS_TAG_VERB_POTENTIAL	//added 2h2a/18-November-2014 - required for NLC 1m2a+	- adds new non-standard pos tag for "able" words
				#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
					#define GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE 	//added 2h2c/18-November-2014 - required for NLC 1m2a+	- adds new non-standard pos tag for "ive" words
				#endif
				#define GIA_FEATURE_POS_TAG_VERB_STATE	//added 2h2b/18-November-2014 - required for NLC 1m2a+	- adds new non-standard pos tag for states "eg open/opened" words
				#define GIA_FEATURE_POS_TAG_VERB_DESCRIPTION	//added 2h2d/18-November-2014 - required for NLC 1m2a+	- adds new non-standard pos tag for states "eg ion/ment" words
			#endif
		#endif
	#endif
	#define GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW	//mandatory 3a1a	//added 2h1c/14-November-2014 - required for NLC 1m1a+
#endif

//#define GIA_ENABLE_WARNINGS


//#define GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA //disabled GIA 2c3c [disabling required for GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN] - CHECKTHIS does not cause problems with alternative test scenarios
	//issue detected (yet to be patched) with the disabling of GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA: advanced referencing of aliases failure

#define GIA_NON_STANDARD_CONDITION_CONDITION_CONNECTIONS_CONNECT_CONJUNCTIONS_TO_THEIR_RELEVANT_PREPOSITIONS	//required for USE_NLC

//#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC	//requirement removed 2o6a
	#define GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR	//mandatory 3a1a //assumes GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES is true (why is this required?)
	#ifdef GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR
		#define GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY	//mandatory 3a1a	//this is required for !GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA
	#endif
//#endif
#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_DEFINITION_BASIC	//removed 3a1a
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_QUOTES_TO_SINGLE_WORDS
	#define GIA_ASSUME_QUOTES_HAVE_BEEN_REDUCED_TO_SINGLE_WORDS	//added 2i36a
#endif
	
#ifdef GIA_SEMANTIC_PARSER
	#define GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS
	#define GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS		//GIA 2b1a - sets GIA into semantic parser (OLD: corpus) read mode
	#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
		#define GIA_SEMANTIC_PARSER_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH
		#ifdef GIA_SEMANTIC_PARSER_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH
			#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS
				#define GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
			#endif
		#else
			#define GIA_SEMANTIC_PARSER_DO_NOT_PARSE_DEPENDENCY_RELATION_FILE
			//#define GIA_SEMANTIC_PARSER_PARSE_PARTIALLY_FILLED_SEMANTIC_RELATIONS
		#endif
	#else
		//this compilation is for development purposes only:
		#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS
			#define GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
			#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
				//#define GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
			#endif
			#define GIA_ADVANCED_REFERENCING_DISABLE_LINKING	//do not link references when generating semantic relation text corpus
		#endif
	#endif
	#define GIA_SEMANTIC_PARSER_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
	#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
		//new implementation of GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_QVAR_AND_IS_QUERY_TAG_TAG_NAME "_$qVarisQuery"
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_QVAR_AND_IS_NAME_QUERY_TAG_TAG_NAME "_$qVarisNameQuery"
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_QVAR_AND_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME "_$qVarisWhichOrEquivalentWhatQuery"
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME "isQuery"
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME "isNameQuery"
		#define GIA_SEMANTIC_PARSER_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME "isWhichOrEquivalentWhatQuery"
	#endif
	#define GIA_SEMANTIC_PARSER_SUBSETS	//2d1a	//switches between just using the primary sentence files generated by GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS and the subset sentence files generated by either
	#ifdef GIA_SEMANTIC_PARSER_SUBSETS
		#define GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE	//NB numberOfWordsInSentence is only currently used by GIA2
		#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE	//temporarily enabled for debugging
		#define GIA_SEMANTIC_PARSER_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE (2)	//ie 2 words in subset is the minimimum	//CHECKTHIS
	#else
		#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
	#endif
	#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
		#define GIA_SEMANTIC_PARSER_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	#elif defined GIA_SEMANTIC_PARSER_SUBSETS
		#define GIA_SEMANTIC_PARSER_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2	//this is required as semantic relations from multiple corpus files may be required to properly parse a sentence [if a single corpus file does not exist for the entire sentence]
	#endif
#endif

#define GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX	//created 2d1a, enabled 3b4a	//OLD: numberOfWordsInSentence is only currently used by GIA2
#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
	#define GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
	//#define GIA_RECORD_MAX_FEATURE_INDEX	//extracts maximum number of words from dependency relation indicies (not robust and not required)
#endif

//variables currently being tested (3a4a+)
#define GIA_RELEX_FIX_DO_NOT_ASSIGN_DEFINITE_IF_UNCOUNTABLE

//variables currently being tested (2a1a+)
#define GIA_TRANSLATOR_SUPPORT_CSUB_AND_COP_GENERATE_ADJECTIVES

//variables currently being tested (1q1a+)
#define GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS	//required for NLC

#define GIA_QUERY_SIMPLIFIED_SEARCH
#define GIA_QUERY_SIMPLIFIED_SEARCH_REPLACE_ADVANCED_SEARCH
//#define GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH

//variables currently being tested (1t1a+)
#define GIA_TRANSLATOR_XML_INTERPRETATION
//#define GIA_TEMPORARY_COMPILATION_GCC_FLAG_E_PREPROCESS_TRANSLATOR_XML_FILES	//for use with compileGIAstopAfterPreprocessingXML.bat only: g++ -E GIAmain.cpp GIAparser.cpp GIAtranslator.cpp GIAtranslatorDefineGrammar.cpp GIAtranslatorDefineReferencing.cpp GIAtranslatorRedistributeRelationsStanford.xml GIAtranslatorRedistributeRelationsRelex.xml GIAtranslatorDefineSubstances.xml GIAtranslatorLinkEntities.xml GIAtranslatorApplyAdvancedFeatures.xml GIAtranslatorOperations.cpp GIAentityNodeClass.cpp GIAentityConnectionClass.cpp GIAconditionNodeClass.cpp GIAsentenceClass.cpp GIAdraw.cpp GIAxmlConversion.cpp GIAcxlConversion.cpp GIAquery.cpp GIAdatabase.cpp GIAnlp.cpp GIAwordnet.cpp GIAnlg.cpp GIApreprocessorMultiwordReduction.cpp GIAbot.cpp XMLparserClass.cpp XMLrulesClass.cpp LDsvg.cpp LDopengl.cpp LDparser.cpp LDsprite.cpp LDreferenceManipulation.cpp LDreferenceClass.cpp RToperations.cpp  RTparser.cpp  RTpixelMaps.cpp  RTppm.cpp  RTraytracer.cpp  RTreferenceManipulation.cpp  RTscene.cpp  RTviewinfo.cpp SHAREDglobalDefs.cpp SHAREDvector.cpp SHAREDvars.cpp
//#define GIA_DO_NOT_USE_UNTESTED_BUGFIX_REGARDING_grammaticalDefiniteIndexOfDeterminerTemp	//1t6b
#define GIA_SPECIFIC_ACTION_NETWORK_INDEXES
#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
#define GIA_GENERIC_ENTITY_INTERPRETATION	//mandatory 3a1a					//1t6b
#define GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION	//mandatory 3a1a
#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
	#define GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION	//1t1a (tested 1ttd, tested with generalised link entity preparation generalised redistribution modifications 1t2m)
	#define GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK			//1t2a (tested 1t3a)
	#define GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES		//1t2b (tested 1t3a)
#endif
#define GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT	//allows questions to be properly parsed during relation redistribution when they are added to inputText also (instead of just inputTextQuery)
//#define GIA_WARNINGS

//variables currently being tested (1s1a+)
#define GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER	//disable this if using previously tested version of Stanford CoreNLP (stanford-corenlp-2012-04-03)
#define GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_WORKAROUND_STANFORD_BUG_SINGLE_DIGIT_TIMES_MARKED_AS_ADJECTIVE
#define GIA_1S8D_LOW_PRI_RELEX_UPDATE_CHECK_THAT_IT_DOESNT_BREAK_STANFORD_OPTIMISATION_APPLY_FIX_TO_IS_NAME_QUERY_PROPOGATION
//#define GIA_1S3A_ADD_INTERPRETION_OF_IN_AS_POSSESSIVE_FOR_SUBSTANCES	//disabled in GIA 1s10d due to misinterpretation of "in" in "Apples are used for eating in the forest."


//variables currently being tested (1r1a+)

#define GIA_BOT
#ifdef GIA_BOT
	#define GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON	//Switch "you for I" (and vice versa)
#endif

#define GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES

#define GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
#define GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY	//added to bring Stanford back into line with Relex: eg He says that you like to swim / ccomp(say, like) -> obj(say, like), Moses knew I was angry. / ccomp(knew-2, angry-5) ->  obj(knew-2, angry-5

#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_TAKE_DEPENDENT_AS_PROPERTY_INSTEAD_OF_GOVERNOR	//interpret "toy"/"red" as property of "shop"/"dog" (originally "shop"/"dog" was interpreted as property of "toy"/"red")
#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_TAKE_DEPENDENT_AS_PROPERTY_INSTEAD_OF_GOVERNOR
	#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_DEFINE_NOUNS_WITH_PRENOMINAL_MODIFIERS_AS_SUBSTANCES		//redundant with GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES 	//only enable this when "toy" is property of "shop" (ie do not enable this when "shop" is property of "toy")
#endif

//variables currently being tested (1q1a+)

#define GIA_SPECIFIC_CONCEPTS	//added 1q4a to take into account specific networkIndexes eg 'red bears' as opposed to 'bears' //eg Red dogs are bad animals. / A blue chicken is a happy bird.
#ifdef GIA_SPECIFIC_CONCEPTS
	//#define GIA_SPECIFIC_CONCEPTS_ASSIGN_TO_PROPERNOUNS	//added 1q10a [activated 1q10c] //removed 1t2d
	//#define GIA_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS	//added 1q10a [activated 1q10c]	//removed 1s8c	//check this does not undo advanced referencing/quering...
#endif
	//NB the phrase 'concept'/'substance quality' is a misnomer, as networkIndexes and qualities are not really substances [NB networkIndexes are considered secondary substances in Aristotle's categories, but GIA has a separate Entity class for networkIndexes marked by isNetworkIndex: which is confusingly not being used in this case of 'specific networkIndexes' - a software development/history artefact]" << endl;
	

//variables currently being tested (1p1a+)

#define STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS	//added 22 Sept to enable Stanford CoreNLP to be used to parse dependency relations with comparable (NOT: same) accuracy as stanford parser (ie when stanford CoreNLP is set as both relation and feature parser)
#ifndef GIA_DEBUG_DISABLE_2i_CODE
	#ifndef GIA_SEMANTIC_PARSER		//NB GIA2 doesn't support STANFORD_PARSER_USE_POS_TAGS (because the semantic relations word types being written must match those being read [and read can only use feature parser])
		#define STANFORD_PARSER_USE_POS_TAGS	//added 23 July 2012 to support Stanford Parser POS tags instead of Stanford CoreNLP POS tags (Stanford Parser POS tags are sometimes more accurate than Stanford CoreNLP POS tags)
	#endif
	#ifndef STANFORD_PARSER_USE_POS_TAGS
		#define STANFORD_CORENLP_POS_TAGS_BUG_GIA_WORKAROUND_SET_DETERMINER_DEPENDENT_TO_NOUN	//added 2i2b - eg "a human" in "A human is a player." should always be interpreted as a noun
	#endif
#endif

#define GIA_APPLY_BUG_WORKAROUND_WHERE_A_NETWORK_INDEX_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE


#define GIA_INPUT_FILE_LISTS
#define GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS

//#define GIA_QUERIES_MUST_BE_QUESTIONS	//disabled 30 June 2012


#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	#define GIA_PREPROCESSOR	//required for legacy support of GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
		//#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE	//added 28 October 2012b - requires GIA_PREPROCESSOR_MULTIWORD_REDUCTION for reading of WikipediaIrregularVerbs.txt - this is used to normalise the tagging of continuous verbs (eg in the making of): it is not 100% successful as corrections are limited to irregular continous verbs (WikipediaIrregularVerbs.txt)
		#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_IRREGULAR_VERB_LIST
		#endif
		#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL		//alternate implementation added 5 July 2013 - requires GIA_PREPROCESSOR_MULTIWORD_REDUCTION for reading of WordnetVerbs.txt - this is used to normalise the tagging of continuous verbs (eg in the making of)
		#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
			#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
		#endif
		//#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
		#ifndef GIA_DEBUG_DISABLE_2h_CODE
			#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP_PROGRESSIVE_CASE		//this is required for both STANFORD_PARSER_USE_POS_TAGS (because stanford tags some verbs eg delivering as noun) and STANFORD_CORENLP_POS_TAGS_BUG_GIA_WORKAROUND_SET_DETERMINER_DEPENDENT_TO_NOUN (because it overwrites verb/vbg determiners eg delivering as noun)
			//#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CORRECT_POS_TAGS_EVEN_IF_LEMMAS_DETECTED_BY_NLP		//this would require thorough testing before enabling (plus it is not required at present)
		#endif
	#endif
	#define GIA_WORD_ORIG	//NB wordOrig is now used by more than just NLG (it is also used by LRP)
	//#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
	#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_IRREGULAR_VERB_LIST	//this is required for parseVerbDataGenerateAllTenseVariants
#endif

//variables currently being tested (1o1a+)

#ifdef USE_CE
	#define GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
	#ifdef GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
		#define GIA_NLP_MAXIMUM_NUMBER_WORDS_PER_SENTENCE_THAT_NLP_CAN_PARSE_DUE_TO_3GB_MEMORY_LIMITATION (150)
		#define GIA_WITH_CE_MINIMUM_NUMBER_WORDS_BEFORE_CONVERTING_PUNCTUATION_MARKS_TO_FULL_STOPS (GIA_NLP_MAXIMUM_NUMBER_WORDS_PER_SENTENCE_THAT_NLP_CAN_PARSE_DUE_TO_3GB_MEMORY_LIMITATION/2)
		#define GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS_BIAS_REPLACEMENT_TOWARDS_END_OF_CODEEXTENSION_BASED_ON_EXPECTED_GREATED_DISTANCE_BETWEEN_PUNCTUATION_MARKS
	#endif
	#define GIA_WITH_CE_USE_CODEEXTENSION_HEIRACHY
	#define GIA_WITH_CE_DERIVE_SCODEEXTENSION_PREPEND
	//#define GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS	//else just use first
	//#define GIA_WITH_CE_DEBUG
#endif

#define GIA_ALIASES
#ifdef GIA_ALIASES
	#define GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE	//NB this is required for advanced referencing to work properly with specific cases of aliasing eg; The name of the red dog is Tom. Tom ate a bone near the house.
	#define GIA_ALIASES_RELEX_COMPATIBILITY
#endif
#define GIA_TEMPORARILY_DISABLE_GIA_XML_READ_CHECKS

//#define GIA_NLG
#ifdef GIA_NLG
	#define GIA_NLG_OUTPUT_TO_COMMAND_LINE
	#define GIA_NLG_NO_MORPHOLOGY_GENERATOR	//NB even NLG2 requires origWord not lemma, so a morphology generator is required in both
	#define GIA_WORD_ORIG	//NB wordOrig is now used by more than just NLG (it is also used by LRP)
#endif

//variables currently being tested (1n1a+)

#define GIA_DATABASE	//requires further testing
#ifdef GIA_DATABASE
	#define GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ	//enable this for debugging only - not required for GIA
	#define GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES	//added 29 Nov 2012
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//added 29 Nov 2012
	#else
		//#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
		//#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
	#endif
	#define GIA_DATABASE_FILESYSTEM
	//#define GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	#define GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
#endif

#define GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES	//added 29 Nov 2012 (this cannot be used at present as <entityNode> idActiveList values must be consecutive in xml file)
#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//added 29 Nov 2012
#else
	//#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
	//#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
#endif

#define GIA_RECORD_SAME_REFERENCE_SET_INFORMATION	//mandatory 3a1a	//separated from GIA_ADVANCED_REFERENCING 2g5a/05 September 2014	//added 2g5a - required for advanced referencing, dream mode (identifyReferenceSetsSpecificConceptsAndLinkWithConcepts{}:identifyReferenceSetDetermineNextCourseOfAction{}:identifyReferenceSet()), and NLC 1j2b+
#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
	#define GIA_ADVANCED_REFERENCING
#endif
#ifdef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY
	#define GIA_ADVANCED_REFERENCING
#endif
#ifdef GIA_ADVANCED_REFERENCING
	#define GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS	//added 2f21a/20-August-2014 - required for NLC 1i2a+, disabled NLC for 1j2b+, optional for NLC 1k11a+
	#define GIA_RECORD_WAS_REFERENCE_INFORMATION
	#ifdef USE_NLC
		#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING	//ie #ifndef GIA_ENABLE_CONCEPT_ADVANCED_REFERENCING_ONLY. Note NLC does not require intrasentence referencing when NLC_USE_ADVANCED_REFERENCING is enabled (ie GIA_DISABLE_CROSS_SENTENCE_REFERENCING is enabled)
			#define GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
		#endif
	#else
		#define GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	#endif
	#define GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_CONCEPT_TO_REFERENCES
#else
	//#define GIA_NON_ADVANCED_REFERENCING
#endif


#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	#define GIA_IDENTIFY_REFERENCE_SET_NETWORK_INDEX_ENTITY_ENTRANCE_DO_NOT_ENTER_ON_AN_ACTION_NODE	//GIA 2a8a	//this update is required for NLC if statement parsing //this update enforces orginal GIA specification: '//an action is considered by default not to be part of the same reference set as its subject/object (eg "the man fires the bow"). An rcmod /"that" is explicitly required for an action to be considered part of the same reference set as its subject/object (eg "the man that fires the bow...")'
	#define GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ONLY	//this is required considering reference look up of non definite sets is never desired
	#ifdef GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ONLY
		#define GIA_ADVANCED_REFERENCING_IDENTIFY_DEFINITE_SETS_ACCEPT_PROPERNOUNS 	//added 12 August 2012
	#endif
	//#define GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
	#define GIA_ADVANCED_REFERENCING_SEARCH_CODE
	#ifdef GIA_ADVANCED_REFERENCING_SEARCH_CODE
		#define GIA_ADVANCED_REFERENCING_ORIGINAL
		//#define GIA_ADVANCED_REFERENCING_UPDATE_NOT_NECESSARY_OR_TESTED
		#define GIA_ADVANCED_REFERENCING_ENSURE_PLURALITY_MATCHES	//added 29 June 2013
	#endif
	#define GIA_ADVANCED_REFERENCING_PREPOSITIONS
	#define GIA_ADVANCED_REFERENCING_CONDITIONS		//all conditions; ie, not just preposition conditions
	#define GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC - added 2 July 2013 (requires testing)

	#define GIA_ADVANCED_REFERENCING_ASSERT_MINIMUM_SENTENCE_INDEX_OF_REFERENCE_SET	//added 1m5aTEMP11 [requires Stanford Parser - incompatible with Relex - because it requires record of the sentence entity index of the determiner "the"; grammaticalIndexOfDeterminer/grammaticalIndexOfDeterminerTemp]
#endif

#define GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING			//default: on	//this enables pronoun detection	//OLD: {this needs to disable some additional parameters also... (NB substances are still being connected, and not created anew)}
#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING
		#define GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES		//default: on
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
			#define GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION		//if using advanced referencing, only use the pronominal coreferences from Stanford (it, she, he, etc) [not optional]
		#endif
	#else
		#ifdef GIA_NON_ADVANCED_REFERENCING
			#define GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES	//default: on
			#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
				//#define GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES_ALL	//Not fully tested, but appears to work at least in simple scenarios
				#ifndef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES_ALL
					#define GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION		//if using non-advanced referencing, only use pronominal coreferences from Stanford (it, she, he, etc) [optional]
				#endif
			#endif
		#else
			#define GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES		//default: on
			#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
				#define GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION		//if not using referencing, only use the pronominal coreferences from Stanford (it, she, he, etc) [not optional]
			#endif
		#endif
	#endif
#endif

//variables currently under attention for testing purposes

#define WORDNET_SEARCH_RELATED_SYNSETS
#define GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS		//this was (effectively) disabled before version 1h3b 	//NB used to be called 'GIA_DO_NOT_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS'
#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
//#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
/*#define GIA_ORIGINAL_PRONOMINAL_COREFERENCE_RESOLUTION_IGNORE_SUBSTANCES_TAKE_NETWORK_INDEXES_ONLY	//untested (removed 1l1c)*/	//commented out completely
#define GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
#define GIA_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILIARY_AND_SET_DIFFERENT_REFERENCE_SET

#ifndef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
	#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_GENERATE_STRANGE_LEMMA
	#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_GENERATE_STRANGE_LEMMA
		#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_USE_ORIGINAL_WORD
	#endif
	#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_ASSIGNMENT_BUG_USE_NER_VALUES	//CHECKTHIS; does this need to be reinstated for GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER?
#endif
#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_MAKE_ALL_LEMMAS_LOWER_CASE	//used to resolve the issue where 'time'/'freedom' and 'Time'/'Freedom' are not matched etc

//variables passed testing:
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
//#define GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP


//Debug [Disable these for official build];
//#define GIA_ADVANCED_REFERENCING_DISABLE_LINKING


//Wordnet
#define USE_WORDNET	(takes into account synonymns)
#ifdef USE_WORDNET
	#define GIA_SYNONYMN_DETECTION
	#ifdef GIA_SYNONYMN_DETECTION
		#define GIA_SYNONYMN_DETECTION_DEFAULT_QUERIES_AND_ADVANCED_REFERENCING
	#endif
#endif


//~External NLP Parser
#define GIA_RELEX
#define GIA_STANFORD_CORENLP
#define GIA_STANFORD_PARSER
#ifdef GIA_RELEX
	#ifdef GIA_NLP_CLIENT_SERVER
		//#define GIA_RELEX_CLIENT
	#endif
	//#define GIA_RELEX_IN_STANFORD_COMPATIBILITY_MODE	//untested - not implemented
	#ifdef GIA_RELEX_IN_STANFORD_COMPATIBILITY_MODE
		#define GIA_STANFORD_DEPENDENCY_RELATIONS
	#endif
	#ifdef LINUX
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
		#define GIA_RELEX_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME "execute-relex-withStanfordCompatibilityMode.sh"
		#ifdef GIA_RELEX_CLIENT
			#define GIA_RELEX_CLIENT_EXECUTABLE_NAME "execute-relexClient.sh"
			#define GIA_RELEX_CLIENT_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME "execute-relexClient-withStanfordCompatibilityMode.sh"
		#endif
	#else
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.bat"
		#define GIA_RELEX_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME "execute-relex-withStanfordCompatibilityMode.bat"
		#ifdef GIA_RELEX_CLIENT
			#define GIA_RELEX_CLIENT_EXECUTABLE_NAME "execute-relexClient.bat"
			#define GIA_RELEX_CLIENT_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME "execute-relexClient-withStanfordCompatibilityMode.bat"
		#endif
	#endif
	#define GIA_RELEX_1_4_0	//default: enabled (otherwise use Relex 1.3.0)
	#ifdef GIA_RELEX_1_4_0
		//#define GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS		//BAI paragraph tag support has not yet been added to Relex 1.3.0	//NB GIA no longer works with BAI Relex <paragraph> tag upgrade - as sentence ID is now used for a variety of purposes (without paragraph id): therefore will need to determine a new method of establishing intra paragraph referencing.
	#endif
#endif
#ifdef GIA_STANFORD_CORENLP		//a more advanced implementation of stanford parser (with lemma, entity name detection, etc: Stanford CoreNLP integrates all our NLP tools for the English language, including the part-of-speech (stanfordPOS) tagger, the named entity recognizer (NER), the parser, and the coreference resolution system)
	#ifdef GIA_NLP_CLIENT_SERVER
		#define GIA_STANFORD_NLP_CLIENT
	#endif	
	#define GIA_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.sh"
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPwithoutPT.sh"
		#ifdef GIA_STANFORD_NLP_CLIENT
			#define GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME "execute-stanfordCoreNLPClient.sh"
			#define GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPClientwithoutPT.sh"
		#endif
	#else
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.bat"
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPwithoutPT.bat"
		#ifdef GIA_STANFORD_NLP_CLIENT
			#define GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME "execute-stanfordCoreNLPClient.bat"
			#define GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPClientwithoutPT.bat"
		#endif
	#endif
#endif
#ifdef GIA_STANFORD_PARSER
	#ifdef GIA_NLP_CLIENT_SERVER
		//#define GIA_STANFORD_PARSER_CLIENT
	#endif
	#define GIA_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.sh"
		#ifdef GIA_STANFORD_PARSER_CLIENT
			#define GIA_STANFORD_PARSER_CLIENT_EXECUTABLE_NAME "execute-stanfordParserClient.sh"
		#endif
	#else
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.bat"
		#ifdef GIA_STANFORD_PARSER_CLIENT
			#define GIA_STANFORD_PARSER_CLIENT_EXECUTABLE_NAME "execute-stanfordParserClient.bat"
		#endif
	#endif
#endif
#define GIA_NLP_PARSER_RELEX (0)
#define GIA_NLP_PARSER_STANFORD_CORENLP (1)
#define GIA_NLP_PARSER_STANFORD_PARSER (2)
#define GIA_NLP_PARSER_NUMBER_OF_TYPES (3)
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_CORENLP)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_PARSER)	//NB GIA_NLP_PARSER_STANFORD_CORENLP will be default in future, once it works better than GIA_NLP_PARSER_STANFORD_PARSER.
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_DEPENDENCY_RELATIONS_TYPE_RELEX (0)
#define GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD (1)
#define GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES (2)		//Relex and Stanford tags
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)

#define GIA_NLP_PARSER_TYPE_RELATIONS (0)
#define GIA_NLP_PARSER_TYPE_FEATURES (1)

//~GIAtranslator
#define GIA_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIAtranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH (6)
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED	//this is now MANDATORY
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY	//this was changed 27 July 2013
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED	//this was changed 27 July 2013
#else
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED
#endif

//~GIAmain
//#define GIA_QUERY_WRITE_ANSWER_TO_FILE

//~GIAquery
#define GIA_QUERY_DOUBLE_ERROR (0.0001)
#define GIA_QUERY_CONFIDENCE_MULTIPLIER (10)
	//these preprocessor have been moved to GIAglobalsDefs.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif



//~GIAdraw
//#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. networkIndex-list -> networkIndexes or substances
#define GIA_DRAW_DISPLAY_ANSWER_CONTEXTS
#define GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	#define GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED		//more robust implementation (should activate when using GIA_DATABASE to ensure all the semantic network connections are visible)
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
		//#define GIA_DRAW_ONLY_PRINT_CONNECTIONS_IN_ONE_DIRECTION	//prevents duplication of connection printing
	#endif
#endif
//#ifdef USE_NLC
	//#define GIA_DRAW_PRINT_ENTITY_INSTANCE_ID
	//#define GIA_DRAW_PRINT_ENTITY_ACTIVELIST_ID
	#define GIA_DRAW_PRINT_ENTITY_SENTENCE_INDEX
	//#define GIA_DRAW_PRINT_ENTITY_ENTITY_INDEX
	#define GIA_DRAW_PRINT_CONNECTION_SENTENCE_INDEX
	#define GIA_DRAW_VERBOSE_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
//#endif
#define GIA_SEMANTIC_NET_XML_REORDER_NETWORK_INDEX_IDS_UPON_XML_WRITE_INSTEAD_OF_XML_READ



#endif
