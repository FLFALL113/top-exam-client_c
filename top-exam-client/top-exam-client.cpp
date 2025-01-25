#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using boost::asio::ip::tcp;

using namespace std;

void write_to_server(tcp::socket& socket, const string& name) {
    try {
        string message;
        getline(cin, message);

        json j;
        j["name"] = name;
        j["message"] = message;

        string str = j.dump();

        str += '\n';

        boost::asio::write(socket, boost::asio::buffer(str));

        cout << "Отправлено ответ: " << message << endl;
    }
    catch (const exception& e) {
        cerr << "Write error: " << e.what() << "\n";
    }
}

bool starts_with(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}
void read_from_server(tcp::socket& socket) {
    try {
        char reply[1024];
        string name;

        cout << "Введите имя: ";
        getline(cin, name);

        while (true) {
            bool flag = false;
            size_t reply_length = socket.read_some(boost::asio::buffer(reply));
            string msg = string(reply, reply_length);

            cout << "Сообщение с сервера: " << msg << "\n";

            if(starts_with(msg,"_quest_"))
            {
                write_to_server(socket,name);
            }
        }
    }
    catch (exception& e) {
        cerr << "Read error: " << e.what() << "\n";
    }
}



int main() {
    setlocale(LC_ALL, "ru");
    try {
        boost::asio::io_context io_context;

        // Разрешение адреса сервера
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "60");

        // Подключение к серверу
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        read_from_server(socket);

        // Ждём завершения потока чтения
        //reader_thread.join();
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}