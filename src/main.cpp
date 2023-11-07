#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

using namespace std;

class LibInterface{
  void *_pLibHnd = nullptr;
  AbstractInterp4Command *(*_pCreateCmd)(void) = nullptr;

  public:
  ~LibInterface(){ if(_pLibHnd) dlclose(_pLibHnd);}

  bool Init(const char *sFilename);
  AbstractInterp4Command *CreateCmd(){assert(_pCreateCmd); return _pCreateCmd();}
};

bool LibInterface::Init(const char *sFileName){
  _pLibHnd = dlopen(sFileName, RTLD_LAZY);

  if(!_pLibHnd){
    cout << "!!! Brak biblioteki: " << sFileName << endl;
    return false;
  }

  void *pFun = dlsym(_pLibHnd, "CreateCmd");

  if(!pFun){

    cout << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return false;
  }

  _pCreateCmd = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  return true;
}


int main(){
  
  LibInterface MoveLibInterf;

  if(!MoveLibInterf.Init("libs/libInterp4Move.so")) return 1;

  AbstractInterp4Command *pCmd = MoveLibInterf.CreateCmd();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  LibInterface SetLibInterf;

  if(!SetLibInterf.Init("libs/libInterp4Set.so")) return 1;

  pCmd = SetLibInterf.CreateCmd();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  LibInterface PauseLibInterf;

  if(!PauseLibInterf.Init("libs/libInterp4Pause.so")) return 1;

  pCmd = PauseLibInterf.CreateCmd();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  LibInterface RotateLibInterf;

  if(!RotateLibInterf.Init("libs/libInterp4Rotate.so")) return 1;

  pCmd = RotateLibInterf.CreateCmd();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;
}
