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
#include <iostream>
#include <vector>
#include <sstream>
#include "NCContext.hpp"
#include "NCMode.hpp"

std::vector<NCShellMode*> vecShellPages;
NCContext* NCContext::m_pGlobalContext = 0;  

bool Read(const std::string& s, std::vector<std::string>& vecInputs) 
{
  std::istringstream iss(s);
  for(;;) 
  {
    std::string token;
    if(!(iss>>token)) 
      break;
    vecInputs.push_back(token);
  }
  if ( !iss.eof())
    return false;
  return true;
}

void ReadEvalPrintLoop() 
{
  std::vector<NCShellMode*>::iterator currentPage = vecShellPages.begin();
  for (;;) 
  {
    std::string prompt;
    (*currentPage)->getPromptString(prompt);
    std::cout << prompt;

    std::string line; 
    std::getline(std::cin, line);
    std::vector<std::string> vecInputs;

    if ( Read(line, vecInputs)) 
    {
      std::string result;
      ShellPageOperation op = (*currentPage)->Evaluate(vecInputs, result);      
      std::cout << result << std::endl;
      if ( op == QUIT) 
      {
        break;
      }
      switch(op) 
      {
      case NEXT:
        if ( currentPage + 1 != vecShellPages.end())
        {
          currentPage++;
        }
        break;
      case SAME:
        break;
      case PREVIOUS:
        if ( currentPage != vecShellPages.begin()) 
        {
          currentPage--;
        }
        break;
      default:
        break;
      }
      result.clear();
    } 
    else 
    {
      std::cout << "Error in parsing input commands "<< std::endl;
    }
  }    
}

int main ( int argc, char** argv) {
  vecShellPages.push_back(new NCShellModeSystem());
  vecShellPages.push_back(new NCShellModeServer());
  vecShellPages.push_back(new NCShellModeRegion());
  ReadEvalPrintLoop();
  return 0;
}


