#pragma once
#include "RS485_control.h"
#include "rs485_toolkit.h"

class RS485Worker
{
public:
    RS485Worker(sonia_embed::RS485Control *rs485_control, DigitalIn *kill_switch, DigitalIn *mission_switch)
        : m_rs_control(rs485_control), m_kill_switch(kill_switch), m_mission_switch(mission_switch) {};

    void start()
    {
        uint8_t data_in[sonia_embed_toolkit::RS485Toolkit::MAX_MSG_SIZE];
        std::pair<size_t, size_t> pair_return;
        while (true)
        {
            pair_return = m_rs_control->receive(data_in);
            if (pair_return.first != sonia_embed::RETURN_NO_MSG && pair_return.first != sonia_embed::RETURN_NO_START_BYTE && pair_return.first != sonia_embed::RETURN_NOT_FOR_ME)
            {
                uint8_t send_data[1] = {m_kill_switch->read() << 1 | m_mission_switch->read()};
                m_rs_control->transmit(0, send_data, 1);
            }
            ThisThread::yield();
        }
    }

private:
    sonia_embed::RS485Control *m_rs_control;
    DigitalIn *m_kill_switch;
    DigitalIn *m_mission_switch;
};