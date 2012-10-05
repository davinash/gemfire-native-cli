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
#include <sstream>

#include "NCContext.hpp"
#include "NCMode.hpp"

template <typename T>
static T string_to_T(std::string const &val) 
{
  std::istringstream istr(val);
  T returnVal;
  istr >> returnVal;
  return returnVal;
}

NCShellModeSystem::NCShellModeSystem() : m_promptString ("NCShell->System-> "),  
  m_pContext     (NCContext::getContext()) 
{
  m_Cmd2FnMap.insert(std::make_pair("help",              &NCShellModeSystem::Help));  
  m_Cmd2FnMap.insert(std::make_pair("quit",              &NCShellModeSystem::Quit));
  m_Cmd2FnMap.insert(std::make_pair("connect",           &NCShellModeSystem::Connect));
  m_Cmd2FnMap.insert(std::make_pair("setsystemproperty", &NCShellModeSystem::SetSystemProperty));
}

NCShellModeSystem::~NCShellModeSystem() 
{
}

void NCShellModeSystem::getPromptString(std::string& inPrompt) 
{
  inPrompt = m_promptString;
}

ShellPageOperation NCShellModeSystem::Evaluate(const std::vector<std::string>& inVecInputs, std::string& outResult) 
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

ShellPageOperation NCShellModeSystem::Help(std::string& outResult) 
{
  for ( Cmd2FnMap::iterator itr = m_Cmd2FnMap.begin(); itr != m_Cmd2FnMap.end(); ++itr ) 
  {
    outResult.append(itr->first).append("\n");
  }
  return SAME;
}

ShellPageOperation NCShellModeSystem::SetSystemProperty(std::string& outResult) 
{
  std::vector<std::string>::size_type size = m_vecInputs.size();

  if ( size == 2 && m_vecInputs[1] == "--help") 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("setsystemproperty propertyName propertyValue").append("\n");
  }
  else if ( size == 3 ) 
  {
    try 
    {
      m_pContext->getPropertiesPtr()->insert(m_vecInputs[1].c_str(),m_vecInputs[2].c_str());      
    }
    catch(const gemfire::Exception& gemfireExcp) 
    {
      outResult.append(gemfireExcp.getMessage());
    } 
    m_systemProperties.insert(std::make_pair(m_vecInputs[1], m_vecInputs[2]));
  } 
  else 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("setsystemproperty propertyName propertyValue").append("\n");
  }
  m_vecInputs.clear();
  return SAME;
}

ShellPageOperation NCShellModeSystem::Quit(std::string& outResult) 
{
  m_vecInputs.clear();
  return QUIT;
}

ShellPageOperation NCShellModeSystem::Connect(std::string& outResult) 
{
  std::vector<std::string>::size_type size = m_vecInputs.size();
  if ( size == 2 && m_vecInputs[1] == "--help" ) 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("connect [Server|Locator] [serverName | locatorName] port").append("\n");
    outResult.append("If the server or locator is running on local machine and default port use following").append("\n");
    outResult.append("connect [Server|Locator]").append("\n");
    return SAME;
  }
  else if ( size == 2 && (m_vecInputs[1] == "Server"  || m_vecInputs[1] == "Locator")) 
  {
    try 
    {
      std::cout.setstate(std::ios::failbit) ;
      m_pContext->setCachePtr(m_pContext->getCacheFactoryPtr()->create());
      std::cout.clear() ;
    }
    catch(const gemfire::Exception& gemfireExcp) 
    { 
      outResult.append(gemfireExcp.getMessage());
      return SAME;
    }
    m_pContext->setServerPortStr(m_vecInputs[1].append("->localhost:40404"));
  } 
  else if( size == 4 ) 
  {
    try 
    {
      if (m_vecInputs[1] == "Server")  
      {
        m_pContext->setCachePtr( m_pContext->getCacheFactoryPtr()->addServer(m_vecInputs[2].c_str(), string_to_T<int>(m_vecInputs[3]))->create());
        m_pContext->setServerPortStr(m_vecInputs[1].append(":").append(m_vecInputs[2]).append(":").append(m_vecInputs[3]));
      } 
      else if (m_vecInputs[1] == "Locator")  
      {        
        m_pContext->setCachePtr(m_pContext->getCacheFactoryPtr()->addServer(m_vecInputs[2].c_str(),string_to_T<int>(m_vecInputs[3]))->create());
        m_pContext->setServerPortStr(m_vecInputs[1].append(":").append(m_vecInputs[3]));
      } 
    } 
    catch(const gemfire::Exception& gemfireExcp) 
    { 
      outResult.append(gemfireExcp.getMessage());
      return SAME;
    }
  } 
  else 
  {
    outResult.append("Insufficient Argument").append("\n");
    outResult.append("Usage:").append("\n");
    outResult.append("connect [Server|Locator] [serverName | locatorName] port").append("\n");
    outResult.append("If the server or locator is running on local machine and default port use following").append("\n");
    outResult.append("connect [Server|Locator]").append("\n");
    return SAME;
  }
  return NEXT;
}
