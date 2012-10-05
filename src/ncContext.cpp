/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of gemfire-native-cli
 *  Copyright (C) 2012 Avinash Dongre ( dongre.avinash@gmail.com )
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/
#include "ncContext.hpp"

ncContext* ncContext::m_pInstance = 0;  

std::string availableCommands[] = {"connect", "addserver","addlocator","createregion","put","get","destroyregion","disconnect","setsystemproperty","help"};

ncContext* ncContext::GetContext() {
    if (!m_pInstance) 
        m_pInstance = new ncContext;
    return m_pInstance;
}

ncContext::ncContext() {
    m_properties   = NULLPTR;
    m_cacheFactory = NULLPTR;
}; 

gemfire::PropertiesPtr ncContext::GetNCPropertiesPtr() {
    if ( m_properties == NULLPTR ) {
        m_properties = gemfire::Properties::create();
    }
    return m_properties;
}

gemfire::CacheFactoryPtr ncContext::GetCacheFactoryPtr() {
    if ( m_cacheFactory == NULLPTR ) {
        m_cacheFactory = gemfire::CacheFactory::createCacheFactory(GetNCPropertiesPtr());        
    }
}

gemfire::CachePtr ncContext::GetCachePtr() {
    if ( m_cachePtr == NULLPTR ) {
        m_cachePtr = GetCacheFactoryPtr()->create();
    }
    return m_cachePtr;
}

gemfire::RegionFactoryPtr ncContext::GetRegionFactoryPtr() {
    if ( m_regionFactory == NULLPTR ) {
        m_regionFactory = GetCachePtr()->createRegionFactory(gemfire::CACHING_PROXY);
    }
    return m_regionFactory;
}
gemfire::RegionPtr ncContext::GetRegionPtr() {
}

void ncContext::ExecuteHelpCommand(std::string& result) {
    for ( std::set<std::string>::iterator iter =  m_commandSet.begin();
        iter != m_commandSet.end();
        ++iter) {
            result += "   " + *iter;
            result += "\n";
    }
}

void ncContext::ExecuteSetSystemPropertyCmd(CmdInputType& input, std::string& result) {
    if ( input.size() != 3 ) {
        result = "Insufficient Arguments";
    } else {
        try {
            GetNCPropertiesPtr()->insert(input[1].c_str(), input[2].c_str());
        } catch ( gemfire::NullPointerException& ) {
        }
    }
}

void ncContext::ExecuteCreateRegionCmd(CmdInputType& input, std::string& result) {
}

void ncContext::ExecuteCommand(CmdInputType& input, std::string& result) {
    if ( input.size() != 0 )  {
        if ( m_commandSet.find(input[0]) == m_commandSet.end()) {
            result = "Invalid Command";
        } else if ( input[0] == "help" ) {
            ExecuteHelpCommand(result);
        } else if ( input[0] == "setsystemproperty") {
            ExecuteSetSystemPropertyCmd(input, result);
        } else if ( input[0] == "createregion") {
            ExecuteCreateRegionCmd(input, result);
        }
    }
}

void ncContext::initialize() {
    for ( int i = 0; 
        i < ( sizeof((availableCommands)) / sizeof((availableCommands)[0])); 
        i++ ) {
            m_commandSet.insert(availableCommands[i]);
    }
}



