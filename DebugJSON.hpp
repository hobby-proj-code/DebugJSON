#ifndef _BAD_JSON_HPP_
#define _BAD_JSON_HPP_

#include "DebugJSON.h"
#include <type_traits>

class IsNull
{
    public:
    unsigned char value;
};

template <class ReturnType>
class JSONSearchResult
{
public:
    bool status = false;
    using ValueDeclarationType = typename std::conditional_t<std::is_same_v<ReturnType, IsNull>,bool,ReturnType>;
    ValueDeclarationType value{};

    using JSONSearchResultGetDefaultType = typename std::conditional_t<
        std::is_array_v<ReturnType> && std::is_same_v<std::remove_all_extents_t<ReturnType>, char>,
        const char *, std::conditional_t<std::is_same_v<ReturnType, IsNull>,bool,ReturnType>>;

    JSONSearchResult() {}
    JSONSearchResult(JSONParseOutput output)
    {
        if constexpr (std::is_same<bool, ReturnType>::value)
        {
            if (status = (output.searchState == JSONSearchStates::JSS_FOUND_COMPLETE && (output.jsonStateForSearch == JS_VALUE_BOOL_FALSE || output.jsonStateForSearch == JS_VALUE_BOOL_TRUE)))
            {
                value = output.jsonStateForSearch == JS_VALUE_BOOL_TRUE;
            }
        }
        else if constexpr (std::is_same<IsNull, ReturnType>::value)
        {
            if (status = (output.searchState == JSONSearchStates::JSS_FOUND_COMPLETE && output.jsonStateForSearch == JS_VALUE_NULL))
            {
                value = output.jsonStateForSearch == JS_VALUE_NULL;
            }
        }
        else if constexpr (std::is_same<int, ReturnType>::value || std::is_same<float, ReturnType>::value || std::is_same<double, ReturnType>::value)
        {
            if (status = (output.searchState == JSONSearchStates::JSS_FOUND_COMPLETE && (output.jsonStateForSearch == JS_VALUE_NUMBER)))
            {
                value = TextToDouble(output.valueStart, output.valueEnd);
            }
        }
        else if constexpr (std::is_array_v<ReturnType> && std::is_same_v<std::remove_all_extents_t<ReturnType>, char>)
        {
            if (status = (output.searchState == JSONSearchStates::JSS_FOUND_COMPLETE && (output.jsonStateForSearch == JS_VALUE_STRING)))
            {
                if (output.valueEnd - ++output.valueStart < sizeof(ReturnType))
                {
                    char *data = value;
                    while ((*data++ = *output.valueStart++, output.valueStart < output.valueEnd))
                        ;
                    *(data - 1) = '\0';
                }
            }
        }
        else
        {
            int a = nullptr; // error not supported type
        }
    }

    JSONSearchResultGetDefaultType get()
    {
        if constexpr (std::is_same<bool, ReturnType>::value || std::is_same<IsNull, ReturnType>::value)
        {
            return get(false);
        }
        else if constexpr (std::is_same<int, ReturnType>::value || std::is_same<float, ReturnType>::value || std::is_same<double, ReturnType>::value)
        {
            return get(0);
        }
        else if constexpr (std::is_array_v<ReturnType> && std::is_same_v<std::remove_all_extents_t<ReturnType>, char>)
        {
            return get("");
        }
        else
        {
            char error = nullptr; // not supported type
        }
    }

    /*ReturnType get(std::conditional<
        std::is_array<ReturnType>::value && std::is_same<std::remove_all_extents<ReturnType>,char>::value,
        const char*,ReturnType>::type
         defaultVal)*/
    JSONSearchResultGetDefaultType get(JSONSearchResultGetDefaultType defaultVal)
    {
        return status ? value : defaultVal;
    }
};

class JSONSearchResultRaw
{
public:
    bool status = false;
    const char* start = nullptr;
    const char* end = nullptr;
    JSONSearchResultRaw(JSONParseOutput output)
    {
        status = (output.searchState == JSONSearchStates::JSS_FOUND_COMPLETE);
        start = output.valueStart;
        end = output.valueEnd;
    }
};

class JSON
{
public:
    char *jsonBuff = nullptr;
    int buffSize = 0;
    bool status = false;
    JSON(char *jsonBuffer, int size) : jsonBuff(jsonBuffer), buffSize(size) {}

    template <class GetType, typename... Args>
    JSONSearchResult<GetType> path(Args... args)
    {
        PathElement pathElems[] = {
            {(const char *)args,
             std::is_same<int, Args>::value}...};
        return JSONSearchResult<GetType>(
            JSONParse(
                JSONParseInput{jsonBuff, pathElems, sizeof(pathElems) / sizeof(PathElement), true, 1, false}));
    }

    template <typename... Args>
    JSONSearchResultRaw getRaw(Args... args)
    {
        PathElement pathElems[] = {
            {(const char *)args,
             std::is_same<int, Args>::value}...};
        return JSONSearchResultRaw(
            JSONParse(
                JSONParseInput{jsonBuff, pathElems, sizeof(pathElems) / sizeof(PathElement), true, 1, false}));
    }

    template <typename... Args>
    int getArraySize(Args... args)
    {
        PathElement pathElems[] = {
            {(const char *)args,
             std::is_same<int, Args>::value}...,
            {(const char *)(-2),
             true}};
        int pathElemsSize = sizeof(pathElems) / sizeof(PathElement);
        JSONParseOutput output = JSONParse(
            JSONParseInput{jsonBuff, pathElems, pathElemsSize, true, 1, false});
        return output.pathLevel == (pathElemsSize - 1) && output.searchState == JSS_FOUND_PARTIAL
                   ? (output.lastArrayLevel + 1)
                   : -1;
    }

    bool validate()
    {
        return JSONParse(JSONParseInput{jsonBuff, nullptr, 0, false, 1, false}).state == JS_COMPLETE;
    }

    /*
    TODO
    bool minify()
    {
        return JSONParse(JSONParseInput{jsonBuff, nullptr, 0, false, 1, true}).state == JS_COMPLETE;
    }*/

    template <class SetType, typename... Args>
    bool set(SetType obj, Args... args)
    {
        PathElement pathElems[] = {
            {(const char *)args,
             std::is_same<int, Args>::value}...};
        

        if constexpr (std::is_same<bool, SetType>::value)
        {
            return JSONSet({jsonBuff, buffSize, pathElems, sizeof(pathElems) / sizeof(PathElement), &obj, JVT_BOOL}).value;   
        }
        else if constexpr (std::is_same<IsNull, SetType>::value)
        {
            return JSONSet({jsonBuff, buffSize, pathElems, sizeof(pathElems) / sizeof(PathElement), nullptr, JVT_NULL}).value;
        }
        else if constexpr (std::is_same<int, SetType>::value || std::is_same<float, SetType>::value || std::is_same<double, SetType>::value)
        {
            double val = obj;
            return JSONSet({jsonBuff, buffSize, pathElems, sizeof(pathElems) / sizeof(PathElement), &val, JVT_NUMBER}).value;
        }
        else if constexpr ((std::is_array_v<SetType> && std::is_same_v<std::remove_all_extents_t<SetType>, char>)
            || std::is_same_v<const char*, SetType>)
        {
            return JSONSet({jsonBuff, buffSize, pathElems, sizeof(pathElems) / sizeof(PathElement), (void*)obj, JVT_STRING}).value;
        }
        else
        {
            int a = nullptr; // error not supported type
        }
    }

    template <typename... Args>
    bool remove(Args... args)
    {
        PathElement pathElems[] = {
            {(const char *)args,
             std::is_same<int, Args>::value}...};
        return JSONRemove({jsonBuff, buffSize, pathElems, sizeof(pathElems) / sizeof(PathElement)}).value;
    }
};

template <int length>
class JSONInstance : public JSON
{
public:
    char json[length]{};
    JSONInstance() : JSON(nullptr, 0)
    {
        jsonBuff = json;
        buffSize = length;
    }

    JSONInstance(const char *string) : JSONInstance()
    {
        char *tempJSON = json;
        char *tempJSONEnd = json + length;
        while (*string != '\0' && tempJSON < tempJSONEnd)
        {
            *tempJSON++ = *string++;
        }
    }
};

#endif
