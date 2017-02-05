/*
 * Create by fuzhuo at 11/12/2013
 * Updated by stewmcc at 04/02/2017
 */
#include "ConfigIni.h"

#include <fstream>
#include <string>
#include <sstream>

 /** Change to use prefered debug technique. **/
#include <windows.h>
void Log(std::string text) {
#ifdef _DEBUG
	std::wstringstream ss;
	ss << text.c_str() << std::endl;
	OutputDebugString(ss.str().c_str());
#endif
}
void DebugLogLine(std::string text) {
#ifdef CONFIGINI_DEBUG
	//  Outputs all the extra debug info to console.
	Log(text);
#endif
}

/// <summary>
/// Little Framework that provides alot of benefit.
/// @StewLMcC - 06/12/16
/// </summary>
namespace LittleLot {
	ConfigIni::ConfigIni(const std::string fileNameWithPath) :
		file_stream_(NULL),
		auto_save_(false),
		ini_filename_(fileNameWithPath) {
		LoadConfigFile();
	}

	ConfigIni::~ConfigIni() {
		if (auto_save_) {
			Log("AUTO save Config file[" + ini_filename_ + "]");
			WriteConfigFile();
		}
	}

	void ConfigIni::WriteConfigFile(const std::string override_filename) {
		std::string file_to_write_too;
		if (override_filename == kdefault_no_override) {
			file_to_write_too = ini_filename_;
		} else {
			file_to_write_too = override_filename;
		}
		std::fstream file_stream_;
		file_stream_.open(file_to_write_too, std::ios_base::out | std::ios_base::trunc);
		DebugLogLine("start write file[" + file_to_write_too + "]");
		std::string index = "";
		bool with_comment = false;
		bool is_start = true;
		for (std::vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
			ConfigIniEntry entry = *entry_iterator;
			if (entry.is_comment) {
				with_comment = true;
				if (is_start) {
					file_stream_ << entry.comment.c_str() << std::endl;
				} else {
					file_stream_ << std::endl << entry.comment.c_str() << std::endl;
				}
				is_start = false;
				DebugLogLine("write comment:" + entry.comment);
				continue;
			}
			if (index != entry.index) {
				index = entry.index;
				if (with_comment || is_start) {
					with_comment = false;
					is_start = false;
					file_stream_ << '[' << entry.index << ']' << std::endl;
					DebugLogLine("write index[" + entry.index + "]");
				} else {
					file_stream_ << std::endl << '[' << entry.index << ']' << std::endl;
					DebugLogLine("write index[" + entry.index + "]");
				}
			}
			if (strlen(entry.name.c_str()) == 0 || strlen(entry.value.c_str()) == 0) {
				DebugLogLine("skip invalid entry");
				continue;
			}
			file_stream_ << entry.name << '=' << entry.value << std::endl;
			DebugLogLine("write :" + entry.name + "=" + entry.value);
		}
		file_stream_ << std::endl;
		file_stream_.close();
		DebugLogLine("write configfile[" + file_to_write_too + "] end");
	}

	void ConfigIni::TurnOffAutoSave() {
		auto_save_ = false;
	}

	bool ConfigIni::GetBoolValue(const std::string index, const std::string name, bool & result) {
		std::string result_string;
		if (GetStringValue(index, name, result_string)) {
			if (result_string == "true") {
				result = true;
			} else if (result_string == "false") {
				result = false;
			} else {
				Log("invalid bool type");
				return false;
			}
			return true;
		}
		return false;
	}

	bool ConfigIni::GetIntValue(const std::string index, const std::string name, int & result) {
		std::string result_string;
		if (GetStringValue(index, name, result_string)) {
			result = stoi(result_string);
			return true;
		}
		return false;
	}

	bool ConfigIni::GetFloatValue(const std::string index, const std::string name, float& result) {
		std::string result_string;
		if (GetStringValue(index, name, result_string)) {
			result = stof(result_string);
			return true;
		}
		return false;
	}

	bool ConfigIni::GetStringValue(const std::string index, const std::string name, std::string& result) {
		DebugLogLine("find index[" + index + "] - name[" + name + "]");
		for (unsigned int i = 0; i < vector_data_.size(); i++) {
			if (vector_data_[i].index == index) {
				DebugLogLine("found index[" + vector_data_[i].index + "]");
				for (; i < vector_data_.size(); i++) {
					if (vector_data_[i].name == name) {
						DebugLogLine("found name[" + vector_data_[i].name + "] - value[" + vector_data_[i].value + "]");
						result = vector_data_[i].value;
						return true;
					}
				}
			}
		}
		Log("DEBUG: [" + index + "]: [" + name + "] not found");
		return false;
	}

	void ConfigIni::SetBoolValue(const std::string index, const std::string name, const bool value) {
		SetStringValueWithIndex(index, name, value ? "true" : "false");
	}

	void ConfigIni::SetIntValue(const std::string index, const std::string name, const int value) {
		SetStringValueWithIndex(index, name, std::to_string(value));
	}

	void ConfigIni::SetFloatValue(const std::string index, const std::string name, const float value) {
		SetStringValueWithIndex(index, name, std::to_string(value));
	}

	void ConfigIni::SetStringValue(const std::string index, const std::string name, const std::string value) {
		SetStringValueWithIndex(index, name, value);
	}

	void ConfigIni::SetStringValueWithIndex(const std::string index, const std::string name, const std::string value) {
		auto_save_ = true;
		// create the new entry.
		ConfigIniEntry entry;
		entry.index = index;
		entry.name = name;
		entry.value = value;

		if (vector_data_.size() == 0) {
			// vector is empty so just push it.
			vector_data_.push_back(entry);

			return;
		}
		// Now we go through all the data and check for an entry.
		std::vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin();
		bool found_index = false;
		bool found_name = false;
		std::vector<ConfigIniEntry>::iterator iterator_insert_position;
		for (entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
			if (found_index == false) {
				// Check if correct index.
				if (entry_iterator->index == index) {
					found_index = true;
				}
			}
			if (found_index == true) {
				// Check if we've went through all the names on this index?
				if (entry_iterator->index != index) {
					break;
				} else {
					iterator_insert_position = entry_iterator;
				}
				// Check if we have the correct name
				if (entry_iterator->name == name) {
					found_name = true;
					iterator_insert_position = entry_iterator;
					break;
				}
				continue;
			}
			iterator_insert_position = entry_iterator;
		}
		// If it exists replace whats their.
		if (found_index && found_name) {
			iterator_insert_position->value = value;
			return;
		}
		// Otherwise it doesn't exist so add it to the end of that index.
		vector_data_.insert(++iterator_insert_position, 1, entry);
	}

	void ConfigIni::LoadConfigFile() {

		std::fstream file_stream_;

		file_stream_.open(ini_filename_, std::ios::in);
		if (!file_stream_) {
			Log("inifile [" + ini_filename_ + "] not found, create a new file: WriteConfigFile()");
			return;
		} else {
			DebugLogLine("file open OK");
		}
		char line[4096];
		char ch;
		int i = 0;
		std::string index;
		std::string temporary_string_data_;
		bool is_comment = false;
		while (!file_stream_.eof()) {
			file_stream_.read(&ch, 1);

			ConfigIniEntry entry;
			if (ch == '#' && i == 0) {
				is_comment = true;
			}
			if (is_comment == true && (ch == '\n' || ch == '\r')) {
				is_comment = false;
				line[i++] = '\0';
				i = 0;
				entry.is_comment = true;
				entry.comment = line;
				vector_data_.push_back(entry);
			}
			if (is_comment == true) {
				line[i++] = ch;
				continue;
			}
			//zfu: all up for comment
			if (ch != '\n' || ch == '\r') {
				line[i++] = ch;
			} else {
				if (i == 0) {
					continue;
				}
				line[i] = '\0';
				temporary_string_data_ = std::string(line);
				DebugLogLine("read one line {" + temporary_string_data_ + "}");
				if (line[0] == '[') {
					index = temporary_string_data_;
				} else {
					entry.index = index.substr(1, index.length() - 2);
					size_t fIndex = temporary_string_data_.find_first_of('=');
					entry.name = temporary_string_data_.substr(0, fIndex);
					entry.value = temporary_string_data_.substr(fIndex + 1, temporary_string_data_.length() - fIndex - 1);
					vector_data_.push_back(entry);
					DebugLogLine("entry: index@[" + entry.index + "]\t name@[" + entry.name + "]\t value@[" + entry.value + "]");
				}
				i = 0;
			}
		}
		if (i != 0) {
			ConfigIniEntry entry;
			entry.index = temporary_string_data_;
			size_t fIndex = temporary_string_data_.find_first_of('=');
			entry.name = temporary_string_data_.substr(0, fIndex);
			entry.value = temporary_string_data_.substr(fIndex + 1, temporary_string_data_.length() - fIndex - 1);
			vector_data_.push_back(entry);
			DebugLogLine("last added entry: index@[" + entry.index + "]\t name@[" + entry.name + "]\t value@[" + entry.value + "]");
		}
		file_stream_.close();
	}

	void ConfigIni::DebugLogAllCurrentData() {
		for (std::vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
			ConfigIniEntry entry = *entry_iterator;
			Log("--------print All Entry of file[" + ini_filename_ + "]------------");
			if (entry.is_comment) {
				Log(entry.comment.c_str());
				continue;
			}
			Log("  index:" + entry.index);
			Log("  name:" + entry.name);
			Log("  value:" + entry.value);
		}
	}

}
