#pragma once
#include "RadioSoftLibInterface.h"
#include "Transport.h"

#include "configuration.h"
#include "error.h"
#include "mesh/NodeDB.h"
#include "Throttle.h"
template <class T> class SoftwareInterface : public RadioSoftLibInterface {
    public:
        SoftwareInterface(Transport *transport);
        virtual bool init() override;
        virtual bool reconfigure() override;
        virtual bool sleep() override;

        bool isIRQPending() override { return 0; }

        void setTCXOVoltage(float voltage) { return; }

    protected:
    
        T lora;

        /**
         * Glue functions called from ISR land
         */
        virtual void disableInterrupt() override;
        virtual void enableInterrupt(void (*callback)()) { lora.setDio1Action(callback); }

        /** can we detect a LoRa preamble on the current channel? */
        virtual bool isChannelActive() override;

        /** are we actively receiving a packet (only called during receiving state) */
        virtual bool isActivelyReceiving() override;

        /**
         * Start waiting to receive a message
         */
        virtual void startReceive() override;

        /**
         *  We override to turn on transmitter power as needed.
         */
        virtual void configHardwareForSend() override;

        /**
         * Add SNR data to received messages
         */
        virtual void addReceiveMetadata(meshtastic_MeshPacket *mp) override;

        virtual void setStandby() override;
        uint32_t getPacketTime(uint32_t pl, bool received) override { return computePacketTime(lora, pl, received); }

    private:
        void setTransmitEnable(bool txon);
};


template <typename T> void SoftwareInterface<T>::disableInterrupt(){
    lora.clearDio1Action();
}

template <typename T> 
SoftwareInterface<T>::SoftwareInterface(Transport *transport) 
    : RadioSoftLibInterface(&lora), lora(transport) 
{

}



template <typename T> bool SoftwareInterface<T>::init()
{
    RadioSoftLibInterface::init();
    
    int res = lora.begin(getFreq(), bw, sf, cr, syncWord, power, preambleLength);
    LOG_INFO("KissInterface init result %d", res);
    
    res += lora.setCRC(true);
    LOG_INFO("KissInterface set CRC result %d", res);

    if (res != RADIOLIB_ERR_NONE) {
        return false;
    }
    LOG_INFO("Frequency set to %f", getFreq());
    LOG_INFO("Spreading factor set to %f", sf);
    LOG_INFO("Bandwidth set to %f", bw);
    LOG_INFO("Power output set to %d", power);

    startReceive();
    
    return res == RADIOLIB_ERR_NONE;
}

template <typename T> bool SoftwareInterface<T>::reconfigure()
{
    RadioSoftLibInterface::reconfigure();
    setStandby();

    int err = lora.setSpreadingFactor(sf);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    err += lora.setBandwidth(bw);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    err += lora.setCodingRate(cr);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    err += lora.setFrequency(getFreq());
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    err += lora.setPreambleLength(preambleLength);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);
    
    err += lora.setSyncWord(syncWord);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    err += lora.setOutputPower(power);
    if (err != RADIOLIB_ERR_NONE)
        RECORD_CRITICALERROR(meshtastic_CriticalErrorCode_INVALID_RADIO_SETTING);

    assert(err == RADIOLIB_ERR_NONE);

    startReceive();

    return RADIOLIB_ERR_NONE;
}



template <typename T> void SoftwareInterface<T>::setStandby(){
    checkNotification(); // handle any pending interrupts before we force standby
    int err = lora.standby();
    if (err != RADIOLIB_ERR_NONE)
        LOG_DEBUG("SoftwareInterface standby %s%d", radioLibErr, err);
    assert(err == RADIOLIB_ERR_NONE);

    isReceiving = false; // If we were receiving, not any more
    activeReceiveStart = 0;
    disableInterrupt();
    completeSending(); // If we were sending, not anymore
    RadioSoftLibInterface::setStandby();
}

template <typename T> void SoftwareInterface<T>::addReceiveMetadata(meshtastic_MeshPacket *mp){
    // LOG_DEBUG("PacketStatus %x", lora.getPacketStatus());
    mp->rx_snr = lora.getSNR();
    mp->rx_rssi = lround(lora.getRSSI());
    LOG_DEBUG("Corrected frequency offset: %f", lora.getFrequencyError());
}

template <typename T> void SoftwareInterface<T>::configHardwareForSend(){
    setTransmitEnable(true);
    RadioSoftLibInterface::configHardwareForSend();
}

template <typename T> void SoftwareInterface<T>::startReceive(){
    setTransmitEnable(false);
    setStandby();
 
    int err = lora.startReceiveDutyCycleAuto(preambleLength, 8, MESHTASTIC_RADIOLIB_IRQ_RX_FLAGS);
    if (err != RADIOLIB_ERR_NONE)
        LOG_ERROR("SoftwareInterface startReceiveDutyCycleAuto %s%d", radioLibErr, err);
    assert(err == RADIOLIB_ERR_NONE);
    RadioSoftLibInterface::startReceive();

    enableInterrupt(isrRxLevel0);
}

template <typename T> bool SoftwareInterface<T>::isChannelActive()
{
    // check if we can detect a LoRa preamble on the current channel
    ChannelScanConfig_t cfg = {.cad = {.symNum = NUM_SYM_CAD,
                                       .detPeak = RADIOLIB_SX126X_CAD_PARAM_DEFAULT,
                                       .detMin = RADIOLIB_SX126X_CAD_PARAM_DEFAULT,
                                       .exitMode = RADIOLIB_SX126X_CAD_PARAM_DEFAULT,
                                       .timeout = 0,
                                       .irqFlags = RADIOLIB_IRQ_CAD_DEFAULT_FLAGS,
                                       .irqMask = RADIOLIB_IRQ_CAD_DEFAULT_MASK}};
    int16_t result;
    setTransmitEnable(false);
    setStandby();
    result = lora.scanChannel(cfg);
    if (result == RADIOLIB_LORA_DETECTED)
        return true;
    if (result != RADIOLIB_CHANNEL_FREE)
        LOG_ERROR("SoftwareInterface scanChannel %s%d", radioLibErr, result);
    assert(result != RADIOLIB_ERR_WRONG_MODEM);

    return false;
}
template <typename T> bool SoftwareInterface<T>::isActivelyReceiving()
{
    // The IRQ status will be cleared when we start our read operation. Check if we've started a header, but haven't yet
    // received and handled the interrupt for reading the packet/handling errors.
    return receiveDetected(lora.getIrqFlags(), RADIOLIB_SX126X_IRQ_HEADER_VALID, RADIOLIB_SX126X_IRQ_PREAMBLE_DETECTED);
}

template <typename T> bool SoftwareInterface<T>::sleep(){
    setStandby();
    lora.sleep(true);    
}

template <typename T> void SoftwareInterface<T>::setTransmitEnable(bool txon)
{
    lora.setTransmitEnable(txon);
}