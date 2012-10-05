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
#include "NativeClientCommandHandler.hpp"

NativeCommandCommandHandler::NativeCommandCommandHandler() {

}

NativeCommandCommandHandler::~NativeCommandCommandHandler() {

}

bool NativeCommandCommandHandler::isValidCommand(std::string command) {
  bool rc = false;
  if ( m_commandSet.find(command) != m_commandSet.end())
    return true;
  return rc;
}

void NativeCommandCommandHandler::initialize() {
  m_commandSet.insert("connect");
  m_commandSet.insert("addserver");
  m_commandSet.insert("addlocator");
  m_commandSet.insert("createregion");
  m_commandSet.insert("put");
  m_commandSet.insert("get");
  m_commandSet.insert("destroyregion");
  m_commandSet.insert("disconnect");
  m_commandSet.insert("setsystemproperty");
  m_commandSet.insert("initclient");
  m_commandSet.insert("help");
}