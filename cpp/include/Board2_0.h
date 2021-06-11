#ifndef GOMOKU_BOARD2_0_H
#define GOMOKU_BOARD2_0_H

#include <iostream>
#include <chrono>
#include <cstring>
#include <unordered_map>


#define BS 19
using Pt = std::pair<int, int>;
struct XyHash
{
    std::size_t operator()(const std::pair<int, int> &x) const
    {
        return std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
    }
};
static std::unordered_map<Pt, Pt, XyHash> GenColumnProjectionMapping() {
    std::unordered_map<Pt, Pt, XyHash> map;
    for (int xp = 0; xp < BS; ++xp)
        for (int yp = 0; yp < BS; ++yp)
            map.insert({{yp, xp}, {xp, yp}});
    return map;
}
static std::unordered_map<Pt, Pt, XyHash> GenDiagonalProjectionMapping(bool up) {
    std::unordered_map<Pt, Pt, XyHash> map;
    for (int w = 1; w <= BS * 2 - 1; ++w)
    {
        int fromColumn = std::max(0, w - BS);
        int endAt = std::min(w, std::min((BS - fromColumn), BS));

        for (int i = 0; i < endAt; ++i) {
            if (up)
                map.insert({
                                   {(std::min(BS, w) - i - 1), (fromColumn + i)},
                                   {(w - 1), i}
                           });
            else
                map.insert({
                                   {(std::min(BS, w) - i - 1), (BS - 1 - (fromColumn + i))},
                                   {(w - 1), i}
                           });
        }
    }
    return map;
}

const auto UP_MAP = GenDiagonalProjectionMapping(false);
const auto DW_MAP = GenDiagonalProjectionMapping(true);
const auto CL_MAP = GenColumnProjectionMapping();

struct Ptr
{
    char 	str[8];
    int		len;
};
constexpr static Ptr wFive{"22222", 5};
constexpr static Ptr wFree4{"022220", 6};
constexpr static Ptr wHalf4_1{"122220", 6};
constexpr static Ptr wHalf4_2{"022221", 6};
constexpr static Ptr wHalf4_3{"22202", 5};
constexpr static Ptr wHalf4_4{"22022", 5};
constexpr static Ptr wHalf4_5{"20222", 5};
constexpr static Ptr wHalf4_6{"02222", 5};
constexpr static Ptr wHalf4_7{"22220", 5};
constexpr static Ptr wFree3_1{"0022200", 7};
constexpr static Ptr wFree3_2{"022200", 6};
constexpr static Ptr wFree3_3{"002220", 6};
constexpr static Ptr wFree3_4{"020220", 6};
constexpr static Ptr wFree3_5{"022020", 6};

constexpr static Ptr wHalf3_1{"002221", 6};
constexpr static Ptr wHalf3_2{"122200", 6};
constexpr static Ptr wHalf3_3{"122020", 6};
constexpr static Ptr wHalf3_4{"020221", 6};
constexpr static Ptr wHalf3_5{"120220", 6};
constexpr static Ptr wHalf3_6{"022021", 6};
constexpr static Ptr wZebra{"2020202", 7};
constexpr static Ptr bFive{"11111", 5};

constexpr static Ptr bFree4{"011110", 6};
constexpr static Ptr bHalf4_1{"211110", 6};
constexpr static Ptr bHalf4_2{"011112", 6};
constexpr static Ptr bHalf4_3{"11101", 5};
constexpr static Ptr bHalf4_4{"11011", 5};
constexpr static Ptr bHalf4_5{"10111", 5};
constexpr static Ptr bHalf4_6{"01111", 5};
constexpr static Ptr bHalf4_7{"11110", 5};
constexpr static Ptr bFree3_1{"0011100", 7};
constexpr static Ptr bFree3_2{"011100", 6};
constexpr static Ptr bFree3_3{"001110", 6};
constexpr static Ptr bFree3_4{"010110", 6};
constexpr static Ptr bFree3_5{"011010", 6};

constexpr static Ptr bHalf3_1{"211100", 6};
constexpr static Ptr bHalf3_2{"001112", 6};
constexpr static Ptr bHalf3_3{"211010", 6};
constexpr static Ptr bHalf3_4{"010112", 6};
constexpr static Ptr bHalf3_5{"210110", 6};
constexpr static Ptr bHalf3_6{"011012", 6};
constexpr static Ptr bZebra{"1010101", 7};

class Board
{
public:
    enum Color
    {
        EMPTY = 0,
        WHITE,
        BLACK
    };
    enum Result
    {
        _ = 0,
        WHITE_WIN,
        BLACK_WIN,
        DRAW
    };
    int move = EMPTY;
    Result result = DRAW;
    int whiteCaptures = 0;
    int blackCaptures = 0;
    using Line = char[BS+1];
    std::array<Line, BS> rows{};
    std::array<Line, BS> columns{};
    std::array<Line, BS+BS-1> down{};
    std::array<Line, BS+BS-1> up{};

    template<typename B>
    int PtrLocalMatch(const B &board, const Ptr &ptr, int row, int col) const {
        // TODO test
        int count{0};
        const char *line = board[row];
        line += std::max(0, col - ptr.len);
        if (!*line)
            return 0;
        const char *match = line;
        do {
            match = strstr(match, ptr.str);
            if (match && match <= line + col) {
                count++;
                match += ptr.len;
            } else
                break;
        } while (match);
        return count;
    }
    enum MatchOnly {
        NONE=0,
        END,
        BEGIN,
    };
    template<typename B> // for tow different arrays std::array<Line, BS> and std::array<Line, BS+BS-1>
    int PtrGlobalMatch(const B &board, const Ptr &ptr, const bool projected = false, const MatchOnly only = NONE) const {
        int count{0};
        if (!projected) // Row/Column Match, Just strstr to the end baby! Except some only given..
        {
            for (const auto& line : board)
            {
                const char *match = line;
                if (only == END) // "............PTR\0" skip ti possible end match at
                    match += BS-ptr.len;
                do {
                    match = strstr(match, ptr.str);
                    if (match != nullptr && (only != BEGIN || match == line)) {
                        count++;
                        match += ptr.len;
                    }
                    // "PTR............\0" Do not continue if BEGIN
                } while (only == NONE && match != nullptr && *match);
            }
        } else { // Up/Down Diagonal, Start from suitable for ptr length length
            // 0       1
            // 1       11
            // 2       111   <---- start
            // 3       1111   if ptr len is 3 start from index 2 and end with 4
            // 4       111   <----  end
            // 5       11
            // 6       1
            const int end = (BS+BS-1) - ptr.len-1;
            for (int i = ptr.len-1; i < end; ++i) {
                const char *match = board[i];
                do {
                    match = strstr(match, ptr.str);
                    if (match != nullptr && (only != BEGIN || match == board[i])) {
                        count++;
                        // start of match is returned, advance forward!
                        match += ptr.len;
                    }
                } while (only == NONE && match != nullptr && *match);
            }
        }

        return (count);
    }

    Color getToken(int x, int y) const {
        switch (rows[y][x]) {
            case '0':
                return Color::EMPTY;
            case '1':
                return Color::BLACK;
            case '2':
                return Color::WHITE;
            default:
                throw std::runtime_error("Trash on board!");
        }
    }
    void setToken(int x, int y, const Color color) {
        const auto &cxy = CL_MAP.at({x, y});
        const auto &uxy = UP_MAP.at({x, y});
        const auto &dxy = DW_MAP.at({x, y});
        rows[x][y] = '0' + color;
        columns[cxy.first][cxy.second] = '0' + color;
        up[uxy.first][uxy.second] = '0' + color;
        down[dxy.first][dxy.second] = '0' + color;
    }

    friend std::ostream& operator<<(std::ostream& os, const Board& board) {
        os << board.move << std::endl;

        for (int i = BS - 1; i >= 0; --i)
        {
            for (int j = 0; j < BS; ++j)
            {
                switch (board.getToken(i, j)) {
                    case Board::EMPTY:
                        os << "_";
                        break;
                    case Board::WHITE:
                        os << "X";
                        break;
                    case Board::BLACK:
                        os << "O";
                        break;
                }
            }
            os << std::endl;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Board& board) {
        board = Board();
        int skip;
        is >> skip >> skip >> board.move;

        for (int i = BS - 1; i > -1; i--)
        {
            for (int j = 0; j < BS; j++) {
                char token;
                is >> token;

                switch (token) {
                    case '_':
                        board.setToken(i, j, Color::EMPTY);
                        break;
                    case 'X':
                        board.setToken(i, j, Color::WHITE);
                        break;
                    case 'O':
                        board.setToken(i, j, Color::BLACK);
                        break;
                    default:
                        break;
                }
            }
        }
        return is;
    }
    int getTokenCount() const
    {
        int count = 0;
        for (const auto &line: rows) {
            for (const auto &item : line) {
                count += int(item != '0' && item != '\0');
            }
        }
        return count;
    }
    int PtrMatchFree3(Color color) const;
    int PtrMatchHalf4(Color color) const;
    int PtrMatchHalf3(Color color) const;
    bool PtrMatch(const Ptr &ptr) const;
    int Eval() const;
};

int Board::PtrMatchFree3(Board::Color color) const {

    if (color == WHITE)
    {
        return PtrGlobalMatch(rows, wFree3_2) +
        PtrGlobalMatch(columns, wFree3_2) +
        PtrGlobalMatch(up, wFree3_2, true) +
        PtrGlobalMatch(down, wFree3_2, true) +

        PtrGlobalMatch(rows, wFree3_3) +
        PtrGlobalMatch(columns, wFree3_3) +
        PtrGlobalMatch(up, wFree3_3, true) +
        PtrGlobalMatch(down, wFree3_3, true) -

        PtrGlobalMatch(rows, wFree3_1) -
        PtrGlobalMatch(columns, wFree3_1) -
        PtrGlobalMatch(up, wFree3_1, true) -
        PtrGlobalMatch(down, wFree3_1, true) +

        PtrGlobalMatch(rows, wFree3_4) +
        PtrGlobalMatch(columns, wFree3_4) +
        PtrGlobalMatch(up, wFree3_4, true) +
        PtrGlobalMatch(down, wFree3_4, true) +

        PtrGlobalMatch(rows, wFree3_5) +
        PtrGlobalMatch(columns, wFree3_5) +
        PtrGlobalMatch(up, wFree3_5, true) +
        PtrGlobalMatch(down, wFree3_5, true);
    }
    else
    {
        return PtrGlobalMatch(rows, bFree3_2) +
        PtrGlobalMatch(columns, bFree3_2) +
        PtrGlobalMatch(up, bFree3_2, true) +
        PtrGlobalMatch(down, bFree3_2, true) +

        PtrGlobalMatch(rows, bFree3_3) +
        PtrGlobalMatch(columns, bFree3_3) +
        PtrGlobalMatch(up, bFree3_3, true) +
        PtrGlobalMatch(down, bFree3_3, true)  -

        PtrGlobalMatch(rows, bFree3_1)  -
        PtrGlobalMatch(columns, bFree3_1)  -
        PtrGlobalMatch(up, bFree3_1, true)  -
        PtrGlobalMatch(down, bFree3_1, true) +

        PtrGlobalMatch(rows, bFree3_4) +
        PtrGlobalMatch(columns, bFree3_4) +
        PtrGlobalMatch(up, bFree3_4, true) +
        PtrGlobalMatch(down, bFree3_4, true) +

        PtrGlobalMatch(rows, bFree3_5) +
        PtrGlobalMatch(columns, bFree3_5) +
        PtrGlobalMatch(up, bFree3_5, true) +
        PtrGlobalMatch(down, bFree3_5, true);
    }
}

int Board::PtrMatchHalf4(Board::Color color) const {
    if (color == Color::WHITE)
    {
        return PtrGlobalMatch(rows, wHalf4_1) +
        PtrGlobalMatch(columns, wHalf4_1) +
        PtrGlobalMatch(up, wHalf4_1, true) +
        PtrGlobalMatch(down, wHalf4_1, true) +

        PtrGlobalMatch(rows, wHalf4_2) +
        PtrGlobalMatch(columns, wHalf4_2) +
        PtrGlobalMatch(up, wHalf4_2, true) +
        PtrGlobalMatch(down, wHalf4_2, true) +

        PtrGlobalMatch(rows, wHalf4_3) +
        PtrGlobalMatch(columns, wHalf4_3) +
        PtrGlobalMatch(up, wHalf4_3, true) +
        PtrGlobalMatch(down, wHalf4_3, true) +

        PtrGlobalMatch(rows, wHalf4_4) +
        PtrGlobalMatch(columns, wHalf4_4) +
        PtrGlobalMatch(up, wHalf4_4, true) +
        PtrGlobalMatch(down, wHalf4_4, true) +

        PtrGlobalMatch(rows, wHalf4_5) +
        PtrGlobalMatch(columns, wHalf4_5) +
        PtrGlobalMatch(up, wHalf4_5, true) +
        PtrGlobalMatch(down, wHalf4_5, true) +

        PtrGlobalMatch(rows, wHalf4_6, false, END) +
        PtrGlobalMatch(columns, wHalf4_6, false, END) +
        PtrGlobalMatch(up, wHalf4_6, true, END) +
        PtrGlobalMatch(down, wHalf4_6, true, END) +

        PtrGlobalMatch(rows, wHalf4_7, false, BEGIN) +
        PtrGlobalMatch(columns, wHalf4_7, false, BEGIN) +
        PtrGlobalMatch(up, wHalf4_7, true, BEGIN) +
        PtrGlobalMatch(down, wHalf4_7, true, BEGIN);

    }
    else
    {
        return PtrGlobalMatch(rows, bHalf4_1) +
        PtrGlobalMatch(columns, bHalf4_1) +
        PtrGlobalMatch(up, bHalf4_1, true) +
        PtrGlobalMatch(down, bHalf4_1, true) +

        PtrGlobalMatch(rows, bHalf4_2) +
        PtrGlobalMatch(columns, bHalf4_2) +
        PtrGlobalMatch(up, bHalf4_2, true) +
        PtrGlobalMatch(down, bHalf4_2, true) +

        PtrGlobalMatch(rows, bHalf4_3) +
        PtrGlobalMatch(columns, bHalf4_3) +
        PtrGlobalMatch(up, bHalf4_3, true) +
        PtrGlobalMatch(down, bHalf4_3, true) +

        PtrGlobalMatch(rows, bHalf4_4) +
        PtrGlobalMatch(columns, bHalf4_4) +
        PtrGlobalMatch(up, bHalf4_4, true) +
        PtrGlobalMatch(down, bHalf4_4, true) +

        PtrGlobalMatch(rows, bHalf4_5) +
        PtrGlobalMatch(columns, bHalf4_5) +
        PtrGlobalMatch(up, bHalf4_5, true) +
        PtrGlobalMatch(down, bHalf4_5, true) +

        PtrGlobalMatch(rows, bHalf4_6, false, END) +
        PtrGlobalMatch(columns, bHalf4_6, false, END) +
        PtrGlobalMatch(up, bHalf4_6, true, END) +
        PtrGlobalMatch(down, bHalf4_6, true, END) +

        PtrGlobalMatch(rows, bHalf4_7, false, BEGIN) +
        PtrGlobalMatch(columns, bHalf4_7, false, BEGIN) +
        PtrGlobalMatch(up, bHalf4_7, true, BEGIN) +
        PtrGlobalMatch(down, bHalf4_7, true, BEGIN);
    }
}

int Board::PtrMatchHalf3(Board::Color color) const {
    if (color == WHITE)
    {
         return PtrGlobalMatch(rows, wHalf3_1) +
         PtrGlobalMatch(columns, wHalf3_1) +
         PtrGlobalMatch(up, wHalf3_1, true) +
         PtrGlobalMatch(down, wHalf3_1, true) +

         PtrGlobalMatch(rows, wHalf3_2) +
         PtrGlobalMatch(columns, wHalf3_2) +
         PtrGlobalMatch(up, wHalf3_2, true) +
         PtrGlobalMatch(down, wHalf3_2, true) +

         PtrGlobalMatch(rows, wHalf3_3) +
         PtrGlobalMatch(columns, wHalf3_3) +
         PtrGlobalMatch(up, wHalf3_3, true) +
         PtrGlobalMatch(down, wHalf3_3, true) +

         PtrGlobalMatch(rows, wHalf3_4) +
         PtrGlobalMatch(columns, wHalf3_4) +
         PtrGlobalMatch(up, wHalf3_4, true) +
         PtrGlobalMatch(down, wHalf3_4, true) +

         PtrGlobalMatch(rows, wHalf3_5) +
         PtrGlobalMatch(columns, wHalf3_5) +
         PtrGlobalMatch(up, wHalf3_5, true) +
         PtrGlobalMatch(down, wHalf3_5, true) +

         PtrGlobalMatch(rows, wHalf3_6) +
         PtrGlobalMatch(columns, wHalf3_6) +
         PtrGlobalMatch(up, wHalf3_6, true) +
         PtrGlobalMatch(down, wHalf3_6, true);
    }
    else
    {
         return PtrGlobalMatch(rows, bHalf3_1) +
         PtrGlobalMatch(columns, bHalf3_1) +
         PtrGlobalMatch(up, bHalf3_1, true) +
         PtrGlobalMatch(down, bHalf3_1, true) +

         PtrGlobalMatch(rows, bHalf3_2) +
         PtrGlobalMatch(columns, bHalf3_2) +
         PtrGlobalMatch(up, bHalf3_2, true) +
         PtrGlobalMatch(down, bHalf3_2, true) +

         PtrGlobalMatch(rows, bHalf3_3) +
         PtrGlobalMatch(columns, bHalf3_3) +
         PtrGlobalMatch(up, bHalf3_3, true) +
         PtrGlobalMatch(down, bHalf3_3, true) +

         PtrGlobalMatch(rows, bHalf3_4) +
         PtrGlobalMatch(columns, bHalf3_4) +
         PtrGlobalMatch(up, bHalf3_4, true) +
         PtrGlobalMatch(down, bHalf3_4, true) +

         PtrGlobalMatch(rows, bHalf3_5) +
         PtrGlobalMatch(columns, bHalf3_5) +
         PtrGlobalMatch(up, bHalf3_5, true) +
         PtrGlobalMatch(down, bHalf3_5, true) +

         PtrGlobalMatch(rows, bHalf3_6) +
         PtrGlobalMatch(columns, bHalf3_6) +
         PtrGlobalMatch(up, bHalf3_6, true) +
         PtrGlobalMatch(down, bHalf3_6, true);
    }
}

bool Board::PtrMatch(const Ptr &ptr) const {
    if (PtrGlobalMatch(rows, ptr))
        return true;
    else if (PtrGlobalMatch(columns, ptr))
        return true;
    else if (PtrGlobalMatch(up, ptr, true))
        return true;
    else if (PtrGlobalMatch(down, ptr, true))
        return true;
    else
        return false;
}

int Board::Eval() const {
    // Game ended
    switch (result) {
        case WHITE_WIN:
            return +100;
        case BLACK_WIN:
            return -100;
        case DRAW: // No available moves
            return 0;
        default:
            break;
    }
    if (PtrMatch(wFive))
        return +50;
    else if (PtrMatch(bFive))
        return -50;
    else if (PtrMatch(wZebra) && move == WHITE)
        return +30;
    else if (PtrMatch(bZebra) && move == BLACK)
        return -30;
    int score{0};

    auto half4sW = PtrMatchHalf4(WHITE);
    auto half4sB = PtrMatchHalf4(BLACK);
    // Free 4 what is win or flanked four and your turn what is almost win
    if (PtrMatch(wFree4) || (half4sW && move == WHITE))
        return +40;
    else if (PtrMatch(bFree4) || (half4sB && move == BLACK))
        return -40;
    else if (half4sW > 1)
        return +35;
    else if (half4sB > 1)
        return -35;
    score += (half4sW - half4sB) * 2;

    auto free3sW = PtrMatchFree3(WHITE);
    auto free3sB = PtrMatchFree3(BLACK);
    // Free 3 what is win or flanked four and your turn what is almost win
    if (free3sW && move == WHITE)
        return +15;
    else if (free3sB && move == BLACK)
        return -15;
    else if (free3sW > 1 && !free3sB)
        return +13;
    else if (free3sB > 1 && !free3sW)
        return -13;
    score += (free3sW - free3sB) * 2;

    // Half 3 what is not a win but counts or flanked four and your turn what is not almost win
    auto half3sW = PtrMatchHalf3(WHITE);
    auto half3sB = PtrMatchHalf3(BLACK);
    score += (half3sW - half3sB);

    // Future captures for available moves // TODO cW - cB * 4

    score += (whiteCaptures - blackCaptures) * 7;
    return (score);
}

#endif //GOMOKU_BOARD2_0_H
