#include "FormData.hpp"

FormData::FormData(): _firstName(""), _lastName(""), _favoriteColor(""), _emailAddress(""), _image("")
{
}

FormData::~FormData()
{
}
std::string	FormData::getFirstName() const {
	return (this->_firstName);
}
std::string	FormData::getLastName() const {
	return (this->_lastName);
}
std::string	FormData::getFavoriteColor() const {
	return (this->_favoriteColor);
}
std::string	FormData::getEmailAdress() const {
	return (this->_emailAddress);
}

void	FormData::setFirstName(std::string firstName) {
	this->_firstName = firstName;
}

void	FormData::setLastName(std::string lastName) {
	this->_lastName = lastName;
}
void	FormData::setFavoriteColor(std::string favoriteColor) {
	this->_favoriteColor = favoriteColor;
}
void	FormData::setEmailAdress(std::string emailAdress) {
	this->_emailAddress = emailAdress;
}

std::ostream	&operator<<(std::ostream &Cout, FormData const objet) {
	Cout << "First Name		= " << objet.getFirstName() << std::endl;
	Cout << "Last Name		= " << objet.getLastName() << std::endl;
	Cout << "Favorite Color = " << objet.getFavoriteColor() << std::endl;
	Cout << "Email Address	= " << objet.getEmailAdress() << std::endl;
	return (Cout);
}