# Food recognition service for blind 
제 14회 공개 SW 개발자대회 출품작   
- website : https://www.oss.kr/
## 1) 프로젝트 개요
우리는 눈을 통해 많은 정보를 받아들이고, 그 정보를 통해 일상 속 많은 선택을 내린다. 그렇기 때문에 시각을
통해 정보를 받지 못하는 것은 생활하는 데에 필수적인 정보를 얻지 못하는 것과 같다. 시각 장애인의 경우 밥을
먹을 때 원하는 반찬을 선택하기가 어렵고 가보지 않은 식당에는 동행인이 없으면 가기 힘들다. 시각장애인을
위해 반찬을 구분해주는 이 시스템은 시각 장애인들이 식사에서 시야각 한계를 극복하고 더욱 다양한 음식을 즐길
수 있는 환경을 제공하고자 한다. 또한, 익숙하지 않은 환경에서 음식을 먹게 될 때 느낄 수 있는 불편함을 줄일 수
있음을 기대한다. 이 시스템은 train data로 학습된 모델과 입력되는 영상을 비교하여 현재 젓가락으로 가리키고 있는 음식이
어떤 음식인지 분류하고 그 음식의 종류를 음성으로 알려줌으로써 식탁에서의 내비게이션 역할을 하게 될 것 이다.


## 2) How to build
▶ 주 사용언어: python3   
- openCV, Tensorflow: 이미지에서 각각의 개별 음식 사진 도출 / 젓가락 포인터 Tracking   
- Teachable Machine: 주어진 음식 사진을 이름과 라벨링
- 클로바 더빙: 음성합성
## 3) Member 
정현성, 박민정, 배한빈, 조혜영

