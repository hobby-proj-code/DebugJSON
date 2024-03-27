#ifndef _BAD_JSON_H_
#define  _BAD_JSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Defines.h"

#define JSON_TRUE "true"
#define JSON_FALSE "false"
#define JSON_NULL "null"


typedef enum JSONStates
{
    JS_START,//0
    JS_FIRST_KEY,//1
    JS_COMMA_OR_END,//2
    JS_KEY,//3
    JS_KEY_VALUE_SEPERATOR,//4
    JS_SEARCH_VALUE_TYPE,//5
    JS_VALUE_OBJECT,//6
    JS_VALUE_BOOL_TRUE,//7
    JS_VALUE_BOOL_FALSE,//8
    JS_VALUE_NULL,//9
    JS_VALUE_NUMBER,//10
    JS_VALUE_STRING,//11
    JS_VALUE_ARRAY,//12
    JS_FIRST_ARRAY,//13
    JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END,//14
    JS_ARRAY_OR_OBJECT,//15
    JS_END,//16
    JS_COMPLETE,//17
    JS_ERROR,//18
} JSONStates;

/*static const char* JSONStatesString[]{
    "JS_START",//0
    "JS_COMMA_OR_END",//1
    "JS_KEY",//2
    "JS_KEY_VALUE_SEPERATOR",//3
    "JS_SEARCH_VALUE_TYPE",//4
    "JS_VALUE_OBJECT",//5
    "JS_VALUE_BOOL_TRUE",//6
    "JS_VALUE_BOOL_FALSE",//7
    "JS_VALUE_NULL",//8
    "JS_VALUE_NUMBER",//9
    "JS_VALUE_STRING",//10
    "JS_VALUE_ARRAY",//12
    "JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END",//13
    "JS_ARRAY_OR_OBJECT",//14
    "JS_END",//15
    "JS_COMPLETE",//16
    "JS_ERROR",//17
};*/

typedef enum JSONSearchStates
{
    JSS_START,//0
    JSS_SEARCH_KEY,//1
    JSS_CAPTURE,//2
    JSS_CAPTURE_VALUE,//3
    JSS_CAPTURE_ARRAY,//4
    JSS_CAPTURE_OBJECT,//5
    JSS_FOUND_PARTIAL,//6
    JSS_FOUND_COMPLETE,//7
    JSS_NOT_FOUND//8
} JSONSearchStates;

typedef enum JSONValueType
{
    JVT_NUMBER,//0
    JVT_NULL,//1
    JVT_STRING,//2
    JVT_BOOL//3
} JSONValueType;


typedef struct PathElement
{
    union
    {
        const char* key;
        int index;
    } path;
    unsigned char elementTypeIndex;
} PathElement;

typedef struct JSONParseInput
{
    char *json;
    PathElement* element; 
    int pathLen; 
    unsigned char search;
    int level;
    unsigned char minify;
} JSONParseInput;

typedef struct JSONParseOutput
{
    char *valueStart;
    char *valueEnd;
    char *keyStart;
    char *keyEnd;
    int pathLevel;
    JSONStates state;
    JSONStates jsonStateForSearch;
    JSONSearchStates searchState;
    int lastArrayLevel;
    //char* prevCommaInSubPart;
    char hasElementInSubPart;
    //char* lastObjStart;
} JSONParseOutput;

typedef struct JSONSetInput
{
    char* buff; 
    int buffLen; 
    PathElement* element; 
    int elemLen; 
    void* value;
    JSONValueType type;
} JSONSetInput;

typedef struct JSONSetOutput
{
    unsigned char value;
} JSONSetOutput;

typedef struct JSONRemoveInput
{
    char* buff; 
    int buffLen; 
    PathElement* element; 
    int elemLen; 
} JSONRemoveInput;

typedef struct JSONRemoveOutput
{
    unsigned char value;
} JSONRemoveOutput;


JSONParseOutput JSONParse(JSONParseInput input);
JSONSetOutput JSONSet(JSONSetInput input);
JSONRemoveOutput JSONRemove(JSONRemoveInput input);

//unsigned char TextToBool(const char* start, const char* end);
double TextToDouble(const char* start, const char* end);

#endif

#ifdef __cplusplus
}
#endif
