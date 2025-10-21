#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client {
public:
    Client(const std::string& serverAddress);
    std::string predict(const std::vector<float>& inputs);

private:
    std::string serverAddress;
};

#endif // CLIENT_H
