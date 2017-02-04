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
#include <sstream>
#include <string>
#include <map>

using namespace std;

// define to enable debug of every line, good for error checking.
#ifdef CONFIGINI_DEBUG
#include <cstdarg>
#endif

void DebugLogLine(const char * text, ...) {
	//  out put all the debug info to console.
#ifdef CONFIGINI_DEBUG

	va_list args;
	va_start(args, text);
	int len;
	char * text_buffer;
	// gets the size of the combined text and args, so dont have buffer overflow.
	len = _vscprintf(text, args) + 1; // _vscprintf doesn't count /0		
	text_buffer = new char[len];

	// basically does the formating of printf but binds entry_iterator to the text buffer instead
	vsprintf_s(text_buffer, len, text, args);

	printf("%s \n", text_buffer);
	va_end(args);
#endif
}

ConfigIni::ConfigIni(const char *fileNameWithPath) :
	file_stream_(NULL),
	auto_save_(false) {
	strcpy_s(ini_filename_, fileNameWithPath);
	LoadConfigFile();
}

ConfigIni::~ConfigIni() {
	if (auto_save_) {
		cout << "AUTO save Config file[" << ini_filename_ << "]" << endl;
		WriteConfigFile();
	}
}

void ConfigIni::WriteConfigFile(const char* filename) {
	auto_save_ = false;
	if (filename == NULL) filename = ini_filename_;
	fstream file_stream_;
	file_stream_.open(filename, ios_base::out | ios_base::trunc);
	DebugLogLine("start write file[%s]", filename);
	string index = string("");
	bool with_comment = false;
	bool is_start = true;
	for (vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		ConfigIniEntry entry = *entry_iterator;
		if (entry.is_comment) {
			with_comment = true;
			if (is_start) file_stream_ << entry.comment.c_str() << endl;
			else file_stream_ << endl << entry.comment.c_str() << endl;
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
	DebugLogLine("write configfile[%s] end", filename);
}

bool ConfigIni::GetBoolValue(const char* index, const char *name) {
	const char *temporary_sting_data_ = GetStringValue(index, name);
	if (temporary_sting_data_ == NULL) { printf("notfound for [%s]-[%s]\n", index, name); return false; }
	if (strcmp(temporary_sting_data_, "true") == 0) return true;
	else return false;
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
	if (temporary_sting_data_ == NULL) { cout << "notfound" << endl; return -1.0; }
	return stof(temporary_sting_data_);
}

const char* ConfigIni::GetStringValue(const char* index, const char *name) {
	DebugLogLine("find index[%s]-name[%s]", index, name);
	for (unsigned int i = 0; i < vector_data_.size(); i++) {
		if (strcmp(vector_data_[i].index.c_str(), index) == 0) {
			DebugLogLine("find index[%s]", vector_data_[i].index.c_str());
			for (; i < vector_data_.size(); i++) {
				if (strcmp(vector_data_[i].name.c_str(), name) == 0)
					return vector_data_[i].value.c_str();
			}
		}
	}
	cout << "DEBUG: [" << index << "] of--[" << name << "] not found" << endl;
	return NULL;
}

void ConfigIni::SetBoolValue(const char* index, const char *name, bool value) {
	if (value) sprintf_s(temporary_sting_data_, "true");
	else sprintf_s(temporary_sting_data_, "false");
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
	if (vector_data_.size() == 0) {/*cout<<"data is NULL, push and return"<<endl; */
		vector_data_.push_back(entry);
		return;
	}
	vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin();
	bool find_index = false;
	bool find_name = false;
	vector<ConfigIniEntry>::iterator iterator_insert_position;
	for (entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		if (find_index == false) {
			if (strcmp(entry_iterator->index.c_str(), index) == 0) {
				find_index = true;
			}
		}
		if (find_index == true) {
			if (strcmp(entry_iterator->index.c_str(), index) != 0) {
				break;
			} else {
				iterator_insert_position = entry_iterator;
			}
			if (strcmp(entry_iterator->name.c_str(), name) == 0) {
				find_name = true;
				iterator_insert_position = entry_iterator;
				break;
			}
			continue;
		}
		iterator_insert_position = entry_iterator;
	}
	if (find_index && find_name) {
		iterator_insert_position->value = string(value);
		return;
	}

	vector_data_.insert(++iterator_insert_position, 1, entry);
}

void ConfigIni::LoadConfigFile() {

	fstream file_stream_;
	string p = ini_filename_;

	file_stream_.open(p, ios::in);
	if (!file_stream_) {		
		printf("inifile [%s] not found, create a new file (WriteConfigFile())\n", ini_filename_);		
		return;
	} else {
		DebugLogLine("file open OK\n");
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
		if (ch == '#' && i == 0) is_comment = true;
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
		if (ch != '\n' || ch == '\r') line[i++] = ch;
		else {
			if (i == 0) continue;
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
		DebugLogLine("last add entry: index@[%s]\t name@[%s]\t value@[%s]", entry.index.c_str(), entry.name.c_str(), entry.value.c_str());
	}
	file_stream_.close();
}

void ConfigIni::DebugLogAllCurrentData() {
	for (vector<ConfigIniEntry>::iterator entry_iterator = vector_data_.begin(); entry_iterator != vector_data_.end(); entry_iterator++) {
		ConfigIniEntry entry = *entry_iterator;
		printf("\n--------print All Entry of file[%s]------------\n", ini_filename_);
		if (entry.is_comment) { cout << entry.comment << endl; continue; }
		printf("  index:%s\n", entry.index.c_str());
		printf("  name:%s\n", entry.name.c_str());
		printf("  value:%s\n", entry.value.c_str());
	}
}

