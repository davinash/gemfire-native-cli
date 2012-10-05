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
#ifndef NC_MODE_HPP_
#define NC_MODE_HPP_

#include <string>
#include <map>
#include <sstream>
#include <set>

#include "NCContext.hpp"

enum ShellPageOperation {
   NEXT,
   SAME,
   PREVIOUS,
   QUIT
};



class NCShellMode {
public:
  NCShellMode() {
  }
  ~NCShellMode() {
  }
  virtual void getPromptString(std::string&) = 0;
  virtual ShellPageOperation Evaluate(const std::vector<std::string>&, std::string&) = 0;
};


class NCShellModeSystem : public NCShellMode {
public:
  typedef ShellPageOperation (NCShellModeSystem::*fptrCmdHandler)(std::string&);
  typedef std::map<std::string, fptrCmdHandler> Cmd2FnMap;
public:
  NCShellModeSystem();
  ~NCShellModeSystem();
  virtual void getPromptString(std::string&);
  virtual ShellPageOperation Evaluate(const std::vector<std::string>&, std::string&);
private:
  ShellPageOperation Help(std::string&);
  ShellPageOperation SetSystemProperty(std::string&);
  ShellPageOperation Quit(std::string&);
  ShellPageOperation Connect(std::string&);
private:
  std::string              m_promptString;
  NCContext*               m_pContext;
  Cmd2FnMap                m_Cmd2FnMap;  
  std::vector<std::string> m_vecInputs;

  std::map<std::string, std::string> m_systemProperties;
};


class NCShellModeServer : public NCShellMode {
public:
  typedef ShellPageOperation (NCShellModeServer::*fptrCmdHandler)(std::string&);
  typedef std::map<std::string, fptrCmdHandler> Cmd2FnMap;
public:
  NCShellModeServer();
  ~NCShellModeServer();
  virtual void getPromptString(std::string&);
  virtual ShellPageOperation Evaluate(const std::vector<std::string>&, std::string&);
private:
  ShellPageOperation Help(std::string&);
  ShellPageOperation Disconnect(std::string&);
  ShellPageOperation CreateRegion(std::string& );
  ShellPageOperation GetRegions(std::string& );
  ShellPageOperation ListRegions(std::string& );

private:
  std::string              m_promptString;
  NCContext*               m_pContext;
  Cmd2FnMap                m_Cmd2FnMap;  
  std::vector<std::string> m_vecInputs;
};

class NCShellModeRegion : public NCShellMode {

public:
  typedef ShellPageOperation (NCShellModeRegion::*fptrCmdHandler)(std::string&);
  typedef std::map<std::string, fptrCmdHandler> Cmd2FnMap;
public:
  NCShellModeRegion();
  ~NCShellModeRegion();
  virtual void getPromptString(std::string&);
  virtual ShellPageOperation Evaluate(const std::vector<std::string>&, std::string&);
private:
  ShellPageOperation Help(std::string&);
  ShellPageOperation LeaveRegion(std::string&);  
  ShellPageOperation Put(std::string& );
  ShellPageOperation Get(std::string&);

private:
  std::string              m_promptString;
  NCContext*               m_pContext;
  Cmd2FnMap                m_Cmd2FnMap;  
  std::vector<std::string> m_vecInputs;

};

#endif
