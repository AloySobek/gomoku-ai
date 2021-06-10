//
// Created by misha on 09.06.2021.
//

#ifndef GOMOKU_PATTERNS_HPP
#define GOMOKU_PATTERNS_HPP
#include "board.hpp"
#include "cstring"

constexpr char P1_FOUR_FREE_PTR[1][7] = {
        "011110",
};
constexpr char P2_FOUR_FREE_PTR[1][7] = {
        "022220",
};
constexpr char P1_THREE_FREE_PTR[3][7] = {
        "01110",
        "011010",
        "010110",
};
constexpr char P2_THREE_FREE_PTR[3][7] = {
        "02220",
        "022020",
        "020220",
};
constexpr char P1_TOW_FREE_PTR[5] = "0110";
constexpr char P2_TOW_FREE_PTR[5] = "0220";

constexpr char P1_CAPTURE_PTR[5] = "1221";
constexpr char P2_CAPTURE_PTR[5] = "2112";

constexpr char P1_WIN_PTR[6] = "11111";
constexpr char P2_WIN_PTR[6] = "22222";


constexpr int8_t ALL_DIRS[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1},
};

class Patterns {
public:
    static void getFlat(int x, int y, int dx, int dy, uint8_t v, uint8_t extent, const int32_t bb[BOARD_SIZE], const int32_t bw[BOARD_SIZE], char res[BOARD_SIZE]) {
        res[BOARD_SIZE/2] = '0' + v;
        int xx, yy;
        for (int i = 1; i <= extent; ++i) {
            xx = x + i * dx;
            yy = y + i * dy;
            if (xx > BOARD_SIZE-1 || xx < 0 || yy > BOARD_SIZE-1 || yy < 0)
                res[BOARD_SIZE/2 + i] = res[BOARD_SIZE/2 + i - 1] == ']' ? '.' : ']';
            else
                res[BOARD_SIZE/2 + i] = bb[yy] & 262144 >> xx ? '1' : (bw[yy] & 262144 >> xx ? '2' : '0');
            xx = x - i * dx;
            yy = y - i * dy;
            if (xx > BOARD_SIZE-1 || xx < 0 || yy > BOARD_SIZE-1 || yy < 0)
                res[BOARD_SIZE/2 - i] = res[BOARD_SIZE/2 - i + 1] == '[' ? '.' : '[';
            else
                res[BOARD_SIZE/2 - i] = bb[yy] & 262144 >> xx ? '1' : (bw[yy] & 262144 >> xx ? '2' : '0');
        }
    }

    static bool isThreeFree(const uint8_t v, const char flat[BOARD_SIZE]) {
        for (const auto &ptr : v == BLACK_STONE ? P1_THREE_FREE_PTR : P2_THREE_FREE_PTR) {
            if (strstr(flat, ptr))
                return true;
        }
        return false;
    }
    static bool isTowFree(const uint8_t v, const char flat[BOARD_SIZE]) {
        return strstr(flat, v == BLACK_STONE ? P1_TOW_FREE_PTR : P2_TOW_FREE_PTR) != nullptr;
    }
    static bool isFourFree(const uint8_t v, const char flat[BOARD_SIZE]) {
        for (const auto &ptr : v == BLACK_STONE ? P1_FOUR_FREE_PTR : P2_FOUR_FREE_PTR) {
            if (strstr(flat, ptr))
                return true;
        }
        return false;
    }
    static bool isWin(const uint8_t v, const char flat[BOARD_SIZE]) {
        return strstr(flat, v == BLACK_STONE ? P1_WIN_PTR : P2_WIN_PTR) != nullptr;
    }
    static bool isCapture(const uint8_t v, const char flat[BOARD_SIZE]) {
        return strstr(flat, v == BLACK_STONE ? P1_CAPTURE_PTR : P2_CAPTURE_PTR) != nullptr;
    }
    static bool isUnderCapture(const uint8_t v, const char flat[BOARD_SIZE]) {
        return strstr(flat, v == BLACK_STONE ? P2_CAPTURE_PTR : P1_CAPTURE_PTR) != nullptr;
    }
};


#endif //GOMOKU_PATTERNS_HPP
