#ifndef JE_FILE_LOGGER_H
#define JE_FILE_LOGGER_H
/*!
	@file jeFileLogger.h
	
	@author Gerald LePage (darkriftx)
	@brief File logger class header

	@par License
	The contents of this file are subject to the Jet3D Public License       
	Version 1.02 (the "License"); you may not use this file except in         
	compliance with the License. You may obtain a copy of the License at       
	http://www.jet3d.com                                                        
                                                                             
	@par
	Software distributed under the License is distributed on an "AS IS"           
	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See           
	the License for the specific language governing rights and limitations          
	under the License.                                                               
                                                                                  
	@par
	The Original Code is Jet3D, released December 12, 1999.                            
	Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           
*/

#include <fstream>
#include <memory>
#include "jeLogger.h"

namespace jet3d
{
	class jeFileLogger : public jeLogger
	{
	private:
		std::ofstream _outputFile;
		std::string _directory;
	protected:
		bool openLog() override
		{
			if (_logOpen)return(false);
			std::ostringstream fName;
			fName << _directory << _name << ".log";
			_outputFile.open(fName.str().c_str());
			if (!_outputFile)return(false);			
			_logOpen = true;
			return(true);
		}

		bool flushLog() override
		{
			if (!_logOpen)return(false);
			_outputFile.flush();
			return(true);
		}

		bool closeLog() override
		{
			if (!_logOpen)return(false);
			_outputFile.close();
			return(true);
		}


		void _logMessage(const LogThreshold& level, const std::string& message) override
		{
			std::string logLevelName(getLogLevelName(level));
			std::string curTimeStamp(getTimeStamp());

			_outputFile << logLevelName << "\t" << curTimeStamp.substr(0, curTimeStamp.length()-2)  << "\t" << message << std::endl;
			flushLog();
		}

		jeFileLogger() {};
		jeFileLogger(const jeFileLogger&) = delete;
		jeFileLogger(jeFileLogger&&) = delete;
		jeFileLogger& operator=(const jeFileLogger&) = delete;
		jeFileLogger& operator=(jeFileLogger&&) = delete;

	public:
		jeFileLogger(const std::string& name, 
			const std::string& directory, 
			unsigned short threshold) : jeLogger(name, threshold),
			_outputFile(name),
			_directory(directory)
		{
			openLog();
			_logMessage(jeLogger::LogThreshold::LogInfo, "Logging started");
		}

		~jeFileLogger()
		{
			_logMessage(jeLogger::LogThreshold::LogInfo, "Logging ended");
			flushLog();
			closeLog();
		}

	};

	typedef std::unique_ptr<jeFileLogger>			jeFileLoggerPtr;
}

#endif //JE_FILE_LOGGER_H