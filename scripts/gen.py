#!/usr/bin/env python3

import argparse
import os
import re
import sys

header_dir = os.path.join(os.path.dirname(__file__), "..", "include/coax")
source_dir = os.path.join(os.path.dirname(__file__), "..", "lib")

headers = [
    os.path.join(header_dir, hdr)
    for hdr in os.listdir(header_dir)
    if os.path.basename(hdr) != "coax.h"
]

sources = [os.path.join(source_dir, src) for src in os.listdir(source_dir)]


headers_included = set()
std_headers_included_hdr = set()
std_headers_included_src = set()


def replace_prefix(code, ident):
    code = re.sub("cx_", ident.lower(), code)
    code = re.sub("CX_", ident.upper(), code)
    return code


def find_std_includes(code, as_hdrs):
    for match in re.finditer(r"#include\s+\<(?P<file>[^\>]+)\>", code):
        fn = match.group("file")
        if fn.startswith("coax"):
            continue
        # FIXME: this is a hack, fix it
        elif fn == "unistd.h" or fn == "windows.h":
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
            c = generate_header_code(f"{header_dir}/{inc}")
            code += c
        code += contents + "\n\n"
    return code


def generate_code(ident):
    ident = re.sub(r"_+$", "", ident) + "_"
    ident_up = ident.upper()

    code = ""
    code += f"#ifndef {ident_up}HEADER_INCLUDED\n"
    code += f"#define {ident_up}HEADER_INCLUDED 1\n"
    code += "\n"
    code += "%%std_headers_hdr%%\n"
    code += "\n"
    code += f"#define {ident_up}INSIDE_HEADER 1\n"
    code += "\n"

    hdr_code = ""
    for hdr in headers:
        hdr = f"{hdr}"
        hdr_code += generate_header_code(hdr)
    hdr_code = find_std_includes(hdr_code, True)
    code += hdr_code

    code += "\n"
    code += f"#endif // {ident_up}HEADER_INCLUDED\n"
    code += "\n"
    code += f"#if defined({ident_up}IMPLMENTATION) && !defined({ident_up}IMPLEMENTATION_INCLUDED)\n"
    code += "\n"
    code += f"#define {ident_up}IMPLEMENTATION_INCLUDED 1\n"
    code += "\n"
    code += "%%std_headers_src%%\n"
    code += "\n"

    src_code = ""
    for src in sources:
        src = f"{src}"
        with open(src, "r") as f:
            contents = f.read()
            src_code += contents.strip()
            src_code += "\n\n"
    src_code = find_std_includes(src_code, False)
    code += src_code

    code += "\n"
    code += f"#endif // {ident_up}IMPLEMENTATION\n"

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
    code = replace_prefix(code, ident)

    return code


def main(args):
    parser = argparse.ArgumentParser(
        description="generates combined single-file include for the coax library",
        epilog="written and maintained by Matt <m@cdbz.ca>",
    )

    parser.add_argument(
        "-n",
        "--ns",
        default="cx_",
        metavar="IDENT",
        help="use a custom namespace/prefix instead of 'cx_'",
    )

    parser.add_argument(
        "-o",
        "--out",
        default="-",
        metavar="FILE",
        help="file to write to (default stdout)",
    )

    args = parser.parse_args(args)

    code = generate_code(args.ns)

    if args.out == "-" or not args.out:
        sys.stdout.write(code)
    else:
        old_contents = None
        try:
            with open(args.out, "r") as infile:
                old_contents = infile.read()
        except:
            pass
        if old_contents is not None and code != old_contents:
            with open(args.out, "w") as outfile:
                outfile.write(code)

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
