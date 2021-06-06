from __future__ import annotations
from typing import List, NamedTuple, Union, Tuple, Optional

class Vec2(NamedTuple):
    x: int
    y: int

    def __add__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x + other, self.y + other)
        return Vec2(self.x + other.x, self.y + other.y)

    def __sub__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x - other, self.y - other)
        return Vec2(self.x - other.x, self.y - other.y)

    def __mul__(self, other: Union[Vec2,  int]) -> Vec2:
        if isinstance(other, int):
            return Vec2(self.x * other, self.y * other)
        return Vec2(self.x * other.x, self.y * other.y)

    def __str__(self):
        return f"({self.x}, {self.y})"

    def __repr__(self):
        return str(self)

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
    
    def in_first_quadrant_square(self, size: int):
        return self.x >= 0 and self.x < size and self.y >= 0 and self.y < size
