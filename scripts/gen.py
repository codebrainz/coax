#!/usr/bin/env python3

import re
import sys

headers = [
    "array.h",
    "dict.h",
    "filefuncs.h",
    "funcs.h",
    "list.h",
    "md5.h",
    "pair.h",
    "queue.h",
    "stack.h",
    "str.h",
    "strfuncs.h",
]

sources = [
    "array.c",
    "dict.c",
    "filefuncs.c",
    "list.c",
    "md5.c",
    "pair.c",
    "queue.c",
    "stack.c",
    "str.c",
    "strfuncs.c",
]

headers_included = set()
std_headers_included_hdr = set()
std_headers_included_src = set()

repl_lower = ("cx_", "m_")
repl_upper = ("CX_", "M_")


def replace_prefix(code):
    code = re.sub(repl_lower[0], repl_lower[1], code)
    code = re.sub(repl_upper[0], repl_upper[1], code)
    return code


def find_std_includes(code, as_hdrs):
    for match in re.finditer(r"#include\s+\<(?P<file>[^\>]+)\>", code):
        fn = match.group("file")
        if fn.startswith("coax"):
            continue
        if as_hdrs:
            std_headers_included_hdr.add(fn)
        else:
            std_headers_included_src.add(fn)
    return code


def find_includes(code):
    includes = []
    for match in re.finditer(r"#include\s+\<coax\/(?P<file>[^\>]+)\>", code):
        includes.append(match.group("file"))
    return includes


def generate_header_code(fn):
    code = ""
    if fn in headers_included:
        return code
    with open(fn, "r") as f:
        contents = f.read().strip()
        includes = find_includes(contents)
        headers_included.add(fn)
        for inc in includes:
            c = generate_header_code(f"include/coax/{inc}")
            code += c
        code += contents + "\n\n"
    return code


def generate_code():
    code = ""
    code += "#ifndef CX_H\n"
    code += "#define CX_H 1\n"
    code += "\n"
    code += "%%std_headers_hdr%%\n"
    code += "\n"

    hdr_code = ""
    for hdr in headers:
        hdr = f"include/coax/{hdr}"
        hdr_code += generate_header_code(hdr)
    hdr_code = find_std_includes(hdr_code, True)
    code += hdr_code

    code += "\n"
    code += "#endif // CX_H\n"
    code += "\n"
    code += "#ifdef CX_IMPLMENTATION\n"
    code += "\n"
    code += "%%std_headers_src%%\n"
    code += "\n"

    src_code = ""
    for src in sources:
        src = f"src/{src}"
        with open(src, "r") as f:
            contents = f.read()
            src_code += contents.strip()
            src_code += "\n\n"
    src_code = find_std_includes(src_code, False)
    code += src_code

    code += "\n"
    code += "#endif // CX_IMPLEMENTATION\n"

    code = re.sub(r"#include\s+\<coax\/(?P<file>[^\>]+)\>", "", code)
    code = re.sub(r"#include\s+\<(?P<file>[^\>]+)\>", "", code)

    hdr_includes = "\n"
    for hinc in sorted(list(std_headers_included_hdr)):
        hdr_includes += f"#include <{hinc}>\n"
    hdr_includes += "\n"
    code = re.sub(r"%%std_headers_hdr%%", hdr_includes, code)

    src_includes = "\n"
    for sinc in sorted(list(std_headers_included_src)):
        src_includes += f"#include <{sinc}>\n"
    src_includes += "\n"
    code = re.sub(r"%%std_headers_src%%", src_includes, code)

    code = re.sub(r"\n{2,}", "\n\n", code)
    code = replace_prefix(code)

    return code


def main(args):
    code = generate_code()
    sys.stdout.write(code)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
