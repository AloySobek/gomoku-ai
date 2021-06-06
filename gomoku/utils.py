from __future__ import annotations
from typing import List, NamedTuple, Union, Tuple, Optional
import sys

from gomoku.vec2 import Vec2

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

EPT = 0
P1 = 1
P2 = 2
ALL_P = (P1, P2)

ALL_AXIS = (Vec2(1, 0), Vec2(0, 1), Vec2(1, 1), Vec2(1, -1))

def _boardFromStr(s: str) -> List[List[int]]:
    res = []
    for i, l in enumerate(s.splitlines()):
        res.append([])
        for c in l:
            res[i].append(int(c, 10) if c != '.' else 0)
    return res


def isDoubleThree(board: List[List[int]], xy: Vec2, v: int) -> bool:
    """
    >>> isDoubleThree(_boardFromStr('''
    ... ......
    ... ..0...
    ... ..11..
    ... ..1.1.
    ... ......
    ... '''.strip()), Vec2(2, 1), P1)
    True
    >>> isDoubleThree(_boardFromStr('''
    ... .........
    ... .1.......
    ... ..1......
    ... .........
    ... ....011..
    ... .........
    ... .........
    ... .........
    ... .........
    ... '''.strip()), Vec2(4, 4), P1)
    True
    """
    return sum(
        (1 for p in (getFreePlacing(board, xy, a, v) for a in ALL_AXIS) if p and p[0] and p[1] == 3)
    ) == 2



def _genPartialPatterns():
    base = {
        2: ("0xx0",),
        3: ("0xxx0", "0xx0x0", "0x0xx0"),
        4: ("0xxxx0", "0xxx0x0",),# "0xx0xx0", "0x0xxx0"),
    }
    _free_patterns = {}
    for k, v in base.items():
        for pl in ALL_P:
            _free_patterns[(k, pl)] = tuple(p.replace('x', f"{pl}") for p in v)
    for k, v in _free_patterns.items():
        _, pl = k
        if pl == P2:
            _free_patterns[k] = tuple(
                list(map(lambda s: str(P1) + s[1:], v)) +
                list(map(lambda s: s[:-1] + str(P1), v))
            )
        else:
            _free_patterns[k] = tuple(
                list(map(lambda s: str(P2) + s[1:], v)) +
                list(map(lambda s: s[:-1] + str(P2), v))
            )
        _free_patterns[k] = tuple(
            list(_free_patterns[k]) +
            list(map(lambda s: "[" + s[1:], _free_patterns[k])) +
            list(map(lambda s: s[:-1] + ']', _free_patterns[k]))
        )

    return _free_patterns


def _genFreePatterns():
    base = {
        2: ("0xx0",),
        3: ("0xxx0", "0xx0x0", "0x0xx0"),
        4: ("0xxxx0",),# "0xxx0x0", "0xx0xx0", "0x0xxx0"),
    }
    _free_patterns = {}
    for k, v in base.items():
        for pl in ALL_P:
            _free_patterns[(k, pl)] = tuple(p.replace('x', f"{pl}") for p in v)
    return _free_patterns

def _genCapturePatterns():
    return {
        P1: ("1221",),
        P2: ("2112",)
    }

def _getWinPatterns():
    return {
        P1: "11111",
        P2: "22222"
    }

_FREE_PATTERNS = _genFreePatterns()
_PARTIAL_PATTERNS = _genPartialPatterns()
_CAPTURE_PATTERNS = _genCapturePatterns()
_WIN_PATTERNS = _getWinPatterns()

def _getFlat(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int, extent: int) -> str:
    """Flattens forward diagonal / row / column / backward diagonal for xy

    1..
    .2.  for xy (1, 1) dxy (1, 1) and extent 2 forms string .121.     from forward diagonal
    ..1  for xy (2, 2) dxy (1, 1) and extent 3 forms string .121...   from forward diagonal
         for xy (1, 1) dxy (1, 0) and extent 4 forms string ....2.... from row

    @TODO: We can cache coordinates for each xy for each dxy in to the dict to optimize
    """
    def get(_xy):
        if (_xy.x < 0 or _xy.y < 0):
            raise IndexError
        return board[_xy.y][_xy.x]

    flat = f"{v}"
    for i in range(1, extent):
        try:
            flat = str(get(xy.move(dxy, -i))) + flat
        except IndexError:
            if flat[0] not in '[.':
                flat = "[" + flat
            else:
                flat = '.' + flat

        try:
            flat += str(get(xy.move(dxy, +i)))
        except IndexError:
            if flat[-1] not in '].':
                flat += ']'
            else:
                flat += '.'

    # eprint('b:', '\n'.join(map(lambda r: "".join(map(str, r)), board)))
    # eprint(flat)
    return flat

def isCapture(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int) -> bool:
    """Test if move xy for player v will capture any opponents tokens

                x012345..
               y_________
    given row  0|000221..|  and xy (3, 0) returns true
    because this move will form 001221.. placement which will capture 22
    resulting in 001001.. placement

    >>> isCapture(_boardFromStr('''
    ... ......1021......
    ... '''.strip()), Vec2(7, 0), Vec2(1, 0), P2)
    False
    >>> isCapture(_boardFromStr('''
    ... .0....
    ... ..2...
    ... ...2..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(1, 0), Vec2(1, 1), P1)
    True
    """
    flat = _getFlat(board, xy, dxy, v, 4)
    for p in _CAPTURE_PATTERNS[v]:
        if p in flat:
            # eprint("capture : ", p, "in", flat)
            return True
    return False

def underCapture(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int) -> bool:
    """Test if move xy is placing playes v token in capture position for opponent

    examples of capture player 2 tokens: ..1221.. -> ..1..1.. tow tokens captured

    >>> underCapture(_boardFromStr('''
    ... .1....
    ... ..0...
    ... ...2..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P2)
    True
    """
    flat = _getFlat(board, xy, dxy, v, 4)
    for p in _CAPTURE_PATTERNS[P1 if v == P2 else P2]:
        # eprint("under capture flat: ", p, "in", flat, p in flat)
        if p in flat:
            return True
    return False

def doCapture(board: List[List[int]], xy: Vec2, v: int) -> int:
    """Returns numer of captures performed on the board board for move xy for player v

    >>> doCapture(_boardFromStr('''
    ... .0....
    ... ..2...
    ... ...2..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(1, 0), P1)
    1
    """
    result = 0

    # eprint('before capture:', '\n'.join(map(lambda r: "".join(map(str, r)), board)))
    for d in ALL_AXIS:
        flat = _getFlat(board, xy, d, v, 4)
        # eprint("docapture: flat", flat[3:], flat[:-3])
        if flat[3:].startswith(_CAPTURE_PATTERNS[v][0]):
            for i in range(1, 3):
                coord = xy.move(d, i)
                board[coord.y][coord.x] = EPT
            result += 1
        if flat[:-3].startswith(_CAPTURE_PATTERNS[v][0]):
            for i in range(1, 3):
                coord = xy.move(d, -i)
                board[coord.y][coord.x] = EPT
            result += 1
    # eprint('after  capture:', '\n'.join(map(lambda r: "".join(map(str, r)), board)))
    return result

def getPartialPlacing(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int) -> Optional[Tuple[bool, int]]:
    """Returns longest semi-blocked placement of tokens for move xy for player v in direction dxy

    >>> getPartialPlacing(_boardFromStr('''
    ... ......
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    (False, 0)
    >>> getPartialPlacing(_boardFromStr('''
    ... .11..
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    (False, 0)
    >>> getPartialPlacing(_boardFromStr('''
    ... .2....
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    (True, 3)
    >>> getPartialPlacing(_boardFromStr('''
    ... .11.2
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    (True, 3)
    >>> getPartialPlacing(_boardFromStr('''
    ... 11...
    ... '''.strip()), Vec2(2, 0), Vec2(1, 0), P1)
    (True, 3)
    """
    flat = _getFlat(board, xy, dxy, v, 7)
    for i in range(4, 1, -1):
        for p in _PARTIAL_PATTERNS[(i, v)]:
            # NOTE: Slice flattened row to ensure we will not match unrelated tokens
            if p in flat[5-i:-(5-i)]:
                # eprint('P:', p, "in", flat, "==", p in flat)
                return True, i
    return False, 0

def getFreePlacing(board: List[List[int]], xy: Vec2, dxy: Vec2, v: int) -> Optional[Tuple[bool, int]]:
    """Returns longest non-blocked placement of tokens for move xy for player v in direction dxy

    >>> getFreePlacing(_boardFromStr('''
    ... ......
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    (True, 3)
    >>> getFreePlacing(_boardFromStr('''
    ... .11..
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    (True, 3)
    >>> getFreePlacing(_boardFromStr('''
    ... .2....
    ... ......
    ... ...1..
    ... ....1.
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    (False, 0)
    >>> getFreePlacing(_boardFromStr('''
    ... .11.2
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    (False, 0)
    >>> getFreePlacing(_boardFromStr('''
    ... ......
    ... ...1..
    ... ...1..
    ... ......
    ... ......
    ... '''.strip()), Vec2(3, 3), Vec2(0, 1), P1)
    (True, 3)
    >>> getFreePlacing(_boardFromStr('''
    ... ......
    ... ......
    ... ...1..
    ... ......
    ... ......
    ... '''.strip()), Vec2(2, 1), Vec2(1, 1), P1)
    (True, 2)
    >>> getFreePlacing(_boardFromStr('''
    ... ..1..
    ... '''.strip()), Vec2(3, 0), Vec2(1, 0), P1)
    (True, 2)
    >>> getFreePlacing(_boardFromStr('''
    ... 11...
    ... '''.strip()), Vec2(2, 0), Vec2(1, 0), P1)
    (False, 0)
    """
    flat = _getFlat(board, xy, dxy, v, 7)
    for i in range(4, 1, -1):
        for p in  _FREE_PATTERNS[(i, v)]:
            # NOTE: Slice flattened row to ensure we will not match unrelated tokens
            if p in flat[5-i:-(5-i)]:
                # eprint('F:', p, "in", flat, "==", p in flat)
                return True, i
    return False, 0

def isWinMove(board: List[List[int]], xy: Vec2, v: int) -> bool:
    """Check if move xy for player v will produce 5 connected tokens resulting in win

    >>> isWinMove(_boardFromStr('''
    ... 01111.
    ... '''.strip()), Vec2(0, 0), P1)
    True
    >>> isWinMove(_boardFromStr('''
    ... 11110.
    ... '''.strip()), Vec2(4, 0), P1)
    True
    >>> isWinMove(_boardFromStr('''
    ... 22220.
    ... '''.strip()), Vec2(4, 0), P2)
    True
    >>> isWinMove(_boardFromStr('''
    ... 22210.
    ... '''.strip()), Vec2(4, 0), P2)
    False
    """
    for d in ALL_AXIS:
        flat = _getFlat(board, xy, d, v, 6)
        if _WIN_PATTERNS[v] in flat:
            return True
    return False


if __name__ == "__main__":
    import doctest
    doctest.testmod()
