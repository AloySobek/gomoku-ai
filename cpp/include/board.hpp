#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <cstdint>
# include <string>
# include <cstring>
# include <bitset>
# include <limits>
# include <chrono>
# include <random>
# include <unordered_map>
# include <Patterns.hpp>

# define BOARD_SIZE 19
# define BS BOARD_SIZE

# define EMPTY_STONE 0
# define BLACK_STONE 1
# define WHITE_STONE 2

// Patterns
typedef struct SPtr
{
    char 	str[8];
    int		len;
} Ptr;

constexpr static Ptr bB5_1{"12222", 5};
constexpr static Ptr bB5_2{"22221", 5};
constexpr static Ptr wB5_1{"21111", 5};
constexpr static Ptr wB5_2{"11112", 5};

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
// Cache mapping from columns and diagonals
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

// END Cache mapping from columns and diagonals

class Board
{
public:
    uint64_t nodes_count{0};
    uint64_t pruned_count{0};
    uint64_t cache_hit_count{0};

    uint64_t black_captures_count{0};
    uint64_t white_captures_count{0};

    int32_t *move_map{new int32_t[BOARD_SIZE * BOARD_SIZE]()};
    int32_t *black_board{new int32_t[BOARD_SIZE]()};
    int32_t *white_board{new int32_t[BOARD_SIZE]()};

    Board();
    bool place_stone_on_board(int8_t x, int8_t y, bool is_black, uint8_t *captures=nullptr);
    bool remove_stone_from_board(int8_t x, int8_t y, bool is_black, uint8_t *captures=nullptr);
    int32_t minimax(int8_t depth, int32_t alpha, int32_t beta, int8_t x, int8_t y, bool maximizer, bool is_black);
    int32_t ai_move(bool is_black);
    void reset();
    void print();

    std::unordered_map<uint64_t, int32_t> hash_map;
private:
    uint64_t *zobrist_table{new uint64_t[BOARD_SIZE * BOARD_SIZE * 2]()};
    uint64_t hash{0};

    int32_t start_x{0}, start_y{0};
    int32_t end_x{0}, end_y{0};
    int32_t most_left{0}, most_right{0};

    void fill_zobrist_table();
    uint64_t get_hash();

    bool five_in_a_row(int32_t x, int32_t y, bool is_black);
    bool open_four(int32_t x, int32_t y, bool is_black);
    bool simple_four(int32_t x, int32_t y, bool is_black);
    bool open_three(int32_t x, int32_t y, bool is_black);
    bool simple_three(int32_t x, int32_t y, bool is_black);
    bool open_two(int32_t x, int32_t y, bool is_black);
    bool simple_two(int32_t x, int32_t y, bool is_black);

// Board v2
public:
    enum Color
    {
        EMPTY = EMPTY_STONE,
        WHITE = WHITE_STONE,
        BLACK = BLACK_STONE
    };
    enum Result
    {
        NO_RESULT = 0,
        WHITE_WIN,
        BLACK_WIN,
        DRAW
    };
    bool lastMoveIsCapture = false;
    using Line = char[BS+1];
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

    Color getToken(int x, int y) const {
        auto cxy = CL_MAP.at({x, y});
        switch (columns[cxy.first][cxy.second]) {
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
private:
    void setToken(int x, int y, const Color color) {
        const auto &cxy = CL_MAP.at({x, y});
        const auto &uxy = UP_MAP.at({x, y});
        const auto &dxy = DW_MAP.at({x, y});
        rows[x][y] = '0' + color;
        columns[cxy.first][cxy.second] = '0' + color;
        up[uxy.first][uxy.second] = '0' + color;
        down[dxy.first][dxy.second] = '0' + color;
    }

public:
    int8_t moveX = 0;
    int8_t moveY = 0;
    int move = EMPTY;
    Result result = NO_RESULT;
    std::array<Line, BS> rows{};
    std::array<Line, BS> columns{};
    std::array<Line, BS+BS-1> down{};
    std::array<Line, BS+BS-1> up{};


    int PtrLocalMatchAll(const Ptr &ptr, int x, int y) const {
        return PtrLocalMatch(rows, ptr, x, y)
               + PtrLocalMatch(columns, ptr, CL_MAP.at({x, y}))
               + PtrLocalMatch(up, ptr, UP_MAP.at({x, y}))
               + PtrLocalMatch(down, ptr, DW_MAP.at({x, y}));
    }

    int PtrLocal5Match(Color color, int x, int y) const {
        return PtrLocalMatch(rows, color == BLACK ? bFive : wFive, x, y)
        + PtrLocalMatch(columns, color == BLACK ? bFive : wFive, CL_MAP.at({x, y}))
        + PtrLocalMatch(up, color == BLACK ? bFive : wFive, UP_MAP.at({x, y}))
        + PtrLocalMatch(down, color == BLACK ? bFive : wFive, DW_MAP.at({x, y}));
    }

    template<typename B>
    int PtrLocalMatch(const B &board, const Ptr &ptr, const Pt &pt) const {
        return PtrLocalMatch(board, ptr, pt.first, pt.second);
    }
    template<typename B>
    int PtrLocalMatch(const B &board, const Ptr &ptr, int x, int y) const {
        int count{0};
        // NOTE: x, y access here
        const char *line = board[x];
//        std::cout << "PLM<" << x << "," << y << ">: " << line;
        line += std::max(0, y - ptr.len);
//        std::cout << "  shift >: " << line << std::endl;
        if (!*line)
            return 0;
        const char *match = line;
        do {
            match = strstr(match, ptr.str);
            if (match && match <= line + y) {
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

    friend std::ostream& operator<<(std::ostream& os, const Board& board) {
        os << board.move << std::endl;

        for (int row = BS - 1; row >= 0; --row)
        {
            for (int col = 0; col < BS; ++col)
            {
                switch (board.getToken(row, col)) {
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

#endif
