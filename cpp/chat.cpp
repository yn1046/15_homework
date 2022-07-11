#include "../headers/chat.h"
#include "iostream"
#include <string.h>

#define DEFAULT_MEM_SIZE 8

Chat::Chat() {
    data_count = 0;
    mem_size = DEFAULT_MEM_SIZE;
    data = new AuthData[mem_size];
}

Chat::~Chat() {
    delete[] data;
}

void Chat::resize() {
    AuthData *save = data;
    int save_ms = mem_size;

    mem_size *= 2;
    data = new AuthData[mem_size];
    data_count = 0;

    for (int i = 0; i < save_ms; i++) {
        AuthData &old_data = save[i];
        if (old_data.status == enDataStatus::engaged) {
            add(old_data.login, old_data.pass_sha1_hash);
        }
    }

    delete[] save;
}

void Chat::reg(Login _login, char _pass[], int pass_length) {
    add(_login, sha1(_pass, pass_length));
}


// ЛОГИН И ХЕШ КИНУТЬ В ХТ
// ЛОГИН — КЛЮЧ
// ХЕШ — ЗНАЧЕНИЕ
bool Chat::login(Login _login, char _pass[], int pass_length) {
    for (int i = 0; i < mem_size; i++) {
        int index = hash_func(_login, i);
        if (data[index].status == enDataStatus::engaged && !strcmp(data[index].login, _login)) {
            return (*data[index].pass_sha1_hash) == (*sha1(_pass, pass_length));
        } else if (data[index].status == enDataStatus::free) {
            return false;
        }
    }

    return false;
}

int Chat::hash_func(Login login, int offset) const {
    // вычисляем индекс
    int sum = 0, i = 0;
    for (; i < strlen(login); i++) {
        sum += login[i];
    }
    // метод умножения
    const double A = 0.6391;
    auto index = int(mem_size * (A * sum - int(A * sum)));
    // квадратичные пробы
    return (index + offset * offset) % mem_size;
}

void Chat::add(Login login, uint *pass_hash) {
    int index = -1, i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < mem_size; i++) {
        index = hash_func(login, i);
        if (data[index].status == enDataStatus::free) {
            // найдена пустая ячейка, занимаем ее
            break;
        }
    }

    if (i >= mem_size) {
        resize();
        add(login, pass_hash);
    } else {
        data[index] = AuthData(login, pass_hash);
        data_count++;
    }
}
