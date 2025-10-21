// test client that connects with ollama server and can be used for talking to ollama models and managing them

// import necessary libraries for ui and http requests
#include "httplib.h"
#include <iostream>
#include <string>
#include <vector>

// define namespace for convenience

using namespace std;

// define server address and port
const string SERVER_ADDRESS = "10.20.0.15";
const int SERVER_PORT = 11434;

// ui element for message input and display

void displayMessage(const string& message) {
    cout << message << endl;
}

void getUserInput(string& input) {
    cout << "Enter your message: ";
    getLine(cin, input);
    cout << endl;
    return input;
}

// main function

int main()
{
    // create http client
    httplib::Client cli(SERVER_ADDRESS, SERVER_PORT);

    // get user input
    string userInput;
    getUserInput(userInput);

    // create json request body
    nlohmann::json request_body;
    request_body["message"] = userInput;

    // send post request to /chat endpoint
    if (auto res = cli.Post("/chat", request_body.dump(), "application/json")) {
        if (res->status == 200) {
            // parse json response
            nlohmann::json response_body = nlohmann::json::parse(res->body);
            displayMessage("Response from server: " + response_body.dump(4)); // pretty print json response
        } else {
            displayMessage("Error: " + to_string(res->status) + "\n" + res->body);
        }
    } else {
        auto err = res.error();
        displayMessage("HTTP request failed: " + httplib::to_string(err));
    }

    return 0;
}