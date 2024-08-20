#include "include.hpp"

int main(int argc, char *argv[], char *envp[])
{
	// std::list<FormData> my_list;
	// std::list<FormData>::iterator it;
	try
	{
		Server server_object(argv[1], envp);
		// my_list = server_object.getList(); // TEST
		// for (int i = 0; i < my_list.size(); ++i)
		// 	std::cout << "First Name		= " << it->getFirstName() << std::endl;// TEST
		// 	std::cout << "Last Name		= " << it->getLastName() << std::endl;// TEST
		// 	std::cout << "Favorite Color = " << it->getFavoriteColor() << std::endl;// TEST
		// 	std::cout << "Email Address	= " << it->getEmailAdress() << std::endl;// TEST
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
