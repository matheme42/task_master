#ifndef CRYPTAGE_HPP
#define CRYPTAGE_HPP

#include <string>

class Cryptage
{
    public:
        std::string crypter(std::string texte, std::string cle);
        std::string decrypter(std::string texte, std::string cle);
};

#endif