#ifndef SCENE_HH
#define SCENE_HH

#include <iostream>
#include <iomanip>
#include <mutex>
#include <vector>
#include <optional>
#include "Config.hh"
#include "AbstractScene.hh"
#include "Klient.hh"

using namespace std;


/*!
 * \brief Ta klasa trochę udaję klasę MobileObj, która jest w zadaniu.
 *
 */
class GeomObject {
  /*!
   * \brief Zestaw sekwencji poleceń symulujących ruch obiektu.
   *
   * Zestaw sekwencji poleceń symulujących ruch obiektu.
   * Każde polecenie odpowiada kolejnym stanom obiektu.
   */
  std::string _Cmd4StatDesc;
  
 public:
 GeomObject(const Object &object) {
    stringstream object_command;
    object_command << object;
    getline(object_command, this->_Cmd4StatDesc);
 }

  /*!
   * \brief Ustawia zestaw poleceń odpowiadających kolejnym stanom
   *        obiektu.
   */
  void SetCmds(std::string command) { _Cmd4StatDesc = command; }
  /*!
   * \brief Udostępnia kolejny zestaw poleceń umożliwiających
   *        zespołu obiektu.
   *
   * Udostępnia kolejny zestaw poleceń umożliwiających
   * zespołu obiektu. Ta metoda "udaje" metodę, która w oryginalnym
   * rozwiązaniu powinna wygenerować odpowiednie polecenie na podstawie
   * przechowywanej informacji o położeniu i orientacji obiektu.
   */
  auto GetStateDesc() const
  {
    return std::string_view{ this->_Cmd4StatDesc };
  }
};





/*!
 * \brief Namiastka sceny z prostą kolekcją trzech wizualizowanych obiektów.
 */
struct Scene: public AbstractScene {
/*!
   * \brief Prosta kolekcja obiektów sceny
   */
  std::optional<Sender> sender = nullopt;
  std::vector<Object> objects{};


  Scene(std::vector<Object> loadedObjects): objects(loadedObjects) {}

  virtual ~Scene() {
    this->share_command({CLOSE_COMMAD}); 
  }

  void connect(int socket) {
    this->sender.emplace(socket);
    this->sender.value().run();
  }

  void init() {
    this->share_command({CLEAR_COMMAND});
    for(const auto &object: this->objects) {
      std::string command = "AddObj "s + object.toCommand().c_str() + "\n";
      this->share_command(command);
    }
  }

  void share_command(std::string command) {
    if(this->sender.has_value()) {
      std::cout << "Putting msg to queue: " << command;
      this->sender.value().send(command);
    }
  }

  virtual void AddMobileObj(AbstractMobileObj *pMobObj) {
    this->objects.emplace_back(pMobObj);
  }

  virtual std::optional<AbstractMobileObj*> FindMobileObj(const char *sName) {
    for(auto &obj : this->objects) {
      if(!obj.name.compare(sName)) {
        return {&obj};
      }
    }

    return nullopt;
  };

  virtual void update(AbstractMobileObj *pMobObj) {
    for(const auto &object: this->objects) {
      if(!object.name.compare(pMobObj->GetName())) {
        std::string command = "UpdateObj "s + object.toCommand().c_str() + "\n";
        this->share_command(command);
        return;
      }
    }
  }
};


#endif