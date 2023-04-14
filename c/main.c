/**********************************************
  > File Name		: main.c
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 10 Oct 2021 11:03:43 AM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>
#include <pthread.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "debug.h"

static char** dyn_props[8] = {"moving", "stationary", "oncoming", "stationary candiate", "unknown", "crossing stationary", "crossing moving", "stopped"};

static float rms_arr[31] = {0.005, 0.006, 0.008, 0.011, 0.014, 0.018, 0.023, 0.029, 0.038, 0.049, 0.063, 0.081, 0.105, 0.135, 0.174, 0.224, 0.288, 0.371, 0.478, 0.616, 0.794, 1.023, 1.317, 1.697, 2.187, 2.817, 3.630, 4.676, 6.025, 7.762, 10.000};

static char** pdh0_arr[8] = {"invalid", "<25%", "<50%", "<75%", "<90%", "<99.9%", "<=100%"};

static char** ambig_state_arr[5] = {"invalid", "unclear", "crossing slope", "clear", "stationary"};

static uint32_t counter = 0;
static int near_counter = -1;
static int far_counter = -1;


//Radar config values
static uint16_t max_distance = 100; //resolution: 2m
static uint8_t output_type = 0; //0: null, 1: objectes, 2: clusters
static uint8_t radar_power = 0; //0: standard, 1: -3dB, 2: -6dB, 3: -9dB
static uint8_t send_quality = 1;
static uint8_t send_ext_info = 1;
static uint8_t sort_index = 1; //0: no sorting, 1: sort by range, 2: sort by RCS

static int s = -1; //socket fd


struct motion {
    float speed;
    uint8_t speed_direction;
    float yaw_rate;
};

static struct motion mo = {0, 0, 0};

void (*func)(void* args);

float parse_str_float(char* optarg) {
    float res = 0;
    int flag = 0, digit = 1;
    char c;
    while (*optarg != '\0') {
        c = *optarg;
        if (c - '0' < 10) {
            if (flag) {
                digit *= 10;
                res += ((c - '0')/digit);
            } else {
                res *= 10;
                res += (c - '0');
            }
        }
        else if (c == '.') {
            flag = 1;
        }
        else {
            LOG_ERR("Unkown char: %c", c);
            return 0;
        }
    }
    return res;
}

int parse_str_int(char* optarg) {
    int res = 0;
    char c;
    
    while (*optarg != '\0') {
        c = *optarg;
        if (c - '0' < 10) {
            res *= 10;
            res += (c - '0');
        }
        else {
            LOG_ERR("Unknown char: %c", c);
            return 0;
        }
    }
    return res;
}

/*
 * set bits in data from start to start-len as *val.
 */
void set_bits(uint8_t data[CAN_MAX_DLEN], int start, int len, void* p) {
    int index = start/8;
    int head = start % 8;
    uint64_t val = *((uint64_t*)p);
    data[index] |= (val & ((1<<(head+1)) - 1))<<(8-head);
    len -= head;
    val >>= head;

    while (len/8) {
        data[--index] |= val & 0xff;
        val >>= 8;
        len -= 8;
    }

    data[--index] |= val & ((1<<(len+1))-1);
}

void set_bit(uint8_t data[CAN_MAX_DLEN], int start) {
    int index = start / 8;
    data[index] |= 1<<((start%8)+1);
}

uint8_t get_bit(uint8_t data[CAN_MAX_DLEN], int start) {
    int index = start / 8;
    uint8_t slot = ((uint8_t)1)<<(8 - (start%8));
    return data[index]&slot;
}

/*
 * calculate a value start at any bit with any length 
 * from an 8 uint8_t array.
 */
float calc_float(uint8_t data[CAN_MAX_DLEN], int start, int len, float res) {
    int ret = 0;
    int head = start % 8;
    int index = start / 8;

    ret += data[index]>>(8-head);
    len -= head;

    while (len/8) {
        res += data[--index]<<head;
        head += 8;
        len -= 8;
    }

    res += (data[--index] & (1<<(len+1) - 1))<<head;
    return ret * res;
}

uint8_t calc_int8(uint8_t data[CAN_MAX_DLEN], int start, int len, uint8_t res) {
    uint8_t ret = 0;
    int head = start % 8;
    int index = start / 8;

    ret += data[index]>>(8-head);
    len -= head;

    while (len/8) {
        res += data[--index]<<head;
        head += 8;
        len -= 8;
    }

    res += (data[--index] & (1<<(len+1) - 1))<<head;
    return ret * res;
}

uint16_t calc_int16(uint8_t data[CAN_MAX_DLEN], int start, int len, uint8_t res) {
    uint16_t ret = 0;
    int head = start % 8;
    int index = start / 8;

    ret += data[index]>>(8-head);
    len -= head;

    while (len/8) {
        res += data[--index]<<head;
        head += 8;
        len -= 8;
    }

    res += (data[--index] & (1<<(len+1) - 1))<<head;
    return ret * res;
}

void radar_input(int s, uint32_t message_id, uint8_t dlc, uint8_t data[CAN_MAX_DLEN]) {
    struct can_frame cf;
    memset(&cf, 0, sizeof(cf));

    cf.can_id = (canid_t)message_id;
    cf.can_dlc = dlc;
    memcpy(cf.data, data, sizeof(data));

    int bytes, count = 5;
    while (count) {
        bytes = write(s, &cf, sizeof(cf));
        if (bytes == sizeof(cf)) {
            return;
        }
        count--;
    }
    CHECK_EXIT(0, "write error");
}

void radar_cfg_general() {
    uint8_t data[8] = {0};
    
    /*
     * valid bits selected:
     *  MaxDistance_valid
     *  RadarPower_valid
     *  OutputType_valid
     *  SendQuality_valid
     *  SendExtInfo_valid
     *  SortIndex_valid
     *  StoreInNVM_valid
     */
    data[0] |= 0xfd;

    //set MaxDistance 
    set_bits(data, 22, 10, (void*)&max_distance);

    //set radar power 
    set_bits(data, 37, 3, (void*)&radar_power);

    //set radar output type 
    set_bits(data, 35, 2, (void*)&output_type);

    //sort index
    set_bits(data, 44, 3, (void*)&sort_index);

    if (send_ext_info) {
        set_bit(data, 42);
    }

    radar_input(s, 0x0200, 8, data);
}


//s: socket fd, type: 0x0: clusters, 0x1: objects
void radar_cfg_filter() {
    uint8_t data[CAN_MAX_DLEN] = {0};
    uint16_t nof_obj = 100;
    uint16_t min_distance = 10; //resolution: 0.1, use global variable max_distance
    int16_t min_azimuth = -1800, max_azimuth = 1800;//with resolution of 0.025
    uint16_t min_vreloncome = 0, max_vreloncome = 1587;//resolution: 0.0315
    uint16_t min_vreldepart = 0, max_vreldepart = 1587;
    int16_t min_RCS = -500, max_RCS = 500;//resolution: 0.025
    uint16_t min_lifetime = 10, max_lifetime = 4095;//resolution: 0.1
    uint16_t min_size = 4, max_size = 160;//resolution: 0.025
    uint16_t min_probexists = 2, max_probexists = 7;//at least 50%

#define uint_count 7
    uint16_t uint_arr[uint_count][3] = {{nof_obj, 0, 0}, {min_distance, max_distance*10, 0x1}, 
    {min_vreloncome, max_vreloncome, 0x3}, {min_vreldepart, max_vreldepart, 0x4}, {min_lifetime, max_lifetime, 0x6},
    {min_size, max_size, 0x7}, {min_probexists, max_probexists, 0x8}};

#define int_count 2
    int16_t int_arr[int_count][3] = {{min_azimuth, max_azimuth, 0x2}, {min_RCS, max_RCS, 0x5}};
    
    for (int i = 0; i < uint_count; i++) {
        set_bits(data, 16, 12, (void*)&uint_arr[i][0]);
        set_bits(data, 32, 12, (void*)&uint_arr[i][1]);
        set_bit(data, 1);//FileterCfg_Valid
        set_bit(data, 2);//FilterCfg_Active
        if (output_type == 2) {
            set_bit(data, 7);//FilterCfg_Type
        }
        set_bits(data, 3, 4, (void*)&uint_arr[i][2]);
        radar_input(s, 0x202, 8, data);
        LOG_INFO("fileter config index: %d", uint_arr[i][2]);
        memset(data, 0, sizeof(data));
    }

    for (int i = 0; i < int_count; i++) {
        set_bits(data, 16, 12, (void*)&int_arr[i][0]);
        set_bits(data, 32, 12, (void*)&int_arr[i][1]);
        set_bit(data, 1);//FileterCfg_Valid
        set_bit(data, 2);//FilterCfg_Active
        if (output_type == 2) {
            set_bit(data, 7);//FilterCfg_Type
        }
        set_bits(data, 3, 4, (void*)&int_arr[i][2]);
        radar_input(s, 0x202, 8, data);
        LOG_INFO("filter config index: %d", int_arr[i][2]);
        memset(data, 0, sizeof(data));
    }
}

void motion_input() {
    uint8_t data[CAN_MAX_DLEN] = {0};
    uint16_t spd = (uint16_t)(mo.speed/0.02);
    set_bits(data, 8, 13, (void*)&spd);
    set_bits(data, 6, 2, (void*)&(mo.speed_direction));
    radar_input(s, 0x300, 2, data);

    memset(data, 0, sizeof(data));
    uint16_t yr = (uint16_t)(mo.yaw_rate/0.01);
    set_bits(data, 8, 16, (void*)&yr);
    radar_input(s, 0x301, 1, data);
}

void print_general(uint8_t id, float dist_long, float dist_lat, float vrel_long, float vrel_lat, char* dyn_prop, float RCS) {
}

void message_handler(struct can_frame cf) {
    switch (cf.can_id) {
        case 0x600://cluster_0_status
            uint8_t nof_clusters_near = cf.data[0];
            uint8_t nof_clusters_far = cf.data[1];
            printf("number of clusters near: %d\n", nof_clusters_near);
            printf("number of clusters far: %d\n", nof_clusters_far);
            break;
        case 0x701://cluster_1_general
            uint8_t id = cf.data[0];
            /*float dist_long = ((cf.data[2] & 0x1f)<<8 + cf.data[3]) * 0.2;*/
            /*float dist_lat = (cf.data[4]<<2 + (cf.data[2] & 0xa0)) * 0.2;*/
            /*float vrel_long = ((cf.data[5] & 0x03)<<8 + cf.data[6]) * 0.2;*/
            float dist_long = calc_float(cf.data, 19, 13, 0.2);
            float dist_lat = calc_float(cf.data, 24, 10, 0.2);
            float vrel_long = calc_float(cf.data, 46, 10, 0.25);

            char* dyn_prop = dyn_props[cf.data[6] & 0xa0];
            float vrel_lat = calc_float(cf.data, 53, 9, 0.25);
            float RCS = calc_float(cf.data, 56, 8, 0.5);
            break;
        case 0x702://cluster_2_quality
            id = cf.data[0];
            uint8_t distlong_rms = calc_int8(cf.data, 11, 5, 1);
            uint8_t vrellong_rms = calc_int8(cf.data, 17, 5, 1);
            uint8_t distlat_rms = calc_int8(cf.data, 22, 5, 1);
            uint8_t vrellat_rms = calc_int8(cf.data, 28, 5, 1);
            uint8_t pdh0 = calc_int8(cf.data, 24, 3, 1);
            uint8_t ambig_state = calc_int8(cf.data, 32, 3, 1);
            uint8_t invalid_state = calc_int8(cf.data, 35, 5, 1);

            break;
        case 0x201: //RadarState message
            uint16_t max_distance_cfg = calc_int16(cf.data, 22, 10, 2);
            if (max_distance_cfg != max_distance) {
                LOG_ERR("max_distance config error");
                radar_cfg_general();
                return ;
            }
            /*
             * errors includes 5 errors:
             *   persistent_error
             *   interference
             *   temperature_error
             *   temporary_error
             *   voltage_error
             */
            uint8_t errors = calc_int8(cf.data, 21, 5, 1);
            CHECK_EXIT(errors, "5 errors: %d", errors);

            uint8_t sort_index_cfg = calc_int8(cf.data, 36, 3, 1);
            if (sort_index_cfg != sort_index) {
                radar_cfg_general();
            }

            uint8_t radar_power_cfg = calc_int8(cf.data, 39, 3, 1);
            if (radar_power_cfg != radar_power) {
                radar_cfg_general();
            }

            uint8_t output_type_cfg = calc_int8(cf.data, 42, 2, 1);
            if (output_type_cfg != output_type) {
                radar_cfg_general();
            }

            uint8_t send_quality_cfg = get_bit(cf.data, 44);
            if (send_quality_cfg != send_quality) {
                radar_cfg_general();
            }

            uint8_t send_ext_info_cfg = get_bit(cf.data, 45);
            if (send_ext_info_cfg != send_ext_info) {
                radar_cfg_general();
            }

            uint8_t motion_rx_state = calc_int8(cf.data, 46, 2, 1);
            if (!motion_rx_state) {
                motion_input();
            }


        default:
            LOG_ERR("Unknown can_id %d", cf.can_id);
            break;
    }
}


void task() {
    radar_cfg_general();
    radar_cfg_filter();
}


int main(int argc, char* argv[]) {
    int res, opt;//socket fd and temp result
    struct sockaddr_can addr;
    struct ifreq ifr;//interface request struct
    struct can_frame cf;

    while ((opt = getopt(argc, argv, "s:d:y:o:")) != -1) {
        switch (opt) {
            case '0': break;
            case 's':
                mo.speed = parse_str_float(optarg);
            case 'd':
                mo.speed_direction = parse_str_int(optarg);
            case 'y':
                mo.yaw_rate = parse_str_float(optarg);
            case 'o':
                output_type = parse_str_int(optarg);
                CHECK_EXIT(output_type > 2, "Invalid output_type");
            case ':':
                return 0;
        }
    }


    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0");
    if (!ioctl(s, SIOCGIFINDEX, &ifr)) {
        CHECK_EXIT(0, "ioctl error");
    }

    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    res = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    CHECK_EXIT(res == 0, "bind error");


    

    int flag = 1;
    
    for(;;) {
        int bytes = read(s, &cf, CAN_MTU);
        switch (bytes) {
            case CAN_MTU:
                message_handler(cf);
                break;
            case CANFD_MTU:
                //make a handler for canfd_frame
                break;
            case -1:
                if (errno == EINTR) {//if beacause interupption
                    continue;
                }

                LOG_ERR("ERROR: %d", errno);
                break;
            default:
                LOG_INFO("received a bad frame: %d", bytes);
                continue;
        }
    }

    if (close(s) != 0) {
        LOG_ERR("close socket fd error");
    }

    return 0;
}

