#ifndef _UTILS_H
#define _UTILS_H

#include <cstdarg>

/// <summary>
/// Little Framework that provides alot of benefit.
/// @StewLMcC - 06/12/16
/// </summary>
namespace LittleLot {
	
	/// <summary>
	/// Outputs the text to the visual studio output window.
	/// Note: Does not run at all in release mode. Contains an endline already.
	/// </summary>
	/// <param name="output"> The text to output to vs window. </param>
	void OutputDebugText(const char* output, ...);

	/// <summary>
	/// Outputs the text to the visual studio output window.
	/// Note: Does not run at all in release mode. Contains an endline already.
	/// This version also takes arguements, Ex: "testing %s, var_string".
	/// </summary>
	/// <param name="output"> The text to output to vs window. </param>
	void OutputDebugText(const char* output, va_list args);
}
#endif