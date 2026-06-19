#pragma once

struct Menu {
    const char **items;
    int size;
};

void menuInit();
void menuLoop();
