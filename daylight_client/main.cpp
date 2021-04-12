//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include<fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include<curl/curl.h>
using boost::asio::ip::tcp;
//
struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
//mover depues a un cliente.cpp

int main(int argc, char* argv[])
{
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    if (argc != 2)
    {
        std::cerr << "Usage: cliente <host/path/filename>" << std::endl;//http://www.animemx.net/lista-de-animes/
        return 1;//un link caido de pureba que se que trae poca info
    }
    //despues sortear el argc
    std::string data = "";
    for (int i = 0; argv[1][i] != NULL; ++i) {
        data += argv[1][i];
    }
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_PORT, 80);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, data.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        //hasta aca anda bien

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        std::cout << res << std::endl;
        /* Check for errors */
        std::ofstream prueba;//abro/creo si no esta/ el archivo para poner lo que reciba del server
        prueba.open("prueba.txt", std::ios::trunc);//borro lo que habia antes
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        else {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);     
            prueba.write(chunk.memory, chunk.size);//guardo en el archivo
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
    }


    
    return 0;
}
/*HTTP/1.1 200 OK
Date: Date (Ej: Tue, 04 Sep 2018 18:21:19 GMT)
Location: 127.0.0.1/prueba.txt
Cache-Control: max-age=30
Expires: Date + 30s (Ej: Tue, 04 Sep 2018 18:21:49 GMT)
Content-Length: 5
Content-Type: text/html; charset=iso-8859-1
prueba.txt
*///respuesta de prueba