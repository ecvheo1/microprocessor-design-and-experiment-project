#define F_CPU 16000000UL
#include <avr/io.h> // ATmega128의 레지스터 등이 정의되어 있음
#include <util/delay.h> // _delay_ms() 함수 등이 정의되어 있음

#include <stdio.h>

#define TRIG 6     //Trigger 신호 (출력 = PE6)
#define ECHO 7     //Echo 신호 (입력 = PE7)
#define SOUND_VELOCITY 340UL //소리 속도 (m/sec)

#include "lcd.h"

void display_lcd(unsigned int distance);
 
// C 언어의 주 실행 함수
int main(void){
    // 포트 A의 방향 설정, 0 : 입력, 1 : 출력
    DDRA = 0B11111111;
 
    // 포트 C의 방향 설정, 0 : 입력, 1 : 출력
    DDRC = 0B11111111;
 
    LCD_Init(); // 텍스트 LCD 초기화 - 함수 호출
    LCD_wBCommand(0x80 | 0x00); // DDRAM Address = 0 설정
    LCD_wString("HELLO"); // 텍스트 LCD 문자열 출력
 
    LCD_wBCommand(0x80 | 0x40);  // DDRAM Address = 0x40 설정
    // WESNET 문자열 출력
    LCD_wString("WORLD!");
   
    unsigned int distance; // 거리 변수
    int i;
    
    DDRB = 0x10; // 버저 출력
    DDRE = ((DDRE|(1<<TRIG)) & ~(1<<ECHO)); // TRIG = 출력 , ECHO = 입력 setting
    
    while (1) {
        TCCR1B = 0x03; // Timer/Counter1 클록 4us(64분주)
        PORTE &= ~(1 << TRIG); // Trig=LOW상태
        _delay_us(10); // 10us동안 유지

        PORTE |= (1 << TRIG); // Trig=HIGH -> 거리 측정 명령 시작
        _delay_us(10); // 10us동안 유지

        PORTE &= ~(1 << TRIG); // Trig=LOW -> 거리 측정 명령 끝

        while (!(PINE & (1 << ECHO))); // Echo=HIGH가 될 때까지 대기

        TCNT1 = 0x0000; // Timer/Counter1 값 초기화

        while (PINE & (1 << ECHO)); // Echo=LOW가 될 때까지 대기

        TCCR1B = 0x00; // Timer/Counter1 클록 정지(클록 입력 차단,CS11~CS10=000)

        distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000); // 거리=속도x시간, 거리 단위는 1mm

        display_lcd(distance);

        if (distance < 300) { //30cm 이내 장애물
            for (i = 0; i < 5; i++) { // 연속하여 "삐~" 지속
                PORTB=0x10;
                _delay_ms(1);

                PORTB=0x00;
                _delay_ms(1);
            }
        } else if (distance < 600) { // 60cm 이내 장애물
            for (i = 0; i < 50; i++) { // 0.1초 동안 "삐~"
                PORTB=0x10;
                _delay_ms(1);

                PORTB=0x00;
                _delay_ms(1);
            }
            _delay_ms(100); // 0.1초 동안 묵음
        } else if (distance < 1000) { // 1m 이내 장애물
            for (i = 0; i < 250; i++) { // 0.5초동안 "삐~"
                PORTB=0x10;
                _delay_ms(1);

                PORTB=0x00;
                _delay_ms(1);
            }
            _delay_ms(300); // 0.3초 동안 묵음
        } else; // 1m 이내 장애물 없을 시 버저 울리지 않음
    }
    
    return 0; // 함수의 형태와 같이 정수형(int)의 값을 반환함
}

void display_lcd(unsigned int distance) {
    unsigned int out = distance / 10;
    char s1[30];
    sprintf(s1, "%d", out);
    LCD_wBCommand(0x01);
    _delay_ms(1.64);

    LCD_wBCommand(0x80 | 0x00);  // DDRAM Address = 0 설정
    LCD_wString("DISTANCE IS");  // 텍스트 LCD 문자열 출력

    LCD_wBCommand(0x80 | 0x40);
    LCD_wString(s1); // 거리 출력
}