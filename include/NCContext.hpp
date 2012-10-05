/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of clib library
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
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
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <map>

#include <gfcpp/GemfireCppCache.hpp>


class NCContext {
public:
  static NCContext* getContext() {
    if (!m_pGlobalContext) 
      m_pGlobalContext = new NCContext;
    return m_pGlobalContext;
  }
  // Gemfire specific Getters
  gemfire::PropertiesPtr getPropertiesPtr() {
    if ( m_PropertiesPtr == NULLPTR )
      m_PropertiesPtr = gemfire::Properties::create();
    return m_PropertiesPtr;
  }
  gemfire::CacheFactoryPtr getCacheFactoryPtr() {
    if ( m_CacheFactoryPtr == NULLPTR )
      m_CacheFactoryPtr = gemfire::CacheFactory::createCacheFactory(getPropertiesPtr());
    return m_CacheFactoryPtr;
  }
  gemfire::CachePtr getCachePtr() {
    return m_CachePtr;
  }
  void setCachePtr(gemfire::CachePtr inPtr ) {
    m_CachePtr = inPtr;
  }

  gemfire::RegionFactoryPtr getRegionFactoryPtr() {
    return m_regionFactory;
  }
  void setRegionFactoryPtr(gemfire::RegionFactoryPtr inPtr ) {
    m_regionFactory = inPtr;
  }

  gemfire::RegionPtr getRegionPtr(std::string inRegionName) {
    if ( m_regionPtrMap.find(inRegionName) != m_regionPtrMap.end()) {
      return m_regionPtrMap.find(inRegionName)->second;
    }
    return NULLPTR;
  }
  void setRegionPtr(std::string inRegionName, gemfire::RegionPtr inPtr ) {
    m_regionPtrMap.insert(std::make_pair(inRegionName, inPtr));
  }

  std::map<std::string, gemfire::RegionPtr>& getRegionMap() {
    return m_regionPtrMap;
  }

  std::string getServerPortStr() {
    return m_ServerPortStr;
  }
  void setServerPortStr(std::string& inServerPortStr) {
    m_ServerPortStr = inServerPortStr;
  }

  void setRegionString(std::string& inRegionPtr) {
    m_RegionStr = inRegionPtr;
  }

  std::string getRegionString() {
    return m_RegionStr;
  }

  void setCurrentRegion(std::string& inRegionName) {
    m_CurrentRegion = inRegionName;
  }

  std::string getCurrentRegion() {
    return m_CurrentRegion;
  }

private:
  NCContext() {
    m_PropertiesPtr   = NULLPTR;
    m_CacheFactoryPtr = NULLPTR;
    m_CachePtr        = NULLPTR;

  }
  NCContext(NCContext const&) {};
  NCContext& operator=(NCContext const&) {};
  static NCContext* m_pGlobalContext;
  gemfire::PropertiesPtr    m_PropertiesPtr;
  gemfire::CacheFactoryPtr  m_CacheFactoryPtr;
  gemfire::CachePtr         m_CachePtr;
  gemfire::RegionFactoryPtr m_regionFactory;
  std::map<std::string, gemfire::RegionPtr> m_regionPtrMap;
  std::string m_ServerPortStr;
  std::string m_RegionStr;
  std::string m_CurrentRegion;
};

#endif