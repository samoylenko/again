extern "C" void Blur( void *pSrc, void *pDst );
extern "C" void PutSprite( void *spr, ULONG sprx, ULONG spry, void *dst, ULONG dstpitch, ULONG x, ULONG y );
extern "C" void IndexedToRGB( void *src8, void *dst32, void *pal );