# ROMDUMP

TI-99/4A executable for ForceCommand.

Requires ForceCommand 2.0 or greater.

## Download

Zipfile in releases: [https://github.com/jedimatt42/fc_romdump/releases]

contains:

- FC_ROMDUMP.DSK : 90K floppy image with ROMDUMP executable
- ROMDUMP : TIFILES format executable
- README.md : this document

## Usage

ROMDUMP <output-filename>

For instance, you run it with the name of the output file as a parameter. This can be
a complete path, or just a relative filename for devices with directories.

- It will prompt for the CRUBASE to dump from.
- It will then scan upto 8K of ROM and estimate the actual size.
- Some cards do not fully decode the ROM, so it may think a 4K ROM is 8K.
- You can override this. Many 4A DSR ROMS are 4096 bytes big.
- It will then save the file.

Here is a transcript of running it.

```
[1100.DSK1.]
$ ROMDUMP CARDROM/BIN

Enter CRUBASE of ROM to dump: 1300
Detected ROM size 8192
Override: 4096
Saving ROM to DIS/FIX 128

[1100.DSK1.]
$ _
```

If no ROM header is seen when that CRUBASE enabled, it will fail instead of writing
an empty file.

Note, on a TIPI equiped machine, you can specify a file name as: TIPI.?X.DSRROM and
TIPI.DSRROM will be created as a raw binary with no TIFILES fiad headers on the 
native host.. so it is ready to copy to a PC and burn.  Otherwise you have to use
whatever other means you have of transferring the file, if that's what you want to do.

