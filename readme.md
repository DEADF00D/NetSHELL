# NetSHELL

NetShell is a small network utility tool to manage remote shell connections from tools like netcat, or any tool which redirect shell `stdout` / `stdin` to tcp stream. It disable shell echoing, and redirect control keys to your remote shell. Allowing you to use `CTRL+C` in your shell, or fire up `ssh` or `less`. It use `python -c 'import pty; pty.spawn("/bin/bash")'` for shell spawn.

## Usage

Connect to a remote server (bind):
```
ns www.deadf00d.com 1337
```
Setup a server for remote connections (reverse)
```
ns -l 1337
```

## How to build ?

```
cd src/
make
```

## How to install ?

```
cp ns /usr/bin/
ns
```
