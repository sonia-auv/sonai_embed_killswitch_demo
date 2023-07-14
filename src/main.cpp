
#include "pin_def.h"

#include "can_control.h"
#include "rs485_control.h"

#include "can_worker.h"
#include "rs485_worker.h"

int main(int argc, char const *argv[])
{
    DigitalIn *k_switch = new DigitalIn(KILL_SWITCH);
    DigitalIn *m_switch = new DigitalIn(MISSION_SWITCH);

    sonia_embed::CanControl *can_control = new sonia_embed::CanControl(CAN_HOCI, CAN_HICO, true);
    can_control->set_filter(8);
    

    CanWorker can_worker_thread(can_control, k_switch, m_switch);

    sonia_embed::RS485Control *rs_control = new sonia_embed::RS485Control(RS485_HOCI, RS485_HICO, RS485_BAUDRATE,
                                                                          RS485_RECEIVER_EN, RS485_TRANSMITER_EN,
                                                                          RS485_TERM_EN);

    rs_control->add_filter(13);
    RS485Worker rs_worker_thread(rs_control, k_switch, m_switch);

    Thread can_thread;
    Thread rs_thread;
    std::pair<size_t, size_t> pair_return;
    uint8_t data_in[112];
    while (true)
    {
        can_thread.start(callback(&can_worker_thread, &CanWorker::start));
        rs_thread.start(callback(&rs_worker_thread, &RS485Worker::start));

        can_thread.join();
        rs_thread.join();
    }

    return 0;
}
