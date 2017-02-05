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
//#define CONFIGINI_DEBUG
#endif

/// <summary>
/// Little Framework that provides alot of benefit.
/// @StewLMcC - 06/12/16
/// </summary>
namespace LittleLot {

	static const std::string kdefault_no_override = "ini.no_override";

	/// <summary>
	/// Outline of each Ini line/entry.
	/// </summary>
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
		ConfigIni(const std::string filename);
		/// <summary>
		/// If auto save is on it will write the config file to stored ini_filename_.
		/// </summary>
		~ConfigIni();
		/// <summary>
		/// Writes the config data to the ini file.
		/// </summary>
		/// <param name="override_filename"> Allows to save it to another file instead. Doesn't change the underlying stored ini_filename. </param>
		void WriteConfigFile(const std::string override_filename = kdefault_no_override);

		/// <summary>
		/// This stops it from saving when it is destroyed.
		/// Editing the ini afterwards will turn auto save back on.
		/// </summary>
		void TurnOffAutoSave();

		/// <summary>
		/// Changes result to searched option.
		/// Returns true if found and result set to value, otherwise false with result unchanged.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option. </param>		
		/// <param name="result"> The value of the option, unchanged if not found. </param>
		/// <returns> True if found otherwise false. </returns>
		bool GetBoolValue(const std::string index, const std::string name, bool& result);

		/// <summary>
		/// Changes result to searched option.
		/// Returns true if found and result set to value, otherwise false with result unchanged.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option. </param>		
		/// <param name="result"> The value of the option, unchanged if not found. </param>
		/// <returns> True if found otherwise false. </returns>
		bool GetIntValue(const std::string index, const std::string name, int& result);

		/// <summary>
		/// Changes result to searched option.
		/// Returns true if found and result set to value, otherwise false with result unchanged.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option. </param>		
		/// <param name="result"> The value of the option, unchanged if not found. </param>
		/// <returns> True if found otherwise false. </returns>
		bool GetFloatValue(const std::string index, const std::string name, float& result);

		/// <summary>
		/// Changes result to searched option.
		/// Returns true if found and result set to value, otherwise false with result unchanged.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option. </param>		
		/// <param name="result"> The value of the option, unchanged if not found. </param>
		/// <returns> True if found otherwise false. </returns>
		bool GetStringValue(const std::string index, const std::string name, std::string& result);

		/// <summary>
		/// If entry exists override, otherwise add entry.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option to change. </param>
		/// <param name="value"> Value to change too. </param>
		void SetBoolValue(const std::string index, const std::string name, const bool value);

		/// <summary>
		/// If entry exists override, otherwise add entry.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option to change. </param>
		/// <param name="value"> Value to change too. </param>
		void SetIntValue(const std::string index, const std::string name, const int value);

		/// <summary>
		/// If entry exists override, otherwise add entry.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option to change. </param>
		/// <param name="value"> Value to change too. </param>
		void SetFloatValue(const std::string index, const std::string name, const float value);

		/// <summary>
		/// If entry exists override, otherwise add entry.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option to change. </param>
		/// <param name="value"> Value to change too. </param>
		void SetStringValue(const std::string index, const std::string name, const std::string value);

		/// <summary>
		/// Outputs all the currently held data of this INI.
		/// </summary>
		void DebugLogAllCurrentData();
	private:
		/// <summary>
		/// If entry exists override, otherwise add entry to the ini file.
		/// </summary>
		/// <param name="index"> Section in ini file. </param>
		/// <param name="name"> Name of option to change. </param>
		/// <param name="value"> Value to change too. </param>
		void SetStringValueWithIndex(const std::string index, const std::string name, const std::string value);
		void LoadConfigFile();
				
		std::string ini_filename_;
		
		std::vector<ConfigIniEntry> vector_data_;
		std::fstream *file_stream_;
		bool auto_save_;		
	};
}
#endif // _CONFIGINI_H

