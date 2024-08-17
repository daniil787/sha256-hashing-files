#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

std::vector<unsigned char> calculate_sha256(const std::vector<unsigned char>& data) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();               
    const EVP_MD* md = EVP_sha256();                    

    if (mdctx == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (1 != EVP_DigestInit_ex(mdctx, md, nullptr)) {   // context initialization
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize EVP_DigestInit_ex");
    }

    if (1 != EVP_DigestUpdate(mdctx, data.data(), data.size())) { // update hash context
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to update EVP_DigestUpdate");
    }

    std::vector<unsigned char> hash(EVP_MD_size(md));
    unsigned int len = 0;

    if (1 != EVP_DigestFinal_ex(mdctx, hash.data(), &len)) {         // getting hash
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize EVP_DigestFinal_ex");
    }

    EVP_MD_CTX_free(mdctx);     // context cleaning
        hash.resize(len);   // fixing size of hash
    return hash;
}

std::vector<unsigned char> readData(const std::string &filePath) {

    if (filePath.empty()) {
    
        throw std::runtime_error("File path is empty");

    }

    std::ifstream file(filePath, std::ios::binary);

    if(!file.is_open()) {

             throw   std::runtime_error("Error of reading the file");

    }

  std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return data;

}



int main() {

   
    std::string filePath;  // The names should be in Latin script

    std::cout << "\nWARNING: The names should be in Latin script!" << std::endl;

    while (true) {

    std::cout << "\nEnter path of file with extention: ";
    
    std::getline(std::cin, filePath);

        try {

            std::vector<unsigned char> data = readData(filePath);
            
            std::vector<unsigned char> hash256 = calculate_sha256(data);

            std::cout << "\nHash SHA256 is: ";

            for (unsigned char byte : hash256) {

                std::cout << std::hex << std::uppercase << std::setw(2)<<std::setfill('0') << (int)byte;

            }
            std::cout << std::endl;

        }

        catch (const std::exception& e) {

            std::cerr << "Error during work: " <<e.what()<< std::endl;
            return 3;
        }
        
    }
    return 0;
}