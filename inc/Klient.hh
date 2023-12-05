#ifndef KLIENT_HH
#define KLIENT_HH

#include <boost/lockfree/spsc_queue.hpp>
#include <thread>

extern const char* CLEAR_COMMAND;
extern const char* CLOSE_COMMAD;

int Send(int Sk2Server, const char *sMesg);
using boost_queue = boost::lockfree::spsc_queue<std::string, boost::lockfree::capacity<1024>>;

class Sender {
  int socket = 0;
  boost_queue queue{};
  std::atomic_bool is_on{true};
  std::optional<std::thread> watching_thread{std::nullopt};

  
 public:
  Sender(int Socket);
  Sender(const Sender&) = delete;
  ~Sender() {
    if(this->watching_thread.has_value()) {
      this->watching_thread.value().join();
    }
  }

  void run();

  void send(std::string command);

  private:

  static void watch(boost_queue &queue, std::atomic_bool &is_on, int socket)
  {
    std::string value;
    while (is_on) {
        while (queue.pop(value)) {
          std::cout << "Sending message from other thread: " << value;
          Send(socket, value.c_str());
        }
        if(!value.compare(CLOSE_COMMAD)) {
          break;
        }
    }
  }
};


bool OpenConnection(int &rSocket);

#endif