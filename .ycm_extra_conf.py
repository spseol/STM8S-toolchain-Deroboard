def Settings(**kwargs):
    flags = (
        "-x c -Wall -Wextra -Werror -ferror-limit=5 "
        "-I./inc -I./SPL/inc -I./src -I./SPL/src"
    )
    return {
        "flags": flags.split(),
    }
