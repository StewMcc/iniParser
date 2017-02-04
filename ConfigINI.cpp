/*
 * Create by fuzhuo at 11/12/2013
 * Updated by stewmcc at 04/02/2017
 */
#include "ConfigIni.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <string>
#include <map>

#include <cstdarg>

using namespace std;

/** Change to use prefered debug technique. **/
#include "OutputDebugConsole.h"
void Log(const char* text, ...) {
	va_list args;
	va_start(args, text);
	LittleLot::OutputDebugText(text, args);
	va_end(args);
}
void DebugLogLine(const char * text, ...) {
#ifdef CONFIGINI_DEBUG
	//  Outputs all the extra debug info to console.
	va_list args;
	va_start(args, text);
	LittleLot::OutputDebugText(text, args);
	va_end(args);
#endif
}

ConfigIni::ConfigIni(const char *fileNameWithPath) :
	file_stream_(NULL),
	auto_save_(false),
	ini_filename_(fileNameWithPath) {
	LoadConfigFile();
}

ConfigIni::~ConfigIni() {
	if (auto_save_) {
		Log("AUTO save Config file[%s]", ini_filename_);
		WriteConfigFile();
	}
}

void ConfigIni::WriteConfigFile(const char* override_filename) {	
	if (override_filename == NULL) {
		override_filename = ini_filename_.c_str();
	}
	fstream file_stream_;
	file_stream_.open(override_filename, ios_base::out | ios_base::trunc);
	DebugLogLine("start write file[%s]", override_filename);
	string index = string("");
	bool with_comment = false;
	bool is_start = true;
	for (vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		ConfigIniEntry entry = *entry_iterator;
		if (entry.is_comment) {
			with_comment = true;
			if (is_start) {
				file_stream_ << entry.comment.c_str() << endl;
			} else {
				file_stream_ << endl << entry.comment.c_str() << endl;
			}
			is_start = false;
			DebugLogLine("write comment:%s", entry.comment.c_str());
			continue;
		}
		if (strcmp(index.c_str(), entry.index.c_str()) != 0) {
			index = entry.index;
			if (with_comment || is_start) {
				file_stream_ << '[' << entry.index << ']' << endl;
				with_comment = false; is_start = false;
				DebugLogLine("write index[%s]", entry.index.c_str());
			} else {
				file_stream_ << endl << '[' << entry.index << ']' << endl;
				DebugLogLine("write index [%s]", entry.index.c_str());
			}
		}
		if (strlen(entry.name.c_str()) == 0 || strlen(entry.value.c_str()) == 0) {
			DebugLogLine("skip invalid entry");
			continue;
		}
		file_stream_ << entry.name << '=' << entry.value << endl;
		DebugLogLine("write :%s=%s", entry.name.c_str(), entry.value.c_str());
	}
	file_stream_ << endl;
	file_stream_.close();
	DebugLogLine("write configfile[%s] end", override_filename);
}

void ConfigIni::TurnOffAutoSave() {
	auto_save_ = false;
}

bool ConfigIni::GetBoolValue(const char* index, const char *name) {
	const char *temporary_sting_data_ = GetStringValue(index, name);
	if (temporary_sting_data_ == NULL) {
		Log("notfound for [%s]-[%s]\n", index, name);
		return false;
	}
	if (strcmp(temporary_sting_data_, "true") == 0) {
		return true;
	} else {
		return false;
	}
}

int ConfigIni::GetIntValue(const char *index, const char* name) {
	const char *temporary_sting_data_ = GetStringValue(index, name);
	if (!temporary_sting_data_) {
		return -1;
	} else {
		return atoi(temporary_sting_data_);
	}
}

float ConfigIni::GetFloatValue(const char* index, const char *name) {
	const char *temporary_sting_data_ = GetStringValue(index, name);
	if (temporary_sting_data_ == NULL) {
		Log("notfound");
		return -1.0;
	}
	return stof(temporary_sting_data_);
}

const char* ConfigIni::GetStringValue(const char* index, const char *name) {
	DebugLogLine("find index[%s]-name[%s]", index, name);
	for (unsigned int i = 0; i < vector_data_.size(); i++) {
		if (strcmp(vector_data_[i].index.c_str(), index) == 0) {
			DebugLogLine("find index[%s]", vector_data_[i].index.c_str());
			for (; i < vector_data_.size(); i++) {
				if (strcmp(vector_data_[i].name.c_str(), name) == 0) {
					return vector_data_[i].value.c_str();
				}
			}
		}
	}
	Log("DEBUG: [%s] of--[%s] not found", index, name);
	return NULL;
}

void ConfigIni::SetBoolValue(const char* index, const char *name, bool value) {
	if (value) {
		sprintf_s(temporary_sting_data_, "true");
	} else {
		sprintf_s(temporary_sting_data_, "false");
	}
	SetStringValueWithIndex(index, name, temporary_sting_data_);
}

void ConfigIni::SetIntValue(const char* index, const char *name, int value) {
	sprintf_s(temporary_sting_data_, "%d", value);
	SetStringValueWithIndex(index, name, temporary_sting_data_);
}

void ConfigIni::SetFloatValue(const char* index, const char *name, float value) {
	sprintf_s(temporary_sting_data_, "%f", value);
	SetStringValueWithIndex(index, name, temporary_sting_data_);
}

void ConfigIni::SetStringValue(const char *index, const char* name, const char* value) {
	SetStringValueWithIndex(index, name, value);
}

void ConfigIni::SetStringValueWithIndex(const char *index, const char* name, const char* value) {
	auto_save_ = true;
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
	vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin();
	bool found_index = false;
	bool found_name = false;
	vector<ConfigIniEntry>::iterator iterator_insert_position;
	for (entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		if (found_index == false) {
			// Check if correct index.
			if (strcmp(entry_iterator->index.c_str(), index) == 0) {
				found_index = true;
			}
		}
		if (found_index == true) {
			// Check if we've went through all the names on this index?
			if (strcmp(entry_iterator->index.c_str(), index) != 0) {
				break;
			} else {
				iterator_insert_position = entry_iterator;
			}
			// Check if we have the correct name
			if (strcmp(entry_iterator->name.c_str(), name) == 0) {
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
		iterator_insert_position->value = string(value);
		return;
	}
	// Otherwise it doesn't exist so add it to the end of that index.
	vector_data_.insert(++iterator_insert_position, 1, entry);
}

void ConfigIni::LoadConfigFile() {

	fstream file_stream_;

	file_stream_.open(ini_filename_, ios::in);
	if (!file_stream_) {
		Log("inifile [%s] not found, create a new file (WriteConfigFile())", ini_filename_);
		return;
	} else {
		DebugLogLine("file open OK");
	}
	char line[4096];
	char ch;
	int i = 0;
	string index;
	string temporary_sting_data_;
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
			temporary_sting_data_ = string(line);
			DebugLogLine("read one line {%s}", temporary_sting_data_.c_str());
			if (line[0] == '[') {
				index = temporary_sting_data_;
			} else {
				entry.index = index.substr(1, index.length() - 2);
				size_t fIndex = temporary_sting_data_.find_first_of('=');
				entry.name = temporary_sting_data_.substr(0, fIndex);
				entry.value = temporary_sting_data_.substr(fIndex + 1, temporary_sting_data_.length() - fIndex - 1);
				vector_data_.push_back(entry);
				DebugLogLine("entry: index@[%s]\t name@[%s]\t value@[%s]", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
			}
			i = 0;
		}
	}
	if (i != 0) {
		ConfigIniEntry entry;
		entry.index = temporary_sting_data_;
		size_t fIndex = temporary_sting_data_.find_first_of('=');
		entry.name = temporary_sting_data_.substr(0, fIndex);
		entry.value = temporary_sting_data_.substr(fIndex + 1, temporary_sting_data_.length() - fIndex - 1);
		vector_data_.push_back(entry);
		DebugLogLine("last added entry: index@[%s]\t name@[%s]\t value@[%s]", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
	}
	file_stream_.close();
}

void ConfigIni::DebugLogAllCurrentData() {
	for (vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		ConfigIniEntry entry = *entry_iterator;
		Log("--------print All Entry of file[%s]------------", ini_filename_);
		if (entry.is_comment) {
			Log(entry.comment.c_str());
			continue;
		}
		Log("  index:%s", entry.index.c_str());
		Log("  name:%s", entry.name.c_str());
		Log("  value:%s", entry.value.c_str());
	}
}

