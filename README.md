# microprocessor-design-and-experiment-project

## 주제
* 차량 후방 감지기
* 저속 주행 및 후진 시에 차량과 장애물 사이의 거리를 실시간으로 감시하는 차량 후방 감지기
* ATMEGA128, HC-SR04, 1602LCD, 가변 저항을 활용한 차량 후방 감지기

## 기능
* 장애물과 차량 간의 거리별로 구별된 경고음 알림 
  * 거리가 1m 이내면 0.5초간 “삐~” 경고음 -> 0.3초간 묵음 반복
  * 거리가 60cm 이내면 0.1초간 “삐~” 경고음 -> 0.1초간 묵음 반복
  * 거리가 30cm 이내면 연속적으로 “삐~” 경고음 지속
* 장애물과 차량 간의 거리를 LCD에 디스플레이 표시

## 결과 이미지
<img width="452" alt="image" src="https://user-images.githubusercontent.com/78195316/209083097-63c12e2e-978e-41b6-97b3-dcf6f5b461f3.png">
