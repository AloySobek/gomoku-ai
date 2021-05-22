#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : game.py
# Author            : Rustam Khafizov <super.rustamm@gmail.com>
# Date              : 22.05.2021 19:51
# Last Modified Date: 22.05.2021 20:19
# Last Modified By  : Rustam Khafizov <super.rustamm@gmail.com>

import sys

class Game:
    board_size = 19

    def __init__(self):
        self.board = [[0 for i in range(Game.board_size)] for j in range(Game.board_size)]

    def next_move(self):
        for y in range(Game.board_size):
            for x in range(Game.board_size):
                if self.board[y][x] == 0:
                    self.board[y][x] = 2
