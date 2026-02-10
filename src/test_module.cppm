//
// Created by zhenya on 04.02.2026.
//


export module mylib.utils;

export namespace mylib {
    int get_int();
    class GameUnit {
        public:
            GameUnit();
            int x;
            int y;
            int z;
    };
}


int mylib::get_int() {
    return 2;
}