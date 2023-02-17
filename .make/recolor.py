import re


class c:
    red = "\033[31m"
    yellow = "\033[33m"
    blue = "\033[34m"
    purple = "\033[35m"
    bold = "\033[1m"
    reset = "\033[0m"


def colorized(color):
    def wrapper(match):
        word = match.group(0)
        return c.bold + color + word + c.reset

    return wrapper


while True:
    try:
        line = input()
    except EOFError:
        break
    else:
        if not re.search("/SPL(-STM8S...)?/", line, re.I):
            line = re.sub(r"[\w\d/-_]+(\.c|\.h)", colorized(c.bold), line, re.I)
            line = re.sub(r"[^:]+error", colorized(c.red), line, re.I)
            line = re.sub(r"[^:]+warning", colorized(c.yellow), line, re.I)
            print(line, end="")
