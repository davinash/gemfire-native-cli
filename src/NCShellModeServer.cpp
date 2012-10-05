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

#include "NCContext.hpp"
#include "NCMode.hpp"

NCShellModeServer::NCShellModeServer() :  m_promptString ("NCShell->"),  
  m_pContext(NCContext::getContext()) 
{
  m_Cmd2FnMap.insert(std::make_pair("help",          &NCShellModeServer::Help));  
  m_Cmd2FnMap.insert(std::make_pair("disconnect",    &NCShellModeServer::Disconnect));
  m_Cmd2FnMap.insert(std::make_pair("createregion",  &NCShellModeServer::CreateRegion));
  m_Cmd2FnMap.insert(std::make_pair("listregions",   &NCShellModeServer::ListRegions));
  m_Cmd2FnMap.insert(std::make_pair("getregion",   &NCShellModeServer::GetRegions));
}

NCShellModeServer::~NCShellModeServer() 
{
}

void NCShellModeServer::getPromptString(std::string& inPrompt) 
{
  inPrompt.clear();
  inPrompt.append("NCShell->").append(m_pContext->getServerPortStr().append("-> "));
}

ShellPageOperation NCShellModeServer::Help(std::string& outResult) 
{
  for ( Cmd2FnMap::iterator itr = m_Cmd2FnMap.begin(); itr != m_Cmd2FnMap.end(); ++itr ) 
  {
    outResult.append(itr->first).append("\n");
  }
  return SAME;
}

ShellPageOperation NCShellModeServer::GetRegions(std::string& outResult) {
  std::vector<std::string>::size_type size = m_vecInputs.size();
  if ( size == 2 && m_vecInputs[1] == "--help" ) 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("getregion name").append("\n");
    return SAME;
  }
  if ( size == 2 ) 
  {
    std::map<std::string, gemfire::RegionPtr> rmap = m_pContext->getRegionMap();
    std::map<std::string, gemfire::RegionPtr>::iterator found = rmap.find(m_vecInputs[1]);
    if ( found == rmap.end() )
    {
      outResult.append(m_vecInputs[1]);
      outResult.append(" Region does not exist , first create the region using \"createregion\" command").append("\n");
      return SAME;
    }
    m_pContext->setCurrentRegion(m_vecInputs[1]);
  }  
  return NEXT;
}

ShellPageOperation NCShellModeServer::CreateRegion(std::string& outResult) {

  std::vector<std::string>::size_type size = m_vecInputs.size();

  if ( size == 2 && m_vecInputs[1] == "--help" ) 
  {
    outResult.append("Usage:").append("\n");
    outResult.append("createregion name type").append("\n");
    outResult.append("name = Name of the Region").append("\n");
    outResult.append("type = PROXY | CACHING_PROXY | CACHING_PROXY_ENTRY_LRU | LOCAL | LOCAL_ENTRY_LRU").append("\n");
    return SAME;
  }
  if ( size == 3 ) 
  {
    if (m_vecInputs[2] == "PROXY" || m_vecInputs[2] == "CACHING_PROXY" || m_vecInputs[2] == "CACHING_PROXY_ENTRY_LRU" || m_vecInputs[2] == "LOCAL" || m_vecInputs[2] == "LOCAL_ENTRY_LRU")  
    {
      int RegionShortcutNum = 0;

      if      ( m_vecInputs[2] == "PROXY"                   ) RegionShortcutNum = 0;
      else if ( m_vecInputs[2] == "CACHING_PROXY"           ) RegionShortcutNum = 1;
      else if ( m_vecInputs[2] == "CACHING_PROXY_ENTRY_LRU" ) RegionShortcutNum = 2;
      else if ( m_vecInputs[2] == "LOCAL"                   ) RegionShortcutNum = 3;
      else if ( m_vecInputs[2] == "LOCAL_ENTRY_LRU"         ) RegionShortcutNum = 4;

      try 
      {
        m_pContext->setRegionFactoryPtr(m_pContext->getCachePtr()->createRegionFactory((gemfire::RegionShortcut)RegionShortcutNum));
        m_pContext->setRegionPtr(m_vecInputs[1], m_pContext->getRegionFactoryPtr()->create(m_vecInputs[1].c_str()));
        m_pContext->setRegionString(m_pContext->getServerPortStr().append("->").append(m_vecInputs[1]));
        m_pContext->setCurrentRegion(m_vecInputs[1]);
      }
      catch(const gemfire::Exception& gemfireExcp) 
      { 
        outResult.append(gemfireExcp.getMessage());
        return SAME;
      }
    } 
    else 
    {
      outResult.append("Type should be one of PROXY | CACHING_PROXY | CACHING_PROXY_ENTRY_LRU | LOCAL | LOCAL_ENTRY_LRU").append("\n");
      return SAME;
    }
  } 
  else 
  {
    outResult.append("Insufficient Argument").append("\n");
    outResult.append("Usage:").append("\n");
    outResult.append("createregion name type").append("\n");
    outResult.append("name = Name of the Region").append("\n");
    outResult.append("type = PROXY | CACHING_PROXY | CACHING_PROXY_ENTRY_LRU | LOCAL | LOCAL_ENTRY_LRU").append("\n");
    return SAME;
  }
  return NEXT;
}
ShellPageOperation NCShellModeServer::ListRegions(std::string& outResult) {
  std::map<std::string, gemfire::RegionPtr> rmap = m_pContext->getRegionMap();
  for ( std::map<std::string, gemfire::RegionPtr>::iterator itr = rmap.begin(); itr != rmap.end(); ++itr ) {
    outResult.append(itr->first).append("\n");
  }
  return SAME;
}

ShellPageOperation NCShellModeServer::Disconnect(std::string& outResult) 
{
  try 
  {
    m_pContext->getCachePtr()->close();
  } 
  catch(const gemfire::Exception& gemfireExcp) 
  { 
    outResult.append(gemfireExcp.getMessage());
    return SAME;
  }
  return PREVIOUS;
}

ShellPageOperation NCShellModeServer::Evaluate(const std::vector<std::string>& inVecInputs, std::string& outResult) 
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




