#ifndef FORMDATA_HPP
#define FORMDATA_HPP

#include "include.hpp"


class FormData
{
	private:
		std::string	_firstName;
		std::string	_lastName;
		std::string	_favoriteColor;
		std::string _emailAddress;
		std::string	_image;
	public:
		FormData();
		~FormData();
		std::string	getFirstName() const;
		std::string	getLastName() const;
		std::string	getFavoriteColor() const;
		std::string	getEmailAdress() const;
		void	setFirstName(std::string firstName);
		void	setLastName(std::string lastName);
		void	setFavoriteColor(std::string favoriteColor);
		void	setEmailAdress(std::string emailAdress);
};

std::ostream	&operator<<(std::ostream &Cout, FormData const objet);


#endif