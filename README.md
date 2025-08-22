# xls2text

Simple command-line tool to dump **all text content** (numbers, strings, formula results) from an **Excel `.xls`** file into a plain text file.

> Only **BIFF8 (Excel 97-2003)** `.xls` is supported.  
> `.xlsx` is **not** supported.

---

## Features

- Cross-platform (Linux / macOS / Windows)  
- Uses [libxls](https://github.com/libxls/libxls) ≥ 1.5  
- Single executable (`xls2text`)  
- Tab-separated output, one row per line

---

## Requirements

| OS      | Package / Build from source |
|---------|-----------------------------|
| Ubuntu  | `sudo apt install libxls-dev` |
| macOS   | `brew install libxls`         |
| Others  | see **Build from source** below |

---

## Quick Start

### 1. Clone
```bash
git clone https://github.com/YOUR_GITHUB_USERNAME/xls2text.git
cd xls2text

Build:

  Linux/macOS

# Install build deps (example for Ubuntu)
sudo apt update
sudo apt install build-essential cmake libxls-dev

# Configure & compile
cmake -B build
cmake --build build

  Run
./build/xls2text input.xls output.txt