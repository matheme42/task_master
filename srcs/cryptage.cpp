#include "cryptage.hpp"

std::string Cryptage::crypter(std::string texte, std::string cle)
{
    if (cle.size() == 0) return texte;
    std::string encryptedMessage;

    encryptedMessage = texte;
    
    for (int i = 0; i < encryptedMessage.size(); i++) {
        encryptedMessage[i] = encryptedMessage[i] + (cle[i % cle.size()]);
    }

    return encryptedMessage;
}

std::string Cryptage::decrypter(std::string texte, std::string cle)
{
    if (cle.size() == 0) return texte;
    std::string decryptedMessage;

    decryptedMessage = texte;

    for (int i = 0; i < decryptedMessage.size(); i++) {
        decryptedMessage[i] = decryptedMessage[i] - (cle[i % cle.size()]);
    }

    return decryptedMessage;
}