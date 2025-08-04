#include "../includes/Server.hpp"

int main(int ac, char **av){
  if (ac != 3){
      std::cerr << "Invalid number of arguments!" << std::endl 
  << "Please insert: <executable name> <listening port> <password>" << std::endl;
      std::exit(1);
  }

  std::string port = av[1];
  std::string password = av[2];
  Server	srv(port, password);
	try
	{
    srv.start();
		srv.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
