#include "myLogger.h"
//#include "Utilities.h"
const string CLogger::m_sFileName = "Log.txt";
CLogger* CLogger:: m_pThis = NULL;
ofstream CLogger::m_Logfile;
CLogger::CLogger()
{

}
CLogger* CLogger::getLogger(){
	if(m_pThis == NULL){
		m_pThis = new CLogger();
		m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app );
	}
	return m_pThis;
}

void CLogger::Log( const char * format, ... )
{
	char sMessage[256];
	va_list args;
	va_start (args, format);
	vsprintf (sMessage,format, args);
	//m_Logfile <<"\n"<<Util::CurrentDateTime()<<":\t";
	m_Logfile << sMessage;
	va_end (args);
}

void CLogger::Log( const string& sMessage )
{
	//m_Logfile <<"\n"<<Util::CurrentDateTime()<<":\t";
	m_Logfile << sMessage;
}

CLogger& CLogger::operator<<(const string& sMessage )
{
	//m_Logfile <<"\n"<<Util::CurrentDateTime()<<":\t";
	m_Logfile << sMessage;
	return *this;
}