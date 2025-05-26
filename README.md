![Lumen OS](./.assets/Lumen.png)

---

*Open Source Hobby Operating System, What More Would You Want?*

**LumenOS** is an open-source, hobbyist operating system that I am making for fun.

## Features

- Designed for x86
- Easy to understand modular project structure
- Easy to build, run, and modify
- Open source

## Dependencies

Before building, install these:
- `gcc-multilib`
- `build-essential`
- `grub-pc-bin`
- `xorriso`
- `qemu-system-i386`
- `mtools`
- `gdb`

## Building LumenOS

To build the project, run:

```bash
make
```

# Running LumenOS

To build and run in QEMU, use:

```bash
make run
```

# Creating an ISO

To generate a bootable ISO image:

Building ISO
```bash
make iso
```
## Debugging with GDB

To launch the system with GDB for debugging:

Launch With GDB
```bash
make debug
```

## Contributing

Contributions are welcome!  
Feel free to fork the repository, open issues, or submit pull requests.  
If you’re new to OS development, this is a decent place to start learning, although I do recommend looking at the [OsDev Wiki](https://wiki.osdev.org) for more info. Although, a word of warning, my code is terrible and filled with bad practices. I am in no way a professional.

## License

LumenOS is released under the open-source GPL-3.0 license. See [LICENSE](LICENSE) for details.