# Metadata generator for firmware verification
This program will generate a binary file containing a metadata structure as
follows:

```c
typedef struct FW_metadata {
    uint8_t hash[SHA256_DIGEST_LENGTH]; /**< SHA256 Hash of firmware image */
    uint8_t shash[SIGN_LEN];            /**< Signed SHA256 */
    uint16_t version;                   /**< Integer representing firmware version */
    uint32_t size;                      /**< Size of firmware image */
    uint32_t appid;                     /**< Integer representing the application ID */
} FW_metadata_t;
```

This structure will be filled with the data obtained from the firmware.

## Usage
To use, you should call `generate-metadata` with the following arguments:

```console
./generate-metadata <firmware.bin> <version> <appid> <output-path>
```

Where:

_\<firmware.bin\>\:_ The firmware in binary format

_\<version\>\:_ The firmware version in 16-bit HEX

_\<appid\>_\: ID for the application in 32-bit HEX

_\<output-path\>_\: The path for fimrware_metadata.bin

The results will be printed if the operation is successful, and a binary
called "firmware-metadata.bin" will be created.
