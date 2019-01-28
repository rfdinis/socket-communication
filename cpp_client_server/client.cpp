#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

int main(){

  try{
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "4321");
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    for(;;){
      char request[max_length];
      std::cin.getline(request, max_length);
      size_t request_length = std::strlen(request);
      boost::asio::write(s, boost::asio::buffer(request, request_length));
    }
  }
  catch (std::exception& e){
    std::cerr << "Oopsie: " << e.what() << "\n";
  }

  return 0;
}
