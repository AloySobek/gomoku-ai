#include <iostream>
#include <bitset>
#include <chrono>
#include <string>
#include <cstring>

#define LEN 5
#define MASK 0b10000
#define REPORT(M) std::cout << M << " " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << " microseconds" << std::endl;
using namespace std;
uint32_t getColumn(const uint32_t b[LEN], uint32_t n)
{
    uint32_t r = 0b0;
    for (int i = 0; i < LEN; ++i) {
        r = r >> 1 | (b[i] & (MASK >> n));
    }
    return r;
}

uint32_t getRow(const uint32_t b[LEN], uint32_t n)
{
    return b[n];
}

uint32_t getUpDiagonal(const uint32_t b[LEN], uint32_t n)
{
    uint32_t r = 0b0;
    uint16_t len =  n < LEN ? n % LEN + 1 : LEN - (n - LEN) - 1;
    uint16_t start = n < LEN ? 0 : LEN - len;
//    std::cout << "d len: " << len << " start: " << start << std::endl;
    for (int i = 0; i < len ; ++i) {
//        std::bitset<LEN> M((MASK >> (start + len - i - 1)));
//        std::bitset<LEN> B(b[start + i]);
//
//        std::bitset<LEN> RP(r);
//        std::bitset<LEN> RS((b[start + i] & (MASK >> (start + len - i - 1))));
//        std::cout << "up: " << "R:" << RP << " M: " << M << " & " << B << "[" << start + i << "]" << " = " << RS << "==";
        r |= (b[start + i] & (MASK >> (start + len - i - 1)));

//        std::bitset<LEN> R(r);
//        std::cout << R << std::endl;
    }
    return r;
}

uint32_t getDownDiagonal(const uint32_t b[LEN], uint32_t n)
{
    uint32_t r = 0b0;
    uint16_t len =  n < LEN ? n % LEN + 1 : LEN - (n - LEN) - 1;
    uint16_t start = (n < LEN ? LEN : len) - 1;
    std::cout << "d len: " << len << " start: " << start << std::endl;
    for (int i = 0; i < len ; ++i) {
//        std::bitset<LEN> M((MASK >> ()));
//        std::bitset<LEN> B(b[start - i]);
//
//        std::bitset<LEN> RP(r);
//        std::bitset<LEN> RS((b[start - i] & (MASK >> (i))));
//        std::cout << "dwn: " << "R:" << RP << " M: " << M << " & " << B << "[" << start - i << "]" << " = " << RS << "==";
        r |= (b[start - i] & (MASK >> (i)));

        std::bitset<LEN> R(r);
        std::cout << R << std::endl;
    }
    return r;
}

uint32_t getColumn2(const uint32_t b[LEN], const uint32_t n)
{
    uint32_t r = 0;
    r >>= 1;
    r |= (b[0] & (MASK >> n));
    r >>= 1;
    r |= (b[1] & (MASK >> n));
    r >>= 1;
    r |= (b[2] & (MASK >> n));
    r >>= 1;
    r |= (b[3] & (MASK >> n));
    r >>= 1;
    r |= (b[4] & (MASK >> n));
    return r;
}

void getFlat(int x, int y, int dx, int dy, uint8_t v, uint8_t extent, const uint32_t bb[LEN], const uint32_t bw[LEN], char res[LEN]) {
    res[LEN/2] = '0' + v;
    int xx, yy;
    for (int i = 1; i <= extent; ++i) {
        xx = x + i * dx;
        yy = y + i * dy;
        if (xx > LEN-1 || xx < 0 || yy > LEN-1 || yy < 0)
            res[LEN/2 + i] = res[LEN/2 + i - 1] == ']' ? '.' : ']';
        else
            res[LEN/2 + i] = bb[yy] & 0b1 >> xx ? '2' : bw[yy] & 0b1 >> xx ? '1' : '0';
        xx = x - i * dx;
        yy = y - i * dy;
        if (xx > LEN-1 || xx < 0 || yy > LEN-1 || yy < 0)
            res[LEN/2 - i] = res[LEN/2 - i + 1] == '[' ? '.' : '[';
        else
            res[LEN/2 - i] = bb[yy] & 0b1 >> xx ? '2' : bw[yy] & 0b1 >> xx ? '1' : '0';
    }
}
constexpr char FPT[][5][7] = {
        {},
        {},
        {
            "0110",
        },
        {
            "01110",
            "011010",
            "010110",
        },
        {
            "011110",
        }
};

int main() {

    uint32_t bw[LEN]={
            0b10000,
            0b10000,
            0b11000,
            0b11100,
            0b11111,
    };
    uint32_t bb[LEN]={
            0b00000,
            0b00010,
            0b00000,
            0b00000,
            0b00000,
    };

//    for (unsigned int i : bb) {
//        std::bitset<LEN> x(i);
//        std::cout << x << std::endl;
//    }
    char flat [LEN] = {'0','0','0','0','0'};
    getFlat(0, 0, 1, 1, 2, 2, bb, bw, flat);
    for (const auto &item : flat)
        std::cout << item;
    std::cout << std::endl;
    getFlat(3, 1, 0, 1, 2, 2, bb, bw, flat);
    for (const auto &item : flat)
        std::cout << item;
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        char flat2 [LEN] = {'0','0','0','0','0'};
        getFlat(1, 1, -1, 1, 2,  2, bb, bw, flat2);
        (void)strstr(flat2, "020");
    }
    for (const auto &item : FPT[3]){
        cout << "FPT[3] = " << item << endl;
    }
    REPORT("getColumn")

    char n[7] = "0110";
    char s[25] = "[000000000011000000000]";
    cout << "strstr: " << n << " & " << s << strstr("0110", "000111000Hello") << endl;

//    for (int i = 0; i < LEN; ++i) {
//        std::bitset<LEN> x(getColumn(b, i));
//        std::cout << "column: " << i << '=' << x << std::endl;
//        (void)x;
//    }
//    REPORT("getColumn")
//
////    start = std::chrono::high_resolution_clock::now();
////    for (int i = 0; i < LEN + LEN - 1; ++i) {
////        std::cout << "getUpDiagonal: " << i << std::endl;
////        std::bitset<LEN> x(getUpDiagonal(b, i));
////        auto len = (i < LEN ? i % LEN + 1 : LEN - (i - LEN) - 1);
////        std::cout << '=';
//////        for (int j = 0; j < LEN; ++j) {
//////            std::cout << x[j];
//////        }
////
////        std::cout << x << std::endl;
////    }
////    REPORT("getRows")
//    start = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < LEN + LEN - 1; ++i) {
//        std::cout << "getUpDiagonal: " << i << std::endl;
//        std::bitset<LEN> x(getDownDiagonal(b, i));
//        std::cout << '=';
//        std::cout << x << std::endl;
//    }
//    REPORT("getRows")

//    std::bitset<LEN> x(MASK);
//    std::cout << "MASK: " << '=' << x << std::endl;
//    std::cout << "test: " << '=' << x << std::endl;
}
