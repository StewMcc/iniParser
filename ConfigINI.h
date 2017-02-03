/*
 * Create by fuzhuo at 11/12/2013
 * Updated by stewmcc at 03/02/2017
 */
#ifndef CONFIGINI_H
#define CONFIGINI_H

#include<string>
#include<vector>

struct ConfigINIEntry{
    ConfigINIEntry():isComment(false){}
    std::string index;
    std::string name;
    std::string value;
    std::string comment;
    bool isComment;
};

class ConfigINI
{
public:
    ConfigINI(const char *fileName, bool autoCreate=false);
    void writeConfigFile(const char *fileName=NULL);
    ~ConfigINI();
    
    bool getBoolValue(const char* index, const char *name);
    int getIntValue(const char* index, const char *name);
    float getFloatValue(const char* index, const char *name);
    const char* getStringValue(const char* index, const char *name);
    
    void setBoolValue(const char* index, const char *name, bool value);
    void setIntValue(const char* index, const char *name, int value);
    void setFloatValue(const char* index, const char *name, float value);
    void setStringValue(const char *index, const char* name, const char* value);

    
    void printAll();
        
    std::vector<ConfigINIEntry> datas;
private:
    char str[4096];//for temp string data
    void setStringValueWithIndex(const char *index, const char* name, const char* value);
    char iniFileName[4096];
    char *data;
    const char (*lineData)[4096];
    void loadConfigFile();
    std::fstream *fStream;
    bool autoSave;
    bool autoCreate;
};

#endif // CONFIGINI_H

