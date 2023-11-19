#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <optional>
#include <vector>
#include "AbstractInterp4Command.hh"
#include "Commands.hh"
#include "Config.hh"
#include "xmlinterp.hh"
#include <memory>

#define LINE_SIZE 500

using namespace std;
using namespace xercesc;

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

bool ExecPreprocesor( const char* NazwaPliku, istringstream &IStrm4Cmds )
{
  string Cmd4Preproc = "cpp -P ";
  char Line[LINE_SIZE];
  ostringstream OTmpStrm;
  Cmd4Preproc += NazwaPliku;
  FILE* pProc = popen(Cmd4Preproc.c_str(),"r");
  if (!pProc) return false;

  while (fgets(Line,LINE_SIZE,pProc)) {
    OTmpStrm << Line;
  }
  IStrm4Cmds.str(OTmpStrm.str());
  return pclose(pProc) == 0;
}

struct ProgramInput {
  char * commands = nullptr;
  char * configuration = nullptr;

  optional<Commands> readCommands() const;
  optional<Config> readConfig() const;
};

optional<ProgramInput> validate_input(int argc, char *argv[]) {
  if(argc == 3) {
    return optional<ProgramInput>{ProgramInput{commands: argv[1], configuration: argv[2]}};
  } else {
    return nullopt;
  }
}



optional<Commands> ProgramInput::readCommands() const {
  istringstream iss{};
  if(ExecPreprocesor(this->commands, iss)) {
    char Line[LINE_SIZE];
    vector<Command> commands{};

    while(!iss.eof()){
      iss.getline(Line, LINE_SIZE);
      cout << "Reading line: " << Line << endl;
      auto ret = Command::parse(Line);
      if( ret.has_value() ) {
        commands.push_back(ret.value());
      } else {
        cout << "Nastepujaca linia posiada nieprawidlowe polecenie: " << Line << endl;
      }
    }
    
    return optional<Commands>{Commands{commands: commands}};
  }


  return nullopt;
}

optional<Config> ProgramInput::readConfig() const {
  try {
    XMLPlatformUtils::Initialize();
  }
  catch (const XMLException& toCatch) {
    char* message = XMLString::transcode(toCatch.getMessage());
    cerr << "Error during initialization! :\n";
    cerr << "Exception message is: \n" << message << "\n";
    XMLString::release(&message);
    return nullopt;
  }

  Config config{};
  std::unique_ptr<DefaultHandler> pHandler(new XMLInterp4Config(config));
  std::unique_ptr<SAX2XMLReader> pParser(XMLReaderFactory::createXMLReader());
  pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
  pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
  pParser->setFeature(XMLUni::fgXercesDynamic, false);
  pParser->setFeature(XMLUni::fgXercesSchema, true);
  pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
  pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);
  pParser->setContentHandler(pHandler.get());
  pParser->setErrorHandler(pHandler.get());

  try {
    
    if (!pParser->loadGrammar("config/config.xsd", xercesc::Grammar::SchemaGrammarType,true)) {
      cerr << "!!! Plik grammar/actions.xsd, '" << endl << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany." << endl;
      return nullopt;
    }
    pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
    pParser->parse(this->configuration);
  }
  catch (const XMLException& Exception) {
    char* sMessage = XMLString::transcode(Exception.getMessage());
    cerr << "Informacja o wyjatku: \n\t" << sMessage << "\n";
    XMLString::release(&sMessage);
    return nullopt;
  }
  catch (const SAXParseException& Exception) {
    char* sMessage = XMLString::transcode(Exception.getMessage());
    char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

    cerr << "Blad! " << endl
          << "    Plik:  " << sSystemId << endl
          << "   Linia: " << Exception.getLineNumber() << endl
          << " Kolumna: " << Exception.getColumnNumber() << endl
          << " Informacja: " << sMessage 
          << endl;

    XMLString::release(&sMessage);
    XMLString::release(&sSystemId);
    return nullopt;
  }
  catch (const std::exception &e) {
    cout << "Zgloszony zostal nieoczekiwany wyjatek: " << e.what() << endl ;
    return nullopt;
  }

  return optional<Config>{config};
}


int start(ProgramInput input) {
  auto retCommands = input.readCommands();
  if(!retCommands.has_value()) {
    cout << "Nie udalo sie otworzyc pliku: " << input.commands << endl;
    return 1;
  }
  Commands commands = retCommands.value();

  auto retConfig = input.readConfig();
  if(!retConfig.has_value()) {
    cout << "Nie udalo sie otworzyc lub sparsowac pliku: " << input.configuration << endl;
    return 1;
  }
  Config config = retConfig.value();



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

int main(int argc, char *argv[]) {
  auto ret = validate_input(argc, argv);
  if( ret.has_value() ) {
    return start(ret.value());
  } else {
    cout << "Do startu programu nalezy podac 2 argumenty bedace sciezkami do plikw z komendami oraz konfiguracja!" << endl;
    return -1;
  }
}

