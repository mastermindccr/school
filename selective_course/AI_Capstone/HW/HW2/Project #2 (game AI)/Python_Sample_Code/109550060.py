import STcpClient_1 as STcpClient
import pickle
import os
import random as rd
import math

'''
    input position (x,y) and direction
    output next node position on this direction
'''
def Next_Node(pos_x,pos_y,direction):
    if pos_y%2==1:
        if direction==1:
            return pos_x,pos_y-1
        elif direction==2:
            return pos_x+1,pos_y-1
        elif direction==3:
            return pos_x-1,pos_y
        elif direction==4:
            return pos_x+1,pos_y
        elif direction==5:
            return pos_x,pos_y+1
        elif direction==6:
            return pos_x+1,pos_y+1
    else:
        if direction==1:
            return pos_x-1,pos_y-1
        elif direction==2:
            return pos_x,pos_y-1
        elif direction==3:
            return pos_x-1,pos_y
        elif direction==4:
            return pos_x+1,pos_y
        elif direction==5:
            return pos_x-1,pos_y+1
        elif direction==6:
            return pos_x,pos_y+1


def checkRemainMove(mapStat):
    free_region = (mapStat == 0)
    temp = []
    for i in range(len(free_region)):
        for j in range(len(free_region[0])):
            if(free_region[i][j] == True):
                temp.append([i,j])
    return temp


'''
    輪到此程式移動棋子
    mapStat : 棋盤狀態(list of list), 為 12*12矩陣, 0=可移動區域, -1=障礙, 1~2為玩家1~2佔領區域
    gameStat : 棋盤歷史順序
    return Step
    Step : 3 elements, [(x,y), l, dir]
            x, y 表示要畫線起始座標
            l = 線條長度(1~3)
            dir = 方向(1~6),對應方向如下圖所示
              1  2
            3  x  4
              5  6
'''

def find_max_length(x, y, direction, free_regions, lengths):
    if lengths[x][y][direction] != 0:
        return lengths[x][y][direction]
    next = Next_Node(x, y, direction)
    if list(next) not in free_regions:
        lengths[x][y][direction] = 1
        return 1
    lengths[x][y][direction] = find_max_length(next[0], next[1], direction, free_regions, lengths)+1
    return lengths[x][y][direction]

# player: 1 for the player, 2 for the opponent
# depth: the max traversed depth of the tree
# mapStat: the current map
# map_scores: a dictionary holding score of all the processed maps
# step: the variable used to save the decided step back to the caller

def get_map_score(player, depth, mapStat, map_scores, step, counter):
    lengths = [[[0 for k in range(7)] for j in range(12)] for i in range(12)]
    free_regions = checkRemainMove(mapStat)
    dict_key = tuple(map(tuple, free_regions))

    # if the same map has been processed already
    if len(dict_key)<=15 and dict_key in map_scores:
        # winable
        if map_scores[dict_key]!=0:
            step[0] = map_scores[dict_key][1][0]
            step[1] = map_scores[dict_key][1][1]
            step[2] = map_scores[dict_key][1][2]
            return 1
        # unwinable
        else:
            return 0
        
    # if it is the last grid, player loses
    if len(free_regions)==1:
        return 0
        
    # if there's no grid, player wins
    if len(free_regions)==0:
        return 1

    # traverse to the maximum depth and still have no answer
    if depth == 0:
        return 0.5
    
    next_player = 0
    if player==1:
        next_player=2
    else:
        next_player=1

    # record whether there's a path that is deeper than the max depth
    tent = False

    # find each possible moves in the current map
    for grid in free_regions:
        for direction in range(1, 7):
            max_length = find_max_length(grid[0], grid[1], direction, free_regions, lengths)
            new_map = mapStat.copy()
            # the move the player is going to make
            move_x = grid[0]
            move_y = grid[1]
            for steps in range(1, 1+(3 if max_length>3 else max_length)):
                new_map[move_x][move_y] = player
                if player==1:
                    score = get_map_score(next_player, depth-1, new_map, map_scores, step, counter+1)
                    # find a way that the next player (opponent) will lose => player win
                    if score==0:
                        step[0] = (grid[0], grid[1])
                        step[1] = steps
                        step[2] = direction
                        if len(dict_key)<=15:
                            map_scores[dict_key] = (1, ((grid[0], grid[1]), steps, direction))
                        return 1
                    # find a way that don't know whether it wins or loses (reach max depth) => save tentatively
                    elif score==0.5:
                        if counter%101==math.floor(rd.random()*101)%101:
                            step[0] = (grid[0], grid[1])
                            step[1] = steps
                            step[2] = direction
                        tent = True
                else:
                    score = get_map_score(next_player, depth-1, new_map, map_scores, step, counter+1)
                    # find a way that the next player (player) will lose => opponent win
                    if score==0:
                        if len(dict_key)<=15:
                            map_scores[dict_key] = (1, ((grid[0], grid[1]), steps, direction))
                        return 1
                    # find a way that don't know whether it wins or loses (reach max depth) => save tentatively
                    elif score==0.5:
                        if counter%101==math.floor(rd.random()*101)%101:
                            step[0] = (grid[0], grid[1])
                            step[1] = steps
                            step[2] = direction
                        tent = True
                (move_x, move_y) = Next_Node(move_x, move_y, direction)
    
    # there's a path that cannot be assured that will win or lose
    if tent:
        return 0.5
    
    # cannot find a way that can win => lose if the opponent makes the best decision every time
    map_scores[dict_key] = 0
    return 0

def Getstep(mapStat, gameStat):
    # record scores off all the map outcomes and the corresponding step
    map_scores = dict()
    valid_move = checkRemainMove(mapStat)

    # give it a valid move to prevent illegal move
    x = valid_move[0][0]
    y = valid_move[0][1]
    l = 1
    d = 1
    step = [(x, y), l, d]
    # depth limit
    depth = 0
    if len(valid_move)<=10:
        depth = 10
    elif len(valid_move)<=15:
        depth = 2
    else:
        depth = 1
    get_map_score(1, depth, mapStat, map_scores, step, 0)
    
    return step
    



# start game
print('start game')
# if the map record exists, load it from the file
if os.path.isfile('record.pkl'):
    with open('record.pkl', 'rb') as f:
        map_scores = pickle.load(f)
while (True):

    (end_program, id_package, mapStat, gameStat) = STcpClient.GetBoard()
    if end_program:
        STcpClient._StopConnect()
        break
    
    decision_step = Getstep(mapStat, map_scores)
    
    STcpClient.SendStep(id_package, decision_step)
