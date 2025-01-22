#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;
using namespace std;
int main() {
    try {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "60"));

        cout << "Enter message for the server: ";
        string message;
        getline(cin, message);

        boost::asio::write(socket, boost::asio::buffer(message));

        char buffer[1024];
        size_t length = socket.read_some(boost::asio::buffer(buffer));
        string response(buffer, length);

        cout << "Response from server: " << response << endl;
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}