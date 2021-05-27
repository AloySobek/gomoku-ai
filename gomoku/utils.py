from __future__ import annotations
from typing import List, NamedTuple, Union
from enum import Enum


class Token(Enum):
    NONE = 0
    PLAYER1 = 1
    PLAYER2 = 2

class Vec2(NamedTuple):
    x: int
    y: int

    def __mul__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x * other, self.y * other)
        return Vec2(self.x * other.x, self.y * other.y)


    def __ne__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x - other, self.y - other)
        return Vec2(self.x - other.x, self.y - other.y)

    def __add__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x + other, self.y + other)
        return Vec2(self.x + other.x, self.y + other.y)

    def __repr__(self):
        return str(self)
    def __str__(self):
        return f"({self.x}, {self.y})"

    def move(self, dxy: Vec2, d: int) -> Vec2:
        """
        >>> Vec2(1, 1).move(Vec2(1, 0), 1)
        (2, 1)
        >>> Vec2(1, 1).move(Vec2(1, 0), -1)
        (0, 1)
        >>> Vec2(1, 1).move(Vec2(1, 1), 1)
        (2, 2)
        >>> Vec2(0, 0).move(Vec2(1, 1), -1)
        (-1, -1)
        """
        return self + dxy * d


def moveCoord(xy: Vec2, dxy: Vec2, d) -> Vec2:
    """
    >>> moveCoord(Vec2(1, 1), Vec2(1, 0), 1)
    (2, 1)
    >>> moveCoord(Vec2(1, 1), Vec2(1, 0), -1)
    (0, 1)
    >>> moveCoord(Vec2(1, 1), Vec2(1, 1), 1)
    (2, 2)
    >>> moveCoord(Vec2(0, 0), Vec2(1, 1), -1)
    (-1, -1)
    """
    return xy + dxy * d

def isFreeTriple(board: List[List[int]], xy: Vec2, dxy: Vec2, v: Token) -> bool:
    return True

if __name__ == "__main__":
    import doctest
    doctest.testmod()

