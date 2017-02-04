#include "ConfigIni.h"
#include <iostream>

/*
 *	Example of ConfigIni being used, with break down of Gotcha's.
 *
 */
int main() {
	ConfigIni ini = ConfigIni("test.ini");
	ini.SetIntValue("section1", "intValue", 1);
	ini.SetFloatValue("section1", "floatValue", 0.1f);
	ini.SetStringValue("section2", "stringValue", "hello, world");
	ini.SetBoolValue("section2", "boolValue", false);

	// This stops it from saving when it is destroyed, Editing files afterwards will turn auto save back on.
	ini.TurnOffAutoSave();

	// This happens by default when it goes out of scope aswell if you have edited anything.
	ini.WriteConfigFile("Newfile.ini");

	// -1 if not found, so best storing >0 values, should more than likely change this.
	// TODO: &Param to fill, and return bool if succeded.
	int intValue = ini.GetIntValue("section1", "intValue");
	
	// Also -1.0 if not found, so best storing >0 values, should more than likely change this.
	// TODO: &Param to fill, and return bool if succeded.
	float floatValue = ini.GetFloatValue("section1", "floatValue");

	// Remember to check if this is valid, as string returns null when not found.
	const char *stringValue = ini.GetStringValue("section2", "stringValue");
	bool boolValue = ini.GetBoolValue("section2", "boolValue");

	std::cout << "[section1]-intValue:\t" << intValue << std::endl;
	std::cout << "[section1]-floatValue:\t" << floatValue << std::endl;	

	// check if its valid, as string returns null.
	if (stringValue) {
		std::cout << "[section2]-stringValue:\t" << stringValue << std::endl;
	}
	std::cout << "[section2]-boolValue:\t" << boolValue << std::endl;	

	return 0;
}

