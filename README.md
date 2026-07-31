# pvd-sputter-control-project
[Education Date : 2026-06-29 ~ 2026-07-24]  
Display Equipment Control Software Training by Display Academy  

  
### 1. 프로젝트 개요
[Project Date: 2026-07-20 ~ 2026-07-24]  
EasyCluster Framework를 이용해 PVD sputtering 공정의 설비 제어 S/W를 구현하였습니다.  

#### 1.1. 공정 개요
- PVD 공정이란
  - '물리적 기상 증착'이라고 불리는 공정으로 디스플레이에서 TFT를 만들 때 금속층을 형성하기 위한 방법 중 하나입니다.
  - 배선의 재료인 금속성 물질을 TFT 기판 위에 도포해 얇은 막을 형성하는 공정입니다. 
- 스퍼터(Sputter)
  - 수십~ 수백 eV 이온이 결합 에너지를 초과해 원자를 방출하게 되고, 표면 원자가 튀어나오는 것을 sputtering이라고 합니다.
  - 공정 순서
    1) 진공 형성
       - 10^(-6)Torr 이하까지 배기를 진행합니다.
    2) 아르곤(Ar) 주입
    3) 방전 방식 선택
       - 금속 타깃의 경우 DC방전(도전성 우수, 안정적) -> 본 프로젝트에서 사용하였습니다.
       - 절연체나 산화물 타깃의 경우에는 RF방전(전하 축적 방지 필요)을 사용합니다.  
    4) 프리 스퍼터링 (본 프로젝트에서는 구현하지 않았습니다.)
    5) 타깃 스퍼터링
       - 아르곤 이온이 타깃 쪽으로 가속되어 충돌합니다. 
       - 이온이 가진 에너지가 타깃 원자의 결합 에너지보다 크면 타깃 표면에서 원자가 튕겨져 방출되게 됩니다.
    6) 배기(Vent) 동작 실행

#### 1.2. 주요 구현
- GUI
- IO Define
- Interlock/Alarm
- Sequence
- Manual
- HandShake
- Recipe

#### 1.3. 설비 구성도
![chamber](GitImage/Chamber.png)
- 프로젝트 진행 중 강사님께 제공받은 이미지 입니다.

### 2. GUI
### 3. IO Define
### 4. Interlock/Alarm
### 5. Sequence
### 6. Manual
### 7. HandShake
### 8. Recipe
### 9. 전체 실행 동작
### 9. EasyCluster 설치 및 프로그램 실행 방법
### 10. 사용 스택, 기술
