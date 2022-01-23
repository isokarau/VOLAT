#include "Client.h"

#include <nlohmann/json.hpp>


using json = nlohmann::json;

Client::Client() {
    WSAStartup(MAKEWORD(2, 1), &WSAData);
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
    }

    addr.sin_addr.s_addr = inet_addr(inetAddr); //������� � �������
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); //����

    if (connect(server, (SOCKADDR *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Server connection failed with error: " << WSAGetLastError() << endl;
    }

}

response Client::login(string name, string password, string game, int num_turns, int num_players) {
    string msg = "{\"name\":\"" + name +
                 "\",\"password\":\"" + password +
                 "\",\"game\":\"" + game +
                 "\",\"num_turns\":" + to_string(num_turns) +
                 ",\"num_players\":" + to_string(num_players) + "}";
    request(1, msg);

    return answer();
}

//response Client::login(string name, string password, string game) {
//    string msg = "{\"name\":\"" + name +
//                 "\",\"password\":\"" + password +
//                 "\",\"game\":\"" + game + "\"}";
//
//    request(1, msg);
//
//    return answer();
//}

response Client::logout() {
    request(2, "");
    return answer();
}


response Client::map() {
    request(3, "");
    return answer();
}

response Client::game_state() {
    request(4, "");
    return answer();
}

response Client::game_action() {
    request(5, "");
    return answer();
}

response Client::turn() {
    request(6, "");
    return answer();
}

response Client::chat(string msg) {
    request(100, "{\"message\":\"" + msg + "\"}");
    return answer();
}


response Client::move(int vehicle_id, int x, int y, int z) {
    string msg = "{\"vehicle_id\":" + to_string(vehicle_id) +
                 ",\"target\":{" +
                 "\"x\":" + to_string(x) +
                 ",\"y\":" + to_string(y) +
                 ",\"z\":" + to_string(z) + "}}";

    request(101, msg);
    return answer();
}

response Client::shoot(int vehicle_id, int x, int y, int z) {
    string msg = "{\"vehicle_id\":" + to_string(vehicle_id) +
                 ",\"target\":{" +
                 "\"x\":" + to_string(x) +
                 ",\"y\":" + to_string(y) +
                 ",\"z\":" + to_string(z) + "}}";

    request(102, msg);
    return answer();
}

void Client::request(int action, string msg) {
    char *buffer = new char[8 + msg.size()];

    for (int i = 0; i < 4; ++i) {
        buffer[i] = ((char *) &action)[i];
    }

    int size_msg = msg.size();
    for (int i = 0; i < 4; ++i) {
        buffer[i + 4] = ((char *) &size_msg)[i];
    }

    for (int i = 0; i < size_msg; i++) {
        buffer[i + 8] = msg[i];
    }

    if (debugging) cout << "Client: " << action << " " << msg << '\n';

    send(server, buffer, 8 + size_msg, 0);
}

response Client::answer() {
    char buffer[4] = {0};

    recv(server, buffer, 4, MSG_WAITALL);
    int result =
            ((buffer[3] & 0xff) << 24) | ((buffer[2] & 0xff) << 16) | ((buffer[1] & 0xff) << 8) | (buffer[0] & 0xff);

    recv(server, buffer, 4, MSG_WAITALL);
    int size_msg =
            ((buffer[3] & 0xff) << 24) | ((buffer[2] & 0xff) << 16) | ((buffer[1] & 0xff) << 8) | (buffer[0] & 0xff);
    if (size_msg == 0)
        return {result, json::parse("{}")};
    char *c_msg = new char[size_msg];
    if (size_msg != 0) recv(server, c_msg, size_msg, MSG_WAITALL);

    string msg;

    for (int i = 0; i < size_msg; i++) {
        msg += c_msg[i];
    }

    if (debugging) cout << "Server: " << result << " " << msg << "\n\n";

    return {result, json::parse(msg)};
}