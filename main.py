# pylint: disable=no-member
import os
import sys
import pygame
import logging
import json
from datetime import datetime
from typing import Tuple, Optional, List
from gomoku.game import Game
from gomoku.utils import isFreeTriple, Vec2, P1

logger = logging.getLogger(__name__)

WINDOW_HEIGHT = 800
WINDOW_WIDTH = 800 + 400

class Color:
    black = (0,0,0)
    white = (200, 200, 200)

class Button:
    LEFT_CLICK = 1
    MIDDLE_CLICK = 2
    RIGHT_CLICK = 3
    SCROLL_UP = 4
    SCROLL_DOWN = 5

class Controller(object):
    offsetY = 62
    offsetX = 64
    pSize = 37.5
    pHeigth = 37.5 / 2
    devMode = False
    EVERY_SEC_EVENT = pygame.USEREVENT+1
    redLight = False


    def __init__(self, screen,  game: Game):
        super()
        self.font30 = pygame.font.SysFont('Comic Sans MS', 30)
        self.font20 = pygame.font.SysFont('Comic Sans MS', 20)
        self.font10 = pygame.font.SysFont('Comic Sans MS', 10)
        self.screen = screen
        self.game = game
        self.bg = pygame.image.load(os.path.join(os.path.dirname(__file__), "images", "board.jpg"))
        self.aiPiece = pygame.image.load(os.path.join(os.path.dirname(__file__), "images", "blackPiece.png"))
        self.plPiece = pygame.image.load(os.path.join(os.path.dirname(__file__), "images", "whitePiece.png"))
        self.pointerImg = pygame.image.load(os.path.join(os.path.dirname(__file__), "images", "pointer.png"))
        self.panel = pygame.image.load(os.path.join(os.path.dirname(__file__), "images", "panel.jpg"))
        self.message = """       Hi there!
Gomoku will crush you!

Ctrl + Shift + d - enter dev mode

    - no auto moves from me :(
    - Shift+Click set Black piece
    - Ctrl+Click remove piece
    - Click set White piece

F1 - make AI move"""



    def draw(self, board: Optional[List[List[int]]] = None, highlight: Optional[List[Tuple[int, int, Tuple[int,int,int]]]] = None):
        """
        board - Draw custom board instead of game.board
        highlight - Highlight pieces at x, y and color of R G B  [(1, 1, (255, 255, 255)), (...), ...]
        """
        highlight = highlight or []
        board = board or self.game.board
        self.screen.fill(Color.black)
        self.screen.blit(self.bg, (0, 0))
        self.screen.blit(self.panel, (800, 0) )
        for y, row in enumerate(board):
            for x, v in enumerate(row):
                if v == 2:
                    self.screen.blit(self.aiPiece, self.coordToPos(x, y))
                elif v == 1:
                    self.screen.blit(self.plPiece, self.coordToPos(x, y))
        pointer = self.posToCoord(*pygame.mouse.get_pos())
        if pointer:
            self.screen.blit(self.pointerImg, self.coordToPos(*pointer))
        for x, y, color in highlight:
            pygame.draw.circle(self.screen, color, tuple(map(lambda v: v + self.pHeigth, self.coordToPos(x, y))), self.pHeigth, width=3)

        for i, l in enumerate(self.message.splitlines()):
            self.screen.blit(self.font30.render(l,  False, (0, 222, 0)), (800 + 37, 70 + 20 * i))

        self.screen.blit(self.font20.render(
            f"Dev mode is {'Active!' if self.devMode else 'Not Active'}",
            False, (0, 222, 0) if not self.devMode else (222, 0, 0)), (800 + 80, 750))
        self.screen.blit(self.font20.render(
            f"My last move took: {getattr(self.game, 'last_ai_time', 0)} ms!",
            False, (0, 222, 0)), (800 + 80, 700))

        if self.redLight:
            pygame.draw.circle(self.screen, (200, 0, 0), (1150, 750) ,self.pHeigth, width=0)

        pygame.display.update()

    def coordToPos(self, x, y) -> Tuple[int, int]:
        return int(x*self.pSize + self.offsetX - self.pHeigth), int(y*self.pSize + self.offsetY -self.pHeigth)

    def posToCoord(self, x, y) -> Optional[Tuple[int, int]]:
        x = x - self.offsetX + self.pHeigth
        y = y - self.offsetY + self.pHeigth
        if x <= 0 or y <= 0:
            return
        cx = int(x / self.pSize)
        cy = int(y / self.pSize)
        if cx > self.game.board_size - 1 or cy > self.game.board_size - 1:
            return
        return cx, cy

    def onUserEvent(self, type_):
        if type_ == self.EVERY_SEC_EVENT:
            self.redLight = not self.redLight

    def pieceUnderMouse(self):
        return self.posToCoord(*pygame.mouse.get_pos())


    def _debugIsFreeThree(self):
        h = []
        for y, row in enumerate(self.game.board):
            for x, v in enumerate(row):
                if v != 0:
                    continue
                for c, d in [
                    ((255, 0,   0), Vec2(1, 0)),   # _
                    ((0, 255,   0), Vec2(0, 1)),   # |
                    ((0,   0, 255), Vec2(1, 1)),   # \
                    ((0, 255, 255), Vec2(1, -1)), # /
                ]:
                    if self.game.is_valid_move(x, y, P1) and isFreeTriple(self.game.board, Vec2(x, y), d, P1):
                        h.append((x, y, c))
        self.draw(highlight=h)
        self.waitKey("c", "Showing possible Free Triple moves")

    def onKey(self, key: int):
        if key == pygame.K_s and pygame.key.get_mods() & pygame.KMOD_CTRL:
            json.dump(self.game.board, open(f'board_{datetime.now().strftime("%H-%M-%S")}.json', 'w'))
        if key == pygame.K_d and pygame.key.get_mods() & pygame.KMOD_CTRL and pygame.key.get_mods() & pygame.KMOD_SHIFT:
            logger.info("Dev mode switched!")
            self.devMode = not self.devMode
        if key == pygame.K_F1:
            self.game.next_move()
        if key == pygame.K_F12:
            self._debugIsFreeThree()

    def onMouseClick(self, button, x, y):
        logger.debug("onMouseClick: code: %s  %s %s", button, x, y)
        if button == Button.LEFT_CLICK:
            self.onMouseLeftClick(x, y)

    def onMouseLeftClick(self, x, y):
        logger.debug("onMouseLeftClick: %s %s", x, y)
        self.makeMove()

    def makeMove(self):
        p = self.pieceUnderMouse()
        if self.devMode and p:
            v = 1
            if pygame.key.get_mods() & pygame.KMOD_SHIFT:
                v = 2
            elif pygame.key.get_mods() & pygame.KMOD_CTRL:
                v = 0
            self.game.set(p[0], p[1], v)
            return


        if not p:
            logger.debug("Mouse not over valid position!")
            return False
        x, y = p
        if not self.game.is_valid_move(x, y, 1):
            logger.debug("Position at %s %s occupied or move not valid! Can't make move!", x, y)
            return False

        self.game.set(x, y, 1)
        self.game.next_move()
        return True

    def waitKey(self, key: str, message=None):
        # pylint: disable=no-member
        logger.info("Waiting key: %s message: %s", key, message)
        pygame.display.set_caption(f"Gomoku (waiting for key press: '{key}')")
        while True:
            for event in pygame.event.get():
                if event.type == pygame.KEYUP and pygame.key.name(event.key) == key:
                    logger.debug("Key %s arrived!", key)
                    pygame.display.set_caption("Gomoku")
                    return

                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()



def main():
    # pylint: disable=no-member
    pygame.init()
    pygame.font.init()
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("Gomoku")
    screen.fill(Color.black)
    game = Game()
    try:
        if len(sys.argv) > 1:
            game.board = json.load(open(sys.argv[1], 'r'))
    except:
        pass
    controller = Controller(screen, game)
    game.controller = controller
    # pygame.time.set_timer(controller.EVERY_SEC_EVENT, 4000)
    while True:
        try:
            controller.draw()
        except Exception:
            logger.exception("Draw failed")
        for event in pygame.event.get():
            logger.debug("pygame: event: %s", pygame.event.event_name(event.type))
            if event.type == pygame.QUIT:
                pygame.quit()
                return
            try:
                if event.type == pygame.MOUSEBUTTONUP:
                    controller.onMouseClick(event.button, *pygame.mouse.get_pos())
                elif event.type == pygame.KEYUP:
                    controller.onKey(event.key)
                elif event.type > pygame.USEREVENT:
                    controller.onUserEvent(event.type)
            except Exception:
                logger.exception("Failed to process event")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG if os.environ.get("DEBUG") else logging.INFO)
    logger = logging.getLogger(__name__)
    print("""Welcome to Gomoku!

Ctrl + Shift + d - enter dev mode

    - no auto moves for AI
    - Shift+Click set Black piece
    - Ctrl+Click remove piece
    - Click set White piece

F1     - make AI move

Ctrl+s - save current board to file to load it later via first argument
F12    - debug isFreeTriple
""")
    main()

