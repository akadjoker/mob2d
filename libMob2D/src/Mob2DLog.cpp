#include "Mob2DCommon.h"

namespace Mob2D {

Mob2DLog*
Mob2DLog::m_pInstance = NULL;

Mob2DLog::~Mob2DLog()
{
}
void Mob2DLog::PushString(const string& error_string)
{
	error_list.push_back(error_string);
}
void Mob2DLog::ConsoleDump()
{
	for(std::list<string>::iterator i = error_list.begin(); i!=error_list.end(); i++)
		{ std::cout<<(*i); }
}
void Mob2DLog::FileDump()
{
	time_t rawtime;
	struct tm * timeinfo;
	char filename_date[80];
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(filename_date, 80, "%a_%b_%d_%H", timeinfo);

	string filename_name = "Mob2DLog_";
	filename_name.append(filename_date);

	std::ofstream myfile;
	myfile.open(filename_name.c_str());
	for(std::list<string>::iterator i = error_list.begin(); i!=error_list.end(); i++)
		{ myfile<<(*i); }
	myfile.close();
}

} // namespace
