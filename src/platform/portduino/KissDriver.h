#pragma once

#include <RadioLib.h>
#include <stdio.h>
#include "Transport.h"
#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <atomic>
#include <deque>
#include <iostream>

class SoftwareDriver : public PhysicalLayer {
public:
    virtual ~SoftwareDriver() = default;
    //unused functions
    virtual int16_t transmit(const uint8_t* data, size_t len, uint8_t addr = 0) {
        (void)data; (void)len; (void)addr;
        return 0;
    }

    virtual int16_t sleep() {
        return 0;
    }

    virtual int16_t standby(uint8_t mode) {
        (void)mode;
        return 0;
    }

    virtual int16_t startReceive() {
        return 0;
    }

    virtual int16_t startReceive(uint32_t timeout,
                                RadioLibIrqFlags_t irqFlags = RADIOLIB_IRQ_RX_DEFAULT_FLAGS,
                                RadioLibIrqFlags_t irqMask  = RADIOLIB_IRQ_RX_DEFAULT_MASK,
                                size_t len = 0) {
    (void)timeout; (void)irqFlags; (void)irqMask; (void)len;
        return 0;
    }

    virtual int16_t receive(uint8_t* data, size_t len, RadioLibTime_t timeout = 0) {
        (void)data; (void)len; (void)timeout;
        return 0;
    }

    virtual int16_t finishTransmit() {
        return 0;
    }

    virtual int16_t finishReceive() {
        return 0;
    }

    virtual int16_t transmitDirect(uint32_t frf = 0) {
        (void)frf;
        return 0;
    }

    virtual int16_t receiveDirect() {
        return 0;
    }

    virtual int16_t setBitRate(float br) {
        (void)br;
        return 0;
    }

    virtual int16_t setFrequencyDeviation(float freqDev) {
        (void)freqDev;
        return 0;
    }

    virtual int16_t setDataShaping(uint8_t sh) {
        (void)sh;
        return 0;
    }

    virtual int16_t setEncoding(uint8_t encoding) {
        (void)encoding;
        return 0;
    }

    virtual int16_t invertIQ(bool enable) {
        (void)enable;
        return 0;
    }

    virtual int16_t checkOutputPower(int8_t power, int8_t* clipped) {
        (void)power;
        if (clipped) {
            *clipped = power;
        }
        return 0;
    }

    virtual int16_t setDataRate(DataRate_t dr, ModemType_t modem = RADIOLIB_MODEM_NONE) {
        (void)dr; (void)modem;
        return 0;
    }

    virtual int16_t checkDataRate(DataRate_t dr, ModemType_t modem = RADIOLIB_MODEM_NONE) {
        (void)dr; (void)modem;
        return 0;
    }

    virtual RadioLibTime_t calculateRxTimeout(RadioLibTime_t timeoutUs) {
        return timeoutUs;
    }

    virtual int16_t setIrqFlags(uint32_t irq) {
        (void)irq;
        return 0;
    }

    virtual int16_t clearIrqFlags(uint32_t irq) {
        (void)irq;
        return 0;
    }

    virtual int16_t startChannelScan() {
        return 0;
    }

    virtual int16_t startChannelScan(const ChannelScanConfig_t& config) {
        (void)config;
        return 0;
    }

    virtual int16_t getChannelScanResult() {
        return 0;
    }

    virtual int16_t scanChannel() {
        return 0;
    }

    virtual uint8_t randomByte() {
        return 0x00;
    }

    virtual void setDirectAction(void (*func)(void)) {
        (void)func;
    }

    virtual void readBit(uint32_t pin) {
        (void)pin;
    }

    virtual int16_t setDIOMapping(uint32_t pin, uint32_t value) {
        (void)pin; (void)value;
        return 0;
    }

    virtual void setPacketReceivedAction(void (*func)(void)) {
        (void)func;
    }

    virtual void clearPacketReceivedAction() {}

    virtual void setPacketSentAction(void (*func)(void)) {
        (void)func;
    }

    virtual void clearPacketSentAction() {}

    virtual void setChannelScanAction(void (*func)(void)) {
        (void)func;
    }

    virtual void clearChannelScanAction() {}

    virtual int16_t setModem(ModemType_t modem) {
        (void)modem;
        return 0;
    }

    virtual int16_t getModem(ModemType_t* modem) {
        if (modem) {
            *modem = RADIOLIB_MODEM_NONE;
        }
        return 0;
    }

    virtual int16_t stageMode(RadioModeType_t mode, RadioModeConfig_t* cfg) {
        (void)mode; (void)cfg;
        return 0;
    }

    virtual int16_t launchMode() {
        return 0;
    }

    virtual Module* getMod() {
        return nullptr;
    }
    
    virtual int16_t begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, uint8_t power, uint16_t preambleLength) = 0;
    virtual int16_t standby() = 0;
    virtual int16_t sleep(bool enable) = 0;
    virtual void setTransmitEnable(bool txon) = 0;
    virtual int16_t setSpreadingFactor(uint8_t sf) = 0;
    virtual int16_t setBandwidth(float bw) = 0;
    virtual int16_t setCodingRate(uint8_t cr) = 0;
    virtual int16_t setFrequency(float freq) = 0;
    virtual int16_t setPreambleLength(size_t length) = 0;
    virtual int16_t setSyncWord(uint8_t syncWord, uint8_t controlBits = 0x44) = 0;
    virtual int16_t setOutputPower(int8_t power) = 0;
    virtual int16_t setCRC(bool enable) = 0;
    virtual int16_t startReceiveDutyCycleAuto(uint16_t senderPreambleLength = 0, uint16_t minSymbols = 0, RadioLibIrqFlags_t irqFlags = RADIOLIB_IRQ_RX_DEFAULT_FLAGS, RadioLibIrqFlags_t irqMask = RADIOLIB_IRQ_RX_DEFAULT_MASK) = 0;
    virtual int16_t scanChannel(const ChannelScanConfig_t &cfg) = 0;
    virtual uint32_t getIrqFlags() = 0;
    virtual int16_t getPacketStatus() = 0;
    virtual float getSNR() = 0;
    virtual float getRSSI() = 0;
    virtual float getFrequencyError() = 0;
    virtual int16_t getLoRaRxHeaderInfo(uint8_t* cr, bool* hasCRC) = 0;
    virtual size_t getPacketLength(bool update = true) = 0;
    virtual RadioLibTime_t calculateTimeOnAir(ModemType_t modem, DataRate_t dr, PacketConfig_t pc, size_t len) = 0;
    virtual RadioLibTime_t getTimeOnAir(size_t len) = 0;
    virtual int16_t startTransmit(const uint8_t *data, size_t len, uint8_t addr = (uint8_t)0U) = 0;
    virtual int16_t readData(uint8_t *data, size_t len) = 0;
    virtual void clearDio1Action() = 0;
    virtual void setDio1Action(void (*func)(void) ) = 0;
};


enum class KISS : uint8_t {
    FEND            = 0xC0,
    FESC            = 0xDB,
    TFEND           = 0xDC,
    TFESC           = 0xDD,

    CMD_UNKNOWN     = 0xFE,
    CMD_DATA        = 0x00,
    CMD_FREQUENCY   = 0x01,
    CMD_BANDWIDTH   = 0x02,
    CMD_TXPOWER     = 0x03,
    CMD_SF          = 0x04,
    CMD_CR          = 0x05,
    CMD_RADIO_STATE = 0x06, 
    CMD_RADIO_LOCK  = 0x07,
    CMD_DETECT      = 0x08,  
    CMD_PROMISC     = 0x0E, 
    CMD_READY       = 0x0F,  
    CMD_STAT_RX     = 0x21,
    CMD_STAT_TX     = 0x22,
    CMD_STAT_RSSI   = 0x23,
    CMD_STAT_SNR    = 0x24,
    CMD_FREQ_ERR    = 0x25,
    CMD_BLINK       = 0x30,
    CMD_RANDOM      = 0x40,
    CMD_FW_VERSION  = 0x50,
    CMD_ROM_READ    = 0x51,
    CMD_PREAMBLE    = 0x52,

    DETECT_REQ      = 0x73,
    DETECT_RESP     = 0x46,

    RADIO_STATE_OFF = 0x00,
    RADIO_STATE_ON  = 0x01,
    RADIO_STATE_ASK = 0xFF,

};

enum RXT {
    IDLE,
    RECEIVE,
    TRANSMIT,
};

typedef struct{
    float   freq;
    float   bw;
    uint8_t sf;
    uint8_t cr;
    uint8_t power;
    uint8_t syncWord;
    uint16_t preambleLength;
} phy_config_t;

typedef struct{
    float snr;
    float rssi;
    float freq_error;
} metadata_t;

class KissDriver : public SoftwareDriver {
public:

    KissDriver(Transport* t) :  t(t), running(false), irqFlags(0) {
        
    }

    int16_t begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, uint8_t power, uint16_t preambleLength) override{
        t->begin(100);
        current_config = {
            .freq = freq,
            .bw = bw,
            .sf = sf,
            .cr = cr,
            .power = power,
            .syncWord = syncWord,
            .preambleLength = preambleLength
        };
        // init phy
        setFrequency(freq);
        setBandwidth(bw);
        setSpreadingFactor(sf);
        setCodingRate(cr);
        setPreambleLength(preambleLength);
        setSyncWord(syncWord);
        setOutputPower(power);

        running = true;
        read_thread = std::thread(&KissDriver::work, this);
        return RADIOLIB_ERR_NONE;
    }
    int16_t standby() override {
        tx_enabled = false;
        state = RXT::IDLE;
        rx_queue.clear();
        return RADIOLIB_ERR_NONE;
    }

    int16_t sleep(bool enable) override{
        return RADIOLIB_ERR_NONE;
    }

    void setTransmitEnable(bool txon) override{
        tx_enabled = txon;
    }

    int16_t setSpreadingFactor(uint8_t sf) override{
        printf("setSpreadingFactor:%d \n", sf);
        return command((uint8_t) KISS::CMD_SF, &sf, sizeof(sf));
    }

    int16_t setBandwidth(float bw) override{
        uint8_t data[4];
        memcpy(data, &bw, 4);
// #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//         printf("setBandwidth reverse bytes");
//         std::reverse(data, data + 4);
// #endif  
        printf("setBandwidth:%f \n", bw);
        return command((uint8_t) KISS::CMD_BANDWIDTH, data, sizeof(data));
    }

    int16_t setCodingRate(uint8_t cr) override{
        printf("setCodingRate:%d \n", cr);
        return command((uint8_t) KISS::CMD_CR, &cr, sizeof(cr));
    }

    int16_t setFrequency(float freq) override{
        uint8_t data[4];
        memcpy(data, &freq, 4);
// #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//         printf("setFrequency reverse bytes");
//         std::reverse(data, data + 4);
// #endif  
        printf("setFrequency:%f \n", freq);
        return command((uint8_t) KISS::CMD_FREQUENCY, data, sizeof(data));
    }

    int16_t setPreambleLength(size_t len) override {
        uint8_t data[2];
        memcpy(data, &len, 2);
// #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//         printf("setPreambleLength reverse bytes");
//         std::reverse(data, data + 2);
// #endif  
        printf("setFrequency:%d \n", len);
        return command((uint8_t) KISS::CMD_PREAMBLE, data, sizeof(data));
    }
    int16_t setSyncWord(uint8_t sync, uint8_t controlBits = 0x44) override{
        return RADIOLIB_ERR_NONE;
    }

    int16_t setOutputPower(int8_t power) override{
        uint8_t p = (uint8_t)power;
        return command((uint8_t) KISS::CMD_TXPOWER, &p, sizeof(power));
    }
    int16_t setCRC(bool enable) override{
        return RADIOLIB_ERR_NONE;
    }

    int16_t startReceiveDutyCycleAuto(uint16_t senderPreambleLength = 0, uint16_t minSymbols = 0, RadioLibIrqFlags_t irqFlags = RADIOLIB_IRQ_RX_DEFAULT_FLAGS, RadioLibIrqFlags_t irqMask = RADIOLIB_IRQ_RX_DEFAULT_MASK) override{
        return RADIOLIB_ERR_NONE;
    }
    int16_t scanChannel(const ChannelScanConfig_t &cfg) override{
        if (state == RXT::RECEIVE){
            return RADIOLIB_LORA_DETECTED;
        }
        return RADIOLIB_CHANNEL_FREE;
    } 
    uint32_t getIrqFlags() override{
        return irqFlags;
    }
    int16_t getPacketStatus() override{
        return RADIOLIB_ERR_NONE;
    }

    float getSNR() override {
        return metadata.snr;
    }
    float getRSSI() override {
        return metadata.rssi;
    }
    float getFrequencyError() override{
        return metadata.freq_error;
    }

    // data communication base class overrides
    int16_t getLoRaRxHeaderInfo(uint8_t* cr, bool* hasCRC) override{
// 0b001 (1) → 4/5
// 0b010 (2) → 4/6
// 0b011 (3) → 4/7
// 0b100 (4) → 4/8
        if(cr){ *cr = current_config.cr; }
        if(hasCRC){ *hasCRC = 1;}
        return RADIOLIB_ERR_NONE;
    }

    size_t  getPacketLength(bool update = true) override{
        return rx_queue.size();
    }

    RadioLibTime_t calculateTimeOnAir(ModemType_t modem, DataRate_t dr, PacketConfig_t pc, size_t len) override{
        uint32_t symbolLength_us = ((uint32_t)(1000 * 10) << dr.lora.spreadingFactor) / (dr.lora.bandwidth * 10) ;
        uint8_t sfCoeff1_x4 = 17; // (4.25 * 4)
        uint8_t sfCoeff2 = 8;
        if(dr.lora.spreadingFactor == 5 || dr.lora.spreadingFactor == 6) {
            sfCoeff1_x4 = 25; // 6.25 * 4
            sfCoeff2 = 0;
        }
        uint8_t sfDivisor = 4*dr.lora.spreadingFactor;
        if(pc.lora.ldrOptimize) {
            sfDivisor = 4*(dr.lora.spreadingFactor - 2);
        }
        const int8_t bitsPerCrc = 16;
        const int8_t N_symbol_header = pc.lora.implicitHeader ? 0 : 20;

        // numerator of equation in section 6.1.4 of SX1268 datasheet v1.1 (might not actually be bitcount, but it has len * 8)
        int16_t bitCount = (int16_t) 8 * len + pc.lora.crcEnabled * bitsPerCrc - 4 * dr.lora.spreadingFactor  + sfCoeff2 + N_symbol_header;
        if(bitCount < 0) {
            bitCount = 0;
        }
        // add (sfDivisor) - 1 to the numerator to give integer CEIL(...)
        uint16_t nPreCodedSymbols = (bitCount + (sfDivisor - 1)) / (sfDivisor);

        // preamble can be 65k, therefore nSymbol_x4 needs to be 32 bit
        uint32_t nSymbol_x4 = (pc.lora.preambleLength + 8) * 4 + sfCoeff1_x4 + nPreCodedSymbols * dr.lora.codingRate * 4;

        return((symbolLength_us * nSymbol_x4) / 4);
    }

    RadioLibTime_t getTimeOnAir(size_t len) override{
        DataRate_t dataRate = {};
        PacketConfig_t packetConfig = {};

        uint8_t _cr = current_config.cr;
        // map CR values 0-4 and 5-7 
        if (_cr < 5) {
            _cr = _cr + 4;
        } else if (_cr == 7) {
            _cr = _cr + 1;
        }
        
        dataRate.lora.codingRate = _cr;
        dataRate.lora.spreadingFactor = current_config.sf;
        dataRate.lora.bandwidth = current_config.bw;

        packetConfig.lora.preambleLength = current_config.preambleLength;
        packetConfig.lora.crcEnabled = true; // always on
        packetConfig.lora.implicitHeader = false; // always explicit
        // We use auto LDRO, meaning it is enabled if the symbol time is >= 16msec
        // ldrOptimize = ( (1 << sf) / bw ) >= 16
        packetConfig.lora.ldrOptimize = (1 << dataRate.lora.spreadingFactor) / dataRate.lora.bandwidth >= 16;
        return (calculateTimeOnAir((ModemType_t) NULL, dataRate, packetConfig, len));
    }
    int16_t startTransmit(const uint8_t *data, size_t len, uint8_t addr = (uint8_t)0U) override{
        if (tx_enabled == false) {
            return 1;
        }
        state = RXT::TRANSMIT;
        return command((uint8_t) KISS::CMD_DATA, data, len);
        
    }
    int16_t readData(uint8_t *data, size_t len) override{
        size_t length = getPacketLength();
        if ( (len != 0) && (len < length) ){
            length = len;
        }

        int16_t crcState = 0;
        if((irqFlags & RADIOLIB_SX126X_IRQ_CRC_ERR) || ((irqFlags & RADIOLIB_SX126X_IRQ_HEADER_ERR) && !(irqFlags & RADIOLIB_SX126X_IRQ_HEADER_VALID))) {
            crcState = RADIOLIB_ERR_CRC_MISMATCH;
        }

        for (size_t i = 0; i < length; i++){
            data[i] = rx_queue.front();
            rx_queue.pop_front();
        }

        // irq clear
        irqFlags = 0;
        
        RADIOLIB_ASSERT(crcState);
        return RADIOLIB_ERR_NONE;
    }

    
    void clearDio1Action() override {
        irqCallback = nullptr;
    }

    void setDio1Action(void (*func)(void) ) override {
        irqCallback = reinterpret_cast<volatile void (*)(void)>(func);
    }

private:
    std::vector<uint8_t> escape(const uint8_t* data, size_t len)
    {
        std::vector<uint8_t> result;
        result.reserve(len * 2); // worst case

        for (size_t i = 0; i < len; ++i) {
            uint8_t b = data[i];
            if (b == (uint8_t)KISS::FEND) {
                result.push_back((uint8_t)KISS::FESC);
                result.push_back((uint8_t)KISS::TFEND);
            } else if (b == (uint8_t)KISS::FESC) {
                result.push_back((uint8_t)KISS::FESC);
                result.push_back((uint8_t)KISS::TFESC);
            } else {
                result.push_back(b);
            }
        }
        return result;
    }
    std::vector<uint8_t> descape(const std::vector<uint8_t>& data){
        std::vector<uint8_t> result;
        result.reserve(data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            uint8_t b = data[i];
            if (b == (uint8_t)KISS::FESC) {
                i = i + 1;
                if (i >= data.size()) {
                    break;
                }
                b = data[i];
                if (b == (uint8_t)KISS::TFEND) {
                    result.push_back((uint8_t)KISS::FEND);
                } else if (b == (uint8_t)KISS::TFESC) {
                    result.push_back((uint8_t)KISS::FESC);
                } else {
                    result.push_back(b);
                }
            } else {
                result.push_back(b);
            }
        }
        return result;
        
    }
    
    int8_t command(uint8_t cmd, const uint8_t* payload, size_t payloadLen)
    {
        auto escaped = escape(payload, payloadLen);
        escaped.insert(escaped.begin(), cmd);
        escaped.insert(escaped.begin(),static_cast<uint8_t>(KISS::FEND));
        escaped.push_back(static_cast<uint8_t>(KISS::FEND));
        if (cmd == (uint8_t)KISS::CMD_DATA){
            size_t sent = t->send(escaped.data(), escaped.size());
            return sent == escaped.size() ? RADIOLIB_ERR_NONE : 1;
        }else{
            return update_config((KISS)cmd, payload, payloadLen);
        }
        
    }

    int16_t update_config(KISS cmd, const uint8_t* payload, size_t payloadLen){
        switch (cmd)
        {
        case KISS::CMD_FREQUENCY:
            memcpy(&current_config.freq,  payload, sizeof(float));
            break;
        case KISS::CMD_CR:
            memcpy(&current_config.cr,  payload, sizeof(uint8_t));
            break;
        case KISS::CMD_SF:
            memcpy(&current_config.sf,  payload, sizeof(uint8_t));
            break;
        case KISS::CMD_BANDWIDTH:
            memcpy(&current_config.bw,  payload, sizeof(float));
            break;
        case KISS::CMD_PREAMBLE:
            memcpy(&current_config.preambleLength,  payload, sizeof(uint16_t));
            break;
        case KISS::CMD_TXPOWER:
            memcpy(&current_config.power,  payload, sizeof(uint8_t));
            break;
        
        default:
            break;
        }
        return RADIOLIB_ERR_NONE;
    }

    void work(){
        uint8_t tmp[512];
#define MIN_PACKET_LEN 4
        while(running){
            size_t len = t->receive(tmp, sizeof(tmp));
            if (len == 0){
                continue;
            }
            buffer.insert(buffer.end(), tmp, tmp + len);
            while(buffer.size() >= MIN_PACKET_LEN){
                auto start = std::find(buffer.begin(), buffer.end(), (uint8_t) KISS::FEND);
            
                if (start == buffer.end()){
                    break;
                }
                auto end = std::find(start + 2, buffer.end(), (uint8_t) KISS::FEND);
                if (end == buffer.end()){
                    break;
                }
                std::vector<uint8_t> frame(start + 1, end);
                
                buffer.erase(buffer.begin(), end+1);
                
                handleFrame(frame);
                
            }
            usleep(2000);
        }
    }

    void handleFrame(std::vector<uint8_t>& frame){
        if (frame.empty()){
            return;
        }
        auto data = descape(frame);
        if (data.empty()){
            return;
        }

        switch ((KISS) data[0]) {
            case KISS::CMD_DATA:
                rx_queue.insert(rx_queue.end(), data.begin() + 1, data.end());
                printf("CMD DATA recvlen:%d \n", data.size()-1);
                break;
            case KISS::CMD_READY:
                
                irqFlags = data[1];
                state = RXT::IDLE;
                // TX/RX done
                if (irqCallback){
                    irqCallback();
                }
                printf("CMD READY\n");
                
                break;
            case KISS::CMD_DETECT:
                // RX begin
                state = RXT::RECEIVE;
                
                irqFlags = data[1];
                printf("CMD DETECT\n");
                break;
            case KISS::CMD_STAT_RSSI:
                metadata.rssi = *reinterpret_cast<const float*>(&data[1]);
                printf("CMD RSSI\n");
                break;
            case KISS::CMD_STAT_SNR:
                metadata.snr =  *reinterpret_cast<const float*>(&data[1]);
                printf("CMD SNR\n");
                break;
            case KISS::CMD_FREQ_ERR:
                metadata.freq_error =  *reinterpret_cast<const float*>(&data[1]);
                break;

            
            // case KISS::CMD_STAT_RX:
            //     break;
            // case KISS::CMD_STAT_TX:
            //     break;
            // default:
            //     break;
        }
    }
    phy_config_t current_config;
    std::deque<uint8_t> rx_queue;
    metadata_t metadata;

    RXT state;
    std::vector<uint8_t> buffer;
    Transport* t;
    std::thread read_thread;
    std::atomic<bool> running;
    std::atomic<bool> tx_enabled;
    std::atomic<uint16_t> irqFlags;

    volatile void (*irqCallback)();
};

