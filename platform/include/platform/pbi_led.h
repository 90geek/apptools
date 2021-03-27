#ifndef _PBI_LED_H_
#define _PBI_LED_H_

typedef enum pbi_led_status_s{
	LED_GREEN,
	LED_RED,
	LED_OFF,
}pbi_led_status_t;

void led_test(void);

void pbi_led_init(void);

void tuner_led(int led_id, pbi_led_status_t status);

void alarm_led(int led_id, pbi_led_status_t status);

#endif
