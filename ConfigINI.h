/*
 * Create by fuzhuo at 11/12/2013
 * Updated by stewmcc at 04/02/2017
 */
#ifndef _CONFIGINI_H
#define _CONFIGINI_H

#include<string>
#include<vector>

#ifdef _DEBUG	
/// <summary>
/// Define to enable debug of every line. Only to be used for error checking.
/// </summary>
#define CONFIGINI_DEBUG
#endif

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
	/// Looks for Ini file if it doesn't exists will just return empty ini file.
	/// </summary>
	/// <param name="filename"> The name of the file to open. </param>	
	ConfigIni(const char *filename);
	/// <summary>
	/// If auto save is on it will write the config file to stored ini_filename_.
	/// </summary>
	~ConfigIni();
	/// <summary>
	/// Writes the config data to the ini file.
	/// </summary>
	/// <param name="override_filename"> Allows to save it to another file instead. Doesn't change the underlying stored ini_filename. </param>
	void WriteConfigFile(const char *override_filename = NULL);

	/// <summary>
	/// This stops it from saving when it is destroyed.
	/// Editing the ini afterwards will turn auto save back on.
	/// </summary>
	void TurnOffAutoSave();
	
	bool GetBoolValue(const char* index, const char *name);
	int GetIntValue(const char* index, const char *name);
	float GetFloatValue(const char* index, const char *name);
	const char* GetStringValue(const char* index, const char *name);

	/// <summary>
	/// If entry exists override, otherwise add entry.
	/// </summary>
	/// <param name="index"> Section in ini file. </param>
	/// <param name="name"> Name of option to change. </param>
	/// <param name="value"> Value to change too. </param>
	void SetBoolValue(const char* index, const char *name, bool value);

	/// <summary>
	/// If entry exists override, otherwise add entry.
	/// </summary>
	/// <param name="index"> Section in ini file. </param>
	/// <param name="name"> Name of option to change. </param>
	/// <param name="value"> Value to change too. </param>
	void SetIntValue(const char* index, const char *name, int value);

	/// <summary>
	/// If entry exists override, otherwise add entry.
	/// </summary>
	/// <param name="index"> Section in ini file. </param>
	/// <param name="name"> Name of option to change. </param>
	/// <param name="value"> Value to change too. </param>
	void SetFloatValue(const char* index, const char *name, float value);

	/// <summary>
	/// If entry exists override, otherwise add entry.
	/// </summary>
	/// <param name="index"> Section in ini file. </param>
	/// <param name="name"> Name of option to change. </param>
	/// <param name="value"> Value to change too. </param>
	void SetStringValue(const char *index, const char* name, const char* value);

	void DebugLogAllCurrentData();
private:
	void SetStringValueWithIndex(const char *index, const char* name, const char* value);
	void LoadConfigFile();

	char temporary_sting_data_[4096];
	std::string ini_filename_;

	std::vector<ConfigIniEntry> vector_data_;
	std::fstream *file_stream_;
	bool auto_save_;	
};

#endif // _CONFIGINI_H

