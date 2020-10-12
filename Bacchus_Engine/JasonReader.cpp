#include "JasonReader.h"
#include <string>

void GetJsonValueFromPath(JSON_Value* pilar, const char* name, JSON_Value** container)
{
	const char* path = json_object_get_string(json_value_get_object(pilar), name);
	(*container) = json_parse_file(path);
}