/** @file quicktest.cpp 
 * 
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Apr. 24, 2019 by daver:
 * @endhistory
 */

#include <iostream>
#include <cjson/cJSON.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>

using namespace std;

int main(int argc, char **argv)
{
    char buf[5000];

    int fd = ::open("../zios/config.json", O_RDONLY);
    assert(fd >= 0);
    int len = ::read(fd, buf, sizeof(buf));
    assert(len > 0);
    buf[len] = '\0';

    cout << "************* JSON **************" << endl
            << buf << endl
            << "************* JSON **************" << endl;


    cJSON* json = cJSON_Parse(buf);
    assert(json != NULL);

    cout << "Hello world" << endl;
    return 0;
}
