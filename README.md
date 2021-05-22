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

Minimax with alpha-beta pruning

https://medium.com/@LukeASalamone/creating-an-ai-for-gomoku-28a4c84c7a52
