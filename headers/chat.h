#pragma once

#include "sha1.h"
#include <string.h>

#define LOGINLENGTH 10  // Зафикcируем длину логина

typedef char Login[LOGINLENGTH]; // тип логин

class Chat {
public:
    Chat();
    ~Chat();
    void reg(Login _login, char _pass[], int pass_length);
    bool login(Login _login, char _pass[], int pass_length);
private:

    enum enDataStatus {
        free,
        engaged,
        deleted
    };

    struct AuthData {
        AuthData() :
                login(""),
                pass_sha1_hash(nullptr),
                status(enDataStatus::free) {
        }

        ~AuthData() {
            if (pass_sha1_hash != nullptr)
                delete[] pass_sha1_hash;
        }

        AuthData(Login _login, uint *sh1) :
                pass_sha1_hash(sh1),
                status(enDataStatus::engaged) {
            strcpy(login, _login);
        }

        AuthData &operator=(const AuthData &other) {
            strcpy(login, other.login);

            if (pass_sha1_hash != nullptr)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            status = other.status;

            return *this;
        }

        bool operator==(const AuthData &other) {
            return status == other.status &&
                   (status != enDataStatus::engaged ||
                    (pass_sha1_hash == other.pass_sha1_hash && !strcmp(login, other.login)));
        }

        Login login;
        uint *pass_sha1_hash;
        enDataStatus status;
    };

    void resize();
    int hash_func(Login login, int offset) const;
    void add(Login login, uint* pass_hash);

    AuthData *data;
    int mem_size;
    int data_count;
};