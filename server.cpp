#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <nlohmann/json.hpp>
#include "httplib.h"

using json = nlohmann::json;
using namespace std;
using namespace httplib;

map<int, double> accounts;
map<int, string> names;

void loadData()
{
    ifstream in("accounts.json");
    if (!in.good())
        return;

    json data;
    in >> data;

    for (auto &item : data.items())
    {
        int acc = stoi(item.key());
        names[acc] = item.value()["name"];
        accounts[acc] = item.value()["balance"];
    }
}

void saveData()
{
    json data;
    for (auto &x : accounts)
    {
        data[to_string(x.first)] = {
            {"name", names[x.first]},
            {"balance", x.second}};
    }
    ofstream out("accounts.json");
    out << data.dump(4);
}

int main()
{
    loadData();

    Server server;

    // Create Account
    server.Post("/create", [&](const Request &req, Response &res)
                {
        json body = json::parse(req.body);

        int acc = body["acc"];
        string name = body["name"];
        double balance = body["balance"];

        if (accounts.count(acc)) {
            res.set_content("{\"message\":\"Account already exists\"}", "application/json");
            return;
        }

        accounts[acc] = balance;
        names[acc] = name;
        saveData();

        res.set_content("{\"message\":\"Account Created\"}", "application/json"); });

    // Deposit
    server.Post("/deposit", [&](const Request &req, Response &res)
                {
        json body = json::parse(req.body);

        int acc = body["acc"];
        double amount = body["amount"];

        accounts[acc] += amount;
        saveData();

        res.set_content("{\"message\":\"Amount Deposited\"}", "application/json"); });

    // Withdraw
    server.Post("/withdraw", [&](const Request &req, Response &res)
                {
        json body = json::parse(req.body);

        int acc = body["acc"];
        double amount = body["amount"];

        if (accounts[acc] < amount) {
            res.set_content("{\"message\":\"Insufficient Balance\"}", "application/json");
            return;
        }

        accounts[acc] -= amount;
        saveData();

        res.set_content("{\"message\":\"Amount Withdrawn\"}", "application/json"); });

    // Balance check
    server.Post("/balance", [&](const Request &req, Response &res)
                {
        json body = json::parse(req.body);
        int acc = body["acc"];

        json response = {{"balance", accounts[acc]}};
        res.set_content(response.dump(), "application/json"); });

    // Serve static website
    server.set_mount_point("/", "./public");

    cout << "Server running at http://localhost:5000\n";
    server.listen("0.0.0.0", 5000);
}
