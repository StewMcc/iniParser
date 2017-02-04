/*
 * Create by fuzhuo at 11/12/2013
 * Updated by stewmcc at 04/02/2017
 * TODO:: Convert so console ouput not specific cout/printf,
 * so easily work with dx11 framework.
 */
#ifndef _CONFIGINI_H
#define _CONFIGINI_H

#include<string>
#include<vector>

struct ConfigIniEntry {
	ConfigIniEntry() :is_comment(false) {}
	std::string index;
	std::string name;
	std::string value;
	std::string comment;
	bool is_comment;
};

/// <summary>
/// Configuration Ini File Parser and writer.
/// Allows the reading, creation and writing of ini files, of basic section,key,value variety.
/// </summary>
class ConfigIni {
public:
	/// <summary>
	/// Looks for Ini file if it doesn't exists it will create an empty one.
	/// </summary>
	/// <param name="filename"> THe name of the file to open. </param>	
	ConfigIni(const char *filename);
	~ConfigIni();
	void WriteConfigFile(const char *filename = NULL);

	bool GetBoolValue(const char* index, const char *name);
	int GetIntValue(const char* index, const char *name);
	float GetFloatValue(const char* index, const char *name);
	const char* GetStringValue(const char* index, const char *name);

	void SetBoolValue(const char* index, const char *name, bool value);
	void SetIntValue(const char* index, const char *name, int value);
	void SetFloatValue(const char* index, const char *name, float value);
	void SetStringValue(const char *index, const char* name, const char* value);

	void DebugLogAllCurrentData();
private:
	void SetStringValueWithIndex(const char *index, const char* name, const char* value);
	void LoadConfigFile();

	char temporary_sting_data_[4096];
	char ini_filename_[4096];

	std::vector<ConfigIniEntry> vector_data_;
	std::fstream *file_stream_;
	bool auto_save_;	
};

#endif // _CONFIGINI_H

