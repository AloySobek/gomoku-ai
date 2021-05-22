Install PyPy

https://www.pypy.org/download.html

```
wget -O ~/pypy3.7-v7.3.4-linux64.tar.bz2 https://downloads.python.org/pypy/pypy3.7-v7.3.4-linux64.tar.bz2
tar -xf ~/pypy3.7-v7.3.4-linux64.tar.bz2 -C ~/

~/pypy3.7-v7.3.4-linux64/bin/pypy3 -m ensurepip
~/pypy3.7-v7.3.4-linux64/bin/pypy3 -m pip install virtualenv
~/pypy3.7-v7.3.4-linux64/bin/pypy3 -m virtualenv .venv
source ./.venv/bin/activate
pip install -r requirements.txt

pypy3 main.py
```

Hotkeys:

`Ctrl + Shift + d` - enter dev mode

    - no auto moves for AI 
    - Shift+Click set Black piece 
    - Ctrl+Click remove piece
    - Click set White piece

`F1` - make AI move

`Ctrl+s` - save current board to file to load it later via first argument

`export DEBUG=1` - for debug logging

Minimax with alpha-beta pruning

https://medium.com/@LukeASalamone/creating-an-ai-for-gomoku-28a4c84c7a52
