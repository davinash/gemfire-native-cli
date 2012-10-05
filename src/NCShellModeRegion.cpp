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
#include <vector>
#include <string>
#include <iostream>

#include "NCContext.hpp"
#include "NCMode.hpp"


NCShellModeRegion::NCShellModeRegion() :  m_promptString ("NCShell->"),  
  m_pContext(NCContext::getContext()) 
{
  m_Cmd2FnMap.insert(std::make_pair("help",          &NCShellModeRegion::Help));  
  m_Cmd2FnMap.insert(std::make_pair("leaveregion",    &NCShellModeRegion::LeaveRegion));
  m_Cmd2FnMap.insert(std::make_pair("put",  &NCShellModeRegion::Put));
  m_Cmd2FnMap.insert(std::make_pair("get",   &NCShellModeRegion::Get));  
}

NCShellModeRegion::~NCShellModeRegion() 
{
}

void NCShellModeRegion::getPromptString(std::string& inPrompt) 
{
  inPrompt.clear();
  inPrompt.append("NCShell->").append(m_pContext->getRegionString().append("-> "));
}

ShellPageOperation NCShellModeRegion::Help(std::string& outResult) 
{
  for ( Cmd2FnMap::iterator itr = m_Cmd2FnMap.begin(); itr != m_Cmd2FnMap.end(); ++itr ) 
  {
    outResult.append(itr->first).append("\n");
  }
  return SAME;
}

ShellPageOperation NCShellModeRegion::LeaveRegion(std::string& outResult) {
  return PREVIOUS;
}

ShellPageOperation NCShellModeRegion::Put(std::string& outResult) {
  std::vector<std::string>::size_type size = m_vecInputs.size();
  if ( size == 2 && m_vecInputs[1] == "--help" ) 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("put key value").append("\n");
  }
  else if ( size == 3 ) 
  {
    gemfire::CacheableKeyPtr keyPtr;
    gemfire::CacheablePtr valuePtr;

    keyPtr   = gemfire::CacheableString::create(m_vecInputs[1].c_str());
    valuePtr = gemfire::CacheableString::create(m_vecInputs[2].c_str());

    try 
    {
      m_pContext->getRegionPtr(m_pContext->getCurrentRegion())->put(keyPtr, valuePtr);
    } 
    catch(const gemfire::Exception& gemfireExcp) 
    { 
      outResult.append(gemfireExcp.getMessage());
      return SAME;
    }
  } 
  else 
  {
    outResult.append("Insufficient or Invalid Argument").append("\n");
    outResult.append("Usage:").append("\n");
    outResult.append("put key value").append("\n");
  }
  return SAME;
}
ShellPageOperation NCShellModeRegion::Get(std::string& outResult) 
{
  std::vector<std::string>::size_type size = m_vecInputs.size();
  if ( size == 2 && m_vecInputs[1] == "--help" ) 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("get key").append("\n");
  }
  else if ( size == 2 ) 
  {    
    gemfire::CacheableKeyPtr keyPtr;
    gemfire::CacheablePtr valuePtr;

    try 
    {
      keyPtr   = gemfire::CacheableString::create(m_vecInputs[1].c_str());
      valuePtr = m_pContext->getRegionPtr(m_pContext->getCurrentRegion())->get(keyPtr);
      if ( valuePtr == NULLPTR ) 
      {
        std::cout << "Value not found for Key " << m_vecInputs[1] << std::endl;
        return SAME;
      }
      gemfire::CacheableStringPtr csp = gemfire::dynCast<gemfire::CacheableStringPtr>(valuePtr);
      std::cout << csp->asChar() << std::endl;
    }
    catch(const gemfire::Exception& gemfireExcp) 
    { 
      outResult.append(gemfireExcp.getMessage());
      return SAME;
    }
  } 
  else 
  {
    outResult.append("Insufficient or Invalid Argument").append("\n");
    outResult.append("Usage:").append("\n");
    outResult.append("get key").append("\n");
  }
  return SAME;
}

ShellPageOperation NCShellModeRegion::Evaluate(const std::vector<std::string>& inVecInputs, std::string& outResult) 
{
  m_vecInputs =  inVecInputs;
  try 
  {
    Cmd2FnMap::iterator fptrItr = m_Cmd2FnMap.find(m_vecInputs.at(0));
    if ( fptrItr == m_Cmd2FnMap.end()) 
    {
      outResult.append("Invalid Command").append("\n");
    } 
    else 
    {
      return (this->*(fptrItr->second))(outResult);
    }    
  } 
  catch (std::out_of_range&) 
  {
    outResult.append("Invalid Command").append("\n");
  }
  return SAME;
}




