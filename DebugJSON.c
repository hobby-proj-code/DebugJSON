#include "DebugJSON.h"
#include <string.h>

typedef enum BeautifyTokenStates
{
    BTS_BEAUTIFY,
    BTS_EXIT
} BeautifyTokenStates;

typedef enum StringTokenStates
{
    STS_TOKEN,
    STS_ESCAPE,
    STS_EXIT,
    STS_ERROR
} StringTokenStates;

typedef enum NumberTokenStates
{
    NTS_NUMBER,
    NTS_DECIMAL,
    NTS_ERROR
} NumberTokenStates;


typedef enum ValueTypesStates
{
    VTS_NULL,
    VTS_BOOL_TRUE,
    VTS_BOOL_FALSE,
    VTS_OBJECT,
    VTS_ARRAY,
    VTS_NUMBER,
    VTS_STRING,
    VTS_ERROR
} ValueTypesStates;

unsigned char CompareTillFirstTermination(const char* firstP, const char* secondP, const char* secondPEnd)
{
    while(*firstP != '\0' && secondP < secondPEnd && *firstP++==*secondP++);
    return *firstP == '\0' && *(firstP-1)==*(secondP-1) && ++secondP == secondPEnd ? 1 : 0;
}

unsigned char DoubleToText(double value, char** buffer)
{
    if(value<0){*(*buffer)++ = '-';value*=-1;}
    
    double majorNumber = 0;
    while(value-majorNumber >= 1)
    {
        double majorNumberAdder = 1;
        while(value >= (majorNumber + majorNumberAdder))
        {
            majorNumberAdder*=2;
        }
        majorNumber += majorNumberAdder/2;
    }
    double minorNumber = value - majorNumber;
    
    int digitsCount = 0;
    double divisor = 1;
    for(
        ;majorNumber/divisor>10
        ;divisor*=10,digitsCount++);
    while(divisor>1)
    {
        int a = majorNumber/divisor;
        *(*buffer)++ = 48 + a;
        majorNumber -= (a*divisor);
        divisor/=10;
    }
    *(*buffer)++ = 48 + (int)majorNumber;
    
    if(minorNumber > 0)
    {
        *(*buffer)++ = '.';
        int a = (minorNumber*=10);
        while(1)
        {
            *(*buffer)++ = 48 + a;
            (minorNumber-=a)*=10;
            if(minorNumber >= 9.9f){break;}
            a = minorNumber;
        }
        *((*buffer)-1) = 48 + a + 1;
    }
    
    return 1;
}

int EstimateDoubleToText(double value)
{
    int buffer = 0;
    if(value<0){buffer++;value*=-1;}
    
    double majorNumber = 0;
    while(value-majorNumber >= 1)
    {
        double majorNumberAdder = 1;
        while(value >= (majorNumber + majorNumberAdder))
        {
            majorNumberAdder*=2;
        }
        majorNumber += majorNumberAdder/2;
    }
    double minorNumber = value - majorNumber;
    
    int digitsCount = 0;
    double divisor = 1;
    for(
        ;majorNumber/divisor>10
        ;divisor*=10,digitsCount++);
    buffer += (++digitsCount);
    
    if(minorNumber > 0)
    {
        buffer++;
        int a = (minorNumber*=10);
        buffer++;
        minorNumber-=a;
        while(minorNumber > 0)
        {
            a = (minorNumber*=10);
            if(a<5){break;}
            buffer++;
            minorNumber-=a;
        }
    }
    
    return buffer;
}

JSONParseOutput JSONParse(JSONParseInput input)
{
    DEBUG("pathLen: %d\r\n", pathLen);
    JSONParseOutput output = {0};
    char *iter = input.json;
    output.state = JS_START;
    output.searchState = JSS_START;
    unsigned char keyOfInterest = 0;
    int arrayLevelParse = -1;
    int isArrayOrObject[input.level];
    for(int counterInit = 0; counterInit < input.level; counterInit++){isArrayOrObject[counterInit] = 0;}
    int levelCounter = 0;
    output.lastArrayLevel = -1;
    unsigned char loop = 1;
    /*
    TODO
    int lengthForMinify = 0;
    if(input.minify)
    {
        const char* minifyIter = input.json;
        while(*minifyIter++ != '\0');
        lengthForMinify = minifyIter - input.json -1;
    }*/

    char *currIter = iter;
    JSONStates currState = output.state;
    #define nextIter iter
    #define nextState output.state

    while (loop)
    {
        
        // DEBUG("%d|%s%.50s\r\n", iter, "loop -> ",iter);
        {
            const unsigned char map[MAP_SIZE] = {MAP_BEAUTIFY};
            while (map[*iter++] == BTS_BEAUTIFY)
                ;
            iter--;
        }
        //DEBUG("%d|%s%.50s\r\n", iter,  "Beautify -> ", iter);

        //////////////////////////////////////////////////////////////////////
        currIter = iter;
        currState = output.state;
        //////////////////////////////////////////////////////////////////////

        switch (output.state)
        {
        case JS_START:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_START -> ", iter);
            if (*iter == '{')
            {
                output.state = JS_FIRST_KEY;
                SET_OBJECT;
                DEBUG_LEVEL_COUNTER;
                iter++;
            }
            else
            {
                //output.valueStart = nullptr;
                //output.valueEnd = nullptr;
                output.state = JS_ERROR;
            }
            break;
        case JS_FIRST_KEY:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_FIRST_KEY -> ", iter);
            if (*iter == '"')
            {output.state = JS_KEY;}
            else if (*iter == '}')
            {output.state = JS_END;}
            else
            {output.state = JS_ERROR;}
            break;
        case JS_COMMA_OR_END:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_COMMA_OR_END -> ", iter);
            if(*iter == ',')
            {output.state = JS_KEY; iter++;}
            else if (*iter == '}')
            {output.state = JS_END;}
            else
            {output.state = JS_ERROR;}
            break;
        case JS_KEY:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_KEY -> ", iter);
            if(*iter++ == '"')
            {
                auto keyStartIter = iter;
                const unsigned char map[STS_EXIT][MAP_SIZE] = {{MAP_TOKEN_STS_TOKEN}, {MAP_TOKEN_STS_ESCAPE}};
                StringTokenStates tokState = STS_TOKEN;
                while ((tokState = (StringTokenStates)map[tokState][*iter++]) < STS_EXIT)
                    ;
                if (tokState == STS_ERROR)
                {
                    output.state = JS_ERROR;
                }
                else
                {
                    output.state = JS_KEY_VALUE_SEPERATOR;
                }
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_KEY_VALUE_SEPERATOR:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_KEY_VALUE_SEPERATOR -> ", iter);
            if (*iter == ':')
            {
                output.state = JS_SEARCH_VALUE_TYPE;
                iter++;
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END -> ", iter);
            if(*iter == ',')
            {
                iter++;
                output.state = JS_SEARCH_VALUE_TYPE;
            }
            else if(*iter == ']')
            {
                iter++;
                UNSET;
                DEBUG_LEVEL_COUNTER;
                output.state = JS_ARRAY_OR_OBJECT;
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_END:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_END -> ", iter);
            if (*iter == '}')
            {
                UNSET;
                DEBUG_LEVEL_COUNTER;
                iter++;
                if(levelCounter == 0)
                {
                    output.state = JS_COMPLETE;
                }
                else
                {
                    output.state = JS_ARRAY_OR_OBJECT;
                }
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_SEARCH_VALUE_TYPE:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_SEARCH_VALUE_TYPE -> ", iter);
            {
                JSONStates vtsToJsMap[] = {JS_VALUE_NULL, JS_VALUE_BOOL_TRUE,
                    JS_VALUE_BOOL_FALSE,
                    JS_VALUE_OBJECT, JS_VALUE_ARRAY, JS_VALUE_NUMBER,
                    JS_VALUE_STRING, JS_ERROR};
                const unsigned char map[MAP_SIZE] = {MAP_VALUE_TYPE_SEARCH};
                output.state = vtsToJsMap[map[*iter]];
            }
            break;
        case JS_VALUE_NULL:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_NULL -> ", iter);
            if(*iter++ == 'n' && *iter++ == 'u' && *iter++ == 'l' && *iter++ == 'l')
            {
                output.state = JS_ARRAY_OR_OBJECT;
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_VALUE_BOOL_TRUE:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_BOOL_TRUE -> ", iter);
            if(*iter++ == 't' && *iter++ == 'r' && *iter++ == 'u' && *iter++ == 'e')
            {
                output.state = JS_ARRAY_OR_OBJECT;
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_VALUE_BOOL_FALSE:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_BOOL_FALSE -> ", iter);
            if(*iter++ == 'f' && *iter++ == 'a' && *iter++ == 'l' && *iter++ == 's' && *iter++ == 'e')
            {
                output.state = JS_ARRAY_OR_OBJECT;
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_VALUE_STRING:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_STRING -> ", iter);
            if(*iter++ == '"')
            {
                const unsigned char map[STS_EXIT][MAP_SIZE] = {{MAP_TOKEN_STS_TOKEN}, {MAP_TOKEN_STS_ESCAPE}};
                StringTokenStates tokState = STS_TOKEN;
                while ((tokState = (StringTokenStates)map[tokState][*iter++]) < STS_EXIT)
                    ;
                if (tokState == STS_ERROR)
                {
                    output.state = JS_ERROR;
                }
                else
                {
                    //iter--;
                    output.state = JS_ARRAY_OR_OBJECT;
                }
            }
            else
            {
                output.state = JS_ERROR;
            }
            break;
        case JS_VALUE_NUMBER:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_NUMBER -> ", iter);
            {
                if(*iter == '-') {iter++;}
                const unsigned char map[MAP_SIZE] = {MAP_NUMBER_TOKEN};
                while (map[*iter++] == NTS_NUMBER);
                iter--;
                if(map[*iter] == NTS_DECIMAL)
                {
                    while (map[*++iter] == NTS_NUMBER);
                }
                if(map[*(iter-1)] == NTS_NUMBER)
                {
                    //iter--;
                    output.state = JS_ARRAY_OR_OBJECT;
                }
                else
                {
                    output.state = JS_ERROR;
                }
            }
            break;
        case JS_VALUE_OBJECT:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_OBJECT -> ", iter);
            {
                iter++;
                SET_OBJECT;
                DEBUG_LEVEL_COUNTER;
                output.state = JS_FIRST_KEY;
            }
            break;
        case JS_VALUE_ARRAY:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_VALUE_ARRAY -> ", iter);
            {
                iter++;
                SET_ARRAY;
                DEBUG_LEVEL_COUNTER;
                output.state = JS_FIRST_ARRAY;
            }
            break;
        case JS_FIRST_ARRAY:
            DEBUG("%d|%s%.50s\r\n", iter,  "JS_FIRST_ARRAY -> ", iter);
            if (*iter == ']')
            {
                iter++;
                UNSET;
                DEBUG_LEVEL_COUNTER;
                output.state = JS_ARRAY_OR_OBJECT;
            }
            else
            {output.state = JS_SEARCH_VALUE_TYPE;}
            break;
        case JS_ARRAY_OR_OBJECT:
            DEBUG_LEVEL_COUNTER;
            if(IS_OBJECT)
            {
                output.state = JS_COMMA_OR_END;
            }
            else
            {
                output.state = JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END;
            }
            break;
        case JS_ERROR:
        case JS_COMPLETE:
            DEBUG_FINAL("Exiting Reason = %d, Success = %d, Key result = %d, Key Len = %d\r\n", output.state, 
                output.state == JS_COMPLETE, output.searchState, output.pathLevel);
            DEBUG_FINAL("%d|%s%.50s\r\n", output.valueStart,  "Value start -> ", output.valueStart);    
            DEBUG_FINAL("%d|%s%.50s\r\n", output.valueEnd,  "Value end -> ", output.valueEnd);    
            loop = 0;
            break;
        default:
            output.state = JS_ERROR;
            break;
        }

        //////////////////////////////////////////////////////
        if(input.search)
        {
            DEBUG_SEARCH("\r\nCurrent State: %s, Next State: %s\r\n", JSONStatesString[currState], JSONStatesString[nextState]);
            DEBUG_SEARCH("Iter: %.50s\r\nNext Iter: %.50s\r\n", currIter, nextIter);
            switch (output.searchState)
            {
            case JSS_START:
                /*if(currState == JS_START && *currIter == '{')
                {
                    output.lastObjStart = currIter;
                }*/
                if(nextState == JS_KEY)
                {
                    output.searchState = JSS_SEARCH_KEY;
                }
                break;
            case JSS_SEARCH_KEY:
                if(currState == JS_COMMA_OR_END)
                {
                    output.prevCommaInSubPart = currIter;
                }
                /*if(currState == JS_VALUE_OBJECT)
                {
                    output.lastObjStart = currIter;
                }*/
                if(currState == JS_KEY && nextState == JS_KEY_VALUE_SEPERATOR)
                {
                    if(levelCounter == (output.pathLevel + 1)
                        && CompareTillFirstTermination((input.element+output.pathLevel)->path.key,currIter+1, nextIter) != 0)
                        //&& *(nextIter-1) == '"')
                    {
                        DEBUG_SEARCH("%d|%s%.50s\r\n", iter,  "MATCH ***** ", currIter+1);
                        output.keyStart = currIter;
                        output.keyEnd = nextIter;
                        output.pathLevel++;
                        output.searchState = JSS_CAPTURE;
                    }
                    output.hasElementInSubPart = 1;
                }
                else if(levelCounter == output.pathLevel && currState == JS_END)
                {
                    if(output.valueStart == 0)
                    {
                        output.valueStart = output.valueEnd = currIter;
                        output.searchState = JSS_NOT_FOUND;
                    }
                    else
                    {
                        output.valueEnd = currIter + 1;
                        output.searchState = output.pathLevel == input.pathLen ? JSS_FOUND_COMPLETE : JSS_FOUND_PARTIAL;
                        output.jsonStateForSearch = currState;
                    }
                }
                break;
            case JSS_CAPTURE:
                if(currState == JS_SEARCH_VALUE_TYPE)
                {
                    switch (nextState)
                    {
                    case JS_VALUE_OBJECT:
                        output.searchState = JSS_CAPTURE_OBJECT;
                        output.prevCommaInSubPart = 0;
                        output.hasElementInSubPart = 0;
                        break;
                    case JS_VALUE_ARRAY:
                        if(arrayLevelParse == -1)
                        {
                            arrayLevelParse = levelCounter + 1;
                            output.valueStart = currIter;
                            output.lastArrayLevel = -1;
                        }
                        output.searchState = JSS_CAPTURE_ARRAY;
                        output.prevCommaInSubPart = 0;
                        output.hasElementInSubPart = 0;
                        break;
                    case JS_VALUE_STRING:
                    case JS_VALUE_NUMBER:
                    case JS_VALUE_NULL:
                    case JS_VALUE_BOOL_TRUE:
                    case JS_VALUE_BOOL_FALSE:
                        output.searchState = JSS_CAPTURE_VALUE;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case JSS_CAPTURE_OBJECT:
                if(currState == JS_VALUE_OBJECT)
                {
                    output.valueStart = currIter;
                    output.searchState = JSS_SEARCH_KEY;
                }
                break;
            case JSS_CAPTURE_ARRAY:
                if(arrayLevelParse == levelCounter)
                {
                    if((currState == JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END || currState == JS_FIRST_ARRAY)
                        && nextState == JS_SEARCH_VALUE_TYPE)
                    {
                        output.lastArrayLevel++;
                        if(currState == JS_VALUE_ARRAY_ELEMENT_SEPERATOR_OR_END)
                        {
                            output.prevCommaInSubPart = currIter;
                        }
                        output.hasElementInSubPart = 1;
                    }
                    if((input.element+output.pathLevel)->elementTypeIndex != 0 &&
                        (input.element+output.pathLevel)->path.index == output.lastArrayLevel)
                    {
                        output.pathLevel++;
                        output.valueStart = currIter;
                        arrayLevelParse = -1;
                        output.searchState = JSS_CAPTURE;
                    }
                }
                else if(levelCounter < arrayLevelParse)
                {
                    arrayLevelParse = -1;
                    output.jsonStateForSearch = currState;
                    if(output.pathLevel < input.pathLen)
                    {
                        //output.valueEnd = currIter;
                        output.valueStart = output.valueEnd = currIter;
                        output.searchState = JSS_FOUND_PARTIAL;
                    }
                    else
                    {
                        output.valueEnd = currIter + 1;
                        output.searchState = JSS_FOUND_COMPLETE;
                    }
                }
                break;
            case JSS_CAPTURE_VALUE:
                output.valueStart = currIter;
                output.valueEnd = nextIter;
                if(output.pathLevel < input.pathLen)
                {
                    output.searchState = JSS_FOUND_PARTIAL;
                    output.jsonStateForSearch = currState;
                }
                else
                {
                    output.searchState = JSS_FOUND_COMPLETE;
                    output.jsonStateForSearch = currState;
                }
                break;
            case JSS_FOUND_PARTIAL:
            case JSS_FOUND_COMPLETE:
            case JSS_NOT_FOUND:
                input.search = 0;
                break;
            default:
                break;
            }
        }
        //////////////////////////////////////////////////////

    }
    return output;
    #undef nextIter
    #undef nextState
}

#define COMMA_ADD(value) (value|=1)
#define BRACES_ADD(value) (value|=2)
#define COMMA_BRACES_ADD(value) (value|=3)
#define ADD_COMMA(value) (value & 1)
#define ADD_BRACES(value) (value & 2)

int EstimateSetLength(PathElement* element, int elemLen, void* value, JSONValueType type, unsigned char bracesAndComma)
{
    int counter = 0;
    //invalid JSON - add curly braced at end and start
    //missing key with no key - add key value
    //missing key - add comma, key value

    //prefix

    //value

    //post fix
    return counter;
}

int EstimateCreateJSONData(PathElement* element, int elemLen, void* value, JSONValueType type, unsigned char bracesAndComma)
{
    int size = 0;
    
    if(ADD_COMMA(bracesAndComma)){size++;}
    //1 for start and 1 for end
    if(ADD_BRACES(bracesAndComma)){size+=2;}

    int counter = 0;
    const char* str = 0;
    for(;counter < elemLen; counter++)
    {
        if((element + counter)->elementTypeIndex == 0)
        {
            if(counter){size++;}
            size++;
            str = (element + counter)->path.key;
            while(*str++ != '\0'){size++;}
            size+=2;
        }
        else
        {
            if(counter){size++;}
        }
    }

    switch (type)
    {
    case JVT_BOOL:
        if(*(unsigned char*)value == 0)
        {size+=5;}
        else
        {size+=4;}
        break;
    case JVT_NUMBER:
        size+=EstimateDoubleToText(*(double*)value);
        break;
    case JVT_STRING:
        str = (const char*)value;
        while(*str++ != '\0'){size++;}
        size+=2;
        break;
    case JVT_NULL:
        {size+=4;}
        break;
    default:
        break;
    }

    if(counter > 0)
    {size+=(counter-1);}

    return size;
}

int CreateJSONData(char* buff, PathElement* element, int elemLen, void* value, JSONValueType type, unsigned char bracesAndComma)
{
    char* iter = buff;

    if(ADD_COMMA(bracesAndComma)){*iter++ = ',';}
    if(ADD_BRACES(bracesAndComma)){*iter++ = '{';}

    int counter = 0;
    const char* str = 0;
    for(;counter < elemLen; counter++)
    {
        if((element + counter)->elementTypeIndex == 0)
        {
            if(counter){*iter++ = '{';}
            *iter++ = '"';
            str = (element + counter)->path.key;
            while(*str != '\0'){*iter++ = *str++;}
            *iter++ = '"'; *iter++ = ':';
        }
        else
        {
            if(counter){*iter++ = '[';}
        }
    }

    switch (type)
    {
    case JVT_BOOL:
        if(*(unsigned char*)value == 0)
        {*iter++='f';*iter++='a';*iter++='l';*iter++='s';*iter++='e';}
        else
        {*iter++='t';*iter++='r';*iter++='u';*iter++='e';}
        break;
    case JVT_NUMBER:
        DoubleToText(*(double*)value,&iter);
        break;
    case JVT_STRING:
        str = (const char*)value;
        *iter++ = '"';
        while(*str != '\0'){*iter++ = *str++;}
        *iter++ = '"';
        break;
    case JVT_NULL:
        {*iter++='n';*iter++='u';*iter++='l';*iter++='l';}
        break;
    default:
        break;
    }

    
    for(counter--;counter > 0; counter--)
    {
        if((element + counter)->elementTypeIndex == 0)
        {*iter++ = '}';}
        else
        {*iter++ = ']';}
    }

    if(ADD_BRACES(bracesAndComma))
    {*iter++ = '}';}

    return iter - buff;
}

/*JSONSetOutput JSONSet(JSONSetInput input)
{
    JSONSetOutput out = {0};
    JSONParseInput parInp = {input.buff, input.element, input.elemLen, 1, 1, 0};
    JSONParseOutput parOut = JSONParse(parInp);

    //estimate length of new value
    unsigned char commaAndBraces = 0;
    //add comma when
    if(parOut.searchState == JSS_FOUND_PARTIAL
        && parOut.hasElementInSubPart == 1)
    {COMMA_ADD(commaAndBraces);}
    //add braces when
    if(parOut.state == JS_ERROR
        || (input.elemLen - parOut.pathLevel) > 0)
    {
        BRACES_ADD(commaAndBraces);
    }
    
    int oldLen = 0, 
        estimatedLen = EstimateCreateJSONData(input.element+parOut.pathLevel, input.elemLen - parOut.pathLevel,
        input.value, input.type, commaAndBraces);
    if(parOut.state != JS_ERROR)
    {
        char* tempBuff = input.buff;
        while(*tempBuff++ != '\0');
        oldLen = tempBuff - input.buff;
    }
    else
    {
        parOut.valueStart = input.buff;
    }
    if((estimatedLen + oldLen) < input.buffLen)
    {
        //move value end by estimatedLen
        if(parOut.valueEnd && oldLen)
        {
            for(char* iterChar = input.buff + oldLen; iterChar >= parOut.valueEnd;)
            {
                *(iterChar + estimatedLen) = *iterChar--;
            }
        }
        else
        {
            *(input.buff + estimatedLen) = '\0';
        }
        CreateJSONData(parOut.valueStart, input.element+parOut.pathLevel, input.elemLen - parOut.pathLevel,
            input.value, input.type, commaAndBraces);
        out.value = 1;
    }

    return out;
}
*/

JSONSetOutput JSONSet(JSONSetInput input)
{
    JSONSetOutput out = {0};
    JSONParseInput parInp = {input.buff, input.element, input.elemLen, 1, 1, 0};
    JSONParseOutput parOut = JSONParse(parInp);

    //estimate length of new value
    unsigned char commaAndBraces = 0;

    //TODO - parOut.hasElementInSubPart no usability remove
    if(parOut.hasElementInSubPart == 1 //handles parOut.valueStart != 0 and {} empty object and [], which will lead incorrct comma add
        && parOut.valueStart == parOut.valueEnd
        && (*parOut.valueStart == '}' || *parOut.valueStart == ']'))
    {
        COMMA_ADD(commaAndBraces);
    }
    //add comma when
    /*if((parOut.searchState == JSS_FOUND_PARTIAL || parOut.searchState == JSS_NOT_FOUND) //check if we can put parOut.searchState != JSS_FOUND_COMPLETE
        && parOut.hasElementInSubPart == 1)
    {COMMA_ADD(commaAndBraces);}*/
    //add braces when
    if(parOut.state == JS_ERROR
        || (input.elemLen > (parOut.pathLevel + 1) && input.element[parOut.pathLevel + 1].elementTypeIndex == 0))
    {
        BRACES_ADD(commaAndBraces);
    }
    
    int oldLen = 0, 
        estimatedLen = EstimateCreateJSONData(input.element+parOut.pathLevel, input.elemLen - parOut.pathLevel,
        input.value, input.type, commaAndBraces);
    if(parOut.state == JS_ERROR)
    {
        parOut.valueStart = input.buff;
        *(input.buff + estimatedLen) = '\0';
    }
    else
    {
        char* tempBuff = parOut.valueEnd;
        while(*tempBuff++ != '\0');
        oldLen = tempBuff - input.buff;
    }
    if((estimatedLen + oldLen) < input.buffLen)
    {
        //move value end by estimatedLen
        if(parOut.state != JS_ERROR)
        {
            //when estimated legth is more than replacement content
            int textToAdjust = estimatedLen - (parOut.valueEnd - parOut.valueStart);
            if(textToAdjust > 0)
            {
                char* frmChar = input.buff + oldLen, *toChar = frmChar + textToAdjust;
                while(frmChar >= parOut.valueEnd)
                {
                    *toChar-- = *frmChar--;
                }
                
            }
            else if(textToAdjust < 0)
            {
                char* frmChar = parOut.valueEnd, *toChar = frmChar + textToAdjust;
                while(*frmChar != '\0')
                {
                    *toChar++ = *frmChar++;
                }
                *toChar = *frmChar;
            }
        }
        CreateJSONData(parOut.valueStart, input.element+parOut.pathLevel, input.elemLen - parOut.pathLevel,
            input.value, input.type, commaAndBraces);
        out.value = 1;
    }

    return out;
}

unsigned char TextToBool(const char* start, const char* end)
{
    return (end - start == 4) ? CompareTillFirstTermination(JSON_TRUE,start, ++end) : 0;
}

double TextToDouble(const char* start, const char* end)
{  
    int negative = 1;
    int divisor = 1;
    double value = 0;
    if(*start == '-'){ negative = -1; start++; }
    while(start < end && *start >= '0' && *start <= '9')
    {
        value = (value * 10) + (*start++-'0');
    }
    if (start < end && *start == '.')
    {
        start++;
        while (start < end && *start >= '0' && *start <= '9')
        {
            value += (((double)(*start++ - '0')) / (divisor *= 10));
        }
    }
    value*=negative;
    return value;
}

