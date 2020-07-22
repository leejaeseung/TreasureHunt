# Minesweeper

# C언어로 구현한 보물 찾기 게임입니다.

맵에 존재하는 여러 개의 깃발 중 단 하나의 보물 깃발을 찾는 게임입니다.

깃발은 다음과 같은 4 가지의 기능을 가집니다.

1. 보물 깃발을 포함한 3, 5, 7(난이도에 따라) 개의 깃발의 모양이 ★ 로 바뀝니다.(힌트 깃발)br
모양이 바뀐 뒤 다시 먹게 되면 플레이어의 현재 점수가 50%의 확률로 2배가 되거나, 1/2 배가 됩니다.
2. 플레이어를 처음 시작 위치로 옮깁니다.(남은 이동 횟수는 그대로)
3. 현재 남아있는 깃발의 위치, 장애물의 위치가 전부 바뀌고, 플레이어는 처음 시작 위치로 돌아갑니다.(남은 이동 횟수는 그대로, 힌트 깃발의 효과도 초기화됩니다.)
4. 보물 깃발로, 깃발 중 단 하나만 존재하며 먹을 시 500점이 추가되고 다음 난이도로 진행합니다.

모든 입력은 'R' or 'r' , 'Q' or 'q' , 방향키(↑,↓,←,→), Enter 키만 가능합니다. 

- 'R' or 'r' 입력 : 게임 진행 시에만 입력 가능, 맵을 초기화 시킵니다.
- 'Q' or 'q' 입력
 - 메인 화면에서 입력 시 : 프로그램을 종료합니다.
 - 게임 진행 시 : 메인 화면으로 돌아갑니다.
- 방향키(↑,↓,←,→) 입력 : 난이도를 선택하거나 플레이어 아이콘을 움직입니다.
- Enter키 입력 : 난이도를 선택합니다.

메인 화면에서 Easy, Normal, Hard 세 가지의 난이도를 고를 수 있습니다.

![메인 화면](https://user-images.githubusercontent.com/23518329/88159356-280eed80-cc48-11ea-8fd9-7b46fd8ca029.PNG)

난이도를 선택하면 보물 찾기 게임이 진행되고, 각 난이도마다 깃발의 개수, 장애물의 개수, 장애물의 길이 등이 다르게 조정됩니다.

<img src="https://user-images.githubusercontent.com/23518329/88159672-9358bf80-cc48-11ea-80e1-8be91930a3d3.png"  width="325" height="400"><img src="https://user-images.githubusercontent.com/23518329/88159719-a8355300-cc48-11ea-8b5b-69374d2646bb.png"  width="325" height="400"><img src="https://user-images.githubusercontent.com/23518329/88159908-ea5e9480-cc48-11ea-9b92-9ca0a737e781.png"  width="325" height="400">

