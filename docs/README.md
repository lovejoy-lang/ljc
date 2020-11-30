# Viewing Documentation

This uses `doxygen` and `sphinx` with `breathe` and `exhale`.
First install Doxygen,

(e.g. Arch Linux)
```console
# pacman -S doxygen
```

Then install the the Python dependencies, by simply
```console
$ pip3 install -r requirements.txt
```

Henceforth, you may run
```console
$ ./local_server.sh
```
which will compile documentation as HTML, and start a small, local, Python
HTTP server (`python3 -m http.server`).
Follow the link given, and you'll be viewing the documentation locally.

