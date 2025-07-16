# SimpleFS

SimpleFS is a file system which can do basic operations on files.

## ⚠️ WARNING

This file system is **experimental and under development**.
It is intended for **educational and testing purposes only**.

❗ Do **not** use it on physical drives or in any production environment.
If you choose to use it on real hardware, **you do so at your own risk**.

## 📦 Project Status

This project is in a **very early development stage**.
Currently, it supports only basic operations:

- creating disk image with shell command `create <path> <size in MB>`
- format disk with shell command `format <path>`
- mounting disk with shell command `mount <diskpath> <mountname>`
- select mounted disk with shell command `select [disk]`
- create new file with shell command `mkfile <path> [content]`

To run SimpleFS shell, just run `simplefs` in `build/` directory

Please note that the file system is not fully functional yet and is intended **for testing and experimentation purposes only**.

## To do list:
- [x] Implement basic FS header
- [x] Create bitmap allocation system
- [x] Add root directory
- [x] Implement file creation
- [x] SimpleFS shell
  - [x] `create`
  - [x] `format`
  - [x] `mount`
  - [x] `select`
  - [x] `mkfile`
  - [x] `cat`
  - [ ] `write`
  - [ ] `rm`
- [ ] Support subdirectories
- [ ] Write/Read file data