#pragma once
#ifndef __JSON_READER_H__
#define __JSON_READER_H__

#include "JSON/parson.h"

void GetJsonValueFromPath(JSON_Value* pilar, const char* name, JSON_Value** container);

#endif 