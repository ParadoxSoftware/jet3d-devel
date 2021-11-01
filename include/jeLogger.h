#ifndef JE_LOGGER_H
#define JE_LOGGER_H
/*!
	@file jeLogger.h
	
	@author Gerald LePage (darkriftx)
	@brief Abstract logger class header

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

#include <iostream>
#include <sstream>
#include <ctime>

namespace jet3d
{
	//Abstract base logging class
	class jeLogger
	{
	protected:
		jeLogger() {}
		jeLogger(const jeLogger& rhs) = delete;
		jeLogger(jeLogger&&) = delete;
		jeLogger& operator=(const jeLogger& rhs) = delete;
		jeLogger& operator=(jeLogger&&) = delete;

	public:
		enum class LogThreshold
		{
			LogFatal = 0x0001,
			LogError = 0x0002,
			LogWarn  = 0x0004,
			LogInfo  = 0x0008,
			LogDebug = 0x0010,
		};

		
	protected:
		std::string _name;
		unsigned short _logThreshold;
		bool _logOpen;

	protected:
		virtual bool openLog()=0;
		virtual bool flushLog()=0;
		virtual bool closeLog()=0;
		virtual void _logMessage(const LogThreshold& level, const std::string& message)=0;
		bool isLogOpen() const noexcept
		{
			return(_logOpen);
		}
		const char* getLogLevelName(const LogThreshold& level) const noexcept
		{
			switch(level)
			{
			case LogThreshold::LogFatal : return("FATAL");
			case LogThreshold::LogError : return("ERROR");
			case LogThreshold::LogWarn  : return("WARN");
			case LogThreshold::LogInfo  : return("INFO");
			case LogThreshold::LogDebug : return("DEBUG");
			default: return("????");
			}
		}

		const char* getTimeStamp() const noexcept
		{
			const time_t curTime = time(NULL);
			return(ctime(&curTime));
		}

	public:
		jeLogger(const std::string& name,
			     unsigned short logThreshold) :
			_name(name),
			_logThreshold(logThreshold),
			_logOpen(false)
		{
		}

		~jeLogger() {};
		
		void setLevelOn(const LogThreshold& level) noexcept
		{
			//Set the level on using the OR bitwise operator
			_logThreshold |= static_cast<unsigned short>(level);
		}

		void setLevelOff(const LogThreshold& level) noexcept
		{
			//Turn the level off using the exclusive OR bitwise operator
			//which sets to zero in the resultant operand any bit that was
			//set to one in both operands of the operation
			_logThreshold ^= static_cast<unsigned short>(level);
		}

		bool isLevelOn(const LogThreshold& level) const noexcept
		{
			return( (static_cast<unsigned short>(level) & _logThreshold) != 0 );
		}

		void logMessage(const LogThreshold& level, const std::string& message)
		{
			if (isLevelOn(level) && isLogOpen())
			{
				_logMessage(level, message);
			}
		}

		void logMessage(const LogThreshold& level, const char* message)
		{
			if (isLevelOn(level) && isLogOpen())
			{
				_logMessage(level, std::string(message));
			}
		}

		void logMessage(const LogThreshold& level, const std::ostringstream& oStr)
		{
			if (isLevelOn(level) && isLogOpen())
			{
				_logMessage(level, oStr.str());
			}
		}
	};

}


#endif //JE_LOGGER_H