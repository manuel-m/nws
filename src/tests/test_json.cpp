#include <iostream>
#include <fstream>
#include <cassert>
#include "json/json.h"

#define TEST_INPUT_FILE "src/tests/data/server.json"

using namespace std;

int main(int argc, char **argv) {

    
    /* sanity */
    {
        ifstream ifs(TEST_INPUT_FILE);
        if (ifs.fail()) {
            cout << "fail to open " << TEST_INPUT_FILE << endl;
            return 1;
        } else {
            ifs.close();
        }
    }

    Json::Value root;
    Json::Reader reader;
    std::ifstream test(TEST_INPUT_FILE, ifstream::binary);

    bool parsingSuccessful = reader.parse(test, root, false);

    assert( parsingSuccessful && "json parse data file");

    assert(root["host"].asString() == string("localhost"));
    assert(root["port"].asInt() == 9990 );


    return 0;
}