#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define TRIG 6     //Trigger 신호 (출력 = PE6)
#define ECHO 7     //Echo 신호 (입력 = PE7)
#define SOUND_VELOCITY 340UL //소리 속도 (m/sec)

unsigned char digit[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char fnd[4];

void display_fnd(unsigned int distance);

int main(void) {

    unsigned int distance;     //거리 변수
    int i;
    
    DDRC = 0xff;    //FND 출력
    DDRG = 0x0f;    //FND 출력
    DDRB = 0x10;    //버저 출력
    DDRE = ((DDRE|(1<<TRIG)) & ~(1<<ECHO)); //TRIG = 출력 , ECHO = 입력 setting
    
    while (1) {
        TCCR1B = 0x03;     //Timer/Counter1 클록 4us(64분주)
        PORTE &= ~(1 << TRIG);     //Trig=LOW상태

        _delay_us(10);      //10us동안 유지

        PORTE |= (1 << TRIG);     //Trig=HIGH -> 거리 측정 명령 시작
        
        _delay_us(10);     //10us동안 유지

        PORTE &= ~(1 << TRIG);     //Trig=LOW -> 거리 측정 명령 끝

        while(!(PINE & (1 << ECHO)));     //Echo=HIGH가 될 때까지 대기

        TCNT1=0x0000;     //Timer/Counter1 값 초기화

        while(PINE & (1 << ECHO));     //Echo=LOW가 될 때까지 대기

        TCCR1B=0x00;     //Timer/Counter1 클록 정지(클록 입력 차단,CS11~CS10=000)

        distance = (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000);     //거리=속도x시간, 거리 단위는 1mm

        display_fnd(distance); //FND에 거리 출력

        if (distance < 300) {     //30cm 이내 장애물

            for (i = 0; i < 5; i++) {    //연속하여 "삐~" 지속

            PORTB=0x10;

            _delay_ms(1);

            PORTB=0x00;

            _delay_ms(1);

            }

        } else if (distance < 600) {    //60cm 이내 장애물

            for ( i = 0; i < 50; i++) {    //0.1초 동안 "삐~"

                PORTB=0x10;

                _delay_ms(1);

                PORTB=0x00;

                _delay_ms(1);

            }

            _delay_ms(100);     //0.1초 동안 묵음

        } else if (distance < 1000){      //1m 이내 장애물

            for (i = 0; i < 250; i++) {     //0.5초동안 "삐~"

            PORTB=0x10;

            _delay_ms(1);

            PORTB=0x00;

            _delay_ms(1);

            }

            _delay_ms(300);      //0.3초 동안 묵음

        } else;     //1m 이내 장애물 없을 시 버저 울리지 않음
    }
}

void display_fnd(unsigned int distance) {
    int i;
    fnd[3] = (distance / 1000) % 10;
    fnd[2] = (distance / 100) % 10;
    fnd[1] = (distance / 10) % 10;
    fnd[0] = (distance / 1) % 10;

    for (i = 0; i < 4; i++) {
        PORTC = digit[fnd[i]] | (i == 1 ? 0x80 : 0x00);
        PORTG = fnd_sel[i];

        _delay_ms(2);

        if (i % 2) {
            _delay_ms(1); 
        }
    }  
}