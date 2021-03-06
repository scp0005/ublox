// Novatel OEM4 Data Structures
#ifndef UBLOXSTRUCTURES_H
#define UBLOXSTRUCTURES_H

#include "stdint.h"

#define MAX_NOUT_SIZE      (5000)   // Maximum size of a NovAtel log buffer (ALMANACA logs are big!)

#define MAXCHAN		50  // Maximum number of signal channels
#define MAX_NUM_SAT 28	// Maximum number of satellites with information in the RTKDATA log
#define EPHEM_CHAN	33
#define MAXSAT 28

// define macro to pack structures correctly with both GCC and MSVC compilers
#ifdef _MSC_VER // using MSVC
	#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
	#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif


//! Header prepended to ubx binary messages
PACK(
struct UbloxHeader {
   uint8_t sync1;   //!< start of packet first byte (0xB5)
   uint8_t sync2;   //!< start of packet second byte (0x62)
   uint8_t message_class; //!< Class that defines basic subset of message (NAV, RXM, etc.)
   uint8_t message_id;		//!< Message ID
   uint16_t payload_length; //!< length of the payload data, excluding header and checksum
});


/*!
 * NAV-SOL Message Structure
 * This message combines Position, velocity and 
 * time solution in ECEF, including accuracy figures.
 * ID: 0x01  0x06  Length=52 bytes
 */
PACK(
struct NavSol{
    UbloxHeader header;
	uint32_t iTOW;
	int32_t fTOW;
	int16_t week;
	uint8_t gpsFix;
	int8_t flags;
	int32_t ecefX;
	int32_t ecefY;
	int32_t ecefZ;
	uint32_t pAcc;
	int32_t ecefVX;
	int32_t ecefVY;
	int32_t ecefVZ;
	uint32_t sAcc;
	uint16_t pDop;
	uint8_t reserved1;
	uint8_t numSV;
	uint32_t reserved2;
    uint8_t checksum[2];
});

/*!
 * NAV-POSLLH Message Structure
 * This message outputs the Geodetic position in 
 * the currently selected Ellipsoid. The default is
 * the WGS84 Ellipsoid, but can be changed with the 
 * message CFG-DAT.
 * ID: 0x01  0x02  Length=28 bytes
 */
PACK(
struct NavPosLLH{
    UbloxHeader header;		//!< Ublox header
    uint32_t iTOW;			//!< GPS millisecond time of week
	int32_t longitude_scaled; //!< longitude in degrees. Scaling 1e-7
	int32_t latitude_scaled; //!< latitude in degrees. Scaling 1e-7
	int32_t height;			 //!< height above ellipsoid [mm]
	int32_t height_mean_sea_level; //!< height above mean sea level [mm]
	uint32_t horizontal_accuracy; //!< horizontal accuracy estimate [mm]
	uint32_t vertical_accuracy;	//!< vertical accuracy estimate [mm]
    uint8_t checksum[2];
});

/*!
 * NAV-VELNED Message Structure
 * This message outputs the current 3D velocity
 * in a north-east-down frame.
 * ID: 0x01  0x12  Length=36 bytes
 */
PACK(
struct NavVelNed{
    UbloxHeader header;		//!< Ublox header
	uint32_t iTOW;
	int32_t velocity_north; //!< north velocity [cm/s]
	int32_t velocity_east; //!< east velocity [cm/s]
	int32_t velocity_down; //!< down velocity [cm/s]
	uint32_t speed; //!< 3D speed [cm/s]
	uint32_t ground_speed; //!< 2D (ground) speed [cm/s]
	int32_t heading_scaled; //!< heading [deg]. Scaling 1e-5
	uint32_t speed_accuracy; //!< speed accuracy estimate [cm/s]
	uint32_t heading_accuracy; //!< course/heading accuracy estimate [deg]. Scaling 1e-5
    uint8_t checksum[2];
});


///////////////////////////////////////////////////////////
// Configuration Messages
///////////////////////////////////////////////////////////
/*!
 * CFM-MSG Message Structure
 * This message requests a message at a given rate.
 * ID: 0x06  0x01 Length=3 bytes
 */
struct CfgMsg {
    UbloxHeader header;		//!< Ublox header
    uint8_t message_class;  //!< class of message to request
    uint8_t message_id;     //!< id of message to request
    uint8_t rate;           //!< rate message will be sent
    uint8_t checksum[2];
};

/*!
 * CFM-CFG Message Structure
 * This message clears, saves, or loads novalitle memory.
 * Set masks to 0x061F to clear, save, or load all values.
 * ID: 0x06  0x09 Length=12 bytes
 */
struct CfgCfg {
    UbloxHeader header;		//!< Ublox header
    uint32_t clear_mask;  //!< clear mask
    uint32_t save_mask;     //!< save mask
    uint32_t load_mask;           //!< load mask
    uint16_t checksum;      //!< Checksum
};

/*!
 * CFM-RST Message Structure
 * This message allows a receiver to be reset.
 * ID: 0x06  0x04 Length=4 bytes
 */
struct CfgRst {
    UbloxHeader header;		//!< Ublox header
    uint16_t nav_bbr_mask;  //!< Nav data to clear: 0x0000 = hot start, 0x0001 = warm start, 0xFFFF=cold start
    uint8_t  reset_mode;     //!< Reset mode
    uint8_t  reserved;       //!< reserved
    uint8_t checksum[2];
};


/*!
 * CFM-PRT Message Structure
 * This message configures a USART or USB port.
 * ID: 0x06  0x00 Length=20 bytes
 */
PACK(
struct CfgPrt {
    UbloxHeader header;		//!< Ublox header
    uint8_t port_id; //!< port identifier (0 or 1 for USART or 3 for USB)
    uint8_t reserved; //!< reserved
    uint16_t tx_ready; //!< transmit ready status
    uint32_t reserved2; //!< reserved
    uint32_t reserved3; //!< reserved
    uint16_t input_mask; //!< input protocol mask
    uint16_t output_mask; //!< output protocol mask
    uint16_t reserved4; //!< reserved
    uint16_t reserved5; //!< reserved
    uint8_t checksum[2];
});

/*!
 * AID-INI Message Structure
 * This message allows a receiver to be reset.
 * ID: 0x0B  0x01 Length=48 bytes
 */
struct AidIni {
    UbloxHeader header;		//!< Ublox header
    int32_t ecefXorLat;  //!< ECEF x position or latitude [cm or deg*1e-7]
    int32_t ecefYorLon;  //!< ECEF y position or longitude [cm or deg*1e-7]
    int32_t ecefZorAlt;  //!< ECEF z position or altitude [cm]
    uint32_t position_accuracy; //!< position accuracy - std dev [cm]
    uint16_t time_configuration; //!< time configuration bit misk
    uint16_t week_number; //!< actual week number
    uint32_t time_of_week; //!< actual time of week [ms]
    int32_t time_of_week_ns; //!< fractional part of time of week [ns]
    uint32_t time_accuracy_ms; //!< time accuracy [ms]
    uint32_t time_accuracy_ns; //!< time accuracy [ns]
    int32_t clock_drift_or_freq; //!< clock drift or frequency [ns/s or Hz*1e-2]
    uint32_t clock_drift_or_freq_accuracy; //!< clock drift or frequency accuracy [ns/s or ppb]
    uint32_t flags; //!< bit field that determines contents of other fields
    uint8_t checksum[2];
};

/*!
 * AID-EPH Message Structure
 * This message allows a receiver to be reset.
 * ID: 0x0B  0x31 Length=8 or 104 bytes
 */
struct AidEph {
    UbloxHeader header;		//!< Ublox header
    uint16_t nav_bbr_mask;  //!< Nav data to clear: 0x0000 = hot start, 0x0001 = warm start, 0xFFFF=cold start
    uint8_t  reset_mode;     //!< Reset mode
    uint8_t  reserved;       //!< reserved
    uint8_t checksum[2];
};

/*!
 * AID-ALM Message Structure
 * This message allows a receiver to be reset.
 * ID: 0x06  0x04 Length=4 bytes
 */
struct AidAlm {
    UbloxHeader header;		//!< Ublox header
    uint16_t nav_bbr_mask;  //!< Nav data to clear: 0x0000 = hot start, 0x0001 = warm start, 0xFFFF=cold start
    uint8_t  reset_mode;     //!< Reset mode
    uint8_t  reserved;       //!< reserved
    uint8_t checksum[2];
};

/*!
 * AID-HUI Message Structure
 * This message allows a receiver to be reset.
 * ID: 0x06  0x04 Length=4 bytes
 */
struct AidHui {
    UbloxHeader header;		//!< Ublox header
    uint16_t nav_bbr_mask;  //!< Nav data to clear: 0x0000 = hot start, 0x0001 = warm start, 0xFFFF=cold start
    uint8_t  reset_mode;     //!< Reset mode
    uint8_t  reserved;       //!< reserved
    uint8_t checksum[2];
};




struct gpsephemb_data {
    unsigned long prn; //PRN number
    double tow; //time stamp of subframe 0 (s)
    unsigned long health; //health status, defined in ICD-GPS-200
    unsigned long iode1; //issue of ephemeris data 1
    unsigned long iode2; //issue of ephemeris data 2
    unsigned long week; //GPS week number
    unsigned long zweek; //z count week number
    double toe; //reference time for ephemeris (s)
    double majaxis; //semi major axis (m)
    double dN; //Mean motion difference (rad/s)
    double anrtime; //mean anomoly reference time (rad)
    double ecc; //eccentricity
    double omega; //arguement of perigee (rad)
    double cuc; //arugument of latitude - cos (rad)
    double cus; //argument of latitude - sine (rad)
    double crc; //orbit radius - cos (rad)
    double crs; //orbit radius - sine (rad)
    double cic; //inclination - cos (rad)
    double cis; //inclination - sine (rad)
    double ia; //inclination angle (rad)
    double dia; //rate of inclination angle (rad/s)
    double wo; //right ascension (rad)
    double dwo; //rate of right ascension (rad/s)
    unsigned long iodc; //issue of data clock
    double toc; //SV clock correction term (s)
    double tgd; //estimated group delay difference
    double af0; //clock aiging parameter 0
    double af1; //clock aiging parameter 1
    double af2; //clock aiging parameter 2
//    yes_no spoof; //anti spoofing on
    double cmot; //corrected mean motion
    double ura; //user range accuracy variance
} ;

struct s_ubx{

	struct s_rxm_eph *rxm_eph;

};

struct s_rxm_eph_W{
	unsigned char bit[4];
};

struct s_rxm_eph_SF{
	struct s_rxm_eph_W W[8];
};

struct s_rxm_eph{
	
	unsigned short len;
	unsigned long svprn;
	unsigned long HOW;
	struct s_rxm_eph_SF SF[3];
	
};

struct range_data {

	double adr;
	double psr;
	float dop;
    uint8_t svprn; // PRN
    int8_t mesQI; // Nav measurements quality indicator -- (>=4 PR+DO OK) (>=5 PR+DO+CP OK) (<6 likel loss carrier lock)
    int8_t cno; // Signal/Noise
    uint8_t lock; // loss lock indicator 
    
} ;
struct range_log {
    
    int32_t iTow;
    int16_t week;
    uint8_t numSV;
    uint8_t reserved;
    range_data data[MAXCHAN];
} ;


enum Message_ID
{
	
	NAV_SOL = 262,
	RXM_EPH = 561,
	rangeID = 528,
	NAV_VELNED = 274,
	NAV_POSLLH = 258,
	MON_VER = 2564,

};

//typedef enum BINARY_LOG_TYPE BINARY_LOG_TYPE;

#endif
