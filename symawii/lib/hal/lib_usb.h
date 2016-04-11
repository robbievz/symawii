/* 
Copyright 2013 Brad Quick

Based one code Copyright (c) 2010, Peter Barrett  

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

** Brad: This file was pretty much hacked from the arduino core.  It will allow the
** usb port to be used for serial comunication. 
*/

#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

//	Standard requests
#define GET_STATUS			0
#define CLEAR_FEATURE		1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR		6
#define SET_DESCRIPTOR		7
#define GET_CONFIGURATION	8
#define SET_CONFIGURATION	9
#define GET_INTERFACE		10
#define SET_INTERFACE		11


// bmRequestType
#define REQUEST_HOSTTODEVICE	0x00
#define REQUEST_DEVICETOHOST	0x80
#define REQUEST_DIRECTION		0x80

#define REQUEST_STANDARD		0x00
#define REQUEST_CLASS			0x20
#define REQUEST_VENDOR			0x40
#define REQUEST_TYPE			0x60

#define REQUEST_DEVICE			0x00
#define REQUEST_INTERFACE		0x01
#define REQUEST_ENDPOINT		0x02
#define REQUEST_OTHER			0x03
#define REQUEST_RECIPIENT		0x03

#define REQUEST_DEVICETOHOST_CLASS_INTERFACE  (REQUEST_DEVICETOHOST + REQUEST_CLASS + REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE  (REQUEST_HOSTTODEVICE + REQUEST_CLASS + REQUEST_INTERFACE)

//	Class requests

#define CDC_SET_LINE_CODING			0x20
#define CDC_GET_LINE_CODING			0x21
#define CDC_SET_CONTROL_LINE_STATE	0x22

#define MSC_RESET					0xFF
#define MSC_GET_MAX_LUN				0xFE

#define HID_GET_REPORT				0x01
#define HID_GET_IDLE				0x02
#define HID_GET_PROTOCOL			0x03
#define HID_SET_REPORT				0x09
#define HID_SET_IDLE				0x0A
#define HID_SET_PROTOCOL			0x0B

//	Descriptors

#define USB_DEVICE_DESC_SIZE 18
#define USB_CONFIGUARTION_DESC_SIZE 9
#define USB_INTERFACE_DESC_SIZE 9
#define USB_ENDPOINT_DESC_SIZE 7

#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5

#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF

#define USB_CONFIG_POWERED_MASK                0x40
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20

// bMaxPower in Configuration Descriptor
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)

// bEndpointAddress in Endpoint Descriptor
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)                  ((addr) | 0x80)

#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03

#define TOBYTES(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)

#define CDC_V1_10                               0x0110
#define CDC_COMMUNICATION_INTERFACE_CLASS       0x02

#define CDC_CALL_MANAGEMENT                     0x01
#define CDC_ABSTRACT_CONTROL_MODEL              0x02
#define CDC_HEADER                              0x00
#define CDC_ABSTRACT_CONTROL_MANAGEMENT         0x02
#define CDC_UNION                               0x06
#define CDC_CS_INTERFACE                        0x24
#define CDC_CS_ENDPOINT                         0x25
#define CDC_DATA_INTERFACE_CLASS                0x0A

#define MSC_SUBCLASS_SCSI						0x06 
#define MSC_PROTOCOL_BULK_ONLY					0x50 

#define HID_HID_DESCRIPTOR_TYPE					0x21
#define HID_REPORT_DESCRIPTOR_TYPE				0x22
#define HID_PHYSICAL_DESCRIPTOR_TYPE			0x23


//	Device
typedef struct {
	u8 len;				// 18
	u8 dtype;			// 1 USB_DEVICE_DESCRIPTOR_TYPE
	u16 usbVersion;		// 0x200
	u8	deviceClass;
	u8	deviceSubClass;
	u8	deviceProtocol;
	u8	packetSize0;	// Packet 0
	u16	idVendor;
	u16	idProduct;
	u16	deviceVersion;	// 0x100
	u8	iManufacturer;
	u8	iProduct;
	u8	iSerialNumber;
	u8	bNumConfigurations;
} DeviceDescriptor;

//	Config
typedef struct {
	u8	len;			// 9
	u8	dtype;			// 2
	u16 clen;			// total length
	u8	numInterfaces;
	u8	config;
	u8	iconfig;
	u8	attributes;
	u8	maxPower;
} ConfigDescriptor;

//	String

//	Interface
typedef struct
{
	u8 len;		// 9
	u8 dtype;	// 4
	u8 number;
	u8 alternate;
	u8 numEndpoints;
	u8 interfaceClass;
	u8 interfaceSubClass;
	u8 protocol;
	u8 iInterface;
} InterfaceDescriptor;

//	Endpoint
typedef struct
{
	u8 len;		// 7
	u8 dtype;	// 5
	u8 addr;
	u8 attr;
	u16 packetSize;
	u8 interval;
} EndpointDescriptor;

// Interface Association Descriptor
// Used to bind 2 interfaces together in CDC compostite device
typedef struct
{
	u8 len;				// 8
	u8 dtype;			// 11
	u8 firstInterface;
	u8 interfaceCount;
	u8 functionClass;
	u8 funtionSubClass;
	u8 functionProtocol;
	u8 iInterface;
} IADDescriptor;

//	CDC CS interface descriptor
typedef struct
{
	u8 len;		// 5
	u8 dtype;	// 0x24
	u8 subtype;
	u8 d0;
	u8 d1;
} CDCCSInterfaceDescriptor;

typedef struct
{
	u8 len;		// 4
	u8 dtype;	// 0x24
	u8 subtype;
	u8 d0;
} CDCCSInterfaceDescriptor4;

typedef struct 
{
    u8	len;
    u8 	dtype;		// 0x24
    u8 	subtype;	// 1
    u8 	bmCapabilities;
    u8 	bDataInterface;
} CMFunctionalDescriptor;
	
typedef struct 
{
    u8	len;
    u8 	dtype;		// 0x24
    u8 	subtype;	// 1
    u8 	bmCapabilities;
} ACMFunctionalDescriptor;

typedef struct 
{
	//	IAD
	IADDescriptor				iad;	// Only needed on compound device

	//	Control
	InterfaceDescriptor			cif;	// 
	CDCCSInterfaceDescriptor	header;
	CMFunctionalDescriptor		callManagement;			// Call Management
	ACMFunctionalDescriptor		controlManagement;		// ACM
	CDCCSInterfaceDescriptor	functionalDescriptor;	// CDC_UNION
	EndpointDescriptor			cifin;

	//	Data
	InterfaceDescriptor			dif;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} CDCDescriptor;

typedef struct 
{
	InterfaceDescriptor			msc;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} MSCDescriptor;

typedef struct
{
	u8 len;			// 9
	u8 dtype;		// 0x21
	u8 addr;
	u8	versionL;	// 0x101
	u8	versionH;	// 0x101
	u8	country;
	u8	desctype;	// 0x22 report
	u8	descLenL;
	u8	descLenH;
} HIDDescDescriptor;

typedef struct 
{
	InterfaceDescriptor			hid;
	HIDDescDescriptor			desc;
	EndpointDescriptor			in;
} HIDDescriptor;


#define D_DEVICE(_class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs) \
	{ 18, 1, 0x200, _class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs }

#define D_CONFIG(_totalLength,_interfaces) \
	{ 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_BUS_POWERED, USB_CONFIG_POWER_MA(500) }

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
	{ 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
	{ 7, 5, _addr,_attr,_packetSize, _interval }

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
	{ 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }

#define D_HIDREPORT(_descriptorLength) \
	{ 9, 0x21, 0x1, 0x1, 0, 1, 0x22, _descriptorLength, 0 }

#define D_CDCCS(_subtype,_d0,_d1)	{ 5, 0x24, _subtype, _d0, _d1 }
#define D_CDCCS4(_subtype,_d0)		{ 4, 0x24, _subtype, _d0 }

#define USB_CDC_TX                 3
#define USB_CDC_RX                 2

#define TX_RX_LED_INIT	//DDRD |= (1<<5)
#define TXLED0			//PORTD |= (1<<5)
#define TXLED1			//PORTD &= ~(1<<5)
#define RXLED0			//PORTD |= (1<<5)
#define RXLED1			//PORTD &= ~(1<<5)

//#define TX_RX_LED_INIT	DDRD |= (1<<5), DDRB |= (1<<0)
//#define TXLED0			PORTD |= (1<<5)
//#define TXLED1			PORTD &= ~(1<<5)
//#define RXLED0			PORTB |= (1<<0)
//#define RXLED1			PORTB &= ~(1<<0)

typedef struct
{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint8_t wValueL;
	uint8_t wValueH;
	uint16_t wIndex;
	uint16_t wLength;
} Setup;

#define TRANSFER_PGM		0x80
#define TRANSFER_RELEASE	0x40
#define TRANSFER_ZERO		0x20

int USB_SendControl(uint8_t flags, const void* d, int len);
int USB_RecvControl(void* d, int len);

uint8_t	lib_usb_charsavailable(uint8_t ep);
int lib_usb_send(uint8_t ep, const void* data, int len);	// blocking
int lib_usb_recv(uint8_t ep, void* data, int len);		// non-blocking
int lib_usb_recv(uint8_t ep);							// non-blocking
void USB_Flush(uint8_t ep);
void lib_usb_init();

#define CDC_ENABLED

#ifdef CDC_ENABLED
#define CDC_INTERFACE_COUNT	2
#define CDC_ENPOINT_COUNT	3
#else
#define CDC_INTERFACE_COUNT	0
#define CDC_ENPOINT_COUNT	0
#endif

#define CDC_ACM_INTERFACE	0	// CDC ACM
#define CDC_DATA_INTERFACE	1	// CDC Data
#define CDC_FIRST_ENDPOINT	1
#define CDC_ENDPOINT_ACM	(CDC_FIRST_ENDPOINT)							// CDC First
#define CDC_ENDPOINT_OUT	(CDC_FIRST_ENDPOINT+1)
#define CDC_ENDPOINT_IN		(CDC_FIRST_ENDPOINT+2)

#define INTERFACE_COUNT		(MSC_INTERFACE + MSC_INTERFACE_COUNT)

#ifdef CDC_ENABLED
#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN
#endif

#define IMANUFACTURER	1
#define IPRODUCT		2
