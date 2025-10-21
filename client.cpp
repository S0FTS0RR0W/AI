// AI client implementation file. 
// This client connects to a Python FastAPI server and sends a prediction request.

// This implementation uses two popular single-header C++ libraries:
// 1. httplib.h: for making HTTP requests. Download from: https://github.com/yhirose/cpp-httplib
// 2. nlohmann/json: for creating and parsing JSON. Download from: https://github.com/nlohmann/json
// You will need to download these header files and place them in your project's include path.

#include "client.h"
#include "httplib.h"
#include "json.hpp"
#include <iostream>

using namespace std;

// Use the nlohmann/json library for convenience
using json = nlohmann::json;

// The server address and port
const std::string SERVER_ADDRESS = "10.20.0.15:8000";
const int SERVER_PORT = 8000;

// Constructor to initialize client with server address
Client::Client(const std::string& serverAddress)
    : serverAddress(serverAddress) {}

// Send a prediction request to the server
std::string Client::predict(const std::vector<float>& inputs) {
    httplib::Client cli(serverAddress, SERVER_PORT);

    // Create the JSON request body
    json request_body;
    request_body["inputs"] = inputs;

    // Send the POST request to the /predict endpoint
    if (auto res = cli.Post("/predict", request_body.dump(), "application/json")) {
        if (res->status == 200) {
            // Parse the JSON response
            json response_body = json::parse(res->body);
            return response_body.dump(4); // Pretty print the JSON response
        } else {
            return "Error: " + std::to_string(res->status) + "\n" + res->body;
        }
    } else {
        auto err = res.error();
        return "HTTP request failed: " + httplib::to_string(err);
    }
}

int main() {
    // Create a client instance
    Client client(SERVER_ADDRESS);

    // Example input data for the prediction
    std::vector<float> input_data = {1.0f, 2.0f, 3.0f, 4.0f};

    // Send the prediction request and print the response
    std::cout << "Sending prediction request with input: [";
    for (size_t i = 0; i < input_data.size(); ++i) {
        std::cout << input_data[i] << (i == input_data.size() - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;

    std::string response = client.predict(input_data);

    std::cout << "\nServer response:" << std::endl;
    std::cout << response << std::endl;

    return 0;
}
