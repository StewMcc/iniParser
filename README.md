# iniParser

A simple tool to read/write .ini files.

Heavily modified version of [fuzhuo's iniParser](https://github.com/fuzhuo/iniParser)

## Sample file: test.ini

	#comment line
	[section1]
	intValue=1
	floatValue=0.100000
  
	[section2]
	stringValue=hello, world
	boolValue=true

## C++ Code to read/write it

### Write
```c++
LittleLot::ConfigIni ini = LittleLot::ConfigIni("test.ini");
ini.SetIntValue("section1", "intValue", 1);
ini.SetFloatValue("section1", "floatValue", 0.1f);
ini.SetStringValue("section2", "stringValue", "hello, world");
ini.SetBoolValue("section2", "boolValue", false);
```

### Saving
```c++
// This stops it from saving when it is destroyed, Editing files afterwards will turn auto save back on.
ini.TurnOffAutoSave();
// Note: cause auto save is turned off it will not actually save/override test.ini when it loses scope.

// This happens by default when it goes out of scope aswell if you have edited anything.
// However here we save it to a seperate file instead of its original file.	
ini.WriteConfigFile("Newfile.ini");
```
### Reading
```c++
// Try to get a parameter
int intValue;
if (ini.GetIntValue("section1", "intValue", intValue)) {
	// it was successful so output to the console.
	std::cout << "[section1]-intValue:\t" << intValue << std::endl;
}
float floatValue;
if (ini.GetFloatValue("section1", "floatValue", floatValue)) {
	std::cout << "[section1]-floatValue:\t" << floatValue << std::endl;
}

std::string string_value;
if (ini.GetStringValue("section2", "stringValue", string_value)) {
	std::cout << "[section2]-stringValue:\t" << string_value << std::endl;
}

bool boolValue;
if (ini.GetBoolValue("section2", "boolValue", boolValue)) {
	std::cout << "[section2]-boolValue:\t" << boolValue << std::endl;
}
```
