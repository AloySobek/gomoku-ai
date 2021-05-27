from __future__ import annotations
from typing import List, NamedTuple, Union
import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

EPT = 0
P1 = 1
P2 = 2

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


def _boardFromStr(s: str) -> List[List[int]]:
    res = []
    for i, l in enumerate(s.splitlines()):
        res.append([])
        for c in l:
            res[i].append(int(c, 10) if c != '.' else 0)
    return res


def isFreeTriple(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int) -> bool:
    """
    >>> isFreeTriple(_boardFromStr('''
    ... ......
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    True
    >>> isFreeTriple(_boardFromStr('''
    ... .11..
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    True
    >>> isFreeTriple(_boardFromStr('''
    ... .2....
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    False
    >>> isFreeTriple(_boardFromStr('''
    ... .11.2
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    False
    >>> isFreeTriple(_boardFromStr('''
    ... ......
    ... ...1..
    ... ...1..
    ... ......
    ... ......
    ... '''.strip()), Vec2(3, 3), Vec2(0, 1), P1)
    True
    >>> isFreeTriple(_boardFromStr('''
    ... ......
    ... ......
    ... ...1..
    ... ......
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    False
    >>> isFreeTriple(_boardFromStr('''
    ... ..1..
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    False
    >>> isFreeTriple(_boardFromStr('''
    ... 11...
    ... '''.strip()), Vec2(2, 0), Vec2(1, 0), P1)
    False
    """
    # eprint('\n'.join(map(lambda r: "".join(map(str, r)), board)))
    size = len(board[0]) - 1
    def _count(dd):
        res = 0
        cur = xy.move(dxy, dd)
        while 42:
            # ...TTT
            #       ^  Blocked!
            if cur.x < 0 or cur.y < 0 or cur.x > size or cur.y > size:
                # eprint("r: -19")
                return size * -1

            t = board[cur.y][cur.x]
            # eprint(f"{dd: 3} | {cur} -> {t: 2} = {res: 2}")

            # ...TTT.
            #       ^  Free!
            if t == EPT:
                break

            # ...TTTN
            #       ^  Blocked!
            if t != v:
                # eprint("r: -19")
                return size * -1

            # Count and move forward
            res += 1
            cur = cur.move(dxy, dd)
        # eprint(f"r: {res}")
        return res
    up = _count(1)
    if up < 0:
        return False
    return up + 1 + _count(-1) == 3

if __name__ == "__main__":
    import doctest
    doctest.testmod()

