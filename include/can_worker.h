#pragma once
#include "can_control.h"
#include "canbus_toolkit.h"
class CanWorker
{
public:
    CanWorker(sonia_embed::CanControl *can_control, DigitalIn *kill_switch, DigitalIn *mission_switch)
        : m_can_control(can_control), m_kill_switch(kill_switch), m_mission_switch(mission_switch) {};

    void start()
    {
        uint8_t data_in[sonia_embed_toolkit::CanBusToolkit::MAX_MSG_SIZE];
        std::pair<size_t, size_t> pair_return;
        while (true)
        {
            pair_return = m_can_control->receive(data_in);
            if (pair_return.first != sonia_embed::RETURN_NO_MSG && pair_return.first != sonia_embed::RETURN_NOT_FOR_ME)
            {
                uint8_t send_data[1] = {m_kill_switch->read() << 1 | m_mission_switch->read()};
                m_can_control->transmit(0, send_data, 1);
            }
            ThisThread::yield();
        }
        
    }

private:
    sonia_embed::CanControl *m_can_control;
    DigitalIn *m_kill_switch;
    DigitalIn *m_mission_switch;
};