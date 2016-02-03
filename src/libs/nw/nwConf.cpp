#include <fstream>
#include <iostream>

#include "nw/nw.h"
#include "json/json.h"

void NwConf::init() {

    this->m_conf.max_body_size = 64000;
    this->m_conf.max_connections = 8;
    this->m_conf.port = NW_LISTEN_DEFAULT_PORT;
}

int NwConf::load(const char *path_) {

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful;

    /* sanity */
    {
        std::ifstream ifs(path_);
        if (ifs.fail()) {
            log_return_1("fail to open %s", path_);
        }
        ifs.close();
    }

    /* parse json */
    {
        std::ifstream test(path_, std::ifstream::binary);
        parsingSuccessful = reader.parse(test, root, false);
        if (parsingSuccessful == 0) log_return_1("json parse data file failed %s", path_);
    }

    /* set data */
    this->m_conf.port = root["port"].asInt();
//    assert(root["host"].asString() == std::string("localhost"));

    return 0;


}

