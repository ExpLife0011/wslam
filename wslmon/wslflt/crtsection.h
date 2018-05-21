#ifndef _CRT_SECTION_H_
#define _CRT_SECTION_H_

typedef void(__cdecl *PVFV)(void);

extern "C" {
    PVFV __crtXia[];
    PVFV __crtXiz[];

    PVFV __crtXca[];
    PVFV __crtXcz[];

    PVFV __crtXpa[];
    PVFV __crtXpz[];

    PVFV __crtXta[];
    PVFV __crtXtz[];
}

#endif // _CRT_SECTION_H_

