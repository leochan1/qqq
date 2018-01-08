#pragma once  
#include <log4cplus/logger.h>  
#include <log4cplus/configurator.h>  
#include <log4cplus/fileappender.h>  
#include <log4cplus/consoleappender.h>  
#include <log4cplus/layout.h>  
#include <log4cplus/logger.h>  
#include <log4cplus/ndc.h>  
#include <log4cplus/helpers/loglog.h> 
#include <log4cplus/streams.h>  
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
using namespace std;  
using namespace log4cplus;  
std::string getFullPath()
{
	char buffer[MAXPATH],szPath[MAXPATH]={0};   
	_getcwd(buffer,MAXPATH);  
	printf("%s",buffer); 
	sprintf(szPath,"%s/log/",buffer);
	if ( access(szPath,0) )
		mkdir(szPath);
	return std::string(szPath);
}
class CMultiLog  {  
private:
	std::string m_szTitile;
	static std::string m_szFuncName;
	
public:
	Logger _logger;
	CMultiLog(string szDefineName){
		szDefineName +=".log";
		initFile(szDefineName);
		SharedAppenderPtr _append (new RollingFileAppender(m_szTitile, 200*1024, 10));
		_append->setName(szDefineName);
		time_t tmp_time=time(NULL);  
		struct tm *p;  
		p = localtime(&tmp_time);  
		char szDate[100]={0};
		sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d-", p->tm_year+1900, p->tm_mon+1, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec); 
		std::string pattern = "%c(%L)[%p]<%t> - %m%n";
		std::auto_ptr<Layout> _layout(new PatternLayout(pattern));
		_append->setLayout( _layout );

		std::string szLogger = std::string(szDate) + m_szFuncName;

		_logger = Logger::getInstance(szLogger);
		_logger.addAppender(_append);
		_logger.setLogLevel(ALL_LOG_LEVEL);
	}
	~CMultiLog(){}
	void initFile(string str)
	{
		time_t tmp_time=time(NULL);  
		struct tm *p;  
		p = localtime(&tmp_time);  
		char szDate[100]={0};
		sprintf(szDate, "%d-%d-%d_", p->tm_year, p->tm_mon, p->tm_mday); 
		m_szTitile = getFullPatg() + szDate + str;
	}
	static void setFuncName(string szFuncname)
	{
		m_szFuncName = szFuncname;
	}
	string getFileName(){return m_szTitile;}
};  
std::string CMultiLog::m_szFuncName="";

#define USE_SEPARATE_LOG(fname)  static CMultiLog _g(fname);  
#define fLog                     _g._logger  


#define Multilog_Debug(strFunc,p) \
	LOG4CPLUS_DEBUG(fLog,strFunc<<"-"<< p)
#define Multilog_Info(strFunc,p) LOG4CPLUS_INFO(fLog, strFunc<< "-"<<p)
#define Multilog_Warn(strFunc,p) LOG4CPLUS_WARN(fLog,strFunc<< "-"<< p)
#define Multilog_Error(strFunc,p) LOG4CPLUS_ERROR(fLog, strFunc<<"-"<< p)

