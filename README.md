# DebugJSON
JSON library for embedded and hobby projects where one need to monitor JSON string for debugging or other purpose. This is not recommended for production use due to slowness beacuse of its design. But can be used if performance is not an issue and low overall RAM usage is needed.

# Features
Below are the features -
* No dynamic memory allocation.
* Supports C and C++.
* No need to parse and unparse JSON string.
* Easy interface for C++

# Funtionality
Below are the list of functionality and their status -

- [x] Get
- [x] Set
- [x] Update
- [x] Remove
- [x] Get array size
- [x] Validate
- [ ] Minify
- [ ] Iterate Key pair

# Usage
* Refer [DebugJSONDevelopment](https://github.com/hobby-proj-code/DebugJSONDevelopment) for C++ usage. 
* Refer [usage](https://github.com/hobby-proj-code/DebugJSON/blob/main/DebugJSON.hpp) for C. Although this is C++ code, but each function contains idea on how to use it with C.

# Design
## DebugJSON design

```mermaid
block-beta
    columns 5
    block:ap:3
        columns 3
        space space d1>"Application"]:1
        space space  space
        p["Parse JSON string"] space space
        space space space
        space o["JSON operations</br>Set, Get, Update etc."] u["Unparse JSON object to string"]
    end
    block:co:2
        columns 3
        space d2>"Common/seperate storage</br>for library and application. "]:1 space
        space m("Memory storage for JSON String") space

        style m fill:#d6d,stroke:#333,stroke-width:4px
    end
    space space space space space
    block:jl:5
        columns 3
        space d3>"JSON Library"]:1 space 
        space space space
        space r["JSON Library"] space
    end
    p --> r
    o --> r
    u --> r
    r --"Read/Write"--> m
    m --"Read/Write"--> r
    m --> u
    m --> p

 ```   

## JSON Libraries usual design
```mermaid
block-beta
    columns 5
    block:ap:5
        columns 5
        space space d1>"Application"]:1 space space 
        space space space space space 
        space space m1["Common/Seperate storage</br>for parse and unparse</br>string"] space space
        space space space space space  
        p["Parse JSON string"] space o["JSON operations</br>Set, Get, Update etc."] space u["Unparse JSON object to string"]

        m1 --> p
        m1 --> u
        style m1 fill:#d6d,stroke:#333,stroke-width:4px
    end
    space space space space space
    block:jl:5
        columns 3
        space d2>"JSON Library"]:1 space 
        space space space
        r["JSON Library"] space m("Memory storage for JSON object")

        r --"Read/Write"--> m
        m --"Read/Write"--> r
        style m fill:#d6d,stroke:#333,stroke-width:4px
    end
    p --> r
    o --> r
    u --> r
```   

