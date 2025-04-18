// ------------- Word Definitions ------------- //
#define STATE_UNSTABLE 0
#define STATE_STABLE 1

// ------------- Important Bytes ------------- //
#define END_BYTE 0b00001110


// ------------- Generic Command Definitions ------------- //
#define SET_EMG_STOP    0b00011111
#define SET_CMD_WEDGE   0b11011010
#define REQ_CMD_SHAPE   0b11010100
#define REQ_CMD_EDGS    0b11010100
#define REQ_CMD_ANGS    0b11010101
#define REQ_CMD_ORNT    0b11010110
#define REQ_CMD_WEDGE   0b11011000
#define REQ_CMD_NBR     0b11010111
#define SET_PARTY_HYPE  0b11010010
#define ITS_THE_POPO    0b11001111
#define RUN_COUP_WIGGLE 0b11010000
#define RUN_PIC_RESTART 0b11011111
#define RUN_PIC_IDLE    0b11000100
#define SET_CMD_DEADBND 0b11001011
#define SET_CMD_SPDGAIN 0b11001010
#define SET_CMD_OKRNGE  0b11001000

#define SET_CMD_TORQUE_ALLOC 0b11001100
#define SET_CMD_TORQUE_MASK 0b00000100

#define SET_CMD_PID_ALLOC 0b11001001
#define SET_CMD_PID_MASK 0b00000100

#define SET_CMD_VEL_ALLOC 0b11000001
#define SET_CMD_KAL_ALLOC 0b11000010


// ------------- Shape Command Definitions ------------- //
#define SHAPE_COMMAND_ALLOC 0b11001101

#define SHAPE_CMD_EDGS_ALLOC 0b00000000
#define SHAPE_CMD_ANGS_ALLOC 0b00000000
#define SHAPE_CMD_LEDS_ALLOC 0b10000000
#define SHAPE_CMD_SPDS_ALLOC 0b11000000

#define SHAPE_CMD_EDGS_MASK 0b00100000
#define SHAPE_CMD_ANGS_MASK 0b00000100
#define SHAPE_CMD_LEDS_MASK 0b00000100
#define SHAPE_CMD_SPDS_MASK 0b00100000

#define SHAPE_CMD_COUP_ALLOC 0b10000000
#define SHAPE_CMD_DRVE_ALLOC 0b01111100

#define SHAPE_CMD_COUP_MASK 0b00100000
#define SHAPE_CMD_DRVE_MASK 0b00000011

#define SHAPE_CMD_WEEL_ALLOC 0b01000000
#define SHAPE_CMD_WEEL_MASK  0b00000100


// ------------- Flag Byte Commands ------------- //
#define FLG_CMD_ENBL_ROTM 0b10000000
#define FLG_CMD_DSBL_ROTM 0b00000000

#define FLG_CMD_ENBL_LINM 0b10000001
#define FLG_CMD_DSBL_LINM 0b00000001

#define FLG_CMD_ENBL_FLG1 0b10000010
#define FLG_CMD_DSBL_FLG1 0b00000010

#define FLG_CMD_ENBL_FLG2 0b10000011
#define FLG_CMD_DSBL_FLG2 0b00000011

#define FLG_CMD_ENBL_FLG3 0b10000100
#define FLG_CMD_DSBL_FLG3 0b00000100

#define FLG_CMD_ENBL_FLG4 0b10000101
#define FLG_CMD_DSBL_FLG4 0b00000101

#define FLG_CMD_ENBL_FLG5 0b10000110
#define FLG_CMD_DSBL_FLG5 0b00000110


// ------------- Data Log Commands ------------- //
#define DLG_CMD_FLAG_SET 0b10100001
#define DLG_CMD_PERD_SET 0b10100010

#define REQ_CMD_ERROR    0b10100000
#define SET_CMD_REL_DIST 0b10100100