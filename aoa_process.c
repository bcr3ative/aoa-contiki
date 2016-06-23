#include <stdio.h>

#include "contiki.h"

#include "aoa_plug.h"

static struct etimer et;
static AoAPlug aoa;
static int angle;

PROCESS(aoa_process, "aoa");
AUTOSTART_PROCESSES(&aoa_process);

PROCESS_THREAD(aoa_process, ev, data) {
    PROCESS_BEGIN();
    AoAPlugInit(&aoa, 1);
    while(1) {
        etimer_set(&et, CLOCK_SECOND*3);
        PROCESS_WAIT_EVENT();
        if (etimer_expired(&et)) {
            set_threshold(&aoa);
            angle = get_aoa_int(&aoa);
            printf("Degrees %d\n", angle);
            etimer_reset(&et);
        }
    }
    PROCESS_END();
}