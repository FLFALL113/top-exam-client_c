#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#define PORT 60
#define IP "127.0.0.1"

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;


int main() {
    setlocale(LC_ALL, "RU");
    try {
#pragma region Connection
        
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve(IP, to_string(PORT)));
        json request;
        cout << "Введите свое имя: ";
        string name;
        getline(cin, name);
        request["name"] = name;
        
        string request_str = request.dump();

        boost::asio::write(socket, boost::asio::buffer(request_str + '\n'));

        char buffer[1024];
        size_t length = socket.read_some(boost::asio::buffer(buffer));
        string response(buffer, length);

        cout << "Ответ с сервера: " << response << endl;
#pragma endregion

    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}