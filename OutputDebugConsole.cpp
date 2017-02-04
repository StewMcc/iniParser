#include "OutputDebugConsole.h"

#include <windows.h>
#include <sstream>

/// <summary>
/// Little Framework that provides alot of benefit.
/// @StewLMcC - 06/12/16
/// </summary>
namespace LittleLot {
	/// <summary>
	/// Combine a char* with argument list generated from (char*,...) type functions.
	/// Usage: 
	/// va_list args;
	/// va_start(args, output);
	/// char * text_buffer = FormatArgsToChar(output, args);
	/// va_end(args);
	/// Creates a single char* from the elipeses arguments.
	/// </summary>
	/// <param name="text"></param>
	/// <param name="args"></param>
	/// <returns></returns>
	char * FormatArgsToChar(const char * text, va_list args) {
		int len;
		char * text_buffer;
		// gets the size of the combined text and args, so dont have buffer overflow.
		len = _vscprintf(text, args) + 1; // _vscprintf doesn't count /0		
		text_buffer = new char[len];

		// basically does the formating of printf but binds it to the text buffer instead
		vsprintf_s(text_buffer, len, text, args);
		return text_buffer;
	}

	/// <summary>
	/// Outputs the text to the visual studio output window.
	/// Note: Does not run in release mode. Contains an endline already.	
	/// Usage: 
	/// va_list args;
	/// va_start(args, output);
	/// OutputDebugText(const char* output, va_list args)
	/// va_end(args);
	/// Creates a single char* from the elipeses arguments.
	/// </summary>
	/// <param name="output"> The text to output to vs window. </param>
	/// <param name="args"> The list of arguments to be combined into the string. </param>
	void OutputDebugText(const char* output, va_list args) {
	#ifdef _DEBUG	
		char * text_buffer = FormatArgsToChar(output, args);
		std::wstringstream ss;
		ss << text_buffer << std::endl;
		OutputDebugString(ss.str().c_str());
	#endif // DEBUG
	}

	/// <summary>
	/// Outputs the text to the visual studio output window.
	/// Note: Does not run in release mode. Contains an endline already.
	/// </summary>
	/// <param name="output"> The text to output to vs window. </param>
	/// <param name=""> As many extra arguments as we want for %s, %d, etc.</param>
	void OutputDebugText(const char* output, ...) {
	#ifdef _DEBUG	
		va_list args;
		va_start(args, output);		
		OutputDebugText(output, args);
		va_end(args);
	#endif // DEBUG
	}

	

}
