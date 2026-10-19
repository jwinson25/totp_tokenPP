// Copy this file to secret.h (git-ignored) and fill in your own secret.
// Bytes = your Base32 secret decoded to raw bytes.
// python3 -c "import base64;print(', '.join('0x%02X'%x for x in base64.b32decode('YOURBASE32')))"
uint8_t hmacKey[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
