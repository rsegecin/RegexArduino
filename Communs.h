#ifndef _COMMUNS_H_
#define _COMMUNS_H_

#include  <avr/io.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef unsigned int size_t;
typedef uint8_t byte;

template<class T, void(T::*PTR)(), size_t I>
struct bind_member
{
	typedef void(*fn_type)();
	explicit bind_member(const T* _ptr)
	{
		ptr = _ptr;
	}
	static void func(void)
	{
		(ptr->*PTR)();
	}
	operator fn_type()
	{
		return &func;
	}
private:
	static const T*  ptr;
};

template<class T, void(T::*PTR)(), size_t I>
const T* bind_member<T, PTR, I>::ptr = NULL;

#ifndef NUL
#define	NUL		00 // Null char
#define	SOH		01 // Start of Heading
#define	STX		02 // Start of Text
#define	ETX		03 // End of Text
#define	EOT		04 // End of Transmission
#define	ENQ		05 // Enquiry
#define	ACK		06 // Acknowledgment
#define	BEL		07 // Bell
#define	BS		08 // Back Space
#define	HT		09 // Horizontal Tab
#define	LF		0A // Line Feed
#define	VT		0B // Vertical Tab
#define	FF		0C // Form Feed
#define	CR		0D // Carriage Return
#define	SO		0E // Shift Out / X-On
#define	SI		0F // Shift In / X-Off
#define	DLE		10 // Data Line Escape
#define	DC1		11 // Device Control 1 (oft. XON)
#define	DC2		12 // Device Control 2
#define	DC3		13 // Device Control 3 (oft. XOFF)
#define	DC4		14 // Device Control 4
#define	NAK		15 // Negative Acknowledgement
#define	SYN		16 // Synchronous Idle
#define	ETB		17 // End of Transmit Block
#define	CAN		18 // Cancel
#define	EM		19 // End of Medium
#define	SUB		1A // Substitute
#define	ESC		1B // Escape
#define	FS		1C // File Separator
#define	GS		1D // Group Separator
#define	RS		1E // Record Separator
#define	US		1F // Unit Separator
#endif

/* Define up the full complement of bit-twiddling macros */
#define BV(bit)					(1 << bit)
#define set_bit(sfr, bit)		(_SFR_BYTE(sfr) |= BV(bit))  // old sbi()
#define clear_bit(sfr, bit)		(_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()
#define set_output(sfr, bit)	set_bit(sfr, bit)
#define set_input(sfr, bit)		clear_bit(sfr, bit)

/* http://www.mail-archive.com/avr-libc-dev@nongnu.org/msg02454.html */
#define STRINGIFY(name) #name
#define CLASS_IRQ(name, vector) \
    static void name(void) asm(STRINGIFY(vector)) \
    __attribute__ ((signal, __INTR_ATTRS))

#endif