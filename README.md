# pvd-sputter-control-project
Display Equipment Control Software Training by Display Academy [Date : 2026-06-29 ~ 2026-07-24]

### 1. 프로젝트 개요
EasyCluster Framework를 이용해 PVD sputtering 공정의 설비 제어 S/W를 구현하였습니다.

##### 1.1. 공정 개요
- PVD 공정이란
  - '물리적 기상 증착'이라고 불리는 공정으로 디스플레이에서 TFT를 만들 때 금속층을 형성하기 위한 방법 중 하나입니다.
  - 배선의 재료인 금속성 물질을 TFT 기판 위에 도포해 얇은 막을 형성하는 공정입니다. 
- Sputter
  - 수십~ 수백 eV 이온이 결합 에너지를 초과해 원자를 방출하게 되고, 표면 원자가 튀어나오는 것을 sputtering이라고 합니다.
  - 공정 순서
    1) 진공 형성  
    2) 아르곤(Ar) 주입
    3) 방전 방식 선택
    4) 프리 스퍼터링 (본 프로젝트에서는 구현하지 않았습니다.)
    5) 타깃 스퍼터링
    6) 배기(Vent) 동작 실행

##### 1.2. 주요 구현 기능
