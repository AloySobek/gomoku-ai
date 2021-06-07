from gomoku.board import Board
from gomoku.board_analyzer import BoardAnalyzer

class Ai:
    def __init__(self):
        pass

    def minimax(self, depth: int, alpha: int, beta: int, board: Board, board_analyzer: BoardAnalyzer, is_black: bool):
        if depth == 0 or board_analyzer.five_in_a_row(board, is_black) != 0:
            return board_analyzer.eval_board(board, is_black)
        if is_black:
            max_h = float("-inf")
            for i in list(board_analyzer.possible_moves.keys()):
                if board.at(i) == 0:
                    board.place_stone(i, False)
                    board_analyzer.add_surroundings(board, i)
                    max_h = max(max_h, self.minimax(depth-1, alpha, beta, board, board_analyzer, False))
                    board_analyzer.remove_surroundings(board, i)
                    board.remove_stone(i, False)
                    alpha = max(alpha, max_h)
                    if max_h >= beta:
                        break
            return max_h
        else:
            min_h = float("inf")
            for i in list(board_analyzer.possible_moves.keys()):
                if board.at(i) == 0:
                    board.place_stone(i, True)
                    board_analyzer.add_surroundings(board, i)
                    min_h = min(min_h, self.minimax(depth-1, alpha, beta, board, board_analyzer, True))
                    board_analyzer.remove_surroundings(board, i)
                    board.remove_stone(i, True)
                    beta = min(beta, min_h)
                    if min_h <= alpha:
                        break
            return min_h
