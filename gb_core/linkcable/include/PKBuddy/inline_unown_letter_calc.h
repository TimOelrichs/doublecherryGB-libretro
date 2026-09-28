

char calculate_Unown_Letter(unsigned char iv0, unsigned char iv1) {

    unsigned char atkBits = (iv0 & 0x60) << 1;
    unsigned char defBits = (iv0 & 0x06) << 3;
    unsigned char spdBits = (iv1 & 0x60) >> 3;
    unsigned char speBits = (iv1 & 0x06) >> 1;

    unsigned char combined = atkBits | defBits | spdBits | speBits;
    return 'A' + (combined / 10);
}

